/******************************************************************************
 File: mkfatal.c

 Date: Jan 1995

 (C) Williams Entertainment

 Mortal Kombat III Fatality Routines
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
#include "mkcanned.h"
#include "mkutil.h"
#include "mkani.h"
#include "mkgame.h"
#include "mkfade.h"
#include "mkguys.h"
#include "mkfatal.h"
#include "mkzap.h"
#include "mkblood.h"
#include "mkreact.h"
#include "mkanimal.h"
#include "mkjoy.h"
#include "moves.h"
#include "mkpal.h"
#include "mksound.h"
#include "mkdrone.h"
#include "valloc.h"
#include "psxspu.h"
#include "mkfile.h"

extern void *fatal_anims[];
extern void *a_reached_kano[];
extern void *a_reached_sonya[];
extern void *a_reached_jax[];
extern void *a_reached_ind[];
extern void *a_reached_sz[];
extern void *a_reached_swat[];
extern void *a_reached_lia[];
extern void *a_reached_robo1[];
extern void *a_reached_robo2[];
extern void *a_reached_lao[];
extern void *a_reached_tusk[];
extern void *a_reached_sg[];
extern void *a_reached_st[];
extern void *a_reached_lk[];
extern void *a_reached_robo1[];

JUMPTBL ochar_fatalities1[]=
{
	kano_lazer,
	sonya_kiss,
	jax_slice,
	ind_light,
	sz_blow,
	swat_taser,
	lia_scream_rip,
	fat_robo_crush,
	cyrax_self_destruct,
	lao_tornado,
	kabal_inflator,
	sg_pound,
	st_spike,
	kang_fire,
	smoke_blowup_earth,

	kitana_decap,
	jade_shaker,
	mileena_nails,
				 
	scorpion_hell,
	reptile_tongue,
	ermac_super_slam,

	osz_head_rip,
	non_violent_finish,
	non_violent_finish,
};

JUMPTBL ochar_fatalities2[]=
{
	kano_skeleton,
	sonya_kiss_crusher,
	jax_grow,
	ind_zap_kill,
	sz_lift_n_freeze,
	sw_plant_bomb,
	lia_hair_spin,
	robo_flame_throw,
	cyrax_helecopter,
	lao_slicer,
	kabal_scare,
	sg_flesh_rip,
	st_suck,
	kang_mk_game,
	smoke_arm,

	kitana_kiss,
	jade_impale,
	mileena_suck_kiss,

	scorpion_fire,
	reptile_vomit,
	ermac_decap_attack,

	non_violent_finish,
	ermac_decap_attack,
	non_violent_finish,
};

static BYTE fataltype;						// fatality #1 or #2
/******************************************************************************
 Function: void do_fatality_1(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	perform fighter fatality #1
******************************************************************************/
void do_fatality_1(void)
{
	init_special(current_proc->pa8);
	fataltype=CHAR_FATAL1;
	ochar_fatalities1[(current_proc->pa8)->ochar]();
}

/******************************************************************************
 Function: void do_fatality_2(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	perform fighter fatality #1
******************************************************************************/
void do_fatality_2(void)
{
	init_special(current_proc->pa8);
	fataltype=CHAR_FATAL2;
	ochar_fatalities2[(current_proc->pa8)->ochar]();
}


//*******************************************************************************
//*******************************************************************************
void kitana_kiss(void)
{
	PROCESS *ta0;
	void *stk_a9;

	fatality_start_pause();

	sans_repell_for_good();
	animate2_a9(6,ANIM_MIL_KISS);
	tsound(0x60);	

	ta0=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	player_normpal();
	current_proc=ta0;

	stk_a9=current_proc->pa9;
	switch ((current_proc->pdata.p_otherguy)->ochar)
	{
		case FT_ROBO1:
		case FT_ROBO2:
		case FT_SMOKE:
			pose_him_a9(7,ANIM_DIZZY);	// DHS makesure we get first dizzy frame
			break;
		default:
			pose_him_a9(0,ANIM_DIZZY);
			break;
	}
	current_proc->pa9=stk_a9;

	takeover_him(wait_forever);
	process_sleep(0x40);

	stk_a9=current_proc->pa9;
	get_char_ani(ANIM_TABLE2,ANIM_STRETCH);
	takeover_him(r_stretch);
	(current_proc->pdata.p_otherproc)->a11=(ADDRESS)current_proc->pa9;
	current_proc->pa9=stk_a9;

	mframew(4);

	process_sleep(0x50);
	process_sleep(0x50);		// give enough time for explosiion to happen to avoid heap problems
	death_blow_complete();
	local_victory_animation;
}

void r_stretch(void) 
{
	OBJECT *obj=current_proc->pa8;

	obj->oheap=(current_proc->pdata.p_otherguy)->oheap;
	(ADDRESS)current_proc->pa9=current_proc->a11;		// arcade mod, data points staight to correct anim for particular char
	((ADDRESS*)current_proc->pa9)+=obj->ochar;		// ptr to character data
	(ADDRESS)current_proc->pa9=(ADDRESS)COMPUTE_ADDR((obj->oheap),GET_LONG(current_proc->pa9));           // get start of animation

	grow_n_shake();
	grow_n_shake();
	grow_n_shake();
	set_inviso(obj);
	create_fx(FX_SWAT_BEXP);
	white_flash();
	wait_forever();
}

void r_ex2on2(void) 
{
	set_inviso(current_proc->pa8);
	create_fx(FX_EX2ON2_BANG);
	set_nocol(current_proc->pa8);
	process_sleep(3);
	ground_player(current_proc->pa8);
	wait_forever();
}

void grow_n_shake(void) 
{
	tsound(0x24);
	tsound(0x25);
	death_scream;

	do_next_a9_frame(current_proc->pa8);
	shake_a8_up(current_proc->pa8,3,3,3,8);
	return;
}

//*******************************************************************************
//*******************************************************************************
void kitana_decap(void)
{
	fatality_start_pause();

	pose2_a9(0,ANIM_FAN_DECAP);
	sans_repell_for_good();

	process_sleep(0x10);

	animate_a0_frames(5,4);

	takeover_him(r_kitana_decap);

	do_next_a9_frame(current_proc->pa8);

	process_sleep(0x10);

	death_blow_complete();
	local_victory_animation;
}

//*******************************************************************************
//*******************************************************************************
void jade_impale(void)
{
	fatality_start_pause();
	(current_proc->pa8)->oheap=player_heap_ptr;		// this fatal is always in memory (dont need a special load)

	do_duck();
	process_sleep(8);

	rsnd_big_whoosh();
	animate_a9(2,ANIM_UPPERCUT);
	if (get_x_dist>SCX(0x50))
	{
		f_no_bonus=0;
		local_victory_animation;
	}

	takeover_him(r_impale_upcut);
	sans_repell_for_good();

	process_sleep(0x20);

	tsound(0x27);
	animate2_a9(4,ANIM_ROD_IMPALE);
	process_sleep(4);
	delete_slave();
	process_sleep(0x20);

	death_blow_complete();
	wait_forever();
}

void r_impale_upcut(void) 
{
	create_blood_proc(BLOOD_UPPERCUT);
	shake_a11(6,6);
	rsnd_big_smack();
	death_scream;
	current_proc->a6=(ADDRESS)impale_call;
	flight_call(current_proc->pa8,SCX(0x20000),-SCY(0x100000),0,5,-1);
	reaction_land();
}

short ochar_staff_lineups[]=
{
	-SCX(0x3a),
	-SCX(0x2a),
	-SCX(0x3a),
	-SCX(0x3a),
	-SCX(0x3a),

	-SCX(0x3a),
	-SCX(0x2a),
	-SCX(0x2a),
	-SCX(0x2a),
	-SCX(0x3a),

	-SCX(0x3a),
	-SCX(0x2a),
	-SCX(0x2a),
	-SCX(0x3a),
	-SCX(0x3a),

	-SCX(0x2a),
	-SCX(0x2a),
	-SCX(0x2a),
		
	-SCX(0x3a),
	-SCX(0x3a),
	-SCX(0x3a),
	-SCX(0x3a),
	-SCX(0x3a),
	-SCX(0x3a),
};

void impale_call(void) 
{
	OBJECT *obj=current_proc->pa8;

	if (distance_from_ground(obj)<SCY(0x1a0))
		return;

	stop_me(obj);
	process_sleep(0x40);

	a8_front_plus_1(obj);
	match_me_with_him();
	flip_multi(obj);

	multi_adjust_xy(obj,ochar_staff_lineups[obj->ochar],-SCY(0x110));
	find_ani_part2(ANIM_KDOWN);
	do_next_a9_frame(obj);

	obj->oyvel.pos=SCY(0x40000);
	obj->ograv.pos=SCY(0x20000);

	/* imfall */
	do
	{
		process_sleep(1);
	}
	while(distance_from_ground(obj)>SCY(0x130));
	rsnd_stab();
	death_scream;

	obj->ograv.pos=0;
	down_the_staff();
	down_the_staff();

	obj->oyvel.pos=0;
	create_blood_proc(BLOOD_PIT);

	stop_me(obj);
	wait_forever();
}

void down_the_staff(void) 
{
	(current_proc->pa8)->oyvel.pos>>=1;
	process_sleep(1);
	return;
}

//*******************************************************************************
//*******************************************************************************
void jade_shaker(void)
{
	void *stk_a9;
	PROCESS *pa0;

	fatality_start_pause();
	sans_repell_for_good();

	get_char_ani(ANIM_TABLE2,ANIM_ROD_SHAKER);
	mframew(4);

	set_his_noedge;

	takeover_him(r_jade_stab);
	get_his_a11_ani(ANIM_JADE_SHOOK);
	match_him_with_me_f();

	double_next_a9();

	shake_him_up(current_proc->pa8,3,0,3,5);

	his_group_sound(8);

	jade_shake_loop(6,4);
	jade_shake_loop(10,3);

	his_group_sound(8);
	
	jade_shake_loop(31,2);

	/* change him to skeleton */
	his_group_sound(8);

	stk_a9=current_proc->pa9;
	find_ani2_part_a14(ANIM_ROD_SHAKER,3);
	current_proc->a11=(ADDRESS)current_proc->pa9;
	current_proc->pa9=stk_a9;

	jade_shake_loop(30,2);

	his_group_sound(9);		// his death scream
	jade_shake_loop(70,1);

	pa0=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	skeleton_explode();
	current_proc=pa0;
	white_flash();


	find_ani2_part2(ANIM_ROD_SHAKER);
	animate_a0_frames(3,0x20);

	backwards_ani2(4,ANIM_ROD_SHAKER);
	death_blow_complete();
	local_victory_animation;
}

void skeleton_explode(void) 
{
	set_inviso(current_proc->pa8);
	multi_adjust_xy(current_proc->pa8,SCX(0x80),0);
	create_fx(FX_SWAT_BEXP);
	return;
}

void jade_shake_loop(WORD pa0,WORD pa1) 
{
	do
	{
		double_next_a9();
		process_sleep(pa1);
	}
	while (--pa0>0);

	return;
} 

void r_jade_stab(void) 
{
	rsnd_stab();
	death_scream;

	current_proc->a10=6;
	do
	{
		create_blood_proc(BLOOD_HAT);
		process_sleep(4);
	}
	while(--current_proc->a10>0);
	wait_forever();
} 

//*******************************************************************************
//*******************************************************************************
void mileena_suck_kiss(void)
{
	fatality_start_pause();

	sans_repell_for_good();

	animate2_a9(6,ANIM_MIL_KISS);
	tsound(0x60);

	takeover_him(r_kiss_suck);
	tsound(0x87);
	process_sleep(0x70);

	animate2_a9(4,ANIM_SUCK_PIT);
	CREATE(PID_BLOOD,bone_vomit_proc);
	process_sleep(0x70);

	death_blow_complete();
	local_victory_animation;
}


void bone_vomit_proc(void) 
{
	current_proc->a10=8;
	do
	{
		create_fx(FX_MILEENA_BONE);
		process_sleep(3);
		rsnd_stab();
		process_sleep(3);
		rsnd_stab();
	}
	while(--current_proc->a10>0);

	process_suicide();
}

void r_kiss_suck(void) 
{
	OBJECT *obj=current_proc->pa8;

	sans_repell_for_good();
	face_opponent(obj);
	shake_a8_up(obj,2,2,3,8);
	flip_multi(obj);
	multi_adjust_xy(obj,-SCX(0x36),0);

	(obj)->oheap=(current_proc->pdata.p_otherguy)->oheap;		// swithc to correct heap 
	get_char_ani(ANIM_TABLE2,ANIM_SUCKED);
	((ADDRESS*)current_proc->pa9)+=(obj)->ochar;		// ptr to character data
	(ADDRESS)current_proc->pa9=(ADDRESS)COMPUTE_ADDR((obj->oheap),GET_LONG(current_proc->pa9));           // get start of animation
	mframew(5);
	set_inviso(obj);
	wait_forever();
}

//*******************************************************************************
//*******************************************************************************
void mileena_nails(void)
{
	ADDRESS stk_a10;

	fatality_start_pause();
	get_char_ani(ANIM_TABLE2,ANIM_SWALLOW_NAILS);
	current_proc->a10=(ADDRESS)current_proc->pa9;
	animate_a0_frames(2,0x24);
	process_sleep(10);

	stk_a10=current_proc->a10;
	takeover_him(r_scared_of_mileena);
	current_proc->a10=stk_a10;

	(ADDRESS)current_proc->pa9=current_proc->a10;
	find_part2();
	mframew(4);

	CREATE(PID_BLOOD,nail_spawn_proc)->a11=(ADDRESS)current_proc->pdata.p_otherguy;

	animate_a0_frames(4,0x15);
	process_sleep(0x10);

	find_ani2_part2(ANIM_MIL_KISS);
	((ADDRESS*)current_proc->pa9)+=4;
	mframew(4);
	process_sleep(0x50);
	death_blow_complete();
	local_victory_animation;
}

void nail_spawn_proc(void) 
{
	current_proc->a10=30;
	do
	{
		CREATE(PID_BLOOD,nail_proc);
		process_sleep(1);
		CREATE(PID_BLOOD,nail_proc);
		process_sleep(1);
		rsnd_small_block();
	}
	while(--current_proc->a10>0);

	process_suicide();
}

void nail_proc(void) 
{
	OBJECT *obj;
	void *animframe;

	current_proc->a10=(ADDRESS)current_proc->pa8;
	find_ani2_part_a14(ANIM_SWALLOW_NAILS,4);

	animframe=(void*)COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->pa9));
	gso_dmawnz(obj,(ADDRESS)animframe,(current_proc->pa8)->oheap,0);
	alloc_cache((OIMGTBL *)animframe,(current_proc->pa8)->oheap,obj);
	lineup_1pwm(obj,(OBJECT *)current_proc->a10);
	multi_adjust_xy(obj,SCX(0x2a),-SCY(0x6));
	a8_front_plus_1(obj);
	insert_object(obj,&objlst);

	set_proj_vel(obj,SCX(0xc0000),-1);
	obj->oyvel.pos=(long)randu(SCY(0x50000));

	/* nail3 */
	do
	{
		process_sleep(1);
	}
	while(abs(((OBJECT*)current_proc->a11)->oxpos.u.intpos-obj->oxpos.u.intpos)>SCX(15));

	if (random() & 0x80000000)
		rsnd_stab();

	/* nail5 */
	stop_a8(obj);

	current_proc->a10=10;
	/* nail4 */
	do
	{
		process_sleep(1);
		obj->oxpos.u.intpos=((OBJECT*)current_proc->a11)->oxpos.u.intpos;
		multi_adjust_xy(obj,-SCX(0x10),0);
	}
	while(--current_proc->a10>0);
	delobjp(obj);
	process_suicide();
} 

void r_scared_of_mileena(void) 
{
	scared_pose();

	shake_a8_up(current_proc->pa8,7,7,3,8);

	/* hit by nails */
	CREATE(PID_BLOOD,nails_blood_spawner);
	shake_a11(6,0x10);

	shake_a8_up(current_proc->pa8,4,0,3,3);

	away_x_vel(current_proc,current_proc->pa8,SCX(0x40000));
	set_noedge(current_proc->pa8);
	kill_and_stop_scrolling();
	shake_a8_up(current_proc->pa8,0x4,0,3,20);
	stop_me(current_proc->pa8);
	wait_forever();
}

void nails_blood_spawner(void) 
{
	current_proc->a10=10;
	do
	{
		process_sleep(2);
		create_blood_proc(BLOOD_HAT);
	}
	while(--current_proc->a10>0);

	process_suicide();
}

//*******************************************************************************
//*******************************************************************************
void scorpion_hell(void)
{
	PROCESS *pa0;

	fatality_start_pause();
	(current_proc->pa8)->oheap=player_heap_ptr;		// no data to load

	center_around_me();
	animate_a9(5,ANIM_VICTORY);

	process_sleep(8);
	shake_a11(4,0x20);

	/* wsh3 */
	current_proc->a10=2;
	do
	{
		ochar_sound(0xf);
		process_sleep(0x30);
	}
	while(--current_proc->a10>0);

	(long)current_proc->a11=(is_he_right(current_proc)==SYSTEM_CARRY_SET)?-0x50:0x50;
	kill_and_stop_scrolling();

	tsound(0x0);
	tsound(0x1);
	white_flash();

	//-DHS- SHOULD BRING UP HELL BACKGROUND

	current_proc->a11=8;

	/* wsh5 */
	do
	{
		(long)current_proc->a10=(long)randu(SCRRGT/2-SCX(0x30));
		CREATE(PID_FX,another_scorpion)->pdata.p_store1=(ADDRESS)current_proc;

		current_proc->a10=SCRRGT-(long)current_proc->a10;
		CREATE(PID_FX,another_scorpion)->pdata.p_store1=(ADDRESS)current_proc;
	
		process_sleep(8);
	}
	while(--current_proc->a11>0);

	process_sleep(0x40);

	current_proc->a11=1;
	process_sleep(8);
	fast_fadeout_jsrp(8);

	kill_all_but_master();

	current_proc->a10=7;
	do
	{
		rsnd_big_smack();
		process_sleep(8);
	}
	while(--current_proc->a10>0);
	process_sleep(0x10);
	nosounds();
	process_sleep(3);

	rsnd_stab();
	process_sleep(10);
	rsnd_stab();
	process_sleep(10);
	rsnd_stab();
	process_sleep(10);
	tsound(0x24);

	process_sleep(0x10);

	pa0=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	death_scream;
	current_proc=pa0;

	process_sleep(0x20);

	object_initialize();
	kill_all_but_master();
	bogus_dlist;

	death_blow_complete();
	local_victory_animation;
}

