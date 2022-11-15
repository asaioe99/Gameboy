uint8_t cpu_step() {

  uint8_t total_clock = 0;
  tmp_clock = 0;

  if (flag_halt) {
    tmp_clock += 16;
  } else {
    execute();
  }

  total_clock += tmp_clock;

  // mmu.update??
  mmu_update(tmp_clock);

  if (ime) {
    tmp_clock = 0;
    int_check();
    //mmu.update
    mmu_update(tmp_clock);

    total_clock += tmp_clock;
  }
  //mmu_update(tmp_clock);
  //total_clock += tmp_clock;

  return total_clock;
}

void int_check() {
  for (uint8_t i = 0; i < 5; i++) {
    if ((IF & (1 << i)) && (IE & (1 << i))) {
      IF &= ~(1 << i); // reset
      ime = 0; //割り込み無効化
      flag_halt = 0; //不明
      switch (i) { //割り込みの優先順位はこの通り
        case 0:
          call_irpt(0x0040); // v-blank 実装済み
          break;
        case 1:
          call_irpt(0x0048); // LCD 実装済み？ -> できていないかも
          break;
        case 2:
          call_irpt(0x0050); // timer 実装したつもり -> できてないかも
          break;
        case 3:
          call_irpt(0x0058); // serial Rustの実装では0x0080 未実装
          break;
        case 4:
          call_irpt(0x0060); // joypad Rustの実装では0x0070
          break;
      }
      break;
    }
  }
}

static inline void execute() {

  code = mmu_read(pc);

  if (pc == 0x100) {
    //chk_init_regs();
    boot = false;
  }

  //if (pc >= 0x213 && pc <= 0x23E && !boot) {
  //  sprintf(buf_b1, "pc:%04X->%02X AF:%02X%02X BC:%02X%02X DE:%02X%02X HL:%02X%02X sp:%04X", pc, code, AR, FR, BR, CR, DR, ER, HR, LR, sp);
  //sprintf(buf_b1, "OAM0:%02X %02X %02X %02X", mmu_read(0xFE00), mmu_read(0xFE01), mmu_read(0xFE02), mmu_read(0xFE03));
  //  Serial.println(buf_b1);
  //}

  if (code == 0xCB) {
    code = mmu_read(++pc);
    pf_op_ptr_array[code]();
  } else {
    op_ptr_array[code]();
  }
}

void ld_br_d8() {
  BR = mmu_read(++pc);
  tmp_clock += 8;
  pc++;
}

void ld_cr_d8() {
  CR = mmu_read(++pc);
  tmp_clock += 8;
  pc++;
}

void ld_dr_d8() {
  DR = mmu_read(++pc);
  tmp_clock += 8;
  pc++;
}

void ld_er_d8() {
  ER = mmu_read(++pc);
  tmp_clock += 8;
  pc++;
}

void ld_hr_d8() {
  HR = mmu_read(++pc);
  tmp_clock += 8;
  pc++;
}

void ld_lr_d8() {
  LR = mmu_read(++pc);
  tmp_clock += 8;
  pc++;
}

void ld_ar_d8() {
  AR = mmu_read(++pc);
  tmp_clock += 8;
  pc++;
}

void ld_br_phl() {
  BR = mmu_read(HL(HR, LR));
  tmp_clock += 8;
  pc++;
}

void ld_cr_phl() {
  CR = mmu_read(HL(HR, LR));
  tmp_clock += 8;
  pc++;
}

void ld_dr_phl() {
  DR = mmu_read(HL(HR, LR));
  tmp_clock += 8;
  pc++;
}

void ld_er_phl() {
  ER = mmu_read(HL(HR, LR));
  tmp_clock += 8;
  pc++;
}

void ld_hr_phl() {
  HR = mmu_read(HL(HR, LR));
  tmp_clock += 8;
  pc++;
}

void ld_lr_phl() {
  LR = mmu_read(HL(HR, LR));
  tmp_clock += 8;
  pc++;
}

void ld_ar_phl() {
  AR = mmu_read(HL(HR, LR));
  tmp_clock += 8;
  pc++;
}

void ld_br_br() {
  tmp_clock += 4;
  pc++;
}

void ld_br_cr() {
  BR = CR;
  tmp_clock += 4;
  pc++;
}

void ld_br_dr() {
  BR = DR;
  tmp_clock += 4;
  pc++;
}

void ld_br_er() {
  BR = ER;
  tmp_clock += 4;
  pc++;
}

void ld_br_hr() {
  BR = HR;
  tmp_clock += 4;
  pc++;
}

void ld_br_lr() {
  BR = LR;
  tmp_clock += 4;
  pc++;
}

void ld_br_ar() {
  BR = AR;
  tmp_clock += 4;
  pc++;
}

void ld_cr_br() {
  CR = BR;
  tmp_clock += 4;
  pc++;
}

void ld_cr_cr() {
  tmp_clock += 4;
  pc++;
}

void ld_cr_dr() {
  CR = DR;
  tmp_clock += 4;
  pc++;
}

void ld_cr_er() {
  CR = ER;
  tmp_clock += 4;
  pc++;
}

void ld_cr_hr() {
  CR = HR;
  tmp_clock += 4;
  pc++;
}

void ld_cr_lr() {
  CR = LR;
  tmp_clock += 4;
  pc++;
}

void ld_cr_ar() {
  CR = AR;
  tmp_clock += 4;
  pc++;
}

void ld_dr_br() {
  DR = BR;
  tmp_clock += 4;
  pc++;
}

void ld_dr_cr() {
  DR = CR;
  tmp_clock += 4;
  pc++;
}

void ld_dr_dr() {
  tmp_clock += 4;
  pc++;
}

void ld_dr_er() {
  DR = ER;
  tmp_clock += 4;
  pc++;
}

void ld_dr_hr() {
  DR = HR;
  tmp_clock += 4;
  pc++;
}

void ld_dr_lr() {
  DR = LR;
  tmp_clock += 4;
  pc++;
}

void ld_dr_ar() {
  DR = AR;
  tmp_clock += 4;
  pc++;
}

void ld_er_br() {
  ER = BR;
  tmp_clock += 4;
  pc++;
}

void ld_er_cr() {
  ER = CR;
  tmp_clock += 4;
  pc++;
}

void ld_er_dr() {
  ER = DR;
  tmp_clock += 4;
  pc++;
}

void ld_er_er() {
  tmp_clock += 4;
  pc++;
}

void ld_er_hr() {
  ER = HR;
  tmp_clock += 4;
  pc++;
}

