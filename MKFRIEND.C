/******************************************************************************
 File: mkfriend.c

 Date: Jan 1995

 (C) Williams Entertainment

 Mortal Kombat III Friendship Routines
******************************************************************************/

/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkos.h"
#include "mkobj.h"
#include "mkfx.h"
#include "mkutil.h"
#include "mkani.h"
#include "mkgame.h"
#include "mkguys.h"
#include "mkanimal.h"
#include "mkfriend.h"
#include "mkzap.h"
#include "mkfatal.h"
#include "mkpal.h"
#include "mksound.h"
#include "mkcanned.h"
#include "mkfile.h"
#include "valloc.h"
#include "psxspu.h"

JUMPTBL ochar_friends[]=
{
	f_kano,			// 0
	f_sonya,			// 1
	f_jax,			// 2
	f_indian,   		// 3
	f_sz,			// 4
	f_swat,			// 5
	f_sindel,   		// 6
	f_sektor,   		// 7
	f_cyrax,			// 8
	f_lao,			// 9
	f_kabal,			// 10
	f_sheeva,  		// 11
	f_shang,			// 12
	f_liu,			// 13
	f_smoke_ninja,		// 14
	f_kitana,		// 15
	f_jade,			// 16
	fr_mileena,		// 17

	f_scorpion,
	f_reptile,
	f_null_friendship,
	f_null_friendship,
	f_null_friendship,
	f_null_friendship,
};

//******************************************************************************
//******************************************************************************
void fr_mileena(void) 
{
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	mframew(5);
	process_sleep(0x60);
	shake_a11(7,8);
	mframew(4);
	tsound(0x66);	
	process_sleep(0x20);
	stack_jump(friendship_complete);
}  

//******************************************************************************
//******************************************************************************
void f_scorpion(void) 
{
	jax_n_box_start();
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	current_proc->a5=(ADDRESS)current_proc->pa9;
	stack_jump(pop_up_my_toy);
}

void f_reptile(void)
{
	clear_inviso(current_proc->pa8);
	jax_n_box_start();
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	current_proc->a5=(ADDRESS)current_proc->pa9;
	stack_jump(pop_up_my_toy);
}

void pop_up_my_toy(void) 
{
	ADDRESS animframe;
	OBJECT *obj;

	tsound(0x92);
	shake_a11(6,6);
	takeover_him(r_scared_of_monkey);

	animframe=(ADDRESS)COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->pa9));
	gso_dmawnz(obj,(ADDRESS)animframe,(current_proc->pa8)->oheap,0);
	alloc_cache((OIMGTBL *)animframe,(current_proc->pa8)->oheap,obj);
	obj->ozval=100;
	lineup_1pwm(obj,(OBJECT*)current_proc->a11);
	insert_object(obj,&objlst);

	framew(4);
	process_sleep(0x30);
	death_blow_complete();
	wait_forever();
}

void jax_n_box_start(void) 
{
	center_around_me();
	get_char_ani(ANIM_TABLE2,ANIM_F2_FRIEND);
	do_next_a9_frame(current_proc->pa8);
	process_sleep(0x10);
	current_proc->a11=(ADDRESS)current_proc->pa8;
	mframew(6);
	return;
}

//******************************************************************************
//******************************************************************************
void f_jade(void) 
{
	kill_and_stop_scrolling();
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	animate_a0_frames(4,3);
	process_sleep(0x10);
	CREATE(PID_FX,friend_ender);

	current_proc->a10=(ADDRESS)current_proc->pa9;
	current_proc->a10-=4;			// 1 LONG
	current_proc->a11=(ADDRESS)current_proc->pa9;
	
	/* pogo_bounce */
	while (1)
	{
		(ADDRESS)current_proc->pa9=current_proc->a11;
		do_next_a9_frame(current_proc->pa8);
		flight(current_proc->pa8,SCX(0x0),-SCY(0xa0000),SCY(0x9000),NEVER_ANI,-1);
		tsound(0xc);

		(ADDRESS)current_proc->pa9=current_proc->a10;
		do_next_a9_frame(current_proc->pa8);
		process_sleep(1);											
	}
}

