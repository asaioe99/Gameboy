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
#include <SPI.h>

uint8_t bootstrap[] = {
  0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
  0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
  0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
  0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
  0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20, 
  0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04, 
  0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2, 
  0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06, 
  0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xE2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20, 
  0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17, 
  0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 
  0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 
  0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 
  0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C, 
  0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20, 
  0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50,
};

uint16_t pc;
uint16_t sp;
uint8_t a;
uint8_t f;
uint8_t b;
uint8_t c;
uint8_t d;
uint8_t e;
uint8_t h;
uint8_t l;
uint8_t cc;

uint8_t oam[0xa0];
uint8_t io[0x80];
uint8_t hram[0x7F];
uint8_t ie;

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
// RAM有効化
void enable_ram() {
  DataBusAsOutput();
  put_byte(0x0000, 0x0A);
  delayMicroseconds(10);
  DataBusAsInput();
}
// RAM無効化
void disable_ram() {
  DataBusAsOutput();
  put_byte(0x0000, 0x00);
  delay(50);  // ?
  DataBusAsInput();
}
// romの指定アドレスから1Byte読み出し
uint8_t get_rom_byte(uint16_t address) {
  set_addr(address);
  PORTL = B00000101;  // CS=1,R=0,W=1を設定
  delayMicroseconds(10);
  uint8_t result = PINF;
  PORTL = B00000111;  // カートリッジ2~4pinに何らかの設定
  delayMicroseconds(10);
  return result;
}
// ramの指定アドレスから1Byte読み出し
byte get_ram_byte(uint16_t address) {
  set_addr(address);
  PORTL = B00000001;
  delayMicroseconds(10);
  byte result = PINF;
  PORTL = B00000111;
  delayMicroseconds(10);
  return result;
}
// 指定アドレスに1Byte書き込み
void put_byte(uint16_t address, uint8_t data) {
  set_addr(address);
  PORTF = data;
  PORTL = B00000110;
  delayMicroseconds(10);
  PORTL = B00000111;
  delayMicroseconds(10);
}

uint8_t put_ram_byte(uint16_t address, uint8_t data) {
  set_addr(address);
  PORTF = data;
  PORTL = B00000010;
  delayMicroseconds(10);
  PORTL = B00000111;
  delayMicroseconds(10);
}

void DataBusAsInput() {
  DDRF = B00000000;
}

void DataBusAsOutput() {
  DDRF = B11111111;
}

// 書き込み
void sram_wt(uint16_t addr, uint8_t data) {

  uint8_t addr_h = (addr >> 4) & 0xFF;
  uint8_t addr_l = addr & 0xFF;

  digitalWrite(10, LOW);
  SPI.transfer(0x02);  //0x02 書き込みモード
  SPI.transfer(addr_h);
  SPI.transfer(addr_l);
  SPI.transfer(data);
  digitalWrite(10, HIGH);
}

// 読み込み
int sram_rd(uint16_t addr) {

  uint8_t r_data;

  uint8_t addr_h = (addr >> 4) & 0xFF;
  uint8_t addr_l = addr & 0xFF;

  digitalWrite(10, LOW);
  SPI.transfer(0x03);  //0x03 読み込みモード
  SPI.transfer(addr_h);
  SPI.transfer(addr_l);
  r_data = SPI.transfer(0);
  digitalWrite(10, HIGH);

  return r_data;
}


// GPIO割り当て（レジスタにより）
void ini() {
  DDRA = B11111111;  // PORT A に読み込みたいアドレスのLSBを指定
  DDRK = B11111111;  // PORT K に読み込みたいアドレスのMSBを指定
  DDRF = B00000000;  // PORT F にByte出力を指定
  DDRL = B00000111;  // PORT L に 最下位bitからWrite、Read、ChipSelect（ROM/RAM）を指定
  // 例えば、ROMから読み出す時は、CS=1,R=0,W=1となる（負論理）
  //        RAMから読み出す時は、CS=0,R=0,W=1となる
  PORTL = B00000000;  // W,R,CSの初期化
  PORTA = B00000000;  // アドレス指定の初期化
  PORTK = B00000000;  //

  // レジスタ設定
  uint16_t pc = 0x0000;
  uint16_t sp = 0x0000;
  uint8_t a = 0x00;
  uint8_t f = 0x00;
  uint8_t b = 0x00;
  uint8_t c = 0x00;
  uint8_t d = 0x00;
  uint8_t e = 0x00;
  uint8_t h = 0x00;
  uint8_t l = 0x00;
  uint8_t cc = 0x00;

  // SPI初期化（SRAM）
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setDataMode(SPI_MODE0);
  /*
  uint8_t oam[0xa0];
  uint8_t io[0x80];
  uint8_t hram[0x7F];
  uint8_t ie;
  */
}

