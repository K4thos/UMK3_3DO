/******************************************************************************
 File: mkobj.h

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Header File For Object Handling Routines
******************************************************************************/

#ifndef __mk_obj_h__
#define __mk_obj_h__

/*
 *	DEFINITIONS
 */

#define NUM_OBJECTS 235			/* total # of objects for display lists */

#define BACK_BIT_RESOLUTION 32	/* # of bits per entry in back bit array */
#define BACK_BIT_SIZE 512		/* total # of bits in array */

#define DISPLAY_PADDING_X SCX(0x28)			/* x padding for display buffer */
#define DISPLAY_PADDING_Y SCY(0x8)			/* y padding for display buffer */

#define LEFT_PADDING	SCX(0x3a)			/* left edge padding */
#define RIGHT_PADDING	SCX(0x30)			/* right edge padding */

/* floor code stuff */
#define FLOOR_X 960					/* # of pixels per floor scan line */

#define FLR_BUFFER_X 160					/* half of the floor draw area */
#define FLR_BUFFER_Y FLOOR_Y				/* height of floor buffer */

/* oflags */
#define M_WRZERO 		0x0001				// write zero data
#define M_WRNONZ 		0x0002				// write non-zero data
#define M_CONZER 		0x0004				// replace zero data with constant
#define M_CONNON 		0x0008				// replace non-zero data with constant
#define M_CONST			0x000c				// any constant data replacement

/* sony remap of above flags from arcade */
#define M_BOX_POLY		0x0002				// draw a solid box

#define M_FLIPH			0x0010				// flip horizontally (DONT CHANGE POS)
#define M_FLIPV			0x0020				// flip vertically (DONT CHANGE POS)
#define M_FIGHTER		0x0040				// set if object is not a fighter (needed for sony vram management)

#define M_PAL			0xff00				// CLUT value offset

#define B_WRZERO		0
#define B_WRNONZ		1
#define B_CONZER		2
#define B_CONNON		3
#define B_FLIPH		4
#define B_FLIPV		5

#define B_PAL			8

/* oflags2 */
#define M_NOSCROLL	0x0001		/* mask, don't scroll this object */
#define M_MULTIPART	0x0002		/* mask, multipart object */
#define M_NOBLOCK	0x0004		/* mask, player can't block at this point */
#define M_NOFLIP	0x0008		/* mask, i can't be flipped at this moment */
#define M_SHADOW	0x0010		/* mask, shadow me */
#define M_INVISO	0x0020		/* mask, invisible object */
#define M_UGLY_PAL	0x0040		/* mask, ugly palette */
#define M_IGNORE_Y	0x0080		/* mask, flag: scroller ignore y y position (dont follow me) */
#define M_NOCOL		0x0100		/* mask, flag: no collisions allowed on me */
#define M_BANK1		0x0200		/* use image bank 1 window */
#define M_NOEDGE	0x0400		/* no edge limits */
#define M_HALF_DAMAGE	0x0800	/* flag: I take half damge now */
#define M_QUARTER_DAMAGE	0x1000	/* flag: i take 1/4 damage now */
#define M_YLOCK		0x2000		/* lock obj to y planee */


#define B_NOSCROLL	0			/* bit, don't scroll this object */
#define B_MULTIPART	1	 		/* bit, multipart object */
#define B_NOBLOCK	2	 		/* bit, player can't block at this point */
#define B_NOFLIP	3	 		/* bit, i can't be flipped at this moment */
#define B_SHADOW	4	 		/* bit, shadow me */
#define B_INVISO	5	 		/* bit, invisible object */
#define B_UGLY_PAL	6	 		/* bit, ugly palette */
#define B_IGNORE_Y	7 			/* bit, flag: scroller ignore y y position (dont follow me) */
#define B_NOCOL 	8			/* bit, flag: no collisions allowed on me */
#define B_BANK1		9			/* use image bank 1 window */
#define B_NOEDGE	10			/* no edge limits */
#define B_HALF_DAMAGE	11		/* flag: i take half damage now */
#define B_QUARTER_DAMAGE	12	/* flag: i take 1/4 damage now */
#define B_YLOCK 13			/* lock obj to y plane */

/*
 *	RAM
 */
__EXTERN__ OBJECT object_ram[NUM_OBJECTS];		/* object storage ram */

