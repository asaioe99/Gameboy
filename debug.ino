void load_rom_header() {
  for (int i = 0; i < 4; i++) {
    rom_header.ety_point[i] = get_byte(0x100 + i);
  }
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 16; i++) {
      rom_header.logo[i + 16 * j] = get_byte(0x104 + i + 16 * j);
    }
  }
  for (int i = 0; i < 16; i++) {
    rom_header.tle[i] = get_byte(0x134 + i);
  }

  rom_header.lsc_code[0] = get_byte(0x144);
  rom_header.lsc_code[1] = get_byte(0x145);
  rom_header.sgb_flag = get_byte(0x146);
  rom_header.ctg_type = get_byte(0x147);
  rom_header.rom_size = get_byte(0x148);
  rom_header.ram_size = get_byte(0x149);
  rom_header.des_code = get_byte(0x14a);
  rom_header.old_code = get_byte(0x14b);
  rom_header.rom_ver = get_byte(0x14c);
  rom_header.hed_chk = get_byte(0x14d);
  rom_header.gbl_chk[0] = get_byte(0x14e);
  rom_header.gbl_chk[1] = get_byte(0x14f);
}

void display_rom_header() {

  char buf[16];

  Serial.println("== DUMP START ==");
  Serial.println("= Entry Point =");
  for (int i = 0; i < 4; i++) {
    sprintf(buf, "%04X : %02X ", 0x100 + i, rom_header.ety_point[i]);
    Serial.print(buf);
    Serial.println("");
  }
  Serial.println("= logo =");
  for (int j = 0; j < 3; j++) {
    sprintf(buf, "%04X : ", 0x104 + j * 16);
    Serial.print(buf);
    for (int i = 0; i < 16; i++) {
      sprintf(buf, "%02X ", rom_header.logo[i + 16 * j]);
      Serial.print(buf);
    }
    Serial.println("");
  }
  Serial.println("= title =");
  sprintf(buf, "%04X : ", 0x134);
  Serial.print(buf);
  for (int i = 0; i < 16; i++) {
    sprintf(buf, "%02X ", rom_header.tle[i]);
    Serial.print(buf);
  }
  Serial.println("");
  Serial.println("= license code =");
  sprintf(buf, "0144 : %02X %02X", rom_header.lsc_code[0], rom_header.lsc_code[1]);
  Serial.println(buf);
  Serial.println("= sgb flag =");
  sprintf(buf, "0146 : %02X", rom_header.sgb_flag);
  Serial.println(buf);
  Serial.println("= cartridge type =");
  sprintf(buf, "0147 : %02X", rom_header.ctg_type);
  Serial.println(buf);
  Serial.println("= rom size =");
  sprintf(buf, "0148 : %02X", rom_header.rom_size);
  Serial.println(buf);
  Serial.println("= ram size =");
  sprintf(buf, "0149 : %02X", rom_header.ram_size);
  Serial.println(buf);
  Serial.println("= destination code =");
  sprintf(buf, "014a : %02X", rom_header.des_code);
  Serial.println(buf);
  Serial.println("= old_llicensee_code =");
  sprintf(buf, "014b : %02X", rom_header.old_code);
  Serial.println(buf);
  Serial.println("= rom_version =");
  sprintf(buf, "014c : %02X", rom_header.rom_ver);
  Serial.println(buf);
  Serial.println("= header checksum =");
  sprintf(buf, "014d : %02X", rom_header.hed_chk);
  Serial.println(buf);
  Serial.println("= license code =");
  sprintf(buf, "014e : %02X %02X", rom_header.gbl_chk[0], rom_header.gbl_chk[1]);
  Serial.println(buf);
  Serial.println("==  END  ==");
}

void dump_rom_bank(uint8_t bank) {

  Serial.print("rom bank : ");
  Serial.println(bank);

  uint16_t offset = 0;
  if (bank > 0) {
    offset = 0x4000;
    switch_rom_bank(bank);  // bank number < 2 ^ (rom_header.rom_size  + 1)
  }

  char buf[16];

  for (uint16_t address = 0; address < 0x4000; address++) {
    if ((address + offset) % 16 == 0) {
      sprintf(buf, "%04X : %02X ", address + offset, get_rom_byte(address + offset));
      Serial.print(buf);
    } else if ((address + offset) % 16 == 15) {
      sprintf(buf, "%02X ", get_rom_byte(address + offset));
      Serial.println(buf);
    } else {
      sprintf(buf, "%02X ", get_rom_byte(address + offset));
      Serial.print(buf);
    }
  }
}

void dump_ram_bank(uint8_t bank) {

  Serial.print("ram bank : ");
  Serial.println(bank);

  switch_ram_bank(bank);

  char buf[16];

  for (uint16_t address = 0xA000; address < 0xC000; address++) {
    if (address % 16 == 0) {
      sprintf(buf, "%04X : %02X ", address, get_ram_byte(address));
      Serial.print(buf);
    } else if (address % 16 == 15) {
      sprintf(buf, "%02X ", get_ram_byte(address));
      Serial.println(buf);
    } else {
      sprintf(buf, "%02X ", get_ram_byte(address));
      Serial.print(buf);
    }
  }
}
