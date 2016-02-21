/******************************************************************************
 File: mkinit.h

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Initialization and Main Routine Headers
******************************************************************************/

#ifndef __mk_init_h__
#define __mk_init_h__

/*
 *	DEFINITIONS
 */
#define SWSTCK_SIZE 16					/* # of entries in switch stack */

/*
 *	RAM
 */
__EXTERN__ WORD f_load;

__EXTERN__ LONG system_marker;					/* set to coded # to indicate initialized system */

__EXTERN__ volatile JOYPAD swcurr;						/* current switch value (P2:P1) */
__EXTERN__ JOYPAD swtemp1;						/* previous switch state(P2:P1) */
__EXTERN__ JOYPAD swtemp2;						/* previous previous switch state (P2:P1) */
__EXTERN__ WORD *swstack;						/* ptr to current switch stack entry */
__EXTERN__ WORD swstmn[SWSTCK_SIZE];			/* switch stack memory */
__EXTERN__ WORD f_gridon;						// flag to turn grid on
__EXTERN__ WORD f_profile;						// flag: turn on profile

__EXTERN__ XYTYPE disp_world;					// current position for world during display
__EXTERN__ void *dlists_ptr;					// current entry in dlists to process

#if PROFILE
__EXTERN__ int prof_start;						// start time of profiler
__EXTERN__ int prof_end;						// end time of profiler
__EXTERN__ int prof_max;
__EXTERN__ int prof_draw_start;
__EXTERN__ int prof_draw_end;
#endif


/*
 *	PROTOTYPES
 */
void main(void);
void main_init(void);
void dipinit(void);
void wipeout(void);

void queue_and_jump(WORD,LONG *,WORD,WORD);
void player_die_check(WORD);
void switch_queue(WORD,LONG *);
void update_bar_common(WORD *,WORD *,WORD *,WORD);
void shadow_p1p2(void);

void p1_button0_open(void);
void p1_button1_open(void);
void p1_button2_open(void);
void p1_button3_open(void);
void p1_button4_open(void);
void p1_button5_open(void);
void p2_button0_open(void);
void p2_button1_open(void);
void p2_button2_open(void);
void p2_button3_open(void);
void p2_button4_open(void);
void p2_button5_open(void);
void p1_button0(void);
void p1_button1(void);
void p1_button2(void);
void p1_button3(void);
void p1_button4(void);
void p1_button5(void);
void p2_button0(void);
void p2_button1(void);
void p2_button2(void);
void p2_button3(void);
void p2_button4(void);
void p2_button5(void);
void p1_start_button(void);
void p1_joy_up(void);
void p1_joy_right(void);
void p1_joy_down(void);
void p1_joy_left(void);
void p2_start_button(void);
void p2_joy_up(void);
void p2_joy_right(void);
void p2_joy_down(void);
void p2_joy_left(void);
void p_grid(void);
void p_profile(void);
void p_colbox(void);
void pdie2(WORD pb0,WORD pb1,WORD pa3,WORD);
void p1_start_open(void);
void p2_start_open(void);
extern WORD *last_switch_ram[];
/*
 *	MACROS
 */
#define update_bar(_la0,_la1) update_bar_common((_la0),(_la1),NULL,0)
#define raise_turbo_bar(_la0,_la1,_la2) update_bar_common((_la0),(_la1),(_la2),1)

#define PAUSE_CHECK if (f_pause) process_suicide()

#endif /* __mk_init_h__ */
