void LD_r_n() {
  switch (code & 0b00111000) {
    case 0b00000000:
      B = get_byte(++pc);
      break;
    case 0b00001000:
      C = get_byte(++pc);
      break;
    case 0b00010000:
      D = get_byte(++pc);
      break;
    case 0b00011000:
      E = get_byte(++pc);
      break;
    case 0b00100000:
      H = get_byte(++pc);
      break;
    case 0b00101000:
      L = get_byte(++pc);
      break;
    case 0b00111000:
      A = get_byte(++pc);
      break;
  }
  cc += 8;
  cc_dec = 8;
  pc++;
}

void LD_r_HL() {
  switch (code & 0b00111000) {
    case 0b00000000:
      B = get_byte(HL(H, L));
      break;
    case 0b00001000:
      C = get_byte(HL(H, L));
      break;
    case 0b00010000:
      D = get_byte(HL(H, L));
      break;
    case 0b00011000:
      E = get_byte(HL(H, L));
      break;
    case 0b00100000:
      H = get_byte(HL(H, L));
      break;
    case 0b00101000:
      L = get_byte(HL(H, L));
      break;
    case 0b00111000:
      A = get_byte(HL(H, L));
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
      t1 = &B;
      break;
    case 0b00001000:
      t1 = &C;
      break;
    case 0b00010000:
      t1 = &D;
      break;
    case 0b00011000:
      t1 = &E;
      break;
    case 0b00100000:
      t1 = &H;
      break;
    case 0b00101000:
      t1 = &L;
      break;
    case 0b00111000:
      t1 = &A;
      break;
  }
  switch (code & 0b00000111) {
    case 0b00000000:
      *t1 = B;
      break;
    case 0b00000001:
      *t1 = C;
      break;
    case 0b00000010:
      *t1 = D;
      break;
    case 0b00000011:
      *t1 = E;
      break;
    case 0b00000100:
      *t1 = H;
      break;
    case 0b00000101:
      *t1 = L;
      break;
    case 0b00000111:
      *t1 = A;
      break;
  }
  cc += 4;
  cc_dec = 4;
  pc++;
}

void LD_HL_r() {
  switch (code & 0b00000111) {
    case 0b00000000:
      put_byte(HL(H, L), B);
      break;
    case 0b00000001:
      put_byte(HL(H, L), C);
      break;
    case 0b00000010:
      put_byte(HL(H, L), D);
      break;
    case 0b00000011:
      put_byte(HL(H, L), E);
      break;
    case 0b00000100:
      put_byte(HL(H, L), H);
      break;
    case 0b00000101:
      put_byte(HL(H, L), L);
      break;
    case 0b00000111:
      put_byte(HL(H, L), A);
      break;
  }
  cc += 8;
  cc_dec = 8;
  pc++;
}

