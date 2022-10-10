# arduinoにSRAMを追加する
arduino mega 2560だけでは、vramもsramもエミュレートすることができません。したがって、何らかの方法で増設しなければなりません。

## 64KByteの追加
ArduinoにSRAMを追加する方法として、SPI接続可能なSRAMを追加する方法があります。

- SRAM 512Kb 23LC512-I/P
  - https://akizukidenshi.com/catalog/g/gI-14062/
- 丸ピンＩＣソケット（８Ｐ）
  - https://akizukidenshi.com/catalog/g/gP-00035/
- カーボン抵抗（炭素皮膜抵抗）　１／４Ｗ１０ｋΩ　（１００本入）
  - https://akizukidenshi.com/catalog/g/gR-25103/
- カーボン抵抗（炭素皮膜抵抗）　１／４Ｗ１００ｋΩ　（１００本入）
  - https://akizukidenshi.com/catalog/g/gR-25104/

抵抗は100本も必要ありません。手持ちがある方はそちらを使用してください。ICソケットも必須ではありませんが、あった方が失敗が少なくて良いと思います。

接続は以下の通りに行います。

図を載せる

## 利用方法
SPIにより接続します。ソースコードは以下の通りです。

```c:sram.ino
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
```
