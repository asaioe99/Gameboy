static inline void ppu_update(uint32_t _clock) {
  uint8_t *LCDC = IO + 0x40;
  uint8_t *STAT = IO + 0x41;
  uint8_t *LY   = IO + 0x44;   //LY
  uint8_t *LYC  = IO + 0x45;  //LYC
  uint32_t num_sp = 0;

  // LCD コントロールレジスタのMSBによりLCD有効でない場合は動作をスルー
  if (!(*LCDC & 0x80)) return;

  scanline_counter += _clock;

  // mode2 -> 3 -> 0 -> 2......0 -> 1
  switch (*STAT & 0x03) {
    case 0:
      if (scanline_counter >= 204) {  // mode0 -> mode1 or mode2
        scanline_counter -= 204;
        *LY += 1;
        if (*LY < 144) {
          *STAT = (*STAT & 0xF8) | 0x02;
          if (*STAT & 0x20) IF |= 0x02;  // mode2: OAM interrupt happens
        } else {
          *STAT = (*STAT & 0xF8) | 0x01;
          if (*STAT & 0x10) IF |= 0x02;  // mode1: V-Blank interrupt happens
          IF |= 0x01;
        }
        if (*LY == *LYC) {
          *STAT |= 0x04;
          if (*STAT & 0x40) {
            IF |= 0x02;
          }
        } else {
          *STAT &= ~0x04;
        }
      }
      break;
    case 3:
      if (scanline_counter >= 172) {  // mode3 -> mode0
        scanline_counter -= 172;
        *STAT &= 0xF8;
        if (*STAT & 0x08) IF |= 0x02;  // mode0: H-Blank interrupt happens
      }
      break;
    case 2:
      if (scanline_counter >= 80) {  // mode2 -> mode3
        scanline_counter -= 80;
        *STAT = (*STAT & 0x07) | 0x03;
        if (*LCDC & 0x02) {
          num_sp = scan_oam(*LY);          
        }
        display_scanline(num_sp);
      }
      break;
    case 1:
      if (scanline_counter >= 456) {  // mode1 -> mode1 or mode2
        scanline_counter -= 456;
        *LY += 1;
        if (*LY >= 154) {
          *STAT = (*STAT & 0xF8) | 0x02;
          if (*STAT & 0x20) IF |= 0x02;  // mode2: OAM interrupt happens
          *LY = 0;
        }
        if (*LY == *LYC) {
          *STAT |= 0x04;
          if (*STAT & 0x40) {
            IF |= 0x02;
          }
        } else {
          *STAT &= ~0x04;
        }
      }
      break;
  }
}

// bg&wnd フェッチ
void display_scanline(uint32_t num_sp) {

  uint8_t *LCDC = IO + 0x40;
  uint16_t SCY  = (uint16_t) * (IO + 0x42);     // SCY BGの描画位置
  uint16_t SCX  = (uint16_t) * (IO + 0x43);     // SCX
  uint16_t LY   = (uint16_t) * (IO + 0x44);
  uint16_t WY   = (uint16_t) * (IO + 0x4A);     // WY windowの描画位置
  uint16_t WX   = (uint16_t) * (IO + 0x4B) - 7; // WX

  uint16_t LY_plus_SCY = LY + SCY;
  uint16_t tile_num_y_1 = (LY_plus_SCY >> 3) & 0x1F;
  uint16_t tile_num_y_2 = LY_plus_SCY - WY;

  uint32_t *pix_mixer = FIFO_bg_wnd + LY * 160;
  uint32_t bg_pix_mixed_c_num;
  uint8_t tile_number;
  uint32_t win_pix_C_number;
  uint32_t bg_pix_C_number;
  uint32_t sp_pix_C_number;

  bool BG_WIN_enable = (*LCDC & 0x01) > 0;
  bool sprite_enable = (*LCDC & 0x02) > 0;
  bool sprite_size_16 = (*LCDC & 0x04) > 0;
  bool WIN_enable = (*LCDC & 0x20) > 0;
  bool WIN_enable_2 = WIN_enable && (LY_plus_SCY >= WY);

  // LY is given from 0xFF44
  for (uint32_t LX = 0; LX < 160; LX++) {
    if (BG_WIN_enable) {  // both BG and window enable
      uint16_t LX_plus_SCX = LX + SCX;
      // get back ground pixel color number of given LY and LX, SCY, SCX
      tile_number = get_tile_number(tile_num_y_1, (LX_plus_SCX >> 3) & 0x1F);
      bg_pix_C_number = get_pix_C_number(tile_number, LY_plus_SCY, LX_plus_SCX);

      // get window pixel color number of given LY and LX, WY, WX
      win_pix_C_number = 0;  // 0 clear. this enable window overray above back ground
      if (WIN_enable_2) {      // window enable
        if (LX_plus_SCX >= WX) {
          tile_number = get_tile_number(tile_num_y_2 & 0x1F, (LX_plus_SCX - WX) & 0x1F);
          win_pix_C_number = get_pix_C_number(tile_number, tile_num_y_2, LX_plus_SCX - WX);
        }
      }
    }

    // sprite
    bool obp1 = false;
    bool bop = false;

    if (sprite_enable && num_sp) {  //sprite enable
      sp_pix_C_number = sprite_size_16 ? scan_oam_16x8(LY, LX, num_sp) : scan_oam_8x8(LY, LX, num_sp);
      obp1 = (sp_pix_C_number & 0x10) > 0;
      bop = (sp_pix_C_number & 0x80) > 0;
    }

    // mix bg FIFO and sp FIFO
    // mix bg and win
    bg_pix_mixed_c_num = win_pix_C_number > 0 ? win_pix_C_number : bg_pix_C_number;

    if (!(sp_pix_C_number & 0x03) || (bop && bg_pix_mixed_c_num)) {
      *pix_mixer = bw_color_number2bit(bg_pix_mixed_c_num);
    } else {
      *pix_mixer = sp_color_number2bit(sp_pix_C_number & 0x03, obp1);
    }
    pix_mixer++;
  }
}

