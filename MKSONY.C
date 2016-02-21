/******************************************************************************
 File: mksony.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III SONY SPECIFIC ROUTINES
******************************************************************************/

/* INCLUDES */
#include "system.h"
#include "mksony.h"
#include "mkbkgd.h"
#include "mkobj.h"
#include "mkos.h"
#include "mkgame.h"
#include "mkpal.h"
#include "mkmain.h"
#include "mkinit.h"
#include "mkguys.h"
#include "mkfile.h"

/*
 * MODULE RAM
 */

/*
 * MODULE PROTOTYPES
 */
static void stack_switch_bits(LONG,WORD);

/******************************************************************************
 Function: void init_sony_system(void)

 By: David Schwartz

 Date: Aug 9, 1994

 Parameters: None

 Returns: None

 Description:	VBLANK ROUTINE
******************************************************************************/
void init_sony_system(void)
{
	int loop;
	RECT clear;
	BLK_FILL clr;

	//PadInit(0);											/* reset pad */
	ResetGraph(0);										/* reset graphics subsystem(0:cold,1:warm) */
	SetGraphDebug(0);									/* graph debug mode */

	SetBlockFill(&clr);
	setRGB0(&clr,0,0,0);
	setXY0(&clr,0,0);
	setWH(&clr,1024,512);
	DrawPrim((u_long *)&clr);

	/* initialize environment for double buffer */
	SetDefDrawEnv(&db[0].draw,BUF0_X_COORD,BUF0_Y_COORD,BUFFER_WIDTH,BUFFER_HEIGHT);		/* buffer 0 */
	SetDefDispEnv(&db[1].disp,BUF0_X_COORD,BUF0_Y_COORD,BUFFER_WIDTH,BUFFER_HEIGHT);

	SetDefDrawEnv(&db[1].draw,BUF1_X_COORD,BUF1_Y_COORD,BUFFER_WIDTH,BUFFER_HEIGHT);		/* buffer 1 */
	SetDefDispEnv(&db[0].disp,BUF1_X_COORD,BUF1_Y_COORD,BUFFER_WIDTH,BUFFER_HEIGHT);

#if PAL_VERSION==1
/*these are the offsets of the active screen area from the top left of the  */
/*screen  pal(320*256)= 21      ntsc(320*240) = 28 */
/*this is to center the letter box effect */

	db[1].disp.screen.x = db[0].disp.screen.x = 1;		//SCREEN_X;
	db[1].disp.screen.y = db[0].disp.screen.y = 21;		//SCREEN_Y;
/*this line sets the hardware to pal mode */
	SetVideoMode(MODE_PAL);
#endif

	/* clear ordering tables */
 	ClearOTag(db[0].ot, OTSIZE);
 	ClearOTag(db[1].ot, OTSIZE);

	/* setup buffer systems */
	cdb  = (cdb==db)? db+1: db;					/* set double buffer ID */
	cdb->curr_prim=cdb->obj_prim;				/* set first poly prim */
	cdb->ot_ptr=cdb->ot+1;						/* set ot ptr */

	/* set auto clear false for draw buffer */
	db[0].draw.isbg=FALSE;
	db[1].draw.isbg=FALSE;

	/* setup clear primitives */
	SetBlockFill(&db[0].clear);					/* setup & and CLEAR primitive, buf 0*/
	db[0].clear.x0=BUF0_X_COORD;
	db[0].clear.y0=BUF0_Y_COORD;
	db[0].clear.w=BUFFER_WIDTH;
	db[0].clear.h=BUFFER_HEIGHT;
	setRGB0(&db[0].clear,0,0,0);
	cdb->primcnt++;
	AddPrim(&db[0].ot,&db[0].clear);

	SetBlockFill(&db[1].clear);					/* setup & and CLEAR primitive, buf 1*/
	db[1].clear.x0=BUF1_X_COORD;
	db[1].clear.y0=BUF1_Y_COORD;
	db[1].clear.w=BUFFER_WIDTH;
	db[1].clear.h=BUFFER_HEIGHT;
	setRGB0(&db[1].clear,0,0,0);
	cdb->primcnt++;
	AddPrim(&db[1].ot,&db[1].clear);

	/* initialize primitives */
	for (loop=0;loop<OBJPRIM_SIZE;loop++)
	{
		SetPolyFT4(&db[0].obj_prim[loop]);
		setRGB0(&db[0].obj_prim[loop],128,128,128);

		SetPolyFT4(&db[1].obj_prim[loop]);
		setRGB0(&db[1].obj_prim[loop],128,128,128);
	}

	/* init control panel sprites primitives */
	for (loop=0;loop<SCORE_RAM_CNT;loop++ )
	{
		SetSprt(&db[0].cp[loop]);
		setRGB0(&db[0].cp[loop],128,128,128);

		SetSprt(&db[1].cp[loop]);
		setRGB0(&db[1].cp[loop],128,128,128);
	}

	/* init floor sprites */
	SetSprt(&db[0].floor[0]);
	setRGB0(&db[0].floor[0],128,128,128);

	SetSprt(&db[1].floor[0]);
	setRGB0(&db[1].floor[0],128,128,128);

	SetSprt(&db[0].floor[1]);
	setRGB0(&db[0].floor[1],128,128,128);

	SetSprt(&db[1].floor[1]);
	setRGB0(&db[1].floor[1],128,128,128);

	/* init box primitives */
	for (loop=0;loop<BOXPRIM_SIZE;loop++)
	{
		SetPolyF4(&db[0].box_prim[loop]);					// buffer 0 bar 0
		setRGB0(&db[0].box_prim[loop],128,128,128);

		SetPolyF4(&db[1].box_prim[loop]);					// buffer 0 bar 0
		setRGB0(&db[1].box_prim[loop],128,128,128);
	}

	/* init joy remap stuff */
	game_map_tbl[0]=pad1_map_tbl[0]=pad2_map_tbl[0]=0x0004;			//r1,block
	game_map_tbl[1]=pad1_map_tbl[1]=pad2_map_tbl[1]=0x0004;			//l1,block
	game_map_tbl[2]=pad1_map_tbl[2]=pad2_map_tbl[2]=0x0008;			//r2,run
	game_map_tbl[3]=pad1_map_tbl[3]=pad2_map_tbl[3]=0x0008;			//l2,run

	game_map_tbl[4]=pad1_map_tbl[4]=pad2_map_tbl[4]=0x0020;			//triangle, hk
	game_map_tbl[5]=pad1_map_tbl[5]=pad2_map_tbl[5]=0x0040;			//circle, lk
	game_map_tbl[6]=pad1_map_tbl[6]=pad2_map_tbl[6]=0x0080;			//x, lp
	game_map_tbl[7]=pad1_map_tbl[7]=pad2_map_tbl[7]=0x0010;			//square, hp

	game_map_tbl[8]=pad1_map_tbl[8]=pad2_map_tbl[8]=0x0100;			//coll box, (SELECT BUTTON)
	game_map_tbl[9]=pad1_map_tbl[9]=pad2_map_tbl[9]=0x0200;			//run, needed for sony box controllers
	game_map_tbl[10]=pad1_map_tbl[10]=pad2_map_tbl[10]=0x0400;		//block, needed for sony box controllers
	game_map_tbl[11]=pad1_map_tbl[11]=pad2_map_tbl[11]=0x0800;		//start

	game_map_tbl[12]=pad1_map_tbl[12]=pad2_map_tbl[12]=0x1000;		//up
	game_map_tbl[13]=pad1_map_tbl[13]=pad2_map_tbl[13]=0x2000;		//right
	game_map_tbl[14]=pad1_map_tbl[14]=pad2_map_tbl[14]=0x4000;		//down
	game_map_tbl[15]=pad1_map_tbl[15]=pad2_map_tbl[15]=0x8000;		//left

/*********************************************************
 ** 			CRITICAL SECTION CODE 					**
 *********************************************************/
	EnterCriticalSection();

	/* Setup Vblank Interrupt Triggering */
#if 0
	vbEvent=OpenEvent(RCntCNT3,EvSpINT,EvMdINTR,mk_vblank_int);
	DisableEvent(vbEvent);
	SetRCnt(RCntCNT3);
#endif
	VSyncCallback(mk_vblank_int);

	/* hblank counter, used for rnd #s */
	SetRCnt(RCntCNT1,32767,RCntMdNOINTR);
	StartRCnt(RCntCNT1);

	ExitCriticalSection();
/*********************************************************
 ** 			CRITICAL SECTION CODE 					**
 *********************************************************/

	return;
}

