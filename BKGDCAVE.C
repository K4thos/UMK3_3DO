/******************************************************************************
 File: bkgdcave.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III cave Routines
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
#include "mkguys.h"
#include "mkjoy.h"

/* Prototypes */
void make_hell_flames(void);

/************************************************************
 * 			S O U L  S T U F F 							*
 ************************************************************/

/********************
 Externals
 ********************/

/*****************
 Externals
*****************/
extern void *cave_anims[];
extern void *a_cave_sky[];
extern void *a_cave_sky_f[];
extern void *CAVESKY1[];
extern void *CAVESKY2[];
extern void *CAVESKY3[];
extern void *CAVESKY4[];
extern void *CAVESKY5[];
extern void *CAVESKY6[];
extern void *CAVESKY7[];
extern void *CAVESKY8[];
extern void *CAVESKY9[];
extern void *CAVESKY10[];
extern void *CAVESKY11[];

extern void *CAVE_SMALL01[];
extern void *CAVE_SMALL02[];
extern void *CAVE_SMALL03[];
extern void *CAVE_SMALL04[];
extern void *CAVE_SMALL05[];
extern void *CAVE_SMALL06[];
extern void *CAVE_SMALL07[];
extern void *CAVE_SMALL08[];
extern void *CAVE_SMALL09[];
extern void *CAVE_SMALL10[];
extern void *CAVE_SMALL11[];
extern void *CAVE_SMALL12[];
extern void *CAVE_MEDIUM01[];
extern void *CAVE_MEDIUM02[];
extern void *CAVE_MEDIUM03[];
extern void *CAVE_MEDIUM04[];
extern void *CAVE_MEDIUM05[];
extern void *CAVE_MEDIUM06[];
extern void *CAVE_MEDIUM07[];
extern void *CAVE_MEDIUM08[];
extern void *CAVE_MEDIUM09[];
extern void *CAVE_MEDIUM10[];
extern void *CAVE_MEDIUM11[];
extern void *CAVE_MEDIUM12[];
extern void *CAVE_LARGE01[];
extern void *CAVE_LARGE02[];
extern void *CAVE_LARGE03[];
extern void *CAVE_LARGE04[];
extern void *CAVE_LARGE05[];
extern void *CAVE_LARGE06[];
extern void *CAVE_LARGE07[];
extern void *CAVE_LARGE08[];
extern void *CAVE_LARGE09[];
extern void *CAVE_LARGE10[];
extern void *CAVE_LARGE11[];
extern void *CAVE_LARGE12[];
extern void *CAVE_XPLODE01[];
extern void *CAVE_XPLODE02[];
extern void *CAVE_XPLODE03[];
extern void *CAVE_XPLODE04[];
extern void *CAVE_XPLODE05[];
extern void *CAVE_XPLODE06[];
extern void *CAVE_XPLODE07[];
extern void *CAVE_XPLODE08[];
extern void *CAVE_XPLODE09[];
extern void *CAVE_XPLODE10[];
extern void *CAVE_XPLODE11[];
extern void *CAVE_XPLODE12[];

extern void *hell_anims[];
extern void *a_hell_flames[];
extern void *a_hell_hit[];
extern void *HELL_STBREAK3[];
extern void *HELL_STBREAK2[];
extern void *HELL_STBREAK1[];
extern void *HELLFLAME01[];
extern void *HELLFLAME02[];
extern void *HELLFLAME03[];
extern void *HELLFLAME04[];
extern void *HELLFLAME05[];
extern void *HELLFLAME06[];
extern void *HELLHIT1[];
extern void *HELLHIT2[];
extern void *HELLHIT3[];
extern void *HELLHIT4[];
extern void *HELLHIT5[];
extern void *HELLHIT6[];
extern void *HELLHIT7[];
extern void *HELLHIT8[];
extern WORD HELLHIT1_P[];

/********************
 ********************/
