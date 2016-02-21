/******************************************************************************
 File: mkfade.c

 Date: march 1995

 (C) Williams Entertainment

 Mortal Kombat III Fade Functions
******************************************************************************/

#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkobj.h"
#include "mkos.h"
#include "mkgame.h"
#include "mkutil.h"
#include "mkani.h"
#include "mkguys.h"
#include "mkpal.h"
#include "mkfade.h"


/******************************************************************************
 Fade Tables
******************************************************************************/

WORD fadein_tab[]={0,0,8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128,0xffff};

WORD fadeout_tab[]={120,112,104,96,88,80,72,64,56,48,40,32,24,16,8,0,0xffff};

WORD fadeout_mercy_tab[]=
{
	120,112,104,96,88,80,72,64,
	64,64,64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,64,64,
	64,72,80,88,96,104,112,120,128,0xffff
};

WORD fadeins_tab[]={0,0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,0xffff};

WORD fadeblak_tab[]={0,0xffff};

WORD fast_fade_tab[]={120,104,88,72,56,40,24,8,0,0xffff};

WORD fast_fadein_tab[]={0,8,24,40,56,72,88,104,128,0xffff};

WORD raiden_fade_tab[]={48,0xffff};

WORD raiden_unfade_tab[]={128,0xffff};

WORD join_in_tab1[]={56,0xffff};

WORD join_in_tab2[]={48,40,32,24,16,8,0,0xffff};

WORD fadein_white_tab[]=
{
	WTOP-(DTOP*1),
	WTOP-(DTOP*2),
	WTOP-(DTOP*3),
	WTOP-(DTOP*4),
	WTOP-(DTOP*5),
	WTOP-(DTOP*6),
	WTOP-(DTOP*7),
	WTOP-(DTOP*8),
	WTOP-(DTOP*9),
	WTOP-(DTOP*10),
	WTOP-(DTOP*11),
	WTOP-(DTOP*12),
	WTOP-(DTOP*13),
	WTOP-(DTOP*14),
	WTOP-(DTOP*15),
	WTOP-(DTOP*16),
	WTOP-(DTOP*17),
	WTOP-(DTOP*18),
	WTOP-(DTOP*19),
	WTOP-(DTOP*20),

	WTOP-(DTOP*21),
	WTOP-(DTOP*22),
	WTOP-(DTOP*23),
	WTOP-(DTOP*24),
	WTOP-(DTOP*25),
	WTOP-(DTOP*26),
	WTOP-(DTOP*27),
	WTOP-(DTOP*28),
	WTOP-(DTOP*29),
	WTOP-(DTOP*30),

	WTOP-(DTOP*31),
	WTOP-(DTOP*32),
	WTOP-(DTOP*33),
	WTOP-(DTOP*34),
	WTOP-(DTOP*35),
	WTOP-(DTOP*36),
	WTOP-(DTOP*37),
	WTOP-(DTOP*38),
	WTOP-(DTOP*39),
	WTOP-(DTOP*40),

	WTOP-(DTOP*41),
	WTOP-(DTOP*42),
	WTOP-(DTOP*43),
	WTOP-(DTOP*44),
	WTOP-(DTOP*45),
	WTOP-(DTOP*46),
	WTOP-(DTOP*47),
	WTOP-(DTOP*48),
	WTOP-(DTOP*49),
	WTOP-(DTOP*50),
	128,					// end up normal colors
	0xffff
};

WORD fadewhite_tab[]={WTOP,0xffff};

WORD *all_palettes[]={NULL};
#define sans_boonpal all_palettes

