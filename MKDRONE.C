/******************************************************************************
 File: mkdrone.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Computer Drone Routines
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
#include "mkjoy.h"
#include "mkprop.h"
#include "mkguys.h"
#include "mkinit.h"
#include "moves.h"
#include "mkzap.h"
#include "mkslam.h"
#include "mkdrone.h"
#include "mkfatal.h"
#include "mkstat.h"
#include "mkfriend.h"
#include "mkanimal.h"
#include "mkboss.h"
#include "mkcanned.h"
#include "moves.h"
#include "mkreact.h"
#include "mkcombo.h"

/******************************************************************************
 Function: void drone_begin(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	Drone Logic Starting Point
******************************************************************************/
void drone_begin(void)
{
	ochar_begin_calls();
	stack_jump(drone_proc);
}

void drone_proc(void)
{
	d_init();
	wait_for_start();				// stay in stance till fight starts

	switch ((current_proc->pa8)->ochar)
	{
		case FT_SMOKE:
			if (randper(300)==SYSTEM_CARRY_SET)
				do_stationary(STAT_INVISO);
			break;
		case FT_REPTILE:
			if (randper(300)==SYSTEM_CARRY_SET)
				do_stationary(STAT_REP_INV);
			break;
	}

	stack_jump(drone_entry);
}

/******************************************************************************
 Function: void drone_entry(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	drone entry point
******************************************************************************/
void drone_entry(void)
{
	POLLING;

	d_to_normal();
	check_winner_status();
	d_beware();
	stance_setup();

	if (am_i_facing_him()==SYSTEM_CARRY_CLR)
	{
		stack_jump(d_turnaround);
	}

	boss_branch(0x10);

	if (am_i_short()==SYSTEM_CARRY_SET)
		d_backup_jsrp();

	/* d_choose */
//************
//	d_stance_pause(0x40*500);
//************

	hangout_check();

	if (q_is_he_reacting()==SYSTEM_CARRY_SET)
	{
		stack_jump(d_wait_finish_react);
	}
	else
	{
		d_attack();
	}
}

/******************************************************************************
 Function: void d_attack(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	drone attack time
******************************************************************************/
WORD rpt_cornered[]={100,100,100,600,650,700,750,800,800,850};

void cornered_attack(void) 
{
	/* cornered_attack */
	if (ask_mr_diff(rpt_cornered)==SYSTEM_CARRY_SET)
	{
		stack_jump(run_in_close);
	}
	else
	{
		stack_jump(stalk_in_close);
	}
}

void d_attack(void)
{
	if (q_am_i_cornered()==SYSTEM_CARRY_SET)
	{
		cornered_attack();
	}

	/* attack according to distance */
	current_proc->a3=d_get_x_dist;

	if (current_proc->a3<SCX(0x45))
	{
		stack_jump(d_attack_very_close);
	}

	if (current_proc->a3<SCX(0x78))
	{
		stack_jump(d_attack_close);
	}

	if (current_proc->a3<SCX(0xf0))
	{
		stack_jump(d_attack_far);
	}

	stack_jump(d_attack_very_far);
}

/******************************************************************************
 Function: void d_attack_very_far(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	drone very far attacks
******************************************************************************/
FUNC dtbl_avf[]=
{
	(FUNC)4,
	d_fflip_jump,
	run_in_close,
	run_in_close,
	d_zap
};

FUNC dtbl_avfh[]=
{
	(FUNC)3,
	d_fflip_jump,
	run_in_close,
	run_in_close
};
void d_attack_very_far(void)
{
	if (is_towards_me()==SYSTEM_CARRY_SET) 
	{
		stack_jump(dont_zap_towards_jumper);
	}
		
	if (q_airborn_counter()==SYSTEM_CARRY_CLR)
		random_do(dtbl_avf);
	else
	{
		stack_jump(dont_zap_towards_jumper);
	}
}

void dont_zap_towards_jumper(void) 
{
	nr_drone_zone();

	if (diff>=6) 
	{
		random_do(dtbl_avfh);
	}
	else
	{
		random_do(dtbl_avf);
	}
}

/******************************************************************************
 Function: void d_attack_far(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	drone far attacks
******************************************************************************/
FUNC dtbl_af[]=
{
	(FUNC)5,
	stalk_in_close,
	run_in_close,
	perhaps_flipk,
	d_zap,
	d_propell_attack
};
void d_attack_far(void)
{
	nr_drone_zone();

	if (q_airborn_counter()==SYSTEM_CARRY_CLR)
		random_do(dtbl_af);
	else
	{
		stack_jump(d_fflip_jump);			// very_far_airborn
	}
}

/******************************************************************************
 Function: void d_attack_close(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	drone close attacks
******************************************************************************/
FUNC dtbl_ac[]=
{
	(FUNC)6,
	d_sweep_kick,
	d_lo_kick,
	d_stalk_crossk,
	d_rapid_lo,
	d_walk_in_4_combos,
	d_propell_attack
};
FUNC dtbl_ach[]=
{
	(FUNC)5,
	d_lo_kick,
	d_stalk_crossk,
	d_rapid_lo,
	d_walk_in_4_combos,
	d_propell_attack
};


WORD rpt_anti_kano[]={100,100,350,500,600,650,750,750,750,750};

void d_attack_close(void)
{
	if (q_airborn_counter()==SYSTEM_CARRY_CLR)
	{
		if (q_is_kano_charging()==SYSTEM_CARRY_SET)
		{
			if (ask_mr_diff(rpt_anti_kano)==SYSTEM_CARRY_SET)
			{
				stack_jump(run_in_close_now);					// close_kano
			}
		}

		if (diff>4)
			random_do(dtbl_ach);
		else random_do(dtbl_ac);
	}
	else
	{
		if (is_towards_me()==SYSTEM_CARRY_SET)
		{
			stack_jump(d_jump_up_kick);
		}
		else
		{
			stack_jump(d_fflip_kick_jump);
		}
	}
}

/******************************************************************************
 Function: void d_attack_very_close(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	drone very close attacks
******************************************************************************/
FUNC dtbl_avc[]=
{
	(FUNC)4,
	d_elbow,
	d_knee,
	d_slam,
	d_stalk_crossk
};
void d_attack_very_close(void)
{
	if (q_airborn_counter()==SYSTEM_CARRY_CLR)
	{
		nr_kano_abuser_check();
		nr_drone_zone();
		random_do(dtbl_avc);
	}
	else
	{
		stack_jump(d_jump_up_kick);
	}
}

WORD rpt_counter_airborns[]={100,100,100,250,333,550,600,700,750,750};

WORD q_airborn_counter(void)
{
	if (is_he_airborn()==SYSTEM_CARRY_CLR)
		q_no;
	else return(ask_mr_diff(rpt_counter_airborns));
}

void nr_drone_zone(void) 
{
	if (clk_tens<=3 || randper(300)==SYSTEM_CARRY_CLR)
		d_rets;				// not much time -->no!

	d_retreat_a11(0x40*3,SCX(0xd0));

	stack_jump(drone_zone);
}

void drone_zone(void) 
{
	stance_wait_yes(0x40*2,q_drone_zone);

	if (d_get_x_dist<ZCLOSE) 
	{
		stack_jump(run_in_close);
	}

	if (d_get_x_dist<ZMED) 
	{
		stack_jump(dz_backup);
	}

	if (d_get_x_dist>ZFAR) 
	{
		stack_jump(dz_forward);
	}

	stack_jump(drone_entry);
}

WORD q_drone_zone(void) 
{
	if (d_either_edge_a5()<SCX(0x50))
		q_yes;
	
	if (d_get_x_dist<ZMED || d_get_x_dist>ZFAR)
		q_yes;

	q_no;
}

void dz_forward(void) 
{
	face_opponent(current_proc->pa8);
	d_walkf_setup();
	do
	{
		if (am_i_facing_him()==SYSTEM_CARRY_CLR) 
		{
			stack_jump(d_turnaround);
		}

		process_sleep(1);
		d_next_anirate();
		d_beware();
	}
	while(d_get_x_dist>ZMED);

	stack_jump(drone_zone);
}

void dz_backup(void) 
{
	face_opponent(current_proc->pa8);
	d_walkb_setup();
	do
	{
		if (am_i_facing_him()==SYSTEM_CARRY_CLR) 
		{
			stack_jump(d_turnaround);
		}

		process_sleep(1);
		d_next_anirate();
		d_beware();

		if (d_either_edge_a5()<SCX(0x50))
		{
			stack_jump(d_cornered);
		}

		if (d_get_x_dist<ZCLOSE) 
		{
			stack_jump(run_in_close);
		}
	}
	while(d_get_x_dist<ZMED);

	stack_jump(drone_zone);
}

/******************************************************************************
 Function: void d_walk_in_4_combos(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	walk or combo time
******************************************************************************/
FUNC dtbl_w4c[]=
{
	(FUNC)2,
	d_elbow,
	d_knee
};

void d_walk_in_4_combos(void)
{
	WORD ta10;

	ta10=0x30;
	if (d_stalk_a11(&ta10,SCX(0x4a))==SYSTEM_ZERO_SET)
		d_reaction_exit();
	else random_do(dtbl_w4c);
}

/******************************************************************************
 Function: WORD d_stalk_a11(WORD *pa10,short pa11)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa10 - ptr to var containing time allowed for event
			 pa11 - x distance parameter

 Returns: SYSTEM_ZERO_SET - within range or time expired
		  SYSTEM_ZERO_CLR - we be close

 Description:	stalk until reach point or run out of time
******************************************************************************/
WORD d_stalk_a11(WORD *pa10,short pa11)
{
	face_opponent(current_proc->pa8);
	d_walkf_setup();

	/* dstk3 */
	do
	{
		if (am_i_facing_him()==SYSTEM_CARRY_CLR)
		{
			stack_jump(d_turnaround);
		}
		process_sleep(1);
		d_check_winner_status();
		d_next_anirate();
		d_beware();

		if (d_get_x_dist<pa11)
			break;					// with range, exit
	}
	while(--(*pa10)>0);				// max time

	return(dist_retp());
}

/******************************************************************************
 Function: WORD d_run_a11(WORD pa10,short pa11)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa10 - time allowed for event
			 pa11 - x distance parameter

 Returns: SYSTEM_ZERO_SET - within range or time expired
		  SYSTEM_ZERO_CLR - we be close

 Description:	run until reach point or run out of time
******************************************************************************/
WORD d_run_a11(WORD pa10,short pa11)
{
	run_setup();

	/* drun3 */
	do
	{
		if (am_i_facing_him()==SYSTEM_CARRY_CLR)
		{
			stack_jump(d_turnaround);
		}
		process_sleep(1);
		reduce_turbo_bar();
		d_check_winner_status();
		d_next_anirate();
		d_beware();

		if (d_get_x_dist<pa11)
			break;					// with range, exit
	}
	while(--pa10>0);				// max time

	return(dist_retp());
}

WORD d_run_till_yes(WORD pa10,FUNCW pa11) 
{
	run_setup();
	face_opponent(current_proc->pa8);

	/* drty3 */
	do
	{
		process_sleep(1);
		reduce_turbo_bar();
		d_check_winner_status();
		d_next_anirate();
		d_beware();
		if (pa11()==SYSTEM_CARRY_SET)
			break;
	}
	while(--pa10>0);

	return(dist_retp());
}

/******************************************************************************
 Function: WORD d_retreat_a11(WORD pa10,short pa11)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa10 - time allowed for event
			 pa11 - x distance parameter

 Returns: SYSTEM_ZERO_SET - within range or time expired
		  SYSTEM_ZERO_CLR - we be close

 Description:	retreat
******************************************************************************/
WORD d_retreat_a11(WORD pa10,short pa11)
{
	face_opponent(current_proc->pa8);
	d_walkb_setup();

	/* dret3 */
	do
	{
		if (am_i_facing_him()==SYSTEM_CARRY_CLR)
		{
			stack_jump(d_turnaround);
		}

		process_sleep(1);
		d_next_anirate();
		d_beware();

		if (d_either_edge_a5()<SCX(0x50))		// too close to edge of world
			d_cornered();

		if (d_get_x_dist>pa11)
			break;					// with range, exit
	}
	while(--pa10>0);				// max time

	return(dist_retp());
}

/******************************************************************************
 Function: WORD dist_retp(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_ZERO_SET - zero set (happens because stop_me() sets zero)

 Description:	distance return
******************************************************************************/
WORD dist_retp(void)
{
	d_stop_me(current_proc->pa8);
	return(SYSTEM_ZERO_SET);
}

/******************************************************************************
 Function: void d_wait_finish_react(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	wait for reaction to finish then return to normal flow
******************************************************************************/
void d_wait_finish_react(void)
{
	if (d_get_x_dist<SCX(0x60))
		d_backoff_a_bit();
	else
	{
		stance_wait_no(0x60,q_is_he_reacting);
		d_reaction_exit();
	}
}

/******************************************************************************
 Function: void d_backoff_a_bit(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	retreat from other player a bit
******************************************************************************/
void d_backoff_a_bit(void)
{
	d_retreat_a11(0x40*1,SCX(0x80));
	d_reaction_exit();
}

/******************************************************************************
 Function: void stalk_in_close(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	stalk in close to player
******************************************************************************/
void stalk_in_close(void)
{
	WORD ta10;

	ta10=0x40*100;
	d_stalk_a11(&ta10,SCX(0x40));
	d_attack_close();
}

/******************************************************************************
 Function: void perhaps_flipk(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	maybe do a flip kick
******************************************************************************/
void perhaps_flipk(void)
{
	if (d_get_x_dist<SCX(0xb5) || d_get_x_dist>SCX(0xf0))
		stalk_in_close();

	frontflip_setup();
	current_proc->a6=(ADDRESS)flipk_scan;
	d_fflip_scan_jsrp();

	d_reaction_exit();
}

void d_flip_punch_jump(void)
{
	frontflip_setup();
	(FUNC)current_proc->a6=fflip_scan;
	d_fflip_scan_jsrp();
	d_reaction_exit();
}

/******************************************************************************
 Function: void run_in_close(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	run in close to other dude
******************************************************************************/
FUNC dtbl_ric[]=
{
	(FUNC)4,
	d_elbow,
	d_knee,
	d_knee,
	d_sweep_kick
};
void run_in_close(void)
{
	if (diff<2)
	{
		stack_jump(perhaps_flipk);
	}
	else
	{
		stack_jump(run_in_close_now);
	}
}

void run_in_close_now(void)
{
	d_run_a11(0x40,SCX(0x40));
	nr_kano_abuser_check();

	if (diff>=5)
		stack_jump(run_in_close_hard);

	if (is_he_short()==SYSTEM_CARRY_SET)
	{
		stack_jump(d_knee);
	}

	random_do(dtbl_ric);
}

FUNC dtbl_rich[]=
{
	(FUNC)3,
	d_elbow,
	d_knee,
	d_slam
};
void run_in_close_hard(void)
{
	random_do(dtbl_rich);
}
/******************************************************************************
 Function: void d_propell_attack(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	do a body propell move
******************************************************************************/
short ochar_props[]=
{
	PROP_KANO_CANNON,		// 0 kano = ball roll
	-1,						// 1
	PROP_JAX_DASH,			// 2 jax = dash punch
	PROP_IND_CHARGE,		// 3 ind = charge
	PROP_SZ_SLIDE,			// 4 sz slide
	PROP_SW_ZOOM,			// 5 swat zoom
	-1,						// 6
	PROP_ROBO_TELE,			// 7 - katsup
	-1,						// 8
	PROP_LAO_TELE,			// 9 - kung lao tele
	PROP_TUSK_BLUR,			// a = tusk blur
	PROP_SG_POUNCE,			// b - sheeva punch
	-1,
	PROP_LK_BIKE,			// d - lk bike
	PROP_ROBO_TELE,
	-1,
	PROP_JADE_PROP,
	PROP_MIL_TELE,
	PROP_SCORP_TELE,
	PROP_NJ_SLIDE,
	PROP_ERMAC_TELE,
	-1,
	-1,
	-1,
};

void d_propell_attack(void)
{
	if (diff<3)
		diff_no_propell();

	d_propell_attack_now();
}

void d_propell_attack_now(void)
{
	(short)current_proc->a0=ochar_props[(current_proc->pa8)->ochar];

	if ((short)current_proc->a0 < 0)
		stalk_in_close();
	else d_body_propell((WORD)current_proc->a0);
}


void diff_no_zap(void)
{
	if (d_get_x_dist<SCX(0x48))
	{
		stack_jump(d_lo_kick);
	}

	stalk_in_close();
}

/******************************************************************************
 Function: void d_zap(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	do a zap move
******************************************************************************/
short ochar_zaps[]=
{
	0,  	// 0
	1,	// 1
	2,	// 2
	4,	// 3
	0x0c,	// 4
	0x1a,	// 5
	0x1b,	// 6
	9,  	// 7
	0x0b,	// 8
	0x0e,	// 9
	0x11,	// 10
	0x18,	// 11
	0x15,	// 12
	0x17,	// 13
	0x1e,	// 14 - smoke

	0x20,	// 15
	0x21,	// 16
	0x29,	// 17 - jade
	0x24,	// 18 - scorpion spear
	0x23,	// 19 - retile spit
	0x2c,	// 20 - ermac
	0x2a,	// 21 - old sz
	0x24,	// 22 - old smoke
	-1,	// 23
	-1,	// 24
	-1,	// 25
};
void d_zap(void)
{
	/* pro: check if he is in flipk range */
	if (is_throwing_allowed()==SYSTEM_CARRY_CLR)
		diff_no_zap();

	d_zap_now();
}

