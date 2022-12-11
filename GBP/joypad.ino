void joypad_init() {
  gpio_init(KEY_R);
  gpio_init(KEY_L);
  gpio_init(KEY_U);
  gpio_init(KEY_D);
  gpio_init(KEY_A);
  gpio_init(KEY_B);
  gpio_init(KEY_X);
  gpio_init(KEY_Y);

  gpio_set_dir(KEY_R, GPIO_IN);
  gpio_set_dir(KEY_L, GPIO_IN);
  gpio_set_dir(KEY_U, GPIO_IN);
  gpio_set_dir(KEY_D, GPIO_IN);
  gpio_set_dir(KEY_A, GPIO_IN);
  gpio_set_dir(KEY_B, GPIO_IN);
  gpio_set_dir(KEY_X, GPIO_IN);
  gpio_set_dir(KEY_Y, GPIO_IN);

  gpio_pull_up(KEY_R);
  gpio_pull_up(KEY_L);
  gpio_pull_up(KEY_U);
  gpio_pull_up(KEY_D);
  gpio_pull_up(KEY_A);
  gpio_pull_up(KEY_B);
  gpio_pull_up(KEY_X);
  gpio_pull_up(KEY_Y);
}

void joypad_update_state() {
  if (gpio_get(KEY_R)) {
    joypad_state |= 0x01;
  } else {
    joypad_state &= 0xFE;
    IF |= 0x10;
  }
  if (gpio_get(KEY_L)) {
    joypad_state |= 0x02;
    } else {
    joypad_state &= 0xFD;
    IF |= 0x10;
  }
  if (gpio_get(KEY_U)) {
    joypad_state |= 0x04;
    } else {
    joypad_state &= 0xFB;
    IF |= 0x10;
  }
  if (gpio_get(KEY_D)) {
    joypad_state |= 0x08;
    } else {
    joypad_state &= 0xF7;
    IF |= 0x10;
  }
  if (gpio_get(KEY_A)) {
    joypad_state |= 0x10;
    } else {
    joypad_state &= 0xEF;
    IF |= 0x10;
  }
  if (gpio_get(KEY_B)) {
    joypad_state |= 0x20;
    } else {
    joypad_state &= 0xDF;
    IF |= 0x10;
  }
  if (gpio_get(KEY_X)) {
    joypad_state |= 0x80;
    } else {
    joypad_state &= 0x7F;
    IF |= 0x10;
  }
  if (gpio_get(KEY_Y)) {
    joypad_state |= 0x40;
    } else {
    joypad_state &= 0xBF;
    IF |= 0x10;
  }
}

static inline uint8_t joypad_read() {
  if (!(*IO & 0x10)) {
    return (*IO & 0xF0) | (joypad_state & 0x0F);
  } else if (!(*IO & 0x20)) {
    return (*IO & 0xF0) | ((joypad_state >> 4) & 0x0F);
  }
  return *IO | 0x0F;
}