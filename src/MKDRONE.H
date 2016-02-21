/******************************************************************************
 File: mkdrone.h

 By: David Schwartz

 Date: Feb 1995

 (C) Williams Entertainment

 Mortal Kombat III Drone Headers
******************************************************************************/

#ifndef __mk_mkdrone_h__
#define __mk_mkdrone_h__

/*
 * 	DEFINITIONS
 */

typedef struct dchance {
	WORD d_prop;		// prob of event to happen
	FUNC d_routine;		// if event routine to jump to
} DCHANCE;

typedef struct
{ 
	short d_char;
	FUNC d_routine;
} DMJ;	

#define ZCLOSE	SCX(0x88)
#define ZMED	SCX(0xd0)
#define ZFAR	SCX(0x100)

#define p_beware_a10 p_store7
#define p_beware_a11 p_store6

/*
 *	RAM
 */
__EXTERN__ WORD dv_hangout;

/*
 *	PROTOTYPES
 */
void drone_begin(void);
void drone_proc(void);
void dont_zap_towards_jumper(void);
void d_attack(void);
void drone_entry(void);
void d_wait_finish_react(void);
void d_backoff_a_bit(void);
void d_attack_very_close(void);
void d_attack_close(void);
void d_attack_far(void);
void d_attack_very_far(void);
void nr_drone_zone(void);
void drone_zone(void);
WORD q_drone_zone(void);
void dz_forward(void); 
void dz_backup(void); 

void drone_zone(void);
WORD q_airborn_counter(void);
void d_hang_out(void);
void hangout_check(void);
void d_stance_pause(WORD pa10);
WORD stance_wait_yes(WORD pa10,FUNCW pa11);
WORD stance_wait_no(WORD pa10,FUNCW pa11);
void d_bflip_jump(void);
void d_bflip_jsrp(void);
void d_jumpup_nocall(void);
void d_jumpup(void);
void d_fflip_scan_jump(void);
void d_bflip_noscan_jsrp(void);
void d_bflip_scan_jsrp(void);
void d_fflip_noscan_jsrp(void);
void d_fflip_scan_jsrp(void);
void d_fflip_jump(void);
void d_fflip_jsrp(void);
void fflip_watchout(void);
void watch_flip_punch(void);
void watch_flip_kick(void);
void d_flip_punch_jump(void);
void run_in_close(void);
void run_in_close_now(void);
void run_in_close_hard(void);
void d_zap(void);
void d_zap_now(void);
void d_do_floor_ice(void);
WORD is_throwing_allowed(void);
void stalk_in_close(void);
void perhaps_flipk(void);
void d_flip_punch_jump(void);
void flipp_scan(void);
void flipk_scan(void);
void d_propell_attack(void);
void d_propell_attack_now(void);
void diff_no_zap(void);
#define diff_no_propell diff_no_zap
void d_walk_in_4_combos(void);
void backflip_setup(void);
void stalk_wait_yes(WORD pa10,FUNCW pa11);
void retreat_wait_yes(WORD pa10,FUNCW pa11);
void frontflip_setup(void);
void stw_proj_proc(void);
void stw_proj_proc_gone(void);
WORD d_wait_yes_still(WORD pa10,FUNCW pa11);

WORD d_stalk_a11(WORD *pa10,short pa11);
WORD d_run_a11(WORD pa10,short pa11);
WORD d_run_till_yes(WORD pa10,FUNCW pa11);
WORD d_retreat_a11(WORD pa10,short pa11);
WORD dist_retp(void);

/* Drone Versions of Normal Routines */
void d_backup_jump(void);
void d_backup_jsrp(void);
void d_slam(void);
void d_elbow(void);
void d_knee(void);
void d_lo_kick(void);
void d_stalk_crossk(void);
void d_crossover_kick(void);
void crossover_scan(void);
void d_fflip_kick_jump(void);
void d_fflip_kick_jsrp(void);
void fflip_scan(void);
void d_jump_up_kick(void);
void jump_up_kick_scan(void);
void d_sweep_kick(void);
void d_uppercut(void);
void d_hi_kick(void);
void d_rapid_lo(void);
void d_rapid_hi(void);
void drone_rfp(WORD pa0,WORD pa1,WORD pa2,WORD pa9);
WORD ask_mr_diff(WORD *pa0);
void random_do(FUNC *pa0);
void chance_do(DCHANCE *pa0);
void ochar_match_jump(DMJ *pa0);
void drone_post_duck_hit(void);
void d_wait_nonattack(WORD pa10);
void d_post_block(void);
void d_turnaround(void);
void d_turnaround_jsrp(void);
void d_getup(void);
void d_get_open(void);
void d_open_jumpover(void);
void d_duck(void);
void d_duck_then_uppercut(void);
void d_duck_fast(void);