void ld_er_lr() {
  ER = LR;
  tmp_clock += 4;
  pc++;
}

void ld_er_ar() {
  ER = AR;
  tmp_clock += 4;
  pc++;
}

void ld_hr_br() {
  HR = BR;
  tmp_clock += 4;
  pc++;
}

void ld_hr_cr() {
  HR = CR;
  tmp_clock += 4;
  pc++;
}

void ld_hr_dr() {
  HR = DR;
  tmp_clock += 4;
  pc++;
}

void ld_hr_er() {
  HR = ER;
  tmp_clock += 4;
  pc++;
}

void ld_hr_hr() {
  tmp_clock += 4;
  pc++;
}

void ld_hr_lr() {
  HR = LR;
  tmp_clock += 4;
  pc++;
}

void ld_hr_ar() {
  HR = AR;
  tmp_clock += 4;
  pc++;
}

void ld_lr_br() {
  LR = BR;
  tmp_clock += 4;
  pc++;
}

void ld_lr_cr() {
  LR = CR;
  tmp_clock += 4;
  pc++;
}

void ld_lr_dr() {
  LR = DR;
  tmp_clock += 4;
  pc++;
}

void ld_lr_er() {
  LR = ER;
  tmp_clock += 4;
  pc++;
}

void ld_lr_hr() {
  LR = HR;
  tmp_clock += 4;
  pc++;
}

void ld_lr_lr() {
  tmp_clock += 4;
  pc++;
}

void ld_lr_ar() {
  LR = AR;
  tmp_clock += 4;
  pc++;
}

void ld_ar_br() {
  AR = BR;
  tmp_clock += 4;
  pc++;
}

void ld_ar_cr() {
  AR = CR;
  tmp_clock += 4;
  pc++;
}

void ld_ar_dr() {
  AR = DR;
  tmp_clock += 4;
  pc++;
}

void ld_ar_er() {
  AR = ER;
  tmp_clock += 4;
  pc++;
}

void ld_ar_hr() {
  AR = HR;
  tmp_clock += 4;
  pc++;
}

void ld_ar_lr() {
  AR = LR;
  tmp_clock += 4;
  pc++;
}

void ld_ar_ar() {
  tmp_clock += 4;
  pc++;
}

void ld_phl_br() {
  mmu_write(HL(HR, LR), BR);
  tmp_clock += 8;
  pc++;
}

void ld_phl_cr() {
  mmu_write(HL(HR, LR), CR);
  tmp_clock += 8;
  pc++;
}

void ld_phl_dr() {
  mmu_write(HL(HR, LR), DR);
  tmp_clock += 8;
  pc++;
}

void ld_phl_er() {
  mmu_write(HL(HR, LR), ER);
  tmp_clock += 8;
  pc++;
}

void ld_phl_hr() {
  mmu_write(HL(HR, LR), HR);
  tmp_clock += 8;
  pc++;
}

void ld_phl_lr() {
  mmu_write(HL(HR, LR), LR);
  tmp_clock += 8;
  pc++;
}

void ld_phl_ar() {
  mmu_write(HL(HR, LR), AR);
  tmp_clock += 8;
  pc++;
}

void ld_phl_d8() {
  mmu_write(HL(HR, LR), mmu_read(++pc));
  tmp_clock += 12;
  pc++;
}

void ld_ar_pr16() {
  switch (code) {
    case 0x0A:
      AR = mmu_read(BC(BR, CR));
      break;
    case 0x1A:
      AR = mmu_read(DE(DR, ER));
      break;
  }
  tmp_clock += 8;
  pc++;
}

void ld_ar_phli() {
  AR = mmu_read(HL(HR, LR));
  LR++;
  if (LR == 0x00) HR++;
  tmp_clock += 8;

  pc++;
}
void ld_ar_phld() {
  AR = mmu_read(HL(HR, LR));
  LR--;
  if (LR == 0xFF) HR--;
  tmp_clock += 8;

  pc++;
}
void ld_pbc_ar() {
  mmu_write(BC(BR, CR), AR);
  tmp_clock += 8;
  pc++;
}

void ld_pde_ar() {
  mmu_write(DE(DR, ER), AR);
  tmp_clock += 8;
  pc++;
}

void ld_phli_ar() {
  mmu_write(HL(HR, LR), AR);
  LR++;
  if (!LR) HR++;
  tmp_clock += 8;
  pc++;
}

void ld_phld_ar() {
  mmu_write(HL(HR, LR), AR);
  LR--;
  if (LR == 0xFF) HR--;
  tmp_clock += 8;

  pc++;
}
void nop() {
  tmp_clock += 4;
  pc++;
}

void inc_bc() {
  CR++;
  if (CR == 0x00) BR++;
  tmp_clock += 8;
  pc++;
}

void inc_de() {
  ER++;
  if (ER == 0x00) DR++;
  tmp_clock += 8;
  pc++;
}

void inc_hl() {
  LR++;
  if (LR == 0x00) HR++;
  tmp_clock += 8;
  pc++;
}

void inc_sp() {
  sp++;
  tmp_clock += 8;
  pc++;
}

void dec_bc() {
  CR--;
  if (CR == 0xFF) BR--;
  tmp_clock += 8;
  pc++;
}

void dec_de() {
  ER--;
  if (ER == 0xFF) DR--;
  tmp_clock += 8;
  pc++;
}

void dec_hl() {
  LR--;
  if (LR == 0xFF) HR--;
  tmp_clock += 8;
  pc++;
}

void dec_sp() {
  sp--;
  tmp_clock += 8;
  pc++;
}

void inc_br() {
  uint8_t t = BR;
  BR++;
  if (BR == 0) {
    FR |= 0x80;
    FR &= 0b10110000;
  } else {
    FR &= 0x30;
  }
  if ((t & 0x0F) == 0x0F) {
    FR |= 0x20;
  } else {
    FR &= 0b11010000;
  }
  tmp_clock += 4;
  pc++;
}

void inc_cr() {
  uint8_t t = CR;
  CR++;
  if (CR == 0) {
    FR |= 0x80;
    FR &= 0b10110000;
  } else {
    FR &= 0x30;
  }
  if ((t & 0x0F) == 0x0F) {
    FR |= 0x20;
  } else {
    FR &= 0b11010000;
  }
  tmp_clock += 4;
  pc++;
}