/******************************************************************************
 Function: void mk_vblank_int(void)

 By: David Schwartz

 Date: Aug 9, 1994

 Parameters: None

 Returns: None

 Description:	VBLANK ROUTINE
				1) vblank bookeeping
				2) stack various switch processes based on joy input
				3) screen drawing and switching stuff
				4) coin handling processing (MIGHT BE ARCADE SPECIFIC ONLY)
******************************************************************************/
static WORD irq_tick;
extern short or1;

void mk_vblank_int(void)
{
	JOYPAD swprev1,swprev2;
	JOYPAD sw0,swstate;

	/* vblank book keeping */
	irq_timer++;
	if ( f_pause<PAUSE_OFF )
		irq_tick++;
	tick=(irq_tick*55)/60;

	/* switch scan routines */
	swcurr.padvalue=PadRead(SONY_PAD_ID);		/* read current joystick (P2:P1) */
	or1+=(swcurr.u.p1pad+swcurr.u.p2pad);

	/* remap input to correct values */
	if ( gstate==GS_FIGHTING || gstate==GS_ROUND_INTRO)
	{
		swcurr.u.p1pad=joy_remap(swcurr.u.p1pad,pad1_map_tbl);
		swcurr.u.p2pad=joy_remap(swcurr.u.p2pad,pad2_map_tbl);
	}
	else
	{
		swcurr.u.p1pad=joy_remap(swcurr.u.p1pad,game_map_tbl);
		swcurr.u.p2pad=joy_remap(swcurr.u.p2pad,game_map_tbl);
	}

	/* save switch values */
	swprev1.padvalue=swtemp1.padvalue;					/* retrieve last joystick (P2:P1) */
	swprev2.padvalue=swtemp2.padvalue;					/* retrieve last last joystick (P2:P1) */

	swtemp2.padvalue=swtemp1.padvalue;					/* update last last joystick */
	swtemp1.padvalue=swcurr.padvalue;						/* update last joystick */

	if ((sw0.padvalue=swcurr.padvalue ^ swprev1.padvalue)) 			/* bits that changed */
	{
		/* closed switch tests */
		if ((swstate.padvalue=sw0.padvalue & swcurr.padvalue))		/* bits that changed and just closed */
		{
		 	swstate.padvalue &= ~(swprev1.padvalue | swprev2.padvalue);   	/* only pass if last 2 stats --> open, open */
		 	stack_switch_bits(swstate.padvalue,SW_STACK_CLOSE_OFFSET);
		}

		/* open switch tests */
		swstate.padvalue = ~swcurr.padvalue & swprev1.padvalue;		/* bits that changed and just opened */
		stack_switch_bits(swstate.padvalue,SW_STACK_OPEN_OFFSET);
	}

	//-NOTE- routines that belong here gravity & scroll are with mkos.c

	/* handle coin processing */

	return;
}