static inline uint32_t scan_oam_8x8(uint16_t LY, uint16_t LX, uint32_t num_sp) {
  uint16_t y_pos;
  uint16_t x_pos;
  uint8_t sp_tile_num;
  uint8_t sp_atr = 0;
  uint32_t sp_pix_C_number = 4;
  int index = 0;

  // scaning OAM
  for (uint32_t i = 0; i < num_sp; i++) {
    int index = *(oam_table + i);

    // get object base address
    y_pos = (uint16_t) * (OAM + index + 0);
    x_pos = (uint16_t) * (OAM + index + 1);
    sp_tile_num = *(OAM + index + 2);
    sp_atr = *(OAM + index + 3); 
    
    if (y_pos < LY + 8) continue;
    if (x_pos < LX) continue;
    if (x_pos > LX + 8) continue;
    
    switch (sp_atr & 0x60) {
      case 0x00:
        sp_pix_C_number = get_pix_C_num_sp(sp_tile_num, LY - y_pos + 16, LX - x_pos + 8);  // normal
        break;
      case 0x20:
        sp_pix_C_number = get_pix_C_num_sp(sp_tile_num, LY - y_pos + 16, x_pos - LX - 1);  // H-mirrored
        break;
      case 0x40:
        sp_pix_C_number = get_pix_C_num_sp(sp_tile_num, y_pos - LY - 9, LX - x_pos + 8);  // V-mirrored
        break;
      case 0x60:
        sp_pix_C_number = get_pix_C_num_sp(sp_tile_num, y_pos - LY - 9, x_pos - LX - 1);  // V&H-mirrored
        break;
    }
  }
  return sp_pix_C_number | (sp_atr & 0x90);
}

static inline uint32_t scan_oam_16x8(uint16_t LY, uint16_t LX, uint32_t num_sp) {
  uint16_t y_pos;
  uint16_t x_pos;
  uint8_t sp_tile_num;
  uint8_t sp_atr = 0;
  uint32_t sp_pix_C_number = 4;
  int index = 0;

  // scaning OAM
  for (uint32_t i = 0; i < num_sp; i++) {
    int index = *(oam_table + i);

    // get object base address
    y_pos = (uint16_t) * (OAM + index + 0);
    x_pos = (uint16_t) * (OAM + index + 1);
    sp_tile_num = *(OAM + index + 2);
    sp_atr = *(OAM + index + 3);
    
    if (x_pos < LX) continue;
    if (x_pos > LX + 8) continue;

    if (y_pos > LY + 8) {  // upper tile selected
      switch (sp_atr & 0x60) {
        case 0x00:
          sp_pix_C_number = get_pix_C_num_sp(sp_tile_num & 0xFE, LY - y_pos + 16, LX - x_pos + 8);  // normal
          break;
        case 0x20:
          sp_pix_C_number = get_pix_C_num_sp(sp_tile_num & 0xFE, LY - y_pos + 16, x_pos - LX - 1);  // H-mirrored
          break;
        case 0x40:
          sp_pix_C_number = get_pix_C_num_sp(sp_tile_num & 0xFE, y_pos - LY - 9, LX - x_pos + 8);  // V-mirrored
          break;
        case 0x60:
          sp_pix_C_number = get_pix_C_num_sp(sp_tile_num & 0xFE, y_pos - LY - 9, x_pos - LX - 1);  // V&H-mirrored
          break;
      }
    } else {  //lower tile selected
      switch (sp_atr & 0x60) {
        case 0x00:
          sp_pix_C_number = get_pix_C_num_sp(sp_tile_num | 0x01, LY - y_pos + 8, LX - x_pos);  // normal
          break;
        case 0x20:
          sp_pix_C_number = get_pix_C_num_sp(sp_tile_num | 0x01, LY - y_pos + 8, x_pos - LX + 7);  // H-mirrored
          break;
        case 0x40:
          sp_pix_C_number = get_pix_C_num_sp(sp_tile_num | 0x01, y_pos - LY - 1, LX - x_pos);  // V-mirrored
          break;
        case 0x60:
          sp_pix_C_number = get_pix_C_num_sp(sp_tile_num | 0x01, y_pos - LY - 1, x_pos - LX + 7);  // V&H-mirrored
          break;
      }
    }
  }
  return sp_pix_C_number | (sp_atr & 0x90);
}

