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

void mode_0() {
  return;
}

void mode_1() {
  return;
}

void mode_2() {
  return;
}

// bg&wnd フェッチ
void mode_3() {
  //display_scanline();
  //display_tile(0);

  return;
}

void mode_sw() {
  switch (*(io + 0x41) & 0b00000011) {
    case 0b00000000: // mode 0
      mode_0();
      break;
    case 0b00000001: // mode 1
      mode_1();
      break;
    case 0b00000010: // mode 2
      mode_2();
      break;
    case 0b00000011: // mode 3
      mode_3();
      break;
  }
  return;
}

void ppu() {
  // flag等 アップデート
  p_flag_update();

  if (*(io + 0x40) & 0b00001000 == 0b00001000) {
    gpio_put(25, LOW);
  } else {
    gpio_put(25, HIGH);
  }

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
/*
      gpio_put(25, HIGH);
      delay(100);
      gpio_put(25, LOW);
*/
// bg&wnd フェッチ
void display_scanline() {

  uint8_t *t_FF40 = io + 0x40;
  uint16_t SCY = (uint16_t)*(io + 0x42); // SCY BGの描画位置
  uint16_t SCX = (uint16_t)*(io + 0x43); // SCX
  uint16_t LY  = (uint16_t)*(io + 0x44);

  uint16_t base_tile_number;
  uint16_t tile_number;
  uint8_t tile_l;
  uint8_t tile_h;

  for (uint16_t i = 0; i < 20; i++) {
    //base_tile_number = (LY & 0b11111000) << 2 + (SCY & 0b11111000) << 2; // LY（scanline number）に対応した先頭のtile number
    base_tile_number = ((LY + SCY) & 0b11111000) << 2;

    if (*t_FF40 & 0b00001000) {
      tile_number = get_byte(0x9C00 + base_tile_number + i); // LYに対応したタイルデータ
    } else {
      //delay(50);
      //gpio_put(25, LOW);
      tile_number = get_byte(0x9800 + base_tile_number + i); // LYに対応したタイルデータ
    }
    if (*t_FF40 & 0b00010000) {
      //tile_l = get_byte(0x8000 + (tile_number << 4) + (LY & 0b00000111) * 2 + (SCY & 0b00000111) * 2);
      //tile_h = get_byte(0x8001 + (tile_number << 4) + (LY & 0b00000111) * 2 + (SCY & 0b00000111) * 2);
      tile_l = get_byte(0x8000 + (tile_number << 4) + ((LY + SCY) & 0b00000111) << 1);
      tile_h = get_byte(0x8001 + (tile_number << 4) + ((LY + SCY) & 0b00000111) << 1);
    } else {
      //tile_l = get_byte(0x8800 + ((int8_t)tile_number << 4) + (LY & 0b00000111) * 2 + (SCY & 0b00000111) * 2));
      //tile_h = get_byte(0x8801 + ((int8_t)tile_number << 4) + (LY & 0b00000111) * 2 + (SCY & 0b00000111) * 2));
      tile_l = get_byte(0x8800 + ((int8_t)tile_number << 4) + ((LY + SCY) & 0b00000111) << 1);
      tile_h = get_byte(0x8801 + ((int8_t)tile_number << 4) + ((LY + SCY) & 0b00000111) << 1);
    }

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
  return;
}
