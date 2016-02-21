/******************************************************************************
 File: mkboss.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Boss Routines
******************************************************************************/

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
#include "mkjoy.h"
#include "mkpal.h"
#include "mkguys.h"
#include "mkboss.h"
#include "mkslam.h"
#include "mkdrone.h"
#include "mkfatal.h"
#include "moves.h"
#include "mkblood.h"
#include "mkmain.h"
#include "mkfx.h"
#include "mkladder.h"
#include "mksound.h"
#include "valloc.h"

JUMPTBL motaro_branches[] =
{
	motaro_hit0,		// 0 - not ever called
	motaro_hit1,		// 1
	motaro_hit2,		// 2
	motaro_hit3,		// 3
	motaro_upcutted,		// 4
	motaro_comboed,		// 5
	motaro_stumble,		// 6
	motaro_slided,		// 7
	motaro_flip_kicked,	// 8
	motaro_hard_comboed,	// 9
	boss_stalk,		// a
	boss_stalk,		// b
	boss_stalk,		// c
	boss_stalk,		// d
	boss_stalk,		// e
	boss_stalk,		// f
	boss_stalk,		// 10
	boss_finish_him,		// 11
	motaro_collapse		// 12
};

JUMPTBL sk_branches[]=
{
	motaro_hit0,			// 0 - not ever called
	sk_hit1,				// 1
	motaro_hit2,			// 2
	sk_hit3,				// 3
	sk_uppcutted,			// 4
	sk_comboed,			// 5
	sk_stumble,			// 6
	sk_slided,			// 7
	boss_stalk,			// 8
	sk_hard_comboed,			// 9
	boss_stalk,			// a
	boss_stalk,			// b
	boss_stalk,			// c
	boss_stalk,			// d
	boss_stalk,			// e
	boss_stalk,			// f
	sk_stalk,			// 10 - used !!
	boss_finish_him,			// 11
	sk_collapse			// 12
};

/******************************************************************************
 Function: void boss_stalk(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  boss stalk other dude
******************************************************************************/
void boss_stalk(void)
{
	if (q_is_he_car()==SYSTEM_CARRY_SET) 			// car = corned and reacting
	{
		if (randper(300)==SYSTEM_CARRY_SET)			// chance of laughing like a smart ass
		{
			stack_jump(sk_laugh);
		}

		if (randper(800)==SYSTEM_CARRY_SET)			// good chance of backing up
		{
			stack_jump(boss_ease_back);
		}
	}

	stack_jump(boss1);
}

FUNC dtbl_boss2[]=
{
	(FUNC)2,
	motaro_hop,
	motaro_hip_jump
};

JUMPTBL mhe_motaro_far_attax[]=
{
	motaro_far_easy,
	motaro_far_med,
	motaro_far_hard,
	motaro_far_hard,
};

void boss1(void)
{
	if (q_ok_motaro_sweep()==SYSTEM_CARRY_SET && randper(300)==SYSTEM_CARRY_SET)
	{
		stack_jump(motaro_sweep);
	}

	/* boss2 */
	if (b_get_x_dist<SCX(0xa0))
		boss_close();

	/* boss_far */
	if (randper(200)==SYSTEM_CARRY_SET)
	{
		/* motaro_nothing */
		random_do(dtbl_boss2);				// chacne of doing notinh
	}
	else
	{
		mhe_motaro_far_attax[get_mhe_long]();
	}
}

FUNC dtbl_mfm[]=
{
	(FUNC)2,
	motaro_tele,
	motaro_hop
};


void motaro_far_med(void)
{
	if (randper(333)==SYSTEM_CARRY_SET)
	{
		stack_jump(motaro_zap);
	}
	else
	{
		random_do(dtbl_mfm);
	}
}

FUNC dtbl_mfh[]=
{
	(FUNC)2,
	motaro_tele,
	motaro_hop
};


void motaro_far_hard(void)
{
	if (randper(500)==SYSTEM_CARRY_SET)
	{
		stack_jump(motaro_zap);
	}
	else
	{
		random_do(dtbl_mfh);
	}
}

FUNC dtbl_mfe[]=
{
	(FUNC)2,
	motaro_tele,
	motaro_hop
};

void motaro_far_easy(void)
{
	if (randper(100)==SYSTEM_CARRY_SET)
	{
		stack_jump(motaro_zap);
	}
	else
	{
		random_do(dtbl_mfe);
	}
}
//***************************************************************
void boss_close(void)
{
	WORD ta10;

	ta10=0x40*3;
	if (q_is_this_a_joke()==SYSTEM_CARRY_SET)
	{
		if (randper(500)==SYSTEM_CARRY_SET)
		{
			stack_jump(motaro_stupid_stance);
		}
	}

	/* bossc2 */
	if (randper(200)==SYSTEM_CARRY_SET)
	{
		d_stance_pause(randu_minimum(0x40,0x10));
	}

	/* bossc4 */
	ta10=0x40*4;
	d_stalk_a11(&ta10,SCX(0xd0));		// stalk till close
	if (q_ok_motaro_sweep()==SYSTEM_CARRY_SET && randper(200)==SYSTEM_CARRY_SET)
	{
		stack_jump(motaro_sweep);
	}

	/* bossc7 */
	ta10=40*4;
	d_stalk_a11(&ta10,SCX(0x48));		// stalk till close

	if (is_he_airborn()==SYSTEM_CARRY_CLR)
		boss_close_attack();

	if (get_his_action(current_proc)==ACT_ANGLE_JUMP)
		boss_counter_angle();

	boss_wait_land();
}

void boss_wait_land(void)
{
	swait_land_jsrp();
	stack_jump(boss1);
}

FUNC dtbl_bca[]=
{
	(FUNC)3,
	motaro_punch,
	motaro_grab_punch,
	motaro_kick
};
void boss_close_attack(void)
{
	random_do(dtbl_bca);
}

/******************************************************************************
 Function: void motaro_zap(void)

 By: David Schwartz

 Date: May 1995

 Parameters:  None

 Returns: None

 Description:  boss zap ya
******************************************************************************/
void motaro_zap(void)
{
	do_proj_motaro_zap();			// do_zap(0x1f)

	if (q_am_i_losing()==SYSTEM_CARRY_SET || randper(100)==SYSTEM_CARRY_SET)
		b_reaction_exit();

	d_stance_pause(randu_minimum(0x20,0x20));

	b_reaction_exit();
}

