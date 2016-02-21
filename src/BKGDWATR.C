/******************************************************************************
 File: bkgdesrt.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Desert Routines
******************************************************************************/

/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkobj.h"
#include "mkos.h"
#include "mkgame.h"
#include "mkutil.h"
#include "mkfx.h"
#include "mkani.h"
#include "mkinit.h"
#include "mkpal.h"
#include "mksound.h"
#include "valloc.h"

/* Prototypes */
void make_water_proc(void);
void water_proc(void);
void water_ani(OIMGTBL *frame,OBJECT *obj);

/************************************************************
 * 			WATER S T U F F 							*
 ************************************************************/

/********************
 Externals
 ********************/
extern void *water_anims[];
extern void *a_water1[];
extern void *WATERA01[];
extern void *WATERC01[];
extern void *WATERD01[];
extern void *WATERB01[];
extern void *WATERA02[];
extern void *WATERC02[];
extern void *WATERD02[];
extern void *WATERB02[];
extern void *WATERA03[];
extern void *WATERC03[];
extern void *WATERD03[];
extern void *WATERB03[];
extern void *WATERA04[];
extern void *WATERC04[];
extern void *WATERD04[];
extern void *WATERB04[];
extern void *WATERA05[];
extern void *WATERC05[];
extern void *WATERD05[];
extern void *WATERB05[];
extern void *WATERA06[];
extern void *WATERC06[];
extern void *WATERD06[];
extern void *WATERB06[];
extern void *WATERA07[];
extern void *WATERC07[];
extern void *WATERD07[];
extern void *WATERB07[];
extern void *WATERA08[];
extern void *WATERC08[];
extern void *WATERD08[];
extern void *WATERB08[];

/********************
 ********************/
ADDRESS *water_perm_list[]=
{
	(ADDRESS *)WATERA01,
	(ADDRESS *)WATERC01,
	(ADDRESS *)WATERD01,
	(ADDRESS *)WATERB01,
	(ADDRESS *)WATERA02,
	(ADDRESS *)WATERC02,
	(ADDRESS *)WATERD02,
	(ADDRESS *)WATERB02,
	(ADDRESS *)WATERA03,
	(ADDRESS *)WATERC03,
	(ADDRESS *)WATERD03,
	(ADDRESS *)WATERB03,
	(ADDRESS *)WATERA04,
	(ADDRESS *)WATERC04,
	(ADDRESS *)WATERD04,
	(ADDRESS *)WATERB04,
	(ADDRESS *)WATERA05,
	(ADDRESS *)WATERC05,
	(ADDRESS *)WATERD05,
	(ADDRESS *)WATERB05,
	(ADDRESS *)WATERA06,
	(ADDRESS *)WATERC06,
	(ADDRESS *)WATERD06,
	(ADDRESS *)WATERB06,
	(ADDRESS *)WATERA07,
	(ADDRESS *)WATERC07,
	(ADDRESS *)WATERD07,
	(ADDRESS *)WATERB07,
	(ADDRESS *)WATERA08,
	(ADDRESS *)WATERC08,
	(ADDRESS *)WATERD08,
	(ADDRESS *)WATERB08,
	NULL
};

/******************************************************************************
 Function: void make_water_proc(void)

 By: David Schwartz

 Date: Nov 1995

 Parameters: None

 Returns: None

 Description:	setup the water background
******************************************************************************/
void make_water_proc(void)
{
	alloc_vram_perm_bg_list((ADDRESS *)water_perm_list,&water_anims);
	CREATE(PID_BANI,water_proc);
}

void water_proc(void)
{
	OBJECT *ta8;
	OBJECT *ta9;
	ADDRESS *ta11;

	do
	{
		ta11=(ADDRESS *)a_water1;

		/* water2 */
		do
		{
			ta8=(OBJECT *)&baklst3;
			ta9=(OBJECT *)&baklst4;

			/* water3 */
			do
			{
				ta8=ta8->olink;
				if (ta8==NULL)
					break;

				(ADDRESS)ta8->oheap=(ADDRESS)water_anims;

				if (ta8->ozval==2)
					water_ani((OIMGTBL *)*(ta11+1),ta8);
				else
				{
					if (ta8->ozval==1)
						water_ani((OIMGTBL *)*(ta11+0),ta8);
				}
			}
			while(1);

			/* water6 */
			do
			{
				ta9=ta9->olink;
				if (ta9==NULL)
					break;

				(ADDRESS)ta9->oheap=(ADDRESS)water_anims;

				if (ta9->ozval==0)
					water_ani((OIMGTBL *)*(ta11+3),ta9);
				else
				{
					if (ta9->ozval==1)
						water_ani((OIMGTBL *)*(ta11+2),ta9);
				}
			}
			while(1);

			/* water8 */
			process_sleep(8);
			ta11+=4;
			if (*ta11==NULL)
				break;
		}
		while(1);
	}
	while(1);
}

void water_ani(OIMGTBL *frame,OBJECT *obj) 
{
	short x,y;

	x=obj->oxpos.u.intpos;
	y=obj->oypos.u.intpos;
	current_proc->pa8=obj;
	animate(SINGLE_ANI,frame,obj->oflags);
	obj->oxpos.u.intpos=x;
	obj->oypos.u.intpos=y;
	return;
}