ADDRESS *hell_perm_list[] =
{
	(ADDRESS *)HELLFLAME01,
	(ADDRESS *)HELLFLAME02,
	(ADDRESS *)HELLFLAME03,
	(ADDRESS *)HELLFLAME04,
	(ADDRESS *)HELLFLAME05,
	(ADDRESS *)HELLFLAME06,
	(ADDRESS *)HELL_STBREAK1,
	(ADDRESS *)HELL_STBREAK2,
	(ADDRESS *)HELL_STBREAK3,
	(ADDRESS *)HELLHIT1,
	(ADDRESS *)HELLHIT2,
	(ADDRESS *)HELLHIT3,
	(ADDRESS *)HELLHIT4,
	(ADDRESS *)HELLHIT5,
	(ADDRESS *)HELLHIT6,
	(ADDRESS *)HELLHIT7,
	(ADDRESS *)HELLHIT8,
	(ADDRESS *)CAVE_SMALL01,
	(ADDRESS *)CAVE_SMALL02,
	(ADDRESS *)CAVE_SMALL03,
	(ADDRESS *)CAVE_SMALL04,
	(ADDRESS *)CAVE_SMALL05,
	(ADDRESS *)CAVE_SMALL06,
	(ADDRESS *)CAVE_SMALL07,
	(ADDRESS *)CAVE_SMALL08,
	(ADDRESS *)CAVE_SMALL09,
	(ADDRESS *)CAVE_SMALL10,
	(ADDRESS *)CAVE_SMALL11,
	(ADDRESS *)CAVE_SMALL12,
	(ADDRESS *)CAVE_MEDIUM01,
	(ADDRESS *)CAVE_MEDIUM02,
	(ADDRESS *)CAVE_MEDIUM03,
	(ADDRESS *)CAVE_MEDIUM04,
	(ADDRESS *)CAVE_MEDIUM05,
	(ADDRESS *)CAVE_MEDIUM06,
	(ADDRESS *)CAVE_MEDIUM07,
	(ADDRESS *)CAVE_MEDIUM08,
	(ADDRESS *)CAVE_MEDIUM09,
	(ADDRESS *)CAVE_MEDIUM10,
	(ADDRESS *)CAVE_MEDIUM11,
	(ADDRESS *)CAVE_MEDIUM12,
	(ADDRESS *)CAVE_LARGE01,
	(ADDRESS *)CAVE_LARGE02,
	(ADDRESS *)CAVE_LARGE03,
	(ADDRESS *)CAVE_LARGE04,
	(ADDRESS *)CAVE_LARGE05,
	(ADDRESS *)CAVE_LARGE06,
	(ADDRESS *)CAVE_LARGE07,
	(ADDRESS *)CAVE_LARGE08,
	(ADDRESS *)CAVE_LARGE09,
	(ADDRESS *)CAVE_LARGE10,
	(ADDRESS *)CAVE_LARGE11,
	(ADDRESS *)CAVE_LARGE12,
	(ADDRESS *)CAVE_XPLODE01,
	(ADDRESS *)CAVE_XPLODE02,
	(ADDRESS *)CAVE_XPLODE03,
	(ADDRESS *)CAVE_XPLODE04,
	(ADDRESS *)CAVE_XPLODE05,
	(ADDRESS *)CAVE_XPLODE06,
	(ADDRESS *)CAVE_XPLODE07,
	(ADDRESS *)CAVE_XPLODE08,
	(ADDRESS *)CAVE_XPLODE09,
	(ADDRESS *)CAVE_XPLODE10,
	(ADDRESS *)CAVE_XPLODE11,
	(ADDRESS *)CAVE_XPLODE12,
	NULL
};

/******************************************************************************
 Function: void make_hell_flames(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	setup the caves background
******************************************************************************/
DTBL_INIT hell_flame_table [] =
{
	{4,HELLFLAME01,SCX(0x84)+24+8+2,(0x005e)+7,a_hell_flames,&baklst3},
	{4,HELLFLAME01,SCX(0x27d)+24+8+2,(0x005e)+7,a_hell_flames,&baklst3},
	{0}
};
void make_hell_flames(void)
{
	alloc_vram_perm_bg_list((ADDRESS *)hell_perm_list,&hell_anims);

	create_dumb_animators_w(hell_flame_table,&hell_anims);

	return;
}

/* Prototypes */
void calla_cave(void);

/************************************************************
 * 			B A L C O N Y & T O W E R  S T U F F 			*
 ************************************************************/


/*****************
*****************/

