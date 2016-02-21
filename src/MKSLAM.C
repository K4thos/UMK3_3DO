/******************************************************************************
 File: mkslam.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Slam/Special Moves Routines
******************************************************************************/

/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkos.h"
#include "mkobj.h"
#include "mkgame.h"
#include "mkutil.h"
#include "mkani.h"
#include "moves.h"
#include "mkfx.h"
#include "mkjoy.h"
#include "mkguys.h"
#include "mkslam.h"
#include "mkstat.h"
#include "mksound.h"

JUMPTBL air_slam_jumps[] =
{
	kano_air_body_slam,	// 0
	wait_forever,		// 1
	wait_forever,		// 2
	wait_forever,		// 3
	wait_forever,		// 4

	wait_forever,		// 5
	wait_forever,		// 6
	wait_forever,		// 7
	robo2_air_slam,		// 8
	wait_forever,		// 9

	wait_forever,		//
	wait_forever,		//
	wait_forever,		//
	wait_forever,		//
	smoke_air_slam,

	wait_forever,
	wait_forever,
	wait_forever,

	scorp_air_slam,
	wait_forever,
	wait_forever,
	wait_forever,
	scorp_air_slam,
	wait_forever,
};

JUMPTBL body_slam_jumps[] =
{
	kano_slam, 		// 0
	sonya_slam,		// 1
	jax_slam,		// 2
	indian_slam, 	// 3
	sz_slam,		// 4
	sw_slam,		// 5
	lia_slam,		// 6
	robo1_slam,		// 7
	robo2_slam,		// 8
	lao_slam,		// 9
	tusk_slam,		// 10
	sg_slam,		// 11
	st_slam,		// 12
	lk_slam,		// 13
	smoke_slam,		// 14
	kitana_slam,
	jade_slam,
	mileena_slam,

	nj_slam,
	nj_slam,
	nj_slam,
	nj_slam,
	nj_smoke_slam,
	noob_slam,
	kano_slam,
	sz_slam
};


WORD ochar_slam_damage[] =
{
 	0x18,			// 0 - kano
 	0x18,
 	0x18,
 	0x18,
 	0x18,
 
 	0x18,
 	0x18,
 	0x18,
 	0x18,
 	0x18,
 
 	0x18,
 	0x18,
 	0x18,
 	0x18,
 	0x18,
 
 	0x18,
 	0x18,
	0x18,
	0x18,
	0x18,
	0x18,

	0x18,
	0x18,
	0x25,
	0x25
};


/******************************************************************************
 Function: void do_air_slam(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	do a characters in air slam
******************************************************************************/
void do_air_slam(void)
{
	stack_jump(air_slam_jumps[(current_proc->pa8)->ochar]);
}

/******************************************************************************
 Function: void do_body_slam(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	do a characters in body slam
******************************************************************************/
void do_body_slam(void)
{
	stack_jump(body_slam_jumps[(current_proc->pa8)->ochar]);
}


/******************************************************************************
 Function: void kano_air_body_slam(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	kano's air body slam
******************************************************************************/
void kano_air_body_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	air_slam_init(obj,ANIM_KANO_AIR_SLAM,ANIM_AFB_KANO);

	match_him_with_me_f();				// line him up

	double_next_a9();					// him and me = first frame

	process_sleep(8);					// still in air

	double_mframew(2);					// step #2 throw

	xfer_to_thrown(air_slamed_by_kano);

	process_sleep(10);

	/* kano fall to ground */
	flight(obj,SCX(0),SCY(0x18000),SCY(0x8000),NEVER_ANI,-1);

	jump_up_land_jsrp();
	reaction_exit(obj);
}

void scorp_air_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	air_slam_init(obj,ANIM_SCORP_AIRTHROW,ANIM_FB_NJ);
	throw_voice();
	match_him_with_me_f();				// line him up
	find_part2();					// skip grab
	
	obj->ograv.pos=SCY(0x4000);
	((OBJECT *)current_proc->a10)->ograv.pos=SCY(0x4000);
	double_mframew(3);

	xfer_to_thrown(thrown_by_nj);

	flight(obj,0,SCY(0x18000),SCY(0x8000),NEVER_ANI,-1);

	animate_a9(3,ANIM_GETUP);
	
	reaction_exit(obj);
}