void LD_HL_n() {
  put_byte((uint16_t)(H << 8) + L, get_byte(++pc));
  cc += 12;
  cc_dec = 12;
  pc++;
}
void LD_A_BC() {
  A = get_byte(((uint16_t)B << 8) + C);
  cc += 8;
  cc_dec = 8;
  pc++;
}
void LD_A_DE() {
  A = get_byte(((uint16_t)D << 8) + E);
  cc += 8;
  cc_dec = 8;
  pc++;
}
void LD_A_HLi() {
  A = get_byte(HL(H, L));
  L++;
  if (L == 0x00) H++;
  cc += 8;
  cc_dec = 8;
  pc++;
}
void LD_A_HLd() {
  A = get_byte(HL(H, L));
  L--;
  if (L == 0xFF) H--;
  cc += 8;
  cc_dec = 8;
  pc++;
}
void LD_BC_A() {
  put_byte(((uint16_t)B << 8) + C, A);
  cc += 8;
  cc_dec = 8;
  pc++;
}
void LD_DE_A() {
  put_byte(((uint16_t)D << 8) + E, A);
  cc += 8;
  cc_dec = 8;
  pc++;
}
void LD_HLi_A() {
  put_byte(HL(H, L), A);
  L++;
  if (L == 0x00) H++;
  cc += 8;
  cc_dec = 8;
  pc++;
}
void LD_HLd_A() {
  put_byte(HL(H, L), A);
  L--;
  if (L == 0xFF) H--;
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
      C++;
      if (C == 0x00) B++;
      break;
    case 0b01:
      E++;
      if (E == 0x00) D++;
      break;
    case 0b10:
      L++;
      if (L == 0x00) H++;
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
      C--;
      if (C == 0xFF) B--;
      break;
    case 0b00010000:
      E--;
      if (E == 0xFF) D--;
      break;
    case 0b00100000:
      L--;
      if (L == 0xFF) H--;
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
      r = &B;
      break;
    case 0b00001000:
      r = &C;
      break;
    case 0b00010000:
      r = &D;
      break;
    case 0b00011000:
      r = &E;
      break;
    case 0b00100000:
      r = &H;
      break;
    case 0b00101000:
      r = &L;
      break;
    case 0b00111000:
      r = &A;
      break;
  }
  t = *r;
  *r = *r + 1;
  if (*r == 0) {
    F |= 0b10000000;
    F &= 0b10110000;
  } else {
    F &= 0b00110000;
  }
  if ((t & 0x0F) == 0x0F) {
    F |= 0b00100000;
  } else {
    F &= 0b11010000;
  }
  cc += 4;
  cc_dec = 4;
  pc++;
}
void DEC_r() {
  uint8_t *r;
  switch (code & 0b00111000) {
    case 0b00000000:
      r = &B;
      break;
    case 0b00001000:
      r = &C;
      break;
    case 0b00010000:
      r = &D;
      break;
    case 0b00011000:
      r = &E;
      break;
    case 0b00100000:
      r = &H;
      break;
    case 0b00101000:
      r = &L;
      break;
    case 0b00111000:
      r = &A;
      break;
  }
  *r = *r - 1;
  if (*r == 0) {
    F |= 0b11000000;
  } else {
    F &= 0b01111111;
    F |= 0b01000000;
  }
  if (*r & 0x0F == 0x0F) {
    F |= 0b00100000;
  } else {
    F &= 0b11010000;
  }
  cc += 4;
  cc_dec = 4;
  pc++;
}
void XOR_r() {
  switch (code & 0b00000111) {
    case 0b000:
      A = A ^ B;
      break;
    case 0b001:
      A = A ^ C;
      break;
    case 0b010:
      A = A ^ D;
      break;
    case 0b011:
      A = A ^ E;
      break;
    case 0b100:
      A = A ^ H;
      break;
    case 0b101:
      A = A ^ L;
      break;
    case 0b110:
      A = A ^ get_byte(HL(H, L));
      cc += 4;
      break;
    case 0b111:
      A = A ^ A;
      break;
  }
  if (A == 0) {
    F = 0b10000000;
  } else {
    F = 0b00000000;
  }
  cc += 4;
  cc_dec = 4;
  pc++;
}
void SUB_r() {
  uint8_t val;
  switch (code & 0b00000111) {
    case 0b000:
      val = B;
      break;
    case 0b001:
      val = C;
      break;
    case 0b010:
      val = D;
      break;
    case 0b011:
      val = E;
      break;
    case 0b100:
      val = H;
      break;
    case 0b101:
      val = L;
      break;
    case 0b110:
      val = get_byte(HL(H, L));
      cc += 4;
      cc_dec = 4;
      break;
    case 0b111:
      val = A;
      break;
  }
  if (A & 0x0F < val & 0x0F) {
    F = 0b01100000;
  } else {
    F = 0b01000000;
  }
  if (A < val) {
    F |= 0b00010000;
  }
  A = A - val;
  if (A == 0) F |= 0b10000000;
  cc += 4;
  cc_dec += 4;
  pc++;
}

void ADD_r() {
  uint8_t val;
  switch (code & 0b00000111) {
    case 0b000:
      val = B;
      break;
    case 0b001:
      val = C;
      break;
    case 0b010:
      val = D;
      break;
    case 0b011:
      val = E;
      break;
    case 0b100:
      val = H;
      break;
    case 0b101:
      val = L;
      break;
    case 0b110:
      val = get_byte(HL(H, L));
      cc += 4;
      cc_dec = 4;
      break;
    case 0b111:
      val = A;
      break;
  }
  if (A & 0x0F + val & 0x0F > 0x0F) {
    F = 0b00100000;
  } else {
    F = 0b00000000;
  }
  if (A + val > 0xFF) {
    F |= 0b00010000;
  }
  A = A + val;
  if (A == 0) F |= 0b10000000;
  cc += 4;
  cc_dec += 4;
  pc++;
}

void CP_r() {
  uint8_t val;
  switch (code & 0b00000111) {
    case 0b000:
      val = B;
      break;
    case 0b001:
      val = C;
      break;
    case 0b010:
      val = D;
      break;
    case 0b011:
      val = E;
      break;
    case 0b100:
      val = H;
      break;
    case 0b101:
      val = L;
      break;
    case 0b110:
      val = get_byte(HL(H, L));
      cc += 4;
      cc_dec = 4;
      break;
    case 0b111:
      val = A;
      break;
  }
  if (A & 0x0F < val & 0x0F) {
    F = 0b01100000;
  } else {
    F = 0b01000000;
  }
  if (A < val) {
    F |= 0b00010000;
  }
  if (A == val) F |= 0b10000000;
  cc += 4;
  cc_dec += 4;
  pc++;
}

