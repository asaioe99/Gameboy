/*
  カートリッジpin割り当て表(カートリッジ左から1pin)
  https://gbdev.gg8.se/wiki/articles/DMG_Schematics
  1: Vcc   9: a3    17: a11  25: d3
  2: CLK? 10: a4   18: a12  26: d4
  3: /WR  11: a5   19: a13  27: d5
  4: /RD  12: a6   20: a14  28: d6
  5: /CS  13: a7   21: a15  29: d7
  6: a0   14: a8   22: d0   30: /RST
  7: a1   15: a9   23: d1   31: Vin
  8: a2   16: a10  24: d2   32: GND
*/

typedef struct {
  uint8_t ety_point[4];
  uint8_t logo[48];
  uint8_t tle[16];
  uint8_t lsc_code[2];
  uint8_t sgb_flag;
  uint8_t ctg_type;
  uint8_t rom_size;
  uint8_t ram_size;
  uint8_t des_code;
  uint8_t old_code;
  uint8_t rom_ver;
  uint8_t hed_chk;
  uint8_t gbl_chk[2];
} _rom_header;

_rom_header rom_header;

// アドレスバスの指定
void set_addr(uint16_t address) {
  PORTA = address & 0xFF;
  PORTK = (address >> 8) & 0xFF;
}
// 指定アドレスから1Byte読み出し
uint8_t get_byte(uint16_t address) {
  set_addr(address);
  PORTL = B00000101;  // CS=1,R=0,W=1を設定
  delayMicroseconds(10);
  uint8_t result = PINF;
  PORTL = B00000111;  // カートリッジ2~4pinに何らかの設定
  delayMicroseconds(10);
  return result;
}
// GPIO割り当て（レジスタにより）
void ini() {
  DDRA = B11111111;   // PORT A に読み込みたいアドレスのLSBを指定
  DDRK = B11111111;   // PORT K に読み込みたいアドレスのMSBを指定
  DDRF = B00000000;   // PORT F にByte出力を指定
  DDRL = B00000111;   // PORT L に 最下位bitからWrite、Read、ChipSelect（ROM/RAM）を指定
  // 例えば、ROMから読み出す時は、CS=1,R=0,W=1となる（負論理）
  //        RAMから読み出す時は、CS=0,R=0,W=1となる
  PORTL = B00000000;  // W,R,CSの初期化
  PORTA = B00000000;  // アドレス指定の初期化
  PORTK = B00000000;  //
}

void load_rom_header() {
  for (int i = 0; i < 4; i++) {
    rom_header.ety_point[i] = get_byte(0x100 + i);
  }
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 16; i++) {
      rom_header.logo[i + 16 * j] = get_byte(0x104 + i + 16 * j);
    }
  }
  for (int i = 0; i < 16; i++) {
    rom_header.tle[i] = get_byte(0x134 + i);
  }

  rom_header.lsc_code[0] = get_byte(0x144);
  rom_header.lsc_code[1] = get_byte(0x145);
  rom_header.sgb_flag    = get_byte(0x146);
  rom_header.ctg_type    = get_byte(0x147);
  rom_header.rom_size    = get_byte(0x148);
  rom_header.ram_size    = get_byte(0x149);
  rom_header.des_code    = get_byte(0x14a);
  rom_header.old_code    = get_byte(0x14b);
  rom_header.rom_ver     = get_byte(0x14c);
  rom_header.hed_chk     = get_byte(0x14d);
  rom_header.gbl_chk[0]  = get_byte(0x14e);
  rom_header.gbl_chk[1]  = get_byte(0x14f);
}

void display_rom_header() {

  char buf[16];

  Serial.println("== DUMP START ==");
  Serial.println("= Entry Point =");
  for (int i = 0; i < 4; i++) {
    sprintf(buf, "%04X : %02X ", 0x100 + i, rom_header.ety_point[i]);
    Serial.print(buf);
    Serial.println("");
  }
  Serial.println("= logo =");
  for (int j = 0; j < 3; j++) {
    sprintf(buf, "%04X : ", 0x104 + j * 16);
    Serial.print(buf);
    for (int i = 0; i < 16; i++) {
      sprintf(buf, "%02X ", rom_header.logo[i + 16 * j]);
      Serial.print(buf);
    }
    Serial.println("");
  }
  Serial.println("= title =");
  sprintf(buf, "%04X : ", 0x134);
  Serial.print(buf);
  for (int i = 0; i < 16; i++) {
    sprintf(buf, "%02X ", rom_header.tle[i]);
    Serial.print(buf);
  }
  Serial.println("");
  Serial.println("= license code =");
  sprintf(buf, "0144 : %02X %02X", rom_header.lsc_code[0], rom_header.lsc_code[1]);
  Serial.println(buf);
  Serial.println("= sgb flag =");
  sprintf(buf, "0146 : %02X", rom_header.sgb_flag);
  Serial.println(buf);
  Serial.println("= cartridge type =");
  sprintf(buf, "0147 : %02X", rom_header.ctg_type);
  Serial.println(buf);
  Serial.println("= rom size =");
  sprintf(buf, "0148 : %02X", rom_header.rom_size);
  Serial.println(buf);
  Serial.println("= ram size =");
  sprintf(buf, "0149 : %02X", rom_header.ram_size);
  Serial.println(buf);
  Serial.println("= destination code =");
  sprintf(buf, "014a : %02X", rom_header.des_code);
  Serial.println(buf); 
  Serial.println("= old_llicensee_code =");
  sprintf(buf, "014b : %02X", rom_header.old_code);
  Serial.println(buf); 
  Serial.println("= rom_version =");
  sprintf(buf, "014c : %02X", rom_header.rom_ver);
  Serial.println(buf); 
  Serial.println("= header checksum =");
  sprintf(buf, "014d : %02X", rom_header.hed_chk);
  Serial.println(buf); 
  Serial.println("= license code =");
  sprintf(buf, "014e : %02X %02X", rom_header.gbl_chk[0], rom_header.gbl_chk[1]);
  Serial.println(buf);
  Serial.println("==  END  ==");
}

// 初期化
void setup() {
  Serial.begin(9600);
  ini();
  load_rom_header();
}

void loop() {

  PORTL = B00000111;  // 読み書き時以外は常にこの状態にするらしい
  delayMicroseconds(10);

  display_rom_header();

  while (1);
}
