void p_flag_update() {

  uint8_t *t_FF40 = io + 0x40;
  uint8_t *t_FF41 = io + 0x41;
  uint8_t *t_FF44 = io + 0x44;
  uint8_t status_LCD = *t_FF41;

  if ((*t_FF40 & 0x80) != 0x80) {
    *t_FF40 = 0x00;
    scaline_counter = 456;
    status_LCD &= 0b11111100;
    status_LCD = ~(status_LCD & (1 << 0));
    status_LCD = ~(status_LCD & (1 << 1));
    *t_FF41 = status_LCD;
    return;
  }
  //現時点のmodeが何であるか取得し更新
  status_LCD &= 0b11111100;              // mode0 デフォ
  if (*t_FF44 >= 144) {                  // mode1
    status_LCD |= 0b00000001;
  } else {
    if (scaline_counter >= 376) {        // mode2
      status_LCD |= 0b00000010;

    } else if (scaline_counter >= 204) { // mode3
      status_LCD |= 0b00000011;
    }
  }

  // put_byte(0xFF41, status_LCD);
  *t_FF41 = status_LCD;

  if (status_LCD == *(io + 0x45)) {
    status_LCD = status_LCD | 0b00000100;
  } else {
    status_LCD = status_LCD & 0b11111011;
  }
  *t_FF41 = status_LCD;
}

void mode_2() {
  return;
}

// bg&wnd フェッチ
void mode_3() {
  return;
}

void draw_scanline() {
  uint8_t *t_FF41 = io + 0x41;

  switch (*t_FF41 & 0b00000011) {
    case 0b00: // mode 0
      break;
    case 0b01: // mode 1
      break;
    case 0b10: // mode 2
      break;
    case 0b11: // mode 3
      mode_3();
      break;
  }
  return;
}

void ppu() {
  // flag等 アップデート
  p_flag_update();
  // LCD コントロールレジスタのMSBによりLCD有効でない場合は動作をスルー
  if (*(io + 0x40) & 0b10000000) {
    scaline_counter -= cc_dec;
  } else {
    return;
  }
  uint8_t *t_FF44 = io + 0x44;
  //　スキャンラインの右端に達したらlyをインクリメントしてスキャンラインナンバーを下に進める
  if (scaline_counter <= 0) {
    *t_FF44 = *t_FF44 + 1;
    scaline_counter += 456;
  } else {
    return;
  }
  // 非ブランクゾーンの場合の処理
  if (*t_FF44 < 144) {
    draw_scanline();
  } else if (*t_FF44 > 153) {
    *t_FF44 = 0;
  } else if (*t_FF44 = 144) {
    draw_scanline();
    *(io + 0x0F) = *(io + 0x0F) | 0b00010000; // v-blank割り込み発生
  }
}

void ini_LCD() {
  SPI.begin();
  pinMode(TFT_DC,   OUTPUT);
  pinMode(TFT_RST,  OUTPUT);
  pinMode(TFT_CS,   OUTPUT);
  SPI.beginTransaction(lcd_SPISettings);
  digitalWrite(TFT_CS, LOW);

  // --- HARD Ware Reset
  digitalWrite(TFT_RST, HIGH);
  delay(500);                  // VDD goes high at start, pause for 500 ms
  digitalWrite(TFT_RST, LOW);  // Bring reset low
  delay(100);                  // Wait 100 ms
  digitalWrite(TFT_RST, HIGH); // Bring out of reset
  delay(120);                  // Wait 500 ms, more then 120 ms
  // --- SOFT Ware Reset
  tftSendCommand(0x01);        // SOFTWARE RESET
  delay(500);

  // --- Initial Comands
  tftSendCommand(0x28);            // Display OFF
  delay(500);
  tftSendCommand(0x11);            // Sleep Out
  delay(500);
  tftSendCommand1(0x3A, 0x03);     // 12Bit Pixel Mode
  delay(100);
  tftSendCommand1(0x26, 0x08);     // Gamma value
  delay(100);
  //tftSendCommand3(0xB3, 0b00010011, 0b00000000, 0b00000000);     // Frame rate
  //delay(100);
  
  tftSendCommand1(0x36, 0b00000000);
  //bool r = false; // MX MY MV ML RGB MH x x:縦向き１
  tftSendCommand2(0xB6, 0x15, 0x02); // Display settings #5
  delay(100);
  tftSendCommand(0x13);            // NomalDisplayMode
  delay(100);
  tftSendCommand(0x21);            // Display Inversion Off
  delay(100);
  cls();
  delay(500);
  tftSendCommand(0x29);            // Display ON
  delay(500);
  //dispStartLine(0);
  SPI.endTransaction();
  digitalWrite(TFT_CS, HIGH);

  return;
}
// 表示開始ライン設定
void dispStartLine(uint16_t y) {
  uint8_t yH = (y >> 8) & 0xFF ;
  uint8_t yL = y & 0xFF ;
  PORTB &= 0b11011111;//digitalWrite(TFT_CS, LOW);
  PORTG &= 0b11111110;//digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(0x37);
  PORTG |= 0b00000001;//digitalWrite(TFT_DC, HIGH); // Command mode
  SPI.transfer(yH);
  SPI.transfer(yL);
  PORTB |= 0b00100000;//digitalWrite(TFT_CS, HIGH);
}

