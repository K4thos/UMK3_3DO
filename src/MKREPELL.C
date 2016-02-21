/******************************************************************************
 File: mkrepell.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Repell Routines
******************************************************************************/

/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkos.h"
#include "mkgame.h"
#include "mkrepell.h"
#include "mkobj.h"

WORD b4;						// reg b4

/******************************************************************************
 Function: void stuff_a6a7(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	evaluate player positions and velocities and set velocity
******************************************************************************/
void stuff_a6a7(void)
{

	if (((short)current_proc->a5 > SCREEN_DISTANCE) && ((p1_obj->oflags2|p2_obj->oflags2) & M_NOEDGE)==0)
	{
		(short)current_proc->a5 -=SCREEN_DISTANCE;			// amount we need to adjust

		if (p2_obj->oxpos.pos >= p1_obj->oxpos.pos)
		{
			/* stuff5 */
			/* layouts is: p1 - p2 */
			if ((long)current_proc->a7>0)
				(long)current_proc->a7=0;		// p2 on right --> can't move right

			if ((long)current_proc->a6<0)
				(long)current_proc->a6=0;		// p1 on left --> can't move left

			/* stuff7, if too far apart --> adjust */
			if ((short)current_proc->a5>3)
			{
				(short)current_proc->a5=((short)(current_proc->a5)>>1);		// take 1/2 error

				/* stuf9 */

				/* adjust p2 pos */
				p2_obj->oxpos.u.intpos +=(-((short)current_proc->a5));

				/* adjust p1 pos */
				p1_obj->oxpos.u.intpos +=((short)current_proc->a5);
			}
		}
		else
		{
			/* layouts is: p2 - p1 */
			if ((long)current_proc->a7<0)
				(long)current_proc->a7=0;		// p2 on left --> can't move left

			if ((long)current_proc->a6>0)
				(long)current_proc->a6=0;		// p1 on right --> can't move right

			/* stuff8 */
			if ((short)current_proc->a5>3)
			{
				(short)current_proc->a5=-((short)(current_proc->a5)>>1);		// take 1/2 error

				/* adjust p2 pos */
				p2_obj->oxpos.u.intpos +=(-((short)current_proc->a5));

				/* adjust p1 pos */
				p1_obj->oxpos.u.intpos +=((short)current_proc->a5);

			}
		}
	}

	/* stuff_velocities */
	p1_obj->oxvel.pos=(long)current_proc->a6;
	p2_obj->oxvel.pos=(long)current_proc->a7;

	return;
}

