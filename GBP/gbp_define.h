#define HBYTE(u) ((u >> 8) & 0xFF)
#define LBYTE(u) (u & 0xFF)
#define AF(A, F) ((uint16_t)A << 8) + F
#define BC(B, C) ((uint16_t)B << 8) + C
#define DE(D, E) ((uint16_t)D << 8) + E
#define HL(H, L) ((uint16_t)H << 8) + L

#define MOSI  11    // MOSI　本来ならGP3 ArduinoIDE上でpinassignが変更可能になったらHWSPIに切り替え
#define CLK   10    // CLK　本来ならGP2
#define CS    9     // Data/Command
#define DC    8     // Data/Command
#define RST   12    // RESET
#define BL    13    // BACK LIGHT

#define MBC_NOT_SUPPORTED  -1
#define MBC_NONE            0
#define MBC_1               1
#define MBC_2               2
#define MBC_3               3
#define MBC_5               5

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
void ld_r8_r8();
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
void ld_pr16_ar();
void ld_phli_ar();
void ld_phld_ar();
void nop();
void inc_bc();
void inc_de();
void inc_hl();
void inc_sp();
void dec_r16();
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
void xor_ar_r8();
void sub_ar_r8();
void add_ar_r8();
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
void ld_r16_d16();
void rla();
void ldh_pd8_ar();
void ldh_ar_pd8();
void pop_r16();
void ld_pcr_ar();
void ld_ar_pcr();
void di();
void ei();
void push_r16();
void ret();
void call_d16();
void cp_d8();
void rl_r8();
void swap();
void bit_();
void res();
void set();
void jp_d16();
void cpl();
void ccf();
void and_d8();
void or_d8();
void or_r8();
void and_r8();
void ld_ar_pa16();
void ld_pa16_ar();
void ret_nz();
void ret_nc();
void ret_z();
void ret_c();
void reti();
void add_hl_r16();
void stop_0();
void jp_cc_d16();
void call_cc_d16();
void adc_ar_r8();
void rst_vec();
void sbc_ar_r8();
void add_ar_d8();
void adc_ar_d8();
void sub_ar_d8();
void sbc_ar_d8();
void sla_r8();
void rlc_r8();
void srl_r8();
void sra_r8();
void rr_r8();
void rra();
void xor_ar_d8();
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
void rrc_r8();
void set_phl();
void res_phl();
void bit_phl();
void rlc_phl();
void rl_phl();
void rrc_phl();
void rr_phl();
void sra_phl();
void srl_phl();
void call_irpt(uint16_t addr);