static inline uint8_t get_tile_number(uint16_t offset_y, uint16_t offset_x) {
  if (*(IO + 0x40) & 0x08) {  // bg offset address select
    return *(VRAM + 0x1c00 + (offset_y << 5) + offset_x);
  } else {
    return *(VRAM + 0x1800 + (offset_y << 5) + offset_x);
  }
}

static inline uint32_t get_pix_C_number(uint8_t tile_number, uint8_t offset_y, uint8_t offset_x) {
  uint32_t h8_tile_l;
  uint32_t h8_tile_h;
  uint32_t t = 7 - offset_x & 0x07;
  uint32_t addr = ((offset_y & 0x07) << 1);
  if (*(IO + 0x40) & 0x10) {  // get pixel color number
    h8_tile_l = *(VRAM + 0x0000 + (tile_number << 4) + addr);
    h8_tile_h = *(VRAM + 0x0001 + (tile_number << 4) + addr);
  } else {
    h8_tile_l = *(VRAM + 0x1000 + ((int16_t)tile_number << 4) + addr);
    h8_tile_h = *(VRAM + 0x1001 + ((int16_t)tile_number << 4) + addr);
  }  // base addr is 8800?
  return ((h8_tile_l & (1 << t)) + (h8_tile_h & (1 << t))) >> t;
}

// return a color number of a pixel in given sprie tile
static inline uint32_t get_pix_C_num_sp(uint8_t tile_number, uint8_t offset_y, uint8_t offset_x) {
  uint32_t h8_tile_l;
  uint32_t h8_tile_h;
  uint32_t t = 7 - offset_x;
  uint32_t addr = (tile_number << 4) + (offset_y << 1);
  h8_tile_l = *(VRAM + addr);
  h8_tile_h = *(VRAM + addr + 1);
  return ((h8_tile_l & (1 << t)) + (h8_tile_h & (1 << t))) >> t;
}

static inline uint32_t bw_color_number2bit(uint32_t color_number) {
  switch (color_number) {
    case 0:  // color number 0
      return (uint32_t) * (IO + 0x47) & 0x03;
    case 1:  // color number 1
      return ((uint32_t) * (IO + 0x47) & 0x0C) >> 2;
    case 2:  // color number 2
      return ((uint32_t) * (IO + 0x47) & 0x30) >> 4;
    case 3:  // color number 3
      return ((uint32_t) * (IO + 0x47) & 0xC0) >> 6;
  }
  return 0;
}

static inline uint32_t sp_color_number2bit(uint32_t color_number, bool obp1) {
  if (obp1) {  // OBP1
    switch (color_number) {
      case 0:  // color number 0
        return (uint32_t) * (IO + 0x49) & 0x03;
      case 1:  // color number 1
        return ((uint32_t) * (IO + 0x49) & 0x0C) >> 2;
      case 2:  // color number 2
        return ((uint32_t) * (IO + 0x49) & 0x30) >> 4;
      case 3:  // color number 3
        return ((uint32_t) * (IO + 0x49) & 0xC0) >> 6;
    }
  } else {
    switch (color_number) {  // OBP0
      case 0:                // color number 0
        return (uint32_t) * (IO + 0x48) & 0x03;
      case 1:  // color number 1
        return ((uint32_t) * (IO + 0x48) & 0x0C) >> 2;
      case 2:  // color number 2
        return ((uint32_t) * (IO + 0x48) & 0x30) >> 4;
      case 3:  // color number 3
        return ((uint32_t) * (IO + 0x48) & 0xC0) >> 6;
    }
  }
  return 0;
}

static inline uint32_t scan_oam(uint8_t LY) {
  uint32_t n = 0;
  for (uint32_t i = 0; i < 40; i++) {
    uint8_t y = *(OAM + i * 4);
    if (!y) continue;
    uint8_t x = *(OAM + i * 4 + 1);
    if (!x) continue;
    if (y >= 160) continue;
    if (y < LY) continue;
    if (y > LY + 16) continue;
    if (x < 168) {
      *(oam_table + n) = i << 2;
      n++;
    }
  }
  return n;
}
