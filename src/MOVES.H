/******************************************************************************
 File: moves.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III MOVES COMBOS HEADER
******************************************************************************/

#ifndef __mk_moves_h__
#define __mk_moves_h__

/*
 *	DEFINITIONS
 */
#define EOW_LX SCX(0x40)			// edge_of_world: left edge bound
#define EOW_RX SCX(0x40)			// edge_of_world: right edge bound

#define UP_GRAV SCY(0x8000)			// up gravity
#define UP_VEL -SCY(0xa0000)		// up velocity

#define ANGLE_GRAV SCY(0x8000)		// angle gravity

#define DONT_TOUCH	13							// command not to change velocity info (flight_call)

/* xfer types */
#define X_GROUND		0
#define X_AIRBORN		1
#define X_FATAL			2
#define X_MERCY			3
#define X_ANIMAL		4
#define X_CLOSE_ANIMAL	5
#define X_FRIEND		6
#define X_BABY			7
#define X_LASTONE		X_BABY


/* r_a10_offset indexes */
#define R_A10_SHAKE 0x00			// post shake reaction
#define R_A10_BIKE 0x01				// post bike reaction

/* secret move header */
/* NOTE, WHEN ADDING TABLES FROM ARCADE MAKE SURE FORMAT IS CORRECT */
typedef struct smove {
	WORD s_p1leg;			// p1 requirements, format change long->word
	WORD s_p2leg;			// p2 requirements, format change long->word
	LONG s_xfer;			// xfer type index, format change word->long
	LONG s_yes_no;			// yes/no routine
	FUNC s_routine;			// address of secret move

 	WORD s_time;			// time for move
 	WORD s_r1;				// move right 1
 	WORD s_r2;				// move right 2

 	WORD s_r3;				// move right 3
 	WORD s_r4;				// move right 4
 	WORD s_r5;				// move right 5
 	WORD s_r6;				// move right 6

 	WORD s_l1;				// move left 1
 	WORD s_l2;				// move left 2
 	WORD s_l3;				// move left 3
 	WORD s_l4;				// move left 4

 	WORD s_l5;				// move left 5
 	WORD s_l6;				// move left 6
 	WORD s_pad;				// padding added to keep alignment
} SMOVE;


/* power moves sets */
#define POW_FLIPKP	4


/* damage values */
#define ZOOM_DAMAGE	0x20

/*
 *	RAM
 */


/*
 *	PROTOTYPES
 */
void reaction_exit(OBJECT *obj);

void ermac_hk_close(void);
void ermac_lk_close(void);
void ermac_hp_close(void);
void ermac_lp_close(void);
void ermac_block_close(void);

void scorp_hp_close(void);
void scorp_lp_close(void);
void scorp_block_close(void);
#define osm_block_close scorp_block_close
void scorp_hk_close(void);
void scorp_lk_close(void);
void scorp_run_close(void);

void rep_hp_close(void);
void rep_lp_close(void);
void rep_block_close(void);
void rep_lk_close(void);
void rep_hk_close(void);

void kit_lp_close(void);
void kit_hp_close(void);
void kit_hk_close(void);
void kit_lk_close(void);
void kit_run_close(void);

void mil_lk_close(void);
void mil_hk_close(void);
void mil_hp_close(void);
void mil_lp_close(void);
void mil_hp_open(void);

void jad_hp_close(void);
void jad_lp_close(void);
void jad_run_close(void);
void jad_lk_close(void);
void jad_hk_close(void);

void sg_block_close(void);
void sg_hk_open(void);
void sg_lp_close(void);
void sg_hk_close(void);
void sg_up(void);
void sg_hp_close(void);

void lk_down_close(void);
void lk_up_close(void);
void lk_block_close(void);
void lk_lk_close(void);
void lk_hp_close(void);
void lk_lp_close(void);
void lk_hk_close(void);
void lk_lk_open(void);

void st_hp_open(void);
void st_lp_open(void);
void st_lk_open(void);
void st_run_close(void);
void st_hk_close(void);
void st_lk_close(void);
void st_up_close(void);
void st_down_close(void);
void st_block_close(void);
void st_lp_close(void);
void st_hp_close(void);
void st_2_sonya(void);

void tusk_up(void);
void tusk_hk_close(void);
void tusk_block_close(void);
void tusk_run_close(void);
void tusk_lk_close(void);
void tusk_lp_close(void);
void tusk_hp_close(void);
void tusk_hp_open(void);