__EXTERN__ WORD scrntlx;						/* screen top left (y:x) */
__EXTERN__ WORD scrntl2x;						/* screen top left (y:x) */

__EXTERN__ WORD scrntly;						/* screen top left (y:x) */
__EXTERN__ WORD scrntl2y;						/* screen top left (y:x) */

__EXTERN__ WORD scrnlrx; 						/* screen bottom left (y:x) */
__EXTERN__ WORD scrnlr2x;						/* screen bottom left (y:x) */

__EXTERN__ WORD scrnlry;						/* screen bottom left (y:x) */
__EXTERN__ WORD scrnlr2y;						/* screen bottom left (y:x) */

#if !MULTIOFF
__EXTERN__ ADDRESS p1_ram[20];					/* player 1 multipart ram */
__EXTERN__ ADDRESS p2_ram[20];		 			/* player 2 multipart ram */
#endif

__EXTERN__ OBJECT *obj_free;					/* ptr to free objects */

__EXTERN__ OBJECT *objlst;						/* player object list */
__EXTERN__ OBJECT *objlst2;						/* object list #2 */
__EXTERN__ OBJECT *objlst3;						/* object list #3 */

/* Note: if add more lists then check for all clear routines */
__EXTERN__ OBJECT *baklst9;						/* background list #9 */
__EXTERN__ OBJECT *baklst8;						/* background list #8 */
__EXTERN__ OBJECT *baklst7;						/* background list #7 */
__EXTERN__ OBJECT *baklst6;						/* background list #6 */
__EXTERN__ OBJECT *baklst5;						/* background list #5 */
__EXTERN__ OBJECT *baklst4;						/* background list #4 */
__EXTERN__ OBJECT *baklst3;						/* background list #3 */
__EXTERN__ OBJECT *baklst2;						/* background list #2 */
__EXTERN__ OBJECT *baklst1;						/* background list #1 */

__EXTERN__ LONG bakbit8[BACK_BIT_SIZE/BACK_BIT_RESOLUTION];	/* background bit object on/off list #8 */
__EXTERN__ LONG bakbit7[BACK_BIT_SIZE/BACK_BIT_RESOLUTION];	/* background bit object on/off list #7 */
__EXTERN__ LONG bakbit6[BACK_BIT_SIZE/BACK_BIT_RESOLUTION];	/* background bit object on/off list #6 */
__EXTERN__ LONG bakbit5[BACK_BIT_SIZE/BACK_BIT_RESOLUTION];	/* background bit object on/off list #5 */
__EXTERN__ LONG bakbit4[BACK_BIT_SIZE/BACK_BIT_RESOLUTION];	/* background bit object on/off list #4 */
__EXTERN__ LONG bakbit3[BACK_BIT_SIZE/BACK_BIT_RESOLUTION];	/* background bit object on/off list #3 */
__EXTERN__ LONG bakbit2[BACK_BIT_SIZE/BACK_BIT_RESOLUTION];	/* background bit object on/off list #2 */
__EXTERN__ LONG bakbit1[BACK_BIT_SIZE/BACK_BIT_RESOLUTION];	/* background bit object on/off list #1 */

/* floor ram */
//__EXTERN__ BYTE floor_src_buffer[FLOOR_X*FLOOR_Y] __attribute__ ((aligned (4)));				// src buffer area for floor
#if ONE_FLOOR_BUFFER==0
__EXTERN__ BYTE floor_draw_bufr[FLR_BUFFER_X*FLR_BUFFER_Y] __attribute__ ((aligned (4)));		// draw buffer left half
#endif
__EXTERN__ BYTE floor_draw_buf[FLR_BUFFER_X*FLR_BUFFER_Y] __attribute__ ((aligned (4)));		// draw buffer right half

/* display stuff */
__EXTERN__ WORD noflip;						/* if set no page flipping needed */
__EXTERN__ FUNC call_every_tick;			/* call routine every tick */