void another_scorpion(void) 
{
	wait_forever();
}

//*******************************************************************************
//*******************************************************************************
void scorpion_fire(void)
{
	OBJECT *stk_a8;
	void *stk_a9;

	fatality_start_pause();

	ochar_sound(0xa);
	current_proc->a11=(ADDRESS)current_proc->pa8;
	scorpion_remove_mask();

	takeover_him(r_scared_of_scorp);
	process_sleep(0x30);

	get_char_ani(ANIM_TABLE2,ANIM_SCORP_FIRE);
	find_last_frame();
	do_next_a9_frame(current_proc->pa8);

	stk_a8=current_proc->pa8;
	stk_a9=current_proc->pa9;
	current_proc->pa8=(OBJECT *)current_proc->pdata.p_store2;
	((ADDRESS*)current_proc->pa9)+=3;
	frame_a9(current_proc->pa8);
	multi_adjust_xy(current_proc->pa8,SCX(0x12),SCY(0x1a));
	current_proc->pa8=stk_a8;
	current_proc->pa9=stk_a9;

	CREATE(PID_BLOOD,scorpion_flame);
	process_sleep(0x18);

	takeover_him(scorp_skeleton_burn);
	process_sleep(0x60);
	death_blow_complete();
	wait_forever();
}

void scorpion_flame(void) 
{
	void *animframe;
	OBJECT *obj;

	tsound(0x1f);

	current_proc->a11=(ADDRESS)current_proc->pa8;

	get_char_ani(ANIM_TABLE2,ANIM_SCORP_FLAME);
	animframe=(void*)COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->pa9));
	gso_dmawnz(obj,(ADDRESS)animframe,(current_proc->pa8)->oheap,0);
	alloc_cache((OIMGTBL *)animframe,(current_proc->pa8)->oheap,obj);
	current_proc->pdata.p_store3=(ADDRESS)obj;

	animframe=(void*)COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->pa9));
	gso_dmawnz(obj,(ADDRESS)animframe,(current_proc->pa8)->oheap,0);
	alloc_cache((OIMGTBL *)animframe,(current_proc->pa8)->oheap,obj);
	lineup_1pwm(obj,(OBJECT *)current_proc->a11);
	obj->ozval=100;
	multi_adjust_xy(obj,SCX(0xc),SCY(0xf));
	insert_object(obj,&objlst);

	framew(4);
	kludge_flame_ani(-SCX(0x10),-SCY(0x10));
	kludge_flame_ani(-SCX(0x10),-SCY(0x10));
	framew(4);

	shake_a11(8,0xd);

	current_proc->a10=(ADDRESS)obj;
	obj=current_proc->pa8=(OBJECT*)current_proc->pdata.p_store3;
	insert_object(obj,&objlst);
	double_flame_ani();

	((ADDRESS*)current_proc->pa9)--;
	obj->oxpos.u.intpos=((OBJECT*)current_proc->a10)->oxpos.u.intpos;
	obj->oypos.u.intpos=((OBJECT*)current_proc->a10)->oypos.u.intpos;
	obj->ozval=((OBJECT*)current_proc->a10)->ozval;
	obj->oflags=((OBJECT*)current_proc->a10)->oflags;
	flip_single(obj);
	multi_adjust_xy(obj,-SCX(0x2e),0);

	/* flame3 */
	do
	{
		double_flame_ani();
		process_sleep(4);
	}
	while(GET_LONG(current_proc->pa9)!=0);

	delobjp(obj);
	delobjp((OBJECT *)current_proc->a10);
	process_suicide();
}

void kludge_flame_ani(short pa0,short pa1) 
{
	multi_adjust_xy(current_proc->pa8,pa0,pa1);
	frame_a9(current_proc->pa8);
	process_sleep(4);
	return;
}

void double_flame_ani(void) 
{
	OBJECT *stk_a8;

	stk_a8=current_proc->pa8;
	frame_a9(current_proc->pa8);
	((ADDRESS*)current_proc->pa9)--;
	current_proc->pa8=(OBJECT*)current_proc->a10;
	frame_a9(current_proc->pa8);
	current_proc->pa8=stk_a8;
	return;
}
//*******************************************************************************
//*******************************************************************************
void reptile_tongue(void)
{
	fatality_start_pause();

	reptile_remove_mask();
	reptile_open_mouth();

	/* make tongue obj */
	grab_reptile_obj(ANIM_HI_TONGUE);
	(current_proc->pa8)->ozval=100;
	multi_adjust_xy(current_proc->pa8,-SCX(0x17),0);
	insert_object(current_proc->pa8,&objlst);
	current_proc->pdata.p_store3=(ADDRESS)current_proc->pa8;	
	framew(1);
	rsnd_big_block();

	takeover_him(r_tongue);
	process_sleep(6);
	tsound(0x25);

	current_proc->a0=(ADDRESS)current_proc->pdata.p_otherproc;
	current_proc->a10=(current_proc->pdata.p_otherproc)->pdata.p_store1;
	retract_tongue();
	process_sleep(8);

	grab_a_slice(ANIM_MED_TONGUE);
	grab_a_slice(ANIM_LO_TONGUE);

	process_sleep(0x20);
	death_blow_complete();
	wait_forever();
}

void grab_a_slice(WORD pa9) 
{
	void *stk_a9;

	stk_a9=current_proc->pa9;
	get_char_ani(ANIM_TABLE2,ANIM_REP_CHOMP_HEAD);
	current_proc->pa8=(OBJECT *)current_proc->pdata.p_store2;
	(ADDRESS)(current_proc->pa8)->oslink=(ADDRESS)current_proc->pa9;
	reptile_open_mouth();
	current_proc->pa9=stk_a9;

	/* stick out tongue */
	get_char_ani(ANIM_TABLE2,pa9);
	current_proc->pa8=(OBJECT *)current_proc->pdata.p_store3;
	(current_proc->pa8)->oypos.u.intpos=(long)((current_proc->pa8)->oslink);

	framew(1);
	rsnd_big_block();
	process_sleep(8);
	retract_tongue();
	return;
}

void reptile_open_mouth(void) 
{
	current_proc->pa8=(OBJECT *)current_proc->pdata.p_store2;
	current_proc->pa9=(void *)(current_proc->pa8)->oslink;
	framew(4);
	(void *)(current_proc->pa8)->oslink=current_proc->pa9;
	return;
}

void retract_tongue(void) 
{
	OBJECT *ta0;

	tsound(0x25);
	tongue_in_ani();
	current_proc->a11=5;
	do
	{
		tongue_in_ani();
	}
	while(--current_proc->a11>0);

	head_ani();
	tongue_in_ani();
	head_ani();
	tongue_in_ani();
	head_ani();
	tongue_in_ani();
	head_ani();
	tongue_in_ani();

	tsound(0x24);
	his_group_sound(9);		// his death scream


	current_proc->a0=current_proc->a10;
	current_proc->a10=(ADDRESS)(((OBJECT*)current_proc->a10)->oslink);
	delobjp((OBJECT *)current_proc->a0);

	ta0=(OBJECT*)current_proc->pdata.p_store3;
	(long)ta0->oslink=ta0->oypos.u.intpos;
	ta0->oypos.u.intpos=SCY(0x5000);

	current_proc->a11=7;
	do
	{
		head_ani_sleep();
	}
	while(--current_proc->a11>0);

	return;
}

void head_ani_sleep(void) 
{
	head_ani();
	process_sleep(2);
	return;
}

void head_ani(void) 
{
	OBJECT *stk_a8=current_proc->pa8;
	void *stk_a9=current_proc->pa9;

	current_proc->pa8=(OBJECT*)current_proc->pdata.p_store2;
	current_proc->pa9=(void *)((current_proc->pa8)->oslink);
	frame_a9(current_proc->pa8);
	(void *)((current_proc->pa8)->oslink)=current_proc->pa9;
	current_proc->pa8=stk_a8;
	current_proc->pa9=stk_a9;
	return;
}

void tongue_in_ani(void) 
{
	OBJECT *obj=current_proc->pa8;
	OBJECT *ta10=(OBJECT*)current_proc->a10;
	short ta0;

	frame_a9(obj);

	ta0=obj->oxpos.u.intpos;
	if ((obj->oflags & M_FLIPH)==0)
	ta0+=obj->osize.u.xpos;

	/* tia3 */
	ta10->oxpos.u.intpos=ta0-(ta10->osize.u.xpos>>1);

	ta10->oypos.u.intpos=obj->oypos.u.intpos+(obj->osize.u.ypos>>1)-(ta10->osize.u.ypos>>1);

	process_sleep(2);
	return;
}

void grab_reptile_obj(WORD pa9) 
{
	OBJECT *obj;
	void *animframe;
	
	animframe=get_char_ani(ANIM_TABLE2,pa9);
	animframe=(void*)COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(animframe));
	gso_dmawnz(obj,(ADDRESS)animframe,(current_proc->pa8)->oheap,0);
	alloc_cache((OIMGTBL *)animframe,(current_proc->pa8)->oheap,obj);
	lineup_1pwm(obj,(OBJECT *)current_proc->a11);

	return;
}

void r_tongue(void) 
{
	OBJECT *obj=current_proc->pa8;
	OBJECT *oa9;
	void *ta0;
	void *animframe;


	(short)current_proc->a11=obj->oypos.u.intpos;
	match_me_with_him();
	flip_multi(obj);
	multi_adjust_xy(obj,-SCX(0xc8),0);
	obj->oypos.u.intpos=(short)current_proc->a11;

	current_proc->a10=(ADDRESS)obj;
	set_inviso(obj);

	(current_proc->pa8)->oheap=(current_proc->pdata.p_otherguy)->oheap;		// get jax heap ptr (where cutup frames are)
	ta0=get_char_ani(ANIM_TABLE2,ANIM_OCHAR_CUTUP);		// get ptr to animation tables
	((ADDRESS*)ta0)+=(current_proc->pa8)->ochar;						// move to ptr of character
	ta0=COMPUTE_ADDR(((current_proc->pa8)->oheap),GET_LONG(ta0));		// get start of animation

	(OBJECT*)current_proc->pdata.p_store1=oa9=obj_free;

	current_proc->a11=10;
	/* rton3 */
	do
	{
		animframe=(void *)GET_LONG(ta0)++;
		if ( animframe==0 )
			break;

		animframe=(void*)COMPUTE_ADDR((current_proc->pa8)->oheap,animframe);
		gso_dmawnz(obj,(ADDRESS)animframe,(current_proc->pa8)->oheap,0);
		alloc_cache((OIMGTBL *)animframe,(current_proc->pa8)->oheap,obj);

		(ADDRESS)oa9->oslink=(ADDRESS)obj;
		oa9=obj;

		lineup_1pwm(obj,(OBJECT*)current_proc->a10);
		cutup_correct_pal();
		obj->ozval=current_proc->a11;
		insert_object(obj,&objlst);
		current_proc->a11--;
	} while (1);

	wait_forever();
}

void scorpion_remove_mask(void) 
{
	OBJECT *obj,*stk_a8;
	void *stk_a9;
	void *animframe;

	get_char_ani(ANIM_TABLE2,ANIM_SCORP_MASK_OFF);
	animate_a0_frames(5,2);

	stk_a8=current_proc->pa8;
	stk_a9=current_proc->pa9;

	find_ani2_part2(ANIM_SCORP_FIRE);

	animframe=(void*)COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->pa9));
	gso_dmawnz(obj,(ADDRESS)animframe,(current_proc->pa8)->oheap,0);
	alloc_cache((OIMGTBL *)animframe,(current_proc->pa8)->oheap,obj);
	lineup_1pwm(obj,(OBJECT *)current_proc->a11);
	obj->ozval=100;
	multi_adjust_xy(obj,-SCX(8),-SCY(0xc));
	insert_object(obj,&objlst);

	current_proc->pdata.p_store2=(ADDRESS)obj;
	obj->oslink=current_proc->pa9;

	current_proc->pa8=stk_a8;
	current_proc->pa9=stk_a9;
	animate_a0_frames(5,4);
	return;
}

void reptile_remove_mask(void) 
{
	OBJECT *stk_a8;
	void *stk_a9;

	get_char_ani(ANIM_TABLE2,ANIM_SCORP_MASK_OFF);
	animate_a0_frames(5,2);

	/* reptile head off */
	stk_a8=current_proc->pa8;
	stk_a9=current_proc->pa9;

	current_proc->pdata.p_store1=(ADDRESS)current_proc->pa8;
	current_proc->a11=(ADDRESS)current_proc->pa8;
	grab_reptile_obj(ANIM_REP_CHOMP_HEAD);
	(current_proc->pa8)->ozval=FRONT_Z+100;
	multi_adjust_xy(current_proc->pa8,-SCX(0x19),-SCY(2));
	insert_object(current_proc->pa8,&objlst);
	current_proc->pdata.p_store2=(ADDRESS)current_proc->pa8;
	(current_proc->pa8)->oslink=current_proc->pa9;

	current_proc->pa8=stk_a8;
	current_proc->pa9=stk_a9;

	animate_a0_frames(5,4);
	return;
}

//*******************************************************************************
//*******************************************************************************
void reptile_vomit(void)
{
	void *stk_a9;
	OBJECT *stk_a8;

	fatality_start_pause();

	stk_a8=current_proc->pa8;
	stk_a9=current_proc->pa9;

	current_proc->a11=(ADDRESS)current_proc->pa8;
	reptile_remove_mask();
	reptile_open_mouth();
	get_char_ani(ANIM_TABLE2,ANIM_VOMIT);
	
	gmo_proc(stk_a8->oheap);

	match_ani_points((OBJECT*)current_proc->a11,current_proc->pa8);
	multi_adjust_xy(current_proc->pa8,-SCX(0x18),-SCY(0x5));
	(current_proc->pa8)->ozval=100;
	insert_object(current_proc->pa8,&objlst);
	takeover_him(r_prevomit);

	mframew(5);

	takeover_him(r_vomit);

	mframew(5);

	current_proc->pa8=stk_a8;
	current_proc->pa9=stk_a9;

	process_sleep(0x30);
	death_blow_complete();
	local_victory_animation;
}

void r_prevomit(void) 
{
	short ta0;

	ta0=(current_proc->pa8)->oypos.u.intpos;
	match_me_with_him();
	multi_adjust_xy(current_proc->pa8,SCX(0x5c),0);
	face_opponent(current_proc->pa8);
	(current_proc->pa8)->oypos.u.intpos=ta0;

	his_ochar_sound(0x13);
	process_sleep(0xd);
	his_ochar_sound(0x10);
	wait_forever();
} 

void r_vomit(void) 
{
	set_inviso(current_proc->pa8);
	current_proc->a10=5;
	create_fx(FX_SMOKE_GUY)->a10=5;
	death_scream;
	wait_forever();
}

//*******************************************************************************
//*******************************************************************************
void ermac_super_slam(void)
{
	fatality_start_pause();

	animate2_a9(2,ANIM_SCORP_SUMMON);
	takeover_him(r_ermac_fatal_slam);
	process_sleep(0xa0);

	backwards_ani2(2,ANIM_SCORP_SUMMON);
	process_sleep(10);

	death_blow_complete();
	local_victory_animation;
}

//*******************************************************************************
//*******************************************************************************
void ermac_decap_attack(void)
{
	fatality_start_pause();

	do_duck();
	process_sleep(8);
	animate_a9(2,ANIM_UPPERCUT);
	takeover_him(r_ermac_upcut);
	process_sleep(0x40);

	death_blow_complete();
	local_victory_animation;
}

//*******************************************************************************
//*******************************************************************************
void osz_head_rip(void)
{
	PROCESS *pa0;

	fatality_start_pause();
	(current_proc->pa8)->oheap=player_heap_ptr;		// no data loaded

	get_char_ani(ANIM_TABLE1,ANIM_HIPUNCH);
	mframew(6);
	sans_repell_for_good();

	takeover_him(r_head_rip);

	process_sleep(0x20);
	fast_fadeout_jsrp(0x20);

	f_doscore=0;
	clr_scrn();

	tsound(0x24);
	process_sleep(0x10);

	pa0=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	death_scream;
	current_proc=pa0;

	process_sleep(0x40);
	tsound(0x25);

	tsound(0x7b);
	process_sleep(0x30);
	
	death_blow_complete();
	local_victory_animation;
}

short ochar_headrip_lineups[][2]=
{
	{SCX(0x47),SCY(0x00)},
	{SCX(0x43),SCY(0x06)},
	{SCX(0x30),SCY(0x0e)},
	{SCX(0x38),SCY(0x05)},
	{SCX(0x41),-SCY(0x01)},
	{SCX(0x47),SCY(0x5)},
	{SCX(0x61),SCY(0x11)},
	{SCX(0x43),SCY(0x5)},
	{SCX(0x43),SCY(0x5)},
	{SCX(0x31),SCY(0x0)},
	{SCX(0x44),SCY(0x8)},
	{SCX(0x22),SCY(0x5)},
	{SCX(0x51),SCY(0x6)},
	{SCX(0x3a),SCY(0x0)},
	{SCX(0x43),SCY(0x5)},
	{SCX(0x49),SCY(0x04)},
	{SCX(0x49),SCY(0x04)},
	{SCX(0x49),SCY(0x04)},
	{SCX(0x44),SCY(0x02)},
	{SCX(0x44),SCY(0x02)},
	{SCX(0x44),SCY(0x02)},
	{SCX(0x44),SCY(0x02)},
	{SCX(0x44),SCY(0x02)},
	{SCX(0x44),SCY(0x02)},
};

void r_head_rip(void) 
{
	match_me_with_him();

	get_char_ani(ANIM_TABLE1,ANIM_DIZZY);

	multi_adjust_xy(current_proc->pa8,ochar_headrip_lineups[(current_proc->pa8)->ochar][0],-ochar_headrip_lineups[(current_proc->pa8)->ochar][1]);
	face_opponent(current_proc->pa8);

	pose_a9(0,ANIM_SCARED);
	group_sound(2);

	shake_a8_up(current_proc->pa8,2,2,3,8);
	wait_forever();
}

