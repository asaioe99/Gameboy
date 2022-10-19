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
  status_LCD &= 0b11111100; // mode0 デフォ
  if (*t_FF44 >= 144) {         // mode1
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
  uint8_t *t_FF40 = io + 0x40;
  uint8_t SCY = *(io + 0x42); // SCY BGの描画位置
  uint8_t SCX = *(io + 0x43); // SCX
  uint8_t LY  = *(io + 0x44);

  uint8_t pic_h;
  uint8_t pic_l;

  static uint16_t bg_offset;
  static uint16_t window_offset;

  if (*t_FF40 & 0b00100000 == 0b00100000) { // ウィンドウが表示有効か確認
    if (*t_FF40 & 0b01000000 == 0b01000000) { // ウィンドウタイルマップのアドレス指定
      window_offset = 0x9C00;
    } else {
      window_offset = 0x9800;
    }
  }
  if (*t_FF40 & 0b00000001 == 0b00000001) { // 背景が表示有効か確認
    if (*t_FF40 & 0b00001000 == 0b00001000) { // 背景タイルマップのアドレス指定
      bg_offset = 0x9C00;
    } else {
      bg_offset = 0x9800;
    }
  }

  for (uint8_t n = 0; n < 20; n++) {
    uint16_t bg_tile_num = SCX + SCY * 0x20 + (LY >> 3) + n;

    if (*t_FF40 & 0b00010000 == 0b00010000) {
      pic_l = get_byte(0x8000 + get_byte(bg_offset + bg_tile_num) * 16 + (LY & 0b00000111) * 2);
      pic_h = get_byte(0x8001 + get_byte(bg_offset + bg_tile_num) * 16 + (LY & 0b00000111) * 2);
    } else {
      pic_l = get_byte(0x9000 + (int8_t)get_byte(bg_offset + bg_tile_num) * 16 + (LY & 0b00000111) * 2);
      pic_h = get_byte(0x9001 + (int8_t)get_byte(bg_offset + bg_tile_num) * 16 + (LY & 0b00000111) * 2);
    }
    //Serial.println(pic_l, HEX);
    //pic_l = 0xFF;
    for (uint8_t i = 0; i < 4; i++) {
      *(FIFO_bg_wnd + 0 + 3 * i) = 0b00000000;
      *(FIFO_bg_wnd + 1 + 3 * i) = 0b00000000;
      *(FIFO_bg_wnd + 2 + 3 * i) = 0b00000000;

      *(FIFO_bg_wnd + 0 + 3 * i) |= (pic_h & (1 << (7 - i * 2))) >> 0;
      *(FIFO_bg_wnd + 0 + 3 * i) |= (pic_h & (1 << (7 - i * 2))) >> 1;
      *(FIFO_bg_wnd + 0 + 3 * i) |= (pic_h & (1 << (7 - i * 2))) >> 4;
      *(FIFO_bg_wnd + 0 + 3 * i) |= (pic_h & (1 << (7 - i * 2))) >> 5;
      *(FIFO_bg_wnd + 1 + 3 * i) |= (pic_h & (1 << (7 - i * 2))) >> 0;
      *(FIFO_bg_wnd + 1 + 3 * i) |= (pic_h & (1 << (7 - i * 2))) >> 1;
      *(FIFO_bg_wnd + 1 + 3 * i) |= (pic_h & (1 << (6 - i * 2))) >> 3;
      *(FIFO_bg_wnd + 1 + 3 * i) |= (pic_h & (1 << (6 - i * 2))) >> 4;
      *(FIFO_bg_wnd + 2 + 3 * i) |= (pic_h & (1 << (6 - i * 2))) << 1;
      *(FIFO_bg_wnd + 2 + 3 * i) |= (pic_h & (1 << (6 - i * 2))) >> 0;
      *(FIFO_bg_wnd + 2 + 3 * i) |= (pic_h & (1 << (6 - i * 2))) >> 3;
      *(FIFO_bg_wnd + 2 + 3 * i) |= (pic_h & (1 << (6 - i * 2))) >> 4;

      *(FIFO_bg_wnd + 0 + 3 * i) |= (pic_l & (1 << (7 - i * 2))) >> 2;
      *(FIFO_bg_wnd + 0 + 3 * i) |= (pic_l & (1 << (7 - i * 2))) >> 3;
      *(FIFO_bg_wnd + 0 + 3 * i) |= (pic_l & (1 << (7 - i * 2))) >> 6;
      *(FIFO_bg_wnd + 0 + 3 * i) |= (pic_l & (1 << (7 - i * 2))) >> 7;
      *(FIFO_bg_wnd + 1 + 3 * i) |= (pic_l & (1 << (7 - i * 2))) >> 2;
      *(FIFO_bg_wnd + 1 + 3 * i) |= (pic_l & (1 << (7 - i * 2))) >> 3;
      *(FIFO_bg_wnd + 1 + 3 * i) |= (pic_l & (1 << (6 - i * 2))) >> 6;
      *(FIFO_bg_wnd + 1 + 3 * i) |= (pic_l & (1 << (6 - i * 2))) >> 7;
      *(FIFO_bg_wnd + 2 + 3 * i) |= (pic_l & (1 << (6 - i * 2))) >> 1;
      *(FIFO_bg_wnd + 2 + 3 * i) |= (pic_l & (1 << (6 - i * 2))) >> 2;
      *(FIFO_bg_wnd + 2 + 3 * i) |= (pic_l & (1 << (6 - i * 2))) >> 5;
      *(FIFO_bg_wnd + 2 + 3 * i) |= (pic_l & (1 << (6 - i * 2))) >> 6;
    }
    //Serial.println(FIFO_bg_wnd[0], HEX);
    drowBitMap(n * 8, LY);
  }
  return;
}

