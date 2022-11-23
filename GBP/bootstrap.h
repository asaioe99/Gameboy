// bootstrap（実物のため、そのままは掲載不可）

const uint8_t bootstrap[] = {
  // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
  0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E, // 0
  0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0, // 1
  0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B, // 2
  0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9, // 3
  0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20, // 4
  0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04, // 5
  0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2, // 6
  0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06, // 7
  0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xE2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20, // 8
  0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17, // 9
  0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, // a
  0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, // b
  0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, // c
  0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C, // d
  0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20, // e
  0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50, // f
};

void (* const op_ptr_array[])(void) = {
    nop,        //0x00
    ld_bc_d16,  //0x01
    ld_pbc_ar,  //0x02
    inc_bc,     //0x03
    inc_br,     //0x04
    dec_br,     //0x05
    ld_br_d8,   //0x06
    rlca,       //0x07
    ld_pd16_sp, //0x08
    add_hl_bc,  //0x09
    ld_ar_pr16, //0x0A
    dec_bc,     //0x0B
    inc_cr,     //0x0C
    dec_cr,     //0x0D
    ld_cr_d8,   //0x0E
    rrca,       //0x0F

    stop_0,     //0x10
    ld_de_d16,  //0x11
    ld_pde_ar,  //0x12
    inc_de,     //0x13
    inc_dr,     //0x14
    dec_dr,     //0x15
    ld_dr_d8,   //0x16
    rla,        //0x17
    jr_d8,      //0x18
    add_hl_de,  //0x19
    ld_ar_pr16, //0x1A
    dec_de,     //0x1B
    inc_er,     //0x1C
    dec_er,     //0x1D
    ld_er_d8,   //0x1E
    rra,        //0x1F

    jr_nz_d8,   //0x20
    ld_hl_d16,  //0x21
    ld_phli_ar, //0x22
    inc_hl,     //0x23
    inc_hr,     //0x24
    dec_hr,     //0x25
    ld_hr_d8,   //0x26
    daa,        //0x27
    jr_z_d8,    //0x28
    add_hl_hl,  //0x29
    ld_ar_phli, //0x2A
    dec_hl,     //0x2B
    inc_lr,     //0x2C
    dec_lr,     //0x2D
    ld_lr_d8,   //0x2E
    cpl,        //0x2F

    jr_nc_d8,   //0x30
    ld_sp_d16,  //0x31
    ld_phld_ar, //0x32
    inc_sp,     //0x33
    inc_phl,    //0x34
    dec_phl,    //0x35
    ld_phl_d8,  //0x36
    scf,        //0x37
    jr_c_d8,    //0x38
    add_hl_sp,  //0x39
    ld_ar_phld, //0x3A
    dec_sp,     //0x3B
    inc_ar,     //0x3C
    dec_ar,     //0x3D
    ld_ar_d8,   //0x3E
    ccf,        //0x3F

    ld_br_br,   //0x40
    ld_br_cr,   //0x41
    ld_br_dr,   //0x42
    ld_br_er,   //0x43
    ld_br_hr,   //0x44
    ld_br_lr,   //0x45
    ld_br_phl,  //0x46
    ld_br_ar,   //0x47
    ld_cr_br,   //0x48
    ld_cr_cr,   //0x49
    ld_cr_dr,   //0x4A
    ld_cr_er,   //0x4B
    ld_cr_hr,   //0x4C
    ld_cr_lr,   //0x4D
    ld_cr_phl,  //0x4E
    ld_cr_ar,   //0x4F

    ld_dr_br,   //0x50
    ld_dr_cr,   //0x51
    ld_dr_dr,   //0x52
    ld_dr_er,   //0x53
    ld_dr_hr,   //0x54
    ld_dr_lr,   //0x55
    ld_dr_phl,  //0x56
    ld_dr_ar,   //0x57
    ld_er_br,   //0x58
    ld_er_cr,   //0x59
    ld_er_dr,   //0x5A
    ld_er_er,   //0x5B
    ld_er_hr,   //0x5C
    ld_er_lr,   //0x5D
    ld_er_phl,  //0x5E
    ld_er_ar,   //0x5F

    ld_hr_br,   //0x60
    ld_hr_cr,   //0x61
    ld_hr_dr,   //0x62
    ld_hr_er,   //0x63
    ld_hr_hr,   //0x64
    ld_hr_lr,   //0x65
    ld_hr_phl,  //0x66
    ld_hr_ar,   //0x67
    ld_lr_br,   //0x68
    ld_lr_cr,   //0x69
    ld_lr_dr,   //0x6A
    ld_lr_er,   //0x6B
    ld_lr_hr,   //0x6C
    ld_lr_lr,   //0x6D
    ld_lr_phl,  //0x6E
    ld_lr_ar,   //0x6F

    ld_phl_br,  //0x70
    ld_phl_cr,  //0x71
    ld_phl_dr,  //0x72
    ld_phl_er,  //0x73
    ld_phl_hr,  //0x74
    ld_phl_lr,  //0x75
    halt,       //0x76
    ld_phl_ar,  //0x77
    ld_ar_br,   //0x78
    ld_ar_cr,   //0x79
    ld_ar_dr,   //0x7A
    ld_ar_er,   //0x7B
    ld_ar_hr,   //0x7C
    ld_ar_lr,   //0x7D
    ld_ar_phl,  //0x7E
    ld_ar_ar,   //0x7F

    add_ar_br,  //0x80
    add_ar_cr,  //0x81
    add_ar_dr,  //0x82
    add_ar_er,  //0x83
    add_ar_hr,  //0x84
    add_ar_lr,  //0x85
    add_ar_phl, //0x86
    add_ar_ar,  //0x87
    adc_ar_br,  //0x88
    adc_ar_cr,  //0x89
    adc_ar_dr,  //0x8A
    adc_ar_er,  //0x8B
    adc_ar_hr,  //0x8C
    adc_ar_lr,  //0x8D
    adc_ar_phl, //0x8E
    adc_ar_ar,  //0x8F

    sub_ar_br,  //0x90
    sub_ar_cr,  //0x91
    sub_ar_dr,  //0x92
    sub_ar_er,  //0x93
    sub_ar_hr,  //0x94
    sub_ar_lr,  //0x95
    sub_ar_phl, //0x96
    sub_ar_ar,  //0x97
    sbc_ar_br,  //0x98
    sbc_ar_cr,  //0x99
    sbc_ar_dr,  //0x9A
    sbc_ar_er,  //0x9B
    sbc_ar_hr,  //0x9C
    sbc_ar_lr,  //0x9D
    sbc_ar_phl, //0x9E
    sbc_ar_ar,  //0x9F

    and_br,     //0xA0
    and_cr,     //0xA1
    and_dr,     //0xA2
    and_er,     //0xA3
    and_hr,     //0xA4
    and_lr,     //0xA5
    and_ar_phl, //0xA6
    and_ar,     //0xA7
    xor_ar_br,  //0xA8
    xor_ar_cr,  //0xA9
    xor_ar_dr,  //0xAA
    xor_ar_er,  //0xAB
    xor_ar_hr,  //0xAC
    xor_ar_lr,  //0xAD
    xor_ar_phl, //0xAE
    xor_ar_ar,  //0xAF

    or_br,      //0xB0
    or_cr,      //0xB1
    or_dr,      //0xB2
    or_er,      //0xB3
    or_hr,      //0xB4
    or_lr,      //0xB5
    or_ar_phl,  //0xB6
    or_ar,      //0xB7
    cp_br,      //0xB8
    cp_cr,      //0xB9
    cp_dr,      //0xBA
    cp_er,      //0xBB
    cp_hr,      //0xBC
    cp_lr,      //0xBD
    cp_phl,     //0xBE
    cp_ar,      //0xBF

    ret_nz,     //0xC0
    pop_bc,     //0xC1
    jp_nz_d16,  //0xC2
    jp_d16,     //0xC3
    call_nz_d16,//0xC4
    push_bc,    //0xC5
    add_ar_d8,  //0xC6
    rst_vec_00, //0xC7
    ret_z,      //0xC8
    ret,        //0xC9
    jp_z_d16,   //0xCA
    nop,        //0xCB
    call_z_d16, //0xCC
    call_d16,   //0xCD
    adc_ar_d8,  //0xCE
    rst_vec_08,  //0xCF

    ret_nc,     //0xD0
    pop_de,     //0xD1
    jp_nc_d16,  //0xD2
    nop,        //0xD3
    call_nc_d16,//0xD4
    push_de,    //0xD5
    sub_ar_d8,  //0xD6
    rst_vec_10, //0xD7
    ret_c,      //0xD8
    reti,       //0xD9
    jp_c_d16,   //0xDA
    nop,        //0xDB
    call_c_d16,//0xDC
    nop,        //0xDD
    sbc_ar_d8,  //0xDE
    rst_vec_18, //0xDF

    ldh_pd8_ar, //0xE0
    pop_hl,     //0xE1
    ld_pcr_ar,  //0xE2
    nop,        //0xE3
    nop,        //0xE4
    push_hl,    //0xE5
    and_d8,     //0xE6
    rst_vec_20, //0xE7
    add_sp_d8,  //0xE8
    jp_hl,      //0xE9
    ld_pa16_ar, //0xEA
    nop,        //0xEB
    nop,        //0xEC
    nop,        //0xED
    xor_d8,     //0xEE
    rst_vec_28, //0xEF

    ldh_ar_pd8, //0xF0
    pop_af,     //0xF1
    ld_ar_pcr,  //0xF2
    di,         //0xF3
    nop,        //0xF4
    push_af,    //0xF5
    or_d8,      //0xF6
    rst_vec_30, //0xF7
    ld_hl_sp_d8,//0xF8
    ld_sp_hl,   //0xF9
    ld_ar_pa16, //0xFA
    ei,         //0xFB
    nop,        //0xFC
    nop,        //0xFD
    cp_d8,      //0xFE
    rst_vec_38, //0xFF
};

