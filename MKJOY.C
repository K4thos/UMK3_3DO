/******************************************************************************
 File: mkjoy.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Joystick routines
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
#include "mkinit.h"
#include "mkstat.h"
#include "mkdrone.h"
#include "mkcombo.h"
#include "mkreact.h"
#include "mkfx.h"
#include "moves.h"
#include "mkscore.h"
#include "mkcanned.h"
#include "mksound.h"
#include "mkfile.h"

WORD bt_null[] =
{
	JI_JOY_IGNORE,			// 0
	JI_JOY_IGNORE,			// 1
	JI_JOY_IGNORE,			// 2
	JI_JOY_IGNORE,			// 3
	JI_JOY_IGNORE,			// 4
	JI_JOY_IGNORE,			// 5
	JI_JOY_IGNORE,			// 6
	JI_JOY_IGNORE,			// 7
	JI_JOY_IGNORE,			// 8
	JI_JOY_IGNORE			// 9
};

WORD bt_angle_jump[] =
{
	JI_JOY_FLIP_PUNCH,		// 0
	JI_JOY_FLIP_PUNCH,		// 1
	JI_JOY_IGNORE,			// 2
	JI_JOY_FLIP_KICK,		// 3
	JI_JOY_FLIP_KICK,		// 4
	JI_JOY_IGNORE,			// 5
	JI_JOY_IGNORE,			// 6
	JI_JOY_IGNORE,			// 7
	JI_JOY_IGNORE,			// 8
	JI_JOY_IGNORE			// 9
};

WORD bt_duck[] =
{
	JI_JOY_UPPERCUT,		// 0
	JI_JOY_DUCK_PUNCH,		// 1
	JI_JOY_DUCK_BLOCK,		// 2
	JI_JOY_DUCK_KICKH,		// 3
	JI_JOY_DUCK_KICKL,		// 4
	JI_JOY_IGNORE,			// 5
	JI_JOY_IGNORE,			// 6
	JI_JOY_IGNORE,			// 7
	JI_JOY_IGNORE,			// 8
	JI_JOY_IGNORE			// 9
};

WORD bt_stance[] =
{
	JI_JOY_HI_PUNCH,		// 0
	JI_JOY_LO_PUNCH,		// 1
	JI_JOY_BLOCK,			// 2
	JI_JOY_HI_KICK,			// 3
	JI_JOY_LO_KICK,			// 4
	JI_JOY_IGNORE,			// 5
	JI_JOY_IGNORE,			// 6
	JI_JOY_IGNORE,			// 7
	JI_JOY_IGNORE,			// 8
	JI_JOY_IGNORE			// 9
};

WORD bt_jump[] =
{
	JI_JUMPUP_PUNCH,		// 0
	JI_JUMPUP_PUNCH,		// 1
	JI_JOY_IGNORE,			// 2
	JI_JUMPUP_KICK,			// 3
	JI_JUMPUP_KICK,			// 4
	JI_JOY_IGNORE,			// 5
	JI_JOY_IGNORE,			// 6
	JI_JOY_IGNORE,			// 7
	JI_JOY_IGNORE,			// 8
	JI_JOY_IGNORE			// 9
};

void do_body_slam(void);

/******************************************************************************
 Function: void joy_begin(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:  startup for a joystick process
******************************************************************************/
void joy_begin(void)
{
	current_proc->pdata.p_flags|=PM_JOY;		// need this here for smoke
	ochar_begin_calls();
	current_proc->joyindex=JI_JOY_IGNORE;
	stack_jump(joy_proc);
}

/******************************************************************************
 Function: void endurance_wake(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:  startup for a endurance rounds
******************************************************************************/
void endurance_wake(void)
{
	back_to_normal();
	disable_all_buttons;

	/* endw0 */
	do
	{
		/* endw0 */
		stance_setup();						// get stance ani

		do
		{
			/* endw */
			process_sleep(1);
			if (am_i_facing_him()==SYSTEM_CARRY_SET)
			{
				turn_around();		// face the chump
				break;				// exit inner loop, so go back to endw0
			}

			/* endx */
			next_anirate();

			if (f_start)
			{
				/* start fighting */
				current_proc->joyindex=JI_JOY_IGNORE;
				stack_jump(joy_proc);
			}
		}
		while(1);
	}
	while(1);
}

/******************************************************************************
 Function: void switcheroo_wake(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:  startup for a switcheroo
******************************************************************************/
void switcheroo_wake(void)
{

	PROCESS *ptemp;
#if 0
	WORD new_char;
	/* switch to other guy */
	while ((new_char=randu(21)-1)==(current_proc->pa8)->ochar);			// dont pick same dude
	if (p1_obj==current_proc->pa8)
		(current_proc->pa8)->ochar=p1_char=p1_name_char=new_char;
	else (current_proc->pa8)->ochar=p2_char=p2_name_char=new_char;
#endif

	/* switch to other guy */
	if ((current_proc->pa8)->ochar==p1_heap_char)
	{
		/* switch to p2 heap and character */
		(current_proc->pa8)->ochar=p2_heap_char;
		(current_proc->pa8)->oheap=p2_heap;
		if ( current_proc->pa8==p1_obj )
			p1_char=p1_name_char=p2_heap_char;
		else p2_char=p2_name_char=p2_heap_char;
	}
	else
	{
		/* switch to p1 heap and character */
		(current_proc->pa8)->ochar=p1_heap_char;
		(current_proc->pa8)->oheap=p1_heap;
		if ( current_proc->pa8==p1_obj )
			p1_char=p1_name_char=p1_heap_char;
		else p2_char=p2_name_char=p1_heap_char;
	}

	/* deal with other dude changing to smoke */
	if ((current_proc->pa8)->ochar==FT_SMOKE)
	{
		ptemp=active_head;
		while ( ptemp!=NULL )
		{
			if ( ptemp->procid==PID_SMOKER && current_proc->pa8==ptemp->pa8)
				break;

			ptemp=ptemp->plink;
			if ( ptemp==NULL )
				create_fx(FX_SMOKE);
		}
	}
	/* update status */
	refresh_score();

	/* update clocks */
	update_clock_ones(entry_12,clk_ones);		// fix arcade bug on clocks being dispalyed screwy
	update_clock_tens(entry_11,clk_tens);

	//triple_sound(0x28+(current_proc->pa8)->ochar);		// announce switch
	clear_inviso(current_proc->pa8);

	current_proc->pdata.p_flags|=PM_ALT_PAL;
	player_normpal();

	pose_a9(0,0);
	ground_ochar();
	current_proc->pdata.p_ganiy=(current_proc->pa8)->oypos.u.intpos;

	current_proc->joyindex=JI_JOY_IGNORE;
	stack_jump(joy_proc);
}