void inc_dr() {
  uint8_t t = DR;
  DR++;
  if (DR == 0) {
    FR |= 0x80;
    FR &= 0b10110000;
  } else {
    FR &= 0x30;
  }
  if ((t & 0x0F) == 0x0F) {
    FR |= 0x20;
  } else {
    FR &= 0b11010000;
  }
  tmp_clock += 4;
  pc++;
}

void inc_er() {
  uint8_t t = ER;
  ER++;
  if (ER == 0) {
    FR |= 0x80;
    FR &= 0b10110000;
  } else {
    FR &= 0x30;
  }
  if ((t & 0x0F) == 0x0F) {
    FR |= 0x20;
  } else {
    FR &= 0b11010000;
  }
  tmp_clock += 4;
  pc++;
}

void inc_hr() {
  uint8_t t = HR;
  HR++;
  if (HR == 0) {
    FR |= 0x80;
    FR &= 0b10110000;
  } else {
    FR &= 0x30;
  }
  if ((t & 0x0F) == 0x0F) {
    FR |= 0x20;
  } else {
    FR &= 0b11010000;
  }
  tmp_clock += 4;
  pc++;
}

void inc_lr() {
  uint8_t t = LR;
  LR++;
  if (LR == 0) {
    FR |= 0x80;
    FR &= 0b10110000;
  } else {
    FR &= 0x30;
  }
  if ((t & 0x0F) == 0x0F) {
    FR |= 0x20;
  } else {
    FR &= 0b11010000;
  }
  tmp_clock += 4;
  pc++;
}

void inc_ar() {
  uint8_t t = AR;
  AR++;
  if (AR == 0) {
    FR |= 0x80;
    FR &= 0b10110000;
  } else {
    FR &= 0x30;
  }
  if ((t & 0x0F) == 0x0F) {
    FR |= 0x20;
  } else {
    FR &= 0b11010000;
  }
  tmp_clock += 4;
  pc++;
}

void dec_br() {
  uint8_t t = BR ;
  BR--;
  if (BR == 0) {
    FR |= 0xC0;
  } else {
    FR &= 0x7F;
    FR |= 0x40;
  }
  if ((t & 0x0F) == 0x00) {
    FR |= 0x20;
  } else {
    FR &= 0xD0;
  }
  tmp_clock += 4;
  pc++;
}
void dec_cr() {
  uint8_t t = CR ;
  CR--;
  if (CR == 0) {
    FR |= 0xC0;
  } else {
    FR &= 0x7F;
    FR |= 0x40;
  }
  if ((t & 0x0F) == 0x00) {
    FR |= 0x20;
  } else {
    FR &= 0xD0;
  }
  tmp_clock += 4;
  pc++;
}
void dec_dr() {
  uint8_t t = DR ;
  DR--;
  if (DR == 0) {
    FR |= 0xC0;
  } else {
    FR &= 0x7F;
    FR |= 0x40;
  }
  if ((t & 0x0F) == 0x00) {
    FR |= 0x20;
  } else {
    FR &= 0xD0;
  }
  tmp_clock += 4;
  pc++;
}
void dec_er() {
  uint8_t t = ER ;
  ER--;
  if (ER == 0) {
    FR |= 0xC0;
  } else {
    FR &= 0x7F;
    FR |= 0x40;
  }
  if ((t & 0x0F) == 0x00) {
    FR |= 0x20;
  } else {
    FR &= 0xD0;
  }
  tmp_clock += 4;
  pc++;
}
void dec_hr() {
  uint8_t t = HR ;
  HR--;
  if (HR == 0) {
    FR |= 0xC0;
  } else {
    FR &= 0x7F;
    FR |= 0x40;
  }
  if ((t & 0x0F) == 0x00) {
    FR |= 0x20;
  } else {
    FR &= 0xD0;
  }
  tmp_clock += 4;
  pc++;
}
void dec_lr() {
  uint8_t t = LR ;
  LR--;
  if (LR == 0) {
    FR |= 0xC0;
  } else {
    FR &= 0x7F;
    FR |= 0x40;
  }
  if ((t & 0x0F) == 0x00) {
    FR |= 0x20;
  } else {
    FR &= 0xD0;
  }
  tmp_clock += 4;
  pc++;
}
void dec_ar() {
  uint8_t t = AR ;
  AR--;
  if (AR == 0) {
    FR |= 0xC0;
  } else {
    FR &= 0x7F;
    FR |= 0x40;
  }
  if ((t & 0x0F) == 0x00) {
    FR |= 0x20;
  } else {
    FR &= 0xD0;
  }
  tmp_clock += 4;
  pc++;
}

void xor_ar_br() {
  AR ^= BR ;
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 4;
  pc++;
}

void xor_ar_cr() {
  AR ^= CR ;
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 4;
  pc++;
}

void xor_ar_dr() {
  AR ^= DR ;
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 4;
  pc++;
}

void xor_ar_er() {
  AR ^= ER ;
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 4;
  pc++;
}

void xor_ar_hr() {
  AR ^= HR ;
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 4;
  pc++;
}

void xor_ar_lr() {
  AR ^= LR ;
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 4;
  pc++;
}

void xor_ar_phl() {
  AR ^= mmu_read(HL(HR, LR));
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 8;
  pc++;
}