void load_rom_header() {
  for (int i = 0; i < 4; i++) {
    rom_header.ety_point[i] = get_rom_byte(0x100 + i);
  }
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 16; i++) {
      rom_header.logo[i + 16 * j] = get_rom_byte(0x104 + i + 16 * j);
    }
  }
  for (int i = 0; i < 16; i++) {
    rom_header.tle[i] = get_rom_byte(0x134 + i);
  }

  rom_header.lsc_code[0] = get_rom_byte(0x144);
  rom_header.lsc_code[1] = get_rom_byte(0x145);
  rom_header.sgb_flag = get_rom_byte(0x146);
  rom_header.ctg_type = get_rom_byte(0x147);
  rom_header.rom_size = get_rom_byte(0x148);
  rom_header.ram_size = get_rom_byte(0x149);
  rom_header.des_code = get_rom_byte(0x14a);
  rom_header.old_code = get_rom_byte(0x14b);
  rom_header.rom_ver = get_rom_byte(0x14c);
  rom_header.hed_chk = get_rom_byte(0x14d);
  rom_header.gbl_chk[0] = get_rom_byte(0x14e);
  rom_header.gbl_chk[1] = get_rom_byte(0x14f);
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
// いずれ消す
void dump_bank00() {
  char buf[16];
  Serial.println("= mbc1 bank 00 dump =");
  for (int j = 0; j < 0x400; j++) {
    sprintf(buf, "%04X : ", j * 16);
    Serial.print(buf);
    for (int i = 0; i < 16; i++) {
      sprintf(buf, "%02X ", get_rom_byte(i + 16 * j));
      Serial.print(buf);
    }
    Serial.println("");
  }
}

void dump_rom_bank(uint8_t bank) {

  Serial.print("rom bank : ");
  Serial.println(bank);

  uint16_t offset = 0;
  if (bank > 0) {
    offset = 0x4000;
    switch_rom_bank(bank);  // bank number < 2 ^ (rom_header.rom_size  + 1)
  }

  char buf[16];

  for (uint16_t address = 0; address < 0x4000; address++) {
    if ((address + offset) % 16 == 0) {
      sprintf(buf, "%04X : %02X ", address + offset, get_rom_byte(address + offset));
      Serial.print(buf);
    } else if ((address + offset) % 16 == 15) {
      sprintf(buf, "%02X ", get_rom_byte(address + offset));
      Serial.println(buf);
    } else {
      sprintf(buf, "%02X ", get_rom_byte(address + offset));
      Serial.print(buf);
    }
  }
}

void switch_rom_bank(uint8_t bank) {
  // rom_sizeが0の場合、bankは存在しない
  if (rom_header.rom_size > 0) {
    DataBusAsOutput();
    put_byte(0x2100, bank);
    DataBusAsInput();
  }
}

void dump_ram_bank(uint8_t bank) {

  Serial.print("ram bank : ");
  Serial.println(bank);

  switch_ram_bank(bank);

  char buf[16];

  for (uint16_t address = 0xA000; address < 0xC000; address++) {
    if (address % 16 == 0) {
      sprintf(buf, "%04X : %02X ", address, get_ram_byte(address));
      Serial.print(buf);
    } else if (address % 16 == 15) {
      sprintf(buf, "%02X ", get_ram_byte(address));
      Serial.println(buf);
    } else {
      sprintf(buf, "%02X ", get_ram_byte(address));
      Serial.print(buf);
    }
  }
}
// ramのバンク切り替え　MBC1のみ対応
void switch_ram_bank(byte bank) {
  DataBusAsOutput();
  put_byte(0x4000, bank);
  DataBusAsInput();
}

void write_ram_bank(uint8_t bank) {

  Serial.print("write ram bank ");
  Serial.println(bank);
  switch_ram_bank(bank);
  DataBusAsOutput();

  for (uint16_t address = 0xA000; address < 0xC000; address++) {
    put_ram_byte(address, 0xcc);
  }

  DataBusAsInput();
}

// 初期化
void setup() {
  Serial.begin(9600);
  ini();
  load_rom_header();  // romheader読み込み
}

uint8_t get_byte(uint8_t addr) {
  if (addr < 0x4000) {
    return get_rom_byte(addr);
  } else if (addr < 0x8000) {
    return get_rom_byte(addr);
  } else if (addr < 0xA000) {
    return sram_rd(addr);
  } else if (addr < 0xC000) {
    return get_ram_byte(addr);
  } else if (addr < 0xE000) {
    return sram_rd(addr);
  } else if (addr < 0xFE00) {  // Mirror of C000~DDFF
    return sram_rd(addr);
  } else if (addr < 0xFEA0) {  // Sprite attribute table (OAM)
    return oam[addr - 0xFE00];
  } else if (addr < 0xFF00) {  // Not Usable
    return sram_rd(addr);
  } else if (addr < 0xFF80) {  // I/O Register
    return io[addr - 0xFF00];
  } else if (addr < 0xFFFF) {  // High RAM
    return hram[addr - 0xFF80];
  } else if (addr == 0xFFFF) { // Interrupt Enable register(IE)
    return ie;
  }
  return 0x0000;
}
// 読み込み
uint8_t fetch(uint8_t pc) {
  return get_byte(pc);
}

void dec_and_exe(uint) {

}

void loop() {

  PORTL = B00000111;  // 読み書き時以外は常にこの状態にするらしい
  delayMicroseconds(10);

  //for debug
  //display_rom_header();
  //dump_bank00();
  //dump_rom_bank(1);
  //enable_ram();
  //write_ram_bank(0);
  //dump_ram_bank(0);
  //disable_ram();



  while (1)
    ;
}