void friend_ender(void) 
{
	process_sleep(0x40*2);
	death_blow_complete();
	process_suicide();
}

void f_null_friendship(void) 
{
	process_sleep(0x40*2);
	death_blow_complete();
	stack_jump(friendship_complete);
}

//******************************************************************************
//******************************************************************************
void f_kitana(void) 
{
	PROCESS *pa0;

	/* calla_for_him */
	pa0=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	center_around_me();
	current_proc=pa0;

	animate2_a9(5,ANIM_F1_FRIEND);
	process_sleep(0x70);

	delete_slave();
	find_ani2_part2(ANIM_F2_FRIEND);
	mframew(4);
	stack_jump(friendship_complete);
}

void spawn_bubble(void) 
{
	CREATE(PID_BLOOD,bubble_proc);
	return;
}

void bubble_proc(void) 
{
	current_proc->a10=40;
	do
	{
		CREATE(PID_FX,bub_1_proc);
		process_sleep(3);
	}
	while(--current_proc->a10>0);
	process_suicide();
}

void bub_1_proc(void) 
{
	ADDRESS animframe;
	OBJECT *obj;

	find_ani2_part2(ANIM_F1_FRIEND);
	animframe=(ADDRESS)COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->pa9));
	current_proc->a11=(ADDRESS)current_proc->pa8;
	gso_dmawnz(obj,(ADDRESS)animframe,(current_proc->pa8)->oheap,0);
	alloc_cache((OIMGTBL *)animframe,(current_proc->pa8)->oheap,obj);
	lineup_1pwm(obj,(OBJECT*)current_proc->a11);
	insert_object(obj,&objlst);

	((ADDRESS *)current_proc->pa9)++;
	framew(1);

	set_proj_vel(current_proc->pa8,SCX(0x20000)+randu(SCX(0x30000)),-1);
	(current_proc->pa8)->oyvel.pos=srandarc(SCY(0x20000));

	process_sleep(20+randu(30));
	framew(4);
	delobjp(current_proc->pa8);
	process_suicide();
}


/******************************************************************************
 Function: void do_friendship(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	perform fighter friendship
******************************************************************************/
void do_friendship(void)
{
	friendship_start_pause();
	init_special(current_proc->pa8);
	ochar_friends[(current_proc->pa8)->ochar]();
}

void friendship_start_pause(void)
{
	OBJECT *obj=current_proc->pa8;

	psxspu_start_cd_fade(FADE_TIME,0);

	init_death_blow(FAT_FRIEND);

	/* load friendship file data */
	player_heap_adj();
	character_texture_load(obj->ochar,CHAR_FRIEND,finish_heap_ptr,FATAL_LOAD|ASYNC_LOAD);

	/* sleep for fade */
	process_sleep(0x20);

#if _CD_VERSION_ == 1
	while ( psxcd_async_on() )
		process_sleep(1);				// what for async load to complete
	module_os_close(module_int);			// make sure to close async file
#endif

	/* load friendship sound data */
	if ( obj==p1_obj )
		PsxSoundLoadFighter1Babality(p1_obj->ochar);
	else PsxSoundLoadFighter2Babality(p2_obj->ochar);

	nosounds();
	play_generic_tune(TRK_FRIENDP1);
	if ( f_music!=0 )
	{
#if _CD_VERSION_ == 1
		while ( psxcd_elapsed_sectors()<75)
			process_sleep(1);
#endif
	}
	//send_code_a3(0x42);

#if 1
	/* sony add for memory */
	player_heap_ptr=(current_proc->pa8)->oheap;
	(current_proc->pa8)->oheap=finish_heap_ptr;		// make sure and restore at end of friendship
#endif

	f_no_bonus=0;								// allow bonus routine to do its thing

	return;
}

