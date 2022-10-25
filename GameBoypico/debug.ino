// bg&wnd フェッチ
void display_tile(uint8_t start) {

  //uint8_t *t_FF40 = io + 0x40;
  uint8_t SCY = *(io + 0x42) << 3; // SCY BGの描画位置
  uint8_t SCX = *(io + 0x43) << 3; // SCX
  //uint8_t LY  = *(io + 0x44);
  uint8_t pic_h;
  uint8_t pic_l;

  static uint16_t bg_offset;
  static uint16_t window_offset;

  uint16_t base_tile_number;
  uint16_t tile_number;
  uint8_t tile_l;
  uint8_t tile_h;
  
  for (uint8_t LY = 0; LY < 144 ; LY++) {
    for (uint8_t i = 0; i < 20; i++) {
      base_tile_number = (LY >> 3) << 5; // LY（scanline number）に対応した先頭のtile number

      tile_number = get_byte(0x9900 + base_tile_number + i); // LYに対応したタイルデータ

      tile_l = get_byte(0x8000 + (tile_number << 4) + (LY & 0b00000111) * 2);
      tile_h = get_byte(0x8001 + (tile_number << 4) + (LY & 0b00000111) * 2);

      uint8_t tmp[12];
      uint16_t t =  i * 12;

      for (int n = 0; n < 4; n++) {

        uint8_t *tmp_0 = FIFO_bg_wnd + 0 + 3 * n + t;
        uint8_t *tmp_1 = FIFO_bg_wnd + 1 + 3 * n + t;
        uint8_t *tmp_2 = FIFO_bg_wnd + 2 + 3 * n + t;
        switch ((tile_l & (0b11000000 >> 2 * n)) >> (6 - 2 * n)) {
          case 0b00:
            *tmp_0 = 0b00000000;
            *tmp_1 = 0b00000000;
            *tmp_2 = 0b00000000;
            break;
          case 0b01:
            *tmp_0 = 0b00000000;
            *tmp_1 = 0b00000011;
            *tmp_2 = 0b00110011;
            break;
          case 0b10:
            *tmp_0 = 0b00110011;
            *tmp_1 = 0b00110000;
            *tmp_2 = 0b00000000;
            break;
          case 0b11:
            *tmp_0 = 0b00110011;
            *tmp_1 = 0b00110011;
            *tmp_2 = 0b00110011;
            break;
        }
        switch ((tile_h & (0b11000000 >> 2 * n)) >> (6 - 2 * n)) {
          //case 0b00:
          //*tmp_0 |= 0b00000000;
          //*tmp_1 |= 0b00000000;
          //*tmp_2 |= 0b00000000;
          //break;
          case 0b01:
            //*tmp_0 |= 0b00000000;
            *tmp_1 |= 0b00001100;
            *tmp_2 |= 0b11001100;
            break;
          case 0b10:
            *tmp_0 |= 0b11001100;
            *tmp_1 |= 0b11000000;
            //*tmp_2 |= 0b00000000;
            break;
          case 0b11:
            *tmp_0 |= 0b11001100;
            *tmp_1 |= 0b11001100;
            *tmp_2 |= 0b11001100;
            break;
        }
      }
    }
    drowBitMap(LY);
  }
  
  return;
}
