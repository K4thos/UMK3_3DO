/******************************************************************************
 File: mkrepell.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Repell Player Header
******************************************************************************/

#ifndef __mk_mkrepell_h__
#define __mk_mkrepell_h__

/* 
 * 	DEFINITIONS
 */

#define REPELL_DISTANCE		SCX(0x40) 				/* repell distance for players */
#define SCREEN_DISTANCE		SCX(0x130)				/* screen distance allowed for players */
#define GRACE_PIXELS		SCX(0x30)				/* grace factor for distances */

/* 
 *	RAM 
 */

/*
 *	PROTOTYPES 
 */
void repell(void);
void get_player_hilo(OBJECT *);
inline void get_player_anis(void); 
void stuff_a6a7(void);


/*
 *	MACROS
 */

#endif /* __mk_mkrepell_h__ */

