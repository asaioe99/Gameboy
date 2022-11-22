#include "gbp_define.h"
#include "bootstrap.h"
//#include "cpu_instrs.h"
//#include "instr_timing.h"
//#include "btn_test.h"
//#include "test2.h" //EI
#include "tr.h"

#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/interp.h"

#include "st7789_lcd.pio.h"

PIO pio = pio0;
uint sm = 0;
  uint8_t c = 0x00;

uint32_t FIFO_bg_wnd[160 * 144];

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
uint16_t timer_div = 0;
uint32_t time_before;
uint32_t tmp_clock = 0;
bool flag_halt = false;
bool ime = false;
bool int_vblank = false;
bool int_lcdc   = false;
bool int_timer  = false;
bool int_joypad = false;
bool boot = true;
bool LCD_f = false;

uint32_t scanline_counter;

uint8_t IE; // 0xFFFF
uint8_t IF; // 0xFF0F

uint8_t code;

uint8_t rom_bank_num;

void setup() {
  // initialization


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

  IE = 0x00;

  IO[0x00] = 0x3F; //JOYP
  IO[0x01] = 0x91; //SB
  IO[0x02] = 0x91; //SC
  IO[0x04] = 0xD3; //DIV
  IO[0x05] = 0x00; //TIMA
  IO[0x06] = 0x00; //TMA
  IO[0x07] = 0xF8; //TAC
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

  clock_cycle = 0;

  while (clock_cycle < 456 * 154) { // 60Hz
    clock_cycle += cpu_step();
  }

  // LCD
  LCD_f = true;
  //dump_tilemap();

  // joypad

  // time synchronization
  // loop until synchoronize
  //Serial.println("1");

}

void setup1() {
  LCD_init();
}

void loop1() {
  if (LCD_f) {
    LCD_drowBitMap();
    LCD_f = false;
  }
}