void nj_smoke_slam(void) 
{
	WORD ta0;

	ta0=(current_proc->pa8)->ochar;
	(current_proc->pa8)->ochar=FT_SCORPION;
	body_slam_init(current_proc->pa8);
	(current_proc->pa8)->ochar=ta0;
	njs13();
	reaction_exit(current_proc->pa8);
}			

void nj_slam(void) 
{
	body_slam_init(current_proc->pa8);
	njs13();
	reaction_exit(current_proc->pa8);
}

void njs13(void) 
{
	grab_animation(3);
	process_sleep(3);

	throw_voice();
	double_mframew(3);
	xfer_to_thrown(thrown_by_nj);
	process_sleep(8);
	mframew(3);
	return;
}



/******************************************************************************
 Function: void air_slamed_by_kano(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	kano's air body slam reaction
******************************************************************************/
void air_slamed_by_kano(void)
{
	OBJECT *obj=current_proc->pa8;

	flight(obj,SCX(0x60000),SCY(0x30000),SCY(0x8000),4,-1);

	/* bounce on ground */
	group_sound(2);

	shake_n_sound();

	damage_to_me(0x10,obj);

	flight(obj,SCX(0x30000),-SCY(0x60000),SCY(0x8000),4,-1);

	land_on_my_back();
}

/******************************************************************************
 Function: void smoke_air_slam(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	smokes's air body slam
******************************************************************************/
void smoke_air_slam(void)
{
	OBJECT *obj=current_proc->pa8;
	PROCESS *ptemp;

	air_slam_init(obj,ANIM_ROBO_AIR_SLAM,ANIM_AFB_ROBO2);

	/* calla_for_him(face_opponent) */
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	face_opponent(current_proc->pa8);
	current_proc=ptemp;

	grab_animation(4);
	process_sleep(3);

	throw_voice();
	double_mframew(3);

	xfer_to_thrown(thrown_by_smoke);
	process_sleep(10);
	drop_down_land_jump();
}

void jade_slam(void) 
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);

	grab_animation(3);
	process_sleep(3);

	throw_voice();
	double_mframew(2);

	xfer_to_thrown(thrown_by_jade);
	process_sleep(10);

	mframew(2);

	reaction_exit(obj);
}

void mileena_slam(void) 
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);

	grab_animation(3);
	process_sleep(3);

	throw_voice();
	double_mframew(2);

	xfer_to_thrown(thrown_by_mileena);
	process_sleep(10);

	mframew(2);

	reaction_exit(obj);
}

void kitana_slam(void) 
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);
	grab_animation(3);
	process_sleep(2);

	throw_voice();
	double_mframew(2);

	xfer_to_thrown(thrown_by_kitana);

	do_next_a9_frame(obj);
	process_sleep(8);
	do_next_a9_frame(obj);
	process_sleep(8);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void smoke_slam(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	smokes's body slam
******************************************************************************/
void smoke_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);

	grab_animation(4);
	process_sleep(3);

	throw_voice();
	double_mframew(3);

	xfer_to_thrown(thrown_by_smoke);
	process_sleep(10);

	face_opponent(obj);
	pose_a9(0,ANIM_STANCE);
	ground_ochar();
	process_sleep(2);

	reaction_exit(obj);
}


/******************************************************************************
 Function: void tusk_slam(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	tusk  body slam
******************************************************************************/
void tusk_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);

	grab_animation(3);

	throw_voice();

	double_mframew(3);

	xfer_to_thrown(thrown_by_tusk);

	process_sleep(10);

	mframew(8);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void kano_slam(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	lao's air body slam
******************************************************************************/
void lao_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);

	grab_animation(2);

	ochar_sound(5);
	//throw_voice();

	double_mframew(3);

	xfer_to_thrown(thrown_by_lao);

	flight(obj,DONT_TOUCH,DONT_TOUCH,SCY(0x8000),2,-1);

	jump_up_land_jsrp();
 	reaction_exit(obj);
}

