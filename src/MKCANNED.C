/******************************************************************************
 File: mkcanned.c

 Date: Nov 1994

 (C) Williams Entertainment

 Mortal Kombat III canned Routines
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
#include "mkmain.h"
#include "mkani.h"
#include "mkcanned.h"
#include "mkreact.h"
#include "mkjoy.h"
#include "mkdrone.h"
#include "mkinit.h"
#include "mkfx.h"
#include "mkpal.h"
#include "mksound.h"
#include "moves.h"
#include "mkscore.h"

#if 0
/******************************************************************************
 Function: new_endurance_guy(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	new endurance guy
******************************************************************************/
void new_endurance_guy(void)
{
	PROCESS *pa0;
	OBJECT *obj=current_proc->pa8;

	set_nocol(obj);
	match_me_with_him();
	ground_ochar();

	get_my_dfe();
	if (current_proc->a6<=current_proc->a5)			// set to closer edge
		obj->oxpos.u.intpos-=97;
		obj->oxpos.u.intpos+=97;

	/* calla_for_him(center_around_me) */
	pa0=current_proc;		// save current proc
	current_proc=current_proc->pdata.p_otherproc;	// no run/jab run
	center_around_me();
	current_proc=pa0;		// restore current Proc

	refresh_score();
	process_sleep(0x20);

	//-SONY- triple_sound(obj->ochar+0x28);		// announce new fighter, cant do it no sound avail
	face_opponent(obj);
	stack_jump(back_to_the_fight);
}
#endif


//******************************************************************************
//******************************************************************************
void tot_replacement_proc(void) 
{
	OBJECT *ta0;
	OBJECT *ta8;
	current_proc->pdata.p_store1=(ADDRESS)current_proc->a10;		// save loser

	ta0=current_proc->pdata.p_otherguy;
	ta8=current_proc->pa8;
	if (ta0!=NULL) 
	{
		if (ta0->ochar==ta8->ochar && ((ta0->oflags2 & M_UGLY_PAL)==0)) 
		{
			player_ugly_pal();
		}
		else
		{
			player_normpal();
		}
	}

	/* trp1 */
	//-DHS- triple_sound(0x28+ta8->ochar);		// not support, cant keep names in memory
	current_proc->pdata.p_ganiy=ta8->oypos.u.intpos;
	get_char_ani(ANIM_TABLE1,ANIM_FFLIP);
	current_proc->a6=0;
	gravity_ani(3,-SCY(0xa0000),ANGLE_GRAV);
	ochar_begin_calls();

	ta0=(OBJECT*)current_proc->a11;
	set_nocol(ta0);

	current_proc->a10=0x40*2;
	stance_setup();
	/* trp2 */
	do
	{
		process_sleep(1);
		if (--current_proc->a10==0)
			break;

		if (ta0->oypos.u.intpos==((PROCESS*)current_proc->pdata.p_store1)->pdata.p_ganiy && ta0->oyvel.pos==0)
			break;
	}
	while(1);

	/* trp3 */
	if (ta8->oid==OID_P1) 
	{
		p1_xvel.pos=0;		// stop x vel
		p1_obj->oxvel.pos=0;		// stop this guy

		//-DHS- due something about loser and make load new guy

		p1_obj=ta8;
		current_proc->procid=p1_proc->procid;
		p1_proc=current_proc;
		(p1_proc->pdata.p_otherproc)->pdata.p_otherproc=current_proc;
		(p1_proc->pdata.p_otherproc)->pdata.p_otherguy=ta8;

		p1_char=ta8->ochar;
		p1_bar=p1_bar_view=FULL_STRENGTH;
		refresh_score();
	}
	else
	{
		p2_xvel.pos=0;		// stop x vel
		p2_obj->oxvel.pos=0;		// stop this guy

		//-DHS- due something about loser and make load new guy

		p2_obj=ta8;
		current_proc->procid=p2_proc->procid;
		p2_proc=current_proc;
		(p2_proc->pdata.p_otherproc)->pdata.p_otherproc=current_proc;
		(p2_proc->pdata.p_otherproc)->pdata.p_otherguy=ta8;

		p2_char=ta8->ochar;
		p2_bar=p2_bar_view=FULL_STRENGTH;
		refresh_score();
	}

	reaction_exit(ta8);
}


