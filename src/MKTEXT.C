/******************************************************************************
 File: mktext.c

 Date: Jan 1995

 (C) Williams Entertainment

 Mortal Kombat III Text Routines
******************************************************************************/

/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkobj.h"
#include "mkos.h"
#include "mkutil.h"
#include "mkani.h"
#include "mkguys.h"
#include "mkmain.h"
#include "mkgame.h"
#include "mkpal.h"
#include "mkfile.h"
#include "mktext.h"

/* local extern info */
extern FNTSETUP pf_15point[];
extern FNTSETUP pf_15point_soff[];
extern FNTSETUP pf_7point[];
extern FNTSETUP pf_7point_left[];
extern FNTSETUP pf_dave_smallc_soff[];
extern FNTSETUP pf_dave_smallc[];
extern FNTSETUP pf_dave_smallr[];
extern FNTSETUP pf_dave_smalll[];
extern WORD YELLOW_p[];
/* 7 Point Italics */
FNTTPAGE font7_panel_info[]=
{
	{FONT7_XOFF+138,FONT7_YOFF+39,6,10},			//	!, 0x21
	{FONT7_XOFF+217,FONT7_YOFF+39,7,5},				//	"
	{FONT7_XOFF+0,FONT7_YOFF+0,0,0},				//	#, invalid
	{FONT7_XOFF+0,FONT7_YOFF+0,0,0},				//	$, invalid
	{FONT7_XOFF+151,FONT7_YOFF+40,10,9},			//	%
	{FONT7_XOFF+0,FONT7_YOFF+0,0,0},				//	&, invalid
	{FONT7_XOFF+211,FONT7_YOFF+39,6,5},				//	'
	{FONT7_XOFF+193,FONT7_YOFF+39,9,12},			//	(
	{FONT7_XOFF+202,FONT7_YOFF+39,9,12},			//	)
	{FONT7_XOFF+0,FONT7_YOFF+0,0,0},				//	*, invalid
	{FONT7_XOFF+0,FONT7_YOFF+0,0,0},				//	+, invalid
	{FONT7_XOFF+133,FONT7_YOFF+39,4,11},			//	,
	{FONT7_XOFF+145,FONT7_YOFF+39,5,6},				//	-
	{FONT7_XOFF+122,FONT7_YOFF+39,3,10},			//	.
	{FONT7_XOFF+184,FONT7_YOFF+38,9,11},			//	/
	{FONT7_XOFF+42,FONT7_YOFF+37,8,9},				//	0
	{FONT7_XOFF+50,FONT7_YOFF+37,6,9},				//	1
	{FONT7_XOFF+56,FONT7_YOFF+37,9,9},				//	2
	{FONT7_XOFF+65,FONT7_YOFF+37,9,9},				//	3
	{FONT7_XOFF+74,FONT7_YOFF+37,8,9},				//	4
	{FONT7_XOFF+82,FONT7_YOFF+37,7,9},				//	5
	{FONT7_XOFF+89,FONT7_YOFF+37,8,10},				//	6
	{FONT7_XOFF+97,FONT7_YOFF+37,8,9},				//	7
	{FONT7_XOFF+105,FONT7_YOFF+37,9,10},			//	8
	{FONT7_XOFF+114,FONT7_YOFF+37,8,10},			//	9
	{FONT7_XOFF+126,FONT7_YOFF+39,4,9},				//	:
	{FONT7_XOFF+0,FONT7_YOFF+0,0,0},				//	;, invalid
	{FONT7_XOFF+0,FONT7_YOFF+0,0,0},				//	<, invalid
	{FONT7_XOFF+145,FONT7_YOFF+39,5,6},				//	=, map to dash
	{FONT7_XOFF+0,FONT7_YOFF+0,0,0},				//	>, invalid
	{FONT7_XOFF+177,FONT7_YOFF+39,6,10},			//	?
	{FONT7_XOFF+0,FONT7_YOFF+0,0,0},				//	@, invalid
	{FONT7_XOFF+57,FONT7_YOFF+27,9,10},				//	A, 0x61
	{FONT7_XOFF+66,FONT7_YOFF+27,9,9},				//	B
	{FONT7_XOFF+75,FONT7_YOFF+27,6,9},				//	C
	{FONT7_XOFF+81,FONT7_YOFF+27,9,9},				//	D
	{FONT7_XOFF+90,FONT7_YOFF+27,8,9},				//	E
	{FONT7_XOFF+98,FONT7_YOFF+27,9,9},				//	F
	{FONT7_XOFF+107,FONT7_YOFF+27,9,9},				//	G
	{FONT7_XOFF+116,FONT7_YOFF+27,9,10},			//	H
	{FONT7_XOFF+125,FONT7_YOFF+27,6,9},				//	I
	{FONT7_XOFF+131,FONT7_YOFF+27,8,11},			//	J
	{FONT7_XOFF+139,FONT7_YOFF+27,10,9},			//	K
	{FONT7_XOFF+149,FONT7_YOFF+27,7,9},				//	L
	{FONT7_XOFF+156,FONT7_YOFF+27,11,9},			//	M
	{FONT7_XOFF+167,FONT7_YOFF+27,10,9},			//	N
	{FONT7_XOFF+178,FONT7_YOFF+27,8,9},				//	O
	{FONT7_XOFF+186,FONT7_YOFF+27,9,9},				//	P
	{FONT7_XOFF+195,FONT7_YOFF+29,9,10},			//	Q
	{FONT7_XOFF+204,FONT7_YOFF+29,10,10},			//	R
	{FONT7_XOFF+214,FONT7_YOFF+27,8,9},				//	S
	{FONT7_XOFF+222,FONT7_YOFF+27,8,9},				//	T
	{FONT7_XOFF+230,FONT7_YOFF+27,8,9},				//	U
	{FONT7_XOFF+238,FONT7_YOFF+27,8,9},				//	V
	{FONT7_XOFF+0,FONT7_YOFF+41,11,9},				//	W
	{FONT7_XOFF+11,FONT7_YOFF+41,10,10},			//	X
	{FONT7_XOFF+21,FONT7_YOFF+41,8,10},				//	Y
	{FONT7_XOFF+29,FONT7_YOFF+41,9,9},				//	Z
	{FONT7_XOFF+161,FONT7_YOFF+39,7,10},			//	[
	{FONT7_XOFF+0,FONT7_YOFF+0,0,0},				//	\, invalid
	{FONT7_XOFF+169,FONT7_YOFF+39,7,10}				//	]
};