/******************************************************************************
 Function: void joy_proc(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	Joystick control routine for players

	This routine was translated from the TMS code.  This code has multiple reentrant points
	and assumes the abilty to reset the stack pointer at any time.  This cannot be done in C
	without using a lot of extended GNU-C extensions.  Therefore the routine was designed to
	emulate the assembly as closly as possible.  That is why the code breaks all of C's rules!
******************************************************************************/
void joy_proc(void)
{
	WORD results;
	long joytemp;
	OBJECT *obj=current_proc->pa8;

	static void *jumptbl[] =
	{
		&&JOY_PROC,
		&&JOY_ENTRY,
		&&JOY_FLIP_PUNCH,
		&&JOY_FLIP_KICK,
		&&JOY_UPPERCUT,
		&&JOY_DUCK_PUNCH,
		&&JOY_DUCK_BLOCK,
		&&JOY_DUCK_KICKH,
		&&JOY_DUCK_KICKL,
		&&JOY_HI_PUNCH,
		&&JOY_LO_PUNCH,
		&&JOY_BLOCK,
		&&JOY_HI_KICK,
		&&JOY_LO_KICK,
		&&JUMPUP_PUNCH,
		&&JUMPUP_KICK,
		&&JOY_UP,
		&&JOY_DOWN,
		&&JOY_GETUP_ENTRY,
		&&JOY_BACK_UP,
		&&JOY_DUCK_BLOCK_LOOP,
		&&JOY_BLOCK_LOOP
	};

	WORD jumpindex;

	jumpindex=current_proc->joyindex;
	current_proc->joyindex=0;
	goto *jumptbl[jumpindex];

JOY_PROC:
	stuff_buttons(obj,bt_stance);

	current_proc->pdata.p_flags |=PM_JOY;					// flag: i am a joystick dude

	process_sleep(1);

JOYB4:
	wait_for_start();							// stance until fight begins

JOY_ENTRY:
	enable_all_buttons;								// allow all buttons to be valid

	if ((JOYSTICK_IN_A0(current_proc) & MASK_JDOWN)==0)
		goto JOYE4;

	if (get_my_height()<=SCY(0x60)) 			// fighter is not standing
		goto JOY_DUCK_ENTRY;
	else goto JOY_DOWN;							// fighter is standing

JOYE4:
	current_proc->pdata.p_downcount=0;				// not down anymore
	stance_setup();

/************************************
 * STANCE JOYSTICK 					*
 ************************************/
JOY_STANCE_LOOP:
	while (TRUE)
	{
		if (am_i_facing_him()==SYSTEM_CARRY_CLR)
		{
			turn_around();							// face the chump
			goto LOCAL_REACTION_EXIT;
		}

JOYE5:
		check_winner_status();						// still fighting?

		if (f_start==0)
			goto JOYB4;

		process_sleep(1);

		if (check_block_bit()==SYSTEM_CARRY_SET)	// block?
			goto JOY_BLOCK;

		if (JOYSTICK_IN_A0(current_proc) & MASK_JRIGHT)			// right?
			goto JOY_RIGHT;

		if (JOYSTICK_IN_A0(current_proc) & MASK_JLEFT)			// left?
			goto JOY_LEFT;

		if (JOYSTICK_IN_A0(current_proc) & MASK_JUP)				// up?
			goto JOY_UP;

		if (JOYSTICK_IN_A0(current_proc) & MASK_JDOWN)			// down?
			goto JOY_DOWN;

		back_to_shang_check();
		next_anirate();
	}

/************************************
 * LEFT/RIGHT JOYSTICK 				*
 ************************************/
JOY_LEFT:
	current_proc->pdata.p_store4=MASK_JLEFT;			// store joystick mask
	current_proc->a4=M_FLIPH ^ (M_FLIPH & obj->oflags);
	goto JOYR2;

JOY_RIGHT:
	current_proc->pdata.p_store4=MASK_JRIGHT;			// store joystick mask
	current_proc->a4=(M_FLIPH & obj->oflags);

JOYR2:
	current_proc->pdata.p_action=0;						// no more stance

	(LONG)current_proc->pa9=ANIM_WALKF;

	if (current_proc->a4!=0) 							// get walk info
		(LONG)current_proc->pa9=get_walk_info_b();
	else
	{
		if (is_run_pressed(obj)==SYSTEM_CARRY_SET)
			goto JOY_RUN;
		else (LONG)current_proc->pa9=get_walk_info_f();
	}

	/* JOYR4 */
	init_anirate(current_proc->a0);

	set_x_vel(obj,current_proc->a1);

	current_proc->pdata.p_store1=(LONG)current_proc->pa9;		// store walk ani offset here

	current_proc->pdata.p_anitab=(ADDRESS *)get_char_ani(ANIM_TABLE1,(LONG)current_proc->pa9);

JOYR6:
	do
	{
		process_sleep(1);
WALKWAKE:
		check_winner_status();

		if ((LONG)current_proc->pdata.p_store1==ANIM_WALKF && is_run_pressed(obj)==SYSTEM_CARRY_SET)
			goto JOY_RUN;

JOYR8:
		/* check for jump angles */
		switch (angle_scan())
		{
			case ANG_JUP_RIGHT:
				goto JOY_UPRIGHT;
				break;
			case ANG_JUP_LEFT:
				goto JOY_UPLEFT;
				break;
		};

		/* walk flip check */
		if (am_i_facing_him()==SYSTEM_CARRY_CLR)
		{
			turn_around();
			goto LOCAL_REACTION_EXIT;
		}

		next_anirate();
	}
	while(MASK_JOYSTICK(current_proc));							// still pressed --> walk again

	/* joystick was released */

	(long)current_proc->a0=abs((long)(current_proc->pdata.p_anitab-(ADDRESS *)current_proc->pa9));

	if ((long)current_proc->a0<3)	// walk frame # on, skip or stop
	{
//		/* skip */
//		(LONG)current_proc->pa9=(LONG)current_proc->pdata.p_store1+1;
//		get_char_ani(ANIM_TABLE1,(LONG)current_proc->pa9);			// skip animation offset
//		(ADDRESS *)current_proc->pa9=(ADDRESS *)current_proc->pa9+current_proc->a0;		// spot to finish off skip ani
//
//		animate_a0_frames(6,2);						// slower skip now!! & finish skip ani
	}

JOYR7:
	stop_me(obj);
	goto LOCAL_REACTION_EXIT;

/************************************
 * RUN JOYSTICK						*
 ************************************/
JOY_RUN:
	run_setup();

JRUN3:
	process_sleep(1);

	/* angle_scan routine */
	switch (angle_scan())
	{
		case ANG_JUP_RIGHT:
			goto JOY_UPRIGHT;
			break;
		case ANG_JUP_LEFT:
			goto JOY_UPLEFT;
			break;
	};

	next_anirate();

	check_winner_status();

	if (reduce_turbo_bar()==SYSTEM_ZERO_SET)
		goto JOYR7;


	(long)current_proc->a5=-SCX(0x80000);
	if (JOYSTICK_IN_A0(current_proc) & MASK_JRIGHT)		// right
		goto JRUN5;

	if (JOYSTICK_IN_A0(current_proc) & MASK_JLEFT) 		// left
	{
		(long)current_proc->a5=-(long)current_proc->a5;
		goto JRUN5;
	}
	else goto JOYR7;					// neither = abort

JRUN5:
	if (obj->oxvel.pos==(long)current_proc->a5)			// we are heading in wrong direction, exit
		goto JOYR7;

	if (MASK_JOYSTICK(current_proc))
		goto JRUN3;
	else goto JOYR7;

/************************************
 * UP JOYSTICK						*
 ************************************/
JOY_UP:
	face_opponent(obj);

	disable_all_buttons;

	/* check for jump angles */
	switch (angle_scan())
	{
		case ANG_JUP_RIGHT:
			goto JOY_UPRIGHT;
			break;
		case ANG_JUP_LEFT:
			goto JOY_UPLEFT;
			break;
	};

	stuff_buttons(obj,bt_jump);

	current_proc->a10=(long)distance_from_ground(obj);

	do_jump_up(NULL);

	reaction_exit(obj);

/************************************
 * DUCK DOWN JOYSTICK				*
 ************************************/
JOY_DUCK_ENTRY:
	get_char_ani(ANIM_TABLE1,ANIM_DUCK);

	/* set to duck frame */
	(LONG *)current_proc->pa9=((LONG *)current_proc->pa9)+2;

	do_next_a9_frame(obj);
	goto JOYD3;

/************************************
 * DOWN JOYSTICK					*
 ************************************/
JOY_DOWN:
	disable_all_buttons;

	do_duck();

/************************************
 * GETUPJOYSTICK					*
 ************************************/
JOY_GETUP_ENTRY:
JOYD3:
	stuff_buttons(obj,bt_duck);

JOYD4:
	current_proc->pdata.p_action=ACT_DUCK;			// reset action !!

	process_sleep(1);

	/* facing him check */
	if (am_i_facing_him()==SYSTEM_CARRY_CLR)
	{
		duck_turnaround();					// no, so turnaround
		goto JOYD3;
	}
JOYD5:
  	inc_downcount(current_proc);					// count duck time

	if (check_block_bit()==SYSTEM_CARRY_SET)		// block?
		goto JOY_DUCK_BLOCK;

	check_winner_status();						// scan for "end of round" while ducking

	if (JOYSTICK_IN_A0(current_proc) & MASK_JDOWN)			// stay down while stick==DOWN
		goto JOYD4;

JOY_BACK_UP:
	disable_all_buttons;
	face_opponent(obj);
	do_backup();
	goto LOCAL_REACTION_EXIT;					// return to normal

/************************************
 * DUCK BLOCK JOYSTICK				*
 ************************************/
JOY_DUCK_BLOCK:
	disable_all_buttons;

JDBLK2:
	do_duck_block();

JOY_DUCK_BLOCK_LOOP:
	process_sleep(1);

	if (am_i_facing_him()==SYSTEM_CARRY_CLR)
	{
		duck_turnaround();
		goto JDBLK2;
	}

JDBLK5:
	if ((JOYSTICK_IN_A0(current_proc) & MASK_JDOWN)==0)			// stay down while stick==DOWN
		goto JOY_BACK_UP;

  	inc_downcount(current_proc);					// count duck time

	check_winner_status();							// scan for "end of round" while ducking

	current_proc->pdata.p_action=ACT_BLOCKLO;		// reset action

	if (check_block_bit()==SYSTEM_CARRY_SET)		// still holding block?
		goto JOY_DUCK_BLOCK_LOOP;					// yes, loop

	/* joy un - duck block */
	current_proc->pdata.p_action=ACT_DUCK;
	backwards_ani(3,ANIM_DUCKBLOCK);

	find_ani_last_frame(ANIM_DUCK);
	do_next_a9_frame(obj);

	goto JOYD3;

/************************************
 * UP LEFT JOYSTICK					*
 ************************************/
JOY_UPLEFT:
	angle_abuse_check();

	do_flip(ANIM_FFLIP,ANIM_BFLIP,-SCX(0x40000),-SCX(0x70000));
	goto LOCAL_REACTION_EXIT;

/************************************
 * UP RIGHT JOYSTICK				*
 ************************************/
JOY_UPRIGHT:
	angle_abuse_check();

	do_flip(ANIM_BFLIP,ANIM_FFLIP,SCX(0x40000),SCX(0x70000));
	goto LOCAL_REACTION_EXIT;

/************************************
 * BLOCK JOYSTICK					*
 ************************************/
JOY_BLOCK:
	disable_all_buttons;
	face_opponent(obj);

	do_block_hi();

JOY_BLOCK_LOOP:
	process_sleep(1);

	if (JOYSTICK_IN_A0(current_proc) & MASK_JDOWN)
		goto JOY_DOWN;

	if (am_i_facing_him()==SYSTEM_CARRY_CLR)
	{
		turn_around();							// face your rival
		goto LOCAL_REACTION_EXIT;
	}

	if (check_block_bit()==SYSTEM_CARRY_SET)
		goto JOY_BLOCK_LOOP;

	do_unblock_hi();

	goto LOCAL_REACTION_EXIT;

/************************************
 * HI PUNCH JOYSTICK				*
 ************************************/
JOY_HI_PUNCH:
	disable_all_buttons;

	me_in_front();

	if (elbow_check()==SYSTEM_CARRY_SET)
		goto JOY_ELBOW;

	stop_me(obj);

	get_char_ani(ANIM_TABLE1,ANIM_HIPUNCH);

JHP4:
	(LONG)current_proc->pdata.p_store3=get_last_button();

	current_proc->pdata.p_stk=2;

	group_sound(0);					// group speech: quick attack
	rsnd_whoosh();

	/* act_mframew */
	act_mframew(ACT_HIPUNCH,3);

	if (f_thatsall!=0)
		goto JOY_UN_HI_PUNCH1;					// round is over --> exit

	/* strike collision detection */
	current_proc->a10=0;							// flag: hi punch
	joytemp=punch_strike_check(2);

JHP2:
	if ((joytemp=punch_sleep(5,joytemp))==SYSTEM_CARRY_CLR)
		goto JOY_UN_HI_PUNCH1;

	if ((((LONG)(current_proc->a0))>>16)==SW_LO_PUNCH)
	{
		/* JOY_PUNCH_HTM1 */							// interupted by med punch!!
		find_ani_part2(ANIM_HIPUNCH);					// pa9 --> part 2
		find_part2();									// pa9 --> part 3
		find_part2();									// pa9 --> part 3
		find_part2();									// pa9 --> part 3
		find_part2();									// pa9 --> part 3
		goto JMP5;
	}

	if ((((LONG)(current_proc->a0))>>16)!=SW_HI_PUNCH)
		goto JOY_UN_HI_PUNCH1;					// non-hi-punch ---> finish punch

JHP5:
	(LONG)current_proc->pdata.p_store3=get_last_button();

	group_sound(0);					// group speech: quick attack
	rsnd_whoosh();

	current_proc->pdata.p_stk=2;

	/* act_mframew */
	act_mframew(ACT_HIPUNCH,3);

	if (f_thatsall!=0)
		goto JOY_UN_HI_PUNCH2;				// round is over --> exit

	/* strike collision detection */
	if (strike_check_a0(obj,2)==SYSTEM_CARRY_CLR)	// strike: hipunch #1
		joytemp=2;
	else joytemp=-1;							// collision = dont check for more

JHP6:
	if ((joytemp=punch_sleep(5,joytemp))==SYSTEM_CARRY_CLR)
		goto JOY_UN_HI_PUNCH2;

	if ((((LONG)(current_proc->a0))>>16)==SW_LO_PUNCH)
	{
		/* JOY_PUNCH_HTM2 */					// interupted by med punch!!
		find_ani_part2(ANIM_HIPUNCH);					// pa9 --> part 2
		find_part2();									// pa9 --> part 3
		find_part2();									// pa9 --> part 3
		find_part2();									// pa9 --> part 3
		find_part2();									// pa9 --> part 3
		find_part2();									// pa9 --> part 3
		goto JMP4;
	}

	if ((((LONG)(current_proc->a0))>>16)==SW_HI_PUNCH)
		goto JHP4;								// non-hi-punch --> finish punch

JOY_UN_HI_PUNCH2:
	find_ani_part2(ANIM_HIPUNCH);					// pa9 --> part 2
	find_part2();									// pa9 --> part 3
	goto UNHIP1;

JOY_UN_HI_PUNCH1:
	find_ani_part2(ANIM_HIPUNCH);					// pa9 --> part 2

UNHIP1:
	find_part2();									// pa9 --> part 3
	find_part2();									// pa9 --> un_hipunch
	mframew(3-1);
	goto LOCAL_REACTION_EXIT;					// return to normal

/************************************
 * ELBOW ATTACK 					*
 ************************************/
JOY_ELBOW:
	do_elbow();
	goto LOCAL_REACTION_EXIT;


/************************************
 * JOY LOW PUNCH 					*
 ************************************/
JOY_LO_PUNCH:
	stop_me(obj);
	disable_all_buttons;

	if (toss_check(obj,SCX(0x040))==SYSTEM_CARRY_SET)
	{
		stack_jump(joy_toss);
	}

	me_in_front();

	get_char_ani(ANIM_TABLE1,ANIM_LOPUNCH);
JMP4:
	(LONG)current_proc->pdata.p_store3=get_last_button();

	current_proc->pdata.p_stk=3;

	group_sound(0);					// group speech: quick attack
	rsnd_whoosh();

	/* act_mframew */
	act_mframew(ACT_LOPUNCH,3);			// lo punch

	if (f_thatsall)
		goto JOY_UN_LO_PUNCH1;								// round is over --> exit

	current_proc->a10=1;									// flag: med punch

	if (strike_check_a0(obj,3)==SYSTEM_CARRY_CLR)			// strike: med punch strike #1
		joytemp=3;
	else joytemp=-1;										// collision = dont check for more

JMP2:
	if ((joytemp=punch_sleep(5,joytemp))==SYSTEM_CARRY_CLR)
		goto JOY_UN_LO_PUNCH1;

	if ((((LONG)(current_proc->a0))>>16)==SW_HI_PUNCH)
	{
		/* JOY_PUNCH_MTH1 */							// interupted by hi punch!!
		find_ani_part2(ANIM_LOPUNCH);					// pa9 --> part 2
		find_part2();									// pa9 --> part 3
		find_part2();									// pa9 --> part 3
		find_part2();									// pa9 --> part 3
		find_part2();									// pa9 --> part 3
		goto JHP5;
	}

	if ((((LONG)(current_proc->a0))>>16)!=SW_LO_PUNCH)
		goto JOY_UN_LO_PUNCH1;					// non-med-punch ---> finish punch


JMP5:
	(LONG)current_proc->pdata.p_store3=get_last_button();

	current_proc->pdata.p_stk=3;

	group_sound(0);					// group speech: quick attack
	rsnd_whoosh();

	/* act_mframew */
	act_mframew(ACT_LOPUNCH,3);

	if (f_thatsall!=0)
		goto JOY_UN_LO_PUNCH2;				// round is over --> exit

	/* strike collision detection */
	if (strike_check_a0(obj,2)==SYSTEM_CARRY_CLR)	// strike: med punch #2
		joytemp=2;
	else joytemp=-1;							// collision = dont check for more

JMP6:
	if ((joytemp=punch_sleep(5,joytemp))==SYSTEM_CARRY_CLR)
		goto JOY_UN_LO_PUNCH2;

	if ((((LONG)(current_proc->a0))>>16)==SW_HI_PUNCH)
	{
		/* JOY_PUNCH_MTH2 */					// interupted by hi punch!!
		find_ani_part2(ANIM_LOPUNCH);					// pa9 --> part 2
		find_part2();									// pa9 --> part 3
		find_part2();									// pa9 --> part 3
		find_part2();									// pa9 --> part 3
		find_part2();									// pa9 --> part 3
		find_part2();									// pa9 --> part 3
		goto JHP4;
	}

	if ((((LONG)(current_proc->a0))>>16)==SW_LO_PUNCH)
		goto JMP4;								// non-med-punch --> finish punch


JOY_UN_LO_PUNCH2:
	find_ani_part2(ANIM_LOPUNCH);					// pa9 --> part 2
	find_part2();									// pa9 --> part 3
	goto UNHIP1;

JOY_UN_LO_PUNCH1:
	find_ani_part2(ANIM_LOPUNCH);					// pa9 --> part 2
	goto UNHIP1;

/************************************
 * JOY UPPERCUT 					*
 ************************************/
JOY_UPPERCUT:
	disable_all_buttons;
	do_uppercut();
	goto LOCAL_REACTION_EXIT;


/************************************
 * JOY DUCK PUNCH 					*
 ************************************/
JOY_DUCK_PUNCH:
	disable_all_buttons;
	do_duck_punch();

	if (JOYSTICK_IN_A0(current_proc) & MASK_JDOWN)
		goto JOYD3;													// stay down while stick = down
	else goto JOY_BACK_UP;

/************************************
 * JOY HI KICK						*
 ************************************/
JOY_HI_KICK:
	disable_all_buttons;

	if (is_stick_away(current_proc)==SYSTEM_CARRY_SET)
	{
		/* JOY_ROUNDHOUSE */
		do_roundhouse();
	}
	else
	{
		if (knee_check()==SYSTEM_CARRY_SET)
			goto JOY_KNEE;

		do_hi_kick();
	}

	goto LOCAL_REACTION_EXIT;									// return to normal control

/************************************
 * JOY LO KICK 					*
 ************************************/
JOY_LO_KICK:
	disable_all_buttons;

	if (knee_check()==SYSTEM_CARRY_SET)
		goto JOY_KNEE;

	if (is_stick_away(current_proc)==SYSTEM_CARRY_SET)
		do_sweep_kick();
	else do_lo_kick();

	goto LOCAL_REACTION_EXIT;

/************************************
 * JOY KNEE 						*
 ************************************/
JOY_KNEE:
	do_knee();
	goto LOCAL_REACTION_EXIT;

/************************************
 * JOY DUCK KICKH					*
 ************************************/
JOY_DUCK_KICKH:
	do_duck_kickh();
	results=current_proc->a0;
	current_proc->pdata.p_action=ACT_RET_KICK;

	if (results==SYSTEM_CARRY_SET)
		process_sleep(16);					// connect sleep time
	else process_sleep(8);					// no connect sleep time

	retract_strike(4);

	current_proc->pdata.p_downcount+=0x26;						// count kick time as "down time"

	/* POST_JOY_DUCK_KICK */
	if (JOYSTICK_IN_A0(current_proc) & MASK_JDOWN)
		goto JOYD3;													// stay down while stick = down
	else goto JOY_BACK_UP;

/************************************
 * JOY DUCK KICKL					*
 ************************************/
JOY_DUCK_KICKL:
	do_duck_kickl();
	current_proc->pdata.p_action=ACT_RET_KICK;

	if (is_he_joy()==SYSTEM_CARRY_SET)
		process_sleep(6);
	else process_sleep(10);

	retract_strike(2);

	current_proc->pdata.p_downcount+=0x8;						// count kick time as "down time"

	/* POST_JOY_DUCK_KICK */
	if (JOYSTICK_IN_A0(current_proc) & MASK_JDOWN)
		goto JOYD3;													// stay down while stick = down
	else goto JOY_BACK_UP;

/************************************
 * JOY FLIP PUNCH 					*
 ************************************/
JOY_FLIP_PUNCH:
	disable_all_buttons;
	do_flip_punch();
	goto LOCAL_REACTION_EXIT;

/************************************
 * JOY FLIP KICK					*
 ************************************/
JOY_FLIP_KICK:
	disable_all_buttons;
	do_flip_kick();
	goto LOCAL_REACTION_EXIT;

/************************************
 * JOY JUMP PUNCH					*
 ************************************/
JUMPUP_PUNCH:
	disable_all_buttons;
	do_jumpup_punch;
	goto LOCAL_REACTION_EXIT;

/************************************
 * JOY JUMP KICK					*
 ************************************/
JUMPUP_KICK:
	disable_all_buttons;
	do_jumpup_kick;
	//goto LOCAL_REACTION_EXIT;

LOCAL_REACTION_EXIT:
	reaction_exit(obj);
}

