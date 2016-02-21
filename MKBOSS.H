/******************************************************************************
 File: mkboss.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III boss Header
******************************************************************************/

#ifndef __mk_mkboss_h__
#define __mk_mkboss_h__

/*
 * 	DEFINITIONS
 */

/* shao kahn animations */
#define ANIM_SK_DIE		0x6
#define ANIM_SK_LAUGH 	0xb
#define ANIM_SK_PUNCH	0xe
#define ANIM_SK_KICK	0x11
#define ANIM_SK_AIR_CHARGE 0x14
#define ANIM_SK_PROJECT	0x17
#define ANIM_SK_TAUNT	0x18
#define ANIM_SK_CHARGE	0x19
#define ANIM_SK_HAMMER 	0x1a
#define ANIM_SK_HIT_HI	0x1c
#define ANIM_SK_HIT_LO	0x1d
#define ANIM_SK_ZAP 	0x24
#define ANIM_SK_DEATHA1	0x26
#define ANIM_SK_DEATHB1	0x27
#define ANIM_SK_DEATHC1	0x28
#define ANIM_SK_DEATHD1	0x29
#define ANIM_SK_DEATHE1	0x2a
#define ANIM_SK_DEATHF1	0x2b
#define ANIM_SK_DEATHG1	0x2c

/* motaro animations */
#define ANIM_MOT_GRAB 		0x04
#define ANIM_MOT_MIST		0x05
#define ANIM_MOT_ZAP		0x06
#define ANIM_MOT_PUNCH 0x0e
#define ANIM_MOT_KICK 0x11
#define ANIM_MOT_HOP 0x1a
#define ANIM_MOT_HIT 0x1c

/* distancees */
#define D_VFAR	SCX(0x100)
#define D_FAR	SCX(0xe0)
#define D_MED	SCX(0x90)
#define D_CLOSE	SCX(0x60)

typedef struct boss_ainfo
{
	WORD b_ani;			// animation
	WORD b_stk;			// strike
	WORD b_speed;		// speed
} BOSS_AINFO;

/*
 *	RAM
 */

/*
 *	PROTOTYPES
 */
extern JUMPTBL motaro_branches[];
#define boss_branches motaro_branches

extern JUMPTBL *sk_catagory_table[];
extern JUMPTBL sk_branches[];

void boss_stalk(void);
void boss1(void);
void motaro_far_med(void);
void motaro_far_hard(void);
void motaro_far_easy(void);

void boss_close(void);
void boss_wait_land(void);
void boss_close_attack(void);
#define boss_finish_him boss_close_attack

void motaro_zap(void);
void motaro_tele(void);

void sk_stalk(void);
void sk_close(void);
void sk_far(void);
void sk_medium(void);
void sk_laugh(void);
void sk_stupid_stance(void);
void motaro_stupid_stance(void);
void ss1(void);
void sk_zap(void);
void sk_hammer(void);
void sk_kick(void);
void sk_punch(void);
void sk_charge(void);
void kchg8(void);
void sk_air_charge(void);
void achg8(void);
void sk_stupid(void);
void sk_taunt(void);

void motaro_sweep(void);
void boss_counter_angle(void);
void boss_ease_back(void);

void sk_stance_pause(void);

void motaro_hop(void);
void mhop7(long pa0,long pa1,short pa9);

void motaro_hip_jump(void);
void motaro_hip_jsrp(void);

void motaro_kick(void);

void motaro_grab_punch(void);
void motaro_grab_punch_now(void);
void motaro_punch(void);
void boss_close_miss(void);
#define boss_post_hit boss_close_miss
void grab_ani(short pa0,short pa1);

void skc_propell(void);
void skc_lk_zap_lo(void);
void skc_zap(void);
void sk_block_zap(void);
void skc_stationary(void);
void skc_swat_gun(void);
void skc_sg_pounce_sd(void);
void skc_dizzy(void);
WORD qb_is_he_dizzy(void);
WORD q_boss_stupid(void);
WORD sk_counter_joke(void);
WORD sk_randper(void);
WORD sk_counter_randper(void);
WORD q_is_this_a_joke(void);
WORD q_am_i_losing(void);
WORD q_am_i_way_ahead(void);
WORD q_am_i_stronger(void);

void mc_fk_sd(void);
void mc_dizzy(void);
void mc_sg_pounce_sd(void);
void cb_robo_tele_sd(void);
#define cb_mil_tele_sd cb_robo_tele_sd
#define cb_zoom_sd cb_robo_tele_sd
WORD q_heading_down(void);
void mc_flipkp(void);
void mc_angle_jump(void);
void mc_sg_pounce(void);
void mc_propell_ls(void);
void mc_hover(void);
void mc_stationary(void);
WORD motaro_randper(void);
void b_block(void);

void motaro_flip_kicked(void);
void motaro_hit_flight(void);
void motaro_upcutted(void);
void motaro_hit3(void);
#define motaro_hit2 motaro_hit3

void motaro_hit1(void);
#define motaro_hit0 motaro_hit1
void motaro_comboed(void);
void motaro_hard_comboed(void);
void motaro_stumble(void);
void motaro_collapse(void);
WORD q_ok_motaro_sweep(void);

void sk_collapse(void);
void sk_uppcutted(void);
void sk_hard_comboed(void);
void sk_stumble(void);
void sk_hit1(void);
void sk_hit3(void);
void sk_knocked_down(void);
#define sk_slided sk_knocked_down
void motaro_slided(void);
void sk_comboed(void);
void sk_airborn_check(void);

WORD q_is_he_car(void);





extern JUMPTBL *motaro_catagory_table[];

/*
 *	MACROS
 */
#define motaro_easy_randper() randper(350)

#define b_wait_forever wait_forever

#define b_reaction_exit d_reaction_exit

#define b_return_to_beware return_to_beware

#define b_return_to_beware_4get \
		{ \
		current_proc->pdata.p_otheract=0; \
		b_return_to_beware; \
		}

#define b_get_x_dist get_x_dist

#define b_check_winner_status check_winner_status

#define b_next_anirate d_next_anirate

#define b_beware d_beware

#define b_retp return

#define b_ochar_sound ochar_sound

#define get_mhe_word ladderorder_a1()
#define get_mhe_long ladderorder_a1()

#define skrp(_pa0) return(randper(_pa0))
#define bs_losing() skrp(75)

#endif /* __mk_mkboss_h__ */

