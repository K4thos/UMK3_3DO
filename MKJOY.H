/******************************************************************************
 File: mkjoy.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Joystick Headers
******************************************************************************/

#ifndef __mk_mkjoy_h__
#define __mk_mkjoy_h__

/*
 * 	DEFINITIONS
 */

/* jumpup kick/punch index */
#define JUP_PUNCH 0x00						// jump up punch setup
#define JUP_KICK 0x01						// jump up kick setup

/* joystick bits */
#define BIT_JLEFT		15
#define BIT_JRIGHT		13
#define BIT_JDOWN		14
#define BIT_JUP			12
#define BIT_JBLOCK		3
#define BIT_JRUN		2
#define BIT_JHIP		4
#define BIT_JLOP		7
#define BIT_JHIK		5
#define BIT_JLOK		4

/* joystick mask,if change update mkcombo.s */
#define MASK_JLEFT		0x8000
#define MASK_JRIGHT		0x2000
#define MASK_JDOWN		0x4000
#define MASK_JUP		0x1000
#define MASK_JBLOCK		0x0008
#define MASK_JRUN		0x0004
#define MASK_JHIP		0x0010
#define MASK_JLOP		0x0080
#define MASK_JHIK		0x0020
#define MASK_JLOK		0x0040
#define MASK_START		0x0800

#define P1_BUTTON_MASK	(MASK_JBLOCK+MASK_JRUN+MASK_JHIP+MASK_JLOP+MASK_JHIK+MASK_JLOK)
#define P2_BUTTON_MASK	(MASK_JBLOCK+MASK_JRUN+MASK_JHIP+MASK_JLOP+MASK_JHIK+MASK_JLOK)

#define P1B0 (MASK_JHIP)
#define P1B1 (MASK_JLOP)
#define P1B2 (MASK_JBLOCK)
#define P1B3 (MASK_JHIK)
#define P1B4 (MASK_JLOK)
#define P1B5 (MASK_JRUN)
#define P1SF2 0

#define P2B0 (MASK_JHIP)
#define P2B1 (MASK_JLOP)
#define P2B2 (MASK_JBLOCK)
#define P2B3 (MASK_JHIK)
#define P2B4 (MASK_JLOK)
#define P2B5 (MASK_JRUN)
#define P2SF2 0

#define M_P1ACTION	(P1B0|P1B1|P1B2|P1B3|P1B4|P1B5)
#define M_P2ACTION	(P2B0|P2B1|P2B2|P2B3|P2B4|P2B5)

#define BUTTON_BITS		M_P1ACTION
#define BUTTON_BITS2	M_P2ACTION
#define BB_NOSTART		(M_P1ACTION|M_P2ACTION)

#define M_PJOY	(MASK_JLEFT|MASK_JRIGHT|MASK_JUP|MASK_JDOWN)

#define P1P2_UP		(MASK_JUP|(MASK_JUP<<16))
#define P1P2_DOWN	(MASK_JDOWN|(MASK_JDOWN<<16))
#define P1P2_LEFT	(MASK_JLEFT|(MASK_JLEFT<<16))
#define P1P2_RIGHT	(MASK_JRIGHT|(MASK_JRIGHT<<16))
#define P1P2_ACTION (M_P1ACTION|(M_P2ACTION<<16))

#define M_P1L	(MASK_JLEFT)
#define M_P2L	(MASK_JLEFT)
#define M_P1R	(MASK_JRIGHT)
#define M_P2R	(MASK_JRIGHT)
#define M_P1START	(MASK_START)
#define M_P2START	(MASK_START)

/* ANGLE SCAN RESULTS */
#define ANG_NOSCAN 0								// NO ANGLE SCANS
#define ANG_JUP_RIGHT 1								// SCAN UPRIGHT
#define ANG_JUP_LEFT 2								// SCAN UPLEFT

/* STRIKER ENTRY STUFF */
#define SI_STRIKER_UPPER		0x00				// upper cut stiker
#define SI_STRIKER_BEHIND		0x01				// behind striker
#define SI_STRIKER_NORMAL		0x02				// normal striker
#define SI_STRIKER_NORMAL2		0x03				// normal striker2

/* JOY BEGIN INDEX COMMANDS */
#define JI_JOY_IGNORE			0x00				// ignore, do nada, execute begin code
#define JI_JOY_ENTRY			0x01				// joy entry code
#define JI_JOY_FLIP_PUNCH		0x02				// flip punch code
#define JI_JOY_FLIP_KICK		0x03				// flip kick code
#define JI_JOY_UPPERCUT			0x04				// uppercut code
#define JI_JOY_DUCK_PUNCH		0x05				// duck punch code
#define JI_JOY_DUCK_BLOCK		0x06				// duck block code
#define JI_JOY_DUCK_KICKH		0x07				// duck kick high code
#define JI_JOY_DUCK_KICKL		0x08				// duck kick low code
#define JI_JOY_HI_PUNCH			0x09				// hi punch code
#define JI_JOY_LO_PUNCH			0x0a				// low punch code
#define JI_JOY_BLOCK			0x0b				// block code
#define JI_JOY_HI_KICK			0x0c				// hi kick code
#define JI_JOY_LO_KICK			0x0d				// med kick code
#define JI_JUMPUP_PUNCH			0x0e				// jump up punch code
#define JI_JUMPUP_KICK			0x0f				// jump up kick code
#define JI_JOY_UP				0x10				// jump up
#define JI_JOY_DOWN				0x11				// go down
#define JI_JOY_GETUP_ENTRY		0x12				// entry point for getting up joystick
#define JI_JOY_BACK_UP			0x13				// entry point for getting back up
#define JI_JOY_DUCK_BLOCK_LOOP	0x14				// entry point for duck block
#define JI_JOY_BLOCK_LOOP		0x15				// entry point for block

