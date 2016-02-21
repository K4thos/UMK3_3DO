/******************************************************************************
 File: mkbonus.h

 By: David Schwartz

 Date: Dec 1994

 (C) Williams Entertainment

 Mortal Kombat III bonus Header
******************************************************************************/

#ifndef __mk_mkbonus_h__
#define __mk_mkbonus_h__

/*
 * 	DEFINITIONS
 */


/*
 *	RAM
 */

/*
 *	PROTOTYPES
 */
void bonus_count(void);
void kill_lingerings(void);
inline char *get_winner_text(void);
void wait_for_bonusv(WORD sector);
WORD get_winner_ochar(void);

/*
 *	MACROS
 */

#endif /* __mk_mkbonus_h__ */

