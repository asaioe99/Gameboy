void execute() {
  code = get_byte(pc);

  /*
    char buf[64];
    if (pc >= 0x24 && pc <= 0x100) {
      sprintf(buf, "pc:%04X->%02X AF:%02X%02X BC:%02X%02X DE:%02X%02X HL:%02X%02X sp:%04X SL:%03d", pc, code, AR, FR, BR, CR, DR, ER, HR, LR, sp, scaline_counter);
      Serial.println(buf);
      sprintf(buf, "LCDC:%02X LCDS:%02X SCY:%02X SCX:%02X LY:%02X LYC:%02X", *(io + 0x40), *(io + 0x41), *(io + 0x42), *(io + 0x43), *(io + 0x44), *(io + 0x45));
      Serial.println(buf);
      //delay(10);
    }

    //if (pc >= 0x55 && pc <= 0x5d) { //0x55 is the one
  */
  if (pc == 0x5D && AR == 0x91) gpio_put(25, HIGH);
  //if (pc == 0x5F) { //0x55 is the one
  //display_tile(0);
  //gpio_put(25, HIGH);
  //delay(5000);
  //gpio_put(25, LOW);
  //delay(1000);
  //if (*(io + 0x40) == 0x91) {
  //  gpio_put(25, HIGH);
  //  delay(5000);
  //}
  //} //else {
  //gpio_put(25, LOW);
  //}
  cc_dec = 0;
  switch (code) {
    case 0xCB: // prefix
      code = get_byte(++pc);
      switch (code) {
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x17:
          RL_r();
          break;
        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43:
        case 0x44:
        case 0x45:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4A:
        case 0x4B:
        case 0x4C:
        case 0x4D:
        case 0x4F:
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x55:
        case 0x57:
        case 0x58:
        case 0x59:
        case 0x5A:
        case 0x5B:
        case 0x5C:
        case 0x5D:
        case 0x5F:
        case 0x60:
        case 0x61:
        case 0x62:
        case 0x63:
        case 0x64:
        case 0x65:
        case 0x67:
        case 0x68:
        case 0x69:
        case 0x6A:
        case 0x6B:
        case 0x6C:
        case 0x6D:
        case 0x6F:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75:
        case 0x77:
        case 0x78:
        case 0x79:
        case 0x7A:
        case 0x7B:
        case 0x7C:
        case 0x7D:
        case 0x7F:
          BIT();
          break;
      }
      break;
    case 0x06: //LD_B_n
    case 0x0E: //LD_C_n
    case 0x16: //LD_D_n
    case 0x1E: //LD_E_n
    case 0x26: //LD_H_n
    case 0x2E: //LD_L_n
    case 0x3E: //LD_A_n
      LD_r_n();
      break;
    case 0x46: //LD_B_(HL)
    case 0x4E: //LD_C_(HL)
    case 0x56: //LD_D_(HL)
    case 0x5E: //LD_E_(HL)
    case 0x66: //LD_H_(HL)
    case 0x6E: //LD_L_(HL)
    case 0x7E: //LD_A_(HL)
      LD_r_HL();
      break;
    case 0x40: //LD_B_B
    case 0x41: //LD_B_C
    case 0x42: //LD_B_D
    case 0x43: //LD_B_E
    case 0x44: //LD_B_H
    case 0x45: //LD_B_L
    case 0x47: //LD_B_A
    case 0x48: //LD_C_B
    case 0x49: //LD_C_C
    case 0x4A: //LD_C_D
    case 0x4B: //LD_C_E
    case 0x4C: //LD_C_H
    case 0x4D: //LD_C_L
    case 0x4F: //LD_C_A
    case 0x50: //LD_D_B
    case 0x51: //LD_D_C
    case 0x52: //LD_D_D
    case 0x53: //LD_D_E
    case 0x54: //LD_D_H
    case 0x55: //LD_D_L
    case 0x57: //LD_D_A
    case 0x58: //LD_E_B
    case 0x59: //LD_E_C
    case 0x5A: //LD_E_D
    case 0x5B: //LD_E_E
    case 0x5C: //LD_E_H
    case 0x5D: //LD_E_L
    case 0x5F: //LD_E_A
    case 0x60: //LD_H_B
    case 0x61: //LD_H_C
    case 0x62: //LD_H_D
    case 0x63: //LD_H_E
    case 0x64: //LD_H_H
    case 0x65: //LD_H_L
    case 0x67: //LD_H_A
    case 0x68: //LD_L_B
    case 0x69: //LD_L_C
    case 0x6A: //LD_L_D
    case 0x6B: //LD_L_E
    case 0x6C: //LD_L_H
    case 0x6D: //LD_L_L
    case 0x6F: //LD_L_A
    case 0x7F: //LD_A_A
    case 0x78: //LD_A_C
    case 0x79: //LD_A_C
    case 0x7A: //LD_A_D
    case 0x7B: //LD_A_E
    case 0x7C: //LD_A_H
    case 0x7D: //LD_A_L
      LD_r_r();
      break;
    case 0x70: //LD_(HL)_B
    case 0x71: //LD_(HL)_C
    case 0x72: //LD_(HL)_D
    case 0x73: //LD_(HL)_E
    case 0x74: //LD_(HL)_H
    case 0x75: //LD_(HL)_L
    case 0x77: //LD_(HL)_A
      LD_HL_r();
      break;
    case 0x36:
      LD_HL_n();
      break;
    case 0x0A:
      LD_A_BC();
      break;
    case 0x1A:
      LD_A_DE();
      break;
    case 0x2A:
      LD_A_HLi();
      break;
    case 0x3A:
      LD_A_HLd();
      break;
    case 0x02:
      LD_BC_A();
      break;
    case 0x12:
      LD_DE_A();
      break;
    case 0x22:
      LD_HLi_A();
      break;
    case 0x32:
      LD_HLd_A();
      break;
    case 0x00:
      NOP();
      break;
    case 0x03: //INC_BC
    case 0x13: //INC_DE
    case 0x23: //INC_HL
    case 0x33: //INC_SP
      INC_rr();
      break;
    case 0x0B: //DEC_BC
    case 0x1B: //DEC_DE
    case 0x2B: //DEC_HL
    case 0x3B: //DEC_SP
      DEC_rr();
      break;
    case 0x04: //INC_B
    case 0x0C: //INC_C
    case 0x14: //INC_D
    case 0x1C: //INC_E
    case 0x24: //INC_H
    case 0x2C: //INC_L
    case 0x3C: //INC_A
      INC_r();
      break;
    case 0x05: //DEC_B
    case 0x0D: //DEC_C
    case 0x15: //DEC_D
    case 0x1D: //DEC_E
    case 0x25: //DEC_H
    case 0x2D: //DEC_L
    case 0x3D: //DEC_A
      DEC_r();
      break;
    case 0xA8: //XOR_B
    case 0xA9: //XOR_C
    case 0xAA: //XOR_D
    case 0xAB: //XOR_E
    case 0xAC: //XOR_H
    case 0xAD: //XOR_L
    case 0xAE: //XOR_(HL)
    case 0xAF: //XOR_L
      XOR_r();
      break;
    case 0x90: //SUB_B
    case 0x91: //SUB_C
    case 0x92: //SUB_D
    case 0x93: //SUB_E
    case 0x94: //SUB_H
    case 0x95: //SUB_L
    case 0x96: //SUB_(HL)
    case 0x97: //SUB_L
      SUB_r();
      break;
    case 0x80: //ADD_B
    case 0x81: //ADD_C
    case 0x82: //ADD_D
    case 0x83: //ADD_E
    case 0x84: //ADD_H
    case 0x85: //ADD_L
    case 0x86: //ADD_(HL)
    case 0x87: //ADD_L
      ADD_r();
      break;
    case 0xB8: //CP_B
    case 0xB9: //CP_C
    case 0xBA: //CP_D
    case 0xBB: //CP_E
    case 0xBC: //CP_H
    case 0xBD: //CP_L
    case 0xBE: //CP_(HL)
    case 0xBF: //CP_L
      CP_r();
      break;
    case 0x20:
    case 0x30:
    case 0x18:
    case 0x28:
    case 0x38:
      JR_cc_d8();
      break;
    case 0x01:
    case 0x11:
    case 0x21:
    case 0x31:
      LD_r16_d16();
      break;
    case 0xE0:
      LDH_a8_a();
      break;
    case 0xF0:
      LDH_a_a8();
      break;
    case 0xC1:
    case 0xD1:
    case 0xE1:
    case 0xF1:
      POP_r16();
      break;
    case 0xE2:
      LDH_c_a();
      break;
    case 0xF2:
      LDH_a_c();
      break;
    case 0xF3:
      DI();
      break;
    case 0xFB:
      EI();
      break;
    case 0xC5:
    case 0xD5:
    case 0xE5:
    case 0xF5:
      PUSH_r16();
      break;
    case 0xC9:
      RET();
      break;
    case 0xEA:
      LD_a16p_a();
      break;
    case 0xCD:
      CALL();
      break;
    case 0xFE:
      CP_d8();
      break;
    default:
      NOP();
  }
}