void smoke_lk_close(void);
void smoke_up_close(void);
void smoke_down_close(void);
void smoke_hk_close(void);
void smoke_run_close(void);
void smoke_lp_close(void);
void smoke_block_close(void);
void smoke_block_close2(void);

void kano_block_close(void);
void kano_lp_close(void);
void kano_hk_close(void);
void kano_lp_open(void);
void kano_hp_close(void);
void kano_lk_close(void);
void kano_hp_open(void);
void kano_lk_open(void);

void sonya_down_close(void);
void sonya_run_close(void);
void sonya_lp_open(void);
void sonya_lk_close(void);
void sonya_hk_close(void);
void sonya_hp_close(void);
void sonya_lp_close(void);
void sonya_block_close(void);
WORD check_st_2_sonya(void);
WORD check_sonya_legs(void);

void lia_up_close(void);
void lia_block_close(void);
void lia_lp_close(void);
void lia_lk_close(void);
void lia_hk_close(void);
void lia_hp_close(void);

void lao_lk_close(void);
void lao_block_close(void);
void lao_down_close(void);
void lao_hp_close(void);
void lao_run_close(void);
void lao_up(void);
void lao_lp_close(void);
void lao_hk_close(void);
void all_run_open(void);

void jax_lk_close(void);
void jax_block_close(void);
void jax_block_close2(void);
void jax_block_open(void);
void jax_lp_open(void);
void jax_hp_close(void);
void jax_lp_close(void);
void jax_hk_close(void);
void jax_lk_open(void);

void ind_block_close(void);
void ind_down_close(void);
void ind_hp_close(void);
void ind_hp_open(void);
void ind_lp_close(void);
void ind_lk_close(void);
void ind_hk_close(void);

void osm_lp_close(void);
void osm_hk_close(void);

void osz_hp_close(void); 
void osz_lp_close(void); 
void osz_lk_close(void); 
void osz_block_close(void); 

void sz_run_close(void);
void sz_up(void);
void sz_hk_close(void);
void sz_hp_close(void);
void sky_ice_xfer(FUNC pa7);
void sz_lk_close(void);
void sz_block_close(void);
void sz_lp_close(void);
void slide_check(void);

void sw_block_close(void);
void sw_hk_close(void);
void sw_lk_close(void);
void sw_lp_close(void);
void sw_hp_close(void);

void robo2_up_close(void);
void robo2_down_close(void);
void robo_hk_close(void);
void robo_block_close(void);
void robo2_run_close(void);
void robo2_hp_close(void);
void robo2_lp_close(void);
void robo2_block_close(void);
void robo2_hk_close(void);
void robo_lp_close(void);
void robo_hp_close(void);
void robo1_down_close(void);
void robo1_up(void);
void robo1_lk_close(void);
void robo2_lk_close(void);

void check_tsl(WORD xfer_flag,FUNC routine,WORD *movetime,WORD timer);
WORD button_bit_check(OBJECT *obj,WORD p1_legal,WORD p2_legal);
WORD illegal_button_check(OBJECT *obj,WORD p1_legal,WORD p2_legal);
inline WORD buttons_in_a2(OBJECT *obj);

void do_ermac_slam(void);
void do_reptile_inv(void);
void do_jade_flash(void);
void do_fan_lift(void);
void do_baby(void);
void do_roundhouse(void);
void do_uppercut(void);
void do_duck_kickl(void);
void do_duck_kickh(void);
void do_duck_punch(void);
void do_sweep_kick(void);
void do_lo_kick(void);
void do_hi_kick(void);
void do_swat_gun(void);
void do_kano_swipe(void);
void do_leg_throw(void);
void do_inviso(void);
void do_quake(void);
void do_noogy(void);
void do_shake(void);
void do_reflect(void);
void do_fast_axe_up(void);
void do_axe_up(void);
void do_lia_scream(void);
void do_lao_spin(void);

void do_kano_roll(void);
void do_bike(void);
void do_ind_charge(void);
void jax_dash_punch(void);
void do_sz_decoy(void);
void do_lia_fly(void);
void do_lao_tele(void);
void do_lao_angle_kick(void);
void do_robo_tele(void);
#define do_smoke_tele do_robo_tele
void do_robo_air_grab(void);
void do_tele_explode(void);
void do_square_wave(void);
void lk_bike_kick(void);
void do_super_kang(void);
void do_sg_pounce(void);
void do_slide(void);
void do_swat_zoom(void);
void do_stick_sweep(void);
void do_tusk_blur(void);
void do_sg_quake(void);
void do_ninja_slide(void);
void do_scorp_tele(void);
void do_reptile_dash(void);
void do_jade_prop(void);
void do_mileena_tele(void);
void do_mileena_roll(void);
void do_ermac_tele(void);
void do_kano_upball(void);


