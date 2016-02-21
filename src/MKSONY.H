/******************************************************************************
 File: mksony.h

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III SONY SPECIFIC STUFF
******************************************************************************/

#ifndef __mk_sony_h__
#define __mk_sony_h__

/* SONY LIBRARIES */
#include <sys/types.h>
#include <r3000.h>
#include <asm.h>
#include <kernel.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libsn.h>

/*
 *	TYPES
 */
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int LONG;
typedef unsigned long ADDRESS;

/* fixed point 16.16 union */
typedef union u_pos {
	long	pos;			/* 16.16 position */
	struct {
		short	fracpos;	/* fractional position */
		short	intpos;		/* integer part */
	} u;
} POS;

/* joy pad info */
typedef union joypad {
	LONG padvalue;			/* pad p2:p1 */
	struct {
		WORD p1pad;
		WORD p2pad;
	} u;
} JOYPAD;

/* y:x pos (integer info) */
typedef union u_xytype {
	long yx;					/* y:x position */
	struct {
		short	xpos;			/* x position */
		short	ypos;			/* y position */
	} u;
} XYTYPE;

/* rgb color info */
typedef struct o_color {
	BYTE red;
	BYTE green;
	BYTE blue;
	WORD rgb;
} COLOR;

/*
 *	DEFINITIONS
 */

#define SONY_PAD_ID 1					/* type of joystick hooked up */

#define MAP_TBL_ENTRIES 16				/* 16 bits for each pad */

#define SCORE_RAM_CNT	22				/* # of sprites needed for the control panel */

#if DEBUG
#define SONY_GRAPH_DEBUG 0 //2				/* set graphics debug mode */
#define SONY_HBLANK 0 //1					/* allow hblank interrupts */
#else
#define SONY_GRAPH_DEBUG 0
#define SONY_HBLANK 0
#endif /* DEBUG */

#define OTSIZE	250								/* size of ording table */

#define OBJPRIM_SIZE 220						/* # of drawing primitives per buffer */

#define BOXPRIM_SIZE 10							/* number of box prim to draw */

#define TEXTURE_MODE 1							/* 0=4 bit, 1=8bit, 2=15 bit */

#define TRANS_RATE 0							/* semi transperency rate */

#define SHADE_TEX 0								/* shading */

/* VRAM Memory Allocation */
#define BUFFER_WIDTH 320						/* width of display buffer */
#define BUFFER_HEIGHT 240						/* height of display buffer */

#define BUF0_X_COORD 0							/* draw buffer 0 x coordinate */
#define BUF0_Y_COORD 0							/* draw buffer 0 y coordinate */

#define BUF1_X_COORD BUFFER_WIDTH				/* draw buffer 1 x coordinate */
#define BUF1_Y_COORD 0							/* draw buffer 1 y coordinate */

#define BACK_TEXTURE_BASE_X 384					/* base x coord of backgrounds */
#define BACK_TEXTURE_BASE_Y 256					/* base y coord of backgrounds */

#define CP_TEXT_X	640							/* control panel texture page start x */
#define CP_TEXT_Y	0							/* control panel texture page start y */

#define OPT_TEXT_X	896							/* option texture page x coord */
#define OPT_TEXT_Y	256							/* option texture page y coord */

#define CLUT_BASE_PAL_X	0						/* base vram CLUT X addr */
#define CLUT_BASE_PAL_Y 240						/* base vram CLUT Y addr */

#define FLOOR_Y 84							/* # of pixels per floor in height */

/* floor goes below control panel stuff, cp art cannot exceed 68 lines */
/* floors cannont go below line 239 */
#define FLR_L_BASE_X	CP_TEXT_X		//640+128
#define FLR_L_BASE_Y	68			//(255-FLOOR_Y)

#define FLR_R_BASE_X	CP_TEXT_X				//(640+128+128)
#define FLR_R_BASE_Y	FLR_L_BASE_Y+FLOOR_Y	//(255-FLOOR_Y)

#define PLYR_1_BASE_X	640+128+1
#define PLYR_1_BASE_Y	0

#define PLYR_2_BASE_X	(640+128+128)+1
#define PLYR_2_BASE_Y	0

#define FONT_15_BASE_X	(640+128)		/* 15 point font vram start x local */
#define FONT_15_BASE_Y	(170)					/* 15 point font vram start y local */

#define LOADING_BASE_X	(640+128+128)		/* loading vram start x local */
#define LOADING_BASE_Y	(170)						/* loading vram start y local */


#define FRAME_WIDTH 1023
#define FRAME_HEIGHT 512

/* screen sizing information */
#define SCRTOP 0
#define SCRBOT (BUFFER_HEIGHT-1)
#define SCRLFT 0
#define SCRRGT (BUFFER_WIDTH-1)

#define SCRNSTX SCRLFT
#define SCRNSTY SCRTOP
#define SCRNENDX SCRRGT
#define SCRNENDY SCRBOT

/* draw buffer primitive */
typedef struct {
	DRAWENV	draw;					/* draw environment */
	DISPENV disp;					/* display environment */
	u_long ot[OTSIZE];				/* ordering table */
	u_long *ot_ptr;					/* ordering table ptr */
	POLY_FT4 obj_prim[OBJPRIM_SIZE];	/* drawing primitives */
	POLY_FT4 *curr_prim;
	BLK_FILL clear;
	LONG primcnt;

	DR_MODE	dm_score;						/* needed to change tpage during draw */
	SPRT cp[SCORE_RAM_CNT];

	DR_MODE dm_floor;						/* change tpage for floor */
	SPRT floor[2];							/* floor left/right buffers */

	DR_MODE dm_test;						/* change tpage for floor */

	POLY_F4	box_prim[BOXPRIM_SIZE];	/* used to draw box primitives */
	POLY_F4 *box_ptr;

#if DEBUG_GRID
	LINE_F2 lines[22];
#endif
} DB;