/* 7 Point Normal */
FNTTPAGE font7N_panel_info[]=
{
	{FONT7N_XOFF+234,FONT7N_YOFF+52,2,7},		//	!, 0x21
	{FONT7N_XOFF+32,FONT7N_YOFF+61,5,3},		//	"
	{FONT7N_XOFF+0,FONT7N_YOFF+0,0,0},			//	#, invalid
	{FONT7N_XOFF+0,FONT7N_YOFF+0,0,0},			//	$, invalid
	{FONT7N_XOFF+243,FONT7N_YOFF+52,8,7},		//	%
	{FONT7N_XOFF+0,FONT7N_YOFF+0,0,0},			//	&, invalid
	{FONT7N_XOFF+29,FONT7N_YOFF+61,2,3},		//	'
	{FONT7N_XOFF+19,FONT7N_YOFF+60,4,9},		//	(
	{FONT7N_XOFF+24,FONT7N_YOFF+60,4,9},		//	)
	{FONT7N_XOFF+0,FONT7N_YOFF+0,0,0},			//	*, invalid
	{FONT7N_XOFF+0,FONT7N_YOFF+0,0,0},			//	+, invalid
	{FONT7N_XOFF+231,FONT7N_YOFF+52,2,8},		//	,
	{FONT7N_XOFF+238,FONT7N_YOFF+52,4,4},		//	-
	{FONT7N_XOFF+223,FONT7N_YOFF+52,2,7},		//	.
	{FONT7N_XOFF+14,FONT7N_YOFF+60,4,9},		//	/
	{FONT7N_XOFF+165,FONT7N_YOFF+52,5,7},		//	0
	{FONT7N_XOFF+171,FONT7N_YOFF+52,3,7},		//	1
	{FONT7N_XOFF+175,FONT7N_YOFF+52,5,7},		//	2
	{FONT7N_XOFF+181,FONT7N_YOFF+52,5,7},		//	3
	{FONT7N_XOFF+187,FONT7N_YOFF+52,5,7},		//	4
	{FONT7N_XOFF+193,FONT7N_YOFF+52,5,7},		//	5
	{FONT7N_XOFF+199,FONT7N_YOFF+52,5,7},		//	6
	{FONT7N_XOFF+205,FONT7N_YOFF+52,5,7},		//	7
	{FONT7N_XOFF+211,FONT7N_YOFF+52,5,7},		//	8
	{FONT7N_XOFF+217,FONT7N_YOFF+52,5,7},		//	9
	{FONT7N_XOFF+227,FONT7N_YOFF+52,2,7},		//	:
	{FONT7N_XOFF+0,FONT7N_YOFF+0,0,0},			//	;, invalid
	{FONT7N_XOFF+0,FONT7N_YOFF+0,0,0},			//	<, invalid
	{FONT7N_XOFF+238,FONT7N_YOFF+52,4,4},		//	-, map to dash
	{FONT7N_XOFF+0,FONT7N_YOFF+0,0,0},			//	>, invalid
	{FONT7N_XOFF+8,FONT7N_YOFF+60,5,9},			//	?
	{FONT7N_XOFF+0,FONT7N_YOFF+0,0,0},			//	@, invalid
	{FONT7N_XOFF+0,FONT7N_YOFF+52,5,7},			//	A, 0x61
	{FONT7N_XOFF+6,FONT7N_YOFF+52,5,7},			//	B
	{FONT7N_XOFF+12,FONT7N_YOFF+52,5,7},		//	C
	{FONT7N_XOFF+18,FONT7N_YOFF+52,5,7},		//	D
	{FONT7N_XOFF+24,FONT7N_YOFF+52,5,7},		//	E
	{FONT7N_XOFF+30,FONT7N_YOFF+52,5,7},		//	F
	{FONT7N_XOFF+36,FONT7N_YOFF+52,5,7},		//	G
	{FONT7N_XOFF+42,FONT7N_YOFF+52,5,7},		//	H
	{FONT7N_XOFF+48,FONT7N_YOFF+52,2,7},		//	I
	{FONT7N_XOFF+51,FONT7N_YOFF+52,5,7},		//	J
	{FONT7N_XOFF+57,FONT7N_YOFF+52,5,7},		//	K
	{FONT7N_XOFF+63,FONT7N_YOFF+52,4,7},		//	L
	{FONT7N_XOFF+68,FONT7N_YOFF+52,8,7},		//	M
	{FONT7N_XOFF+77,FONT7N_YOFF+52,6,7},		//	N
	{FONT7N_XOFF+84,FONT7N_YOFF+52,6,7},		//	O
	{FONT7N_XOFF+91,FONT7N_YOFF+52,5,7},		//	P
	{FONT7N_XOFF+97,FONT7N_YOFF+52,6,9},		//	Q
	{FONT7N_XOFF+104,FONT7N_YOFF+52,5,7},		//	R
	{FONT7N_XOFF+110,FONT7N_YOFF+52,5,7},		//	S
	{FONT7N_XOFF+116,FONT7N_YOFF+52,6,7},		//	T
	{FONT7N_XOFF+123,FONT7N_YOFF+52,5,7},		//	U
	{FONT7N_XOFF+129,FONT7N_YOFF+52,6,7},		//	V
	{FONT7N_XOFF+136,FONT7N_YOFF+52,9,7},		//	W
	{FONT7N_XOFF+146,FONT7N_YOFF+52,6,7},		//	X
	{FONT7N_XOFF+153,FONT7N_YOFF+52,5,7},		//	Y
	{FONT7N_XOFF+159,FONT7N_YOFF+52,5,7},		//	Z
	{FONT7N_XOFF+0,FONT7N_YOFF+60,3,9},			//	[
	{FONT7N_XOFF+0,FONT7N_YOFF+0,0,0},			//	\, invalid
	{FONT7N_XOFF+4,FONT7N_YOFF+60,3,9}			//	]
};

