static inline uint8_t mmu_read_pc(uint16_t addr) {
  if (boot) {
    return *(bootstrap + addr);
  } else if (addr < 0x4000) {
    return *(rom_bank00 + addr);
  } else if (addr >= 0x4000 && addr < 0x8000) {
    return mbc_read_rom(addr);
  } else if (addr >= 0xC000 && addr < 0xE000) {
    return *(WRAM + addr - 0xC000);
  } else if (addr >= 0xFF80 && addr < 0xFFFF) {  // High RAM stack 何と使用されている！
   return *(HRAM + addr - 0xFF80);
  }
  return 0;
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
    return *(WRAM + addr - 0xE000);              // ?
  } else if (addr >= 0xFE00 && addr < 0xFEA0) {  // Sprite attribute table (OAM)
    return *(OAM + addr - 0xFE00);
  } else if (addr >= 0xFF00 && addr < 0xFF80) {  // I/O Register
    if (addr == 0xFF00) {
      return joypad_read();
    } else if (addr == 0xFF0F) {
      return IF;  // Interrupu flag
    } else if (addr == 0xFF04) {
      return (uint8_t)(timer_div >> 8);
    } else {
      return *(IO + addr - 0xFF00);
    }
  } else if (addr >= 0xFF80 && addr < 0xFFFF) {  // High RAM stack
    return *(HRAM + addr - 0xFF80);
  } else if (addr == 0xFFFF) {  // Interrupt Enable register(IE)
    return IE;
  }
  return 0;
}

static inline void mmu_write(uint16_t addr, uint8_t data) {
  if (addr >= 0x2000 && addr < 0x4000) {  // this area is not for write but used for change rom bank
    switch_rom_bank(data & 0x1F);
    //} else if (addr >= 0x4000 && addr < 0x8000) {
  } else if (addr >= 0x8000 && addr < 0xA000) {
    *(VRAM + addr - 0x8000) = data;
  } else if (addr >= 0xA000 && addr < 0xC000) {
    *(CRAM + addr - 0xA000) = data;
  } else if (addr >= 0xC000 && addr < 0xE000) {
    *(WRAM + addr - 0xC000) = data;
  } else if (addr >= 0xE000 && addr < 0xFE00) {  // Mirror of C000~DDFF
    *(WRAM + (addr - 0xE000)) = data;            //?
  } else if (addr >= 0xFE00 && addr < 0xFEA0) {  // Sprite attribute table (OAM)
    *(OAM + addr - 0xFE00) = data;
  } else if (addr >= 0xFF00 && addr < 0xFF80) {  // I/O Register
    if (addr == 0xFF46) {
      dma(data);
    } else if (addr == 0xFF04) {
      timer_div = 0;  // Divider regster reset
    } else if (addr == 0xFF00) {
      *IO = (*IO & 0xCF) | (data & 0x30);  // joypad
    } else if (addr == 0xFF07) {
      *(IO + 0x07) = data & 0x07;  // Divider regster reset
    } else if (addr == 0xFF0F) {
      IF = data;  // Interrupu flag
    } else {
      *(IO + addr - 0xFF00) = data;
    }
  } else if (addr >= 0xFF80 && addr < 0xFFFF) {  // High RAM
    *(HRAM + addr - 0xFF80) = data;
  } else if (addr == 0xFFFF) {  // Interrupt Enable register(IE)
    IE = data;
  }
}

static inline uint8_t mmu_read_sp(uint16_t addr) {
  if (addr >= 0xFF80 && addr < 0xFFFF) {  // High RAM stack
    return *(HRAM + addr - 0xFF80);
  } else if (addr >= 0xC000 && addr < 0xE000) {
    return *(WRAM + addr - 0xC000);
  } else if (addr >= 0xE000 && addr < 0xFE00) {  // Mirror of C000~DDFF
    return *(WRAM + addr - 0xE000);              // ?
  } else if (addr == 0xFFFF) {                   // Interrupt Enable register(IE)
    return IE;
  } else if (addr >= 0xA000 && addr < 0xC000) {
    return *(CRAM + addr - 0xA000);
  }
  return 0;
}