/******************************************************************************
 Function: void stack_switch_bits(LONG swstate,WORD swoffset)

 By: David Schwartz

 Date: Aug 9, 1994

 Parameters: swstate = bits to add to switch state
			 swoffset = base offset into table

 Returns: None

 Description:	This routine will add an entry to the switch stack for each
				bit set in the swstate.  These switches are then retrieved in the
				exec(main) loop and joystick processes are setup.
******************************************************************************/
void stack_switch_bits(LONG swstate,WORD swoffset)
{
	LONG sw0;
	WORD i=0;

	while (swstate)
	{
		if ((sw0=swstate & 1))			/* is lowest bit set, if so add to switch stack */
		{
			if (swstack>=&swstmn[SWSTCK_SIZE])	/* check if we have gone past end of stack */
				return;

			*swstack=i+swoffset;		/* push switch number onto stack (0-31)+OFFSET */
			swstack++;
		}

		i++;							/* next bit position */

		swstate=(0xfffffffe & swstate)>>1;	/* remove bit and shift next bit down */
	}

	return;
}

/******************************************************************************
 Function: void ermac(WORD error)

 By: David Schwartz

 Date: Aug 9, 1994

 Parameters: error #

 Returns: None

 Description:	Report to terminal an error an lock

 System Errors:
  1 = 1 player state has 2 players active (THIS IS NOT GOOD)
  2 = winner_status bad (JOY)
  3 = exobj_er (MKUTIL)
  4 = exprc_er (MKUTIL)
  5 = object_setup could not get an object (mkutil)
  6 = decomp buffer outage
  7 =
  8 = swpal ran out of pals
  9 = rgbstuff could not find a pal
 10 = rgbstuff could not find a pal
 11 = color cycler could not find a pal
 12 = scroll error
 13 = reached code that was not expected to execute (mkutil, frame_a9)
 14 = delobj, cant find object to delete (mkobj)
 15 = alloc_vram_perm, cant alloc cache entry for perm image (mkutil.c)
 16 = no more process left to allocate, (mkos.c)
******************************************************************************/
void ermac(WORD error)
{
	int wait=0;

#if 0
	switch (error)
	{
		case 1:
			CPRT("1 player state has 2 players active (THIS IS NOT GOOD)");
			break;
		case 2:
			CPRT("winner_status bad (MKJOY)");
			break;
		case 3:
			CPRT("exobj_er (MKUTIL)");
			break;
		case 4:
			CPRT("exprc_er (MKUTIL)");
			break;
		case 5:
			CPRT("object_setup could not get an object (mkutil)");
			break;
		case 6:
			CPRT("ran out of image decomp buffer, (mksony.c)");
			break;
		case 7:
			break;
		case 8:
			CPRT("swpal ran out of pals");
			break;
		case 9:
			CPRT(" rgbstuff could not find a pal");
			break;
		case 10:
			CPRT("rgbstuff could not find a pal");
			break;
		case 11:
			CPRT("color cycler could not find a pal");
			break;
		case 12:
			CPRT("scroll error");
			break;
		case 13:
			CPRT("frame_a9 code wasn't to be executed (mkutil)");
			break;
		case 14:
			CPRT("delobj, object could not be found (mkobj)");
			break;
		case 15:
			CPRT("alloc_vram_perm, can't alloca vram cache entry (mkutil.c)");
			break;
		case 16:
			CPRT("process_create, no more process left to allocate (mkos.c)");
			break;
		default:
			CPRT("UNKNOWN ERROR");
	}
#endif

	while(wait==0)
		POLLING;
	return;
}