//*******************************************************************************
// SMOKE FATALITY #1
//*******************************************************************************
//extern OIMGTBL SMBOMB1A;
void smoke_arm(void)
{
	OBJECT *obj;
	PROCESS *ta0;
	void *animframe;

	fatality_start_pause();

	ochar_sound(0);
	get_char_ani(ANIM_TABLE2,ANIM_ROBO_ARMBOMB);
	mframew(6);
	takeover_him(open_wide);
	ochar_sound(1);

	process_sleep(8);

	current_proc->a10=(ADDRESS)current_proc->pa8;
	current_proc->a0=(ADDRESS)current_proc->pa9;
	animframe=get_char_ani(ANIM_TABLE2,ANIM_ROBO_SBOMB);
	current_proc->pa9=(void *)current_proc->a0;
	animframe=(void*)COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(animframe));
	gso_dmawnz(obj,(ADDRESS)animframe,(current_proc->pa8)->oheap,0);
	alloc_cache((OIMGTBL *)animframe,(current_proc->pa8)->oheap,obj);
	lineup_1pwm(obj,(OBJECT *)current_proc->a10);
	multi_adjust_xy(obj,0,-SCY(0x10));
	insert_object(obj,&objlst);

	ta0=CREATE(PID_ROBO_BOMB,smoke_dropping);
	ta0->pdata.p_otherguy=current_proc->pdata.p_otherguy;
	ta0->pdata.p_otherproc=current_proc->pdata.p_otherproc;
	obj=current_proc->pa8=(OBJECT *)current_proc->a10;

	ochar_sound(2);

	mframew(6);
	delete_slave();

	process_sleep(0x30);
	death_blow_complete();
	local_victory_animation;
}

#if 0
void *ochar_reached[]=
{
	a_reached_kano,
	a_reached_sonya,
	a_reached_jax,
	a_reached_ind,
	a_reached_sz,

	a_reached_swat,
	a_reached_lia,
	a_reached_robo1,
	a_reached_robo2,
	a_reached_lao,

	a_reached_tusk,
	a_reached_sg,
	a_reached_st,
	a_reached_lk,
	a_reached_robo1
};
#endif

short ochar_wide_adjusts[]=
{
	SCX(0x70),
	SCX(0x70),
	SCX(0x80),
	SCX(0x80),
	SCX(0x70),

	SCX(0x70),
	SCX(0x70),
	SCX(0x80),
	SCX(0x80),
	SCX(0x60),

	SCX(0x60),
	SCX(0x5d),
	SCX(0x70),
	SCX(0x60),
	SCX(0x90),

	SCX(0x70),
	SCX(0x70),
	SCX(0x70),

	SCX(0x70),
	SCX(0x70),
	SCX(0x70),
	SCX(0x70),
	SCX(0x70),
	SCX(0x70),
};

void open_wide(void)
{
	player_normpal();
	face_opponent(current_proc->pa8);

	//current_proc->pa9=ochar_reached[(current_proc->pa8)->ochar];
	//(current_proc->pa8)->oheap=&fatal_anims;				// set to correct heap ptr
	(current_proc->pa8)->oheap=(current_proc->pdata.p_otherguy)->oheap;				// set to correct heap ptr
	get_char_ani(ANIM_TABLE2,ANIM_OCHAR_REACH);
	((ADDRESS*)current_proc->pa9)+=(current_proc->pa8)->ochar;
	current_proc->pa9=(void*)COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->pa9));

	((ADDRESS*)current_proc->pa9)+=3;
	do_next_a9_frame(current_proc->pa8);
	death_scream;

	match_me_with_him();
	flip_multi(current_proc->pa8);

	multi_adjust_xy(current_proc->pa8,-ochar_wide_adjusts[(current_proc->pa8)->ochar],0);
	wait_forever();
}

void smoke_dropping(void)
{
	OBJECT *obj=current_proc->pa8;

	obj->oyvel.pos=SCY(0x20000);
	/* drop4 */
	do
	{
		process_sleep(1);
		obj->oyvel.pos+=SCY(0x2000);

	}
	while(abs(ground_y-obj->oypos.u.intpos)>SCY(0x80));

	takeover_him(eat_this_shit);
	delobjp(obj);
	process_suicide();
}

void eat_this_shit(void)
{
	rsnd_big_block();
	shake_a8_up(current_proc->pa8,0x0,0x2,2,4);

	process_sleep(0x20);
	set_inviso(current_proc->pa8);
	create_fx(FX_SWAT_BEXP);
	white_flash();
	wait_forever();
}

//*******************************************************************************
// SMOKE FATALITY #2
//*******************************************************************************
extern OIMGTBL smstar;
extern void *misc_anims[];
//extern void *a_earth[];
void smoke_blowup_earth(void)
{
	WORD ta0;
	OBJECT *obj;
	OBJECT *ta8=current_proc->pa8;
	void *ta9=current_proc->pa9;
	ADDRESS ta11=current_proc->a11;
	void *heap;

	fatality_start_pause();
	robo_open_chest(ta8);
	heap=ta8->oheap;

	ta0=20;
	/* blow4 */
	do
	{
		get_char_ani(ANIM_TABLE2,ANIM_ROBO_BOMB);
		obj=get_proj_obj_m(current_proc->pa8);
		insert_object(obj,&objlst);

		a5_front_plus_1(obj);
		adjust_xy_a5(obj,0,SCY(0x28));
		current_proc->pa8=ta8;
		current_proc->pa9=ta9;
		current_proc->a11=ta11;

		CREATE(PID_BANI,earth_bomb);

		process_sleep(10);
	}
	while(--ta0>0);

	process_sleep(0x30);
	robo_close_chest(ta8);
	wfe_him;

	gstate=GS_PITFALL;

	fadeout_jsrp(30);
	
	kill_all_but_master();

	(PROCESS *)current_proc->a0=process_exist(PID_MASTER|0x8001,0xffff);
	((PROCESS *)current_proc->a0)->procid&=(~0x8000);
	bogus_dlist;

	current_proc->a10=70;
	/* starz */
	do
	{
		gso_dmawnz(obj,(ADDRESS)&smstar,misc_anims,0);
		alloc_cache(&smstar,&misc_anims,obj);
		obj->oxpos.u.intpos=randu(SCX(400));
		obj->oypos.u.intpos=randu(SCY(256));
		insert_object(obj,&objlst);
	}
	while(--current_proc->a10>0);

	//current_proc->pa9=a_earth;
	//gmo_proc(&fatal_anims);
	p1_obj=get_object();
	p2_obj=get_object();
	p1_obj->oheap=p2_obj->oheap=heap;
	(current_proc->pa8)->oheap=heap;
	get_char_ani(ANIM_TABLE2,ANIM_ROBO_EARTH);
	gmo_proc(heap);
	set_noscroll(current_proc->pa8);
	set_xy_coordinates(current_proc->pa8,SCX(0xc5),SCY(0x80));
	(current_proc->pa8)->ozval=100;
	insert_object(current_proc->pa8,&objlst);

	ta9=current_proc->pa9;
	fadein_jsrp(10);
	current_proc->pa9=ta9;

	tsound(0x87);
	shake_a8_up(current_proc->pa8,4,0,3,8);

	mframew(5);

	tsound(0);
	tsound(1);
	white_flash();
	white_flash();
	delobjp(current_proc->pa8);

	shake_a11(0x8,0x10);

	process_sleep(0x30);
	death_blow_complete();
	wait_forever();
}

void kill_all_but_master(void) 
{
	PROCESS *pa0;

	pa0=process_exist(PID_MASTER,0xffff);
	pa0->procid|=0x8000;
	murder_myoinit_score();
	pa0->procid=PID_MASTER;
	return;
}
void earth_bomb(void)
{
	current_proc->pa8=(OBJECT *)current_proc->a10;

	set_proj_vel(current_proc->pa8,SCX(0x8000)+randu(SCX(0x90000)),1);

	current_proc->pdata.p_ganiy=ground_y-SCY(0x15);

	init_anirate(3);
	get_char_ani(ANIM_TABLE2,ANIM_ROBO_BOMB);
	(current_proc->pa8)->oyvel.pos=0;
	fbomb_gravity(current_proc->pa8);

	fbomb_gravity2(current_proc->pa8,-SCY(0x50000));
	fbomb_gravity2(current_proc->pa8,-SCY(0x30000));

	stop_a8(current_proc->pa8);
	process_suicide();
}

void fbomb_gravity2(OBJECT *pa8,long pa0)
{
	pa8->oyvel.pos=pa0;
	process_sleep(1);
	bgrav3(pa8);
}

void bgrav3(OBJECT *pa8)
{
	do
	{
		process_sleep(1);
		next_anirate();
		--current_proc->a11;
		(current_proc->pa8)->oyvel.pos+=SCY(0x8000);
	}
	while((current_proc->pa8)->oyvel.pos<0 || (current_proc->pa8)->oypos.u.intpos<current_proc->pdata.p_ganiy);

	current_proc->a0=(current_proc->pdata.p_store2 ^ 1)?9:8;
	ochar_sound(current_proc->a0);

	(current_proc->pa8)->oxvel.pos>>=1;

	return;
}

//*******************************************************************************
// Liu Kang Fatality #1
//*******************************************************************************
//extern OIMGTBL MK1GAME;
void kang_mk_game(void)
{
	PROCESS *pa0;

	fatality_start_pause();

	ochar_sound(6);
	backwards_ani2(4,ANIM_LK_REFORM);
	set_inviso(current_proc->pa8);
	pa0=CREATE(PID_BLOOD,mk_game_cabinet);
	pa0->pdata.p_otherguy=current_proc->pdata.p_otherguy;
	pa0->pdata.p_otherproc=current_proc->pdata.p_otherproc;

	process_sleep(0x60);
	stack_jump(kang_reform);
}

void mk_game_cabinet(void)
{
	OBJECT *obj=current_proc->pa8;

	get_char_ani(ANIM_TABLE2,ANIM_LK_MK1GAME);
	gso_dmawnz(obj,(ADDRESS)current_proc->pa9,obj->oheap,0);
	alloc_cache((OIMGTBL *)current_proc->pa9,obj->oheap,obj);
	a8_front_plus_1(obj);

	center_about_x(obj,(current_proc->pdata.p_otherguy)->oxpos.u.intpos);

	obj->oypos.u.intpos=ground_y-SCY(0x1b0);
	insert_object(obj,&objlst);

	takeover_him(r_mk_game_crush);

	gravity_ani_ysize(SCY(0x20000),SCY(0x6000));
	obj->ochar=FT_LK;
	ochar_sound(8);
	shake_a11(0x8,0x8);
	obj->ozval=0;
	process_suicide();
}

void r_mk_game_crush(void)
{
	center_around_me();
	process_sleep(20);
	death_scream;
	stack_jump(r_jax_stomp);
}

//*******************************************************************************
// Liu Kang Fatality #2
//*******************************************************************************
void kang_fire(void)
{
	OBJECT *obj=current_proc->pa8;

	fatality_start_pause();

	current_proc->pdata.p_store4=obj->oxpos.u.intpos;
	sans_repell_for_good();
	wfe_him;

	ochar_sound(7);
	ochar_sound(6);
	backwards_ani2(4,ANIM_LK_REFORM);

	takeover_him(lk_skeleton_burn);

	match_me_with_him();
	get_char_ani(ANIM_TABLE2,ANIM_LK_FIRE);
	mframew(4);
	set_inviso(obj);
	process_sleep(0x60);

	ochar_sound(6);
	obj->oxpos.u.intpos=current_proc->pdata.p_store4;
	stack_jump(kang_reform);
}

void kang_reform(void)
{
	clear_inviso(current_proc->pa8);
	player_normpal();
	get_char_ani(ANIM_TABLE2,ANIM_LK_REFORM);
	mframew(4);

	death_blow_complete();

	local_victory_animation;
}

void lk_skeleton_burn(void)
{
	center_around_me();
	process_sleep(10);
	stack_jump(sb_skeleton_burn);
}

//*******************************************************************************
// Shang Tsung Fatality #1
//*******************************************************************************
void st_suck(void)
{
	fatality_start_pause();

	get_char_ani(ANIM_TABLE2,ANIM_ST_SPIKE);
	do_next_a9_frame(current_proc->pa8);		// need this to allow flip to work
	flip_multi(current_proc->pa8);
	mframew(4);

	takeover_him(soul_afloat);
	process_sleep(0x90);

	backwards_ani2(4,ANIM_ST_SPIKE);

	death_blow_complete();
	local_victory_animation;
}

extern void *ochar_floaters[];
void soul_afloat(void)
{
	OBJECT *obj=current_proc->pa8;
	PROCESS *ptemp;

//	obj->oheap=(current_proc->pdata.p_otherguy)->oheap;
//	//current_proc->pa9=ochar_floaters[obj->ochar];
//	get_char_ani(ANIM_TABLE2,ANIM_OCHAR_FLOAT);
//	((ADDRESS*)current_proc->pa9)+=(current_proc->pa8)->ochar;
//	do_next_a9_frame(obj);
	pose_a9(0,ANIM_DIZZY);			// UMK3

	obj->ograv.pos=-SCY(0x1000);
	process_sleep(45);

	obj->ograv.pos=0;

	/* soul3 */
	do
	{
		process_sleep(1);
		obj->oyvel.pos-=(obj->oyvel.pos>>3);
	}
	while(abs(obj->oyvel.pos)>SCY(0x800));

	stop_me(obj);
	obj->oheap=(current_proc->pdata.p_otherguy)->oheap;
	current_proc->pa9=(void *)get_his_char_ani(ANIM_TABLE2,ANIM_ST_SOUL);
	ptemp=CREATE(PID_FX,green_shit);
	ptemp->a0=(ADDRESS)(current_proc->pdata.p_otherguy)->oheap;			// sony psx addition
	process_sleep(5);
	his_ochar_sound(8);
	process_sleep(10);

	death_scream;
	current_proc->pa9=(void*)get_his_char_ani(ANIM_TABLE2,ANIM_ST_SOUL);
	find_part2();
	find_part2();
	do_next_a9_frame(obj);
	process_sleep(0x30);

	current_proc->pdata.p_ganiy=ground_y-SCY(0xb0);

	flight(obj,0,SCY(0x20000),SCY(0x8000),NEVER_ANI,-1);

	his_ochar_sound(9);
	mframew(4);
	wait_forever();
}

void green_shit(void)
{
	OBJECT *obj=current_proc->pa8;

	gmo_proc(obj->oheap);
	a8_front_plus_1(current_proc->pa8);
	insert_object(current_proc->pa8,&objlst);
	current_proc->a10=(ADDRESS)current_proc->pa8;
	current_proc->pa8=obj;

	lineup_a0_onto_a1((OBJECT *)current_proc->a10,obj);
	obj=current_proc->pa8=(OBJECT *)current_proc->a10;
	mframew(5);

	obj->oyvel.pos=-SCY(0x3000);
	process_sleep(0x40);

	delobjp(obj);
	process_suicide();
}

//*******************************************************************************
// Shang Tsung Fatality #2
//*******************************************************************************
void st_spike(void)
{
	void *ta9;

	fatality_start_pause();
	sans_repell_for_good();
	wfe_him;

	get_char_ani(ANIM_TABLE2,ANIM_ST_SPIKE);
	mframew(4);

	CREATE(PID_FX,appearing_spikes);
	process_sleep(0x10);

	mframew(5);
	process_sleep(0x10);

	ta9=current_proc->pa9;
	current_proc->pa9=(void*)get_his_char_ani(ANIM_TABLE1,ANIM_FB_ST);
	((ADDRESS*)current_proc->pa9)+=9*3;
	current_proc->a11=(ADDRESS)current_proc->pa9;
	current_proc->a10=(ADDRESS)current_proc->pdata.p_otherguy;
	current_proc->pa9=ta9;

	make_him_face_me();

	double_mframew(4);
	takeover_him(st_spiked);
	process_sleep(0x20);

	flip_multi(current_proc->pa8);
	backwards_ani(4,ANIM_DUCK);

	death_blow_complete();
	local_victory_animation;
}

void st_spiked(void)
{
	create_fx(FX_SPIKE_BLOOD);
	shake_a11(6,6);
	death_scream;
	rsnd_stab();
	ground_player(current_proc->pa8);
	find_ani_part2(ANIM_KDOWN);
	find_last_frame();
	do_next_a9_frame(current_proc->pa8);

	multi_adjust_xy(current_proc->pa8,0,-SCY(0x10));
	wait_forever();
}

//extern OIMGTBL SPIKE9;
void appearing_spikes(void)
{
	OBJECT *obj=current_proc->pa8;
	void *animframe;

	shake_a11(3,3);
	ochar_sound(7);
	current_proc->a11=(ADDRESS)current_proc->pa8;
	animframe=get_char_ani(ANIM_TABLE2,ANIM_ST_SPIKEBED);
	animframe=(void*)COMPUTE_ADDR(obj->oheap,GET_LONG(animframe));
	gso_dmawnz(obj,(ADDRESS)animframe,obj->oheap,0);
	alloc_cache((OIMGTBL *)animframe,obj->oheap,obj);

	//gso_dmawnz(obj,(ADDRESS)&SPIKE9,fatal_anims,0);
	//alloc_cache(&SPIKE9,&fatal_anims,obj);
	lineup_1pwm(current_proc->pa8,(OBJECT *)current_proc->a11);
	multi_adjust_xy(current_proc->pa8,-SCX(0xa0),SCY(0x50));
	insert_object(current_proc->pa8,&objlst);
	process_suicide();
}

