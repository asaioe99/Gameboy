void execute() {

  code = get_byte(pc);

  if (pc == 0x100) {
    //chk_init_regs();
    start_flag = 0;
  }

  if (pc >= 0x40 && pc <= 0x69 && !start_flag || pc >=0x338 && pc <= 0x3B6) {
    //gpio_put(25, HIGH);
    sprintf(buf_b1, "pc:%04X->%02X AF:%02X%02X BC:%02X%02X DE:%02X%02X HL:%02X%02X sp:%04X", pc, code, AR, FR, BR, CR, DR, ER, HR, LR, sp);
    //sprintf(buf_b1, "OAM0:%02X %02X %02X %02X", get_byte(0xFE00), get_byte(0xFE01), get_byte(0xFE02), get_byte(0xFE03));
    //sprintf(buf_b2, "[sp]:%02X%02X", get_byte(sp + 1), get_byte(sp));

    //sprintf(buf_b2, "LCDC:%02X STAT:%02X SCY:%02X SCX:%02X LY:%02X LYC:%02X WY:%02X WX:%02X", *(io + 0x40), *(io + 0x41), *(io + 0x42), *(io + 0x43), *(io + 0x44), *(io + 0x45), *(io + 0x4A), *(io + 0x4B));
    //if (pc >= 0xC000) {
    Serial.println(buf_b1);
    //Serial.println(buf_b2);
    //delay(5000);
    //}
    //if (pc >= 0xC000 && pc < 0xE000) {
    //  while (1) {}
    //}
  }
  //gpio_put(25, LOW);

  switch (code) {
    case 0xCB: // prefix
      code = get_byte(++pc);
      switch (code) {
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x23:
        case 0x24:
        case 0x25:
        case 0x27:
          sla_r8();
          break;
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x07:
          rlc_r8();
          break;
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x17:
          rl_r8();
          break;
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1F:
          rr_r8();
          break;
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x37:
          swap();
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
          bit_();
          break;
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8F:
        case 0x90:
        case 0x91:
        case 0x92:
        case 0x93:
        case 0x94:
        case 0x95:
        case 0x97:
        case 0x98:
        case 0x99:
        case 0x9A:
        case 0x9B:
        case 0x9C:
        case 0x9D:
        case 0x9F:
        case 0xA0:
        case 0xA1:
        case 0xA2:
        case 0xA3:
        case 0xA4:
        case 0xA5:
        case 0xA7:
        case 0xA8:
        case 0xA9:
        case 0xAA:
        case 0xAB:
        case 0xAC:
        case 0xAD:
        case 0xAF:
        case 0xB0:
        case 0xB1:
        case 0xB2:
        case 0xB3:
        case 0xB4:
        case 0xB5:
        case 0xB7:
        case 0xB8:
        case 0xB9:
        case 0xBA:
        case 0xBB:
        case 0xBC:
        case 0xBD:
        case 0xBF:
          res();
          break;
        case 0xC0:
        case 0xC1:
        case 0xC2:
        case 0xC3:
        case 0xC4:
        case 0xC5:
        case 0xC7:
        case 0xC8:
        case 0xC9:
        case 0xCA:
        case 0xCB:
        case 0xCC:
        case 0xCD:
        case 0xCF:
        case 0xD0:
        case 0xD1:
        case 0xD2:
        case 0xD3:
        case 0xD4:
        case 0xD5:
        case 0xD7:
        case 0xD8:
        case 0xD9:
        case 0xDA:
        case 0xDB:
        case 0xDC:
        case 0xDD:
        case 0xDF:
        case 0xE0:
        case 0xE1:
        case 0xE2:
        case 0xE3:
        case 0xE4:
        case 0xE5:
        case 0xE7:
        case 0xE8:
        case 0xE9:
        case 0xEA:
        case 0xEB:
        case 0xEC:
        case 0xED:
        case 0xEF:
        case 0xF0:
        case 0xF1:
        case 0xF2:
        case 0xF3:
        case 0xF4:
        case 0xF5:
        case 0xF7:
        case 0xF8:
        case 0xF9:
        case 0xFA:
        case 0xFB:
        case 0xFC:
        case 0xFD:
        case 0xFF:
          set();
          break;
        case 0x38:
        case 0x39:
        case 0x3A:
        case 0x3B:
        case 0x3C:
        case 0x3D:
        case 0x3F:
          srl_r8();
          break;
        case 0x28:
        case 0x29:
        case 0x2A:
        case 0x2B:
        case 0x2C:
        case 0x2D:
        case 0x2F:
          sra_r8();
          break;
        case 0x08:
        case 0x09:
        case 0x0A:
        case 0x0B:
        case 0x0C:
        case 0x0D:
        case 0x0F:
          rrc_r8();
          break;
        case 0xC6:
        case 0xD6:
        case 0xE6:
        case 0xF6:
        case 0xCE:
        case 0xDE:
        case 0xEE:
        case 0xFE:
          set_phl();
          break;
        case 0x86:
        case 0x96:
        case 0xA6:
        case 0xB6:
        case 0x8E:
        case 0x9E:
        case 0xAE:
        case 0xBE:
          res_phl();
          break;
        case 0x46:
        case 0x56:
        case 0x66:
        case 0x76:
        case 0x4E:
        case 0x5E:
        case 0x6E:
        case 0x7E:
          bit_phl();
          break;
        case 0x36:
          swap_phl();
          break;
        case 0x26:
          sla_phl();
          break;
        case 0x06:
          rlc_phl();
          break;
        case 0x16:
          rl_phl();
          break;
        case 0x0E:
          rrc_phl();
          break;
        case 0x1E:
          rr_phl();
          break;
        case 0x2E:
          sra_phl();
          break;
        case 0x3E:
          srl_phl();
          break;
        default:
          gpio_put(25, HIGH);
          while (1) {
            Serial.print("pc:");
            Serial.print(pc, HEX);
            Serial.print(" code: CB ");
            Serial.println(code, HEX);
            Serial.print("rom_bank_num:");
            Serial.println(rom_bank_num, HEX);
            chk_init_regs();
            dump_tilemap();
            delay(10000);
          }
      }
      break;
    case 0x06: //B
    case 0x0E: //C
    case 0x16: //D
    case 0x1E: //E
    case 0x26: //H
    case 0x2E: //L
    case 0x3E: //A
      ld_r8_d8();
      break;
    case 0x46: //LD_B_(HL)
    case 0x4E: //LD_C_(HL)
    case 0x56: //LD_D_(HL)
    case 0x5E: //LD_E_(HL)
    case 0x66: //LD_H_(HL)
    case 0x6E: //LD_L_(HL)
    case 0x7E: //LD_A_(HL)
      ld_r8_phl();
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
    case 0x78: //LD_A_C
    case 0x79: //LD_A_C
    case 0x7A: //LD_A_D
    case 0x7B: //LD_A_E
    case 0x7C: //LD_A_H
    case 0x7D: //LD_A_L
    case 0x7F: //LD_A_A
      ld_r8_r8();
      break;
    case 0x70: //LD_(HL)_B
    case 0x71: //LD_(HL)_C
    case 0x72: //LD_(HL)_D
    case 0x73: //LD_(HL)_E
    case 0x74: //LD_(HL)_H
    case 0x75: //LD_(HL)_L
    case 0x77: //LD_(HL)_A
      ld_phl_r8();
      break;
    case 0x36:
      ld_phl_d8();
      break;
    case 0x0A:
    case 0x1A:
      ld_ar_pr16();
      break;
    case 0x2A:
      ld_ar_phli();
      break;
    case 0x3A:
      ld_ar_phld();
      break;
    case 0x02:
    case 0x12:
      ld_pr16_ar();
      break;
    case 0x22:
      ld_phli_ar();
      break;
    case 0x32:
      ld_phld_ar();
      break;
    case 0x00:
      nop();
      break;
    case 0x03: //INC_BC
    case 0x13: //INC_DE
    case 0x23: //INC_HL
    case 0x33: //INC_SP
      inc_r16();
      break;
    case 0x0B: //DEC_BC
    case 0x1B: //DEC_DE
    case 0x2B: //DEC_HL
    case 0x3B: //DEC_SP
      dec_r16();
      break;
    case 0x04: //INC_B
    case 0x0C: //INC_C
    case 0x14: //INC_D
    case 0x1C: //INC_E
    case 0x24: //INC_H
    case 0x2C: //INC_L
    case 0x3C: //INC_A
      inc_r8();
      break;
    case 0x05: //DEC_B
    case 0x0D: //DEC_C
    case 0x15: //DEC_D
    case 0x1D: //DEC_E
    case 0x25: //DEC_H
    case 0x2D: //DEC_L
    case 0x3D: //DEC_A
      dec_r8();
      break;
    case 0xA8: //XOR_B
    case 0xA9: //XOR_C
    case 0xAA: //XOR_D
    case 0xAB: //XOR_E
    case 0xAC: //XOR_H
    case 0xAD: //XOR_L
    case 0xAE: //XOR_(HL)
    case 0xAF: //XOR_L
      xor_ar_r8();
      break;
    case 0x90: //SUB_B
    case 0x91: //SUB_C
    case 0x92: //SUB_D
    case 0x93: //SUB_E
    case 0x94: //SUB_H
    case 0x95: //SUB_L
    case 0x96: //SUB_(HL)
    case 0x97: //SUB_L
      sub_ar_r8();
      break;
    case 0x80: //ADD_B
    case 0x81: //ADD_C
    case 0x82: //ADD_D
    case 0x83: //ADD_E
    case 0x84: //ADD_H
    case 0x85: //ADD_L
    case 0x86: //ADD_(HL)
    case 0x87: //ADD_L
      add_ar_r8();
      break;
    case 0xB8: //CP_B
    case 0xB9: //CP_C
    case 0xBA: //CP_D
    case 0xBB: //CP_E
    case 0xBC: //CP_H
    case 0xBD: //CP_L
    case 0xBE: //CP_(HL)
    case 0xBF: //CP_L
      cp_r8();
      break;
    case 0x20:
    case 0x30:
    case 0x18:
    case 0x28:
    case 0x38:
      jr_cc_d8();
      break;
    case 0x01:
    case 0x11:
    case 0x21:
    case 0x31:
      ld_r16_d16();
      break;
    case 0xE0:
      ldh_pd8_ar();
      break;
    case 0xF0:
      ldh_ar_pd8();
      break;
    case 0xC1:
    case 0xD1:
    case 0xE1:
    case 0xF1:
      pop_r16();
      break;
    case 0xE2:
      ld_pcr_ar();
      break;
    case 0xF2:
      ld_ar_pcr();
      break;
    case 0xF3:
      di();
      break;
    case 0xFB:
      ei();
      break;
    case 0xC5:
    case 0xD5:
    case 0xE5:
    case 0xF5:
      push_r16();
      break;
    case 0xC9:
      ret();
      break;
    case 0xCD:
      call_d16();
      break;
    case 0xFE:
      cp_d8();
      break;
    case 0x17:
      rla();
      break;
    case 0xC3:
      jp_d16();
      break;
    case 0x2F:
      cpl();
      break;
    case 0x3F:
      ccf();
      break;
    case 0xE6:
      and_d8();
      break;
    case 0xF6:
      or_d8();
      break;
    case 0xB0:
    case 0xB1:
    case 0xB2:
    case 0xB3:
    case 0xB4:
    case 0xB5:
    case 0xB7:
      or_r8();
      break;
    case 0xA0:
    case 0xA1:
    case 0xA2:
    case 0xA3:
    case 0xA4:
    case 0xA5:
    case 0xA7:
      and_r8();
      break;
    case 0xFA:
      ld_ar_pa16();
      break;
    case 0xEA:
      ld_pa16_ar();
      break;
    case 0xC0:
    case 0xD0:
    case 0xC8:
    case 0xD8:
      ret_cc();
      break;
    case 0xD9:
      reti();
      break;
    case 0x09:
    case 0x19:
    case 0x29:
    case 0x39:
      add_hl_r16();
      break;
    case 0x10:
      stop_0();
      break;
    case 0xC2:
    case 0xD2:
    case 0xCA:
    case 0xDA:
      jp_cc_d16();
      break;
    case 0xC4:
    case 0xD4:
    case 0xCC:
    case 0xDC:
      call_cc_d16();
      break;
    case 0x88: //ADC_B
    case 0x89: //ADC_C
    case 0x8A: //ADC_D
    case 0x8B: //ADC_E
    case 0x8C: //ADC_H
    case 0x8D: //ADC_L
    case 0x8E: //ADC_(HL)
    case 0x8F: //ADC_A
      adc_ar_r8();
      break;
    case 0xC7: //RST 00H
    case 0xD7: //RST 10H
    case 0xE7: //RST 20H
    case 0xF7: //RST 30H
    case 0xCF: //RST 08H
    case 0xDF: //RST 18H
    case 0xEF: //RST 28H
    case 0xFF: //RST 38H
      rst_vec();
      break;
    case 0x98: //SBC_B
    case 0x99: //SBC_C
    case 0x9A: //SBC_D
    case 0x9B: //SBC_E
    case 0x9C: //SBC_H
    case 0x9D: //SBC_L
    case 0x9E: //SBC_(HL)
    case 0x9F: //SBC_A
      sbc_ar_r8();
      break;
    case 0xC6:
      add_ar_d8();
      break;
    case 0xCE:
      adc_ar_d8();
      break;
    case 0xD6:
      sub_ar_d8();
      break;
    case 0xDE:
      sbc_ar_d8();
      break;
    case 0x1F:
      rra();
      break;
    case 0xEE:
      xor_ar_d8();
      break;
    case 0xB6:
      or_ar_phl();
      break;
    case 0xA6:
      and_ar_phl();
      break;
    case 0x35:
      dec_phl();
      break;
    case 0x34:
      inc_phl();
      break;
    case 0x76:
      halt();
      break;
    case 0x07:
      rlca();
      break;
    case 0x27:
      daa();
      break;
    case 0x37:
      scf();
      break;
    case 0x08:
      ld_pd16_sp();
      break;
    case 0xE8:
      add_sp_d8();
      break;
    case 0xE9:
      jp_hl();
      break;
    case 0xF9:
      ld_sp_hl();
      break;
    case 0x0F:
      rrca();
      break;
    case 0xF8:
      ld_hl_sp_d8();
      break;
    default:
      gpio_put(25, HIGH);
      while (1) {
        Serial.print("pc:");
        Serial.print(pc, HEX);
        Serial.print(" code:");
        Serial.println(code, HEX);
        Serial.print("rom_bank_num:");
        Serial.println(rom_bank_num, HEX);
        chk_init_regs();
        dump_tilemap();
        delay(10000);
      }
      nop();
  }
}