/******************************************************************************
 Function: void motaro_tele(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  boss beam over
******************************************************************************/
void motaro_tele(void)
{
	init_special(current_proc->pa8);

	tsound(0x27);
	set_nocol(current_proc->pa8);
	get_char_ani(ANIM_TABLE1,ANIM_MOT_MIST);
	mframew(3);
	set_inviso(current_proc->pa8);

	/* figure out teleport spot */
	if (q_is_he_cornered()==SYSTEM_CARRY_CLR)
	{
		match_me_with_him();
		multi_adjust_xy(current_proc->pa8,-SCX(0x70),0);
	}
	else
	{
		/* tele_cornered */
		match_me_with_him();
		flip_multi(current_proc->pa8);
		multi_adjust_xy(current_proc->pa8,-SCX(0x70),0);
	}

	/* untele */
	process_sleep(3);
	ground_player(current_proc->pa8);
	clear_inviso(current_proc->pa8);
	mframew(3);
	clear_nocol(current_proc->pa8);
	b_reaction_exit();
}

/*******************/
/* SHAO KAHN ATTAX */
/*******************/
void sk_stalk(void)
{
	WORD ta10;

	if (q_boss_stupid()==SYSTEM_CARRY_SET)
		sk_stupid();

	if (b_get_x_dist<D_CLOSE)
		sk_close();

	if (b_get_x_dist<D_MED)
		sk_medium();

	if (b_get_x_dist<D_FAR)
		sk_far();

	if (randper(200)==SYSTEM_CARRY_SET)
		sk_zap();
	else
	{
		/* sk_stalk_far */
		ta10=0x40*4;
		d_stalk_a11(&ta10,D_FAR);

		if (randper(500)==SYSTEM_CARRY_SET)
			sk_far();

		/* sk_stalk_med */
		ta10=0x40*4;
		d_stalk_a11(&ta10,D_MED);

		if (randper(500)==SYSTEM_CARRY_SET)
			sk_medium();

		ta10=0x40*4;
		d_stalk_a11(&ta10,D_CLOSE);
		sk_close();
	}
}

FUNC dtbl_skc[]=
{
	(FUNC)4,
	sk_hammer,
	sk_punch,
	sk_kick,
	sk_charge
};
void sk_close(void)
{
	if (q_is_he_car()==SYSTEM_CARRY_SET && randper(700)==SYSTEM_CARRY_SET)
	{
		stack_jump(boss_ease_back);
	}
	else
	{
		random_do(dtbl_skc);
	}
}

FUNC dtbl_skf[]=
{
	(FUNC)4,
	sk_zap,
	sk_charge,
	sk_air_charge,
	sk_stance_pause
};

FUNC dtbl_skfh[]= 
{
	(FUNC)9,
	sk_air_charge,
	sk_zap,
	sk_zap,
	sk_zap,
	sk_charge,
	sk_charge,
	sk_charge,
	sk_charge,
	sk_charge,
};

void sk_far(void)
{
	if (get_his_matchw()!=0)
		random_do(dtbl_skfh);
	else random_do(dtbl_skf);
}

FUNC dtbl_skm[]=
{
	(FUNC)4,
	sk_hammer,
	sk_charge,
	sk_air_charge,
	sk_zap
};

FUNC dtbl_skmh[]=
{
	(FUNC)7,
	sk_hammer,
	sk_hammer,
	sk_charge,
	sk_charge,
	sk_air_charge,
	sk_zap,
	sk_zap
};
void sk_medium(void)
{
	if (get_his_matchw()!=0)
	random_do(dtbl_skmh);
	else random_do(dtbl_skm);
}

void sk_laugh(void)
{
	init_special(current_proc->pa8);
	rsnd_ochar_sound(3,4);
	get_char_ani(ANIM_TABLE1,ANIM_SK_LAUGH);
	animate_a0_frames(5,0xc);
	b_reaction_exit();
}

void sk_stupid_stance(void)
{
	current_proc->a10=0x40*3;
	if (q_is_this_a_joke()==SYSTEM_CARRY_SET)
	{
		stack_jump(ss1)
	}
	else
	{
		stack_jump(motaro_stupid_stance);
	}
}

void motaro_stupid_stance(void)
{
	current_proc->a10=randu_minimum(0x30,0x30);
	stack_jump(ss1)
}

void ss1(void)
{
	stance_setup();
	/* ss3 */
	do
	{
		next_anirate();
		check_winner_status();
		if (am_i_facing_him()==SYSTEM_CARRY_CLR)
			d_turnaround();
		process_sleep(1);
	}
	while(--current_proc->a10>0);

	b_reaction_exit();
}

void sk_zap(void)
{
	group_sound(0);
	do_proj_sk_zap();			// do_zap(0x1d)
	b_reaction_exit();
}

void sk_hammer(void)
{
	init_special(current_proc->pa8);
	group_sound(0);
	if (striker(SI_STRIKER_NORMAL,1,0,ANIM_SK_HAMMER,2,2)==SYSTEM_CARRY_CLR)
		boss_close_miss();
	else
	{
		process_sleep(0xe);
		boss_post_hit();
	}
}

void sk_kick(void)
{
	init_special(current_proc->pa8);
	group_sound(0);
	if (striker(SI_STRIKER_NORMAL,2,0,ANIM_SK_KICK,2,1)==SYSTEM_CARRY_CLR)
		boss_close_miss();
	else
	{
		process_sleep(0xe);
		boss_post_hit();
	}
}

void sk_punch(void)
{
	init_special(current_proc->pa8);
	group_sound(0);
	if (striker(SI_STRIKER_NORMAL,3,0,ANIM_SK_PUNCH,1,0)==SYSTEM_CARRY_CLR)
		boss_close_miss();
	else
	{
		process_sleep(0xe);
		boss_post_hit();
	}
}

