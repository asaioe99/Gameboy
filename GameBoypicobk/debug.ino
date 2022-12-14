// bg&wnd フェッチ

void display_tile() {

  uint8_t  tile_l;
  uint8_t  tile_h;
  uint16_t tile_number;
  uint16_t *tmp = FIFO_bg_wnd;
  uint16_t SCX = 0;
  uint16_t SCY = 0;

  for (uint8_t LY = 0; LY < 144; LY++) {

    uint8_t  SCX_low_3bit = SCX & 0b00000111;
    uint16_t in_tile_y_offset  = ((LY + SCY) & 0b00000111) << 1;
    uint16_t out_tile_y_offset = ((LY + SCY) & 0b11111000) << 2;
    uint16_t base_tile_number;

    //base_tile_number = 0x9C00 + (SCX & 0b00011111) + out_tile_y_offset; // LYに対応したタイルデータ
    base_tile_number = 0x9800 + (SCX & 0b00011111) + out_tile_y_offset;


    for (uint16_t i = 0; i < 21 ; i++) {
      tile_number = get_byte(base_tile_number + i); // numberだけどアドレス

      tile_l = get_byte(0x8000 + (tile_number << 4) + in_tile_y_offset);
      tile_h = get_byte(0x8001 + (tile_number << 4) + in_tile_y_offset);

      //tile_l = get_byte(0x8800 + ((int16_t)tile_number << 4) + (int16_t)in_tile_y_offset);
      //tile_h = get_byte(0x8801 + ((int16_t)tile_number << 4) + (int16_t)in_tile_y_offset);


      uint8_t start_bit = 0;
      uint8_t end_bit   = 8;

      if (i == 0) {
        start_bit = SCX_low_3bit;
      } else if (i == 20) {
        end_bit = SCX_low_3bit;
      }

      for (uint8_t n = start_bit; n < end_bit; n++) {
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
    //drowBitMap(LY);
  }
  return;
}

void tile_display(uint8_t tile_number, uint8_t x, uint8_t y) {

  for (uint8_t i = 0; i < 8; i++) {
    uint8_t color_l = get_byte(0x8000 + tile_number * 16 + i * 2);
    uint8_t color_h = get_byte(0x8001 + tile_number * 16 + i * 2);

    for (uint8_t n = 0; n < 8; n++) {
      uint16_t data = 0b0000000000000000;
      if (color_l & (0b10000000 >> n)) {
        data  = 0b0011100111000111;
      }
      if (color_h & (0b10000000 >> n)) {
        data |= 0b1100011000011000;
      }

      SendCommand4(0x2A, 0, 40 + n + x * 8, 0, 40 + n + x * 8); // Colmun Address
      SendCommand4(0x2B, 0, 40 + i + y * 8, 0, 40 + i + y * 8); // Row Address

      gpio_put(CS, LOW);
      gpio_put(DC, LOW); // Command mode
      SPI_transfer(0x2C);
      gpio_put(DC, HIGH); // Data mode

      for (uint8_t bit = 0x80; bit; bit >>= 1) {
        gpio_put(MOSI, ((uint8_t)(data & 0x00FF)) & bit);
        gpio_put(CLK, HIGH);
        gpio_put(CLK, LOW);
      }
      for (uint8_t bit = 0x80; bit; bit >>= 1) {
        gpio_put(MOSI, ((uint8_t)(data >> 8)) & bit);
        gpio_put(CLK, HIGH);
        gpio_put(CLK, LOW);
      }
    }
  }
  gpio_put(CS, HIGH);
}

void dump_tilemap() {
  for (int n = 0; n < 16 * 16; n++) {
    tile_display(n, n % 16, n / 16);
  }
}

void chk_init_regs() {
  Serial.print("AF:");
  Serial.print(AR, HEX);
  Serial.println(FR, HEX);
  Serial.print("BC:");
  Serial.print(BR, HEX);
  Serial.println(CR, HEX);
  Serial.print("DE:");
  Serial.print(DR, HEX);
  Serial.println(ER, HEX);
  Serial.print("HL:");
  Serial.print(HR, HEX);
  Serial.println(LR, HEX);
  Serial.print("sp:");
  Serial.println(sp, HEX);
  Serial.print("pc:");
  Serial.println(pc, HEX);

  Serial.print("FF05:00 -> ");Serial.println(get_byte(0xFF05), HEX);//TIME
  Serial.print("FF06:00 -> ");Serial.println(get_byte(0xFF06), HEX);//TMA
  Serial.print("FF07:00 -> ");Serial.println(get_byte(0xFF07), HEX);//TAC
  Serial.print("FF10:80 -> ");Serial.println(get_byte(0xFF10), HEX);//NR10
  Serial.print("FF11:BF -> ");Serial.println(get_byte(0xFF11), HEX);//NR11
  Serial.print("FF12:F3 -> ");Serial.println(get_byte(0xFF12), HEX);//NR12
  Serial.print("FF14:BF -> ");Serial.println(get_byte(0xFF14), HEX);//NR14
  Serial.print("FF16:3F -> ");Serial.println(get_byte(0xFF16), HEX);//NR21
  Serial.print("FF17:00 -> ");Serial.println(get_byte(0xFF17), HEX);//NR22
  Serial.print("FF19:BF -> ");Serial.println(get_byte(0xFF19), HEX);//NR24
  Serial.print("FF1A:7F -> ");Serial.println(get_byte(0xFF1A), HEX);//NR30
  Serial.print("FF1B:FF -> ");Serial.println(get_byte(0xFF1B), HEX);//NR31
  Serial.print("FF1C:9F -> ");Serial.println(get_byte(0xFF1C), HEX);//NR32
  Serial.print("FF1E:BF -> ");Serial.println(get_byte(0xFF1E), HEX);//NR33
  Serial.print("FF20:FF -> ");Serial.println(get_byte(0xFF20), HEX);//NR41
  Serial.print("FF21:00 -> ");Serial.println(get_byte(0xFF21), HEX);//NR42
  Serial.print("FF22:00 -> ");Serial.println(get_byte(0xFF22), HEX);//NR43
  Serial.print("FF23:BF -> ");Serial.println(get_byte(0xFF23), HEX);//NR30
  Serial.print("FF24:77 -> ");Serial.println(get_byte(0xFF24), HEX);//NR50
  Serial.print("FF25:F3 -> ");Serial.println(get_byte(0xFF25), HEX);//NR51
  Serial.print("FF26:F1 -> ");Serial.println(get_byte(0xFF26), HEX);//NR52
  Serial.print("FF40:91 -> ");Serial.println(get_byte(0xFF40), HEX);//LCDC
  Serial.print("FF42:00 -> ");Serial.println(get_byte(0xFF42), HEX);//SCY
  Serial.print("FF43:00 -> ");Serial.println(get_byte(0xFF43), HEX);//SCX
  Serial.print("FF45:00 -> ");Serial.println(get_byte(0xFF45), HEX);//LYC
  Serial.print("FF47:FC -> ");Serial.println(get_byte(0xFF47), HEX);//BGP
  Serial.print("FF48:FF -> ");Serial.println(get_byte(0xFF48), HEX);//OBP0
  Serial.print("FF49:FF -> ");Serial.println(get_byte(0xFF49), HEX);//OBP1
  Serial.print("FF4A:00 -> ");Serial.println(get_byte(0xFF4A), HEX);//WY
  Serial.print("FF4B:00 -> ");Serial.println(get_byte(0xFF4B), HEX);//WX
  Serial.print("FFFF:00 -> ");Serial.println(get_byte(0xFFFF), HEX);//IE
}
