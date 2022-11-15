#include "gbp_define.h"
#include "bootstrap.h"
#include "cpu_instrs.h"
//#include "btn_test.h"

uint8_t SPIBuf[360]; // SPI転送用バッファ
uint16_t FIFO_bg_wnd[160 * 144];

uint8_t VRAM[0x2000]; //
uint8_t OAM[0xa0];
uint8_t IO[0x80];
uint8_t HRAM[0x7F];
uint8_t WRAM[0x2000];
uint8_t CRAM[0x2000];

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

//for debug
char buf_b1[64];

uint32_t clock_cycle;
uint8_t tmp_clock = 0;
bool flag_halt = false;
bool ime = true;
bool int_vblank = false;
bool int_lcdc   = false;
bool int_timer  = false;
bool int_joypad = false;
bool boot = true;

int16_t scaline_counter;

uint8_t IE; // 0xFFFF
uint8_t IF; // 0xFF0F

uint8_t code;

uint8_t rom_bank_num;

void setup() {
  // initialization
  LCD_init();

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

  IO[0x00] = 0x3F; //JOYP
  IO[0x01] = 0x91; //SB
  IO[0x02] = 0x91; //SC
  IO[0x04] = 0xD3; //DIV
  IO[0x05] = 0x00; //TIMA
  IO[0x06] = 0x00; //TMA
  IO[0x07] = 0xF8; //TAC
  IO[0x0F] = 0x00; //IF
  IO[0x40] = 0x91; //LCDC
  IO[0x41] = 0x80; //STAT
  IO[0x42] = 0x00; //SCY
  IO[0x43] = 0x00; //SCX
  IO[0x44] = 0x00; //LY
  IO[0x45] = 0x00; //LYC
  IO[0x46] = 0xFF; //DMA
  IO[0x47] = 0xFC; //BGP
  IO[0x48] = 0xFF; //OBP0
  IO[0x49] = 0xFF; //OBP1
  IO[0x4A] = 0x00; //WY
  IO[0x4B] = 0x00; //WX

  rom_bank_num = 1;

  Serial.begin(115200);
  pinMode(25, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  uint32_t clock_cycle = 0;

  while (clock_cycle < 456 * 154 * 10) { // 6Hz
    clock_cycle += (uint32_t)cpu_step();
  }

  // LCD
  LCD_drowBitMap();
  //dump_tilemap();

  // joypad

  // time synchronization
  // loop until synchoronize
    //Serial.println("1");

}