void sk_charge(void)
{
	init_special(current_proc->pa8);

	group_sound(0);

	ochar_sound(0);

	get_char_ani(ANIM_TABLE1,ANIM_SK_CHARGE);
	init_anirate(3);

	create_fx(FX_TRAIL);
	towards_x_vel(current_proc,current_proc->pa8,SCX(0x80000));

	current_proc->a11=0x14;
	current_proc->a10=6;			// # of frames without coll check

	/* kchg6 */
	do
	{
		process_sleep(1);
		next_anirate();
		if (--current_proc->a10==0)
		{
			current_proc->a10=1;
			if (strike_check_a0(current_proc->pa8,3)==SYSTEM_CARRY_SET)
			{
				stop_me(current_proc->pa8);
				get_char_ani(ANIM_TABLE1,ANIM_SK_CHARGE);
				find_last_frame();
				do_next_a9_frame(current_proc->pa8);
				process_sleep(0x20);
				kchg8();
			}
		}
	}
	while(--current_proc->a11>0);

	/* decellerate */
	if (am_i_facing_him()==SYSTEM_CARRY_SET)
	{
		current_proc->a11=8;
		/* kchg5 */
		do
		{
			current_proc->a0=abs((current_proc->pa8)->oxvel.pos);
			current_proc->a0-=((current_proc->a0)>>2);
			current_proc->a0-=((current_proc->a0)>>3);

			towards_x_vel(current_proc,current_proc->pa8,(long)current_proc->a0);
			process_sleep(1);
		}
		while(--current_proc->a11>0);
	}

	/* kchg7 */
	stop_me(current_proc->pa8);
	process_sleep(8);
	kchg8();
}

void kchg8(void)
{
	find_ani_part2(ANIM_SK_CHARGE);
	mframew(3);
	b_reaction_exit();
}

void sk_air_charge(void)
{
	OBJECT *obj=current_proc->pa8;

	init_special(obj);

	group_sound(0);
	ochar_sound(0);
	set_nocol(obj);

	get_char_ani(ANIM_TABLE1,ANIM_SK_AIR_CHARGE);
	do_next_a9_frame(obj);
	process_sleep(3);
	do_next_a9_frame(obj);
	process_sleep(3);

	init_anirate(4);
	create_fx(FX_TRAIL);

	obj->oyvel.pos=-SCY(0x40000);
	towards_x_vel(current_proc,obj,SCX(0xa0000));
	current_proc->a11=0x10;
	current_proc->a10=4;

	/* achg6 */
	do
	{
		process_sleep(1);
		next_anirate();
		if (--current_proc->a10==0)
		{
			current_proc->a10=1;
			if (strike_check_a0(obj,5)==SYSTEM_CARRY_SET)
			{
				clear_nocol(obj);
				stop_me(obj);
				get_char_ani(ANIM_TABLE1,ANIM_SK_AIR_CHARGE);
				find_last_frame();
				do_next_a9_frame(obj);
				process_sleep(0x18);
				achg8();
			}
		}
		/* achg3 */
	}
	while(--current_proc->a11>0);

	/* decellerate */
	clear_nocol(obj);

	if (am_i_facing_him()==SYSTEM_CARRY_SET)
	{
		current_proc->a11=8;
		/* achg5 */
		do
		{
			current_proc->a0=abs((current_proc->pa8)->oxvel.pos);
			current_proc->a0-=((current_proc->a0)>>2);
			current_proc->a0-=((current_proc->a0)>>3);

			towards_x_vel(current_proc,current_proc->pa8,(long)current_proc->a0);
			process_sleep(1);
		}
		while(--current_proc->a11>0);
	}

	/* achg7 */
	stop_me(obj);
	process_sleep(8);
	achg8();
}

void achg8(void)
{
	find_ani_part2(ANIM_SK_AIR_CHARGE);
	do_next_a9_frame(current_proc->pa8);
	drop_down_land_jump();
}

FUNC dtbl_stupid[]=
{
	(FUNC)5,
	boss_ease_back,
	sk_taunt,
	sk_laugh,
	sk_stupid_stance,
	sk_stupid_stance
};

void sk_stupid(void)
{
	random_do(dtbl_stupid);
}

void sk_taunt(void)
{
	rsnd_ochar_sound(3,8);
	get_char_ani(ANIM_TABLE1,ANIM_SK_TAUNT);

	current_proc->a10=2;
	/* riwk5 */
	do
	{
		mframew(3);
		process_sleep(10+randu(6));
	}
	while(--current_proc->a10>0);

	find_ani_part2(ANIM_SK_TAUNT);
	find_part2();
	mframew(3);
	d_stance_pause(randu_minimum(0x10,0x10));
	b_reaction_exit();
}


/****************/
/* MOTARO ATTAX */
/****************/

/******************************************************************************
 Function: void motaro_sweep(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  boss sweep ya
******************************************************************************/
void motaro_sweep(void)
{
	b_ochar_sound(2);

	if (striker(SI_STRIKER_NORMAL,3,0,ANIM_SWEEP,2,2)==SYSTEM_CARRY_SET)
	{
		/* mot_sweep_hit */
		process_sleep(8);
		mframew(4);
		process_sleep(10);
		mframew(4);

		if (d_front_me_a5()<SCX(0x120) || randper(500)==SYSTEM_CARRY_SET)
			motaro_hip_jump();

		mhop7(-SCX(0xa0000),-SCY(0xb0000),ANIM_MOT_HOP);
	}
	else
	{
		mframew(4);
		mframew(4);
		b_reaction_exit();
	}
}

/******************************************************************************
 Function: void boss_counter_angle(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  counter an angle attack
******************************************************************************/
void boss_counter_angle(void)
{
	if (is_towards_me()==SYSTEM_CARRY_CLR || motaro_easy_randper()==SYSTEM_CARRY_CLR)
		boss_wait_land();

	if (b_get_x_dist>SCX(0x80))
		boss_wait_land();

	motaro_punch();
}

/******************************************************************************
 Function: void boss_ease_back(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  boss backoff a bit
******************************************************************************/
void boss_ease_back(void)
{
	(LONG)current_proc->pa9=get_walk_info_b();
	set_x_vel(current_proc->pa8,(long)current_proc->a1);
	get_char_ani(ANIM_TABLE1,(LONG)current_proc->pa9);
	init_anirate(current_proc->a0);

	current_proc->a10=0x60;
	/* ease3 */
	do
	{
		if (am_i_facing_him()==SYSTEM_CARRY_CLR)
		{
			stack_jump(d_turnaround);
		}
		process_sleep(1);
		if (b_get_x_dist>SCX(0x100))
			break;

		b_check_winner_status();
		b_next_anirate();
		b_beware();
		//b_get_x_dist;
	}
	while(--current_proc->a10>0);

	sk_stance_pause();
}

void sk_stance_pause(void)
{
	/* ease5 */
	d_stance_pause(randu_minimum(0x10,0x10));
	b_reaction_exit();
}