WORD is_loser_on_ground(void) 
{
	OBJECT *obj;
	process_sleep(1);

	obj=(OBJECT*)current_proc->pdata.p_store1;
	if (obj->oyvel.pos==0)
		q_yes;
	
	q_no;
}

/******************************************************************************
 Function: void finish_him(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	finish him move time (winner status=3)
******************************************************************************/
void finish_him(void)
{
	if (current_proc->pdata.p_flags & PM_FINISH)
		return;

	if (current_proc->procid==PID_P1)
	{
		(WORD)current_proc->a0=p1_matchw;
		(WORD)current_proc->a1=p2_matchw;
	}
	else
	{
		(WORD)current_proc->a0=p2_matchw;
		(WORD)current_proc->a1=p1_matchw;
	}

	if (((WORD)current_proc->a0) < ((WORD)current_proc->a1))
	{
		dizzy_dude();					// more wins for me ?, no --> dizzy
	}
	else
	{
		current_proc->pdata.p_flags|=PM_FINISH;			// flag: i am finishing him

		/* fhim5 */
		if (am_i_joy()==SYSTEM_CARRY_CLR)
		{
			stack_jump(d_finish_him);
		}
		else
		{
			stack_jump_joy(joy_proc,JI_JOY_ENTRY);
		}
	}
}

FUNC dizzy_calls[]=
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,

	NULL,
	NULL,
	robo_dizzy_call,
	robo_dizzy_call,
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
	NULL
};

/******************************************************************************
 Function: void dizzy_dude(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	guy who's got his shit kicked
******************************************************************************/
void dizzy_dude(void)
{
	OBJECT *obj=current_proc->pa8;

	disable_all_buttons;
	face_opponent(obj);
	zero_my_p_hit(current_proc);

	if (winner_status==3)
	{
		obj->oflags2 &=(~M_NOFLIP);			// i can be flipped
		current_proc->pdata.p_flags|=PM_SITDUCK;	// match over ---> i'm a sitting duk
		set_no_block(obj);							// i cant block
	}

	/* dd4 */
	if (am_i_short()==SYSTEM_CARRY_SET)
		animate_a9(0x0004,ANIM_GETUP);

	/* dd5 */
	current_proc->pdata.p_action=ACT_STUNNED;		// define action

  init_anirate(6);

	get_char_ani(ANIM_TABLE1,ANIM_DIZZY);

	if (dizzy_calls[obj->ochar]!=NULL)
		dizzy_calls[obj->ochar]();

	/* dd7 */
	do
	{
		/* dizzy wake */
		current_proc->p_comp_flag=PC_DIZZY;			// signal other routine that at dizzy_wake
		process_sleep(1);
		current_proc->p_comp_flag=PC_CLEAR;

		/* dizzy */
		next_anirate();
	}
	while(winner_status==3 || f_death!=0);					// death blow started ??

	collapse_on_ground();
}

/******************************************************************************
 Function: void robo_dizzy_call(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	perform robo dizzy
******************************************************************************/
void robo_dizzy_call(void)
{
	PROCESS *ptemp;

	ptemp=create_fx(FX_ROBO_DIZZY);

	(PROCESS *)(ptemp->pdata.p_store1)=current_proc;

	return;
}

/******************************************************************************
 Function: void endurance_trans(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	winner status = 4, edurance transition
******************************************************************************/
void endurance_trans(void)
{
	if ((p1_obj==current_proc->pa8 && p1_bar==0) || (p2_obj==current_proc->pa8 && p2_bar==0))
		collapse_on_ground();
	else
	{
		stack_jump(endurance_wake);
	}
}

/******************************************************************************
 Function: void plwins(WORD pid)

 By: David Schwartz

 Date: Dec 1994

 Parameters: pid - proc id of winner

 Returns: None

 Description:	perform victory
******************************************************************************/
void plwins(WORD pid)
{
	if (current_proc->procid!=pid) 			// am i the winner
	{
		/* i lost */
		disable_all_buttons;
		collapse_on_ground();
	}
	else victory_animation();					// yes --> celebrate
}