/* If you cans!! */
inline void nr_hikick_if_u_can(void);
inline void nr_sweep_if_u_can(void);
inline void nr_uppercut_if_u_can(void);
void if_u_can(WORD pa0,FUNC pa10);

/* Drone Versions of external routines */
void d_block(void);
void d_duck_block(void);
void d_block_projectile(void);
void d_to_normal(void);
void d_stance_setup(void);
void d_walkb_setup(void);
void d_walkf_setup(void);
LONG d_either_edge_a5(void);
LONG d_front_me_a5(void);
LONG d_behind_me_a5(void);

/* FAQ */
long get_his_y_vel(void);
WORD q_my_back_to_him(void);
WORD q_is_he_dropping(void);
WORD q_am_i_a_boss(void);
WORD q_is_he_a_boss(void);
WORD q_square_lower(void);
WORD q_is_he_lower(short pa7);
WORD q_is_he_below_ground(void);
WORD q_is_he_reacting(void);
WORD q_is_he_dizzy(void);
WORD is_he_attacking(void);
WORD is_he_him(WORD);
WORD is_towards_me(void);
WORD q_will_he_reach_me(void);
WORD is_propell_close(void);
WORD is_flyk_close(void);
WORD q_is_he_cornered(void);
WORD q_his_action_0(void);
WORD yes_means_no(FUNCW pa0);
WORD q_am_i_cornered(void);

inline void drone_long_jump(WORD pa0);
void non_violent_finish(void);
void d_finish_him(void);
void d_upcut_finish(void);
void d_background_fatal(void);
void drone_babality(void);
void drone_friendship(void);
void drone_animality(void);
void drone_execute_fatality(void);
void d_get_close_2_u(void);
void fatality_align(short pa0);
WORD fatality_stalk_a11(WORD pa10,short pa11);
void fatality_retreat_a11(void);
void d_fatality_abort(void);
#define d_fatality_cornered d_fatality_abort
void cornered_attack(void);
#define d_cornered cornered_attack


void d_init(void);


/* BEWARE */
void c_react_flipk(void);
void d_beware(void);
void c_angle_jump(void);
void zap_close_away_flipper(void);
void do_fast_orb(void);
void d_lk_hi_zap_jump(WORD pa0);
void d_zap_jump(WORD pa0);

void run_in_juk(void);
void caj_towards_me(void);
void caj_corner(void);
WORD q_corner_backf_land(void);

void st_delayed_whatever(void);
void st2ind(void);
void st2sonya(void);
void st2tusk(void);
void st2lia(void);

void sg_delayed_quake(void);
void tusk_jump_up_zap(void);
#define lk_jump_up_zap tusk_jump_up_zap
#define kitana_jump_up_zap tusk_jump_up_zap
WORD tusk_jup_scan(void);
void robo2_delayed_net(void);
WORD q_is_he_net_close(void);
void d_backup_zap(void);
WORD q_backup_zap(void);
void lia_delayed_scream(void);
WORD q_is_he_scream_close(void);
void swat_zap_hi(void);
void sz_sky_zap(void);
void ind_delayed_axe(void);
WORD q_is_he_axe_close(void);
void jax_delayed_quake(void);
void d_quake(void);
WORD q_jax_smash(void);
void kano_upball(void);
void d_do_mileena_roll(void);

void sonya_delayed_bike(void);
WORD q_is_he_bike_close(void);

void c_icharge_sd(void);
void c_tusk_blur_sd(void);
void c_reptile_dash(void);
#define c_tusk_blur c_reptile_dash

void c_robo_bomb(void);
#define c_bomb run_in_close

void c_st_zap2(void);
void c_stzap23(void);
#define c_st_zap3 c_st_zap2

void c_lk_zap_lo(void);
void c_lkzaplo(void);

void c_lao_zap(void);
void c_hat(void);

void c_robo_net(void);
void c_net(void);

