/******************************************************************************
 File: mkstat.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Stationary Attack Routines
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
#include "mkani.h"
#include "mkguys.h"
#include "moves.h"
#include "mkgame.h"
#include "mkcanned.h"
#include "mkjoy.h"
#include "mkreact.h"
#include "mkfx.h"
#include "mkslam.h"
#include "mkbug.h"
#include "mkzap.h"
#include "mkstat.h"
#include "mkblood.h"
#include "mkpal.h"
#include "mksound.h"
#include "mkdrone.h"
#include "mkfile.h"
#include "mkfatal.h"
#include "psxspu.h"
#include "valloc.h"


extern WORD BABYJAD_P[];
extern WORD BABYMIL_P[];
extern WORD BABYSCO_P[];
extern WORD BABYREP_P[];
extern WORD BABYOSZ_P[];
extern WORD BABYSMO_P[];
extern WORD noob_P[];

/******************************************************************************
 Function: void do_stationary(WORD pa0)

 By: David Schwartz

 Date: May 1995

 Parameters: pa0 - stationary attack to perform

 Returns: None

 Description:	stationary attacks
******************************************************************************/
JUMPTBL stat_jumps[]=
{
	stat_do_lao_spin,			//  0
	stat_do_lia_scream, 	//  1
	stat_do_axe_up,		//  2
	stat_do_fast_axe_up, //  3
	stat_do_reflect,			//  4
	stat_do_shake,			//  5
	stat_do_noogy,			//  6
	stat_do_quake,		//  7
	stat_do_inviso,			//  8
	stat_do_leg_throw,		//  9
	stat_do_kano_swipe,		//  a
	stat_do_swat_gun,		//  b
	NULL,					//  c
	NULL,					//  d
	NULL,					//  e
	NULL,					//  f
	stat_do_hi_kick,		// 10
	stat_do_lo_kick,		// 11
	stat_do_sweep_kick, 	// 12
	stat_do_duck_punch, 	// 13
	stat_do_duck_kickh, 	// 14
	stat_do_duck_kickl, 	// 15
	stat_do_uppercut, 		// 16
	stat_do_roundhouse, 	// 17
	stat_do_babality,	 	// 18
	stat_do_fan_lift,	 	// 19
	stat_do_jade_flash,	 	// 1a
	stat_do_reptile_inv, 	// 1b
	stat_do_ermac_slam,	 	// 1c
	NULL,					// 1d
	NULL,					// 1e
	NULL,					// 1f
	NULL					// 20
};

void do_stationary(WORD pa0)
{
	if (stat_jumps[pa0]==NULL)
		LOCKUP;
	else
	{
		stat_jumps[pa0]();
	}
}

void stat_do_ermac_slam(void) 
{
	OBJECT *obj=current_proc->pa8;

	init_special_act(ACT_ERMAC_SLAM,obj);

	animate2_a9(0x2,ANIM_ERMAC_SLAM);

	ochar_sound(0xf);			// floor rumble
	shake_a11(0x3,0x8);

	current_proc->a10=0x10;
	/* emsl3 */
	do
	{
		process_sleep(1);
		if (q_is_he_a_boss()==SYSTEM_CARRY_CLR) 
		{
			if (strike_check_a0(obj,0x1a)==SYSTEM_CARRY_SET) 
			{
				/* ermac_slam_hit */
				update_tsl(l_ermac_slam,obj);

				if (f_block)	// blocked??
					break;
					
				process_sleep(8);

				goto UNSUM;
			}
		}
	}
	while(--current_proc->a10>0);

	/* emsl5 */
	current_proc->pdata.p_action=ACT_ER_SLAM_SD;

	process_sleep(0x30);

UNSUM:
	backwards_ani2(2,ANIM_ERMAC_SLAM);

	reaction_exit(obj);
}

void stat_do_swat_gun(void) 
{
	OBJECT *obj=current_proc->pa8;

	init_special_act(ACT_ERMAC_SLAM,obj);

	animate2_a9(0x2,ANIM_SWAT_GUN);

	init_anirate(2);

	current_proc->a11=0x20;
	/* swgun4 */
	do
	{
		process_sleep(1);
		next_anirate();
		if ((current_proc->pdata.p_otherguy)->ochar!=FT_MOTARO) 
		{
			if (strike_check_a0(obj,0x18)==SYSTEM_CARRY_SET) 
			{
				/* gun hit */
				current_proc->pdata.p_action=ACT_SWAT_GUN_SD;

				current_proc->a11=(is_he_airborn()==SYSTEM_CARRY_CLR) ? 0x20:8;
				/* swgun7 */
				do
				{
					process_sleep(1);
					next_anirate();
				}
				while(--current_proc->a11>0 );
				break;
			}
		}
	}
	while(--current_proc->a11>0);

	/* ungun */
	delete_slave();

	update_tsl(l_swat_gun,obj);
	find_ani2_part_a14(ANIM_SWAT_GUN,3);
	mframew(2);
	reaction_exit(obj);
}

void stat_do_reptile_inv(void) 
{
	init_special_act(ACT_INVISO,current_proc->pa8);

	animate2_a9(0x2,ANIM_REP_INV);

	create_fx(FX_INVISO_POOF);

	ochar_sound(9);			// poof sound
	process_sleep(10);
	set_inviso(current_proc->pa8);
	reaction_exit(current_proc->pa8);
}

void stat_do_jade_flash(void) 
{
	init_special(current_proc->pa8);
	CREATE(PID_JADE_FLASH,jade_flash_proc)->a10=(ADDRESS)current_proc;		// pass a10-jade proc
	update_tsl(l_flash,current_proc->pa8);
	reaction_exit(current_proc->pa8);
}

void jade_flash_proc(void) 
{
	short t_a9=10;			// flash count

	/* jadef3 */
	do
	{
		player_swpal((PROCESS *)current_proc->a10,white_p,current_proc->pa8);
		jade_flash_sleep();
		jade_normpal();
		jade_flash_sleep();

		ochar_sound(0xc);
		
	}
	while(--t_a9>0);

	process_suicide();
}

void jade_flash_sleep(void) 
{
	process_sleep(3);
	if (((PROCESS *)current_proc->a10)->pdata.p_flags & PM_REACTING) 
	{
		jade_normpal();
		process_suicide();
	}

	return;
}

void jade_normpal(void) 
{
	PROCESS *ta13;

	ta13=current_proc;
	current_proc=(PROCESS *)current_proc->a10;
	player_normpal();
	current_proc=ta13;
	return;
}

void stat_do_fan_lift(void) 
{
	OBJECT *obj=current_proc->pa8;

	init_special_act(ACT_FAN_LIFT,obj);

	get_char_ani(ANIM_TABLE2,ANIM_KIT_FANLIFT);
	mframew(4);

	init_anirate(1);
	set_no_block(obj);

	ochar_sound(2);

	current_proc->a10=0x40*3;
	current_proc->a11=1;

	/* spin3 */
	do
	{
		process_sleep(1);
		if (strike_check_a0(obj,0x12)==SYSTEM_CARRY_SET)
			break;

		if (--current_proc->a11==0) 
		{
			current_proc->a11=12;
			CREATE(PID_SONIC,wave_proc);
		}

		next_anirate();
	}
	while(--current_proc->a10>0);

	/* post_fan_lift */
	current_proc->a10=(f_block==0)?0x10:0x50;
	/* spin6 */
	do
	{
		process_sleep(1);
		
		if (--current_proc->a11==0) 
		{
			current_proc->a11=12;
			CREATE(PID_SONIC,wave_proc);
		}

		next_anirate();
	}
	while(--current_proc->a10>0 );

	get_char_ani(ANIM_TABLE2,ANIM_KIT_FANLIFT);
	find_part2();
	find_part2();
	mframew(4);
	back_to_normal();

	reaction_exit(obj);
}

