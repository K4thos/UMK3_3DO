/******************************************************************************
 File: mkhstd.h

 By: David Schwartz

 Date: Jan 1995

 (C) Williams Entertainment

 Mortal Kombat III High SCore header
******************************************************************************/

#ifndef __mk_mkhstd_h__
#define __mk_mkhstd_h__

/*
 * 	DEFINITIONS
 */
typedef struct hscore
{
	char hinit[4];			// initials
	WORD wiar;					// # of wins in a row
} HSCORE;

#define ENTRIES_PER_PAGE	10

#define ABC_STARTX	SCX(0x5d)
#define ABC_STARTY	(SCY(0x50)+12)

#define ABC_PER_LINE	9

#define P1_BMASK P1_BUTTON_MASK
#define P2_BMASK P2_BUTTON_MASK
#define P_JMASK (MASK_JUP|MASK_JDOWN|MASK_JLEFT|MASK_JRIGHT)

#define ABC_SPACEX	SCX(0x1a)
#define ABC_SPACEY	SCX(0x1b)

/*
 *	RAM
 */

/*
 *	PROTOTYPES
 */
void enter_initials(void);
void print_1_character(char ta0,short ta2x,short ta2y);
void get_initial_proc(void);
WORD check_for_rub(void);

void high_score_background(void);
void high_score_text(void);
OBJECT *get_one_initial(WORD *pa2x,WORD *pa2y,WORD letnum);
void button_wait(void);
void box_entry(void);
void boxme(RECT *box);
void valid_move(void);
void erasebox(void);
void insert_entry(void);
void print_rankings(void);
void print_line(WORD i);
WORD does_a0_cut_it(WORD pa0);

/*
 *	MACROS
 */

#endif /* __mk_mkhstd_h__ */

