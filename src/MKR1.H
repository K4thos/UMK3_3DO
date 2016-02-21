/******************************************************************************
 File: mkr1.h

 By: David Schwartz

 Date: Mar 1995

 (C) Williams Entertainment

 Mortal Kombat III Round Intro Header
******************************************************************************/

#ifndef __mk_mkr1_h__
#define __mk_mkr1_h__

/*
 * 	DEFINITIONS
 */

/*
 *	RAM
 */

/*
 *	PROTOTYPES
 */
void do_round_jsrps(void);
void psycho_score_onoff(void);
WORD pso3(void);
void wf3(char *pa8,FUNC pa14);
void p1_supercap_proc(void);
void p2_supercap_proc(void);
void p1_hadicap_proc(void);
void p2_hadicap_proc(void);
void p1_nopower_proc(void);
void p2_nopower_proc(void);
void hand2(WORD pa11,WORD pa10);
PROCESS *wait_for_charge(void);
void msg1(char *msg);
void msg_round1(void);
void switcheroo_proc(void);
void switcheroo_check(ADDRESS *pa11);
void dark_fight_proc(void);
void dark_sleep(void);
void lights_out(void);
void lights_on(void);
void dark_velocities(void);
void lights_on_hit(void);
void lights_on_slam(void);
void space_game(void);
void post_wf_smoke(void);
void post_wf_noob(void);
void post_wf_shao(void);
void post_wf_motaro(void);
void postw2(WORD pa0);
void special_game_init(void);
void regenerate_proc(void);
void r_shadow_fighting(void);
void r_force_curback(char *pa8, WORD pa11);








/*
 *	MACROS
 */

#endif /* __mk_mkr1_h__ */

