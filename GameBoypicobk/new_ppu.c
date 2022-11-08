void ppu() {
    uint8_t *t_FF40 = io + 0x40;
    uint16_t LY  = (uint16_t) * (io + 0x44);
    uint16_t SCY = (uint16_t) * (io + 0x42); // SCY BGの描画位置
    uint16_t SCX = (uint16_t) * (io + 0x43); // SCX
    uint16_t WY  = (uint16_t) * (io + 0x4A); // WY windowの描画位置
    uint16_t WX  = (uint16_t) * (io + 0x4B) - 7; // WX

    uint16_t *pix_mixture = FIFO_bg_wnd + LY * 160;

// LY is given from 0xFF44
    for (uint16_t LX = 0; LX < 160; LX++) {
        if (*t_FF40 & 0b00000001) { // both BG and window enable
            // get back ground pixel color number of given LY and LX, SCY, SCX
            uint16_t tile_number = get_tile_number(((LY >> 3) + (SCY >> 3)) & 0x1F, ((LX >> 3) + (SXY >> 3)) & 0x1F);
            uint8_t bg_pix_C_number = get_pix_C_number(tile_number, ((LY >> 3) + (SCY >> 3)) & 0x1F, ((LX >> 3) + (SXY >> 3)) & 0x1F);

            // get window pixel color number of given LY and LX, WY, WX
            if (*t_FF40 & 0b00100000) { // window enable
                if (SCY + LY >= WY) {
                    if (SCX + LX >= WX) {
                        tile_number = get_tile_number(SCY + LY - WY, SCX + LX - WX);
                        uint8_t win_pix_C_number = get_pix_C_number(tile_number, SCY + LY - WY, SCX + LX - WX);
                    }
                }
            }
        }
        // overray window above back ground
        if (win_pix_C_number) {
            *pix_mixture = bw_color_number2bit(win_pix_C_number);
        } else {
            *pix_mixture = bw_color_number2bit(bg_pix_C_number);
        }
        pix_mixture++;
    }
}

uint16_t get_tile_number(uint8_t offset_y, uint8_t offset_x) {
    if (*(io + 0x40) & 0b00001000) { // bg offset address select 
        return get_byte(0x9c00 + (offset_y << 5) + offset_x);
    } else {
        return get_byte(0x9800 + (offset_y << 5) + offset_x);
    }
}

uint8_t get_pix_C_number(uint16_t tile_number, uint8_t offset_y, uint8_t offset_x) {
    if (*(io + 0x40) & 0b00010000) { // get pixel color number 
        return get_byte(0x8000 + (tile_number << 4) + ((offset_y & 0x07) << 1)) & (0x80 >> (offset_x & 0x07)) + get_byte(0x8001 + (tile_number << 4) + ((offset_y & 0x07) << 1)) & (0x80 >> (offset_x & 0x07));
    } else {
        return get_byte(0x9000 + (tile_number << 4) + ((offset_y & 0x07) << 1)) & (0x80 >> (offset_x & 0x07)) + get_byte(0x9001 + (tile_number << 4) + ((offset_y & 0x07) << 1)) & (0x80 >> (offset_x & 0x07));
    } // base addr is 8800?
}

uint16_t bw_color_number2bit(uint8_t color_number) {
    switch((*(io + 0x47) & (0x00000011 << (color_number << 1))) >> (color_number << 1)) {
        case 0: // White
            return 0b0000000000000000;
        case 1: // Light gray
            return 0b0101001010001010;
        case 2: // Dark gray
            return 0b1010010100010100;
        case 3: // Black
            return 0b1111011110011110;
    }
}
