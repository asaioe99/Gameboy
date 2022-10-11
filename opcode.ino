
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
