void execute(uint8_t pc) {
  uint8_t code = fetch(pc);

  switch(code) {
    case 0x06: //LD_B_n
    case 0x0E: //LD_C_n
    case 0x16: //LD_D_n
    case 0x1E: //LD_E_n
    case 0x26: //LD_H_n
    case 0x2E: //LD_L_n
    case 0x3E: //LD_A_n
      LD_r_n(code);
      break;
    case 0x46: //LD_B_(HL)
    case 0x4E: //LD_C_(HL)
    case 0x56: //LD_D_(HL)
    case 0x5E: //LD_E_(HL)
    case 0x66: //LD_H_(HL)
    case 0x6E: //LD_L_(HL)
    case 0x7E: //LD_A_(HL)
      LD_r_HL(code);
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
      LD_r_r(code);
      break;
    case 0x70: //LD_(HL)_B 
    case 0x71: //LD_(HL)_C
    case 0x72: //LD_(HL)_D
    case 0x73: //LD_(HL)_E
    case 0x74: //LD_(HL)_H
    case 0x75: //LD_(HL)_L
    case 0x77: //LD_(HL)_A
      LD_HL_r(code);
      break;
    case 0x36:
      LD_HL_n(code);
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
      INC_rr(code);
      break;
    case 0x0B: //DEC_BC
    case 0x1B: //DEC_DE
    case 0x2B: //DEC_HL
    case 0x3B: //DEC_SP
      DEC_rr(code);
      break;
    case 0x04: //INC_B
    case 0x0C: //INC_C
    case 0x14: //INC_D
    case 0x1C: //INC_E
    case 0x24: //INC_H
    case 0x2C: //INC_L
    case 0x3C: //INC_A
      INC_r(code); 
      break;
    case 0x05: //DEC_B
    case 0x0D: //DEC_C
    case 0x15: //DEC_D
    case 0x1D: //DEC_E
    case 0x25: //DEC_H
    case 0x2D: //DEC_L
    case 0x3D: //DEC_A
      DEC_r(code); 
      break;
    case 0xA8: //XOR_B
    case 0xA9: //XOR_C
    case 0xAA: //XOR_D
    case 0xAB: //XOR_E
    case 0xAC: //XOR_H
    case 0xAD: //XOR_L
    case 0xAE: //XOR_(HL)
    case 0xAF: //XOR_L
      XOR_r(code); 
      break;
    case 0x90: //SUB_B
    case 0x91: //SUB_C
    case 0x92: //SUB_D
    case 0x93: //SUB_E
    case 0x94: //SUB_H
    case 0x95: //SUB_L
    case 0x96: //SUB_(HL)
    case 0x97: //SUB_L
      SUB_r(code); 
      break;
    case 0x80: //ADD_B
    case 0x81: //ADD_C
    case 0x82: //ADD_D
    case 0x83: //ADD_E
    case 0x84: //ADD_H
    case 0x85: //ADD_L
    case 0x86: //ADD_(HL)
    case 0x87: //ADD_L
      ADD_r(code); 
      break;
    case 0xB8: //CP_B
    case 0xB9: //CP_C
    case 0xBA: //CP_D
    case 0xBB: //CP_E
    case 0xBC: //CP_H
    case 0xBD: //CP_L
    case 0xBE: //CP_(HL)
    case 0xBF: //CP_L
      CP_r(code); 
      break;
    case 0x20:
    case 0x30:
    case 0x18:
    case 0x28:
    case 0x38:
      JR_cc_d8(code);
      break;
    case 0x01:
    case 0x11:
    case 0x21:
    case 0x31:
      LD_r16_d16(code);
      break;
    case 0xE0:
      LD_a8_a(code);
      break;
    case 0xF0:
      LD_a_a8(code);
      break;
    default:
      NOP();
  }
}