/******************************************************************************
 Function: void motaro_hop(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  motaro hop move
******************************************************************************/
void motaro_hop(void)
{
	b_ochar_sound(2);

	if (b_get_x_dist>=SCX(0xe0))
	{
		mhop7(-SCX(0xa0000),-SCY(0x70000),ANIM_MOT_HOP);
	}

	/* mhop3 */
	get_char_ani(ANIM_TABLE1,ANIM_MOT_HOP);
	animate_a0_frames(0x0003,0x0002);

	/* take off!! */
	mhop7(-SCX(0xa0000),-SCY(0x70000),-1);
}

void mhop7(long pa0,long pa1,short pa9)
{
	flight(current_proc->pa8,pa0,pa1,SCY(0x8000),4,pa9);
	shake_n_sound();

	find_ani_part2(ANIM_MOT_HOP);
	mframew(3);
	b_reaction_exit();
}

/******************************************************************************
 Function: void motaro_hip_jump(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  motaro hip jumps
******************************************************************************/
void motaro_hip_jump(void)
{
	check_winner_status();
	motaro_hip_jsrp();
	b_reaction_exit();
}

void motaro_hip_jsrp(void)
{
	b_ochar_sound(0);

	get_char_ani(ANIM_TABLE1,ANIM_MOT_HOP);		// hip = hop in place
	animate_a0_frames(0x0003,0x0002);
	flight(current_proc->pa8,SCX(0x10000),-SCY(0x60000),SCY(0x8000),4,-1);
	shake_n_sound();

	find_ani_part2(ANIM_MOT_HOP);
	mframew(3);
	return;
}

/******************************************************************************
 Function: void motaro_kick(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  motaro kick
*******************************************************************************/
void motaro_kick(void)
{
	get_char_ani(ANIM_TABLE1,ANIM_MOT_KICK);

	b_ochar_sound(1);

	if (am_i_facing_him()==SYSTEM_CARRY_SET)
	{
		/* not facing, need to turnaround */
		mframew(1);				// yes turnaround
		((ADDRESS *)current_proc->pa9)-=4;		// adjust for "find_part2"
	}

	/* motk3 */
	find_part2();

	mframew(2);
	current_proc->a10=4;
	/* motk5 */
	do
	{
		if (strike_check_a0(current_proc->pa8,1)==SYSTEM_CARRY_SET)
		{
			/* motaro_kick_hit */
			process_sleep(10);
			break;
		}

		process_sleep(1);
	}
	while(--current_proc->a10>0);

	/* motk7 */
	mframew(3);

	/* post kick = turn back around */
	if (am_i_facing_him()==SYSTEM_CARRY_CLR)
		animate_a9(0x0003,ANIM_MOT_KICK);		// need to turnaround

	/* motk8 */
	pose_a9(0,ANIM_STANCE);
	b_reaction_exit();
}

/******************************************************************************
 Function: void motaro_grab_punch(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  motaro grab punch
*******************************************************************************/
void motaro_grab_punch(void)
{
	if (is_he_airborn()==SYSTEM_CARRY_SET)
		motaro_punch();
	motaro_grab_punch_now();
}

void motaro_grab_punch_now(void)
{
	b_ochar_sound(2);

	me_in_back();
	wfe_him;									// make him wait forever
	stop_him(current_proc->pdata.p_otherguy);
	animate_a9(0x0003,ANIM_MOT_GRAB);			// reach out

	sans_repell_3();
	pose_him_a9(0,ANIM_STUMBLE);

	/* grab2 */
	if (q_is_he_cornered()==SYSTEM_CARRY_SET)
		away_x_vel(current_proc,current_proc->pa8,SCX(0x30000));
	else away_x_vel(current_proc,current_proc->pa8,SCX(0x00000));

	current_proc->a10=6;
	/* grab4 */
	do
	{
		process_sleep(1);
		match_him_with_me_f();
		adjust_him_xy(-SCX(0x50),SCY(0x10));
		sans_repell_3();
	}
	while(--current_proc->a10>0);

	stop_me(current_proc->pa8);
	find_ani_part2(ANIM_MOT_GRAB);

	f_norepell=0x60;			// start the count down

	if ((current_proc->pdata.p_otherproc)->p_comp_flag==PC_WAITF_WAKE)
	{
		grab_ani(SCX(0x20),-SCY(0x10));
		grab_ani(SCX(0x23),-SCY(0x30));
		grab_ani(SCX(0x0),SCY(0x0));		// lift him up

		shake_him_up(current_proc->pa8,SCX(0x4),SCY(0x00),3,2);
		takeover_him(drop_down_land_jump);
		process_sleep(1);
		away_x_vel(current_proc->pdata.p_otherproc,current_proc->pdata.p_otherguy,SCX(0x20000));
		find_ani_part_a14(ANIM_MOT_GRAB,3);
		me_in_front();

		mframew(2);
		strike_check_a0(current_proc->pa8,0);

		mframew(5);
	}

	/* motaro_grab_abort */
	b_reaction_exit();
}

void grab_ani(short pa0,short pa1)
{
	adjust_him_xy(pa0,pa1);
	do_next_a9_frame(current_proc->pa8);
	process_sleep(6);
}


BOSS_AINFO boss_attack_info[]=
{
	{ANIM_MOT_PUNCH,0x00,0x02}
};

/******************************************************************************
 Function: void motaro_punch(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  motaro punch
*******************************************************************************/
void motaro_punch(void)
{
	b_ochar_sound(2);

	if (striker(SI_STRIKER_NORMAL,boss_attack_info[0].b_speed,0,boss_attack_info[0].b_ani,3,boss_attack_info[0].b_stk)==SYSTEM_CARRY_SET)
		process_sleep(0x10-8);

	boss_close_miss();
}

void boss_close_miss(void)
{
	process_sleep(8);
	mframew(3);
	b_reaction_exit();
}