//*******************************************************************************
// Sheeva Fatality #1
//*******************************************************************************
//extern void *ochar_pounded[];
void sg_pound(void)
{
	OBJECT *obj=current_proc->pa8;
	void *ta9;

	fatality_start_pause();
	sans_repell_for_good();

	get_char_ani(ANIM_TABLE2,ANIM_SG_POUND);

	mframew(3);
	wfe_him;

	ta9=current_proc->pa9;
	//current_proc->pa9=ochar_pounded[(current_proc->pdata.p_otherguy)->ochar];
	get_char_ani(ANIM_TABLE2,ANIM_OCHAR_POUND);							// pt to correct entry in ochar_pounded table
	((ADDRESS*)current_proc->pa9)+=(current_proc->pdata.p_otherguy)->ochar;
	current_proc->pa8=(current_proc->pdata.p_otherguy);
	(current_proc->pa8)->oheap=obj->oheap;			// switch other dudes heap
	(current_proc->pa8)->oflags2&=(~M_MULTIPART);
	current_proc->pa9=(void *)COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->pa9));
	animate(SINGLE_ANI,(OIMGTBL *)current_proc->pa9,(current_proc->pa8)->oflags);
	current_proc->pa9=ta9;
	current_proc->pa8=obj;

	lineup_1pwm(current_proc->pdata.p_otherguy,obj);
	adjust_him_xy(-SCX(30),0);
	chop_off_his_height(SCY(0x24));
	current_proc->pa9=ta9;

	mframew(3);
	chop_off_his_height(SCY(0x20));

	mframew(3);
	chop_off_his_height(SCY(0x20));

	mframew(3);
	death_blow_complete();
	local_victory_animation;
}

void chop_off_his_height(WORD pa6)
{
	OBJECT *pa0=current_proc->pdata.p_otherguy;
	PROCESS *ptemp;

	pa0->oypos.u.intpos+=pa6;
	pa0->osize.u.ypos-=pa6;
	pa0->header.t_height-=pa6;
	shake_a11(6,8);

	ochar_sound(5);
	his_group_sound(2);

	/* pounded_blood */
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	create_blood_proc(BLOOD_UPPERCUT);
	current_proc=ptemp;

	process_sleep(0x10);
	return;
}

//*******************************************************************************
// Sheeva Fatality #2
//*******************************************************************************
WORD flesh_lineups[]=
{
 	SCX(0x00),
 	SCX(0x10),
 	SCX(0x10),
 	SCX(0x10),
 	SCX(0x10),

 	SCX(0x00),
 	SCX(0x10),
 	SCX(0x10),
 	SCX(0x10),
 	SCX(0x10),

 	SCX(0x10),
 	SCX(0x10),
 	SCX(0x10),
 	SCX(0x10),
 	SCX(0x10),

 	SCX(0x10),
 	SCX(0x10),
 	SCX(0x10),

 	SCX(0x10),
 	SCX(0x10),
 	SCX(0x10),
 	SCX(0x10),
 	SCX(0x10),
 	SCX(0x10),

};

void sg_flesh_rip(void)
{
	fatality_start_pause();

	takeover_him(about_2b_ripped);
	sans_repell_for_good();

	get_char_ani(ANIM_TABLE2,ANIM_SG_RIP);
	mframew(4);

	adjust_him_xy(flesh_lineups[(current_proc->pdata.p_otherguy)->ochar],0);
	make_him_face_me();

	process_sleep(0x20);
	takeover_him(flesh_ripped_off);
	tsound(0x70);
	shake_a11(5,7);
	mframew(4);
	process_sleep(0x40);

	death_blow_complete();
	local_victory_animation;
}

void about_2b_ripped(void)
{
	face_opponent(current_proc->pa8);
	back_to_normal();
	pose_a9(0x0000,ANIM_DIZZY);
	do_next_a9_frame(current_proc->pa8);
	wait_forever();
}

void flesh_ripped_off(void)
{
	death_scream;
	face_opponent(current_proc->pa8);
	CREATE(PID_BLOOD,ripped_skeleton);

	//current_proc->pa9=ochar_reached[(current_proc->pa8)->ochar];
	//(current_proc->pa8)->oheap=&fatal_anims;
	(current_proc->pa8)->oheap=(current_proc->pdata.p_otherguy)->oheap;
	get_char_ani(ANIM_TABLE2,ANIM_OCHAR_REACH);
	((ADDRESS*)current_proc->pa9)+=(current_proc->pa8)->ochar;
	current_proc->pa9=(void*)COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->pa9));
	((ADDRESS*)current_proc->pa9)+=2;
	mframew(3);
	wait_forever();
}

extern void *ochar_ripped_anis[];

short ochar_skeleton_adj[][2]=
{
	{0,0},
	{0,SCY(8)},
	{SCX(0),SCY(0x010)},
	{0,0},
	{0,0},

	{0,0},
	{0,SCY(6)},
	{0,0},
	{0,0},
	{0,0},

	{0,0},
	{0,0},
	{SCX(8),-SCY(8)},
	{0,0},
	{0,0},
	
	{0,0},
	{0,0},
	{0,0},

	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0},
};

void ripped_skeleton(void)
{
	short ta0x,ta0y;
	OBJECT *obj=current_proc->pa8;

	//current_proc->a11=(ADDRESS)ochar_ripped_anis;
	current_proc->a11=(ADDRESS)get_char_ani(ANIM_TABLE2,ANIM_OCHAR_RIP);

	ta0x=ochar_skeleton_adj[(current_proc->pa8)->ochar][0];
	ta0y=ochar_skeleton_adj[(current_proc->pa8)->ochar][1];

	current_proc->a10=(ADDRESS)current_proc->pa8;
	current_proc->pa9=(void*)current_proc->a11;
	find_part2();
	if ((current_proc->pa8)->ochar==FT_SG)
		((ADDRESS*)current_proc->pa9)++;				// sheeva use other skeleton

	current_proc->pa9=(void*)COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->pa9));
	gmo_proc(obj->oheap);
	insert_object(current_proc->pa8,&objlst);
	(current_proc->pa8)->ochar=obj->ochar;
	lineup_a0_onto_a1(current_proc->pa8,(OBJECT *)current_proc->a10);

	multi_adjust_xy(current_proc->pa8,ta0x,ta0y);
	find_last_frame();
	do_next_a9_frame(current_proc->pa8);

	//if (((OBJECT *)current_proc->a10)->ochar==FT_SG)
		//wait_forever();

	skel_blood();
	skel_blood();
	skel_blood();
	skel_blood();

	wait_forever();
}

//*******************************************************************************
// Kabal Fatality #1
//*******************************************************************************
void kabal_scare(void)
{
	OBJECT *pa8=current_proc->pa8;
	OBJECT *obj;
	void *ta9;
	void *animframe;
	void *ta10;

	fatality_start_pause();

	get_char_ani(ANIM_TABLE2,ANIM_TUSK_UGLY);
	mframew(5);
	process_sleep(10);
	ta9=current_proc->pa9;

	current_proc->a10=(ADDRESS)current_proc->pa8;
	get_char_ani(ANIM_TABLE2,ANIM_TUSK_UGLY2);
	animframe=COMPUTE_ADDR(pa8->oheap,GET_LONG(current_proc->pa9));
	current_proc->a11=(ADDRESS)current_proc->pa9;
	gso_dmawnz(obj,(ADDRESS)animframe,pa8->oheap,0);
	alloc_cache((OIMGTBL*)animframe,pa8->oheap,obj);
	a8_front_plus_1(obj);
	lineup_1pwm(obj,pa8);
	insert_object(obj,&objlst);
	current_proc->a10=(ADDRESS)obj;
	current_proc->pa8=pa8;
	current_proc->pa9=ta9;

	face_and_head_ani();
	face_and_head_ani();
	face_and_head_ani();
	ochar_sound(9);
	face_ani(5);

	ta9=current_proc->pa9;
	((ADDRESS*)current_proc->pa9)-=2;
	do_next_a9_frame(current_proc->pa8);
	current_proc->pa9=ta9;
	face_ani(5);
	process_sleep(6);

	face_and_head_ani();
	face_and_head_ani();
	face_and_head_ani();

	pa8=current_proc->pa8;
	ta9=current_proc->pa9;
	ta10=(void*)current_proc->a10;
	animframe=(void*)current_proc->a11;
	takeover_him(sacred_2_death);
	current_proc->pa8=pa8;
	current_proc->pa9=ta9;
	(void*)current_proc->a10=ta10;
	(void*)current_proc->a11=animframe;
	ochar_sound(9);
	face_ani(3);

	face_and_head_ani();
	face_and_head_ani();
	face_and_head_ani();

	delobjp((OBJECT *)current_proc->a10);

	death_blow_complete();
	local_victory_animation;
}

void sacred_2_death(void)
{
	center_around_me();
	pose_a9(0,ANIM_SCARED);
	CREATE(PID_BANI,my_ghost)->pdata.p_otherguy=current_proc->pdata.p_otherguy;

	face_opponent(current_proc->pa8);
	death_scream;
	away_x_vel(current_proc,current_proc->pa8,SCX(0x10000));

	shake_a8_up(current_proc->pa8,0,4,3,12);
	stop_me(current_proc->pa8);
	process_sleep(10);
	stack_jump(collapse_on_ground);
}

void face_and_head_ani(void)
{
	next_head_ani();
	do_next_a9_frame(current_proc->pa8);
	process_sleep(4);
	return;
}

void face_ani(WORD pa0)
{
	OBJECT *ta8;
	void *ta9;

	ta8=current_proc->pa8;
	ta9=current_proc->pa9;
	current_proc->pa8=(OBJECT *)current_proc->a10;
	current_proc->pa9=(void *)current_proc->a11;
	framew(pa0);
	(OBJECT *)current_proc->a10=current_proc->pa8;
	(void *)current_proc->a11=current_proc->pa9;
	current_proc->pa8=ta8;
	current_proc->pa9=ta9;
	return;
}

void next_head_ani(void)
{
	OBJECT *ta8;
	void *ta9;

	ta8=current_proc->pa8;
	ta9=current_proc->pa9;
	current_proc->pa8=(OBJECT *)current_proc->a10;
	current_proc->pa9=(void *)current_proc->a11;
	frame_a9(current_proc->pa8);
	(OBJECT *)current_proc->a10=current_proc->pa8;
	(void *)current_proc->a11=current_proc->pa9;
	current_proc->pa8=ta8;
	current_proc->pa9=ta9;
	return;
}


extern WORD	KNSCAR_P[];
extern WORD	SBSCAR_P[];
extern WORD	JXSCAR_P[];
extern WORD	INSCAR_P[];
extern WORD	SZSCAR_P[];
extern WORD	OBSCAR_P[];
extern WORD	LIASCAR_P[];
extern WORD	SMSCAR_P[];
extern WORD	CYSCAR_P[];
extern WORD	KLSCAR_P[];
extern WORD	TRSCAR1_P[];
extern WORD	SHSCAR_P[];
extern WORD	TSSCAR_P[];
extern WORD	TSSCAR_P[];
extern WORD	SMSCAR_P[];

WORD *ochar_scared_pals[]=
{
	KNSCAR_P,
	SBSCAR_P,
	JXSCAR_P,
	INSCAR_P,
	SZSCAR_P,
	OBSCAR_P,
	LIASCAR_P,
	SMSCAR_P,
	CYSCAR_P,
	KLSCAR_P,
	TRSCAR1_P,
	SHSCAR_P,
	TSSCAR_P,
	TSSCAR_P,
	SMSCAR_P
};

void my_ghost(void)
{
	current_proc->a10=(ADDRESS)current_proc->pa8;
	//current_proc->pa9=ochar_stance1[(current_proc->pa8)->ochar];
	get_char_ani(ANIM_TABLE1,ANIM_STANCE);
	gmo_proc((current_proc->pa8)->oheap);

	match_ani_points((OBJECT *)current_proc->a10,current_proc->pa8);
	(current_proc->pa8)->ochar=((OBJECT*)current_proc->a10)->ochar;

	swpal(ochar_scared_pals[(current_proc->pa8)->ochar],current_proc->pa8);
	insert_object(current_proc->pa8,&objlst);

	pose_a9(0,ANIM_SCARED);
	set_proj_vel(current_proc->pa8,-SCX(0x20000),3);

	shake_a8_up(current_proc->pa8,0,4,3,12);
	stop_a8(current_proc->pa8);
	process_sleep(10);

	set_proj_vel(current_proc->pa8,SCX(0xa0000),3);
	get_char_ani(ANIM_TABLE1,ANIM_RUN);


	//mframew(3);
	init_anirate(3);
	do
	{
		next_anirate();
		process_sleep(1);
		if ( get_x_dist>700 )
			stop_a8(current_proc->pa8);
	} while (1);

	process_suicide();
}

//*******************************************************************************
// Kabal Fatality #2
//*******************************************************************************
void kabal_inflator(void)
{
	PROCESS *ptemp;

	fatality_start_pause();

	/* calla_for_him (center_around_me) */
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	center_around_me();
	current_proc=ptemp;

	get_char_ani(ANIM_TABLE2,ANIM_TUSK_TUBE);
	mframew(4);

	current_proc->a11=(ADDRESS)current_proc->pa9;

	//current_proc->pa9=ochar_floaters;
	get_char_ani(ANIM_TABLE2,ANIM_OCHAR_FLOAT);
	takeover_him(pumped);
	(current_proc->pdata.p_otherproc)->a11=(ADDRESS)current_proc->pa9;
	(current_proc->pdata.p_otherguy)->oheap=(current_proc->pa8)->oheap;

	current_proc->a10=3;
	/* kab3 */
	do
	{
		current_proc->pa9=(void *)current_proc->a11;
		ochar_sound(7);
		mframew(5);
	}
	while(--current_proc->a10>0);

	mframew(5);
	process_sleep(0x40);

	death_blow_complete();
	local_victory_animation;
}

WORD ochar_dizzy_offsets[]=
{
	0,
	5,
	2,
	0,
	1,

	0,	// 5
	8,	// 6
	0x17,	// 7
	0x17,	// 8
	0,	// 9

	6,
	0,
	0,
	8,
	0x17,	// 14 - smoke

	0,
	0,
	0,

	0,
	0,
	0,
	0,
	0,
	0,
};

#define ANIM_BIGHEAD 16
void pumped(void)
{
	OBJECT *ta8,*obj;
	void *animframe;

	//current_proc->pa9=(void *)current_proc->a11;
	//((ADDRESS*)current_proc->pa9)+=(current_proc->pa8)->ochar;
	//current_proc->a10=(ADDRESS)COMPUTE_ADDR(&fatal_anims,GET_LONG(current_proc->pa9));
	//((ADDRESS*)current_proc->a10)+=3;

	get_char_ani(ANIM_TABLE1,ANIM_DIZZY);
	((ADDRESS*)current_proc->pa9)+=ochar_dizzy_offsets[(current_proc->pa8)->ochar];
	do_next_a9_frame(current_proc->pa8);

	get_char_ani(ANIM_TABLE2,ANIM_BIGHEAD);
	((ADDRESS*)current_proc->pa9)+=(current_proc->pa8)->ochar;
	current_proc->a10=(ADDRESS)COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->pa9));

	get_char_ani(ANIM_TABLE2,ANIM_OCHAR_FLOAT);
	((ADDRESS*)current_proc->pa9)+=(current_proc->pa8)->ochar;
	do_next_a9_frame(current_proc->pa8);

	match_me_with_him();
	flip_multi(current_proc->pa8);
	multi_adjust_xy(current_proc->pa8,-SCX(0xd0),0);
	ground_player(current_proc->pa8);
	player_normpal();

	ta8=current_proc->pa8;
	current_proc->a11=(ADDRESS)ta8;
	//animframe=COMPUTE_ADDR(&fatal_anims,GET_LONG(current_proc->a10));
	//gso_dmawnz(obj,(ADDRESS)animframe,fatal_anims,0);
	//alloc_cache((OIMGTBL *)animframe,&fatal_anims,obj);
	animframe=COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->a10));
	((ADDRESS*)current_proc->a10)++;
	gso_dmawnz(obj,(ADDRESS)animframe,(current_proc->pa8)->oheap,0);
	alloc_cache((OIMGTBL *)animframe,(current_proc->pa8)->oheap,obj);
	lineup_1pwm(obj,(OBJECT *)current_proc->a11);
	a8_front_plus_1(obj);
	//-DHS- possible additions
	insert_object(obj,&objlst);

	current_proc->pa9=(void *)current_proc->a10;
	his_ochar_sound(8);			// pump sound

	framew(8);

	((ADDRESS*)current_proc->pa9)-=2;
	/* pmp4 */
	do
	{
		frame_a9(obj);
		process_sleep(4);
		((ADDRESS*)current_proc->pa9)-=2;
	}
	while((ADDRESS)current_proc->pa9!=current_proc->a10);

	current_proc->pa9=(void*)current_proc->a10;
	framew(4);

	shake_a8_up(obj,2,0,3,0x10);

	current_proc->a10=(ADDRESS)obj;
	current_proc->pa8=ta8;

	current_proc->a11=0x3e;
	/* pump4 */
	do
	{
		process_sleep(1);
		ta8->ograv.pos=-SCY(0x2000);
		((OBJECT *)current_proc->a10)->oyvel.pos=ta8->oyvel.pos;
	}
	while(--current_proc->a11>0);

	((OBJECT*)current_proc->a10)->oyvel.pos=0;
	stop_me(ta8);
	set_inviso(ta8);

	create_fx(FX_INFLATE_BOOM);
	wait_forever();
}

//*******************************************************************************
// Kung Lao Fatality #1
//*******************************************************************************
void lao_slicer(void)
{
	PROCESS *ptemp;
	OBJECT *obj;

	/* calla_for_him (center_around_me) */
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	center_around_me();
	current_proc=ptemp;

	fatality_start_pause();
	pose2_a9(0,ANIM_LAO_SLICE);
	process_sleep(0x10);

	ochar_sound(1);
	mframew(4);

	wfe_him;
	(current_proc->pdata.p_otherproc)->a11=0;	// flag: 1st time around

	obj=get_proj_obj_m(current_proc->pa8);
	adjust_xy_a5(obj,SCX(0x40),SCY(0x60));
	insert_object(obj,&objlst);
	create_proj_proc(slice_hat_proc);

	process_sleep(0x40*3);

	backwards_ani2(4,ANIM_LAO_SLICE);

	death_blow_complete();
	local_victory_animation;
}

void slice_hat_proc(void)
{
	slice_speed_set();
	sob3();

	slice_on_by(SCY(0x10));
	slice_on_by(SCY(0x30));
	slice_on_by(SCY(0x60));
	process_suicide();
}

void slice_speed_set(void)
{
	set_proj_vel(current_proc->pa8,SCX(0xd0000),NEVER_ANI);
	return;
}