void friendship_complete(void)
{
	death_blow_complete();
	player_normpal();

#if 1
	/* sony add for memory */
	(current_proc->pa8)->oheap=player_heap_ptr;		// make sure and restore at end of animality
#endif

	wait_forever();
}

/******************************************************************************
 Function: void f_kano(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	perform kano friendship
******************************************************************************/
extern void *a_kano_friend[];
void f_kano(void)
{
	//current_proc->pa9=a_kano_friend;
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	mframew_5;
	stack_jump(friendship_complete);
}

/******************************************************************************
 Function: void f_sonya(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	perform sonya friendship
******************************************************************************/
void f_sonya(void)
{
	current_proc->a10=25;
	do
	{
		CREATE(PID_BLOOD,sonya_flower_proc);
	}
	while(--current_proc->a10>0);

	process_sleep(0x30);
	death_blow_complete();
	player_normpal();

	/* sony add for memory */
	(current_proc->pa8)->oheap=player_heap_ptr;		// make sure and restore at end of
	stack_jump(victory_animation);
}

void sonya_flower_proc(void) 
{
	ADDRESS animframe;
	OBJECT *obj;

	process_sleep(20+randu(10));
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	animframe=(ADDRESS)COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->pa9));
	gso_dmawnz_ns(obj,(ADDRESS)animframe,(current_proc->pa8)->oheap,0);
	alloc_cache((OIMGTBL *)animframe,(current_proc->pa8)->oheap,obj);

	obj->oxpos.u.intpos=randu(SCRRGT);

	ground_a8();
	obj->oypos.u.intpos+=randu(SCY(5));
	a8_front_plus_1(obj);
	insert_object(obj,&objlst);

	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	framew(3+randu(6));
	process_suicide();
}

/******************************************************************************
 Function: void f_jax(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	perform jax friendship
******************************************************************************/
void f_jax(void)
{
	CREATE(PID_BANI,end_friend_proc);
	//current_proc->pa9=a_jax_friend;
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	do_next_a9_frame(current_proc->pa8);
	process_sleep(0x20);
	mframew_3;
	stack_jump(friendship_complete);
}

void end_friend_proc(void)
{
	process_sleep(0x80);
	death_blow_complete();
	process_suicide();
}

/******************************************************************************
 Function: void f_indian(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	perform indian friendship
******************************************************************************/
//extern void *a_ind_friend[];
//extern OIMGTBL *MK2GAME;
char txt_kano[]="I'VE NEVER SEEN A KANO TRANSFORMATION";
char txt_raiden1[]="NO, BUT I KNOW HOW TO DO";
char txt_raiden2[]="A RAYDEN TRANSFORMATION";
void f_indian(void)
{
	OBJECT *obj,*ta8;
	void *animframe;

	//current_proc->pa9=a_ind_friend;
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);

	other_ochar_sound(4,FT_ST);

	mframew_5;
	process_sleep(0x20);

	ta8=current_proc->pa8;
	current_proc->a10=(ADDRESS)current_proc->pa8;

	current_proc->a0=(ADDRESS)current_proc->pa9;
	animframe=(void*)get_char_ani(ANIM_TABLE2,ANIM_F2_FRIEND);		// get mk2game frame ptr
	current_proc->pa9=(void*)current_proc->a0;
	animframe=(void*)COMPUTE_ADDR(ta8->oheap,GET_LONG(animframe));

	gso_dmawnz(obj,(ADDRESS)animframe,ta8->oheap,0);
	alloc_cache((OIMGTBL*)animframe,ta8->oheap,obj);

	lineup_1pwm(obj,(OBJECT *)current_proc->a10);
	multi_adjust_xy(obj,-(SCX(0x70)),-SCY(0x100));
	insert_object(obj,&objlst);
	gravity_ani_ysize(SCY(0x20000),SCY(0x6000));
	shake_n_sound();

	if ( random()<0 )
	{
		p7_centered(txt_raiden1,160,SCY(0xf0)-14);
		p7_centered(txt_raiden2,160,SCY(0xf0));
	}
	else
	{
		p7_centered(txt_kano,160,SCY(0xf0));
	}

	current_proc->pa8=ta8;
	process_sleep(0x60);
	death_blow_complete();

	wait_forever();
}

