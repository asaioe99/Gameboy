void LD_r_n(uint8_t code) {
  switch((code & 0b00111000) >> 3) {
    case 0b000:
      B = get_byte(++pc);
      break;
    case 0b001:
      C = get_byte(++pc);
      break;
    case 0b010:
      D = get_byte(++pc);
      break;
    case 0b011:
      E = get_byte(++pc);
      break;
    case 0b100:
      H = get_byte(++pc);
      break;
    case 0b101:
      L = get_byte(++pc);
      break;
    case 0b111:
      A = get_byte(++pc);
      break;
  }
  cc += 8;
  pc++;
}

void LD_r_HL(uint8_t code) {
  switch((code & 0b00111000) >> 3) {
    case 0b000:
      B = get_byte(((uint16_t)H << 4) + L);
      break;
    case 0b001:
      C = get_byte(((uint16_t)H << 4) + L);
      break;
    case 0b010:
      D = get_byte(((uint16_t)H << 4) + L);
      break;
    case 0b011:
      E = get_byte(((uint16_t)H << 4) + L);
      break;
    case 0b100:
      H = get_byte(((uint16_t)H << 4) + L);
      break;
    case 0b101:
      L = get_byte(((uint16_t)H << 4) + L);
      break;
    case 0b111:
      A = get_byte(((uint16_t)H << 4) + L);
      break;
  }
    cc += 8;
    pc++;
}

void LD_r_r(uint8_t code) {
  uint8_t *t1;
  switch((code & 0b00111000) >> 3) {
    case 0b000:
      t1 = &B;
      break;
    case 0b001:
      t1 = &C;
      break;
    case 0b010:
      t1 = &D;
      break;
    case 0b011:
      t1 = &E;
      break;
    case 0b100:
      t1 = &H;
      break;
    case 0b101:
      t1 = &L;
      break;
    case 0b111:
      t1 = &A;
      break;
  }
  switch(code & 0b00000111) {
    case 0b000:
      *t1 = B;
      break;
    case 0b001:
      *t1 = C;
      break;
    case 0b010:
      *t1 = D;
      break;
    case 0b011:
      *t1 = E;
      break;
    case 0b100:
      *t1 = H;
      break;
    case 0b101:
      *t1 = L;
      break;
    case 0b111:
      *t1 = A;
      break;
  }
  cc += 4;
  pc++;
}

void LD_HL_r(uint8_t code) {
    switch(code & 0b00000111) {
    case 0b000:
      put_byte(((uint16_t)H << 4) + L, B);
      break;
    case 0b001:
      put_byte(((uint16_t)H << 4) + L, C);
      break;
    case 0b010:
      put_byte(((uint16_t)H << 4) + L, D);
      break;
    case 0b011:
      put_byte(((uint16_t)H << 4) + L, E);
      break;
    case 0b100:
      put_byte(((uint16_t)H << 4) + L, H);
      break;
    case 0b101:
      put_byte(((uint16_t)H << 4) + L, L);
      break;
    case 0b111:
      put_byte(((uint16_t)H << 4) + L, A);
      break;  
    }
    cc += 8;
    pc++;
}