static inline void mmu_write_sp(uint16_t addr, uint8_t data) {
  if (addr >= 0xFF80 && addr < 0xFFFF) {  // High RAM
    *(HRAM + addr - 0xFF80) = data;
  } else if (addr >= 0xC000 && addr < 0xE000) {
    *(WRAM + addr - 0xC000) = data;
  } else if (addr >= 0xE000 && addr < 0xFE00) {  // Mirror of C000~DDFF
    *(WRAM + (addr - 0xE000)) = data;            //?
  } else if (addr >= 0xFF80 && addr < 0xFFFF) {  // High RAM
    *(HRAM + addr - 0xFF80) = data;
  } else if (addr == 0xFFFF) {  // Interrupt Enable register(IE)
    IE = data;
  } else if (addr >= 0xA000 && addr < 0xC000) {
    *(CRAM + addr - 0xA000) = data;
  }
}

static inline uint8_t mmu_read_io(uint16_t addr) {
  if (addr < 0xFF80) {  // I/O Register
    if (addr == 0xFF00) {
      return joypad_read();
    } else if (addr == 0xFF0F) {
      return IF;  // Interrupu flag
    } else if (addr == 0xFF04) {
      return (uint8_t)(timer_div >> 8);
    } else {
      return *(IO + addr - 0xFF00);
    }
  } else if (addr >= 0xFF80 && addr < 0xFFFF) {  // High RAM stack
    return *(HRAM + addr - 0xFF80);
  } else {        // Interrupt Enable register(IE)
    return IE;
  }
  return 0;
}

static inline void mmu_write_io(uint16_t addr, uint8_t data) {
  if (addr < 0xFF80) {  // I/O Register
    if (addr == 0xFF46) {
      dma(data);
    } else if (addr == 0xFF00) {
      *IO = (*IO & 0xCF) | (data & 0x30);  // joypad
    } else if (addr == 0xFF04) {
      timer_div = 0;  // Divider regster reset
    } else if (addr == 0xFF07) {
      *(IO + 0x07) = data & 0x07;  // Divider regster reset
    } else if (addr == 0xFF0F) {
      IF = data;  // Interrupu flag
    } else {
      *(IO + addr - 0xFF00) = data;
    }
  } else if (addr >= 0xFF80 && addr < 0xFFFF) {  // High RAM
    *(HRAM + addr - 0xFF80) = data;
  } else {  // Interrupt Enable register(IE)
    IE = data;
  }
}

static inline uint8_t* get_dma_source_p(uint16_t addr) {
  if (addr >= 0x4000 && addr < 0x8000) {
    return mbc_source_p() + addr - 0x8000;
  } else if (addr < 0x4000) {
    return (uint8_t *)rom_bank00 + addr;
  } else if (addr >= 0xA000 && addr < 0xC000) {
    return CRAM + addr - 0xA000;
  } else if (addr >= 0xC000 && addr < 0xE000) {
    return WRAM + addr - 0xC000;
  } else if (addr >= 0xE000 && addr < 0xFE00) {  // Mirror of C000~DDFF
    return WRAM + addr - 0xE000;              // ?
  }
  return 0;
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
      return 0xFD;  //未定義の命令
  }
}

static inline uint8_t* mbc_source_p() {
  switch (rom_bank_num) {
    case 0x01:
      return (uint8_t *)rom_bank01;
                /*
      case 0x02:
      return (uint8_t *)rom_bank02;
      case 0x03:
      return (uint8_t *)rom_bank03;
      case 0x04:
      return (uint8_t *)rom_bank04;
      case 0x05:
      return (uint8_t *)rom_bank05;
      case 0x06:
      return (uint8_t *)rom_bank06;
      case 0x07:
      return (uint8_t *)rom_bank07;
      case 0x08:
      return (uint8_t *)rom_bank08;
      case 0x09:
      return (uint8_t *)rom_bank09;
      case 0x0A:
      return (uint8_t *)rom_bank0A;
      case 0x0B:
      return (uint8_t *)rom_bank0B;
      case 0x0C:
      return (uint8_t *)rom_bank0C;
      case 0x0D:
      return (uint8_t *)rom_bank0D;
      case 0x0E:
      return (uint8_t *)rom_bank0E;
      case 0x0F:
      return (uint8_t *)rom_bank0F;

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
      return 0x00;
  }
}

static inline void dma(uint8_t addr_h) {
  uint16_t addr = (uint16_t)addr_h << 8;
  uint8_t *source = get_dma_source_p(addr);
  memcpy(OAM, source, 0x9F);
}

static inline void switch_rom_bank(uint8_t data) {
  rom_bank_num = data;
  if (data == 0x00 || data == 0x20 || data == 0x40 || data == 0x60) {
    rom_bank_num++;
  }
}