void other_ochar_sound(WORD pa0,WORD pa1)
{
	WORD ta2;

	ta2=(current_proc->pa8)->ochar;
	(current_proc->pa8)->ochar=pa1;
	ochar_sound(pa0);
	(current_proc->pa8)->ochar=ta2;
	return;
}

/******************************************************************************
 Function: void f_sz(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	perform subzero friendship
******************************************************************************/
//extern void *a_sz_friend[];
void f_sz(void)
{
	//current_proc->pa9=a_sz_friend;
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	ochar_sound(0);
	mframew_5;
	process_sleep(0x20);
	stack_jump(friendship_complete);
}

/******************************************************************************
 Function: void f_swat(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	perform swat friendship
******************************************************************************/
//extern void *a_swat_friend[];
void f_swat(void)
{
	CREATE(PID_BANI,swat_friend_proc);

	//current_proc->a11=(curback==BKGD_SOUL_MOD)?6:13;

	/* swatf3 */
#if 0
	do
	{
		CREATE(PID_BANI,swat_crossing_people);
	}
	while(--current_proc->a11>0);
#endif
	CREATE(PID_BANI,swat_crossing_people);

	//current_proc->pa9=a_swat_friend;
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	do_next_a9_frame(current_proc->pa8);

	if ((current_proc->pa8)->oflags & M_FLIPH)
		flip_multi(current_proc->pa8);

	mframew_5;
	ochar_sound(8);
	process_sleep(0x40);
	mframew_5;
	stack_jump(friendship_complete);
}

void swat_friend_proc(void)
{
	process_sleep(0xa0);
	death_blow_complete();
	process_suicide();
}

void swat_crossing_people(void)
{
	OBJECT *odog,*owagon;
	void *dogani,*wagonani;

	current_proc->pdata.p_otherguy=current_proc->pa8;

	process_sleep(0x50);

	/* create dog */
	get_char_ani(ANIM_TABLE2,ANIM_F2_FRIEND);						// get ptr to anim table
	dogani=current_proc->pa9;
	current_proc->pa9=COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->pa9));	// get ptr to frame
	gso_dmawnz(odog,(ADDRESS)current_proc->pa9,(current_proc->pa8)->oheap,0);
	alloc_cache((OIMGTBL *)current_proc->pa9,(current_proc->pa8)->oheap,odog);

	odog->oxpos.u.intpos=worldtlx.u.intpos-SCX(0x30);
	odog->oxvel.pos=SCX(0x30000);
	odog->oypos.u.intpos=ground_y-SCY(0x20);
	odog->ozval=100;
	insert_object(odog,&objlst);

	/* create wagon */
	get_char_ani(ANIM_TABLE2,ANIM_F3_FRIEND);						// get ptr to anim table
	wagonani=current_proc->pa9;
	current_proc->pa9=COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->pa9));	// get ptr to frame
	gso_dmawnz(owagon,(ADDRESS)current_proc->pa9,(current_proc->pa8)->oheap,0);
	alloc_cache((OIMGTBL *)current_proc->pa9,(current_proc->pa8)->oheap,owagon);

	match_ani_points(odog,owagon);				// match to two
	multi_adjust_xy(owagon,-66,-25);				// lineup wagon with dog
	owagon->oxvel.pos=SCX(0x30000);				// set speed to match dog
	owagon->ozval=200;
	insert_object(owagon,&objlst);

	/* cross */
	do
	{
		process_sleep(3);

		/* update dog */
		current_proc->pa8=odog;
		current_proc->pa9=dogani;
		frame_a9(odog);
		dogani=current_proc->pa9;

		/* update wagon */
		current_proc->pa8=owagon;
		current_proc->pa9=wagonani;
		frame_a9(owagon);
		wagonani=current_proc->pa9;
	} while (abs(worldtlx.u.intpos-odog->oxpos.u.intpos)<700);

	delobjp(odog);
	delobjp(owagon);

	process_suicide();
}

