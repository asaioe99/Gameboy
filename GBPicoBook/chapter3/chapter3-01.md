# bootstarapの仕組みについて
## ちょっとした問題点
本書では、段階的にエミュレータを作成するためにも、bootstrapの動作を再現するところから作業を始めたいと考えています。しかしながら、様々な理由により直接これをバイナリ形式として紹介することは良くないそうですので、詳細については各自調べてみてください。

本章では、代わりにc言語の基本事項を確認したいと思います。

!!! tip 本章での目標：c言語の基本を理解する

    - bit操作に慣れる
    - 何かしらを感じ取る

### 市民ふれあいc言語講座
以下のソースコードは、c言語の学習のためのものであり、何らかのアーキテクチャとは一切関係のない、無作為に作成されたものです。

```c
// (0x95-0xa7): Decompress and copy the data to VRAM.
void DecompressAndCopy(uint8_t data, uint16_t *addr) {
  uint8_t mask0 = 0b00000001;
  uint8_t mask1 = 0b00000011;
  uint8_t res = 0;
  for (int i = 0; i < 4; ++i) {
    res |= (data & mask0) ? mask1 : 0;
    mask0 <<= 1;
    mask1 <<= 2;
  }
  *addr = res;
  *(addr+2) = res;
}

void main() {
  // VRAMのアドレスを0クリアして初期化
  for (uint16_t addr = 0x9FFF; addr >= 0x8000; --addr) {
    write_byte(addr, 0x00);
  }

  // サウンド関係の初期設定　本書では扱わないので理解しなくても良いです
  write_byte(0xFF26, 0x80); 
  write_byte(0xFF11, 0x80);
  write_byte(0xFF12, 0xF3);
  write_byte(0xFF25, 0xF3);
  write_byte(0xFF24, 0x77);

  // BB3 (0x1d-0x24): Init the color palette.
  mem[0xff47] = 0xfc; // Set up BG and window colour palette.

  // BB4 (0x27-0x32): Load the logo.
  u8 *vram = 0x8010;
  for (u8 *logo = 0x0104; logo < 0x0134; ++logo) {
    u8 data = *logo;
    DecompressAndCopy(data, vram);
    vram += 4;
    DecompressAndCopy(data >> 4, vram);
    vram += 4;
  }

  // (0x34-3e): Load the registered trademark.
  u8 *vram = 0x80d0;
  for (u8 *logo = 0xd8; logo < 0xe0; ++logo) {
    *vram = *logo;
    vram += 2;
  }

  // (0x40-0x53): Selecting the right tiles.
  int a = 25;
  u8 *mem = 0x9910;
  *mem = a;
  mem = 0x992f;
  for (int j = 0; j < 2; ++j) {
    for (int i = 12; i > 0; --i) {
      a--;
      *mem = a;
      mem--;
    }
    mem = 0x990f;
  }

  // (0x55-0x5d): Display init.
  uint16_t* SCY = 0xFF42; // グラフィック描画に利用されるレジスタのアドレス（16bit）
  *SCY = 100;
  uint16_t *LCDC = 0xFF40; // グラフィック描画に利用されるレジスタのアドレス（16bit）
  *LCDC = 0x91 // 0b10010001であるが、各bitは描画に必要な設定と対応している

  // (0x5f-0x93): Showtime.
  int d = 100;
  int h = 0;
  for (int d = 100; d > 0; --d) {
    // Wait for 2 frames.
    for (int e = 2; i > 0; --i) {
      for (int c = 12; j > 0; --j) {
        while (vline() != 144) {}
      }
    }
    h++;
    u16 *sound_f_low;
    u16 *sound_f_high;
    sound_f_low = 0xFF13;
    sound_f_high = 0xFF14;
    e = 0x83;
    if (h == 98) {
      goto BB80;
    }
    e = 0xc1;
    if (h != 100) {
      goto BB86;
    }
    BB80:
    *sound_f_high = e;
    *sound_f_high = 0x87;
    BB86:
    *scroll_y -= 1;
  }

  // Let the logo rest a short time.
  for (int d = 32; d > 0; --d) {
    for (int e = 2; i > 0; --i) {
      for (int c = 12; j > 0; --j) {
        while (vline() != 144) {}
      }
    }
  }

  // (0xe0-0xfe) Checking the logo.
  *cartridge_logo = 0x104
  *boot_logo = 0xa8
  for (int i = 0; i < 48; ++i) {
    if (cartridge_logo[i] != boot_logo[i]) {
      while (true) {};  // Loop forever.
    }
  }

  *cartridge_header = 0x134
  sum = 0x19;
  for (int i = 0; i =< 25; ++i) {
    sum += cartridge_header[i];
  }

  if (sum != 0) {
    while (true) {}; // Loop forever.
  }

  unload_boot_rom();

  return;
}
```

