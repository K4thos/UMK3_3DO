/******************************************************************************
 File: bkgdpit.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Pit Routines
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
#include "mkreact.h"
#include "moves.h"
#include "mkguys.h"

/* Prototypes */

/************************************************************
 * 			P I T / T H R O N E  S T U F F 					*
 ************************************************************/

/********************
 Externals
 ********************/
extern void *pit_anims[];
extern void *a_saw[];
extern void *SAW2[];
extern void *SAW4[];
extern void *SAW6[];
extern void *THRONEWALL[];

ADDRESS *pit_perm_list[]=
{
	(ADDRESS *)SAW2,
	(ADDRESS *)SAW4,
	(ADDRESS *)SAW6,
	(ADDRESS *)THRONEWALL,
	NULL
};

void calla_pit(void)
{
	alloc_vram_perm_bg_list((ADDRESS *)pit_perm_list,&pit_anims);
	return;
}


/******************************************************************************
 Function: void build_pit_saw(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	build the grinders for the pit fatality
******************************************************************************/
void build_pit_saw(void)
{
	OBJECT *ta8=current_proc->pa8;
	OBJECT *obj;
	PROCESS *pa0;
	short ta7;
	short ta9;

	current_proc->a10=(ADDRESS)current_proc->pa8;
	ta9=ta8->oxpos.u.intpos;

	get_my_dfe();

	ta7=-SCX(0xa0);
	if ( (long)current_proc->a5>=SCX(0x70) )
	{
		ta7=-SCX(0x100);
		if ( (long)current_proc->a6<=SCX(0x70) )
		{
			ta7=-SCX(0x180);
		}
	}

	/* build2 */
	ta9+=ta7;
	current_proc->a11=3;
	/* build3 */
	do
	{
		gso_dmawnz(obj,(ADDRESS)SAW2,pit_anims,0);
		alloc_cache((OIMGTBL *)SAW2,&pit_anims,obj);
		obj->oypos.u.intpos=current_proc->pdata.p_ganiy+6;
		obj->oxpos.u.intpos=ta9;
		ta9+=SCX(0x80)+16;
		insert_object(obj,&objlst);
		CREATE(PID_BANI,saw_spin_proc);

	} while ( --current_proc->a11>0 );

	pa0=process_exist(PID_SCROLL,0xffff);
	pa0->pdata.p_store1=(ADDRESS)current_proc->pa8;

	current_proc->pa8=ta8;
	return;
}

void saw_spin_proc(void)
{
	while ( 1 )
	{
		current_proc->pa9=a_saw;
		framew(3);
	}
}

/******************************************************************************
 Function: void pit_scroll_proc(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:  falling down the pit scroll proc
******************************************************************************/
void pit_scroll_proc(void)
{
	PROCESS *pa0;
	OBJECT *oa11=(OBJECT*)current_proc->a11;
	OBJECT *obj,*oa9;
	short ta0,ta1;

	stop_scrolling();
	scrolly.pos=0;

	white_flash();

	/* NOW... we put up the grad wall */
	//gso_dmawnz_ns(obj,(ADDRESS)THRONEWALL,pit_anims,0);			// left side slab
	//alloc_cache(THRONEWALL,&pit_anims,obj);
	gso_dmawnz_ns(obj,(ADDRESS)&THRONEWALL,pit_anims,0);			// left side slab
	alloc_cache((OIMGTBL *)&THRONEWALL,&pit_anims,obj);
	set_xy_coordinates(obj,0,SCY(0xfe)+21);
	oa9=obj;
	insert_object(obj,&baklst4);

	//gso_dmawnz_ns(obj,(ADDRESS)THRONEWALL,pit_anims,0);			// right side slab
	//alloc_cache(THRONEWALL,&pit_anims,obj);
	gso_dmawnz_ns(obj,(ADDRESS)&THRONEWALL,pit_anims,0);			// right side slab
	alloc_cache((OIMGTBL *)&THRONEWALL,&pit_anims,obj);
	flip_single(obj);
	obj->oypos.u.intpos=oa9->oypos.u.intpos;
	obj->oxpos.u.intpos=oa9->oxpos.u.intpos+oa9->osize.u.xpos;
	insert_object(obj,&baklst4);

	bridge_list_fixed(&baklst3,worldtlx3.u.intpos);
	bridge_list_fixed(&baklst4,worldtlx4.u.intpos);

	(ADDRESS)bak3mods=(ADDRESS)bak4mods=SKIP_BAKMOD;		// dont update these lists

	pa0=process_exist(PID_BACKG,0xffff);
	pa0->a10=1;							// background updateer = every tick
	current_proc->pdata.p_store1=0;			// init: clear this long

	current_proc->a10=0x40;				// time before switching camera view

	/* pits1, follow him down the pit */
	do
	{
		do
		{
			process_sleep(1);

			ta0=oa11->oxpos.u.intpos;
			ta1=worldtlx.u.intpos+SCRRGT/2;
			if (abs(ta0-ta1)<6)
			{
				/* pits3 (stop_scrolling) */
				stop_scrolling();						// just about centered --> stop scroll
			}
			else
			{
				if (ta0>ta1)
				{
					/* pit_scroll_right */
					if (worldtlx.u.intpos>=right_edge)
						stop_scrolling();
					else ss_20000(0);
				}
				else
				{
					/* pit_scroll_left */
					if (worldtlx.u.intpos<=left_edge)
						stop_scrolling();
					else ss_20000(1);
				}
			}

			/* pits3 */
			if ( current_proc->a10==0 )
				goto PITS5;
		}
		while(--current_proc->a10>0);

		current_proc->a10=0;
		bridge_list_scroll(&baklst3,-SCY(0x10000));
		bridge_list_scroll(&baklst4,-SCY(0x10000));

PITS5:
	/* pits5 */
	scrolly.pos=oa11->oyvel.pos+((oa11->oyvel.pos)>>3)+((oa11->oyvel.pos)>>4);		// scroll faster then dude falling

	oa9=(OBJECT *)current_proc->pdata.p_store1;
	if ( oa9!=NULL )
	{
 		(short)current_proc->a0=oa9->oypos.u.intpos;
 		((short)current_proc->a0)-=(short)(SCRBOT-SCY(0x60));
	}

	} while (current_proc->pdata.p_store1==0 || (((short)current_proc->a0)>worldtly.u.intpos));			// add 2nd requirement

	scrolly.pos=0;
	worldtly.u.intpos=(short)current_proc->a0;
	stop_scrolling();

	bridge_list_scroll(&baklst3,0);					// stop the slower scroll !!!!
	bridge_list_scroll(&baklst4,0);

	/* pits6 */
	do
	{
		current_proc->p_comp_flag=PC_PIT_STOP;
		process_sleep(10);
	} while ( 1);

}