/******************************************************************************
 Function: void f_sindel(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	perform sindel friendship
******************************************************************************/
//extern void *a_lia_friend[];
//extern void *a_football[];
void f_sindel(void)
{
	PROCESS *ta0;

	//current_proc->pa9=a_lia_friend;
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	mframew_5;

	rsnd_big_block();
	shake_a11(0x3,0x3);

	ta0=CREATE(PID_BANI,football_proc);
	ta0->pa9=(void *)current_proc->pdata.p_slave;
	current_proc->a10=(ADDRESS)ta0;
	current_proc->pdata.p_slave=NULL;
	do_next_a9_frame(current_proc->pa8);
	process_sleep(0x10);

	mframew_5;
	ochar_sound(9);
	process_sleep(0x40);
	stack_jump(friendship_complete);
}

void football_proc(void)
{
	if ((current_proc->pa8)->oflags & M_FLIPH)
		((OBJECT*)current_proc->pa9)->oxvel.pos=-SCX(0xa0000);
	else ((OBJECT*)current_proc->pa9)->oxvel.pos=SCX(0xa0000);

	((OBJECT*)current_proc->pa9)->oyvel.pos=-SCY(0xa0000);
	current_proc->pa8=(OBJECT *)current_proc->pa9;
	get_char_ani(ANIM_TABLE2,ANIM_F2_FRIEND);
	find_part2();
	init_anirate(2);

	/* foot5 */
	do
	{
		process_sleep(1);
		next_anirate();
	}
	while(((current_proc->pa8)->oypos.u.intpos+100)>worldtly.u.intpos);

	delobjp(current_proc->pa8);

	process_suicide();
}

/******************************************************************************
 Function: void f_sektor(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	perform sektor friendship
******************************************************************************/
//extern void *a_robo1_friend[];
//extern void *a_dinger[];
void f_sektor(void)
{
	CREATE(PID_BANI,dinger_proc);
	//current_proc->pa9=a_robo1_friend;
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	do_next_a9_frame(current_proc->pa8);
	process_sleep(0x50);
	mframew_5;
	stack_jump(friendship_complete);
}

void dinger_proc(void)
{
	ochar_sound(0x21);

	current_proc->a10=(ADDRESS)current_proc->pa8;
	//current_proc->pa9=a_dinger;
	//gmo_proc(&friendship_anims);
	get_char_ani(ANIM_TABLE2,ANIM_F2_FRIEND);
	gmo_proc((current_proc->pa8)->oheap);
	lineup_a0_onto_a1(current_proc->pa8,(OBJECT *)current_proc->a10);
	insert_object(current_proc->pa8,&objlst);
	mframew_4;
	process_sleep(0x40);
	mframew_4;
	process_suicide();
}

/******************************************************************************
 Function: void f_cyrax(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	perform cyrax  friendship
******************************************************************************/
//extern void *a_robo2_friend[];
void f_cyrax(void)
{
	CREATE(PID_BANI,end_friend_proc);
	//current_proc->pa9=a_robo2_friend;
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	mframew_5;
	stack_jump(friendship_complete);
}