/* 15 Point Italics */
FNTTPAGE font15_panel_info[]=
{
	{FONT15_XOFF+207,FONT15_YOFF+15,8,12},				//	!, 0x21
	{FONT15_XOFF+48,FONT15_YOFF+27,9,5},				//	"
	{FONT15_XOFF+0,FONT15_YOFF+0,0,0},					//	#, invalid
	{FONT15_XOFF+0,FONT15_YOFF+0,0,0},					//	$, invalid
	{FONT15_XOFF+222,FONT15_YOFF+15,13,12},				//	%
	{FONT15_XOFF+0,FONT15_YOFF+0,0,0},					//	&, invalid
	{FONT15_XOFF+42,FONT15_YOFF+27,6,5},				//	'
	{FONT15_XOFF+20,FONT15_YOFF+27,11,14},				//	(
	{FONT15_XOFF+31,FONT15_YOFF+27,11,14}, 				//	)
	{FONT15_XOFF+0,FONT15_YOFF+0,0,0},					//	*, invalid
	{FONT15_XOFF+0,FONT15_YOFF+0,0,0},					//	+, invalid
	{FONT15_XOFF+201,FONT15_YOFF+15,6,14}, 				//	,
	{FONT15_XOFF+215,FONT15_YOFF+15,7,8},				//	-
	{FONT15_XOFF+191,FONT15_YOFF+15,4,12},				//	.
	{FONT15_XOFF+10,FONT15_YOFF+27,10,12}, 				//	/
	{FONT15_XOFF+80,FONT15_YOFF+15,12,12}, 				//	0
	{FONT15_XOFF+92,FONT15_YOFF+15,8,12}, 				//	1
	{FONT15_XOFF+100,FONT15_YOFF+15,12,12},				//	2
	{FONT15_XOFF+112,FONT15_YOFF+15,11,12},				//	3
	{FONT15_XOFF+123,FONT15_YOFF+15,11,12},				//	4
	{FONT15_XOFF+133,FONT15_YOFF+15,12,12},				//	5
	{FONT15_XOFF+145,FONT15_YOFF+15,10,12},				//	6
	{FONT15_XOFF+155,FONT15_YOFF+15,11,12},				//	7
	{FONT15_XOFF+166,FONT15_YOFF+15,13,12},				//	8
	{FONT15_XOFF+179,FONT15_YOFF+15,11,12},				//	9
	{FONT15_XOFF+195,FONT15_YOFF+15,6,12},				//	:
	{FONT15_XOFF+0,FONT15_YOFF+0,0,0},					//	;, invalid
	{FONT15_XOFF+0,FONT15_YOFF+0,0,0},					//	<, invalid
	{FONT15_XOFF+215,FONT15_YOFF+15,7,8},				//	-, map to dash
	{FONT15_XOFF+0,FONT15_YOFF+0,0,0},					//	>, invalid
	{FONT15_XOFF+0,FONT15_YOFF+27,10,12},				//	?
	{FONT15_XOFF+0,FONT15_YOFF+0,0,0},					//	@, invalid
	{FONT15_XOFF+0,FONT15_YOFF+0,14,14},				//	A, 0x61
	{FONT15_XOFF+14,FONT15_YOFF+0,13,12},				//	B
	{FONT15_XOFF+27,FONT15_YOFF+0,10,12},				//	C
	{FONT15_XOFF+37,FONT15_YOFF+0,14,12},				//	D
	{FONT15_XOFF+51,FONT15_YOFF+0,12,12},				//	E
	{FONT15_XOFF+64,FONT15_YOFF+0,12,14},				//	F
	{FONT15_XOFF+76,FONT15_YOFF+0,12,12},				//	G
	{FONT15_XOFF+88,FONT15_YOFF+0,15,14},				//	H
	{FONT15_XOFF+103,FONT15_YOFF+0,8,12},				//	I
	{FONT15_XOFF+111,FONT15_YOFF+0,11,15},				//	J
	{FONT15_XOFF+122,FONT15_YOFF+0,14,12},				//	K
	{FONT15_XOFF+136,FONT15_YOFF+0,9,12},				//	L
	{FONT15_XOFF+145,FONT15_YOFF+0,15,12},				//	M
	{FONT15_XOFF+160,FONT15_YOFF+0,14,12},				//	N
	{FONT15_XOFF+174,FONT15_YOFF+0,12,12},				//	O
	{FONT15_XOFF+186,FONT15_YOFF+0,12,12},				//	P
	{FONT15_XOFF+198,FONT15_YOFF+0,12,12},				//	Q
	{FONT15_XOFF+210,FONT15_YOFF+0,13,14},				//	R
	{FONT15_XOFF+223,FONT15_YOFF+0,11,12},				//	S
	{FONT15_XOFF+234,FONT15_YOFF+0,11,12},				//	T
	{FONT15_XOFF+0,FONT15_YOFF+14,13,12},				//	U
	{FONT15_XOFF+13,FONT15_YOFF+14,12,12},				//	V
	{FONT15_XOFF+25,FONT15_YOFF+14,17,12},				//	W
	{FONT15_XOFF+42,FONT15_YOFF+14,14,12},				//	X
	{FONT15_XOFF+56,FONT15_YOFF+14,11,13},				//	Y
	{FONT15_XOFF+67,FONT15_YOFF+14,13,12},				//	Z
	{FONT15_XOFF+235,FONT15_YOFF+15,9,12},				//	[
	{FONT15_XOFF+0,FONT15_YOFF+0,0,0},					//	\, invalid
	{FONT15_XOFF+244,FONT15_YOFF+15,9,12}				//	]
};