ADDRESS *cave_perm_list[]=
{
	(ADDRESS *)CAVESKY1,
	(ADDRESS *)CAVESKY2,
	(ADDRESS *)CAVESKY3,
	(ADDRESS *)CAVESKY4,
	(ADDRESS *)CAVESKY5,
	(ADDRESS *)CAVESKY6,
	(ADDRESS *)CAVESKY7,
	(ADDRESS *)CAVESKY8,
	(ADDRESS *)CAVESKY9,
	(ADDRESS *)CAVESKY10,
	(ADDRESS *)CAVESKY11,
	(ADDRESS *)CAVE_SMALL01,
	(ADDRESS *)CAVE_SMALL02,
	(ADDRESS *)CAVE_SMALL03,
	(ADDRESS *)CAVE_SMALL04,
	(ADDRESS *)CAVE_SMALL05,
	(ADDRESS *)CAVE_SMALL06,
	(ADDRESS *)CAVE_SMALL07,
	(ADDRESS *)CAVE_SMALL08,
	(ADDRESS *)CAVE_SMALL09,
	(ADDRESS *)CAVE_SMALL10,
	(ADDRESS *)CAVE_SMALL11,
	(ADDRESS *)CAVE_SMALL12,
	(ADDRESS *)CAVE_MEDIUM01,
	(ADDRESS *)CAVE_MEDIUM02,
	(ADDRESS *)CAVE_MEDIUM03,
	(ADDRESS *)CAVE_MEDIUM04,
	(ADDRESS *)CAVE_MEDIUM05,
	(ADDRESS *)CAVE_MEDIUM06,
	(ADDRESS *)CAVE_MEDIUM07,
	(ADDRESS *)CAVE_MEDIUM08,
	(ADDRESS *)CAVE_MEDIUM09,
	(ADDRESS *)CAVE_MEDIUM10,
	(ADDRESS *)CAVE_MEDIUM11,
	(ADDRESS *)CAVE_MEDIUM12,
	(ADDRESS *)CAVE_LARGE01,
	(ADDRESS *)CAVE_LARGE02,
	(ADDRESS *)CAVE_LARGE03,
	(ADDRESS *)CAVE_LARGE04,
	(ADDRESS *)CAVE_LARGE05,
	(ADDRESS *)CAVE_LARGE06,
	(ADDRESS *)CAVE_LARGE07,
	(ADDRESS *)CAVE_LARGE08,
	(ADDRESS *)CAVE_LARGE09,
	(ADDRESS *)CAVE_LARGE10,
	(ADDRESS *)CAVE_LARGE11,
	(ADDRESS *)CAVE_LARGE12,
	(ADDRESS *)CAVE_XPLODE01,
	(ADDRESS *)CAVE_XPLODE02,
	(ADDRESS *)CAVE_XPLODE03,
	(ADDRESS *)CAVE_XPLODE04,
	(ADDRESS *)CAVE_XPLODE05,
	(ADDRESS *)CAVE_XPLODE06,
	(ADDRESS *)CAVE_XPLODE07,
	(ADDRESS *)CAVE_XPLODE08,
	(ADDRESS *)CAVE_XPLODE09,
	(ADDRESS *)CAVE_XPLODE10,
	(ADDRESS *)CAVE_XPLODE11,
	(ADDRESS *)CAVE_XPLODE12,
	NULL
};

/******************************************************************************
 Function: void calla_cave(void)

 By: David Schwartz

 Date:  Jan 1995

 Parameters: None

 Returns: None

 Description:	init cave
******************************************************************************/
DTBL_INITA cave_sky_table [] =
{
	{4,SCX(0x84)+0x18,(0x005e)-54-3,a_cave_sky,&baklst4},
	{0x8004,SCX(0x17d)+0xac,(0x005e)-54-2,a_cave_sky_f,&baklst4},
	{0}
};

void cave_floor_glow(void);

void calla_cave(void)
{
	alloc_vram_perm_bg_list((ADDRESS *)cave_perm_list,&cave_anims);

	CREATE(PID_CYCLE,cave_floor_glow);

	create_dumb_animators(cave_sky_table,&cave_anims);

	return;
}

extern WORD CAVFLR_P[];
extern WORD CAVEFLRx_PAL[];
extern WORD CAVEFLRy_PAL[];
extern WORD CAVEFLRz_PAL[];