/******************************************************************
SHAO KANH COUNTERS
******************************************************************/
JUMPTBL skc_counter_zap[]=
{
	skc_zap,			// >00
	skc_zap,			// >01
	skc_zap,			// >02
	skc_zap,			// >03
	skc_zap,			// >04
	skc_zap,			// >05
	skc_zap,			// >06
	skc_zap,			// >07
	skc_zap,			// >08
	skc_zap,			// >09
	skc_zap,			// >0a
	skc_zap,			// >0b
	skc_zap,			// >0c
	skc_zap,			// >0d
	skc_zap,			// >0e
	skc_zap,			// >0f
	skc_zap,			// >10
	skc_zap,			// >11
	skc_zap,			// >12
	skc_zap,			// >13
	skc_lk_zap_lo,		// >14
	skc_zap,			// >15
	skc_zap,			// >16
	skc_zap,			// >17
	skc_zap,			// >18
	skc_zap,			// >19
	skc_zap,			// >1a
	skc_zap,			// >1b
	skc_zap,			// >1c
	skc_zap,			// 1d
	skc_zap,			// 1e
	skc_zap,			// 1f
	skc_zap,			// 20
	skc_zap,			// 21
	skc_zap,			// 22
	skc_zap,			// 23
	skc_zap,			// 24
	skc_zap,			// 25
	skc_zap,			// 26
	skc_zap,			// 27
	skc_zap,			// 28
	skc_zap,			// 29
	skc_zap,			// 2a
};

JUMPTBL skc_counter_stationary[]=
{
	skc_stationary,  	// >100
	skc_stationary,  	// >101
	skc_stationary,  	// >102
	skc_stationary,  	// >103
	skc_stationary,  	// >104
	skc_stationary,  	// >105
	skc_stationary,  	// >106
	skc_stationary,  	// >107
	skc_stationary,  	// >108
	skc_stationary,  	// >109
	skc_stationary,  	// >10a
	skc_stationary,  	// >10b
	skc_stationary,  	// >10c
	skc_stationary,  	// >10d
	skc_stationary,  	// >10e
	skc_stationary,  	// >10f
	skc_stationary,  	// >110
	skc_stationary,  	// >111
	skc_stationary,  	// >112
	skc_stationary,  	// >113
	skc_stationary,  	// >114
	skc_stationary,  	// >115
	skc_stationary,  	// >116
	skc_stationary,  	// >117
	skc_stationary,  	// >118
	skc_stationary,  	// >119
	skc_stationary,  	// >11a
	skc_stationary,  	// >11b
	skc_swat_gun,  		// >11c
	skc_stationary,  	// >11d
	skc_stationary,  	// >11e
	skc_stationary,  	// >11f
};

JUMPTBL skc_counter_sitting_duck[]=
{
	NULL,		// 6NULL,NULL,
	NULL,		// 6NULL,1
	NULL,		// 6NULL,2
	NULL,		// 6NULL,3
	NULL,		// 6NULL,4
	NULL,		// 6NULL,5
	NULL,		// 6NULL,6
	NULL,		// 6NULL,7
	NULL,		// 6NULL,8
	NULL,		// 6NULL,9
	NULL,		// 6NULL,a
	NULL,		// 6NULL,b
	NULL,	//mc_fk_sd   	// 6NULL,c
	NULL,		// 6NULL,d
	NULL,		// 6NULL,e
	NULL,		// 6NULL,f
	NULL,		// 61NULL,
	NULL,		// 611
	NULL,		// 612
	NULL,		// 613
	NULL,		// 614
	NULL,		// 615
	NULL,		// 616
	NULL,		// 617
	NULL,		// 618
	NULL,		// 619
	NULL,		// 61a
	NULL,		// 61b
	NULL,		// 61c
	NULL,		// 61d
	NULL,		// 61e
	NULL,		// 61f
	skc_dizzy,   	// 62NULL,
	NULL,		// 621
	NULL,		// 622
	NULL,		// 623
	NULL,		// 624
	NULL,		// 625
	NULL,		// 626
	NULL,		// 627
	NULL,		// 628
	NULL,		// 629
	NULL,		// 62a
	NULL,		// 62b
	NULL,		// 62c
	NULL,		// 62d
	NULL,		// 62e
	skc_sg_pounce_sd,		// 62f
	NULL,		// 630
	NULL,		// 631
	NULL,		// 632
};

JUMPTBL skc_counter_propell[]=
{
	skc_propell,		// >200
	skc_propell,		// >200
	skc_propell,		// >201
	skc_propell,		// >202
	skc_propell,		// >203
	skc_propell,		// >204
	skc_propell,		// >205
	skc_propell,	     // >206
	skc_propell,		// >207
	skc_propell,		// >208
	skc_propell,		// >209
	skc_propell,		// >20a
	skc_propell,		// >20b
	skc_propell,		// >20c
	skc_propell,		// >20d
	skc_propell,		// >20e
	skc_propell,		// >20f
	skc_propell,		// >210
	skc_propell,		// >211
	skc_propell,		// >212
	skc_propell,		// >213
	skc_propell,		// >214
	skc_propell,		// >215
	skc_propell,		// >216
	skc_propell,		// >217
	skc_propell,		// >218
	skc_propell,		// >219
	skc_propell,		// >21a
	skc_propell,		// >21b
	skc_propell,		// >21c
	skc_propell,		// >21d
};

JUMPTBL *sk_catagory_table[]=
{
	skc_counter_zap,
	skc_counter_stationary,
	skc_counter_propell,
	NULL,
	NULL,
	NULL,
	skc_counter_sitting_duck,
	NULL
};

void skc_propell(void)
{
	if (b_get_x_dist<SCX(0x90))
		d_block();
	else
	{
		b_return_to_beware_4get;
	}
}

void skc_lk_zap_lo(void)
{
	if (get_x_dist<SCX(0x70))
	{
		stack_jump(sk_charge);
	}
	else
	{
		stack_jump(sk_air_charge);
	}
}

void skc_zap(void)
{
	if (randper(800)==SYSTEM_CARRY_CLR)
	{
		b_return_to_beware;
	}

	if (sk_counter_randper()==SYSTEM_CARRY_CLR)
		sk_block_zap();

	if (get_x_dist<SCX(0x70))
	{
		stack_jump(sk_charge);
	}
	else
	{
		stack_jump(sk_zap);
	}
}

void sk_block_zap(void)
{
	face_opponent(current_proc->pa8);
	do_block_hi();
	wait_proj_spawn();
	wait_proj_pass();
	b_reaction_exit();
}

void skc_stationary(void)
{
	if (sk_randper()==SYSTEM_CARRY_CLR)
	{
		b_return_to_beware;
	}

	if (b_get_x_dist>SCX(0x70))
	{
		b_return_to_beware_4get;
	}

	d_block();
}

void skc_swat_gun(void) 
{
	if (sk_randper()==SYSTEM_CARRY_CLR)
	{
		b_return_to_beware;
	}

	if (b_get_x_dist>SCX(0xd0))
	{
		sk_zap();
	}

	d_block();
}

