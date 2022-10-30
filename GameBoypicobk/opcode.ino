void LD_r_n() {
  switch (code & 0b00111000) {
    case 0b00000000:
      BR = get_byte(++pc);
      break;
    case 0b00001000:
      CR = get_byte(++pc);
      break;
    case 0b00010000:
      DR = get_byte(++pc);
      break;
    case 0b00011000:
      ER = get_byte(++pc);
      break;
    case 0b00100000:
      HR = get_byte(++pc);
      break;
    case 0b00101000:
      LR = get_byte(++pc);
      break;
    case 0b00111000:
      AR = get_byte(++pc);
      break;
  }
  cc += 8;
  cc_dec = 8;
  pc++;
}

void LD_r_HL() {
  switch (code & 0b00111000) {
    case 0b00000000:
      BR = get_byte(HL(HR, LR));
      break;
    case 0b00001000:
      CR = get_byte(HL(HR, LR));
      break;
    case 0b00010000:
      DR = get_byte(HL(HR, LR));
      break;
    case 0b00011000:
      ER = get_byte(HL(HR, LR));
      break;
    case 0b00100000:
      HR = get_byte(HL(HR, LR));
      break;
    case 0b00101000:
      LR = get_byte(HL(HR, LR));
      break;
    case 0b00111000:
      AR = get_byte(HL(HR, LR));
      break;
  }
  cc += 8;
  cc_dec = 8;
  pc++;
}

void LD_r_r() {
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
  cc_dec = 4;
  pc++;
}

void LD_HL_r() {
  switch (code & 0b00000111) {
    case 0b00000000:
      put_byte(HL(HR, LR), BR );
      break;
    case 0b00000001:
      put_byte(HL(HR, LR), CR);
      break;
    case 0b00000010:
      put_byte(HL(HR, LR), DR);
      break;
    case 0b00000011:
      put_byte(HL(HR, LR), ER);
      break;
    case 0b00000100:
      put_byte(HL(HR, LR), HR);
      break;
    case 0b00000101:
      put_byte(HL(HR, LR), LR);
      break;
    case 0b00000111:
      put_byte(HL(HR, LR), AR);
      break;
  }
  cc += 8;
  cc_dec = 8;
  pc++;
}

