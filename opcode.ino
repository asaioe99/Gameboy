
// 0x06
void LD_B_n() {
  pc++;
  B = get_byte(pc);
  cc += 8;
  pc++;
}
// 0x0E
void LD_C_n() {
  pc++;
  C = get_byte(pc);
  cc += 8;
  pc++;
}
// 0x16
void LD_D_n() {
  pc++;
  D = get_byte(pc);
  cc += 8;
  pc++;
}
// 0x1E
void LD_E_n() {
  pc++;
  E = get_byte(pc);
  cc += 8;
  pc++;
}
// 0x26
void LD_H_n() {
  pc++;
  H = get_byte(pc);
  cc += 8;
  pc++;
}
// 0x2E
void LD_L_n() {
  pc++;
  L = get_byte(pc);
  cc += 8;
  pc++;
}
// 0x7F
void LD_A_A() {
    A = A;
    cc +=4;
    pc++;
}
// 0x78
void LD_A_B() {
    A = B;
    cc +=4;
    pc++;
}
// 0x79
void LD_A_C() {
    A = C;
    cc +=4;
    pc++;
}
// 0x7A
void LD_A_D() {
    A = D;
    cc +=4;
    pc++;
}
// 0x7B
void LD_A_E() {
    A = E;
    cc +=4;
    pc++;
}
// 0x7C
void LD_A_H() {
    A = H;
    cc +=4;
    pc++;
}
// 0x7D
void LD_A_L() {
    A = L;
    cc +=4;
    pc++;
}
// 0x7E
void LD_A_lHLl() {
    A = get_byte((H << 4) + L);
    cc +=8;
    pc++;
}