WORD *death_fade_excludes[] =
{
	bpal_flash1_P,
	bpal_flash2_P,
	KANORED_P,    	// 0 - kano
	SBGREEN_P,    	// 1 - sonya
	JAXPRP_P,     	// 2 - jax
	INBLU_P,	     	// 3 - indian
	SZBLU_P,	     	// 4 - turk (subzero)
	OBRED_P,	     	// 5 - swat
	LPRP_P,	     	// 6 - lia
	R1PAL1_P,     	// 7 - robo1
	R2PAL1_P,	// 8 - robo2
	KUNG_P,		// 9 - kung lao
	TSKTAN_P,	// 10 - tusk
	FGRED_P,		// 11 - she goro
	TSYEL_P,		// 12 - shang tsun
	LKRED_P,		// 13 - kang
	BGPAL1_P,	// 14 - shao kahn
	SMOKE1_P,
	SMOKE2_P,

	KANOBLU_P,	//  0 - kano
	SBLTBLU_P,	//  1 - sonya
	JZXGRN_P,	//  2 - jax
	INYEL_P,		//  3 - indian
	SZBLUGR_P,	//  4 - subzero
	OLTBLU_P,	//  5 - swat
	LBLU_P,		//  6 - lia
	R1PAL2_P,     	//  7 - robo1
	R2PAL2_P,	//  8 - robo2
	KUNGBLU_P,	//  9 - kung lao
	TSBLU_P,		// 10 - tusk
	FGBLUE_P,	// 11 - she goro
	TSRED_P,		// 12 - shang tsung
	LKYELO_P,	// 13 - kang
	SCORE_P,
	BLOOD_P,
	OIL_P,
	SHEEBL_P,
	WHITE_p,
	KIT1_P,		// 15 - kitana
	JADE1_P,		// 16 - jade
	MIL1_P,		// 17 - mileena
	SCORP1_P,	// 18 - scorpion
	REP1_P,		// 19 - reptile
	ERMAC1_P,	// 21 - ermac
	SUBZ1_P,		// 20 - classic subzero

	KIT2_P,		// 15 - kitana
	JADE2_P,		// 16 - jade
	MIL2_P,		// 17 - mileena
	SCORP2_P,
	REP2_P,
	ERMAC2_P,
	SUBZ2_P,
	noob_p,		// smoke
	NJSMOKE_P,	// 21 - ninja smoke
	NULL
};

/******************************************************************************
 Function: void fade_this_pal(WORD pa0,WORD pa5,WORD pa10,WORD *pa11)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa0 - palette offset
			 pa5 - sleep time between fades
			 pa10 - rgb flags
			 pa11 - fade table

 Returns: None

 Description: 	setup a single palette fade
******************************************************************************/
void fade_this_pal(WORD pa0,WORD pa5,WORD pa10,WORD *pa11)
{
	PROCESS *ptemp;

	ptemp=CREATE(PID_FADEPAL,single_pal_fader);
	(LONG)ptemp->pa9=pa0;
	(LONG)ptemp->pa8=pa5;
	(LONG)ptemp->a10=pa10;
	(WORD *)ptemp->a11=pa11;
	return;
}

/******************************************************************************
 Function: void single_pal_fader(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: current_proc->pa9 - palette offset (a0)
			 current_proc->pa8 - sleep time between fades (a5)
			 current_proc->a10 - rgb flags (a10)
			 current_proc->a11 - fade table (a11)

 Returns: None

 Description: 	single palette fade
******************************************************************************/
void single_pal_fader(void)
{
	WORD l;
	PALINFO *palptr;

	l=(((LONG)current_proc->pa9)-getClut(FORE_CLUT_START_X,FORE_CLUT_START_Y))>>CLUT_ID_SHIFT;
	if (l>=NUM_PALS) LOCKUP;
	palptr=palram+l;

	do
	{
		newfade((WORD *)(&(current_proc->p_context.c_stack)),palptr->palid);
		palset_xfer(FORE_CLUT_START_X+((l%CLUT_FACTOR)<<CLUT_SHIFT_SIZE),FORE_CLUT_START_Y+(l/CLUT_FACTOR),(WORD *)(&(current_proc->p_context.c_stack)));
		process_sleep((LONG)current_proc->pa8);
	}
	while(GET_WORD(current_proc->a11)!=0xffff);

	process_suicide();
}

/******************************************************************************
 Function: void newfade(WORD *pa0,WORD *pa1)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa0 - dest ram for palette
			 pa1 - source for palette
			 current_proc->a11 - color multi

 Returns: None

 Description: 	each color in palette will be mult by a2 then div by 128
				apply the color mult and do one iteration on the pal
******************************************************************************/
void newfade(WORD *pa0,WORD *pa1)
{
	LONG ta14;
	LONG ta3,ta5,ta7;
	WORD high=0x000;
	WORD ta2;

	ta2=GET_WORD(current_proc->a11);

	ta14=GET_LONG(pa1)++;					// # of entries
	*((LONG *)pa0)=ta14;					// write count
	pa0+=2;									// next dest

	do
	{
		ta3=ta5=ta7=GET_WORD(pa1)++;

		ta3&=0x001f;				// red
		ta5=(ta5 & 0x03e0)>>5;		// green
		ta7=(ta7 & 0x7c00)>>5;		// blue

		/* red */
		ta3+=ta2;
		if (ta3>31)
			ta3=31;

		/* green */
		ta5-=ta2;
		if ( ta5 & 0x8000 )
			ta5=0;

		/* blue */
		ta7-=ta2;
		if ( ta7 & 0x8000 )
			ta7=0;

		ta3|=(ta5<<5);
		ta3|=(ta7<<10);
		ta3|=high;					// 1st entry will be zero all others will have bit sets

		if ( high==0 )
			ta3=0;					// keep color 0 black trans

		high=0x8000;

		*pa0=(WORD)ta3;
		pa0++;
	}
	while(--ta14>0);

	((WORD *)current_proc->a11)++;

	return;
}

