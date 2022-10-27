// bg&wnd フェッチ

void display_tile() {

  uint16_t LY;

  uint16_t tile_number;
  uint8_t tile_l;
  uint8_t tile_h;

  uint16_t *tmp = FIFO_bg_wnd;

  for (LY = 0; LY < 144; LY++) {

    uint8_t SCX_bit210 = 0;
    uint16_t SCX_bit76543 = 0;
    uint16_t intile_y_offset = (LY & 0b00000111) << 1;
    uint16_t outtile_y_offset = (LY & 0b11111000) << 2;
    uint16_t base_tile_number = SCX_bit76543 + outtile_y_offset;


    //base_tile_number = 0x9C00 + SCX_bit76543 + outtile_y_offset; // LYに対応したタイルデータ

    base_tile_number = 0x9900 + SCX_bit76543 + outtile_y_offset;

    for (uint16_t i = 0; i < 21 ; i++) {

      tile_number = get_byte(base_tile_number + i);

      tile_l = get_byte(0x8000 + (tile_number << 4) + intile_y_offset);
      tile_h = get_byte(0x8001 + (tile_number << 4) + intile_y_offset);

      //tile_l = get_byte(0x8800 + ((int16_t)tile_number << 4) + (int16_t)intile_y_offset);
      //tile_h = get_byte(0x8801 + ((int16_t)tile_number << 4) + (int16_t)intile_y_offset);


      if (i == 0) {
        for (int n = SCX_bit210; n < 8; n++) {
          *tmp = 0b0000000000000000;
          if (tile_l & (0b10000000 >> n)) {
            *tmp  = 0b0011100111000111;
          }
          if (tile_h & (0b10000000 >> n)) {
            *tmp |= 0b1100011000011000;
          }
          tmp++;
        }
      } else if (i == 20) {
        for (int n = 0; n < SCX_bit210; n++) {
          *tmp = 0b0000000000000000;
          if (tile_l & (0b10000000 >> n)) {
            *tmp =  0b0011100111000111;
          }
          if (tile_h & (0b10000000 >> n)) {
            *tmp |= 0b1100011000011000;
          }
          tmp++;
        }
      } else {
        for (int n = 0; n < 8; n++) {
          *tmp = 0b0000000000000000;
          if (tile_l & (0b10000000 >> n)) {
            *tmp =  0b0011100111000111;
            //*tmp =  0b0011100111111111;
          }
          if (tile_h & (0b10000000 >> n)) {
            *tmp |= 0b1100011000011000;
          }
          tmp++;
        }
      }
    }
    drowBitMap(LY);
  }
  return;
}
