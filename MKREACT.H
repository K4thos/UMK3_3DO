/******************************************************************************
 File: mkreact.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III PLAYER REACTION HEADER
******************************************************************************/

#ifndef __mk_mkreact_h__
#define __mk_mkreact_h__

/*
 * 	DEFINITIONS
 */

/* reaction start codes */
#define REACT_START 0								/* execute reaction_start */
#define REACT_BLOCK 1								/* execute blocked_start */

/*
 *	RAM
 */
__EXTERN__ WORD f_blast;

/*
 *	PROTOTYPES
 */
void fall_in_lava(void);

void r_ermac_fatal_slam(void);
void death_slam_pause(void);
void r_ermac_slam(void);
void slammed_shake_up(void);
void pose_stumble_frame_1(void);
void slammed_zoom_up(WORD pa10);
void slammed_slam_down(void);

void r_swat_gun(void);
void at_least_ground_level(void);

void r_floor_ice(void);
void slip_sleep(void);
PROCESS *get_his_floor_ice(void);

void r_mileena_roll(void);
void r_mileena_tele(void);

void r_reptile_dash(void);
void r_scorp_tele(void);
void r_spit(void);
void r_orb(void);

void r_boomerang(void);
void r_fan_lift(void); 
WORD is_he_flipped(void);
void r_fan(void);

void r_scorpion_spear(void);
void r_smoke_spear(void);
void spear0(WORD pa0);

void r_dummy(void);
void r_null_speared(void);
void r_tusk_saw(void);
void r_zoom(void);
void r_zoom_flipped(void);
void r_scream(void);
void r_net(void);
void net_sleep(void);
WORD if_shao_then_pass(WORD pa6);
void r_jax_dash(void);
void r_axe_up(void);
void r_hi_kick(void);
void r_lo_kick(void);
void r_duck_punch(void);
void r_duck_airpunch(void);
void r_duck_kickh(void);
void r_duck_kickl(void);
void r_robo_tele(void);
void r_robo_bomb(void);
void r_combo6(void);
void r_lao_spin(void);
void r_uppercut(void);

void fall_down_bell_tower(void);
OBJECT *make_a_spike(void);
void fix_broken_pieces(OBJECT *pa8);
void floor_piece(void);
void broken_floor_a(void);
void broken_floor_b(void);
void flr2(OIMGTBL *pa5);
void baklst_down_2_floors(OBJECT **pa8);
void scroll_down_1_floor(void);
void crash_fx(void);

void bell_fall_scan(void);
void fall_down_pit(void);
void machine_sound(void);
void bone_grind_sound(void);
void pit_fall_scan(void);

void fall_on_trax(void);
void train_sound_proc(void);

void blast_entry(long pa4,void *pa5,void *pa6,long pa7);
void blast_init(long pa4,void *pa5,void *pa6,long pa7);
void smoke_if_smoke(void);

void r_combo5(void);
void r_combo0(void);
void r_combo1(void);
void r_combo1_stab(void);
void r_combo_klang(void);
void combo1(void);
void r_combo2(void);
void combo2(void);
void r_combo2_stab(void);
void r_combo3(void);
void r_boss_hit1(void);
#define r_sk_punch r_boss_hit1
void r_combo4(void);
void combo43(void);
void combo_setup(void);
void combo_sitting_duck(void);
void r_flip_punch(void);
void r_tusk_elbow(void);
void rek3(void);
void r_elbow_knee(void);
void r_stick_sweep(void);
void r_sweep(void);
void sweep3(void);
void r_flip_kick(void);
void r_last_noogy(void);
#define r_angle_kick r_last_noogy
void onback3(void);
void r_hi_punch(void);
void r_lo_punch(void);
void r_airpunch(void);
void avoid_corner_trap(WORD pa1);
void r_kano_roll(void);
void r_roundhouse(void);
void r_square(void);
void r_kano_swipe(void);
#define r_axe_horz r_kano_swipe
void r_motaro_kick(void);
void r_sk_hammer(void);
void r_sk_air_charge(void);
void dizzy_by_boss(void);
void r_decoy_freeze(void);
void r_a10_offset(void);
void r_slide(void);
void r_pounce2(void);
void r_pounce(void);
void r_superkang(void);
void r_bike_kicked(void);
void r_bike_kicked_done(void);
void r_summon(void);
void r_tusk_blur(void);
void r_tusk_zap(void);
void r_lk_zap(void);
void r_skull(void);
void r_sg_zap(void);
void r_mileena_zap(void);
void zap_stumble(void);
#define r_ermac_zap zap_stumble
void update_his_hit_queue(WORD pa7);


