

void p_flag_update() {
  uint8_t status_LCD = get_byte(0xFF41);

  if ((get_byte(0xFF40) & 0x80) != 0x80) {
    put_byte(0xFF40, 0x00);
    scaline_counter = 456;
    status_LCD &= 0b11111100;
    status_LCD = ~(status_LCD & (1 << 0));
    status_LCD = ~(status_LCD & (1 << 1));
    put_byte(0xFF41, status_LCD);
    return;
  }

  status_LCD = get_byte(0xFF41);

  if (get_byte(0xFF44) >= 144) {         // mode1
    status_LCD = status_LCD & 0b11111101;
  } else {
    if (scaline_counter >= 376) {        // mode2
      status_LCD = status_LCD & 0b11111110;
    } else if (scaline_counter >= 204) { // mode3
      status_LCD = status_LCD & 0b11111111;
    } else {                             // mode0
      status_LCD = status_LCD & 0b11111100;
    }
  }
  
  put_byte(0xFF41, status_LCD);
  
  status_LCD = get_byte(0xFF41);
  // LYC == LY -> status reg
  if (get_byte(0xFF44) == get_byte(0xFF45)) {
    status_LCD = status_LCD | 0b00000100;
  } else {
    status_LCD = status_LCD & 0b11111011;
  }

  put_byte(0xFF41, status_LCD);
}


void draw_scanline() {
  return;
}


void ppu() {
  uint16_t offset = 0x9800;
  uint8_t r_addr;

  // flag アップデート
  p_flag_update();

  if (get_byte(0xFF40) & 0b10000000) {
    scaline_counter -= cc_dec;
  } else {
    return;
  }
  
  if (scaline_counter <= 0) {
    put_byte(0xFF44, get_byte(0xFF44) + 1);
    scaline_counter += 456;

    if (get_byte(0xFF44) == 144) {
      draw_scanline();
      put_byte(0xFF0F, (get_byte(0xFF0F) | 0b00000001));
    } else if (get_byte(0xFF44) > 153) {
      put_byte(0xFF44, 0);
    } else if (get_byte(0xFF44) < 144) {
      draw_scanline();
    }
  }
}
