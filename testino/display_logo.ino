/*
カートリッジpin割り当て表(カートリッジ左から1pin)
https://gbdev.gg8.se/wiki/articles/DMG_Schematics
1: Vcc   9: a3 	 17: a11  25: d3
2: CLK? 10: a4   18: a12  26: d4
3: /WR  11: a5   19: a13  27: d5
4: /RD  12: a6   20: a14  28: d6
5: /CS  13: a7   21: a15  29: d7
6: a0	  14: a8   22: d0	  30: /RST
7: a1	  15: a9   23: d1	  31: Vin
8: a2	  16: a10	 24: d2	  32: GND
*/
// アドレスバスの指定
void set_addr(uint16_t address) { 
  PORTA = address & 0xFF;
  PORTK = (address >> 8) & 0xFF;
}
// 指定アドレスから1Byte読み出し
uint8_t get_byte(uint16_t address) {
  set_addr(address);
  PORTL = B00000101; // CS=1,R=0,W=1を設定
  delayMicroseconds(10);
  uint8_t result = PINF;
  PORTL = B00000111; // カートリッジ2~4pinに何らかの設定
  delayMicroseconds(10);
  return result;
}
// GPIO割り当て（レジスタにより）
void init() {
  DDRA = B11111111; // PORT A に読み込みたいアドレスのLSBを指定
  DDRK = B11111111; // PORT K に読み込みたいアドレスのMSBを指定
  DDRF = B00000000; // PORT F にByte出力を指定
  DDRL = B00000111; // PORT L に 最下位bitからWrite、Read、ChipSelect（ROM/RAM）を指定
                    // 例えば、ROMから読み出す時は、CS=1,R=0,W=1となる（負論理）
                    //        RAMから読み出す時は、CS=0,R=0,W=1となる
  PORTL = B00000000;// W,R,CSの初期化
  PORTA = B00000000;// アドレス指定の初期化
  PORTK = B00000000;//
}
// 初期化
void setup() {
  Serial.begin(9600);
  init();
}
// ロゴ表示
void loop() {
  Serial.println("== DUMP START ==");
  
  PORTL = B00000111; // 読み書き時以外は常にこの状態にするらしい
  delayMicroseconds(10);
  
  char buf[3];
  Serial.print("logo:");
  for (int j = 0; j < 3 ; j++) {
    for (int i = 0; i < 16; i++) {
      sprintf(buf,”%02X”,get_byte(0x104 + i + 16 * j));
      Serial.print(buf);
    }
  Serial.println("");
  }
  Serial.println("==  END  ==");
  
  while (1) ;
}