WORD *cfg_tbl[]=
{
	CAVFLR_P,
	CAVEFLRx_PAL,
	CAVEFLRy_PAL,
	CAVEFLRz_PAL,
	CAVEFLRy_PAL,
	CAVEFLRx_PAL,
	CAVFLR_P,
	NULL
};

void cave_floor_glow(void) 
{
	WORD **ta10;

	while (1)
	{
		ta10=cfg_tbl;

		do
		{
			free_pal(skew_constpal);
			skew_constpal=get_fore_pal((void *)(*ta10));
			process_sleep(3+randu(9));
			
			ta10++;
		}
		while(*ta10!=NULL);
	}
}


#if 1
void fall_in_lava(void) 
{
	OBJECT *obj=current_proc->pa8;
	short ta0;

	f_shadows=0;

	(short)current_proc->pdata.p_store6=obj->oxpos.u.intpos;
	group_sound(9);						// death scream

	dallprc(PID_SCROLL);
	dallprc(PID_REPELL);
	dallprc(PID_SMOKER);

	obj->oyvel.pos=-SCY(0x100000);			// zoom off screen
	get_char_ani(ANIM_TABLE1,ANIM_KDOWN);
	init_anirate(6);

	/* trax3 */
	do
	{
		process_sleep(1);
		next_anirate();
	}
	while(distance_off_ground(current_proc,obj)<SCY(0x100));

	stop_me(obj);
	clear_shadow_bit(obj);

	create_fx(FX_SUBWAY_ROX);
	process_sleep(0x50);

	current_proc->a10=(ADDRESS)current_proc->pa8;
	pose_a9(2,ANIM_KDOWN);
	obj->oscale.yx=0x01c001c0;
	clear_shadow_bit(obj);
	obj->ochar=((OBJECT*)current_proc->a10)->ochar;
	obj->oid=((OBJECT*)current_proc->a10)->oid;

	if (current_proc->procid==PID_P1)
	{
		delobj(p1_obj);				// pull off objlst
		obj_free=obj_free->olink;			// remove from free list so it can be active again
		p1_obj=obj;
	}
	else
	{
		delobj(p2_obj);				// pull off objlst
		obj_free=obj_free->olink;			// remove from free list so it can be active again
		p2_obj=obj;					// stuff new obj
	}

	player_froze_pal();
	player_normpal();

	insert_object(obj,&baklst7);
	obj->oypos.u.intpos=0;

	set_noscroll(obj);

	ta0=(current_proc->pdata.p_otherguy)->oxpos.u.intpos;
	ta0-=worldtlx.u.intpos;

	if (ta0<=SCRRGT/2)
		ta0+=SCX(0x80);
	else ta0-=SCX(0x80);

	if (ta0<SCX(0x80))
		ta0=SCX(0x80);

	obj->oxpos.u.intpos=ta0;

	group_sound(2);
	obj->oyvel.pos=SCY(0x20000);
	obj->ograv.pos=SCY(0x6000);

	/* lava8 */
	do
	{
		process_sleep(1);
	}
	while(obj->oypos.u.intpos<SCY(0xb2)-24);

	shake_a11(4,6);
	tsound(0x5f);				// -DHS- sound
	stop_me(obj);
	obj->oypos.u.intpos+=(obj->header.t_height<<8)/obj->oscale.u.ypos;
	obj->oscale.yx=0x01000100;

	current_proc->pa9=a_hell_hit;
	obj->oheap=hell_anims;
	obj->oflags &=(~M_FIGHTER);		// not a fighter anymore, clear the flag
	obj->oflags2 &=~M_MULTIPART;
	swpal(HELLHIT1_P,obj);
	framew(5);
	obj->oypos.u.intpos=0xf00;			// offscreen please
	process_sleep(0x20);

	if ((swcurr.u.p1pad & P1B5) && (swcurr.u.p2pad & P2B5)) 
	{
		tsound(0x66);				// crispy if both runs
		process_sleep(0x40);
	}

	if ((swcurr.u.p1pad & P1B0) && (swcurr.u.p2pad & P2B0)) 
	{
		current_proc->a11=0x1d;				// speech crispy
		create_fx(FX_LAVA_DAN);
		process_sleep(0x48);
	}

	death_blow_complete();

	wait_forever();
}
#endif