FNTLOOKUP font_lists[] =
{
	{0,font15_panel_info},				// 15 pt
	{0,font7_panel_info},				// 7 pt
	{0,font7N_panel_info}				// 7 pt
};

/******************************************************************************
 Function: void p15_centered(char *pa8,short pa9x,short pa9y)

 By: David Schwartz

 Date: Dec 1994

 Parameters: pa8 - ptr to char string
			 pa9x - x pos
			 pa9y - y pos

 Returns: None

 Description:	setup a 15 point font print and print
******************************************************************************/
void p15_centered(char *pa8,short pa9x,short pa9y)
{
	lm_setup(pf_15point);
	fnt_state.fnt_posx=pa9x;
	fnt_state.fnt_posy=pa9y;
	mk_printf(pa8);

	return;
}

/******************************************************************************
 Function: void p15_centered_soff(char *pa8,short pa9x,short pa9y)

 By: David Schwartz

 Date: Dec 1994

 Parameters: pa8 - ptr to char string
			 pa9x - x pos
			 pa9y - y pos

 Returns: None

 Description:	setup a 15 point font print and print no shadow
******************************************************************************/
void p15_centered_soff(char *pa8,short pa9x,short pa9y)
{
	lm_setup(pf_15point_soff);
	fnt_state.fnt_posx=pa9x;
	fnt_state.fnt_posy=pa9y;
	mk_printf(pa8);

	return;
}

