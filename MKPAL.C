/******************************************************************************
 File: mkpal.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Palette Routines
******************************************************************************/

/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkos.h"
#include "mkpal.h"

/******************************************************************************
 Function: void init_paltrans_queue(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	initialize the queue for palette transfers
******************************************************************************/
inline void init_paltrans_queue(void)
{
	paltrans_start=paltrans_end=paltrans_queue;				/* set empty queue (start=end=qstart) */

	return;
}

/******************************************************************************
 Function: WORD findpal(WORD *pal_id)

 By: David Schwartz

 Date: Sept 1994

 Parameters: pal_id - pal id to find

 Returns: color pal # the pal_id is using
		  0xffff - pal not found

 Description:	find the color map # to which the given palette is assigned
******************************************************************************/
WORD findpal(WORD *pal_id)
{
	WORD l;

	for (l=0;l<NUM_PALS;l++)
		if (palram[l].palid==pal_id)
			return(l);

	return(UNKNOWN_PAL);
}

/******************************************************************************
 Function: void clear_pal(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	clear out palette allocation ram
******************************************************************************/
void clear_pal(void)
{
	init_paltrans_queue();

	clear_fore_pal();

	clear_back_pal();


#if DEBUG
	usefpcnt=usebpcnt=0;
#endif

	return;
}

/******************************************************************************
 Function: void clear_fore_pal(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	clear out foreground palette map ram
******************************************************************************/
void clear_fore_pal(void)
{
	int l;

	for (l=0;l<NUM_F_PALS;l++)
		memset(fpalram+l,0,sizeof(PALINFO));

	return;
}

/******************************************************************************
 Function: void clear_back_pal(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	clear out background palette map ram
******************************************************************************/
void clear_back_pal(void)
{
	int l;

	for (l=0;l<NUM_B_PALS;l++)
		memset(bpalram+l,0,sizeof(PALINFO));

	return;
}

/******************************************************************************
 Function: void palset_xfer(WORD,WORD,WORD *)

 By: David Schwartz

 Date: Sept 1994

 Parameters: cx - vram x addr
 			 cy - vram y addr
 			 clut - ptr to CLUT data

 Returns: the clut # that this entry uses

 Description:	add a palette transfer to queue
******************************************************************************/
void palset_xfer(WORD cx,WORD cy,WORD *clut)
{
	paltrans_end->csx=cx;
	paltrans_end->csy=cy;
	paltrans_end->count=GET_LONG(clut)++;
	paltrans_end->csrc=(ADDRESS *)clut;

	paltrans_end=((paltrans_end+1)==paltrans_queue+PALQ_MAX_ENTRY) ? paltrans_queue:paltrans_end+1;

	return;
}

/******************************************************************************
 Function: WORD get_fore_pal(WORD *pal_id)

 By: David Schwartz

 Date: Sept 1994

 Parameters: pal_id - ptr to CLUT data

 Returns: the clut # that this entry uses

 Description:	allocate a palette for the incoming CLUT (FOREGROUNDS)
******************************************************************************/
WORD get_fore_pal(WORD *pal_id)
{
	PALINFO *palptr;
	short l=0;
	WORD cx,cy;

	palptr=palram;

	/* search, if id already exists in palette maps */
	while (l<NUM_PALS)
	{
		if (palptr->palid==pal_id)
		{
			palptr->palcnt++;				/* inc # of objects using this palette */
			return(getClut((FORE_CLUT_START_X+((l%CLUT_FACTOR)<<CLUT_SHIFT_SIZE)),(FORE_CLUT_START_Y+(l/CLUT_FACTOR))));
		}
		palptr++;
		l++;
	}

	/* find a spare palette */
	l=0;
	palptr=fpalram;

	while (l<NUM_F_PALS)
	{
		if (palptr->palcnt==0 && (palptr->paltime==0 || palptr->paltime!=tick))
		{
			//setup palette transfer
			palset_xfer((cx=FORE_CLUT_START_X+((l%CLUT_FACTOR)<<CLUT_SHIFT_SIZE)),(cy=FORE_CLUT_START_Y+(l/CLUT_FACTOR)),pal_id);
			palptr->palcnt++;				/* inc # of objects using this palette */
			palptr->palid=pal_id;		/* set id */

#if DEBUG
			usefpcnt++;
#endif

			return(getClut(cx,cy));
		}

		l++;
		palptr++;

	}

#if DEBUG
	while (1)
	{
		POLLING;
	}
#endif

	return(0);									/* no available palettes */
}

