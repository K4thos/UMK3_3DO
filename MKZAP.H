/******************************************************************************
 File: mkzap.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III BODY zapelled Moves
******************************************************************************/

#ifndef __mk_mkzap_h__
#define __mk_mkzap_h__

/* 
 * 	DEFINITIONS
 */

/* zap offsets */
#define ZAP_KANO_ZAP  	0x00				// kano zap 
#define ZAP_SONYA_ZAP 	0x01				// sonya zap
#define ZAP_JAX_ZAP1  	0x02				// jax zap 1
#define ZAP_JAX_ZAP2  	0x03				// jax zap 2
#define ZAP_IND_ZAP	  	0x04				// indian zap
#define ZAP_SZ_ION	  	0x05				// subzero ice on
#define ZAP_SZ_IBEHIND	0x06				// subzero ice behind
#define ZAP_SZ_IFRONT  	0x07				// subzero ice front
#define ZAP_SW_ZAP		0x08				// swat zap
#define ZAP_ROBO_ZAP	0x09				// robo zap
#define ZAP_ROBO_ZAP2	0x0a				// robo zap2
#define ZAP_ROBO_NET	0x0b				// robo net
#define ZAP_SZ_ZAP		0x0c				// sz ice zap
#define ZAP_LIA_ANGLEZ	0x0d				// lia anglez
#define ZAP_LAO_ZAP		0x0e				// kung lao zap
#define ZAP_ROBO_BOMBF	0x0f				// robo bomb full
#define ZAP_ROBO_BOMBM	0x10				// robo bomb mid
#define ZAP_TUSK_ZAP	0x11				// tusk raider zap
#define ZAP_ST_SUMMON	0x12				// shang tsu summons
#define ZAP_ST_ZAP1		0x13				// shang tsu zap 1
#define ZAP_ST_ZAP2		0x14				// shang tsu zap 2
#define ZAP_ST_ZAP3		0x15				// shang tsu zap 3
#define ZAP_LK_HI		0x16				// lk zap hi
#define ZAP_LK_LO		0x17				// lk zap lo
#define ZAP_SG_ZAP		0x18				// she goro
#define ZAP_SWAT_BOMBH	0x19				// swat bomb hi
#define ZAP_SWAT_BOMBL	0x1a				// swat bomb lo
#define ZAP_LIA_FZAP	0x1b				// lia forward spit
#define ZAP_TUSK_FLOOR	0x1c				// kabal saw blade
#define ZAP_SK_ZAP		0x1d				// shao kahn zap
#define ZAP_SMOKE_SPEAR	0x1e				// smokes spear
#define ZAP_MOTARO_ZAP	0x1f				// motaro zap
#define ZAP_KIT_ZAP		0x20
#define ZAP_JADE_ZAPM	0x21
#define ZAP_REP_ORB		0x22
#define ZAP_REP_SPIT	0x23
#define ZAP_SCORP_SPEAR	0x24
#define ZAP_JADE_ZAPH	0x25
#define ZAP_JADE_ZAPL	0x26
#define ZAP_JADE_ZAPR	0x27
#define ZAP_REP_ORBF	0x28
#define ZAP_MIL_ZAP		0x29
#define ZAP_OSZ_ZAP		0x2a
#define ZAP_FLOOR_ICE	0x2b
#define ZAP_ERMAC_ZAP	0x2c

/* zap init index */
#define ZINIT_AIR		0x00
#define ZINIT_SPEC_ACT	0x01
#define ZINIT_SPEC		0x02

typedef struct r_rocket { 
	FUNC r_routine;
	short r_grav;
	WORD r_accelcnt;
} RROCKET;

/* 
 *	RAM 
 */

/*
 *	PROTOTYPES 
 */
void do_proj_ermac_zap(void);
WORD q_is_he_react_fk(void);
void ermac_zap_proc(void);

void do_proj_floor_ice(void);
void floor_ice_proc(void);
void floor_ice_hit(void);
void ice_melt(void);

void do_proj_osz_zap(void); 

void do_proj_mileena_zap(void); 
void air_sai_proc(void);
void sai_proc(void);
void sai3(short pa0,short pa1);

void do_proj_smoke_spear(void);
void do_proj_scorpion_spear(void);
void ssp2(FUNC pa7);
void smoke_spear_proc(void); 
void scorpion_spear_proc(void);
void ssp3(short pa0,short pa1);
void sspear_hit(void);
void sspear_cancelled(void);
void sspear_blocked(void);
void stuff_rope_flag2(WORD pa0);
void scorp_rope_pull(void);
void pull_clip(void);
void clip_scorpion_rope(short pa3,short pa4);
void lineup_rope_with_him(void);
void get_single_spear_obj(void);
void double_shaker(short pa1);
void tugged_in_by_spear(void);


void do_proj_spit(void);
void spit_proc(void);
void spit_prezap_hit(void);
void spit_prezap(short pa1x,short pa1y,short pa2x,short pa2y);

void do_proj_reptile_orb_fast(void);
void do_proj_reptile_orb(void);
void orb3(long pa11);
void orb_calla(void);
void orb_proc(void);

void do_proj_jade_zap_ret(void);
void do_proj_jade_zap_lo(void);
void do_proj_jade_zap_hi(void);
void do_proj_jade_zap_med(void);
void jzap3(long pa0);
void boomerang_proc(void);
void boomerang_trail(void);
void boomerang_call(void);
void boom_trail_proc(void);

void do_proj_kitana_zap(void);
void fan_proc(void);

void do_proj_motaro_zap(void);
void motaro_zap_proc(void);
void mot_zap_call(void);



void do_proj_sk_zap(void);
void sk_zap_proc(void);