void slice_on_by(WORD pa0)
{
	flip_multi(current_proc->pa8);
	slice_speed_set();
	(current_proc->pa8)->oypos.u.intpos=pa0+highest_mpart(current_proc->pdata.p_otherguy);
	sob3();
	return;
}

void sob3(void)
{
	PROCESS *ptemp;

	do
	{
		process_sleep(1);
	}
	while(abs((current_proc->pa8)->oxpos.u.intpos-(current_proc->pdata.p_otherguy)->oxpos.u.intpos)>SCX(10));

	sob4();

	/* calla_for_him (death_scream) */
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	death_scream;
	current_proc=ptemp;

	return;

}
	
void sob4(void)
{
	takeover_him(hat_sliced_xfer);
	(current_proc->pdata.p_otherproc)->pdata.p_store4=(current_proc->pa8)->oxvel.pos;		// pass vel

	ochar_sound(8);		// hat sliced sound

	/* fly_offscreen */
	do
	{
		process_sleep(1);
	}
	while(proj_onscreen_test(current_proc->pa8)==SYSTEM_CARRY_SET);
	stop_a8(current_proc->pa8);
	process_sleep(0x10);
	return;
}

void hat_sliced_xfer(void)
{
	OBJECT *oa9,*obj;
	void *animframe;
	void *ta0;

	if (current_proc->a11!=0)
	{
		stack_jump(lop_a_piece_off);
	}

	/* first_slice */
	current_proc->a11=10;
	current_proc->a10=(ADDRESS)current_proc->pa8;
	set_inviso(current_proc->pa8);

	oa9=obj_free;
	current_proc->pdata.p_store6=(ADDRESS)oa9;

//	ta0=COMPUTE_ADDR(&fatal_anims,ochar_cutups[(current_proc->pa8)->ochar]);
	(current_proc->pa8)->oheap=(current_proc->pdata.p_otherguy)->oheap;		// get jax heap ptr (where cutup frames are)
	ta0=get_char_ani(ANIM_TABLE2,ANIM_OCHAR_CUTUP);		// get ptr to animation tables
	((ADDRESS*)ta0)+=(current_proc->pa8)->ochar;						// move to ptr of character
	ta0=COMPUTE_ADDR(((current_proc->pa8)->oheap),GET_LONG(ta0));		// get start of animation
	/* hatsl3 */
	do
	{
		animframe=(void *)GET_LONG(ta0)++;
		if ( animframe==0 )
			break;

		animframe=(void*)COMPUTE_ADDR((current_proc->pa8)->oheap,animframe);
		gso_dmawnz(obj,(ADDRESS)animframe,(current_proc->pa8)->oheap,0);
		alloc_cache((OIMGTBL *)animframe,(current_proc->pa8)->oheap,obj);
		cutup_correct_pal();

		(OBJECT *)oa9->oslink=obj;
		oa9=obj;

		lineup_1pwm(obj,(OBJECT*)current_proc->a10);
		insert_object(obj,&objlst);
		obj->ozval=current_proc->a11;
		current_proc->a11--;
	} while (1);

	wait_forever();
}

void lop_a_piece_off(void)
{
	current_proc->pa8=(OBJECT *)current_proc->pdata.p_store6;
	CREATE(PID_BLOOD,lopped_piece)->a11=current_proc->pdata.p_store4;
	current_proc->pdata.p_store6=(ADDRESS)(current_proc->pa8)->oslink;
	wait_forever();
}

void lopped_piece(void)
{
	(long)current_proc->a11>>=1;
	(current_proc->pa8)->oxvel.pos=(long)current_proc->a11;
	gravity_ani_ysize(0,SCY(0x6000));

	(long)current_proc->a11>>=1;
	(current_proc->pa8)->oxvel.pos=(long)current_proc->a11;
	gravity_ani_ysize(-SCY(0x20000),SCY(0x6000));
	rsnd_stab();
	process_suicide();
}

/******************************************************************************
 Function: void gravity_ani_ysize(long pa1,long pa10)

 By: David Schwartz

 Date: May 1995

 Parameters: pa1 - yvel
						 pa10 - grav

 Returns: None

 Description:	gravity drop routine
******************************************************************************/
void gravity_ani_ysize(long pa1,long pa10)
{
	OBJECT *obj=current_proc->pa8;

	obj->oyvel.pos=pa1;

	/* gas2 */
	do
	{
		do
		{
			process_sleep(1);
			obj->oyvel.pos+=pa10;
		} while (obj->oyvel.pos<0);

	} while ((obj->oypos.u.intpos+obj->osize.u.ypos)<ground_y );

	stop_a8(obj);
	return;
}

//*******************************************************************************
// Kung Lao Fatality #2
//*******************************************************************************
void lao_tornado(void)
{
	fatality_start_pause();
	sans_repell_for_good();

	ochar_sound(6);
	normal_spin_intro();

	shake_a11(0x4,0x20);

	takeover_him(tornado_sucked);
	CREATE(PID_FX,nado_sounds);

	get_char_ani(ANIM_TABLE2,ANIM_LAO_TOP);
	animate_a0_frames(0x3,0x30);
	player_normpal();
	normal_spin_intro();

	ochar_sound_stop(6);		// dont let tornado sound go for ever
	ochar_sound_stop(7);

	death_blow_complete();
	local_victory_animation;
}

void tornado_sucked(void)
{
	player_normpal();
	me_in_back();
	(long)current_proc->a10=SCX(0x4000);

	/* sucked3 */
	do
	{
		process_sleep(1);
		((long)current_proc->a10)+=SCX(0x2000);
		towards_x_vel(current_proc,current_proc->pa8,(long)current_proc->a10);
	}
	while(get_x_dist>SCX(8));
	set_inviso(current_proc->pa8);
	stop_me(current_proc->pa8);
	face_opponent(current_proc->pa8);
	create_fx(FX_LAO_NADO);

	wait_forever();
}

void normal_spin_intro(void)
{
	get_char_ani(ANIM_TABLE2,ANIM_LAO_SPIN);
	animate_a0_frames(0x1,0x20);
	return;
}

void nado_sounds(void)
{
	while (1)
	{
		ochar_sound(6);
		ochar_sound(7);
		process_sleep(0x40);
	}
}

//*******************************************************************************
// Cyrax Fatality #1
//*******************************************************************************
void cyrax_helecopter(void)
{
	OBJECT *obj=current_proc->pa8;

	fatality_start_pause();

	sans_repell_for_good();

	pose2_a9(0,ANIM_ROBO_HELI);
	process_sleep(0x20);

	hele_sound();
	animate_a0_frames(0x2,0x10);
	animate_a0_frames(0x1,0x10);

	hele_sound();
	find_ani2_part2(ANIM_ROBO_HELI);
	animate_a0_frames(0x1,0x10);

	hele_sound();
	find_ani2_part2(ANIM_ROBO_HELI);
	find_part2();

	current_proc->a10=1;
	init_anirate(2);
	obj->ograv.pos=-SCY(0x2000);

	/* hele4 */
	do
	{
		hele_sleep();
	}
	while(distance_off_ground(current_proc,obj)<SCY(0x80));

	stop_me(obj);
	obj->oxpos.u.intpos=(current_proc->pdata.p_otherguy)->oxpos.u.intpos;
	flip_multi_v(obj);

	obj->oyvel.pos=SCY(0x40000);
	/* hele7 */
	do
	{
		hele_sleep();
	}
	while(distance_off_ground(current_proc,obj)>SCY(0));

	current_proc->a10=20;
	takeover_him(hele_cutup);
	obj->oyvel.pos=SCY(0x20000);				// slower

	current_proc->pdata.p_store5=1;

	/* hele8 */
	do
	{
		hele_sleep();

		if (--current_proc->pdata.p_store5==0)
		{
			create_fx(FX_CYRAX_CUTUP)->pa8=current_proc->pdata.p_otherguy; // fool blood routine
			current_proc->pdata.p_store5=22;			// blood explode
		}
	}
	while(distance_off_ground(current_proc,obj)>-SCY(0x80));

	/* helea */
	do
	{
		hele_sleep();
	}
	while(distance_off_ground(current_proc,obj)>-(SCY(0x12a)+56));


	/* kill heli sound, sony problem to fix */
	wess_seq_stop(FT_ROBO_WHIRL);						// kill all sounds for that group
	wess_seq_stop(FT_ROBO_QUAKE);						// kill all sounds for that group


	stop_a8(obj);
	back_to_normal();
	init_special(obj);
	set_inviso(obj);
	match_me_with_him();
	ground_player(obj);
	cyrax_implode();

	death_blow_complete();
	local_victory_animation;
}

//extern void*ochar_cutups[];
void hele_cutup(void)
{
	void *animframe;
	void *ta0;
	OBJECT *obj;
	WORD timer;

	shake_a11(0x6,0x20);

	back_to_normal();
	current_proc->a10=(ADDRESS)current_proc->pa8;		// me
	set_inviso(current_proc->pa8);

  	//ta0=COMPUTE_ADDR(&fatal_anims,ochar_cutups[(current_proc->pa8)->ochar]);
	(current_proc->pa8)->oheap=(current_proc->pdata.p_otherguy)->oheap;		// get jax heap ptr (where cutup frames are)
	ta0=get_char_ani(ANIM_TABLE2,ANIM_OCHAR_CUTUP);		// get ptr to animation tables
	((ADDRESS*)ta0)+=(current_proc->pa8)->ochar;						// move to ptr of character
	ta0=COMPUTE_ADDR(((current_proc->pa8)->oheap),GET_LONG(ta0));		// get start of animation
	current_proc->a6=10;
	current_proc->a11=0x10;
	/* helec2 */
	do
	{
		animframe=(void*)GET_LONG(ta0)++;
		if ( animframe==0 )
			break;

		animframe=(void*)COMPUTE_ADDR((current_proc->pa8)->oheap,animframe);
		gso_dmawnz(obj,(ADDRESS)animframe,(current_proc->pa8)->oheap,0);
		alloc_cache((OIMGTBL *)animframe,(current_proc->pa8)->oheap,obj);
		cutup_correct_pal();

		obj->ozval=current_proc->a6;
		current_proc->a6--;

		CREATE(PID_BLOOD,hele_slice)->a11=current_proc->a11;
		current_proc->a11+=10;
	} while (1);

	/* hcut3 */
	current_proc->pa8=(OBJECT *)current_proc->a10;
	death_scream;
	current_proc->a10=10;
	timer=1;
	/* hcut4 */
	do
	{
		process_sleep(8);

		if (current_proc->a10 & 1)
		{
			(current_proc->pa8)->oypos.u.intpos+=SCY(10);

			if ( --timer==0 )
			{
				create_fx(FX_CYRAX_COPTER);
				timer=2;
			}
		}

		rsnd_stab();
	}
	while(--current_proc->a10>0);

	wait_forever();
}

void hele_slice(void)
{
	lineup_1pwm(current_proc->pa8,(OBJECT *)current_proc->a10);
	insert_object(current_proc->pa8,&objlst);
	process_sleep(current_proc->a11);
	delobjp(current_proc->pa8);
	process_suicide();
}

void hele_sleep(void)
{
	process_sleep(1);
	if (--current_proc->a10==0)
	{
		current_proc->a10=0x20;
		hele_sound();
	}
	next_anirate();
	return;
}

void hele_sound(void)
{
	ochar_sound(0x1d);
	ochar_sound(0x1e);
	return;
}

//*******************************************************************************
// Cyrax Fatality #2
//*******************************************************************************
void cyrax_self_destruct(void)
{
	fatality_start_pause();

	ochar_sound(0x1b);

	get_char_ani(ANIM_TABLE2,ANIM_ROBO_DESTRUCT);
	mframew(6);
	current_proc->a11=(ADDRESS)current_proc->pa9;

	animate_a11(4);
	animate_a11(4);
	animate_a11(4);

	get_char_ani(ANIM_TABLE2,ANIM_ROBO_DESTRUCT);
	find_part2();
	find_part2();

	delete_slave();
	if ((current_proc->pa8)->oflags & M_FLIPH)
		flip_multi(current_proc->pa8);				// avoid reverse text

	/* selfd3 */
	mframew(6);
	current_proc->a11=(ADDRESS)current_proc->pa9;
	animate_a11(6);
	animate_a11(6);
	animate_a11(6);
	animate_a11(6);
	animate_a11(6);

	create_fx(FX_CYRAX_EXPLDE)->pdata.p_otherguy=current_proc->pdata.p_otherguy;
	(current_proc->pdata.p_otherguy)->oheap=(current_proc->pa8)->oheap;			// set correct heaps
	wfe_him;
	delete_slave();
	set_inviso(current_proc->pa8);
	set_inviso(current_proc->pdata.p_otherguy);

	process_sleep(0x80);

	death_blow_complete();
	wait_forever();
}

void animate_a11(WORD pa0)
{
	current_proc->pa9=(void *)current_proc->a11;
	mframew(pa0);
	return;
}

//*******************************************************************************
// Jax Fatality #1
//*******************************************************************************
//extern OIMGTBL JAXGROWFINA;
//extern OIMGTBL JAXGROWFINB;
//extern OIMGTBL JAXBIGBOOT;
void jax_grow(void)
{
	OBJECT *obj=current_proc->pa8;
	OBJECT *objhim;
	void *animframe;

	fatality_start_pause();

	//sans_repell_for_good();
	dallprc(PID_REPELL);

	takeover_him(grow_victum);

	(short)current_proc->pdata.p_store4=obj->oxpos.u.intpos;

	get_char_ani(ANIM_TABLE2,ANIM_JAX_GROW);
	animframe=(void *)COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->pa9));
	animframe=(void *)COMPUTE_ADDR(obj->oheap,GET_LONG(animframe));
	do_next_a9_frame(obj);

	ochar_sound(0xa);

	(ADDRESS)current_proc->pa9=0x01500150;
	/* jgrow3 */
	do
	{
		obj->oscale.yx=(ADDRESS)current_proc->pa9;

		obj->oxpos.u.intpos=(short)current_proc->pdata.p_store4-get_scaled_x_size(animframe);
		ground_scaled_jax(animframe);

		process_sleep(0x1);
		(ADDRESS)current_proc->pa9-=0x00040004;
	}
	while((ADDRESS)current_proc->pa9!=0x00500050);

	process_sleep(0x30);

	set_inviso(obj);

	current_proc->pa8=objhim=get_object();
	copy_obj(obj,objhim);
	objhim->oflags&=(~M_FIGHTER);
	objhim->oflags2&=(~M_MULTIPART);
	get_char_ani(ANIM_TABLE2,ANIM_JAX_BOOT);
	current_proc->pa9=(void*)COMPUTE_ADDR(objhim->oheap,GET_LONG(current_proc->pa9));
	animate(SINGLE_ANI,current_proc->pa9,objhim->oflags);
	insert_object(objhim,&objlst);
	objhim->oscale.yx=0x01000100;
	objhim->ozval=100;

	lineup_1pwm(objhim,current_proc->pdata.p_otherguy);

	multi_adjust_xy(objhim,-SCX(0x20),0);

	(long)current_proc->pa9=ground_y-objhim->osize.u.ypos;
	objhim->oypos.u.intpos=(long)current_proc->pa9-(SCRBOT-SCY(0x20)-32);

	takeover_him(r_jax_stomp);

	objhim->oyvel.pos=SCY(0x140000);			// down comes da boot
	/* stomp2 */
	do
	{
		process_sleep(1);
	}
	while(objhim->oypos.u.intpos<(long)current_proc->pa9);
	stop_a8(objhim);
	objhim->oypos.u.intpos=(long)current_proc->pa9;

	ochar_sound(8);
	ochar_sound(9);

	rsnd_ground();
	shake_a11(0xa,0xa);

	process_sleep(0x10);
	death_blow_complete();
	wait_forever();
}

void r_jax_stomp(void)
{
	get_char_ani(ANIM_TABLE1,ANIM_DUCK);
	mframew(3);
	set_inviso(current_proc->pa8);
	create_fx(FX_JAX_STOMP);
	wait_forever();
}

void grow_victum(void)
{
	short ta11,ta9;

	process_sleep(6);

	face_opponent(current_proc->pa8);
	ta11=(current_proc->pa8)->oxpos.u.intpos;
	multi_adjust_xy(current_proc->pa8,SCX(0xb0)+32,0);		// +16
	ta9=(current_proc->pa8)->oxpos.u.intpos;

	if (((current_proc->pa8)->oflags & M_FLIPH)==0)
		ta9-=SCRRGT;
	lineup_on_a9(ta9);

	(current_proc->pa8)->oxpos.u.intpos=ta11;

	process_sleep(0x40);
	death_scream;
	pose_a9(0,ANIM_SCARED);
	shake_a8_up(current_proc->pa8,4,0,3,9);
	wait_forever();
}

void ground_scaled_jax(OIMGTBL *pa5)
{
	long ta1,ta3;

	ta1=pa5->size.u.ypos<<8;
	ta3=(((LONG)current_proc->pa9) & 0xffff0000)>>16;
	ta1/=ta3;
	(current_proc->pa8)->oypos.u.intpos=ground_y-ta1+24;
	return;
}

short get_scaled_x_size(OIMGTBL *pa5)
{
	long ta11,ta3;

	ta11=pa5->size.u.xpos<<8;
	ta3=((LONG)current_proc->pa9) &0xffff;

	if ( (current_proc->pa8)->oflags & M_FLIPH )
		return(-(ta11/ta3));
	return(ta11/ta3);
}

//*******************************************************************************
// Jax Fatality #2
//*******************************************************************************
void jax_slice(void)
{
	fatality_start_pause();

	ochar_sound(7);

	animate2_a9(5,ANIM_JAX_MORPH);
	process_sleep(0x30);

	takeover_him(get_sliced_up);

	animate_a0_frames(0x3,0x20);
	get_char_ani(ANIM_TABLE2,ANIM_JAX_MORPH);
	find_last_frame();
	do_next_a9_frame(current_proc->pa8);
	process_sleep(0x10);

	takeover_him(post_sliced_up);

	find_ani2_part2(ANIM_JAX_MORPH);
	find_part2();
	mframew(5);

	death_blow_complete();
	local_victory_animation;
}