void LD_HL_n(uint8_t code) {
    put_byte((uint16_t)(H << 4) + L, get_byte(++pc));
    cc += 12;
    pc++;
}
void LD_A_BC() {
    A = get_byte(((uint16_t)B << 4) + C);
    cc += 8;
    pc++;
}
void LD_A_DE() {
    A = get_byte(((uint16_t)D << 4) + E);
    cc += 8;
    pc++;
}
void LD_A_HLi() {
    A = get_byte(((uint16_t)H << 4) + L);
    L++;
    if (L == 0x00) H++;
    cc += 8;
    pc++;
}
void LD_A_HLd() {
    A = get_byte(((uint16_t)H << 4) + L);
    L--;
    if (L == 0xFF) H--;
    cc += 8;
    pc++;
}
void LD_BC_A() {
    put_byte(((uint16_t)B << 4) + C, A);
    cc += 8;
    pc++;
}
void LD_DE_A() {
    put_byte(((uint16_t)D << 4) + E, A);
    cc += 8;
    pc++;
}
void LD_HLi_A() {
    put_byte(((uint16_t)H << 4) + L, A);
    L++;
    if (L == 0x00) H++;
    cc += 8;
    pc++;
}
void LD_HLd_A() {
    put_byte(((uint16_t)H << 4) + L, A);
    L--;
    if (L == 0xFF) H--;
    cc += 8;
    pc++;
}
void NOP() {
    cc += 4;
    pc++;
}
void INC_rr(uint8_t code) {
    switch((code & 0b00110000) >> 4) {
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
    pc++;
}
void DEC_rr(uint8_t code) {
    switch((code & 0b00110000) >> 4) {
    case 0b00:
      C--;
      if (C == 0xFF) B--;
      break;
    case 0b01:
      E--;
      if (E == 0xFF) D--;
      break;
    case 0b10:
      L--;
      if (L == 0xFF) H--;
      break;
    case 0b11:
      sp--;
      break;
    }
    cc += 8;
    pc++;
}
void INC_r(uint8_t code) {
    switch((code & 0b00111000) >> 3) {
    case 0b000:
      B++;
      break;
    case 0b001:
      C++;
      break;
    case 0b010:
      D++;
      break;
    case 0b011:
      E++;
      break;
    case 0b100:
      H++;
      break;
    case 0b101:
      L++;
      break;
    case 0b111:
      A++;
      break;  
    }
    cc += 4;
    pc++;
}
void DEC_r(uint8_t code) {
    switch((code & 0b00111000) >> 3) {
    case 0b000:
      B--;
      break;
    case 0b001:
      C--;
      break;
    case 0b010:
      D--;
      break;
    case 0b011:
      E--;
      break;
    case 0b100:
      H--;
      break;
    case 0b101:
      L--;
      break;
    case 0b111:
      A--;
      break;  
    }
    cc += 4;
    pc++;
}
void XOR_r(uint8_t code) {
    switch(code & 0b00000111) {
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
      A = A ^ get_byte(((uint16_t)H << 4) + L);
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
    pc++;
}
void SUB_r(uint8_t code) {
  uint8_t val;  
    switch(code & 0b00000111) {
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
      val = get_byte(((uint16_t)H << 4) + L);
      cc += 4;
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
    pc++;
}

void ADD_r(uint8_t code) {
  uint8_t val;  
    switch(code & 0b00000111) {
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
      val = get_byte(((uint16_t)H << 4) + L);
      cc += 4;
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
    pc++;
}

void CP_r(uint8_t code) {
  uint8_t val;  
    switch(code & 0b00000111) {
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
      val = get_byte(((uint16_t)H << 4) + L);
      cc += 4;
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
    pc++;
}

void JR_cc_d8(uint8_t code) {
  switch(code) {
    case 0x18:
      pc = pc + 2 + get_byte(pc + 1);
      cc += 12;
      break;
    case 0x20:
      if (F >> 7 == 0) {
        pc = pc + 2 + get_byte(pc + 1);
        cc += 12;
      } else {
        cc += 8;
        pc += 2;
      }
      break;
    case 0x28:
      if ((F & 0b10000000) >> 7) {
        pc = pc + 2 + get_byte(pc + 1);
        cc += 12;
      } else {
        cc += 8;
        pc += 2;
      }
      break;
    case 0x30:
      if (F & 0b00010000 == 0) {
        pc = pc + 2 + get_byte(pc + 1);
        cc += 12;
      } else {
        cc += 8;
        pc += 2;
      }
      break;
    case 0x38:
      if (F & 0b00010000) {
        pc = pc + 2 + get_byte(pc + 1);
        cc += 12;
      } else {
        cc += 8;
        pc += 2;
      }
      break;
  }
}

void LD_r16_d16(uint8_t code) {
  switch((code & 0b00110000) >> 4) {
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
      SP = get_byte(++pc);
      SP += get_byte(++pc) << 4;
      break;
  }
  cc += 12;
  pc++;
}

void RLA() {
  if (A >> 7) {
    A = (A << 1) + 1;
    F = 0b00010000;
  } else {
    A = A << 1;
  }
  cc += 4;
  pc++;
}

void LDH_a8_a(uint8_t code) {
  put_byte(0xFF00 + get_byte(++pc), A);
  cc += 12;
  pc++;
}

void LDH_a_a8(uint8_t code) {
  A = get_byte(0xFF00 + get_byte(++pc));
  cc += 12;
  pc++;
}

void POP_r16(uint8_t code) {
  switch((code & 0b00110000) >> 4) {
    case 0b00:
      C = get_byte(SP++);
      B = get_byte(SP++);
      break;
    case 0b01:
      E = get_byte(SP++);
      D = get_byte(SP++);
      break;
    case 0b10:
      L = get_byte(SP++);
      H = get_byte(SP++);
      break;
    case 0b11:
      F = get_byte(SP++) & 0xF0;
      A = get_byte(SP++);
      break;
  }
  cc += 12;
  pc++;
}

void LDH_c_a() {
  put_byte(0xFF + C, A);
  cc += 8;
  pc++;
}

void LDH_a_c() {
  A = get_byte(0xFF + C);
  cc += 8;
  pc++;
}

void DI() {
  ime = 0;
  cc += 4;
  pc++;
}

void EI() {
  ime = 1;
  cc += 4;
  pc++;
}

void PUSH_r16(uint8_t code) {
  switch((code & 0b00110000) >> 4) {
    case 0b00:
      put_byte(SP--, B);
      put_byte(SP--, C);
      break;
    case 0b01:
      put_byte(SP--, D);
      put_byte(SP--, E);
      break;
    case 0b10:
      put_byte(SP--, H);
      put_byte(SP--, L);
      break;
    case 0b11:
      put_byte(SP--, A);
      put_byte(SP--, F);
      break;
  }
  cc += 16;
  pc++;
}
void RET() {
  pc = (get_byte(SP + 1) << 4) + get_byte(SP);
  SP -= 2;
  cc += 16;
}

void LD_a16p_a() {
  put_byte(get_byte(pc+1) + (get_byte(pc+2) << 4), A);
  cc += 16;
  pc += 3;
}

void CALL() {
  put_byte(--SP, get_byte(pc + 4));
  put_byte(--SP, get_byte(pc + 3));
  cc += 24;
  pc += 3;
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
  pc++;
}
