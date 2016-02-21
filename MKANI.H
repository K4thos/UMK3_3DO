/******************************************************************************
 File: mkani.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Animation routine header
******************************************************************************/

#ifndef __mk_mkani_h__
#define __mk_mkani_h__

/*
 * 	DEFINITIONS
 */

/* single animate indexs */
#define SINGLE_ANI0 0
#define SINGLE_ANI	1

/* which animation table to use */
#define ANIM_TABLE1 0						/* animation table 1, basic moves */
#define ANIM_TABLE2 1						/* animation table 2, special moves */

/* animation commands */
#define ANI_END			0					// animation script "end" command
#define ANI_JUMP		1					// animation script "jump" command
#define ANI_FLIP		2					// animation script "flip" command
#define ANI_ADJUSTX		3					// animation script "adjust x" command
#define ANI_ADJUSTXY	4					// animation script "adjust xy" command
#define ANI_NOSLEEP		5					// animation script "skip sleep" command
#define ANI_CALLA		6					// animation script call routine (next long)
#define ANI_SOUND		7					// animation script make a sound call
#define ANI_OCHAR_JUMP	8					// animation script ochar conditional jump
#define ANI_FLIP_V		9					// animation script vertical flip
#define ANI_OFFSET_XY	10					// animation script offset xy
#define ANI_SLADD		11					// animation script add slave
#define ANI_SLANI		12					// animation script slave animate
#define ANI_SWPAL		13					// animation script switch to next img pal
#define ANI_SLANI_SLEEP 14					// animation script slave animate sleep
#define ANI_OCHAR_SOUND 15
#define ANI_LASTCOM		15					// last animation command

#define NEVER_ANI 0x0fff					// never animate

/*
 *	RAM
 */

/*
 *	PROTOTYPES
 */
void find_ani2_part2(short);
void find_ani_part2(short);
inline void find_part2(void);
void find_part_a14(WORD);
void find_last_frame(void);
void animate2(OBJECT *,ADDRESS *);
inline void do_first_a9_frame(short);
WORD do_next_a9_frame(OBJECT *);
void animate_a9(WORD,WORD);
void animate2_a9(WORD,WORD);
void pose_a9(WORD,WORD);
void pose_him_a9(WORD,WORD);
void pose2_a9(WORD,WORD);
void pose_him_a0(WORD);
void double_mframew(WORD);
void mframew(WORD);
void mframew_him(WORD);
void ground_multi(OBJECT *obj);
void multi_adjust_xy(OBJECT *,short,short);
void match_ani_points(OBJECT *,OBJECT *);
void backwards_ani(WORD,WORD);
void backwards_ani2(WORD,WORD);
void bani2(OBJECT *,WORD);
void animate_a0_frames(WORD,WORD);
void init_anirate(WORD);
void next_anirate(void);
WORD frame_a9(OBJECT *);
void flip_multi(OBJECT *);
void flip_multi_v(OBJECT *);
WORD double_next_a9(void);
int highest_mpart(OBJECT *);
int lowest_mpart(OBJECT *);
int leftmost_mpart(OBJECT *);
int rightmost_mpart(OBJECT *);
void flip_single(OBJECT *);
void ganiof(OIMGTBL *,WORD);
void animate(WORD,OIMGTBL *,WORD);
void framew(WORD);

void stsw_sound(void);
void st_tumble_sound(void);
void morph_swpal(void);
void motaro_leg_stance(void);
void camp_fire_sound(void);

/*
 *	MACROS
 */

/* set animation and execute find_part_a14 */
#define find_ani_part_a14(_animnum,_acnt) \
		(get_char_ani(ANIM_TABLE1,(_animnum))),(find_part_a14((_acnt)))

#define find_ani2_part_a14(_animnum,_acnt) \
		(get_char_ani(ANIM_TABLE2,(_animnum))),(find_part_a14((_acnt)))

/* set animation and execute find_last_frame */
#define find_ani_last_frame(_animnum) \
		(get_char_ani(ANIM_TABLE1,(_animnum))),(find_last_frame())

/* act_mframew */
#define act_mframew(_act,_sleep) \
		current_proc->pdata.p_action=_act, \
		mframew(_sleep)

/* do_his_next_a9_frame */
#define do_his_next_a9_frame(_obj) do_next_a9_frame(_obj)

/* adjust_xy_a5 */
#define adjust_xy_a5(_a10,_x,_y) multi_adjust_xy((_a10),(_x),(_y))

#define ground_player(_obj) (_obj)->oypos.u.intpos=current_proc->pdata.p_ganiy

/* lineup a0 onto a1 */
#define lineup_a0_onto_a1(_la0,_la1) match_ani_points((_la1),(_la0))

#endif /* __mk_mkani_h__ */