void wave_proc(void) 
{
	OBJECT *obj=current_proc->pa8;

	(OBJECT *)current_proc->a11=current_proc->pa8;		// a11 - kitana
	get_char_ani(ANIM_TABLE2,ANIM_KIT_FANLIFT);
	find_part_a14(4);
	gso_dmawnz(obj,(ADDRESS)COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->pa9)),obj->oheap,0);
	alloc_cache((OIMGTBL *)COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->pa9)),obj->oheap,obj);

	lineup_1pwm(obj,(OBJECT *)current_proc->a11);
	insert_object(obj,&objlst);

	framew(4);
	delobjp(obj);
	process_suicide();
}

/******************************************************************************
 Function: void stat_do_babality(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	make a baby
******************************************************************************/
void stat_do_babality(void)
{
	init_special(current_proc->pa8);
	baby_start_pause();

	takeover_him(turn_into_a_baby);
	process_sleep(0x50);

	/* babality_complete */
	death_blow_complete();
	player_normpal();
	stack_jump(victory_animation);
}

void baby_start_pause(void)
{
	OBJECT *obj=current_proc->pa8;

	psxspu_start_cd_fade(FADE_TIME,0);

	init_death_blow(FAT_BABY);
	pose_a9(0,ANIM_STANCE);

	/* load babality file data */
	player_heap_adj();
	character_texture_load((current_proc->pdata.p_otherguy)->ochar,CHAR_BABY,finish_heap_ptr,ASYNC_LOAD|FATAL_LOAD); // want to load baby file for loser, hence otherguy

	/* sleep for fade */
	process_sleep(0x20);

#if _CD_VERSION_ == 1
	while ( psxcd_async_on() )
		process_sleep(1);				// what for async load to complete
	module_os_close(module_int);			// make sure to close async file
#endif

	/* load babality sound data */
	if ( obj==p1_obj )
		PsxSoundLoadFighter1Babality(p1_obj->ochar);
	else PsxSoundLoadFighter2Babality(p2_obj->ochar);

	f_no_bonus=0;								// allow bonus routine to do its thing
}

void turn_into_a_baby(void)
{
	OBJECT *obj=current_proc->pa8;

	tsound(0x8c);
	current_proc->a10=(ADDRESS)current_proc->pa8;

	/* change to winners heap, where baby data located */
	obj->oheap=finish_heap_ptr;									// pt to finishing heap

	create_fx(FX_INVISO_POOF);
	process_sleep(8);
	set_inviso(current_proc->pa8);

	get_char_ani(ANIM_TABLE2,ANIM_OCHAR_BABY);			// get ptr to ochar_babies table
	current_proc->pa9=(void*)COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->pa9));	// get ptr to actual image data
	gso_dmawnz(obj,(ADDRESS)current_proc->pa9,obj->oheap,0);
	alloc_cache((OIMGTBL *)current_proc->pa9,obj->oheap,obj);

	switch ((current_proc->pa8)->ochar)
	{
		case FT_JADE:
			swpal(BABYJAD_P,obj);
			break;
		case FT_MILEENA:
			swpal(BABYMIL_P,obj);
			break;
		case FT_SCORPION:
			swpal(BABYSCO_P,obj);
			break;
		case FT_REPTILE:
			swpal(BABYREP_P,obj);
			break;
		case FT_OLDSZ:
			swpal(BABYOSZ_P,obj);
			break;
		case FT_OLDSMOKE:
			swpal(BABYSMO_P,obj);
			break;
		case FT_NOOB:
			swpal(noob_P,obj);
			break;
	}

	obj->oxpos.u.intpos=((OBJECT*)current_proc->a10)->oxpos.u.intpos;
	obj->oypos.u.intpos=ground_y-obj->osize.u.ypos;

	insert_object(obj,&objlst);
	process_sleep(0x30);
	tsound(0x8d);				// cry
	wait_forever();
}

/******************************************************************************
 Function: void stat_do_roundhouse(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: 	None

 Returns: None

 Description:	perform a roundhouse kick
******************************************************************************/
void stat_do_roundhouse(void)
{
	OBJECT *obj=current_proc->pa8;

	init_special(obj);

	tsound(2);			// roundhouse whoosh
	group_sound(0);

	current_proc->pdata.p_stk=0x0d;

	if (striker(SI_STRIKER_NORMAL,3,ACT_RHOUSE,ANIM_RHOUSE,3,0x0d)==SYSTEM_CARRY_SET)
		process_sleep(10);						// hit --> hold this frame longer

	 retract_strike(4);

	return;
}

/******************************************************************************
 Function: void stat_do_sweep_kick(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: 	None

 Returns: None

 Description:	perform a sweep kick
******************************************************************************/
void stat_do_sweep_kick(void)
{
	OBJECT *obj=current_proc->pa8;

	init_special(obj);

	current_proc->pdata.p_stk=0x04;

	if (striker(SI_STRIKER_BEHIND,3,ACT_SWEEP,ANIM_SWEEP,1,0x04)==SYSTEM_CARRY_SET)
	{
		process_sleep(6);
		retract_strike_act(ACT_SWEEP_SD,6);			// flag: i am hittable
	}
	else retract_strike_act(ACT_SWEEP_SD,5);		// flag: i am hittable

	return;
}

/******************************************************************************
 Function: void stat_do_duck_punch(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: 	None

 Returns: None

 Description:	perform a duck punch
 ******************************************************************************/
void stat_do_duck_punch(void)
{
	OBJECT *obj=current_proc->pa8;

	group_sound(0);
	rsnd_whoosh();
	init_special(obj);

	current_proc->pdata.p_stk=0x05;

	striker(SI_STRIKER_NORMAL,3,ACT_DUCKPUNCH,ANIM_DUCKPUNCH,1,0x05);

	retract_strike(3);

	return;
}

/******************************************************************************
 Function: void stat_do_duck_kickh(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: 	None

 Returns: current_proc->a0:
 		  SYSTEM_CARRY_SET - kick hit
		  SYSTEM_CARRY_CLR - kick miss

 Description:	perform a duck high kick
******************************************************************************/
void stat_do_duck_kickh(void)
{
	OBJECT *obj=current_proc->pa8;

	group_sound(0);
	rsnd_whoosh();
	init_special(obj);

	current_proc->pdata.p_stk=0x06;

	current_proc->a0=striker(SI_STRIKER_NORMAL,3,ACT_DUCK_KICKH,ANIM_DUCKKICKH,5,0x06);
	return;
}

/******************************************************************************
 Function: void stat_do_duck_kickl(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: 	None

 Returns: None

 Description:	perform a duck low kick
******************************************************************************/
void stat_do_duck_kickl(void)
{
	OBJECT *obj=current_proc->pa8;

	group_sound(0);
	rsnd_whoosh();
	init_special(obj);

	current_proc->pdata.p_stk=0x07;

	striker(SI_STRIKER_NORMAL,2,ACT_DUCK_KICKL,ANIM_DUCKKICKL,2,0x07);

	return;
}

