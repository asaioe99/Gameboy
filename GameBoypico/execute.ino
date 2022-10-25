void execute() {
  code = get_byte(pc);

  char buf[64];
  if (pc >= 0x24 && pc <= 0x100) {
    sprintf(buf, "pc:%04X->%02X AF:%02X%02X BC:%02X%02X DE:%02X%02X HL:%02X%02X sp:%04X SL:%03d", pc, code, AR, FR, BR, CR, DR, ER, HR, LR, sp, scaline_counter);
    Serial.println(buf);
    sprintf(buf, "LCDC:%02X LCDS:%02X SCY:%02X SCX:%02X LY:%02X LYC:%02X", *(io + 0x40), *(io + 0x41), *(io + 0x42), *(io + 0x43), *(io + 0x44), *(io + 0x45));
    Serial.println(buf);
    //delay(10);
  }

  //if (pc >= 0x55 && pc <= 0x5d) { //0x55 is the one

  if (pc == 0xFA) { //0x55 is the one
    display_tile(0);
    gpio_put(25, HIGH);
  } else {
    gpio_put(25, LOW);
  }

  cc_dec = 0;
  if (code == 0xCB) {
    code = get_byte(++pc);
    switch (code & 0b11000000) {
      case 0b00000000: // 0x0*** ~ 0x3***
        if ((code >= 0x10 && code <= 0x15) || code == 0x17) {
          RL_r();
          return;
        }
        break;
      case 0b01000000: // 0x4*** ~ 0x7***
        if (code & 0b00000111 == 0b00000110) {
          return;
        } else {
          BIT();
          return;
        }
        break;
      case 0b10000000: // 0x8*** ~ 0xB***
        break;
      case 0b11000000: // 0xC*** ~ 0xF***
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
            JR_cc_d8();
            return;
          }
          break;
        case 0b00000001: // 1
          LD_r16_d16();
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
          INC_rr();
          return;
          break;
        case 0b00000100: // 4
          if (code == 0x34) {
            return;
          } else {
            INC_r();
            return;
          }
          break;
        case 0b00000101: // 5
          if (code == 0x35) {
            return;
          } else {
            DEC_r();
            return;
          }
          break;
        case 0b00000110:
          if (code == 0x06 || code == 0x16 || code == 0x26) {
            LD_r_n();
            return;
          } else {
            LD_HL_n();
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
            JR_cc_d8();
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
          DEC_rr();
          return;
          break;
        case 0b00001100: // C
          INC_r();
          return;
          break;
        case 0b00001101: // D
          DEC_r();
          return;
          break;
        case 0b00001110:
          LD_r_n();
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
          LD_r_HL();
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
        default:
          LD_r_r();
          break;
      }
      break;
    case 0b10000000: //0x80 ~ 0xB0
      switch (code & 0b11110000) {
        case 0b10000000: //0x80
          if (code >= 0x80 && code <= 0x87) {
            ADD_r();
            return;
          }
          break;
        case 0b10010000: //0x90
          if (code >= 0x90 && code <= 0x97) {
            SUB_r();
            return;
          }
          break;
          break;
        case 0b10100000: //0xA0
          if (code >= 0xA8 && code <= 0xAF) {
            XOR_r();
            return;
          }
          break;
        case 0b10110000: //0xB0
          if (code >= 0xB8 && code <= 0xBF) {
            CP_r();
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
            LDH_a8_a();
            return;
          } else {
            LDH_a_a8();
            return;
          }
          break;
        case 0b00000001: // 1
          POP_r16();
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
          PUSH_r16();
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