inline void cc_hi_punch(void);
inline void cc_lo_punch(void);
void ccp3(WORD,WORD);
void cc_block_sweep(void);
inline void cc_block_avoid_corner(void);
void b_boss_hit1(void);
inline void cc_ken_masters(void);
inline void cc_block_upcut(void);
#define cc_duck_kickh cc_ken_masters
#define cc_block_weak cc_ken_masters
void ken_masters_xfer(void);
void reaction_start(WORD,FUNC,WORD,FUNC);
void r_airborn_duck_kick(void);
void generic_airborn_hit(void);
void combo_airborn_hit(void);
void airborn_hit_no_sound(void);
void r_kano_zap(void);
void r_rocket(void);
#define r_swat_bomb r_rocket
void r_sonya_zap(void);
void r_ind_zap(void);
#define r_lia_zap r_ind_zap
#define r_sw_zap r_ind_zap
void r_quake(void);
void r_post_shake(void);
void r_post_bike(void);
void r_jade_prop(void);
void r_ind_charge(void);
void r_sk_charge(void);
void r_jax_zap(void);
void r_freeze(void);
void freeze_wakeup(void);
void r_hat(void);
void stumble_back_vel(void);
void reaction_start_chores(void);
void update_hit_queue(void);
void b_uppercut(void);
void b_hard_ken_masters(void);
void block2(FUNC);
void b_knee_hit(void);
void b_hard(void);
#define b_scream b_hard
void b_hard_silent(void);
void b_weak_no_masters(void);
void b_weak(void);
void b_weak_silent(void);
void b_knee_elbow(void);
void b_combo(void);
void b_combo_hard(void);
void b_duck_hit_soft(void);
void b_duck_hit_hard(void);
void b_lo_punch(void);
void b_punch(void);
void cc_punch(void);
void avoid_corner_trap_b(WORD pa1);
void b_sweep(void);
void block_shake_n_exit(WORD,WORD);
void block_shake_ani(WORD pa0);
void block_exit(void);
inline WORD get_block_ani_off(void);
void block_shake(WORD,WORD,WORD);
void reaction_land(void);
void land_on_my_back(void);
void gup2(WORD);
void repell_one_of_us(long,long,JUMPTBL);
void check_stay_down(void);
inline void shake_n_sound(void);
void reverse_ani(WORD,WORD);
void shake_on_my_back(WORD,WORD);
void back_to_the_fight(void);
void fall_on_my_back(void);
void move_slave_too(void);
void suspend_wait_action(void);
void suspend_wait_action_jsrp(void);
WORD *get_my_hitq(OBJECT *pa8);


extern JUMPTBL block_xfers[];
extern JUMPTBL reaction_tables[];

/*
 *	MACROS
 */
#define sweepup_reaction_exit gup2(ANIM_SWEEPUP)
#define sweepup_local_reaction_exit gup2(ANIM_SWEEPUP)
#define getup_local_reaction_exit gup2(ANIM_GETUP)
#define getup_reaction_exit gup2(ANIM_GETUP)

/* stumble back routine, set a0 to correct vel */
#define stumble_back { \
			((long)current_proc->a0=SCX(0x30000));	\
			stumble_back_vel(); \
			}

/* tall_or_short_ani routine */
#define tall_or_short_ani(_std,_duck) ((am_i_short()==SYSTEM_CARRY_SET) ? (_duck):(_std))

/* inc_p_block */
#define inc_p_block current_proc->pdata.p_block++;

#endif /* __mk_mkreact_h__ */

