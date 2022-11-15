// bg&wnd フェッチ

void tile_display(uint8_t tile_number, uint8_t x, uint8_t y) {

  for (uint8_t i = 0; i < 8; i++) {
    uint8_t color_l = mmu_read(0x8000 + tile_number * 16 + i * 2);
    uint8_t color_h = mmu_read(0x8001 + tile_number * 16 + i * 2);

    for (uint8_t n = 0; n < 8; n++) {
      uint16_t data = 0b0000000000000000;
      if (color_l & (0b10000000 >> n)) {
        data  = 0b0011100111000111;
      }
      if (color_h & (0b10000000 >> n)) {
        data |= 0b1100011000011000;
      }

      SendCommand4(0x2A, 0, 40 + n + x * 8, 0, 40 + n + x * 8); // Colmun Address
      SendCommand4(0x2B, 0, 40 + i + y * 8, 0, 40 + i + y * 8); // Row Address

      gpio_put(CS, LOW);
      gpio_put(DC, LOW); // Command mode
      SPI_transfer(0x2C);
      gpio_put(DC, HIGH); // Data mode

      for (uint8_t bit = 0x80; bit; bit >>= 1) {
        gpio_put(MOSI, ((uint8_t)(data & 0x00FF)) & bit);
        gpio_put(CLK, HIGH);
        gpio_put(CLK, LOW);
      }
      for (uint8_t bit = 0x80; bit; bit >>= 1) {
        gpio_put(MOSI, ((uint8_t)(data >> 8)) & bit);
        gpio_put(CLK, HIGH);
        gpio_put(CLK, LOW);
      }
    }
  }
  gpio_put(CS, HIGH);
}

void display_tile() {

  uint8_t  tile_l;
  uint8_t  tile_h;
  uint16_t tile_number;
  uint16_t *tmp = FIFO_bg_wnd;
  uint16_t SCX = 0;
  uint16_t SCY = 0;

  for (uint8_t LY = 0; LY < 144; LY++) {

    uint8_t  SCX_low_3bit = SCX & 0b00000111;
    uint16_t in_tile_y_offset  = (LY & 0b00000111) << 1;
    uint16_t out_tile_y_offset = (LY & 0b11111000) << 2;
    uint16_t base_tile_number;

    base_tile_number = 0x9800 + (SCX & 0b00011111) + out_tile_y_offset;

    for (uint16_t i = 0; i < 21 ; i++) {
      tile_number = mmu_read(base_tile_number + i); // numberだけどアドレス

      tile_l = mmu_read(0x8000 + (tile_number << 4) + in_tile_y_offset);
      tile_h = mmu_read(0x8001 + (tile_number << 4) + in_tile_y_offset);

      uint8_t start_bit = 0;
      uint8_t end_bit   = 8;

      if (i == 0) {
        start_bit = SCX_low_3bit;
      } else if (i == 20) {
        end_bit = SCX_low_3bit;
      }

      for (uint8_t n = start_bit; n < end_bit; n++) {
        *tmp = 0b0000000000000000;
        if (tile_l & (0b10000000 >> n)) {
          *tmp  = 0b0011100111000111;
        }
        if (tile_h & (0b10000000 >> n)) {
          *tmp |= 0b1100011000011000;
        }
        tmp++;
      }
    }
  }
  return;
}

void dump_tilemap() {
  for (int n = 0; n < 16 * 16; n++) {
    tile_display(n, n % 16, n / 16);
  }
}
