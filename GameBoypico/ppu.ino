void p_flag_update() {

  uint8_t *t_FF40 = io + 0x40;
  uint8_t *t_FF41 = io + 0x41;
  uint8_t *t_FF44 = io + 0x44;
  uint8_t status_LCD = *t_FF41;

  if ((*t_FF40 & 0x80) != 0x80) {
    *t_FF40 = 0x00;
    scaline_counter = 456;
    status_LCD &= 0b11111100;
    status_LCD = ~(status_LCD & (1 << 0));
    status_LCD = ~(status_LCD & (1 << 1));
    *t_FF41 = status_LCD;
    return;
  }
  //現時点のmodeが何であるか取得し更新
  status_LCD &= 0b11111100;              // mode0 デフォ
  if (*t_FF44 >= 144) {                  // mode1
    status_LCD |= 0b00000001;
  } else {
    if (scaline_counter >= 376) {        // mode2
      status_LCD |= 0b00000010;

    } else if (scaline_counter >= 204) { // mode3
      status_LCD |= 0b00000011;
    }
  }

  // put_byte(0xFF41, status_LCD);
  *t_FF41 = status_LCD;

  if (status_LCD == *(io + 0x45)) {
    status_LCD = status_LCD | 0b00000100;
  } else {
    status_LCD = status_LCD & 0b11111011;
  }
  *t_FF41 = status_LCD;
}

void mode_2() {
  return;
}

// bg&wnd フェッチ
void mode_3() {
  display_scanline();
  return;
}

void draw_scanline() {
  uint8_t *t_FF41 = io + 0x41;

  switch (*t_FF41 & 0b00000011) {
    case 0b00: // mode 0
      digitalWrite(25, LOW);
      break;
    case 0b01: // mode 1
      digitalWrite(25, HIGH);
      break;
    case 0b10: // mode 2
      digitalWrite(25, LOW);
      break;
    case 0b11: // mode 3
      digitalWrite(25, LOW);
      mode_3();
      break;
  }
  return;
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
    display_scanline();
  } else {
    return;
  }
  // 非ブランクゾーンの場合の処理
  if (*t_FF44 < 144) {
    draw_scanline();

  } else if (*t_FF44 > 153) {
    *t_FF44 = 0;

  } else if (*t_FF44 = 144) {
    *(io + 0x0F) = *(io + 0x0F) | 0b00010000; // v-blank割り込み発生
  }
}

// bg&wnd フェッチ
void display_scanline() {

  uint8_t *t_FF40 = io + 0x40;

  uint8_t SCY = *(io + 0x42) << 3; // SCY BGの描画位置
  uint8_t SCX = *(io + 0x43) << 3; // SCX


  uint8_t LY  = *(io + 0x44);
  uint8_t pic_h;
  uint8_t pic_l;

  static uint16_t bg_offset;
  static uint16_t window_offset;

  uint16_t base_tile_number;
  uint16_t tile_number;
  uint8_t tile_l;
  uint8_t tile_h;

  for (uint8_t i = 0; i < 20; i++) {
    base_tile_number = SCX + SCY * 32 + (LY >> 3) << 5; // LY（scanline number）に対応した先頭のtile number

    tile_number = get_byte(0x9800 + base_tile_number); // LYに対応したタイルデータ

    if (*t_FF40 & 0b00010000 == 0b00010000) {
      tile_l = get_byte(0x8000 + (tile_number << 4) + (LY & 0b00000111) * 2);
      tile_h = get_byte(0x8001 + (tile_number << 4) + (LY & 0b00000111) * 2);
    } else {
      tile_l = get_byte(0x8800 + ((int8_t)tile_number << 4) + (LY & 0b00000111) * 2);
      tile_h = get_byte(0x8801 + ((int8_t)tile_number << 4) + (LY & 0b00000111) * 2);
    }

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

  //digitalWrite(25, LOW);
  return;
}