void get_sliced_up(void)
{
	death_scream;
	set_ignore_y(current_proc->pa8);

	shake_a11(0x4,0xe);

	current_proc->a10=current_proc->a11=1;
	find_ani_part2(ANIM_STUMBLE);
	init_anirate(3);
	/* slice1 */
	do
	{
		(long)current_proc->a0=-SCY(0x20000);

		/* slice2 */
		do
		{
			(current_proc->pa8)->oyvel.pos=(long)current_proc->a0;
			process_sleep(1);
			if (--current_proc->a11==0)
			{
				current_proc->a11=30;
				create_fx(FX_STAND_BLOOD);
			}

			/* slice3 */
			if (--current_proc->a10==0)
			{
				rsnd_stab();
				current_proc->a10=9;
			}

			/* slice4 */
			next_anirate();

			if (distance_off_ground(current_proc,current_proc->pa8)<SCY(0x40))
				break;

			current_proc->a0=0;
		}
		while(1);
	}
	while(1);

}

void post_sliced_up(void)
{
	OBJECT *obj;
	void *animframe;
	void *ta0;

	current_proc->a10=(ADDRESS)current_proc->pa8;
	set_inviso(current_proc->pa8);

	current_proc->a11=10;

//	ta0=COMPUTE_ADDR(&fatal_anims,ochar_cutups[(current_proc->pa8)->ochar]);
	(current_proc->pa8)->oheap=(current_proc->pdata.p_otherguy)->oheap;		// get jax heap ptr (where cutup frames are)
	ta0=get_char_ani(ANIM_TABLE2,ANIM_OCHAR_CUTUP);		// get ptr to animation tables
	((ADDRESS*)ta0)+=(current_proc->pa8)->ochar;						// move to ptr of character
	ta0=COMPUTE_ADDR(((current_proc->pa8)->oheap),GET_LONG(ta0));		// get start of animation

	/* posts3 */
	do
	{
		animframe=(void*)GET_LONG(ta0)++;
		if ( animframe==0 )
			wait_forever();

		animframe=(void*)COMPUTE_ADDR((current_proc->pa8)->oheap,animframe);
		gso_dmawnz(obj,(ADDRESS)animframe,(current_proc->pa8)->oheap,0);
		alloc_cache((OIMGTBL *)animframe,(current_proc->pa8)->oheap,obj);
		cutup_correct_pal();

		obj->ozval=current_proc->a11+FRONT_Z;
		current_proc->a11--;
		CREATE(PID_BLOOD,slice_fall);
	} while (1);
}

void slice_fall(void)
{
	lineup_1pwm(current_proc->pa8,(OBJECT *)current_proc->a10);
	insert_object(current_proc->pa8,&objlst);

	process_sleep(randu_minimum(0x20,0x20));

	current_proc->pdata.p_ganiy=ground_y;
	current_proc->a6=0;
	current_proc->pa9=0;
	gravity_ani(NEVER_ANI,0,SCY(0x6000));

	if ((random()&0x8000))
		(current_proc->pa8)->oxvel.pos=randu_minimum(SCX(0x30000),SCX(0x10000));
	else (current_proc->pa8)->oxvel.pos=-randu_minimum(SCX(0x30000),SCX(0x10000));

	current_proc->a6=0;
	current_proc->pa9=0;
	gravity_ani(NEVER_ANI,-SCY(0x40000),SCY(0x6000));
	rsnd_stab();
	stop_a8(current_proc->pa8);
	wait_forever();
}


extern WORD JX2CUT_P[];
extern WORD SMCUT_P[];
extern WORD KL2CUT_P[];
extern WORD CYCUT_P[];
extern WORD RBCUT_P[];
extern WORD JDCUT_P[];
extern WORD MLCUT_P[];
extern WORD SCRCUT_P[];
extern WORD REPCUT_P[];
extern WORD SUBCUT_P[];
extern WORD OSMCUT_P[];

typedef struct 
{
	short pchar;
	WORD *pal;
} SCCP;

SCCP special_case_cut_pals[]=
{
	{FT_SMOKE,SMCUT_P},
	{FT_ROBO2,CYCUT_P},
	{FT_ROBO1,RBCUT_P},

	{FT_JADE,JDCUT_P},
	{FT_MILEENA,MLCUT_P},

	{FT_SCORPION,SCRCUT_P},
	{FT_REPTILE,REPCUT_P},
	{FT_OLDSZ,SUBCUT_P},
	{FT_OLDSMOKE,OSMCUT_P},
	{FT_NOOB,noob_p},
	{-1,NULL},
};

void ft_pal_switch(short pa0,SCCP *pa1) 
{
	while (pa1->pchar!=-1)
	{
		if (pa0==pa1->pchar) 
		{
			swpal(pa1->pal,current_proc->pa8);
			break;
		}

		pa1++;
	}

	return;
}

/* current_proc->a10 = orig guy */
void cutup_correct_pal(void)
//-DHS- correct if needed
{
	OBJECT *obj=(OBJECT *)current_proc->a10;

	switch (obj->ochar)
	{
		case FT_JAX:
			if ((current_proc->pdata.p_otherguy)->ochar!=FT_JAX)
				return;
			if (obj->oid==PID_P1)
				return;
				swpal(JX2CUT_P,current_proc->pa8);
			return;
			break;
		case FT_LAO:
			if ((current_proc->pdata.p_otherguy)->ochar!=FT_LAO)
				return;
			if (obj->oid==PID_P1)
				return;
			swpal(KL2CUT_P,current_proc->pa8);
			return;
			break;
	}

	ft_pal_switch(obj->ochar,special_case_cut_pals);

	return;
}

//*******************************************************************************
// Sektor Fatality #1
//*******************************************************************************
void robo_flame_throw(void)
{
	fatality_start_pause();

	animate2_a9(5,ANIM_ROBO_FLAME);

	takeover_him(robo_skeleton_burn);
	process_sleep(1);
	delete_slave();
	process_sleep(0x20);

	find_ani2_part2(ANIM_ROBO_FLAME);
	mframew(5);

	process_sleep(0x80);

	death_blow_complete();
	local_victory_animation;
}

//*******************************************************************************
// Lia Fatality #1
//*******************************************************************************
void lia_hair_spin(void)
{
	fatality_start_pause();

	get_char_ani(ANIM_TABLE2,ANIM_LIA_HAIR);

	takeover_him(slide_behind_hair);

	mframew(4);
	process_sleep(0x30);

	takeover_him(hair_spin);

	mframew(3);
	process_sleep(0x40);

	death_blow_complete();

	local_victory_animation;
}

void slide_behind_hair(void)
{
	away_x_vel(current_proc,current_proc->pa8,SCX(0x40000));
	/* sbh3 */
	do
	{
		process_sleep(1);
	}
	while(get_x_dist<SCX(0x98));

	stop_me(current_proc->pa8);
	wait_forever();
}

void hair_spin(void)
{
	OBJECT *obj=current_proc->pa8;

	death_scream;
	center_around_me();
	his_ochar_sound(7);
	his_ochar_sound(8);
	shake_a11(0x5,0x20);
	create_fx(FX_LIA_SPIN);

	obj->oheap=(current_proc->pdata.p_otherguy)->oheap;			// switch to lias heap to get correct anims
	get_char_ani(ANIM_TABLE2,ANIM_LIA_TOP);
	do_next_a9_frame(obj);
	current_proc->a11=(ADDRESS)current_proc->pa9;

	current_proc->a10=0x60/2;
	/* rtbl5 */
	do
	{
		process_sleep(2);
		do_next_a9_frame(obj);
	}
	while(--current_proc->a10>0);

	current_proc->pa9=(void*)current_proc->a11;
	find_part2();
	mframew(4);
	set_inviso(obj);
	wait_forever();
}

//*******************************************************************************
// Lia Fatality #2
//*******************************************************************************
void lia_scream_rip(void)
{
	fatality_start_pause();

	//current_proc->pa9=ochar_ripped_anis;
	get_char_ani(ANIM_TABLE2,ANIM_OCHAR_RIP);
	current_proc->a11=(ADDRESS)current_proc->pa9;		// save  ochar table
	animate2_a9(4,ANIM_LIA_SCREAM);

	ochar_sound(8);
	ochar_sound(1);

	takeover_him(r_scream_ripped);
	(current_proc->pdata.p_otherproc)->a11=current_proc->a11;
	animate_a0_frames(0x5,0xc);

	process_sleep(0x40);

	backwards_ani2(5,ANIM_LIA_SCREAM);
	death_blow_complete();
	local_victory_animation;
}

void flesh_rip_sound(void)
{
	tsound(0x70);
	process_sleep(0x40);
	tsound(0x70);
	process_suicide();
}

void r_scream_ripped(void)
{
	OBJECT *obj=current_proc->pa8;

	CREATE(PID_BLOOD,flesh_rip_sound);
	shake_a11(0x6,0x30);

	face_opponent(obj);

	obj->oheap=(current_proc->pdata.p_otherguy)->oheap;		// switch to other heap where fatal data located
	CREATE(PID_BLOOD,remaining_skel);

	//current_proc->a11=(ADDRESS)ochar_ripped_anis;
	//obj->oheap=&fatal_anims;
	//get_char_ani(ANIM_TABLE2,ANIM_OCHAR_RIP);
	if (obj->ochar==FT_LIA && obj->oid==OID_P2)
		(ADDRESS*)current_proc->pa9=((ADDRESS *)current_proc->a11+24);
	else (ADDRESS*)current_proc->pa9=((ADDRESS *)current_proc->a11+obj->ochar);

	current_proc->pa9=COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->pa9));

	rip_ani();

	initial_skeleton_shake();

	away_x_vel(current_proc,obj,SCX(0xa000));
	do_next_a9_frame(obj);
	process_sleep(5);
	do_next_a9_frame(obj);
	process_sleep(5);
	rip_ani();
	do_next_a9_frame(obj);
	process_sleep(5);
	do_next_a9_frame(obj);
	process_sleep(5);
	do_next_a9_frame(obj);
	process_sleep(5);

	stop_me(obj);
	set_inviso(obj);
	wait_forever();
}

void rip_ani(void)
{
	void *animframe;

	animframe=COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(current_proc->pa9));	// ptr to multi part info of frame
	animframe=COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(animframe));					// ptr to actual frame
	((OIMGTBL *)animframe)++;			// check for new palette													// ptr to pal data if any
	animframe=(void*)GET_LONG(animframe);		// pallete ptr if any
	if (((ADDRESS)animframe) & 0x80000000)			// SONY PSX code test position
		player_swpal(current_proc,animframe,current_proc->pa8);
	do_next_a9_frame(current_proc->pa8);
	return;
}

void initial_skeleton_shake(void)
{
	away_x_vel(current_proc,current_proc->pa8,SCX(0x8000));
	shake_a8_up(current_proc->pa8,3,0,3,6);
	return;
}

void remaining_skel(void)
{
	OBJECT *oa10=current_proc->pa8;

	current_proc->pa9=(void*)current_proc->a11;
	//oa10->oheap=&fatal_anims;
	find_part2();

	if (oa10->ochar==FT_SG)
		((ADDRESS*)current_proc->pa9)++;			// sheeva use her skelton

	/* rsr1 */
	//current_proc->pa9=COMPUTE_ADDR(&fatal_anims,GET_LONG(current_proc->pa9));
	//gmo_proc(&fatal_anims);
	current_proc->pa9=COMPUTE_ADDR(oa10->oheap,GET_LONG(current_proc->pa9));
	gmo_proc(oa10->oheap);
	insert_object(current_proc->pa8,&objlst);
	lineup_a0_onto_a1(current_proc->pa8,oa10);
	(current_proc->pa8)->ochar=oa10->ochar;		// need to know what character blood type

	multi_adjust_xy(current_proc->pa8,ochar_skeleton_adj[oa10->ochar][0],ochar_skeleton_adj[oa10->ochar][1]);
	initial_skeleton_shake();
	mframew(8);

	skel_blood();
	skel_blood();
	skel_blood();
	skel_blood();

	wait_forever();
}

void skel_blood(void)
{
	current_proc->a11=0x0010fff0;
	skinny_spawn;
	current_proc->a11=0x0020fffc;
	skinny_spawn;
	current_proc->a11=0x0030fff8;
	skinny_spawn;
	process_sleep(0x10);
	current_proc->a11=0x0030fff2;
	skinny_spawn;
	current_proc->a11=0x0040fff0;
	skinny_spawn;
	process_sleep(0x10);
	current_proc->a11=0x0040fff8;
	skinny_spawn;
	current_proc->a11=0x0038fff8;
	skinny_spawn;
	process_sleep(0x10);
	return;
}

//*******************************************************************************
// Swat Fatality #1
//*******************************************************************************
void sw_plant_bomb(void)
{
	PROCESS *ptemp;
	OBJECT *obj=current_proc->pa8;

	fatality_start_pause();

	sans_repell_for_good();

	/* calla_for_him (player_normpal) */
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	player_normpal();
	current_proc=ptemp;

	wfe_him;
	obj->ozval=100;
	animate2_a9(6,ANIM_SWAT_BOMBER);

	get_char_ani(ANIM_TABLE2,ANIM_SWAT_BOMBER);
	find_part_a14(3);
	attach_obj_to_him();
	((OBJECT*)current_proc->a10)->oypos.u.intpos+=SCY(0x40)-16;		// on chest

	obj->oxpos.pos=(current_proc->pdata.p_otherguy)->oxpos.pos-6;

	get_my_dfe();
	if (current_proc->a6>current_proc->a5)
		set_x_vel(obj,SCX(0x60000));
	else set_x_vel(obj,-SCX(0x60000));

	current_proc->a11=4;
	flight(obj,DONT_TOUCH,-SCY(0xa0000),ANGLE_GRAV,3,ANIM_FFLIP);

	face_opponent(obj);
	tsound(0x18);
	get_char_ani(ANIM_TABLE1,ANIM_FFLIP);
	do_next_a9_frame(obj);
	process_sleep(3);

	find_ani2_part2(ANIM_SWAT_BOMBER);
	mframew(6);					// plug ears
	process_sleep(0x20);

	get_char_ani(ANIM_TABLE2,ANIM_SWAT_BOMBER);
	find_part_a14(4);

	current_proc->a11=(ADDRESS)current_proc->pa8;
	current_proc->pa8=(OBJECT*)current_proc->a10;
	do_next_a9_frame(current_proc->pa8);		// BANG
	process_sleep(2);
	delobjp(current_proc->pa8);			// rid of bomb/Bang

	current_proc->pa8=current_proc->pdata.p_otherguy;
	set_inviso(current_proc->pa8);
	ptemp=create_fx(FX_SWAT_BEXP);
	ptemp->pdata.p_otherguy=obj;
	ptemp->pdata.p_otherproc=current_proc;
	current_proc->pa8=obj;
	white_flash();

	process_sleep(0x40);

	death_blow_complete();
	local_victory_animation;
}

void attach_obj_to_him(void)
{
	OBJECT *ta8=current_proc->pa8;

	gmo_proc(ta8->oheap);
	lineup_a0_onto_a1(current_proc->pa8,current_proc->pdata.p_otherguy);
	a8_front_plus_1(current_proc->pa8);
	insert_object(current_proc->pa8,&objlst);
	current_proc->a10=(ADDRESS)current_proc->pa8;
	current_proc->pa8=ta8;

	return;
}

//*******************************************************************************
// Swat Fatality #2
//*******************************************************************************
void swat_taser(void)
{
	fatality_start_pause();

	get_char_ani(ANIM_TABLE2,ANIM_SWAT_TASER);
	mframew(5);					// put out gun
	process_sleep(0x10);

	ochar_sound(7);
	mframew(5);					// fire

	takeover_him(r_taser);

	current_proc->a10=18;
	/* taz3 */
	do
	{
		ochar_sound(6);
		find_ani2_part2(ANIM_SWAT_TASER);
		find_part2();
		mframew(4);
	}
	while(--current_proc->a10>0);

	death_blow_complete();
	local_victory_animation;
}


short taser_lineups[]=
{
	-SCX(0x110),
	-SCX(0x110),
	-SCX(0xf8),
	-SCX(0x108),
	-SCX(0x120),

	-SCX(0x120),
	-SCX(0x120),
	-SCX(0x108),
	-SCX(0x108),
	-SCX(0x100),

	-SCX(0x100),
	-SCX(0xf0),
	-SCX(0x120),
	-SCX(0x100),
	-SCX(0x108),

	-SCX(0x110),
	-SCX(0x110),
	-SCX(0x110),
	-SCX(0x110),
	-SCX(0x110),
	-SCX(0x110),

	-SCX(0x110),
	-SCX(0x110),
	-SCX(0x110),
	-SCX(0x110),
	-SCX(0x110),
	-SCX(0x110),
};

void r_taser(void)
{
	fatal_offset(current_proc->pa8,taser_lineups[(current_proc->pa8)->ochar],0);
	shake_a11(0xa,0x10);

	tsound(0x1e);
	death_scream;
	shocker_shaker(9);
	stack_jump(collapse_on_ground);
}