/******************************************************************************
 Function: void angle_abuse_check(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:  set the angle abuse flag, if constantly happening
******************************************************************************/
void angle_abuse_check(void)
{
	if (get_tsl(l_angle,current_proc->pa8)<=0x40)	// check last one
		f_aabuse++;									// its been recent lets notify

	/* aac9 */
	update_tsl(l_angle,current_proc->pa8);

	return;
}

/******************************************************************************
 Function: WORD am_i_him(LONG char_check)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: SYSTEM_CARRY_SET - if I am char
		  SYSTEM_CARRY_CLR - if I am not char

 Description:	answers the question" am i the person (char_check)
******************************************************************************/
inline WORD am_i_him(LONG char_check)
{
	if (current_proc->pa8->ochar==char_check)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: WORD am_i_him(LONG char_check)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: SYSTEM_CARRY_SET - if I am char
		  SYSTEM_CARRY_CLR - if I am not char

 Description:	answers the question" am i the person (char_check)
******************************************************************************/
short punch_strike_check(WORD pa0)
{
	if (strike_check_a0(current_proc->pa8,pa0)==SYSTEM_CARRY_CLR)	// strike: hipunch #1
		return(pa0);
	else return(-1);							// collision = dont check for more
}

/******************************************************************************
 Function: void stuff_buttons(OBJECT *obj,WORD *vector)

 By: David Schwartz

 Date: Sept 1994

 Parameters: obj - player object
			 vector - new button vector

 Returns: None

 Description:	stuff a new button vector into p?_button
******************************************************************************/
inline void stuff_buttons(OBJECT *obj,WORD *vector)
{
	if (obj==p1_obj)
		p1_button=vector;
	else p2_button=vector;

	return;
}

/******************************************************************************
 Function: WORD check_block_bit(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	answers the question with a carry, is the fighter currently blocking
******************************************************************************/
inline WORD check_block_bit(void)
{
	if (JOYSTICK_IN_A0(current_proc) & MASK_JBLOCK)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: WORD check_block_bit(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	answers the question with a carry, is the other fighter currently blocking
******************************************************************************/
inline WORD check_his_block_bit(void)
{
	if (*((current_proc->pdata.p_otherproc)->pdata.p_joyport) & MASK_JBLOCK)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: void check_winner_status(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	players call this routine between every move to check the status of the game

	winner_status:
		0 - no outcome yet
		1 - player 1 wins
		2 - player 2 wins
		3 - finish him off mode
		4 - endurance transition
******************************************************************************/
void check_winner_status(void)
{
	switch (winner_status)
	{
		case 0:
			break;
		case 1:
			player_1_wins;
			break;
		case 2:
			player_2_wins;
			break;
		case 3:
			finish_him();
			break;
		case 4:
			endurance_trans();
			break;
		default:
			ermac(2);						// we screwed up bigtime !!
	}

	return;
}

/******************************************************************************
 Function: void stop_me(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	stop a player from moving in the x direction
******************************************************************************/
void stop_me(OBJECT *obj)
{
	stop_a8(obj);

	obj->ograv.pos=0;							/* stop gravity */

	set_x_vel(obj,0);
}

/******************************************************************************
 Function: void set_x_vel(OBJECT *obj,long vel);

 By: David Schwartz

 Date: Sept 1994

 Parameters: vel - set player velocity

 Returns: None

 Description:	set a players x velocity based on the current obj
******************************************************************************/
void set_x_vel(OBJECT *obj,long vel)
{

#if PAL_VERSION==5
	vel=(vel*6)/5;
#endif

	if (obj==p1_obj)
	{
		p1_xvel.pos=vel;
		return;
	}

	if (obj==p2_obj)
		p2_xvel.pos=vel;

		return;
}


/******************************************************************************
 Function: int angle_scan(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: ANG_NOSCAN - no scans recognized
		  ANG_JUP_RIGHT - scanned jump up and right
		  ANG_JUP_LEFT - scanned jump up and left

 Description:	determine if a player is trying to do an angle move
******************************************************************************/
int angle_scan(void)
{
	if ((JOYSTICK_IN_A0(current_proc) & (MASK_JRIGHT+MASK_JUP))==(MASK_JRIGHT+MASK_JUP))
		return(ANG_JUP_RIGHT);

	if ((JOYSTICK_IN_A0(current_proc) & (MASK_JLEFT+MASK_JUP))==(MASK_JLEFT+MASK_JUP))
		return(ANG_JUP_LEFT);

	return(ANG_NOSCAN);
}

/******************************************************************************
 Function: int look_for_angle(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: ANG_NOSCAN - no scans recognized
		  ANG_JUP_RIGHT - scanned jump up and right
		  ANG_JUP_LEFT - scanned jump up and left

 Description:	watches for joystick angle combos
******************************************************************************/
int look_for_angle(void)
{
	int loop=3;

	do
	{
	if ((JOYSTICK_IN_A0(current_proc) & (MASK_JRIGHT+MASK_JUP))==(MASK_JRIGHT+MASK_JUP))
		return(ANG_JUP_RIGHT);

	if ((JOYSTICK_IN_A0(current_proc) & (MASK_JLEFT+MASK_JUP))==(MASK_JLEFT+MASK_JUP))
		return(ANG_JUP_LEFT);

	process_sleep(1);							// give a change to go up - right/left

	}
	while(--loop>0);

	return(ANG_NOSCAN);
}

/******************************************************************************
 Function: void do_backup(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	joy/drone get backup routine
******************************************************************************/
void do_backup(void)
{
	current_proc->pdata.p_action=ACT_BACKUP;			// define action

	backwards_ani(2,ANIM_DUCK);			// get up animation routine

	return;
}

/******************************************************************************
 Function: void do_duck(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	joy/drone duck animation routine
******************************************************************************/
void do_duck(void)
{
	stop_me(current_proc->pa8);
	face_opponent(current_proc->pa8);
	get_char_ani(ANIM_TABLE1,ANIM_DUCK);

	/* act_mframew */
	act_mframew(ACT_DUCK,2);

	return;
}

/******************************************************************************
 Function: void do_duck_block(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	joy/drone duck block animation routine
******************************************************************************/
void do_duck_block(void)
{
	stop_me(current_proc->pa8);
	face_opponent(current_proc->pa8);
	get_char_ani(ANIM_TABLE1,ANIM_DUCKBLOCK);

	/* act_mframew */
	act_mframew(ACT_BLOCKLO,3);

	return;
}

/******************************************************************************
 Function: void do_flip(WORD ani_1,WORD ani_2,long wxvel,long rxvel)

 By: David Schwartz

 Date: Sept 1994

 Parameters: ani_1 - animation #1 (left flip) (a0)
			 ani_2 - animation #2 (right flip) (a1)
			 wxvel - init x walk velocity (a6)
			 rxvel - init x run velocity (a11)

 Returns: None

 Description:	joy/drone angle jump routine
******************************************************************************/
void do_flip(WORD ani_1,WORD ani_2,long wxvel,long rxvel)
{
	WORD ani_flip;
	OBJECT *obj=current_proc->pa8;

	/* determine run or walk jump velocity */
	current_proc->a5=(ADDRESS)current_proc->pa9;
	get_char_ani(ANIM_TABLE1,ANIM_RUN);
	current_proc->a7=(ADDRESS)current_proc->pa9;
	find_last_frame();

	if (( current_proc->a5 >= current_proc->a7) && (current_proc->a5 < (ADDRESS)current_proc->pa9) && (f_superjumps))
		wxvel=rxvel;

	/* JUR3 */
	group_sound(1);				// group speech: jump "huh"

	/* flip init */
	ani_flip=(obj->oflags & M_FLIPH) ? ani_1:ani_2;			// pick correct flip

JUR4:
	stop_me(obj);

	disable_all_buttons;

	stuff_buttons(current_proc->pa8,bt_angle_jump);				// now you can allow attacks

	current_proc->pdata.p_action=ACT_ANGLE_JUMP;	// define action

	(short)current_proc->pdata.p_store8=obj->oxpos.u.intpos;	// save starting x pos

	if (is_he_right(current_proc)==SYSTEM_CARRY_SET)
		wxvel=-wxvel;

	(ADDRESS *)current_proc->a6=(ADDRESS *)(angle_jump_call);
	current_proc->a11=4;							// flip whoosh sound speed
	flight_call(obj,wxvel,-SCY(0xa0000),SCY(0x8000),3,ani_flip);

	angle_jump_land_jsrp();

	reaction_exit(obj);
}

/******************************************************************************
 Function: void angle_jump_call(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: current_proc->a11 - sound whosh counter

 Returns: None

 Description:	angle jump code
******************************************************************************/
void angle_jump_call(void)
{
	FUNC ta6;

	if (am_i_joy()==SYSTEM_CARRY_CLR)
	{
		if ((ta6=(FUNC)current_proc->pdata.p_store1)!=NULL)
			ta6();
		return;
	}

	/* ajc3 */
	next_flip_whoosh();

	if (current_proc->pa8->oyvel.pos>0)				// vertical direction, upwards skip
	{
		/* if close to ground then no more attacking */
		if (abs(current_proc->pdata.p_ganiy-current_proc->pa8->oypos.u.intpos)<20)
		{
			disable_all_buttons;						// no more attacks
			(ADDRESS *)current_proc->pdata.p_store4=NULL;	// no need for this code any more
		}
	}

	return;
}

/******************************************************************************
 Function: void next_flip_whoosh(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: current_proc->a11 - whoosh counter

 Returns: None

 Description:	make whoosh sound, when timed
******************************************************************************/
inline void next_flip_whoosh(void)
{
	if (--current_proc->a11==0)
	{
		current_proc->a11=12;						// reset counter

		if ((current_proc->pa8)->oid==OID_P1)
			triple_sound(0x7);
		else triple_sound(0x6);
	}

	return;
}

/******************************************************************************
 Function: WORD am_i_airborn(OBJECT *obj)

 By: David Schwartz

 Date: Sept 1994

 Parameters: obj - current object

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	answers the questin am i airborn
******************************************************************************/
inline WORD am_i_airborn(OBJECT *obj)
{
	if (obj->oyvel.pos!=0 || obj->oypos.u.intpos!=current_proc->pdata.p_ganiy)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: WORD is_stick_toward(PROCESS *ptemp)

 By: David Schwartz

 Date: Sept 1994

 Parameters: ptemp = prcoess to check

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	answers the questin "is my joystick moving away from the other dude"
******************************************************************************/
inline WORD is_stick_toward(PROCESS *ptemp)
{
	return(isa5(ptemp,(is_he_right(ptemp)==SYSTEM_CARRY_SET) ? MASK_JRIGHT: MASK_JLEFT));
}

/******************************************************************************
 Function: WORD is_stick_away(PROCESS *ptemp)

 By: David Schwartz

 Date: Sept 1994

 Parameters: ptemp = prcoess to check

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	answers the questin "is my joystick moving away from the other dude"
******************************************************************************/
inline WORD is_stick_away(PROCESS *ptemp)
{
	return(isa5(ptemp,(is_he_right(ptemp)==SYSTEM_CARRY_SET) ? MASK_JLEFT: MASK_JRIGHT));
}

/******************************************************************************
 Function: WORD isa5(PROCESS *ptemp,WORD joymask)

 By: David Schwartz

 Date: Jan 1995

 Parameters: ptemp = prcoess to check
			 joymask = mask to check for

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	answers the questin "is my joystick moving away from the other dude, based on the mask", common routine
******************************************************************************/
inline WORD isa5(PROCESS *ptemp,WORD joymask)
{
	if (joymask & JOYSTICK_IN_A0(ptemp))
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: void me_in_front(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters: None

 Returns: None

 Description:	puts my object in front of the other guys
******************************************************************************/
inline void me_in_front(void)
{
	current_proc->pdata.p_otherguy->ozval=BACK_Z;
	current_proc->pa8->ozval=FRONT_Z;
	return;
}

/******************************************************************************
 Function: void me_in_back(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters: None

 Returns: None

 Description:	puts my object in back of the other guys
******************************************************************************/
inline void me_in_back(void)
{
	current_proc->pdata.p_otherguy->ozval=FRONT_Z;
	current_proc->pa8->ozval=BACK_Z;
	return;
}

/******************************************************************************
 Function: void punch_sleep(WORD sleeptime,long strkoff)

 By: David Schwartz

 Date: Oct 1994

 Parameters: sleeptime - total sleep time (assuming no buttons pressed)
			 strkoff - strike_check offset
			 current_proc->p_store3 - button/time that started it all

 Returns: SYSTEM_CARRY_SET - intrupted by additional button press
		  current_proc->a0 - last button/time pressed

 Description:	sleep used after a punch has been made (and hit)
******************************************************************************/
WORD punch_sleep(WORD sleeptime,long strkoff)
{
	do
	{
		process_sleep(1);

		if (strkoff>=0)							// neg = dont check for collision
			strkoff=punch_strike_check(strkoff);

		/* check old state with current state, if different leave right now */

		if (((LONG)current_proc->a0=get_last_button()) != ((LONG)current_proc->pdata.p_store3))
			return(am_i_facing_him());			// allow repeats only if im facing him
	}
	while(--sleeptime>0);

	return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: LONG get_last_button(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters: None

 Returns: last button/time

 Description:	returns last button/time combo
******************************************************************************/
LONG get_last_button(void)
{
	LONG *queue;
	LONG queindex;

	queindex=get_bcq_next_pointer(&queue);
	return(previous_q_entry(&queindex,queue));
}

/******************************************************************************
 Function: WORD strike_check_box(OBJECT *obj,WORD pa0,short pa1x,short pa1y,short pa2x,short pa2y)

 By: David Schwartz

 Date: Feb 1995

 Parameters: obj - object
			 pa0 - strike table offset
			 pa1x- x offset
			 pa1y- y offset
			 pa2x- x size
			 pa2t- y size

 Returns: SYSTEM_CARRY_SET - collision has occured
		  SYSTEM_CARRY_CLR - collision has not occured

		  f_block = set if attack was blocked (b2)

 Description:	looks for collisions on "strike" routines
******************************************************************************/
WORD strike_check_box(OBJECT *obj,WORD pa0,short pa1x,short pa1y,short pa2x,short pa2y)
{
	short damage;
	short temp,bar_reduce,duck_flag;
	STRKTBL *strkinfo;

	XYTYPE tl_attack,lr_attack;					// attackers box pos
	XYTYPE tl_defend,lr_defend;					// defenders box pos


	strkinfo=get_char_stk(obj,pa0);

	f_block=0;									// set block flag

	if ((current_proc->pdata.p_otherguy)->oflags2 & M_NOCOL)
		return(SYSTEM_CARRY_CLR);				// he is invincible --> no collision allowed

	/* setup attack collision box */
	tl_attack.u.ypos=obj->oypos.u.intpos+pa1y;		// top left ypos

	if (obj->oflags & M_FLIPH)
		tl_attack.u.xpos=obj->oxpos.u.intpos-pa1x;	// flipped then strike box is xpos-xoff
	else tl_attack.u.xpos=obj->oxpos.u.intpos+(pa1x-pa2x);	// no flip strike box is xpos+(xoff-xsize)

	lr_attack.u.xpos=tl_attack.u.xpos+pa2x;
	lr_attack.u.ypos=tl_attack.u.ypos+pa2y;


#if COLLISION_BOX
	attack_box(&tl_attack,&lr_attack);
#endif

	/* setup defend collision box */
	tl_defend.u.ypos=highest_mpart(current_proc->pdata.p_otherguy);
	lr_defend.u.ypos=lowest_mpart(current_proc->pdata.p_otherguy);
	tl_defend.u.xpos=leftmost_mpart(current_proc->pdata.p_otherguy);
	lr_defend.u.xpos=rightmost_mpart(current_proc->pdata.p_otherguy);

	temp=abs(lr_defend.u.xpos-tl_defend.u.xpos);

	cbox_squeeze(&tl_defend,&lr_defend,temp,strkinfo->sflags);

#if COLLISION_BOX
	victim_box(&tl_defend,&lr_defend);
#endif

	/* perform collision detection */
	if (tl_attack.u.xpos>lr_defend.u.xpos ||
		tl_attack.u.ypos>lr_defend.u.ypos ||
		lr_attack.u.xpos<tl_defend.u.xpos ||
		lr_attack.u.ypos<tl_defend.u.ypos)
		return(SYSTEM_CARRY_CLR);						// no collision
	else
	{
		/* collision occured */
		disable_his_buttons;

		(current_proc->pdata.p_otherproc)->pdata.p_hitby=current_proc->pdata.p_action;		// update: last hit by!

		/* set whoose strength bar to reduce */
		bar_reduce=(p1_obj==current_proc->pdata.p_otherguy) ? PLAYER1_TYPE:PLAYER2_TYPE;

		if (is_he_blocking(&duck_flag,strkinfo->sflags)==SYSTEM_CARRY_SET)
		{
			/* strike blocked routine */
			damage=strkinfo->sdamage & 0x00ff;			// use "blocked damage" half
			f_block=1;									// flag: strike was blocked!!

			/* if no strength left, then block was useless and fall to that code */
			if ((short)get_my_strength(current_proc->pdata.p_otherguy)-damage>0)
			{
				/* setup new xfer point */
				fastxfer(current_proc->pdata.p_otherproc,*(block_xfers+(strkinfo->sroutine & 0x00ff)));

				/* block damage */
				bar_reducer(bar_reduce,&damage);
				add_combo_damage(damage);

				return(SYSTEM_CARRY_SET);
			}
		}

		/* block_is_useless routine */
		damage=(strkinfo->sdamage & 0xff00)>>8;			// use "hit damage" half
		bar_reducer(bar_reduce,&damage);
		add_combo_damage(damage);

		react_xfer_him(*(reaction_tables[((strkinfo->sroutine & 0xff00)>>8)]));

		return(SYSTEM_CARRY_SET);
	}
}

/******************************************************************************
 Function: WORD strike_check(STRKTBL *strkinfo,WORD testflag)

 By: David Schwartz

 Date: Oct 1994

 Parameters: strkinfo - info that defines a strike box for a particular attack
			 testflag - set if this check is a test (b1)

 Returns: SYSTEM_CARRY_SET - collision has occured
		  SYSTEM_CARRY_CLR - collision has not occured

		  f_block = set if attack was blocked (b2)

 Description:	looks for collisions on "strike" routines
******************************************************************************/
WORD strike_check(STRKTBL *strkinfo,WORD testflag)
{
	short damage;
	short temp,bar_reduce,duck_flag;
	OBJECT *obj=current_proc->pa8;

	XYTYPE tl_attack,lr_attack;					// attackers box pos
	XYTYPE tl_defend,lr_defend;					// defenders box pos

	f_block=0;									// set block flag

	if ((current_proc->pdata.p_otherguy)->oflags2 & M_NOCOL)
		return(SYSTEM_CARRY_CLR);				// he is invincible --> no collision allowed

	/* setup attack collision box */
	tl_attack.u.ypos=obj->oypos.u.intpos+strkinfo->syoff*SCF_Y/100;		// top left ypos

	if (obj->oflags & M_FLIPH)
		tl_attack.u.xpos=obj->oxpos.u.intpos-strkinfo->sxoff*SCF_X/100;	// flipped then strike box is xpos-xoff
	else tl_attack.u.xpos=obj->oxpos.u.intpos+(strkinfo->sxoff-strkinfo->sxsize)*SCF_X/100;	// no flip strike box is xpos+(xoff-xsize)

	lr_attack.u.xpos=tl_attack.u.xpos+strkinfo->sxsize*SCF_X/100;
	lr_attack.u.ypos=tl_attack.u.ypos+strkinfo->sysize*SCF_Y/100;

#if COLLISION_BOX
	if (testflag==0)
		attack_box(&tl_attack,&lr_attack);
#endif

	/* setup defend collision box */
	tl_defend.u.ypos=highest_mpart(current_proc->pdata.p_otherguy);
	lr_defend.u.ypos=lowest_mpart(current_proc->pdata.p_otherguy);
	tl_defend.u.xpos=leftmost_mpart(current_proc->pdata.p_otherguy);
	lr_defend.u.xpos=rightmost_mpart(current_proc->pdata.p_otherguy);

	temp=abs(lr_defend.u.xpos-tl_defend.u.xpos);

	cbox_squeeze(&tl_defend,&lr_defend,temp,strkinfo->sflags);

#if COLLISION_BOX
	victim_box(&tl_defend,&lr_defend);
#endif

	/* perform collision detection */
	if (tl_attack.u.xpos>lr_defend.u.xpos ||
		tl_attack.u.ypos>lr_defend.u.ypos ||
		lr_attack.u.xpos<tl_defend.u.xpos ||
		lr_attack.u.ypos<tl_defend.u.ypos)
		return(SYSTEM_CARRY_CLR);						// no collision
	else
	{
		/* collision occured */
		if (testflag==1)					// is this a test??
			return(SYSTEM_CARRY_SET);		// signify hit

		disable_his_buttons;

		(current_proc->pdata.p_otherproc)->pdata.p_hitby=current_proc->pdata.p_action;		// update: last hit by!

		/* set whoose strength bar to reduce */
		bar_reduce=(p1_obj==current_proc->pdata.p_otherguy) ? PLAYER1_TYPE:PLAYER2_TYPE;

		if (is_he_blocking(&duck_flag,strkinfo->sflags)==SYSTEM_CARRY_SET)
		{
			/* strike blocked routine */
			damage=strkinfo->sdamage & 0x00ff;			// use "blocked damage" half
			f_block=1;									// flag: strike was blocked!!

			/* if no strength left, then block was useless and fall to that code */
			if ((short)get_my_strength(current_proc->pdata.p_otherguy)-damage>0)
			{
				/* setup new xfer point */
				fastxfer(current_proc->pdata.p_otherproc,*(block_xfers+(strkinfo->sroutine & 0x00ff)));

				/* block damage */
				bar_reducer(bar_reduce,&damage);
				add_combo_damage(damage);

				return(SYSTEM_CARRY_SET);
			}
		}

		/* block_is_useless routine */
		damage=(strkinfo->sdamage & 0xff00)>>8;			// use "hit damage" half
		bar_reducer(bar_reduce,&damage);
		add_combo_damage(damage);

		react_xfer_him(*(reaction_tables[((strkinfo->sroutine & 0xff00)>>8)]));

		return(SYSTEM_CARRY_SET);
	}
}

/******************************************************************************
 Function: void react_xfer_him(JUMPTBL jumpptr)

 By: David Schwartz

 Date: Oct 1994

 Parameters: jumpptr - ptr to function to jump to

 Returns: None

 Description:	transfer reacting player to correct routine and cleanup switch processes
******************************************************************************/
void react_xfer_him(JUMPTBL jumpptr)
{
	PROCESS *pxfer;

	pxfer=current_proc->pdata.p_otherproc;

	if (pxfer->procid==PID_P1)
		dallprc(PID_SWITCH1);
	else dallprc(PID_SWITCH2);

	fastxfer(pxfer,jumpptr);					// xfer other guy

	pxfer->pdata.p_flags |=PM_REACTING;			// otherguy - flag: reacting !!

	return;
}

/******************************************************************************
 Function: WORD bar_reducer(WORD ptype,WORD *damage)

 By: David Schwartz

 Date: Oct 1994

 Parameters: ptype - player 1(0) or player 2(1)
			 damage - ptr to amount to sub from p?_bar

 Returns: SYSTEM_CARRY_SET - round is over
		  SYSTEM_CARRY_CLR - continue fighting

 Description:	add to players drop ram while is subtracted from his power
******************************************************************************/
WORD bar_reducer(WORD ptype,WORD *damage)
{
	WORD *pbar;
	OBJECT *ta1;
	WORD *f_damage;


	if (p1_bar==0 || p2_bar==0)
		return(SYSTEM_CARRY_SET);			// player dead --> round over

	if (ptype==PLAYER1_TYPE)				// damage to correct player
	{
		ta1=p1_obj;
		ochar_xlate_damage(p1_char,damage);
		pbar=&p1_bar;						// set to correct bar
		f_damage=&f_p2_half_damage;
	}
	else
	{
		ta1=p2_obj;
		ochar_xlate_damage(p2_char,damage);
		pbar=&p2_bar;						// set to correct bar
		f_damage=&f_p2_half_damage;
	}


	if (ta1->oflags2 & M_QUARTER_DAMAGE)
		(*damage)>>=2;
	else
	{
		if (ta1->oflags2 & M_HALF_DAMAGE)
			(*damage)>>=2;
	}
		
	if (*f_damage)		// special code stuff
		(*damage)>>=1;

	if (*damage==0)
		*damage=1;				// min is 1pixel damage

	(*pbar)-=*damage;						// inflict pain (damage)

	if ((short)*pbar<0)						// values must be >=0
	{
		*pbar=0;
		return(SYSTEM_CARRY_SET);			// player is dead, round is over
	}

	/* not_dead_yet */
	if ((*pbar<=6 && *pbar!=0) && (f_warnsound & (ptype+1))==0) 							// check if player is in danger and hasnt been alerted
	{
		f_warnsound |=(ptype+1);
		create_fx(FX_DANGER)->a11=(ptype+1);
	}
	return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: void ochar_xlate_damage(WORD pchar,WORD *damage)

 By: David Schwartz

 Date: Oct 1994

 Parameters: pchar - player character
			 damage - amount to sub from p?_bar

 Returns: SYSTEM_CARRY_SET - round is over
		  SYSTEM_CARRY_CLR - continue fighting

 Description:	add to players drop ram while is subtracted from his power
******************************************************************************/
void ochar_xlate_damage(WORD pchar,WORD *damage)
{

	WORD ta2=*damage;

	if (pchar==FT_SK || pchar==FT_MOTARO || pchar==FT_NOOB)
		ta2=(ta2>>1)+(ta2>>2);

	*damage=ta2;

	return;
}

/******************************************************************************
 Function: void cbox_squeeze(XYTYPE *tl,XYTYPE *lr,short pa4,WORD pa6)

 By: David Schwartz

 Date: Mar 1995

 Parameters: tl - top left of box
			 lr - low right of box
			 pa4 - x size of fighter
			 pa6 - flags

 Returns: None

 Description:	squeeze the colliison box
******************************************************************************/
void cbox_squeeze(XYTYPE *tl,XYTYPE *lr,short pa4,WORD pa6)
{
	short temp;

	if (pa6 & SBM_SQUEEZE)
	{
		if ((current_proc->pdata.p_otherguy)->ochar==FT_MOTARO)
		{
			temp=abs(tl->u.ypos-lr->u.ypos)>>8;		// 1/8 height
			tl->u.ypos+=temp;
			lr->u.ypos-=temp;

			temp=((current_proc->pdata.p_otherguy)->oflags & M_FLIPH) ? -SCX(0x25):SCX(0x25);
			tl->u.xpos+=temp;
			lr->u.xpos+=temp;				// adjust for motaros wierdness
		}

		/* cbs6 */
		pa4=(pa4>>2)+(pa4>>3);			// compute 3/8 wideness

		if ( f_death!=FAT_PIT )
		{
			tl->u.xpos+=pa4;				// dudes left collision x
			lr->u.xpos-=pa4;				// dudes right collision box
		}
	}
}

/******************************************************************************
 Function: WORD elbow_check(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	answers the question: are the dudes real close together and if so
			    then do a elbow
******************************************************************************/
inline WORD elbow_check(void)
{
	if (is_he_airborn()==SYSTEM_CARRY_SET || get_x_dist>SCX(0x4a))
		return(SYSTEM_CARRY_CLR);
	else return(SYSTEM_CARRY_SET);
}

/******************************************************************************
 Function: WORD knee_check(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	answers the question: are the dudes real close together and if so
			    then do a knee
******************************************************************************/
inline WORD knee_check(void)
{
	if (is_he_airborn()==SYSTEM_CARRY_SET || get_x_dist>SCX(0x4a))
		return(SYSTEM_CARRY_CLR);
	else return(SYSTEM_CARRY_SET);
}

/******************************************************************************
 Function: WORD toss_check(OBJECT *obj,WORD toss_dist)

 By: David Schwartz

 Date: Nov 1994

 Parameters: toss_dist - x distance to qualify "close enough" to toss !!
			 obj - dude who wants to throw

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	answers the question: are the dudes close enough for me to toss the other
******************************************************************************/
WORD toss_check(OBJECT *obj,WORD toss_dist)
{
	int edgedist;

	OBJECT *obj_other=current_proc->pdata.p_otherguy;

	if (q_is_he_a_boss()==SYSTEM_CARRY_SET)
		return(SYSTEM_CARRY_CLR);					// boss cant be flipped

	if (f_sans_throws!=0)
		return(SYSTEM_CARRY_CLR);			// throws disabled!!

	if (obj_other->oflags2 & M_NOFLIP)
		return(SYSTEM_CARRY_CLR);					// check to see if allowed to be flipped?

	if (is_he_airborn()==SYSTEM_CARRY_SET)
		return(SYSTEM_CARRY_CLR);					// cant toss if hes airborn

	if (get_x_dist>toss_dist)
		return(SYSTEM_CARRY_CLR);					// dudes are to far apart

	if (am_i_facing_him()==SYSTEM_CARRY_CLR)
		return(SYSTEM_CARRY_CLR);					// hes on the wrong side of me!!

	/* we are close enough !! */

	if (get_his_action(current_proc) == ACT_LAND)
		return(SYSTEM_CARRY_CLR);					// cant toss when other dude is landing

	if (is_he_joy()==SYSTEM_CARRY_CLR)				// he is joy = ok to throw
		toss_drone_check(current_proc->pa8);

	/* no throwing when near a wall */
TOSS_OK:
	if (am_i_joy()==SYSTEM_CARRY_SET)
	{
		if (is_he_right(current_proc)==SYSTEM_CARRY_SET)				// get edge that is behind me
			edgedist=abs(obj->oxpos.u.intpos-left_edge);
		else edgedist=abs(obj->oxpos.u.intpos-right_edge-SCRRGT);

		if (edgedist<CLOSE_TO_EDGE+8)
			return(SYSTEM_CARRY_CLR);					// close to edge --> no throwing
	}

TCHK4:
	if (is_stick_away(current_proc->pdata.p_otherproc)==SYSTEM_CARRY_SET)
		return(SYSTEM_CARRY_CLR);
	else return(SYSTEM_CARRY_SET);						// joy is not away --> ok to flip
}

WORD rt_drone_reverse[] ={0,100,200,300,400,500,700,700,800,900};

/******************************************************************************
 Function: WORD toss_drone_check(OBJECT *obj)

 By: David Schwartz

 Date: Dec 1994

 Parameters: obj - dude who wants to throw

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	 check for drone throw
******************************************************************************/
void toss_drone_check(OBJECT *obj)
{
	if (winner_status==3)
		return;					// finish him = ok to throw

	if (randper(rt_drone_reverse[diff])==SYSTEM_CARRY_CLR)
		return;

	if (get_his_action(current_proc)==ACT_FROZEN)
		return;					// froze dude --> no counter throw

	react_xfer_him(drone_reverse_toss);

	process_sleep(4);			// let him slam me

	reaction_exit(obj);
}

/******************************************************************************
 Function: void drone_reverse_toss(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	 drone reverse toss
******************************************************************************/
void drone_reverse_toss(void)
{
	do_body_slam();
	reaction_exit(current_proc->pa8);
}

/**************************************************************************
 *											     *
 *                         STRIKE ROUTINES						     *
 *                         STRIKE ROUTINES						     *
 *                         STRIKE ROUTINES						     *
 *                         STRIKE ROUTINES						     *
 *                         STRIKE ROUTINES						     *
 *											     *
 **************************************************************************/

/******************************************************************************
 Function: WORD striker(WORD strkindex,WORD anispeed,WORD actnum,WORD anioff,WORD framecnt,WORD strkoff)

 By: David Schwartz

 Date: Oct 1994

 Parameters: strkindex - prologue routine to execute
 			 anispeed - animation speed rate (a0)
			 actnum - current action performing (a1)
			 anioff - animation offset (a9)
			 framecnt - # of frames to look for collision (a10)
			 strkoff - strike table offset (a11)

 Returns: SYSTEM_CARRY_SET - hit
		  SYSTEM_CARRY_CLR - miss

 Description:	general purpose attack routine
******************************************************************************/
WORD striker(WORD strkindex,WORD anispeed,WORD actnum,WORD anioff,WORD framecnt,WORD strkoff)
{
	OBJECT *obj=current_proc->pa8;


	switch (strkindex)
	{
		case SI_STRIKER_NORMAL:
			init_special(obj);
			if ((short)anioff>=0)
				get_char_ani(ANIM_TABLE1,anioff);			// setup animation
			break;

		case SI_STRIKER_NORMAL2:
			init_special(obj);
			get_char_ani(ANIM_TABLE2,anioff);			// setup animation
			goto ATTK2;

		case SI_STRIKER_BEHIND:
			init_special(obj);							// setup stuff
			me_in_back();
			if ((short)anioff>=0)
				get_char_ani(ANIM_TABLE1,anioff);			// setup animation
			break;

		case SI_STRIKER_UPPER:
			init_special(obj);							// setup stuff
			break;
	}

ATTK5:
#if MK_EJBBUG
	kilobj2(OID_CBOX,0xffff);
#endif /* MK_EJBBUG */

ATTK2:
	/* act_mframew */
	act_mframew(actnum,anispeed);								// setup attack

	/* check for collision */
	do
	{
		if (strike_check_a0(obj,strkoff) == SYSTEM_CARRY_SET )	// look for strike
			return(SYSTEM_CARRY_SET);							// hit -->yes
		else process_sleep(1);
	}
	while(--framecnt>0);

	return(SYSTEM_CARRY_CLR);									// carry clear --> no hit
}

/******************************************************************************
 Function: WORD is_run_pressed(OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - dude to check

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	answers the question: is the current fighters run button pressed
******************************************************************************/
WORD is_run_pressed(OBJECT *obj)
{
	WORD *joyport;
	WORD *pturbo;

	if (current_proc->procid==PID_P1)
		joyport=&swcurr.u.p1pad;
	else joyport=&swcurr.u.p2pad;

	if (((*joyport) & MASK_JRUN)==0)
		return(SYSTEM_CARRY_CLR);

	/* run_yes */
	pturbo=(p1_obj==obj)? &p1_turbo:&p2_turbo;

	if (*pturbo==0)
	{
		/* no_turbo_left */
		pturbo=(p1_obj==obj)? &p1_dont_raise:&p2_dont_raise;
		*pturbo=TURBO_RAISE_DELAY;
		return(SYSTEM_CARRY_CLR);
	}
	else return(SYSTEM_CARRY_SET);
}

/******************************************************************************
 Function: void run_setup(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	setup dude to run
******************************************************************************/
void run_setup(void)
{
static BYTE run_speeds[] = {
	3,   					// 0 - kano
	3,						// 1 - sonya
	6,						// 2 - jax
	3,						// 3 - indian
	3,						// 4 - turk (subzero)
	3,						// 5 - swat
	6,						// 6 - lia
	3,						// 7 - robot ninja
	6,						// 8 - robot ninja
	3,						// 9 - kung lao
	6,						// 10 - tusk
	6,						// 11 - she goro
	4,						// 12 - shang tsu
	4,						// 13 - liu kang
	6,						// 14 - smoke
	3,						// 15 - kitana
	3,						// 16 - jade
	3,						// 17 - mileena
	3,						// 18 - scorpion
	3,						// 19 - reptile
	3,						// 20 - ermac
	3,						// 21 - oldsz
	3,						// 22 - oldsmoke
	3,						// 23 - noob saibot
	6,						// 24 - motaro
	3,						// 25 - shao kahn
};
	get_char_ani(ANIM_TABLE1,ANIM_RUN);
	init_anirate(run_speeds[(current_proc->pa8)->ochar]);
	towards_x_vel(current_proc,current_proc->pa8,SCX(0x80000));
	kill_group_sound(7,current_proc->pa8);		// prevent multi run sounds
	group_sound(7);
	return;
}

/******************************************************************************
 Function: void zero_turbo_bar(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	zero the turbo bar
******************************************************************************/
void zero_turbo_bar(void)
{
	if (f_unlim_run)
		return;						// unlimited run!!

	turbo_bar_setup();
	*((WORD *)current_proc->a6)=TURBO_RAISE_DELAY;		// dont raise this bar for a few ticks
	*((WORD *)current_proc->a5)=0;			// stuff updated value
	return;
}

/******************************************************************************
 Function: WORD reduce_turbo_bar(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_ZERO_SET - no turbo
		  SYSTEM_ZERO_CLR - turbo

 Description:	reduce the turbo bar
******************************************************************************/
WORD reduce_turbo_bar(void)
{

	if (f_unlim_run)
		return(SYSTEM_ZERO_CLR);						// unlimited run!!

	turbo_bar_setup();

	/* rtb5 */
	*((WORD *)current_proc->a6)=TURBO_RAISE_DELAY;

	if (*((WORD *)current_proc->a5)==0)
		return(SYSTEM_ZERO_SET);

	(*((WORD *)current_proc->a5))--;
	return(SYSTEM_ZERO_CLR);
}

/******************************************************************************
 Function: void turbo_bar_setup(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:  set ptrs to correct player ram for turbo bars
******************************************************************************/
void turbo_bar_setup(void)
{
	if (p1_obj==current_proc->pa8)
	{
		(WORD *)current_proc->a5=&p1_turbo;
		(WORD *)current_proc->a6=&p1_dont_raise;
	}
	else
	{
		(WORD *)current_proc->a5=&p2_turbo;
		(WORD *)current_proc->a6=&p2_dont_raise;
	}
}

/******************************************************************************
 Function: void ochar_begin_calls(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:  specific char init calls
******************************************************************************/
void ochar_begin_calls(void)
{
	switch ((current_proc->pa8)->ochar)
	{
		case FT_SMOKE:
			create_fx(FX_SMOKE);			// start the smoke screen
			if (am_i_joy()==SYSTEM_CARRY_SET)
				CREATE(PID_OLDSMOKE,transform_smoke);
			break;
		case FT_OLDSMOKE:
			create_fx(FX_SMOKE);			// start the smoke screen
			break;
		case FT_ST:
			current_proc->pdata.p_flags|=PM_SHANG;
			break;
	}

	return;
}

void transform_smoke(void) 
{
	WORD t_a10;
	WORD t_a9;
	WORD *swtch;

	if (current_proc->pa8==p1_obj) 
	{
	  t_a10=P1B0;			//-DHS- "GET REAL CODES FROM CHICAGO "
	  t_a9=M_P1ACTION;

	  swtch=&swcurr.u.p1pad;
	} 
	else
	{
	  t_a10=P2B0;			//-DHS- "GET REAL CODES FROM CHICAGO "
	  t_a9=M_P2ACTION;
	  swtch=&swcurr.u.p2pad;
	}

	/* trans3 */
	current_proc->a11=0x20;

	/* trans4 */
	do
	{
		process_sleep(1);

		if (((*swtch) & t_a9)!=t_a10)
			process_suicide();
	}
	while(--current_proc->a11>0);

	if (f_start!=0)
		process_suicide();			// only if no fight

	if (current_proc->pa8==p1_obj)
		fastxfer(p1_proc,r_transform_oldsmoke);
	else fastxfer(p2_proc,r_transform_oldsmoke);

	process_suicide();
}

void r_transform_oldsmoke(void) 
{
	create_fx(FX_INVISO_POOF);
	tsound(0x81);
	process_sleep(0x10);

	if (current_proc->pa8==p1_obj) 
	{
		p1_obj->ochar=FT_OLDSMOKE;
		p1_char=FT_OLDSMOKE;
	}
	else
	{
		p2_obj->ochar=FT_OLDSMOKE;
		p2_char=FT_OLDSMOKE;
	}

	newchar_load(FT_OLDSMOKE);

	player_froze_pal();
	player_normpal();

	ground_ochar();
	current_proc->pdata.p_ganiy=(current_proc->pa8)->oypos.u.intpos;
	pose_a9(0,ANIM_STANCE);
	process_sleep(8);

	animate_a9(5,ANIM_VICTORY);

	process_sleep(0x20);

	current_proc->joyindex=JI_JOY_IGNORE;
	stack_jump(joy_proc);
}

void newchar_load(WORD pa9) 
{
	OBJECT *obj=current_proc->pa8;

	psxcd_pause();

	character_texture_load(pa9,CHAR_NORMAL,obj->oheap,SYNC_LOAD);
	if (obj==p1_obj)							// set heap to belong to correct owner
		p1_heap_char=pa9;
	else p2_heap_char=pa9;

	if (obj->oid==OID_P1  )
		PsxSoundLoadFighter1(pa9);
	else PsxSoundLoadFighter2(pa9);

	psxcd_restart(cd_vol_tbl[f_music]);

	return;
}