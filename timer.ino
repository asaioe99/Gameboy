void timer_update(uint8_t _clock) {
  // Timer
  if (!(clock_cycle % 0xFF)) *(IO + 0x04) += 1; // increment DIV something wrong
  if (*(IO + 0x07) & 0x04) { // TAC
    switch (*(IO + 0x07) & 0x03) {
      case 0:
        if (!(clock_cycle % 0x04FF)) *(IO + 0x05) += 1;
        break;
      case 1:
        if (!(clock_cycle % 0x0F))   *(IO + 0x05) += 1;
        break;
      case 2:
        if (!(clock_cycle % 0x4F))   *(IO + 0x05) += 1;
        break;
      case 3:
        if (!(clock_cycle % 0xFF))   *(IO + 0x05) += 1;
        break;
    }
    if (!*(IO + 0x05)) { // TIMA count up
      *(IO + 0x05) = *(IO + 0x06); // TMA
      int_timer = true; // timer interrupt
    }
  }
}