//*******************************************************************************
// Subzero Fatality #1
//*******************************************************************************
void sz_lift_n_freeze(void)
{
	PROCESS *ptemp;
	OBJECT *obj;
	void *ta9;

	fatality_start_pause();

	get_char_ani(ANIM_TABLE2,ANIM_SZ_LIFT);
	animate_a0_frames(8,1);				// reach out
	wfe_him;
	make_him_face_me();

	get_his_a11_ani(ANIM_FB_SG);
	double_next_a9();
	process_sleep(6);

	do_next_a9_frame(current_proc->pa8);

	/* calla_for_him(find_ani_part2) */
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	find_ani_part2(ANIM_KDOWN);
	((ADDRESS *)current_proc->pa9)++;
	do_next_a9_frame(current_proc->pa8);
	current_proc=ptemp;

	match_him_with_me();
	((OBJECT *)current_proc->a10)->oypos.u.intpos=ground_y-SCY(0x120);

	shake_a11(0xa,0x10);
	tsound(0x87);					 // add more zest-fully-clean

	takeover_him(freeze_into_boomer);

	((ADDRESS*)current_proc->pa9)--;
	current_proc->a11=(ADDRESS)current_proc->pa9;

	/* shake him over my head */
	current_proc->pa9=(void*)current_proc->a11;
	liftshake();
	liftshake();
	current_proc->pa9=(void*)current_proc->a11;
	liftshake();
	liftshake();
	current_proc->pa9=(void*)current_proc->a11;
	liftshake();
	liftshake();

	process_sleep(0x20);

	tsound(0x24);
	tsound(0x25);
	shake_a11(0xa,0xa);

	a8_back_minus_1(current_proc->pa8);
	ta9=(void *)current_proc->pa8;
	current_proc->pa8=current_proc->pdata.p_otherguy;
	create_fx(FX_SZ_GUTS);
	current_proc->pa8=(OBJECT *)ta9;

	/* calla_for_him(death_scream) */
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	death_scream;
	current_proc=ptemp;

	make_him_invisible;
	((ADDRESS*)current_proc->pa9)++;		// UMK3
	mframew(4);

	/* create an object for the other half of the other dude */
	obj=current_proc->pa8;
	ta9=current_proc->pa9;
	find_ani2_part2(ANIM_SZ_LIFT);
	((ADDRESS*)current_proc->pa9)++;
	gmo_proc(obj->oheap);
	insert_object(current_proc->pa8,&objlst);
	current_proc->a11=(ADDRESS)current_proc->pa8;
	CREATE(PID_BANI,frozen_half_ani);
	current_proc->pa9=ta9;
	current_proc->pa8=obj;

	lineup_a0_onto_a1((OBJECT *)current_proc->a11,obj);

	tsound(0);
	tsound(1);
	shake_a11(0xa,0x10);

	mframew(2);

	process_sleep(0x18);

	death_blow_complete();
	local_victory_animation;
}

void frozen_half_ani(void)
{
	flip_multi(current_proc->pa8);
	find_part2();
	find_part2();
	mframew(2);
	process_suicide();
}

void liftshake(void)
{
	do_next_a9_frame(current_proc->pa8);
	lifts3(current_proc->pa8,3);
	process_sleep(3);
	lifts3(current_proc->pa8,-3);
	process_sleep(3);
	return;
}

void lifts3(OBJECT *pa0,short pa2)
{
	pa0->oxpos.u.intpos+=pa2;
	(current_proc->pdata.p_otherguy)->oxpos.u.intpos+=pa2;
	return;
}

void freeze_into_boomer(void)
{
	void *animframe;
	OBJECT *obj=current_proc->pa8;

	current_proc->pa9=(void *)get_his_char_ani(ANIM_TABLE2,ANIM_SZ_BOOMER);
	obj->oheap=(current_proc->pdata.p_otherguy)->oheap;		// need to point to SZ data area
	animframe=COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->pa9));
	animframe=COMPUTE_ADDR(obj->oheap,GET_LONG(animframe));
	((OIMGTBL*)animframe)++;
	swpal((void*)GET_LONG(animframe),obj);
	do_next_a9_frame(obj);

	obj->oypos.u.intpos=ground_y-SCY(0x90);

	mframew(6);

	wait_forever();
}

//*******************************************************************************
// Subzero Fatality #2
//*******************************************************************************
void sz_blow(void)
{
	fatality_start_pause();

	get_char_ani(ANIM_TABLE2,ANIM_SZ_BLOW);

	ochar_sound(7);

	mframew(5);
	takeover_him(r_ice_blow);
	center_around_him();

	mframew(5);
	delete_slave();
	process_sleep(0x040);

	death_blow_complete();
	local_victory_animation;
}

void r_ice_blow(void)
{
	his_ochar_sound(8);
	face_opponent(current_proc->pa8);

	death_scream;
	player_froze_pal();
	pose_a9(0,ANIM_THUD);

	shake_a8_up(current_proc->pa8,4,0,3,10);

	mframew(4);

	shake_a11(0xa,0x10);
	tsound(0);
	tsound(1);

	current_proc->pa9=(void *)get_his_char_ani(ANIM_TABLE2,ANIM_SZ_THUD);
	(current_proc->pa8)->oheap=(current_proc->pdata.p_otherguy)->oheap;		// switch to using sz heap
	do_next_a9_frame(current_proc->pa8);
	ground_multi(current_proc->pa8);

	away_x_vel(current_proc,current_proc->pa8,SCX(0x18000));

	mframew(5);
	stop_me(current_proc->pa8);
	wait_forever();
}

//*******************************************************************************
// Indian Fatality #1
//*******************************************************************************
void ind_zap_kill(void)
{
	fatality_start_pause();

	get_char_ani(ANIM_TABLE2,ANIM_IND_OCHOP);
	mframew(5);
	process_sleep(6);

	mframew(3);
	delete_slave();
	takeover_him(r_ind_lightning);

	process_sleep(0x40);
	death_blow_complete();
	local_victory_animation;
}

void r_ind_lightning(void)
{
	fatal_offset(current_proc->pa8,-SCX(0xb8),0);
	tsound(0);
	tsound(1);
	shake_a11(0xa,0xd);
	death_scream;
	shocker_shaker(4);
	stack_jump(collapse_on_ground);
}

//*******************************************************************************
// Indian Fatality #2
//*******************************************************************************
void ind_light(void)
{
	OBJECT *obj=current_proc->pa8;
	void *ta9;
	PROCESS *ptemp;

	fatality_start_pause();

	animate2_a9(6,ANIM_IND_SKY_LIGHT);
	process_sleep(0x10);

	ta9=current_proc->pa9;
	get_char_ani(ANIM_TABLE2,ANIM_IND_SKY_ZAP);
	gmo_proc(obj->oheap);
	a8_front_plus_1(current_proc->pa8);
	insert_object(current_proc->pa8,&objlst);
	lineup_a0_onto_a1(current_proc->pa8,current_proc->pdata.p_otherguy);
	multi_adjust_xy(current_proc->pa8,0,SCY(0x90));
	CREATE(PID_BANI,light_animator);
	//CREATE(PID_BANI,light_sound_proc);
	current_proc->pa8=obj;
	current_proc->pa9=ta9;

	ochar_sound(10);
	mframew(5);

	process_sleep(0x10);
	ochar_sound(11);
	process_sleep(0x20);

	/* calla_for_him(death_scream) */
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	death_scream;
	current_proc=ptemp;
	make_him_invisible;
	ochar_sound(12);
	process_sleep(0x10);
	ochar_sound(11);
	process_sleep(0x20);

	death_blow_complete();
	local_victory_animation;
}
void light_animator(void)
{
	mframew(6);
	delobjp(current_proc->pa8);
	process_suicide();
}

//*******************************************************************************
// Sonya Fatality #1
//*******************************************************************************
void sonya_kiss_crusher(void)
{
	PROCESS *pa0;
	OBJECT *ta8;

	fatality_start_pause();

	get_char_ani(ANIM_TABLE2,ANIM_SONYA_DKISS);
	mframew(3);

	ta8=current_proc->pa8;
	get_char_ani(ANIM_TABLE2,ANIM_SONYA_EBALL);
	current_proc->a11=(ADDRESS)current_proc->pa9;
	gmo_proc(ta8->oheap);
	insert_object(current_proc->pa8,&objlst);
	current_proc->a10=(ADDRESS)current_proc->pa8;
	current_proc->pa8=ta8;
	match_ani_points(ta8,(OBJECT*)current_proc->a10);

	adjust_xy_a5((OBJECT*)current_proc->a10,-SCX(0x20),-SCY(0x10));		// lineup with sonya

	ochar_sound(6);

	pa0=CREATE(PID_BANI,crusher_orb);
	pa0->pdata.p_otherguy=current_proc->pdata.p_otherguy;
	pa0->pdata.p_otherproc=current_proc->pdata.p_otherproc;

	ochar_sound(7);

	/* calla_for_him(center_around_me) */
	pa0=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	center_around_me();
	current_proc=pa0;

#if PAL_VERSION==1
	process_sleep(0xa0*6/5);
#else
	process_sleep(0x90);
#endif

	death_blow_complete();
	local_victory_animation;
}

void crusher_orb(void)
{
	OBJECT *obj=(OBJECT *)current_proc->a10;

	current_proc->pa8=obj;
	a8_front_plus_1(obj);

	current_proc->pa9=(void*)current_proc->a11;
	mframew(3);

	set_proj_vel(obj,SCX(0x20000),0);
	init_anirate(3);

	/* crush4 */
	do
	{
		process_sleep(1);
		next_anirate();
	}
	while(abs(obj->oxpos.u.intpos-(current_proc->pdata.p_otherguy)->oxpos.u.intpos)>SCX(0xb1));

	stop_a8(obj);

	match_ani_points(current_proc->pdata.p_otherguy,obj);

	multi_adjust_xy(obj,-SCX(0xb1),0);
	takeover_him(crush_stuggle);

	animate_a0_frames(3,0xd);

	takeover_him(crush_duck);
	current_proc->pa9=(void*)current_proc->a11;
	find_part2();
	find_part2();

	mframew(4);

	delobjp(obj);
	wait_forever();
}

void crush_stuggle(void)
{
	face_opponent(current_proc->pa8);
	death_scream;
	find_ani_part2(ANIM_STUMBLE);
	mframew(4);
	wait_forever();
}

void crush_blood(void)
{
	create_blood_proc(BLOOD_UPPERCUT);
  process_sleep(4);
	create_blood_proc(BLOOD_UPPERCUT);
	process_suicide();
}

void crush_duck(void)
{
	void *tempheap;				// fix bug where sonya goes back to orig heap which prevents guts from showing up

	tempheap=(current_proc->pdata.p_otherguy)->oheap;

	CREATE(PID_BLOOD,crush_blood);
	create_fx(FX_SONYA_CRUSH);

	shake_a11(0xc,0xc);
	tsound(0x23);

	animate_a9(0x4,0x4);

	(current_proc->pa8)->oheap=tempheap;		// switch to sonya fatal heap
	current_proc->pa9=(void*)get_char_ani(ANIM_TABLE2,ANIM_SONYA_GUTZ);
	do_next_a9_frame(current_proc->pa8);

	(current_proc->pa8)->oypos.u.intpos=ground_y;

	multi_adjust_xy(current_proc->pa8,-SCX(0xa8),-SCY(0x90));
	dallprc(PID_SCROLL);
	dallprc(PID_REPELL);
	wait_forever();
}

//*******************************************************************************
// Sonya Fatality #2
//*******************************************************************************
void sonya_kiss(void)
{
	PROCESS *pa0;
	OBJECT *ta8=current_proc->pa8;
	OBJECT *obj;
	void *animframe;

	fatality_start_pause();

	find_ani2_part2(ANIM_SONYA_DKISS);
	current_proc->a11=(ADDRESS)current_proc->pa9;
	animframe=COMPUTE_ADDR(ta8->oheap,GET_LONG(current_proc->pa9));
	gso_dmawnz(obj,(ADDRESS)animframe,ta8->oheap,0);
	alloc_cache((OIMGTBL *)animframe,ta8->oheap,obj);
	current_proc->a10=(ADDRESS)obj;
	insert_object(obj,&objlst);
	current_proc->pa8=ta8;
	lineup_1pwm(obj,ta8);

	get_char_ani(ANIM_TABLE2,ANIM_SONYA_DKISS);
	kissani();
	kissani();

	animframe=COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->a11));
	((OIMGTBL*)animframe)++;
	swpal((void*)GET_LONG(animframe),obj);		// new pal for orb

	/* kiss4 */
	do
	{
		kissani();
	}
	while(GET_LONG(current_proc->pa9)!=NULL);

	ochar_sound(6);
	pa0=CREATE(PID_FX,kiss_orb);
	pa0->pdata.p_otherguy=current_proc->pdata.p_otherguy;
	pa0->pdata.p_otherproc=current_proc->pdata.p_otherproc;

	process_sleep(0x90);

	death_blow_complete();
	local_victory_animation;
}

void kiss_orb(void)
{
	OBJECT *obj;
	long ta3,ta5;

	obj=current_proc->pa8=(OBJECT *)current_proc->a10;
	current_proc->pa9=(void*)current_proc->a11;

	if (is_he_right(current_proc)==SYSTEM_CARRY_SET)
	{
		(long)current_proc->a5=SCX(0x60000);
		(long)current_proc->a11=-SCX(0x8000);
	}
	else
	{
		(long)current_proc->a5=-SCX(0x60000);
		(long)current_proc->a11=SCX(0x8000);
	}

	/* kflame4 */
	obj->oxvel.pos=(long)current_proc->a5;
	process_sleep(4);

	obj->oyvel.pos=-SCY(0x70000);

	/* kflame5 */
	do
	{
		orb_sleep_1();
		obj->oxvel.pos+=(long)current_proc->a11;
		obj->oyvel.pos+=SCY(0x4000);
	}
	while(obj->oyvel.pos<0);

	current_proc->a11=10;
	/* kflame6 */
	do
	{
		orb_sleep_1();
		obj->oxvel.pos+=(long)current_proc->a11;
		obj->oyvel.pos+=SCY(0x8000);
	}
	while(--current_proc->a11>0);

	stop_a8(obj);

	ta3=(current_proc->pdata.p_otherguy)->oxpos.u.intpos;
	ta5=ground_y-SCY(0x58);

	ta3-=obj->oxpos.u.intpos;
	ta5-=obj->oypos.u.intpos;

	ta3<<=16;
	ta5<<=16;

	ta3/=0x10;
	ta5/=0x10;

	obj->oxvel.pos=ta3;
	obj->oyvel.pos=ta5;

	init_anirate(3);
	current_proc->a11=0x10;
	/* kisso3 */
	do
	{
		orb_sleep_1();
	}
	while(--current_proc->a11>0);

	takeover_him(sb_skeleton_burn);
	delobjp(obj);
	process_suicide();
}

void kissani(void)
{
	OBJECT *obj=current_proc->pa8;
	void *ta9;

	do_next_a9_frame(current_proc->pa8);
	ta9=current_proc->pa9;
	current_proc->pa8=(OBJECT*)current_proc->a10;
	current_proc->pa9=(void*)current_proc->a11;
	frame_a9(current_proc->pa8);
	current_proc->a11=(ADDRESS)current_proc->pa9;
	current_proc->pa8=obj;
	current_proc->pa9=ta9;
	process_sleep(4);
	return;
}

void orb_sleep_1(void)
{
	next_anirate();
	process_sleep(1);
	return;
}


//extern void *a_robo_skeleton_burn[];
void robo_skeleton_burn(void)
{
	face_opponent(current_proc->pa8);
	center_around_me();
	//current_proc->pa9=a_robo_skeleton_burn;
	(current_proc->pa8)->oheap=(current_proc->pdata.p_otherguy)->oheap;	// switch to sonya to get correct heap info
	get_char_ani(ANIM_TABLE2,ANIM_ROBO_SKEL);
	stack_jump(skburn3);
}

void scorp_skeleton_burn(void) 
{
	center_around_me();
	stack_jump(sb_skeleton_burn);
}

//extern void *a_sb_skeleton_burn[];
void sb_skeleton_burn(void)
{
	//current_proc->pa9=a_sb_skeleton_burn;
	(current_proc->pa8)->oheap=(current_proc->pdata.p_otherguy)->oheap;	// switch to sonya to get correct heap info
	get_char_ani(ANIM_TABLE2,ANIM_SB_SKEL);
	stack_jump(skburn3);
}

void skburn3(void)
{
	death_scream;
	CREATE(PID_BANI,skel_fire_proc);
	process_sleep(0x12);
	set_inviso(current_proc->pa8);
	wait_forever();
}

void skel_fire_proc(void)
{
	current_proc->a10=(ADDRESS)current_proc->pa8;
	gmo_proc((current_proc->pa8)->oheap);
	lineup_a0_onto_a1(current_proc->pa8,(OBJECT *)current_proc->a10);
	flip_multi(current_proc->pa8);
	a8_front_plus_1(current_proc->pa8);
	(current_proc->pa8)->oypos.u.intpos-=6;			// move up a tad
	insert_object(current_proc->pa8,&objlst);
	tsound(0x20);

	mframew(5);
	current_proc->a10=(ADDRESS)current_proc->pa9;

	current_proc->a11=3;
	/* sfire4 */
	do
	{
		current_proc->pa9=(void*)current_proc->a10;
		mframew(5);
	}
	while(--current_proc->a11>0);

	mframew(5);
	process_sleep(6);

	tsound(0x21);
	mframew(6);
	wait_forever();
}

//*******************************************************************************
// Kano Fatality #1
//*******************************************************************************
extern void *a_kano_rip_skel[];
void kano_skeleton(void)
{
	PROCESS *ptemp;
	void *animframe;

	fatality_start_pause();

	sans_repell_for_good();

	//current_proc->pa9=a_kano_rip_skel;
	//heap=(current_proc->pa8)->oheap;				// remeber my heap
	//(current_proc->pa8)->oheap=&fatal_anims;	// set for fatality
	get_char_ani(ANIM_TABLE2,ANIM_KANO_RIP);
	mframew(6);
	wfe_him;
	make_him_face_me();

	/* calla_for_him(player_normpal) */
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	player_normpal();
	current_proc=ptemp;

	//current_proc->a11=(ADDRESS)ochar_reached[(current_proc->pdata.p_otherguy)->ochar];
	//current_proc->a10=(ADDRESS)current_proc->pdata.p_otherguy;
	//(current_proc->pdata.p_otherguy)->oheap=&fatal_anims;			// switch heap
	(current_proc->pdata.p_otherguy)->oheap=(current_proc->pa8)->oheap;			// switch heap
	current_proc->a0=(ADDRESS)current_proc->pa9;
	animframe=get_char_ani(ANIM_TABLE2,ANIM_OCHAR_REACH);
	(ADDRESS)current_proc->pa9=current_proc->a0;
	((ADDRESS*)animframe)+=(current_proc->pdata.p_otherguy)->ochar;
	current_proc->a11=(ADDRESS)COMPUTE_ADDR((current_proc->pa8)->oheap,GET_LONG(animframe));
	current_proc->a10=(ADDRESS)current_proc->pdata.p_otherguy;

	double_next_a9();

	his_group_sound(2);

	ochar_sound(7);
	shake_him_up(current_proc->pdata.p_otherguy,3,0,3,4);
	process_sleep(0xb);

	ochar_sound(7);
	xfer_otherguy(death_shake);
	shake_a8_up(current_proc->pa8,3,0,3,4);

	ochar_sound(7);
	his_group_sound(9);

	shake_a11(8,8);

	xfer_otherguy(skin_fall);
	(current_proc->pdata.p_otherproc)->a11=current_proc->a11;

	mframew(8);
	death_blow_complete();

	wait_forever();
}