void skc_sg_pounce_sd(void) 
{
	if (randper(700)==SYSTEM_CARRY_CLR) 
	{
		b_return_to_beware;
	}

	sk_kick();
}

void skc_dizzy(void)
{
	if (randper(500)==SYSTEM_CARRY_SET)
	{
		b_return_to_beware;
	}

	if (randper(400)==SYSTEM_CARRY_SET)
		sk_laugh();

	stance_wait_no(0x40*3,qb_is_he_dizzy);
	d_stance_pause(0x40);
	b_reaction_exit();
}

WORD qb_is_he_dizzy(void)
{
	if (get_his_action(current_proc)==ACT_DIZZY)
		q_yes;
	else q_no;
}

WORD q_boss_stupid(void)
{
	bs_losing();
}

WORD sk_counter_joke(void)
{
	skrp(75);
}

WORD mhe_sk_randpers[]={600,700,775,775};

WORD sk_randper(void)
{
	if (q_is_this_a_joke()==SYSTEM_CARRY_SET)
		return(sk_counter_joke());
	else
	{
		skrp(mhe_sk_randpers[get_mhe_word]);
	}
}

WORD mhe_sk_counter_randpers[]={600,700,850,850};
WORD sk_counter_randper(void)
{
	skrp(mhe_sk_counter_randpers[get_mhe_word]);
}

WORD q_is_this_a_joke(void)
{
	if (get_my_matchw()==0)			// no wins = i am not way ahead
		q_no;

	if (get_his_matchw()!=0)		// he had a win = i am not way ahead
		q_no;

	if (get_my_strength(current_proc->pa8)<get_my_strength(current_proc->pdata.p_otherguy))
		q_no;

	if (get_my_strength(current_proc->pdata.p_otherguy)>FULL_STRENGTH/2)
		q_no;
	else
	{
		q_yes;
	}
}

WORD q_am_i_losing(void)
{
	PROCESS *ptemp;
	WORD result;

	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	result=q_am_i_way_ahead();
	current_proc=ptemp;
	return(result);
}

WORD q_am_i_way_ahead(void)
{
	if (get_my_matchw()==0 || get_his_matchw!=0 || q_am_i_stronger()==SYSTEM_CARRY_CLR)
		q_no;
	else
	{
		q_yes;
	}
}

WORD q_am_i_stronger(void)
{
	if (get_my_strength(current_proc->pa8)<get_my_strength(current_proc->pdata.p_otherguy))
		q_no;
	else
	{
		q_yes;
	}
}
/******************************************************************
 MOTARO COUNTERS
******************************************************************/
JUMPTBL mot_counter_stationary[]=
{
	mc_stationary,	// 0
	mc_stationary,	// 1
	mc_stationary,	// 2
	mc_stationary,	// 3
	mc_stationary,	// 4
	mc_stationary,	// 5
	mc_stationary,	// 6
	mc_stationary,	// 7
	mc_stationary,	// 8
	mc_stationary,	// 9
	mc_stationary,	// a
	mc_stationary,	// b
	mc_stationary,	// c
	mc_stationary,	// d
	mc_stationary,	// e
	mc_stationary,	// f
	mc_stationary,	// 10
	mc_stationary,	// 11
	mc_stationary,	// 12
	mc_stationary,	// 13
	mc_stationary,	// 14
	mc_stationary,	// 15
	mc_stationary,	// 16
	mc_stationary,	// 17
	mc_stationary,	// 18
	mc_stationary,	// 19
	mc_stationary,	// 1a
	mc_stationary,	// 1b
	mc_stationary,	// 1c
	mc_stationary,	// 1d
	mc_stationary,	// 1e
	mc_stationary,	// 1f
};

JUMPTBL mot_counter_propell[]=
{
	mc_flipkp,	//c_flykick	// 200 - flying (flip) kick
	mc_flipkp,	//c_flypunch	// 201 - flying (flip) punch
	mc_propell_ls,	//c_sz_decoy	// 202 -
	mc_hover,		//0		// 203
	mc_propell_ls,	//c_sbike		// 204
	mc_propell_ls,	//c_jax_dash	// 205
	mc_propell_ls,	//c_lao_tele	// 206
	mc_propell_ls,	//c_square	// 207
	mc_propell_ls,	//c_robo_tele  	// 208
	mc_propell_ls,	//0		// 209
	mc_propell_ls,	//c_tele_explode	// 20a
	mc_propell_ls,	//c_kano_roll  	// 20b
	mc_propell_ls,	//c_lao_angle 	// 20c
	mc_propell_ls,	//c_bike		// 20d
	mc_propell_ls,	//c_superkang	// 20e
	mc_sg_pounce,	//c_sg_pounce 	// 20f
	mc_propell_ls,	//c_bike		// 210
	mc_propell_ls,	//c_zoom		// 211
	mc_propell_ls,	//0		// 212
	mc_propell_ls,	//c_tusk_blur	// 213
	mc_propell_ls,	//c_ind_charge	// 214
	mc_propell_ls,	//c_scorp_tele	// 215
	mc_propell_ls,	//c_reptile_dash	// 216
	mc_propell_ls,	//c_jade_prop	// 217
	mc_propell_ls,	//c_mileena_tele	// 218
	mc_propell_ls,	//c_mileena_roll	// 219
};

JUMPTBL mot_counter_non_attacks[]=
{
	NULL,		// 0
	NULL,		// 1
	NULL,		// 2
	NULL,		// 3
	NULL,		// 4
	NULL,		// 5
	NULL,		// 6
	NULL,		// 7
	mc_angle_jump,	// 8
	NULL,		// 9
	NULL,		// a
	NULL,		// b
	NULL,		// c
	NULL,		// d
	NULL		// e
};

