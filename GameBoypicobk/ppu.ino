void p_flag_update() {

  uint8_t *t_FF40 = io + 0x40;
  uint8_t *t_FF41 = io + 0x41;
  uint8_t *t_FF44 = io + 0x44; //LY

  if (!(*t_FF40 & 0b10000000)) { //reset ppu
    *t_FF44 = 0x00;
    scaline_counter = 456;
    //*t_FF41 &= 0b11111100;
    //*t_FF41 |= 0b00000001;
    *t_FF41 = 0b10000000;
    return;
  }

  //現時点のmodeが何であるか取得し更新
  *t_FF41 &= 0b11111100;                 // mode0 H-Blank
  if (*t_FF44 >  144) {                  // mode1 V-Blank
    *t_FF41 |= 0b00000001;
  } else {
    if (scaline_counter >= 376 && scaline_counter <= 456) {       // mode2 OAM Scan
      *t_FF41 |= 0b00000010;
    } else if (scaline_counter >= 204 && scaline_counter < 376) { // mode3 Drawing
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
  if (!(*(io + 0x40) & 0b10000000)) {
    return;
  }

  uint8_t *t_FF44 = io + 0x44;
  //　スキャンラインの右端に達したらlyをインクリメントしてスキャンラインナンバーを下に進める
  if (scaline_counter < 0) {
    // 非ブランクゾーンの場合の処理
    if (*t_FF44 < 144) {
      display_scanline();
    } else if (*t_FF44 == 144) {
      drowBitMap();
      //gpio_put(25, HIGH);
      //dump_tilemap();
      //tile_display(0x0d, 1, 1);
      *(io + 0x0F) |= 0b00000001; // v-blank割り込み発生
    } else if (*t_FF44 > 153) {
      *t_FF44 = 0;
    }
    *t_FF44 += 1;
    scaline_counter += 456;
  }
}

// bg&wnd フェッチ
void display_scanline() {

  uint8_t *t_FF40 = io + 0x40;
  uint16_t SCY = (uint16_t) * (io + 0x42); // SCY BGの描画位置
  uint16_t SCX = (uint16_t) * (io + 0x43); // SCX
  uint16_t WY  = (uint16_t) * (io + 0x4A); // WY windowの描画位置
  uint16_t WX  = (uint16_t) * (io + 0x4B); // WX - 7
  uint16_t LY  = (uint16_t) * (io + 0x44);

  uint8_t  tile_l;
  uint8_t  tile_h;
  uint8_t  tile_l_w;
  uint8_t  tile_h_w;
  uint16_t tile_number;
  uint16_t *tmp = FIFO_bg_wnd + LY * 160;

  uint8_t  in_tile_x_offset = SCX & 0b00000111;
  uint16_t out_tile_x_offset  = ((SCX & 0b11111000) >> 3) & 0b00011111;

  uint16_t in_tile_y_offset  = ((LY + SCY) & 0b00000111) << 1;
  uint16_t out_tile_y_offset = (((LY + SCY) % 256) & 0b11111000) << 2;
  uint16_t base_tile_number;
  uint16_t tile_number_w;

  // SCY循環対応？
  if (*t_FF40 & 0b00001000) {
    base_tile_number = 0x9C00 + out_tile_y_offset; // LYに対応したタイルデータ
  } else {
    base_tile_number = 0x9800 + out_tile_y_offset;
  }

  for (uint16_t i = 0; i < 21 ; i++) {
    tile_number = get_byte(base_tile_number + ((i + out_tile_x_offset) % 32)); // numberだけどアドレス

    // window overlay
    tile_l_w = 0x00;
    tile_h_w = 0x00;
    if (*t_FF40 & 0b00100000) {
      if ((LY >> 3) >= WY) {
        if ((base_tile_number + i) >= (WX - 7)) {

          if (*t_FF40 & 0b01000000) {
            uint16_t tile_number_w = 0x9C00 + ((base_tile_number + i - WX + 7) & 0b00011111) + ((WY - (LY >> 3)) << 5); // LYに対応したタイルデータ
          } else {
            uint16_t tile_number_w = 0x9800 + ((base_tile_number + i - WX + 7) & 0b00011111) + ((WY - (LY >> 3)) << 5);
          }

          if (*t_FF40 & 0b00010000) {
            tile_l_w = get_byte(0x8000 + (tile_number_w << 4) + in_tile_y_offset);
            tile_h_w = get_byte(0x8001 + (tile_number_w << 4) + in_tile_y_offset);
          } else {
            tile_l_w = get_byte(0x9000 + ((int16_t)tile_number_w << 4) + (int16_t)in_tile_y_offset);
            tile_h_w = get_byte(0x9001 + ((int16_t)tile_number_w << 4) + (int16_t)in_tile_y_offset);
          } // base addr is 8800?
        }
      }
    }

    if (*t_FF40 & 0b00010000) {
      tile_l = get_byte(0x8000 + (tile_number << 4) + in_tile_y_offset);
      tile_h = get_byte(0x8001 + (tile_number << 4) + in_tile_y_offset);
    } else {
      tile_l = get_byte(0x9000 + ((int16_t)tile_number << 4) + (int16_t)in_tile_y_offset);
      tile_h = get_byte(0x9001 + ((int16_t)tile_number << 4) + (int16_t)in_tile_y_offset);
    } // base addr is 8800?

    uint8_t start_bit = 0;
    uint8_t end_bit   = 8;

    if (i == 0) {
      start_bit = in_tile_x_offset ;
    } else if (i == 20) {
      end_bit = in_tile_x_offset ;
    }

    // パレット処理　まだその場しのぎ
    for (uint8_t n = start_bit; n < end_bit; n++) {
      *tmp = 0b0000000000000000;
      uint8_t tile_mask = 0b10000000 >> n;

      if ((tile_l_w | tile_h_w ) & tile_mask) { // overlay
        if (tile_l_w & tile_mask) {
          *tmp  = 0b0011100111000111;
        }
        if (tile_h_w & tile_mask) {
          *tmp |= 0b1100011000011000;
        }
      } else {
        if (tile_l & tile_mask) {
          *tmp  = 0b0011100111000111;
        }
        if (tile_h & tile_mask) {
          *tmp |= 0b1100011000011000;
        }
      }
      tmp++;
    }
  }
  return;
}
