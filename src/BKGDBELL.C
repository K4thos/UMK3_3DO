/******************************************************************************
 File: bkgdbell.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Bell Routines
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
#include "mkblood.h"
#include "mkfatal.h"

/* Prototypes */
void calla_bell(void);

/************************************************************
 * 			B E L L  S T U F F 								*
 ************************************************************/

/********************
 Externals
 ********************/
extern void *bell_anims[];
extern void *BELLHOLE2[];
extern void *BELLHOLE1A[];
extern void *BELLHOLE1B[];
extern void *EJBSPIKE[];

ADDRESS *bell_perm_list[]=
{
	(ADDRESS *)BELLHOLE2,
	(ADDRESS *)BELLHOLE1A,
	(ADDRESS *)BELLHOLE1B,
	(ADDRESS *)EJBSPIKE,
	NULL
};


/******************************************************************************
 Function: void calla_bell(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	call bell
******************************************************************************/
void calla_bell(void)
{
	alloc_vram_perm_bg_list((ADDRESS *)bell_perm_list,&bell_anims);
	return;
}

/******************************************************************************
 Function: void fall_down_bell_tower(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	die by bell tower fall
******************************************************************************/
void fall_down_bell_tower(void)
{
	current_proc->pdata.p_store7=(ADDRESS)current_proc->pa8;
	group_sound(9);
	clear_shadow_bit(current_proc->pa8);
	center_around_me();

	shake_a11(0x6,0xa);

	dallprc(PID_REPELL);

	(long)current_proc->a0=((SCX(600)-(current_proc->pa8)->oxpos.u.intpos)<<16)/0x50;
	(FUNC)current_proc->a6=bell_fall_scan;
	flight_call(current_proc->pa8,(long)current_proc->a0,-SCY(0x120000),SCY(0x6000),5,ANIM_KDOWN);
	stop_a8(current_proc->pa8);
	wait_forever();
}

