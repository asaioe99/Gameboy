void draw_tile(uint16_t addr, uint8_t x, uint8_t y) {
  uint16_t color;
  uint8_t t1, t2;
  for (uint8_t i = 0; i < 8; i++) {
    for (uint8_t j = 0; j < 8; j++) {
      t1 = get_byte(addr + i * 2);
      t2 = get_byte(addr + i * 2 + 1);
      color = ((t1 & (0b10000000 >> j)) >> (7 - j)) + ((t2 & (0b10000000 >> j)) >> (7 - j));
      //Serial.print(color);  
      color = color * 64;
      tft.drawPixel(x * 8 + j, y * 8 + i, color);
    }
    //Serial.println("");
  }
}

void ppu() {
  uint16_t offset = 0x9800;
  uint8_t r_addr;
  for (uint8_t y = 0; y < 18; y++) {
    for (uint8_t x = 0; x < 20; x++) {
      r_addr = get_byte(offset + x + y * 0x20);
      //Serial.print(offset + x + y * 0x20, HEX);
      //Serial.print(" ");
      //Serial.println(r_addr + 0x8000, HEX);
      draw_tile(r_addr + 0x8000, x, y);
    }
  }
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}
