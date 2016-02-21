/******************************************************************************
 File: mkslam.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Slam/Special Moves Routines
******************************************************************************/

#ifndef __mk_mkslam_h__
#define __mk_mkslam_h__

/* 
 * 	DEFINITIONS
 */


/* 
 *	RAM 
 */

/*
 *	PROTOTYPES 
 */
void do_air_slam(void);
void do_body_slam(void);

void common_slam(WORD);
void xfer_him_to_flipped_pause(WORD);
void flipped_pause(void);
void xfer_to_thrown(FUNC);
void grab_animation(WORD);
void body_slam_init(OBJECT *);
void air_slam_init(OBJECT *,WORD,WORD);
void stick_him_with_me(OBJECT *);

void jade_slam(void);
void mileena_slam(void);
void kitana_slam(void);

void scorp_air_slam(void);

void nj_smoke_slam(void);
#define noob_slam nj_smoke_slam
void nj_slam(void);
void njs13(void);


void smoke_air_slam(void);
void smoke_slam(void);
#define thrown_by_smoke thrown_by_tusk

void do_back_breaker(void);
void bb_fall_call(void);
void post_broken_back(void);
void last_knockdown_frame(void);


void kano_air_body_slam(void);
void kano_slam(void);
void thrown_by_kano(void);
void air_slamed_by_kano(void);

void sonya_slam(void);
void thrown_by_sonya(void);
#define thrown_by_mileena thrown_by_sonya
void thrown_by_nj(void);
void slam_damage(OBJECT *obj);

void lao_slam(void);
#define thrown_by_lao thrown_by_lia 

void jax_slam(void);
void jax_multi_slam_pose(WORD);
void jax_slam_sleep(WORD);
inline WORD read_lp_tick_state(OBJECT *);
void thrown_by_jax(void);

void indian_slam(void);
#define thrown_by_indian thrown_by_lia

void sz_slam(void);
void thrown_by_sz(void);

void sw_slam(void);
#define thrown_by_swat thrown_by_lia

void lia_slam(void);
void thrown_by_lia(void);

void robo2_air_slam(void);
void thrown_by_robo2(void);
#define thrown_by_robo thrown_by_lia
void robo2_slam(void);
void robo1_slam(void);
#define thrown_by_jade thrown_by_lia
#define thrown_by_kitana thrown_by_lia

void air_slamed_by_robo2(void);

void grab_voice(void);
void throw_voice(void);

void tusk_slam(void);
#define thrown_by_tusk thrown_by_lia

void lk_slam(void);
#define thrown_by_lk thrown_by_sonya

void sg_slam(void);
void thrown_by_sg(void);

void st_slam(void);

void ground_slammed_init(void);
void drop_down_land(void);
void drop_down_land_jump(void);


/*
 *	MACROS
 */

#endif /* __mk_mkslam_h__ */