#define ONE_FLOOR SCY(0x1ad)
extern void *bell2_module[];
void bell_fall_scan(void)
{
	long a0,a1,a2,a3,a4,a5,a6,a7;
	OBJECT *oa8,*oa0;
	OBJECT *obj=current_proc->pa8;

	if (distance_off_ground(current_proc,current_proc->pa8)<=224)
		return;						// wait to get off screen

	stop_me(current_proc->pa8);
	obj->oxpos.u.intpos=0xb0;		// SCX(600);
	process_sleep(0x50);			// let scroller catch up!

	set_noedge(current_proc->pdata.p_otherguy);		// dont allow otherguy to update position with neew world edges

	obj->oyvel.pos=SCY(0xc0000);
	obj->ograv.pos=0;
	set_x_vel(current_proc->pa8,0);

	/* bfall3 */
	do
	{
		process_sleep(1);
	}
	while(distance_off_ground(current_proc,current_proc->pa8)>SCY(0x10));

	dallprc(PID_BACKG);
	dallprc(PID_SCROLL);
	stop_scrolling();

	a0=worldtlx.pos;
	a1=worldtlx1.pos;
	a2=worldtlx2.pos;
	a3=worldtlx3.pos;
	a4=worldtlx4.pos;
	a5=worldtlx5.pos;
	a6=worldtlx6.pos;
	a7=worldtlx7.pos;

	/* init */
	init_background_module(bell2_module);
	worldtly.u.intpos-=SCY(0x50);
	multi_plane();
	worldtly.u.intpos+=SCY(0x50);

	worldtlx.pos=a0;
	worldtlx1.pos=a1;
	worldtlx2.pos=a2;
	worldtlx3.pos=a3;
	worldtlx4.pos=a4;
	worldtlx5.pos=a5;
	worldtlx6.pos=a6;
	worldtlx7.pos=a7;

	(OBJECT *)a0=baklst8;
	a7=0;
	/* adj8 */
	do
	{
		a1=((OBJECT *)a0)->oypos.u.intpos=((OBJECT *)a0)->oypos.u.intpos+ONE_FLOOR;
		a1+=((OBJECT *)a0)->osize.u.ypos;
		if (a1>=a7)
			a7=a1;				// new lowest point

		(OBJECT *)a0=((OBJECT*)a0)->olink;
	}
	while((OBJECT *)a0!=NULL);

	/* tack on xtra pieces to bottom of floor */
	(long)current_proc->a7=a7;
	(long)current_proc->a6=worldtlx.u.intpos;
	current_proc->a3=0;
	floor_piece();
	broken_floor_a();
	broken_floor_b();
	current_proc->a3^=1;
	floor_piece();

	CREATE(PID_FX,crash_fx)->pa8=(OBJECT *)(current_proc->pdata.p_store7);
	scrolly.pos=SCY(0xc0000);					// dont fall 2 fast

	/* wait_old_offscreen */
	do
	{
		process_sleep(1);
	}
	while(worldtly.u.intpos<SCRBOT);

	current_proc->pa8=(OBJECT*)current_proc->pdata.p_store7;
	clear_shadow_bit(current_proc->pdata.p_otherguy);
	set_inviso(current_proc->pdata.p_otherguy);

	/* get rid of bell tower #1 images */
	f_skew=0;
	delolist(&baklst1);
	delolist(&baklst2);
	delolist(&baklst3);
	delolist(&baklst4);
	delolist(&baklst5);
	delolist(&baklst6);
	delolist(&baklst7);

	/* make another copy of bell2 on list 7 */
	(short)current_proc->a10=worldtly.u.intpos;
	worldtlx7.pos=worldtlx8.pos;
	current_proc->pa8=oa8=baklst8;
	oa0=get_object();
	copy_obj(oa8,oa0);
	baklst7=oa0;
	goto copy81;

	/* copy8 */
	do
	{
		oa0=get_object();
		copy_obj(oa8,oa0);
		((OBJECT *)current_proc->pa9)->olink=oa0;
copy81:
		oa0->oypos.u.intpos+=ONE_FLOOR;
		oa0->olink=NULL;
		(OBJECT *)current_proc->pa9=oa0;
		oa8=oa8->olink;
	}
	while(oa8!=NULL);

	scroll_down_1_floor();
	baklst_down_2_floors(&baklst8);
	scroll_down_1_floor();
	baklst_down_2_floors(&baklst7);

	/* speed up past the guy !! */
	scrolly.pos=SCY(0x100000);				// dont fall to fast
	scroll_down_1_floor();

	/* fix borken pieces !!! */
	fix_broken_pieces(baklst7);
	fix_broken_pieces(baklst8);

	baklst_down_2_floors(&baklst8);
	scroll_down_1_floor();
	baklst_down_2_floors(&baklst7);
	scroll_down_1_floor();

	current_proc->a10=14;
	(short)current_proc->a3=worldtlx.u.intpos;
	/* spike3 */
	do
	{
		oa0=make_a_spike();
		oa0->ozval=0;

		oa0=make_a_spike();
		oa0->ozval=100;

	}
	while(--current_proc->a10>0 );

	process_sleep(5);
	scrolly.pos=0;
	current_proc->pa8=oa8=(OBJECT*)current_proc->pdata.p_store7;

	/* falld3, fall till I meet my maker !!! */
	do
	{
		process_sleep(1);
	}
	while(oa8->oypos.u.intpos<SCY(0xa40) );

	rsnd_stab();
	death_scream;
	find_ani_part2(ANIM_KDOWN);
	do_next_a9_frame(oa8);

	oa8->oyvel.pos=SCY(0xa0000);

	/* falld4 */
	do
	{
		process_sleep(1);
		oa8->oyvel.pos>>=4;
	}
	while(oa8->oyvel.pos>SCY(0x10000) );

	ground_y=oa8->oypos.u.intpos+SCY(0x100);
	create_blood_proc(BLOOD_PIT);

	process_sleep(10);

	death_blow_complete();
	f_no_bonus=0;								// allow bonus routine to do its thing
	stop_a8(oa8);
	wait_forever();
}

