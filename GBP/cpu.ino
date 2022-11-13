uint8_t cpu_step() {

  uint8_t total_clock = 0;
  tmp_clock = 0;

  if (flag_halt) {
    tmp_clock += 16;
  } else {
    execute();
  }

  //total_clock += tmp_clock;

  // mmu.update??
  //mmu_update(tmp_clock);

  if (ime) {
    //tmp_clock = 0;
    int_check();
    //mmu.update
    //mmu_update(tmp_clock);

    //total_clock += tmp_clock;
  }
  mmu_update(tmp_clock);
  total_clock += tmp_clock;

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

void execute() {

  code = mmu_read(pc);

  if (pc == 0x100) {
    //chk_init_regs();
    boot = false;
  }

  if (pc >= 0x213 && pc <= 0x23E && !boot) {
    sprintf(buf_b1, "pc:%04X->%02X AF:%02X%02X BC:%02X%02X DE:%02X%02X HL:%02X%02X sp:%04X", pc, code, AR, FR, BR, CR, DR, ER, HR, LR, sp);
    //sprintf(buf_b1, "OAM0:%02X %02X %02X %02X", mmu_read(0xFE00), mmu_read(0xFE01), mmu_read(0xFE02), mmu_read(0xFE03));
    Serial.println(buf_b1);
  }

  if (code == 0xCB) {
    code = mmu_read(++pc);
    pf_op_ptr_array[code]();
  } else {
    op_ptr_array[code]();
  }
}

void ld_r8_d8() {
  switch (code) {
    case 0x06:
      BR = mmu_read(++pc);
      break;
    case 0x0E:
      CR = mmu_read(++pc);
      break;
    case 0x16:
      DR = mmu_read(++pc);
      break;
    case 0x1E:
      ER = mmu_read(++pc);
      break;
    case 0x26:
      HR = mmu_read(++pc);
      break;
    case 0x2E:
      LR = mmu_read(++pc);
      break;
    case 0x3E:
      AR = mmu_read(++pc);
      break;
  }
  tmp_clock += 8;

  pc++;
}

void ld_r8_phl() {
  switch (code) {
    case 0x46:
      BR = mmu_read(HL(HR, LR));
      break;
    case 0x4E:
      CR = mmu_read(HL(HR, LR));
      break;
    case 0x56:
      DR = mmu_read(HL(HR, LR));
      break;
    case 0x5E:
      ER = mmu_read(HL(HR, LR));
      break;
    case 0x66:
      HR = mmu_read(HL(HR, LR));
      break;
    case 0x6E:
      LR = mmu_read(HL(HR, LR));
      break;
    case 0x7E:
      AR = mmu_read(HL(HR, LR));
      break;
  }
  tmp_clock += 8;

  pc++;
}

void ld_r8_r8() {
  uint8_t *t1;
  switch (code & 0x38) {
    case 0x00:
      t1 = &BR ;
      break;
    case 0b00001000:
      t1 = &CR;
      break;
    case 0x10:
      t1 = &DR;
      break;
    case 0b00011000:
      t1 = &ER;
      break;
    case 0x20:
      t1 = &HR;
      break;
    case 0b00101000:
      t1 = &LR;
      break;
    case 0x38:
      t1 = &AR;
      break;
  }
  switch (code & 0b00000111) {
    case 0x00:
      *t1 = BR ;
      break;
    case 0x01:
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
  tmp_clock += 4;

  pc++;
}

void ld_phl_r8() {
  switch (code) {
    case 0x70:
      mmu_write(HL(HR, LR), BR);
      break;
    case 0x71:
      mmu_write(HL(HR, LR), CR);
      break;
    case 0x72:
      mmu_write(HL(HR, LR), DR);
      break;
    case 0x73:
      mmu_write(HL(HR, LR), ER);
      break;
    case 0x74:
      mmu_write(HL(HR, LR), HR);
      break;
    case 0x75:
      mmu_write(HL(HR, LR), LR);
      break;
    case 0x77:
      mmu_write(HL(HR, LR), AR);
      break;
  }
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
void ld_pr16_ar() {
  switch (code) {
    case 0x02:
      mmu_write(BC(BR, CR), AR);
      break;
    case 0x12:
      mmu_write(DE(DR, ER), AR);
      break;
  }
  tmp_clock += 8;

  pc++;
}

void ld_phli_ar() {
  mmu_write(HL(HR, LR), AR);
  LR++;
  if (LR == 0x00) HR++;
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
  tmp_clock += 8;

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
  tmp_clock += 8;

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
    FR |= 0x80;
    FR &= 0b10110000;
  } else {
    FR &= 0b00110000;
  }
  if ((t & 0x0F) == 0x0F) {
    FR |= 0x20;
  } else {
    FR &= 0b11010000;
  }
  tmp_clock += 4;

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
    FR &= 0x7F;
    FR |= 0b01000000;
  }
  if (t & 0x0F == 0x00) {
    FR |= 0x20;
  } else {
    FR &= 0b11010000;
  }
  tmp_clock += 4;

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
      AR ^= mmu_read(HL(HR, LR));
      tmp_clock += 4;

      break;
    case 0xAF:
      AR ^= AR;
      break;
  }
  if (AR == 0) {
    FR = 0x80;
  } else {
    FR = 0x00;
  }
  tmp_clock += 4;
  // cc_decは都度0リセットされる。これで正しい
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
      val_t = mmu_read(HL(HR, LR));
      tmp_clock += 4;

      break;
    case 0x87:
      val_t = AR;
      break;
  }
  if (AR & 0x0F + val_t & 0x0F > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((uint16_t)AR + (uint16_t)val_t > 0xFF) {
    FR |= 0x10;
  }
  AR += val_t;
  if (AR == 0) FR |= 0x80;
  tmp_clock += 4;

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
      val_t = mmu_read(HL(HR, LR));
      tmp_clock += 4;

      break;
    case 0xBF:
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
  if (AR == val_t) FR |= 0x80;
  tmp_clock += 4;

  pc++;
}

