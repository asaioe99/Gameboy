#include "rom.h"
//#include "cpu_instrs.h"
//#include "instr_timing.h"
//#include "helloworld.h"
//#include "vblank.h"
//#include "any_test.h"
#include "btn_test.h"

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

bool start_flag = 1;

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
bool halted;

uint8_t mbcType;
uint8_t rom_bank_num;
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

  io[0x00] = 0x91; //JOYP
  io[0x01] = 0x91; //SB
  io[0x02] = 0x91; //SC

  io[0x04] = 0xD3; //DIV
  io[0x05] = 0x00; //TIMA
  io[0x06] = 0x00; //TMA
  io[0x07] = 0xF8; //TAC

  io[0x0F] = 0x00; //TAC

  io[0x40] = 0x91; //LCDC
  io[0x41] = 0x80; //STAT
  io[0x42] = 0x00; //SCY
  io[0x43] = 0x00; //SCX
  io[0x44] = 0x00; //LY
  io[0x45] = 0x00; //LYC
  io[0x46] = 0xFF; //DMA
  io[0x47] = 0xFC; //BGP
  io[0x48] = 0xFF; //OBP0
  io[0x49] = 0xFF; //OBP1
  io[0x4A] = 0x00; //WY
  io[0x4B] = 0x00; //WX

  rom_bank_num = 1;
  ime = 1; // 初期値0だとhaltで死ぬ
  ie = 0;
  halted = 0;

  scaline_counter = 456;
}

uint8_t get_byte(uint16_t addr) {
  if (addr < 0x0100 && (start_flag == 1)) {
    return *(bootstrap + addr);
  } else if (addr < 0x4000) {
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
    return *(WRAM + ((addr - 0xE000) & 0x1FFF)); // ?
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
    Serial.print("get_byte() from an unassigned address :");
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
    *(WRAM + ((addr - 0xE000) & 0x1FFF)) = data; //?
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
  delay(3000);
}

void loop() {
  while (cc < 70224) {
    // haltなら16クロックスルーするように書き換え
    if (!halted) {
      execute();
    } else {
      cc += 16;
    }
    ppu();
    // 割り込み処置 timer未実装のため、haltは正しく実装していない
    //gpio_put(25, ime);
    if (ime) { //IMEフラグが0の場合はそもそも考慮しない
      for (uint8_t i = 0; i < 5; i++) {
        bool int_flag = (*(io + 0x0F) & (0x00000001 << i)) > 0;
        bool int_enbl = (ie & (0x00000001 << i)) > 0;
        if (int_flag && int_enbl) {

          *(io + 0x0F) &= ~(0x00000001 << i);

          ime = 0; //割り込み無効化
          halted = 0; //不明

          switch (i) { //割り込みの優先順位はこの通り
            case 0:
              call_irpt(0x0040);
              break;
            case 1:
              call_irpt(0x0048);
              break;
            case 2:
              call_irpt(0x0050);
              break;
            case 3:
              call_irpt(0x0058); //Rustの実装では0x0080
              break;
            case 4:
              call_irpt(0x0060); //Rustの実装では0x0070
              break;
          }
        }
      }
    }
  }
  cc = 0;
}