void d_zap_now(void)
{
	(short)current_proc->a0=ochar_zaps[((current_proc->pa8)->ochar)];

	if ((short)current_proc->a0 <0)
		stalk_in_close();
	else do_zap((WORD)current_proc->a0);
}

void d_do_floor_ice(void) 
{
	do_zap(ZAP_FLOOR_ICE);
	d_reaction_exit();
}

WORD is_throwing_allowed(void)
{
	if (diff<2)
		q_no;
	else q_yes;
}
/******************************************************************************
 Function: void d_bflip_jump(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	do a backflip and exit
******************************************************************************/
void d_bflip_jump(void)
{
	d_bflip_jsrp();
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_bflip_jsrp(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	do a backflip and return
******************************************************************************/
void d_bflip_jsrp(void)
{
	if (d_behind_me_a5()<=SCX(0x70)) 			// check to close to edge
	{
		/* bflip_abort */
		stack_jump(d_attack);
	}
	else
	{
		backflip_setup();
		d_bflip_noscan_jsrp();
	}

	return;
}

/******************************************************************************
 Function: void d_jumpup_nocall(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	do a jumpup
******************************************************************************/
void d_jumpup_nocall(void)
{
	current_proc->a11=0;
	stack_jump(d_jumpup);
}

/******************************************************************************
 Function: void d_jumpup(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: current_proc->a11 - func call

 Returns: None

 Description:	do a jumpup
******************************************************************************/
void d_jumpup(void)
{
	do_jump_up((FUNC *)current_proc->a11);
	stack_jump(d_land);
}

/******************************************************************************
 Function: void d_fflip_scan_jump(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: current_proc->a6 - func call

 Returns: None

 Description:	do a frontflip w/call and return return
******************************************************************************/
void d_fflip_scan_jump(void)
{
	d_fflip_scan_jsrp();
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_bflip_noscan_jsrp(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: current_proc->a6 - func call

 Returns: None

 Description:	do a backflip w/call and return return
******************************************************************************/
void d_bflip_noscan_jsrp(void)
{
	current_proc->a6=0;
	d_bflip_scan_jsrp();
}

/******************************************************************************
 Function: void d_bflip_scan_jsrp(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: current_proc->a6 - func call

 Returns: None

 Description:	do a backflip w/call and return return
******************************************************************************/
void d_bflip_scan_jsrp(void)
{
	ADDRESS ta6=current_proc->a6;

	backflip_setup();

	/* dflip3 */
	current_proc->a6=ta6;
	current_proc->pdata.p_store1=ta6;			// drone routine to call
	do_flip(current_proc->a0,current_proc->a1,(long)current_proc->a11,(long)current_proc->a6);
}

/******************************************************************************
 Function: void d_fflip_noscan_jsrp(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: current_proc->a6 - func call

 Returns: None

 Description:	do a backflip w/call and return return
******************************************************************************/
void d_fflip_noscan_jsrp(void)
{
	current_proc->a6=0;
	d_fflip_scan_jsrp();
}

/******************************************************************************
 Function: void d_fflip_scan_jsrp(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: current_proc->a6 - func call

 Returns: None

 Description:	do a backflip w/call and return return
******************************************************************************/
void d_fflip_scan_jsrp(void)
{
	ADDRESS ta6=current_proc->a6;

	frontflip_setup();

	/* dflip3 */
	//current_proc->a6=ta6;
	current_proc->pdata.p_store1=ta6;			// drone routine to call
	do_flip(current_proc->a0,current_proc->a1,(long)current_proc->a11,(long)current_proc->a6);
}

/******************************************************************************
 Function: void d_fflip_jump(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	do a frontflip and exit
******************************************************************************/
void d_fflip_jump(void)
{
	d_fflip_jsrp();
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_fflip_jsrp(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	do a frontflip and return
******************************************************************************/
void d_fflip_jsrp(void)
{
	frontflip_setup();

	if (diff<4)
		d_fflip_noscan_jsrp();
	else
	{
		(FUNC)current_proc->a6=fflip_watchout;
		d_fflip_scan_jsrp();
	}
	return;
}

void fflip_watchout(void)
{
	if (d_get_x_dist>SCX(0xa0))
		return;

	if (q_is_he_airborn()==SYSTEM_CARRY_SET)
	{
		stack_jump(watch_flip_punch);
	}
	else
	{
		stack_jump(watch_flip_kick);
	}
}

void watch_flip_punch(void)
{
	do_flip_punch();
	d_reaction_exit();
}

void watch_flip_kick(void)
{
	do_flip_kick();
	d_reaction_exit();
}
/******************************************************************************
 Function: void frontflip_setup(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	setup drone for a front flip
******************************************************************************/
void frontflip_setup(void)
{
	if (is_he_right(current_proc)==SYSTEM_CARRY_SET)
	{
		(long)current_proc->a11=SCX(0x40000);			// walk angle jump x vel
		(long)current_proc->a6=SCX(0x70000);			// run angle jump x vel
		current_proc->a1=ANIM_FFLIP;					// forward flip
		current_proc->a0=ANIM_BFLIP;					// backward flip
	}
	else
	{
		(long)current_proc->a11=-SCX(0x40000);			// walk angle jump x vel
		(long)current_proc->a6=-SCX(0x70000);			// run angle jump x vel
		current_proc->a0=ANIM_FFLIP;					// forward flip
		current_proc->a1=ANIM_BFLIP;					// backward flip
	}

	return;
}

/******************************************************************************
 Function: void backflip_setup(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	setup drone for a back flip
******************************************************************************/
void backflip_setup(void)
{
	if (is_he_right(current_proc)==SYSTEM_CARRY_CLR)
	{
		(long)current_proc->a11=SCX(0x40000);			// walk angle jump x vel
		(long)current_proc->a6=SCX(0x70000);			// run angle jump x vel
		current_proc->a1=ANIM_FFLIP;					// forward flip
		current_proc->a0=ANIM_BFLIP;					// backward flip
	}
	else
	{
		(long)current_proc->a11=-SCX(0x40000);			// walk angle jump x vel
		(long)current_proc->a6=-SCX(0x70000);			// run angle jump x vel
		current_proc->a0=ANIM_FFLIP;					// forward flip
		current_proc->a1=ANIM_BFLIP;					// backward flip
	}

	return;
}

void stalk_wait_yes(WORD pa10,FUNCW pa11)
{
	face_opponent(current_proc->pa8);
	d_walkf_setup();

	/* stwy3 */
	do
	{
		if (am_i_facing_him()==SYSTEM_CARRY_CLR)
			d_turnaround();

		process_sleep(1);
		d_check_winner_status();
		d_next_anirate();
		d_beware();

		if (pa11()==SYSTEM_CARRY_SET)
			break;
	}
	while(--pa10>0);

	return;
}

void retreat_wait_yes(WORD pa10,FUNCW pa11)
{
	face_opponent(current_proc->pa8);
	d_walkb_setup();

	/* rtwy3 */
	do
	{
		if (am_i_facing_him()==SYSTEM_CARRY_CLR)
			d_turnaround();

		process_sleep(1);
		d_next_anirate();
		d_beware();

		if (d_either_edge_a5()<SCX(0x50))
			d_cornered();

		if (pa11()==SYSTEM_CARRY_SET)
			break;
	}
	while(--pa10>0);

	return;
}

/******************************************************************************
 Function: void d_hang_out(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	chill and hang out awhile
******************************************************************************/
void d_hang_out(void)
{
	dv_hangout--;				// count this

	d_stance_pause(randu_minimum(0x40*2,0x30));
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_stance_pause(WORD pa10)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa10 - time to hang

 Returns: None

 Description:	hang in stance and wait, pay attention to da world
******************************************************************************/
void d_stance_pause(WORD pa10)
{
	d_stance_setup();

	/* dsp3 */
	do
	{
		d_next_anirate();
		d_beware();
		d_check_winner_status();
		if (am_i_facing_him()==SYSTEM_CARRY_CLR)
		{
			stack_jump(d_turnaround);
		}
		process_sleep(1);
	}
	while(--pa10>0);

	d_retp;

	return;
}

/******************************************************************************
 Function: WORD stance_wait_yes(WORD pa10,FUNCW pa11)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa10 - max wait time
			 pa11 - routine we are waiting for a yes from

 Returns: None

 Description:	wait in stance until routine oks continueing
******************************************************************************/
WORD stance_wait_yes(WORD pa10,FUNCW pa11)
{
	d_stance_setup();

	/* dsp8 */
	do
	{
		next_anirate();
		d_beware();
		if (pa11()==SYSTEM_CARRY_SET)
		{
			return(SYSTEM_CARRY_SET);
		}
		process_sleep(1);
	}
	while(--pa10>0);

	return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: WORD stance_wait_no(WORD pa10,FUNCW pa11)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa10 - max wait time
			 pa11 - routine we are waiting for a no from

 Returns: None

 Description:	wait in stance until routine oks continueing
******************************************************************************/
WORD stance_wait_no(WORD pa10,FUNCW pa11)
{
	d_stance_setup();

	/* swn8 */
	do
	{
		next_anirate();
		d_beware();
		if (pa11()==SYSTEM_CARRY_CLR)
		{
			return(SYSTEM_CARRY_SET);
		}
		process_sleep(1);
	}
	while(--pa10>0);

	return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: void stw_proj_proc(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
void stw_proj_proc(void)
{
	stance_wait_no(0x50,q_is_proj_gone);
}

/******************************************************************************
 Function: void stw_proj_proc_gone(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
void stw_proj_proc_gone(void)
{
	stance_wait_yes(0x50,q_is_proj_gone);
}

/******************************************************************************
 Function: WORD d_wait_yes_still(WORD pa10,FUNCW pa11)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
WORD d_wait_yes_still(WORD pa10,FUNCW pa11)
{
	do
	{
		process_sleep(1);
		if (pa11()==SYSTEM_CARRY_SET)
			return(SYSTEM_CARRY_SET);
	}
	while(--pa10>0);

	return(SYSTEM_CARRY_CLR);

}

void flipp_scan(void)
{
	if (get_x_dist>SCX(0x80))
		d_rets;
	else
	{
		do_flip_punch();
		d_reaction_exit();
	}
}

/******************************************************************************
 Function: void flipk_scan(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   scan routine for flip kick
******************************************************************************/
void flipk_scan(void)
{
	if (get_x_dist>SCX(0x80))
		d_rets;
	else
	{
		do_flip_kick();
		d_reaction_exit();
	}
}

/******************************************************************************
 Function: void hangout_check(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   scan routine for flip kick
******************************************************************************/
DCHANCE c_hc = {200,d_hang_out};
void hangout_check(void)
{
	if (diff>4 || dv_hangout==0)			// hard no hangouts
		d_rets;

	chance_do(&c_hc);

	return;
}

/********************************************************
 Drone versions of normal routines
********************************************************/

/******************************************************************************
 Function: void d_backup_jump(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   get backup and exit
******************************************************************************/
void d_backup_jump(void)
{
	d_backup_jsrp();
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_backup_jsrp(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   get backup and return
******************************************************************************/
void d_backup_jsrp(void)
{
	current_proc->pdata.p_action=ACT_BACKUP;
	d_get_char_ani(ANIM_DUCK);
	current_proc->a10=(ADDRESS)current_proc->pa9;
	((ADDRESS *)current_proc->pa9)+=2;
	d_do_next_a9_frame(current_proc->pa8);
	process_sleep(2);
	d_beware();

	(ADDRESS)current_proc->pa9=current_proc->a10;
	((ADDRESS *)current_proc->pa9)++;
	d_do_next_a9_frame(current_proc->pa8);
	process_sleep(2);
	d_beware();

	(ADDRESS)current_proc->pa9=current_proc->a10;
	d_do_next_a9_frame(current_proc->pa8);
	process_sleep(2);

	d_retp;
}

/******************************************************************************
 Function: void d_slam();

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   slam dude and exit
******************************************************************************/
void d_slam(void)
{
	if (q_is_he_a_boss()==SYSTEM_CARRY_SET || q_is_he_airborn()==SYSTEM_CARRY_SET)
	{
		stack_jump(d_rapid_hi);
	}

	do_body_slam();
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_elbow();

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   elbow dude and exit
******************************************************************************/
void d_elbow(void)
{
	if (q_is_he_airborn()==SYSTEM_CARRY_SET)
	{
		stack_jump(d_rapid_lo);
	}

	if (is_he_short()==SYSTEM_CARRY_SET)
	{
		stack_jump(d_knee);
	}

	do_elbow();

	d_reaction_exit();
}

/******************************************************************************
 Function: void d_knee(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   knee dude and exit
******************************************************************************/
void d_knee(void)
{
	if (q_is_he_airborn()==SYSTEM_CARRY_SET)
	{
		stack_jump(d_rapid_lo);
	}

	do_knee();

	d_reaction_exit();
}

/******************************************************************************
 Function: void d_lo_kick(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   lo kick dude and exit
******************************************************************************/
void d_lo_kick(void)
{
	do_lo_kick();

	d_reaction_exit();
}

/******************************************************************************
 Function: void d_stalk_crossk(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   stalk and if close before timeout then kick
******************************************************************************/
void d_stalk_crossk(void)
{
	WORD ta10;

	ta10=0x40*1;
	d_stalk_a11(&ta10,SCX(0x40));
	if (ta10==0)
		d_reaction_exit();

	d_crossover_kick();
}

/******************************************************************************
 Function: void d_crossover_kick(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   cross over scan fflip kick
******************************************************************************/
void d_crossover_kick(void)
{
	nr_kano_abuser_check();

	current_proc->a6=(ADDRESS)crossover_scan;
	stack_jump(d_fflip_scan_jump);
}

/******************************************************************************
 Function: void crossover_scan(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   call while doing crossover fflip checking if i went over dude (neck kick time)
******************************************************************************/
void crossover_scan(void)
{
	OBJECT *pa8=current_proc->pa8;

	(short)current_proc->a1=pa8->oxpos.u.intpos;
	(short)current_proc->a2=(current_proc->pdata.p_otherguy)->oxpos.u.intpos;

	if (pa8->oxvel.pos>0)
	{
		if (((short)current_proc->a1)<((short)current_proc->a2))
			d_rets;
	}
	else
	{
		if (((short)current_proc->a2)<((short)current_proc->a1))
			d_rets;
	}

	/* scan_flip_kick */
	do_flip_kick();
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_fflip_kick_jump(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   drone
******************************************************************************/
void d_fflip_kick_jump(void)
{
	d_fflip_kick_jsrp();
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_fflip_kick_jsrp(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   drone
******************************************************************************/
void d_fflip_kick_jsrp(void)
{
	current_proc->a6=(ADDRESS)fflip_scan;
	stack_jump(d_fflip_scan_jump);
}

/******************************************************************************
 Function: void fflip_scan(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   drone
******************************************************************************/
void fflip_scan(void)
{
	if (d_get_x_dist<SCX(0x70))
	{
		/* scan_flip_kick) */
		do_flip_kick();
		d_reaction_exit();
	}

	return;
}

void d_jump_up_kick(void)
{
	do_jump_up(jump_up_kick_scan);
	d_reaction_exit();
}

void jump_up_kick_scan(void)
{
	if (get_x_dist>SCX(0x90))
		return;

	do_jumpup_kick;
}

/******************************************************************************
 Function: void d_sweep_kick(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   drone sweep kick
******************************************************************************/
FUNC dtbl_sk[] =
{
	(FUNC)2,
	d_get_open,
	d_backoff_a_bit
};

void d_sweep_kick(void)
{
	do_sweep_kick();

	if (q_is_he_reacting()==SYSTEM_CARRY_CLR)
		d_reaction_exit();

	random_do(dtbl_sk);
}

/******************************************************************************
 Function: void d_uppercut(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   drone uppercut
******************************************************************************/
void d_uppercut(void)
{
	do_uppercut();
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_hi_kick(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   drone hi kick
******************************************************************************/
void d_hi_kick(void)
{
	do_hi_kick();
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_rapid_lo(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   drone rapid lo punches
******************************************************************************/
void d_rapid_lo(void)
{
	drone_rfp(7,2,3,ANIM_LOPUNCH);
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_rapid_hi(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   drone rapid hi punches
******************************************************************************/
void d_rapid_hi(void)
{
	drone_rfp(7,2,2,ANIM_HIPUNCH);
	d_reaction_exit();
}

/******************************************************************************
 Function: void drone_rfp(WORD pa0,WORD pa1,WORD pa2,WORD pa9)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa0 - max times i am allowed to hit
			 pa1 - sleep time after a strike
			 pa2 - stk offset #1
			 pa9 - animation offset to use

 Returns: None

 Description:   drone rapid punch routine
******************************************************************************/
void drone_rfp(WORD pa0,WORD pa1,WORD pa2,WORD pa9)
{
	OBJECT *pa8=current_proc->pa8;

	current_proc->pdata.p_anirate=pa1;
	init_special(pa8);

	d_get_char_ani(pa9);

	/* drfp1 */
	do
	{
		mframew(3);
		/* drf5 */
		if (strike_check_a0(pa8,pa2)==SYSTEM_CARRY_CLR || --pa0==0)
		{
			/* drfp3, unpunch #1 */
			find_ani_part2(pa9);
			find_part2();
			find_part2();
			break;
		}
		process_sleep(current_proc->pdata.p_anirate);

		mframew(3);
		if (strike_check_a0(pa8,pa2)==SYSTEM_CARRY_CLR || --pa0==0)
		{
			/* drfp2, unpunch #2 */
			find_ani_part2(pa9);
			find_part2();
			find_part2();
			find_part2();
			break;
		}
		process_sleep(current_proc->pdata.p_anirate);
	}
	while(1);

	/* drfp4 */
	mframew(3);
	d_retp;
}

/******************************************************************************
 Function: WORD ask_mr_diff(WORD *pa0)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa0 - table of 10 randper values which are used based on the
				   magical variable called diff

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description: magical routine which asks mr diff if i can perform
                a certain act.
******************************************************************************/
WORD ask_mr_diff(WORD *pa0)
{
	if (diff>9)
		return(randper(*(pa0+7)));		// diff is fucked up use 7
	else return(randper(*(pa0+diff)));
}

/******************************************************************************
 Function: void random_do(FUNC *pa0)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa0 - ptr to table of funcs to random select from
			 entry 1 = # of entries to consider

 Returns: None

 Description: do one of the actions in the table randomly
******************************************************************************/
void random_do(FUNC *pa0)
{
	WORD ta0;

	ta0=*((WORD *)pa0);
	ta0=randu(ta0);

	(*(pa0+ta0))();

	/* code hopefully never gets here */
#if DEBUG
	LOCKUP;
#endif

	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void chance_do(DCHANCE *pa0)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa0 - ptr to chance routine structure

 Returns: None

 Description: perform action if dice roll percent
******************************************************************************/
void chance_do(DCHANCE *pa0)
{
	if (randper(pa0->d_prop)==SYSTEM_CARRY_CLR)
		return;

	stack_jump(pa0->d_routine);
}

void ochar_match_jump(DMJ *pa0) 
{
	while (pa0->d_char!=-1)
	{	
		if (pa0->d_char==(short)(current_proc->pa8)->ochar) 
		{
			stack_jump(pa0->d_routine);
		}

		pa0++;
	}
}

/******************************************************************************
 Function: void drone_post_duck_hit(void);

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: what to do after a duck hit
******************************************************************************/
void drone_post_duck_hit(void)
{
	d_wait_nonattack(0x40*2);
	do_backup();
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_wait_nonattack(WORD pa10)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa10 - wait timer

 Returns: None

 Description: what for an attack or time out
******************************************************************************/
void d_wait_nonattack(WORD pa10)
{
	WORD results;
	do
	{
		process_sleep(1);

		if (--pa10==0)
			break;

		results=is_he_attacking();
	}
	while(results==SYSTEM_CARRY_SET);

	/* wna9 */
	d_retp;
}

/******************************************************************************
 Function: void d_post_block(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: what to do after a block
******************************************************************************/
void d_post_block(void)
{
	if (is_he_attacking()==SYSTEM_CARRY_SET) 
	{
		/* d_unblock */
		current_proc->pdata.p_action=ACT_BLOCKHI;
		d_wait_nonattack(0x40*2);
	}

	current_proc->pdata.p_action=0;

	if (am_i_short()==SYSTEM_CARRY_CLR)
		d_get_char_ani(ANIM_STANDBLOCK);
	else d_get_char_ani(ANIM_DUCKBLOCK);

	/* pblk3 */
	current_proc->a10=(ADDRESS)current_proc->pa9;
	((ADDRESS *)current_proc->pa9)++;
	d_do_next_a9_frame(current_proc->pa8);
	d_beware();
	process_sleep(1);
	d_beware();
	process_sleep(1);
	d_beware();
	process_sleep(1);
	d_beware();

	(ADDRESS)current_proc->pa9=current_proc->a10;
	d_do_next_a9_frame(current_proc->pa8);

	process_sleep(1);
	d_beware();
	process_sleep(1);
	d_beware();
	process_sleep(1);
	d_beware();

	if (am_i_short()==SYSTEM_CARRY_SET)
	{
		stack_jump(d_backup_jump);
	}

	d_reaction_exit();
}

/******************************************************************************
 Function: void d_turnaround(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: drone turnaround w/exit
******************************************************************************/
void d_turnaround(void)
{
	d_turnaround_jsrp();
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_turnaround_jsrp(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: drone turnaround w/return
******************************************************************************/
void d_turnaround_jsrp(void)
{
	if (am_i_facing_him()==SYSTEM_CARRY_SET)
		d_retp;

	d_stop_me(current_proc->pa8);
	d_get_char_ani(ANIM_TURN);
	//init_anirate(ta_speeds[(current_proc->pa8)->ochar][0]);		// get turn speed
	init_anirate(2);		// get turn speed

	/* dturn7 */
	do
	{
		process_sleep(1);
		d_beware();
		next_anirate();
	}
	while(GET_LONG(current_proc->pa9)!=0);

	d_retp;
}

/******************************************************************************
 Function: void d_getup(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: current_proc->a11 - ptr to speed tables for getting up

 Returns: None

 Description: drone getup
******************************************************************************/
void d_getup(void)
{
	current_proc->pdata.p_action=ACT_GETUP;
	current_proc->pdata.p_otheract=0;				// dont let anything get by me !

	d_get_char_ani(ANIM_GETUP);

	/* dgu4 */
	current_proc->a0=*(((WORD *)current_proc->a11)+(current_proc->pa8)->ochar);
	init_anirate(current_proc->a0);

	/* dgu5 */
	do
	{
		process_sleep(1);
		d_beware();
		d_next_anirate();
	}
	while(GET_LONG(current_proc->pa9) != 0 );

	d_reaction_exit();
}

/******************************************************************************
 Function: void d_get_open(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: drone get open away from other dude
******************************************************************************/
void d_get_open(void)
{
	short ta5,ta6;

	/* get_my_dfe */
	ta5=abs((current_proc->pa8)->oxpos.u.intpos-left_edge);
	ta6=abs(right_edge+SCRRGT-((current_proc->pa8)->oxpos.u.intpos));

	if (ta6>ta5)
	{
		if (is_he_right(current_proc)==SYSTEM_CARRY_SET)
		{
			stack_jump(d_open_jumpover);
		}
		else
		{
			stack_jump(d_bflip_jump);
		}
	}
	else
	{
		if (is_he_right(current_proc)==SYSTEM_CARRY_CLR)
		{
			stack_jump(d_open_jumpover);
		}
		else
		{
			stack_jump(d_bflip_jump);
		}
	}
}

/******************************************************************************
 Function: void d_open_jumpover(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: drone jumpover otherdude
******************************************************************************/
void d_open_jumpover(void)
{
	d_fflip_jsrp();
	check_winner_status();
	d_bflip_jump();
}

/******************************************************************************
 Function: void d_duck(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: drone duck
******************************************************************************/
void d_duck(void)
{
	do_duck();
	d_wait_nonattack(0x40*2);
	d_backup_jump();
}

/******************************************************************************
 Function: void d_duck_then_uppercut(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: drone duck and uppercut
******************************************************************************/
void d_duck_then_uppercut(void)
{
	do_duck();
	d_wait_nonattack(0x40*2);
	stack_jump(d_uppercut);
}

/******************************************************************************
 Function: void d_duck_fast(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: drone duck fast
******************************************************************************/
void d_duck_fast(void)
{
	current_proc->pdata.p_action=ACT_DUCK;			// define my DIFFERENT action !!!
	d_stop_me(current_proc->pa8);
	face_opponent(current_proc->pa8);
	d_get_char_ani(ANIM_DUCK);
	mframew(1);
	return;
}

/********************************************************
 If you cans !!
********************************************************/

/******************************************************************************
 Function: inline void nr_hikick_if_u_can(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description: hikick if possible
******************************************************************************/
inline void nr_hikick_if_u_can(void)
{
	if_u_can(0,d_hi_kick);
}


/******************************************************************************
 Function: inline void nr_sweep_if_u_can(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: sweep if possible
******************************************************************************/
inline void nr_sweep_if_u_can(void)
{
	if_u_can(4,d_sweep_kick);
}

/******************************************************************************
 Function: inline void nr_uppercut_if_u_can(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: uppercut if possible
******************************************************************************/
inline void nr_uppercut_if_u_can(void)
{
	if_u_can(8,d_uppercut);
}

/******************************************************************************
 Function: void if_u_can(WORD pa0,FUNC pa10)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa0 - strike offset
			 pa10 - routine to run if strike is possible

 Returns: None

 Description: if attack will hit then run routine
******************************************************************************/
void if_u_can(WORD pa0,FUNC pa10)
{
	if (d_strike_check_a0_test(current_proc->pa8,pa0)==SYSTEM_CARRY_CLR)
		d_rets;

	stack_jump(pa10);
}

/********************************************************
 Drone Versions of external routines
********************************************************/

/******************************************************************************
 Function: void d_block(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: drone block
******************************************************************************/
void d_block(void)
{
	face_opponent(current_proc->pa8);
	do_block_hi();
	d_wait_nonattack(0x40*2);
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_duck_block(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: drone duck block
******************************************************************************/
void d_duck_block(void)
{
	stop_me(current_proc->pa8);
	face_opponent(current_proc->pa8);
	d_get_char_ani(ANIM_DUCKBLOCK);
	act_mframew(ACT_BLOCKLO,3);
	d_wait_nonattack(0x40*2);
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_block_projectile(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: drone block projectile
******************************************************************************/
void d_block_projectile(void)
{
	stance_wait_no(0x30,q_is_proj_gone);
	do_block_hi();
	d_wait_yes_still(0x50,q_is_proj_gone);
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_to_normal(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: drone back to normal
******************************************************************************/
void d_to_normal(void)
{
	back_to_normal();
	current_proc->pdata.p_otheract=0;
	return;
}

/******************************************************************************
 Function: void d_stance_setup(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: drone setup stance move
******************************************************************************/
void d_stance_setup(void)
{
	d_stop_me(current_proc->pa8);

	current_proc->a5=(ADDRESS)current_proc->pa9;
	stance_setup();
	do_next_a9_frame(current_proc->pa8);
	((ADDRESS *)current_proc->pa9)--;

	/* sss1 */
	while (GET_LONG(current_proc->pa9)==ANI_OCHAR_JUMP)
	{
		((ADDRESS *)current_proc->pa9)+=5;			// jump command skip bs
	}

	/* sss2 */
	current_proc->a4=(ADDRESS)current_proc->pa9;

	/* sss3 */
	while ((GET_LONG(current_proc->a4)++)!=ANI_JUMP);
	((ADDRESS *)current_proc->a4)-=2;			// a4-> last stance frame

	/* a9 --> top 	a4 --> bottom	a5--> where we were before */
	if (current_proc->a5<(ADDRESS)current_proc->pa9)
		return;					//	b4 -> above the top --> use new ani

	if (current_proc->a5>current_proc->a4)
		return;					//	b4 -> below the bottom --> use new ani

	(ADDRESS)current_proc->pa9=current_proc->a5;

	return;
}

/******************************************************************************
 Function: void d_walkb_setup(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: drone setup walk backward move
******************************************************************************/
void d_walkb_setup(void)
{
	get_walk_info_b();

	/* dwset3 */
	init_anirate(current_proc->a0);
	set_x_vel(current_proc->pa8,(long)current_proc->a1);
	d_get_char_ani((short)current_proc->a2);

	return;
}

/******************************************************************************
 Function: void d_walkf_setup(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: drone setup walk forward move
******************************************************************************/
void d_walkf_setup(void)
{
	get_walk_info_f();

	/* dwset3 */
	init_anirate(current_proc->a0);
	set_x_vel(current_proc->pa8,(long)current_proc->a1);
	d_get_char_ani((short)current_proc->a2);

	return;
}

/******************************************************************************
 Function: LONG d_either_edge_a5(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: distance from closest side, (current_proc->a5)

 Description: get distance from closest edge
******************************************************************************/
LONG d_either_edge_a5(void)
{
	get_my_dfe();

	if (current_proc->a5 > current_proc->a6)
		current_proc->a5=current_proc->a6;

	/* eith3 */
	return(current_proc->a5);
}

/******************************************************************************
 Function: LONG d_front_me_a5(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: distance from closest side, (current_proc->a5)

 Description:
******************************************************************************/
LONG d_front_me_a5(void)
{
	LONG temp;

	d_get_my_dfe();
	temp=current_proc->a5;
	current_proc->a5=current_proc->a6;
	current_proc->a6=temp;

	/* beh1 */
	if ((current_proc->pa8)->oflags & M_FLIPH)
		current_proc->a5=current_proc->a6;

	return(current_proc->a5);
}

/******************************************************************************
 Function: LONG d_behind_me_a5(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: distance from closest side, (current_proc->a5)

 Description:
******************************************************************************/
LONG d_behind_me_a5(void)
{
	d_get_my_dfe();

	/* beh1 */
	if ((current_proc->pa8)->oflags & M_FLIPH)
		current_proc->a5=current_proc->a6;

	return(current_proc->a5);
}

/********************************************************
				FAQ
********************************************************/

long get_his_y_vel(void)
{
	return((long)current_proc->a0=(current_proc->pdata.p_otherguy)->oyvel.pos);
}

/******************************************************************************
 Function: WORD q_my_back_to_him(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - back to him
		  SYSTEM_CARRY_CLR - no

 Description: question: is my back to him
******************************************************************************/
WORD q_my_back_to_him(void)
{
	return(yes_means_no(am_i_facing_him));
}

/******************************************************************************
 Function: WORD q_is_he_dropping(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - falling
		  SYSTEM_CARRY_CLR - no

 Description: question: is the otherguy falling
******************************************************************************/
WORD q_is_he_dropping(void)
{
	OBJECT *ta0=current_proc->pdata.p_otherguy;

	if (ta0->oyvel.pos>0)
		q_yes;
	else q_no;
}

/******************************************************************************
 Function: WORD q_is_he_a_boss(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - boss
		  SYSTEM_CARRY_CLR - no

 Description: question: is the otherguy the boss
******************************************************************************/
WORD q_is_he_a_boss(void)
{
	if ((((current_proc->pdata.p_otherguy)->ochar)==FT_MOTARO) || (((current_proc->pdata.p_otherguy)->ochar)==FT_SK))
		q_yes;
	else q_no;
}

/******************************************************************************
 Function: WORD q_am_i_a_boss(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - boss
		  SYSTEM_CARRY_CLR - no

 Description: question: is the otherguy the boss
******************************************************************************/
WORD q_am_i_a_boss(void)
{
	if ((((current_proc->pa8)->ochar)==FT_MOTARO) || (((current_proc->pa8)->ochar)==FT_SK))
		q_yes;
	else q_no;
}

/******************************************************************************
 Function: WORD q_square_lower(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - lower
		  SYSTEM_CARRY_CLR - no

 Description: question: is the otherguy lower then square
******************************************************************************/
WORD q_square_lower(void)
{
	 return(q_is_he_lower(SCY(0x50)));
}

/******************************************************************************
 Function: WORD q_is_he_lower(short pa7)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa7 - distance to check against

 Returns: SYSTEM_CARRY_SET - lower
		  SYSTEM_CARRY_CLR - no

 Description: question: is the otherguy lower
******************************************************************************/
WORD q_is_he_lower(short pa7)
{
	OBJECT *ta0=current_proc->pdata.p_otherguy;

	if (ta0->oyvel.pos<0)			// heading up = no
		q_no;

	if (((current_proc->pdata.p_otherproc)->pdata.p_ganiy-ta0->oypos.u.intpos)>pa7)
		q_no;
	else q_yes;
}

/******************************************************************************
 Function: WORD q_is_he_below_ground(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - otherdude is reacting
		  SYSTEM_CARRY_CLR - no

 Description: question: is the otherguy below ground
******************************************************************************/
WORD q_is_he_below_ground(void)
{
	(short)current_proc->a0=(current_proc->pdata.p_otherguy)->oypos.u.intpos;
	if ((short)current_proc->a0<0)
		q_no;

	(short)current_proc->a1=(current_proc->pdata.p_otherproc)->pdata.p_ganiy;
	if (((short)current_proc->a0)>((short)current_proc->a1))
		q_yes;
	else q_no;
}
/******************************************************************************
 Function: WORD q_is_he_reacting(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - otherdude is reacting
		  SYSTEM_CARRY_CLR - no

 Description: question: is the otherguy reacting
******************************************************************************/
WORD q_is_he_reacting(void)
{
	if (d_get_his_action(current_proc)==ACT_REACTING)
		q_yes;
	else q_no;
}

/******************************************************************************
 Function: WORD q_is_he_dizzy(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - otherdude is dizzy
		  SYSTEM_CARRY_CLR - no

 Description: question: is the otherguy dizzy
******************************************************************************/
WORD q_is_he_dizzy(void)
{
	if ((current_proc->pdata.p_otherproc)->p_comp_flag==PC_DIZZY)
		q_yes;
	else q_no;
}

/******************************************************************************
 Function: WORD is_he_attacking(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	question: is the other dude attacking
******************************************************************************/
WORD is_he_attacking(void)
{
	WORD action;

	action=d_get_his_action(current_proc) & 0xff00;

	if (action==ACT_ATTACKS || action==ACT_BODY_PROPELL)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: WORD is_he_him(WORD ochar)

 By: David Schwartz

 Date: Dec 1994

 Parameters: ochar - guy to check againsts

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	question: is the other dude me
******************************************************************************/
WORD is_he_him(WORD ochar)
{
	if ((current_proc->pdata.p_otherguy)->ochar==ochar)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: WORD is_towards_me(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	question: is the other dude moving towards me
******************************************************************************/
WORD is_towards_me(void)
{
	long xvel;

	//xvel=(current_proc->pdata.p_otherguy)->oxvel.pos;
	xvel=(p1_obj==current_proc->pdata.p_otherguy)?p1_xvel.pos:p2_xvel.pos;

	if (xvel==0)
		return(SYSTEM_CARRY_CLR);

	if (xvel>0)
	{
		/* moving right */
		if (is_he_right(current_proc)==SYSTEM_CARRY_SET)
			return(SYSTEM_CARRY_CLR);						// moving right & he is right
		else return(SYSTEM_CARRY_SET);
	}
	else
	{
		/* moving left */
		if (is_he_right(current_proc)==SYSTEM_CARRY_SET)
			return(SYSTEM_CARRY_SET);						// moving left & he is right
		else return(SYSTEM_CARRY_CLR);
	}
}

/******************************************************************************
 Function: WORD q_will_he_reach_me(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	question: will the otherdude reach me
******************************************************************************/
WORD q_will_he_reach_me(void)
{
	OBJECT *ta8;
	PROCESS *ta13;
	WORD results;

	ta13=current_proc;
	ta8=ta13->pa8;

	current_proc=current_proc->pdata.p_otherproc;
	results=d_strike_check_a0_test(current_proc->pa8,current_proc->pdata.p_stk);

	current_proc=ta13;
	current_proc->pa8=ta8;

	return(results);
}

/******************************************************************************
 Function: WORD is_propell_close(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	question: is he close during propell
******************************************************************************/
WORD is_propell_close(void)
{
	if (is_he_body_propell()==SYSTEM_CARRY_CLR)
		q_yes;

	if (get_x_dist<SCX(0x70)+10)
		q_yes;
	else q_no;
}

/******************************************************************************
 Function: WORD is_flyk_close(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	question: is he close during flyk
******************************************************************************/
WORD is_flyk_close(void)
{
	if (is_he_body_propell()==SYSTEM_CARRY_CLR)
		q_yes;

	if (get_x_dist<SCX(0x80))
		q_yes;
	else q_no;
}

/******************************************************************************
 Function: WORD q_is_he_cornered(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	question: is he cornered
******************************************************************************/
WORD q_is_he_cornered(void)
{
	OBJECT *ta8;
	PROCESS *ta13;
	WORD results;

	ta13=current_proc;
	ta8=ta13->pa8;

	current_proc=current_proc->pdata.p_otherproc;
	results=q_am_i_cornered();

	current_proc=ta13;
	current_proc->pa8=ta8;

	return(results);
}

/******************************************************************************
 Function: WORD q_his_action_0(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	question: is his action 0
******************************************************************************/
WORD q_his_action_0(void)
{
	if (d_get_his_action(current_proc)==0)
		q_yes;
	else q_no;
}

/******************************************************************************
 Function: WORD yes_mean_no(FUNCW pa0)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	question: answers the passed in question in the opposite
******************************************************************************/
WORD yes_means_no(FUNCW pa0)
{
	if (pa0()==SYSTEM_CARRY_SET)
		q_no;
	else q_yes;
}

/******************************************************************************
 Function: WORD q_am_i_cornered(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	question: answers the question am i cornered
******************************************************************************/
WORD q_am_i_cornered(void)
{
	if (d_behind_me_a5()>SCX(0x90))
		q_no;
	else q_yes;
}

/******************************************************************************
 Function: inline void drone_long_jump(WORD pa0)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa0 - jump table offset

 Returns: None

 Description:	drone jump to code based on offset and table
******************************************************************************/
JUMPTBL drone_longs[]=
{
	d_finish_him,	// 0
	wait_forever,	// 1
	wait_forever,	// 2
	wait_forever,	// 3
	wait_forever,	// 4
	wait_forever,	// 5
	wait_forever,	// 6
	wait_forever	// 7
};

inline void drone_long_jump(WORD pa0)
{
	stack_jump(drone_longs[pa0]);
}

/******************************************************************************
 Function: void d_finish_him(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	drone finish him time
******************************************************************************/
FUNC dtbl_dfh[]=
{
	(FUNC)3,
	d_upcut_finish,																// dont look 2 much in a hurry
	d_upcut_finish,																// dont look 2 much in a hurry
	d_elbow
};
void d_finish_him(void)
{
	boss_branch(0x11);

	if (f_no_violence==0 && f_no_blood==0)
	{
		if (randper(350)==SYSTEM_CARRY_SET)
			drone_execute_fatality();			// replace arcade audit code with rnd event

		/* non_fatal_finish */
		if (randper(400)==SYSTEM_CARRY_SET)
		{
			if (curback==BKGD_SUBWAY_MOD || curback==BKGD_PIT_MOD || curback==BKGD_BELL_MOD || curback==BKGD_HADES_MOD)
			{
				stack_jump(d_background_fatal);
			}
		}
	}

	stack_jump(non_violent_finish);
}

void non_violent_finish(void)
{
	/* non_violent_finish */
	d_get_close_2_u();
	if (stance_wait_yes(0x40,q_is_he_dizzy)==SYSTEM_CARRY_CLR)
		d_fatality_abort();
	else random_do(dtbl_dfh);
}

void d_upcut_finish(void)
{
	d_stance_pause(0x10);				// dont look 2 much in a hurry
	do_duck();
	process_sleep(8);
	if (q_is_he_dizzy()==SYSTEM_CARRY_CLR)
		d_reaction_exit();
	else
	{
		stack_jump(d_uppercut);
	}
}

void d_background_fatal(void)
{
	d_get_close_2_u();
	if (stance_wait_yes(0x40,q_is_he_dizzy)==SYSTEM_CARRY_CLR)
		d_fatality_abort();
	else
	{
		stack_jump(do_pit_fatality);
	}
}

void drone_babality(void)
{
	face_opponent(current_proc->pa8);
	d_stance_setup();
	current_proc->a10=0x40;
	/* bab3 */
	do
	{
		d_next_anirate();
		process_sleep(1);
	}
	while(--current_proc->a10>0);

	stack_jump(stat_do_babality);
}

short ochar_friendship_distances[]=
{
	SCX(0x80),
	SCX(0x80),
	SCX(0x80),
	SCX(0x80),
	SCX(0x80),

	SCX(0x80),
	SCX(0x80),
	SCX(0xc0),
	SCX(0x80),
	SCX(0x80),

	SCX(0x80),
	SCX(0x80),
	SCX(0x80),
	SCX(0x80),
	SCX(0x80),

	SCX(0x80),
	SCX(0x80),
	SCX(0x80),
	SCX(0x80),
	SCX(0x80),

	SCX(0x80),
	SCX(0x80),
	SCX(0x80),
	SCX(0x80),
	SCX(0x80),
};

void drone_friendship(void)
{
	fatality_align(ochar_friendship_distances[(current_proc->pa8)->ochar]);
	stack_jump(do_friendship);
}

short ochar_animality_distances[]=
{
	SCX(0x50),		// kano
	SCX(0x50),		// sonya
	SCX(0x50),		// jax
	SCX(0x50),		// nightwolf
	SCX(0x50),		// subzero
	SCX(0x68),		// stryker
	SCX(0x50),		// sindel
	SCX(0x50),		// sektor
	SCX(0x50),		// cyrax
	SCX(0x50),		// lao
	SCX(0x50),		// kabal
	SCX(0x50),		// sheeva
	SCX(0x60),		// shang tsung
	SCX(0x68), 		// liu kang
	SCX(0xd0),		// smoke

	SCX(0x50),		// kitana
	SCX(0x60),		// jade
	SCX(0x80),		// mileena

	SCX(0xa0),		// smoke
	SCX(0xa0),		// smoke
	SCX(0x50),		// smoke
	SCX(0x50),		// smoke
	SCX(0x50),		// smoke
	SCX(0x50),		// smoke
};

void drone_animality(void)
{
	fatality_align(ochar_animality_distances[(current_proc->pa8)->ochar]);
	if (stance_wait_yes(0x40,q_is_he_dizzy)==SYSTEM_CARRY_CLR)
		d_fatality_abort();
	else
	{
		stack_jump(do_animality);
	}
}

short ochar_fatality_distances[][2]=
{
	{SCX(0xa0),SCX(0x48)},		// kano
	{SCX(0x80),SCX(0xe0)},		// sonya
	{SCX(0x50),SCX(0xd0)},		// jax
	{SCX(0x50),SCX(0xc0)},		// nightwolf
	{SCX(0xc0),SCX(0x4a)},		// subzero
	{SCX(0xf8),SCX(0x48)},		// stryker
	{SCX(0x50),SCX(0x70)},		// sindel
	{SCX(0x90),SCX(0xb0)},		// sektor
	{SCX(0x50),SCX(0x80)},		// cyrax
	{SCX(0x80),SCX(0x60)},		// lao
	{SCX(0xc0),SCX(0x50)},		// kabal
	{SCX(0x50),SCX(0x50)},		// sheeva
	{SCX(0x50),SCX(0x70)},		// shang tsung
	{SCX(0x80),SCX(0x80)},		// liu kang
	{SCX(0xf0),SCX(0x80)},		// smoke

	{SCX(0x40),SCX(0x40)},		// kitana
	{SCX(0x50),SCX(0x40)},		// jade
	{SCX(0x100),SCX(0x50)},		// mileena

	{SCX(0x80),SCX(0xc0)},		// 
	{SCX(0xe0),SCX(0x60)},		// 
	{SCX(0xa0),SCX(0x48)},		// 
	{SCX(0x80),SCX(0x80)},		// 
	{SCX(0x80),SCX(0x80)},		// 
	{SCX(0x80),SCX(0x80)},		// 

};

void drone_execute_fatality(void)
{
	if (randper(200)==SYSTEM_CARRY_SET)		// determine if we want to have fun (small chance)
	{
		if (random() & 0x80000000)
		{
			drone_babality();
		}
		else
		{
			drone_friendship();
		}
	}

	/* def2 */
	if (randper(50)==SYSTEM_CARRY_SET)
	{
		drone_animality();
	}

	if (random() & 0x80000000)
	{
		/* do fatal 1 */
		fatality_align(ochar_fatality_distances[(current_proc->pa8)->ochar][0]);
		if (stance_wait_yes(0x40,q_is_he_dizzy)==SYSTEM_CARRY_CLR)
			d_fatality_abort();
		else
		{
			stack_jump(do_fatality_1);
		}
	}
	else
	{
		/* do fatal 2 */
		fatality_align(ochar_fatality_distances[(current_proc->pa8)->ochar][1]);
		if (stance_wait_yes(0x40,q_is_he_dizzy)==SYSTEM_CARRY_CLR)
			d_fatality_abort();
		else
		{
			stack_jump(do_fatality_2);
		}
	}
}

void d_get_close_2_u(void)
{
	if (get_x_dist>=SCX(0x100))
		d_fflip_jsrp();

	d_stalk_a11_ntl(SCX(0x40)-1);
}

/* pa0=distance */
void fatality_align(short pa0)
{
	(short)current_proc->a11=pa0;

	if (pa0<get_x_dist)
	{
		/* def5 */
		if (get_x_dist>=SCX(0x100))
			d_fflip_jsrp();

		/* def6 */
		if (fatality_stalk_a11(0x40*3,pa0)==SYSTEM_CARRY_CLR)
			d_fatality_abort();
		else return;
	}
	else
	{
		fatality_retreat_a11();
		return;
	}
}

/* pa10=time, pa11 = distance */
WORD fatality_stalk_a11(WORD pa10,short pa11)
{
	face_opponent(current_proc->pa8);
	d_walkf_setup();

	/* fstk3 */
	do
	{
		if (am_i_facing_him()==SYSTEM_CARRY_CLR)
			d_turnaround();
		process_sleep(1);
		d_check_winner_status();
		d_next_anirate();
		if (d_get_x_dist<pa11)
			return(SYSTEM_CARRY_SET);
	}
	while(--pa10>0);

	return(SYSTEM_CARRY_CLR);
}

/* (short)current_proc->a11 <-max distance */
void fatality_retreat_a11(void)
{
	current_proc->a10=0x40*2;
	do
	{
		if (q_am_i_cornered()==SYSTEM_CARRY_SET)
			d_fatality_cornered();

		if (abs(d_get_x_dist-(short)current_proc->a11)<SCX(0x100))
			break;

		d_bflip_jsrp();
	}
	while(1);

	/* fret4 */
	face_opponent(current_proc->pa8);
	d_walkb_setup();
	do
	{
		/* fret4 */
		process_sleep(1);
		d_next_anirate();
		if (d_get_x_dist>(short)current_proc->a11)
			return;
	}
	while(--current_proc->a10>0);

	d_fatality_abort();
}

void d_fatality_abort(void)
{
	stack_jump(victory_animation);
}

/**************************************************************************
*											     *
*      BEWARE										     *
*											     *
**************************************************************************/

/******************************************************************************
 Function: void d_init(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	drone init
******************************************************************************/
void d_init(void)
{
	current_proc->pdata.p_flags &=(~PM_JOY);		// flag : i am a drone

	dv_hangout=2;

	return;
}

JUMPTBL counter_nonattack[]=
{
	NULL,			// 300
	NULL,			// 301
	NULL,			// 302
	NULL,			// 303
	NULL,			// 304
	NULL,			// 305
	NULL,			// 306
	NULL,			// 307
	c_angle_jump,	// 308
	NULL,			// 309
	NULL,			// 30a
	NULL,			// 30b
	NULL,			// 30c
	NULL			// 30d
};

JUMPTBL counter_zap[]=
{
	NULL,			// ccc
	c_kano_zap,    	// 01
	c_sonya_zap,	// 02
	c_jax_zap1, 	// 03
	c_jax_zap2,  	// 04
	c_ind_zap,	// 05
	c_sky_ice,  	// 06
	NULL,		// 07	swat gun
	c_robo_zap2,	// 08
	c_robo_zap,	// 09
	c_robo_net,	// 0a
	c_lia_anglez,  	// 0b
	c_lao_zap,	// 0c
	c_robo_bomb,	// 0d
	c_tusk_zap,	// 0e
	c_floor_zap,	// 0f
	c_st_zap1,	// 10
	c_st_zap2,	// 11
	c_st_zap3,	// 12
	c_lk_zaphi,	// 13
	c_lk_zap_lo,   	// 14
	c_lk_zap_air,		// 15
	c_tusk_zap_air,		// 16
	c_sg_zap,    	// 17
	c_swat_bomb_hi,	// 18
	c_swat_bomb_lo,	// 19
	c_forward_ice,	// 1a
	c_lia_forward,	// 1b
	c_floor_blade,	// 1c
	c_spear,		// 1d
	c_kitana_zap,	// 1e
	c_air_fan,	// 1f
	c_jade_zap,	// 20
	c_reptile_orb,	// 21
	c_fast_orb,	// 22
	c_spit,		// 23
	c_mileena_zap,	// 24
	c_mil_air_zap,	// 25
	c_floor_ice,	// 26
	c_ermac_zap,	// 27
	NULL,		// 28
	NULL,		// 29
	NULL,		// 2a
};

JUMPTBL counter_attack_table[]=
{
	NULL,		// ccc
	c_hipunch,   	// 101 - hi punch
	c_lopunch,   	// 102 - lo punch
	c_hikick,	// 103 - hi kick
	c_lokick,	// 104 - lo kick
	c_rhouse,	// 105 - roundhouse kick
	c_duck_kickh,	// 106 - duck kick hi
	c_duck_kickl,	// 107 - duck kick lo
	c_duckpunch,	// 108
	c_knee,		// 109
	c_elbow,		// 10a
	c_jupkick, 	// 10b
	c_juppunch, 	// 10c
	c_sweep,		// 10d
	c_uppercut,  	// 10e
	c_lia_scream,	// 10f
	c_leg_grab,	// 110
	c_shake,		// 111
	c_noogy,		// 112
	c_quake,		// 113
	NULL,		// 114
	c_axe_up,	// 115
	NULL,		// 116
	c_kano_swipe, 	// 117
	c_quake,		// 118
	c_laospin,   	// 119
	NULL,		// 11a
	c_fan_lift,	// 11b
	c_swat_gun,	// 11c
	c_ermac_slam,	// 11d
	NULL,
	NULL,
	NULL,
};

JUMPTBL counter_propell[]=
{
	c_flykick,	// 200 - flying (flip) kick
	c_flypunch,	// 201 - flying (flip) punch
	c_sz_decoy,	// 202 -
	NULL,		// 203
	c_sbike,		// 204
	c_jax_dash,	// 205
	c_lao_tele,	// 206
	c_square,	// 207
	c_robo_tele,  	// 208
	NULL,		// 209
	c_tele_explode,	// 20a
	c_kano_roll,  	// 20b
	c_lao_angle, 	// 20c
	c_bike,		// 20d
	c_superkang,	// 20e
	c_sg_pounce, 	// 20f
	c_bike,		// 210
	c_zoom,		// 211
	c_swat_stick,	// 212
	c_tusk_blur,	// 213
	c_ind_charge,	// 214
	c_scorp_tele,	// 215
	c_reptile_dash,	// 216
	c_jade_prop,	// 217
	c_mileena_tele,	// 218
	c_mileena_roll,	// 219
	c_kano_upball,	// 21a
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

JUMPTBL counter_sitting_duck[]=
{
	NULL,		// 600
	NULL,		// 601
	c_kroll_sd,  	// 602
	NULL,		// 603
	c_proj_sd,	// 604
	c_icharge_sd,		// 605
	NULL,		// 606
	c_caught_net,	// 607
	c_leg_sd, 	// 608
	NULL,		// 609
	NULL,		// 60a
	NULL,		// 60b
	NULL,		// 60c
	NULL,		// 60d
	NULL,		// 60e
	NULL,		// 60f
	c_frozen,	// 610
	c_sbike_sd,	// 611
	c_superk_sd,	// 612
	NULL,		// 613
	c_stsw_sd,	// 614
	c_screamed,	// 615
	NULL,		// 616
	NULL,		// 617
	c_kswipe_sd,	// 618
	NULL,		// 619
	NULL,		// 61a
	NULL,	//act_noogy_sd	// 61b
	c_ssd,		// 61c
	c_lk_bike_sd,	// 61d
	c_zoom_sd,	// 61e
	c_tusk_blur_sd,	// 61f
	NULL,		// 620
	NULL,		// 621
	c_robo_tele_sd,	// 622
	c_speared,	// 623
	NULL,		// 624
	NULL,		// 625
	NULL,		// 626
	NULL,		// 627
	NULL,		// 628
	c_swat_gun_sd,	// 629
	c_er_slam_sd,	// 62a
	NULL,		// 62b
	c_jaxdash_sd,	// 62c
	c_upball_sd,	// 62d
	c_slam_bounce,  	// 62e
	NULL,		// 62f
	NULL,		// 630
	NULL,		// 631
	NULL,		// 632
};

JUMPTBL counter_unrelated[]=
{
	NULL,			// 500
	NULL,			// 501
	NULL,			// 502
	NULL,			// 503
	NULL,			// 504
	NULL,			// 505
	NULL,			// 506
	c_react_flipk,	 	// 507
	NULL,			// 508
	NULL,			// 509
	NULL,			// 50a
	NULL,			// 50b
	NULL,			// 50c
};


JUMPTBL *drone_catagory_table[]=
{
	counter_zap,				// 0
	counter_attack_table,	// 1
	counter_propell,			// 2
	counter_nonattack,		// 3
	NULL,					// 4
	counter_unrelated, 		// 5
	counter_sitting_duck,	// 6
	NULL					// 7
};

JUMPTBL **ochar_cat_tables[]=
{
	drone_catagory_table,		// 0
	drone_catagory_table,		// 1
	drone_catagory_table,		// 2
	drone_catagory_table,		// 3
	drone_catagory_table,		// 4
	drone_catagory_table,		// 5
	drone_catagory_table,		// 6
	drone_catagory_table,		// 7
	drone_catagory_table,		// 8
	drone_catagory_table,		// 9
	drone_catagory_table,		// 10
	drone_catagory_table,		// 11
	drone_catagory_table,		// 12
	drone_catagory_table,		// 13
	drone_catagory_table,		// 14 - smoke
	drone_catagory_table,		// 17
	drone_catagory_table,		// 18
	drone_catagory_table,		// 19
	drone_catagory_table,		// 20
	drone_catagory_table,		// 21
	drone_catagory_table,		// 22
	drone_catagory_table,		// 23
	motaro_catagory_table,	// 24 - motaro
	sk_catagory_table,		// 25 - shao kahn
};

JUMPTBL crf_tbl[]= 
{
	d_do_kano_swipe,			// 0 - kano
	d_leg_grab,			// 1 - sonya
	d_propell_attack_now,		// 2 - jax
	d_zap_now,			// 3 - ind
	d_zap_now,			// 4 - sz - slide
	d_do_swat_gun,			// 5 - swat - 
	d_lia_scream,			// 6 - 
	d_propell_attack_now,		// 7 - 
	d_zap_now,			// 8 - 
	NULL,		// 9 - 
	d_propell_attack_now,	 	// 10 -
	d_zap_now,			// 11 - sheeva - tele
	d_zap_now,			// 12 - shang
	d_bike_kick,			// 13 - 
	NULL,	   	// 14 -
	d_zap_now,			// 15 - 
	d_propell_attack_now,		// 16 - jade
	d_do_mileena_roll,			// 17 - 
	d_zap_now,			// 18 - scorp
	do_fast_orb,			// 19 - reptile
	do_mystic_drop,			// 20 - 
	d_do_floor_ice,			// 21
	d_zap_now,			// 22
	drone_sweep_closeup_sd,	// 23
};

void c_react_flipk(void) 
{
	if (should_i_promove()==SYSTEM_CARRY_CLR) 
	{
		return_to_beware;
	}
	else
	{
		if (crf_tbl[(current_proc->pa8)->ochar]!=NULL) 
		{
			crf_tbl[(current_proc->pa8)->ochar]();
		}
	}

	return_to_beware;
}

/******************************************************************************
 Function: void d_beware(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	drone beware of other code
******************************************************************************/
void d_beware(void)
{
	WORD ta2;
	WORD ta1;
	JUMPTBL **tbl;
	JUMPTBL *tblptr;

	if (winner_status !=0 )
		return;								// we have a winner

	if ((ta1=d_get_his_action(current_proc))==0)
		return;								// no act --> no react

	if (current_proc->pdata.p_otheract==ta1)			// same as before
		return;

	current_proc->pdata.p_otheract=ta1;

	ta2=ta1>>8;						// catagory offset

	tbl=ochar_cat_tables[(current_proc->pa8)->ochar];	// get correct player table

	tblptr=*(tbl+ta2);

	if (tblptr==NULL)
		return;						// no catagory

	ta1&=0xff;
	if (*(tblptr+ta1)==NULL)
		return;						// no reaction

	current_proc->pdata.p_beware_a10=current_proc->a10;
	current_proc->pdata.p_beware_a11=current_proc->a11;

	(*(tblptr+ta1))();				// react to action

	/* bew8 */
	return;
}

WORD rpt_angle[]={100,100,100,150,200,400,450,600,700,750};
void c_angle_jump(void)
{
	if (diff!=0)
	{
		if (f_aabuse>10)
			goto ANGLE_ABUSER_HERE;			// you abuse me I abuse you
	}

	/* let_em_abuse */
	if (ask_mr_diff(rpt_angle)==SYSTEM_CARRY_CLR)
	{
		return_to_beware;
	}

ANGLE_ABUSER_HERE:
	if ((current_proc->pdata.p_otherguy)->oxvel.pos==0) 
	{
		return_to_beware_4get;
	}

	if (is_towards_me()==SYSTEM_CARRY_SET)
		caj_towards_me();
	else
	{
		if (q_is_he_cornered()==SYSTEM_CARRY_SET)
			caj_corner();
		else
		{
			/* angle jump - away from me */
			if (d_get_x_dist<SCX(0x60))
				d_flip_punch_jump();

			if (d_get_x_dist<SCX(0xb0))
				zap_close_away_flipper();

		}
	}
	return_to_beware;
}

void zap_close_away_flipper(void)
{
	if ((current_proc->pa8)->ochar==FT_SUBZERO)
		d_zap_jump(ZAP_SZ_IBEHIND);

	if ((current_proc->pa8)->ochar==FT_SWAT)
		d_zap_jump(ZAP_SWAT_BOMBH);

	if ((current_proc->pa8)->ochar==FT_LK)
		d_lk_hi_zap_jump(ZAP_LK_HI);

	d_zap();
}

void do_fast_orb(void) 
{
	do_zap(ZAP_REP_ORBF);
	d_reaction_exit();
}

void d_lk_hi_zap_jump(WORD pa0)
{
	do_zap(ZAP_LK_HI);
	d_reaction_exit();
}

void d_zap_jump(WORD pa0)
{
	do_zap(pa0);
	d_reaction_exit();
}

JUMPTBL ochar_towards_counters[]=
{
	kano_upball,
	sonya_delayed_bike,
	jax_delayed_quake,
	ind_delayed_axe,
	sz_sky_zap,

	swat_zap_hi,
	lia_delayed_scream,
	d_propell_attack_now,		
	robo2_delayed_net,
	d_backup_zap,

	tusk_jump_up_zap,
	sg_delayed_quake,   
	st_delayed_whatever,
	kitana_jump_up_zap,
	d_propell_attack_now,		

	run_in_juk,
	run_in_juk,
	run_in_juk,

	run_in_juk,
	run_in_juk,
	run_in_juk,
	run_in_juk,
	run_in_juk,
	run_in_juk,
};

void caj_towards_me(void)
{
	if (get_his_y_vel()>0)
	{
		return_to_beware;
	}
	else
	{
		if ((current_proc->pa8)->ochar!=FT_LK && get_x_dist<SCX(0x80))
		{
			stack_jump(d_hi_kick);
		}

		/* cajt3 */
		if (get_x_dist<SCX(0x90))
		{
			stack_jump(d_jump_up_kick);
		}

//		if (get_x_dist<SCX(0xb0))
//		{
//			stack_jump(d_flip_punch_jump);
//		}

		if (ochar_towards_counters[(current_proc->pa8)->ochar]==NULL)
		{
			return_to_beware;
		}
		else
		{
			ochar_towards_counters[(current_proc->pa8)->ochar]();
		}
	}

	return;
}

void run_in_juk(void) 
{
	d_run_a11(0x30,SCX(0x70));
	d_jump_up_kick();	
}

void caj_corner(void)
{
	WORD ta10;

	if (get_x_dist<SCX(0x48))
	{
		stack_jump(d_uppercut);
	}

	if (get_x_dist<SCX(0x88))
	{
		stack_jump(d_flip_punch_jump);
	}

	if (get_x_dist<SCX(0xc0))
	{
		/* caj_corner_far */
		if (randper(500)==SYSTEM_CARRY_SET)
		{
			stack_jump(d_zap);
		}
		else
		{
			ta10=0x40*2;
			d_stalk_a11(&ta10,SCX(0xa0));
			d_reaction_exit();
		}
	}
	else
	{
		stance_wait_yes(0x40,q_corner_backf_land);
		stack_jump(d_zap);
	}
}

WORD q_corner_backf_land(void)
{
	if (abs(get_his_y_vel())>SCY(0x20000))
		q_no;
	else q_yes;
}

/******************************************************************************
 Function: void st_delayed_whatever(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:  various shang morph routines
******************************************************************************/
FUNC sdw_tbl[]=
{
	(FUNC)4,
	st2ind,
	st2sonya,
	st2tusk,
	st2lia
};
void st_delayed_whatever(void)
{
	random_do(sdw_tbl);
}

void st2ind(void)
{
	if ( q_shang_morph(FT_INDIAN)==SYSTEM_CARRY_SET )
	{
		do_st_2_ind();
		ind_delayed_axe();
	}
	else
	{
		d_backup_zap();
	}
}

void st2sonya(void)
{
	if ( q_shang_morph(FT_SONYA)==SYSTEM_CARRY_SET )
	{
		do_st_2_sonya();
 		sonya_delayed_bike();
	}
	else
	{
		d_backup_zap();
	}
}

void st2tusk(void)
{
	if ( q_shang_morph(FT_TUSK)==SYSTEM_CARRY_SET )
	{
		do_st_2_tusk();
		tusk_jump_up_zap();
	}
	else
	{
		d_backup_zap();
	}
}

void st2lia(void)
{
	if ( q_shang_morph(FT_LIA)==SYSTEM_CARRY_SET )
	{
		do_st_2_lia();
		lia_delayed_scream();
	}
	else
	{
		d_backup_zap();
	}
}

/******************************************************************************
 Function: void sg_delayed_quake(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:  delayed quake from sheeva
******************************************************************************/
void sg_delayed_quake(void)
{
	retreat_wait_yes(0x40,q_jax_smash);
	if (d_get_x_dist<SCX(0xc0))
	{
		/* sg_quake_abort */
		if (q_am_i_cornered()==SYSTEM_CARRY_SET)
		{
			stack_jump(d_flip_punch_jump);
		}
		else
		{
			stack_jump(d_bflip_jump);
		}
	}
	else
	{
		do_body_propell(PROP_SG_QUAKE);
	}
}

/******************************************************************************
 Function: void tusk_jump_up_zap(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:  jump up zap attack
******************************************************************************/
void tusk_jump_up_zap(void)
{
	do_jump_up(tusk_jup_scan);
	d_reaction_exit();
}

WORD tusk_jup_scan(void)
{
	if (distance_off_ground(current_proc,current_proc->pa8)<SCX(0x40))
		q_no;

	stack_jump(d_zap_now);
	q_yes;
}

/******************************************************************************
 Function: void robo2_delayed_net(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:  send net
******************************************************************************/
void robo2_delayed_net(void)
{
	retreat_wait_yes(0x40,q_is_he_net_close);
	stack_jump(d_zap);
}

WORD q_is_he_net_close(void)
{
	if (abs(get_his_y_vel())>SCY(0x10000))
		q_no;
	else q_yes;
}

/******************************************************************************
 Function: void d_backup_zap(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
void d_backup_zap(void)
{
	retreat_wait_yes(0x40,q_backup_zap);
	stack_jump(d_zap);
}

WORD q_backup_zap(void)
{
	if (get_his_y_vel()<0)
		q_no;

	if (distance_off_ground(current_proc->pdata.p_otherproc,current_proc->pdata.p_otherguy)>SCY(0x70))
		q_no;
	else q_yes;
}

/******************************************************************************
 Function: void lia_delayed_scream(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
void lia_delayed_scream(void)
{
	stalk_wait_yes(0x40,q_is_he_scream_close);
	do_lia_scream();
	d_reaction_exit();
}

WORD q_is_he_scream_close(void)
{
	if (d_get_x_dist>SCX(0xb0))
		q_no;
	else q_yes;
}

/******************************************************************************
 Function: void swat_zap_hi(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:  zapem hi
******************************************************************************/
void swat_zap_hi(void)
{
	do_zap(ZAP_SWAT_BOMBH);
	d_reaction_exit();
}

/******************************************************************************
 Function: void sz_sky_zap(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:  freeze him
******************************************************************************/
void sz_sky_zap(void)
{
	do_zap(ZAP_SZ_IFRONT);
	d_reaction_exit();
}

/******************************************************************************
 Function: void ind_delayed_axe(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:  axe up
******************************************************************************/
void ind_delayed_axe(void)
{
	stalk_wait_yes(0x40,q_is_he_axe_close);
	do_axe_up();
	d_reaction_exit();
}

WORD q_is_he_axe_close(void)
{
	if (d_get_x_dist>SCX(0x70))
		q_no;
	else q_yes;
}

/******************************************************************************
 Function: void jax_delayed_quake(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:  shake the world
******************************************************************************/
void jax_delayed_quake(void)
{
//	if (q_am_i_cornered()==SYSTEM_CARRY_SET)
//	{
//		return_to_beware;
//	}

	retreat_wait_yes(0x40,q_jax_smash);

	if (d_get_x_dist<SCX(0xb0))
		d_reaction_exit();
	do_quake();
	d_reaction_exit();
}

void d_quake(void) 
{
	do_quake();
	d_reaction_exit();
}

WORD q_jax_smash(void)
{
	if (get_his_y_vel()<0)
		q_no;
	else q_yes;
}

void kano_upball(void) 
{
	stalk_wait_yes(0x40,q_is_he_bike_close);

	do_body_propell(PROP_KANO_UBALL);
	d_reaction_exit();
}

void d_do_mileena_roll(void) 
{
	do_body_propell(PROP_MIL_ROLL);
	d_reaction_exit();
}

/******************************************************************************
 Function: void sonya_delayed_bike(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:  let ride a bike
******************************************************************************/
void sonya_delayed_bike(void)
{
	stalk_wait_yes(0x40,q_is_he_bike_close);

	do_body_propell(PROP_SONYA_BIKE);
	d_reaction_exit();
}

WORD q_is_he_bike_close(void)
{
	if (d_get_x_dist>SCX(0x80))
		q_no;
	else
	{
		q_yes;
	}
}

void c_icharge_sd(void) 
{
	nr_uppercut_if_u_can();
	c_tusk_blur_sd();
	return;
}

void c_tusk_blur_sd(void)
{
	d_run_a11(0x40,SCX(0x40));
	d_knee();
}

void c_reptile_dash(void) 
{
	if (count_q_repeats(ACT_TUSK_BLUR)!=0) 
		d_block();

	if (should_i_promove()==SYSTEM_CARRY_SET)
		d_block();
	
	return_to_beware;
}

/******************************************************************************
	ZAPS
******************************************************************************/
/******************************************************************************
 Function: void c_robo_bomb(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose robo bomb response
******************************************************************************/
JUMPTBL cav_bomb[]=
{
	c_bomb,
	c_bomb
};
void c_robo_bomb(void)
{
	react_jump_table_act(cav_bomb);
	return;
}

/******************************************************************************
 Function: void c_st_zap2(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose shang tsu zap
******************************************************************************/
JUMPTBL cav_st_zap2[]=
{
	c_stzap23,
	c_stzap23
};
void c_st_zap2(void)
{
	react_jump_table_act(cav_st_zap2);
	return;
}

void c_stzap23(void)
{
	nr_sweep_if_u_can();
	duck_under_mproj();
}

/******************************************************************************
 Function: void c_lk_zap_lo(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose liu kang lo zap
******************************************************************************/
JUMPTBL cav_lk_zaplo[]=
{
	c_lkzaplo,
	c_lkzaplo
};
void c_lk_zap_lo(void)
{
	react_jump_table_act(cav_lk_zaplo);
	return;
}

void c_lkzaplo(void)
{
	if (d_get_x_dist<SCX(0x80))
	{
		stack_jump(d_crossover_kick);
	}
	else
	{
		stack_jump(d_fflip_kick_jump);
	}
}

/******************************************************************************
 Function: void c_lao_zap(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose lao zap
******************************************************************************/
JUMPTBL cav_lao_zap[]=
{
	c_hat,
	c_hat
};
void c_lao_zap(void)
{
	react_jump_table_act(cav_lao_zap);
	return;
}

void c_hat(void)
{
	nr_sweep_if_u_can();

	if (d_get_x_dist<SCX(0xc0))
	{
		stack_jump(d_block_projectile);
	}
	else
	{
		stack_jump(d_flipk_over_proj);
	}
}

/******************************************************************************
 Function: void c_robo_net(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose robo net
******************************************************************************/
JUMPTBL cav_robo_net[]=
{
	d_block_projectile,
	d_block_projectile
};
void c_robo_net(void)
{
	react_jump_table_act(cav_robo_net);
	return;
}

void c_net(void)
{
	if (d_get_x_dist<SCX(0x80))
	{
		stack_jump(d_block_projectile);
	}
	else
	{
		stack_jump(d_flipk_over_proj);
	}
}

/******************************************************************************
 Function: void c_robo_zap2(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose robo tracker
******************************************************************************/
JUMPTBL cav_robo_zap2[]=
{
	c_tracker_rocket,
	c_tracker_rocket
};
void c_robo_zap2(void)
{
	react_jump_table_act(cav_robo_zap2);
	return;
}

void c_tracker_rocket(void)
{
	nr_sweep_if_u_can();

	if (d_get_x_dist<SCX(0xe0))
	{
		stack_jump(run_in_close);
	}

	stw_proj_proc();

	stance_wait_yes(0x70,q_is_tracker_close);

	stack_jump(run_in_close);
}

WORD q_is_tracker_close(void)
{
	PROCESS *pa0;

	pa0=get_his_proj_proc();

	if (pa0==NULL)
		q_yes;

	if (abs((pa0->pa8)->oxpos.u.intpos-(current_proc->pa8)->oxpos.u.intpos)<SCX(0x70))
		q_yes;
	else q_no;
}

/******************************************************************************
 Function: void c_lia_anglez(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose
******************************************************************************/
JUMPTBL cav_lia_anglez[]=
{
	c_lia_zap,
	c_lia_zap
};
void c_lia_anglez(void)
{
	react_jump_table_act(cav_lia_anglez);
	return;
}

void c_lia_zap(void)
{
	if (d_get_x_dist<SCX(0xc0))
	{
		/* cliaz3 */
		d_run_a11(0x40,SCX(0x40));
		d_uppercut();
	}

	if (d_get_x_dist>SCX(0xf0))
		d_fflip_kick_jump();

	if (q_am_i_cornered()==SYSTEM_CARRY_CLR)
		d_bflip_jump();

	stw_proj_proc();
	do_jump_up(NULL);
	d_run_a11(0x40,SCX(0x40));
	d_reaction_exit();
}

/******************************************************************************
 Function: void c_swat_bomb_lo(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose
******************************************************************************/
JUMPTBL cav_swat_blo[]=
{
	c_bomblo_pro,
	c_bomblo_pro
};
void c_swat_bomb_lo(void)
{
	react_jump_table_act(cav_swat_blo);
	return;
}

void c_bomblo_pro(void)
{
	nr_sweep_if_u_can();
	if (d_get_x_dist<SCX(0xb0))
		duck_under_proj();
	else d_fflip_kick_jump();
}

/******************************************************************************
 Function: void c_swat_bomb_hi(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose
******************************************************************************/
JUMPTBL cav_swat_bhi[]=
{
	d_block_projectile,
	c_bombhi_pro
};
void c_swat_bomb_hi(void)
{
	react_jump_table_act(cav_swat_bhi);
	return;
}

void c_bombhi_pro(void)
{
	nr_sweep_if_u_can();
	if (d_get_x_dist<SCX(0x110))
		duck_under_proj();
	else run_in_close();
}

/******************************************************************************
 Function: void c_sky_ice(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose
******************************************************************************/
JUMPTBL cav_sz_skyice[]=
{
	d_block_projectile,
	c_sky_pro
};
void c_sky_ice(void)
{
	react_jump_table_act(cav_sz_skyice);
	return;
}

void c_sky_pro(void)
{
	nr_sweep_if_u_can();
	stance_wait_no(0x30,q_is_proj_gone);
	run_in_close();
}

/******************************************************************************
 Function: void c_floor_blade(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	counter floor blade
******************************************************************************/
void c_floor_blade(void)
{
	if (d_get_x_dist<SCX(0xd0))
		run_in_close_now();

	if (d_get_x_dist<SCX(0xf0))
		d_fflip_kick_jump();

	run_then_flipk();
}

WORD q_does_blade_exist(void)
{
	if (((PROCESS *)current_proc->a0=process_exist(PID_BLADE,-1))==NULL)
		q_no;
	else q_yes;
}

WORD q_is_blade_close(void)
{
	PROCESS *ptemp;

	if (q_does_blade_exist()==SYSTEM_CARRY_CLR)
		q_yes;

	ptemp=(PROCESS *)current_proc->a0;

	if (ptemp->pdata.p_otherguy!=current_proc->pa8)
		q_yes;								// this is mine

	if (abs((current_proc->pa8)->oxpos.u.intpos-(ptemp->pa8)->oxpos.u.intpos)>SCX(0x50))
		q_no;					// to far
	else
	{
		current_proc->a11=0;
		q_yes;
	}
}

void c_floor_ice(void) 
{
	if (ask_mr_diff(rpt_promoves)==SYSTEM_CARRY_CLR) 
	{
		return_to_beware;
	}

	if (d_get_x_dist<SCX(0x70)) 
	{
		run_in_close();
	}
	else
	{
		stack_jump(drone_zone);
	}
}

/******************************************************************************
 Function: void c_jax_zap2(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose
******************************************************************************/
JUMPTBL cav_jax_zap[]=
{
	duck_under_mproj,
	duck_under_mproj
};
void c_jax_zap2(void)
{
	react_jump_table_act(cav_jax_zap);
	return;
}

/******************************************************************************
 Function: void c_st_zap1(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose
******************************************************************************/
JUMPTBL cav_st_zap1[]=
{
	standard_zap_counter,
	standard_zap_counter
};

DMJ szc[]=
{
	{FT_JADE,jade_anti_zap},
	{FT_ROBO1,d_propell_attack_now},
	{FT_INDIAN,indian_reflect},
	{FT_SG,d_propell_attack_now},
	{FT_MILEENA,d_propell_attack_now},
	{FT_SMOKE,d_propell_attack_now},
	{FT_ROBO2,cyrax_counter_zap},
	{FT_LK,lk_zap_low},
	{-1,NULL},
};

void standard_zap_counter(void) 
{
	ochar_match_jump(szc);

	if ((current_proc->pdata.p_otherguy)->ochar==FT_ROBO2)
		d_block_projectile();

	if (get_x_dist<SCX(0xa0))
		duck_under_proj();

	if (get_x_dist>SCX(0xf0))
		run_then_flipk();

	if (get_x_dist>SCX(0xd0))
		run_then_duck_under();

	nr_sweep_if_u_can();

	nr_jump_over_proj();
}

void c_st_zap1(void)
{
	react_jump_table_act(cav_st_zap1);
	return;
}

/******************************************************************************
 Function: void duck_under_proj(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	duck under projectile
******************************************************************************/
void duck_under_proj(void)
{
	do_duck();
	wait_proj_spawn();
	wait_proj_pass();
	d_backup_jump();
}

void run_then_flipk(void) 
{
	d_run_till_yes(0x30,q_run_then_flipk);
	d_fflip_kick_jump();
}

WORD q_run_then_flipk(void) 
{
	if (get_x_dist<SCX(0xf0))
		q_yes;
	else q_no;
}

void run_then_duck_under(void) 
{
	d_run_till_yes(0x30,q_run_then_duck);
	duck_under_proj();
}

WORD q_run_then_duck(void) 
{
	if (get_x_dist<SCX(0xb0))
		q_yes;
	else return(q_his_proj_proc());
}

void lk_zap_low(void) 
{
	d_zap_jump(ZAP_LK_LO);
}

void indian_reflect(void) 
{
	do_stationary(STAT_REFLECT);
	d_reaction_exit();
}

void do_st_summon(void) 
{
	do_zap(ZAP_ST_SUMMON);
	d_reaction_exit();
}

void d_do_swat_gun(void) 
{
	do_stationary(STAT_SWATG);
	d_reaction_exit();
}

void d_do_kano_swipe(void) 
{
	do_stationary(STAT_KSWIPE);
	d_reaction_exit();
}

void do_mystic_drop(void) 
{
	do_stationary(STAT_ERMACS);
	d_reaction_exit();
}

void cyrax_counter_zap(void) 
{
	d_body_propell(PROP_TELE_EXP);
	d_reaction_exit();
}

void jade_anti_orb(void) 
{
	do_stationary(STAT_JADE);
	run_in_close_now();
}

void c_ermac_slam(void) 
{
	if (ask_mr_diff(rpt_promoves)==SYSTEM_CARRY_CLR) 
	{
		return_to_beware;
	}

	if (d_get_x_dist<SCX(0x70))
		run_in_close();

	if (d_get_x_dist<SCX(0xf0))
		d_zap_now();

	d_block();
}

DMJ cro_tbl[]= 
{
	{FT_ROBO1,d_propell_attack_now},
	{FT_ROBO2,d_zap_now},
	{FT_SMOKE,d_propell_attack_now},
	{FT_JADE,jade_anti_orb},
	{FT_MILEENA,d_propell_attack_now},
	{FT_SCORPION,scorp_anti_orb},
	{FT_SG,d_propell_attack_now},
	{FT_SUBZERO,d_zap_now},
	{-1,NULL}
};
void c_reptile_orb(void) 
{
	if (ask_mr_diff(rpt_promoves)==SYSTEM_CARRY_CLR) 
	{
		return_to_beware;
	}
	
	ochar_match_jump(cro_tbl);

	if (d_get_x_dist<SCX(0xf0))
		run_in_fk();

	if (d_get_x_dist<SCX(0xb0))
		run_in_and_slam();

	if (d_get_x_dist>SCX(0xe0))
		d_fflip_kick_jump();

	stack_jump(block_orb);
}

void block_orb(void) 
{
	face_opponent(current_proc->pa8);
	do_block_hi();
	wait_proj_spawn();
	d_wait_yes_still(0x50,q_is_proj_gone);
	d_reaction_exit();
}

void run_in_fk(void) 
{
	d_run_a11(0x40,SCX(0xd0));
	d_fflip_kick_jump();
}

void scorp_anti_orb(void) 
{
	if (d_get_x_dist<SCX(0xb0))
		run_in_and_slam();

	if (d_get_x_dist>SCX(0xf0))
		d_propell_attack_now();

	block_orb();
}

void run_in_and_slam(void) 
{
	d_run_a11(0x40,SCX(0x40));
	d_slam();
}

void c_mil_air_zap(void) 
{
	if ((ground_y-(current_proc->pdata.p_otherguy)->oxpos.u.intpos)>SCX(0xa0))
		d_zap_now();

	duck_under_proj();
}

void c_lk_zap_air(void) 
{
	if (distance_off_ground(current_proc->pdata.p_otherproc,current_proc->pdata.p_otherguy)>SCY(0x18))
		d_zap_now();

	duck_under_proj();
}

/******************************************************************************
 Function: void duck_under_mproj(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	duck under multi projectiles
******************************************************************************/
void duck_under_mproj(void)
{
	do_duck();
	wait_proj_spawn();
	d_wait_yes_still(0x50,q_is_proj_gone);
	d_backup_jump();
}

WORD q_is_proj_gone(void)
{
	if (get_his_proj_proc()==NULL)
		q_yes;
	else q_no;
}

/******************************************************************************
 Function: void wait_proj_spawn(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	wait for a projectile to be created
******************************************************************************/
void wait_proj_spawn(void)
{
	current_proc->a10=0x20;			// max wait time
	do
	{
		process_sleep(1);
		if (--current_proc->a10)
			break;		// goto wps9

		if (process_exist(p1p2_pick(current_proc->pa8,PID_PROJ1,PID_PROJ2),0xf00)!=NULL)
			break;		// goto wait_proj_found

		/* no proj found */
		current_proc->a1=get_his_action(current_proc);

		if (current_proc->a1==ACT_PROJ_SD || current_proc->a1==0 || current_proc->a1 > 0xff)
			break;		// goto wps9
	}
	while(1);

	/* wps9, wait_proj_found */
	return;
}

/******************************************************************************
 Function: void wait_proj_pass(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	wait for a projectile to pass by
******************************************************************************/
void wait_proj_pass(void)
{
	PROCESS *ta0;
	OBJECT *ta5;

	do
	{
		process_sleep(1);

		ta0=get_his_proj_proc();

		if (ta0==NULL)
			return;

		ta5=ta0->pa8;										// a5 = proj obj
		(WORD *)current_proc->a0=ta0->pdata.p_joyport;		// a0 = stk table
		current_proc->a3=ta5->oxpos.u.intpos;
		current_proc->a2=*((WORD *)current_proc->a0);		// a2 = x offset (stk table)
		current_proc->a4=*((WORD *)(((WORD *)current_proc->a0)+2));		// a4 = x size (stk table)
		if (ta5->oflags & M_FLIPH)
		{
			(short)current_proc->a2=-((short)current_proc->a2);
			(short)current_proc->a4=-((short)current_proc->a4);
		}

		/* wpp5 */
		(short)current_proc->a3+=((short)current_proc->a2);
		(short)current_proc->a3-=((short)current_proc->a4);		// tail end of proj scan
		(short)current_proc->a2=(current_proc->pa8)->oxpos.u.intpos;

		if (ta5->oflags & M_FLIPH)
		{
			current_proc->a1=current_proc->a2;
			current_proc->a2=current_proc->a3;
			current_proc->a3=current_proc->a1;
		}
	}
	while(((short)current_proc->a2)>((short)current_proc->a3) );

	ta0->pdata.p_action=ACT_PROJ_PASSED;			// flag: this proj is pass my ahh
	return;
}

/******************************************************************************
 Function: PROCESS *get_his_proj_proc(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	get a ptr to other dudes projectle
******************************************************************************/
PROCESS *get_his_proj_proc(void)
{
	PROCESS *ta0;

	current_proc->a1=p1p2_pick(current_proc->pa8,PID_PROJ2,PID_PROJ1);		// id we are looking for

	ta0=(PROCESS *)&active_head;
	while ((ta0=ta0->plink)!=NULL)
	{
		if ((ta0->procid & 0xff00)==current_proc->a1)
			break;
	}

	return(ta0);
}

/******************************************************************************
 Function: void nr_jump_over_proj(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
void nr_jump_over_proj(void)
{
	if (d_get_x_dist<=SCX(0xb6))
		d_rets;
	else
	{
		if (d_get_x_dist<=SCX(0xf0))
			d_fflip_kick_jump();
		else
		{
			stack_jump(d_flipk_over_proj);
		}
	}
}

/******************************************************************************
 Function: void d_flipk_over_proj(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
void d_flipk_over_proj(void)
{
	stw_proj_proc();
	stance_wait_yes(0x20,q_proj_jclose);
	stack_jump(d_fflip_kick_jump);
}

/******************************************************************************
 Function: WORD q_his_proj_proc(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
WORD q_his_proj_proc(void)
{
	if (get_his_proj_proc()==NULL)
		q_no;
	else q_yes;
}

/******************************************************************************
 Function: WORD q_proj_jclose(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
WORD q_proj_jclose(void)
{
	short ta5;
	WORD ta0;

	ta0=(current_proc->pa8)->ochar;

	if (ta0==FT_SCORPION || ta0==FT_SMOKE || ta0==FT_OLDSMOKE)
		ta5=SCX(0xc0);
	else ta5=SCX(0x90);

	if (abs(his_proj_front_x()-(current_proc->pa8)->oxpos.u.intpos)>ta5)
		q_no;
	else q_yes;
}

/******************************************************************************
 Function: short his_proj_front_x(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
short his_proj_front_x(void)
{
	PROCESS *ta7;
	OBJECT *ta5;
	WORD *ta0;

	ta7=get_his_proj_proc();

	/* -SONY- bug fix (arcade/SONY????) */
	if (ta7==NULL)
		return(10000);

	ta0=ta7->pdata.p_joyport;

	ta5=ta7->pa8;			// a5 - proj obj
	(short)current_proc->a3=ta5->oxpos.u.intpos;
	(short)current_proc->a2=*ta0;		// a2 = x offset (stk table)
	(short)current_proc->a4=*(ta0+2);
	if (ta5->oflags & M_FLIPH)
	{
		(short)current_proc->a2=-((short)current_proc->a2);
		(short)current_proc->a4=-((short)current_proc->a4);
	}

	(short)current_proc->a3+=(short)current_proc->a2;
	(short)current_proc->a3+=(short)current_proc->a4;

	return((short)current_proc->a3);
}

/******************************************************************************
 Function: void c_floor_zap(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
JUMPTBL cav_fzap[]=
{
	run_in_close_now,
	run_in_close_now
};
void c_floor_zap(void)
{
	react_jump_table_act(cav_fzap);
}

/******************************************************************************
	SITTING DUCKS
******************************************************************************/

/******************************************************************************
 Function: void c_stsw_sd(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
void c_slam_bounce(void) 
{
	d_hi_kick();
}

void c_stsw_sd(void)
{
	if (nr_attack_sd()==SYSTEM_CARRY_SET)
	{
		return_to_beware;
	}

	nr_uppercut_if_u_can();
	nr_sweep_if_u_can();
	if (d_get_x_dist>SCX(0x90))
		d_zap();
	d_reaction_exit();
}

/******************************************************************************
 Function: void c_robo_tele_sd(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
void c_robo_tele_sd(void)
{
	if (nr_attack_sd()==SYSTEM_CARRY_SET)
	{
		return_to_beware;
	}
	nr_uppercut_if_u_can();
	d_hi_kick();
}

void c_jaxdash_sd(void) 
{
	if (should_i_promove()==SYSTEM_CARRY_CLR) 
	{
		return_to_beware;
	}

	if (d_get_x_dist>SCX(0x56))
		run_in_close();

	d_knee();
}

/******************************************************************************
 Function: void c_lk_bike_sd(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	liu kang bike sitting duck response
******************************************************************************/
void c_superk_sd(void)
{
	WORD ta10=0x30;

	d_stalk_a11(&ta10,SCX(0x48));
	nr_uppercut_if_u_can();
	d_rapid_lo();
}

/******************************************************************************
 Function: void c_screamed(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
void c_screamed(void)
{
	WORD ta10=0x30;

	d_stalk_a11(&ta10,SCX(0x45));
	stance_wait_no(0x30,q_is_he_airborn);
	d_elbow();
}

void c_er_slam_sd(void) 
{
	if (should_i_promove()==SYSTEM_CARRY_CLR) 
	{
		return_to_beware;
	}

	if (d_get_x_dist<SCX(0xa0))
		attack_closeup_sd();

	run_in_close();
}

void c_proj_sd(void)
{
	if (should_i_promove()==SYSTEM_CARRY_CLR) 
	{
		return_to_beware;
	}

	if (d_get_x_dist<SCX(0xa0))
		attack_closeup_sd();

	drone_sweep_closeup_sd();
}

void drone_sweep_closeup_sd(void) 
{
	nr_sweep_if_u_can();
	return_to_beware;
}

JUMPTBL dwcsd_tbl[]=
{
	d_propell_attack_now,		// 0 - kano
	d_leg_grab,			// 1 - sonya
	d_propell_attack_now,		// 2 - jax
	d_propell_attack_now,		// 3 - ind
	d_propell_attack_now,		// 4 - sz - slide
	d_propell_attack_now,		// 5 - swat - 
	d_lia_scream,			// 6 - 
	d_zap_now,			// 7 - 
	d_zap_now,			// 8 - 
	d_zap_now,			// 9 - 
	d_propell_attack_now,		// 10 - tusk - spin
	d_propell_attack_now,		// 11 - sheeva - tele
	d_zap_now,			// 12 - shang
	d_bike_kick,			// 13 - 
	d_zap_now,			// 14 - smoke = spear
	d_zap_now,			// 15 - 
	d_propell_attack_now,		// 16 - jade - shadow kick,
	drone_sweep_closeup_sd,	// 17 - 
	d_zap_now,			// 18 - scorp
	d_zap_now,			// 19 - reptile
	drone_sweep_closeup_sd,	// 20 - 
	drone_sweep_closeup_sd,	// 21
	drone_sweep_closeup_sd,	// 22
	drone_sweep_closeup_sd,	// 23
};

void attack_closeup_sd(void) 
{
	dwcsd_tbl[(current_proc->pa8)->ochar]();
}

void d_leg_grab(void) 
{
	do_stationary(STAT_LEG_THROW);
}

void d_lia_scream(void) 
{
	do_lia_scream();
	d_reaction_exit();
}

void d_bike_kick(void) 
{
	d_body_propell(PROP_LK_BIKE);
}

void c_caught_net(void)
{
	WORD ta10;

	if (should_i_promove()==SYSTEM_CARRY_CLR) 
	{
		return_to_beware;
	}

	ta10=0x30;
	d_stalk_a11(&ta10,SCX(0x40));
	if (q_is_he_airborn()==SYSTEM_CARRY_SET)
		d_uppercut();
	else
	{
		run_in_close_now();
	}
}

void c_frozen(void)
{
	WORD ta10=0x30;

	if (d_get_x_dist>=SCX(0x70))
	{
		run_in_close_now();
	}

	/* c_froze_closer */

	d_stalk_a11(&ta10,SCX(0x40));
	if (q_is_he_airborn()==SYSTEM_CARRY_CLR)
		d_attack_very_close();

	if (distance_off_ground(current_proc->pdata.p_otherproc,current_proc->pdata.p_otherguy)<SCY(0x10))
		d_uppercut();

	d_jump_up_kick();
}

/******************************************************************************
 Function: void c_sbike_sd(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	sonya bike sitting duck response
******************************************************************************/
void c_sbike_sd(void)
{
	if (nr_attack_sd()==SYSTEM_CARRY_SET)
	{
		return_to_beware;
	}

	if (get_x_dist>SCX(0xd0))
	{
		stack_jump(d_zap);
	}
	else
	{
		stack_jump(d_fflip_kick_jump);
	}
}

/******************************************************************************
 Function: void c_zoom_sd(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
void c_zoom_sd(void)
{
	if (nr_attack_sd()==SYSTEM_CARRY_SET)
	{
		return_to_beware;
	}

	willy_go_round();
}

/******************************************************************************
 Function: void c_leg_sd(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
void c_leg_sd(void)
{
	if (nr_attack_sd()==SYSTEM_CARRY_SET)
	{
		return_to_beware;
	}

	if (get_x_dist>SCX(0xb0))
	{
		stack_jump(d_zap);
	}
	else
	{
		stack_jump(d_propell_attack);
	}
}

/******************************************************************************
 Function: void willy_go_round(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
void willy_go_round(void)
{
	WORD ta10=0x30;

	d_stalk_a11(&ta10,SCX(0x40));
	stance_wait_yes(0x30,q_willy_uppercut);
	stack_jump(d_uppercut);
}

WORD q_willy_uppercut(void)
{
	if (distance_off_ground(current_proc->pdata.p_otherproc,current_proc->pdata.p_otherguy)>SCY(0x40))
		q_no;
	else q_yes;
}

/******************************************************************************
 Function: WORD nr_attack_sd(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - return to beware
		  SYSTEM_CARRY_CLR - normal processing to continue

 Description:	MOD FROM ARCADE SINCE WE NEED THE C LANG TO FALL BACK TO BEWARE ROUTINES
******************************************************************************/
WORD nr_attack_sd(void)
{
	if (should_i_promove()==SYSTEM_CARRY_CLR)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
	PROPELLS
******************************************************************************/

/******************************************************************************
 Function: void c_sz_decoy(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose sz decoy response
******************************************************************************/
void c_sz_decoy(void)
{
	if (diff<3)
	{
		if (ask_mr_diff(rpt_counter)==SYSTEM_CARRY_CLR)
		{
			return_to_beware;
		}
	}

	counter_decoy();
}

JUMPTBL cdecoy_tbl[] =
{
	decoy_zap_wait,
	decoy_zap_wait,
	decoy_zap_wait,
	decoy_zap_wait,
	decoy_zap_wait,

	decoy_zap_wait,
	decoy_zap_wait,
	d_propell_attack,				// 7 - robo1
	decoy_zap_wait,
	decoy_zap_wait,

	decoy_zap_wait,
	decoy_zap_wait,
	decoy_zap_wait,
	decoy_zap_wait,
	d_propell_attack,

	decoy_zap_wait,
	decoy_zap_wait,
	decoy_zap_wait,
	decoy_zap_wait,
	decoy_zap_wait,

	decoy_zap_wait,
	decoy_zap_wait,
	decoy_zap_wait,
	decoy_zap_wait,
	decoy_zap_wait,
};

void counter_decoy(void)
{
	cdecoy_tbl[(current_proc->pa8)->ochar]();
}

void decoy_zap_wait(void)
{
	if (ochar_zaps[(current_proc->pa8)->ochar]>=0)
		do_zap(ochar_zaps[(current_proc->pa8)->ochar]);

	decoy_wait();
}

void decoy_wait(void)
{
	stance_wait_no(0x80,q_is_decoy_alive);
	d_reaction_exit();
}

WORD q_is_decoy_alive(void)
{
	if (process_exist(p1p2_pick(current_proc->pa8,PID_DECOY2,PID_DECOY1),-1)==NULL)
		q_no;
	else q_yes;
}

/******************************************************************************
 Function: void c_lao_tele(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose lao tele response
******************************************************************************/
void c_lao_tele(void)
{
	if (ask_mr_diff(rpt_counter)==SYSTEM_CARRY_CLR)
	{
		return_to_beware;
	}

	stance_wait_yes(0x50,q_is_he_below_ground);

	if (d_get_x_dist>SCX(0x70))
		d_lo_kick();

	if (diff>4)
	{
		stack_jump(d_uppercut);
	}
	else
	{
		stack_jump(d_lo_kick);
	}
}

/******************************************************************************
 Function: void c_sg_pounce(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose sg pounce
******************************************************************************/
JUMPTBL cav_pounce[]=
{
	av_pounce,
	av_pounce
};
void c_sg_pounce(void)
{
	react_jump_table_act(cav_pounce);
	return;
}

void av_pounce(void)
{
	run_setup();
	face_opponent(current_proc->pa8);
	current_proc->a10=0x10;
	/* avp3 */
	do
	{
		process_sleep(1);
		reduce_turbo_bar();
		d_next_anirate();
	}
	while(--current_proc->a10>0);

	stop_me(current_proc->pa8);
	d_turnaround_jsrp();
	swait_land_jsrp();
	run_in_close_now();
}

/******************************************************************************
 Function: void c_sbike(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose bike move response
******************************************************************************/
JUMPTBL cav_sbike[]=
{
	av_sonya_bike,
	av_sonya_bike
};
void c_sbike(void)
{
	react_jump_table_act(cav_sbike);
	return;
}

/******************************************************************************
 Function: void av_sonya_bike(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	avoid sonya bike
******************************************************************************/
void av_sonya_bike(void)
{
	if (get_x_dist>=SCX(0x70))
		swait_nonattack_jump();

	/* asb2 */
	do_duck();

	/* asb1 */
	while (1)
	{
		process_sleep(1);
		if (am_i_facing_him()==SYSTEM_CARRY_CLR)
		{
			/* asb3 */
			duck_turnaround();
			d_wait_nonattack(0x40*2);
			d_reaction_exit();
		}

		if (is_he_attacking()==SYSTEM_CARRY_CLR)
			d_reaction_exit();
	}
}

void swait_nonattack_jump(void)
{
 	stance_wait_no(0x40,is_he_attacking);
 	d_reaction_exit();
}

void swait_land_jsrp(void)
{
	stance_wait_no(0x40,is_he_airborn);
}

/******************************************************************************
 Function: void c_lao_angle(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose kung lao angles response
******************************************************************************/
JUMPTBL cav_langle[]=
{
	av_lao_angle,
	av_lao_angle
};
void c_lao_angle(void)
{
	react_jump_table_act(cav_langle);
	return;
}

/******************************************************************************
 Function: void av_lao_angle(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	avoid kung lao angle attack
******************************************************************************/
void av_lao_angle(void)
{
	stance_wait_yes(0x30,lao_angle_wait);

	if (d_get_his_action(current_proc)!=ACT_LAO_ANGLE)
		d_reaction_exit();						// not a threat

	d_block();
	return;
}

/******************************************************************************
 Function: WORD lao_angle_wait(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - angle is not a threat
		  SYSTEM_CARRY_CLR - threat

 Description:	answer is angle kick a threat
******************************************************************************/
WORD lao_angle_wait(void)
{
	if (d_get_his_action(current_proc)!=ACT_LAO_ANGLE)
		q_yes;

	if (d_get_x_dist<SCX(0x70))
		q_yes;
	else q_no;
}

/******************************************************************************
 Function: void c_tele_explode(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose teleport explode response
******************************************************************************/
JUMPTBL cav_te[]=
{
	av_tele_explode,
	av_tele_explode
};
void c_tele_explode(void)
{
	react_jump_table_act(cav_te);
	return;
}

/******************************************************************************
 Function: void av_tele_explode(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	avoid teleport explode
******************************************************************************/
void av_tele_explode(void)
{
	stance_wait_yes(0x10,q_my_back_to_him);

	if (am_i_facing_him()==SYSTEM_CARRY_CLR)
		d_turnaround_jsrp();

	/* avtel3 */
	d_stance_pause(0x10);					// DEBUG: do nada

	stack_jump(d_hi_kick);
}

/******************************************************************************
 Function: void c_robo_tele(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:  choose robot teleport response
******************************************************************************/
JUMPTBL cav_rtele[]=
{
	av_robo_tele,
	av_robo_tele
};
void c_robo_tele(void)
{
	react_jump_table_act(cav_rtele);
	return;
}

JUMPTBL cav_stele[]=
{
	av_scorp_tele,
	av_scorp_tele
};
void c_scorp_tele(void)
{
	react_jump_table_act(cav_stele);
	return;
}

void av_scorp_tele(void) 
{
	d_block();
}

/******************************************************************************
 Function: void av_robo_tele(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	avoid robot teleport
******************************************************************************/
void av_robo_tele(void)
{
	d_stance_pause(0x6);			// DEBUG: do nada
	d_block();
}

/******************************************************************************
 Function: void c_square(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	 choose sonya square response
******************************************************************************/
JUMPTBL cav_square[]=
{
	ct_square,
	ct_square
};
void c_square(void)
{
	react_jump_table_act(cav_square);
	return;
}

/******************************************************************************
 Function: void ct_square(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	counter sonya square
******************************************************************************/
void ct_square(void)
{
	if (d_get_x_dist<=SCX(0xfc)) 			// she is not 2 far to cross over
	{
		/* close */
		stance_wait_yes(0x40,q_my_back_to_him);
		d_turnaround_jsrp();
	}

	/* sq_far */
	stance_wait_yes(0x40,q_is_he_dropping);

	if (d_get_x_dist<SCX(0x50))
	{
		/* hit_drop_sonya */
		stance_wait_yes(0x40,q_square_lower);

		if (d_either_edge_a5()<SCX(0x50))
		{
			stack_jump(d_uppercut);
		}
		else
		{
			stack_jump(d_hi_kick);
		}
	}
	else
	{
		stack_jump(d_zap);
	}
}

/******************************************************************************
 Function: void c_kano_roll(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose kano roll reponse
******************************************************************************/
JUMPTBL cav_kroll[]=
{
	ct_propell,
	ct_propell
};
void c_kano_roll(void)
{
	react_jump_table_act(cav_kroll);
	return;
}

/******************************************************************************
 Function: void ct_propell(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	counter propell
******************************************************************************/
void ct_propell(void)
{
//	if (is_towards_me()==SYSTEM_CARRY_CLR)
//	{
//		return_to_beware;
//	}

	stance_wait_yes(0x30,is_propell_close);
	stack_jump(d_block);
}

/******************************************************************************
 Function: void c_jax_dash(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose jax dash roll reponse
******************************************************************************/
JUMPTBL cav_dash[]=
{
	d_block,
	ct_jax_dash
};
void c_jax_dash(void)
{
	react_jump_table_act(cav_dash);
	return;
}

void ct_jax_dash(void)
{
	do_duck();
	current_proc->a10=0x40;
	do
	{
		process_sleep(1);
		d_beware();
	}
	while(--current_proc->a10>0);
	d_reaction_exit();
}

/******************************************************************************
 Function: void c_zoom(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	counter propell zoom
******************************************************************************/
JUMPTBL cav_zoom[]=
{
	ct_zoom,
	ct_zoom
};
void c_zoom(void)
{
	react_jump_table_act(cav_zoom);
	return;
}

void ct_zoom(void)
{
	d_block();
}

/******************************************************************************
 Function: void c_flykick(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose flying kick response
******************************************************************************/
JUMPTBL cav_flyk[]=
{
	av_flyattack,
	av_flyattack
};
void c_flykick(void)
{
	react_jump_table_act(cav_flyk);
	return;
}

/******************************************************************************
 Function: void av_flyattack(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	avoid fly attack
******************************************************************************/
void av_flyattack(void)
{
	if (is_towards_me()==SYSTEM_CARRY_CLR)
	{
		/* away from me */
		if (get_x_dist<SCX(0x40))
		{
			stack_jump(d_block);
		}

		if (q_will_he_reach_me()==SYSTEM_CARRY_SET)
		{
			stack_jump(d_zap_now);
		}

		d_block();
	}

	/* flykick_towards_me */
	if (q_will_he_reach_me()==SYSTEM_CARRY_SET)
	{
		d_block();
	}

	if (get_x_dist<SCX(0x80))
	{
		stack_jump(d_block);
	}

	if ((current_proc->pdata.p_otherguy)->oyvel.pos<0)
		flykick_heading_up();

	if (distance_off_ground(current_proc->pdata.p_otherproc,current_proc->pdata.p_otherguy)<SCY(0x28))
		flykick_heading_up();
	
	/* flyk2 */
	stance_wait_yes(0x30,is_flyk_close);
	if (d_get_x_dist>=SCX(0xa0))
		d_reaction_exit();					// far away from me ---> exit

	if (q_is_he_airborn()==SYSTEM_CARRY_CLR)
		d_reaction_exit();

	stack_jump(d_block);

	/* flyk3 */
	return_to_beware;
}

void flykick_heading_up(void) 
{
	if (distance_off_ground(current_proc->pdata.p_otherproc,current_proc->pdata.p_otherguy)>SCY(0x38))
	{
		stack_jump(run_under_flykick);
	}

	do_duck();
	d_wait_yes_still(0x30,q_is_kick_over);

	nr_uppercut_if_u_can();

	run_in_close_now();
}

WORD q_is_kick_over(void) 
{
	if (d_get_his_action(current_proc)==ACT_POST_AIRSTK)
		q_yes;

	if (is_he_airborn()==SYSTEM_CARRY_CLR)
		q_yes;

	q_no;
}

void run_under_flykick(void) 
{
	d_run_till_yes(0x30,q_run_under_fk);
	stack_jump(d_flip_punch_jump);
}

WORD q_run_under_fk(void) 
{
	if (am_i_facing_him()==SYSTEM_CARRY_SET)
		q_no;

	q_yes;
}


/******************************************************************************
 Function: WORD is_he_body_propell(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes in body proell
		  SYSTEM_CARRY_CLR - nope

 Description:	question: is he attacking with a body propell move
******************************************************************************/
WORD is_he_body_propell(void)
{
	if ((d_get_his_action(current_proc) & 0xff00) == ACT_BODY_PROPELL)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: void c_swat_stick(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	choose swat stick response
******************************************************************************/
JUMPTBL cav_swst[]=
{
	ct_stick_sweep,
	ct_stick_sweep
};
void c_swat_stick(void)
{
	react_jump_table_act(cav_swst);
}

void ct_stick_sweep(void)
{
	if (is_throwing_allowed()==SYSTEM_CARRY_SET)
	{
		/* stsw_zap */
		if (d_get_x_dist<SCX(0x80))
			d_crossover_kick();

		if (d_get_x_dist>SCX(0xd0))
			d_zap();

		d_block();
	}

	if (d_get_x_dist<SCX(0x80))
		d_crossover_kick();

	d_block();
}

/******************************************************************************
 Function: void c_uppercut(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose uppercut response
******************************************************************************/
JUMPTBL cav_ucut[]=
{
	ct_uppercut,
	ct_uppercut
};
void c_uppercut(void)
{
	if (q_will_he_reach_me()==SYSTEM_CARRY_CLR)
	{
		return_to_beware;
	}
	else
	{
		react_jump_table_act(cav_ucut);
	}
}

void ct_uppercut(void)
{
	d_duck_fast();
	d_wait_nonattack(0x40*2);
	nr_uppercut_if_u_can();
	nr_sweep_if_u_can();
	d_reaction_exit();
}

/******************************************************************************
 Function: void c_jupkick(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose jump kick response
******************************************************************************/
JUMPTBL cav_juk[]=
{
	d_duck,
	d_duck
};
void c_jupkick(void)
{
	if (get_x_dist<=SCX(0x60))
		react_jump_table_act(cav_juk);

	return_to_beware;
}

/******************************************************************************
 Function: void c_duckpunch(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose duck punch response
******************************************************************************/
JUMPTBL cav_dp[]=
{
	d_block,
	d_crossover_kick
};
void c_duckpunch(void)
{
	if (q_will_he_reach_me()==SYSTEM_CARRY_CLR)
	{
		return_to_beware;
	}

	if (q_is_he_cornered()==SYSTEM_CARRY_SET)
		d_crossover_kick();

	react_jump_table_act(cav_dp);

}

/******************************************************************************
 Function: void ct_duck_kick(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	counter duck kick
******************************************************************************/
void ct_duck_kick(void)
{
	if (q_is_he_cornered()==SYSTEM_CARRY_SET)
	{
		stack_jump(d_block);
	}
	else
	{
		stack_jump(d_crossover_kick);
	}
}

/******************************************************************************
 Function: void c_knee(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose duck punch response
******************************************************************************/
JUMPTBL cav_knee[]=
{
	d_avoid_elbow_knee,
	d_avoid_elbow_knee
};
void c_knee(void)
{
	avoid_agressive_bastards();
	react_jump_table_act(cav_knee);
}

WORD rpt_elbow_knee[]={100,300,450,550,650,700,700,750,800,850};

void d_avoid_elbow_knee(void)
{
	if (ask_mr_diff(rpt_elbow_knee)==SYSTEM_CARRY_CLR)
	{
		stack_jump(d_block);
	}

	if (get_x_dist<SCX(0x4a))
	{
		stack_jump(d_slam);
	}
	else
	{
		stack_jump(d_block);
	}
}

void avoid_agressive_bastards(void)
{
	if (diff<2)
		d_rets;

	if (current_proc->pdata.p_action!=ACT_GETUP)
		d_rets;

	if (get_x_dist<SCX(0x48))
		d_slam();
	else
	{
		d_sweep_kick();
	}
}

/******************************************************************************
 Function: void c_sweep(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose sweep kick response
******************************************************************************/
JUMPTBL cav_sweep[]=
{
	av_sweep,
	ct_sweep,
};
void c_sweep(void)
{
	if (diff>=3)
	{
		if (current_proc->pdata.p_action==ACT_REACT_FLIPK || current_proc->pdata.p_action==ACT_GETUP)
			av_sweep();
	}

	/* csweep 3*/
	if (q_will_he_reach_me()==SYSTEM_CARRY_SET)
		react_jump_table_act(cav_sweep);

	return_to_beware;
}

/******************************************************************************
 Function: void av_sweep(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	avoid sweep kick
******************************************************************************/
void av_sweep(void)
{
	if (q_am_i_cornered()==SYSTEM_CARRY_SET)
	{
		stack_jump(d_duck_block);
	}
	else
	{
		stack_jump(d_bflip_jump);
	}
}

/******************************************************************************
 Function: void c_lia_scream(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	lia scream reponse
******************************************************************************/
void c_lia_scream(void)
{
	if (ask_mr_diff(rpt_counter)==SYSTEM_CARRY_CLR)
	{
		return_to_beware;
	}

	if (d_get_x_dist>SCX(0xa0))
	{
		stack_jump(d_zap_now);
	}

	if (d_get_x_dist<SCX(0x80)) 
	{
		stack_jump(d_sweep_kick);
	}

	retreat_wait_yes(0x40,q_dist_lift);

	stack_jump(d_zap_now);
}

WORD q_dist_lift(void) 
{
	if (d_get_x_dist>SCX(0xa0))
		q_yes;
	
	q_no;
}

void c_swat_gun(void)
{
	if (ask_mr_diff(rpt_counter)==SYSTEM_CARRY_CLR)
	{
		return_to_beware;
	}

	if (d_get_x_dist<SCX(0xd0))
	{
		stack_jump(d_zap_now);
	}

	stack_jump(d_block);
}

/******************************************************************************
 Function: void c_hipunch(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose hi punch response
******************************************************************************/
void c_hipunch(void)
{
	cpch3(ACT_HIPUNCH);
	return;
}

/******************************************************************************
 Function: void c_lopunch(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose lo punch response
******************************************************************************/
void c_lopunch(void)
{
	cpch3(ACT_LOPUNCH);
	return;
}

/******************************************************************************
 Function: void cpch3(WORD pa1)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa1 - action

 Returns: None

 Description:	choose punch response
******************************************************************************/
JUMPTBL cav_punch[]=
{
	d_sweep_kick,
	d_sweep_kick
};
void cpch3(WORD pa1)
{
	if (d_get_x_dist>SCX(0x60))
		return_to_beware_4get;

	react_jump_table(pa1,cav_punch);

	return_to_beware;
}

/******************************************************************************
 Function: void c_noogy(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose noogy reponse
******************************************************************************/
void c_noogy(void)
{
	ckik3(ACT_NOOGY);
	return;
}

/******************************************************************************
 Function: void c_shake(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose shake reponse
******************************************************************************/
void c_shake(void)
{
	ckik3(ACT_SHAKE);
	return;
}

/******************************************************************************
 Function: void c_rhouse(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose round house response
******************************************************************************/
void c_rhouse(void)
{
	if (d_get_x_dist>SCX(0xd0)) 
	{
		return_to_beware;
	}

	if (q_will_he_reach_me()==SYSTEM_CARRY_SET) 
	{
		stack_jump(ct_kicks);
	}

	d_stop_me(current_proc->pa8);
	nr_sweep_if_u_can();
	stack_jump(d_zap);
}

/******************************************************************************
 Function: void c_hikick(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose hi kick response
******************************************************************************/
void c_hikick(void)
{
	ckik3(ACT_HIKICK);
	return;
}

/******************************************************************************
 Function: void c_lokick(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	choose lo kick response
******************************************************************************/
void c_lokick(void)
{
	ckik3(ACT_LOKICK);
	return;
}

/******************************************************************************
 Function: void ckik3(WORD pa1)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa1 - action

 Returns: None

 Description:	choose punch response
******************************************************************************/
JUMPTBL cav_kick[]=
{
	d_block,
	ct_kicks
};
void ckik3(WORD pa1)
{
	if (q_will_he_reach_me()==SYSTEM_CARRY_SET)
		react_jump_table(pa1,cav_kick);

	/* kick_will_miss */
	nr_sweep_if_u_can();

	return_to_beware;
}

/******************************************************************************
 Function: void ct_kicks(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	counter kicks
******************************************************************************/
void ct_kicks(void)
{
	if (d_get_his_action(current_proc)==ACT_LOKICK)
	{
		stack_jump(d_duck);
	}

	if (d_strike_check_a0_test(current_proc->pa8,8)==SYSTEM_CARRY_SET)
	{
		stack_jump(d_duck_then_uppercut);
	}
	else
	{
		stack_jump(d_sweep_kick);
	}
}

/******************************************************************************
 Function: void c_quake(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	choose quake reponse
******************************************************************************/
JUMPTBL cav_quake[]=
{
	ct_quake,
	ct_quake
};
void c_quake(void)
{
	react_jump_table_act(cav_quake);
}

void ct_quake(void)
{
	nr_hikick_if_u_can();
	d_jumpup_nocall();
}

/******************************************************************************
 Function: void c_lao_spin(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	choose lao spin response
******************************************************************************/
JUMPTBL cav_lspin[]=
{
	ct_laospin,
	ct_laospin
};
void c_laospin(void)
{
	react_jump_table_act(cav_lspin);
}

void ct_laospin(void)
{
	WORD ta10;

	if (diff<=3)
	{
		ta10=0x40*3;
		d_stalk_a11(&ta10,70);
		nr_sweep_if_u_can();
	}

	/* laosp3 */
	if (d_get_x_dist>=SCX(0x70))
		d_stance_pause(0x20);

	d_zap();
}

/******************************************************************************
 Function: void c_kano_swipe(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	choose kano swipe
******************************************************************************/
JUMPTBL cav_kswipe[]=
{
	ct_kswipe,
	ct_kswipe
};
void c_kano_swipe(void)
{
	react_jump_table_act(cav_kswipe);
}

void ct_kswipe(void)
{
	if (d_get_x_dist<SCX(0x90))
	{
		stack_jump(d_block);
	}

	if (should_i_promove()==SYSTEM_CARRY_SET)
	{
		stack_jump(d_zap);
	}
	else
	{
		swait_nonattack_jump();
	}
}

/******************************************************************************
 Function: void c_axe_up(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	choose axe up
******************************************************************************/
JUMPTBL cav_axeu[]=
{
	ct_axe_up,
	ct_axe_up
};
void c_axe_up(void)
{
	if (q_will_he_reach_me()==SYSTEM_CARRY_CLR)
	{
		return_to_beware;
	}
	else react_jump_table_act(cav_axeu);
}

void ct_axe_up(void)
{
	nr_sweep_if_u_can();
	stack_jump(d_block);
}

/******************************************************************************
 Function: void c_leg_grab(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	choose leg grab reponse
******************************************************************************/
JUMPTBL cav_legg[]=
{
	ct_leg,
	ct_leg
};
void c_leg_grab(void)
{
	react_jump_table_act(cav_legg);
}

void ct_leg(void)
{
	if (q_will_he_reach_me()==SYSTEM_CARRY_CLR)
		swait_nonattack_jump();
	else
	{
		stack_jump(d_block);
	}
}

/******************************************************************************
 Function: void d_land(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	drone land
******************************************************************************/
void d_land(void)
{
	d_beware_mframew(2);
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_beware_mframew(WORD pa0)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa0 - anim rate

 Returns: None

 Description:	beware check and mframew animation
******************************************************************************/
void d_beware_mframew(WORD pa0)
{
	current_proc->pdata.p_otheract=0;
	init_anirate(pa0);

	/* dbmf3 */
	do
	{
		process_sleep(1);
		next_anirate();
		d_beware();
	}
	while(GET_LONG(current_proc->pa9)!=0);

	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: nr_kano_abuser_check(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	slam any kano abusers
******************************************************************************/
void nr_kano_abuser_check(void)
{
	if (q_is_kano_charging()==SYSTEM_CARRY_SET)
	{
		if (ask_mr_diff(rpt_anti_kano)==SYSTEM_CARRY_SET)
			d_slam();
	}

	return;
}

WORD q_is_kano_charging(void)
{
	OBJECT *obj=current_proc->pa8;
	OBJECT *oa0=current_proc->pdata.p_otherguy;

	if (oa0->ochar!=FT_KANO)
		q_no;

	if (obj->oid==OID_P1)
	{
		if ((swcurr.u.p1pad & P1B4)==0)
			q_no;
	}
	else
	{
		if ((swcurr.u.p2pad & P2B4)==0)
			q_no;
	}

	/* calla_for_him (get_last_kano_charge) */
	if (get_last_kano_charge(oa0)<0x48)
		q_no;
	else q_yes;
}

WORD get_last_kano_charge(OBJECT *pa8)
{
	return(get_tsl(l_lk,pa8));
}

/******************************************************************************
 Function: WORD should_i_promove(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:
******************************************************************************/
WORD should_i_promove(void)
{
	return(ask_mr_diff(rpt_counter));
}

/******************************************************************************
 Function: void react_jump_table(WORD pa1,FUNC pa2)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa1 - action to avoid repeats
			 pa2 - counter table/avoid table
			 current_proc->a7 - (0-3) (magnitude of hits)

 Returns: None

 Description:	react to given move
******************************************************************************/
WORD rpt_promoves[] = {100,500,680,700,750,775,785,795,800,850};

WORD rpt_counter[] = {100,350,550,600,700,750,800,800,800,900};

WORD d_randpers[] = {150,175,300,400,550,600,650,700,750,750};

WORD repeat_table[] = {3,3,3,2,2,2,2,1,1,1};

void react_jump_table(WORD pa1,FUNC *pa2)
{
	count_q_repeats(pa1);				// current_proc->a3 = # of hits so far
	if (repeat_table[diff]>current_proc->a3)
	{
		if (randper(d_randpers[diff])==SYSTEM_CARRY_CLR)
		{
			return_to_beware;
		}
		else
		{
			/* hdr5, react to him */

			if (ask_mr_diff(rpt_counter)==SYSTEM_CARRY_CLR)
				goto HDR7;
		}
	}

	/* hdr6 */
	pa2++;								// 2nd entry = counter

HDR7:
	(*pa2)();					// 1st entry = avoid
}

/******************************************************************************
 Function: WORD count_q_repeats(WORD pa1)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa1 - action

 Returns: current_proc->a3 - # of matches found
		 SYSTEM_ZERO_SET/CLR - depending on a3

 Description:	count number of matches with current action
******************************************************************************/
WORD count_q_repeats(WORD pa1)
{
	get_my_hitq(current_proc->pa8);				// a0 = my hit queue
	current_proc->a3=0;
	scan_1_entry(pa1);
	scan_1_entry(pa1);
	scan_1_entry(pa1);
	scan_1_entry(pa1);
	scan_1_entry(pa1);
	scan_1_entry(pa1);				// scan all six entries

	if (current_proc->a3==0)
		return(SYSTEM_ZERO_SET);
	else return(SYSTEM_ZERO_CLR);
}

/******************************************************************************
 Function: void scan_1_entry(WORD pa1)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa1 - action

 Returns: None

 Description:	scan and check for a match
******************************************************************************/
void scan_1_entry(WORD pa1)
{
	if (*((WORD *)current_proc->a0) == pa1)
		current_proc->a3++;

	((WORD *)current_proc->a0)++;

	return;
}

/**************************************************************************
*											     *
*  Drone external references								     *
*											     *
**************************************************************************/

/******************************************************************************
 Function: void d_flip_kick_jump(FUNC pa6)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa6 - drone routine to call

 Returns: None

 Description:	angle jump call routine
******************************************************************************/
void d_flip_kick_jump(FUNC pa6)
{
	do_flip_kick();
	d_reaction_exit();
}

/******************************************************************************
 Function: void d_reaction_exit(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	reaction exit
******************************************************************************/
void d_reaction_exit(void)
{
	reaction_exit(current_proc->pa8);
}

/******************************************************************************
 Function: void boss_branch(WORD pa0)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa0 - branch offset

 Returns: None

 Description:	boss branch routine
******************************************************************************/
void boss_branch(WORD pa0)
{
	if ((current_proc->pa8)->ochar==FT_MOTARO)
	{
		stack_jump(motaro_branches[pa0]);
	}

	if ((current_proc->pa8)->ochar==FT_SK)
	{
		stack_jump(sk_branches[pa0]);
	}
}

/* drone beginnings */