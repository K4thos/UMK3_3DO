/******************************************************************************
 File: mkfade.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III fade Header
******************************************************************************/

#ifndef __mk_mkfade_h__
#define __mk_mkfade_h__

/* 
 * 	DEFINITIONS
 */
#define PRE_A4 0x001f					// a4 - pre mult mask for 5 bits of red
#define PRE_A6 0x03e0					// a6 - pre mult mask for 5 bits of green
#define PRE_A8 0x7c00					// a8 - pre mult mask for 5 bits of blue

#define POST_A9 (PRE_A4<<7)				// a9 - post mult max for 5 bits of red
#define POST_A10 (PRE_A6<<7)			// a10 - post mult max for 5 bits of green
#define POST_A11 (PRE_A8<<7)			// a11 - post mult max for 5 bits of blue

#define WTOP 700
#define DTOP 8

/* 
 *	RAM 
 */

/*
 *	PROTOTYPES 
 */
void fade_this_pal(WORD pa0,WORD pa5,WORD pa10,WORD *pa11);
void single_pal_fader(void);
void newfade(WORD *pa0,WORD *pa1);
void fast_fadeout_jsrp(WORD pa10);
void fadeout_jsrp(WORD pa10);
void fadein_white_jsrp(void);
void fadewhite(WORD **pa0);
void white_2_blax(void);
void fadein_white(WORD **pa0);
void fade_white(WORD **pa0);
void fadeout_mercy(WORD **pa0);
void fast_fadein_jsrp(WORD pa10);
void fadein_jsrp(WORD pa10);
void fade_sleep_a0(WORD pa0);
void fade_all_sky(void);
void raiden_dimmer(void);
void raiden_undimmer(void);
void pause_dimmer(void);
void pause_undimmer(void);
void fadeblak(WORD **);
void fadeins(WORD **pa0);
void fadein(WORD **pa0);
void join_in_fade1(WORD **pa0);
void join_in_fade2(WORD **pa0);
void fade_in_fast(void);
void fade_all_fast(void);
void fadein_all_fast(void);
void fadeout(WORD **pa0);
void fader(WORD **pa0,WORD *pa11);
void fadeproc(void);
void fadepal(WORD *pa0,WORD *pa1,WORD pa2);
void skydown(void);
void skyup(void);
WORD xcolor(WORD pa0,LONG pa1);

/*
 *	MACROS
 */
#define fadeall(_la0,_la11) fader(_la0,_la11)
#define skydie(_ta1) xcolor(irqskye,_ta1),process_suicide()
#endif /* __mk_mkfade_h__ */

