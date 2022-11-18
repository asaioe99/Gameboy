void timer_update(uint8_t _clock) {
  // Timer
  uint8_t shift;
  time_before = timer_div;
  timer_div += _clock;

  if (*(IO + 0x07) & 0x04) { // TAC
    switch (*(IO + 0x07) & 0x03) {
      case 0:
        shift = 10;
        break;
      case 1:
        shift = 4;
        break;
      case 2:
        shift = 6;
        break;
      case 3:
        shift = 8;
        break;
    }

    uint32_t t1 = timer_div >> shift;
    uint32_t t2 = time_before >> shift;
    uint32_t bit_mask = (1 << (16 - shift)) - 1;
    uint32_t diff = (t1 - t2) & bit_mask;

    if (diff) {
      *(IO + 0x05) += (uint8_t)diff;
      if (*(IO + 0x05) == 0) {
        *(IO + 0x05) = *(IO + 0x06) + (uint8_t)diff - 1; // TMA
        int_timer  = true;
      }
    }
  }
}
