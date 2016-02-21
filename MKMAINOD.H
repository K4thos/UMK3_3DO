/******************************************************************************
 File: mkmain.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Main Header
******************************************************************************/

#ifndef __mk_mkmain_h__
#define __mk_mkmain_h__

/*
 * 	DEFINITIONS
 */
#define FULL_STRENGTH	0xa6				/* power bars at max */

/* pause flags */
#define PAUSE_CLEAR 	0x00
#define PAUSE_P1_THAW	0x01
#define PAUSE_P2_THAW	0x02
#define PAUSE_OFF		0x03				// pause is off but waiting for player to clear for debounce
#define PAUSE_P1_SET	0x03
#define PAUSE_P2_SET	0x04
#define PAUSE_P1_INIT	0x05
#define PAUSE_P2_INIT	0x06

/* clock stuff */
#define CLOCK_SPEED 11						/* # of ticks before dec clock counters */
#define CLOCK_INIT_DIGIT 9					/* clock digit init */

/* round status */
#define ROUND_P1_WON 0						/* player 1 won round */
#define ROUND_P2_WON 1						/* player 2 won round */
#define ROUND_P1_P2_TIE 2					/* player 1 tied player 2 */
#define ROUND_TIMEOUT 3						/* ran out of time */

/* adjustment value offsets */
#define ADJCCONT 12							/* # of coins to continue */

/* briefcase code structure */
typedef struct bcc
{
	LONG b_code;
	JUMPTBL b_func;
} BCC;

/*
 *	RAM
 */
__EXTERN__ WORD f_play3;					// set when at PLAY3 label
__EXTERN__ FUNC play3_pa14;				// if set then return to this point instead of continue (nasty hack for C)
__EXTERN__ WORD round_results;
__EXTERN__ volatile WORD f_sync;						// semaphore type var
__EXTERN__ WORD f_victory_start;

/*
 *	PROTOTYPES
 */
void start_entry(WORD *,WORD);
void start_a_game(WORD *pstate,WORD poffset);
void game_loop(void);
void round_loop(void);
void game_init(void);
void endurance_round(void);
void set_winner_status(void);
WORD is_endurance_possible(WORD pa11,WORD *pa0);
void game_over(void);
inline void init_player_variables(void);
void master_proc_mercy(void);
inline WORD count_active_players(void);
WORD set_drone_ochar(void);
WORD *drone_char_point_a6(void);
WORD current_drone_a0(void);
WORD who_is_alone(void);
WORD play_1_round(void);
void play3(void);
WORD master_mercy_entry(void);
void restore_power(WORD *pa5,WORD *pa6);
void mercy_start(void);
void results_of_round(WORD);
void match_init(void);
void round_init(void);
void round_intro_fx(void);
void start_tune(void);
void try_briefcase(void);
void clear_combo_ram(void);
void print_round_num(void);
void zero_round_init_words(void);
void zero_round_init_longs(void);
void inc_winners_wiar(void);
WORD fatality_wait(WORD);
WORD is_finish_him_allowed(void);
void finish_him_sequence(void);
void loser_in_buyin(void);
void print_timeout_msg(void);
void adv_winner_map(void);
void end_of_match_chores(void);
void amode_demo_game(void);
void add_to_diff(short);
void diff_adjust(void);
WORD ladderorder_a1(void);
void boss_minimum_diff(void);
void check_battle_milestone(void);
void weed_out_cheaters(void);
void flash_pmsg(void);
void flash_pmsg2(void);
void finish_him_or_her(void);
void check_enter_initials1(void);
void check_enter_initials2(void);
void barge_in_message(void);
void advance_curback(void);
short get_adj(WORD);
short get_adj_diff(void);
void reset_map_variables(void);

void player_heap_adj(void);

void pause_setup(WORD poffset);
void pause_hold(WORD poffset);
WORD get_background_tune(void);
void play_background_tune(void);
void play_finish_tune(void);
void play_ending_chord(void);

void bc_real_kombat(void);
void bc_super_endur(void);
void bc_bar_regenerate(void);
void bc_p1p2_half_damage(void);
void bc_p2_half_damage(void);
void bc_p1_half_damage(void);
void bc_no_special_moves(void);
void bc_superjumps(void);
void bc_no_combos(void);
void bc_wf_smoke(void);
void bc_intro_message(void);
void bc_psycho_kombat2(void);
void bc_unlim_run(void);
void bc_uppercut_recovery(void);
void bc_no_powerbars(void);
void bc_nofear_clue(void);
void bc_wf_noob(void);
void bc_wf_sk(void);
void bc_wf_motaro(void);
void bc_space_game(void);
void bc_p2_supercap(void);
void bc_p1_supercap(void);
void bc_jackbot(void);
void bc_switcheroo(void);
void bc_dark_fighting(void);
void bc_p2_handicap(void);
void bc_p1_handicap(void);
void bc_blocking_disabled(void);
void bc_throwing_disabled(void);
void stuff_round_123(WORD pa0);

void hidden_game_load(void);

/*
 *	MACROS
 */
#define SHOW_SCORES	f_doscore=1
#define DONT_SHOW_SCORES f_doscore=0

#define corpse_a9_proc(_proc) (_proc)->pdata.p_flags|=PM_CORPSE

#endif /* __mk_mkmain_h__ */

