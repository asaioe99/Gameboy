uint8_t mbc_get_rom(uint16_t addr) {
  switch (rom_bank_num) {
    case 0x01:
      return *(rom_bank01 + addr - 0x4000);
    case 0x02:
      return *(rom_bank02 + addr - 0x4000);
    case 0x03:
      return *(rom_bank03 + addr - 0x4000);
      /*
    case 0x04:
      return *(rom_bank04 + addr - 0x4000);
    case 0x05:
      return *(rom_bank05 + addr - 0x4000);
    case 0x06:
      return *(rom_bank06 + addr - 0x4000);
    case 0x07:
      return *(rom_bank07 + addr - 0x4000);
    case 0x08:
      return *(rom_bank08 + addr - 0x4000);
    case 0x09:
      return *(rom_bank09 + addr - 0x4000);
    case 0x0A:
      return *(rom_bank0A + addr - 0x4000);
    case 0x0B:
      return *(rom_bank0B + addr - 0x4000);
    case 0x0C:
      return *(rom_bank0C + addr - 0x4000);
    case 0x0D:
      return *(rom_bank0D + addr - 0x4000);
    case 0x0E:
      return *(rom_bank0E + addr - 0x4000);
    case 0x0F:
      return *(rom_bank0F + addr - 0x4000);
    case 0x10:
      return *(rom_bank10 + addr - 0x4000);
    case 0x11:
      return *(rom_bank11 + addr - 0x4000);
    case 0x12:
      return *(rom_bank12 + addr - 0x4000);
    case 0x13:
      return *(rom_bank13 + addr - 0x4000);
    case 0x14:
      return *(rom_bank14 + addr - 0x4000);
    case 0x15:
      return *(rom_bank15 + addr - 0x4000);
    case 0x16:
      return *(rom_bank16 + addr - 0x4000);
    case 0x17:
      return *(rom_bank17 + addr - 0x4000);
    case 0x18:
      return *(rom_bank18 + addr - 0x4000);
    case 0x19:
      return *(rom_bank19 + addr - 0x4000);
    case 0x1A:
      return *(rom_bank1A + addr - 0x4000);
    case 0x1B:
      return *(rom_bank1B + addr - 0x4000);
    case 0x1C:
      return *(rom_bank1C + addr - 0x4000);
    case 0x1D:
      return *(rom_bank1D + addr - 0x4000);
    case 0x1E:
      return *(rom_bank1E + addr - 0x4000);
    case 0x1F:
      return *(rom_bank1F + addr - 0x4000);
    case 0x21:
      return *(rom_bank21 + addr - 0x4000);
      */
    default:
      return 0xFD; //未定義の命令
  }
}

uint8_t switch_rom_bank(uint8_t data) {
  rom_bank_num = data;
  if (data == 0x00 || data == 0x20 || data == 0x40 || data == 0x60) {
    rom_bank_num++;
  }
  Serial.print("bank switch:");
  Serial.println(rom_bank_num, HEX);
}

// ramの指定アドレスから1Byte読み出し
uint8_t mbc_get_ram(uint16_t addr) {
  return *(WRAM + addr - 0xC000);
}

void mbc_put_ram(uint16_t addr, uint8_t data) {
}

// カートリッジタイプの取得
void Get_cartridge_Type() {
  switch (get_byte(0x0147)) { // Cartridge Typeの識別
    case 0x00: // NO MBC
    case 0x08: // ROM + SRAM
    case 0x09: // ROM + SRAM + BATT
      mbcType = MBC_NONE;
      break;
    case 0x01: // MBC1
    case 0x02: // MBC1 + SRAM
    case 0x03: // MBC1 + SRAM + BATT
    case 0xFF: // Hack to accept HuC1 as a MBC1
      mbcType = MBC_1;
      break;
    case 0x05: // MBC2 + SRAM
    case 0x06: // MBC2 SRAM + BATT
      mbcType = MBC_2;
      break;
    case 0x0F: // MBC3 + TIMER + BATT
    case 0x10: // MBC3 + TIMER + BATT + SRAM
    case 0x11: // MBC3
    case 0x12: // MBC3 + SRAM
    case 0x13: // MBC3 + BATT + SRAM
    case 0xFC: // Game Boy Camera
      mbcType = MBC_3;
      break;
    case 0x19: // MBC5
    case 0x1A: // MBC5 + SRAM
    case 0x1B: // MBC5 + BATT + SRAM
    case 0x1C: // RUMBLE
    case 0x1D: // RUMBLE + SRAM
    case 0x1E: // RUMBLE + BATT + SRAM
      mbcType = MBC_5;
      break;
    case 0x0B: // MMMO1
    case 0x0C: // MMM01 + SRAM
    case 0x0D: // MMM01 + SRAM + BATT
    case 0x15: // MBC4
    case 0x16: // MBC4 + SRAM
    case 0x17: // MBC4 + SRAM + BATT
    case 0x22: // MBC7 + BATT + SRAM
    case 0x55: // GG
    case 0x56: // GS3
    case 0xFD: // TAMA 5
    case 0xFE: // HuC3
      mbcType = MBC_NOT_SUPPORTED;
      break;
    default:
      mbcType = MBC_NOT_SUPPORTED;
  }
}
