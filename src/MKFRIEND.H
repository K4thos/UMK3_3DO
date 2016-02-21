/******************************************************************************
 File: mkfriend.h

 By: David Schwartz

 Date: Jan 1995

 (C) Williams Entertainment

 Mortal Kombat III friendship header
******************************************************************************/

#ifndef __mk_mkfriend_h__
#define __mk_mkfriend_h__

/*
 * 	DEFINITIONS
 */


/*
 *	RAM
 */

/*
 *	PROTOTYPES
 */
void do_friendship(void);
void friendship_start_pause(void);
void friendship_complete(void);

void fr_mileena(void);

void f_null_friendship(void);

void f_scorpion(void);
void f_reptile(void);
void pop_up_my_toy(void);
void jax_n_box_start(void);

void f_jade(void);
void friend_ender(void);

void f_kitana(void);
void spawn_bubble(void);
void bubble_proc(void);
void bub_1_proc(void);

void f_kano(void);
void f_sonya(void);
void sonya_flower_proc(void);
void f_jax(void);
void end_friend_proc(void);

void f_indian(void);
void other_ochar_sound(WORD pa0,WORD pa1);

void f_sz(void);

void f_swat(void);
void swat_friend_proc(void);
void swat_crossing_people(void);

void f_sindel(void);
void football_proc(void);

void f_sektor(void);
void dinger_proc(void);

void f_cyrax(void);

void f_lao(void);
void lao_dog_sounds(void);
void cute_lil_doggy(void);
void hat_proc(void);

void f_kabal(void);
void f_sheeva(void);

void f_shang(void);
void bounce(void);

void f_liu(void);
void wall_dragon_proc(void);

void f_smoke_ninja(void);

/*
 *	MACROS
 */
#define mframew_5 mframew(5)
#define mframew_4 mframew(4)
#define mframew_3 mframew(3)

/* notes about new friendships 

MILEENA 
a_mirror_mirror -> ANIM_F1_FRIEND (0x1a)
tsound(1);
tsound(0x66);

SCORPION
a_skull_in_da_box -> ANIM_F1_FRIEND  (done)
a_crank_box	-> ANIM_F2_FRIEND (done)
tsound(0x92)

REPTILE
a_snake_in_da_box -> ANIM_F1_FRIEND (done)
a_crank_box	-> ANIM_F2_FRIEND (done)
tsound(0x92)

JADE
a_pogo_stick ->ANIM_F1_FRIEND
tsound(0xc)


KITANA
a_kitana_bubbles ->ANIM_F1_FRIEND
a_kitana_kiss -> ANIM_F2_FRIEND


SONYA
a_flower -> ANIM_F1_FRIEND
*/

#endif /* __mk_mkfriend_h__ */