/* pa0 - list, a1 - scroll value to stuff */
void bridge_list_scroll(OBJECT **pa0,long pa1)
{
	OBJECT *obj=(OBJECT *)pa0;

	while (obj->olink != NULL)
	{
		obj=obj->olink;
		obj->oyvel.pos=pa1;
	}

	return;
}

void bridge_list_fixed(OBJECT **pa0,short pa5)
{
	OBJECT *ta8;
	ta8=(OBJECT *)pa0;

	/* blf5 */
	while ((ta8=ta8->olink)!=NULL)
	{
		ta8->oxpos.u.intpos-=pa5;
		ta8->oypos.u.intpos-=worldtly.u.intpos;
		set_noscroll(ta8);					// sky dont scroll
	}

	return;
}

/******************************************************************************
 Function: void fall_down_pit(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	die by pit
******************************************************************************/
void fall_down_pit(void)
{
	PROCESS *ptemp;
	long ta0;

	group_sound(9);			// death scream

	ptemp=process_exist(PID_SCROLL,0xffff);
	fastxfer(ptemp,pit_scroll_proc);
	ptemp->a11=(ADDRESS)current_proc->pa8;

	current_proc->pdata.p_ganiy+=SCY(0x500);			// floor way down there

	if ( (current_proc->pa8)->oxpos.u.intpos<SCX(0x1c3) || (current_proc->pa8)->oxpos.u.intpos>SCX(0x2f0)  )
		ta0=SCX(0x8000);
	else ta0=SCX(0x20000);

	/* normfall */
	(FUNC)current_proc->a6=pit_fall_scan;
	flight_call(current_proc->pa8,ta0,-SCY(0xc0000),SCY(0x6000),5,ANIM_KDOWN);

	set_inviso(current_proc->pdata.p_otherguy);				// make sure we dont see winner

	/* build the machinery down yonder !! */
	current_proc->pdata.p_ganiy+=SCY(0x460);
	build_pit_saw();

	/* continue falling ! */
	(current_proc->pa8)->oyvel.pos=(0x130000);
	(current_proc->pa8)->oxvel.pos=(long)current_proc->a11;

	/* fdp4 */
	do
	{
		process_sleep(1);
		ptemp=process_exist(PID_SCROLL,0xffff);
	} while (ptemp->p_comp_flag!=PC_PIT_STOP);

	set_inviso(current_proc->pa8);
	(current_proc->pa8)->oyvel.pos=0;
	(current_proc->pa8)->oxvel.pos=0;

	process_sleep(0x20);									// pause for dramatic effect
	clear_inviso(current_proc->pa8);

	flight(current_proc->pa8,(long)current_proc->a11,SCY(0x130000),SCY(0x6000),5,ANIM_KDOWN);

	create_fx(FX_PIT_MULCH);

	if ((current_proc->pa8)->ochar==FT_SMOKE)			// kill smoke dont need at this point
		process_kill_class(PID_SMOKER,-1);

	ground_y=(current_proc->pa8)->oypos.u.intpos+SCY(0x50);

	CREATE(PID_FX,machine_sound);
	CREATE(PID_FX,bone_grind_sound);

	shake_a8_up(current_proc->pa8,5,0,3,3);
	set_inviso(current_proc->pa8);

#if PAL_VERSION==1
	process_sleep(0x80*6/5);
#else
	process_sleep(0x60);
#endif
	death_blow_complete();
	f_no_bonus=0;								// allow bonus routine to do its thing
	wait_forever();
}

void machine_sound(void)
{
	current_proc->a10=6;
	do
	{
		tsound(0x22);
		process_sleep(0x10);
	} while ( --current_proc->a10>0 );

	process_suicide();
}

void bone_grind_sound(void)
{
	current_proc->a10=3;
	do
	{
		tsound(0x23);
		process_sleep(0x10);
	} while ( --current_proc->a10>0 );

	process_suicide();
}

void pit_fall_scan(void)
{
	long ta0;

	(long)current_proc->a11=(current_proc->pa8)->oxvel.pos;
	ta0=(current_proc->pa8)->oyvel.pos;
	if (ta0>SCY(0x130000))
	{
		(current_proc->pa8)->ograv.pos=0;
		(current_proc->pa8)->oyvel.pos=SCY(0x130000);

	}

	return;
}

