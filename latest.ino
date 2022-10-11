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

// bootstrap（実物のため、そのままは掲載不可）
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
uint8_t A;
uint8_t F;
uint8_t B;
uint8_t C;
uint8_t D;
uint8_t E;
uint8_t H;
uint8_t L;
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

// 命令の関数配列化
void (*LDrr[])() = {
    LD_B_B, LD_B_C, LD_B_D, LD_B_E, LD_B_H, LD_B_L, LD_B_HL,
    LD_C_B, LD_C_C, LD_C_D, LD_C_E, LD_C_H, LD_C_L, LD_C_HL,
    LD_D_B, LD_D_C, LD_D_D, LD_D_E, LD_D_H, LD_D_L, LD_D_HL,
    LD_E_B, LD_E_C, LD_E_D, LD_E_E, LD_E_H, LD_E_L, LD_E_HL,
    LD_H_B, LD_H_C, LD_H_D, LD_H_E, LD_H_H, LD_H_L, LD_H_HL,
    LD_L_B, LD_L_C, LD_L_D, LD_L_E, LD_L_H, LD_L_L, LD_L_HL,
    };

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
  // とりあえず素通し
  return get_byte(pc);
}

void execute(uint8_t pc) {
  uint8_t code = fetch(pc);

  

  switch(code) {
    case 0x06: //LD_B_n
    case 0x0E: //LD_C_n
    case 0x16: //LD_D_n
    case 0x1E: //LD_E_n
    case 0x26: //LD_H_n
    case 0x2E: //LD_L_n
      LD_r_n(code);
      break;
    case 0x46:
    case 0x4E:
    case 0x56:
    case 0x5E:
    case 0x66:
    case 0x6E:
    case 0x7E:
      LD_r_HL(code);
      break;
    case 0x40: //LD_B_B
    case 0x41: //LD_B_C
    case 0x42: //LD_B_D
    case 0x43: //LD_B_E
    case 0x44: //LD_B_H
    case 0x45: //LD_B_L
    case 0x48: //LD_C_B
    case 0x49: //LD_C_C
    case 0x4A: //LD_C_D
    case 0x4B: //LD_C_E
    case 0x4C: //LD_C_H
    case 0x4D: //LD_C_L
    case 0x50: //LD_D_B
    case 0x51: //LD_D_C
    case 0x52: //LD_D_D
    case 0x53: //LD_D_E
    case 0x54: //LD_D_H
    case 0x55: //LD_D_L
    case 0x58: //LD_E_B
    case 0x59: //LD_E_C
    case 0x5A: //LD_E_D
    case 0x5B: //LD_E_E
    case 0x5C: //LD_E_H
    case 0x5D: //LD_E_L
    case 0x60: //LD_H_B
    case 0x61: //LD_H_C
    case 0x62: //LD_H_D
    case 0x63: //LD_H_E
    case 0x64: //LD_H_H
    case 0x65: //LD_H_L
    case 0x68: //LD_L_B
    case 0x69: //LD_L_C
    case 0x6A: //LD_L_D
    case 0x6B: //LD_L_E
    case 0x6C: //LD_L_H
    case 0x6D: //LD_L_L
    case 0x7F: //LD_A_A
    case 0x78: //LD_A_C
    case 0x79: //LD_A_C
    case 0x7A: //LD_A_D
    case 0x7B: //LD_A_E
    case 0x7C: //LD_A_H
    case 0x7D: //LD_A_L
      LD_r_r(code);
      break;
  }
}

// 初期化
void setup() {
  Serial.begin(9600);
  ini();
  load_rom_header();  // romheader読み込み
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
