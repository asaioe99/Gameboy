void LD_r_n(uint8 code) {
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
  }
  cc += 8;
  pc++;
}

void LD_r_HL(uint8_t code) {
  switch((code & 0b00111000) >> 3) {
    case 0b000:
      B = get_byte((H << 4) + L);
      break;
    case 0b001:
      C = get_byte((H << 4) + L);
      break;
    case 0b010:
      D = get_byte((H << 4) + L);
      break;
    case 0b011:
      E = get_byte((H << 4) + L);
      break;
    case 0b100:
      H = get_byte((H << 4) + L);
      break;
    case 0b101:
      L = get_byte((H << 4) + L);
      break;
    case 0b111:
      A = get_byte((H << 4) + L);
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