void draw_scanline() {
  if (!pic_flag) return;
  uint8_t *t_FF41 = io + 0x41;

  switch (*t_FF41 & 0b00000011) {
    case 0b00: // mode 0
      //Serial.println("mode 0");
      break;
    case 0b01: // mode 1
      //Serial.println("mode 1");
      break;
    case 0b10: // mode 2
      //Serial.println("mode 2");
      break;
    case 0b11: // mode 3
      //Serial.println("mode 3");
      mode_3();
      pic_flag = 0;
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
    pic_flag = 1;
  }
  // 非ブランクゾーンの場合の処理
  if (*t_FF44 < 144) {
    //Serial.println("draw start!");
    draw_scanline();
  } else if (*t_FF44 > 153) {
    //Serial.println("blan zone!");
    *t_FF44 = 0;
  } else if (*t_FF44 = 144) {
    //Serial.println("144 draw start!");
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
  delay(500);                  // Wait 500 ms, more then 120 ms
  // --- SOFT Ware Reset
  tftSendCommand(0x01);        // SOFTWARE RESET
  delay(500);

  // --- Initial Comands
  tftSendCommand(0x28);            // Display OFF
  delay(500);
  tftSendCommand(0x11);            // Sleep Out
  delay(500);
  tftSendCommand1(0x3A, 0x05);     // 12Bit Pixel Mode

  delay(10);
  tftSendCommand1(0x36, 0b00000000);
  bool r = false; // MX MY MV ML RGB MH x x:縦向き１
  tftSendCommand2(0xB6, 0x15, 0x02); // Display settings #5
  tftSendCommand(0x13);            // NomalDisplayMode
  tftSendCommand(0x21);            // Display Inversion Off
  cls(r);
  delay(500);
  tftSendCommand(0x29);            // Display ON
  delay(500);
  dispStartLine(0);
  fill(0, 0, 240, 240, B11111000, B00000000);
  for (int i = 0; i < 16; i++) {
    //drowBitMap(15, 15 + i * 4);
  }
  SPI.endTransaction();
  digitalWrite(TFT_CS, HIGH);
  return;
}
// 表示開始ライン設定
void dispStartLine(uint16_t y) {
  uint8_t yH = (y >> 8) & 0xFF ;
  uint8_t yL = y & 0xFF ;
  digitalWrite(TFT_CS, LOW);
  digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(0x37);
  digitalWrite(TFT_DC, HIGH); // Command mode
  SPI.transfer(yH);
  SPI.transfer(yL);
  digitalWrite(TFT_CS, HIGH);
}

// BITマップ表示
void drowBitMap(uint8_t offsetX , uint8_t offsetY) {
  uint8_t endX = offsetX + 7;
  uint8_t endY = offsetY;

  uint8_t xsL = offsetX;
  uint8_t xeL = endX;
  uint8_t ysL = offsetY;
  uint8_t yeL = endY;

  SPI.beginTransaction(lcd_SPISettings);

  tftSendCommand4(0x2A, 0, xsL, 0, xeL) ; // Colmun Address
  tftSendCommand4(0x2B, 0, ysL, 0, yeL) ; // Row Address

  uint8_t test[] = {0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77,
                    0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77,
                    0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77,
                    0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77,
                   };

  digitalWrite(TFT_CS, LOW);
  digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(0x2C);
  digitalWrite(TFT_DC, HIGH); // Data mode
  SPI.transfer(FIFO_bg_wnd, 12);
  //SPI.transfer(test, 64);
  digitalWrite(TFT_CS, HIGH);
  SPI.endTransaction();
}

// 画面クリア
void cls(bool rot) {
  if (rot) {
    tftSendCommand4(0x2A, 0, 0, 0, 239) ; // Colmun Address
    tftSendCommand4(0x2B, 0, 0, 0x01, 0x3F) ; // Row Address
  } else {
    tftSendCommand4(0x2A, 0, 0, 0x01, 0x3F) ; // Colmun Address
    tftSendCommand4(0x2B, 0, 0, 0, 239) ; // Row Address
  }
  digitalWrite(TFT_CS, LOW);
  digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(0x2C);
  digitalWrite(TFT_DC, HIGH); // Data mode
  for (int i = 0; i < 240; i++) {
    memset(SPIBuf, 0, 320 * 2) ;
    SPI.transfer(SPIBuf, 320 * 2);
  }
  digitalWrite(TFT_DC, LOW); // Command mode
  digitalWrite(TFT_CS, HIGH);
}

// TFTにコマンドを送信
void tftSendCommand(uint8_t command) {
  digitalWrite(TFT_CS, LOW);
  digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(command);
  digitalWrite(TFT_CS, HIGH);
}

// TFTにコマンド+1バイトデータを送信
void tftSendCommand1(uint8_t command, uint8_t data1) {
  digitalWrite(TFT_CS, LOW);
  digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(command);
  digitalWrite(TFT_DC, HIGH); // Command mode
  SPI.transfer(data1);
  digitalWrite(TFT_CS, HIGH);
}

// TFTにコマンド+2バイトデータを送信
void tftSendCommand2(uint8_t command, uint8_t data1, uint8_t data2) {
  digitalWrite(TFT_CS, LOW);
  digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(command);
  digitalWrite(TFT_DC, HIGH); // Command mode
  SPI.transfer(data1);
  SPI.transfer(data2);
  digitalWrite(TFT_CS, HIGH);
}

// TFTにコマンド+3バイトデータを送信 いらないかも
void tftSendCommand3(uint8_t command, uint8_t data1, uint8_t data2, uint8_t data3) {
  digitalWrite(TFT_CS, LOW);
  digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(command);
  digitalWrite(TFT_DC, HIGH); // Command mode
  SPI.transfer(data1);
  SPI.transfer(data2);
  SPI.transfer(data3);
  digitalWrite(TFT_CS, HIGH);
}

// TFTにコマンド+4バイトデータを送信
void tftSendCommand4(uint8_t command, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4) {
  digitalWrite(TFT_CS, LOW);
  digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(command);
  digitalWrite(TFT_DC, HIGH); // Command mode
  SPI.transfer(data1);
  SPI.transfer(data2);
  SPI.transfer(data3);
  SPI.transfer(data4);
  digitalWrite(TFT_CS, HIGH);
}
// 塗りつぶし
void fill(int xs, int ys , int xe, int ye , uint8_t c1 , uint8_t c2) {
  uint8_t xsH = HBYTE(xs) ;
  uint8_t xsL = LBYTE(xs) ;
  uint8_t ysH = HBYTE(ys) ;
  uint8_t ysL = LBYTE(ys) ;
  uint8_t xeH = HBYTE(xe) ;
  uint8_t xeL = LBYTE(xe) ;
  uint8_t yeH = HBYTE(ye) ;
  uint8_t yeL = LBYTE(ye) ;

  tftSendCommand4(0x2A, xsH, xsL, xeH, xeL) ; // Colmun Address
  tftSendCommand4(0x2B, ysH, ysL, yeH, yeL) ; // Row Address
  digitalWrite(TFT_CS, LOW);
  digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(0x2C);
  digitalWrite(TFT_DC, HIGH); // Data mode
  int w = ye - ys + 1 ;
  for (int i = 0; i < xe - xs + 1; i++) {
    for (int col = 0 ; col < w * 2 ; col += 2 ) {
      SPIBuf[col] = c1 ;
      SPIBuf[col + 1] = c2 ;
    }
    SPI.transfer(SPIBuf, w * 2);
  }
  digitalWrite(TFT_DC, LOW); // Command mode
  delay(5) ;
  digitalWrite(TFT_CS, HIGH);
}