void JR_cc_d8() {
  switch (code) {
    case 0x18:
      pc = pc + 2 + (int8_t)get_byte(pc + 1);
      cc += 12;
      cc_dec = 4;
      break;
    case 0x20:
      if (F >> 7 == 0) {
        pc = pc + 2 + (int8_t)get_byte(pc + 1);
        cc += 12;
      } else {
        cc += 8;
        pc += 2;
      }
      break;
    case 0x28:
      if ((F & 0b10000000) >> 7) {
        pc = pc + 2 + (int8_t)get_byte(pc + 1);
        cc += 12;
      } else {
        cc += 8;
        pc += 2;
      }
      break;
    case 0x30:
      if (F & 0b00010000 == 0) {
        pc = pc + 2 + (int8_t)get_byte(pc + 1);
        cc += 12;
      } else {
        cc += 8;
        pc += 2;
      }
      break;
    case 0x38:
      if (F & 0b00010000) {
        pc = pc + 2 + (int8_t)get_byte(pc + 1);
        cc += 12;
        cc_dec = 4;
      } else {
        cc += 8;
        pc += 2;
      }
      break;
  }
  cc_dec += 8;
}

void LD_r16_d16() {
  switch ((code & 0b00110000) >> 4) {
    case 0b00:
      C = get_byte(++pc);
      B = get_byte(++pc);
      break;
    case 0b01:
      E = get_byte(++pc);
      D = get_byte(++pc);
      break;
    case 0b10:
      L = get_byte(++pc);
      H = get_byte(++pc);
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

  if (A >> 7) {
    A = (A << 1) + ((F & 0b00010000) >> 4);
    F = 0b00010000;
  } else {
    A = (A << 1) + ((F & 0b00010000) >> 4);
    F = 0b00000000;
  }
  cc += 4;
  cc_dec = 4;
  pc++;
}

void LDH_a8_a() {
  put_byte(0xFF00 + get_byte(++pc), A);
  cc += 12;
  cc_dec = 12;
  pc++;
}

void LDH_a_a8() {
  A = get_byte(0xFF00 + get_byte(++pc));
  cc += 12;
  cc_dec = 12;
  pc++;
}

void POP_r16() {
  switch ((code & 0b00110000) >> 4) {
    case 0b00:
      C = get_byte(sp++);
      B = get_byte(sp++);
      break;
    case 0b01:
      E = get_byte(sp++);
      D = get_byte(sp++);
      break;
    case 0b10:
      L = get_byte(sp++);
      H = get_byte(sp++);
      break;
    case 0b11:
      F = get_byte(sp++) & 0xF0;
      A = get_byte(sp++);
      break;
  }
  cc += 12;
  cc_dec = 12;
  pc++;
}

void LDH_c_a() {
  put_byte(0xFF + C, A);
  cc += 8;
  cc_dec = 8;
  pc++;
}

void LDH_a_c() {
  A = get_byte(0xFF00 + C);
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
    case 0b00:
      put_byte(--sp, B);
      put_byte(--sp, C);
      break;
    case 0b01:
      put_byte(--sp, D);
      put_byte(--sp, E);
      break;
    case 0b10:
      put_byte(--sp, H);
      put_byte(--sp, L);
      break;
    case 0b11:
      put_byte(--sp, A);
      put_byte(--sp, F);
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

void LD_a16p_a() {
  put_byte(get_byte(pc + 1) + ((uint16_t)get_byte(pc + 2) << 8), A);
  cc += 16;
  cc_dec = 16;
  pc += 3;
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
  if (A & 0x0F < val & 0x0F) {
    F = 0b01100000;
  } else {
    F = 0b01000000;
  }
  if (A < val) {
    F |= 0b00010000;
  }
  if (A == val) F |= 0b10000000;
  cc += 8;
  cc_dec = 8;
  pc++;
}

void RL_r() {
  uint8_t* r;
  uint8_t t;
  switch (code & 0b00000111) {
    case 0b000:
      r = &B;
      break;
    case 0b001:
      r = &C;
      break;
    case 0b010:
      r = &D;
      break;
    case 0b011:
      r = &E;
      break;
    case 0b100:
      r = &H;
      break;
    case 0b101:
      r = &L;
      break;
    case 0b111:
      r = &A;
      break;
  }
  if (*r >> 7) {
    *r = (*r << 1) + ((F & 0b00010000) >> 4);
    F = 0b00010000;
  } else {
    *r = (*r << 1) + ((F & 0b00010000) >> 4);
    F = 0b00000000;
  }
  if (*r == 0) F | 0b10000000;
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
      r = B;
      break;
    case 0b001:
      r = C;
      break;
    case 0b010:
      r = D;
      break;
    case 0b011:
      r = E;
      break;
    case 0b100:
      r = H;
      break;
    case 0b101:
      r = L;
      break;
    case 0b111:
      r = A;
      break;
  }
  r = r & mask;
  if (r == mask) {
    F = F & 0b01111111;
  } else {
    F = F | 0b10000000;
  }
  F = F & 0b10010000;
  cc += 8;
  cc_dec = 8;
  pc++;
}