void c_robo_zap2(void);
void c_tracker_rocket(void);
WORD q_is_tracker_close(void);

void c_lia_anglez(void);
void c_lia_zap(void);

void c_swat_bomb_lo(void);
void c_bomblo_pro(void);
void c_swat_bomb_hi(void);
void c_bombhi_pro(void);

void c_sky_ice(void);
void c_sky_pro(void);

void c_floor_blade(void);
WORD q_does_blade_exist(void);
WORD q_is_blade_close(void);

void c_floor_ice(void);
void c_jax_zap2(void);

void c_st_zap1(void);
void ckzap1(void);
#define c_ermac_zap c_st_zap1
#define c_mileena_zap c_st_zap1
#define c_jade_zap c_st_zap1
#define c_spit c_st_zap1
#define c_kitana_zap c_st_zap1
#define c_spear c_st_zap1
#define c_lia_forward c_st_zap1
#define c_forward_ice c_st_zap1
#define c_lk_zaphi c_st_zap1
#define c_sg_zap c_st_zap1
#define c_tusk_zap c_st_zap1
#define c_robo_zap c_st_zap1
#define c_ind_zap c_st_zap1
#define c_jax_zap1 c_st_zap1
#define c_sonya_zap c_st_zap1
#define c_kano_zap c_st_zap1
void standard_zap_counter(void);

void run_then_flipk(void);
WORD q_run_then_flipk(void);

void run_then_duck_under(void);
WORD q_run_then_duck(void);

void lk_zap_low(void);
void indian_reflect(void);

void do_st_summon(void);
void d_do_swat_gun(void);
void d_do_kano_swipe(void);
void do_mystic_drop(void);
void cyrax_counter_zap(void);
void jade_anti_orb(void);
#define jade_anti_zap jade_anti_orb

void c_ermac_slam(void);
void c_reptile_orb(void);
#define c_fast_orb c_reptile_orb
void block_orb(void);
void run_in_fk(void);
void scorp_anti_orb(void);
void run_in_and_slam(void);
void c_mil_air_zap(void);
#define c_air_fan c_mil_air_zap

void c_lk_zap_air(void);
#define c_tusk_zap_air c_lk_zap_air










void duck_under_proj(void);
void duck_under_mproj(void);
WORD q_is_proj_gone(void);

void wait_proj_spawn(void);
void wait_proj_pass(void);

PROCESS *get_his_proj_proc(void);
void nr_jump_over_proj(void);
void d_flipk_over_proj(void);

WORD q_his_proj_proc(void);
WORD q_proj_jclose(void);
short his_proj_front_x(void);
void c_floor_zap(void);

void c_slam_bounce(void);
void c_stsw_sd(void);
void c_robo_tele_sd(void);
void c_jaxdash_sd(void);
void c_screamed(void);
#define c_speared c_screamed
void c_er_slam_sd(void);
void c_proj_sd(void);
#define c_swat_gun_sd c_proj_sd
void drone_sweep_closeup_sd(void);
void attack_closeup_sd(void);
#define c_ssd c_tusk_blur_sd
void d_leg_grab(void);
void d_lia_scream(void);
void d_bike_kick(void);
void c_caught_net(void);
void c_frozen(void);
void c_superk_sd(void);
void c_sbike_sd(void);
#define c_upball_sd c_sbike_sd
void c_zoom_sd(void);
#define c_lk_bike_sd c_zoom_sd
#define c_kroll_sd c_zoom_sd
void c_leg_sd(void);
#define c_kswipe_sd c_leg_sd
#define counter_grounded_sd c_leg_sd
void willy_go_round(void);
WORD q_willy_uppercut(void);
WORD nr_attack_sd(void);

void c_sz_decoy(void);
void counter_decoy(void);
void decoy_zap_wait(void);
void decoy_wait(void);
WORD q_is_decoy_alive(void);

void c_lao_tele(void);

void c_sg_pounce(void);
void av_pounce(void);

void c_sbike(void);
#define c_kano_upball c_sbike
void av_sonya_bike(void);
void swait_nonattack_jump(void);
void swait_land_jsrp(void);

void c_lao_angle(void);
void av_lao_angle(void);
WORD lao_angle_wait(void);

void c_tele_explode(void);
void av_tele_explode(void);

void c_robo_tele(void);
void av_robo_tele(void);

void c_scorp_tele(void);
void av_scorp_tele(void);


