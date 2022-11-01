#include "rom.h"
#include "cpu_instrs.h"

#define HBYTE(u) ((u >> 8) & 0xFF)
#define LBYTE(u) (u & 0xFF)
#define AF(A, F) ((uint16_t)A << 8) + F
#define BC(B, C) ((uint16_t)B << 8) + C
#define DE(D, E) ((uint16_t)D << 8) + E
#define HL(H, L) ((uint16_t)H << 8) + L

#define MOSI  11    // MOSI　本来ならGP3 ArduinoIDE上でpinassignが変更可能になったらHWSPIに切り替え
#define CLK   10    // CLK　本来ならGP2
#define CS    9     // Data/Command
#define DC    8     // Data/Command
#define RST   12    // RESET
#define BL    13    // BACK LIGHT

#define MBC_NOT_SUPPORTED  -1
#define MBC_NONE            0
#define MBC_1               1
#define MBC_2               2
#define MBC_3               3
#define MBC_5               5

//for debug
char buf_f1[64];
char buf_f2[64];
char buf_b1[64];
char buf_b2[64];

uint8_t SPIBuf[360] ; // SPI転送用バッファ
uint16_t FIFO_bg_wnd[160 * 144];

uint8_t VRAM[0x2000];
uint8_t oam[0xa0];
uint8_t io[0x80];
uint8_t hram[0x7F];
uint8_t WRAM[0x2000];
uint8_t CRAM[0x2000];
uint8_t ie;

uint16_t pc;
uint16_t sp;
uint8_t  AR;
uint8_t  BR;
uint8_t  CR;
uint8_t  DR;
uint8_t  ER;
uint8_t  FR;
uint8_t  HR;
uint8_t  LR;
uint32_t cc;

// 内容不明（忘れた）
bool ime; //割り込み許可

uint8_t mbcType;
uint8_t rom_bank_num;
uint8_t cc_dec;
uint8_t code;
int16_t scaline_counter;

// GPIO割り当て（レジスタにより）
void ini() {
  // レジスタ設定
  pc = 0x0000;
  sp = 0x0000;
  AR = 0x00;
  BR = 0x00;
  CR = 0x00;
  DR = 0x00;
  ER = 0x00;
  FR = 0x00;
  HR = 0x00;
  LR = 0x00;
  cc = 0x00;

  rom_bank_num = 1;
  ime = true;
}

uint8_t get_byte(uint16_t addr) {
  if (addr < 0x0100) {
    return *(bootstrap + addr);
  } else if (addr >= 0x0100 && addr < 0x4000) {
    return *(rom_bank00 + addr);
  } else if (addr >= 0x4000 && addr < 0x8000) {
    return mbc_get_rom(addr);
  } else if (addr >= 0x8000 && addr < 0xA000) {
    return *(VRAM + addr - 0x8000);
  } else if (addr >= 0xA000 && addr < 0xC000) {
    return *(CRAM + addr - 0xA000);
  } else if (addr >= 0xC000 && addr < 0xE000) {
    return *(WRAM + addr - 0xC000);
  } else if (addr >= 0xE000 && addr < 0xFE00) {  // Mirror of C000~DDFF

  } else if (addr >= 0xFE00 && addr < 0xFEA0) {  // Sprite attribute table (OAM)
    return *(oam + addr - 0xFE00);
  } else if (addr >= 0xFEA0 && addr < 0xFF00) {  // Not Usable

  } else if (addr >= 0xFF00 && addr < 0xFF80) {  // I/O Register
    return *(io + addr - 0xFF00);
  } else if (addr >= 0xFF80 && addr < 0xFFFF) {  // High RAM stack
    return *(hram + addr - 0xFF80);
  } else if (addr == 0xFFFF) { // Interrupt Enable register(IE)
    return ie;
  }

  gpio_put(25, HIGH);
  while (1) {
    Serial.print("pc:");
    Serial.print(pc, HEX);
    Serial.print(" code:");
    Serial.println(code, HEX);
    Serial.print("rom_bank_num:");
    Serial.println(rom_bank_num, HEX);
    Serial.print("get_byte() at an unassigned address :");
    Serial.println(addr, HEX);
    chk_init_regs();
    dump_tilemap();
    delay(10000);
  }
  return 0x00;
}

void put_byte(uint16_t addr, uint8_t data) {
  if (addr >= 0xFF00 && addr < 0xFF80) {  // I/O Register
    *(io + addr - 0xFF00) = data;
  } else if (addr >= 0x2000 && addr < 0x4000) { // this area is not for write but used for change rom bank
    switch_rom_bank(data & 0b00011111);
  } else if (addr >= 0x8000 && addr < 0xA000) {
    *(VRAM + addr - 0x8000) = data;
  } else if (addr >= 0xA000 && addr < 0xC000) {
    *(CRAM + addr - 0xA000) = data;
  } else if (addr >= 0xC000 && addr < 0xE000) {
    *(WRAM + addr - 0xC000) = data;
  } else if (addr >= 0xE000 && addr < 0xFE00) {  // Mirror of C000~DDFF

  } else if (addr >= 0xFE00 && addr < 0xFEA0) {  // Sprite attribute table (OAM)
    *(oam + addr - 0xFE00) = data;
  } else if (addr >= 0xFEA0 && addr < 0xFF00) {  // Not Usable

  } else if (addr >= 0xFF80 && addr < 0xFFFF) {  // High RAM
    *(hram + addr - 0xFF80) = data;
  } else if (addr == 0xFFFF) { // Interrupt Enable register(IE)
  }
}
// 初期化
void setup() {
  Serial.begin(115200);
  ini();
  ini_LCD();
  Get_cartridge_Type();
  pinMode(25, OUTPUT);
}

void loop() {
  while (cc < 70224) {
    execute();
    ppu();
  }
  cc = 0;
}