void skin_fall(void)
{
	current_proc->pa9=(void*)current_proc->a11;
	animate_a0_frames(8,2);

	shake_a8_up(current_proc->pa8,3,0,3,10);

	his_ochar_sound(8);
	mframew(8);
	wait_forever();
}

void death_shake(void)
{
	shake_a8_up(current_proc->pa8,3,0,3,4);
	wait_forever();
}

//*******************************************************************************
// Kano Fatality #2
//*******************************************************************************
void kano_lazer(void)
{
	PROCESS *ptemp;
	OBJECT *obj=current_proc->pa8;
	WORD ta10;

	fatality_start_pause();

	ochar_sound(5);

	get_char_ani(ANIM_TABLE2,ANIM_KANO_LASER);
	mframew(4);

	ochar_sound(6);

	takeover_him(local_r_laser);
	current_proc->a11=(ADDRESS)current_proc->pa9;

	/* burn it in!! */
	ta10=4;
	/* laz3 */
	do
	{
		current_proc->pa9=(void *)current_proc->a11;
		mframew(6);
	}
	while(--ta10>0);

	current_proc->pa8=current_proc->pdata.p_otherguy;
	set_inviso(current_proc->pa8);
	ptemp=create_fx(FX_LASER_EXPLODE);
	ptemp->pdata.p_otherguy=obj;			// set process up correctly
	ptemp->pdata.p_otherproc=current_proc;
	current_proc->pa8=obj;

	wfe_him;
	mframew(4);
	delete_slave();
	dallprc(PID_FX);
	death_blow_complete();
	local_victory_animation;
}

WORD test_fade_tab[]={1,2,4,6,8,10,12,14,16,18,20,0xffff};
short ochar_laser_lineups[][2]=
{
	{-SCX(0xb8),SCY(0x00)},
	{-SCX(0xb8),SCY(0x00)},
	{-SCX(0xa8),-SCY(0x10)},
	{-SCX(0xac),SCY(0x00)},
	{-SCX(0xb8),SCY(0x00)},

	{-SCX(0xb8),SCY(0x00)},
	{-SCX(0xd0),-SCY(0x13)},
	{-SCX(0xb8),SCY(0x00)},
	{-SCX(0xb8),SCY(0x00)},
	{-SCX(0xad),SCY(0x00)},

	{-SCX(0xb8),SCY(0x00)},
	{-SCX(0x9a),-SCY(0x09)},
	{-SCX(0xc8),SCY(0x05)},
	{-SCX(0xac),SCY(0x00)},
	{-SCX(0xb8),SCY(0x00)},
	{-SCX(0xb8),SCY(0x00)},
	{-SCX(0xb8),SCY(0x00)},
	{-SCX(0xb8),SCY(0x00)},
	{-SCX(0xb8),SCY(0x00)},
	{-SCX(0xb8),SCY(0x00)},
	{-SCX(0xb8),SCY(0x00)},
	{-SCX(0xb8),SCY(0x00)},
	{-SCX(0xb8),SCY(0x00)},
	{-SCX(0xb8),SCY(0x00)},
};

void local_r_laser(void)
{
	OBJECT *obj=current_proc->pa8;

	group_sound(9);
	set_noedge(current_proc->pa8);

	fade_this_pal(obj->opal,6,1,test_fade_tab);

	get_char_ani(ANIM_TABLE1,ANIM_SCARED);

	match_me_with_him();
	flip_multi(obj);

	multi_adjust_xy(obj,ochar_laser_lineups[obj->ochar][0],ochar_laser_lineups[obj->ochar][1]);
	mframew(4);

	center_around_me();

	shake_a8_up(obj,0x0,0x3,3,5);

	create_fx(FX_SMOKE_GUY)->a10=2;

	shake_a8_up(obj,0x0,0x5,3,10);

	create_fx(FX_SMOKE_GUY)->a10=3;

	/* lazagain */
	do
	{
		shake_a8_up(obj,0x0,0x5,3,40);
	}
	while(1);


}

/******************************************************************************
 Function: void fat_robo_crush(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	robot crush fatality
******************************************************************************/
void fat_robo_crush(void)
{
	PROCESS *ptemp;

	fatality_start_pause();

	robo_open_chest(current_proc->pa8);
	ochar_sound(0x16);

	/* calla_for_him */
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	center_around_me();
	current_proc=ptemp;

	(OBJECT *)current_proc->pdata.p_store4=current_proc->pa8;

	get_char_ani(ANIM_TABLE2,ANIM_ROBO_CRUSH_ARMS);
	do_next_a9_frame(current_proc->pa8);
	process_sleep(6);

	(OBJECT *)current_proc->a10=current_proc->pa8;				// save me here
	a8_back_minus_1(current_proc->pa8=current_proc->pdata.p_slave);
	mframew(5);							// ani: arms coming out

	create_arm_obj();
	current_proc->pdata.p_store1=current_proc->a10;

	mframew(5);								// ano: arms coming out

	create_arm_obj();
	current_proc->pdata.p_store2=current_proc->a10;
	a10_frame_a9();
	create_arm_obj();
	current_proc->pdata.p_store3=current_proc->a10;
	a10_frame_a9();
	xfer_otherguy(wait_forever);				// disable him
	(void *)current_proc->pdata.p_store5=current_proc->pa9;
	process_sleep(0x10);

	ochar_sound(0x17);
	his_group_sound(9);

	/* crush down */
	crush_him_more(-9,0,0);

	crush_him_more(-9,0,4);				// frame 0: ani off= 4
	crush_him_more(-9,1,4);				// frame 1: ani off= 4
	crush_him_more(-15,2,4);			// frame 2: ani off= 4

	clear_shadow_bit(current_proc->pa8);

	/* calla_for_him */
	set_inviso(current_proc->pdata.p_otherguy);			// hide him
	ochar_sound(0x18);
	kill_group_sound(9,current_proc->pdata.p_otherguy);

	get_char_ani(ANIM_TABLE2,ANIM_ROBO_CRUSH_BLOOD);
	(void *)current_proc->pdata.p_store7=current_proc->pa9;		// save blood ani

	ptemp=gmo_proc((current_proc->pa8)->oheap);
	a8_front_plus_1(current_proc->pa8);
	(OBJECT *)current_proc->a0=current_proc->pa8;
	current_proc->pa8=(OBJECT *)current_proc->pdata.p_store4;			// me
	match_ani_points(current_proc->pa8,((OBJECT *)current_proc->a0));
	insert_object((OBJECT*)current_proc->a0,&objlst);
	current_proc->pdata.p_store6=current_proc->a0;

	current_proc->pa8=(OBJECT *)current_proc->a0;		// pass blood object
	create_fx(FX_ROBO_CRUNCH_LIMB);						// bones and limbs and stuff

	/* reshape top and bottom pieces */
	current_proc->pa9=(void *)current_proc->pdata.p_store5;		// save
	current_proc->pa8=(OBJECT *)(current_proc->pdata.p_store3);
	frame_a9(current_proc->pa8);
	current_proc->pa8=(OBJECT *)(current_proc->pdata.p_store2);
	frame_a9(current_proc->pa8);

	/* animate blood */
	adjust_top_and_bottom((0x28),-(0x24));
	crush_sleep_5();

	current_proc->pa8=(OBJECT *)(current_proc->pdata.p_store6);
	current_proc->pa9=(void *)(current_proc->pdata.p_store7);
	((ADDRESS *)current_proc->pa9)++;

	process_sleep(6);

	adjust_top_and_bottom((0x04),-(0x08));
	do_next_a9_frame(current_proc->pa8);
	crush_sleep_5();

	adjust_top_and_bottom((0x03),-(0x03));
	mframew(5);

	/* retract arm */
	delobjp((OBJECT *)current_proc->pdata.p_store1);
	delobjp((OBJECT *)current_proc->pdata.p_store2);
	delobjp((OBJECT *)current_proc->pdata.p_store3);			// rid of unwanted pieces

	current_proc->pa8=(OBJECT *)(current_proc->pdata.p_store4);		// me
	(OBJECT *)current_proc->a10=current_proc->pdata.p_slave;					// arm obj (slave)
	find_ani2_part2(ANIM_ROBO_CRUSH_ARMS);
	find_part2();
	find_part2();

	mframew(6);

	delete_slave();

	robo_close_chest(current_proc->pa8);

	death_blow_complete();

	local_victory_animation;
}

/******************************************************************************
 Function: void adjust_top_and_bottom(short pa0,short pa1)

 By: David Schwartz

 Date: Jan 1995

 Parameters: pa0 - bottom adjustment
			 pa1 - top adjustment

 Returns: None

 Description:	adjust top and bottom part of robot crusher
******************************************************************************/
void adjust_top_and_bottom(short pa0,short pa1)
{
	adjust_piece(current_proc->pdata.p_store2,pa1);
	adjust_piece(current_proc->pdata.p_store3,pa0);

	return;
}

/******************************************************************************
 Function: void crush_him_more(short pa1,WORD pa9frm,short pa9off);

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa1 - amount to adjust top/bottom pieces
			 pa9frm - frame #
			 pa9off - ani offset for him

 Returns: None

 Description:	adjust top and bottom part of robot crusher
******************************************************************************/
void crush_him_more(short pa1,WORD pa9frm,short pa9off)
{
	PROCESS *ptemp;

	adjust_piece(current_proc->pdata.p_store3,pa1);			// bottom piece
	adjust_him_xy(0,pa1);

	adjust_piece(current_proc->pdata.p_store2,-pa1);			// top piece


	if ((LONG)current_proc->pa9!=0)
	{
		/* calla_for_him */
		ptemp=current_proc;
		current_proc=current_proc->pdata.p_otherproc;
		player_normpal();
		pose_a9(pa9frm,pa9off);
		current_proc=ptemp;
	}

	/* chm9 */
	crush_sleep_5();

	return;
}

/******************************************************************************
 Function: void crush_sleep_5(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	continue crush and sleep
******************************************************************************/
void crush_sleep_5(void)
{
	OBJECT *oa0,*oa5,*oa6;

	oa0=(OBJECT *)(current_proc->pdata.p_store1);		// clippable guy
	oa5=(OBJECT *)(current_proc->pdata.p_store2);		// top piece
	oa0->oypos.u.intpos=oa5->oypos.u.intpos;

	oa6=(OBJECT *)(current_proc->pdata.p_store3);		// bottom piece

	oa0->osize.u.ypos=oa6->osize.u.ypos+oa6->oypos.u.intpos-oa5->oypos.u.intpos;

	if (oa0->osize.u.ypos<SCY(3))
		oa0->osize.u.ypos=SCY(3);

	oa0->header.t_height=oa0->osize.u.ypos;	// clip proper amount

	/* crslp4 */
	process_sleep(4);

	return;
}

/******************************************************************************
 Function: void create_arm_obj(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	create a crushing arm object
******************************************************************************/
extern OIMGTBL RBCRUSH4BAR;
void create_arm_obj(void)
{
	OBJECT *oa8=current_proc->pa8;
	OBJECT *obj;
	void *animframe;
	void *temp;

	(OBJECT *)current_proc->a10=oa8;

	temp=current_proc->pa9;
	animframe=get_char_ani(ANIM_TABLE2,ANIM_ROBO_4BAR);
	current_proc->pa9=temp;
	animframe=(void*)COMPUTE_ADDR(oa8->oheap,GET_LONG(animframe));

	gso_dmawnz(obj,(ADDRESS)animframe,oa8->oheap,0);
	alloc_cache((OIMGTBL*)animframe,oa8->oheap,obj);

	lineup_1pwm(obj,oa8);
	a8_front_plus_1(obj);
	insert_object(obj,&objlst);
	(OBJECT *)current_proc->a10=obj;

	current_proc->pa8=oa8;

	return;
}

/******************************************************************************
 Function: void a10_frame_a9(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	animate a frame
******************************************************************************/
void a10_frame_a9(void)
{
	OBJECT *oa8=current_proc->pa8;
	OBJECT *obj=(OBJECT *)current_proc->a10;

	current_proc->pa8=obj;
	a8_back_minus_1(obj);
	frame_a9(obj);
	current_proc->pa8=oa8;

	return;
}

//extern void *a_kano_shocked[];
extern WORD BUZZER[];
void shocker_shaker(WORD pa11)
{
	void *ta10;
	void *heap;

	heap=(current_proc->pa8)->oheap;			// remember old heap
//	current_proc->pa9=a_kano_shocked;
//	(current_proc->pa8)->oheap=fatal_anims;			// heap fixup
	(current_proc->pa8)->oheap=(current_proc->pdata.p_otherguy)->oheap;			// heap fixup
	get_char_ani(ANIM_TABLE2,ANIM_SHOCKER);
	((ADDRESS*)current_proc->pa9)+=((current_proc->pa8)->ochar)*5;
	player_swpal(current_proc,BUZZER,current_proc->pa8);
	ta10=current_proc->pa9;

	/* shsh3 */
	do
	{
		current_proc->pa9=ta10;
		current_proc->a10=3;
		create_fx(FX_SMOKE_GUY);

		dallprc(PID_SMOKER);

		mframew(4);
	}
	while(--pa11>0);

	(current_proc->pa8)->oheap=heap;			// restore old heap

	return;
}

void fatal_offset(OBJECT *obj,short pa0,short pa1)
{
	match_me_with_him();
	flip_multi(obj);
	multi_adjust_xy(obj,pa0,pa1);
	return;
}

void sans_repell_for_good(void)
{
	f_norepell=0x500;
	return;
}

void fatality_start_pause(void)
{
	OBJECT *obj=current_proc->pa8;

	if (gstate==GS_AMODE) 
	{
		f_shadows=0;
		f_no_bonus=0;								// allow bonus routine to do its thing
		return;
	}

	psxspu_start_cd_fade(FADE_TIME,0);

	init_death_blow(FAT_FATALITY);

	pose_a9(0,0);

	/* load fatality file data */
	player_heap_adj();
	character_texture_load(obj->ochar,fataltype,finish_heap_ptr,FATAL_LOAD|ASYNC_LOAD);

	/* sleep for fade */
	process_sleep(0x20);

#if _CD_VERSION_ == 1
	while ( psxcd_async_on() )
		process_sleep(1);				// what for async load to complete
	module_os_close(module_int);			// make sure to close async file
#endif

 	/* load fatality sound data */
	if ( obj==p1_obj )
		PsxSoundLoadFighter1Fatality(p1_obj->ochar);
	else PsxSoundLoadFighter2Fatality(p2_obj->ochar);

#if 1
	/* sony add for memory */
	player_heap_ptr=(current_proc->pa8)->oheap;
	(current_proc->pa8)->oheap=finish_heap_ptr;		// make sure and restore at end of friendship
#endif

	f_no_bonus=0;								// allow bonus routine to do its thing

	return;
}

/******************************************************************************
 Function: void init_death_blow(WORD pa1)

 By: David Schwartz

 Date: May 1995

 Parameters: pa1 - type of ending

 Returns: None

 Description:	start spilling blood and guts
******************************************************************************/
void init_death_blow(WORD pa1)
{
	/* should not need this code, secret move code should filter  */
	//if (f_no_violence)
	//{
	//	f_no_bonus=0;
	//	wait_forever();			// init_abort
	//}

	/* initdb3 */
	f_death=f_final_act=pa1;
	init_special(current_proc->pa8);
	CREATE(PID_FX,make_db_tone);

	if (f_death!=FAT_PIT)
		CREATE(PID_FADE,death_blow_fader);			// no fade on pit fatality

	/* initdb5 */
	return;
}

extern WORD *death_fade_excludes[];
void death_blow_fader(void)
{
	dallprc(PID_CYCLE);			// kill any color cycle

	f_fade=1;
	(LONG)(CREATE(PID_FADE,skydown))->pa9=0x800;
	fadeout(death_fade_excludes);
	process_sleep(7);

	dallprc(PID_FADE);
	f_shadows=0;
	process_suicide();
}

void make_db_tone(void)
{
	process_sleep(2);

	if ( f_music!=0 )
		wess_seq_trigger(TS_FATAL_START);

	process_suicide();
}

void death_blow_complete(void)
{
	WORD pa1;

	pa1=f_death;
	f_death=-1;
	f_no_bonus=0;								// allow bonus routine to do its thing (redundant)

	if (pa1!=FAT_FRIEND)
	{
#if 0
		if (pa1==FAT_ANIMALITY)
			send_code_a3(0x3b);
		else send_code_a3(0x37);
#endif

	if ( f_music!=0 )
		wess_seq_trigger(TS_WINS_TONE);
	}

#if 1
	/* sony add for memory */
	if (pa1==FAT_FATALITY || pa1==FAT_ANIMALITY)
	{
		(current_proc->pa8)->oheap=player_heap_ptr;		// make sure and restore at end of animality
	}
#endif

	return;
}

/******************************************************************************
 Function: void do_pit_fatality(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	the dreaded pit mulch fatality
******************************************************************************/
void do_pit_fatality(void)
{
	init_special(current_proc->pa8);
	CREATE(PID_FX,make_db_tone);
	f_death=FAT_PIT;

	(current_proc->pdata.p_otherproc)->a10=0xedb00;		// flag send em down to hell
	do_duck();
	process_sleep(8);
	do_uppercut();

	find_ani_part2(ANIM_UPPERCUT);
	mframew(5);
	local_victory_animation;
}

void make_him_face_me(void)
{
	PROCESS *ptemp;

	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	face_opponent(current_proc->pa8);
	current_proc=ptemp;
	return;
}

void r_scared_of_skunk(void) 
{
	scared_pose();
	shake_a8_up(current_proc->pa8,0,4,3,12);
	stack_jump(collapse_on_ground);
}

void r_scared_of_scorp(void) 
{
	scared_pose();
	shake_a8_up(current_proc->pa8,0,4,3,12);
	wait_forever();
}

void scared_pose(void) 
{
	pose_a9(0,ANIM_SCARED);
	face_opponent(current_proc->pa8);
	death_scream;
	return;
}