/******************************************************************************
 Function: void kano_slam(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	kano's air body slam
******************************************************************************/
void kano_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);

	grab_voice();

	grab_animation(3);

	process_sleep(3);

	throw_voice();

	double_mframew(4);

	xfer_to_thrown(thrown_by_kano);

	do_next_a9_frame(obj);

	process_sleep(16);

	mframew(8);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void thrown_by_kano(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	being thrown by kano
******************************************************************************/
void thrown_by_kano(void)
{
	OBJECT *obj=current_proc->pa8;

	flight(obj,SCX(0xa0000),-SCY(0x10000),SCY(0x6000),6,-1);

	damage_to_me(ochar_slam_damage[(current_proc->pdata.p_otherguy)->ochar],obj);

	land_on_my_back();
}

/******************************************************************************
 Function: void robo2_air_slam(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	robot 2 air slam
******************************************************************************/
void robo2_air_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	air_slam_init(obj,ANIM_ROBO_AIR_SLAM,ANIM_AFB_ROBO2);

	match_him_with_me_f();				// line him up

	double_next_a9();					// him and me = first frame

	process_sleep(6);					// still in air

	double_mframew(3);					// step #2 throw

	xfer_to_thrown(air_slamed_by_robo2);

	process_sleep(10);

	drop_down_land();
	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void drop_down_land(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	drop to ground and land routine
******************************************************************************/
void drop_down_land(void)
{
	flight(current_proc->pa8,SCX(0),SCY(0x18000),SCY(0x8000),NEVER_ANI,-1);

	jump_up_land_jsrp();
	return;
}

/******************************************************************************
 Function: void drop_down_land_jump(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	drop to ground and land routine
******************************************************************************/
void drop_down_land_jump(void)
{
	flight(current_proc->pa8,SCX(0),SCY(0x18000),SCY(0x8000),NEVER_ANI,-1);

	jump_up_land_jsrp();
	reaction_exit(current_proc->pa8);
}



/******************************************************************************
 Function: void lk_slam(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	liu kang body slam
******************************************************************************/
void lk_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);

	grab_animation(3);

	process_sleep(3);

	throw_voice();
	double_mframew(3);
	xfer_to_thrown(thrown_by_lk);

	process_sleep(10);

	mframew(3);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void sg_slam(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	she goro body slam
******************************************************************************/
void sg_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);
	grab_animation(3);
	process_sleep(3);

	throw_voice();
	me_in_back();

	double_next_a9();
	process_sleep(4);
	double_next_a9();
	process_sleep(4);
	double_next_a9();
	process_sleep(10);

	double_mframew(3);
	xfer_to_thrown(thrown_by_sg);
	do_next_a9_frame(obj);

	process_sleep(10);

	mframew(6);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void sonya_slam(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	sonya's air body slam
******************************************************************************/
void sonya_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);

	grab_animation(3);

	process_sleep(3);

	throw_voice();

	double_mframew(4);

	xfer_to_thrown(thrown_by_sonya);

	process_sleep(16);

	mframew(8);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void thrown_by_sonya(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	being thrown by sonya
******************************************************************************/
void thrown_by_sonya(void)
{
	OBJECT *obj=current_proc->pa8;

	flight(obj,SCX(0x80000),SCY(0),SCY(0x6000),6,-1);

	slam_damage(obj);
}

void thrown_by_nj(void) 
{
	OBJECT *obj=current_proc->pa8;

	flight(obj,SCX(0x90000),SCY(0),SCY(0x5000),6,-1);

	slam_damage(obj);
}

void slam_damage(OBJECT *obj) 
{
	damage_to_me(ochar_slam_damage[(current_proc->pdata.p_otherguy)->ochar],obj);

	land_on_my_back();
	
}

/******************************************************************************
 Function: void jax_slam(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	jax's air body slam
******************************************************************************/
void jax_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);

	/* calla_for_him routine substit */
	set_ignore_y(current_proc->pdata.p_otherguy);

	grab_animation(3);
	process_sleep(3);

	current_proc->pdata.p_store1=read_lp_tick_state(obj);		// remember when he 1st pressed low punch
	current_proc->pdata.p_store2=0;						// flag: multislam ok
	current_proc->pdata.p_store3=4;						// total slams allowed

	throw_voice();

	while (TRUE)
	{
		do
		{
			/* JSLAM5 */
			double_next_a9();

			jax_slam_sleep(3);
		}
		while(GET_LONG(current_proc->pa9)!=0);

		/* effects */
		ochar_sound(5);			// jax slam on ground sound
		his_group_sound(2);		// him: wasted voice
		shake_a11(0x0009,0x0009);

		/* damage */
		add_combo_damage(0x0a);
		damage_to_him(0x0a,current_proc->pdata.p_otherguy);		// inflict pain
		inc_his_p_hit();

		if (am_i_joy()==SYSTEM_CARRY_SET)
			if (current_proc->pdata.p_store2!=0)
				break;				// to bad --> no moe slammin

		if (--current_proc->pdata.p_store3==0)
			break;				// max slams allowed

		jax_slam_sleep(4);

		f_norepell=0x30;					// disable repell

		jax_multi_slam_pose(4);
		jax_multi_slam_pose(3);
		jax_multi_slam_pose(2);

		flip_multi(current_proc->pa8);

		multi_adjust_xy(current_proc->pa8,-SCX(0x0f),0); 	// lineup with previous flipped frame

		flip_multi((OBJECT *)current_proc->a10);

		jax_multi_slam_pose(2);
	}

	/*	JAX_SLAM_OVER */
	xfer_to_thrown(thrown_by_jax);
	process_sleep(10);

	find_ani_part_a14(ANIM_THROW,3);			// out of slam ani

	mframew(5);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void jax_multi_slam_pose(WORD offset)

 By: David Schwartz

 Date: Jan 1995

 Parameters: offset input

 Returns: None

 Description:	mutli slam pose for jax
******************************************************************************/
void jax_multi_slam_pose(WORD offset)
{

	current_proc->a11=get_his_char_ani(ANIM_TABLE1,ANIM_FB_JAX);	// a11 - his ani (flipped by me)

	current_proc->a10=(ADDRESS)current_proc->pdata.p_otherguy;	// a10 - his obj

	find_ani_part2(ANIM_THROW);

	((ADDRESS *)current_proc->pa9)+=offset;				// jax correct "slam" frame

	((ADDRESS *)(current_proc->a11))+=(offset+(offset<<1));	// his matching "slammed"

	double_next_a9();

	jax_slam_sleep(3);

	return;
}

/******************************************************************************
 Function: void jax_slam_sleep(WORD sleep)

 By: David Schwartz

 Date: Nov 1994

 Parameters: sleep - sleep time

 Returns: None

 Description:	sleep and check still pressing hi punch (only)
******************************************************************************/
void jax_slam_sleep(WORD sleep)
{
	OBJECT *obj=current_proc->pa8;

	process_sleep(sleep);

	if (get_tsl(l_hp,obj)<=12) 			// no tapping hi punch = no multi slam
	{
		if (read_lp_tick_state(obj)==current_proc->pdata.p_store1)			// remember when 1st pressed low punch
			return;															// touch since, no--> cool
	}

	/* mutli_slam_no */
	current_proc->pdata.p_store2=1;				// flag: no multislam

	return;
}

/******************************************************************************
 Function: inline WORD read_lp_tick_state(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: dude who wants to know

 Returns: None

 Description:	reads last lp tick state
******************************************************************************/
inline WORD read_lp_tick_state(OBJECT *obj)
{
	WORD result;

	result=(obj->oid==PID_P1)? l_lp[PLAYER1_TYPE]: l_lp[PLAYER2_TYPE];
	return(result);
}

/******************************************************************************
 Function: void thrown_by_jax(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	being thrown by jax
******************************************************************************/
void thrown_by_jax(void)
{
	common_slam(0);
}

/******************************************************************************
 Function: void thrown_by_robo2(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	being thrown by jax
******************************************************************************/
void thrown_by_robo2(void)
{
	OBJECT *obj=current_proc->pa8;

	ground_player(obj);

	find_ani_part2(ANIM_KDOWN);

	do_next_a9_frame(obj);

	group_sound(2);				// wasted

	rsnd_ground();

	shake_a11(0x0008,0x0008);
	set_half_damage(obj);
	current_proc->pdata.p_action=ACT_SLAM_BOUNCE;
	current_proc->pdata.p_damage=0x1a;
	damage_to_me(0x1a,obj);

	/* inc_p_hit */
	current_proc->pdata.p_hit++;

	flight(obj,-SCX(0x30000),-SCY(0xa0000),SCY(0x8000),4,-1);

	land_on_my_back();
}

/******************************************************************************
 Function: void thrown_by_sg(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	being thrown by she goro
******************************************************************************/
void thrown_by_sg(void)
{
	ground_slammed_init();
	current_proc->pdata.p_hit++;
	current_proc->pdata.p_damage+=35;
	damage_to_me(35,current_proc->pa8);
	set_half_damage(current_proc->pa8);

	process_sleep(3);

	flight(current_proc->pa8,SCX(0x40000),-SCY(0xa0000),SCY(0x8000),4,-1);

	common_slam(0);			// we already gave out damage
}

void air_slamed_by_robo2(void) 
{
	/* inc p hit */
	current_proc->pdata.p_hit++;
	current_proc->pdata.p_damage+=0x19;
	common_slam(0x19);
}

/******************************************************************************
 Function: void thrown_by_lia(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	being thrown by lia
******************************************************************************/
void thrown_by_lia(void)
{
	common_slam(32);
}

/******************************************************************************
 Function: void comman_slam(WORD damage)

 By: David Schwartz

 Date: Jan 1995

 Parameters: damage - amount slam causes

 Returns: None

 Description:	common slam routine for all
******************************************************************************/
void ground_slammed_init(void)
{
	OBJECT *obj=current_proc->pa8;

	ground_player(obj);

	find_ani_part2(ANIM_KDOWN);

	do_next_a9_frame(obj);

	group_sound(2);					// wasted

	shake_n_sound();
}

/******************************************************************************
 Function: void comman_slam(WORD damage)

 By: David Schwartz

 Date: Jan 1995

 Parameters: damage - amount slam causes

 Returns: None

 Description:	common slam routine for all
******************************************************************************/
void common_slam(WORD damage)
{
	OBJECT *obj=current_proc->pa8;

	ground_slammed_init();

	if (damage!=0)
		damage_to_me(damage,obj);

	flight(obj,SCX(0x30000),-SCY(0x60000),SCY(0x8000),4,-1);

	land_on_my_back();
}

/******************************************************************************
 Function: void indian_slam(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	indian's air body slam
******************************************************************************/
void indian_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);

	grab_animation(3);

	process_sleep(3);

	throw_voice();

	double_mframew(3);

	xfer_to_thrown(thrown_by_indian);

	process_sleep(10);

	mframew(8);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void st_slam(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	shang tsu body slam
******************************************************************************/
void st_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);

	grab_animation(3);

	process_sleep(3);

	throw_voice();

	double_mframew(3);

	xfer_to_thrown(thrown_by_indian);

	process_sleep(10);

	mframew(8);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void sz_slam(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	sub zero's body slam
******************************************************************************/
void sz_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);

	grab_animation(3);

	process_sleep(3);

	double_next_a9();
	process_sleep(8);						// hold em up for a sec

	me_in_back();
	throw_voice();

	double_mframew(4);

	xfer_to_thrown(thrown_by_sz);

	process_sleep(12);						// hold last frame

	do_next_a9_frame(obj);					// retract

	process_sleep(5);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void thrown_by_sz(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	being thrown by sub zero
******************************************************************************/
void thrown_by_sz(void)
{
	OBJECT *obj=current_proc->pa8;

	flight(obj,SCX(0x60000),-SCY(0x60000),SCY(0xa000),NEVER_ANI,-1);

	damage_to_me(25,obj);

	land_on_my_back();
}

/******************************************************************************
 Function: void do_back_breaker(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:  jax back breaker
******************************************************************************/
void do_back_breaker(void)
{
	OBJECT *obj=current_proc->pa8;
	PROCESS *ptemp;
	void *ta9;

	air_init_special(obj);
	stop_me(obj);
	stop_him(current_proc->pdata.p_otherguy);
	clear_inviso(current_proc->pdata.p_otherguy);
	clear_shadow_bit(current_proc->pdata.p_otherguy);
	clear_shadow_bit(obj);

	f_norepell=0x38;				// disable repell
	takeover_him_sr(flipped_pause);
	current_proc->pdata.p_action=ACT_BACKBREAK;					// define my action
	(current_proc->pdata.p_otherproc)->a11=ACT_BACKBREAK;		// pass my action

	ochar_sound(6);					// gotcha
	me_in_front();

	get_char_ani(ANIM_TABLE2,ANIM_JAX_BACK);
	do_next_a9_frame(obj);
	multi_adjust_xy(obj,0,0);

	/* pose him --> back getting snapped */
	/* calla_for_him (player_normpal & last_knockdown_frame) */
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	player_normpal();
	last_knockdown_frame();
	current_proc=ptemp;

	match_him_with_me();
	adjust_him_xy(SCX(0xa),-SCY(0x6b));
	process_sleep(6);

	(FUNC)current_proc->a6=bb_fall_call;
	flight_call(obj,0,SCY(0x20000),SCY(0xa000),NEVER_ANI,-1);
	stop_him(current_proc->pdata.p_otherguy);

	get_char_ani(ANIM_TABLE2,ANIM_JAX_BACK);
	((ADDRESS *)current_proc->pa9)++;
	do_next_a9_frame(obj);				// breaker frame #2

	adjust_him_xy(SCX(0x0),SCY(0x70));

	current_proc->pa9=(void*)get_his_char_ani(ANIM_TABLE1,ANIM_BACK_BROKE);
	current_proc->pdata.p_store5=(ADDRESS)ta9=(ADDRESS)current_proc->pa9;

	/* calla_for_him (do_next_a9_frame) */
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	current_proc->pa9=ta9;
	do_next_a9_frame(current_proc->pa8);
	current_proc=ptemp;

	process_sleep(3);

	ochar_sound(0xb);			// back break
	his_group_sound(2);
	shake_a11(0x5,0x5);

	process_sleep(3);
	damage_to_him(0x1a,current_proc->pdata.p_otherguy);
	takeover_him(post_broken_back);

	process_sleep(0x18);
	do_backup();
	reaction_exit(current_proc->pa8);
}

void bb_fall_call(void)
{
	(current_proc->pdata.p_otherguy)->oyvel.pos=(current_proc->pa8)->oyvel.pos;
	return;
}

void post_broken_back(void)
{
	flight(current_proc->pa8,SCX(0x38000),-SCY(0x80000),SCY(0xa000),NEVER_ANI,-1);
	land_on_my_back();
}

void last_knockdown_frame(void)
{
	get_char_ani(ANIM_TABLE1,ANIM_KDOWN);
	find_last_frame();
	do_next_a9_frame(current_proc->pa8);
	return;
}

/******************************************************************************
 Function: void sw_slam(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	swat body slam
******************************************************************************/
void sw_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);

	grab_animation(4);
	process_sleep(2);

	throw_voice();

	double_mframew(4);

	xfer_to_thrown(thrown_by_swat);

	process_sleep(10);

	mframew(8);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void lia_slam(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	lia body slam
******************************************************************************/
void lia_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	ochar_sound(0);					// lia hair whip sound

	body_slam_init(obj);
	grab_animation(1);
	process_sleep(3);

	double_mframew(4);

	xfer_to_thrown(thrown_by_lia);

	process_sleep(8);

	mframew(4);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void robo2_slam(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	robo2 body slam
******************************************************************************/
void robo2_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);
	grab_animation(3);
	process_sleep(4);

	ochar_sound(7);

	init_anirate(4);

	away_x_vel(current_proc,obj,SCX(0x20000));


	obj->oyvel.pos=-SCY(0x180000);
	obj->ograv.pos=SCY(0x1a000);

	stick_him_with_me(obj);

	/* r2slam1, airborn stuff */
	do
	{
		process_sleep(1);

		stick_him_with_me(obj);
		if (--current_proc->pdata.p_anicount==0)
		{
			current_proc->pdata.p_anicount=current_proc->pdata.p_anirate;			// reload anicount
			if (GET_LONG(current_proc->pa9)!=0)			// ani done
				double_next_a9();
		}

		/* r2slam3 */
	}
	while(obj->oypos.u.intpos<(current_proc->pdata.p_ganiy+SCY(0x10)));		// hit ground yet??

	sans_repell;

	shake_a11(0x0008,0x0008);

	rsnd_ground();

	his_group_sound(2);			// him: wasted voice

	stop_me(obj);

	stop_him(current_proc->pdata.p_otherguy);

	xfer_to_thrown(thrown_by_robo2);

	process_sleep(14);

	face_opponent(obj);

	ground_player(obj);

	do_backup();

	reaction_exit(obj);
}

