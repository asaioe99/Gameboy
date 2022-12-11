#define HBYTE(u) ((u >> 8) & 0xFF)
#define LBYTE(u) (u & 0xFF)
#define AF(A, F) ((uint16_t)A << 8) | F
#define BC(B, C) ((uint16_t)B << 8) | C
#define DE(D, E) ((uint16_t)D << 8) | E
#define HL(H, L) ((uint16_t)H << 8) | L

#define PIN_DIN 11
#define PIN_CLK 10
#define PIN_CS 9
#define PIN_DC 8
#define PIN_RESET 12
#define PIN_BL 13

#define KEY_R 20
#define KEY_L 16
#define KEY_U 2
#define KEY_D 18
#define KEY_A 15
#define KEY_B 17
#define KEY_X 19
#define KEY_Y 21

#define SERIAL_CLK_DIV 2.f

#define MBC_NOT_SUPPORTED  -1
#define MBC_NONE            0
#define MBC_1               1
#define MBC_2               2
#define MBC_3               3
#define MBC_5               5

void callback_joypad_R();

void ld_br_d8();
void ld_cr_d8();
void ld_dr_d8();
void ld_er_d8();
void ld_hr_d8();
void ld_lr_d8();
void ld_ar_d8();
void ld_br_phl();
void ld_cr_phl();
void ld_dr_phl();
void ld_er_phl();
void ld_hr_phl();
void ld_lr_phl();
void ld_ar_phl();
void ld_br_br();
void ld_br_cr();
void ld_br_dr();
void ld_br_er();
void ld_br_hr();
void ld_br_lr();
void ld_br_ar();
void ld_cr_br();
void ld_cr_cr();
void ld_cr_dr();
void ld_cr_er();
void ld_cr_hr();
void ld_cr_lr();
void ld_cr_ar();
void ld_dr_br();
void ld_dr_cr();
void ld_dr_dr();
void ld_dr_er();
void ld_dr_hr();
void ld_dr_lr();
void ld_dr_ar();
void ld_er_br();
void ld_er_cr();
void ld_er_dr();
void ld_er_er();
void ld_er_hr();
void ld_er_lr();
void ld_er_ar();
void ld_hr_br();
void ld_hr_cr();
void ld_hr_dr();
void ld_hr_er();
void ld_hr_hr();
void ld_hr_lr();
void ld_hr_ar();
void ld_lr_br();
void ld_lr_cr();
void ld_lr_dr();
void ld_lr_er();
void ld_lr_hr();
void ld_lr_lr();
void ld_lr_ar();
void ld_ar_br();
void ld_ar_cr();
void ld_ar_dr();
void ld_ar_er();
void ld_ar_hr();
void ld_ar_lr();
void ld_ar_ar();
void ld_phl_br();
void ld_phl_cr();
void ld_phl_dr();
void ld_phl_er();
void ld_phl_hr();
void ld_phl_lr();
void ld_phl_ar();
void ld_phl_d8();
void ld_ar_pr16();
void ld_ar_phli();
void ld_ar_phld();
void ld_pbc_ar();
void ld_pde_ar();
void ld_phli_ar();
void ld_phld_ar();
void nop();
void inc_bc();
void inc_de();
void inc_hl();
void inc_sp();
void dec_bc();
void dec_de();
void dec_hl();
void dec_sp();
void inc_br();
void inc_cr();
void inc_dr();
void inc_er();
void inc_hr();
void inc_lr();
void inc_ar();
void dec_br();
void dec_cr();
void dec_dr();
void dec_er();
void dec_hr();
void dec_lr();
void dec_ar();
void xor_ar_br();
void xor_ar_cr();
void xor_ar_dr();
void xor_ar_er();
void xor_ar_hr();
void xor_ar_lr();
void xor_ar_phl();
void xor_ar_ar();
void sub_ar_br();
void sub_ar_cr();
void sub_ar_dr();
void sub_ar_er();
void sub_ar_hr();
void sub_ar_lr();
void sub_ar_phl();
void sub_ar_ar();
void add_ar_br();
void add_ar_cr();
void add_ar_dr();
void add_ar_er();
void add_ar_hr();
void add_ar_lr();
void add_ar_phl();
void add_ar_ar();
void cp_br();
void cp_cr();
void cp_dr();
void cp_er();
void cp_hr();
void cp_lr();
void cp_phl();
void cp_ar();
void jr_d8();
void jr_nz_d8();
void jr_z_d8();
void jr_nc_d8();
void jr_c_d8();
void ld_bc_d16();
void ld_de_d16();
void ld_hl_d16();
void ld_sp_d16();
void rla();
void ldh_pd8_ar();
void ldh_ar_pd8();
void pop_bc();
void pop_de();
void pop_hl();
void pop_af();
void ld_pcr_ar();
void ld_ar_pcr();
void di();
void ei();
void push_r16();
void push_bc();
void push_de();
void push_hl();
void push_af();
void ret();
void call_d16();
void cp_d8();
void rl_br();
void rl_cr();
void rl_dr();
void rl_er();
void rl_hr();
void rl_lr();
void rl_ar();
void swap_br();
void swap_cr();
void swap_dr();
void swap_er();
void swap_hr();
void swap_lr();
void swap_ar();
void bit_0_br();
void bit_1_br();
void bit_2_br();
void bit_3_br();
void bit_4_br();
void bit_5_br();
void bit_6_br();
void bit_7_br();
void bit_0_cr();
void bit_1_cr();
void bit_2_cr();
void bit_3_cr();
void bit_4_cr();
void bit_5_cr();
void bit_6_cr();
void bit_7_cr();
void bit_0_dr();
void bit_1_dr();
void bit_2_dr();
void bit_3_dr();
void bit_4_dr();
void bit_5_dr();
void bit_6_dr();
void bit_7_dr();
void bit_0_er();
void bit_1_er();
void bit_2_er();
void bit_3_er();
void bit_4_er();
void bit_5_er();
void bit_6_er();
void bit_7_er();
void bit_0_hr();
void bit_1_hr();
void bit_2_hr();
void bit_3_hr();
void bit_4_hr();
void bit_5_hr();
void bit_6_hr();
void bit_7_hr();
void bit_0_lr();
void bit_1_lr();
void bit_2_lr();
void bit_3_lr();
void bit_4_lr();
void bit_5_lr();
void bit_6_lr();
void bit_7_lr();
void bit_0_ar();
void bit_1_ar();
void bit_2_ar();
void bit_3_ar();
void bit_4_ar();
void bit_5_ar();
void bit_6_ar();
void bit_7_ar();
void res_0_br();
void res_1_br();
void res_2_br();
void res_3_br();
void res_4_br();
void res_5_br();
void res_6_br();
void res_7_br();
void res_0_cr();
void res_1_cr();
void res_2_cr();
void res_3_cr();
void res_4_cr();
void res_5_cr();
void res_6_cr();
void res_7_cr();
void res_0_dr();
void res_1_dr();
void res_2_dr();
void res_3_dr();
void res_4_dr();
void res_5_dr();
void res_6_dr();
void res_7_dr();
void res_0_er();
void res_1_er();
void res_2_er();
void res_3_er();
void res_4_er();
void res_5_er();
void res_6_er();
void res_7_er();
void res_0_hr();
void res_1_hr();
void res_2_hr();
void res_3_hr();
void res_4_hr();
void res_5_hr();
void res_6_hr();
void res_7_hr();
void res_0_lr();
void res_1_lr();
void res_2_lr();
void res_3_lr();
void res_4_lr();
void res_5_lr();
void res_6_lr();
void res_7_lr();
void res_0_ar();
void res_1_ar();
void res_2_ar();
void res_3_ar();
void res_4_ar();
void res_5_ar();
void res_6_ar();
void res_7_ar();
void set_0_br();
void set_1_br();
void set_2_br();
void set_3_br();
void set_4_br();
void set_5_br();
void set_6_br();
void set_7_br();
void set_0_cr();
void set_1_cr();
void set_2_cr();
void set_3_cr();
void set_4_cr();
void set_5_cr();
void set_6_cr();
void set_7_cr();
void set_0_dr();
void set_1_dr();
void set_2_dr();
void set_3_dr();
void set_4_dr();
void set_5_dr();
void set_6_dr();
void set_7_dr();
void set_0_er();
void set_1_er();
void set_2_er();
void set_3_er();
void set_4_er();
void set_5_er();
void set_6_er();
void set_7_er();
void set_0_hr();
void set_1_hr();
void set_2_hr();
void set_3_hr();
void set_4_hr();
void set_5_hr();
void set_6_hr();
void set_7_hr();
void set_0_lr();
void set_1_lr();
void set_2_lr();
void set_3_lr();
void set_4_lr();
void set_5_lr();
void set_6_lr();
void set_7_lr();
void set_0_ar();
void set_1_ar();
void set_2_ar();
void set_3_ar();
void set_4_ar();
void set_5_ar();
void set_6_ar();
void set_7_ar();
void jp_d16();
void cpl();
void ccf();
void and_d8();
void or_d8();
void or_br();
void or_cr();
void or_dr();
void or_er();
void or_hr();
void or_lr();
void or_ar();
void and_br();
void and_cr();
void and_dr();
void and_er();
void and_hr();
void and_lr();
void and_ar();
void ld_ar_pa16();
void ld_pa16_ar();
void ret_nz();
void ret_nc();
void ret_z();
void ret_c();
void reti();
void add_hl_sp();
void add_hl_bc();
void add_hl_de();
void add_hl_hl();
void stop_0();
void jp_nz_d16();
void jp_nc_d16();
void jp_z_d16();
void jp_c_d16();
void call_nz_d16();
void call_nc_d16();
void call_z_d16();
void call_c_d16();
void adc_ar_br();
void adc_ar_cr();
void adc_ar_dr();
void adc_ar_er();
void adc_ar_hr();
void adc_ar_lr();
void adc_ar_phl();
void adc_ar_ar();
void rst_vec_00();
void rst_vec_08();
void rst_vec_10();
void rst_vec_18();
void rst_vec_20();
void rst_vec_28();
void rst_vec_30();
void rst_vec_38();
void sbc_ar_br();
void sbc_ar_cr();
void sbc_ar_dr();
void sbc_ar_er();
void sbc_ar_hr();
void sbc_ar_lr();
void sbc_ar_phl();
void sbc_ar_ar();
void add_ar_d8();
void adc_ar_d8();
void sub_ar_d8();
void sbc_ar_d8();
void sla_br();
void sla_cr();
void sla_dr();
void sla_er();
void sla_hr();
void sla_lr();
void sla_ar();
void rlc_br();
void rlc_cr();
void rlc_dr();
void rlc_er();
void rlc_hr();
void rlc_lr();
void rlc_ar();
void srl_br();
void srl_cr();
void srl_dr();
void srl_er();
void srl_hr();
void srl_lr();
void srl_ar();
void sra_br();
void sra_cr();
void sra_dr();
void sra_er();
void sra_hr();
void sra_lr();
void sra_ar();
void rr_br();
void rr_cr();
void rr_dr();
void rr_er();
void rr_hr();
void rr_lr();
void rr_ar();
void rra();
void rrc_br();
void rrc_cr();
void rrc_dr();
void rrc_er();
void rrc_hr();
void rrc_lr();
void rrc_ar();
void xor_d8();
void or_ar_phl();
void and_ar_phl();
void dec_phl();
void inc_phl();
void halt();
void rlca();
void daa();
void scf();
void ld_pd16_sp();
void add_sp_d8();
void jp_hl();
void ld_sp_hl();
void rrca();
void ld_hl_sp_d8();
void swap_phl();
void sla_phl();
void set_0_phl();
void set_1_phl();
void set_2_phl();
void set_3_phl();
void set_4_phl();
void set_5_phl();
void set_6_phl();
void set_7_phl();
void res_phl();
void bit_phl();
void rlc_phl();
void rl_phl();
void rrc_phl();
void rr_phl();
void sra_phl();
void srl_phl();
void call_irpt(uint16_t addr);
