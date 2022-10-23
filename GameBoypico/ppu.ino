void ini_LCD() {

  /*
      tft.init(240, 240);           // Init ST7789 240x240
      tft.setSPISpeed(80000000);
      tft.startWrite();
      tft.fillScreen(ST77XX_WHITE);
      tft.endWrite();
      delay(500);
      tft.fillScreen(ST77XX_BLACK);
      tft.endWrite();
      delay(500);
      tft.drawPixel(tft.width() / 2, tft.height() / 2, ST77XX_GREEN);
      delay(500);
  */

  SPI.begin();
  /*
    gpio_init(TFT_DC);                //gpio番号を初期化する
    gpio_set_dir(TFT_DC,   GPIO_OUT); //出力に設定する
    gpio_init(TFT_RST);
    gpio_set_dir(TFT_RST,  GPIO_OUT);
    gpio_init(TFT_CS);
    gpio_set_dir(TFT_CS,   GPIO_OUT);
    gpio_init(TFT_MOSI);
    gpio_set_dir(TFT_MOSI, GPIO_OUT);
    gpio_init(TFT_SCLK);
    gpio_set_dir(TFT_SCLK, GPIO_OUT);

    gpio_put(TFT_DC, 1);
  */
  pinMode(TFT_DC,   OUTPUT);
  pinMode(TFT_RST,  OUTPUT);
  pinMode(TFT_CS,   OUTPUT);
  pinMode(TFT_MOSI, OUTPUT);
  pinMode(TFT_SCLK, OUTPUT);

  SPI.beginTransaction(lcd_SPISettings);
  for (int i = 0; i < 1000; i++) {
    Serial.println("Hello World!");
  }

  digitalWrite(TFT_CS, LOW);
  delayMicroseconds(10);
  Serial.println("chk1");
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
  delayMicroseconds(10);

  return;
}

// 画面クリア
void cls() {
  SPI.beginTransaction(lcd_SPISettings);

  tftSendCommand4(0x2A, 0, 0, 0, 239) ; // Colmun Address
  tftSendCommand4(0x2B, 0, 0, 0, 239) ; // Row Address

  digitalWrite(TFT_CS, LOW);
  delayMicroseconds(10);
  digitalWrite(TFT_DC, LOW); // Command mode
  delayMicroseconds(10);

  SPI.transfer(0x2C);

  digitalWrite(TFT_DC, HIGH); // Data mode
  delayMicroseconds(10);
  memset(SPIBuf, 0b11111111, 360);//0b00000000
  for (int i = 0; i < 240; i++) {
    SPI.transfer(SPIBuf, 360);
  }
  digitalWrite(TFT_DC, LOW); // Command mode
  delayMicroseconds(10);
  digitalWrite(TFT_CS, HIGH);
  delayMicroseconds(10);

  SPI.endTransaction();
}

// TFTにコマンドを送信
void tftSendCommand(uint8_t command) {
  digitalWrite(TFT_CS, LOW);
  delayMicroseconds(10);
  digitalWrite(TFT_DC, LOW); // Command mode
  delayMicroseconds(10);
  SPI.transfer(command);
  digitalWrite(TFT_CS, HIGH);
  delayMicroseconds(10);
}

// TFTにコマンド+1バイトデータを送信
void tftSendCommand1(uint8_t command, uint8_t data1) {
  digitalWrite(TFT_CS, LOW);
  delayMicroseconds(10);
  digitalWrite(TFT_DC, LOW); // Command mode
  delayMicroseconds(10);
  SPI.transfer(command);
  digitalWrite(TFT_DC, HIGH); // Command mode
  delayMicroseconds(10);
  SPI.transfer(data1);
  digitalWrite(TFT_CS, HIGH);
  delayMicroseconds(10);
}

// TFTにコマンド+2バイトデータを送信
void tftSendCommand2(uint8_t command, uint8_t data1, uint8_t data2) {
  digitalWrite(TFT_CS, LOW);
  delayMicroseconds(10);
  digitalWrite(TFT_DC, LOW); // Command mode
  delayMicroseconds(10);
  SPI.transfer(command);
  digitalWrite(TFT_DC, HIGH); // Command mode
  delayMicroseconds(10);
  SPI.transfer(data1);
  SPI.transfer(data2);
  digitalWrite(TFT_CS, HIGH);
  delayMicroseconds(10);
}
// TFTにコマンド+3バイトデータを送信
void tftSendCommand3(uint8_t command, uint8_t data1, uint8_t data2, uint8_t data3) {
  digitalWrite(TFT_CS, LOW);
  delayMicroseconds(10);
  digitalWrite(TFT_DC, LOW); // Command mode
  delayMicroseconds(10);
  SPI.transfer(command);
  digitalWrite(TFT_DC, HIGH); // Command mode
  delayMicroseconds(10);
  SPI.transfer(data1);
  SPI.transfer(data2);
  SPI.transfer(data3);
  digitalWrite(TFT_CS, HIGH);
  delayMicroseconds(10);
}
// TFTにコマンド+4バイトデータを送信
void tftSendCommand4(uint8_t command, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4) {
  digitalWrite(TFT_CS, LOW);
  delayMicroseconds(10);
  digitalWrite(TFT_DC, LOW); // Command mode
  delayMicroseconds(10);
  SPI.transfer(command);
  digitalWrite(TFT_DC, HIGH); // Command mode
  delayMicroseconds(10);
  SPI.transfer(data1);
  SPI.transfer(data2);
  SPI.transfer(data3);
  SPI.transfer(data4);
  digitalWrite(TFT_CS, HIGH);
  delayMicroseconds(10);
}