/******************************************************************************
 Function: void p7_centered(char *pa8,short pa9x,short pa9y)

 By: David Schwartz

 Date: Dec 1994

 Parameters: pa8 - ptr to char string
			 pa9x - x pos
			 pa9y - y pos

 Returns: None

 Description:	setup a 15 point font print and print
******************************************************************************/
void p7_centered(char *pa8,short pa9x,short pa9y)
{
	lm_setup(pf_7point);
	fnt_state.fnt_posx=pa9x;
	fnt_state.fnt_posy=pa9y;
	mk_printf(pa8);

	return;
}

void p7_left(char *pa8,short pa9x,short pa9y)
{
	lm_setup(pf_7point_left);
	fnt_state.fnt_posx=pa9x;
	fnt_state.fnt_posy=pa9y;
	mk_printf(pa8);

	return;
}

/******************************************************************************
 Function: void pds_centered(char *pa8,short pa9x,short pa9y)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa8 - ptr to char string
			 pa9x - x pos
			 pa9y - y pos

 Returns: None

 Description:	dave small font centered
******************************************************************************/
void pds_centered(char *pa8,short pa9x,short pa9y)
{
	lm_setup(pf_dave_smallc);
	fnt_state.fnt_posx=pa9x;
	fnt_state.fnt_posy=pa9y;
	mk_printf(pa8);

	return;
}

/******************************************************************************
 Function: void pds_centered_soff(char *pa8,short pa9x,short pa9y)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa8 - ptr to char string
			 pa9x - x pos
			 pa9y - y pos

 Returns: None

 Description:	dave small font centered, no shadow
******************************************************************************/
void pds_centered_soff(char *pa8,short pa9x,short pa9y)
{
	lm_setup(pf_dave_smallc_soff);
	fnt_state.fnt_posx=pa9x;
	fnt_state.fnt_posy=pa9y;
	mk_printf(pa8);

	return;
}


/******************************************************************************
 Function: void pds_left(char *pa8,short pa9x,short pa9y)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa8 - ptr to char string
			 pa9x - x pos
			 pa9y - y pos

 Returns: None

 Description:	dave small font left
******************************************************************************/
void pds_left(char *pa8,short pa9x,short pa9y)
{
	lm_setup(pf_dave_smalll);
	fnt_state.fnt_posx=pa9x;
	fnt_state.fnt_posy=pa9y;
	mk_printf(pa8);

	return;
}

/******************************************************************************
 Function: void pds_right(char *pa8,short pa9x,short pa9y)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa8 - ptr to char string
			 pa9x - x pos
			 pa9y - y pos

 Returns: None

 Description:	dave small font right justify
******************************************************************************/
void pds_right(char *pa8,short pa9x,short pa9y)
{
	lm_setup(pf_dave_smallr);
	fnt_state.fnt_posx=pa9x;
	fnt_state.fnt_posy=pa9y;
	mk_printf(pa8);

	return;
}

/******************************************************************************
 Function: void setup_text_fonts(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	load in text fonts, setup to use printfs, etc
******************************************************************************/
void setup_text_fonts(void)
{
	RECT frame;

	/* load fonts into VRAM */
	frame.w=128;
	frame.h=60;
	frame.x=FONT_15_BASE_X;
	frame.y=FONT_15_BASE_Y;
	texture_level_load(LVL_FONTS,SYNC_LOAD);
	LOADIMAGE(&frame,(ADDRESS *)p2_heap);
	font_lists[0].f_tpage=font_lists[1].f_tpage=font_lists[2].f_tpage=GetTPage(TEXTURE_MODE,TRANS_RATE,FONT_15_BASE_X,FONT_15_BASE_Y);
	DrawSync(0);
	return;
}

/******************************************************************************
 Function: void lm_printf(FNTSETUP *fnt_info)

 By: David Schwartz

 Date: Jan 1995

 Parameters: fnt_info - data to init fnt_state, print string follows

 Returns: None

 Description:	setup the fnt state and print out the string
******************************************************************************/
void lm_printf(FNTSETUP *fnt_info)
{
	lm_setup(fnt_info);
	fnt_info++;					// move to string

	/* convert info */
	mk_printf((char *)fnt_info);

	return;
}