/******************************************************************************
 Function: void psel_victory_animation(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	player select celebrate victory (victory_jumps)
******************************************************************************/
void psel_victory_animation(void)
{
	current_proc->pdata.p_flags&=(~PM_ALT_PAL);

	(ADDRESS)current_proc->pa9=(ADDRESS)(current_proc->pa8)->oslink;
	init_special(current_proc->pa8);
	(ADDRESS)(current_proc->pa8)->oslink=(ADDRESS)current_proc->pa9;
	f_victory_start=2;

	stack_jump(victory_animation);
}

void ending_victory_animation(void)
{
	stop_a8(current_proc->pa8);
	ground_player(current_proc->pa8);
	current_proc->pdata.p_flags|=PM_SPECIAL;
	f_victory_start=2;

	stack_jump(victory_animation);
}

/******************************************************************************
 Function: void victory_animation(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	celebrate victory (victory_jumps)
******************************************************************************/
void victory_animation(void)
{
	OBJECT *obj=current_proc->pa8;
	WORD ta1;


	ta1=obj->ochar;

	if ( obj->ochar!=FT_LIA || (f_final_act!=FAT_ANIMALITY && f_victory_start!=2))
	{
		clear_inviso(obj);
		set_shadow_bit(obj);
		init_special(obj);
		if (am_i_shang()==SYSTEM_CARRY_SET) 
			obj->ochar=FT_ST;
	}

	f_victory_start=1;

	switch (obj->ochar)
	{
		case FT_KANO:	// 0
			get_char_ani(ANIM_TABLE1,ANIM_VICTORY);
			mframew(4);
			break;

		case FT_SONYA:  // 1
			get_char_ani(ANIM_TABLE1,ANIM_VICTORY);
			mframew(5);
			process_sleep(12);
			mframew(4);
			break;

		case FT_JAX:	// 2
			get_char_ani(ANIM_TABLE1,ANIM_VICTORY);
			mframew(6);
			//mframew_2x(6,12);
			break;

		case FT_INDIAN:	// 3
			animate_a9(0x0005,ANIM_VICTORY);
			process_sleep(8);
			ochar_sound(2);
			do_next_a9_frame(obj);
			process_sleep(9);
			mframew(6);
			break;

		case FT_SWAT:	// 5
			get_char_ani(ANIM_TABLE1,ANIM_VICTORY);
			mframew(6);				// animate

			current_proc->a11=3;
			do
			{
				process_sleep(6);
				find_ani_part2(ANIM_VICTORY);
				mframew(2);				// BANG!
			}
			while(--current_proc->a11>0);
			delete_slave();
			mframew(6);
			break;

		case FT_LIA:	// 6
			if (gstate==GS_PSEL)
			{
				/* lia_psel_victory */
				find_ani_part2(ANIM_VICTORY);
				mframew(4);				// aniamte
				break;
			}
			else
			{
				/* lia_victory */
				set_ignore_y(obj);
				get_char_ani(ANIM_TABLE1,ANIM_VICTORY);
				if ( f_final_act!=FAT_ANIMALITY )
					stop_me(obj);
			}

			/* liav4 */
			mframew(4);				// aniamte

			if ( gstate==GS_GAMEOVER )
				break;						// dont float on gameover cast screen

			obj->ograv.pos=-SCY(0x4000);			// accel up
			process_sleep(0x0c);

			/* power down */
			/* LIAV6 */
			while(1)
			{
				obj->ograv.pos=(long)current_proc->a10=SCY(0x2000);			// head down

				/* LIAV3 */
				do
				{
					process_sleep(1);
				}
				while(obj->oyvel.pos<0);

				process_sleep(0x0d);

				/* power up */
				(long)current_proc->a10=-(long)current_proc->a10;
				obj->ograv.pos=(long)current_proc->a10;			// head down

				/* LIAV7 */
				do
				{
					process_sleep(1);
				}
				while(obj->oyvel.pos>0);

				process_sleep(0x0d);
			}
			break;

		case FT_MOTARO:
			ochar_sound(1);
			center_around_me();

		case FT_SK:
		case FT_TUSK:
		case FT_SG:
			get_char_ani(ANIM_TABLE1,ANIM_VICTORY);
			mframew(7);
			break;

		case FT_JADE:
			animate_a9(3,ANIM_VICTORY);
			break;

		case FT_ST:
			obj->ochar=ta1;
			if (obj->ochar!=FT_ST)
				back_to_shang_form();

		default:		// 4 5 7 8 9 ...
			get_char_ani(ANIM_TABLE1,ANIM_VICTORY);
			mframew(5);
			break;
	}

	wait_forever();
}

