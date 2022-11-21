static inline void ppu_update(uint32_t _clock) {
  uint8_t *LCDC = IO + 0x40;
  uint8_t *STAT = IO + 0x41;
  uint8_t *LY   = IO + 0x44; //LY
  uint8_t *LYC  = IO + 0x45; //LYC

  // LCD コントロールレジスタのMSBによりLCD有効でない場合は動作をスルー
  if (!(*LCDC & 0x80)) {
    return;
  }

  scanline_counter += _clock;

  // mode2 -> 3 -> 0 -> 2......0 -> 1
  switch (*STAT & 0x03) {
    case 0:
      if (scanline_counter >= 204) { // mode0 -> mode1 or mode2
        scanline_counter -= 204;
        //display_scanline(); // こうすると波打たない
        *LY += 1;
        if (*LY < 144) {
          *STAT = (*STAT & 0xF8) | 0x02;
          if (*STAT & 0x20) int_lcdc = true; // mode2: OAM interrupt happens
        } else {
          *STAT = (*STAT & 0xF8) | 0x01;
          if (*STAT & 0x10) int_lcdc = true; // mode1: V-Blank interrupt happens
          int_vblank = true;
        }
        if (*LY == *LYC) {
          *STAT |= 0x04;
          if (*STAT & 0x40) {
            int_lcdc = true;
          }
        } else {
          *STAT &= ~0x04;
        }
      }
      break;
    case 3:
      if (scanline_counter >= 172) { // mode3 -> mode0
        scanline_counter -= 172;
        *STAT &= 0xF8;
        if (*STAT & 0x08) int_lcdc = true; // mode0: H-Blank interrupt happens
      }
      break;
    case 2:
      if (scanline_counter >= 80) { // mode2 -> mode3
        scanline_counter -= 80;
        *STAT = (*STAT & 0xF8) | 0x03;
        display_scanline();
      }
      break;
    case 1:
      if (scanline_counter >= 456) { // mode1 -> mode1 or mode2
        scanline_counter -= 456;
        *LY += 1;
        if (*LY >= 154) {
          *STAT = (*STAT & 0xF8) | 0x02;
          if (*STAT & 0x20) int_lcdc = true; // mode2: OAM interrupt happens
          *LY = 0;
        }
        if (*LY == *LYC) {
          *STAT |= 0x04;
          if (*STAT & 0x40) {
            int_lcdc = true;
          }
        } else {
          *STAT &= ~0x04;
        }
      }
      break;
  }
}
// bg&wnd フェッチ
void display_scanline() {

  uint8_t *LCDC = IO + 0x40;
  uint16_t LY  = (uint16_t) * (IO + 0x44);
  uint16_t SCY = (uint16_t) * (IO + 0x42); // SCY BGの描画位置
  uint16_t SCX = (uint16_t) * (IO + 0x43); // SCX
  uint16_t WY  = (uint16_t) * (IO + 0x4A); // WY windowの描画位置
  uint16_t WX  = (uint16_t) * (IO + 0x4B) - 7; // WX

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
  bool WIN_enable    = (*LCDC & 0x20) > 0;
  bool sprite_enable = (*LCDC & 0x02) > 0;

  // LY is given from 0xFF44
  for (uint32_t LX = 0; LX < 160; LX++) {
    if (BG_WIN_enable) { // both BG and window enable
      // get back ground pixel color number of given LY and LX, SCY, SCX
      tile_number = get_tile_number(tile_num_y_1, ((LX + SCX) >> 3) & 0x1F);
      bg_pix_C_number = get_pix_C_number(tile_number, LY_plus_SCY, LX + SCX);

      // get window pixel color number of given LY and LX, WY, WX
      win_pix_C_number = 0; // 0 clear. this enable window overray above back ground
      if (WIN_enable) { // window enable
        if (SCY + LY >= WY) {
          if (SCX + LX >= WX) {
            tile_number = get_tile_number(tile_num_y_2 & 0x1F, (LX + SCX - WX) & 0x1F);
            win_pix_C_number = get_pix_C_number(tile_number, tile_num_y_2, LX + SCX - WX);
          }
        }
      }
    }

    // sprite
    uint16_t y_pos;
    uint16_t x_pos;
    uint8_t sp_tile_num;
    uint8_t sp_atr;
    uint8_t num_stripe = 0;
    bool sp_enable = false;

    if (sprite_enable) { //sprite enable
      for (uint32_t i = 0; i < 160; i += 4) { // scaning OAM
        // get object base address
        y_pos = (uint16_t) * (OAM + i + 0);
        x_pos = (uint16_t) * (OAM + i + 1);
        sp_tile_num       = *(OAM + i + 2);
        sp_atr            = *(OAM + i + 3);

        if (y_pos == 0 || y_pos >= 160) continue;
        if (x_pos == 0 || x_pos >= 168) continue;

        if (*LCDC & 0x04) { // sprite size 8x16
          if (y_pos <= (LY + 16) && y_pos > LY + 8) { // upper tile selected
            // X-position
            if (x_pos <= (LX + 8) && x_pos > LX) {
              sp_enable = true;
              if (sp_atr & 0x40) { // Vertically mirrored
                if (sp_atr & 0x20) { // Horizontally mirrored
                  sp_pix_C_number = get_pix_C_num_sp(sp_tile_num & 0xFE, y_pos - LY - 9, x_pos - LX - 1); // V&H-mirrored
                } else {
                  sp_pix_C_number = get_pix_C_num_sp(sp_tile_num & 0xFE, y_pos - LY - 9, LX - x_pos + 8); // V-mirrored
                }
              } else {
                if (sp_atr & 0x20) { // Horizontally mirrored
                  sp_pix_C_number = get_pix_C_num_sp(sp_tile_num & 0xFE, LY - y_pos + 16, x_pos - LX - 1); // H-mirrored
                } else {
                  sp_pix_C_number = get_pix_C_num_sp(sp_tile_num & 0xFE, LY - y_pos + 16, LX - x_pos + 8); // normal
                }
              }
            }
          } else if (y_pos <= (LY + 8) && y_pos > LY) { //lower tile selected
            // X-position
            if (x_pos <= (LX + 8) && x_pos > LX) {
              sp_enable = true;
              if (sp_atr & 0x40) { // Vertically mirrored
                if (sp_atr & 0x20) { // Horizontally mirrored
                  sp_pix_C_number = get_pix_C_num_sp(sp_tile_num | 0x01, y_pos - LY - 1, x_pos - LX + 7); // V&H-mirrored
                } else {
                  sp_pix_C_number = get_pix_C_num_sp(sp_tile_num | 0x01, y_pos - LY - 1, LX - x_pos); // V-mirrored
                }
              } else {
                if (sp_atr & 0x20) { // Horizontally mirrored
                  sp_pix_C_number = get_pix_C_num_sp(sp_tile_num | 0x01, LY - y_pos + 8, x_pos - LX + 7); // H-mirrored
                } else {
                  sp_pix_C_number = get_pix_C_num_sp(sp_tile_num | 0x01, LY - y_pos + 8, LX - x_pos); // normal
                }
              }
            }
          }
        } else { // sprite size 8x8
          // Y-position
          if (y_pos <= (LY + 16) && (LY + 8) < y_pos) {
            // X-position
            if (x_pos <= (LX + 8) && LX < x_pos) {
              sp_enable = true;
              if (sp_atr & 0x40) { // Vertically mirrored
                if (sp_atr & 0x20) { // Horizontally mirrored
                  sp_pix_C_number = get_pix_C_num_sp(sp_tile_num, y_pos - LY - 9, x_pos - LX - 1); // V&H-mirrored
                } else {
                  sp_pix_C_number = get_pix_C_num_sp(sp_tile_num, y_pos - LY - 9, LX - x_pos + 8); // V-mirrored
                }
              } else {
                if (sp_atr & 0x20) { // Horizontally mirrored
                  sp_pix_C_number = get_pix_C_num_sp(sp_tile_num, LY - y_pos + 16, x_pos - LX - 1); // H-mirrored
                } else {
                  sp_pix_C_number = get_pix_C_num_sp(sp_tile_num, LY - y_pos + 16, LX - x_pos + 8); // normal
                }
              }
            }
          }
        } // end of sprite size 8x8
      } // endo of scaning OAM
    } // end of sprite enable

    // mix bg FIFO and sp FIFO
    // mix bg and win
    if (win_pix_C_number) { // window enable
      bg_pix_mixed_c_num = win_pix_C_number;
    } else { // window disable
      bg_pix_mixed_c_num = bg_pix_C_number;
    }

    if (BG_WIN_enable) { // both BG and window enable
      if (sp_enable) { //sprite enable
        if (!sp_pix_C_number) { // sprite pix color is 0
          *pix_mixer = bw_color_number2bit(bg_pix_mixed_c_num); //ok
        } else if (sp_atr & 0x80) { // BG-to-OBJ-Priority is true
          if (bg_pix_mixed_c_num) { // and bg-w pix color is not 0
            *pix_mixer = bw_color_number2bit(bg_pix_mixed_c_num); //ok
          }
        } else {
          *pix_mixer = sp_color_number2bit(sp_pix_C_number, sp_atr); // none of the above conditions apply
        }
      } else { //sprite disable
        *pix_mixer = bw_color_number2bit(bg_pix_mixed_c_num);
      }
    } else { // both BG and window disable
      if (sp_enable) { //sprite enable
        *pix_mixer = sp_color_number2bit(sp_pix_C_number, sp_atr);
      }
    }
    pix_mixer++;
  }
}

