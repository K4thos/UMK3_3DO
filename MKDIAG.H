/******************************************************************************
 File: mkdiag.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III diags header
******************************************************************************/

#ifndef __mk_mkdiag_h__
#define __mk_mkdiag_h__

/*
 * 	DEFINITIONS
 */
#define GAME_CREDITS  5					// # of credits to start

/* dipswitch equates */
#define DIP_VIOLENCE		0x0001
#define DIP_BLOOD			0x0002
#define DIP_LOW_BLOW		0x0004
#define DIP_AMSOUNDS		0x0008

/*
 *	RAM
 */

/*
 *	PROTOTYPES
 */

/* diagcoin, coinage */
//void p_start(void);
//void p_cont(void);
//WORD cr_strtp(void);
//WORD cr_contp(void);
//WORD get_credits(void);

/* diagcmos, audits */
inline short get_adj_diff_word(short *);

/* diagdtst, dip switch/memory card */
WORD read_dip(void);


/*
 *	MACROS
 */

#endif /* __mk_mkdiag_h__ */