void LD_HL_n() {
  put_byte((uint16_t)(HR << 8) + LR, get_byte(++pc));
  cc += 12;
  cc_dec = 12;
  pc++;
}
void LD_A_BC() {
  AR = get_byte(((uint16_t)BR << 8) + CR);
  cc += 8;
  cc_dec = 8;
  pc++;
}
void LD_A_DE() {
  AR = get_byte(((uint16_t)DR << 8) + ER);
  cc += 8;
  cc_dec = 8;
  pc++;
}
void LD_A_HLi() {
  AR = get_byte(HL(HR, LR));
  LR++;
  if (LR == 0x00) HR++;
  cc += 8;
  cc_dec = 8;
  pc++;
}
void LD_A_HLd() {
  AR = get_byte(HL(HR, LR));
  LR--;
  if (LR == 0xFF) HR--;
  cc += 8;
  cc_dec = 8;
  pc++;
}
void LD_BC_A() {
  put_byte(((uint16_t)BR  << 8) + CR, AR);
  cc += 8;
  cc_dec = 8;
  pc++;
}
void LD_DE_A() {
  put_byte(((uint16_t)DR << 8) + ER, AR);
  cc += 8;
  cc_dec = 8;
  pc++;
}
void LD_HLi_A() {
  put_byte(HL(HR, LR), AR);
  LR++;
  if (LR == 0x00) HR++;
  cc += 8;
  cc_dec = 8;
  pc++;
}
void LD_HLd_A() {
  put_byte(HL(HR, LR), AR);
  LR--;
  if (LR == 0xFF) HR--;
  cc += 8;
  cc_dec = 8;
  pc++;
}
void NOP() {
  cc += 4;
  cc_dec = 4;
  pc++;
}
void INC_rr() {
  switch ((code & 0b00110000) >> 4) {
    case 0b00:
      CR++;
      if (CR == 0x00) BR ++;
      break;
    case 0b01:
      ER++;
      if (ER == 0x00) DR++;
      break;
    case 0b10:
      LR++;
      if (LR == 0x00) HR++;
      break;
    case 0b11:
      sp++;
      break;
  }
  cc += 8;
  cc_dec = 8;
  pc++;
}
void DEC_rr() {
  switch (code & 0b00110000) {
    case 0b00000000:
      CR--;
      if (CR == 0xFF) BR --;
      break;
    case 0b00010000:
      ER--;
      if (ER == 0xFF) DR--;
      break;
    case 0b00100000:
      LR--;
      if (LR == 0xFF) HR--;
      break;
    case 0b00110000:
      sp--;
      break;
  }
  cc += 8;
  cc_dec = 8;
  pc++;
}
void INC_r() {
  uint8_t *r;
  uint8_t t;
  switch (code & 0b00111000) {
    case 0b00000000:
      r = &BR ;
      break;
    case 0b00001000:
      r = &CR;
      break;
    case 0b00010000:
      r = &DR;
      break;
    case 0b00011000:
      r = &ER;
      break;
    case 0b00100000:
      r = &HR;
      break;
    case 0b00101000:
      r = &LR;
      break;
    case 0b00111000:
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
  cc_dec = 4;
  pc++;
}
void DEC_r() {
  uint8_t *r;
  switch (code & 0b00111000) {
    case 0b00000000:
      r = &BR ;
      break;
    case 0b00001000:
      r = &CR;
      break;
    case 0b00010000:
      r = &DR;
      break;
    case 0b00011000:
      r = &ER;
      break;
    case 0b00100000:
      r = &HR;
      break;
    case 0b00101000:
      r = &LR;
      break;
    case 0b00111000:
      r = &AR;
      break;
  }
  *r = *r - 1;
  if (*r == 0) {
    FR |= 0b11000000;
  } else {
    FR &= 0b01111111;
    FR |= 0b01000000;
  }
  if (*r & 0x0F == 0x0F) {
    FR |= 0b00100000;
  } else {
    FR &= 0b11010000;
  }
  cc += 4;
  cc_dec = 4;
  pc++;
}
void XOR_r() {
  switch (code & 0b00000111) {
    case 0b000:
      AR = AR ^ BR ;
      break;
    case 0b001:
      AR = AR ^ CR;
      break;
    case 0b010:
      AR = AR ^ DR;
      break;
    case 0b011:
      AR = AR ^ ER;
      break;
    case 0b100:
      AR = AR ^ HR;
      break;
    case 0b101:
      AR = AR ^ LR;
      break;
    case 0b110:
      AR = AR ^ get_byte(HL(HR, LR));
      cc += 4;
      break;
    case 0b111:
      AR = AR ^ AR;
      break;
  }
  if (AR == 0) {
    FR = 0b10000000;
  } else {
    FR = 0b00000000;
  }
  cc += 4;
  cc_dec = 4;
  pc++;
}
void SUB_r() {
  uint8_t val;
  switch (code & 0b00000111) {
    case 0b000:
      val = BR ;
      break;
    case 0b001:
      val = CR;
      break;
    case 0b010:
      val = DR;
      break;
    case 0b011:
      val = ER;
      break;
    case 0b100:
      val = HR;
      break;
    case 0b101:
      val = LR;
      break;
    case 0b110:
      val = get_byte(HL(HR, LR));
      cc += 4;
      cc_dec = 4;
      break;
    case 0b111:
      val = AR;
      break;
  }
  if (AR & 0x0F < val & 0x0F) {
    FR = 0b01100000;
  } else {
    FR = 0b01000000;
  }
  if (AR < val) {
    FR |= 0b00010000;
  }
  AR = AR - val;
  if (AR == 0) FR |= 0b10000000;
  cc += 4;
  cc_dec += 4;
  pc++;
}

void ADD_r() {
  uint8_t val;
  switch (code & 0b00000111) {
    case 0b000:
      val = BR ;
      break;
    case 0b001:
      val = CR;
      break;
    case 0b010:
      val = DR;
      break;
    case 0b011:
      val = ER;
      break;
    case 0b100:
      val = HR;
      break;
    case 0b101:
      val = LR;
      break;
    case 0b110:
      val = get_byte(HL(HR, LR));
      cc += 4;
      cc_dec = 4;
      break;
    case 0b111:
      val = AR;
      break;
  }
  if (AR & 0x0F + val & 0x0F > 0x0F) {
    FR = 0b00100000;
  } else {
    FR = 0b00000000;
  }
  if (AR + val > 0xFF) {
    FR |= 0b00010000;
  }
  AR = AR + val;
  if (AR == 0) FR |= 0b10000000;
  cc += 4;
  cc_dec += 4;
  pc++;
}

void CP_r() {
  uint8_t val;
  switch (code & 0b00000111) {
    case 0b000:
      val = BR ;
      break;
    case 0b001:
      val = CR;
      break;
    case 0b010:
      val = DR;
      break;
    case 0b011:
      val = ER;
      break;
    case 0b100:
      val = HR;
      break;
    case 0b101:
      val = LR;
      break;
    case 0b110:
      val = get_byte(HL(HR, LR));
      cc += 4;
      cc_dec = 4;
      break;
    case 0b111:
      val = AR;
      break;
  }
  if (AR & 0x0F < val & 0x0F) {
    FR = 0b01100000;
  } else {
    FR = 0b01000000;
  }
  if (AR < val) {
    FR |= 0b00010000;
  }
  if (AR == val) FR |= 0b10000000;
  cc += 4;
  cc_dec += 4;
  pc++;
}

void JR_cc_d8() {
  switch (code) {
    case 0x18:
      pc = pc + 2 + (int8_t)get_byte(pc + 1);
      cc += 12;
      cc_dec = 12;
      return;
    case 0x20:
      if (FR >> 7 == 0) {
        pc = pc + 2 + (int8_t)get_byte(pc + 1);
        cc += 12;
        cc_dec = 12;
      } else {
        cc += 8;
        cc_dec = 8;
        pc += 2;
      }
      break;
    case 0x28:
      if ((FR & 0b10000000) >> 7) {
        pc = pc + 2 + (int8_t)get_byte(pc + 1);
        cc += 12;
        cc_dec = 12;
      } else {
        cc += 8;
        cc_dec = 8;
        pc += 2;
      }
      break;
    case 0x30:
      if (FR & 0b00010000 == 0) {
        pc = pc + 2 + (int8_t)get_byte(pc + 1);
        cc += 12;
        cc_dec = 12;
      } else {
        cc += 8;
        cc_dec = 8;
        pc += 2;
      }
      break;
    case 0x38:
      if (FR & 0b00010000) {
        pc = pc + 2 + (int8_t)get_byte(pc + 1);
        cc += 12;
        cc_dec = 12;
      } else {
        cc += 8;
        cc_dec = 8;
        pc += 2;
      }
      break;
  }
}

void LD_r16_d16() {
  switch ((code & 0b00110000) >> 4) {
    case 0b00:
      CR = get_byte(++pc);
      BR  = get_byte(++pc);
      break;
    case 0b01:
      ER = get_byte(++pc);
      DR = get_byte(++pc);
      break;
    case 0b10:
      LR = get_byte(++pc);
      HR = get_byte(++pc);
      break;
    case 0b11:
      sp = get_byte(++pc);
      sp += ((uint16_t)get_byte(++pc)) << 8;
      break;
  }
  cc += 12;
  cc_dec = 12;
  pc++;
}

void RLA() {

  if (AR >> 7) {
    AR = (AR << 1) + ((FR & 0b00010000) >> 4);
    FR = 0b00010000;
  } else {
    AR = (AR << 1) + ((FR & 0b00010000) >> 4);
    FR = 0b00000000;
  }
  cc += 4;
  cc_dec = 4;
  pc++;
}

void LDH_a8_a() {
  put_byte(0xFF00 + get_byte(++pc), AR);
  cc += 12;
  cc_dec = 12;
  pc++;
}

void LDH_a_a8() {
  AR = get_byte(0xFF00 + get_byte(++pc));
  cc += 12;
  cc_dec = 12;
  pc++;
}

void POP_r16() {
  switch ((code & 0b00110000) >> 4) {
    case 0b00:
      CR = get_byte(sp++);
      BR = get_byte(sp++);
      break;
    case 0b01:
      ER = get_byte(sp++);
      DR = get_byte(sp++);
      break;
    case 0b10:
      LR = get_byte(sp++);
      HR = get_byte(sp++);
      break;
    case 0b11:
      FR = get_byte(sp++) & 0xF0;
      AR = get_byte(sp++);
      break;
  }
  cc += 12;
  cc_dec = 12;
  pc++;
}

void LDH_c_a() {
  put_byte(0xFF + CR, AR);
  cc += 8;
  cc_dec = 8;
  pc++;
}

void LDH_a_c() {
  AR = get_byte(0xFF00 + CR);
  cc += 8;
  cc_dec = 8;
  pc++;
}

void DI() {
  ime = 0;
  cc += 4;
  cc_dec = 4;
  pc++;
}

void EI() {
  ime = 1;
  cc += 4;
  cc_dec = 4;
  pc++;
}

void PUSH_r16() {
  switch ((code & 0b00110000) >> 4) {
    case 0b00: //0xc5
      put_byte(--sp, BR );
      put_byte(--sp, CR);
      break;
    case 0b01:
      put_byte(--sp, DR);
      put_byte(--sp, ER);
      break;
    case 0b10:
      put_byte(--sp, HR);
      put_byte(--sp, LR);
      break;
    case 0b11:
      put_byte(--sp, AR);
      put_byte(--sp, FR);
      break;
  }
  cc += 16;
  cc_dec = 16;
  pc++;
}
void RET() {
  pc = ((uint16_t)get_byte(sp + 1) << 8) + get_byte(sp);
  sp += 2;
  cc += 16;
  cc_dec = 16;
}

void CALL() {
  put_byte(--sp, (uint8_t)(((pc + 3) & 0xFF00) >> 8));
  put_byte(--sp, (uint8_t)((pc + 3) & 0x00FF));
  cc += 24;
  cc_dec = 24;
  pc = get_byte(pc + 1) + ((uint16_t)get_byte(pc + 2) << 8);
}

void CP_d8() {
  uint8_t val = get_byte(++pc);
  if (AR & 0x0F < val & 0x0F) {
    FR = 0b01100000;
  } else {
    FR = 0b01000000;
  }
  if (AR < val) {
    FR |= 0b00010000;
  }
  if (AR == val) FR |= 0b10000000;
  cc += 8;
  cc_dec = 8;
  pc++;
}

void RL_r() {
  uint8_t* r;
  uint8_t t;
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
  if (*r >> 7) {
    *r = (*r << 1) + ((FR & 0b00010000) >> 4);
    FR = 0b00010000;
  } else {
    *r = (*r << 1) + ((FR & 0b00010000) >> 4);
    FR = 0b00000000;
  }
  if (*r == 0) FR | 0b10000000;
  cc += 8;
  cc_dec = 8;
  pc++;
}

void SWAP() {
  uint8_t* r;
  uint8_t t;
  switch (code & 0b00000111) {
    case 0b000:
      r = &BR;
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
  t = *r;
  *r = t >> 4;
  *r = t << 4;

  if (t) {
    FR = 0b00000000;
  } else {
    FR = 0b10000000;
  }
  cc += 8;
  cc_dec = 8;
  pc++;
}
void BIT() {
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
  r = r & mask;
  if (r) {
    FR &= 0b01111111;
  } else {
    FR |= 0b10000000;
  }
  FR |= 0b00100000;
  FR &= 0b10110000;
  cc += 8;
  cc_dec = 8;
  pc++;
}

void RES() {
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
  cc_dec = 8;
  pc++;
}

void SET() {
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
  cc_dec = 8;
  pc++;
}

void jp_a16() {
  pc = (uint16_t)get_byte(pc + 1) + ((uint16_t)get_byte(pc + 2) << 8);
  cc += 16;
  cc_dec = 16;
}

void cpl() {
  AR = ~AR;
  FR |= 0b01100000;
  cc += 4;
  cc_dec = 4;
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
  cc_dec = 4;
  pc++;
}

void and_d8() {
  uint8_t val = get_byte(++pc);
  AR &= val;
  if (AR) {
    FR = 0b00100000;
  } else {
    FR = 0b10100000;
  }
  cc += 8;
  cc_dec = 8;
  pc++;
}
void or_d8() {
  uint8_t val = get_byte(++pc);
  AR |= val;
  if (AR) {
    FR = 0b00000000;
  } else {
    FR = 0b10000000;
  }
  cc += 8;
  cc_dec = 8;
  pc++;
}

void or_r8() {
  switch (code & 0b00000111) {
    case 0b000:
      AR |= BR ;
      break;
    case 0b001:
      AR |= CR;
      break;
    case 0b010:
      AR |= DR;
      break;
    case 0b011:
      AR |= ER;
      break;
    case 0b100:
      AR |= HR;
      break;
    case 0b101:
      AR |= LR;
      break;
    case 0b111:
      AR |= AR;
      break;
  }
  if (AR) {
    FR = 0b00000000;
  } else {
    FR = 0b10000000;
  }
  cc += 4;
  cc_dec = 4;
  pc++;
}
void and_r8() {
  switch (code & 0b00000111) {
    case 0b000:
      AR &= BR ;
      break;
    case 0b001:
      AR &= CR;
      break;
    case 0b010:
      AR &= DR;
      break;
    case 0b011:
      AR &= ER;
      break;
    case 0b100:
      AR &= HR;
      break;
    case 0b101:
      AR &= LR;
      break;
    case 0b111:
      AR &= AR;
      break;
  }
  if (AR) {
    FR = 0b00100000;
  } else {
    FR = 0b10100000;
  }
  cc += 4;
  cc_dec = 4;
  pc++;
}
void ld_a_pa16() {
  AR = get_byte(get_byte(pc + 1) + (uint16_t)(get_byte(pc + 2) << 8));
  cc += 16;
  cc_dec = 16;
  pc += 3;
}

void ld_pa16_a() {
  put_byte(get_byte(pc + 1) + (uint16_t)(get_byte(pc + 2) << 8), AR);
  cc += 16;
  cc_dec = 16;
  pc += 3;
}

void ret_cc() {
  switch (code) {
    case 0xC0: //not zero
      if (!(FR & 0b10000000)) {
        cc += 20;
        cc_dec += 20;
        pc = ((uint16_t)get_byte(sp + 1) << 8) + get_byte(sp);
        sp += 2;
        return;
      }
      break;
    case 0xD0: //not carry
      if (!(FR & 0b00010000)) {
        cc += 20;
        cc_dec += 20;
        pc = ((uint16_t)get_byte(sp + 1) << 8) + get_byte(sp);
        sp += 2;
        return;
      }
      break;
    case 0xC8: //zero
      if (FR & 0b10000000) {
        cc += 20;
        cc_dec += 20;
        pc = ((uint16_t)get_byte(sp + 1) << 8) + get_byte(sp);
        sp += 2;
        return;
      }
      break;
    case 0xD8: //carry
      if (FR & 0b00010000) {
        cc += 20;
        cc_dec += 20;
        pc = ((uint16_t)get_byte(sp + 1) << 8) + get_byte(sp);
        sp += 2;
        return;
      }
      break;
  }
  cc += 8;
  cc_dec = 8;
  pc++;
}
void reti() {
  ime = true;
  pc = ((uint16_t)get_byte(sp + 1) << 8) + get_byte(sp);
  sp += 2;
  cc += 16;
  cc_dec = 16;
}