static inline uint8_t get_tile_number(uint16_t offset_y, uint16_t offset_x) {
  if (*(IO + 0x40) & 0x08) { // bg offset address select
    return *(VRAM + 0x1c00 + (offset_y << 5) + offset_x);
  } else {
    return *(VRAM + 0x1800 + (offset_y << 5) + offset_x);
  }
}

static inline uint32_t get_pix_C_number(uint8_t tile_number, uint8_t offset_y, uint8_t offset_x) {
  uint32_t h8_tile_l;
  uint32_t h8_tile_h;
  uint32_t t = 7 - offset_x & 0x07;
  if (*(IO + 0x40) & 0x10) { // get pixel color number
    h8_tile_l = *(VRAM + 0x0000 + (tile_number << 4) + ((offset_y & 0x07) << 1));
    h8_tile_h = *(VRAM + 0x0001 + (tile_number << 4) + ((offset_y & 0x07) << 1));
  } else {
    h8_tile_l = *(VRAM + 0x1000 + ((int16_t)tile_number << 4) + ((offset_y & 0x07) << 1));
    h8_tile_h = *(VRAM + 0x1001 + ((int16_t)tile_number << 4) + ((offset_y & 0x07) << 1));
  } // base addr is 8800?
  return ((h8_tile_l & (1 << t)) + (h8_tile_h & (1 << t))) >> t;
}

