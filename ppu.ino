void p_flag_update() {
  uint8_t status_LCD = *(io + 0x41);
  uint8_t *t_FF40 = io + 0x40;
  uint8_t *t_FF41 = io + 0x41;
  uint8_t *t_FF44 = io + 0x44;
  
  if ((*t_FF40 & 0x80) != 0x80) {
    *t_FF40 = 0x00;
    scaline_counter = 456;
    status_LCD &= 0b11111100;
    status_LCD = ~(status_LCD & (1 << 0));
    status_LCD = ~(status_LCD & (1 << 1));
    *t_FF41 = status_LCD;
    return;
  }

  status_LCD = *t_FF41;

  if (*t_FF44 >= 144) {         // mode1
    status_LCD = status_LCD & 0b11111101;
  } else {
    if (scaline_counter >= 376) {        // mode2
      status_LCD = status_LCD & 0b11111110;
    } else if (scaline_counter >= 204) { // mode3
      status_LCD = status_LCD & 0b11111111;
    } else {                             // mode0
      status_LCD = status_LCD & 0b11111100;
    }
  }
  
  // put_byte(0xFF41, status_LCD);
  *t_FF41 = status_LCD;
  
  status_LCD = *t_FF41;
  if (*t_FF44 == *(io + 0x45)) {
    status_LCD = status_LCD | 0b00000100;
  } else {
    status_LCD = status_LCD & 0b11111011;
  }

  *t_FF41 = status_LCD;
}

void draw_scanline() {
  return;
}

void ppu() {
  // flag アップデート
  p_flag_update();

  if (*(io + 0x40) & 0b10000000) {
    scaline_counter -= cc_dec;
  } else {
    return;
  }
  
  if (scaline_counter <= 0) {
    uint8_t *t_FF44 = io + 0x44;
    *t_FF44 = *t_FF44 + 1;
    scaline_counter += 456;

    if (*t_FF44 < 144) {
      draw_scanline();
      *(io + 0x0F) = *(io + 0x0F) | 0b00000001;
    } else if (*t_FF44 > 153) {
      *t_FF44 = 0;
    } else if (*t_FF44 = 144) {
      draw_scanline();
    }
  }
}

void ini_LCD() {
  // -----  ST3389 INITIAL -----
  pinMode(TFT_DC,   OUTPUT);
  pinMode(TFT_RST,  OUTPUT);
  pinMode(TFT_CS,   OUTPUT);
  SPI.beginTransaction(spi_SPISettings);
  digitalWrite(TFT_CS, LOW);

  // --- HARD Ware Reset
  digitalWrite(TFT_RST, HIGH);
  delay(500);                  // VDD goes high at start, pause for 500 ms
  digitalWrite(TFT_RST, LOW);  // Bring reset low
  delay(100);                  // Wait 100 ms
  digitalWrite(TFT_RST, HIGH); // Bring out of reset
  delay(500);                  // Wait 500 ms, more then 120 ms
  // --- SOFT Ware Reset
  tftSendCommand(0x01) ;       // SOFTWARE RESET
  delay(50);

  // --- Initial Comands
  bool r = false ;
  tftSendCommand(0x28) ;            // Display OFF : 0x29 - Display ON
  delay(500);
  tftSendCommand(0x11) ;            // Sleep Out
  delay(500);
  tftSendCommand1(0x3A, 0x05) ;     // 16Bit Pixel Mode
  // // delay(1);
  tftSendCommand1(0x36, B00000000) ;
  r = false; // MX MY MV ML RGB MH x x:縦向き１
  tftSendCommand2(0xB6, 0x15, 0x02) ; // Display settings #5

  tftSendCommand(0x13) ;            // NomalDisplayMode
  tftSendCommand(0x21) ;            // Display Inversion Off

  cls(r) ;
  delay(500);
  tftSendCommand(0x29) ;            // Display ON : 0x28 - Display OFF
  delay(500);
  dispStartLine(0);
  SPI.endTransaction(spi_SPISettings);
  digitalWrite(TFT_CS, HIGH);
}
// 表示開始ライン設定
void dispStartLine(uint16_t y) {
  uint8_t yH = (y >> 8) & 0xFF ;
  uint8_t yL = y & 0xFF ;
  digitalWrite(TFT_CS, LOW);
  // // delay(1);
  digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(0x37);
  digitalWrite(TFT_DC, HIGH); // Command mode
  SPI.transfer(yH);
  SPI.transfer(yL);
  digitalWrite(TFT_CS, HIGH);
}
// BITマップ表示
void drowBitMap(int offsetX , int offsetY , int width, int hight, uint8_t bitMap[]) {
  uint8_t endX = offsetX + width - 1;
  uint8_t endY = offsetY + hight - 1;

  uint8_t xsH = HBYTE(offsetX) ;
  uint8_t xsL = LBYTE(offsetX) ;
  uint8_t xeH = HBYTE(endX) ;
  uint8_t xeL = LBYTE(endX) ;
  uint8_t ysH = HBYTE(offsetY) ;
  uint8_t ysL = LBYTE(offsetY) ;
  uint8_t yeH = HBYTE(endY) ;
  uint8_t yeL = LBYTE(endY) ;

  tftSendCommand4(0x2A, xsH, xsL, xeH, xeL) ; // Colmun Address
  tftSendCommand4(0x2B, ysH, ysL, yeH, yeL) ; // Row Address

  int pos = 0 ;
  digitalWrite(TFT_CS, LOW);
  // // delay(1);
  digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(0x2C);
  digitalWrite(TFT_DC, HIGH); // Data mode
  for (int row = 0 ; row < hight ; row ++) {
    // 表示データ転送
    // Arduinoだとメモリが足りなくて１画面分を一気に転送する事ができない
    // なので、1Lineずつ送信している
    for (int col = 0 ; col < width * 2 ; col ++ ) {
      SPIBuf[col] = pgm_read_byte(bitMap + pos);
      pos ++ ;
    }
    SPI.transfer(SPIBuf, width * 2);
  }
  digitalWrite(TFT_DC, LOW); // Command mode
  digitalWrite(TFT_CS, HIGH);
  // // delay(1);
}