/******************************************************************************
 Function: void fadein_white_jsrp(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description: fadein a white palette
******************************************************************************/
void fadein_white_jsrp(void)
{
	fadewhite(sans_boonpal);
	DISPLAY_ON;
	fadein_white(all_palettes);

	CREATE(PID_FADE,white_2_blax);

	process_sleep(3);
	return;
}

void white_2_blax(void)
{
	short ta10;
	ta10=31;
	/* w2b3 */
	do
	{
		irqskye=ta10|(ta10<<5)|(ta10<<10);
		ta10-=2;
	}
	while(ta10>=0);

	irqskye=0;
	process_suicide();
}

void fadein_white(WORD **pa0)
{
	fader(pa0,fadein_white_tab);
}

void fadewhite(WORD **pa0)
{
	fader(pa0,fadewhite_tab);
}

void fadeout_mercy(WORD **pa0)
{
	fader(pa0,fadeout_mercy_tab);
}

/******************************************************************************
 Function: void fast_fadeout_jsrp(WORD pa10)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa10 - sleep time after procs are started

 Returns: None

 Description:
******************************************************************************/
void fast_fadeout_jsrp(WORD pa10)
{
	PROCESS *ptemp;

	ptemp=CREATE(PID_FADE,skydown);
	(LONG)ptemp->pa9=0x400;
	fade_all_fast();

	fade_sleep_a0(pa10);

	return;
}

/******************************************************************************
 Function: void fadeout_jsrp(WORD pa10)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa10 - sleep time after procs are started

 Returns: None

 Description:
******************************************************************************/
void fadeout_jsrp(WORD pa10)
{
	fade_all_sky();
	fade_sleep_a0(pa10);
	return;
}

/******************************************************************************
 Function: void fast_fadein_jsrp(WORD pa10)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa10 - sleep time after procs are started

 Returns: None

 Description:
******************************************************************************/
void fast_fadein_jsrp(WORD pa10)
{
	PROCESS *ptemp;

	fadeblak(all_palettes);
	ptemp=CREATE(PID_FADE,skyup);
	(LONG)ptemp->pa9=0x400;
	ptemp->a10=irqskye;
	process_sleep(5);					// allow time for process to die out

	fade_in_fast();
	DISPLAY_ON;

	fade_sleep_a0(pa10);

	return;
}

/******************************************************************************
 Function: void fadein_jsrp(WORD pa10)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa10 - sleep time after procs are started

 Returns: None

 Description:
******************************************************************************/
void fadein_jsrp(WORD pa10)
{
	PROCESS *ptemp;

	fadeblak(all_palettes);				// fade all pal black (instant)
	process_sleep(5);							// akllow time for process to die out
	DISPLAY_ON;

	fadein(all_palettes);				// fade in

	ptemp=CREATE(PID_FADE,skyup);
	(LONG)ptemp->pa9=0x800;
	ptemp->a10=irqskye;

	fade_sleep_a0(pa10);
	return;
}

/******************************************************************************
 Function: void fade_sleep_a0(WORD pa0)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa0 - sleep time

 Returns: None

 Description: 	sleep after a fade has started
******************************************************************************/
void fade_sleep_a0(WORD pa0)
{
	if (pa0!=0)
		process_sleep(pa0);

	return;
}

/******************************************************************************
 Function: void fade_all_sky(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description: 	fade out everything
******************************************************************************/
void fade_all_sky(void)
{
	PROCESS *ptemp;

	CREATE(PID_FADE,skydown);

	fadeout(all_palettes);

	return;
}

/******************************************************************************
 Function: void fade_all(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description: 	fade out everything
******************************************************************************/
void fade_all(void)
{
	fadeout(all_palettes);

	return;
}

/******************************************************************************
 Function: void raiden_dimmer(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description: 	dim everything ala raiden
******************************************************************************/
void raiden_dimmer(void)
{
	fader(death_fade_excludes,raiden_fade_tab);

	return;
}