OBJECT *make_a_spike(void)
{
	OBJECT *obj;

	gso_dmawnz(obj,(ADDRESS)EJBSPIKE,bell_anims,0);
	alloc_cache((OIMGTBL *)EJBSPIKE,&bell_anims,obj);
	obj->oxpos.u.intpos=(short)current_proc->a3;
	obj->oypos.u.intpos=SCY(0xa80)+12;
	(short)current_proc->a3+=SCX(15);			// spike spacing
	insert_object(obj,&objlst);
	return(obj);
}

void fix_broken_pieces(OBJECT *pa8)
{
	OBJECT *oa7;
	WORD ta10;

	do
	{
		if (pa8->ozval==1000)
			break;
	}
	while(pa8=pa8->olink);

	/* fbp4 */
	oa7=pa8->olink;
	oa7=oa7->olink;				// jump ahead 2 - grab this y !!
	(short)current_proc->a7=oa7->oypos.u.intpos;
	(short)current_proc->a6=worldtlx8.u.intpos;
	current_proc->a3=0;

	ta10=4;
	/* fbp3 */
	do
	{
		pa8->oheap=bell_anims;
		current_proc->pa8=pa8;
		animate(SINGLE_ANI,(OIMGTBL*)&BELLHOLE2,pa8->oflags);
		if (current_proc->a3)
			flip_single(pa8);

		/* fbp6 */
		pa8->oxpos.u.intpos=(short)current_proc->a6;
		pa8->oypos.u.intpos=(short)current_proc->a7;

		(short)current_proc->a6+=pa8->osize.u.xpos;
		current_proc->a3^=1;

	}
	while((pa8=pa8->olink)!=NULL && --ta10>0);		// arcade mod, arcade pices at end
																								// of list ours are at begining (only mod 4 pieces)

	return;
}

void floor_piece(void)
{
		flr2((OIMGTBL*)&BELLHOLE2);
}

void broken_floor_a(void)
{
	(short)current_proc->a7-=SCY(25);
	flr2((OIMGTBL*)&BELLHOLE1A);
	(short)current_proc->a7+=SCY(25);
	return;
}

void broken_floor_b(void)
{
	(short)current_proc->a7-=SCY(25);
	flr2((OIMGTBL*)&BELLHOLE1B);
	(short)current_proc->a7+=SCY(25);
	return;
}

void flr2(OIMGTBL *pa5)
{
	OBJECT *obj;

	gso_dmawnz(obj,(ADDRESS)pa5,bell_anims,0);
	alloc_cache(pa5,&bell_anims,obj);
	if (current_proc->a3)
		flip_single(obj);

	/* flr3 */
	obj->oypos.u.intpos=(short)current_proc->a7;
	obj->oxpos.u.intpos=(short)current_proc->a6;
	obj->ozval=1000;
	(short)current_proc->a6+=obj->osize.u.xpos;
	insert_object(obj,&baklst8);
	return;
}

void baklst_down_2_floors(OBJECT **pa8)
{
	OBJECT *oa0=(OBJECT *)pa8;
	/* daf3 */
	while ((oa0=oa0->olink)!=NULL)
	{
		oa0->oypos.u.intpos+=ONE_FLOOR*2;
	}

	return;
}

void scroll_down_1_floor(void)
{
	short ta10;

	ta10=worldtly.u.intpos;

	/* osgb3 */
	do
	{
		process_sleep(1);
	}
	while(abs(worldtly.u.intpos-ta10)<ONE_FLOOR );

	return;
}

void crash_fx(void)
{
	process_sleep(3);
	current_proc->a10=6;
	/* crash3 */
	do
	{
		shake_a11(5,5);
		tsound(1);
		group_sound(2);
		process_sleep(0x23);
	}
	while(--current_proc->a10>0);

	process_sleep(1);
	tsound(0xb);

	process_suicide();
}