/* context switch storage */
typedef struct mc_reg {
	ADDRESS c_wakeup;				/* ptr to next code to execute */
	ADDRESS c_stackptr;				/* stack ptr */
	ADDRESS c_frameptr;				/* frame ptr */
	ADDRESS c_globalptr;			/* global ptr */
	ADDRESS c_s0;					/* s0 reg */
	ADDRESS c_s1;					/* s1 reg */
	ADDRESS c_s2;					/* s2 reg */
	ADDRESS c_s3;					/* s3 reg */
	ADDRESS c_s4;					/* s4 reg */
	ADDRESS c_s5;					/* s5 reg */
	ADDRESS c_s6;					/* s6 reg */
	ADDRESS c_s7;					/* s7 reg */
} MCONTEXT;

typedef struct c_reg {
	ADDRESS c_wakeup;				/* ptr to next code to execute */
	ADDRESS c_stackptr;				/* stack ptr */
	ADDRESS c_frameptr;				/* frame ptr */
	ADDRESS c_globalptr;			/* global ptr */
	ADDRESS c_s0;					/* s0 reg */
	ADDRESS c_s1;					/* s1 reg */
	ADDRESS c_s2;					/* s2 reg */
	ADDRESS c_s3;					/* s3 reg */
	ADDRESS c_s4;					/* s4 reg */
	ADDRESS c_s5;					/* s5 reg */
	ADDRESS c_s6;					/* s6 reg */
	ADDRESS c_s7;					/* s7 reg */
	ADDRESS c_stack[STKSIZE] __attribute__ ((aligned (8)));		/* stack space for the processor
																   MUST BE DOUBLEWORD aligned */
} CONTEXT;

/* Player Heap Area */
#define PLAYER_HEAP_SIZE	(510000*2)

/*
 *	RAM
 */
__EXTERN__ DB db[2];						/* double buffer environment */
__EXTERN__ DB *cdb;							/* current draw buffer */
__EXTERN__ WORD	bkgd_base_tpage;			/* base texture page for backgrounds */
__EXTERN__ WORD control_floor_tpage;		/* tpage info for control panel texture page & floors */
__EXTERN__ WORD load_tpage;				/* loading message tpage */

__EXTERN__ u_long vbEvent;					/* event # for vblank routine */

__EXTERN__ WORD pad1_map_tbl[MAP_TBL_ENTRIES];			// remap table for joystick 1
__EXTERN__ WORD pad2_map_tbl[MAP_TBL_ENTRIES];			// remap table for joystick 2
__EXTERN__ WORD game_map_tbl[MAP_TBL_ENTRIES];			// table for non fight modes

/* image decomp buffer area */
#define DECOMP_BUFFER_SIZE	(1024*50)			/* # of bytes in image decomp buffer */
__EXTERN__ LONG image_dcomp_buffer[DECOMP_BUFFER_SIZE/4];	// holds both image decomp buffers
__EXTERN__ LONG *idcomp_ptr;						// decomp buffer ptr to next free area
__EXTERN__ LONG *idcomp_base;

extern BYTE player_heap[PLAYER_HEAP_SIZE] __attribute__ ((aligned(4)));
#define p1_heap (&player_heap[0])
#define p2_heap (&player_heap[PLAYER_HEAP_SIZE/2])

/*
 *	PROTOTYPES
 */
void init_sony_system(void);
void mk_vblank_int(void);
void ermac(WORD);
BYTE *uncompress_image(BYTE *src);
WORD joy_remap(WORD,WORD *);
void load_perm_art(void);

/*
 *	MACROS
 */
/* yes & no answers */
#define q_yes return(SYSTEM_CARRY_SET)
#define q_no return(SYSTEM_CARRY_CLR)


#define SET_BLUE(rgb)  ((rgb) & (0x1f<<10))
#define SET_GREEN(rgb)  ((rgb) & (0x1f<<5))
#define SET_RED(rgb)  ((rgb) & 0x1f)

#define GET_BLUE(rgb)  (((rgb) & 0x7c00)>>10)
#define GET_GREEN(rgb)  (((rgb) & 0x3e0)>>5)
#define GET_RED(rgb)  ((rgb) & 0x1f)

#define SET_RGB(r,g,b) (32*32*(b)+32*(g)+(r))

#define DISPLAY_OFF	displayon=0,SetDispMask(0)
#define DISPLAY_ON displayon=1,SetDispMask(1)

#define GET_BYTE(b) *((BYTE*)b)
#define GET_WORD(w) *((WORD*)w)
#define GET_LONG(l) *((LONG*)l)

#define COMPUTE_ADDR(_base,_offset) ((BYTE *)(_base)+(LONG)(_offset))


u_short DLoadTpage(u_long *pix, int tp, int abr, int x, int y, int w, int h);
int DLoadImage(RECT *rect, u_long *p);

#if DEBUG==0
#define LOADTPAGE(_pix, _tp, _abr, _x, _y, _w, _h) \
	LoadTPage(_pix, _tp, _abr, _x, _y, _w, _h);

#define LOADIMAGE(_rect,_p) \
	LoadImage(_rect,_p)

#else

#define LOADTPAGE(_pix, _tp, _abr, _x, _y, _w, _h) \
		DLoadTPage(_pix, _tp, _abr, _x, _y, _w, _h);

#define LOADIMAGE(_rect,_p) \
	DLoadImage(_rect,_p)

#endif

#endif 	/* __mk_sony_h__ */

