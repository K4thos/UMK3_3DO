/******************************************************************************
 File: mktext.h

 By: David Schwartz

 Date: Dec 1994

 (C) Williams Entertainment

 Mortal Kombat III text Header
******************************************************************************/

#ifndef __mk_mktext_h__
#define __mk_mktext_h__

/*
 * 	DEFINITIONS
 */

/* font defnitions */
#define FONT_15	0x00			// 15 point italic font
#define FONT_7	0x01			// 7 point italic font
#define FONT_7N	0x02			// 7 point normal font

#define FONT15_XOFF (FONT_15_BASE_X%128)			// # of pixels offset from tpage where 15 pt font is located
#define FONT7_XOFF (FONT_15_BASE_X%128)				// # of pixels offset from tpage where 7 pt font is located
#define FONT7N_XOFF (FONT_15_BASE_X%128)				// # of pixels offset from tpage where 7 pt font is located

#define FONT15_YOFF (FONT_15_BASE_Y%256)			// # of pixels offset from tpage where 15 pt font is located
#define FONT7_YOFF (FONT_15_BASE_Y%256)			// # of pixels offset from tpage where 7 pt font is located
#define FONT7N_YOFF (FONT_15_BASE_Y%256)			// # of pixels offset from tpage where 7 pt font is located

/* font routines definitions, update mkchar.inc */
#define STRCNRMOS 0x00				// set noscroll,shadow, insert objlst bits, center justify
#define STRCNRMOS_SCR 0x01			// shadow, insert objlst bits, center justify
#define STRLNRMOS_SCR 0x02			// shadow, insert objlst bits, left justify
#define STRRNRMOS_SCR 0x03			// shadow, insert objlst bits, right justify
#define STRCNRMO_SCR 0x04			// no shadow, insert objlst bits, center justify
#define STRLNRMO_SCR 0x05			// no shadow, insert objlst bits, left justify
#define STRRNRMO_SCR 0x06			// no shadow, insert objlst bits, right justify

/* stringer flag defn */
#define STR_SHADOW 0x8000		// shadow mask
#define STR_JUSTIFY 0x0003		// justify mask
#define STR_SCROLL 0x2000		// no scroll set mask
#define STR_OBJ	   0x1000		// insert on obj list mask

/* stringer control flags */
#define NEW_SLEEP		0x01	// change current sleep time
#define NEW_PCOLOR		0x02	// change the current print color
#define NEW_CONTROL		0x03	// change current dma control word
#define NEW_FONT		0x04	// change current font
#define NEW_XY			0x05	// change current print position
#define NEW_SPACING		0x06	// change current spacing
#define ADD_DYDX		0x07	// add this [y,x] to the current position
#define NEW_PALETTE 	0x08	// use new palette, specified by i.d.
#define ASCII_TAB		0x09
#define UNDERLINE_ON	0x0a	// turn on underlining
#define UNDERLINE_OFF	0x0b	// turn off underlining
#define ASCII_CR		0x0d


/* font tpage info */
typedef struct fnttpage
{
	BYTE u0,v0;						// offset within tpage (u0 even)
	BYTE w,h;						// width & height of character
} FNTTPAGE;

typedef struct fntlookup
{
	WORD f_tpage;					// base tpage for font
	FNTTPAGE *f_ptr;				// ptr to font info
} FNTLOOKUP;

typedef struct fnt_setup
{
	WORD fnt_num;					// fnt number (a11)
	WORD fnt_spacing;				// fnt spacing (a10)
	short fnt_posx;					// fnt position x (a9)
	short fnt_posy;					// fnt position y (a9)
	WORD *fnt_pal;					// fnt palette/color (a5)
	WORD fnt_routine;				// fnt routine index to execute (a1)
	WORD fnt_sleep;					// fnt sleep (a0)
} FNTSETUP;

/*
 *	RAM
 */
__EXTERN__ FNTSETUP fnt_state;		// current fnt state
__EXTERN__ WORD text_draw;
//__EXTERN__ char fnt_buffer[256];	// buffer to draw from for fnt
#define fnt_buffer ((char*)idcomp_ptr)	// use decompress buffer as temp work area
__EXTERN__ WORD string_underline;	// clear out underling
__EXTERN__ WORD string_shadow;		// shadow string
__EXTERN__ XYTYPE fnt_spacing;		// fnt spacing sizing info (a6)


extern FNTTPAGE font7_panel_info[];
extern FNTTPAGE font15_panel_info[];
extern FNTLOOKUP font_lists[];

#define font15_num (font15_panel_info+'0'-0x21)
#define font7_num (font7_panel_info+'0'-0x21)
/*
 *	PROTOTYPES
 */
void p15_centered(char *pa8,short pa9x,short pa9y);
void p15_centered_soff(char *pa8,short pa9x,short pa9y);
void p7_centered(char *pa8,short pa9x,short pa9y);
void p7_left(char *pa8,short pa9x,short pa9y);
void p7_left_p1(char *pa8,short pa9x,short pa9y,WORD parm1);
void pds_centered_soff(char *pa8,short pa9x,short pa9y);
void pds_centered(char *pa8,short pa9x,short pa9y);
void pds_right(char *pa8,short pa9x,short pa9y);
void pds_left(char *pa8,short pa9x,short pa9y);


void setup_text_fonts(void);
void pds_centered_p1(char *pa8,short pa9x,short pa9y,WORD parm1);
void lm_printf(FNTSETUP *);
void lm_printf_p1(FNTSETUP *fnt_info,WORD parm1);
void printf_p1(char *msg,WORD parm1);
void printf_s1(char *msg,char *parm1);
void mk_printf(char *);			// equiv to printf in arcade game
void stringer(void);
void spacing(void);
void justify(WORD);
short strnglen(void);
void copy_obj(OBJECT *,OBJECT *);


/* palettes (imgpal.s) */
extern void *text_P[];

/* text texture (mkcontro.s) */
extern void *mk15_font_TXT[];

/*
 *	MACROS
 */

/* setup the fnt draw state */
#define lm_setup(_info) memcpy(&fnt_state,(_info),sizeof(FNTSETUP))

#endif /* __mk_mktext_h__ */