/******************************************************************************
 Function: WORD joy_remap(WORD input,WORD *tbl)

 By: David Schwartz

 Date: Jan 1995

 Parameters: input - input bit pattern from joystick
			 tbl - convert table lookup maps input to output

 Returns: remapped joystick buttons

 Description:	This routine will remap the joystick values, based on the user selctions
******************************************************************************/
WORD joy_remap(WORD input,WORD *tbl)
{
	short i;
	WORD output=0;

	for (i=0;i<MAP_TBL_ENTRIES;i++)
	{
		if (input & 1)
		{
			/* bit set, remap */
			output|=*(tbl+i);		// map table entry contains correct bit to set in output
		}

		input>>=1;					// shift down next bit
	}

	return(output);
}



void unsonyRLE(BYTE *, BYTE **);
void expand_simpleRLE(BYTE *, BYTE **);
void unbetterSimpleRLE(BYTE *src, BYTE **dst);

/******************************************************************************
 Function: BYTE *uncompress_image(BYTE *src)

 By: David Schwartz

 Date: Jan 1995

 Parameters: src - src address compressed image

			 Each image has a 32 bit header:
				bits [31..24] = compression type (0=None, 1=64 RLE, 2=256 RLE)
				bits [23..0] = # of bytes in src compress data + 4 bytes for header

 Returns: dest - dest decompression buffer area

 Description:	This routine will uncompress data via the correct routine.  The results
				will be dumped to the current decompress buffer
******************************************************************************/
BYTE *uncompress_image(BYTE *src)
{
	BYTE *dest;
	BYTE *start;
	WORD i;

	switch (((*( (LONG *)src)) >>24)&0x3f)
	{
		case 0:		// no compression
			start=src+sizeof(LONG);
			break;
		case 1:		// 64 color comp
			start=dest=(BYTE *)idcomp_ptr;
			unsonyRLE(src,&dest);
			idcomp_ptr=(LONG *)(((LONG)(dest+3)) & (~3));
   			break;
		case 2:		// 256 color comp
			start=dest=(BYTE *)idcomp_ptr;
			expand_simpleRLE(src,&dest);
			idcomp_ptr=(LONG *)(((LONG)(dest+3)) & (~3));
			break;
		case 3:
			start=dest=(BYTE *)idcomp_ptr;
			unbetterSimpleRLE(src,&dest);
			idcomp_ptr=(LONG *)(((LONG)(dest+3)) & (~3));
			break;
	}

#if DEBUG
	if ( (idcomp_ptr-idcomp_base)>50*1024 && gstate==GS_FIGHTING )
	{
		LOCKUP;
	}
#endif

	return(start);
}