void do_kano_zap(void);
void do_sonya_zap(void);
void do_jax_zap1(void);
void do_jax_zap2(void);
void do_ind_zap(void);
void do_sky_ice_on(void);
void do_sky_ice_behind(void);
void do_sky_ice_front(void);
void do_sw_zap(void);
void do_robo_zap(void);
void do_robo_zap2(void);
void do_robo_net(void);
void do_sz_forward_zap(void);
void do_lia_anglez(void);
void do_lao_zap(void);
void do_bomb_full(void);
void do_bomb_mid(void);
void do_tusk_zap(void);
void do_summon(void);
void do_st_zap1(void);
void do_st_zap2(void);
void do_st_zap3(void);
void lk_zap_hi(void);
void lk_zap_lo(void);
void do_sg_zap(void);
void do_swat_bomb_hi(void);
void do_swat_bomb_lo(void);
void do_lia_forward(void);
void do_floor_blade(void);
void do_smoke_spear(void);
void do_kitana_zap(void);
void do_jade_zap_med(void);
void do_reptile_orb(void);
void do_spit(void);
void do_scorpion_spear(void);
void do_jade_zap_hi(void);
void do_jade_zap_lo(void);
void do_jade_zap_ret(void);
void do_reptile_orb_fast(void);
void do_mileena_zap(void);
void do_osz_zap(void);
void do_floor_ice(void);
void do_ermac_zap(void);

void fatality_xfer_range(short xmin,short xmax,FUNC fatal_func);
void fatality_xfer(FUNC fatal_func);
void close_animality_xfer(FUNC anim_func);
void animality_xfer(FUNC anim_func);
void babality_xfer(FUNC fatal_func);
void special_xfer(WORD xfertype,FUNC routine);
WORD is_my_proj_alive(void);
WORD stick_look_lr(WORD p1_legal,WORD p2_legal,WORD *scom);
inline LONG previous_q_entry(LONG *qindex,LONG *quebase);
inline LONG get_bcq_next_pointer(LONG **quebase);
inline LONG get_jcq_next_pointer(LONG **quebase);

void do_st_2_kano(void);
void do_st_2_sonya(void);
void do_st_2_jax(void);
void do_st_2_ind(void);
void do_st_2_sz(void);
void do_st_2_swat(void);
void do_st_2_lia(void);
void do_st_2_robo1(void);
void do_st_2_robo2(void);
void do_st_2_lao(void);
void do_st_2_tusk(void);
void do_st_2_sg(void);
void do_st_2_lk(void);
void do_st_2_smoke(void);
void do_st_2_kitana(void);
void do_st_2_jade(void);
void do_st_2_mileena(void);
void do_st_2_scorpion(void);
void do_st_2_reptile(void);
void do_st_2_ermac(void);
void do_st_2_oldsz(void);
void do_st_2_oldsmoke(void);
void shang_morph(WORD pa9);

void init_special(OBJECT *obj);
void air_init_special(OBJECT *obj);
void secret_move_search(SMOVE *pa14);
void do_mercy(void);

