void ini_LCD() {


  pinMode(TFT_DC,   OUTPUT);
  pinMode(TFT_RST,  OUTPUT);
  pinMode(TFT_CS,   OUTPUT);
  pinMode(TFT_MOSI, OUTPUT);
  pinMode(TFT_SCLK, OUTPUT);

  SPI.begin();
  
  delay(1000);
  Serial.println("SPI BEGIN");
  
  SPI.endTransaction();
  delay(100);
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
  tftSendCommand1(0x3A, 0x03);     // 12Bit Pixel Mode
  delay(100);
  tftSendCommand1(0x26, 0x08);     // Gamma value
  delay(100);
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

// 画面クリア
void cls() {
  SPI.beginTransaction(lcd_SPISettings);
  tftSendCommand4(0x2A, 0, 0, 0, 239) ; // Colmun Address
  tftSendCommand4(0x2B, 0, 0, 0, 239) ; // Row Address
  digitalWrite(TFT_CS, LOW);
  digitalWrite(TFT_DC, LOW); // Command mode
  SPI.transfer(0x2C);
  
  digitalWrite(TFT_DC, HIGH); // Data mode
  memset(SPIBuf, 0b11111111, 360);//0b00000000
  for (int i = 0; i < 240; i++) {
    SPI.transfer(SPIBuf, 360);
  }
  digitalWrite(TFT_DC, LOW); // Command mode
  digitalWrite(TFT_CS, HIGH);
  SPI.endTransaction();
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
// TFTにコマンド+3バイトデータを送信
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