/******************************************************************************
 Function: WORD get_back_pal(WORD *pal_id)

 By: David Schwartz

 Date: Sept 1994

 Parameters: clut - ptr to CLUT data

 Returns: the clut # that this entry uses

 Description:	allocate a palette for the incoming CLUT (BACKGROUNDS)
******************************************************************************/
WORD get_back_pal(WORD *pal_id)
{
	PALINFO *palptr;
	WORD l=0;
	WORD cx,cy;

	palptr=palram;

	/* search, if id already exists in palette maps */
	while (l<NUM_PALS)
	{
		if (palptr->palid==pal_id)
		{
			palptr->palcnt++;				/* inc # of objects using this palette */
			return(getClut((FORE_CLUT_START_X+((l%CLUT_FACTOR)<<CLUT_SHIFT_SIZE)),(FORE_CLUT_START_Y+(l/CLUT_FACTOR))));
		}
		palptr++;
		l++;
	}

	/* find a spare palette */
	l=0;
	palptr=bpalram;

	while (l<NUM_B_PALS)
	{
		if (palptr->palcnt==0 && (palptr->paltime==0 ||palptr->paltime!=tick))
		{
			//setup palette transfer
			palset_xfer((cx=FORE_CLUT_START_X+(((NUM_F_PALS+l)%CLUT_FACTOR)<<CLUT_SHIFT_SIZE)),(cy=FORE_CLUT_START_Y+((NUM_F_PALS+l)/CLUT_FACTOR)),pal_id);
			palptr->palcnt++;				/* inc # of objects using this palette */
			palptr->palid=pal_id;		/* set id */
#if DEBUG
			usebpcnt++;
#endif
			return(getClut(cx,cy));
		}
		l++;
		palptr++;

	}

		return(get_fore_pal(pal_id));
#if 0
	while (1)
	{
		POLLING;
	}
#endif

	return(0);									/* no available palettes */
}

/******************************************************************************
 Function: void free_pal(WORD clut_id)

 By: David Schwartz

 Date: Sept 1994

 Parameters: clut_id - clut id to free

 Returns: None

 Description:	free a palette
******************************************************************************/
void free_pal(WORD clut_id)
{
	short l;
	PALINFO *palptr;

	/* convert clut id into palram index */
	l=(clut_id-getClut(FORE_CLUT_START_X,FORE_CLUT_START_Y))>>CLUT_ID_SHIFT;

	if (l>=NUM_PALS) LOCKUP;

	palptr=palram+l;

	switch (palptr->palcnt)
	{
		case 0:
			LOCKUP;
			break;
		case 1:
			palptr->paltime=tick;			/* remember ticked freed, since can't allocate same tick */
#if DEBUG
		if (l<NUM_F_PALS)
			usefpcnt--;
		else usebpcnt--;
#endif
		default:
			palptr->palcnt--;
	}

	return;
}

/******************************************************************************
 Function: void unget_pal(WORD clut_id)

 By: David Schwartz

 Date: Sept 1994

 Parameters: clut_id - clut id to free

 Returns: None

 Description:	clears out a palettes allocation count
******************************************************************************/
void unget_pal(WORD clut_id)
{
	short l;
	PALINFO *palptr;

	/* convert clut id into palram index */
	l=(clut_id-getClut(FORE_CLUT_START_X,FORE_CLUT_START_Y))>>CLUT_ID_SHIFT;

	if (l>=NUM_PALS) LOCKUP;

	palptr=palram+l;

	if (palptr->palcnt>0)
		palptr->paltime=tick;			/* remember ticked freed, since can't allocate same tick */

	palptr->palcnt=0;

#if DEBUG
	if (l<NUM_F_PALS)
		usefpcnt--;
	else usebpcnt--;
#endif

	return;
}

/******************************************************************************
 Function: void blow_pal(WORD *pal_id)

 By: David Schwartz

 Date: Sept 1994

 Parameters: clut_id - clut id to free

 Returns: None

 Description:	zero out the address and color count of a palette map entry
******************************************************************************/
void blow_pal(WORD *pal_id)
{
	short l=0;
	PALINFO *palptr=palram;

	/* find entry to blow up */
	while (l<NUM_PALS)
	{
		if (palptr->palid==pal_id)
		{
			palptr->palcnt=0;				/* kill count */
			palptr->palid=0;				/* kill id */
#if DEBUG
	if (l<NUM_F_PALS)
		usefpcnt--;
	else usebpcnt--;
#endif
			return;
		}
		palptr++;
		l++;
	}
	return;
}


/******************************************************************************
 Function: void transfer_palettes(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	transfers all palettes in queue
******************************************************************************/
void transfer_palettes(void)
{
	RECT frame;

	frame.h=1;
	while (paltrans_start != paltrans_end)
	{
		frame.x=paltrans_start->csx;
		frame.y=paltrans_start->csy;
		frame.w=paltrans_start->count;
		LOADIMAGE(&frame,paltrans_start->csrc);

		if ((++paltrans_start)==(paltrans_queue+PALQ_MAX_ENTRY))		/* wrap ptr if needed */
			paltrans_start=paltrans_queue;
	}

	return;
}
