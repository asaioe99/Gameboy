void ld_r8_d8() {
  switch (code) {
    case 0x06:
      BR = get_byte(++pc);
      break;
    case 0x0E:
      CR = get_byte(++pc);
      break;
    case 0x16:
      DR = get_byte(++pc);
      break;
    case 0x1E:
      ER = get_byte(++pc);
      break;
    case 0x26:
      HR = get_byte(++pc);
      break;
    case 0x2E:
      LR = get_byte(++pc);
      break;
    case 0x3E:
      AR = get_byte(++pc);
      break;
  }
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void ld_r8_phl() {
  switch (code) {
    case 0x46:
      BR = get_byte(HL(HR, LR));
      break;
    case 0x4E:
      CR = get_byte(HL(HR, LR));
      break;
    case 0x56:
      DR = get_byte(HL(HR, LR));
      break;
    case 0x5E:
      ER = get_byte(HL(HR, LR));
      break;
    case 0x66:
      HR = get_byte(HL(HR, LR));
      break;
    case 0x6E:
      LR = get_byte(HL(HR, LR));
      break;
    case 0x7E:
      AR = get_byte(HL(HR, LR));
      break;
  }
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void ld_r8_r8() {
  uint8_t *t1;
  switch (code & 0b00111000) {
    case 0b00000000:
      t1 = &BR ;
      break;
    case 0b00001000:
      t1 = &CR;
      break;
    case 0b00010000:
      t1 = &DR;
      break;
    case 0b00011000:
      t1 = &ER;
      break;
    case 0b00100000:
      t1 = &HR;
      break;
    case 0b00101000:
      t1 = &LR;
      break;
    case 0b00111000:
      t1 = &AR;
      break;
  }
  switch (code & 0b00000111) {
    case 0b00000000:
      *t1 = BR ;
      break;
    case 0b00000001:
      *t1 = CR;
      break;
    case 0b00000010:
      *t1 = DR;
      break;
    case 0b00000011:
      *t1 = ER;
      break;
    case 0b00000100:
      *t1 = HR;
      break;
    case 0b00000101:
      *t1 = LR;
      break;
    case 0b00000111:
      *t1 = AR;
      break;
  }
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void ld_phl_r8() {
  switch (code) {
    case 0x70:
      put_byte(HL(HR, LR), BR);
      break;
    case 0x71:
      put_byte(HL(HR, LR), CR);
      break;
    case 0x72:
      put_byte(HL(HR, LR), DR);
      break;
    case 0x73:
      put_byte(HL(HR, LR), ER);
      break;
    case 0x74:
      put_byte(HL(HR, LR), HR);
      break;
    case 0x75:
      put_byte(HL(HR, LR), LR);
      break;
    case 0x77:
      put_byte(HL(HR, LR), AR);
      break;
  }
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void ld_phl_d8() {
  put_byte(HL(HR, LR), get_byte(++pc));
  cc += 12;
  scaline_counter -= 12;
  pc++;
}

void ld_ar_pr16() {
  switch (code) {
    case 0x0A:
      AR = get_byte(BC(BR, CR));
      break;
    case 0x1A:
      AR = get_byte(DE(DR, ER));
      break;
  }
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void ld_ar_phli() {
  AR = get_byte(HL(HR, LR));
  LR++;
  if (LR == 0x00) HR++;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}
void ld_ar_phld() {
  AR = get_byte(HL(HR, LR));
  LR--;
  if (LR == 0xFF) HR--;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}
void ld_pr16_ar() {
  switch (code) {
    case 0x02:
      put_byte(BC(BR, CR), AR);
      break;
    case 0x12:
      put_byte(DE(DR, ER), AR);
      break;
  }
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void ld_phli_ar() {
  put_byte(HL(HR, LR), AR);
  LR++;
  if (LR == 0x00) HR++;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}
void ld_phld_ar() {
  put_byte(HL(HR, LR), AR);
  LR--;
  if (LR == 0xFF) HR--;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}
void nop() {
  cc += 4;
  scaline_counter -= 4;
  pc++;
}
void inc_r16() {
  switch (code) {
    case 0x03:
      CR++;
      if (CR == 0x00) BR++;
      break;
    case 0x13:
      ER++;
      if (ER == 0x00) DR++;
      break;
    case 0x23:
      LR++;
      if (LR == 0x00) HR++;
      break;
    case 0x33:
      sp++;
      break;
  }
  cc += 8;
  scaline_counter -= 8;
  pc++;
}
void dec_r16() {
  switch (code) {
    case 0x0B:
      CR--;
      if (CR == 0xFF) BR--;
      break;
    case 0x1B:
      ER--;
      if (ER == 0xFF) DR--;
      break;
    case 0x2B:
      LR--;
      if (LR == 0xFF) HR--;
      break;
    case 0x3B:
      sp--;
      break;
  }
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void inc_r8() {
  uint8_t *r;
  uint8_t t;
  switch (code) {
    case 0x04:
      r = &BR ;
      break;
    case 0x0C:
      r = &CR;
      break;
    case 0x14:
      r = &DR;
      break;
    case 0x1C:
      r = &ER;
      break;
    case 0x24:
      r = &HR;
      break;
    case 0x2C:
      r = &LR;
      break;
    case 0x3C:
      r = &AR;
      break;
  }
  t = *r;
  *r = *r + 1;
  if (*r == 0) {
    FR |= 0b10000000;
    FR &= 0b10110000;
  } else {
    FR &= 0b00110000;
  }
  if ((t & 0x0F) == 0x0F) {
    FR |= 0b00100000;
  } else {
    FR &= 0b11010000;
  }
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void dec_r8() {
  uint8_t *r;
  uint8_t t;
  switch (code) {
    case 0x05:
      r = &BR ;
      break;
    case 0x0D:
      r = &CR;
      break;
    case 0x15:
      r = &DR;
      break;
    case 0x1D:
      r = &ER;
      break;
    case 0x25:
      r = &HR;
      break;
    case 0x2D:
      r = &LR;
      break;
    case 0x3D:
      r = &AR;
      break;
  }
  t = *r;
  *r = *r - 1;
  if (*r == 0) {
    FR |= 0b11000000;
  } else {
    FR &= 0b01111111;
    FR |= 0b01000000;
  }
  if (t & 0x0F == 0x00) {
    FR |= 0b00100000;
  } else {
    FR &= 0b11010000;
  }
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void xor_ar_r8() {
  switch (code) {
    case 0xA8:
      AR ^= BR ;
      break;
    case 0xA9:
      AR ^= CR;
      break;
    case 0xAA:
      AR ^= DR;
      break;
    case 0xAB:
      AR ^= ER;
      break;
    case 0xAC:
      AR ^= HR;
      break;
    case 0xAD:
      AR ^= LR;
      break;
    case 0xAE:
      AR ^= get_byte(HL(HR, LR));
      cc += 4;
      scaline_counter -= 4;
      break;
    case 0xAF:
      AR ^= AR;
      break;
  }
  if (AR == 0) {
    FR = 0b10000000;
  } else {
    FR = 0b00000000;
  }
  cc += 4;
  scaline_counter -= 4; // cc_decは都度0リセットされる。これで正しい
  pc++;
}

void sub_ar_r8() {
  uint8_t val_t;
  switch (code) {
    case 0x90:
      val_t = BR ;
      break;
    case 0x91:
      val_t = CR;
      break;
    case 0x92:
      val_t = DR;
      break;
    case 0x93:
      val_t = ER;
      break;
    case 0x94:
      val_t = HR;
      break;
    case 0x95:
      val_t = LR;
      break;
    case 0x96:
      val_t = get_byte(HL(HR, LR));
      cc += 4;
      scaline_counter -= 4;
      break;
    case 0x97:
      val_t = AR;
      break;
  }
  if (AR & 0x0F < val_t & 0x0F) {
    FR = 0b01100000;
  } else {
    FR = 0b01000000;
  }
  if (AR < val_t) {
    FR |= 0b00010000;
  }
  AR = AR - val_t;
  if (AR == 0) FR |= 0b10000000;
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void add_ar_r8() {
  uint8_t val_t;
  switch (code) {
    case 0x80:
      val_t = BR ;
      break;
    case 0x81:
      val_t = CR;
      break;
    case 0x82:
      val_t = DR;
      break;
    case 0x83:
      val_t = ER;
      break;
    case 0x84:
      val_t = HR;
      break;
    case 0x85:
      val_t = LR;
      break;
    case 0x86:
      val_t = get_byte(HL(HR, LR));
      cc += 4;
      scaline_counter -= 4;
      break;
    case 0x87:
      val_t = AR;
      break;
  }
  if (AR & 0x0F + val_t & 0x0F > 0x0F) {
    FR = 0b00100000;
  } else {
    FR = 0b00000000;
  }
  if ((uint16_t)AR + (uint16_t)val_t > 0xFF) {
    FR |= 0b00010000;
  }
  AR += val_t;
  if (AR == 0) FR |= 0b10000000;
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void cp_r8() {
  uint8_t val_t;
  switch (code) {
    case 0xB8:
      val_t = BR;
      break;
    case 0xB9:
      val_t = CR;
      break;
    case 0xBA:
      val_t = DR;
      break;
    case 0xBB:
      val_t = ER;
      break;
    case 0xBC:
      val_t = HR;
      break;
    case 0xBD:
      val_t = LR;
      break;
    case 0xBE:
      val_t = get_byte(HL(HR, LR));
      cc += 4;
      scaline_counter -= 4;
      break;
    case 0xBF:
      val_t = AR;
      break;
  }
  if (AR & 0x0F < val_t & 0x0F) {
    FR = 0b01100000;
  } else {
    FR = 0b01000000;
  }
  if (AR < val_t) {
    FR |= 0b00010000;
  }
  if (AR == val_t) FR |= 0b10000000;
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void jr_cc_d8() {
  switch (code) {
    case 0x18:
      pc = pc + 2 + (int8_t)get_byte(pc + 1);
      cc += 12;
      scaline_counter -= 12;
      return;
    case 0x20:
      if (!(FR & 0b10000000)) {
        pc = pc + 2 + (int8_t)get_byte(pc + 1);
        cc += 12;
        scaline_counter -= 12;
        return;
      }
      break;
    case 0x28:
      if (FR & 0b10000000) {
        pc = pc + 2 + (int8_t)get_byte(pc + 1);
        cc += 12;
        scaline_counter -= 12;
        return;
      }
      break;
    case 0x30:
      if (!(FR & 0b00010000)) {
        pc = pc + 2 + (int8_t)get_byte(pc + 1);
        cc += 12;
        scaline_counter -= 12;
        return;
      }
      break;
    case 0x38:
      if (FR & 0b00010000) {
        pc = pc + 2 + (int8_t)get_byte(pc + 1);
        cc += 12;
        scaline_counter -= 12;
        return;
      }
      break;
  }
  cc += 8;
  scaline_counter -= 8;
  pc += 2;
}

void ld_r16_d16() {
  switch (code) {
    case 0x01:
      CR = get_byte(++pc);
      BR = get_byte(++pc);
      break;
    case 0x11:
      ER = get_byte(++pc);
      DR = get_byte(++pc);
      break;
    case 0x21:
      LR = get_byte(++pc);
      HR = get_byte(++pc);
      break;
    case 0x31:
      sp = (uint16_t)get_byte(++pc);
      sp += ((uint16_t)get_byte(++pc)) << 8;
      break;
  }
  cc += 12;
  scaline_counter -= 12;
  pc++;
}

void rla() {
  if (AR >> 7) {
    AR = (AR << 1) + ((FR & 0b00010000) >> 4);
    FR = 0b00010000;
  } else {
    AR = (AR << 1) + ((FR & 0b00010000) >> 4);
    FR = 0b00000000;
  }
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void ldh_pd8_ar() {
  put_byte(0xFF00 + (uint16_t)get_byte(++pc), AR);
  cc += 12;
  scaline_counter -= 12;
  pc++;
}

void ldh_ar_pd8() {
  AR = get_byte(0xFF00 + (uint16_t)get_byte(++pc));
  cc += 12;
  scaline_counter -= 12;
  pc++;
}

void pop_r16() {
  switch (code) {
    case 0xC1:
      CR = get_byte(sp++);
      BR = get_byte(sp++);
      break;
    case 0xD1:
      ER = get_byte(sp++);
      DR = get_byte(sp++);
      break;
    case 0xE1:
      LR = get_byte(sp++);
      HR = get_byte(sp++);
      break;
    case 0xF1:
      FR = get_byte(sp++) & 0xF0;
      AR = get_byte(sp++);
      break;
  }
  cc += 12;
  scaline_counter -= 12;
  pc++;
}

void ld_pcr_ar() {
  put_byte(0xFF00 + CR, AR);
  cc += 8;
  scaline_counter -= 8;
  pc++; //表では2だった
}

void ld_ar_pcr() {
  AR = get_byte(0xFF00 + CR);
  cc += 8;
  scaline_counter -= 8;
  pc++; //表では2だった
}

void di() {
  ime = 0;
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void ei() {
  ime = 1;
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void push_r16() {
  switch (code) {
    case 0xC5: //0xc5
      put_byte(--sp, BR);
      put_byte(--sp, CR);
      break;
    case 0xD5:
      put_byte(--sp, DR);
      put_byte(--sp, ER);
      break;
    case 0xE5:
      put_byte(--sp, HR);
      put_byte(--sp, LR);
      break;
    case 0xF5:
      put_byte(--sp, AR);
      put_byte(--sp, FR);
      break;
  }
  cc += 16;
  scaline_counter -= 16;
  pc++;
}

void ret() {
  pc = ((uint16_t)get_byte(sp + 1) << 8) + (uint16_t)get_byte(sp);
  sp += 2;
  cc += 16;
  scaline_counter -= 16;
}

void call_d16() { ///// not correct push a return address
  put_byte(--sp, (uint8_t)(((pc + 3) & 0xFF00) >> 8));
  put_byte(--sp, (uint8_t)((pc + 3) & 0x00FF));
  cc += 24;
  scaline_counter -= 24;
  pc = (uint16_t)get_byte(pc + 1) + ((uint16_t)get_byte(pc + 2) << 8);
}

void cp_d8() {
  uint8_t val_t = get_byte(++pc);
  if (AR & 0x0F < val_t & 0x0F) {
    FR = 0b01100000;
  } else {
    FR = 0b01000000;
  }
  if (AR < val_t) {
    FR |= 0b00010000;
  }
  if (AR == val_t) FR |= 0b10000000;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void rl_r8() {
  uint8_t* r;
  uint8_t t;
  switch (code) {
    case 0x10:
      r = &BR ;
      break;
    case 0x11:
      r = &CR;
      break;
    case 0x12:
      r = &DR;
      break;
    case 0x13:
      r = &ER;
      break;
    case 0x14:
      r = &HR;
      break;
    case 0x15:
      r = &LR;
      break;
    case 0x17:
      r = &AR;
      break;
  }
  if (*r >> 7) {
    *r = (*r << 1) + ((FR & 0b00010000) >> 4);
    FR = 0b00010000;
  } else {
    *r = (*r << 1) + ((FR & 0b00010000) >> 4);
    FR = 0b00000000;
  }
  if (*r == 0) FR |= 0b10000000;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void swap() {
  uint8_t* r;
  switch (code) {
    case 0x30:
      r = &BR;
      break;
    case 0x31:
      r = &CR;
      break;
    case 0x32:
      r = &DR;
      break;
    case 0x33:
      r = &ER;
      break;
    case 0x34:
      r = &HR;
      break;
    case 0x35:
      r = &LR;
      break;
    case 0x37:
      r = &AR;
      break;
  }
  *r = ((*r << 4) & 0xF0) + (*r >> 4);
  if (*r) {
    FR = 0b00000000;
  } else {
    FR = 0b10000000;
  }
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void bit_() {
  uint8_t b = (code & 0b00111000) >> 3;
  uint8_t r;
  uint8_t mask = 0b00000001 << b;
  switch (code & 0b00000111) {
    case 0b000:
      r = BR ;
      break;
    case 0b001:
      r = CR;
      break;
    case 0b010:
      r = DR;
      break;
    case 0b011:
      r = ER;
      break;
    case 0b100:
      r = HR;
      break;
    case 0b101:
      r = LR;
      break;
    case 0b111:
      r = AR;
      break;
  }
  r &= mask;
  if (r) {
    FR &= 0b01111111;
  } else {
    FR |= 0b10000000;
  }
  FR |= 0b00100000;
  FR &= 0b10110000;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void res() {
  uint8_t b = (code & 0b00111000) >> 3;
  uint8_t *r;
  uint8_t mask = 0b00000001 << b;
  switch (code & 0b00000111) {
    case 0b000:
      r = &BR ;
      break;
    case 0b001:
      r = &CR;
      break;
    case 0b010:
      r = &DR;
      break;
    case 0b011:
      r = &ER;
      break;
    case 0b100:
      r = &HR;
      break;
    case 0b101:
      r = &LR;
      break;
    case 0b111:
      r = &AR;
      break;
  }
  *r &= (~mask);
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void set() {
  uint8_t b = (code & 0b00111000) >> 3;
  uint8_t *r;
  uint8_t mask = 0b00000001 << b;
  switch (code & 0b00000111) {
    case 0b000:
      r = &BR ;
      break;
    case 0b001:
      r = &CR;
      break;
    case 0b010:
      r = &DR;
      break;
    case 0b011:
      r = &ER;
      break;
    case 0b100:
      r = &HR;
      break;
    case 0b101:
      r = &LR;
      break;
    case 0b111:
      r = &AR;
      break;
  }
  *r |= mask;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void jp_d16() {
  pc = (uint16_t)get_byte(pc + 1) + ((uint16_t)get_byte(pc + 2) << 8);
  cc += 16;
  scaline_counter -= 16;
}

void cpl() {
  AR = ~AR;
  FR |= 0b01100000;
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void ccf() {
  FR &= 0b10010000;
  if (FR & 0b00010000) {
    FR &= 0b11100000;
  } else {
    FR |= 0b00010000;
  }
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void and_d8() {
  AR &= get_byte(++pc);
  if (AR) {
    FR = 0b00100000;
  } else {
    FR = 0b10100000;
  }
  cc += 8;
  scaline_counter -= 8;
  pc++;
}
void or_d8() {
  AR |= get_byte(++pc);
  if (AR) {
    FR = 0b00000000;
  } else {
    FR = 0b10000000;
  }
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void or_r8() {
  switch (code) {
    case 0xB0:
      AR |= BR ;
      break;
    case 0xB1:
      AR |= CR;
      break;
    case 0xB2:
      AR |= DR;
      break;
    case 0xB3:
      AR |= ER;
      break;
    case 0xB4:
      AR |= HR;
      break;
    case 0xB5:
      AR |= LR;
      break;
    case 0xB7:
      AR |= AR;
      break;
  }
  if (AR) {
    FR = 0b00000000;
  } else {
    FR = 0b10000000;
  }
  cc += 4;
  scaline_counter -= 4;
  pc++;
}
void and_r8() {
  switch (code) {
    case 0xA0:
      AR &= BR ;
      break;
    case 0xA1:
      AR &= CR;
      break;
    case 0xA2:
      AR &= DR;
      break;
    case 0xA3:
      AR &= ER;
      break;
    case 0xA4:
      AR &= HR;
      break;
    case 0xA5:
      AR &= LR;
      break;
    case 0xA7:
      AR &= AR;
      break;
  }
  if (AR) {
    FR = 0b00100000;
  } else {
    FR = 0b10100000;
  }
  cc += 4;
  scaline_counter -= 4;
  pc++;
}
void ld_ar_pa16() {
  AR = get_byte((uint16_t)get_byte(pc + 1) + (uint16_t)(get_byte(pc + 2) << 8));
  cc += 16;
  scaline_counter -= 16;
  pc += 3;
}

void ld_pa16_ar() {
  put_byte((uint16_t)get_byte(pc + 1) + (uint16_t)(get_byte(pc + 2) << 8), AR);
  cc += 16;
  scaline_counter -= 16;
  pc += 3;
}

void ret_cc() {
  switch (code) {
    case 0xC0: //not zero
      if (!(FR & 0b10000000)) {
        cc += 20;
        scaline_counter -= 20;
        pc = ((uint16_t)get_byte(sp + 1) << 8) + (uint16_t)get_byte(sp);
        sp += 2;
        return;
      }
      break;
    case 0xD0: //not carry
      if (!(FR & 0b00010000)) {
        cc += 20;
        scaline_counter -= 20;
        pc = ((uint16_t)get_byte(sp + 1) << 8) + (uint16_t)get_byte(sp);
        sp += 2;
        return;
      }
      break;
    case 0xC8: //zero
      if (FR & 0b10000000) {
        cc += 20;
        scaline_counter -= 20;
        pc = ((uint16_t)get_byte(sp + 1) << 8) + (uint16_t)get_byte(sp);
        sp += 2;
        return;
      }
      break;
    case 0xD8: //carry
      if (FR & 0b00010000) {
        cc += 20;
        scaline_counter -= 20;
        pc = ((uint16_t)get_byte(sp + 1) << 8) + (uint16_t)get_byte(sp);
        sp += 2;
        return;
      }
      break;
  }
  cc += 8;
  scaline_counter -= 8;
  pc++;
}
void reti() {
  ime = 1;
  pc = ((uint16_t)get_byte(sp + 1) << 8) + (uint16_t)get_byte(sp);
  sp += 2;
  cc += 16;
  scaline_counter -= 16;
}

void add_hl_r16() { //動作怪しい
  uint16_t HL = HL(HR, LR);
  uint16_t r16;
  uint32_t t;
  switch (code) {
    case 0x09:
      r16 = BC(BR, CR);
      break;
    case 0x19:
      r16 = DE(DR, ER);
      break;
    case 0x29:
      r16 = HL(HR, LR);
      break;
    case 0x39:
      r16 = sp;
      break;
  }
  if ((HL & 0x0FFF) + (r16 & 0x0FFF) > 0x0FFF) { //N H
    FR |= 0b00100000;
    FR &= 0b10100000;
  } else {
    FR &= 0b10000000;
  }
  t = (uint32_t)HL + (uint32_t)r16;
  if (t > 0xFFFF) {  // C
    FR |= 0b00010000;
  }
  HR = (uint8_t)(t & 0x0000FF00);
  LR = (uint8_t)(t & 0x000000FF);
  pc++;
}
void stop_0() {
  cc += 4;
  scaline_counter -= 4;
  pc += 2;
}

void jp_cc_d16() {
  switch (code) {
    case 0xC2: //not zero
      if (!(FR & 0b10000000)) {
        jp_d16();
        return;
      }
      break;
    case 0xD2: //not carry
      if (!(FR & 0b00010000)) {
        jp_d16();
        return;
      }
      break;
    case 0xCA: //zero
      if (FR & 0b10000000) {
        jp_d16();
        return;
      }
      break;
    case 0xDA: //carry
      if (FR & 0b00010000) {
        jp_d16();
        return;
      }
      break;
  }
  cc += 12;
  scaline_counter -= 12;
  pc += 3;
}

void call_cc_d16() {
  switch (code) {
    case 0xC4: //not zero
      if (!(FR & 0b10000000)) {
        call_d16();
        return;
      }
      break;
    case 0xD4: //not carry
      if (!(FR & 0b00010000)) {
        call_d16();
        return;
      }
      break;
    case 0xCC: //zero
      if (FR & 0b10000000) {
        call_d16();
        return;
      }
      break;
    case 0xDC: //carry
      if (FR & 0b00010000) {
        call_d16();
        return;
      }
      break;
  }
  cc += 12;
  scaline_counter -= 12;
  pc += 3;
}

void adc_ar_r8() {
  uint8_t val_t;
  uint8_t c_val_t = (FR & 0b00010000) >> 4;
  switch (code) {
    case 0x88:
      val_t = BR ;
      break;
    case 0x89:
      val_t = CR;
      break;
    case 0x8A:
      val_t = DR;
      break;
    case 0x8B:
      val_t = ER;
      break;
    case 0x8C:
      val_t = HR;
      break;
    case 0x8D:
      val_t = LR;
      break;
    case 0x8E:
      val_t = get_byte(HL(HR, LR));
      cc += 4;
      scaline_counter -= 4;
      break;
    case 0x8F:
      val_t = AR;
      break;
  }
  if (AR & 0x0F + val_t & 0x0F + c_val_t > 0x0F) {
    FR = 0b00100000;
  } else {
    FR = 0b00000000;
  }
  if ((uint16_t)AR + (uint16_t)val_t + (uint16_t)c_val_t > 0xFF) {
    FR |= 0b00010000;
  }
  AR = AR + val_t + c_val_t;
  if (AR == 0) FR |= 0b10000000;
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void rst_vec() {
  put_byte(--sp, (uint8_t)(((pc + 1) & 0xFF00) >> 8));
  put_byte(--sp, (uint8_t)((pc + 1) & 0x00FF));
  cc += 16;
  scaline_counter -= 16;
  pc = (uint16_t)code - 0xC7;
}

void sbc_ar_r8() {
  uint8_t val_t;
  uint8_t c_val_t = (FR & 0b00010000) >> 4;
  switch (code) {
    case 0x98:
      val_t = BR;
      break;
    case 0x99:
      val_t = CR;
      break;
    case 0x9A:
      val_t = DR;
      break;
    case 0x9b:
      val_t = ER;
      break;
    case 0x9c:
      val_t = HR;
      break;
    case 0x9d:
      val_t = LR;
      break;
    case 0x9e:
      val_t = get_byte(HL(HR, LR));
      cc += 4;
      scaline_counter -= 4;
      break;
    case 0x9f:
      val_t = AR;
      break;
  }
  if ((AR & 0x0F) < (val_t & 0x0F) + c_val_t) {
    FR = 0b01100000;
  } else {
    FR = 0b01000000;
  }
  if ((uint16_t)AR < (uint16_t)val_t + (uint16_t)c_val_t) {
    FR |= 0b00010000;
  }
  AR = AR - val_t;
  if (AR == 0) FR |= 0b10000000;
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void add_ar_d8() {
  uint8_t val_t = get_byte(++pc);
  if (AR & 0x0F + val_t & 0x0F > 0x0F) {
    FR = 0b00100000;
  } else {
    FR = 0b00000000;
  }
  if ((uint16_t)AR + (uint16_t)val_t > 0xFF) {
    FR |= 0b00010000;
  }
  AR = AR + val_t;
  if (AR == 0) FR |= 0b10000000;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void adc_ar_d8() {
  uint8_t val_t = get_byte(++pc);
  uint8_t c_val_t = (FR & 0b00010000) >> 4;
  if (AR & 0x0F + val_t & 0x0F + c_val_t > 0x0F) {
    FR = 0b00100000;
  } else {
    FR = 0b00000000;
  }
  if ((uint16_t)AR + (uint16_t)val_t + (uint16_t)c_val_t > 0xFF) {
    FR |= 0b00010000;
  }
  AR = AR + val_t;
  if (AR == 0) FR |= 0b10000000;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void sub_ar_d8() {
  uint8_t val_t = get_byte(++pc);
  if (AR & 0x0F < val_t & 0x0F) {
    FR = 0b01100000;
  } else {
    FR = 0b01000000;
  }
  if (AR < val_t) {
    FR |= 0b00010000;
  }
  AR = AR - val_t;
  if (AR == 0) FR |= 0b10000000;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void sbc_ar_d8() {
  uint8_t val_t = get_byte(++pc);
  uint8_t c_val_t = (FR & 0b00010000) >> 4;
  if ((AR & 0x0F) < (val_t & 0x0F) + c_val_t) {
    FR = 0b01100000;
  } else {
    FR = 0b01000000;
  }
  if ((uint16_t)AR < (uint16_t)val_t + (uint16_t)c_val_t) {
    FR |= 0b00010000;
  }
  AR = AR - val_t;
  if (AR == 0) FR |= 0b10000000;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void sla_r8() {
  uint8_t *val_t;
  switch (code) {
    case 0x20:
      val_t = &BR ;
      break;
    case 0x21:
      val_t = &CR;
      break;
    case 0x22:
      val_t = &DR;
      break;
    case 0x23:
      val_t = &ER;
      break;
    case 0x24:
      val_t = &HR;
      break;
    case 0x25:
      val_t = &LR;
      break;
    case 0x27:
      val_t = &AR;
      break;
  }
  if (*val_t & 0b10000000 > 0) {
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  *val_t = *val_t << 1;
  if (*val_t == 0) FR |= 0b10000000;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}
void rlc_r8() {
  uint8_t* val_t;
  switch (code) {
    case 0x00:
      val_t = &BR;
      break;
    case 0x01:
      val_t = &CR;
      break;
    case 0x02:
      val_t = &DR;
      break;
    case 0x03:
      val_t = &ER;
      break;
    case 0x04:
      val_t = &HR;
      break;
    case 0x05:
      val_t = &LR;
      break;
    case 0x07:
      val_t = &AR;
      break;
  }
  if (*val_t & 0b10000000 > 0) {
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  *val_t  = (*val_t << 1) + ((*val_t & 0b10000000) >> 7);
  if (*val_t  == 0) FR |= 0b10000000;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void srl_r8() {
  uint8_t* val_t;
  switch (code) {
    case 0x38:
      val_t = &BR ;
      break;
    case 0x39:
      val_t = &CR;
      break;
    case 0x3A:
      val_t = &DR;
      break;
    case 0x3B:
      val_t = &ER;
      break;
    case 0x3C:
      val_t = &HR;
      break;
    case 0x3D:
      val_t = &LR;
      break;
    case 0x3F:
      val_t = &AR;
      break;
  }
  if (*val_t & 0b00000001 > 0) {
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  *val_t = *val_t >> 1;
  if (*val_t == 0) FR |= 0b10000000;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void sra_r8() {
  uint8_t* val_t;
  switch (code) {
    case 0x28:
      val_t = &BR ;
      break;
    case 0x29:
      val_t = &CR;
      break;
    case 0x2A:
      val_t = &DR;
      break;
    case 0x2B:
      val_t = &ER;
      break;
    case 0x2C:
      val_t = &HR;
      break;
    case 0x2D:
      val_t = &LR;
      break;
    case 0x2F:
      val_t = &AR;
      break;
  }
  if (*val_t & 0b00000001 > 0) { // 矛盾あり　無条件で0とするという記述もある
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  *val_t = (*val_t >> 1) + (*val_t & 0b10000000);
  if (AR == 0) FR |= 0b10000000;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void rr_r8() {
  uint8_t* val_t;
  switch (code) {
    case 0x18:
      val_t = &BR;
      break;
    case 0x19:
      val_t = &CR;
      break;
    case 0x1A:
      val_t = &DR;
      break;
    case 0x1B:
      val_t = &ER;
      break;
    case 0x1C:
      val_t = &HR;
      break;
    case 0x1D:
      val_t = &LR;
      break;
    case 0x1F:
      val_t = &AR;
      break;
  }
  uint8_t bit_0 = *val_t & 0b00000001;
  if (FR & 0b00010000) {
    *val_t = (*val_t >> 1) | 0b10000000;
  } else {
    *val_t = (*val_t >> 1);
  }
  if (bit_0) {
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  if (*val_t == 0) FR |= 0b10000000;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void rra() {
  uint8_t bit_0 = AR & 0b00000001;
  if (FR & 0b00010000) {
    AR = (AR >> 1) | 0b10000000;
  } else {
    AR = (AR >> 1);
  }
  if (bit_0) {
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void xor_ar_d8() {
  AR = AR ^ get_byte(++pc);
  if (AR == 0) {
    FR = 0b10000000;
  } else {
    FR = 0b00000000;
  }
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void or_ar_phl() {
  AR |= get_byte(HL(HR, LR));
  if (AR == 0) {
    FR = 0b10000000;
  } else {
    FR = 0b00000000;
  }
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void and_ar_phl() {
  AR &= get_byte(HL(HR, LR));
  if (AR == 0) {
    FR = 0b10000000;
  } else {
    FR = 0b00000000;
  }
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void dec_phl() {
  uint8_t val_t = get_byte(HL(HR, LR));
  if ((val_t & 0x0F) == 0x00) {
    FR |= 0b00100000;
  } else {
    FR &= 0b11010000;
  }
  put_byte(HL(HR, LR), val_t - 1);
  if (val_t == 0x01) {
    FR |= 0b10000000;
  } else {
    FR &= 0b01110000;
  }
  FR |= 0b01000000;
  cc += 12;
  scaline_counter -= 12;
  pc++;
}

void inc_phl() {
  uint8_t val_t = get_byte(HL(HR, LR));
  if ((val_t & 0x0F) == 0x0F) {
    FR |= 0b00100000;
  } else {
    FR &= 0b11010000;
  }
  put_byte(HL(HR, LR), val_t + 1);
  if (val_t == 0xFF) {
    FR |= 0b10000000;
  } else {
    FR &= 0b01110000;
  }
  FR &= 0b10110000;
  cc += 12;
  scaline_counter -= 12;
  pc++;
}
void halt() { //Timer未実装のため、halt後のtimer割り込みがなくループになるので、手抜き中
  if (ime) {
    //省電力モード？
  } else {
    
  }
  //halted = 1;
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void rlca() {
  if (AR & 0b10000000 > 0) {
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  AR = (AR << 1) | (AR >> 7);

  cc += 4;
  scaline_counter -= 4;
  pc++;
}
void daa() {
  if (!(FR & 0b01000000)) {
    if ((FR & 0b00010000) || AR > 0x99) {
      AR += 0x60;
      FR |= 0b00010000;
    }
    if ((FR & 0b00100000) || ((AR & 0x0F) > 0x09)) AR -= 0x06;
  } else {
    if (FR & 0b00010000) AR -= 0x60;
    if (FR & 0b00100000) AR -= 0x06;
  }
  if (AR == 0) {
    FR |= 0b10000000;
  } else {
    FR &= 0b01110000;
  }
  FR &= 0b11010000;
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void scf() {
  FR &= 0b10000000;
  FR |= 0b00010000;
  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void ld_pd16_sp() {
  put_byte(((uint16_t)get_byte(pc + 2) << 8) + get_byte(pc + 1), (uint8_t)(sp & 0x00FF));
  put_byte(((uint16_t)get_byte(pc + 2) << 8) + get_byte(pc + 1) + 1, (uint8_t)(sp >> 8));
  cc += 20;
  scaline_counter -= 20;
  pc += 3;
}

void add_sp_d8() {
  uint16_t val_t = (uint16_t)get_byte(++pc);

  if ((sp & 0x0F) + (val_t & 0x0F) > 0x0F) {
    FR = 0b00100000;
  } else {
    FR = 0b00000000;
  }
  if ((sp & 0xFF) + (val_t & 0xFF) > 0xFF) FR |= 0b00010000;

  sp += val_t;
  cc += 16;
  scaline_counter -= 16;
  pc++;
}

void jp_hl() {
  pc = HL(HR, LR);
  cc += 4;
  scaline_counter -= 4;
}

void ld_sp_hl() {
  sp = HL(HR, LR);
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void rrca() {
  if (AR & 0b00000001 > 0) {
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  AR = (AR >> 1) | (AR << 7);

  cc += 4;
  scaline_counter -= 4;
  pc++;
}

void ld_hl_sp_d8() {
  uint16_t val_t = (uint16_t)get_byte(++pc);

  if ((sp & 0x0F) + (val_t & 0x0F) > 0x0F) {
    FR = 0b00100000;
  } else {
    FR = 0b00000000;
  }
  if ((sp & 0xFF) + (uint16_t)(val_t & 0xFF) > 0xFF) FR |= 0b00010000;

  val_t += sp;
  HR = (uint8_t)(val_t >> 8);
  LR = (uint8_t)(val_t & 0x00FF);
  cc += 12;
  scaline_counter -= 12;
  pc++;
}

void swap_phl() {
  uint8_t val_t = get_byte(HL(HR, LR));
  val_t = ((val_t << 4) & 0xF0) + (val_t >> 4);
  if (val_t) {
    FR = 0b00000000;
  } else {
    FR = 0b10000000;
  }
  put_byte(HL(HR, LR), val_t);
  cc += 16;
  scaline_counter -= 16;
  pc += 2;
}

void sla_phl() {
  uint8_t val_t = get_byte(HL(HR, LR));
  if (val_t & 0b10000000) {
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  val_t = val_t << 1;
  if (!val_t) FR |= 0b10000000;
  put_byte(HL(HR, LR), val_t);
  cc += 16;
  scaline_counter -= 16;
  pc += 2;
}
void rrc_r8() {
  uint8_t* val_t;
  switch (code) {
    case 0x08:
      val_t = &BR ;
      break;
    case 0x09:
      val_t = &CR;
      break;
    case 0x0A:
      val_t = &DR;
      break;
    case 0x0B:
      val_t = &ER;
      break;
    case 0x0C:
      val_t = &HR;
      break;
    case 0x0D:
      val_t = &LR;
      break;
    case 0x0F:
      val_t = &AR;
      break;
  }
  if (*val_t & 0b00000001 > 0) {
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  *val_t = (*val_t >> 1) | ((*val_t & 0b00000001) << 7);
  if (*val_t == 0) FR |= 0b10000000;
  cc += 8;
  scaline_counter -= 8;
  pc++;
}

void set_phl() {
  uint8_t b = (code & 0b00111000) >> 3;
  uint8_t mask = 0b00000001 << b;
  uint8_t r = get_byte(HL(HR, LR));
  r |= mask;
  put_byte(HL(HR, LR), r);
  cc += 16;
  scaline_counter -= 16;
  pc++;
}

void res_phl() {
  uint8_t b = (code & 0b00111000) >> 3;
  uint8_t mask = 0b00000001 << b;
  uint8_t r = get_byte(HL(HR, LR));
  r &= (~mask);
  put_byte(HL(HR, LR), r);
  cc += 16;
  scaline_counter -= 16;
  pc++;
}

void bit_phl() {
  uint8_t b = (code & 0b00111000) >> 3;
  uint8_t mask = 0b00000001 << b;
  uint8_t r = get_byte(HL(HR, LR));

  r = r & mask;
  if (r) {
    FR &= 0b01111111;
  } else {
    FR |= 0b10000000;
  }
  FR |= 0b00100000;
  FR &= 0b10110000;
  cc += 16;
  scaline_counter -= 16;
  pc++;
}

void rlc_phl() {
  uint8_t val_t = get_byte(HL(HR, LR));

  if (val_t & 0b10000000 > 0) {
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  val_t  = (val_t << 1) + ((val_t & 0b10000000) >> 7);
  if (val_t  == 0) FR |= 0b10000000;
  put_byte(HL(HR, LR), val_t);
  cc += 16;
  scaline_counter -= 16;
  pc++;
}

void rl_phl() {
  uint8_t val_t = get_byte(HL(HR, LR));

  if (val_t >> 7) {
    val_t = (val_t << 1) + ((FR & 0b00010000) >> 4);
    FR = 0b00010000;
  } else {
    val_t = (val_t << 1) + ((FR & 0b00010000) >> 4);
    FR = 0b00000000;
  }
  if (val_t == 0) FR |= 0b10000000;
  put_byte(HL(HR, LR), val_t);
  cc += 16;
  scaline_counter -= 16;
  pc++;
}

void rrc_phl() {
  uint8_t val_t = get_byte(HL(HR, LR));

  if (val_t & 0b00000001 > 0) {
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  AR = (val_t >> 1) + ((val_t & 0b00000001) << 7);
  if (AR == 0) FR |= 0b10000000;
  put_byte(HL(HR, LR), val_t);
  cc += 16;
  scaline_counter -= 16;
  pc++;
}

void rr_phl() {
  uint8_t val_t = get_byte(HL(HR, LR));
  uint8_t bit_0 = val_t & 0b00000001;
  if (FR & 0b00010000) {
    val_t = (val_t >> 1) | 0b10000000;
  } else {
    val_t = (val_t >> 1);
  }
  if (bit_0) {
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  if (val_t == 0) FR |= 0b10000000;
  put_byte(HL(HR, LR), val_t);
  cc += 16;
  scaline_counter -= 16;
  pc++;
}

void sra_phl() {
  uint8_t val_t = get_byte(HL(HR, LR));

  if (val_t & 0b00000001 > 0) { // 矛盾あり　無条件で0とするという記述もある
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  val_t = (val_t >> 1) + (val_t & 0b10000000);
  if (AR == 0) FR |= 0b10000000;
  put_byte(HL(HR, LR), val_t);
  cc += 16;
  scaline_counter -= 16;
  pc++;
}

void srl_phl() {
  uint8_t val_t = get_byte(HL(HR, LR));
  if (val_t & 0b00000001 > 0) {
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  val_t = val_t >> 1;
  if (val_t == 0) FR |= 0b10000000;
  put_byte(HL(HR, LR), val_t);
  cc += 16;
  scaline_counter -= 16;
  pc++;
}

void call_irpt(uint16_t addr) { ///// not correct push a return address
  put_byte(--sp, (uint8_t)((pc & 0xFF00) >> 8));
  put_byte(--sp, (uint8_t)(pc & 0x00FF));
  cc += 32; //正しいのか分からないけど、多分これくらい
  scaline_counter -= 32;
  pc = addr;
}
