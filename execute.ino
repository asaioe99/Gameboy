void execute() {
  uint8_t code = fetch(pc);

  char buf[64];
  if (pc > 0xE0) {
    sprintf(buf, "pc:%04X->%02X AF:%02X%02X BC:%02X%02X DE:%02X%02X HL:%02X%02X sp:%04X LY:%02X SL:%03d", pc, code, A, F, B, C, D, E, H, L, sp, get_byte(0xFF44), scaline_counter);
    Serial.println(buf);
    //delay(10);
  }
  cc_dec = 0;
  if (code == 0xCB) {
    code = fetch(++pc);
    switch (code) {
      case 0x10:
      case 0x11:
      case 0x12:
      case 0x13:
      case 0x14:
      case 0x15:
      case 0x17:
        RL_r(code);
        return;
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
        BIT(code);
        return;
        break;
    }
    return;
  }
  switch (code & 0b11000000) {
    case 0b00000000: // 0x0*** ~ 0x3***
      switch (code & 0b00001111) {
        case 0b00000000:
          if (code == 0x00) {
            NOP();
            return;
          } else if (code == 0x10) {
            return;
          } else {
            JR_cc_d8(code);
            return;
          }
          break;
        case 0b00000001: // 1
          LD_r16_d16(code);
          return;
          break;
        case 0b00000010:
          if (code == 0x02) {
            LD_BC_A();
            return;
          } else if (code == 0x12) {
            LD_DE_A();
            return;
          } else if (code == 0x22) {
            LD_HLi_A();
            return;
          } else {
            LD_HLd_A();
            return;
          }
          break;
        case 0b00000011:
          INC_rr(code);
          return;
          break;
        case 0b00000100: // 4
          if (code == 0x34) {
            return;
          } else {
            INC_r(code);
            return;
          }
          break;
        case 0b00000101: // 5
          if (code == 0x35) {
            return;
          } else {
            DEC_r(code);
            return;
          }
          break;
        case 0b00000110:
          if (code == 0x06 || code == 0x16 || code == 0x26) {
            LD_r_n(code);
            return;
          } else {
            LD_HL_n(code);
            return;
          }
          break;
        case 0b00000111: // 7
          if (code == 0x17) {
            RLA();
            return;
          }
          break;
        case 0b00001000: // 8
          if (code == 0x08) {
            return;
          } else {
            JR_cc_d8(code);
            return;
          }
          break;
        case 0b00001001:
          break;
        case 0b00001010: // A
          if (code == 0x0A) {
            LD_A_BC();
            return;
          } else if (code == 0x1A) {
            LD_A_DE();
            return;
          } else if (code == 0x2A) {
            LD_A_HLi();
            return;
          } else {
            LD_A_HLd();
            return;
          }
          break;
        case 0b00001011: // B
          DEC_rr(code);
          return;
          break;
        case 0b00001100: // C
          INC_r(code);
          return;
          break;
        case 0b00001101: // D
          DEC_r(code);
          return;
          break;
        case 0b00001110:
          LD_r_n(code);
          return;
          break;
        case 0b00001111:
          break;
      }
      break;
    case 0b01000000: // ok
      if (code == 0x76) return; // HALT
      switch (code & 0b00000111) {
        case 0b110:
          LD_r_HL(code);
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
        default:
          LD_r_r(code);
          break;
      }
      break;
    case 0b10000000: //0x80 ~ 0xB0
      switch (code & 0b11110000) {
        case 0b10000000: //0x80
          if (code >= 0x80 && code <= 0x87) {
            ADD_r(code);
            return;
          }
          break;
        case 0b10010000: //0x90
          if (code >= 0x90 && code <= 0x97) {
            SUB_r(code);
            return;
          }
          break;
          break;
        case 0b10100000: //0xA0
          if (code >= 0xA8 && code <= 0xAF) {
            XOR_r(code);
            return;
          }
          break;
        case 0b10110000: //0xB0
          if (code >= 0xB8 && code <= 0xBF) {
            CP_r(code);
            return;
          }
          break;
      }
      break;
    case 0b11000000:
      switch (code & 0b00001111) {
        case 0b00000000: //0
          if (code == 0xC0) {
            return;
          } else if (code == 0xD0) {
            return;
          } else if (code == 0xE0) {
            LDH_a8_a(code);
            return;
          } else {
            LDH_a_a8(code);
            return;
          }
          break;
        case 0b00000001: // 1
          POP_r16(code);
          return;
          break;
        case 0b00000010: // 2
          if (code == 0xC2) {
            return;
          } else if (code == 0xD2) {
            return;
          } else if (code == 0xE2) {
            LDH_c_a();
            return;
          } else {
            LDH_a_c();
            return;
          }
          break;
        case 0b00000011:
          if (code == 0xF3) {
            DI();
            return;
          }
          break;
        case 0b00000100:
          break;
        case 0b00000101: //5
          PUSH_r16(code);
          return;
          break;
        case 0b00000110:
          break;
        case 0b00000111:
          break;
        case 0b00001000:
          break;
        case 0b00001001:
          if (code == 0xC9) {
            RET();
            return;
          }
          break;
        case 0b00001010:
          if (code == 0xEA) {
            LD_a16p_a();
            return;
          }
          break;
        case 0b00001011: // B
          if (code == 0xFB) {
            EI();
            return;
          }
          break;
        case 0b00001100:
          break;
        case 0b00001101: // D
          if (code == 0xCD) {
            CALL();
            return;
          }
          break;
        case 0b00001110: // E
          if (code == 0xFE) {
            CP_d8();
            return;
          }
          break;
        case 0b00001111:
          break;
      }
      break;
  }
  return;
}
