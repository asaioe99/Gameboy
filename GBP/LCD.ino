static const uint8_t st7789_init_seq[] = {
  1, 20, 0x01,                         // Software reset
  1, 10, 0x11,                         // Exit sleep mode
  2, 2 , 0x3a, 0x05,                   // Set colour mode to 16 bit
  2, 0 , 0x36, 0x60,                   // Set MADCTL: row then column, refresh is bottom to top ????
  3, 0 , 0xB6, 0x15, 0x02,             // Display settings #5
  2, 0 , 0xB4, 0x00,                   // Display inversion control
  1, 0 , 0x13,                         // NomalDisplayMode
  1, 0 , 0x20,                         // Display Inversion Off
  5, 0 , 0x2a, 0x00, 0x00, 0x00, 0xf0, // CASET: column addresses from 0x28 to 0xc7 (f0)
  5, 0 , 0x2b, 0x00, 0x00, 0x00, 0xf0, // RASET: row addresses from 0x28 to 0xb7 (f0)
  1, 2 , 0x21,                         // Inversion on, then 10 ms delay (supposedly a hack?)
  1, 2 , 0x13,                         // Normal display on, then 10 ms delay
  1, 2 , 0x29,                         // Main screen turn on, then wait 500 ms
  0                                    // Terminate list
};

static inline void lcd_set_dc_cs(bool dc, bool cs) {
  sleep_us(1);
  gpio_put_masked((1u << PIN_DC) | (1u << PIN_CS), !!dc << PIN_DC | !!cs << PIN_CS);
  sleep_us(1);
}

static inline void lcd_write_cmd(PIO pio, uint sm, const uint8_t *cmd, size_t count) {
  st7789_lcd_wait_idle(pio, sm);
  lcd_set_dc_cs(0, 0);
  st7789_lcd_put(pio, sm, *cmd++);
  if (count >= 2) {
    st7789_lcd_wait_idle(pio, sm);
    lcd_set_dc_cs(1, 0);
    for (size_t i = 0; i < count - 1; ++i)
      st7789_lcd_put(pio, sm, *cmd++);
  }
  st7789_lcd_wait_idle(pio, sm);
  lcd_set_dc_cs(1, 1);
}

static inline void lcd_init(PIO pio, uint sm, const uint8_t *init_seq) {
  const uint8_t *cmd = init_seq;
  while (*cmd) {
    lcd_write_cmd(pio, sm, cmd + 2, *cmd);
    sleep_ms(*(cmd + 1) * 5);
    cmd += *cmd + 2;
  }
}

static inline void st7789_start_pixels(PIO pio, uint sm) {
  uint8_t cmd = 0x2c; // RAMWR
  lcd_write_cmd(pio, sm, &cmd, 1);
  lcd_set_dc_cs(1, 0);
}

void LCD_init() {

  uint offset = pio_add_program(pio, &st7789_lcd_program);
  st7789_lcd_program_init(pio, sm, offset, PIN_DIN, PIN_CLK, SERIAL_CLK_DIV);

  gpio_init(PIN_CS);
  gpio_init(PIN_DC);
  gpio_init(PIN_RESET);
  gpio_init(PIN_BL);
  gpio_set_dir(PIN_CS, GPIO_OUT);
  gpio_set_dir(PIN_DC, GPIO_OUT);
  gpio_set_dir(PIN_RESET, GPIO_OUT);
  gpio_set_dir(PIN_BL, GPIO_OUT);

  gpio_put(PIN_CS, 1);
  gpio_put(PIN_RESET, 1);
  lcd_init(pio, sm, st7789_init_seq);
  gpio_put(PIN_BL, 1);

}

// BITマップ表示
static inline void LCD_drowBitMap() {
  st7789_start_pixels(pio, sm);

  int i = 0;
  while (i < 241 * 40) {
    st7789_lcd_put(pio, sm, c);
    st7789_lcd_put(pio, sm, c);
    i++;
  }
  int y = 0;
  while (y < 144) {
    i = 0;
    while (i < 40) {
      st7789_lcd_put(pio, sm, c);
      st7789_lcd_put(pio, sm, c);
      i++;
    }
    int x = 0;
    while (x < 160) {
      uint32_t t = *(FIFO_bg_wnd + x + 160 * y);
      if (t & 0b10101010) {
        if (t & 0b01010101) {
          st7789_lcd_put(pio, sm, 0x08);
          st7789_lcd_put(pio, sm, 0x61);
        } else {
          st7789_lcd_put(pio, sm, 0x5A);
          st7789_lcd_put(pio, sm, 0xEB);
        }
      } else {
        if (t & 0b01010101) {
          st7789_lcd_put(pio, sm, 0x9D);
          st7789_lcd_put(pio, sm, 0x75);
        } else {
          st7789_lcd_put(pio, sm, 0xFF);
          st7789_lcd_put(pio, sm, 0xFF);
        }
      }
      x++;
    }
    y++;
    i = 0;
    while (i < 41) {
      st7789_lcd_put(pio, sm, c);
      st7789_lcd_put(pio, sm, c);
      i++;
    }
  }
  i = 0;
  while (i < 241 * 56) {
    st7789_lcd_put(pio, sm, c);
    st7789_lcd_put(pio, sm, c);
    i++;
  }
}