void jr_cc_d8() {
  switch (code) {
    case 0x18:
      pc = pc + 2 + (int8_t)mmu_read(pc + 1);
      tmp_clock += 12;

      return;
    case 0x20:
      if (!(FR & 0x80)) {
        pc = pc + 2 + (int8_t)mmu_read(pc + 1);
        tmp_clock += 12;

        return;
      }
      break;
    case 0x28:
      if (FR & 0x80) {
        pc = pc + 2 + (int8_t)mmu_read(pc + 1);
        tmp_clock += 12;

        return;
      }
      break;
    case 0x30:
      if (!(FR & 0x10)) {
        pc = pc + 2 + (int8_t)mmu_read(pc + 1);
        tmp_clock += 12;

        return;
      }
      break;
    case 0x38:
      if (FR & 0x10) {
        pc = pc + 2 + (int8_t)mmu_read(pc + 1);
        tmp_clock += 12;

        return;
      }
      break;
  }
  tmp_clock += 8;

  pc += 2;
}

void ld_r16_d16() {
  switch (code) {
    case 0x01:
      CR = mmu_read(++pc);
      BR = mmu_read(++pc);
      break;
    case 0x11:
      ER = mmu_read(++pc);
      DR = mmu_read(++pc);
      break;
    case 0x21:
      LR = mmu_read(++pc);
      HR = mmu_read(++pc);
      break;
    case 0x31:
      sp = (uint16_t)mmu_read(++pc);
      sp += ((uint16_t)mmu_read(++pc)) << 8;
      break;
  }
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

void pop_r16() {
  switch (code) {
    case 0xC1:
      CR = mmu_read(sp++);
      BR = mmu_read(sp++);
      break;
    case 0xD1:
      ER = mmu_read(sp++);
      DR = mmu_read(sp++);
      break;
    case 0xE1:
      LR = mmu_read(sp++);
      HR = mmu_read(sp++);
      break;
    case 0xF1:
      FR = mmu_read(sp++) & 0xF0;
      AR = mmu_read(sp++);
      break;
  }
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

void push_r16() {
  switch (code) {
    case 0xC5: //0xc5
      mmu_write(--sp, BR);
      mmu_write(--sp, CR);
      break;
    case 0xD5:
      mmu_write(--sp, DR);
      mmu_write(--sp, ER);
      break;
    case 0xE5:
      mmu_write(--sp, HR);
      mmu_write(--sp, LR);
      break;
    case 0xF5:
      mmu_write(--sp, AR);
      mmu_write(--sp, FR);
      break;
  }
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
    FR = 0b01100000;
  } else {
    FR = 0b01000000;
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
  tmp_clock += 8;

  pc++;
}

void set() {
  uint8_t b = (code & 0x38) >> 3;
  uint8_t *r;
  uint8_t mask = 0x01 << b;
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
  tmp_clock += 8;

  pc++;
}

void jp_d16() {
  pc = (uint16_t)mmu_read(pc + 1) + ((uint16_t)mmu_read(pc + 2) << 8);
  tmp_clock += 16;

}

void cpl() {
  AR = ~AR;
  FR |= 0b01100000;
  tmp_clock += 4;

  pc++;
}

void ccf() {
  FR &= 0b10010000;
  if (FR & 0x10) {
    FR &= 0b11100000;
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

void ret_cc() {
  switch (code) {
    case 0xC0: //not zero
      if (!(FR & 0x80)) {
        tmp_clock += 20;

        pc = ((uint16_t)mmu_read(sp + 1) << 8) + (uint16_t)mmu_read(sp);
        sp += 2;
        return;
      }
      break;
    case 0xD0: //not carry
      if (!(FR & 0x10)) {
        tmp_clock += 20;

        pc = ((uint16_t)mmu_read(sp + 1) << 8) + (uint16_t)mmu_read(sp);
        sp += 2;
        return;
      }
      break;
    case 0xC8: //zero
      if (FR & 0x80) {
        tmp_clock += 20;

        pc = ((uint16_t)mmu_read(sp + 1) << 8) + (uint16_t)mmu_read(sp);
        sp += 2;
        return;
      }
      break;
    case 0xD8: //carry
      if (FR & 0x10) {
        tmp_clock += 20;

        pc = ((uint16_t)mmu_read(sp + 1) << 8) + (uint16_t)mmu_read(sp);
        sp += 2;
        return;
      }
      break;
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
    FR |= 0x20;
    FR &= 0b10100000;
  } else {
    FR &= 0x80;
  }
  t = (uint32_t)HL + (uint32_t)r16;
  if (t > 0xFFFF) {  // C
    FR |= 0x10;
  }
  HR = (uint8_t)(t & 0x0000FF00);
  LR = (uint8_t)(t & 0x000000FF);
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
  AR = AR + val_t;
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
  AR = AR - val_t;
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
  if (*val_t & 0x01 > 0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  *val_t = *val_t >> 1;
  if (*val_t == 0) FR |= 0x80;
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
  uint8_t bit_0 = *val_t & 0x01;
  if (FR & 0x10) {
    *val_t = (*val_t >> 1) | 0x80;
  } else {
    *val_t = (*val_t >> 1);
  }
  if (bit_0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  if (*val_t == 0) FR |= 0x80;
  tmp_clock += 8;
  pc++;
}

void rra() {
  uint8_t bit_0 = AR & 0x01;
  if (FR & 0x10) {
    AR = (AR >> 1) | 0x80;
  } else {
    AR = (AR >> 1);
  }
  if (bit_0) {
    FR = 0x10;
  } else {
    FR = 0x00;
  }
  tmp_clock += 4;
  pc++;
}

void xor_ar_d8() {
  AR = AR ^ mmu_read(++pc);
  if (AR == 0) {
    FR = 0x80;
  } else {
    FR = 0x00;
  }
  tmp_clock += 8;
  pc++;
}

void or_ar_phl() {
  AR |= mmu_read(HL(HR, LR));
  if (AR == 0) {
    FR = 0x80;
  } else {
    FR = 0x00;
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
  if (!(FR & 0b01000000)) {
    if ((FR & 0x10) || AR > 0x99) {
      AR += 0x60;
      FR |= 0x10;
    }
    if ((FR & 0x20) || ((AR & 0x0F) > 0x09)) AR -= 0x06;
  } else {
    if (FR & 0x10) AR -= 0x60;
    if (FR & 0x20) AR -= 0x06;
  }
  if (AR == 0) {
    FR |= 0x80;
  } else {
    FR &= 0b01110000;
  }
  FR &= 0b11010000;
  tmp_clock += 4;
  pc++;
}

void scf() {
  FR &= 0x80;
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
  uint16_t val_t = (uint16_t)mmu_read(++pc);

  if ((sp & 0x0F) + (val_t & 0x0F) > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((sp & 0xFF) + (val_t & 0xFF) > 0xFF) FR |= 0x10;

  sp += val_t;
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
  uint16_t val_t = (uint16_t)mmu_read(++pc);

  if ((sp & 0x0F) + (val_t & 0x0F) > 0x0F) {
    FR = 0x20;
  } else {
    FR = 0x00;
  }
  if ((sp & 0xFF) + (uint16_t)(val_t & 0xFF) > 0xFF) FR |= 0x10;

  val_t += sp;
  HR = (uint8_t)(val_t >> 8);
  LR = (uint8_t)(val_t & 0x00FF);
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
