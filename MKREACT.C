/******************************************************************************
 File: mkreact.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Collision Reaction routines
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
#include "mkguys.h"
#include "mkutil.h"
#include "mkani.h"
#include "mkjoy.h"
#include "mkblood.h"
#include "mkfx.h"
#include "moves.h"
#include "mkdrone.h"
#include "mkreact.h"
#include "mkblood.h"
#include "mksound.h"
#include "mkfatal.h"
#include "valloc.h"
#include "psxspu.h"

JUMPTBL reaction_tables [] =
{
		r_hi_kick,		// 0
		r_lo_kick,		// 1
		r_hi_punch,		// 2
		r_lo_punch,		// 3
		r_sweep,			// 4
		r_duck_punch,		// 5
		r_duck_kickh,		// 6
		r_duck_kickl,		// 7
		r_uppercut,		// 8
		r_elbow_knee,   		// 9
		r_flip_kick,		// a
		r_flip_punch,		// b
		r_roundhouse,		// c
		wait_forever,		// d - keep this as wait_forever !!!!!!!
		r_combo2_stab,			// e
		r_combo1_stab,			// f
		r_combo0,		// 10
		r_combo1,		// 11
		r_combo2,		// 12
		r_combo3,		// 13
		r_combo4,		// 14
		r_combo5,		// 15
		r_combo_klang,	// 16
		r_combo6,		// 17
		r_dummy,		// 18
		r_dummy,		// 19

/*
* Kano
*/
		r_kano_roll,		// 1a
		r_kano_zap,			// 1b
		r_kano_swipe,		// 1c
/*
* sonya
*/
		r_square,		// 1d
		r_sonya_zap,		// 1e
		r_dummy,			// 1f
		r_dummy,			// 20
		r_dummy,			// 21
		r_dummy,			// 22
/*
* Jax
*/
		r_jax_zap,		// 23
		r_jax_dash,		// 24
		r_dummy,			// 25
		r_dummy,			// 26
		r_dummy,			// 27
/*
* Night Wolf
*/
		r_ind_zap,		// 28
		r_ind_charge,		// 29
		r_axe_horz,		// 2a
		r_axe_up,		// 2b
/*
* Subzero
*/
		r_freeze,		// 2c
		r_slide,			// 2d
		r_dummy,			// 2e
		r_dummy,			// 2f
/*
* Swat guy (Kurtis Striker)
*/
		r_sw_zap,			// 30
		r_swat_bomb,		// 31
		r_zoom,				// 32
		r_stick_sweep,		// 33
		r_swat_gun,			// 34
		r_dummy,			// 35
/*
* Shao Kahn's Queen (Lia)
*/
		r_scream,		// 36
		r_lia_zap,		// 37
		r_dummy,			// 38
		r_dummy,			// 39
		r_dummy,			// 3a
/*
* Robo Ninjas
*/
		r_rocket,			// 3b
		r_net,				// 3c
		r_robo_tele,		// 3d
		r_robo_bomb,		// 3e
		r_smoke_spear,			// 3f
		r_dummy,			// 40
		r_dummy,			// 41
		r_dummy,			// 42
		r_dummy,			// 43
		r_dummy,			// 44
/*
* Kung Lao
*/
		r_hat,			// 45
		r_angle_kick,		// 46
		r_lao_spin,			// 47
		r_dummy,			// 48
		r_dummy,			// 49

/*
* Tusk
*/
		r_tusk_zap,		// 4a
		r_tusk_blur,		// 4b
		r_tusk_elbow,		// 4c
		r_tusk_saw,			// 4d
		r_dummy,
		r_dummy,
/*
* Shang
*/
		r_summon,		// 50
		r_skull,		// 51
		r_dummy,		// 52
		r_dummy,		// 53
		r_dummy,		// 54
/*
* Liu Kang
*/
		r_lk_zap,		// 55
		r_bike_kicked,	// 56
		r_superkang,	// 57
		r_dummy,		// 58
		r_dummy,		// 59
		r_dummy,		// 5a
/*
* Sheeva
*/
		r_pounce,	// 5b
		r_sg_zap,	// 5c
		r_dummy,		// 5d
		r_dummy,		// 5e
		r_dummy,		// 5f

/*
* Motaro
*/
		r_boss_hit1,		// 60
		r_motaro_kick,		// 61

/*
* Shao Kahn
*/
		r_sk_punch,			// 62
		r_sk_hammer,		// 63
		r_sk_charge,		// 64
		r_sk_air_charge,	// 65

/*
* Female ninja (Kitana)
*/
	r_fan,		// 66
	r_fan_lift,	// 67
	r_boomerang,	// 68
	r_jade_prop,	// 69
	r_mileena_tele,	// 6a
	r_mileena_roll,	// 6b
	r_mileena_zap,	// 6c
	r_dummy,		// 6d
	r_dummy,		// 6e
	r_dummy,		// 6f
	r_dummy,		// 70
/*
* Male ninjas (Scorpion/Reptile/Subzero)
*/
	r_orb,			// 71
	r_spit,			// 72
	r_scorp_tele,		// 73
	r_reptile_dash,		// 74
	r_scorpion_spear, 	// 75
	r_floor_ice,		// 76
	r_ermac_zap,		// 77
	r_ermac_slam		// 78
};

JUMPTBL block_xfers [] =
{
	b_weak,				// 0
	b_hard,				// 1
	b_uppercut,			// 2
	b_sweep,	 		// 3
	b_hard_silent,		// 4
	b_weak_silent,		// 5
	b_hard_ken_masters,	// 6
	b_knee_elbow,		// 7
	b_scream,			// 8
	r_robo_bomb, 	  	// 9
	b_duck_hit_soft,   	// a
	b_duck_hit_hard,   	// b
	r_robo_bomb,		// c - you cant block this !!
	wait_forever,   	// d - keep this as "wait_forever"
	r_pounce,			// e - HAMMER SAYS: you cant block this !!
	b_combo,			// f - blocked a combo hit
	b_weak_no_masters,	// 10
	b_punch,			// 11
	b_combo_hard,		// 12
	wait_forever,		// 13
	wait_forever,		// 14
	b_boss_hit1,		// 15
	r_floor_ice,		// 16
	b_lo_punch			// 17
};

//******************************************************************************
//******************************************************************************

void r_ermac_fatal_slam(void) 
{
	center_around_me();

	slammed_shake_up();
	slammed_zoom_up(8);
	slammed_slam_down();
	death_slam_pause();

	slammed_zoom_up(8);
	slammed_slam_down();
	death_slam_pause();
	
	slammed_zoom_up(8);
	slammed_slam_down();
	death_slam_pause();
	
	slammed_zoom_up(8);
	slammed_slam_down();
	death_slam_pause();

	set_inviso(current_proc->pa8);
	create_fx(FX_SWAT_BEXP);
	set_nocol(current_proc->pa8);
	process_sleep(8);
	ground_player(current_proc->pa8);
	wait_forever();
}

void death_slam_pause(void) 
{
	death_scream;
	find_ani_part2(ANIM_KDOWN);
	find_last_frame();
	do_next_a9_frame(current_proc->pa8);
	shake_a11(0x8,0x5);
	tsound(0x81);
	process_sleep(3);
	pose_stumble_frame_1();
	return;
}

void r_ermac_slam(void) 
{
	reaction_start(REACT_START,NULL,0,NULL);

	slammed_shake_up();
	slammed_zoom_up(8);

	slammed_slam_down();
	damage_to_me(9,current_proc->pa8);

	find_ani_part2(ANIM_KDOWN);
	((ADDRESS*)current_proc->pa9)++;
	do_next_a9_frame(current_proc->pa8);
	shake_a11(0x8,0xc);
	dec_my_p_hit;
	rsnd_ground();
	group_sound(2);

	process_sleep(8);

	flight(current_proc->pa8,-SCX(0x38000),-SCY(0x80000),SCY(0x6000),4,ANIM_ORB_BANGED);
	stop_me(current_proc->pa8);
	land_on_my_back();
}

void slammed_shake_up(void) 
{
	pose_stumble_frame_1();

	(current_proc->pa8)->oyvel.pos=-SCY(0x20000);
	shake_a8_up(current_proc->pa8,3,3,2,4);
	return;
}

void pose_stumble_frame_1(void) 
{
	pose_a9(0,ANIM_STUMBLE);
	group_sound(2);
	return;
}

void slammed_zoom_up(WORD pa10) 
{
	(current_proc->pa8)->oyvel.pos=-SCY(0x40000);
	(current_proc->pa8)->ograv.pos=-SCY(0x30000);
	process_sleep(pa10);
	find_ani_part2(ANIM_KDOWN);
	do_next_a9_frame(current_proc->pa8);
	return;
}

void slammed_slam_down(void) 
{
	(current_proc->pa8)->ograv.pos=SCY(0x80000);
	/* reslam3 */
	do
	{
		process_sleep(1);
	}
	while((current_proc->pa8)->oypos.u.intpos<current_proc->pdata.p_ganiy);
	stop_me(current_proc->pa8);
	ground_player(current_proc->pa8);

	return;
}
//******************************************************************************
//******************************************************************************
void r_swat_gun(void) 
{
	reaction_start(REACT_START,NULL,0,NULL);

	at_least_ground_level();

	set_half_damage(current_proc->pa8);
	shake_a11(0x5,0xa);
	away_x_vel(current_proc,current_proc->pa8,SCX(0x30000));
	death_scream;

	find_ani_part2(ANIM_STUMBLE);
	init_anirate(3);

	current_proc->a11=0x30;
	current_proc->a10=6;			// blood freq
	/* swgun */
	do
	{
		process_sleep(1);
		if (--current_proc->a10==0) 
		{
			create_blood_proc(BLOOD_HAT);
			rsnd_stab();
			current_proc->a10=6;
		}

		next_anirate();
	}
	while(--current_proc->a11>0);

	reaction_exit(current_proc->pa8);
}

void at_least_ground_level(void) 
{
	if ((current_proc->pa8)->oypos.u.intpos<current_proc->pdata.p_ganiy)
		(current_proc->pa8)->oypos.u.intpos=current_proc->pdata.p_ganiy;

	return;
}

//******************************************************************************
//******************************************************************************
void r_floor_ice(void) 
{
	OBJECT *obj=current_proc->pa8;
	PROCESS *pa0;

	reaction_start(REACT_START,NULL,0,NULL);
	
	if (am_i_joy()==SYSTEM_CARRY_CLR) 
	{
		if ((pa0=get_his_floor_ice())==NULL)
			reaction_exit(obj);

		if (pa0->a11<=0x20)
			reaction_exit(obj);
	}

	/* rfloor3 */
	current_proc->pdata.p_hitby=ACT_FLOOR_ICE;

	set_no_block(obj);
	ground_player(obj);
	current_proc->pdata.p_action=ACT_SLIPPING;
	find_ani_part2(ANIM_STUMBLE);

	current_proc->pdata.p_store8=(am_i_joy()==SYSTEM_CARRY_SET)?0x50:0x38;

	/* rfloor4 */
	group_sound(8);
	init_anirate(4);

	/* decide initial vel */
	if (abs(obj->oxpos.u.intpos-current_proc->a10)>abs(obj->oxpos.u.intpos-current_proc->a11)) 
		goto RFI6;

RFI1:
	set_x_vel(obj,SCX(0x10000));
	do
	{
		slip_sleep();
	}
	while(abs(obj->oxpos.u.intpos-current_proc->a11)>SCX(0x5));

RFI6:
	set_x_vel(obj,-SCX(0x10000));
	do
	{
		slip_sleep();
	}
	while(abs(obj->oxpos.u.intpos-current_proc->a11)>SCX(0x5));

	goto RFI1;
}

void slip_sleep(void) 
{
	process_sleep(1);
	next_anirate();
	if (--current_proc->pdata.p_store8==0)
		reaction_exit(current_proc->pa8);
	return;
}

PROCESS *get_his_floor_ice(void) 
{
	return(process_exist(((current_proc->pa8)->oid==1)?PID_FLOOR_ICE2:PID_FLOOR_ICE1,0xffff));
}