/******************************************************************************
 Function: void unsonyRLE(BYTE *src, BYTE **dst)

 By: David Schwartz

 Date: Jan 1995

 Parameters: src - src address compressed image
			 dest - ptr to dest decompression buffer area

 Returns: None

 Description:	This routine will uncompression a src image into a decomp buffer (256 color)
******************************************************************************/
void expand_simpleRLE(BYTE *src, BYTE **dst)
{
	int i;
	signed char *s, *d, ch;
	long srcBytes;

	s = (signed char *)src + sizeof(long);
	d = (signed char *)(*dst);
	srcBytes = (*(long *)src & 0x00FFFFFF) - sizeof(long);
	while (srcBytes > 0)
		if ((i = *s++) < 0)
		{               					/* if < 0 then zero based count */
			ch = *s++;                      /* byte to be repeated */
			for (srcBytes -= 2; i <= 0; i++)
				*d++ = ch;
		}
		else
			for (srcBytes -= i + 2; i >= 0; i--)
				*d++ = *s++;                /* zero based bytes of data */

	*dst=d;
}  /* expand_simpleRLE */

/******************************************************************************
 Function: void unsonyRLE(BYTE *src, BYTE **dst)

 By: David Schwartz

 Date: Jan 1995

 Parameters: src - src address compressed image
			 dest - ptr to dest decompression buffer area

 Returns: None

 Description:	This routine will uncompression a src image into a decomp buffer (64 color)
******************************************************************************/
void unsonyRLE(BYTE *src, BYTE **dst)
{
	short someshort;
	long Lrunlen;
	int complete, partial;
	BYTE *packdata, *packlen;
	WORD *buffer;

	static void *table[32] = {&&case2, &&case4, &&case6, &&case8, &&case10, &&case12,
							  &&case14, &&case16, &&case18, &&case20, &&case22, &&case24,
							  &&case26, &&case28, &&case30, &&case32, &&case34, &&case36,
							  &&case38, &&case40, &&case42, &&case44, &&case46, &&case48,
							  &&case50, &&case52, &&case54, &&case56, &&case58, &&case60,
							  &&case62, &&case64};

	packdata = src;
	packlen = packdata + (*((long*)packdata) & 0x00FFFFFF);
	buffer = (WORD *)(*dst);

	for (packdata += 4; packdata < packlen; ) {
		someshort = *(((WORD *)packdata)++);

		if (someshort & 0x8000) {
			Lrunlen = someshort & 0x01FF;
			someshort &= 0x7E00;
			someshort = (someshort >> 1) | (someshort >> 9);
			complete = Lrunlen >> 6;
			partial = Lrunlen & 63;

			if (!partial) {
				partial = 64;
				complete--;
			}

			goto *table[(partial >> 1) - 1];

			case64:	*(((WORD *)buffer)++) = someshort;
			case62:	*(((WORD *)buffer)++) = someshort;
			case60:	*(((WORD *)buffer)++) = someshort;
			case58:	*(((WORD *)buffer)++) = someshort;
			case56:	*(((WORD *)buffer)++) = someshort;
			case54:	*(((WORD *)buffer)++) = someshort;
			case52:	*(((WORD *)buffer)++) = someshort;
			case50:	*(((WORD *)buffer)++) = someshort;
			case48:	*(((WORD *)buffer)++) = someshort;
			case46:	*(((WORD *)buffer)++) = someshort;
			case44:	*(((WORD *)buffer)++) = someshort;
			case42:	*(((WORD *)buffer)++) = someshort;
			case40:	*(((WORD *)buffer)++) = someshort;
			case38:	*(((WORD *)buffer)++) = someshort;
			case36:	*(((WORD *)buffer)++) = someshort;
			case34:	*(((WORD *)buffer)++) = someshort;
			case32:	*(((WORD *)buffer)++) = someshort;
			case30:	*(((WORD *)buffer)++) = someshort;
			case28:	*(((WORD *)buffer)++) = someshort;
			case26:	*(((WORD *)buffer)++) = someshort;
			case24:	*(((WORD *)buffer)++) = someshort;
			case22:	*(((WORD *)buffer)++) = someshort;
			case20:	*(((WORD *)buffer)++) = someshort;
			case18:	*(((WORD *)buffer)++) = someshort;
			case16:	*(((WORD *)buffer)++) = someshort;
			case14:	*(((WORD *)buffer)++) = someshort;
			case12:	*(((WORD *)buffer)++) = someshort;
			case10:	*(((WORD *)buffer)++) = someshort;
			case8:	*(((WORD *)buffer)++) = someshort;
			case6:	*(((WORD *)buffer)++) = someshort;
			case4:	*(((WORD *)buffer)++) = someshort;
			case2:	*(((WORD *)buffer)++) = someshort;

			if (complete--)
				goto case64;

		}
		else *(((WORD *)buffer)++) = someshort;
	}

	*dst=(BYTE *)buffer;
}  /* unsonyRLE */