// BITマップ表示
void drowBitMap(uint8_t y) {

  SPI.beginTransaction(lcd_SPISettings);

  tftSendCommand4(0x2A, 0, 0, 0, 159) ; // Colmun Address
  tftSendCommand4(0x2B, 0, y, 0, y) ; // Row Address

  PORTB &= 0b11011111;//digitalWrite(TFT_CS, LOW);
  PORTG &= 0b11111110;//digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(0x2C);
  PORTG |= 0b00000001;//digitalWrite(TFT_DC, HIGH); // Data mode
  SPI.transfer(FIFO_bg_wnd, 240);
  PORTB |= 0b00100000;//digitalWrite(TFT_CS, HIGH);

  SPI.endTransaction();
}

// 画面クリア
void cls() {
  SPI.beginTransaction(lcd_SPISettings);

  tftSendCommand4(0x2A, 0, 0, 0, 239) ; // Colmun Address
  tftSendCommand4(0x2B, 0, 0, 0, 239) ; // Row Address

  PORTB &= 0b11011111;//digitalWrite(TFT_CS, LOW);
  PORTG &= 0b11111110;//digitalWrite(TFT_DC, LOW); // Command mode
  
  SPI.transfer(0x2C);
  
  PORTG |= 0b00000001;//digitalWrite(TFT_DC, HIGH); // Data mode
  memset(SPIBuf, 0b00000000, 360);//0b00000000
  for (int i = 0; i < 240; i++) {
    SPI.transfer(SPIBuf, 360);
  }
  PORTG &= 0b11111110;//digitalWrite(TFT_DC, LOW); // Command mode
  PORTB |= 0b00100000;//digitalWrite(TFT_CS, HIGH);

  SPI.endTransaction();
}

// TFTにコマンドを送信
void tftSendCommand(uint8_t command) {
  PORTB &= 0b11011111;//digitalWrite(TFT_CS, LOW);
  PORTG &= 0b11111110;//digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(command);
  PORTB |= 0b00100000;//digitalWrite(TFT_CS, HIGH);
}

// TFTにコマンド+1バイトデータを送信
void tftSendCommand1(uint8_t command, uint8_t data1) {
  PORTB &= 0b11011111;//digitalWrite(TFT_CS, LOW);
  PORTG &= 0b11111110;//digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(command);
  PORTG |= 0b00000001;//digitalWrite(TFT_DC, HIGH); // Command mode
  SPI.transfer(data1);
  PORTB |= 0b00100000;//digitalWrite(TFT_CS, HIGH);
}

// TFTにコマンド+2バイトデータを送信
void tftSendCommand2(uint8_t command, uint8_t data1, uint8_t data2) {
  PORTB &= 0b11011111;//digitalWrite(TFT_CS, LOW);
  PORTG &= 0b11111110;//digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(command);
  PORTG |= 0b00000001;//digitalWrite(TFT_DC, HIGH); // Command mode
  SPI.transfer(data1);
  SPI.transfer(data2);
  PORTB |= 0b00100000;//digitalWrite(TFT_CS, HIGH);
}
// TFTにコマンド+3バイトデータを送信
void tftSendCommand3(uint8_t command, uint8_t data1, uint8_t data2, uint8_t data3) {
  PORTB &= 0b11011111;//digitalWrite(TFT_CS, LOW);
  PORTG &= 0b11111110;//digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(command);
  PORTG |= 0b00000001;//digitalWrite(TFT_DC, HIGH); // Command mode
  SPI.transfer(data1);
  SPI.transfer(data2);
  SPI.transfer(data3);
  PORTB |= 0b00100000;//digitalWrite(TFT_CS, HIGH);
}
// TFTにコマンド+4バイトデータを送信
void tftSendCommand4(uint8_t command, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4) {
  PORTB &= 0b11011111;//digitalWrite(TFT_CS, LOW);
  PORTG &= 0b11111110;//digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(command);
  PORTG |= 0b00000001;//digitalWrite(TFT_DC, HIGH); // Command mode
  SPI.transfer(data1);
  SPI.transfer(data2);
  SPI.transfer(data3);
  SPI.transfer(data4);
  PORTB |= 0b00100000;//digitalWrite(TFT_CS, HIGH);
}