/******************************************************************************
 Function: void repell(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	process to keep players from overlapping each other
******************************************************************************/
void repell(void)
{
	while (TRUE)
	{
REPELL:
		process_sleep(1);

		if (f_norepell != 0)
		{
			f_norepell--;
		}

		b4=f_norepell;

		/* rep1 */
		get_player_hilo(p1_obj);
		(short)current_proc->a4=(short)current_proc->a6;
		(short)current_proc->a5=(short)current_proc->a7-GRACE_PIXELS;			// a4 = high pt : a5 = low point

		get_player_hilo(p2_obj);
		(short)current_proc->a7=(short)current_proc->a7-GRACE_PIXELS;			// a6 = high pt : a7 = low point

		if ((short)current_proc->a5 < (short)current_proc->a6 || (short)current_proc->a7 < (short)current_proc->a4)
		{
			 /* player_above_player */
			 get_player_anis();
			 (long)current_proc->a6=p1_xvel.pos;
			 (long)current_proc->a7=p2_xvel.pos;
			 stuff_a6a7();
		}
		else
		{
			get_player_anis();

			if (b4==0)
			{
				/* super close check --> repell now if TRUE and repell is not disabled */
				if ((short)current_proc->a5 < (REPELL_DISTANCE-3))
				{
					/* repell_now */
					if (b4!=0)					// redundant, but
						goto REPELL;

					if (p2_obj->oxpos.u.intpos >= p1_obj->oxpos.u.intpos)
					{
						(long)current_proc->a7=0x30000;
						(long)current_proc->a6=-0x30000;		// p1 on left
						stuff_a6a7();
					}
					else
					{
						(long)current_proc->a7=-0x30000;
						(long)current_proc->a6=0x30000;			// p2 on left
						stuff_a6a7();
					}

					goto REPELL;
				}
			}

			/* rep2, process player 1 velocity, if far apart or repell disabled or no movement */
			if ((((long)current_proc->a6=p1_xvel.pos) != 0) && ((short)current_proc->a5 < REPELL_DISTANCE) && (b4 == 0))
			{
				if ((long)current_proc->a6 >=0)
				{
					/* check moving right/he's to the left */
					if (p2_obj->oxpos.u.intpos > p1_obj->oxpos.u.intpos)
					{
						if (p2_xvel.pos>=0)
						{
							/* REP5 insert */
							(long)current_proc->a7=(long)current_proc->a6=(long)(current_proc->a6)>>1;	//	push at half speed)
							stuff_a6a7();
							goto REPELL;
						}
						else
						{
							/* stuff_zeroes, fighting each other --> both zero */
							p1_obj->oxvel.pos=p2_obj->oxvel.pos=0;
							goto REPELL;
						}
					}
				}
				else
				{
					/* REP3 */
					if (p2_obj->oxpos.pos < p1_obj->oxpos.pos)
					{
						if (p2_xvel.pos>0)
						{
							/* stuff_zereos, fighting each other --> both zero */
							p1_obj->oxvel.pos=p2_obj->oxvel.pos=0;
							goto REPELL;
						}
						else
						{
							/* REP5 insert */
							(long)current_proc->a7=(long)current_proc->a6=(long)(current_proc->a6)>>1;	//	push at half speed)
							stuff_a6a7();
							goto REPELL;
						}
					}
				}
			}


			/* rep6, process player 2 velocity, if far apart or repell disabled or no movement */
			if ((((long)current_proc->a7=p2_xvel.pos)!=0) && ((short)current_proc->a5<REPELL_DISTANCE) && (b4==0))
			{
				if ((long)current_proc->a7 >= 0)
				{
					/* i'm moveing toward from him --> not ok */
					if (p1_obj->oxpos.u.intpos > p2_obj->oxpos.u.intpos)
					{
						if (p1_xvel.pos < 0)
						{
							/* stuff_zeros, ---> <--- pressing, zero both vels */
							p1_obj->oxvel.pos=p2_obj->oxvel.pos=0;
							goto REPELL;
						}
						else
						{
							/* REP8 insert */
							(long)current_proc->a7=(long)current_proc->a6=(long)(current_proc->a7)>>1;	//	push at half speed)
							stuff_a6a7();
							goto REPELL;
						}
					}
				}
				else
				{
					/* REP7 */
					if (p1_obj->oxpos.u.intpos < p2_obj->oxpos.u.intpos)
					{
						if (p1_xvel.pos > 0)
						{
							/* stuff_zeros, --> <-- pressing, zero both vels */
							p1_obj->oxvel.pos=p2_obj->oxvel.pos=0;
							goto REPELL;
						}
						else
						{
							/* REP8 insert */
							(long)current_proc->a7=(long)current_proc->a6=(long)(current_proc->a7)>>1;	//	push at half speed)
							stuff_a6a7();
							goto REPELL;
						}
					}
				}
			}

			/* default case for p1 xvel & p2 xvel */
			stuff_a6a7();

		} /* main else clause */
	} /* while */
}

/******************************************************************************
 Function: void get_player_hilo(OBJECT *obj)

 By: David Schwartz

 Date: Sept 1994

 Parameters: obj - object player main object (1st on list)

 Returns: current_proc->a6 =  highest y point (lowest coord value)
		  current_proc->a7 =  lowest y point (highest coord value)

 Description:	get player hi and low coordinates
******************************************************************************/
void get_player_hilo(OBJECT *obj)
{
	int lowest=-0xfffff;
	int highest=0x7fffffff;
	P_RAM *pramptr;
	short ypos;


	pramptr=(P_RAM *)obj->oimg;
	ypos=obj->oypos.u.intpos;

#if !MULTI_OFF
	while (pramptr->prsagptr != 0)
#endif
	{
		if ((ypos-pramptr->prpos.u.ypos)<highest)
			highest=ypos-pramptr->prpos.u.ypos;

		if ((ypos-pramptr->prpos.u.ypos+pramptr->prsize.u.ypos)>lowest)
			lowest=ypos-pramptr->prpos.u.ypos+pramptr->prsize.u.ypos;

#if !MULTI_OFF
		pramptr++;
#endif
	}

	(int)current_proc->a6=highest;
	(int)current_proc->a7=lowest;

	return;
}

/******************************************************************************
 Function: void get_player_anis(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: current_proc->a4 =  y distance
		  current_proc->a5 =  x distance

 NOTE: THIS ROUTINE DIFFERS FROM ORIGINAL IN THAT IT DOES NOT STORE A0-A3, SINCE
	   THE LOOKUP TIME WILL BE THE SAME IF THE RESULTS ARE STORED IN A0-A3 OR LEFT ALONE

 Description:	get player animations distances
******************************************************************************/
inline void get_player_anis(void)
{
	(short)current_proc->a4=abs(p1_obj->oypos.u.intpos-p2_obj->oypos.u.intpos);		// y distance

	(short)current_proc->a5=abs(p1_obj->oxpos.u.intpos-p2_obj->oxpos.u.intpos);		// x distance

}
