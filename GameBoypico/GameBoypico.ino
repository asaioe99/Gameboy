#include <SPI.h>
#include <stdio.h>
//#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789


#define HBYTE(u) ((u >> 8) & 0xFF)
#define LBYTE(u) (u & 0xFF)
#define HL(H, L) ((uint16_t)H << 8) + L

#define TFT_DC 8
#define TFT_RST 12
#define TFT_MOSI 11
#define TFT_SCLK 10
#define TFT_CS 9

//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

SPISettings lcd_SPISettings = SPISettings(80000000, MSBFIRST, SPI_MODE3);  //SPI_CLOCK_DIV2よりも速くした

uint8_t SPIBuf[360];       // SPI転送用バッファ
uint8_t FIFO_bg_wnd[240];  //16pix毎

// bootstrap（実物のため、そのままは掲載不可）
const uint8_t bootstrap[] = {
  // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
  0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,  // 0
  0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,  // 1
  0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,  // 2
  0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,  // 3
  0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,  // 4
  0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,  // 5
  0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,  // 6
  0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,  // 7
  0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xE2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,  // 8
  0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,  // 9
  0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,  // a
  0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,  // b
  0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,  // c
  0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C,  // d
  0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,  // e
  0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50,  // f
};

uint16_t pc;
uint16_t sp;
uint8_t AR;
uint8_t BR;
uint8_t CR;
uint8_t DR;
uint8_t ER;
uint8_t FR;
uint8_t HR;
uint8_t LR;
uint32_t cc;

uint8_t oam[0xa0];
uint8_t io[0x80];
uint8_t hram[0x7F];
uint8_t ie;
uint8_t ime;
uint8_t cc_dec;
uint8_t code;
int16_t scaline_counter;

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

// GPIO割り当て（レジスタにより）
void ini() {

  // レジスタ設定
  pc = 0x0000;
  sp = 0x0000;
  AR = 0x00;
  FR = 0x00;
  BR = 0x00;
  CR = 0x00;
  DR = 0x00;
  ER = 0x00;
  HR = 0x00;
  LR = 0x00;
  cc = 0x00;

  // フラグ初期化
  put_byte(0xFF05, 0x00);
  put_byte(0xFF06, 0x00);
  put_byte(0xFF07, 0x00);
  put_byte(0xFF10, 0x80);
  put_byte(0xFF11, 0xBF);
  put_byte(0xFF12, 0xF3);
  put_byte(0xFF14, 0xBF);
  put_byte(0xFF16, 0x3F);
  put_byte(0xFF17, 0x00);
  put_byte(0xFF19, 0xBF);
  put_byte(0xFF1A, 0x7F);
  put_byte(0xFF1B, 0xFF);
  put_byte(0xFF1C, 0x9F);
  put_byte(0xFF1E, 0xBF);
  put_byte(0xFF20, 0xFF);
  put_byte(0xFF21, 0x00);
  put_byte(0xFF22, 0x00);
  put_byte(0xFF23, 0xBF);
  put_byte(0xFF24, 0x77);
  put_byte(0xFF25, 0xF3);
  put_byte(0xFF26, 0xF1);
  put_byte(0xFF40, 0x91);
  put_byte(0xFF42, 0x00);
  put_byte(0xFF43, 0x00);
  put_byte(0xFF45, 0x00);
  put_byte(0xFF47, 0xFC);
  put_byte(0xFF48, 0xFF);
  put_byte(0xFF49, 0xFF);
  put_byte(0xFF4A, 0x00);
  put_byte(0xFF4B, 0x00);
  put_byte(0xFFFF, 0x00);
}


uint8_t get_byte(uint16_t addr) {
  if (addr <= 0x0100) {
    return bootstrap[addr];
  }
  return 0x0000;
}

void put_byte(uint16_t addr, uint8_t data) {
}

// 初期化
void setup() {
  Serial.begin(115200);
  Serial.println("Hello World!");
  ini();

  ini_LCD();
  //delay(100);
  //load_rom_header();  // romheader読み込み
  //display_rom_header();
}

void loop() {

  //delayMicroseconds(1000000);
  //Serial.println("Hello7World!");
  //dump_rom_bank(1);
  //enable_ram();
  //write_ram_bank(0);
  //dump_ram_bank(0);
  //disable_ram();

  //while (cc < 70224) {
  execute();
  //Serial.println("Hello World!");
  //  ppu();
  //}
  cc = 0;
}