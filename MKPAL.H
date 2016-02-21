/******************************************************************************
 File: mkpal.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III pal Header 
******************************************************************************/

#ifndef __mk_mkpal_h__
#define __mk_mkpal_h__

/* 
 * 	DEFINITIONS
 */
#define NUM_F_PALS 30	//128								/* # of foreground palettes */
#define NUM_B_PALS 34	//128								/* # of background palettes */
#define NUM_PALS	(NUM_F_PALS+NUM_B_PALS)			/* # of dynamic allocated palettes */

#define CLUT_SIZE	256	//64								/* # of colors in a clut */
#define CLUT_SHIFT_SIZE	8	//6							/* 2^x=CLUT_SIZE, used for shifting */
#define CLUT_FACTOR 4	//16								/* # of cluts per row */
#define CLUT_ID_SHIFT	4	//2									/* shift conversion factor id->palram index */

#define FORE_CLUT_START_X CLUT_BASE_PAL_X			/* vram start for Foreground CLUTS X addr */
#define FORE_CLUT_START_Y CLUT_BASE_PAL_Y			/* vram start for Foreground CLUTS Y addr */

#define BKGD_CLUT_START_X (FORE_CLUT_START_X+((NUM_F_PALS%CLUT_FACTOR)<<CLUT_SHIFT_SIZE))	/* vram start for Background CLUTS X addr */
#define BKGD_CLUT_START_Y (FORE_CLUT_START_Y+(NUM_F_PALS/CLUT_FACTOR))	/* vram start for Background CLUTS Y addr */

#define PALQ_MAX_ENTRY 64							/* # of entries in palette transfer queue */

#define UNKNOWN_PAL 0xffff						// returned value if cant find palette (findpal)
#define END_STUFF 0x0000						// ending flptr stuff, (0000 should never be a color used */

typedef struct p_palqent {
	WORD csx;										/* dest x addr */
	WORD csy;										/* dest y addr */
	WORD count;										/* # of entries */
	ADDRESS *csrc;									/* src address of clut */
} PALQENT;

typedef struct p_palinfo {
	WORD *palid;										/* palette id (unique identifier palette data) */
	short palcnt;									/* palette allocation count (# of objs using pal) */
	WORD paltime;									/* @tick value when this palette was deleted */
} PALINFO;

/* 
 *	RAM 
 */
__EXTERN__ PALQENT paltrans_queue[PALQ_MAX_ENTRY];	/* transfer queue */
__EXTERN__ PALQENT *paltrans_start;					/* start ptr in transfer que */
__EXTERN__ PALQENT *paltrans_end;					/* end ptr in transfer que */

__EXTERN__ PALINFO palram[NUM_F_PALS+NUM_B_PALS];

#if DEBUG
__EXTERN__ WORD usefpcnt;							/* # of fore pals being used */
__EXTERN__ WORD usebpcnt;							/* # of back pals being used */
#endif

/*
 *	PROTOTYPES 
 */
inline void init_paltrans_queue(void);
WORD findpal(WORD*);
void clear_pal(void);
inline void clear_fore_pal(void);
inline void clear_back_pal(void);
WORD get_fore_pal(WORD *);
WORD get_back_pal(WORD *);
void palset_xfer(WORD,WORD,WORD *);
void free_pal(WORD);
void unget_pal(WORD);
void blow_pal(WORD*);
void transfer_palettes(void);

/* perm pals located in imgpal.s, if add update mkchar.inc */
extern WORD bpal_black_P[];
extern WORD bpal_red_P[];
//extern void *bpal_blue_P[];
extern WORD bpal_white_P[];
//extern WORD *bpal_grey_P[];
//extern WORD *bpal_yellow_P[];
//extern WORD *bpal_green_P[];
//extern WORD *bpal_dark_blue_P[];
extern WORD bpal_flash1_P[];
extern WORD bpal_flash2_P[];
extern WORD bpal_flash3_P[];
extern WORD bpal_flash4_P[];
extern WORD bpal_flash5_P[];
//extern WORD *boonpal[];
extern WORD SCORE_P[];
extern WORD BLOOD_P[];
extern WORD OIL_P[];
extern WORD SHEEBL_P[];
extern WORD WHITE_p[];

/*
 *	MACROS
 */
#define fpalram palram
#define bpalram (palram+NUM_F_PALS)

#define delpal(p)	free_pal(p->opal)

#endif /* __mk_mkpal_h__ */

