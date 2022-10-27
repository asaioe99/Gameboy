void ini_LCD() {
  // pin assign
  pinMode(TFT_MOSI, OUTPUT);
  pinMode(TFT_CLK, OUTPUT);
  pinMode(TFT_CS,  OUTPUT);
  pinMode(TFT_DC,  OUTPUT);
  pinMode(TFT_BL,  OUTPUT);
  pinMode(TFT_RST, OUTPUT);

  // BACK LIGHT ON
  gpio_put(TFT_BL, HIGH);

  // --- HARD Ware Reset
  gpio_put(TFT_RST, HIGH);
  delay(120);                  // VDD goes high at start, pause for 500 ms
  gpio_put(TFT_RST, LOW);  // Bring reset low
  delay(120);                  // Wait 100 ms
  gpio_put(TFT_RST, HIGH); // Bring out of reset
  delay(120);                  // Wait 500 ms, more then 120 ms
  // --- SOFT Ware Reset
  tftSendCommand(0x01) ;       // SOFTWARE RESET
  delay(50);

  // --- Initial Comands
  bool r = false ;
  tftSendCommand(0x28) ;            // Display OFF : 0x29 - Display ON
  delay(500);

  tftSendCommand(0x11) ;            // Sleep Out
  delay(500);
  tftSendCommand1(0x3A, 0x05) ;       // 16Bit Pixel Mode
  delay(10);
  tftSendCommand1(0x36, B01100000) ; r = false; // MX MY MV ML RGB MH x x：横向き２
  tftSendCommand2(0xB6, 0x15, 0x02) ; // Display settings #5
  tftSendCommand1(0xB4, 0x00) ;     // Display inversion control

  tftSendCommand(0x13) ;            // NomalDisplayMode
  tftSendCommand(0x20) ;            // Display Inversion Off

  cls(r) ;
  delay(500);
  tftSendCommand(0x29) ;            // Display ON : 0x28 - Display OFF
  delay(500);
}

// ===== SPI WRITE =====
// ソフトウエアＳＰＩ通信:MODE0 MSBFARST（送信のみ）
void SPI_transfer(uint8_t data) {
  for (uint8_t bit = 0x80; bit; bit >>= 1) {
    gpio_put(TFT_MOSI, data & bit);
    gpio_put(TFT_CLK, HIGH);
    gpio_put(TFT_CLK, LOW);
  }
}

void SPI_transfer(uint8_t* data, int dataLen) {
  for (int d = 0; d < dataLen; d++) {
    SPI_transfer(*(data + d));
  }
}

// TFTにコマンドを送信
void tftSendCommand(uint8_t command) {
  gpio_put(TFT_CS, LOW); //
  gpio_put(TFT_DC, LOW); // Command mode
  SPI_transfer(command);
  gpio_put(TFT_CS, HIGH);  //
}

// TFTにコマンド+1バイトデータを送信
void tftSendCommand1(uint8_t command, uint8_t data1) {
  gpio_put(TFT_CS, LOW); //
  gpio_put(TFT_DC, LOW); // Command mode
  SPI_transfer(command);
  gpio_put(TFT_DC, HIGH); // Command mode
  SPI_transfer(data1);
  gpio_put(TFT_CS, HIGH);  //
}

// TFTにコマンド+2バイトデータを送信
void tftSendCommand2(uint8_t command, uint8_t data1, uint8_t data2) {
  gpio_put(TFT_CS, LOW); //
  gpio_put(TFT_DC, LOW); // Command mode
  SPI_transfer(command);
  gpio_put(TFT_DC, HIGH); // Command mode
  SPI_transfer(data1);
  SPI_transfer(data2);
  gpio_put(TFT_CS, HIGH);  //
}

// TFTにコマンド+3バイトデータを送信
void tftSendCommand3(uint8_t command, uint8_t data1, uint8_t data2, uint8_t data3) {
  gpio_put(TFT_CS, LOW); //
  gpio_put(TFT_DC, LOW); // Command mode
  SPI_transfer(command);
  gpio_put(TFT_DC, HIGH); // Command mode
  SPI_transfer(data1);
  SPI_transfer(data2);
  SPI_transfer(data3);
  gpio_put(TFT_CS, HIGH);  //
}

// TFTにコマンド+4バイトデータを送信
void tftSendCommand4(uint8_t command, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4) {
  gpio_put(TFT_CS, LOW); //
  gpio_put(TFT_DC, LOW); // Command mode
  SPI_transfer(command);
  gpio_put(TFT_DC, HIGH); // Command mode
  SPI_transfer(data1);
  SPI_transfer(data2);
  SPI_transfer(data3);
  SPI_transfer(data4);
  gpio_put(TFT_CS, HIGH);  //
}

// 画面クリア
void cls(bool rot) {

  tftSendCommand4(0x2A, 0, 0, 0, 239) ; // Colmun Address
  tftSendCommand4(0x2B, 0, 0, 0, 239) ; // Row Address

  gpio_put(TFT_CS, LOW);
  gpio_put(TFT_DC, LOW); // Command mode
  SPI_transfer(0x2C);
  gpio_put(TFT_DC, HIGH); // Data mode
  for (int i = 0; i < 240; i++) {
    for (int j = 0; j < 240; j++) {
      SPI_transfer(0b11111111);
      SPI_transfer(0b11111111);
    }
  }
  gpio_put(TFT_DC, LOW); // Command mode
  gpio_put(TFT_CS, HIGH);  //
}
// BITマップ表示
void drowBitMap(uint8_t y) {

  tftSendCommand4(0x2A, 0, 40, 0, 199) ; // Colmun Address
  tftSendCommand4(0x2B, 0, y + 40, 0, y + 40) ; // Row Address

  gpio_put(TFT_CS, LOW);
  gpio_put(TFT_DC, LOW); // Command mode
  SPI_transfer(0x2C);
  gpio_put(TFT_DC, HIGH); // Data mode
  for (int d = 0; d < 320; d++) {
    SPI_transfer(*((uint8_t *)FIFO_bg_wnd + d));
  }
  gpio_put(TFT_CS, HIGH);
}
