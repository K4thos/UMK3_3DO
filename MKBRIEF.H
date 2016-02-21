/******************************************************************************
 File: mkbrief.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Brief case header
******************************************************************************/

#ifndef __mk_brief_h__
#define __mk_brief_h__

/*
 *	DEFINITIONS
 */

typedef struct bcc
{
	LONG b_code;
	JUMPTBL b_func;
} BCC;

/*
 *	RAM
 */

/*
 *	PROTOTYPES
 */
void bc_p1_handicap(void);
void bc_p2_handicap(void);
void bc_throwing_disabled(void);
void bc_blocking_disabled(void);
void bc_jackbot(void);
void bc_switcheroo(void);
void bc_dark_fighting(void);
void stuff_round_123(WORD pa0);

extern BCC brief_case_codes[];

/*
 *	MACROS
 */

#endif	/* __mk_brief_h__ */