/******************************************************************************
 Function: void lm_printf_p1(FNTSETUP *fnt_info,WORD parm1)

 By: David Schwartz

 Date: Jan 1995

 Parameters: fnt_info - data to init fnt_state, print string follows
			 p1 - parameter 1

 Returns: None

 Description:	setup the fnt state and print out the string has 1 parameter to stick into the string
******************************************************************************/
void lm_printf_p1(FNTSETUP *fnt_info,WORD parm1)
{
	lm_setup(fnt_info);
	fnt_info++;					// move to string

	/* convert info */
	sprintf(fnt_buffer,(char *)fnt_info,parm1);

	stringer();

	return;
}

void pds_centered_p1(char *pa8,short pa9x,short pa9y,WORD parm1)
{
	lm_setup(pf_dave_smallc);
	fnt_state.fnt_posx=pa9x;
	fnt_state.fnt_posy=pa9y;
	/* convert info */
	sprintf(fnt_buffer,pa8,parm1);

	stringer();


	return;
}

void p7_left_p1(char *pa8,short pa9x,short pa9y,WORD parm1)
{
	lm_setup(pf_7point_left);
	fnt_state.fnt_posx=pa9x;
	fnt_state.fnt_posy=pa9y;
	/* convert info */
	sprintf(fnt_buffer,pa8,parm1);

	stringer();

	return;
}



/******************************************************************************
 Function: void printf_p1(char *msg,WORD parm1)

 By: David Schwartz

 Date: Jan 1995

 Parameters: msg - message text
			 p1 - parameter 1

 Returns: None

 Description:	print out the string has 1 parameter to stick into the string
******************************************************************************/
void printf_p1(char *msg,WORD parm1)
{
	/* convert info */
	sprintf(fnt_buffer,msg,parm1);
	stringer();

	return;
}

/******************************************************************************
 Function: void printf_s1(char *msg,char *parm1)

 By: David Schwartz

 Date: Jan 1995

 Parameters: msg - message text
			 p1 - parameter 1 (string)

 Returns: None

 Description:	print out the string has 1 parameter to stick into the string
******************************************************************************/
void printf_s1(char *msg,char *parm1)
{
	/* convert info */
	sprintf(fnt_buffer,msg,parm1);
	stringer();

	return;
}


/******************************************************************************
 Function: void mk_printf(char *cptr)

 By: David Schwartz

 Date: Jan 1995

 Parameters: cptr - text to print

 Returns: None

 Description:	print out the string
******************************************************************************/
void mk_printf(char *cptr)
{
	memcpy(fnt_buffer,cptr,strlen(cptr)+1);
	((char *)current_proc->pa8)=fnt_buffer;

	stringer();

	return;
}