/******************************************************************************
 Function: void stat_do_uppercut(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: 	None

 Returns: None

 Description:	perform the massive uppercut blow
******************************************************************************/
void stat_do_uppercut(void)
{
	WORD result;

	OBJECT *obj=current_proc->pa8;

	init_special(obj);

	rsnd_big_whoosh();

	do_first_a9_frame(ANIM_UPPERCUT);
	process_sleep(1);
	do_next_a9_frame(obj);
	process_sleep(1);									// even FASTER uppercut !!!

	current_proc->pdata.p_stk=0x08;

	result=striker(SI_STRIKER_UPPER,2,ACT_UPPERCUT,ANIM_UPPERCUT,1,0x08);

	current_proc->pdata.p_action=ACT_UPCUT_SD;			// signal: i am a sitting duck

	if (result==SYSTEM_CARRY_CLR)
		process_sleep(10);								// miss sleep
	else
	{
		if (f_block==1)
			process_sleep(18);							// blocked, so use blocked sleep time
		else
		{
			if (f_upcut_rec==1)
				process_sleep(0x4);
			else process_sleep(0x20);
		}
	}

	/* upcut4 */
	if (winner_status==0)
	{
		/* upcut_wait */
		do
		{
			process_sleep(1);
			if (get_his_action(current_proc)==ACT_FROZEN || get_his_action(current_proc)==ACT_HOVER || f_upcut_rec!=0)
				break;
		}
		while(distance_off_ground(current_proc->pdata.p_otherproc,current_proc->pdata.p_otherguy)>SCY(0x50));
	}

	/* upcut8 */
	act_mframew(ACT_UPCUT_SD,4);

	back_to_normal();

	return;
}

/******************************************************************************
 Function: void stat_do_lo_kick(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: 	None

 Returns: None

 Description:	perform a lo kick
******************************************************************************/
void stat_do_lo_kick(void)
{
	OBJECT *obj=current_proc->pa8;

	init_special(obj);

	rsnd_big_whoosh();

	group_sound(0);

	current_proc->pdata.p_stk=0x01;

	kick2(1,ACT_LOKICK,ANIM_MEDKICK,1);

	return;
}

/******************************************************************************
 Function: void kick2(WORD anispeed,WORD actnum,WORD anioff,WORD stkoff)

 By: David Schwartz

 Date: Nov 1994

 Parameters: 	anispeed - animation speed
				actnum - action number
				anioff - animation offset
				strkoff - strike table offset

 Returns: None

 Description:	generic kick2 routine
******************************************************************************/
void kick2(WORD anispeed,WORD actnum,WORD anioff,WORD stkoff)
{
	if (striker(SI_STRIKER_NORMAL,anispeed,actnum,anioff,6,stkoff) == SYSTEM_CARRY_SET)
		process_sleep(0xc);

	retract_strike_act(ACT_RET_KICK,3);

	return;
}

/******************************************************************************
 Function: void stat_do_hi_kick(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: 	None

 Returns: None

 Description:	perform a high kick
******************************************************************************/
void stat_do_hi_kick(void)
{
	OBJECT *obj=current_proc->pa8;

	init_special(obj);

	current_proc->pdata.p_stk=0x00;

	rsnd_big_whoosh();

	group_sound(0);

	kick2(1,ACT_HIKICK,ANIM_HIKICK,0);

	return;
}

