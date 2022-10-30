void ini_LCD() {
  // pin assign
  pinMode(MOSI, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(CS,  OUTPUT);
  pinMode(DC,  OUTPUT);
  pinMode(BL,  OUTPUT);
  pinMode(RST, OUTPUT);

  // BACK LIGHT ON
  gpio_put(BL, HIGH);

  // --- HARD Ware Reset
  gpio_put(RST, HIGH);
  delay(120);                  // VDD goes high at start, pause for 500 ms
  gpio_put(RST, LOW);  // Bring reset low
  delay(120);                  // Wait 100 ms
  gpio_put(RST, HIGH); // Bring out of reset
  delay(120);                  // Wait 500 ms, more then 120 ms
  // --- SOFT Ware Reset
  SendCommand(0x01);       // SOFTWARE RESET
  delay(50);

  // --- Initial Comands
  SendCommand(0x28);            // Display OFF : 0x29 - Display ON
  delay(500);

  SendCommand(0x11);            // Sleep Out
  delay(500);
  SendCommand1(0x3A, 0x05);       // 16Bit Pixel Mode
  delay(10);
  SendCommand1(0x36, 0b01100000); // MX MY MV ML RGB MH x x：横向き２
  SendCommand2(0xB6, 0x15, 0x02); // Display settings #5
  SendCommand1(0xB4, 0x00);     // Display inversion control

  SendCommand(0x13);            // NomalDisplayMode
  SendCommand(0x20);            // Display Inversion Off

  cls();
  delay(500);
  SendCommand(0x29);            // Display ON : 0x28 - Display OFF
  delay(500);
}

void SPI_transfer(uint8_t data) {
  for (uint8_t bit = 0x80; bit; bit >>= 1) {
    gpio_put(MOSI, data & bit);
    gpio_put(CLK, HIGH);
    gpio_put(CLK, LOW);
  }
}

void SPI_transfer(uint8_t* data, int dataLen) {
  for (int d = 0; d < dataLen; d++) {
    SPI_transfer(*(data + d));
  }
}

// TFTにコマンドを送信
void SendCommand(uint8_t command) {
  gpio_put(CS, LOW); //
  gpio_put(DC, LOW); // Command mode
  SPI_transfer(command);
  gpio_put(CS, HIGH);  //
}

// TFTにコマンド+1バイトデータを送信
void SendCommand1(uint8_t command, uint8_t data1) {
  gpio_put(CS, LOW); //
  gpio_put(DC, LOW); // Command mode
  SPI_transfer(command);
  gpio_put(DC, HIGH); // Command mode
  SPI_transfer(data1);
  gpio_put(CS, HIGH);  //
}

// TFTにコマンド+2バイトデータを送信
void SendCommand2(uint8_t command, uint8_t data1, uint8_t data2) {
  gpio_put(CS, LOW); //
  gpio_put(DC, LOW); // Command mode
  SPI_transfer(command);
  gpio_put(DC, HIGH); // Command mode
  SPI_transfer(data1);
  SPI_transfer(data2);
  gpio_put(CS, HIGH);  //
}

// TFTにコマンド+3バイトデータを送信
void SendCommand3(uint8_t command, uint8_t data1, uint8_t data2, uint8_t data3) {
  gpio_put(CS, LOW); //
  gpio_put(DC, LOW); // Command mode
  SPI_transfer(command);
  gpio_put(DC, HIGH); // Command mode
  SPI_transfer(data1);
  SPI_transfer(data2);
  SPI_transfer(data3);
  gpio_put(CS, HIGH);  //
}

// TFTにコマンド+4バイトデータを送信
void SendCommand4(uint8_t command, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4) {
  gpio_put(CS, LOW); //
  gpio_put(DC, LOW); // Command mode
  SPI_transfer(command);
  gpio_put(DC, HIGH); // Command mode
  SPI_transfer(data1);
  SPI_transfer(data2);
  SPI_transfer(data3);
  SPI_transfer(data4);
  gpio_put(CS, HIGH);  //
}

// 画面クリア
void cls() {

  SendCommand4(0x2A, 0, 0, 0, 239) ; // Colmun Address
  SendCommand4(0x2B, 0, 0, 0, 239) ; // Row Address

  gpio_put(CS, LOW);
  gpio_put(DC, LOW); // Command mode
  SPI_transfer(0x2C);
  gpio_put(DC, HIGH); // Data mode
  for (int i = 0; i < 240; i++) {
    for (int j = 0; j < 240; j++) {
      SPI_transfer(0b11111111);
      SPI_transfer(0b11111111);
    }
  }
  gpio_put(DC, LOW); // Command mode
  gpio_put(CS, HIGH);
}
// BITマップ表示
//void drowBitMap(uint16_t y) {
void drowBitMap() {

  SendCommand4(0x2A, 0, 40, 0, 199) ; // Colmun Address
  SendCommand4(0x2B, 0, 40, 0, 183) ; // Row Address

  gpio_put(CS, LOW);
  gpio_put(DC, LOW); // Command mode
  SPI_transfer(0x2C);
  gpio_put(DC, HIGH); // Data mode
  for (uint16_t d = 0; d < 160 * 144; d++) {
    for (uint8_t bit = 0x80; bit; bit >>= 1) {
      gpio_put(MOSI, (uint8_t)(*(FIFO_bg_wnd + d) & 0x00FF) & bit);
      gpio_put(CLK, HIGH);
      gpio_put(CLK, LOW);
    }
    for (uint8_t bit = 0x80; bit; bit >>= 1) {
      gpio_put(MOSI, (uint8_t)(*(FIFO_bg_wnd + d) >> 8) & bit);
      gpio_put(CLK, HIGH);
      gpio_put(CLK, LOW);
    }
    //SPI_transfer((uint8_t)(*(FIFO_bg_wnd + d) & 0x00FF));
    //SPI_transfer((uint8_t)(*(FIFO_bg_wnd + d) >> 8));
  }
  gpio_put(CS, HIGH);
}