// MUST MATCH JUMP TABLE AT THE BEGINNING OF joy_begin

#define TURBO_RAISE_DELAY	40

/*
 *	RAM
 */

/*
 *	PROTOTYPES
 */
void joy_begin(void);
void endurance_wake(void);
void switcheroo_wake(void);
void joy_proc(void);
void angle_abuse_check(void);

inline WORD am_i_him(LONG);
inline void stuff_buttons(OBJECT *,WORD *);
WORD am_i_facing_him(void);
inline WORD check_block_bit(void);
inline WORD check_his_block_bit(void);
void check_winner_status(void);
void stop_me(OBJECT *);
void set_x_vel(OBJECT *,long);
void walk_flip_check(void);
int angle_scan(void);
int look_for_angle(void);
void do_backup(void);
void do_duck(void);
void do_duck_block(void);
void do_flip(WORD,WORD,long,long);
void angle_jump_call(void);
inline void next_flip_whoosh(void);
inline void me_in_front(void);
inline void me_in_back(void);
inline WORD is_stick_toward(PROCESS *ptemp);
inline WORD is_stick_away(PROCESS *);
inline WORD isa5(PROCESS *,WORD);
short punch_strike_check(WORD pa0);
WORD punch_sleep(WORD,long);
LONG get_last_button(void);
WORD strike_check(STRKTBL *,WORD);
WORD strike_check_box(OBJECT *obj,WORD pa0,short pa1x,short pa1y,short pa2x,short pa2y);
WORD bar_reducer(WORD,WORD*);
void ochar_xlate_damage(WORD,WORD *);
void react_xfer_him(JUMPTBL);
inline WORD elbow_check(void);
void cbox_squeeze(XYTYPE *tl,XYTYPE *lr,short pa4,WORD pa6);
WORD striker(WORD,WORD,WORD,WORD,WORD,WORD);
WORD toss_check(OBJECT *,WORD);
void toss_drone_check(OBJECT *);
inline WORD knee_check(void);
WORD is_run_pressed(OBJECT *obj);
void run_setup(void);
WORD reduce_turbo_bar(void);
void zero_turbo_bar(void);
void turbo_bar_setup(void);
void draw_collision_box(XYTYPE *,XYTYPE *,WORD);
void game_over(void);
void drone_reverse_toss(void);
#define joy_toss drone_reverse_toss

void ochar_begin_calls(void);
void transform_smoke(void);
void r_transform_oldsmoke(void);
void newchar_load(WORD pa9);

/* externs */
extern WORD bt_null[];
extern WORD bt_angle_jump[];
extern WORD bt_duck[];
extern WORD bt_stance[];
extern WORD bt_jump[];

/*
 *	MACROS
 */
#define enable_all_buttons stuff_buttons(current_proc->pa8,bt_stance)
#define disable_all_buttons stuff_buttons(current_proc->pa8,bt_null)

#define disable_his_buttons stuff_buttons(current_proc->pdata.p_otherguy,bt_null)

#define JOYSTICK_IN_A0(_p) (*((_p)->pdata.p_joyport))			// returns current joystick values

#define MASK_JOYSTICK(_p) ((_p)->pdata.p_store4 & JOYSTICK_IN_A0(_p))	// TRUE, if mask==current joyvalue

#define inc_downcount(_p) (_p)->pdata.p_downcount++

/* striek check, clear not test flag */
#define strike_check_a0(_obj,_offset) strike_check(get_char_stk(_obj,_offset),0)

/* strike check, set test flag */
#define strike_check_a0_test(_obj,_offset) strike_check(get_char_stk(_obj,_offset),1)

/* abs x distance between players */
#define get_x_dist (abs((current_proc->pa8)->oxpos.u.intpos-(current_proc->pdata.p_otherguy)->oxpos.u.intpos))

/* abs y distance between players */
#define get_y_dist (abs((current_proc->pa8)->oypos.u.intpos-(current_proc->pdata.p_otherguy)->oypos.u.intpos))

/* stop him */
#define stop_him(_obj) stop_me(_obj)

/* collision box draw macros */
#define attack_box(_tl,_lr) draw_collision_box(_tl,_lr,0)
#define victim_box(_tl,_lr) draw_collision_box(_tl,_lr,1)

/* answer various stick direction questions */
#define is_stick_up(_proc) isa5(_proc,MASK_JUP)
#define is_stick_down(_proc) isa5(_proc,MASK_JDOWN)
#define is_stick_left(_proc) isa5(_proc,MASK_JLEFT)
#define is_stick_right(_proc) isa5(_proc,MASK_JRIGHT)

#endif /* __mk_mkjoy_h__ */