/******************************************************************************
 Function: void f_lao(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	perform lao friendship
******************************************************************************/
//extern void *a_lao_friend[];
//extern void *a_dog[];
//extern OIMGTBL *LAODOG1;
//extern OIMGTBL *FETCHHAT2;
void f_lao(void)
{
	CREATE(PID_BANI,end_friend_proc);
	CREATE(PID_BANI,cute_lil_doggy);
	CREATE(PID_BANI,lao_dog_sounds);

	//current_proc->pa9=a_lao_friend;
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	mframew_4;
	CREATE(PID_BANI,hat_proc);
	process_sleep(0x60);

	tsound(0x8b);
	mframew_4;
	process_sleep(0x30);

	mframew(8);
	stack_jump(friendship_complete);
}

void lao_dog_sounds(void)
{
	process_sleep(8);

	current_proc->a10=5;
	/* dog3 */
	do
	{
		tsound(0x8a);
		process_sleep(0x10);
	}
	while(--current_proc->a10>0 );
	process_suicide();
}

void cute_lil_doggy(void)
{
	OBJECT *obj;

	current_proc->a10=(ADDRESS)current_proc->pa8;
	get_char_ani(ANIM_TABLE2,ANIM_F2_FRIEND);						// get ptr to anim table
	current_proc->pa9=COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->pa9));	// get ptr to frame
	gso_dmawnz(obj,(ADDRESS)current_proc->pa9,(current_proc->pa8)->oheap,0);
	alloc_cache((OIMGTBL *)current_proc->pa9,(current_proc->pa8)->oheap,obj);

	if (((OBJECT *)current_proc->a10)->oflags & M_FLIPH)
	{
		obj->oxpos.u.intpos=SCRRGT+SCX(0x30)+SCX(0x30)+worldtlx.u.intpos-SCX(0x30);
		obj->oxvel.pos=-SCX(0x60000);
		flip_single(obj);
	}
	else
	{
		obj->oxpos.u.intpos=worldtlx.u.intpos-SCX(0x30);
		obj->oxvel.pos=SCX(0x60000);
	}

	/* cute3 */
	obj->oypos.u.intpos=ground_y-SCY(0x20);
	obj->ozval=100;
	insert_object(obj,&objlst);

	//current_proc->pa9=a_dog;
	get_char_ani(ANIM_TABLE2,ANIM_F2_FRIEND);
	init_anirate(3);
	do
	{
		next_anirate();
		process_sleep(1);

	} while (abs(worldtlx.u.intpos-obj->oxpos.u.intpos)<700);

	stop_a8(obj);

	wait_forever();
}

void hat_proc(void)
{
	OBJECT *obj;

	current_proc->a10=(ADDRESS)current_proc->pa8;
	//gso_dmawnz(obj,(ADDRESS)&FETCHHAT2,friendship_anims,0);
	//alloc_cache(&FETCHHAT2,&friendship_anims,obj);
	get_char_ani(ANIM_TABLE2,ANIM_F3_FRIEND);						// get ptr to anim table
	current_proc->pa9=COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->pa9));	// get ptr to frame
	gso_dmawnz(obj,(ADDRESS)current_proc->pa9,(current_proc->pa8)->oheap,0);
	alloc_cache((OIMGTBL *)current_proc->pa9,(current_proc->pa8)->oheap,obj);

	lineup_a0_onto_a1(obj,(OBJECT *)current_proc->a10);
	multi_adjust_xy(obj,SCX(0x50),SCY(0x10));
	insert_object(obj,&objlst);

	obj->oyvel.pos=-SCY(0x40000);
	set_proj_vel(obj,SCX(0xa0000),2);
	process_suicide();
}

/******************************************************************************
 Function: void f_kabal(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	perform kabal friendship
******************************************************************************/
//extern void *a_tusk_friend[];
void f_kabal(void)
{
	CREATE(PID_BANI,end_friend_proc);
	//current_proc->pa9=a_tusk_friend;
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	mframew_5;
	stack_jump(friendship_complete);
}

/******************************************************************************
 Function: void f_sheeva(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	perform sheeva friendship
******************************************************************************/
//extern void *a_sg_friend[];
void f_sheeva(void)
{
	CREATE(PID_BANI,end_friend_proc);
	//current_proc->pa9=a_sg_friend;
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	mframew_5;
	ochar_sound(6);
	mframew_5;
	stack_jump(friendship_complete);
}