/******************************************************************************
 Function: void unbetterSimpleRLE(BYTE *src, BYTE **dst)

 By: David Schwartz

 Date: Jan 1995

 Parameters: src - src address compressed image
			 dest - ptr to dest decompression buffer area

 Returns: None

 Description:	This routine will uncompression a src image into a decomp buffer (64 color)
******************************************************************************/

#define NEXTBYTE(var)   \
switch (state++) {      \
	case 0:                                     \
		c1 = *src++;                            \
		c2 = *src++;                            \
		c3 = *src++;                            \
		var = c1 >> 2;                          \
		break;                                  \
	case 1:                                     \
		var = (c2 >> 4) | ((c1 << 4) & 0x30);   \
		break;                                  \
	case 2:                                     \
		var = ((c2 << 2) & 0x3C) | (c3 >> 6);   \
		break;                                  \
	case 3:                                     \
		var = c3 & 0x3F;                        \
		state = 0;                              \
		break;                                  \
}

void unbetterSimpleRLE(BYTE *src, BYTE **dst)
{
	int i, state;
	BYTE ch, c1, c2, c3;
	long srcBytes;
	BYTE *buffer;

	srcBytes = (*(long *)src & 0x00FFFFFF) - (sizeof(long));
	srcBytes = srcBytes + (srcBytes / 3) - (src[3] >> 6);
	src += sizeof(long);
	buffer = *dst;
	state = 0;
	while (srcBytes > 0)
	{
		NEXTBYTE(i);
		if (i < 32)
		{                       			/* if < 32 then zero based count */
			NEXTBYTE(ch);                   /* byte to be repeated */
			for (srcBytes -= 2; i >= 0; i--)
				*buffer++ = ch;
		}
		else
		{
			i -= 32;
			for (srcBytes -= i + 2; i >= 0; i--)
				NEXTBYTE(*buffer++);           /* zero based bytes of data */
		}
	}

	*dst=buffer;
}  /* unbetterSimpleRLE */



#if DEBUG==1
u_short DLoadTPage(u_long *pix, int tp, int abr, int x, int y, int w, int h)
{
	volatile WORD wait;

	if ((ADDRESS)pix<0x80000000 || (ADDRESS)pix>0x807d0000)
	{
		wait=0;
		while (wait==0)
		{
			POLLING;
		}

		return(0);

	}
	else
	{
		return(LoadTPage(pix,tp,abr,x,y,w,h));
	}
}

int DLoadImage(RECT *rect, u_long *p)
{
	volatile WORD wait;

	if ((ADDRESS)p<0x80000000 || (ADDRESS)p>0x807d0000)
	{
		wait=0;
		while (wait==0)
		{
			POLLING;
		}

		return(0);
	}
	else
	{
		return(LoadImage(rect,p));
	}
}
#endif

/******************************************************************************
 Function: void load_perm_art(void)

 By: David Schwartz

 Date: April 1995

 Parameters: None

 Returns: None

 Description:	load in text fonts and control panel art (perm) only need to do once
******************************************************************************/
void load_perm_art(void)
{

	/* load control panel graphics */
	texture_level_load(LVL_CONTROL,SYNC_LOAD);
	control_floor_tpage=LOADTPAGE((ADDRESS *)p2_heap,TEXTURE_MODE,0,CP_TEXT_X,CP_TEXT_Y,256,256);		// 0 trans for score bar
	DrawSync(0);

	/* load loading message graphics */
	texture_level_load(LVL_LOADING,SYNC_LOAD);
	load_tpage=LOADTPAGE((ADDRESS *)p2_heap,TEXTURE_MODE,0,LOADING_BASE_X,LOADING_BASE_Y,256,70);		// 0 trans for score bar
	DrawSync(0);

	/* load text graphics */
	setup_text_fonts();

	return;
}