__EXTERN__ POS scrolly;						/* y scroll value */
__EXTERN__ POS worldtly8;					/* background #8 x world coord */
__EXTERN__ POS worldtly7;					/* background #7 x world coord */
__EXTERN__ POS worldtly6;					/* background #6 x world coord */
__EXTERN__ POS worldtly5;					/* background #5 x world coord */
__EXTERN__ POS worldtly4;					/* background #4 x world coord */
__EXTERN__ POS worldtly3;					/* background #3 x world coord */
__EXTERN__ POS worldtly2;					/* background #2 x world coord */
__EXTERN__ POS worldtly1;					/* background #1 x world coord */
__EXTERN__ POS worldtly;					/* top left y screen coord of world */
__EXTERN__ POS scrollx8;					/* background #8 scroll value */
__EXTERN__ POS scrollx7;					/* background #7 scroll value */
__EXTERN__ POS scrollx6;					/* background #6 scroll value */
__EXTERN__ POS scrollx5;					/* background #5 scroll value */
__EXTERN__ POS scrollx4;					/* background #4 scroll value */
__EXTERN__ POS scrollx3;					/* background #3 scroll value */
__EXTERN__ POS scrollx2;					/* background #2 scroll value */
__EXTERN__ POS scrollx1;					/* background #1 scroll value */
__EXTERN__ POS scrollx;						/* x scroll value */

__EXTERN__ POS worldtlx8;					/* background #8 x world coord */
__EXTERN__ POS worldtlx7;					/* background #7 x world coord */
__EXTERN__ POS worldtlx6;					/* background #6 x world coord */
__EXTERN__ POS worldtlx5;					/* background #5 x world coord */
__EXTERN__ POS worldtlx4;					/* background #4 x world coord */
__EXTERN__ POS worldtlx3;					/* background #3 x world coord */
__EXTERN__ POS worldtlx2;					/* background #2 x world coord */
__EXTERN__ POS worldtlx1;					/* background #1 x world coord */
__EXTERN__ POS worldtlx;					/* top left x screen coord (world) */

/* temp background pos */
__EXTERN__ OBJECT *baklst8_t;				/* temp background list #8 */
__EXTERN__ OBJECT *baklst7_t;				/* temp background list #7 */
__EXTERN__ OBJECT *baklst6_t;				/* temp background list #6 */
__EXTERN__ OBJECT *baklst5_t;				/* temp background list #5 */
__EXTERN__ OBJECT *baklst4_t;				/* temp background list #4 */
__EXTERN__ OBJECT *baklst3_t;				/* temp background list #3 */
__EXTERN__ OBJECT *baklst2_t;				/* temp background list #2 */
__EXTERN__ OBJECT *baklst1_t;				/* temp background list #1 */
__EXTERN__ OBJECT *objlst_t;				/* temp player object list */
__EXTERN__ POS worldtly_t;					/* temp top left y screen coord of world */

__EXTERN__ POS worldtlx8_t;					/* temp background #8 x world coord */
__EXTERN__ POS worldtlx7_t;					/* temp background #7 x world coord */
__EXTERN__ POS worldtlx6_t;					/* temp background #6 x world coord */
__EXTERN__ POS worldtlx5_t;					/* temp background #5 x world coord */
__EXTERN__ POS worldtlx4_t;					/* temp background #4 x world coord */
__EXTERN__ POS worldtlx3_t;					/* temp background #3 x world coord */
__EXTERN__ POS worldtlx2_t;					/* temp background #2 x world coord */
__EXTERN__ POS worldtlx1_t;					/* temp background #1 x world coord */
__EXTERN__ POS worldtlx_t;					/* temp top left x screen coord (world) */

/*
 *	PROTOTYPES
 */
void object_initialize(void);
void zero_word_init(void);
void zero_long_init(void);
OBJECT *get_object(void);
void insert_object(OBJECT *,OBJECT **);
void display_object_lists(XYTYPE *,OBJECT *);
void del_loop(OBJECT *,OBJECT **);
void kilgen(WORD,WORD,OBJECT **);
void gravity_n_bounds(OBJECT *,short,short);
void yzsort(void);
void sort_list(OBJECT **);
void shadow_code(OBJECT *,short);
void floor_code(void);
WORD scrtst(OBJECT *);

void SetUVWH(POLY_FT4 *ft4, int u0, int v0, int w, int h);

/*
 *	MACROS
 */
#define kilobj(_id,_mask) kilgen((_id),(_mask),&objlst)
#define kilobj2(_id,_mask) kilgen((_id),(_mask),&objlst2)

#endif /* __mk_obj_h__ */
