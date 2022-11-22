static inline void timer_update(uint32_t _clock) {
  // Timer
  //IO[0x05] = 0x00; //TIMA
  //IO[0x06] = 0x00; //TMA
  //IO[0x07] = 0xF8; //TAC
  uint32_t shift;
  time_before = (uint32_t)timer_div;
  timer_div += (uint16_t)_clock;
  uint8_t TAC = *(IO + 0x07);
  if (TAC & 0x04) { // TAC
    switch (TAC & 0x03) {
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

    uint32_t t1 = (uint32_t)timer_div >> shift;
    uint32_t t2 = time_before >> shift;
    uint32_t bit_mask = (1 << (16 - shift)) - 1;
    uint32_t diff = (t1 - t2) & bit_mask;

    if (diff > 0) {
      uint8_t *TIMA = (IO + 0x05);
      if ((uint16_t)*TIMA + (uint16_t)diff > 0xFF) {
        *TIMA = *(IO + 0x06) + ((uint8_t)diff - 1);
        int_timer  = true;
      } else {
        *TIMA += (uint8_t)diff;
      }
    }
  }
}
