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
  uint16_t LY  = (uint16_t) * (io + 0x44);
  uint16_t SCY = (uint16_t) * (io + 0x42); // SCY BGの描画位置
  uint16_t SCX = (uint16_t) * (io + 0x43); // SCX
  uint16_t WY  = (uint16_t) * (io + 0x4A); // WY windowの描画位置
  uint16_t WX  = (uint16_t) * (io + 0x4B) - 7; // WX

  uint16_t *pix_mixture = FIFO_bg_wnd + LY * 160;
  uint8_t tile_number;
  uint8_t win_pix_C_number;
  uint8_t bg_pix_C_number;

  // LY is given from 0xFF44
  for (uint16_t LX = 0; LX < 160; LX++) {
    if (*t_FF40 & 0b00000001) { // both BG and window enable
      // get back ground pixel color number of given LY and LX, SCY, SCX
      tile_number = get_tile_number(((LY + SCY) >> 3) & 0x1F, ((LX + SCX) >> 3) & 0x1F);
      bg_pix_C_number = get_pix_C_number(tile_number, LY + SCY, LX + SCX);

      // get window pixel color number of given LY and LX, WY, WX
      win_pix_C_number = 0;
      if (*t_FF40 & 0b00100000) { // window enable
        if (SCY + LY >= WY) {
          if (SCX + LX >= WX) {
            tile_number = get_tile_number((SCY + LY - WY) & 0x1F, (SCX + LX - WX) & 0x1F);
            win_pix_C_number = get_pix_C_number(tile_number, SCY + LY - WY, SCX + LX - WX);
          }
        }
      }
    }
    // sprite
    uint8_t* obj;
    uint8_t y_pos;
    uint8_t x_pos;
    uint8_t sp_tile_num;
    uint8_t sp_atr;
    if (*t_FF40 & 0b00000010) { //sprite enable
      for (uint8_t i = 0; i < 40; i++) {
        obj = (oam + (i << 2)); // get object base address
        y_pos = *obj;
        x_pos = *(obj + 1);
        sp_tile_num = *(obj + 2);
        sp_atr = *(obj + 3);
        if (*t_FF40 & 0b00000100) { // sprite size 8x16

        } else {                    // sprite size 8x8

          // Y-position
          if (y_pos - 16 <= LY && y_pos - 8 > LY) {
            // X-position
            if (x_pos - 8 <= LX && x_pos > LX) {

            }
          }
        }
      }
    }



    // overray window above back ground
    if (win_pix_C_number) {
      *pix_mixture = bw_color_number2bit(win_pix_C_number);
    } else {
      *pix_mixture = bw_color_number2bit(bg_pix_C_number);
    }
    pix_mixture++;
  }
}

uint8_t get_tile_number(uint16_t offset_y, uint16_t offset_x) {
  if (*(io + 0x40) & 0b00001000) { // bg offset address select
    return get_byte(0x9c00 + (offset_y << 5) + offset_x);
  } else {
    return get_byte(0x9800 + (offset_y << 5) + offset_x);
  }
}

uint8_t get_pix_C_number(uint8_t tile_number, uint8_t offset_y, uint8_t offset_x) {
  uint8_t h8_tile_l;
  uint8_t h8_tile_h;
  if (*(io + 0x40) & 0b00010000) { // get pixel color number
    h8_tile_l = get_byte(0x8000 + (tile_number << 4) + ((offset_y & 0x07) << 1));
    h8_tile_h = get_byte(0x8001 + (tile_number << 4) + ((offset_y & 0x07) << 1));
  } else {
    h8_tile_l = get_byte(0x9000 + ((int16_t)tile_number << 4) + ((offset_y & 0x07) << 1));
    h8_tile_h = get_byte(0x9001 + ((int16_t)tile_number << 4) + ((offset_y & 0x07) << 1));
  } // base addr is 8800?
  return ((h8_tile_l & (1 << (7 - offset_x & 0x07))) >> (7 - offset_x & 0x07)) + ((h8_tile_h & (1 << (7 - offset_x & 0x07))) >> (7 - offset_x & 0x07));
}

uint16_t bw_color_number2bit(uint8_t color_number) {
  //Serial.println(color_number);
  uint8_t color;
  switch (color_number) {
    case 0: // color number 0
      color = (*(io + 0x47) & 0x03) >> 0;
      break;
    case 1: // color number 1
      color = (*(io + 0x47) & 0x0C) >> 2;
      break;
    case 2: // color number 2
      color = (*(io + 0x47) & 0x30) >> 4;
      break;
    case 3: // color number 3
      color = (*(io + 0x47) & 0xC0) >> 6;
      break;
  }
  //Serial.println(color);
  switch (color) {
    case 0: // White
      return ~0b1111011110011110;
    case 1: // Light gray
      return ~0b1010010100010100;
    case 2: // Dark gray
      return ~0b0101001010001010;
    case 3: // Black
      return ~0b0000000000000000;
  }
}