void xor_ar_ar() {
  AR = 0x00;
  FR = 0x80;
  tmp_clock += 4;
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
      val_t = mmu_read(HL(HR, LR));
      tmp_clock += 4;
      break;
    case 0x97:
      val_t = AR;
      break;
  }
  if ((AR & 0x0F) < (val_t & 0x0F)) {
    FR = 0b01100000;
  } else {
    FR = 0b01000000;
  }
  if (AR < val_t) {
    FR |= 0x10;
  }
  AR = AR - val_t;
  if (AR == 0) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void add_ar_br() {
  if (AR & 0x0F + BR & 0x0F > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)BR > 0xFF) {
    FR |= 0x10;
  }
  AR += BR;
  if (!AR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void add_ar_cr() {
  if (AR & 0x0F + CR & 0x0F > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)CR > 0xFF) {
    FR |= 0x10;
  }
  AR += CR;
  if (!AR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void add_ar_dr() {
  if (AR & 0x0F + DR & 0x0F > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)DR > 0xFF) {
    FR |= 0x10;
  }
  AR += DR;
  if (!AR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void add_ar_er() {
  if (AR & 0x0F + ER & 0x0F > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)ER > 0xFF) {
    FR |= 0x10;
  }
  AR += ER;
  if (!AR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void add_ar_hr() {
  if (AR & 0x0F + HR & 0x0F > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)HR > 0xFF) {
    FR |= 0x10;
  }
  AR += HR;
  if (!AR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void add_ar_lr() {
  if (AR & 0x0F + LR & 0x0F > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)LR > 0xFF) {
    FR |= 0x10;
  }
  AR += LR;
  if (!AR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void add_ar_phl() {
  uint8_t val_t = mmu_read(HL(HR, LR));
  if (AR & 0x0F + val_t & 0x0F > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)val_t > 0xFF) {
    FR |= 0x10;
  }
  AR += val_t;
  if (!AR) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void add_ar_ar() {
  if (AR & 0x0F + AR & 0x0F > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if (AR & 0x80) {
    FR |= 0x10;
  }
  AR <<= 1;
  if (!AR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void cp_br() {
  if ((AR & 0x0F) < (BR & 0x0F)) {
    FR = 0x60;
  } else {
    FR = 0x40;
  }
  if (AR < BR) {
    FR |= 0x10;
  }
  if (AR == BR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void cp_cr() {
  if ((AR & 0x0F) < (CR & 0x0F)) {
    FR = 0x60;
  } else {
    FR = 0x40;
  }
  if (AR < CR) {
    FR |= 0x10;
  }
  if (AR == CR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void cp_dr() {
  if ((AR & 0x0F) < (DR & 0x0F)) {
    FR = 0x60;
  } else {
    FR = 0x40;
  }
  if (AR < DR) {
    FR |= 0x10;
  }
  if (AR == DR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void cp_er() {
  if ((AR & 0x0F) < (ER & 0x0F)) {
    FR = 0x60;
  } else {
    FR = 0x40;
  }
  if (AR < ER) {
    FR |= 0x10;
  }
  if (AR == ER) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void cp_hr() {
  if ((AR & 0x0F) < (HR & 0x0F)) {
    FR = 0x60;
  } else {
    FR = 0x40;
  }
  if (AR < HR) {
    FR |= 0x10;
  }
  if (AR == HR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void cp_lr() {
  if ((AR & 0x0F) < (LR & 0x0F)) {
    FR = 0x60;
  } else {
    FR = 0x40;
  }
  if (AR < LR) {
    FR |= 0x10;
  }
  if (AR == LR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void cp_phl() {
  uint8_t val_t = mmu_read(HL(HR, LR));
  if ((AR & 0x0F) < (val_t & 0x0F)) {
    FR = 0x60;
  } else {
    FR = 0x40;
  }
  if (AR < val_t) {
    FR |= 0x10;
  }
  if (AR == val_t) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void cp_ar() {
  FR = 0xC0;
  tmp_clock += 4;
  pc++;
}

void jr_d8() {
  pc = pc + 2 + (int8_t)mmu_read(pc + 1);
  tmp_clock += 12;
}

void jr_nz_d8() {
  if (!(FR & 0x80)) {
    pc = pc + 2 + (int8_t)mmu_read(pc + 1);
    tmp_clock += 12;
    return;
  }
  tmp_clock += 8;
  pc += 2;
}

void jr_z_d8() {
  if (FR & 0x80) {
    pc = pc + 2 + (int8_t)mmu_read(pc + 1);
    tmp_clock += 12;
    return;
  }
  tmp_clock += 8;
  pc += 2;
}

void jr_nc_d8() {
  if (!(FR & 0x10)) {
    pc = pc + 2 + (int8_t)mmu_read(pc + 1);
    tmp_clock += 12;
    return;
  }
  tmp_clock += 8;
  pc += 2;
}

void jr_c_d8() {
  if (FR & 0x10) {
    pc = pc + 2 + (int8_t)mmu_read(pc + 1);
    tmp_clock += 12;
    return;
  }
  tmp_clock += 8;
  pc += 2;
}

void ld_bc_d16() {
  CR = mmu_read(++pc);
  BR = mmu_read(++pc);
  tmp_clock += 12;
  pc++;
}

void ld_de_d16() {
  ER = mmu_read(++pc);
  DR = mmu_read(++pc);
  tmp_clock += 12;
  pc++;
}

void ld_hl_d16() {
  LR = mmu_read(++pc);
  HR = mmu_read(++pc);
  tmp_clock += 12;
  pc++;
}

void ld_sp_d16() {
  sp = (uint16_t)mmu_read(++pc);
  sp += (uint16_t)mmu_read(++pc) << 8;
  tmp_clock += 12;
  pc++;
}

void rla() {
  if (AR >> 7) {
    AR = (AR << 1) + ((FR & 0x10) >> 4);
    FR = 0x10;
  } else {
    AR = (AR << 1) + ((FR & 0x10) >> 4);
    FR = 0x00;
  }
  tmp_clock += 4;
  pc++;
}

void ldh_pd8_ar() {
  mmu_write(0xFF00 + (uint16_t)mmu_read(++pc), AR);
  tmp_clock += 12;
  pc++;
}

void ldh_ar_pd8() {
  AR = mmu_read(0xFF00 + (uint16_t)mmu_read(++pc));
  tmp_clock += 12;
  pc++;
}

void pop_bc() {
  CR = mmu_read(sp++);
  BR = mmu_read(sp++);
  tmp_clock += 12;
  pc++;
}

void pop_de() {
  ER = mmu_read(sp++);
  DR = mmu_read(sp++);
  tmp_clock += 12;
  pc++;
}

void pop_hl() {
  LR = mmu_read(sp++);
  HR = mmu_read(sp++);
  tmp_clock += 12;
  pc++;
}

void pop_af() {
  FR = mmu_read(sp++) & 0xF0;
  AR = mmu_read(sp++);
  tmp_clock += 12;
  pc++;
}

void ld_pcr_ar() {
  mmu_write(0xFF00 + CR, AR);
  tmp_clock += 8;
  pc++; //表では2だった
}

void ld_ar_pcr() {
  AR = mmu_read(0xFF00 + CR);
  tmp_clock += 8;
  pc++; //表では2だった
}

void di() {
  ime = 0;
  tmp_clock += 4;
  pc++;
}

void ei() {
  ime = 1;
  tmp_clock += 4;
  pc++;
}

void push_bc() {
  mmu_write(--sp, BR);
  mmu_write(--sp, CR);
  tmp_clock += 16;
  pc++;
}

void push_de() {
  mmu_write(--sp, DR);
  mmu_write(--sp, ER);
  tmp_clock += 16;
  pc++;
}

void push_hl() {
  mmu_write(--sp, HR);
  mmu_write(--sp, LR);
  tmp_clock += 16;
  pc++;
}

void push_af() {
  mmu_write(--sp, AR);
  mmu_write(--sp, FR);
  tmp_clock += 16;
  pc++;
}

void ret() {
  pc = ((uint16_t)mmu_read(sp + 1) << 8) + (uint16_t)mmu_read(sp);
  sp += 2;
  tmp_clock += 16;
}

void call_d16() { ///// not correct push a return address
  mmu_write(--sp, (uint8_t)(((pc + 3) & 0xFF00) >> 8));
  mmu_write(--sp, (uint8_t)((pc + 3) & 0x00FF));
  tmp_clock += 24;
  pc = (uint16_t)mmu_read(pc + 1) + ((uint16_t)mmu_read(pc + 2) << 8);
}

void cp_d8() { // miss
  uint8_t val_t = mmu_read(++pc);
  if ((AR & 0x0F) < (val_t & 0x0F)) {
    FR = 0x60;
  } else {
    FR = 0x40;
  }
  if (AR < val_t) {
    FR |= 0x10;
  }
  if (AR == val_t) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void rl_r8() {
  uint8_t* r;
  uint8_t t;
  switch (code) {
    case 0x10:
      r = &BR;
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
    *r = (*r << 1) + ((FR & 0x10) >> 4);
    FR = 0x10;
  } else {
    *r = (*r << 1) + ((FR & 0x10) >> 4);
    FR = 0x00;
  }
  if (*r == 0) FR |= 0x80;
  tmp_clock += 8;
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
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 8;
  pc++;
}

void bit_() {
  uint8_t b = (code & 0x38) >> 3;
  uint8_t r;
  uint8_t mask = 0x01 << b;
  switch (code & 0x07) {
    case 0b000:
      r = BR;
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
    FR &= 0x7F;
  } else {
    FR |= 0x80;
  }
  FR |= 0x20;
  FR &= 0b10110000;
  tmp_clock += 8;
  pc++;
}

void res() {
  uint8_t b = (code & 0x38) >> 3;
  uint8_t *r;
  uint8_t mask = 0x01 << b;
  switch (code & 0x07) {
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
  tmp_clock += 8;
  pc++;
}

void set() {
  uint8_t b = (code & 0x38) >> 3;
  uint8_t *r;
  uint8_t mask = 0x01 << b;
  switch (code & 0x07) {
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
  tmp_clock += 8;
  pc++;
}

void jp_d16() {
  pc = (uint16_t)mmu_read(pc + 1) + ((uint16_t)mmu_read(pc + 2) << 8);
  tmp_clock += 16;
}

void cpl() {
  AR = ~AR;
  FR |= 0x60;
  tmp_clock += 4;
  pc++;
}

void ccf() {
  FR &= 0x90;
  if (FR & 0x10) {
    FR &= 0xE0;
  } else {
    FR |= 0x10;
  }
  tmp_clock += 4;
  pc++;
}

void and_d8() {
  AR &= mmu_read(++pc);
  if (AR) {
    FR = 0x20;
  } else {
    FR = 0b10100000;
  }
  tmp_clock += 8;
  pc++;
}
void or_d8() {
  AR |= mmu_read(++pc);
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 8;
  pc++;
}

void or_br() {
  AR |= BR ;
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 4;
  pc++;
}

void or_cr() {
  AR |= CR ;
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 4;
  pc++;
}

void or_dr() {
  AR |= DR ;
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 4;
  pc++;
}

void or_er() {
  AR |= ER ;
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 4;
  pc++;
}

void or_hr() {
  AR |= HR ;
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 4;
  pc++;
}

void or_lr() {
  AR |= LR ;
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 4;
  pc++;
}

void or_ar() {
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 4;
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
    FR = 0x20;
  } else {
    FR = 0b10100000;
  }
  tmp_clock += 4;
  pc++;
}

void ld_ar_pa16() {
  AR = mmu_read((uint16_t)mmu_read(pc + 1) + (uint16_t)(mmu_read(pc + 2) << 8));
  tmp_clock += 16;
  pc += 3;
}

void ld_pa16_ar() {
  mmu_write((uint16_t)mmu_read(pc + 1) + (uint16_t)(mmu_read(pc + 2) << 8), AR);
  tmp_clock += 16;
  pc += 3;
}

void ret_nz() {
  if (!(FR & 0x80)) {
    tmp_clock += 20;
    pc = ((uint16_t)mmu_read(sp + 1) << 8) + (uint16_t)mmu_read(sp);
    sp += 2;
    return;
  }
  tmp_clock += 8;
  pc++;
}

void ret_nc() {
  if (!(FR & 0x10)) {
    tmp_clock += 20;
    pc = ((uint16_t)mmu_read(sp + 1) << 8) + (uint16_t)mmu_read(sp);
    sp += 2;
    return;
  }
  tmp_clock += 8;
  pc++;
}

void ret_c() {
  if (FR & 0x10) {
    tmp_clock += 20;
    pc = ((uint16_t)mmu_read(sp + 1) << 8) + (uint16_t)mmu_read(sp);
    sp += 2;
    return;
  }
  tmp_clock += 8;
  pc++;
}

void ret_z() {
  if (FR & 0x80) {
    tmp_clock += 20;
    pc = ((uint16_t)mmu_read(sp + 1) << 8) + (uint16_t)mmu_read(sp);
    sp += 2;
    return;
  }
  tmp_clock += 8;
  pc++;
}

void reti() {
  ime = 1;
  pc = ((uint16_t)mmu_read(sp + 1) << 8) + (uint16_t)mmu_read(sp);
  sp += 2;
  tmp_clock += 16;
}

void add_hl_sp() { //動作怪しい
  uint16_t HL = HL(HR, LR);
  uint32_t t;

  if ((HL & 0x0FFF) + (sp & 0x0FFF) > 0x0FFF) { //H
    FR |= 0x20;
  } else {
    FR &= 0xD0;
  }
  t = (uint32_t)HL + (uint32_t)sp;
  if (t > 0xFFFF) {  // C
    FR |= 0x10;
  }
  FR &= 0xB0; // N
  HR = (uint8_t)((t & 0x0000FF00) >> 8);
  LR = (uint8_t)(t & 0x000000FF);
  pc++;
}

void add_hl_bc() { //動作怪しい
  uint16_t HL = HL(HR, LR);
  uint16_t BC = BC(BR, CR);
  uint32_t t;

  if ((HL & 0x0FFF) + (BC & 0x0FFF) > 0x0FFF) { //H
    FR |= 0x20;
  } else {
    FR &= 0xD0;
  }
  t = (uint32_t)HL + (uint32_t)BC;
  if (t > 0xFFFF) {  // C
    FR |= 0x10;
  }
  FR &= 0xB0; // N
  HR = (uint8_t)((t & 0x0000FF00) >> 8);
  LR = (uint8_t)(t & 0x000000FF);
  pc++;
}

void add_hl_de() { //動作怪しい
  uint16_t HL = HL(HR, LR);
  uint16_t DE = DE(DR, ER);
  uint32_t t;

  if ((HL & 0x0FFF) + (DE & 0x0FFF) > 0x0FFF) { //H
    FR |= 0x20;
  } else {
    FR &= 0xD0;
  }
  t = (uint32_t)HL + (uint32_t)DE;
  if (t > 0xFFFF) {  // C
    FR |= 0x10;
  }
  FR &= 0xB0; // N
  HR = (uint8_t)((t & 0x0000FF00) >> 8);
  LR = (uint8_t)(t & 0x000000FF);
  pc++;
}

void add_hl_hl() {
  uint16_t HL = HL(HR, LR);
  if (HL & 0x0800) { //H
    FR |= 0x20;
  } else {
    FR &= 0xD0;
  }
  if (HL & 0x8000) {  // C
    FR |= 0x10;
  }
  FR &= 0xB0; // N
  HL <<= 1;
  HR = (uint8_t)((HL & 0xFF00) >> 8);
  LR = (uint8_t)(HL & 0x00FF);
  pc++;
}

void stop_0() {
  tmp_clock += 4;
  pc += 2;
}

void jp_cc_d16() {
  switch (code) {
    case 0xC2: //not zero
      if (!(FR & 0x80)) {
        jp_d16();
        return;
      }
      break;
    case 0xD2: //not carry
      if (!(FR & 0x10)) {
        jp_d16();
        return;
      }
      break;
    case 0xCA: //zero
      if (FR & 0x80) {
        jp_d16();
        return;
      }
      break;
    case 0xDA: //carry
      if (FR & 0x10) {
        jp_d16();
        return;
      }
      break;
  }
  tmp_clock += 12;
  pc += 3;
}

void call_cc_d16() {
  switch (code) {
    case 0xC4: //not zero
      if (!(FR & 0x80)) {
        call_d16();
        return;
      }
      break;
    case 0xD4: //not carry
      if (!(FR & 0x10)) {
        call_d16();
        return;
      }
      break;
    case 0xCC: //zero
      if (FR & 0x80) {
        call_d16();
        return;
      }
      break;
    case 0xDC: //carry
      if (FR & 0x10) {
        call_d16();
        return;
      }
      break;
  }
  tmp_clock += 12;
  pc += 3;
}

void adc_ar_r8() {
  uint8_t val_t;
  uint8_t c_val_t = (FR & 0x10) >> 4;
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
      val_t = mmu_read(HL(HR, LR));
      tmp_clock += 4;
      break;
    case 0x8F:
      val_t = AR;
      break;
  }
  if (AR & 0x0F + val_t & 0x0F + c_val_t > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)val_t + (uint16_t)c_val_t > 0xFF) {
    FR |= 0x10;
  }
  AR = AR + val_t + c_val_t;
  if (AR == 0) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void adc_ar_br() {
  uint8_t c = (FR & 0x10) >> 4;
  if (AR & 0x0F + BR & 0x0F + c > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)BR + (uint16_t)c > 0xFF) {
    FR |= 0x10;
  }
  AR += (BR + c);
  if (!AR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void adc_ar_cr() {
  uint8_t c = (FR & 0x10) >> 4;
  if (AR & 0x0F + CR & 0x0F + c > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)CR + (uint16_t)c > 0xFF) {
    FR |= 0x10;
  }
  AR += (CR + c);
  if (!AR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void adc_ar_dr() {
  uint8_t c = (FR & 0x10) >> 4;
  if (AR & 0x0F + DR & 0x0F + c > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)DR + (uint16_t)c > 0xFF) {
    FR |= 0x10;
  }
  AR += (DR + c);
  if (!AR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void adc_ar_er() {
  uint8_t c = (FR & 0x10) >> 4;
  if (AR & 0x0F + ER & 0x0F + c > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)ER + (uint16_t)c > 0xFF) {
    FR |= 0x10;
  }
  AR += (ER + c);
  if (!AR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void adc_ar_hr() {
  uint8_t c = (FR & 0x10) >> 4;
  if (AR & 0x0F + HR & 0x0F + c > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)HR + (uint16_t)c > 0xFF) {
    FR |= 0x10;
  }
  AR += (HR + c);
  if (!AR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void adc_ar_lr() {
  uint8_t c = (FR & 0x10) >> 4;
  if (AR & 0x0F + LR & 0x0F + c > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)LR + (uint16_t)c > 0xFF) {
    FR |= 0x10;
  }
  AR += (LR + c);
  if (!AR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void adc_ar_phl() {
  uint8_t val_t = mmu_read(HL(HR, LR));
  uint8_t c = (FR & 0x10) >> 4;
  if (AR & 0x0F + val_t & 0x0F + c > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)val_t + (uint16_t)c > 0xFF) {
    FR |= 0x10;
  }
  AR += (val_t + c);
  if (!AR) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void adc_ar_ar() {
  uint8_t c = (FR & 0x10) >> 4;
  if (AR & 0x0F + AR & 0x0F + c > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)AR + (uint16_t)c > 0xFF) {
    FR |= 0x10;
  }
  AR += (AR + c);
  if (!AR) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void rst_vec() {
  mmu_write(--sp, (uint8_t)(((pc + 1) & 0xFF00) >> 8));
  mmu_write(--sp, (uint8_t)((pc + 1) & 0x00FF));
  tmp_clock += 16;
  pc = (uint16_t)code - 0xC7;
}

void sbc_ar_r8() {
  uint8_t val_t;
  uint8_t c_val_t = (FR & 0x10) >> 4;
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
      val_t = mmu_read(HL(HR, LR));
      tmp_clock += 4;
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
    FR |= 0x10;
  }
  AR = AR - val_t;
  if (AR == 0) FR |= 0x80;
  tmp_clock += 4;
  pc++;
}

void add_ar_d8() {
  uint8_t val_t = mmu_read(++pc);
  if (AR & 0x0F + val_t & 0x0F > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)val_t > 0xFF) {
    FR |= 0x10;
  }
  AR = AR + val_t;
  if (AR == 0) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void adc_ar_d8() {
  uint8_t val_t = mmu_read(++pc);
  uint8_t c_val_t = (FR & 0x10) >> 4;
  if ((AR & 0x0F) + (val_t & 0x0F) + c_val_t > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)val_t + (uint16_t)c_val_t > 0xFF) {
    FR |= 0x10;
  }
  AR += (val_t + c_val_t);
  if (AR == 0) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void sub_ar_d8() {
  uint8_t val_t = mmu_read(++pc);
  if ((AR & 0x0F) < (val_t & 0x0F)) {
    FR = 0b01100000;
  } else {
    FR = 0b01000000;
  }
  if (AR < val_t) {
    FR |= 0x10;
  }
  AR = AR - val_t;
  if (AR == 0) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void sbc_ar_d8() {
  uint8_t val_t = mmu_read(++pc);
  uint8_t c_val_t = (FR & 0x10) >> 4;
  if ((AR & 0x0F) < (val_t & 0x0F) + c_val_t) {
    FR = 0b01100000;
  } else {
    FR = 0b01000000;
  }
  if ((uint16_t)AR < (uint16_t)val_t + (uint16_t)c_val_t) {
    FR |= 0x10;
  }
  AR -= (val_t + c_val_t);
  if (AR == 0) FR |= 0x80;
  tmp_clock += 8;
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
  if (*val_t & 0x80 > 0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  *val_t = *val_t << 1;
  if (*val_t == 0) FR |= 0x80;
  tmp_clock += 8;
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
  if (*val_t & 0x80 > 0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  *val_t  = (*val_t << 1) + ((*val_t & 0x80) >> 7);
  if (*val_t  == 0) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void srl_br() {
  if (BR & 0x01) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  BR = BR >> 1;
  if (!BR) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void srl_cr() {
  if (CR & 0x01) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  CR = CR >> 1;
  if (!CR) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void srl_dr() {
  if (DR & 0x01) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  DR = DR >> 1;
  if (!DR) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void srl_er() {
  if (ER & 0x01) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  ER = ER >> 1;
  if (!ER) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void srl_hr() {
  if (HR & 0x01) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  HR = HR >> 1;
  if (!HR) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void srl_lr() {
  if (LR & 0x01) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  LR = LR >> 1;
  if (!LR) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void srl_ar() {
  if (AR & 0x01) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  AR = AR >> 1;
  if (!AR) FR |= 0x80;
  tmp_clock += 8;
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
  if (*val_t & 0x01 > 0) { // 矛盾あり　無条件で0とするという記述もある
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  *val_t = (*val_t >> 1) + (*val_t & 0x80);
  if (AR == 0) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void rr_br() {
  uint8_t bit_0 = BR & 0x01;
  BR >>= 1;
  if (FR & 0x10)  BR |= 0x80;
  if (bit_0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  if (!BR) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void rr_cr() {
  uint8_t bit_0 = CR & 0x01;
  CR >>= 1;
  if (FR & 0x10) CR |= 0x80;
  if (bit_0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  if (!CR) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void rr_dr() {
  uint8_t bit_0 = DR & 0x01;
  DR >>= 1;
  if (FR & 0x10) DR |= 0x80;
  if (bit_0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  if (!DR) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void rr_er() {
  uint8_t bit_0 = ER & 0x01;
  ER >>= 1;
  if (FR & 0x10) ER |= 0x80;
  if (bit_0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  if (!ER) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void rr_hr() {
  uint8_t bit_0 = HR & 0x01;
  HR >>= 1;
  if (FR & 0x10) HR |= 0x80;
  if (bit_0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  if (!HR) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void rr_lr() {
  uint8_t bit_0 = LR & 0x01;
  LR >>= 1;
  if (FR & 0x10) LR |= 0x80;
  if (bit_0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  if (!LR) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void rr_ar() {
  uint8_t bit_0 = AR & 0x01;
  AR >>= 1;
  if (FR & 0x10) AR |= 0x80;
  if (bit_0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  if (!AR) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void rra() {
  uint8_t bit_0 = AR & 0x01;
  AR >>= 1;
  if (FR & 0x10) AR |= 0x80;
  if (bit_0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  tmp_clock += 4;
  pc++;
}

void xor_ar_d8() {
  AR ^= mmu_read(++pc);
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 8;
  pc++;
}

void or_ar_phl() {
  AR |= mmu_read(HL(HR, LR));
  if (AR) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  tmp_clock += 8;
  pc++;
}

void and_ar_phl() {
  AR &= mmu_read(HL(HR, LR));
  if (AR == 0) {
    FR = 0x80;
  } else {
    FR = 0x00;
  }
  tmp_clock += 8;
  pc++;
}

void dec_phl() {
  uint8_t val_t = mmu_read(HL(HR, LR));
  if ((val_t & 0x0F) == 0x00) {
    FR |= 0x20;
  } else {
    FR &= 0b11010000;
  }
  mmu_write(HL(HR, LR), val_t - 1);
  if (val_t == 0x01) {
    FR |= 0x80;
  } else {
    FR &= 0b01110000;
  }
  FR |= 0b01000000;
  tmp_clock += 12;
  pc++;
}

void inc_phl() {
  uint8_t val_t = mmu_read(HL(HR, LR));
  if ((val_t & 0x0F) == 0x0F) {
    FR |= 0x20;
  } else {
    FR &= 0b11010000;
  }
  mmu_write(HL(HR, LR), val_t + 1);
  if (val_t == 0xFF) {
    FR |= 0x80;
  } else {
    FR &= 0b01110000;
  }
  FR &= 0b10110000;
  tmp_clock += 12;
  pc++;
}
void halt() { //Timer未実装のため、halt後のtimer割り込みがなくループになるので、手抜き中
  if (ime) {
    //省電力モード？
  } else {

  }
  //halted = 1;
  tmp_clock += 4;
  pc++;
}

void rlca() {
  if (AR & 0x80 > 0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  AR = (AR << 1) | (AR >> 7);
  tmp_clock += 4;
  pc++;
}

void daa() {
  if (!(FR & 0x40)) {
    if ((FR & 0x10) || AR > 0x99) {
      AR += 0x60;
      FR |= 0x10;
    }
    if ((FR & 0x20) || ((AR & 0x0F) > 0x09)) AR += 0x06;
  } else {
    if (FR & 0x10) AR -= 0x60;
    if (FR & 0x20) AR -= 0x06;
  }
  if (AR == 0) {
    FR |= 0x80;
  } else {
    FR &= 0x70;
  }
  FR &= 0xD0;
  tmp_clock += 4;
  pc++;
}

void scf() {
  FR &= 0x90;
  FR |= 0x10;
  tmp_clock += 4;
  pc++;
}

void ld_pd16_sp() {
  mmu_write(((uint16_t)mmu_read(pc + 2) << 8) + mmu_read(pc + 1), (uint8_t)(sp & 0x00FF));
  mmu_write(((uint16_t)mmu_read(pc + 2) << 8) + mmu_read(pc + 1) + 1, (uint8_t)(sp >> 8));
  tmp_clock += 20;
  pc += 3;
}

void add_sp_d8() {
  int8_t val_t = (int8_t)mmu_read(++pc);
  if ((sp & 0x0F) + ((uint16_t)val_t & 0x0F) > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((sp & 0xFF) + ((uint16_t)val_t & 0xFF) > 0xFF) FR |= 0x10;
  sp = (uint16_t)((int32_t)sp + val_t);
  tmp_clock += 16;
  pc++;
}

void jp_hl() {
  pc = HL(HR, LR);
  tmp_clock += 4;
}

void ld_sp_hl() {
  sp = HL(HR, LR);
  tmp_clock += 8;
  pc++;
}

void rrca() {
  if (AR & 0x01 > 0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  AR = (AR >> 1) | (AR << 7);
  tmp_clock += 4;
  pc++;
}

void ld_hl_sp_d8() {
  int8_t val_t = (int8_t)mmu_read(++pc);
  if ((sp & 0x0F) + ((uint16_t)val_t & 0x0F) > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((sp & 0xFF) + ((uint16_t)val_t & 0xFF) > 0xFF) FR |= 0x10;
  HR = (uint8_t)((uint16_t)((int32_t)sp + val_t) >> 8);
  LR = (uint8_t)((uint16_t)((int32_t)sp + val_t) & 0x00FF);
  tmp_clock += 12;
  pc++;
}

void swap_phl() {
  uint8_t val_t = mmu_read(HL(HR, LR));
  val_t = ((val_t << 4) & 0xF0) + (val_t >> 4);
  if (val_t) {
    FR = 0x00;
  } else {
    FR = 0x80;
  }
  mmu_write(HL(HR, LR), val_t);
  tmp_clock += 16;
  pc += 2;
}

void sla_phl() {
  uint8_t val_t = mmu_read(HL(HR, LR));
  if (val_t & 0x80) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  val_t = val_t << 1;
  if (!val_t) FR |= 0x80;
  mmu_write(HL(HR, LR), val_t);
  tmp_clock += 16;
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
  if (*val_t & 0x01 > 0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  *val_t = (*val_t >> 1) | ((*val_t & 0x01) << 7);
  if (*val_t == 0) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void set_phl() {
  uint8_t b = (code & 0x38) >> 3;
  uint8_t mask = 0x01 << b;
  uint8_t r = mmu_read(HL(HR, LR));
  r |= mask;
  mmu_write(HL(HR, LR), r);
  tmp_clock += 16;
  pc++;
}

void res_phl() {
  uint8_t b = (code & 0x38) >> 3;
  uint8_t mask = 0x01 << b;
  uint8_t r = mmu_read(HL(HR, LR));
  r &= (~mask);
  mmu_write(HL(HR, LR), r);
  tmp_clock += 16;

  pc++;
}

void bit_phl() {
  uint8_t b = (code & 0x38) >> 3;
  uint8_t mask = 0x01 << b;
  uint8_t r = mmu_read(HL(HR, LR));
  r = r & mask;
  if (r) {
    FR &= 0x7F;
  } else {
    FR |= 0x80;
  }
  FR |= 0x20;
  FR &= 0b10110000;
  tmp_clock += 16;
  pc++;
}

void rlc_phl() {
  uint8_t val_t = mmu_read(HL(HR, LR));
  if (val_t & 0x80 > 0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  val_t  = (val_t << 1) + ((val_t & 0x80) >> 7);
  if (val_t  == 0) FR |= 0x80;
  mmu_write(HL(HR, LR), val_t);
  tmp_clock += 16;
  pc++;
}

void rl_phl() {
  uint8_t val_t = mmu_read(HL(HR, LR));

  if (val_t >> 7) {
    val_t = (val_t << 1) + ((FR & 0x10) >> 4);
    FR = 0x10;
  } else {
    val_t = (val_t << 1) + ((FR & 0x10) >> 4);
    FR = 0x00;
  }
  if (val_t == 0) FR |= 0x80;
  mmu_write(HL(HR, LR), val_t);
  tmp_clock += 16;
  pc++;
}

void rrc_phl() {
  uint8_t val_t = mmu_read(HL(HR, LR));

  if (val_t & 0x01 > 0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  AR = (val_t >> 1) + ((val_t & 0x01) << 7);
  if (AR == 0) FR |= 0x80;
  mmu_write(HL(HR, LR), val_t);
  tmp_clock += 16;
  pc++;
}

void rr_phl() {
  uint8_t val_t = mmu_read(HL(HR, LR));
  uint8_t bit_0 = val_t & 0x01;
  if (FR & 0x10) {
    val_t = (val_t >> 1) | 0x80;
  } else {
    val_t = (val_t >> 1);
  }
  if (bit_0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  if (val_t == 0) FR |= 0x80;
  mmu_write(HL(HR, LR), val_t);
  tmp_clock += 16;
  pc++;
}

void sra_phl() {
  uint8_t val_t = mmu_read(HL(HR, LR));

  if (val_t & 0x01 > 0) { // 矛盾あり　無条件で0とするという記述もある
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  val_t = (val_t >> 1) + (val_t & 0x80);
  if (AR == 0) FR |= 0x80;
  mmu_write(HL(HR, LR), val_t);
  tmp_clock += 16;
  pc++;
}

void srl_phl() {
  uint8_t val_t = mmu_read(HL(HR, LR));
  if (val_t & 0x01 > 0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  val_t = val_t >> 1;
  if (val_t == 0) FR |= 0x80;
  mmu_write(HL(HR, LR), val_t);
  tmp_clock += 16;
  pc++;
}

void call_irpt(uint16_t addr) { ///// not correct push a return address
  mmu_write(--sp, (uint8_t)((pc & 0xFF00) >> 8));
  mmu_write(--sp, (uint8_t)(pc & 0x00FF));
  tmp_clock += 32; //正しいのか分からないけど、多分これくらい
  pc = addr;
}