/******************************************************************************
 Function: void stringer(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: fnt_state - init state information
			 fnt_buffer - set to text to print
			 current_proc->pa8 - ptr to fnt_buffer

			str_flags - insert:justify
			bit 17 = set the "no_scroll" bit in oflags2
			bit 16 = 1 insert on obj list
			bit 15 = 1 shadow the characters that get kicked out

			justify =  0 left justify
					   1 center justify
					   2 right justify

 Returns: None

 Description:	read fnt_buffer and create an object lists of text to display
******************************************************************************/
void stringer(void)
{
	LONG str_flags;
	short b2;				// emul b2 register
	XYTYPE initpos;		// a3
	char cur_char;
	FNTTPAGE *fptr;
	OBJECT *obj,*tobj;			// current object

	/* setup correct flag state */
	switch (fnt_state.fnt_routine)
	{
		case STRCNRMOS:
			str_flags=0x18001;
			break;
		case STRCNRMOS_SCR:
			str_flags=0x18001|STR_SCROLL;
			break;
		case STRLNRMOS_SCR:
			str_flags=0x18000|STR_SCROLL;
			break;
		case STRRNRMOS_SCR:
			str_flags=0x18002|STR_SCROLL;
			break;
		case STRCNRMO_SCR:
			str_flags=0x10001|STR_SCROLL;
			break;
		case STRLNRMO_SCR:
			str_flags=0x10002|STR_SCROLL;
			break;
		case STRRNRMO_SCR:
			str_flags=0x10000|STR_SCROLL;
			break;
	}

	string_underline=0;			// clear shadowing and underling flag
	(char *)current_proc->pa8=fnt_buffer;			// set to start of string to display

	string_shadow=(str_flags & STR_SHADOW) ? 1:0;

	/* stringer_noshad */
	spacing();

	initpos.u.xpos=fnt_state.fnt_posx;
	initpos.u.ypos=fnt_state.fnt_posy;

	justify(str_flags & STR_JUSTIFY);

	/* stringr2 */
	while (((short)*((char *)current_proc->pa8))!=0)
	{
		cur_char=*((char *)current_proc->pa8);
		((char *)(current_proc->pa8))++;					// next char
		b0=0;					// not a space, yet

		if (cur_char<' ')
		{
			/* control character */
			switch ((short)cur_char)
			{
				case NEW_XY:
					fnt_state.fnt_posx=(short)(*((char *)(current_proc->pa8)));
					((char *)(current_proc->pa8))++;					// next char
					fnt_state.fnt_posy=(short)(*((char *)(current_proc->pa8)));
					((char *)(current_proc->pa8))++;					// next char
					break;
				case NEW_SPACING:
					fnt_state.fnt_spacing=(WORD)(*((char *)(current_proc->pa8)));
					((char *)(current_proc->pa8))++;					// next char
					break;
				case ASCII_CR:
					fnt_state.fnt_posx=initpos.u.xpos;			// get back to where we started
					fnt_state.fnt_posy=initpos.u.ypos+fnt_spacing.u.ypos;	// move down 1 line
					initpos.u.xpos=fnt_state.fnt_posx;		// store position
					initpos.u.ypos=fnt_state.fnt_posy;
					justify(str_flags & STR_JUSTIFY);
					break;
				case ADD_DYDX:
					fnt_state.fnt_posx+=((short)(*((char *)(current_proc->pa8))));		// add delta x
					((char *)(current_proc->pa8))++;					// next char
					fnt_state.fnt_posy+=((short)(*((char *)(current_proc->pa8))));			// add delta y
					((char *)(current_proc->pa8))++;					// next char
					break;
			}
		}
		else
		{
			if (cur_char==' ')
			{
				/* spacing info setup */
				b2=fnt_state.fnt_posx;
				fnt_state.fnt_posx+=(fnt_spacing.u.xpos+fnt_state.fnt_spacing);

				if (string_underline==0)
					goto SKIP_DRAW;

				b0=b2;
				b2=fnt_state.fnt_posx;		// swap locations
				fnt_state.fnt_posx=b0;

				b0=1;
				cur_char='a';

				goto SKIP_DRAW;
			}

			/* strnchar, legal character draw time */
			fptr=(font_lists[fnt_state.fnt_num].f_ptr)+((WORD)cur_char-0x21);		// get ptr to correct character

			if (text_draw==1) 
			{
				
					setXY4(cdb->curr_prim, \
						fnt_state.fnt_posx,fnt_state.fnt_posy, \
						fnt_state.fnt_posx+fptr->w,fnt_state.fnt_posy, \
						fnt_state.fnt_posx,fnt_state.fnt_posy+fptr->h, \
						fnt_state.fnt_posx+fptr->w,fnt_state.fnt_posy+fptr->h);

					SetUVWH(cdb->curr_prim,fptr->u0,	\
							fptr->v0, \
							fptr->w, \
							fptr->h);

					cdb->curr_prim->tpage=font_lists[fnt_state.fnt_num].f_tpage;
					cdb->curr_prim->clut=get_fore_pal(fnt_state.fnt_pal);
					DrawPrim(cdb->curr_prim);
					DrawSync(0);
				
			}
			else
			{
				if ((obj=get_object())!=NULL)
				{
					obj->oflags2=0;

					if (str_flags & STR_SCROLL)
						obj->oflags2|=M_NOSCROLL;		// set no scroll bit

					obj->osag=fptr;

					obj->oxvel.pos=0;			// clear out position & vel info
					obj->oyvel.pos=0;
					obj->oxpos.pos=0;
					obj->oypos.pos=0;

#if CD_DEBUG
					if ( f_cdbug==1 )
					{
						obj->oid=OID_CD;				// stuff findable ID
						obj->opal=get_fore_pal(YELLOW_p);
						obj->ozval=1000;
					}
					else
					{
						obj->oid=OID_TEXT;				// stuff findable ID
						obj->opal=get_fore_pal(fnt_state.fnt_pal);
						obj->ozval=1;
					}
#else
					obj->oid=OID_TEXT;				// stuff findable ID
					obj->opal=get_fore_pal(fnt_state.fnt_pal);
					obj->ozval=1;
#endif

					obj->oxpos.u.intpos=fnt_state.fnt_posx;		// eliminated anim point stuff
					obj->oypos.u.intpos=fnt_state.fnt_posy;

					/* setup header stuff */
					obj->header.tpage=font_lists[fnt_state.fnt_num].f_tpage;
					obj->header.t_xoffset=(fptr->u0);
					obj->header.t_yoffset=fptr->v0;
					obj->header.t_width=fptr->w;
					obj->header.t_height=fptr->h;

					if (b0==0)
						insert_object(obj,&objlst2);

					/* check for underlining */

					/* check for shadowing */
					if (string_shadow!=0)
					{
						tobj=obj;
						if ((obj=get_object())!=NULL)
						{
							copy_obj(tobj,obj);

							if (fnt_state.fnt_num==FONT_15)
							{
								obj->oxpos.u.intpos+=1;
								obj->oypos.u.intpos+=1;
							}
							else
							{
								obj->oxpos.u.intpos+=1;
								obj->oypos.u.intpos+=1;
							}

							obj->ozval=0;				// normal letters in front of shadows

							obj->opal=get_fore_pal(bpal_black_P);		// shadow is done in black

							insert_object(obj,&objlst2);
						}
					}
				}
			}

			/* strngdun */
			if (b0==0)
			{
				/* if characters is one need special spacing routine */
				if (cur_char=='1')
					fptr++;

				fnt_state.fnt_posx+=(fnt_state.fnt_spacing+fptr->w);	// pass character size and font spacing
			}
			else
			{
				fnt_state.fnt_posx=b2;
			}

			/* strng_bypass_1 */
			if (fnt_state.fnt_sleep!=0)
				process_sleep(fnt_state.fnt_sleep);
		}

SKIP_DRAW:
	}

	/* stringrx */
	return;
}

