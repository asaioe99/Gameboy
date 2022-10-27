void p_flag_update() {

  uint8_t *t_FF40 = io + 0x40;
  uint8_t *t_FF41 = io + 0x41;
  uint8_t *t_FF44 = io + 0x44;

  if ((*t_FF40 & 0b10000000) == 0b00000000) {
    *t_FF44 = 0x00;
    scaline_counter = 456;
    *t_FF41 &= 0b11111100;
    *t_FF41 |= 0b00000001;
    return;
  }
  //現時点のmodeが何であるか取得し更新
  *t_FF41 &= 0b11111100;              // mode0 デフォ
  if (*t_FF44 >= 144) {                  // mode1
    *t_FF41 |= 0b00000001;
  } else {
    if (scaline_counter >= 376) {        // mode2
      *t_FF41 |= 0b00000010;
    } else if (scaline_counter >= 204) { // mode3
      *t_FF41 |= 0b00000011;
    }
  }

  if (*t_FF44 == *(io + 0x45)) {
    *t_FF41 = *t_FF41 | 0b00000100;
  } else {
    *t_FF41 = *t_FF41 & 0b11111011;
  }
}

void ppu() {
  // flag等 アップデート
  p_flag_update();

  // LCD コントロールレジスタのMSBによりLCD有効でない場合は動作をスルー
  if (*(io + 0x40) & 0b10000000) {
    scaline_counter -= cc_dec;
  } else {
    return;
  }

  uint8_t *t_FF44 = io + 0x44;
  //　スキャンラインの右端に達したらlyをインクリメントしてスキャンラインナンバーを下に進める
  if (scaline_counter <= 0) {
    *t_FF44 = *t_FF44 + 1;
    scaline_counter += 456;
    // 非ブランクゾーンの場合の処理
    if (*t_FF44 < 144) {
      display_scanline();
    } else if (*t_FF44 == 144) {
      *(io + 0x0F) = *(io + 0x0F) | 0b00010000; // v-blank割り込み発生
    } else {
      *t_FF44 = 0;
    }
  }
}

// bg&wnd フェッチ
void display_scanline() {

  uint8_t *t_FF40 = io + 0x40;
  uint16_t SCY = (uint16_t) * (io + 0x42); // SCY BGの描画位置
  uint16_t SCX = (uint16_t) * (io + 0x43); // SCX
  uint16_t LY  = (uint16_t) * (io + 0x44);

  uint8_t  tile_l;
  uint8_t  tile_h;
  uint16_t tile_number;
  uint16_t *tmp = FIFO_bg_wnd;

  uint8_t  SCX_low_3bit = SCX & 0b00000111;
  uint16_t intile_y_offset  = ((LY + SCY) & 0b00000111) << 1;
  uint16_t outtile_y_offset = ((LY + SCY) & 0b11111000) << 2;
  uint16_t base_tile_number;

  if (*t_FF40 & 0b00001000) {
    base_tile_number = 0x9C00 + (SCX & 0b00011111) + outtile_y_offset; // LYに対応したタイルデータ
  } else {
    base_tile_number = 0x9800 + (SCX & 0b00011111) + outtile_y_offset;
  }

  for (uint16_t i = 0; i < 21 ; i++) {
    tile_number = get_byte(base_tile_number + i); // numberだけどアドレス

    if (*t_FF40 & 0b00010000) {
      tile_l = get_byte(0x8000 + (tile_number << 4) + intile_y_offset);
      tile_h = get_byte(0x8001 + (tile_number << 4) + intile_y_offset);
    } else {
      tile_l = get_byte(0x8800 + ((int16_t)tile_number << 4) + (int16_t)intile_y_offset);
      tile_h = get_byte(0x8801 + ((int16_t)tile_number << 4) + (int16_t)intile_y_offset);
    }
    
    uint8_t start_bit = 0;
    uint8_t end_bit   = 8;
    
    if (i == 0) {
      start_bit = SCX_low_3bit;
    } else if (i == 20) {
      end_bit = SCX_low_3bit;
    }

    for (int n = start_bit; n < end_bit; n++) {
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
  drowBitMap(LY);
  return;
}