void (* const pf_op_ptr_array[])(void) = {
    rlc_br,     //0x00
    rlc_cr,     //0x01
    rlc_dr,     //0x02
    rlc_er,     //0x03
    rlc_hr,     //0x04
    rlc_lr,     //0x05
    rlc_phl,    //0x06
    rlc_ar,     //0x07
    rrc_br,     //0x08
    rrc_cr,     //0x09
    rrc_dr,     //0x0A
    rrc_er,     //0x0B
    rrc_hr,     //0x0C
    rrc_lr,     //0x0D
    rrc_phl,    //0x0E
    rrc_ar,     //0x0F

    rl_br,      //0x10
    rl_cr,      //0x11
    rl_dr,      //0x12
    rl_er,      //0x13
    rl_hr,      //0x14
    rl_lr,      //0x15
    rl_phl,     //0x16
    rl_ar,      //0x17
    rr_br,      //0x18
    rr_cr,      //0x19
    rr_dr,      //0x1A
    rr_er,      //0x1B
    rr_hr,      //0x1C
    rr_lr,      //0x1D
    rr_phl,     //0x1E
    rr_ar,      //0x1F

    sla_br,     //0x20
    sla_cr,     //0x21
    sla_dr,     //0x22
    sla_er,     //0x23
    sla_hr,     //0x24
    sla_lr,     //0x25
    sla_phl,    //0x26
    sla_ar,     //0x27
    sra_br,     //0x28
    sra_cr,     //0x29
    sra_dr,     //0x2A
    sra_er,     //0x2B
    sra_hr,     //0x2C
    sra_lr,     //0x2D
    sra_phl,    //0x2E
    sra_ar,     //0x2F

    swap_br,    //0x30
    swap_cr,    //0x31
    swap_dr,    //0x32
    swap_er,    //0x33
    swap_hr,    //0x34
    swap_lr,    //0x35
    swap_phl,   //0x36
    swap_ar,    //0x37
    srl_br,     //0x38
    srl_cr,     //0x39
    srl_dr,     //0x3A
    srl_er,     //0x3B
    srl_hr,     //0x3C
    srl_lr,     //0x3D
    srl_phl,    //0x3E
    srl_ar,     //0x3F

    bit_0_br,   //0x40
    bit_0_cr,   //0x41
    bit_0_dr,   //0x42
    bit_0_er,   //0x43
    bit_0_hr,   //0x44
    bit_0_lr,   //0x45
    bit_phl,    //0x46
    bit_0_ar,   //0x47
    bit_1_br,   //0x48
    bit_1_cr,   //0x49
    bit_1_dr,   //0x4A
    bit_1_er,   //0x4B
    bit_1_hr,   //0x4C
    bit_1_lr,   //0x4D
    bit_phl,    //0x4E
    bit_1_ar,   //0x4F

    bit_2_br,   //0x50
    bit_2_cr,   //0x51
    bit_2_dr,   //0x52
    bit_2_er,   //0x53
    bit_2_hr,   //0x54
    bit_2_lr,   //0x55
    bit_phl,    //0x56
    bit_2_ar,   //0x57
    bit_3_br,   //0x58
    bit_3_cr,   //0x59
    bit_3_dr,   //0x5A
    bit_3_er,   //0x5B
    bit_3_hr,   //0x5C
    bit_3_lr,   //0x5D
    bit_phl,    //0x5E
    bit_3_ar,   //0x5F

    bit_4_br,   //0x60
    bit_4_cr,   //0x61
    bit_4_dr,   //0x62
    bit_4_er,   //0x63
    bit_4_hr,   //0x64
    bit_4_lr,   //0x65
    bit_phl,    //0x66
    bit_4_ar,   //0x67
    bit_5_br,   //0x68
    bit_5_cr,   //0x69
    bit_5_dr,   //0x6A
    bit_5_er,   //0x6B
    bit_5_hr,   //0x6C
    bit_5_lr,   //0x6D
    bit_phl,    //0x6E
    bit_5_ar,   //0x6F

    bit_6_br,   //0x70
    bit_6_cr,   //0x71
    bit_6_dr,   //0x72
    bit_6_er,   //0x73
    bit_6_hr,   //0x74
    bit_6_lr,   //0x75
    bit_phl,    //0x76
    bit_6_ar,   //0x77
    bit_7_br,   //0x78
    bit_7_cr,   //0x79
    bit_7_dr,   //0x7A
    bit_7_er,   //0x7B
    bit_7_hr,   //0x7C
    bit_7_lr,   //0x7D
    bit_phl,    //0x7E
    bit_7_ar,   //0x7F

    res_0_br,   //0x80
    res_0_cr,   //0x81
    res_0_dr,   //0x82
    res_0_er,   //0x83
    res_0_hr,   //0x84
    res_0_lr,   //0x85
    res_phl,    //0x86
    res_0_ar,   //0x87
    res_1_br,   //0x88
    res_1_cr,   //0x89
    res_1_dr,   //0x8A
    res_1_er,   //0x8B
    res_1_hr,   //0x8C
    res_1_lr,   //0x8D
    res_phl,    //0x8E
    res_1_ar,   //0x8F

    res_2_br,   //0x90
    res_2_cr,   //0x91
    res_2_dr,   //0x92
    res_2_er,   //0x93
    res_2_hr,   //0x94
    res_2_lr,   //0x95
    res_phl,    //0x96
    res_2_ar,   //0x97
    res_3_br,   //0x98
    res_3_cr,   //0x99
    res_3_dr,   //0x9A
    res_3_er,   //0x9B
    res_3_hr,   //0x9C
    res_3_lr,   //0x9D
    res_phl,    //0x9E
    res_3_ar,   //0x9F

    res_4_br,   //0xA0
    res_4_cr,   //0xA1
    res_4_dr,   //0xA2
    res_4_er,   //0xA3
    res_4_hr,   //0xA4
    res_4_lr,   //0xA5
    res_phl,    //0xA6
    res_4_ar,   //0xA7
    res_5_br,   //0xA8
    res_5_cr,   //0xA9
    res_5_dr,   //0xAA
    res_5_er,   //0xAB
    res_5_hr,   //0xAC
    res_5_lr,   //0xAD
    res_phl,    //0xAE
    res_5_ar,   //0xAF

    res_6_br,   //0xB0
    res_6_cr,   //0xB1
    res_6_dr,   //0xB2
    res_6_er,   //0xB3
    res_6_hr,   //0xB4
    res_6_lr,   //0xB5
    res_phl,    //0xB6
    res_6_ar,   //0xB7
    res_7_br,   //0xB8
    res_7_cr,   //0xB9
    res_7_dr,   //0xBA
    res_7_er,   //0xBB
    res_7_hr,   //0xBC
    res_7_lr,   //0xBD
    res_phl,    //0xBE
    res_7_ar,   //0xBF

    set_0_br,   //0xC0
    set_0_cr,   //0xC1
    set_0_dr,   //0xC2
    set_0_er,   //0xC3
    set_0_hr,   //0xC4
    set_0_lr,   //0xC5
    set_0_phl,  //0xC6
    set_0_ar,   //0xC7
    set_1_br,   //0xC8
    set_1_cr,   //0xC9
    set_1_dr,   //0xCA
    set_1_er,   //0xCB
    set_1_hr,   //0xCC
    set_1_lr,   //0xCD
    set_1_phl,  //0xCE
    set_1_ar,   //0xCF

    set_2_br,   //0xD0
    set_2_cr,   //0xD1
    set_2_dr,   //0xD2
    set_2_er,   //0xD3
    set_2_hr,   //0xD4
    set_2_lr,   //0xD5
    set_2_phl,  //0xD6
    set_2_ar,   //0xD7
    set_3_br,   //0xD8
    set_3_cr,   //0xD9
    set_3_dr,   //0xDA
    set_3_er,   //0xDB
    set_3_hr,   //0xDC
    set_3_lr,   //0xDD
    set_3_phl,  //0xDE
    set_3_ar,   //0xDF

    set_4_br,   //0xE0
    set_4_cr,   //0xE1
    set_4_dr,   //0xE2
    set_4_er,   //0xE3
    set_4_hr,   //0xE4
    set_4_lr,   //0xE5
    set_4_phl,  //0xE6
    set_4_ar,   //0xE7
    set_5_br,   //0xE8
    set_5_cr,   //0xE9
    set_5_dr,   //0xEA
    set_5_er,   //0xEB
    set_5_hr,   //0xEC
    set_5_lr,   //0xED
    set_5_phl,  //0xEE
    set_5_ar,   //0xEF

    set_6_br,   //0xF0
    set_6_cr,   //0xF1
    set_6_dr,   //0xF2
    set_6_er,   //0xF3
    set_6_hr,   //0xF4
    set_6_lr,   //0xF5
    set_6_phl,  //0xF6
    set_6_ar,   //0xF7
    set_7_br,   //0xF8
    set_7_cr,   //0xF9
    set_7_dr,   //0xFA
    set_7_er,   //0xFB
    set_7_hr,   //0xFC
    set_7_lr,   //0xFD
    set_7_phl,  //0xFE
    set_7_ar,   //0xFF
};