// return a color number of a pixel in given sprie tile
static inline uint32_t get_pix_C_num_sp(uint8_t tile_number, uint8_t offset_y, uint8_t offset_x) {
  uint32_t h8_tile_l;
  uint32_t h8_tile_h;
  uint32_t t = 7 - offset_x;
  h8_tile_l = *(VRAM + 0x0000 + (tile_number << 4) + (offset_y << 1));
  h8_tile_h = *(VRAM + 0x0001 + (tile_number << 4) + (offset_y << 1));
  return ((h8_tile_l & (1 << t)) + (h8_tile_h & (1 << t))) >> t;
}

static inline uint32_t bw_color_number2bit(uint32_t color_number) {
  switch (color_number) {
    case 0:  // color number 0
      return (uint32_t)*(IO + 0x47) & 0x03;
    case 1:  // color number 1
      return (uint32_t)*(IO + 0x47) & 0x0C;
    case 2:  // color number 2
      return (uint32_t)*(IO + 0x47) & 0x30;
    case 3:  // color number 3
      return (uint32_t)*(IO + 0x47) & 0xC0;
  }
  return 0;
}

static inline uint32_t sp_color_number2bit(uint32_t color_number, uint8_t sp_atr) {
  if (sp_atr & 0x10) {  // OBP1
    switch (color_number) {
      case 0:  // color number 0
        return (uint32_t)*(IO + 0x49) & 0x03;
      case 1:  // color number 1
        return (uint32_t)*(IO + 0x49) & 0x0C;
      case 2:  // color number 2
        return (uint32_t)*(IO + 0x49) & 0x30;
      case 3:  // color number 3
        return (uint32_t)*(IO + 0x49) & 0xC0;
    }
  } else {
    switch (color_number) {  // OBP0
      case 0:                // color number 0
        return (uint32_t)*(IO + 0x48) & 0x03;
      case 1:  // color number 1
        return (uint32_t)*(IO + 0x48) & 0x0C;
      case 2:  // color number 2
        return (uint32_t)*(IO + 0x48) & 0x30;
      case 3:  // color number 3
        return (uint32_t)*(IO + 0x48) & 0xC0;
    }
  }
  return 0;
}
