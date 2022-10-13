Paint_SetRotate(ROTATE_270):

void draw_tile(uint16_t addr, uint8_t x, uint8_t y) {
    uint16_t color;
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 8; j++) {
            color = (get_byte(addr) & 0b10000000) + (get_byte(addr + 1) & 0b10000000);
            color = color << 8
            Paint_SetPixel(0, 0, color);
        }
    }

}

void ppu() {
    uint16_t offset = 0x9800;
    uint8_t r_addr;
    for (uint8_t x = 0; x < 20; x++) {
        for (uint8_t y = 0; y < 18; y++) {
            r_addr = get_byte(x + y * 0x20);
            draw(r_addr, x, y);
        }
    }
    get_byte()
}