/******************************************************************************
 Function: void f_shang(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	perform shang friendship
******************************************************************************/
//extern OIMGTBL *ostrich1;
void f_shang(void)
{
	OBJECT *obj=current_proc->pa8;
	void *animframe;

	CREATE(PID_BANI,end_friend_proc);
	tsound(0x8c);
	create_fx(FX_INVISO_POOF);

	process_sleep(8);

	set_inviso(current_proc->pa8);
	current_proc->a10=(ADDRESS)current_proc->pa8;

	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	animframe=(void*)COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->pa9));

	gso_dmawnz(obj,(ADDRESS)animframe,obj->oheap,0);
	alloc_cache((OIMGTBL *)animframe,obj->oheap,obj);

	obj->oxpos.u.intpos=((OBJECT*)current_proc->a10)->oxpos.u.intpos;
	obj->oypos.u.intpos=ground_y-SCY(0x20);
	insert_object(obj,&objlst);
	process_sleep(0x40);

	bounce();
	bounce();

	obj->oxvel.pos=SCX(0x30000);
	/* shang4 */
	current_proc->a10=5;
	do
	{
		bounce();
	}
	while(--current_proc->a10>0 );
	stop_a8(current_proc->pa8);
	wait_forever();
}

void bounce(void)
{
	long ta11;

	ta11=-randu_minimum(SCY(0x40000),SCY(0x40000));

	(current_proc->pa8)->oyvel.pos=ta11;
	process_sleep(2);

	/* bnc3 */
	do
	{
		process_sleep(1);
		(current_proc->pa8)->oyvel.pos=ta11;
		ta11+=SCY(0x8000);
	}
	while((current_proc->pa8)->oypos.u.intpos+SCY(0x20)<ground_y );
	tsound(0x71);

	return;
}

/******************************************************************************
 Function: void f_liu(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	perform liu kang friendship
******************************************************************************/
//extern void *a_kang_friend[];
//extern void *a_wall_dragon[];
void f_liu(void)
{
	CREATE(PID_BANI,wall_dragon_proc);
	//current_proc->pa9=a_kang_friend;
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	mframew_5;
	process_sleep(0x20);
	mframew_5;
	process_sleep(10);
	mframew_5;
	process_sleep(0x30);
	stack_jump(friendship_complete);
}

void wall_dragon_proc(void)
{
	current_proc->a10=(ADDRESS)current_proc->pa8;
	//current_proc->pa9=a_wall_dragon;
	get_char_ani(ANIM_TABLE2,ANIM_F2_FRIEND);
	gmo_proc((current_proc->pa8)->oheap);

	lineup_a0_onto_a1(current_proc->pa8,(OBJECT *)current_proc->a10);
	multi_adjust_xy(current_proc->pa8,0,-SCY(0x30));
	insert_object(current_proc->pa8,&objlst);
	mframew_5;
	process_sleep(0x20);
	mframew_4;
	process_suicide();
}

/******************************************************************************
 Function: void f_smoke_ninja(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	perform smoke friendship
******************************************************************************/
//extern void *a_smoke_friend[];
void f_smoke_ninja(void)
{
	CREATE(PID_BANI,end_friend_proc);
	robo_open_chest(current_proc->pa8);

	//current_proc->pa9=a_smoke_friend;
	get_char_ani(ANIM_TABLE2,ANIM_F1_FRIEND);
	mframew_5;

	ochar_sound(0x22);		// horn
	mframew_5;

	process_sleep(0x10);
	mframew_5;
	delete_slave();

	robo_close_chest(current_proc->pa8);

#if 1
	/* sony add for memory */
	(current_proc->pa8)->oheap=player_heap_ptr;		// make sure and restore at end of
#endif

	stack_jump(victory_animation);
}