/******************************************************************************
 Function: void stick_him_with_me(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: obj - dude

 Returns: None

 Description:	stick other dude with me
******************************************************************************/
void stick_him_with_me(OBJECT *obj)
{
	((OBJECT *)(current_proc->a10))->oxvel.pos=obj->oxvel.pos;		// illegitiamte way
	((OBJECT *)(current_proc->a10))->oyvel.pos=obj->oyvel.pos;		// illegitiamte way

	set_x_vel((OBJECT *)current_proc->a10,obj->oxvel.pos);			// set him the legit way

	return;
}

/******************************************************************************
 Function: void robo1_slam(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	robo body slam
******************************************************************************/
void robo1_slam(void)
{
	OBJECT *obj=current_proc->pa8;

	body_slam_init(obj);

	ochar_sound(0);

	grab_animation(1);

	mframew(2);					// grab him quickly

	ochar_sound(1);				// mech arm throw sound

	double_mframew(3);

	xfer_to_thrown(thrown_by_robo);

	ochar_sound(2);				// mech arm in sound

	mframew(3);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void xfer_to_thrown(FUNC routine)

 By: David Schwartz

 Date: Nov 1994

 Parameters: routine - routine to xfer other dude to

 Returns: None

 Description:	xfer guy to thrown
******************************************************************************/
void xfer_to_thrown(FUNC routine)
{
	takeover_him_routine(TH_NORMAL,routine);

	(ADDRESS)(current_proc->pdata.p_otherproc)->pa9=current_proc->a11;		// pass anim ptr
}

/******************************************************************************
 Function: void xfer_him_to_flipped_pause(WORD actnum)

 By: David Schwartz

 Date: Nov 1994

 Parameters: actnum - my new action

 Returns: None

 Description:	setup my action and xfer other player to flipped pause
******************************************************************************/
void xfer_him_to_flipped_pause(WORD actnum)
{
	current_proc->pdata.p_action=actnum;				// define my action

	takeover_him_routine(TH_SR,flipped_pause);

	(current_proc->pdata.p_otherproc)->a11=actnum;		// pass my action for him to look 4
}

/******************************************************************************
 Function: void flipped_pause(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: current_proc->a11 - action being done on my

 Returns: None

 Description:	wait until other player is done doing action then ground me
******************************************************************************/
void flipped_pause(void)
{
	lights_on_slam();

	do
	{
		process_sleep(1);
	}
	while(current_proc->a11==((current_proc->pdata.p_otherproc)->pdata.p_action));

	ground_player(current_proc->pa8);

	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void grab_animation(WORD sleeptime)

 By: David Schwartz

 Date: Nov 1994

 Parameters: sleeptime - time to sleep

 Returns: None

 Description:	grab an animation
******************************************************************************/
void grab_animation(WORD sleeptime)
{
	get_char_ani(ANIM_TABLE1,ANIM_THROW);
	mframew(sleeptime);

	return;
}

/******************************************************************************
 Function: void body_slam_init(OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - dude whose slamming

 Returns: None

 Description:	init for a body slam
******************************************************************************/
WORD ochar_slammed_anis[]=
{
	ANIM_FB_KANO,			// 0x26
	ANIM_FB_SONYA,
	ANIM_FB_JAX,
	ANIM_FB_IND,
	ANIM_FB_SZ,

	ANIM_FB_SWAT,			// 0x2b
	ANIM_FB_LIA,
	ANIM_FB_ROBO1,
	ANIM_FB_ROBO2,
	ANIM_FB_LAO,

	ANIM_FB_TUSK,			// 0x30
	ANIM_FB_SG,
	ANIM_FB_ST,
	ANIM_FB_LK,
	ANIM_FB_SMOKE,

	0x35,					// 35, not used (motaro)
	0x36,					// 36, not used (shao)
	ANIM_FB_MILEENA,
	ANIM_FB_NJ,
	ANIM_FB_NJ1,			
	ANIM_FB_NJ2,			
	ANIM_FB_NJ3,			
	ANIM_DUMMYA,			

	ANIM_AFB_KANO,		
	ANIM_LEGGED,			
	ANIM_PROJECTILE,		

	ANIM_FB_KANO,
	ANIM_FB_SZ
};

void body_slam_init(OBJECT *obj)
{
	PROCESS *ptemp;

	grab_voice();
	init_special(obj);

	lights_on_slam();

	delete_slave();

	f_norepell=0x30;					// disable repell

	current_proc->a11=get_his_char_ani(ANIM_TABLE1,ochar_slammed_anis[obj->ochar]);		// add "flipped by me" offset, a11= his ani
	(OBJECT *)current_proc->a10=current_proc->pdata.p_otherguy;						// a10 = his obj

	get_char_ani(ANIM_TABLE1,ANIM_THROW);

	/* calla_for_him(face_opponent) */
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	face_opponent(current_proc->pa8);
	back_to_normal();
	clear_inviso(current_proc->pa8);
	current_proc=ptemp;

	xfer_him_to_flipped_pause(ACT_BODYSLAM);

	if (test_gameflag(M_GF_THROWYES))
		create_fx(FX_THROW_ENCOURAGE);

	return;
}

/******************************************************************************
 Function: void air_slam_init(OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - dude whose slamming

 Returns: None

 Description:	init for an airslam
******************************************************************************/
void air_slam_init(OBJECT *obj,WORD aninum_thrower,WORD aninum_throwee)
{
	ADDRESS animframe;

	air_init_special(obj);

	lights_on_slam();

	delete_slave();

	animframe=(ADDRESS)current_proc->pa9;

	current_proc->a11=get_his_char_ani(ANIM_TABLE1,aninum_throwee);

	current_proc->a10=(ADDRESS)current_proc->pdata.p_otherguy;

	(ADDRESS)current_proc->pa9=animframe;
	get_char_ani(ANIM_TABLE2,aninum_thrower);

	xfer_him_to_flipped_pause(ACT_BODYSLAM);

	f_norepell=0x30;

	return;
}

/******************************************************************************
 Function: void grab_voice(void)

 By: David Schwartz

 Date: Dec 1995

 Parameters: None

 Returns: None

 Description:	grab voice sound effect
******************************************************************************/
void grab_voice(void)
{
	group_sound(3);
	return;
}

/******************************************************************************
 Function: void throw_voice(void)

 By: David Schwartz

 Date: Dec 1995

 Parameters: None

 Returns: None

 Description:	throw voice sound effect
******************************************************************************/
void throw_voice(void)
{
	group_sound(4);
	return;
}
