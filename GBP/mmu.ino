void mmu_update(uint8_t _clock) {
  //self.catridge.update(_clock);
  ppu_update(_clock);
  timer_update(_clock);
  //self.joypad.update(_clock);

  if (int_vblank) {
    IF |= 0x01;
    int_vblank = false;
  }
  if (int_lcdc) {
    IF |= 0x02;
    int_lcdc   = false;
  }
  if (int_timer) {
    IF |= 0x04;
    int_timer  = false;
  }
  //if (int_joypad) {
  //  IF |= 0x10;
  //  int_joypad = false;
  //}
}

static inline uint8_t mmu_read(uint16_t addr) {
  if (addr < 0x0100 && boot) {
    return *(bootstrap + addr);
  } else if (addr < 0x4000) {
    return *(rom_bank00 + addr);
  } else if (addr >= 0x4000 && addr < 0x8000) {
    return mbc_read_rom(addr);
  } else if (addr >= 0x8000 && addr < 0xA000) {
    return *(VRAM + addr - 0x8000);
  } else if (addr >= 0xA000 && addr < 0xC000) {
    return *(CRAM + addr - 0xA000);
  } else if (addr >= 0xC000 && addr < 0xE000) {
    return *(WRAM + addr - 0xC000);
  } else if (addr >= 0xE000 && addr < 0xFE00) {  // Mirror of C000~DDFF
    return *(WRAM + addr - 0xE000); // ?
  } else if (addr >= 0xFE00 && addr < 0xFEA0) {  // Sprite attribute table (OAM)
    return *(OAM + addr - 0xFE00);
    //} else if (addr >= 0xFEA0 && addr < 0xFF00) {  // Not Usable

  } else if (addr >= 0xFF00 && addr < 0xFF80) {  // I/O Register
    if (addr == 0xFF00) {
      return *(IO + addr - 0xFF00) | 0xCF;
    } else  if (addr == 0xFF0F) {
      return IF; // Interrupu flag
    } else if (addr == 0xFF04) {
      return (uint8_t)(timer_div >> 8);
    } else {
      return *(IO + addr - 0xFF00);
    }
  } else if (addr >= 0xFF80 && addr < 0xFFFF) {  // High RAM stack
    return *(HRAM + addr - 0xFF80);
  } else if (addr == 0xFFFF) { // Interrupt Enable register(IE)
    return 0xFF; // read 0xFF
  }
  return 0;
}

static inline void mmu_write(uint16_t addr, uint8_t data) {
  if (addr >= 0x2000 && addr < 0x4000) { // this area is not for write but used for change rom bank
    switch_rom_bank(data & 0x1F);
    //} else if (addr >= 0x4000 && addr < 0x8000) {
  } else if (addr >= 0x8000 && addr < 0xA000) {
    *(VRAM + addr - 0x8000) = data;
  } else if (addr >= 0xA000 && addr < 0xC000) {
    *(CRAM + addr - 0xA000) = data;
  } else if (addr >= 0xC000 && addr < 0xE000) {
    *(WRAM + addr - 0xC000) = data;
  } else if (addr >= 0xE000 && addr < 0xFE00) {  // Mirror of C000~DDFF
    *(WRAM + (addr - 0xE000) ) = data; //?
  } else if (addr >= 0xFE00 && addr < 0xFEA0) {  // Sprite attribute table (OAM)
    *(OAM + addr - 0xFE00)  = data;
    //} else if (addr >= 0xFEA0 && addr < 0xFF00) {  // Not Usable
  } else if (addr >= 0xFF00 && addr < 0xFF80) {  // I/O Register
    if (addr == 0xFF46) {
      dma(data);
    } else if (addr == 0xFF04) {
      timer_div = 0; // Divider regster reset
    } else if (addr == 0xFF07) {
      *(IO + 0x07) = data & 0x07; // Divider regster reset
    } else if (addr == 0xFF0F) {
      IF = data; // Interrupu flag
    } else {
      *(IO + addr - 0xFF00) = data;
    }
  } else if (addr >= 0xFF80 && addr < 0xFFFF) {  // High RAM
    *(HRAM + addr - 0xFF80) = data;
  } else if (addr == 0xFFFF) { // Interrupt Enable register(IE)
    IE = data;
  }
}

static inline uint8_t mbc_read_rom(uint16_t addr) {
  switch (rom_bank_num) {
    case 0x01:
      return *(rom_bank01 + addr - 0x4000);
    /*
      case 0x02:
      return *(rom_bank02 + addr - 0x4000);
      case 0x03:
      return *(rom_bank03 + addr - 0x4000);

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

void dma(uint8_t addr_h) {
  for (uint16_t i = 0x00; i < 0xa0; i++) {
    mmu_write(0xFE00 + i, mmu_read(((uint16_t)addr_h << 8) + i));
  }
}

void switch_rom_bank(uint8_t data) {
  rom_bank_num = data;
  if (data == 0x00 || data == 0x20 || data == 0x40 || data == 0x60) {
    rom_bank_num++;
  }
}
