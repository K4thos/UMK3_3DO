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
void force_it(void);
void start_entry(WORD *,WORD);
void goto_mode_select(WORD *pstate,WORD poffset);
void post_mode_select(void);
void buyin_to_1_on_1(void);
void two_on_two(void);
WORD round_is_over(void);
void tournament_play(void);
void round_1_vs(void);
void game_loop(void);
void round_loop(void);
void one_on_one(void);
void game_init(void);
void set_winner_status(void);
void play_1_match(void);
void spawn_endurance_guy(WORD pa0);
WORD is_endurance_possible(void);
short a0_next_endurance_guy(void);
void fatal_demo_loop(void);
void game_over(void);
inline void init_player_variables(void);
void master_proc_mercy(void);
inline WORD count_active_players(void);
WORD set_drone_ochar(void);
WORD *drone_char_point_a6(void);
short current_drone_a0(void);
WORD who_is_alone(void);
WORD play_1_round(void);
void play3(void);
WORD master_mercy_entry(void);
#define continue_fighting master_mercy_entry
void restore_power(WORD *pa5,WORD *pa6);
void mercy_start(void);
void wait_for_wingman(WORD *pbar);
PROCESS *spawn_wingman(PROCESS *pa1,WORD pa5);
void results_of_round(WORD);
void match_init(void);
void round_init(void);
void round_intro_fx(void);
void start_tune(void);
void try_kombat_kodes(void);
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
void ochar_force_curback(void);
void adv_winner_map(void);
void end_of_match_chores(void);
void amode_demo_game(void);
void stuff_a0_diff(void);
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

void hidden_game_load(void);

/*
 *	MACROS
 */
#define SHOW_SCORES	f_doscore=1
#define DONT_SHOW_SCORES f_doscore=0

#define corpse_a9_proc(_proc) (_proc)->pdata.p_flags|=PM_CORPSE

#endif /* __mk_mkmain_h__ */