/******************************************************************************
 Function: void mframew_2x(WORD anispeed,WORD pause)

 By: David Schwartz

 Date: Dec 1994

 Parameters: anispeed - animation speed
			 pause - pause between animations

 Returns: None

 Description:	animate through a list of multipart frames twice
******************************************************************************/
void mframew_2x(WORD anispeed,WORD pause)
{
	mframew(anispeed);

	process_sleep(pause);

	mframew(anispeed);

	return;
}

short ochar_dead_adjusts[][2]=
{
	{26-82,-93-(-93)},		 // 0, kano
	{36-101+4,-96-(-97)+4},		 //   1, sonya
	{39-101,-106-(-110)-4},		 //   2, jax
	{33-95,-96-(-98)},		 //   3, indian
	{33-100,-95-(-103)-8},		 //   4, subzero
	{31-101,-97-(-94)},		 //   5, swat
	{45-83,-98-(-101)-4},		 //   6, lia
	{40-87,-93-(-97)},		 //   7, robo1
	{40-87,-93-(-97)},		 //   8, robo2
	{36-101,-91-(-91)},		 //   9, lao
	{33-85,-96-(-98)},		 //   a, tusk
	{54-113,-101-(-107)-8},		 //   b, sg
	{55-83,-89-(-86)+4},		 //   c, st
	{38-101,-93-(-99)-8+4},		 //   d, lk
	{40-87,-93-(-97)},		 //   e, smoke

	{40-87,-93-(-97)},		 //   e, smoke
	{40-87,-93-(-97)},		 //   e, smoke
	{40-87,-93-(-97)},		 //   e, smoke

	{40-87,-93-(-97)},		 //   e, smoke
	{40-87,-93-(-97)},		 //   e, smoke
	{40-87,-93-(-97)},		 //   e, smoke
	{40-87,-93-(-97)},		 //   e, smoke
	{40-87,-93-(-97)},		 //   e, smoke
	{40-87,-93-(-97)},		 //   e, smoke
	{40-87,-93-(-97)},		 //   e, smoke

	{SCX(0),0},		 //
	{SCX(0),0},		 //
	{SCX(0),0},		 //
	{SCX(0),0},		 //
	{SCX(0),0}		 //
};
/******************************************************************************
 Function: void collapse_on_ground()

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	collapse to ground
******************************************************************************/
void collapse_on_ground(void)
{
	OBJECT *obj=current_proc->pa8;

	player_normpal();
	set_noedge(obj);
	stop_me(obj);

	boss_branch(0x12);
	animate_a9(0x0004,ANIM_THUD);
	find_ani_part2(ANIM_KDOWN);
	find_last_frame();
	do_next_a9_frame(obj);

	multi_adjust_xy(obj,ochar_dead_adjusts[obj->ochar][0],ochar_dead_adjusts[obj->ochar][1]);

	shake_n_sound();
	clear_shadow_bit(obj);
	wait_forever();
}

void wings1(void) 
{
	wng1(-SCX(0x50000));
}

void wings2(void) 
{
	wng1(SCX(0x50000));
}

void wng1(long pa11) 
{
	current_proc->pdata.p_otherguy=(OBJECT *)current_proc->a0;
	stance_setup();

	current_proc->a10=0x40;
	/* wng3 */
	do
	{
		process_sleep(1);
		next_anirate();
	}
	while(--current_proc->a10>0);
	(current_proc->pa8)->oxvel.pos=pa11;

	current_proc->pdata.p_ganiy=(current_proc->pa8)->oypos.u.intpos;

	get_char_ani(ANIM_TABLE1,ANIM_BFLIP);
	current_proc->a6=0;
	gravity_ani(3,-SCY(0xa0000),ANGLE_GRAV);

#if !MULTI_OFF
	process_kill((current_proc->pa8)->oslink);	//-CHECK IF USED
#endif
	
	delobjp(current_proc->pa8);

	dallobj(OID_TEXT);
	process_suicide();
}