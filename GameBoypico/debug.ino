// bg&wnd フェッチ
void display_tile(uint8_t start) {

  uint8_t SCY = *(io + 0x42) << 3; // SCY BGの描画位置
  uint8_t SCX = *(io + 0x43) << 3; // SCX

  uint16_t base_tile_number;
  uint16_t tile_number;
  uint8_t tile_l;
  uint8_t tile_h;

  for (uint16_t LY = 0; LY < 144 ; LY++) {
    for (int16_t i = 0; i < 20; i++) {
      base_tile_number = (LY & 0b11111000) << 2; // LY（scanline number）に対応した先頭のtile number
      tile_number = get_byte(0x9800 + base_tile_number + i); // LYに対応したタイルデータ
      tile_l = get_byte(0x8000 + (tile_number << 4) + (LY & 0b00000111) * 2);
      tile_h = get_byte(0x8001 + (tile_number << 4) + (LY & 0b00000111) * 2);

      uint8_t *tmp_0 = FIFO_bg_wnd + (i << 4);
      uint8_t *tmp_1 = tmp_0 + 1;
      
      for (int n = 0; n < 8; n++) {
        tmp_0 += 2;
        tmp_1 += 2;

        *tmp_0 = 0b00000000;
        *tmp_1 = 0b00000000;

        if (tile_l & (0b10000000 >> n)) {
          *tmp_0 = 0b00111001;
          *tmp_1 = 0b11000111;
        }
        if (tile_h & (0b10000000 >> n)) {
          *tmp_0 |= 0b11000110;
          *tmp_1 |= 0b00011000;
        }
      }
    }
    drowBitMap(LY);
  }
  return;
}