/******************************************************************************
 Function: void raiden_undimmer(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description: 	undim everything ala raiden
******************************************************************************/
void raiden_undimmer(void)
{
	fader(death_fade_excludes,raiden_unfade_tab);

	return;
}

/******************************************************************************
 Function: void pause_dimmer(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description: 	dim everything ala pause
******************************************************************************/
void pause_dimmer(void)
{
	fader(NULL,raiden_fade_tab);

	return;
}

/******************************************************************************
 Function: void pause_undimmer(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description: 	undim everything ala pause
******************************************************************************/
void pause_undimmer(void)
{
	fader(NULL,raiden_unfade_tab);

	return;
}


/******************************************************************************
 Function: void fadeblak(WORD **pa0)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description: 	fade all down to black
******************************************************************************/
void fadeblak(WORD **pa0)
{
	fader(pa0,fadeblak_tab);

	return;
}

/******************************************************************************
 Function: void fadeins(WORD **pa0)

 By: David Schwartz

 Date: Mar 1995

 Parameters: ptr to table of palettes not to fade

 Returns: None

 Description: 	slow version of fadein
******************************************************************************/
void fadeins(WORD **pa0)
{
	fader(pa0,fadeins_tab);

	return;
}

/******************************************************************************
 Function: void fadein(WORD **pa0)

 By: David Schwartz

 Date: Mar 1995

 Parameters: ptr to table of palettes not to fade

 Returns: None

 Description: 	fadein
******************************************************************************/
void fadein(WORD **pa0)
{
	fader(pa0,fadein_tab);

	return;
}

/******************************************************************************
 Function: void join_in_fade1(WORD **pa0)

 By: David Schwartz

 Date: Mar 1995

 Parameters: ptr to table of palettes not to fade

 Returns: None

 Description: 	join in fade
******************************************************************************/
void join_in_fade1(WORD **pa0)
{
	fader(pa0,join_in_tab1);

	return;
}

/******************************************************************************
 Function: void join_in_fade2(WORD **pa0)

 By: David Schwartz

 Date: Mar 1995

 Parameters: ptr to table of palettes not to fade

 Returns: None

 Description: 	join in fade 2
******************************************************************************/
void join_in_fade2(WORD **pa0)
{
	fader(pa0,join_in_tab2);

	return;
}

/******************************************************************************
 Function: void fade_in_fast(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description: 	fast fadein of all pals
******************************************************************************/
void fade_in_fast(void)
{
	fader(all_palettes,fast_fadein_tab);

	return;
}

/******************************************************************************
 Function: void fade_all_fast(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description: 	fast fade of all pals
******************************************************************************/
void fade_all_fast(void)
{
	fader(all_palettes,fast_fade_tab);

	return;
}

/******************************************************************************
 Function: void fadein_all_fast(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description: 	fast in fade of all pals
******************************************************************************/
void fadein_all_fast(void)
{
	fader(all_palettes,fast_fadein_tab);

	return;
}

/******************************************************************************
 Function: void fadeout(WORD **pa0)

 By: David Schwartz

 Date: Mar 1995

 Parameters: ptr to table of palettes not to fade

 Returns: None

 Description: 	fadeout
******************************************************************************/
void fadeout(WORD **pa0)
{
	fader(pa0,fadeout_tab);

	return;
}

/******************************************************************************
 Function: void fader(WORD **pa0,WORD *pa11)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa0 - ptr to table of palettes not to fade
			 pa11 - ptr to a table of mult values

 Returns: None

 Description: 	the fader routine
******************************************************************************/
void fader(WORD **pa0,WORD *pa11)
{
	short i;
	WORD **ta7;
	PROCESS *ptemp;

	/* pallp, walk through all palettes and see */
	for (i=0;i<NUM_PALS;i++)
	{
		if (palram[i].palcnt!=0) 				// if pal allocated
		{
			if (pa0!=NULL)
			{
				/* check to see if pal is on exlcusion list */
				ta7=pa0;
				while (*ta7!=NULL)
				{
					if (*ta7==palram[i].palid)
						goto SKIPFADE;

					ta7++;
				}
			}

			/* skfpalx */
			ptemp=CREATE(PID_FADE,fadeproc);
			(WORD *)(ptemp->pa8)=palram[i].palid;
			(LONG)ptemp->pa9=i;
			(WORD *)(ptemp->a11)=pa11;

SKIPFADE:
		}
	}

	/* pallpx */
	return;
}