JUMPTBL mot_counter_sitting_duck[]=
{
	NULL,		// 600
	NULL,		// 601
	NULL,		// 602
	NULL,		// 603
	NULL,		// 604
	NULL,		// 605
	NULL,		// 606
	NULL,		// 607
	NULL,		// 608
	NULL,		// 609
	NULL,		// 60a
	NULL,		// 60b
	mc_fk_sd,   	// 60c
	NULL,		// 60d
	NULL,		// 60e
	NULL,		// 60f
	NULL,		// 610
	NULL,		// 611
	NULL,		// 612
	NULL,		// 613
	NULL,		// 614
	NULL,		// 615
	NULL,		// 616
	NULL,		// 617
	NULL,		// 618
	NULL,		// 619
	NULL,		// 61a
	NULL,		// 61b
	NULL,		// 61c
	NULL,		// 61d
	cb_zoom_sd,		// 61e
	NULL,		// 61f
	mc_dizzy,   	// 620
	NULL,	     	// 621
	cb_robo_tele_sd,	// 622
	NULL,		// 623
	NULL,		// 624
	NULL,		// 625

	NULL,		// 626
	cb_mil_tele_sd, 	// 627
	NULL,		// 628
	NULL,		// 629
	NULL,		// 62a
	NULL,		// 62b
	NULL,		// 62c
	NULL,		// 62d
	NULL,		// 62e
	mc_sg_pounce_sd,	// 62f
	NULL,		// 630
	NULL,		// 631
	NULL,		// 632
	NULL,		// 633
};

JUMPTBL *motaro_catagory_table[]=
{
	NULL,
	mot_counter_stationary,
	mot_counter_propell,
	mot_counter_non_attacks,
	NULL,
	NULL,
	mot_counter_sitting_duck,
	NULL
};

void mc_fk_sd(void)
{
	if (randper(500)==SYSTEM_CARRY_CLR || b_get_x_dist>SCX(0x90))
	{
		b_return_to_beware;
	}
	else
	{
		stance_wait_no(0x40,q_is_he_airborn);
		stack_jump(motaro_grab_punch_now);
	}

}

/******************************************************************************
 Function: void mc_dizzy(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  motaro counter dizzy
*******************************************************************************/
void mc_dizzy(void)
{
	if (randper(500)==SYSTEM_CARRY_SET)
	{
		b_return_to_beware;
	}

	d_stance_pause(0x40*2);
	b_reaction_exit();
}

/******************************************************************************
 Function: void mc_angle_jump(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  motaro counter angle jump
*******************************************************************************/
void mc_angle_jump(void)
{
	if (is_towards_me()==SYSTEM_CARRY_CLR || motaro_easy_randper()==SYSTEM_CARRY_CLR || b_get_x_dist>SCX(0x80))
	{
		b_return_to_beware;
	}

	stack_jump(motaro_punch);
}

void mc_sg_pounce_sd(void) 
{
	if (randper(750)==SYSTEM_CARRY_CLR) 
	{
		b_return_to_beware;
	}

	if (get_x_dist>SCX(0x90)) 
	{
		b_return_to_beware;
	}

	if (get_x_dist>SCX(0x70)) 
	{
		motaro_kick();
	}

	current_proc->a10=8;
	/* sgwait3 */
	do
	{
		process_sleep(1);
		face_opponent(current_proc->pa8);
	}
	while(--current_proc->a10>0);

	if (random() & 0x8000000)
		motaro_grab_punch();
	else motaro_punch();
}

void cb_robo_tele_sd(void) 
{
	if (randper(700)==SYSTEM_CARRY_CLR) 
	{
		b_return_to_beware;
	}

	if (get_x_dist>SCX(0x90)) 
	{
		motaro_zap();
	}

	stance_wait_yes(0x40,q_heading_down);

	if (get_x_dist>SCX(0x70)) 
	{
		motaro_kick();
	}

	motaro_punch();
}

WORD q_heading_down(void) 
{
	if ((current_proc->pdata.p_otherguy)->oyvel.pos<0)
		q_no;
	else q_yes;
}

FUNC dtbl_flpk[]=
{
	(FUNC)2,
	d_block,
	motaro_tele
};
void mc_flipkp(void)
{
	if (q_is_this_a_joke()==SYSTEM_CARRY_SET)
	{
		return_to_beware;
	}

	if (is_towards_me()==SYSTEM_CARRY_SET)
	{
		if (b_get_x_dist>SCX(0x70))
		{
			b_return_to_beware_4get;
		}
		else
		{
			random_do(dtbl_flpk);
		}
	}
	else
	{
		/* mc_flipk_away */
		if (b_get_x_dist>SCX(0x80))
		{
			b_return_to_beware;
		}
		else
		{
			d_block();
		}
	}
}

void mc_sg_pounce(void)
{
	motaro_randper();
	d_block();
}

/******************************************************************************
 Function: void mc_propell_ls(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  motaro counter
*******************************************************************************/
void mc_propell_ls(void)
{
	motaro_randper();

	if (b_get_x_dist>SCX(0x70))
	{
		b_return_to_beware_4get;
	}

	stack_jump(d_block);
}

/******************************************************************************
 Function: void mc_hover(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  motaro hover
*******************************************************************************/
void mc_hover(void)
{
	motaro_randper();

	if (b_get_x_dist>SCX(0x70))
	{
		stack_jump(motaro_hop);
	}
	else
	{
		stack_jump(motaro_punch);
	}
}

/******************************************************************************
 Function: void mc_stationary(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  motaro stationary
*******************************************************************************/
void mc_stationary(void)
{
	if (motaro_randper()==SYSTEM_CARRY_CLR)
	{
		b_return_to_beware;
	}
	else
	{
		if (b_get_x_dist>SCX(0x8a))
		{
			b_return_to_beware_4get;
		}
		else
		{
			stack_jump(b_block);
		}
	}
}

WORD motaro_randper(void)
{
	if (q_is_this_a_joke()==SYSTEM_CARRY_SET)
	{
		/* motaro_joke_randper */
		return(randper(100));
	}
	else
	{
		return(randper(500));
	}
}


/******************************************************************************
 Function: void b_block(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  motaro block
*******************************************************************************/
void b_block(void)
{
	face_opponent(current_proc->pa8);
	do_block_hi();
	d_wait_nonattack(0x40*1);

	if (b_get_x_dist<SCX(0x80))
	{
		stack_jump(motaro_grab_punch);
	}

	b_reaction_exit();
}

/**************************************************************************
*											     *
*    MOTARO REACTIONS !!									     *
*											     *
**************************************************************************/
void motaro_flip_kicked(void)
{
	rsnd_big_smack();
	rsnd_ochar_sound(2,3);

	shake_a11(6,6);
	away_x_vel(current_proc,current_proc->pa8,SCX(0x50000));
	animate_a9(4,ANIM_STUMBLE);

	d_stance_pause(randu_minimum(0x10,0x10));
	b_reaction_exit();
}

void motaro_hit_flight(void)
{
	flight(current_proc->pa8,SCX(0x40000),-SCY(0x40000),SCY(0x6000),5,ANIM_KDOWN);
	land_on_my_back();
}

