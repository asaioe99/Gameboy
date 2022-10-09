#include <SPI.h>

// 書き込み
void ram_wt(uint16_t addr, uint8_t data) {

  uint8_t addr_h = (addr >> 4) & 0xFF;
  uint8_t addr_l = addr & 0xFF;
 
  digitalWrite(10,LOW);
  SPI.transfer(0x02);          //0x02 書き込みモード
  SPI.transfer(addr_h);
  SPI.transfer(addr_l);
  SPI.transfer(data);
  digitalWrite(10,HIGH);
}

// 読み込み
int ram_rd(uint16_t addr) {

  uint8_t r_data;
 
  uint8_t addr_h = (addr >> 4) & 0xFF;
  uint8_t addr_l = addr & 0xFF;
 
  digitalWrite(10,LOW);
  SPI.transfer(0x03);           //0x03 読み込みモード
  SPI.transfer(addr_h);
  SPI.transfer(addr_l);
  r_data = SPI.transfer(0);
  digitalWrite(10,HIGH);
 
  return r_data;
}

void setup() {
  Serial.begin(115200);

  pinMode (10, OUTPUT);
  digitalWrite(10,HIGH);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setDataMode(SPI_MODE0);
}

void loop() {
  uint8_t wt_data = 0xFF;
  uint8_t rd_data;
  uint16_t address = 0x0000;
  char buf[16];

  while(true) {
    ram_wt(address, wt_data);
    rd_data = ram_rd(address);
    sprintf(buf, "%04X : %02X/%02X", address, wt_data, rd_data);
    Serial.println(buf);
    address++;
    delayMicroseconds(10);
  }
}