//******************************************************************************
//******************************************************************************
void r_mileena_roll(void) 
{
	current_proc->pdata.p_action=ACT_MILEENA_ROLL;

	reaction_start(REACT_START,NULL,0,NULL);
	rsnd_react_voice();
	set_half_damage(current_proc->pa8);

	rsnd_big_smack();

	flight(current_proc->pa8,-SCX(0x38000),-SCY(0x80000),SCY(0x6000),4,ANIM_ORB_BANGED);
	land_on_my_back();
}

//******************************************************************************
//******************************************************************************
void r_mileena_tele(void) 
{
	current_proc->pdata.p_hitby=ACT_MILEENA_TELE;
	rsnd_big_smack();

	reaction_start(REACT_START,generic_airborn_hit,0,NULL);
	rsnd_react_voice();
	shake_a11(4,4);
	stumble_back;
}

//******************************************************************************
//******************************************************************************
void r_reptile_dash(void) 
{
	if (am_i_airborn(current_proc->pa8)==SYSTEM_CARRY_SET)
		reaction_exit(current_proc->pa8);

	reaction_start(REACT_START,NULL,0,NULL);
	stance_setup();

	current_proc->a10=20;
	/* rrd3 */
	do
	{
		process_sleep(1);
		next_anirate();
	}
	while(--current_proc->a10>0);
	
	reaction_exit(current_proc->pa8);
}

//******************************************************************************
//******************************************************************************
void r_scorp_tele(void) 
{
	rsnd_big_smack();
	set_half_damage(current_proc->pa8);

	reaction_start(REACT_START,generic_airborn_hit,0,NULL);
	shake_a11(3,3);

	group_sound(2);
	stumble_back;
}

//******************************************************************************
//******************************************************************************
void r_spit(void) 
{
	current_proc->pdata.p_hitby=ACT_SPIT_ON;

	his_ochar_sound(0x10);
	reaction_start(REACT_START,generic_airborn_hit,0,NULL);

	group_sound(2);
	stumble_back;
}

//******************************************************************************
//******************************************************************************
void r_orb(void) 
{
	current_proc->pdata.p_hitby=ACT_ORBBED;

	reaction_start(REACT_START,NULL,0,NULL);
	rsnd_react_voice();

	set_half_damage(current_proc->pa8);
	flight(current_proc->pa8,-SCX(0x38000),-SCY(0x80000),SCY(0x6000),4,ANIM_ORB_BANGED);
	stop_me(current_proc->pa8);
	land_on_my_back();
}

//******************************************************************************
//******************************************************************************
void r_boomerang(void) 
{
	shake_a11(4,6);

	group_sound(2);
	his_ochar_sound(5);
	create_blood_proc(BLOOD_BOOMER);
	reaction_start(REACT_START,airborn_hit_no_sound,0,NULL);

	(LONG)current_proc->a0=SCX(0x40000);
	stumble_back_vel();
}

//******************************************************************************
//******************************************************************************
void r_fan_lift(void) 
{
	OBJECT *obj=current_proc->pa8;
	short ta9=4;

	reaction_start(REACT_START,NULL,0,NULL);

	do_first_a9_frame(ANIM_STUMBLE);

	current_proc->pdata.p_action=ACT_WAVE_SD;
	set_half_damage(current_proc->pa8);

	(current_proc->pa8)->oyvel.pos=(distance_from_ground(current_proc->pa8)>SCY(0xa8))?SCY(0x0):-SCY(0x10000);

	shake_a8_up(current_proc->pa8,4,0,3,8);

	/* float up/away */
	obj->oyvel.pos=( distance_from_ground(current_proc->pa8) >SCY(0xf0) )  ? SCY(0x0) : -SCY(0x10000);

	set_x_vel(current_proc->pa8,(is_he_flipped()==SYSTEM_CARRY_SET)?-SCX(0x20000):SCX(0x20000));

	current_proc->a10=3;
	current_proc->a11=0x40;
	
	/* rwave7 */
	do
	{
		process_sleep(1);
		if (--current_proc->a10==0) 
		{
			current_proc->a10=3;
			ta9=-ta9;
			multi_adjust_xy(current_proc->pa8,ta9,ta9);	// shake while floating
		}

		/* rwave9 */
		if (distance_from_ground(current_proc->pa8)>SCY(0xf0))
			(current_proc->pa8)->oyvel.pos=0;
	}
	while(--current_proc->a11>0);
	
	reaction_exit(current_proc->pa8);
}