/******************************************************************************
 Function: void fadeproc(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: current_proc->pa8 - ptr to palette to be faded
			 current_proc->pa9 - palette slot of palette
			 current_proc->a11 - fade multiplier table

 Returns: None

 Description: 	process to actually fade a palette
******************************************************************************/
void fadeproc(void)
{
	WORD ta2;
	WORD l;

	l=(LONG)current_proc->pa9;

	/* fadeplp */
	while ((ta2=GET_WORD(current_proc->a11)++)!=0xffff)
	{
		/* use bottom of stack for temp storage area for palettes */
		fadepal((WORD *)(&(current_proc->p_context.c_stack)),(WORD *)current_proc->pa8,ta2);
		palset_xfer(FORE_CLUT_START_X+((l%CLUT_FACTOR)<<CLUT_SHIFT_SIZE),FORE_CLUT_START_Y+(l/CLUT_FACTOR),(WORD *)(&(current_proc->p_context.c_stack)));
		process_sleep(1);
	}

	/* fadeprcx */
	process_sleep(1);			// give last xfer a chance to go
	process_suicide();
}

/******************************************************************************
 Function: void fadepal(WORD *pa0,WORD *pa1,WORD pa2)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa0 - dest ram for palette
			 pa1 - source for palette
			 pa2 - color mulitplier

 Returns: None

 Description: 	each color in palette will be mult by a2 then div by 128
				apply the color mult and do one iteration on the pal
******************************************************************************/
void fadepal(WORD *pa0,WORD *pa1,WORD pa2)
{
	LONG ta14;
	LONG ta3,ta5,ta7;
	WORD high=0x000;

	ta14=GET_LONG(pa1)++;					// # of entries
	*((LONG *)pa0)=ta14;					// write count
	pa0+=2;									// next dest

	do
	{
		ta3=GET_WORD(pa1)++;

		ta7=(ta3 & PRE_A8)*pa2;
		ta5=(ta3 & PRE_A6)*pa2;
		ta3=(ta3 & PRE_A4)*pa2;

		if (ta3>POST_A9)
			ta3=POST_A9;
		else ta3&=POST_A9;

		if (ta5>POST_A10)
			ta5=POST_A10;
		else ta5&=POST_A10;

		if (ta7>POST_A11)
			ta7=POST_A11;
		else ta7&=POST_A11;

		ta3=(ta3|ta5|ta7)>>7;

		ta3|=high;					// 1st entry will be zero all others will have bit sets
		high=0x8000;

		*pa0=(WORD)ta3;
		pa0++;
	}
	while(--ta14>0);

	return;
}

/******************************************************************************
 Function: void skydown(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: current_proc->pa9 - fader speed

 Returns: None

 Description: 	fade down the sky color and die
******************************************************************************/
void skydown(void)
{
	long ta8;
	WORD ta10=irqskye;

	ta8=0x10000;

	/* skydown1 */
	do
	{
		irqskye=xcolor(ta10,(LONG)ta8);
		process_sleep(1);
		ta8-=(long)current_proc->pa9;
	}
	while(ta8>0);

	skydie(0);
}

/******************************************************************************
 Function: void skyup(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: current_proc->pa9 - fader speed
			 current_proc->a10 - final color we wanna end up with

 Returns: None

 Description: 	fade up the sky color and die
******************************************************************************/
void skyup(void)
{
	long ta8=0;

	/* skup1 */
	do
	{
		irqskye=xcolor((WORD)current_proc->a10,(LONG)ta8);
		process_sleep(1);
		ta8+=(long)current_proc->pa9;
	}
	while(ta8<0x10000);

	skydie(0x10000);
}

/******************************************************************************
 Function: WORD xcolor(WORD pa0,LONG pa1)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa0 - color value
			 pa1 - x factor (16.16)

 Returns: x color value

 Description: 	fade up the sky color and die
******************************************************************************/
WORD xcolor(WORD pa0,LONG pa1)
{
	LONG ta3;
	WORD ta4=0;

	ta4=(((pa0 & 0x1f) * pa1) & 0x1f0000);		// red

	ta3=(((pa0 & 0x3e0) * pa1) & 0x1f0000)>>11;		// green
	ta4|=(WORD)ta3;

	ta3=(((pa0 & 0x7c00) * pa1) & 0x1f0000)>>6;		// blue
	ta4|=(WORD)ta3;

	return(ta4);
}