/******************************************************************************
 Function: void spacing(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: fnt_spacing - sets variables with correct spacing

 Description:	set the spacing for the current font
******************************************************************************/
void spacing(void)
{
	FNTTPAGE *fptr;

	if (fnt_state.fnt_num==FONT_15)
	{
		fnt_spacing.u.xpos=SCX(0x0a);
		fnt_spacing.u.ypos=SCY(0x13);
	}
	else
	{
		fptr=font_lists[fnt_state.fnt_num].f_ptr;			// get pointer to font info

		fptr+='H'-0x21;				// USE 'H' for space and newline stuff

		/* x sizing = (x size of 'H')/2 */
		fnt_spacing.u.xpos=(fptr->w)>>1;

		/* x sizing = 3*(y size of 'H')/2 */
		fnt_spacing.u.ypos=(fptr->h)+((fptr->h)>>1);
	}

	return;
}

/******************************************************************************
 Function: void justify(WORD jindex)

 By: David Schwartz

 Date: Jan 1995

 Parameters: jindex - which justify setting to use
			0 - left
			1 - center
			2 - right

			fnt_buffer - string to justify
			fnt_state.fnt_posx - x pos of string
			fnt_state.fnt_posy - y pos of string

 Returns: fnt_state.fnt_posx - justified x pos
		  fnt_state.fnt_posy - justified y pos

 Description:	justify the string position
******************************************************************************/
void justify(WORD jindex)
{
	switch (jindex)
	{
		case 1:	// center justify
			fnt_state.fnt_posx=fnt_state.fnt_posx-strnglen()/2;
			break;
		case 2: // right justify
			fnt_state.fnt_posx=fnt_state.fnt_posx-strnglen();
			break;
	}

	return;
}

/*number of argument bytes for control character (1-1f) */
BYTE control_tab[] =
{
	2,2,2,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

/******************************************************************************
 Function: short strnglen(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: fnt_buffer - string to display


 Returns: pixel length of string

 Description:	return length of string (pixels)
******************************************************************************/
short strnglen(void)
{
	char *fptr;
	short fontsize=0;
	short xsize;
	FNTTPAGE *fntptr;

	/* x sizing = (x size of 'H')/2 */
	fntptr=font_lists[fnt_state.fnt_num].f_ptr;			// get pointer to font info
	xsize=((fntptr+'H'-0x21)->w)>>1;

	fptr=((char *)current_proc->pa8);

	while (*fptr!=0 && *fptr!=ASCII_CR)
	{
		if (*fptr<0x20)
		{
			fptr+=control_tab[(WORD)*fptr];		// skip past control command args
		}
		else
		{
			/* process character */
			if (*fptr==' ')
				fontsize+=fnt_spacing.u.xpos;
			else
			{
				if (*fptr=='1')
					fontsize+=(fntptr+'2'-0x21)->w;
				else fontsize+=(fntptr+(WORD)(*fptr)-0x21)->w;
			}

			fontsize+=fnt_state.fnt_spacing;		// add in space between chars
		}
		fptr++;					// next char
	}
	return(fontsize);
}

/******************************************************************************
 Function: void copy_obj(OBJECT *src,OBJECT *dest)

 By: David Schwartz

 Date: Jan 1995

 Parameters: src - object to copy
			 dest - object to store info


 Returns: None

 Description:	copy's the src obj into the dest obj, without copying the links
******************************************************************************/
void copy_obj(OBJECT *src,OBJECT *dest)
{
	memcpy(&(dest->ograv),&(src->ograv),sizeof(OBJECT)-sizeof(OBJECT *)-sizeof(void*)-sizeof(PROCESS *));
	dest->oplink=0;
}