void do_proj_tusk_floor(void);
void blade_proc(void);
void saw_strike_check(void);

void do_proj_lia_forward_zap(void);
void lia_forward_proc(void);

void do_proj_sz_zap(void);
void osz_forward_entry(void);
void sz_post_zap(void);
void ice_collision_check(short pa0x,short pa0y,short pa1x,short pa1y);
void sz_zap_proc(void);
void sz_zap_hit(void);

void do_proj_sonya_zap(void);
void sonya_zap_proc(void);

void do_proj_lao_zap(void);
void lao_hat_proc(void);
void lao_zap_call(void);
void hat_ring_proc(void);

void do_lk_zap_air(void);
void do_proj_lk_zap_lo(void);
void do_proj_lk_zap_hi(void);
void lk_zap_entry(void);
WORD lk_prezap(short pa1x,short pa1y,short pa2x,short pa2y);
void make_dragon_explode(void);
void lk_zap_proc(void);
void lk_prezap_hit(void);


void do_proj_st_zap1(void);
void do_proj_st_zap2(void);
void do_proj_st_zap3(void);
void stz1(WORD pa1,WORD pa11);
void st_zap_jsrp(WORD pa10);
void skull_proc(void);

void do_proj_summon(void);
inline void stuff_others_a0(PROCESS *pa0);
void master_summon_proc(void);
void summon_spawn(void);
void summon_proc(void);
void summon_flame_animator(void);

void do_proj_tusk_zap(void);
void photon_proc(void);

void do_proj_jax_zap2(void);
void do_proj_jax_zap1(void);
void jax_zap_jsrp(OBJECT *);
void jax_zap_proc(void);
void jax_zap_hit(OBJECT *);
void jax_proj_calla(void);

void do_proj_kano_zap(void);
void kano_zap_proc(void);

void do_proj_ind_zap(void);
void ind_zap_proc(void);

void do_proj_lia_anglez(void);
void angle_zap_proc(void);
void angle_zap_call(void);
void angle_zap_explode(OBJECT *);
void angle_zap_jsrp(OBJECT *,WORD);
void angle_zap_hit(OBJECT *obj);

void do_proj_sw_zap(void);
void swat_proj_proc(void);
void sw_proj_hit(OBJECT *);

void do_proj_swat_bomb_hi(void);
void do_proj_swat_bomb_lo(void);
void bomb33(void);
void swat_bomb_proc(void);
void bomb_call(void);
void make_lineup_explode(OBJECT *pa8,short pa11x,short pa11y);

void do_proj_sg_zap(void);
void sg_zap_proc(void);
void sg_trail_spawn(void);
void sg_zap_trail(void);

void do_proj_sky_ice_behind(void);
void do_proj_sky_ice_front(void);
void do_proj_sky_ice_on(void);
void doice5();
void sky_ice_proc(void);

void do_proj_robo_net(void);
void net_proc(void);
void robo_open_chest(OBJECT *);
void robo_close_chest(OBJECT *);
void robo_open_chest_fast(OBJECT *obj);

void do_proj_robo_bomb(void);
void do_proj_robo_bomb_mid(void);
void bomb3(FUNC pa11);
void robo_bomb_mid(void);
void robo_bomb_full(void);
void rbomb4(long pa0);
long get_bomb_vel(void);

void robo_bomb_proc(void);
void bomb_gravity2(OBJECT *obj,long vel);
void bomb_gravity(OBJECT *obj);
void bgrav9(OBJECT *);

extern RROCKET rocket_routines[];
void do_proj_robo_zap2(void);
void do_proj_robo_zap(void);
void rzap3(FUNC);

void rocket2_proc(void);
void rocket_hunt(void);
void rocket_smoke(WORD);
void rr_up(void);
void rr_down(void);
void rr_nothing(void);
void update_target(void);
void rocket_explode(OBJECT *);
void rocket_explode_fx(OBJECT *);
void delete_target_obj(void);
WORD point_rocket(OBJECT *);

void rocket1_proc(void);
void rocket1_flight_call(void);

void do_zap(WORD);
OBJECT *get_proj_obj_m(OBJECT *);
void tell_world_stk(WORD pa0);
void setup_proj_obj(OBJECT *);
inline void i_am_a_sitting_duck(void);
void set_proj_vel(OBJECT *,long,WORD);
void delete_proj_and_die(OBJECT *);
void zap_init(WORD,WORD,WORD,OBJECT *);
void do_proj_sitting_duck(WORD);
PROCESS *create_proj_proc(FUNC);
WORD projectile_flight_call(OBJECT *,WORD);
void projectile_reflect(OBJECT *);
void benedict_arnold_projectile(OBJECT *);
WORD proj_strike_check(OBJECT *,WORD);
WORD proj_onscreen_test(OBJECT *);
WORD is_jade_protected(void);
short get_frontmost_point(OBJECT *);
void hit_proj_lineup_x(OBJECT *,short);
void hit_proj_lineup_xy(OBJECT *,short,short);
WORD is_he_motaro(void);
WORD q_his_react_flag_set(void);
WORD local_strike_check_box(WORD stk,short pa0x,short pa0y,short pa1x,short pa1y);

/*
 *	MACROS
 */
#define dead_projectile(_proc) ((_proc)->pdata.p_action=ACT_PROJ_DEAD)

#define projectile_flight(_proc,_obj,_stk) \
		((_proc)->a6=0),projectile_flight_call((_obj),(_stk))

#define p1p2_pid_stuff(_proc,_obj,_p1,_p2) (_proc)->procid=((_obj)->oid==OID_P2)?(_p2):(_p1)

#endif /* __mk_mkzap_h__ */