void c_square(void);
void ct_square(void);

void c_kano_roll(void);
#define c_bike c_kano_roll
#define c_superkang c_kano_roll
#define c_mileena_roll c_kano_roll
#define c_mileena_tele c_kano_roll
void ct_propell(void);

void c_jax_dash(void);
#define c_ind_charge c_jax_dash
#define c_jade_prop c_jax_dash
void ct_jax_dash(void);

void c_zoom(void);
void ct_zoom(void);

void flykick_heading_up(void);

WORD q_is_kick_over(void);
void run_under_flykick(void);
WORD q_run_under_fk(void);



void c_flykick(void);
#define c_flypunch c_flykick
void av_flyattack(void);

WORD is_he_body_propell(void);

void c_swat_stick(void);
void ct_stick_sweep(void);

void c_uppercut(void);
void ct_uppercut(void);

void c_jupkick(void);
#define c_juppunch c_jupkick

void c_duckpunch(void);
#define c_duck_kickh c_duckpunch
#define c_duck_kickl c_duckpunch
void ct_duck_kick(void);

void c_knee(void);
//void c_elbow(void);
#define c_elbow c_knee
void d_avoid_elbow_knee(void);
void avoid_agressive_bastards(void);

void c_sweep(void);
void av_sweep(void);
#define ct_sweep d_crossover_kick

void c_swat_gun(void);
void c_lia_scream(void);
#define c_fan_lift c_lia_scream
WORD q_dist_lift(void);



void c_hipunch(void);
void c_lopunch(void);
void cpch3(WORD pa1);

void c_noogy(void);
void c_shake(void);
void c_rhouse(void);
void c_hikick(void);
void c_lokick(void);
void ckik3(WORD pa1);
void ct_kicks(void);

void c_laospin(void);
void ct_laospin(void);

void c_quake(void);
void ct_quake(void);

void c_kano_swipe(void);
void ct_kswipe(void);

void c_axe_up(void);
void ct_axe_up(void);

void c_leg_grab(void);
void ct_leg(void);

void d_land(void);
void d_beware_mframew(WORD pa0);

void nr_kano_abuser_check(void);
WORD q_is_kano_charging(void);
WORD get_last_kano_charge(OBJECT *pa8);


WORD should_i_promove(void);

void react_jump_table(WORD pa1,FUNC *pa2);
extern WORD rpt_promoves[];
extern WORD rpt_counter[];
extern WORD d_randpers[];
extern WORD repeat_table[];
WORD count_q_repeats(WORD pa1);
void scan_1_entry(WORD pa1);

/* drone external references */
void d_flip_kick_jump(FUNC pa6);
#define d_body_propell do_body_propell
#define d_check_winner_status check_winner_status
#define d_strike_check_a0_test(_la8,_la0) strike_check_a0_test(_la8,_la0)
#define d_get_my_dfe get_my_dfe
#define d_stop_me(_la8) stop_me(_la8)
void d_reaction_exit(void);
#define d_get_his_action(_proc) get_his_action((_proc))
#define d_next_anirate next_anirate
#define d_get_x_dist get_x_dist
#define d_get_char_ani(_la9) get_char_ani(ANIM_TABLE1,(_la9))
#define d_do_next_a9_frame(_la8) do_next_a9_frame(_la8)
void boss_branch(WORD pa0);


/*
 *	MACROS
 */

/* stalk macros */
#define d_stalk_a11_ntl(_la11) \
			current_proc->a10=0x40*500, d_stalk_a11(&(*((WORD *)current_proc->a10)),(_la11))

/* return macros */
#define d_rets return
#define d_retp return

/* q_is_he_airborn */
#define q_is_he_airborn is_he_airborn

/* return to beware */
#define return_to_beware \
		{ \
		current_proc->a10=current_proc->pdata.p_beware_a10; \
		current_proc->a11=current_proc->pdata.p_beware_a11; \
		return; \
		}

#define return_to_beware_4get \
		{ \
		current_proc->pdata.p_otheract=0; \
		current_proc->a10=current_proc->pdata.p_beware_a10; \
		current_proc->a11=current_proc->pdata.p_beware_a11; \
		return; \
		}


#define react_jump_table_act(_la2) react_jump_table(d_get_his_action(current_proc),(_la2))

#endif /* __mk_mkdrone_h__ */