void motaro_upcutted(void)
{
	if (am_i_airborn(current_proc->pa8)==SYSTEM_CARRY_SET)
		motaro_hit_flight();

	rsnd_big_smack();
	rsnd_ochar_sound(2,3);

	shake_a11(6,6);
	away_x_vel(current_proc,current_proc->pa8,SCX(0x60000));
	animate_a9(4,ANIM_STUMBLE);

	d_stance_pause(randu_minimum(0x10,0x10));
	b_reaction_exit();
}

/******************************************************************************
 Function: void motaro_hit3(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  motaro hit 3
*******************************************************************************/
void motaro_hit3(void)
{
	if (am_i_airborn(current_proc->pa8)==SYSTEM_CARRY_SET)
		motaro_hit_flight();

	rsnd_big_smack();
	rsnd_ochar_sound(0x0002,0x0003);

	away_x_vel(current_proc,current_proc->pa8,SCX(0x40000));

	get_char_ani(ANIM_TABLE1,ANIM_MOT_HIT);
	animate_a0_frames(0x0003,0x0002);
	process_sleep(6);
	mframew(3);
	b_reaction_exit();
}

/******************************************************************************
 Function: void motaro_comboed(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  motaro react to comboed
*******************************************************************************/
FUNC dtbl_mc[]=
{
	(FUNC)2,
	motaro_hip_jump,
	boss_ease_back
};
void motaro_comboed(void)
{
	if (am_i_airborn(current_proc->pa8)==SYSTEM_CARRY_SET)
		motaro_hit_flight();

	rsnd_big_smack();
	away_x_vel(current_proc,current_proc->pa8,SCX(0x10000));
	get_char_ani(ANIM_TABLE1,ANIM_MOT_HIT);
	animate_a0_frames(0x0003,0x0002);
	process_sleep(6);
	mframew(3);

	if (current_proc->pdata.p_hit<3)
		b_reaction_exit();

	random_do(dtbl_mc);
}

/******************************************************************************
 Function: void motaro_hit1(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  motaro hit 1
*******************************************************************************/
void motaro_hit1(void)
{
	if (am_i_airborn(current_proc->pa8)==SYSTEM_CARRY_SET)
		motaro_hit_flight();

	rsnd_med_smack();
	rsnd_ochar_sound(0x0002,0x0003);

	away_x_vel(current_proc,current_proc->pa8,SCX(0x20000));

	get_char_ani(ANIM_TABLE1,ANIM_MOT_HIT);
	animate_a9(0x0003,ANIM_MOT_HIT);
	b_reaction_exit();
}

void motaro_hard_comboed(void)
{
	avoid_corner_trap(2);
	shake_a11(6,6);
	rsnd_big_smack();

	away_x_vel(current_proc,current_proc->pa8,SCX(0x40000));
	animate_a9(3,ANIM_STUMBLE);
	motaro_stumble();
}

/******************************************************************************
 Function: void motaro_stumble(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  motaro stumble
*******************************************************************************/
void motaro_stumble(void)
{
	away_x_vel(current_proc,current_proc->pa8,SCX(0x40000));
	animate_a9(0x0004,ANIM_STUMBLE);
	back_to_normal();
	d_stance_pause(randu_minimum(0x10,0x10));
	b_reaction_exit();
}

/******************************************************************************
 Function: void motaro_collapse(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  motaro collapse
*******************************************************************************/
void motaro_collapse(void)
{
	animate_a9(0x0003,ANIM_KDOWN);
	wait_forever();
}

/******************************************************************************
 Function: WORD q_ok_motaro_sweep(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:  can motaro sweep kick
*******************************************************************************/
WORD q_ok_motaro_sweep(void)
{
	if (b_get_x_dist>SCX(0xd0) || b_get_x_dist<SCX(0x80) || is_he_airborn()==SYSTEM_CARRY_SET)
		q_no;
	else q_yes;
}

/**************************************************************************
*											     *
*  Shao Kahn reactions !									     *
*											     *
**************************************************************************/
void sk_collapse(void)
{
	animate_a9(3,ANIM_KDOWN);

	shake_n_sound();
	mframew(3);
	b_wait_forever();
}

void sk_uppcutted(void)
{
	create_blood_proc(BLOOD_UPPERCUT);
	shake_a11(6,6);
	rsnd_big_smack();

	group_sound(2);
	flight(current_proc->pa8,SCX(0x20000),-SCY(0xc0000),SCY(0x6000),5,ANIM_KDOWN);
	reaction_land();
}

void sk_hard_comboed(void)
{
	shake_a11(6,6);
	rsnd_big_smack();
	group_sound(6);

	sk_airborn_check();
	current_proc->a0=SCX(0x50000);
	stumble_back_vel();
}

void sk_stumble(void)
{
	sk_airborn_check();
	current_proc->a0=SCX(0x40000);
	stumble_back_vel();
}

void sk_hit1(void)
{
	rsnd_med_smack();
	shake_a11(4,4);
	group_sound(6);

	sk_airborn_check();
	away_x_vel(current_proc,current_proc->pa8,SCX(0x40000));
	animate_a9(4,ANIM_SK_HIT_HI);
	b_reaction_exit();
}

void sk_hit3(void)
{
	shake_a11(6,6);
	rsnd_big_smack();
	group_sound(6);
	sk_knocked_down();
}

void sk_knocked_down(void)
{
	group_sound(6);
	motaro_slided();
}

void motaro_slided(void)
{
	flight(current_proc->pa8,SCX(0x40000),-SCY(0x30000),SCY(0x5000),5,ANIM_KDOWN);
	land_on_my_back();
}

void sk_comboed(void)
{
	rsnd_big_smack();
	sk_airborn_check();

	group_sound(6);
	away_x_vel(current_proc,current_proc->pa8,SCX(0x30000));
	get_char_ani(ANIM_TABLE1,ANIM_SK_HIT_HI);
	animate_a0_frames(3,2);
	process_sleep(6);
	mframew(3);
	b_reaction_exit();
}

void sk_airborn_check(void)
{
	if (am_i_airborn(current_proc->pa8)==SYSTEM_CARRY_SET)
		sk_knocked_down();

	return;
}

WORD q_is_he_car(void)
{
	if (q_is_he_cornered()==SYSTEM_CARRY_CLR)
		q_no;

	if (q_is_he_reacting()==SYSTEM_CARRY_CLR)
		q_no;

	q_yes;
}
