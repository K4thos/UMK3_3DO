/******************************************************************************
 File: mkprop.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III BODY Propelled Moves
******************************************************************************/

#ifndef __mk_mkprop_h__
#define __mk_mkprop_h__

/* 
 * 	DEFINITIONS
 */

/* prop offsets */
#define PROP_KANO_CANNON	0x00			// kano cannonball
#define PROP_SONYA_BIKE		0x01			// sonya bike kick
#define PROP_IND_CHARGE		0x02			// indian charge
#define PROP_JAX_DASH		0x03			// jax dash punch
#define PROP_SZ_DECOY		0x04			// sub zero decoy
#define PROP_LIA_FLY		0x05			// lia fly 
#define PROP_LAO_TELE		0x06
#define PROP_LAO_ANGLE_KICK	0x07
#define PROP_ROBO_TELE		0x08
#define PROP_ROBO_AIR_GRAB	0x09
#define PROP_TELE_EXP		0x0a
#define PROP_LIA_AFLOAT		0x0b
#define PROP_SONYA_SWAVE	0x0c
#define PROP_LK_BIKE		0x0d
#define PROP_LK_SUPER		0x0e
#define PROP_SG_POUNCE		0x0f
#define PROP_SZ_SLIDE		0x10
#define PROP_SW_ZOOM		0x11
#define PROP_SW_STICK		0x12
#define PROP_TUSK_BLUR		0x13
#define PROP_SG_QUAKE		0x14
#define PROP_NJ_SLIDE		0x15
#define PROP_SCORP_TELE		0x16
#define PROP_REP_DASH		0x17
#define PROP_JADE_PROP		0x18
#define PROP_MIL_TELE		0x19
#define PROP_MIL_ROLL		0x1a
#define PROP_ERMAC_TELE		0x1b
#define PROP_KANO_UBALL		0x1c

#define LSPEED	(0x70000)				// robo limb explosion speed

/* 
 *	RAM 
 */

/*
 *	PROTOTYPES 
 */
void do_body_propell(WORD);

void prop_do_kano_upball(void);
void upball_x_damping(OBJECT *obj);

void prop_do_mileena_prop(void);
void mileena_prop_hit(void);
void post_mileena_prop(void);
void mileena_prop_blocked(void);

void prop_do_mileena_tele(void);
void mileena_teleport_call(void);

void prop_do_jade_prop(void);
void jade_prop_hit(void);
void jade_prop_damping(OBJECT *obj);

void prop_do_reptile_dash(void);
void reptile_dash_hit(void);

void prop_do_ermac_tele(void);
void prop_do_scorp_tele(void);
void sctele_scalla_1(void);
void sctele_scalla_2(void);
void redirect_scroller(FUNC p_ar7);

void new_float_ani(void);
void prop_jax_dash_punch(void);
void dash_hit(OBJECT *);
void dash_unblur(OBJECT *);

void prop_sonya_bike_kick(void);
void bike_hit(OBJECT *);
void bike_scan_call(void);
void bike_hit_call(OBJECT *);

void prop_do_lao_angle(void);
void lao_angle_scan(void);

void prop_do_lao_tele(void);

void prop_kano_cannon_ball(void);
void kroll_hit(OBJECT *);

void prop_ind_charge(void);
void icharge_hit(OBJECT *);

void prop_do_sz_decoy(void);
void decoy_proc(void);
void shake_collision_check(short);
WORD decoy_collision_check(void);
void decoy9(void);

void prop_do_tele_explode(void);
void robo_limb_in(void);
void robo_limb_out(void);
void teleport_next_to(void);
void cyrax_implode(void);

void prop_do_robo_air_grab(void);
void prop_do_robo_tele(void);
void tele_scan(void);
void tele_scan2(void);

void prop_do_lia_fly(void);
void prop_do_lia_stay_afloat(void);
void main_hover_loop(void);
void settle_down(OBJECT *);
void flight_move_ani(OBJECT *);
void accelerate_x(OBJECT *,long);
void hover_sleep_1(OBJECT *);
void set_float_ani(WORD);
void hover_land(OBJECT *);

void prop_do_sonya_square_wave(void);

void prop_do_slide(void);
#define prop_do_ninja_slide prop_do_slide
void slide_is_over(OBJECT *obj);

void prop_do_sg_quake(void);
void prop_do_sg_pounce(void);
void pounce_jsrp(long pa0,FUNC pa11);
void blur_catchup(OBJECT *obj);
void pounce_scan(void);
void pounce_adjust_him(void);
void pounce_ground_him(OBJECT *pa8);

void prop_do_super_kang(void);
void super_kick_land(OBJECT *obj);

void prop_do_lk_bike(void);
void bike_call(void);
void biked_suspend(void);

void prop_do_tusk_blur(void);
void tusk_blur_block(OBJECT *pa8);
void reptile_dash_blocked(OBJECT *pa8);
void blur_blocked_setup(void);


void prop_do_stick_sweep(void);

void prop_do_swat_zoom(void);
void zoom_hit(OBJECT *pa8);
void zoom_blocked(OBJECT *pa8);
void zoom_damping(OBJECT *pa8);
void zoom_ground_scan(OBJECT *pa8);
#define scorpion_tele_blocked zoom_blocked


/*
 *	MACROS
 */
#define local_ochar_sound(_off) ochar_sound(_off)

// #define no_action(_proc) (_proc)->pdata.p_action=0

#define set_his_p_action(_la0,_la1) (((_la0)->pdata.p_otherproc)->pdata.p_action=(_la1))

#endif /* __mk_mkprop_h__ */