/******************************************************************************
 Function: void stat_do_kano_swipe(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	do kano swipe attack
******************************************************************************/
void stat_do_kano_swipe(void)
{

	OBJECT *obj=current_proc->pa8;

	init_special_act(ACT_KANO_SWIPE,obj);
	ochar_sound(0);

	current_proc->pdata.p_power=POW_FLIPKP;			// I am above flip kicks/punches

	current_proc->pdata.p_stk=0x12;				// define my stk table offset

	get_char_ani(ANIM_TABLE2,ANIM_KANO_SKNIFE);
	mframew(2);
	strike_check_a0(obj,0x12);

	current_proc->pdata.p_action=ACT_KSWIPE_SD;
	mframew(4);

	local_zero_power;

	process_sleep(0x08);

	mframew(4);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void stat_do_leg_throw(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	do sonya's leg throw
******************************************************************************/
void stat_do_leg_throw(void)
{
	OBJECT *obj=current_proc->pa8;
	PROCESS *ptemp;

	init_special_act(ACT_LEG_GRAB,obj);

	current_proc->pdata.p_stk=0x10;			// tell da world

	rsnd_big_whoosh();			// sound = deep whoosh sound

	get_char_ani(ANIM_TABLE2,ANIM_SONYA_LEG);

	animate_a0_frames(0x0002,0x0003);

	mframew(1);

	if (q_is_he_a_boss()==SYSTEM_CARRY_SET || strike_check_a0(obj,0x10)==SYSTEM_CARRY_CLR)
	{
		/* leg_miss */
		post_leg_sleep(0x10);
	}

	current_proc->pdata.p_action=ACT_LEG_SD;		// i am a sitting duck

	clear_inviso(current_proc->pdata.p_otherguy);

	if (f_block!=0)
	{
		rsnd_big_block();
		post_leg_sleep(0x18);
	}

	/* success */
	update_his_hit_queue(ACT_LEG_GRAB);

	/* calla_for_him */
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	face_opponent(current_proc->pa8);
	current_proc=ptemp;

	rsnd_big_block();
	his_group_sound(2);				// him: wasted voice

	f_norepell=0x060;							// disable repells

	xfer_him_to_flipped_pause(ACT_LEG_GRAB);
	inc_his_p_hit();

	current_proc->a11=get_his_char_ani(ANIM_TABLE1,ANIM_LEGGED);		// a11 = his ani
	current_proc->a10=(ADDRESS)current_proc->pdata.p_otherguy;			// a10 = his obj

	get_char_ani(ANIM_TABLE2,ANIM_SONYA_LEG);
	find_part2();

	double_next_a9();							// 1st frame for both

	shake_him_up(current_proc->pdata.p_otherguy,0x0000,0x0003,3,5);

	double_mframew(4);							// animate together

	takeover_him(r_leg_slammed);

	mframew(5);									// finish slam animation

	reaction_exit(obj);
}

/******************************************************************************
 Function: void post_leg_sleep(WORD sleeptime)

 By: David Schwartz

 Date: Nov 1994

 Parameters: sleeptime - time to sleep

 Returns: None

 Description:	after leg attack miss/block code
******************************************************************************/
void post_leg_sleep(WORD sleeptime)
{
	current_proc->pdata.p_action=ACT_LEG_SD;

	process_sleep(sleeptime);

	get_char_ani(ANIM_TABLE2,ANIM_SONYA_LEG);
	find_part2();

	mframew(5);								// undo ani

	mframew(5);								// undo ani

	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void r_leg_slammed(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	reaction code to being leg slammed
******************************************************************************/
void r_leg_slammed(void)
{
	OBJECT *obj=current_proc->pa8;

	current_proc->pdata.p_action=ACT_LEGGED;

	ground_player(obj);

	find_ani_part2(ANIM_KDOWN);					// knock down on back

	do_next_a9_frame(obj);

	group_sound(2);				// group speed: wasted

	shake_n_sound();

	current_proc->a10=(current_proc->pdata.p_hit<=3)?35:(35>>2);
	damage_to_me(current_proc->a10,obj);
	current_proc->pdata.p_damage+=current_proc->a10;

	flight(obj,SCX(0x30000),-SCY(0x60000),SCY(0x8000),4,-1);

	land_on_my_back();
}

/******************************************************************************
 Function: void stat_do_inviso(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	turn invisable
******************************************************************************/
void stat_do_inviso(void)
{
	init_special(current_proc->pa8);

	get_char_ani(ANIM_TABLE2,ANIM_ROBO_INVISO);
	mframew(2);

	ochar_sound(0xa);				// make an epxlo sound

	create_fx(FX_INVISO_POOF);
	process_sleep(6);

	if ((current_proc->pa8)->oflags2 & M_INVISO)
		do_un_inviso();
	else
	{
		set_inviso(current_proc->pa8);
		clear_shadow_bit(current_proc->pa8);
	}

	reaction_exit(current_proc->pa8);
}

void do_un_inviso(void)
{
	clear_inviso(current_proc->pa8);
	set_shadow_bit(current_proc->pa8);
	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void stat_do_quake(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	do jax's earth quake
******************************************************************************/
void stat_do_quake(void)
{
	OBJECT *obj=current_proc->pa8;

	init_special_act(ACT_QUAKE,obj);				// i can't block

	get_char_ani(ANIM_TABLE2,ANIM_JAX_QUAKE);

	mframew(4);

	process_sleep(6);			// hold that fist up jax !!!

	tsound(2);				// roundhouse whoosh
	group_sound(0);				// whoosh

	mframew(3);					// wham !!

	if (is_he_airborn()==SYSTEM_CARRY_CLR && q_is_he_a_boss()==SYSTEM_CARRY_CLR)
	{
		/* guy was on ground, damage time */
		damage_to_him(0x15,current_proc->pdata.p_otherguy);
		takeover_him(r_quake);
	}

	/* quake3*/
	shake_a11(0x0009,0x000e);			// earthquake

	ochar_sound(4);						// boom sound

	current_proc->pdata.p_action=ACT_PROJ_SD;

	mframew(3);

	delete_slave();

	do_backup();

	reaction_exit(obj);
}

/******************************************************************************
 Function: void stat_do_noogy(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	do jax's noogy punch
******************************************************************************/
void stat_do_noogy(void)
{
	OBJECT *obj=current_proc->pa8;
	int loop;

	init_special(obj);

	current_proc->pdata.p_stk=0x14;			// define my stk table

	group_sound(1);				// voice: jump

	get_char_ani(ANIM_TABLE2,ANIM_JAX_NOOGY);

	act_mframew(ACT_NOOGY,3);				// reach out and grab someone

	/* noog3 */
	loop=6;
	do
	{
		if (strike_check_a0(obj,0x14)==SYSTEM_CARRY_SET)
		{
			if (f_block==0)
				noogy_hit(obj);
			else noogy_blocked(obj);
		}
		process_sleep(1);
	}
	while(--loop>0);

	noogy_blocked(obj);
}

/******************************************************************************
 Function: void noogy_blocked(OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - dude who was blocked

 Returns: None

 Description:	jax noogy punch blocked
******************************************************************************/
void noogy_blocked(OBJECT *obj)
{
	lights_on_hit();

	current_proc->pdata.p_action=ACT_NOOGY_SD;
	process_sleep(0x0d);

	get_char_ani(ANIM_TABLE2,ANIM_WALKF);
	current_proc->a10=(ADDRESS)current_proc->pa9;
	((ADDRESS *)current_proc->pa9)++;
	do_next_a9_frame(obj);
	process_sleep(6);
	(ADDRESS)current_proc->pa9=current_proc->a10;
	do_next_a9_frame(obj);
	process_sleep(6);

	reaction_exit(obj);
}


/******************************************************************************
 Function: void noogy_hit(OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - dude who hit

 Returns: None

 Description:	jax noogy punch hit
******************************************************************************/
void noogy_hit(OBJECT *obj)
{
	PROCESS *ptemp,*pother=current_proc->pdata.p_otherproc;
	ADDRESS animtemp,animframe;

	lights_on_hit();

	ochar_sound(6);

	me_in_front();

	takeover_him(noogy_suspended);

	animtemp=(ADDRESS)current_proc->pa9;		// save current process animptr

	current_proc->a11=(ADDRESS)current_proc->pa9=get_his_char_ani(ANIM_TABLE1,ANIM_HITHI);
	((ADDRESS *)current_proc->pa9)++;
	do_his_next_a9_frame(pother->pa8);				// hit hi frame 3

	(ADDRESS)current_proc->pa9=animtemp;		// restore current process animptr

	edge_of_world_lineup(noog_lineup_1);

	shake_him_up(current_proc->pdata.p_otherguy,0x0003,0x0003,3,3);

	adjust_him_xy(SCX(0x0000c),SCY(0));
	do_next_a9_frame(obj);
	process_sleep(4);

	adjust_him_xy(SCX(0x000c),SCY(0x00005));
	do_next_a9_frame(obj);
	process_sleep(4);


	pose_him_a0(ANIM_HITHI);
	do_next_a9_frame(obj);
	process_sleep(4);


	/* pow pow pow !!! */
	current_proc->pdata.p_store1=0;			// flag: no exit

	/* noog5 */
	current_proc->pdata.p_anicount=5;

	while (1)
	{
		mframew(3);							// wham
		add_combo_damage(7);
		damage_to_him(0x7,pother->pa8);
		inc_his_p_hit();

		/* fx and sound !!! */
		his_group_sound(2);					// him: wasted voice
		tsound(0x83);
		ochar_sound(0xc);

		/* calla_for_him */
		ptemp=current_proc;
		current_proc=current_proc->pdata.p_otherproc;
		upcut_blood_me();
		current_proc=ptemp;

		ptemp=CREATE(PID_FX,noogy_shaker);
		ptemp->pdata.p_otherguy=current_proc->pdata.p_otherguy;	// mod from arcade, save a step

		if (current_proc->pdata.p_anicount==1 || current_proc->pdata.p_store1!=0)
			break;							// not tapping --> exit

//*********************
		animtemp=current_proc->a11;
		animframe=(ADDRESS)current_proc->pa9;
		(ADDRESS)current_proc->pa9=current_proc->a11;
		do
		{
			do_his_next_a9_frame(current_proc->pdata.p_otherguy);
			process_sleep(2);
		}
		while(GET_LONG(current_proc->pa9)!=0);
		current_proc->a11=animtemp;
		(ADDRESS)current_proc->pa9=animframe;
//*********************

		noogy_early_check(obj);
		process_sleep(2);

		noogy_early_check(obj);
		process_sleep(2);

		current_proc->pdata.p_anicount--;
	}

	/* noogy_last_hit */
	xfer_otherguy(r_last_noogy);
	dec_his_p_hit;
	process_sleep(0x10);
	animate_a0_frames(0x0004,0x0004);

	reaction_exit(obj);
}

void upcut_blood_me(void) 
{
	create_blood_proc(BLOOD_UPPERCUT);
	return;
}

/******************************************************************************
 Function: inline void noogy_suspended(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	suspend otherguy, he is being noogied
******************************************************************************/
void noogy_suspended(void)
{
	do
	{
		f_norepell=4;
		process_sleep(2);
	}
	while ((current_proc->pdata.p_otherproc)->pdata.p_action==ACT_NOOGY);

	reaction_exit(current_proc->pa8);

}

/******************************************************************************
 Function: inline void noogy_early_check(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	check to see if noogy over with early
******************************************************************************/
inline void noogy_early_check(OBJECT *obj)
{
	if (get_tsl(l_lp,obj)>=8)
		current_proc->pdata.p_store1=1;			// flag: exit
	return;
}

/******************************************************************************
 Function: void noogy_shaker(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	shaker process for being noogied
******************************************************************************/
void noogy_shaker(void)
{
	shake_a11(0x0009,0x0006);

	shake_him_up(current_proc->pdata.p_otherguy,0x0005,0x0000,3,4);

	process_suicide();
}

short ochar_noogy_lineups[]=
{
	-SCX(0x54),-SCY(0x08),
	-SCX(0x48),-SCY(0x10),
	-SCX(0x48),-SCY(0x10),
	-SCX(0x48),-SCY(0x10),
	-SCX(0x48),-SCY(0x10),

	-SCX(0x48),-SCY(0x10),
	-SCX(0x48),-SCY(0x10),
	-SCX(0x48),-SCY(0x10),
	-SCX(0x48),-SCY(0x10),
	-SCX(0x48),SCY(00),

	-SCX(0x48),-SCY(0x10),
	-SCX(0x48),-SCY(0x10),
	-SCX(0x48),-SCY(0x10),
	-SCX(0x48),-SCY(0x10),
	-SCX(0x48),-SCY(0x10),

	-SCX(0x48),-SCY(0x10),
	-SCX(0x48),-SCY(0x10),
	-SCX(0x48),-SCY(0x10),

	-SCX(0x58),-SCY(0x10),
	-SCX(0x58),-SCY(0x10),
	-SCX(0x58),-SCY(0x10),
	-SCX(0x58),-SCY(0x10),
	-SCX(0x58),-SCY(0x10),
	-SCX(0x58),-SCY(0x10),

	-SCX(0x48),-SCY(0x10),
	-SCX(0x4b),-SCY(0x1a)
};

/******************************************************************************
 Function: void noogy_shaker(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	shaker process for being noogied
******************************************************************************/
void noog_lineup_1(void)
{
	match_him_with_me_f();
	adjust_him_xy(ochar_noogy_lineups[current_proc->pa8->ochar*2],ochar_noogy_lineups[current_proc->pa8->ochar*2+1]);
	return;
}

/******************************************************************************
 Function: void edge_of_world_lineup(FUNC routine)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	lineup noogy, keeping edge of world in account
******************************************************************************/
void edge_of_world_lineup(FUNC routine)
{
	OBJECT *obj=current_proc->pdata.p_otherguy;
	int loop;

	routine();

	if (abs(obj->oxpos.u.intpos-left_edge)<EOW_LX || abs(obj->oxpos.u.intpos-SCRRGT-right_edge)<EOW_RX)
	{
		away_x_vel(current_proc,current_proc->pa8,SCX(0x60000));

		loop=8;
		do
		{
			process_sleep(1);
			routine();
		}
		while(--loop>0);

		stop_me(current_proc->pa8);
		routine();
	}

	return;
}

/******************************************************************************
 Function: void stat_do_shake(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	do kano shake
******************************************************************************/
void stat_do_shake(void)
{
	int loop;

	OBJECT *obj=current_proc->pa8;

	init_special_act(ACT_SHAKE,obj);

	current_proc->pdata.p_stk=0x10;			// define my stk table offset

	animate2_a9(0x0003,ANIM_KANO_SHAKE);

	/* shake3 */
	loop=6;
	do
	{
		if (q_is_he_a_boss()==SYSTEM_CARRY_CLR)
		{
			if (strike_check_a0(obj,0x10)==SYSTEM_CARRY_SET)
			{
				if (f_block!=0)
					break;
				else shake_hit(obj);
			}
		}

		process_sleep(1);
	}
	while(--loop>0);

	/* shake_blocked */
	lights_on_hit();
	process_sleep(10);

	get_char_ani(ANIM_TABLE2,ANIM_KANO_SHAKE);
	backwards_ani(5,-1);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void shake_hit(OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - kano dude

 Returns: None

 Description:	kano shake hit code
******************************************************************************/
void shake_hit(OBJECT *obj)
{
	int loop;
	ADDRESS anif1,anif2;
	LONG *idptr;

	lights_on_hit();

	takeover_him(shake_suspended);

	idptr=idcomp_ptr;					// prevent overflow, this frame wont be displayed so this is ok
	match_him_with_me_f();
	idcomp_ptr=idptr;

	me_in_front();

	get_his_a11_ani(ANIM_SHOOK);

	his_group_sound(8);

	double_mframew(5);							// lift up 2-gether

	/* shake4 */
	loop=4;
	do
	{
		anif1=(ADDRESS)current_proc->pa9;
		anif2=(ADDRESS)current_proc->a11;

		double_mframew(3);

		(ADDRESS)current_proc->pa9=anif1;
		(ADDRESS)current_proc->a11=anif2;

		damage_to_him(8,current_proc->pdata.p_otherguy);
	}
	while(--loop>0);

	double_next_a9();
	process_sleep(3);

	double_next_a9();
	process_sleep(3);

	double_next_a9();

	xfer_him_to_a10_r(R_A10_SHAKE);

	process_sleep(0x08);				// hold with arms extended

	get_char_ani(ANIM_TABLE2,ANIM_KANO_SHAKE);

	backwards_ani(2,-1);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void shake_suspended(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	kano shake suspend code
******************************************************************************/
void shake_suspended(void)
{
	do
	{
		f_norepell=4;
		process_sleep(2);
	}
	while ((current_proc->pdata.p_otherproc)->pdata.p_action==ACT_SHAKE);

	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void stat_do_lao_spin(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	lao spin attack
******************************************************************************/
void stat_do_lao_spin(void)
{
	WORD pa0;

	init_special_act(ACT_LAOSPIN,current_proc->pa8);

	get_char_ani(ANIM_TABLE2,ANIM_LAO_SPIN);
	init_anirate(1);
	current_proc->a11=1;

	/* hhspin3, spin without need to tap button */
	current_proc->a10=1;
	pa0=6;
	do
	{
		process_sleep(1);
		next_lao_anirate();
	}
	while(--pa0>0);

	/* hhspin4, now you gotta tap */
	do
	{
		process_sleep(1);
		next_lao_anirate();
		if (get_tsl(l_run,current_proc->pa8)>0xe)		// still tapping
			break;
	}
	while(strike_check_a0(current_proc->pa8,0x13)==SYSTEM_CARRY_CLR);

	/* hhspin_hit */
	if (f_block) 
	{
		set_no_block(current_proc->pa8);
		current_proc->a11=0x30;
	}
	else current_proc->a11=0x20;

	/* lao_spin_not_blocked */
	init_anirate(3);

	/* hhspin7 */
	do
	{
		process_sleep(1);
		next_lao_anirate();
	}
	while(--current_proc->a11>0);

	reaction_exit(current_proc->pa8);
}

void next_lao_anirate(void) 
{
	next_anirate();
	if (--current_proc->a10>0) 
	{
		ochar_sound(6);
		current_proc->a10=30;
	}

	return;
}

/******************************************************************************
 Function: void stat_do_lia_scream(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	lia scream attack
******************************************************************************/
void stat_do_lia_scream(void)
{
	short la11,la0;
	PROCESS *ptemp;
	OBJECT *obj=current_proc->pa8;

	init_special_act(ACT_LIA_SCREAM,obj);

	get_char_ani(ANIM_TABLE2,ANIM_LIA_SCREAM);

	mframew(3);								// lean forward

	if (get_his_action(current_proc)==ACT_SCREAMED)
		stack_jump(r_scream);						// double scream = I am screamed

	current_proc->pdata.p_store2=0;			// flag: no hit yet
	init_anirate(3);

	current_proc->pdata.p_store3=1;			// sound call timer
	ptemp=create_fx(FX_SCREAM_SHAKER);
	(PROCESS *)ptemp->a10=current_proc;

	la11=1;
	la0=0x50;

	/* scream3 */
	do
	{
		current_proc->p_comp_flag=PC_SCREAM_WAKE;			// flag to indicate @ scream wake
		process_sleep(1);
		current_proc->p_comp_flag=PC_CLEAR;

		/* scream_wake */
		next_anirate();

		if (--current_proc->pdata.p_store3==0)
		{
			ochar_sound(1);				// sound call for scream
			current_proc->pdata.p_store3=0x58;
		}

		/* scream6 */
		if (current_proc->pdata.p_store2==0)
		{
			if (strike_check_a0(obj,0x11)==SYSTEM_CARRY_SET)
			{
				if (f_block==0)
				{
					current_proc->pdata.p_store2=1;		// flag hit
					la0=0x20;							// wrap things up
					dec_his_p_hit;
				}
				else
				{
					/* screamed_blocked */
					set_no_block(obj);
					current_proc->a10=0x28;
					/* scblk3 */
					do
					{
						process_sleep(1);
						if (--current_proc->a11==0) 
						{
							current_proc->a11=0x10;
							CREATE(PID_SCREAM_WAVE,scream_wave);
						}
						next_anirate();
					}
					while(--current_proc->a10>0);
					break;			// should goto SCREAM5
				}
			}
		}
		/* scream2 */
		if (--la11==0)
		{
			la11=0x10;						// reset counter
			CREATE(PID_SCREAM_WAVE,scream_wave);
		}
	}
	/* scream4 */
	while(--la0>0);

	/* scream5 */
	get_char_ani(ANIM_TABLE2,ANIM_LIA_SCREAM);
	find_part2();
	find_part2();
	delete_slave();

	mframew(3);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void scream_wave(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	scream wave
******************************************************************************/
void scream_wave(void)
{
	OBJECT *obj;
	PROCESS *ptemp;

	obj=(OBJECT *)current_proc->a10=current_proc->pa8;			// a10 = my lia

	//(LONG)current_proc->pa9=(LONG)COMPUTE_ADDR(obj->oheap ,(*(lia_anitab2+1)));
	get_char_ani(ANIM_TABLE2,ANIM_LIA_SHOCK);

	ptemp=gmo_proc((current_proc->pa8)->oheap);

	obj=current_proc->pa8;
	obj->oslink=ptemp;			// link me

	obj->ozval=FRONT_Z+1;

	match_ani_points((OBJECT *)current_proc->a10,obj);			// lineup wave with LIA

	insert_object(obj,&objlst);

	mframew(5);

	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void stat_do_fast_axe_up(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	indian fast axe up
******************************************************************************/
void stat_do_fast_axe_up(void)
{
	set_half_damage(current_proc->pdata.p_otherguy);
	axeup3(0x0002,ANIM_IND_AXE_UP,4);
}

/******************************************************************************
 Function: void stat_do_axe_up(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	indian axe up
******************************************************************************/
void stat_do_axe_up(void)
{
	axeup3(0x0003,ANIM_IND_AXE_UP,10);
}

/******************************************************************************
 Function: void axeup3(WORD pa9hi,WORD pa9lo,WORD pa11)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa9hi = [32..16] - anim rate
			 pa9lo = [15..0] - anim #
			 pa11 - sleep time

 Returns: None

 Description:	indian axe up common routine
******************************************************************************/
void axeup3(WORD pa9hi,WORD pa9lo,WORD pa11)
{
	OBJECT *obj=current_proc->pa8;

	init_special_act(ACT_AXE_UP,obj);

	ochar_sound(5);

	current_proc->pdata.p_power=POW_FLIPKP;		// I am above flip kicks/punches

	animate2_a9(pa9hi,pa9lo);

	current_proc->pdata.p_store1=0;			// no hit yet
	init_anirate(3);

	/* asc2 */
	do
	{
		process_sleep(1);
		if (current_proc->pdata.p_store1==0)
		{
			if (strike_check_a0(obj,0x15)==SYSTEM_CARRY_SET)
				current_proc->pdata.p_store1=1;
		}

		/* asc3 */
		next_anirate();
	}
	while(GET_LONG(current_proc->pa9)!=0);

	current_proc->pdata.p_action=ACT_SSD;

	local_zero_power;

	process_sleep(pa11);			// fast or slow sleep

	delete_slave();					// no need for glow no more

	((ADDRESS*)current_proc->pa9)++;

	mframew(2);

	reaction_exit(obj);
}

#if 0
/******************************************************************************
 Function: void do_axe_horz(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	indian axe horz
******************************************************************************/
void do_axe_horz(void)
{
	OBJECT *obj=current_proc->pa8;

	init_special_act(ACT_AXE_HORZ,obj);

	tsound(0);
	animate2_a9(0x0003,ANIM_IND_HORZ);

	strike_check_a0(obj,0x13);

	mframew(3);

	process_sleep(10);

	delete_slave();

	mframew(2);

	reaction_exit(obj);
}
#endif

/******************************************************************************
 Function: void stat_do_reflect(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	indian reflect
******************************************************************************/
void stat_do_reflect(void)
{
	OBJECT *obj=current_proc->pa8;

	init_special_act(ACT_REFLECT,obj);

	ochar_sound(7);

	get_char_ani(ANIM_TABLE2,ANIM_IND_REFLECT);

	mframew(5);
	mframew(5);

	current_proc->pdata.p_action=ACT_REF_SD;

	delete_slave();

	find_ani2_part2(ANIM_IND_REFLECT);
	find_part2();

	mframew(4);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void xfer_him_to_a10_r(WORD offset);

 By: David Schwartz

 Date: Nov 1994

 Parameters: offset - transfer to current_proc->a10

 Returns: None

 Description:	transfer other fighter to r_a10_offset routine
******************************************************************************/
void xfer_him_to_a10_r(WORD offset)
{
	(current_proc->pdata.p_otherproc)->a10=offset;
	takeover_him(r_a10_offset);

	return;
}

/******************************************************************************
 Function: void retract_strike_act(WORD actnum,WORD sleeptime)

 By: David Schwartz

 Date: Oct 1994

 Parameters: actnum - action number
			 sleeptime - animation sleep rate

 Returns: None

 Description:	retract an attack and clean up fighter state
******************************************************************************/
void retract_strike_act(WORD actnum,WORD sleeptime)
{
	/* act_mframew */
	act_mframew(actnum,sleeptime);

	back_to_normal();
	return;
}



/******************************************************************************
		STANDARD MOVES
******************************************************************************/
/******************************************************************************
 Function: void do_unblock_hi(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	exit from standing block (hi block)
******************************************************************************/
void do_unblock_hi(void)
{
	ADDRESS *temp;

	current_proc->pdata.p_action=0;

	find_ani_last_frame(ANIM_STANDBLOCK);

	((ADDRESS *)current_proc->pa9)--;			// a9 --> block frame 2
	temp=(ADDRESS *)current_proc->pa9-1;		// temo --> block frame 1

	do_next_a9_frame(current_proc->pa8);
	process_sleep(4);
	(ADDRESS *)current_proc->pa9=temp;
	do_next_a9_frame(current_proc->pa8);
	process_sleep(4);

	return;
}

/******************************************************************************
 Function: void do_block_hi(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	perform standing block (hi block)
******************************************************************************/
void do_block_hi(void)
{
	stop_me(current_proc->pa8);

	get_char_ani(ANIM_TABLE1,ANIM_STANDBLOCK);

	/* act_mframew */
	act_mframew(ACT_BLOCKHI,3);					// block stance
}

/******************************************************************************
 Function: void turn_around(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	turn fighter around
******************************************************************************/
void turn_around(void)
{
	stop_me(current_proc->pa8);
	disable_all_buttons;

	get_char_ani(ANIM_TABLE1,ANIM_TURN);

	mframew(2);
#if 0
	mframew(ta_speeds[current_proc->pa8->ochar][0]);

	if (ta_speeds[current_proc->pa8->ochar][1]!=0)
		process_sleep(ta_speeds[current_proc->pa8->ochar][0]);
#endif
}

#if 0
/* "turn around" speeds */
WORD ta_speeds[][2] = {
	{2,0},				// 0
	{2,0},				// 1
	{2,0},				// 2
	{2,0},				// 3
	{2,0},				// 4
	{2,0},				// 5
	{2,0},				// 6
	{2,0},				// 7
	{2,0},				// 8
	{2,0},				// 9
	{2,0},				// 10
	{2,0},				// 11
	{2,0},				// 12
	{2,0},				// 13
	{2,0},				// 14
	{2,0},				// 15 - motaro
	{2,0},				// 16 - sk
	{2,0},				// 17 - noob
	{2,0},				// 17 - noob
	{2,0},				// 17 - noob
	{2,0},				// 17 - noob
	{2,0},				// 17 - noob
	{2,0},				// 17 - noob
	{2,0},				// 17 - noob
	{2,0},				// 17 - noob
	{2,0},				// 17 - noob
	{2,0}				// 18 -
};
#endif

/******************************************************************************
 Function: void duck_turnaround(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	turn fighter around animation
******************************************************************************/
void duck_turnaround(void)
{
	get_char_ani(ANIM_TABLE1,ANIM_DUCKTURN);

	mframew(2);

	return;
}

/******************************************************************************
 Function: void do_jumpup_kick_punch(WORD jtype)

 By: David Schwartz

 Date: Nov 1994

 Parameters: 	jtype - punch routine (JUP_PUNCH) || kick routine (JUP_KICK)

 Returns: None

 Description:	jump up punch or kick routine
******************************************************************************/
void do_jumpup_kick_punch(WORD jtype)
{
	OBJECT *obj=current_proc->pa8;
	WORD results;

	switch (jtype)
	{
		case JUP_PUNCH:
			current_proc->pdata.p_action=ACT_JUPPUNCH;
			face_opponent(obj);
			group_sound(0);
			rsnd_whoosh();

			current_proc->pdata.p_stk=0x09;									// strike table offset

			current_proc->a4=POW_FLIPKP;
			results=air_strike(0x09,15,3,UP_GRAV,ANIM_FLIPPUNCH);
			break;
		case JUP_KICK:
			current_proc->pdata.p_action=ACT_JUPKICK;
			face_opponent(obj);
			group_sound(0);
			rsnd_whoosh();

			current_proc->pdata.p_stk=0x0a;									// strike table offset

			current_proc->a4=POW_FLIPKP;
			results=air_strike(0x0a,16,3,UP_GRAV,ANIM_JUMPUPKICK);
			break;
	}

	if (results==SYSTEM_CARRY_SET)
	{
		combo_air_pause(8);

		/* hit routine */
		current_proc->pdata.p_action=ACT_HITFALL_SD;		// action: falling to ground !

		/* stop in midair */
//		grav.pos=obj->ograv.pos;
//		yvel.pos=obj->oyvel.pos;
//		obj->oyvel.pos=0;
//		obj->ograv.pos=0;
//		process_sleep(8);
//		obj->ograv.pos=grav.pos;
//		obj->oyvel.pos=yvel.pos;

		((ADDRESS *)current_proc->pa9)++;					// pa9 --> jump kick finish animation

		current_proc->pdata.p_store4=NULL;						// no more routine callin'
		obj->ograv.pos=SCY(0xe000);
		flight_loop(obj);
	}

	jump_up_land_jsrp();

	reaction_exit(obj);
}

/******************************************************************************
 Function: void do_flip_kick(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: 	None

 Returns: None

 Description:	flip in air kick
******************************************************************************/
void do_flip_kick(void)
{
	OBJECT *obj=current_proc->pa8;

	face_opponent(obj);
	rsnd_whoosh();
	group_sound(0);

	current_proc->pdata.p_stk=0x0b;

	current_proc->pdata.p_action=ACT_FLYKICK;

	current_proc->a4=POW_FLIPKP;
	if (air_strike(0x0b,15,3,ANGLE_GRAV,ANIM_FLIPKICK)==SYSTEM_CARRY_SET)
	{
		/* wham !*/
		update_l_block_fk();
		me_in_front();

		current_proc->pdata.p_action=ACT_FK_SD;			// flag: i'm vulnerable

		mid_air_pause(obj,8);								// pause ticks

		current_proc->pdata.p_store4=NULL;				// avoid double drone kicks
		obj->ograv.pos=SCY(0xc000);
		flight_loop(obj);
	}

	/* jfk8 */
	//current_proc->pdata.p_action=ACT_FK_SD;

	angle_jump_land_jsrp();

	return;
}

/******************************************************************************
 Function: void do_flip_punch(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: 	None

 Returns: None

 Description:	flip in air punch
******************************************************************************/
void do_flip_punch(void)
{
	OBJECT *obj=current_proc->pa8;

	face_opponent(obj);
	rsnd_whoosh();
	group_sound(0);

	current_proc->pdata.p_stk=0x0c;

	current_proc->pdata.p_action=ACT_FLYPUNCH;

	current_proc->a4=POW_FLIPKP;
	if (air_strike(0x0c,16,3,ANGLE_GRAV,ANIM_FLIPPUNCH)==SYSTEM_CARRY_SET)
	{
		/* pow !*/
		update_l_block_fk();
		combo_air_pause(8);								// pause ticks

		current_proc->pdata.p_action=ACT_FK_SD;			// flag: i'm vulnerable
		current_proc->pdata.p_store4=NULL;				// avoid double drone punches
		obj->ograv.pos=SCY(0xc000);
		flight_loop(obj);
	}

	current_proc->pdata.p_action=ACT_FK_SD;

	/* jfp8 */
	angle_jump_land_jsrp();

	return;
}

void update_l_block_fk(void) 
{
	if (f_block==1) 
		update_tsl(l_block_fk,current_proc->pdata.p_otherguy);
 
 	return;
}

void update_block_fk(void) 
{
	update_tsl(l_block_fk,current_proc->pa8);
 	return;
}

void combo_air_pause(WORD pa0) 
{
	OBJECT *obj=current_proc->pa8;
	short ta0;
	WORD *ta10,*ta11;

	current_proc->pdata.p_store1=f_block;
	stop_me(obj);
	clear_combo_butn();
	current_proc->a10=current_proc->pdata.p_anicount=pa0;
	process_sleep(4);

	if (is_he_airborn()==SYSTEM_CARRY_SET) 
	{
		cap_airborn();
		return;
	}

	if (am_i_joy()==SYSTEM_CARRY_CLR)
		cap_elbow();			// drone ==combo

	if (obj==p1_obj) 
	{
	ta11=&c_hp[0];
	ta10=&c_hk[0];
	}
	else
	{
	ta11=&c_hp[1];
	ta10=&c_hk[1];
	}

	/*
	 * a10 = kick button location
	 * a11 = punch button location 
	 */
	ta0=(short)current_proc->pdata.p_anicount-4;
	if (ta0<0)
		ta0=1;
	do
	{
		process_sleep(1);
		if (*ta11!=0)
			cap_elbow();

		if (*ta10!=0)
			cap_knee();
	}
	while(--ta0>0);
	return;
}

void cap_elbow(void) 
{
	air_combo_setup();
	do_elbow();
	reaction_exit(current_proc->pa8);
}

void cap_knee(void) 
{
	air_combo_setup();
	do_knee();
	reaction_exit(current_proc->pa8);
}

void cap_airborn(void) 
{
	short ta10;

	ta10=current_proc->a10;
	ta10-=4;
	if (ta10<0)
		ta10=1;
	(long)current_proc->a10=(long)ta10;
	process_sleep(ta10);
	return;
}

void air_combo_setup(void) 
{
	if (current_proc->pdata.p_store1==0) 
	{
		/* calla_for_him */
		set_no_block(current_proc->pdata.p_otherguy);
	}

	ground_player(current_proc->pa8);
	match_me_with_him();
	flip_multi(current_proc->pa8);
	multi_adjust_xy(current_proc->pa8,-SCX(0x40),0);

	return;
}

/******************************************************************************
 Function: WORD air_strike(WORD strkoff,WORD framecnt,WORD anispeed,long gravity,WORD anioff)

 By: David Schwartz

 Date: Nov 1994

 Parameters: strkoff - strike table offset (a0)
			 framecnt - # of frames to look for collision (a1)
 			 anispeed - animation speed rate (a2)
			 gravity - gravity (a3)
			 anioff - animation offset (a9)
			 current_proc->a4 = pwoer level which I cannot handle

 Returns: SYSTEM_CARRY_SET - hit
		  SYSTEM_CARRY_CLR - miss

 Description:	routine for making a strike animation while in the air
******************************************************************************/
WORD air_strike(WORD strkoff,WORD framecnt,WORD anispeed,long gravity,WORD anioff)
{
	OBJECT *obj=current_proc->pa8;

	obj->ograv.pos=gravity;								// set yer gravity
	current_proc->pdata.p_store2=current_proc->a4;

	init_anirate(anispeed);

	get_char_ani(ANIM_TABLE1,anioff);					// animation setup

AS2:
	process_sleep(1);
	next_anirate();

	if (GET_LONG(current_proc->pa9)==0)
	{
		if (current_proc->pdata.p_store2>(current_proc->pdata.p_otherproc)->pdata.p_power) 	// i have enough power to take them on
		{
	 		if (strike_check_a0(obj,strkoff)==SYSTEM_CARRY_SET)
	 			return(SYSTEM_CARRY_SET);
		}
	}

AS5:
		if ((--framecnt)==0)									// no hit, count this pass
		{
AS7:
			((ADDRESS*)current_proc->pa9)++;						// 2nd part of animation
			current_proc->pdata.p_action=ACT_POST_AIRSTK;
			current_proc->pdata.p_store4=NULL;					// dornes -> dont try to kick again
			flight_loop(obj);
			return(SYSTEM_CARRY_CLR);
		}

AS3:
		if (obj->oyvel.pos<0)								// head upwards = loop
			goto AS2;

		if (obj->oypos.u.intpos<current_proc->pdata.p_ganiy)
			goto AS2;										// am I on the ground

		stop_me(obj);
		ground_player(obj);									// landed attack failed
		return(SYSTEM_CARRY_CLR);
}

/******************************************************************************/
/******************************************************************************
 Function: void mid_air_pause(OBJECT *obj,WORD sleep)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - dude to pause
 			 sleep - time to sleep

 Returns: None

 Description:	pause a dude in the air for "sleep" ticks
******************************************************************************/
inline void mid_air_pause(OBJECT *obj,WORD sleep)
{
	stop_me(obj);
	process_sleep(sleep);
	return;
}

/***********************************************************************
 *
 * Jumps and Landings
 *
 ***********************************************************************/

/******************************************************************************
 Function: void angle_jump_land_jsrp(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: 	None

 Returns: None

 Description:	land from an angle jump
******************************************************************************/
void angle_jump_land_jsrp(void)
{
	current_proc->pdata.p_action=ACT_LAND;			// action: landing on the ground

	face_opponent(current_proc->pa8);
	tsound(0x18);

	get_char_ani(ANIM_TABLE1,ANIM_FFLIP);

	allow_moves();
	do_next_a9_frame(current_proc->pa8);

	process_sleep(3);		// allow moves during this sleep

	return;
}

/******************************************************************************
 Function: void land_on_yer_feet(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: 	None

 Returns: None

 Description:	land on your feet then normal retrun
******************************************************************************/
void land_on_yer_feet(void)
{
	flight(current_proc->pa8,0,0,SCY(0x8000),NEVER_ANI,-1);
	stack_jump(jump_up_land_jump);
}

/******************************************************************************
 Function: void jump_up_land_jump(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: 	None

 Returns: None

 Description:	jump up punch or kick land routine
******************************************************************************/
void jump_up_land_jump(void)
{
	jump_up_land_jsrp();
	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void do_jump_up(void *pa11)

 By: David Schwartz

 Date: Feb 1995

 Parameters:   routine - routine to call every tick NULL=None

 Returns: None

 Description:	joy drone jumpup routine
******************************************************************************/
void do_jump_up(void *pa11)
{
	group_sound(1);				// group speech: jump "huh"

	current_proc->pdata.p_action=ACT_JUMPUP;

	get_char_ani(ANIM_TABLE1,ANIM_JUMPUP);
	((ADDRESS *)current_proc->pa9)++;

	(void *)current_proc->a6=pa11;				// set call routine

	flight_call(current_proc->pa8,0,UP_VEL,UP_GRAV,4,-1);

	jump_up_land_jsrp();
	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void jump_up_land_jsrp(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: 	None

 Returns: None

 Description:	jump up punch or kick land routine
******************************************************************************/
void jump_up_land_jsrp(void)
{
	current_proc->pdata.p_action=ACT_LAND;
	tsound(0x18);						// feet gently landing

	get_char_ani(ANIM_TABLE1,ANIM_JUMPUP);

	if (am_i_joy()==SYSTEM_CARRY_CLR)
		stack_jump(d_land);

	allow_moves();
	current_proc->a10=(ADDRESS)current_proc->pa9;
	((ADDRESS *)current_proc->pa9)++;
	do_next_a9_frame(current_proc->pa8);
	process_sleep(3);
	(ADDRESS)current_proc->pa9=current_proc->a10;
	do_next_a9_frame(current_proc->pa8);
	process_sleep(3);

	return;
}

/******************************************************************************
 Function: void allow_moves(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: 	None

 Returns: None

 Description:	allow moves again
******************************************************************************/
void allow_moves(void)
{
	enable_all_buttons;
	back_to_normal();
	return;
}

