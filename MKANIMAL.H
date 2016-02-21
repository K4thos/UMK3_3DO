/******************************************************************************
 File: mkanimal.h

 By: David Schwartz

 Date: Jan 1995

 (C) Williams Entertainment

 Mortal Kombat III animalship header
******************************************************************************/

#ifndef __mk_mkanimal_h__
#define __mk_mkanimal_h__

/*
 * 	DEFINITIONS
 */


/*
 *	RAM
 */

/*
 *	PROTOTYPES
 */
void do_animality(void);
void animality_start_pause(void);

void cute_animality_start(void);

void null_animal(void);
void mileena_skunk(void);
void odor_proc(void);
void stink_cloud(void);

void scorpion_pengo(void);
void egg_proc(void);
void pengo_animate(void);
void r_egg(void);

void reptile_monkey(void);
void set_vel_flip(long pa0);
void r_scared_of_monkey(void);

void jade_kitty(void);
void kitty_spin(void);

void kitana_bunny(void);
void r_rabbit(void);
void crunch_sounds(void);

void cyrax_shark(void);
void eaten_by_shark(void);
void eaten_by_snake(void);

void lao_cheetah(void);

void liu_kang_dragon(void);

void shang_tsung_snake(void);

void sheeva_scorpion(void);
void stung_by_scorpion(void);

void kabal_skeleton(void);
void dino_bucked(void);

void sektor_bat(void);
WORD q_bat_4(void);
WORD q_bat_3(void);
WORD q_bat_2(void);
WORD q_bat_1(void);
void r_bat_bite(void);
#define r_kitana_decap r_bat_bite
void head_pop_off(long pa0,long pa1);
void cutup_body_init(void);
void r_ermac_upcut(void);
void animate_till_a11_stop(FUNCW pa11);
void animate_till_a11(FUNCW pa11);

void smoke_bull_shit(void);
void hit_by_bull(void);

void sindel_wasp(void);
void stung_a_bunch(void);

void swat_dino(void);
void bit_in_half(void);

void sz_polar(void);

void kano_spider(void);
void spider_shake(void);
void spider_shake_jsrp(void);

void sonya_eagle(void);

void jax_lion(void);
void lion_mauled(void);

void indian_wolf(void);

void next_anirate_a10(WORD pa10);
PROCESS *create_fx_for_him(WORD pa0);

void unmorph_and_exit(void);
void animal_morph(WORD pa10);
void animality_tune(void);
void animality_complete(void);

void face_him_at_me(void);
void center_around_him(void);

void kill_and_stop_scrolling(void);
void ground_a8(void);

/*
 *	MACROS
 */
#define kill_repell dallprc(PID_REPELL)

/*
	 ANIMAL NOTES 
ALL NEED FLOOR_EXPLODE IN PLACE FOR FX_INVISO_POOF
ALL USE tsound(0x8c)



MILEENA: (DONE)
a_skunk	-> ANIM_FT1_ANIMAL

SCORPION: (DONE)
a_egg -> ANIM_FT1_ANIMAL
a_pengo -> ANIM_FT2_ANIMAL
animation file setup like swat bomb fatality for blood and guts

REPTILE
a_monkey -> ANIM_FT1_ANIMAL (DONE)

JADE 
a_kitty -> ANIM_FT1_ANIMAL
uses hair spun routine will need to emulate
tsound(0x92)

KITANA (DONE)
a_bunny -> ANIM_FT1_ANIMAL
tsound(0x24)
tsound(0x25)
tsound(0x92)

CYRAX
tsound(0x92)

LAO
tsound(0x95)

LIU KANG
tsound(0x92)

SWAT 
tsound(0x92)

SZ POLAR
INDIAN
tsound(0x95)
*/

#endif /* __mk_mkanimal_h__ */