WORD q_smoke_tele(void);
WORD q_spear_ok(void);
WORD q_robo_flame_fatal(void);
WORD q_taser_fatal(void);
WORD q_close_fatal(void);
WORD q_far_fatal(void);
WORD q_vomit_fatal(void);
WORD q_tongue_fatal(void);
WORD q_skull_fatal(void);
WORD q_half_screen_fatal(void);
WORD q_fatal_dist(short pa5,short pa6);
WORD q_fatality_req(void);
WORD q_sz_decoy(void);
WORD q_blurred_req(void);
WORD q_pounce_ok_now(void);
WORD q_bike_req(void);
WORD q_lia_scream(void);
WORD q_sz_forward_zap(void);
WORD q_kabal_animal(void);
WORD q_eatit_fatal(void);
WORD q_bubble_fatal(void);
WORD q_earth_fatal(void);
WORD q_st_spike_fatal(void);
WORD q_pit_fatal(void);
WORD q_inflate_fatal(void);
WORD q_swat_animal(void);
WORD q_lao_hat_fatal(void);
WORD q_scream_fatal(void);
WORD q_lia_hair_fatal(void);
WORD q_grow_fatal(void);
WORD q_kano_animal(void);
WORD q_close_animal(void);
WORD q_sherip_fatal(void);
WORD q_skel_fatal(void);
WORD q_ind_axe_fatal(void);
WORD q_lk_animal(void);
WORD q_smoke_animal(void);
WORD q_shang_animal(void);
WORD q_animal_dist(short pa5,short pa6);
WORD is_master_in_finish(void);
WORD q_mercy_req(void);
WORD q_mercy(void);
WORD q_dinger_friend(void);
WORD q_smoke_friend(void);
WORD q_ind_friend(void);
WORD q_sonya_friend(void);
WORD q_close_friend(void);
WORD q_friend(void);
WORD q_babality(void);
WORD q_animal_req(void);
WORD q_floor_blade(void);
WORD q_lk_friend(void);
WORD q_robo_crush(void);
WORD q_shang_morph_lk(void);
WORD q_shang_morph_sg(void);
WORD q_shang_morph_lao(void);
WORD q_shang_morph_robo1(void);
WORD q_shang_morph_swat(void);
WORD q_shang_morph_tusk(void);
WORD q_shang_morph_ind(void);
WORD q_shang_morph_robo2(void);
WORD q_shang_morph_kano(void);
WORD q_shang_morph_jax(void);
WORD q_shang_morph_sz(void);
WORD q_shang_morph_lia(void);
WORD q_shang_morph(WORD pa0);
WORD q_robo_net(void);
WORD q_ind_charge(void);
WORD q_swat_zoom(void);
WORD q_f_mileena(void);
WORD q_f_ermac(void);
WORD q_f_oldsz(void);
WORD q_ermac_slam(void);
WORD q_mileena_zap(void);
WORD q_mileena_roll(void);
WORD q_jade_zap_ret(void);
WORD q_jade_flash(void);
WORD q_scorp_tele(void);
WORD q_jade_prop(void);
WORD q_fan_lift(void);
WORD q_noogy(void);
WORD q_jax_dash(void);
WORD q_spear(void);
WORD q_swat_gun(void);
WORD q_scorp_airthrow(void);
WORD q_kit_fan(void);
WORD q_reptile_orb_fast(void);
WORD q_reptile_orb_slow(void);
WORD qorb3(WORD *);
WORD q_both_punches(void);
WORD q_slide(void);
WORD q_lp_block_lk(void);
WORD q_floor_ice(void);
WORD q_jax_zap(void);
WORD q_st_zap(void);
WORD q_kit_animal(void);
WORD q_lao_spin(void);
WORD q_ermac_decap(void);
WORD q_ermac_fatal(void);
WORD q_kano_upball(void);
WORD q_kano_swipe(void);
WORD he_is_sweeping(void);

WORD home_code_eval(WORD ptype,WORD p_legal,WORD *scom);
void home_code_check(WORD ptype,SMOVE *pa14);
void home_code_que(WORD swoffset,LONG *b_queue);
void hc_free_play(void);
void hc_smoke(void);
void hc_cheat(void);
void hc_cheat1(void);


/* switch tables */
extern JUMPTBL *switch_close_jumps[];
extern JUMPTBL *switch_open_jumps[];
extern WORD ta_speeds[][2];

/*
 *	MACROS
 */

/* init_special_act */
#define init_special_act(_act,_obj) \
		(current_proc->pdata.p_action=(_act)), (init_special((_obj)))

/* various xfer routines */
#define restricted_xfer(_func) (special_xfer(0,(_func)))
#define airborn_xfer(_func) (special_xfer(1,(_func)))
#define free_xfer(_func) (special_xfer(2,(_func)))
#define fatality_xfer_close(_xmax,_ffunc) fatality_xfer_range(0,(_xmax),(_ffunc))
#define friendship_xfer(_func) babality_xfer(_func)
#define mercy_xfer(_func) babality_xfer(_func)

/* jumpup punch kick */
#define do_jumpup_punch do_jumpup_kick_punch(JUP_PUNCH)
#define do_jumpup_kick do_jumpup_kick_punch(JUP_KICK)

/* p hit status */
#define get_his_p_hit(_la13) ((_la13)->pdata.p_otherproc)->pdata.p_hit

#endif /* __mk_moves_h__ */
