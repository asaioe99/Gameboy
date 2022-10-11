void LD_r_n(uint8 code) {
  uint8_t g = (code & 0b00111000) >> 3;
  uint8_t *t;
  switch(g) {
    case 0b000:
      t = &B;
      break;
    case 0b001:
      t = &C;
      break;
    case 0b010:
      t = &D;
      break;
    case 0b011:
      t = &E;
      break;
    case 0b100:
      t = &H;
      break;
    case 0b101:
      t = &L;
      break;
  }
  *t = get_byte(++pc);
  cc += 8;
  pc++;
}

void LD_r_HL(uint8_t code) {
  uint8_t g = (code & 0b00111000) >> 3;
  uint8_t *t;
  switch(g) {
    case 0b000:
      t = &B;
      break;
    case 0b001:
      t = &C;
      break;
    case 0b010:
      t = &D;
      break;
    case 0b011:
      t = &E;
      break;
    case 0b100:
      t = &H;
      break;
    case 0b101:
      t = &L;
      break;
    case 0b111:
      t = &A;
      break;
  }
    *t = get_byte((H << 4) + L);
    cc +=8;
    pc++;
}

void LD_r_r(uint8_t code) {
  uint8_t g1 = (code & 0b00111000) >> 3;
  uint8_t g2 =  code & 0b00000111;
  uint8_t *t1;
  uint8_t *t2;
  switch(g1) {
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
      t1 = &L;
      break;
  }
  switch(g2) {
    case 0b000:
      t2 = &B;
      break;
    case 0b001:
      t2 = &C;
      break;
    case 0b010:
      t2 = &D;
      break;
    case 0b011:
      t2 = &E;
      break;
    case 0b100:
      t2 = &H;
      break;
    case 0b101:
      t2 = &L;
      break;
    case 0b111:
      t2 = &L;
      break;
  }
  *t1 = *t2;
  cc += 4;
  pc++;
}