WORD is_he_flipped(void) 
{
	if ((current_proc->pdata.p_otherguy)->oflags & M_FLIPH)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

//******************************************************************************
//******************************************************************************
void r_fan(void) 
{
	reaction_start(REACT_START,NULL,if_shao_then_pass(1),NULL);

	set_half_damage(current_proc->pa8);

	group_sound(2);
	his_ochar_sound(1);

	find_ani_part2(ANIM_STUMBLE);
	away_x_vel(current_proc,current_proc->pa8,SCX(0x30000));

	init_anirate(6);

	current_proc->a10=0x24;
	/* rhat_wake */
	do
	{
		current_proc->p_comp_flag=PC_RHAT_WAKE;
		process_sleep(1);
		current_proc->p_comp_flag=PC_CLEAR;

		/* rhat_wake */
		if (current_proc->pdata.p_anicount==1)
			create_blood_proc(BLOOD_HAT);

		/* rhat5 */
		next_anirate();
	}
	while(--current_proc->a10>0);

	reaction_exit(current_proc->pa8);
}

//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************



/******************************************************************************
 Function: void r_dummy(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	dummy react
******************************************************************************/
void r_dummy(void)
{
	wait_forever();
}

/******************************************************************************
 Function: void r_null_speared(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	spears cancel
******************************************************************************/
void r_null_speared(void)
{
	reaction_start(REACT_START,NULL,0,NULL);
	current_proc->a0=SCX(0x40000);
	stumble_back_vel();
}

/******************************************************************************
 Function: void r_spear(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	smoke spear
******************************************************************************/
void r_scorpion_spear(void) 
{
	spear0(0x11);
}

void r_smoke_spear(void) 
{
	spear0(0x1f);
}

void spear0(WORD pa0)
{
	his_ochar_sound(pa0);
	
	current_proc->pdata.p_hitby=ACT_SPEAR;

	reaction_start(REACT_START,NULL,0,NULL);
	group_sound(2);
	create_blood_proc(BLOOD_SPEAR);

	/* rspear4 */
	do
	{
		process_sleep(2);
	}
	while((current_proc->pdata.p_otherproc)->pdata.p_action==ACT_ROPE_PULL);

	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void r_tusk_saw(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	react to tusk saw blade
******************************************************************************/
void r_tusk_saw(void)
{
	reaction_start(REACT_START,NULL,0,NULL);
	create_blood_proc(BLOOD_SAW_BLOOD);

	group_sound(2);

	find_ani_part2(ANIM_STUMBLE);

	shake_a11(0x3,0xc);
	animate_a0_frames(0x4,0xd);
	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void r_zoom(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	swat zoom react
******************************************************************************/
void r_zoom(void)
{
	reaction_start(REACT_START,NULL,1,NULL);
	stack_jump(suspend_wait_action);
}

/******************************************************************************
 Function: void r_zoom_flipped(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	swat zoom flipped react
******************************************************************************/
void r_zoom_flipped(void)
{
	flight(current_proc->pa8,-SCX(0x80000),-SCY(0x60000),SCY(0x8000),4,-1);
	current_proc->a10=(current_proc->pdata.p_hit<=3)?ZOOM_DAMAGE:ZOOM_DAMAGE>>2;
	damage_to_me(current_proc->a10,current_proc->pa8);
	current_proc->pdata.p_damage+=current_proc->a10;
	land_on_my_back();
}

/******************************************************************************
 Function: void r_net(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	react to lia scream
******************************************************************************/
void r_net(void)
{
	OBJECT *obj=current_proc->pa8;
	short loop;
	long vel;

	current_proc->pdata.p_hitby=ACT_ROBO_NET;

	reaction_start(REACT_START,NULL,if_shao_then_pass(1),NULL);
	dec_my_p_hit;

	group_sound(8);

	clear_noflip(obj);
	set_no_block(obj);

	current_proc->pdata.p_action=ACT_CAUGHT_NET;

	find_ani_part2(ANIM_STUMBLE);
	init_anirate(4);

	vel=SCX(0x30000);

	loop=0x50;
	/* net4 */
	do
	{
		vel-=SCX(0x2000);				// sub friction value

		away_x_vel(current_proc,obj,vel);		// start moving away

		net_sleep();

		if (loop<0x28) 						// dont check distance so soon
		{
			if (get_x_dist<=(SCX(0x46)-4))			// close --> stop
				loop=1;
		}
		/* nett3 */
	}
	while(--loop>0);

	/* still */
	stop_me(obj);

	loop=0x20;
	do
	{
		net_sleep();
	}
	while(--loop>0);

	if (am_i_airborn(obj)==SYSTEM_CARRY_SET)
		flight(obj,0,0,SCY(0x8000),5,ANIM_KDOWN);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void net_sleep(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	sleep and animate for net
******************************************************************************/
void net_sleep(void)
{
	current_proc->p_comp_flag=PC_NET_WAKE;		// mod from assemb, code emulate address flag
	process_sleep(1);
	current_proc->p_comp_flag=PC_CLEAR;
	next_anirate();

	return;
}

/******************************************************************************
 Function: WORD if_shao_then_pass(WORD pa6)

 By: David Schwartz

 Date: May 1995

 Parameters: pa6 - a6 value

 Returns: correct a6 value

 Description:	shao kahn treated like every other dude
******************************************************************************/
WORD if_shao_then_pass(WORD pa6)
{
	if ((current_proc->pa8)->ochar==FT_SK)
		return(0);
	else
	{
		return(pa6);
	}
}

/******************************************************************************
 Function: void r_slide(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	react to sz slide
******************************************************************************/
void r_slide(void)
{
	OBJECT *obj=current_proc->pa8;

	reaction_start(REACT_START,NULL,7,NULL);

	group_sound(5);				// tripped voice
	away_x_vel(current_proc,obj,SCX(0x30000));
	current_proc->a6=0;
	flight(obj,SCX(0x38000),-SCY(0x60000),SCY(0x6000),4,ANIM_KDOWN);
	land_on_my_back();
}

/******************************************************************************
 Function: void r_pounce2(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	react to pounce
******************************************************************************/
void r_pounce2(void)
{
	OBJECT *obj=current_proc->pa8;

	group_sound(2);				// group speech: wasted

	current_proc->a11=3;

	/* pounce1 */
	do
	{
		find_ani_part2(ANIM_KDOWN);
		current_proc->a10=(ADDRESS)current_proc->pa9;
		((ADDRESS *)current_proc->a10)++;			// toggle frame 2
		((ADDRESS *)current_proc->pa9)-=2;			// toggle frame 1
		do_next_a9_frame(obj);
		ground_multi(obj);
		process_sleep(3);
		ground_ochar();
		current_proc->pa9=(void *)current_proc->a10;
		do_next_a9_frame(obj);
		process_sleep(3);
	}
	while(--current_proc->a11>0);

	/* pounce4, copy */
	if (get_his_action(current_proc)==ACT_SG_POUNCE)
		suspend_wait_action_jsrp();
	getup_reaction_exit;
}

/******************************************************************************
 Function: void r_pounce(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	react to pounce
******************************************************************************/
void r_pounce(void)
{
	reaction_start(REACT_START,NULL,if_shao_then_pass(1),NULL);

	current_proc->pdata.p_action=ACT_POUNCE;

	group_sound(2);

	get_char_ani(ANIM_TABLE1,ANIM_KDOWN);
	mframew(2);
	mframew(2);

	/* pounce4, if mod check rpounce2 */
	if (get_his_action(current_proc)==ACT_SG_POUNCE)
		suspend_wait_action_jsrp();
	getup_reaction_exit;
}

/******************************************************************************
 Function: void r_superkang(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	react to lk superkang
******************************************************************************/
void r_superkang(void)
{
	OBJECT *obj=current_proc->pa8;

	reaction_start(REACT_START,NULL,1,NULL);

	rsnd_med_smack();
	group_sound(2);
	shake_a11(0x0005,0x0005);

	flight(obj,SCX(0x60000),-SCY(0x30000),SCY(0x4000),5,ANIM_KDOWN);
	land_on_my_back();
}

/******************************************************************************
 Function: void r_bike_kicked(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	react to lk bike
******************************************************************************/
void r_bike_kicked(void)
{
	process_sleep(0x80);
	stack_jump(r_bike_kicked_done);
}

/******************************************************************************
 Function: void r_bike_kicked_done(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	finish react to lk bike
******************************************************************************/
void r_bike_kicked_done(void)
{
	if (am_i_airborn(current_proc->pa8)==SYSTEM_CARRY_CLR)
	{
		stumble_back;
	}

	fall_on_my_back();
}

/******************************************************************************
 Function: void r_summon(void)

 By: David Schwartz

 Date: feb 1995

 Parameters: None

 Returns: None

 Description:	react to st summons
******************************************************************************/
void r_summon(void)
{
	OBJECT *obj=current_proc->pa8;

	reaction_start(REACT_START,NULL,1,NULL);
	group_sound(2);

	face_opponent(obj);
	flip_multi(obj);

	flight(obj,-SCX(0x30000),-SCY(0x90000),SCY(0x6000),5,ANIM_KDOWN);
	reaction_land();
}

/******************************************************************************
 Function: void r_scream(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	react to lia scream
******************************************************************************/
void r_scream(void)
{
	OBJECT *obj=current_proc->pa8;
	OBJECT *ta5=current_proc->pdata.p_otherguy;

	reaction_start(REACT_START,NULL,0,NULL);		// boss = react like normal dude
	set_no_block(obj);

	current_proc->pdata.p_action=ACT_SCREAMED;
	dec_my_p_hit;				//  scream does not = hit

	current_proc->pdata.p_store1=0;			// flag: not stopped yet
	stop_me(obj);

	pose_a9(0,ANIM_STUMBLE);			// pose: stumble

	/* avoid getting caught behind SINDEL */
	if (ta5->oflags & M_FLIPH)
	{
		current_proc->a6=ta5->oxpos.u.intpos;
		current_proc->a7=obj->oxpos.u.intpos;
	}
	else
	{
		current_proc->a7=ta5->oxpos.u.intpos;
		current_proc->a6=obj->oxpos.u.intpos;
	}

	if (current_proc->a6<=current_proc->a7)
	{
		current_proc->a10=obj->oypos.u.intpos;
		lineup_a0_onto_a1(obj,ta5);
		flip_multi(obj);
		multi_adjust_xy(obj,-SCX(0x20),0);
		obj->oypos.u.intpos=current_proc->a10;
	}

	/* scream1 */
	if (am_i_airborn(obj)==SYSTEM_CARRY_SET)
		obj->oyvel.pos=SCY(0x10000);

	/* scream2 */
	(short)current_proc->a11=4;
	current_proc->a10=(0x40*2)/3;

	/* scream3 */
	do
	{
		process_sleep(3);

		if (is_he_airborn()==SYSTEM_CARRY_SET || get_his_action(current_proc)==ACT_SCREAMED)
		{
			/* if she jumped she screwed up */
			/* scream_abort */
			stop_me(obj);
			ground_player(obj);
			reaction_exit(obj);
		}

		obj->oxpos.u.intpos+=(short)current_proc->a11;
		(short)current_proc->a11=-(short)current_proc->a11;

		if (obj->oypos.u.intpos>=current_proc->pdata.p_ganiy)
		{
			obj->oyvel.pos=0;
			ground_player(obj);				// ground him

			if (get_x_dist<=SCX(0x50))
			{
				current_proc->pdata.p_store1=1;			// falg: stopped !!!!
				stop_me(obj);
			}
			else
			{
				/* scream4 */
				if (current_proc->pdata.p_store1==0)
					towards_x_vel(current_proc,obj,SCX(0x20000));
			}
		}

	/* scream5 */
	}
	while(--current_proc->a10>0);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void r_jax_dash(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to jax dash
******************************************************************************/
void r_jax_dash(void)
{
	reaction_start(REACT_START,NULL,2,NULL);

	group_sound(2);
	rsnd_big_smack();
	shake_a11(0x0006,0x0006);

	flight(current_proc->pa8,SCX(0x80000),-SCY(0x60000),SCY(0x6000),5,ANIM_KDOWN);

	reaction_land();
}

/******************************************************************************
 Function: void r_axe_up(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to axe up
******************************************************************************/
void r_axe_up(void)
{
	create_blood_proc(BLOOD_UPPERCUT);
	his_ochar_sound(9);			// axe hit sound
	set_half_damage(current_proc->pa8);

	reaction_start(REACT_START,NULL,1,cc_ken_masters);

	group_sound(2);

	flight(current_proc->pa8,SCX(0x10000),-SCY(0x80000),SCY(0x6000),5,ANIM_KDOWN);

	reaction_land();
}

/******************************************************************************
 Function: void r_hi_kick(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to hikick
******************************************************************************/
void r_hi_kick(void)
{
	OBJECT *obj=current_proc->pa8;

	create_blood_proc(BLOOD_FACE_PUNCH);

	group_sound(2);
	rsnd_big_smack();

	reaction_start(REACT_START,generic_airborn_hit,1,NULL);				// initial reaction setup

	away_x_vel(current_proc,obj,SCX(0x48000));				// head away from otherguy

	get_char_ani(ANIM_TABLE1,ANIM_HITHI);
	do_next_a9_frame(obj);
	process_sleep(2);
	do_next_a9_frame(obj);
	process_sleep(6);

	current_proc->a10=10;
	/* damping loop */
	do
	{
		process_sleep(1);
		away_x_vel(current_proc,obj,abs(obj->oxvel.pos)-(abs(obj->oxvel.pos)>>6));
	}
	while(--current_proc->a10>0);

	stop_me(obj);

	get_char_ani(ANIM_TABLE1,ANIM_HITHI);
	((ADDRESS *)current_proc->pa9)++;
	mframew(0x0004);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void r_lo_kick(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to lokick
******************************************************************************/
void r_lo_kick(void)
{
	rsnd_body_hit();
	rsnd_react_voice();

	reaction_start(REACT_START,generic_airborn_hit,1,NULL);				// initial reaction setup

	//current_proc->pdata.p_action=ACT_REACTING;

	away_x_vel(current_proc,current_proc->pa8,SCX(0x40000));							// head away from otherguy

	if (am_i_short()==SYSTEM_CARRY_SET)
		animate_a9(0x0004,ANIM_DUCKHIT);
	else animate_a9(0x0004,ANIM_HITLO);

	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void r_duck_punch(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to duck punched
******************************************************************************/
void r_duck_punch(void)
{
//REV21	reaction_start(REACT_START,generic_airborn_hit,1,cc_ken_masters);
	reaction_start(REACT_START,r_duck_airpunch,1,cc_ken_masters);

	rsnd_smack();
	rsnd_react_voice();

	away_x_vel(current_proc,current_proc->pa8,SCX(0x40000));

	if (get_my_height()<SCY(0x80))
		animate_a9(0x0003,ANIM_DUCKHIT);
	else animate_a9(0x0003,ANIM_HITLO);


	stop_me(current_proc->pa8);

	if (am_i_joy()==SYSTEM_CARRY_SET)
	{
		reaction_exit(current_proc->pa8);
	}
	else
	{
		stack_jump(drone_post_duck_hit);
	}
}

void r_duck_airpunch(void)
{
	rsnd_med_smack();
	rsnd_react_voice();

	avoid_corner_trap(3);

	flight(current_proc->pa8,SCX(0x38000),-SCY(0x60000),SCY(0x8000),5,ANIM_KDOWN);
	land_on_my_back();
}
/******************************************************************************
 Function: void r_duck_kickh(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to duck hi kick
******************************************************************************/
void r_duck_kickh(void)
{
	reaction_start(REACT_START,generic_airborn_hit,1,cc_ken_masters);

	rsnd_smack();
	rsnd_react_voice();

	away_x_vel(current_proc,current_proc->pa8,SCX(0x48000));

	if (get_my_height()>SCY(0x80))
		animate_a9(0x0003,ANIM_HITLO);
	else animate_a9(0x0003,ANIM_DUCKHIT);

	stop_me(current_proc->pa8);

	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void r_duck_kickl(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to duck low kick
******************************************************************************/
void r_duck_kickl(void)
{
	reaction_start(REACT_START,r_airborn_duck_kick,1,cc_ken_masters);

	rsnd_smack();
	rsnd_react_voice();

	away_x_vel(current_proc,current_proc->pa8,SCX(0x30000));

	if (get_my_height()>SCY(0x80))
		animate_a9(3,ANIM_HITLO);
	else animate_a9(3,ANIM_DUCKHIT);

	stop_me(current_proc->pa8);

	if (am_i_joy()==SYSTEM_CARRY_SET)
	{
		reaction_exit(current_proc->pa8);
	}
	else
	{
		stack_jump(drone_post_duck_hit);
	}
}

/******************************************************************************
 Function: void r_robo_tele(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	react to robo tele
******************************************************************************/
void r_robo_tele(void)
{
	PROCESS *ta13;

	group_sound(2);				// group speech: wasted
	rsnd_big_smack();
	shake_a11(0x0006,0x0006);

	/* calla_for_him */
	ta13=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	zero_turbo_bar();
	current_proc=ta13;

	current_proc->pdata.p_hitby=ACT_ROBO_TELE;

	reaction_start(REACT_START,NULL,4,NULL);

	current_proc->pdata.p_action=ACT_ROBO_TELED;

	flight(current_proc->pa8,SCX(0x10000),-SCY(0xb0000),SCY(0x6000),5,ANIM_KDOWN);

	reaction_land();
}

/******************************************************************************
 Function: void r_robo_bomb(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	react to robo bomb
******************************************************************************/
void r_robo_bomb(void)
{
	reaction_start(REACT_START,NULL,1,NULL);

	group_sound(2);				// group speech: wasted
	rsnd_big_smack();

	shake_a11(0x0006,0x0006);

	face_opponent(current_proc->pa8);
	flip_multi(current_proc->pa8);

	flight(current_proc->pa8,-SCX(0x30000),-SCY(0xc0000),SCY(0x6000),5,ANIM_KDOWN);

	reaction_land();
}

/******************************************************************************
 Function: void r_combo6(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	react to combo 6
******************************************************************************/
void r_combo6(void)
{
	set_half_damage(current_proc->pa8);
	reaction_start(REACT_START,NULL,4,cc_ken_masters); // 4= boss stumble

	create_blood_proc(BLOOD_UPPERCUT);
	shake_a11(0x0006,0x0006);
	rsnd_big_smack();
	group_sound(2);

	create_fx(FX_COMMENT);
	flight(current_proc->pa8,SCX(0x10000),-SCY(0xc0000),SCY(0x6000),5,ANIM_KDOWN);

	reaction_land();
}

/******************************************************************************
 Function: void r_combo5(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	react to combo 5
******************************************************************************/
void r_combo5(void)
{
	if ((current_proc->pa8)->ochar==FT_LK)
		set_half_damage(current_proc->pa8);
	else set_quarter_damage(current_proc->pa8);

	reaction_start(REACT_START,NULL,4,(current_proc->a10==0xedb00)?NULL:cc_ken_masters);

	create_blood_proc(BLOOD_UPPERCUT);
	shake_a11(6,6);
	rsnd_big_smack();
	group_sound(2);

	/* rup3 */
	create_fx(FX_COMMENT);

	flight(current_proc->pa8,SCX(0x20000),-SCY(0xc0000),SCY(0x6000),5,ANIM_KDOWN);
	reaction_land();
}

void r_lao_spin(void) 
{
 	reaction_start(REACT_START,NULL,4,NULL);
	set_half_damage(current_proc->pa8);

	current_proc->pdata.p_action=ACT_LAO_SPINNED;

	create_blood_proc(BLOOD_UPPERCUT);
	shake_a11(6,6);
	rsnd_big_smack();
	group_sound(2);
	flight(current_proc->pa8,SCX(0x20000),-SCY(0xc0000),SCY(0x6000),5,ANIM_KDOWN);
	reaction_land();
}

/******************************************************************************
 Function: void r_uppercut(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to upper cut
******************************************************************************/
extern void *street_module[];
extern void *tower_module[];
extern void *cave_module[];
extern void *roof_module[];

extern void *dlists_subway_to_street[];
extern void *dlists_bank_to_roof[];
extern void *dlists_soul_to_tower[];
extern void *dlists_hell_to_cave[];

void r_uppercut(void)
{
	if (current_proc->a10==0xedb00)
		reaction_start(REACT_START,NULL,4,NULL);
 	else reaction_start(REACT_START,NULL,4,cc_ken_masters);

	create_blood_proc(BLOOD_UPPERCUT);
	shake_a11(0x0006,0x0006);
	rsnd_big_smack();

	if (current_proc->a10==0xedb00)
	{
		/* background_death */
		gstate=GS_PITFALL;				// new game state

		psxspu_start_cd_fade(FADE_TIME,0);

		if (curback==BKGD_PIT_MOD)
			fall_down_pit();

		if (curback==BKGD_BELL_MOD)
			fall_down_bell_tower();

		if (curback==BKGD_SUBWAY_MOD)
			fall_on_trax();

		if (curback==BKGD_HADES_MOD)
			fall_in_lava();

	}

	/* pit_abort */
	group_sound(2);

	if (gstate!=GS_AMODE && get_my_strength(current_proc->pa8)!=0 && process_exist(PID_DARK,-1)==NULL) 	// no blast in dark
	{
		if (process_exist(PID_REGENERATE,-1)==NULL && process_exist(PID_SWITCHEROO,-1)==NULL)			// only blast if not regenerate or switcheroo
		{
			if (curback==BKGD_SUBWAY_MOD)
				blast_entry(0xe4,street_module,dlists_subway_to_street,1);		// blast_through_subway

			if (curback==BKGD_BANK_MOD)
				blast_entry(SCY(0x125),roof_module,dlists_bank_to_roof,3);			// blast_through_bank

			if (curback==BKGD_SOUL_MOD)
				blast_entry(SCY(0x125),tower_module,dlists_soul_to_tower,4);				// blast_through_soul

		if (curback==BKGD_HADES_MOD)
			blast_entry(SCY(0x125),cave_module,dlists_hell_to_cave,0xd);				// blast_through_soul

		}
	}

	/* rup3 */
	create_fx(FX_COMMENT);

	flight(current_proc->pa8,SCX(0x20000),-SCY(0xc0000),SCY(0x6000),5,ANIM_KDOWN);
	reaction_land();
}


/******************************************************************************
 Function: void r_tusk_elbow(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	react to tusk elbow
******************************************************************************/
void r_tusk_elbow(void)
{
	create_blood_proc(BLOOD_COMBO);
	create_blood_proc(BLOOD_COMBO);
	rsnd_stab();
	rek3();
}

/******************************************************************************
 Function: void r_elbow_knee(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to elbow
******************************************************************************/
void r_elbow_knee(void)
{
	rsnd_med_smack();
	rek3();
}

void rek3(void)
{
	create_blood_proc(BLOOD_COMBO);

	rsnd_react_voice();
	shake_a11(0x0004,0x0004);

	reaction_start(REACT_START,generic_airborn_hit,1,NULL);
	set_no_block(current_proc->pa8);

	away_x_vel(current_proc,current_proc->pa8,SCX(0x10000));

	animate_a9(0x0004,ANIM_HITHI);

	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void r_flip_punch(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	react to flip punch
******************************************************************************/
void r_flip_punch(void)
{
	rsnd_react_voice();
	shake_a11(0x0004,0x0004);

	reaction_start(REACT_START,generic_airborn_hit,1,NULL);
	rsnd_med_smack();
	set_no_block(current_proc->pa8);

	away_x_vel(current_proc,current_proc->pa8,SCX(0x10000));

	animate_a9(0x0004,ANIM_HITHI);

	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void r_stick_sweep(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	react to stick sweep kick
******************************************************************************/
void r_stick_sweep(void)
{
	reaction_start(REACT_START,NULL,1,NULL);

	rsnd_body_hit();
	group_sound(6);

	ground_player(current_proc->pa8);

	stack_jump(sweep3);
}

/******************************************************************************
 Function: void r_sweep(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to sweep kick
******************************************************************************/
void r_sweep(void)
{
	reaction_start(REACT_START,generic_airborn_hit,1,NULL);

	rsnd_body_hit();
	group_sound(5);					// group speech:

	stack_jump(sweep3);
}

/******************************************************************************
 Function: void sweep3(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	common part of sweep kick react
******************************************************************************/
void sweep3(void)
{
	animate_a9(0x0005,ANIM_SWEEPFALL);

	shake_n_sound();

	if (am_i_joy()==SYSTEM_CARRY_CLR)
	{
		/* drone code */
		d_beware_mframew(0x0004);			// ani: standing unblock

		current_proc->pdata.p_otheract=0;

		d_beware();
		process_sleep(1);
		d_beware();
		process_sleep(1);
		d_beware();
		process_sleep(1);
	}
	else
	{
		/* joy_swept3 */
		mframew(0x0004);								// finish fall ani
	}

	check_stay_down();
	sweepup_local_reaction_exit;
}

/******************************************************************************
 Function: void r_flip_kick(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to flip kick
******************************************************************************/
void r_flip_kick(void)
{
	rsnd_med_smack();
	reaction_start(REACT_START,generic_airborn_hit,if_shao_then_pass(8),NULL);
	set_half_damage(current_proc->pa8);
	current_proc->pdata.p_action=ACT_REACT_FLIPK;
	onback3();
}

/******************************************************************************
 Function: void r_last_noogy(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	react to noogy
******************************************************************************/
void r_last_noogy(void)
{
	rsnd_med_smack();
	reaction_start(REACT_START,generic_airborn_hit,1,NULL);
	onback3();
}

/******************************************************************************
 Function: void onback3(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	be on back from early reaction
******************************************************************************/
void onback3(void)
{
	rsnd_react_voice();
	create_blood_proc(BLOOD_FACE2);

	flight(current_proc->pa8,SCX(0x30000),-SCY(0x80000),SCY(0x8000),5,ANIM_KDOWN);

	shake_n_sound();
	find_ani_part2(ANIM_KDOWN);

	if (am_i_joy()==SYSTEM_CARRY_SET)
	{
		mframew(5);
		process_sleep(4);
	}
	else
	{
		d_beware_mframew(3);
		d_beware();
		process_sleep(1);
		d_beware();
		process_sleep(1);
		d_beware();
		process_sleep(1);
	}

	getup_reaction_exit;
}

/******************************************************************************
 Function: void r_hi_punch(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to hipunch
******************************************************************************/
void r_hi_punch(void)
{
	inc_p_block;

	reaction_start(REACT_START,r_airpunch,1,cc_hi_punch);

	rsnd_react_voice();
	rsnd_smack();
	dec_my_p_hit;

	create_blood_proc(BLOOD_FACE_PUNCH);

	current_proc->pdata.p_action=ACT_PUNCHED;

	animate_a9(0x0003,ANIM_HITHI);

	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void r_lo_punch(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to lopunch
******************************************************************************/
void r_lo_punch(void)
{
	inc_p_block;

	reaction_start(REACT_START,r_airpunch,1,cc_lo_punch);

	rsnd_react_voice();
	rsnd_body_hit();
	dec_my_p_hit;

	current_proc->pdata.p_action=ACT_PUNCHED;

	if (am_i_short()==SYSTEM_CARRY_CLR)
		animate_a9(0x0003,ANIM_HITLO);						// ani: hit in chest
	else animate_a9(0x0003,ANIM_DUCKHIT);					// ani: duck hit (guy short)

	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void r_airpunch(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to airpunch
******************************************************************************/
void r_airpunch(void)
{
	PROCESS *pa0;

	rsnd_med_smack();
	rsnd_react_voice();

	current_proc->pdata.p_action=ACT_AIRPUNCHED;

	/* calla_for_him */
	pa0=current_proc;		// save current proc
	current_proc=current_proc->pdata.p_otherproc;	// no run/jab run
	zero_turbo_bar();
	current_proc=pa0;		// restore current Proc

	avoid_corner_trap(2);				// # of hits to trigger it

	flight(current_proc->pa8,SCX(0x30000),-SCY(0x60000),SCY(0x7000),0x5,ANIM_KDOWN);

	land_on_my_back();
}

/******************************************************************************
 Function: void avoid_corner_trap(WORD pa1)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa1 - # of hits which triggers repell

 Returns: None

 Description:	avoid caught in corner trap, prevent multi hits in corner after thrshold
******************************************************************************/
void avoid_corner_trap(WORD pa1)
{
	if (am_i_close_to_edge(current_proc)==SYSTEM_CARRY_SET && current_proc->pdata.p_hit>=pa1)
	{
		xfer_otherguy(ken_masters_xfer);
	}

	return;
}

//******************************************************************************
//******************************************************************************
void r_jade_prop(void) 
{
	reaction_start(REACT_START,NULL,3,NULL);

	shake_a11(6,6);
	group_sound(2);
	rsnd_big_smack();
	
	flight(current_proc->pa8,SCX(0x60000),-SCY(0x40000),SCY(0x6000),5,ANIM_KDOWN);
	land_on_my_back();
}

/******************************************************************************
 Function: void r_ind_charge(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: react to indian charge
******************************************************************************/
void r_ind_charge(void)
{
	reaction_start(REACT_START,NULL,3,NULL);

	shake_a11(0x0006,0x0006);
	group_sound(2);
	rsnd_big_smack();

	flight(current_proc->pa8,SCX(0x40000),-SCY(0x30000),SCY(0x6000),5,ANIM_KDOWN);
	land_on_my_back();
}

/******************************************************************************
 Function: void r_kano_roll(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to kano roll
******************************************************************************/
void r_kano_roll(void)
{
	reaction_start(REACT_START,NULL,3,NULL);

	shake_a11(0x0006,0x0006);

	group_sound(2);					// group speech: wasted
	rsnd_big_smack();

	flight(current_proc->pa8,SCX(0x40000),-SCY(0x40000),SCY(0x4000),0x5,ANIM_KDOWN);

	land_on_my_back();
}

/******************************************************************************
 Function: void r_roundhouse(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to round house
******************************************************************************/
void r_roundhouse(void)
{
	create_blood_proc(BLOOD_ROUNDHOUSE);

	group_sound(2);					// group speech: wasted
	rsnd_big_smack();
	shake_a11(0x0006,0x0006);

	reaction_start(REACT_START,NULL,1,cc_ken_masters);

	flight(current_proc->pa8,SCX(0x60000),-SCY(0x80000),SCY(0x8000),5,ANIM_KDOWN);

	land_on_my_back();
}

/******************************************************************************
 Function: void r_square(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to square ???
******************************************************************************/
void r_square(void)
{
	group_sound(2);					// group speech: wasted
	rsnd_big_smack();

	shake_a11(0x0008,0x0008);

	reaction_start(REACT_START,NULL,1,NULL);

	flight(current_proc->pa8,SCX(0x50000),-SCY(0x50000),SCY(0x8000),5,ANIM_KDOWN);

	land_on_my_back();
}

/******************************************************************************
 Function: void r_kano_swipe(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	react to kano swipe
******************************************************************************/
void r_kano_swipe(void)
{
	group_sound(2);					// group speech: wasted
	tsound(0xf);

	create_blood_proc(BLOOD_UPPERCUT);

	reaction_start(REACT_START,airborn_hit_no_sound,1,NULL);

	pose_a9(0,ANIM_STUMBLE);

	away_x_vel(current_proc,current_proc->pa8,SCX(0x60000));
	process_sleep(8);

	(LONG)current_proc->a0=SCX(0x40000);
	stumble_back_vel();
}


FUNC a10_r_table[]=
{
	r_post_shake,
	r_post_bike
};


/******************************************************************************
 Function: void r_a10_offset(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: current_proc->a10

 Returns: None

 Description:	call correct call routine based on offset
******************************************************************************/
void r_a10_offset(void)
{
	a10_r_table[current_proc->a10]();
};

/******************************************************************************
 Function: inline void cc_hi_punch(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	corner check hi punch
******************************************************************************/
inline void cc_hi_punch(void)
{
	ccp3(6,4);
}

/******************************************************************************
 Function: inline void cc_lo_punch(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	corner check lo punch
******************************************************************************/
inline void cc_lo_punch(void)
{
	ccp3(5,3);
}

/******************************************************************************
 Function: void ccp3(WORD hold,WORD pull_away)

 By: David Schwartz

 Date: Nov 1994

 Parameters: hold - # of hits if stick is not pulled away (a5)
			 pull_away - # of hits if stick is pulled away (a6)

 Returns: None

 Description:	check to see if player should stumble when hit
******************************************************************************/
void ccp3(WORD hold,WORD pull_away)
{
	if (am_i_joy()==SYSTEM_CARRY_SET && is_stick_away(current_proc)==SYSTEM_CARRY_CLR)
		pull_away=hold;

	/* stck4 */
	if (pull_away<=current_proc->pdata.p_block)
	{
		/* separate_us */
		if (am_i_close_to_edge(current_proc)==SYSTEM_CARRY_CLR)
		{
			(LONG)current_proc->a0=SCX(0x30000);
			stumble_back_vel();
		}
		else
		{
			takeover_him(ken_masters_xfer);
			block_exit();
		}
	}

	return;
}

/******************************************************************************
 Function: inline void cc_block_sweep(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	corner check block sweep
******************************************************************************/
void cc_block_sweep(void)
{
	repell_one_of_us(0x30000,0x30000,NULL);
}

/******************************************************************************
 Function: inline cc_block_avoid_corner(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	corner check avoid
******************************************************************************/
inline void cc_block_avoid_corner(void)
{
	avoid_corner_trap_b(1);

	return;
}

/******************************************************************************
 Function: inline void cc_ken_masters(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	corner check ken masters
******************************************************************************/
inline void cc_ken_masters(void)
{
	avoid_corner_trap(1);

	return;
}

inline void cc_block_upcut(void) 
{
	avoid_corner_trap(0);

	return;
}

/******************************************************************************
 Function: void ken_masters_xfer(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	corner check ken masters
******************************************************************************/
void ken_masters_xfer(void)
{
	OBJECT *obj=current_proc->pa8;

	if (am_i_airborn(obj)==SYSTEM_CARRY_SET)
	{
		/* getting seperated in the air */
		flight(obj,SCX(0x38000),-SCY(0x60000),SCY(0x8000),5,ANIM_KDOWN);

		land_on_my_back();
	}
	else
	{
		/* ags_on_ground */
		away_x_vel(current_proc,obj,SCX(0x70000));					// move away from him
		current_proc->a11=10;
		/* ags4 */
		do
		{
			process_sleep(1);
			move_slave_too();
		}
		while(--current_proc->a11>0);
	}

	/* ags6, slow down */
	do
	{
		process_sleep(1);
		move_slave_too();
		away_x_vel(current_proc,obj,abs(obj->oxvel.pos>>1));			// 1/2 speed
	}
	while (abs(obj->oxvel.pos>>1)>0x1000);

	/* ags7 */
	move_slave_too();
	wait_for_his_dog(SCY(0x050));

	reaction_exit(obj);
}

/******************************************************************************
 Function: void move_slave_too(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	move slave when push back others
******************************************************************************/
void move_slave_too(void)
{
	OBJECT *ta0=current_proc->pdata.p_slave;
	if (ta0!=NULL)
	{
		ta0->oxvel.pos=(current_proc->pa8)->oxvel.pos;
		ta0->oyvel.pos=(current_proc->pa8)->oyvel.pos;
		match_ani_points(current_proc->pa8,ta0);
	}

	return;
}

#if 0
JUMPTBL ochar_block_start_calls[] =
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

JUMPTBL ochar_react_start_calls[] =
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

#endif


/******************************************************************************
 Function: void reaction_start(WORD rid,FUNC airroutine,WORD boss_offset,FUNC ccroutine)

 By: David Schwartz

 Date: Nov 1994

 Parameters: rid - id code of prolog to reaction (combine pieces of code with case)
			 airroutine - routine to call for reaction if airborn(a5)
			 boss_offset - boss reaction offset (a6)
			 routine - corner check routine to run (a7)



 Returns: None

 Description:	standard reaction start routine.  everyone reacting to a hit
				should call this routine 1st.
******************************************************************************/
extern JUMPTBL motaro_branches[];
extern JUMPTBL sk_branches[];

void reaction_start(WORD rid,FUNC airroutine,WORD boss_offset,FUNC ccroutine)
{
	switch (rid)
	{
		case REACT_START:
			current_proc->pdata.p_action=ACT_REACTING;
			reaction_start_chores();

//			if (ochar_react_start_calls[current_proc->pa8->ochar]!=NULL)
//				ochar_react_start_calls[current_proc->pa8->ochar]();
			break;
		case REACT_BLOCK:
			reaction_start_chores();
			zero_my_p_hit(current_proc);
			inc_p_block;
			airroutine=NULL;
//			if (ochar_block_start_calls[current_proc->pa8->ochar]!=NULL)
//				ochar_block_start_calls[current_proc->pa8->ochar]();
			break;
	}

RST5:
	lights_on_hit();

	if (airroutine!=NULL && am_i_airborn(current_proc->pa8)==SYSTEM_CARRY_SET)
		airroutine();

RST2:
	if (ccroutine!=NULL)
		ccroutine();

RST9:
	if (boss_offset==0)
		return;

RSTA:
	if ((current_proc->pa8)->ochar==FT_MOTARO)
		motaro_branches[boss_offset]();

	if ((current_proc->pa8)->ochar==FT_SK)
		sk_branches[boss_offset]();

	return;
}

/******************************************************************************
 Function: void update_his_hit_queue(WORD pa7)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: update other dudes hit queu
******************************************************************************/
void update_his_hit_queue(WORD pa7)
{
	PROCESS *pa13;

	pa13=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	current_proc->pdata.p_hitby=pa7;
	update_hit_queue();
	current_proc=pa13;
	return;
}

/******************************************************************************
 Function: void combo_airborn_hit(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: combo airborn hit reaction code with the correct hit sound effect
******************************************************************************/
void combo_airborn_hit(void)
{
	group_sound(2);
	rsnd_big_smack();
	shake_a11(0x0006,0x0006);

	flight(current_proc->pa8,SCX(0x50000),-SCY(0x30000),SCY(0x8000),5,ANIM_KDOWN);
	land_on_my_back();
}

/******************************************************************************
 Function: void generic_airborn_hit(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: generic airborn hit reaction code with the correct hit sound effect
******************************************************************************/
void r_airborn_duck_kick(void)
{
	avoid_corner_trap(1);
	generic_airborn_hit();
}

void generic_airborn_hit(void)
{
	rsnd_med_smack();
	rsnd_react_voice();

	airborn_hit_no_sound();
	return;
}

/******************************************************************************
 Function: void airborn_hit_no_sound(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: generic airborn hit reaction code
******************************************************************************/
void airborn_hit_no_sound(void)
{
	flight(current_proc->pa8,SCX(0x38000),-SCY(0x60000),SCY(0x8000),5,ANIM_KDOWN);

	land_on_my_back();
}

/******************************************************************************
 Function: void r_tusk_blur(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description: react to tusk blur
******************************************************************************/
void r_tusk_blur(void)
{
	OBJECT *obj=current_proc->pa8;
	void *heap;
	void *ta9;
	void *ta10;
	PROCESS *ta13;
	WORD ta0,ta1;
	void *shape;

	reaction_start(REACT_START,NULL,if_shao_then_pass(1),NULL);

	stop_me(obj);
	set_no_block(obj);

	current_proc->pdata.p_action=ACT_BLURRED;

	/* patch from arcade game to deal with a CDROM instead of a ROM */
	ta13=current_proc;
	current_proc=current_proc->pdata.p_otherproc;			// other dude bettter be tusk or we be in trouble
	ta10=current_proc->pa9;									// save other dudes current anim ptr
	get_char_ani(ANIM_TABLE2,ANIM_TUSK_TOP);
	ta9=current_proc->pa9;									// save anim pt for us
	current_proc->pa9=ta10;									// restore other dudes anim ptr
	ta10=(current_proc->pa8)->oheap;						// get other dudes heap ptr we will need it
	current_proc=ta13;										// restore us
	current_proc->pa9=ta9;									// set our anim ptr


	shape=obj->oshape;

	heap=obj->oheap;											// save our heap ptr
	obj->oheap=ta10;							// set for the correct heap, other dudes
	do_next_a9_frame(obj);
	ta9=current_proc->pa9;		// big hack: bug fix
	obj->oheap=heap;							// restore our heap
	get_char_ani(ANIM_TABLE1,ANIM_STANCE);
	obj->oshape=shape;

	current_proc->pdata.p_store1=4;
	current_proc->a10=0x60/2;

	/* rtbl5 */
	do
	{
		process_sleep(2);
		obj->oheap=ta10;							// set for the correct heap, other dudes
		current_proc->pa9=ta9;		// big hack: bug fix
		do_next_a9_frame(obj);
		ta9=current_proc->pa9;		// big hack: bug fix
		obj->oheap=heap;							// restore our heap
		obj->oshape=shape;
		get_char_ani(ANIM_TABLE1,ANIM_STANCE);
		if (--current_proc->pdata.p_store1==0)
		{
			rsnd_whoosh();
			current_proc->pdata.p_store1=4;
		}
	}
	while(--current_proc->a10>0);

	ta0=current_proc->pdata.p_hit;
	ta1=current_proc->pdata.p_damage;
	current_proc->pdata.p_hit=0;
	current_proc->pdata.p_damage=0;
	back_to_normal();
	current_proc->pdata.p_hit=ta0;
	current_proc->pdata.p_damage=ta1;

	current_proc->pdata.p_action=ACT_BLURRED;

	if (am_i_airborn(obj)==SYSTEM_CARRY_SET)
		fall_on_my_back();
	else
	{
		ground_player(obj);
		if (q_am_i_a_boss()==SYSTEM_CARRY_CLR)
		{
			get_char_ani(ANIM_TABLE1,ANIM_DIZZY);
			animate_a0_frames(0x0005,0x000c);
		}

		reaction_exit(obj);
	}
}

/******************************************************************************
 Function: void r_tusk_zap(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: react to tusk zap
******************************************************************************/
void r_tusk_zap(void)
{
	shake_a11(0x0006,0x0006);
	reaction_start(REACT_START,generic_airborn_hit,1,NULL);
	his_ochar_sound(0);
	current_proc->a0=SCX(0x40000);
	stumble_back_vel();
}

/******************************************************************************
 Function: void r_skull(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: react to skull
******************************************************************************/
void r_skull(void)
{
	zap_stumble();
}

/******************************************************************************
 Function: void r_lk_zap(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: react to lk zap
******************************************************************************/
void r_lk_zap(void)
{
	his_ochar_sound(2);
	zap_stumble();
}

/******************************************************************************
 Function: void r_rocket(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description: react to robo rocket
******************************************************************************/
void r_rocket(void)
{
	shake_a11(0x0006,0x0006);
	zap_stumble();
}

/******************************************************************************
 Function: void r_kano_zap(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: react to kano zap
******************************************************************************/
void r_kano_zap(void)
{
	his_ochar_sound(2);
	zap_stumble();
}

/******************************************************************************
 Function: void r_sg_zap(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: react to she goro zap
******************************************************************************/
void r_sg_zap(void)
{
	his_ochar_sound(2);
	zap_stumble();
}

void r_mileena_zap(void) 
{
	his_ochar_sound(9);
	set_half_damage(current_proc->pa8);
	zap_stumble();
}

/******************************************************************************
 Function: void r_sonya_zap(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: react to kano zap
******************************************************************************/
void r_sonya_zap(void)
{
	his_ochar_sound(1);
	zap_stumble();
}

/******************************************************************************
 Function: void zap_stumble(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: react to kano zap
******************************************************************************/
void zap_stumble(void)
{
	reaction_start(REACT_START,generic_airborn_hit,6,NULL);

	shake_a11(0x0006,0x0006);
	current_proc->a0=SCX(0x40000);
	stumble_back_vel();
}

/******************************************************************************
 Function: void r_ind_zap(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: react to indian zap
******************************************************************************/
void r_ind_zap(void)
{
	group_sound(2);

	reaction_start(REACT_START,generic_airborn_hit,1,NULL);
	current_proc->a0=SCX(0x40000);
	stumble_back_vel();
}

/******************************************************************************
 Function: void r_quake(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: react to jax quake
******************************************************************************/
void r_quake(void)
{
	current_proc->pdata.p_action=ACT_QUAKE;

	reaction_start(REACT_START,generic_airborn_hit,6,NULL);

	group_sound(2);

	away_x_vel(current_proc,current_proc->pa8,SCX(0x28000));

	animate_a9(0x0004,ANIM_STUMBLE);

	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void r_post_shake(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: react to post shake
******************************************************************************/
void r_post_shake(void)
{
	flight(current_proc->pa8,SCX(0x40000),-SCY(0x40000),SCY(0x8000),5,ANIM_KDOWN);
	reaction_land();
}

/******************************************************************************
 Function: void r_post_bike(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: react to post bike kick (sonya)
******************************************************************************/
void r_post_bike(void)
{
	flight(current_proc->pa8,SCX(0x40000),-SCY(0x60000),SCY(0xa000),5,ANIM_KDOWN);
	reaction_land();
}

/******************************************************************************
 Function: void r_sk_charge(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description: react to sk charge
******************************************************************************/
void r_sk_charge(void)
{
	rsnd_big_smack();
	group_sound(2);
	shake_a11(0x0006,0x0006);

	reaction_start(REACT_START,airborn_hit_no_sound,6,NULL);

	away_x_vel(current_proc,current_proc->pa8,SCX(0x50000));
	animate_a9(3,ANIM_STUMBLE);
	reaction_exit(current_proc->pa8);
}


/******************************************************************************
 Function: void r_jax_zap(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: react to jax zap
******************************************************************************/
void r_jax_zap(void)
{
	group_sound(2);
	shake_a11(0x0006,0x0006);

	reaction_start(REACT_START,airborn_hit_no_sound,1,NULL);

	(long)current_proc->a0=SCX(0x40000);
	stumble_back_vel();
}

/******************************************************************************
 Function: void r_hat(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: react to hat attack
******************************************************************************/
void r_hat(void)
{
	reaction_start(REACT_START,NULL,if_shao_then_pass(1),NULL);

	local_group_sound(2);

	find_ani_part2(ANIM_STUMBLE);

	away_x_vel(current_proc,current_proc->pa8,SCX(0x30000));

	init_anirate(6);
	current_proc->a10=0x24;
	/* rhat3 */
	do
	{
		current_proc->p_comp_flag=PC_RHAT_WAKE;
		process_sleep(1);
		current_proc->p_comp_flag=PC_CLEAR;

		/* rhat_wake */
		if (current_proc->pdata.p_anicount==1)
			create_blood_proc(BLOOD_HAT);

		/* rhat5 */
		next_anirate();
	}
	while(--current_proc->a10>0);

	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void stumble_back_vel(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: current_proc->a0 - stumble velocity

 Returns: None

 Description:	make player stumble back
******************************************************************************/
void stumble_back_vel(void)
{
	away_x_vel(current_proc,current_proc->pa8,(long)current_proc->a0);

	animate_a9(0x0004,ANIM_STUMBLE);

	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void reaction_start_chores(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters:

 Returns: None

 Description: reaction setup stuff
******************************************************************************/
void reaction_start_chores(void)
{
	OBJECT *obj=current_proc->pa8;

	/* inc_p_hit */
	current_proc->pdata.p_hit++;

	obj->ograv.pos=0;

	if ((current_proc->pdata.p_flags & PM_SITDUCK) != 0)
		f_thatsall=1;								// i was a sitting duck --> thats all!

RST1:
	current_proc->pdata.p_flags |=PM_REACTING;		// flag: i am reacting to an attack

	delete_slave();									// no slaves!!

	obj->oflags2=(obj->oflags2 & (~(M_NOBLOCK | M_INVISO | M_IGNORE_Y | M_NOCOL))) | M_NOFLIP | M_SHADOW;	// i am hit --> now allow blocking !!
																					// clear inviso, set shadow, set no flip me if im out o control

	current_proc->pdata.p_stk=0;					// i am no longer striking!!
	current_proc->pdata.p_power=0;				// not invinicble to anything

	player_normpal();
	face_opponent(obj);
	stop_me(obj);

	update_hit_queue();

	return;
}

/******************************************************************************
 Function: void update_hit_queue(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: track the last 5 hits to the person
******************************************************************************/
void update_hit_queue(void)
{
	WORD *hitque;

	if (current_proc->pa8==p1_obj)									// set correct hit queue
		hitque=p1_hitq;
	else hitque=p2_hitq;

	/* shift 5 entries down */
	*(hitque+5)=*(hitque+4);
	*(hitque+4)=*(hitque+3);
	*(hitque+3)=*(hitque+2);
	*(hitque+2)=*(hitque+1);
	*(hitque+1)=*(hitque+0);
	*(hitque+0)=current_proc->pdata.p_hitby;

	return;
}

/******************************************************************************
 Function: void b_uppercut(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: block uppercut
******************************************************************************/
void b_uppercut(void)
{
	rsnd_big_block();

	shake_a11(0x0004,0x0004);

	reaction_start(REACT_BLOCK,NULL,0,cc_block_upcut);

	away_x_vel(current_proc,current_proc->pa8,SCX(0x40000));

	block_shake_n_exit(4,2);
}

/******************************************************************************
 Function: void b_boss_hit1(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: boss hit block
******************************************************************************/
void b_boss_hit1(void)
{
	reaction_start(REACT_BLOCK,NULL,0,NULL);

	rsnd_big_block();

	away_x_vel(current_proc,current_proc->pa8,SCX(0x60000));

	block_shake(ANIM_STANDBLOCK,3,2);

	(long)current_proc->a0=SCX(0x40000);
	stumble_back_vel();
}

/******************************************************************************
 Function: void b_hard_ken_masters(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: block hard
******************************************************************************/
void b_hard_ken_masters(void)
{
	rsnd_big_block();
	block2(cc_block_avoid_corner);
}

/******************************************************************************
 Function: void b_hard(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: block hard
******************************************************************************/
void b_hard(void)
{
	rsnd_big_block();

	b_hard_silent();
}

/******************************************************************************
 Function: void b_hard_silent(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: block hard quiet
******************************************************************************/
void b_hard_silent(void)
{
	block2(NULL);
}

/******************************************************************************
 Function: void block2(FUNC pa7)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: block common routine
******************************************************************************/
void block2(FUNC pa7)
{
	reaction_start(REACT_BLOCK,NULL,0,pa7);

	/* block3 */
	shake_a11(0x0004,0x0004);
	away_x_vel(current_proc,current_proc->pa8,SCX(0x20000));

	block_shake_n_exit(3,2);
}

/******************************************************************************
 Function: void b_weak_no_masters(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: block weak
******************************************************************************/
void b_weak_no_masters(void)
{
	rsnd_small_block();

	reaction_start(REACT_BLOCK,NULL,0,NULL);

	block_shake_n_exit(3,2);
}

/******************************************************************************
 Function: void b_weak(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: block weak
******************************************************************************/
void b_weak(void)
{
	rsnd_small_block();

	reaction_start(REACT_BLOCK,NULL,0,cc_block_weak);

	block_shake_n_exit(3,2);
}

/******************************************************************************
 Function: void b_weak_silent(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description: block weak
******************************************************************************/
void b_weak_silent(void)
{
	reaction_start(REACT_BLOCK,NULL,0,cc_block_weak);

	block_shake_n_exit(3,2);
}

/******************************************************************************
 Function: void b_sweep(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	block sweep kick routine
******************************************************************************/
void b_sweep(void)
{
	rsnd_small_block();

	reaction_start(REACT_BLOCK,NULL,0,cc_block_sweep);

	block_shake_n_exit(2,2);
}

/******************************************************************************
 Function: void block_shake_n_exit(WORD scnt,WORD sleep)

 By: David Schwartz

 Date: Nov 1994

 Parameters: scnt - # of times to shake (a10)
			 sleep - sleep time (a11)

 Returns: None

 Description:	shake and exit for blocking a hit routine
******************************************************************************/
void block_shake_n_exit(WORD scnt,WORD sleep)
{
	block_shake(get_block_ani_off(),scnt,sleep);
	block_exit();
}

/******************************************************************************
 Function: void block_exit(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	exit for blocking a hit routine
******************************************************************************/
void block_exit(void)
{
	/* block_exit */
	current_proc->pdata.p_hit=0;				// ok, but you gotta start the count over
	current_proc->pdata.p_block=0;				// ok, but you gotta start the count over

	back_to_normal();

	if (am_i_joy()==SYSTEM_CARRY_CLR)
	{
		stack_jump(d_post_block);
	}
	else
	{
		if (am_i_short()==SYSTEM_CARRY_SET)
		{
			stack_jump_joy(joy_proc,JI_JOY_DUCK_BLOCK_LOOP);
		}
		else
		{
			stack_jump_joy(joy_proc,JI_JOY_BLOCK_LOOP);
		}
	}
}

/******************************************************************************
 Function: WORD get_block_ani_off(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: animation offset to display

 Description:	determine which block animation to display
******************************************************************************/
inline WORD get_block_ani_off(void)
{
	void *start,*ta9;

	if ((current_proc->pa8)->ochar==FT_SG)
	{
		/* sg_block_ani_offset, special case BS for sheeva */
		ta9=current_proc->pa9;
		start=get_char_ani(ANIM_TABLE1,ANIM_DUCKBLOCK);
		find_last_frame();
		((ADDRESS*)current_proc->pa9)+=2;				// fix, wierd bug
		if ((ADDRESS)ta9>=(ADDRESS)start && (ADDRESS)ta9<=(ADDRESS)current_proc->pa9)
		{
			current_proc->pa9=ta9;
			return(ANIM_DUCKBLOCK);
		}
		else
		{
			current_proc->pa9=ta9;
			return(ANIM_STANDBLOCK);
		}
	}
	else return(tall_or_short_ani(ANIM_STANDBLOCK,ANIM_DUCKBLOCK));
}

/******************************************************************************
 Function: void block_shake(WORD anioff,WORD scnt,WORD sleep)

 By: David Schwartz

 Date: Nov 1994

 Parameters: scnt - # of times to shake (a9)
			 sleep - sleep time (a10)
			 anioff - animation offset (a11)

 Returns: None

 Description:	Handy routine to shake someone who is blocking
******************************************************************************/
void block_shake(WORD anioff,WORD scnt,WORD sleep)
{
	OBJECT *obj=current_proc->pa8;

	get_char_ani(ANIM_TABLE1,anioff);
	((ADDRESS *)current_proc->pa9)++;

	/* blk2 */
	do
	{
		block_shake_ani(sleep);
		block_shake_ani(sleep);
		((ADDRESS *)current_proc->pa9)-=2;
	}
	while(--scnt>0);

	stop_me(obj);

	return;
}

void block_shake_ani(WORD pa0)
{
	do_next_a9_frame(current_proc->pa8);
	current_proc->p_comp_flag=PC_BLOCK_SHAKE_WAKE;
	process_sleep(pa0);
	current_proc->p_comp_flag=PC_CLEAR;
	return;
}
/******************************************************************************
 Function: void fall_on_my_back(short pa0,short pa1,long pa2,WORD pa3)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	fall on my back routine
******************************************************************************/
void fall_on_my_back(void)
{
	flight(current_proc->pa8,0,0,SCY(0x8000),5,ANIM_KDOWN);

	reaction_land();
}

/******************************************************************************
 Function: void reaction_land(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	landing reaction
******************************************************************************/
void reaction_land(void)
{
	shake_n_sound();

	find_ani_part2(ANIM_KDOWN);

	mframew(4);

	process_sleep(3);

	getup_local_reaction_exit;
}

/******************************************************************************
 Function: void land_on_my_back(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	dude will land on his back
******************************************************************************/
void land_on_my_back(void)
{
	shake_n_sound();

	shake_on_my_back(3,1);

	process_sleep(4);

	getup_local_reaction_exit;
}

WORD getup_speeds[]= {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};

/******************************************************************************
 Function: void gup2(WORD ani)

 By: David Schwartz

 Date: Nov 1994

 Parameters: ani - animation offset

 Returns: None

 Description:	local reaction exit code for getting up
	sweepup_local_reaction_exit
	getup_local_reaction_exit
******************************************************************************/
void gup2(WORD ani)
{
	WORD *speedtbl=getup_speeds;

	(WORD *)current_proc->a11=getup_speeds;		// set for drone process

	check_stay_down();				// should I even get up at all ?
	check_winner_status();
	back_to_normal();						// clear "phit not getting zeroed" bug

	if (am_i_joy()==SYSTEM_CARRY_CLR)
	{
		stack_jump(d_getup);
	}

	/* is_stick_down */
	if (JOYSTICK_IN_A0(current_proc) & MASK_JDOWN)
	{
		/* getup_stay_ducked */
		back_to_normal();
		find_ani_last_frame(ANIM_DUCK);
		do_next_a9_frame(current_proc->pa8);

		stack_jump_joy(joy_proc,JI_JOY_GETUP_ENTRY);
	}
	else
	{
		get_char_ani(ANIM_TABLE1,ani);

		init_anirate(speedtbl[current_proc->pa8->ochar]);							// getup/sweepup speeds

		do
		{
GUP4:
			next_anirate();

			if (GET_LONG(current_proc->pa9)==0)
			{
				process_sleep(2);
				reaction_exit(current_proc->pa8);
			}

			if (ani!=ANIM_SWEEPUP)
			{
				if (JOYSTICK_IN_A0(current_proc) & MASK_JDOWN)
				{
					/* joy_getup_abort */
					player_normpal();
					stack_jump_joy(joy_proc,JI_JOY_DOWN);
				}

				if (JOYSTICK_IN_A0(current_proc) & MASK_JUP)
				{
					/* joy_getup_jump */
					player_normpal();
					stack_jump_joy(joy_proc,JI_JOY_UP);
				}
			}

			process_sleep(1);
		}
		while(1);
	}
}

/******************************************************************************
 Function: void repell_one_of_us(long vel_me,long vel_him,JUMPTBL xferpt)

 By: David Schwartz

 Date: Nov 1994

 Parameters: vel_me - velocity to repell me at
			 vel_him - velocity to repell him at
			 xferpt - routine to xfer attacker

 Returns: None

 Description:	Repells one of us based on our position on the scr
******************************************************************************/
void repell_one_of_us(long vel_me,long vel_him,JUMPTBL xferpt)
{
	if (am_i_close_to_edge(current_proc)==SYSTEM_CARRY_SET)
	{
		if (xferpt!=NULL)
			takeover_him(xferpt);

		away_x_vel(current_proc->pdata.p_otherproc,current_proc->pdata.p_otherguy,vel_him);
	}
	else away_x_vel(current_proc,current_proc->pa8,vel_me);

	return;
}

/******************************************************************************
 Function: void check_stay_down(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	check if guy should even bother getting up
******************************************************************************/
void check_stay_down(void)
{
	if ((current_proc->pdata.p_flags & PM_CORPSE) || (!(winner_status==0 || winner_status==3 || winner_status==current_proc->procid)))
	{
		/* i lose --> stay down */

		clear_shadow_bit(current_proc->pa8);

		wait_forever();
	}

	return;
}

/******************************************************************************
 Function: inline void shake_n_sound(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	shake dude and make sound
******************************************************************************/
inline void shake_n_sound(void)
{
	shake_a11(0x0006,0x0006);
	rsnd_ground();

	return;
}

/******************************************************************************
 Function: void reverse_ani(WORD sleeptime,WORD anioff)

 By: David Schwartz

 Date: Nov 1994

 Parameters: sleeptime - sleep time
			 anioff - animation offset

 Returns: None

 Description:	 Walk backwards through an animation starting at the
                  2nd to last frame
******************************************************************************/
void reverse_ani(WORD sleeptime,WORD anioff)
{
	get_char_ani(ANIM_TABLE1,anioff);
	current_proc->a10=(ADDRESS)(((ADDRESS *)current_proc->pa9)-1);
	find_last_frame();
	((ADDRESS *)current_proc->pa9)--;

	do
	{
		do_next_a9_frame(current_proc->pa8);
		process_sleep(sleeptime);
		(ADDRESS *)current_proc->pa9=((ADDRESS *)current_proc->pa9)-2;
	}
	while((ADDRESS)current_proc->pa9 != current_proc->a10);

	return;
}

/******************************************************************************
 Function: void shake_on_my_back(WORD anispeed,WORD scount)

 By: David Schwartz

 Date: Nov 1994

 Parameters: anispeed - speed of ani (a0)
			 scount - # of times to shake (2 frames each)

 Returns: None

 Description:	do the "shake on my back" animation given:
******************************************************************************/
void shake_on_my_back(WORD anispeed,WORD scount)
{
	find_ani_part2(ANIM_KDOWN);

	(ADDRESS *)current_proc->a11=(ADDRESS *)current_proc->pa9;

	do
	{
		(ADDRESS *)current_proc->pa9=(ADDRESS *)current_proc->a11;
		mframew(anispeed);
	}
	while(--scount>0);

	return;
}

/******************************************************************************
 Function: void blast_entry(long pa4,void *pa5,void *pa6,long pa7)

 By: David Schwartz

 Date: May 1995

 Parameters: pa4 - start pos
			 pa5 - module to init
			 pa6 - display list bridge
			 pa7 - new curback

 Returns: None

 Description:	blast player through roof
******************************************************************************/
void blast_entry(long pa4,void *pa5,void *pa6,long pa7)
{
	OBJECT *obj=current_proc->pa8;

	f_blast=1;

	blast_init(pa4,pa5,pa6,pa7);

	xfer_otherguy(wait_forever);
	no_edge_limits();

	/* flying up = special case */
	away_x_vel(current_proc,obj,SCX(0x10000));

	obj->oyvel.pos=-SCY(0x120000);
	obj->ograv.pos=SCY(0x5800);
	get_char_ani(ANIM_TABLE1,ANIM_KDOWN);
	init_anirate(15);

	/* blst4 */
	do
	{
		process_sleep(1);
		next_anirate();
	}
	while(obj->oyvel.pos<0);			// wait until we start heading down

	/* fall to ground */
	flight(obj,DONT_TOUCH,DONT_TOUCH,0x8000,5,-1);

	ground_ochar();
	current_proc->pdata.p_ganiy=obj->oypos.u.intpos; 	// new ground

	xfer_otherguy(back_to_the_fight);

	shake_n_sound();
	find_ani_part2(ANIM_KDOWN);
	mframew(4);

	process_sleep(0x20);

	f_blast=0;

	smoke_if_smoke();
	getup_reaction_exit;
}

void smoke_if_smoke(void)
{
	if ((current_proc->pa8)->ochar==FT_SMOKE)
		create_fx(FX_SMOKE);

	return;
}

void blast_init(long pa4,void *pa5,void *pa6,long pa7)
{
	PROCESS *pa0;

	immune_a0(current_proc->pdata.p_otherproc);		// dont kill otherguy
	immune_a0(exprc_er(PID_MASTER));

	/* make multi part procs immune */
	immune_mpp();
	MURDER;
	pa0=CREATE(PID_BLAST,background_blast);

	pa0->pdata.p_store5=(ADDRESS)pa4;		// position
	pa0->pdata.p_store8=(ADDRESS)pa5;		// new module
	pa0->pdata.p_store7=(ADDRESS)pa6;		// transition dlist
	pa0->pdata.p_store6=(ADDRESS)pa7;		// new curback

	unimmune_a0(current_proc->pdata.p_otherproc);
	unimmune_a0(exprc_er(PID_MASTER|0x8000));
	unimmune_mpp();

	return;
}

/******************************************************************************
 Function: void back_to_the_fight(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	return to the fight
******************************************************************************/
void back_to_the_fight(void)
{
	OBJECT *obj=current_proc->pa8;
	short xpos,ypos;

	get_screen_coordinates(current_proc->pdata.p_otherguy,&xpos,&ypos);

	if (xpos>(SCRRGT/2))
		obj->oxpos.u.intpos=SCX(0x40);
	else obj->oxpos.u.intpos=SCRRGT-SCX(0x40);			// pick other half of screen

	obj->oypos.u.intpos=SCRBOT;						// come from the bottom

	world_a5(obj);									// put me in the world


	/* pop up */
	get_char_ani(ANIM_TABLE1,ANIM_JUMPUP);
	((ADDRESS*)current_proc->pa9)++;
	do_next_a9_frame(obj);							// pose: jump up
	face_opponent(obj);

	obj->ograv.pos=0;
	obj->oyvel.pos=-SCY(0xc0000);

	do
	{
		process_sleep(1);
	}
	while(obj->oypos.u.intpos>current_proc->pdata.p_ganiy);

	flight(obj,0,obj->oyvel.pos,SCY(0x10000),NEVER_ANI,-1);

	ground_ochar();
	current_proc->pdata.p_ganiy=obj->oypos.u.intpos; 	// new ground

	clear_nocol(current_proc->pa8);
	smoke_if_smoke();

	jump_up_land_jsrp();
	reaction_exit(obj);
}

/******************************************************************************
 Function: void r_combo0(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	combo reaction 0 - hit to face
******************************************************************************/
void r_combo0(void)
{
	create_blood_proc(BLOOD_COMBO);

	combo_setup();

	reaction_start(REACT_START,combo_airborn_hit,5,NULL);
	set_no_block(current_proc->pa8);
	rsnd_big_smack();

	get_char_ani(ANIM_TABLE1,ANIM_HITHI);
	current_proc->a10=(ADDRESS)current_proc->pa9;
	((ADDRESS *)current_proc->pa9)+=3;
	do_next_a9_frame(current_proc->pa8);
	process_sleep(3);
	(ADDRESS)current_proc->pa9=current_proc->a10;
	mframew(4);
	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void r_combo1_stab(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	combo reaction 1 - stumble away stabbing
******************************************************************************/
void r_combo1_stab(void)
{
	combo_setup();
	reaction_start(REACT_START,combo_airborn_hit,5,NULL);
	set_no_block(current_proc->pa8);
	rsnd_stab();
	stack_jump(combo1);
}

/******************************************************************************
 Function: void r_combo1_klang(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	combo reaction klang
******************************************************************************/
void r_combo_klang(void)
{
	combo_setup();
	reaction_start(REACT_START,combo_airborn_hit,5,NULL);
	set_no_block(current_proc->pa8);
	rsnd_stab();
	stack_jump(combo1);
}

/******************************************************************************
 Function: void r_combo1(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	combo reaction 1 - stumble away
******************************************************************************/
void r_combo1(void)
{
	combo_setup();
	reaction_start(REACT_START,combo_airborn_hit,5,NULL);

	create_blood_proc(BLOOD_COMBO);
	set_no_block(current_proc->pa8);
	rsnd_smack();
	stack_jump(combo1);
}

void combo1(void)
{
	create_blood_proc(BLOOD_COMBO);

	away_x_vel(current_proc,current_proc->pa8,SCX(0x20000));

	get_char_ani(ANIM_TABLE1,ANIM_HITHI);
	current_proc->a10=(ADDRESS)current_proc->pa9;
	((ADDRESS *)current_proc->pa9)+=3;
	do_next_a9_frame(current_proc->pa8);
	process_sleep(3);
	(ADDRESS)current_proc->pa9=current_proc->a10;
	mframew(4);
	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void r_combo2_stab(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	combo reaction 2 - stumble away stabbing
******************************************************************************/
void r_combo2_stab(void)
{
	combo_setup();
	reaction_start(REACT_START,combo_airborn_hit,9,NULL);
	set_no_block(current_proc->pa8);
	rsnd_stab();
	stack_jump(combo2);
}

/******************************************************************************
 Function: void r_combo2(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	combo reaction 2 - knocked on yer back
******************************************************************************/
void r_combo2(void)
{
	combo_setup();
	reaction_start(REACT_START,combo_airborn_hit,9,NULL);
	set_no_block(current_proc->pa8);
	rsnd_big_smack();
	stack_jump(combo2);
}

void combo2(void)
{
	create_blood_proc(BLOOD_COMBO);
	set_half_damage(current_proc->pa8);

	create_fx(FX_COMMENT);

	flight(current_proc->pa8,SCX(0x40000),-SCY(0x80000),SCY(0x8000),5,ANIM_KDOWN);
	land_on_my_back();
}

/******************************************************************************
 Function: void r_combo3(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	combo reaction 3 - knocked High on yer back
******************************************************************************/
void r_combo3(void)
{
	combo_setup();

	reaction_start(REACT_START,combo_airborn_hit,9,NULL);
	set_no_block(current_proc->pa8);
	rsnd_big_smack();
	set_half_damage(current_proc->pa8);

	create_blood_proc(BLOOD_COMBO);
	create_fx(FX_COMMENT);

	flight(current_proc->pa8,SCX(0x20000),-SCY(0x80000),SCY(0x8000),5,ANIM_KDOWN);
	land_on_my_back();
}

/******************************************************************************
 Function: void r_boss_hit1(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	big boss hit
******************************************************************************/
void r_boss_hit1(void)
{
	create_blood_proc(BLOOD_COMBO);
	create_blood_proc(BLOOD_UPPERCUT);
	stack_jump(combo43);
}

/******************************************************************************
 Function: void r_combo4(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	combo reaction 4 - nailed!!
******************************************************************************/
void r_combo4(void)
{
	set_half_damage(current_proc->pa8);
	stack_jump(combo43);
}

void combo43(void)
{
	reaction_start(REACT_START,NULL,9,NULL);
	set_no_block(current_proc->pa8);

	create_blood_proc(BLOOD_COMBO);

	group_sound(2);				// group speech: wasted
	rsnd_big_smack();
	create_fx(FX_COMMENT);
	shake_a11(0x000a,0x000a);		// shake upon contact

	flight(current_proc->pa8,SCX(0xa0000),-SCY(0x80000),SCY(0x8000),5,ANIM_KDOWN);
	land_on_my_back();
}

/******************************************************************************
 Function: void combo_setup(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	combo setup routine
******************************************************************************/
void combo_setup(void)
{
	group_sound(2);			// group speech: wasted
	shake_a11(0x0006,0x0006);
	return;
}

/******************************************************************************
 Function: void combo_sitting_duck(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	combo sitting duck routine
******************************************************************************/
void combo_sitting_duck(void)
{
	get_char_ani(ANIM_TABLE1,ANIM_HITHI);
	((ADDRESS *)current_proc->pa9)++;
	mframew(4);

	process_sleep(8);

	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void r_sk_hammer(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	shao kahn hammer
******************************************************************************/
void r_sk_hammer(void)
{
	reaction_start(REACT_START,r_motaro_kick,1,NULL);

	his_ochar_sound(2);
	group_sound(9);

	shake_a11(0xa,0xa);

	create_blood_proc(BLOOD_COMBO);
	create_blood_proc(BLOOD_UPPERCUT);

	away_x_vel(current_proc,current_proc->pa8,SCX(0x30000));
	animate_a9(0x4,ANIM_STUMBLE);
	stop_me(current_proc->pa8);

	dizzy_by_boss();
}

/******************************************************************************
 Function: void r_sk_air_charge(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	shao kahn air charge
******************************************************************************/
void r_sk_air_charge(void)
{
	reaction_start(REACT_START,NULL,1,NULL);

	create_blood_proc(BLOOD_COMBO);
	create_blood_proc(BLOOD_UPPERCUT);

	group_sound(2);
	rsnd_big_smack();
	shake_a11(0xa,0xa);

	flight(current_proc->pa8,SCX(0x60000),-SCY(0x60000),SCY(0x8000),5,ANIM_KDOWN);

	shake_n_sound();
	find_ani_part2(ANIM_KDOWN);
	mframew(6);
	process_sleep(4);
	getup_reaction_exit;
}

/******************************************************************************
 Function: void r_motaro_kick(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	motaro kicked us good
******************************************************************************/
void r_motaro_kick(void)
{
	create_blood_proc(BLOOD_COMBO);
	create_blood_proc(BLOOD_UPPERCUT);

	reaction_start(REACT_START,NULL,1,NULL);

	group_sound(2);
	rsnd_big_smack();
	shake_a11(0x000a,0x000a);

	flight(current_proc->pa8,SCX(0x80000),-SCY(0x80000),SCY(0x8000),5,ANIM_KDOWN);

	shake_n_sound();
	shake_on_my_back(3,1);

	process_sleep(4);
	check_stay_down();
	check_winner_status();

	animate_a9(0x0004,ANIM_GETUP);

	reaction_exit(current_proc->pa8);
}

void dizzy_by_boss(void)
{
	current_proc->pdata.p_action=ACT_DIZZY;
	get_char_ani(ANIM_TABLE1,ANIM_DIZZY);
	animate_a0_frames(0x5,0x10);
	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void r_decoy_freeze(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	react to SZ decoy freeze
******************************************************************************/
void r_decoy_freeze(void)
{
	create_fx(FX_FREEZE_BLAST);
	his_ochar_sound(5);
	r_freeze();
}

/******************************************************************************
 Function: void r_freeze(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description: react to be frozen
******************************************************************************/
void r_freeze(void)
{
	OBJECT *obj=current_proc->pa8;
	PROCESS *pa0;
	void *animframe,*ta9;

	lights_on_slam();

	current_proc->pdata.p_hitby=ACT_SKY_ICE;		// make sure this gets queued
	reaction_start_chores();
	dec_my_p_hit;

	if (current_proc->pdata.p_action==ACT_FROZEN)
	{
		/* freeze_backfire */
		takeover_him(r_freeze);
		freeze_wakeup();
	}

	stop_me(obj);				// sans movement
	set_no_block(obj);			// sans blocking
	me_in_back();


	/* low on strength = frosty !! */
	if (get_my_strength(obj)<=8)
	{
		pa0=create_fx(FX_FORDEN);
		pa0->a11=0x1c;					// pass sound : Frosty!!
	}

	/* freez3 */
	current_proc->pdata.p_action=ACT_FROZEN;

	obj->oflags2&=(~M_NOFLIP);			// i can be flipped

	player_froze_pal();

//******************************
	if (obj->ochar==FT_TUSK)
	{
		ta9=current_proc->pa9;
		animframe=get_char_ani(ANIM_TABLE2,ANIM_TUSK_BLUR);
		((ADDRESS*)animframe)+=4;			// skip intro
		find_part2();
		if ( (((ADDRESS)ta9)>=((ADDRESS)animframe)) && (((ADDRESS)ta9)<=((ADDRESS)current_proc->pa9)))
		{
			pose2_a9(0,ANIM_TUSK_BLUR);			// do freeze in blur frame
		}
		else current_proc->pa9=ta9;
	}
//******************************

	shake_a8_up(obj,SCX(0x0004),SCY(0),3,2);

	shake_a8_up(obj,SCX(0x0004),SCY(0),3,8);

	process_sleep(0x60);

	freeze_wakeup();
}

/******************************************************************************
 Function: void freeze_wakeup(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description: wakeup from being frozen
******************************************************************************/
void freeze_wakeup(void)
{
	player_normpal();

	if (am_i_airborn(current_proc->pa8)==SYSTEM_CARRY_CLR)
	{
		reaction_exit(current_proc->pa8);		// grounded--> cool
	}
	else
	{
		flight(current_proc->pa8,0,SCY(0x10000),SCY(0x8000),5,ANIM_KDOWN);
		land_on_my_back();
	}
}

/******************************************************************************
 Function: void b_knee_elbow(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	block knee or elbow hit
******************************************************************************/
void b_knee_elbow(void)
{
	shake_a11(0x0004,0x0004);
	stack_jump(b_combo);
}

/******************************************************************************
 Function: void b_combo(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	block combo
******************************************************************************/
void b_combo(void)
{
	reaction_start(REACT_BLOCK,NULL,0,cc_block_avoid_corner);
	rsnd_big_block();

	away_x_vel(current_proc,current_proc->pa8,SCX(0x3c000));

	block_shake_n_exit(3,2);
}

/******************************************************************************
 Function: void b_combo_hard(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	block combo hard
******************************************************************************/
void b_combo_hard(void)
{
	shake_a11(0x4,0x4);

	reaction_start(REACT_BLOCK,NULL,0,cc_block_avoid_corner);
	rsnd_big_block();

	away_x_vel(current_proc,current_proc->pa8,SCX(0x4c000));

	block_shake_n_exit(4,3);
}

/******************************************************************************
 Function: void b_duck_hit_soft(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	block soft duck hit
******************************************************************************/
void b_duck_hit_soft(void)
{
	reaction_start(REACT_BLOCK,generic_airborn_hit,0,cc_block_avoid_corner);
	rsnd_small_block();

	away_x_vel(current_proc,current_proc->pa8,SCX(0x20000));

	block_shake_n_exit(3,2);
}

/******************************************************************************
 Function: void b_duck_hit_hard(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	block hard duck hit
******************************************************************************/
void b_duck_hit_hard(void)
{
	reaction_start(REACT_BLOCK,generic_airborn_hit,0,cc_block_avoid_corner);
	rsnd_small_block();

	away_x_vel(current_proc,current_proc->pa8,SCX(0x20000));

	block_shake_n_exit(3,2);
}

void b_lo_punch(void) 
{
	rsnd_small_block();

	reaction_start(REACT_BLOCK,NULL,0,cc_punch);

	away_x_vel(current_proc,current_proc->pa8,(am_i_short()==SYSTEM_CARRY_SET)?SCX(0x50000):SCX(0x30000));

	block_shake_n_exit(2,3);
}

/******************************************************************************
 Function: void b_punch(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	block punch
******************************************************************************/
void b_punch(void)
{
	rsnd_small_block();
	reaction_start(REACT_BLOCK,NULL,0,cc_punch);

	away_x_vel(current_proc,current_proc->pa8,SCX(0x20000));

	block_shake_n_exit(3,2);
}

/******************************************************************************
 Function: void cc_punch(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	condition check punch
******************************************************************************/
void cc_punch(void)
{
	avoid_corner_trap_b(3);
	return;
}

/******************************************************************************
 Function: void avoid_corner_trap_b(WORD pa1)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	avoid corner trap
******************************************************************************/
void avoid_corner_trap_b(WORD pa1)
{
	WORD ta0;

	ta0=current_proc->pdata.p_hit;

	current_proc->pdata.p_hit=current_proc->pdata.p_block;		// fake out avoid corner check
	avoid_corner_trap(pa1);
	current_proc->pdata.p_hit=ta0;

	return;
}

/******************************************************************************
 Function: void suspend_wait_action(void);

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	suspend and wait for action
******************************************************************************/
void suspend_wait_action(void)
{
	suspend_wait_action_jsrp();
	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void suspend_wait_action_jsrp(void);

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	suspend and wait for action
******************************************************************************/
void suspend_wait_action_jsrp(void)
{
	current_proc->a11=get_his_action(current_proc);

	/* susp3 */
	do
	{
		current_proc->p_comp_flag=PC_SUSPEND_WAKE;
		process_sleep(2);
		current_proc->p_comp_flag=PC_CLEAR;

		/* suspend_wait_wake */
	}
	while(get_his_action(current_proc)==current_proc->a11);

	return;
}


/******************************************************************************
 Function: WORD *get_my_hitq(OBJECT *pa8)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa8  - object who wants 2 know

 Returns: current_proc->a0 ptr to my hit queue

 Description:	get a pointer to a hit queue
******************************************************************************/
WORD *get_my_hitq(OBJECT *pa8)
{
	if (p1_obj==pa8)
		return((WORD *)current_proc->a0=p1_hitq);
	else return((WORD *)current_proc->a0=p2_hitq);
}


#if 0
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
extern JOYPAD swcurr;
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