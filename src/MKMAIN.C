/******************************************************************************
 File: mkmain.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Main Game Flow Routines
******************************************************************************/

/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkobj.h"
#include "psxcd.h"
#include "mkos.h"
#include "mkgame.h"
#include "mkinit.h"
#include "mkguys.h"
#include "mkr1.h"
#include "mkrepell.h"
#include "mkmain.h"
#include "mkutil.h"
#include "mkani.h"
#include "moves.h"
#include "mkmain.h"
#include "mksel.h"
#include "mkjoy.h"
#include "mkopt.h"
#include "mkdiag.h"
#include "mkamode.h"
#include "mkladder.h"
#include "mkscore.h"
#include "mktext.h"
#include "mkvs.h"
#include "mkfx.h"
#include "mkpal.h"
#include "mkfade.h"
#include "mkbonus.h"
#include "mkreact.h"
#include "mksound.h"
#include "mkend.h"
#include "mkmode.h"
#include "mkhstd.h"
#include "mkfile.h"
#include "psxspu.h"
#include "psxsnd.h"
#include "mkbio.h"
#include "mkbuyin.h"
#include "mksound.h"

/* local extern info */
extern FNTSETUP pf_15point_soff[];
extern FNTSETUP pf_entered[];
extern FNTSETUP pf_round_num[];
extern void *fl_entered[];
extern void *fl_finish[];
extern WORD LOADING_P[];

extern short *endurance_tables[];
void end_of_tournament(void);

void force_it(void)
{
	if (gstate==GS_AMODE)
		return;

	p1_char=FT_ST;
	p2_char=FT_SCORPION;
	curback=BKGD_WATER_MOD;
	return;
}

HSCORE factory_initials[]=
{
	{"MK4",10}, 			// 1
	{"WWF",9}, 			// 2
	{"OPN",9},  			// 3
	{"ICE",7},  			// 4
	{"MK3",6}, 			// 8
	{"MK2",6},  			// 5
	{"MK1",5},  			// 6
	{"CAR",5},  			// 7
	{"ROD",4}, 			// 9
	{"OFF",3}, 			// 10
	{"WIT",5}, 			// 11
	{"HOU",4}, 			// 12
	{"TTT",4}, 			// 13
	{"RYU",4}, 			// 14
	{"TWG",3} 			// 15
};

/******************************************************************************
 Function: void start_entry(WORD *pstate,WORD poffset)

 By: David Schwartz

 Date: Sept 1994

 Parameters: pstate - ptr to player state of whom start was pushed (a2)
			 poffset - (0=p1 1=p2) (a4)

 Returns: None

 Description:	Based on game start and/or player state react to the start button
				being pushed.
				1) start a new game
				2) barge into an existing game
				3) do nothing
				4) auto select player
******************************************************************************/
void start_entry(WORD *pstate,WORD poffset)
{
	f_start_pressed=1;					/* flag: start button pressed !! */

	/* BASED ON GAME STATE EXECUTE CORRECT ACTION */
	switch (gstate)
	{
		case 0x00:							// 0 - null
		case 0x08:							// 8 - diags
		case 0x09:							// 9 - chop sequence
		case 0x0a:							// a - enter initials
		case 0x0c:							// c - octopus ??
		case 0x0d:							// d - post player select fade
		case 0x0e:							// e -
		case 0x0f:							// f - intro effect
		case 0x11:							// 11 -
		case 0x12:							// 12 -
		case 0x13:							// 13 -
		case 0x14:							// 14 - mode select
		case 0x15:							// 15 - already in options death
			process_suicide();
			break;

		case 0x01: 	// start_during_amode	 1 - attract mode
		case 0x0b:	// start_during_gover	 b - game over start!!
			/* amode_game_start */
			/* Note: This is an addition to the arcade game.
				This is where we hooked in the option stuff
			*/
		    wess_seq_stop(TUNE_GAMEOVER);
			if (df_fastrun==0)
				opt_main();

			/* gover_game_start */
			murder_myoinit_score();
			DISPLAY_ON;

			game_init();

			/* MUST FALL INTO do_pstate_jump case */

		case 0x03: // start_during_buyin	 3 - buyin
		case 0x04: // start_during_psel		 4 - fighter select
			/* do_pstate_jump */
			switch (*pstate)
			{
				case 0x01:					// 1 - active
				case 0x04:					// 4 -
				case 0x05:					// 5 -
					process_suicide();
					break;

				case 0x03:	 // pstate_is_psel	3 - auto select check
					switch (mode_of_play)
					{
						case 0:		// random_sel_1on1
							if (&p1_state==pstate)
							{
								current_proc->a5=MASK_JUP;		// p1 up mask
								current_proc->a6=FT_KITANA;
								current_proc->a0=PID_P1SEL;
								current_proc->a14=0;			// no alt
							}
							else
							{
								current_proc->a5=MASK_JUP<<16;		// p2 up mask
								current_proc->a6=FT_SCORPION;
								current_proc->a0=PID_P2SEL;
								current_proc->a14=0;			// no alt
							}
							break;		// random_sel_1on1
						case 1:		// random_sel_2on2
							if (&p1_state==pstate)
							{
								current_proc->a5=MASK_JUP;		// p1 up mask
								current_proc->a6=FT_KITANA;
								current_proc->a0=PID_P1SEL;
								current_proc->a14=PID_P2SEL;			// no alt
							}
							else
							{
								current_proc->a5=MASK_JUP<<16;		// p2 up mask
								current_proc->a6=FT_SCORPION;
								current_proc->a0=PID_P3SEL;
								current_proc->a14=PID_P4SEL;			// no alt
							}
							break;		// random_sel_2on2
						default:
							process_suicide();
							break;
					}

					/* asc3 */
					if ((((PROCESS *)(current_proc->a0))=process_exist(current_proc->a0,-1))!=NULL)		// do i have a cursor process
						goto ASC4;

					if (current_proc->a14==0)
						process_suicide();

					if ((((PROCESS *)(current_proc->a0))=process_exist(current_proc->a14,-1))==NULL)		// do i have a cursor process
						process_suicide();

ASC4:
					/* asc4 */
					if (((PROCESS *)current_proc->a0)->p_comp_flag!=PC_CURSOR_LOOP)							// verify its where it should be
						process_suicide();

					current_proc->a1=((PROCESS *)current_proc->a0)->pdata.p_store6;				// p_mychar

					if (*((WORD *)(current_proc->a1))!=current_proc->a6)						// is it at the original spot
						process_suicide();

					if (swcurr.padvalue & current_proc->a5)
						fastxfer((PROCESS *)current_proc->a0,auto_player_select);				// start auto player select

					process_suicide();
					break;	// case 3, pstate_in_psel

				case 0x00:	// pstate_is_zero 	0 - not in game
					if (gstate==GS_PSEL)
						join_in_during_psel(pstate);
						goto_mode_select(pstate,poffset);
					break;

				case 0x02:	// pstate_is_buyin	2 - buyin
					switch (mode_of_play)
					{
						case 0:		// buyin_1_on_1	Standard MK3
							if (count_active_players()==0 && credits==0 && f_freeplay==0)
								process_suicide();					// one player game with no credits, its over with
						case 1:		// buyin_2_on_2	2 on 2
							*pstate=PS_BUYIN;
							goto_mode_select(pstate,poffset);
							break;
						case 2:		// tournament_play
							stack_jump(tournament_play);
							break;
					}
					break;
			}	/* pstate switch */
			break;			// start_during_buyin, start_during_psel		
							
		case 0x02:  // barge_game_start		2 - fighting
			if (*pstate==PS_ACTIVE) 	// player is active so he is allowed to pause game
			{
				/* pause game */
				if (f_pause || winner_status==3)
					process_suicide();		// dont allow pause to keep happening or during fatalities

				if ((clk_ones+clk_tens) == 0)
					process_suicide();		// no pause at end of round

				pause_setup(poffset);
				pause_hold(poffset);
				process_suicide();
			}

		case 0x05:							// 5 - bonus count
		case 0x06:							// 6 - pre-fight setup
		case 0x07:							// 7 - round intro
			/* barge_game_start */
			if (*pstate==PS_ACTIVE || f_death>0)
				process_suicide();
			else
			{
				clr_scrn();
				murder_myoinit();
				idcomp_base=image_dcomp_buffer;			// reset buffer

				advance_curback();

				b0=p1_state;			// save pstates
				b1=p2_state;

				game_init();

				p2_state=b1;			// restore pstates
				p1_state=b0;

				goto_mode_select(pstate,poffset);
			}
			break;

			case 0x10:		// start_during_debug		10 - debug
				process_suicide();
				break;
	} /* gstate switch */

/*
	SHOULD ONLY GET TO THIS CODE FROM ONE OF THE ABOVE CASES
	NOTHING SHOULD FALL THROUGH TO THIS POINT!!!
*/
	while (TRUE);
}

void goto_mode_select(WORD *pstate,WORD poffset)
{
	WORD *pstate_tmp;

	idcomp_base=image_dcomp_buffer;			// reset buffer
	call_every_tick=NULL;					// dont call anything
	current_proc->procid=PID_MASTER;				/* i am the master game flow proc */

	/* sag3 */
	if (poffset==0) 								/* clear win in a row & total rounds won */
	{
		p1_wiar=p1_rwon=0;
		pstate_tmp=&p1_state;
	}
	else
	{
		p2_wiar=p2_rwon=0;
		pstate_tmp=&p2_state;
	}

	update_pmsg();

	mode_select();

	stack_jump(post_mode_select);
}

void post_mode_select(void)
{
	current_proc->procid=PID_MASTER;

	gstate=GS_PSEL;						/* game state = player select mode */

	eliminate_buyins();

	match_init();
	player_select();

	if (count_active_players()==1 && f_freeplay==0)
		credits--;							// only 1 players, that going cost

	stack_jump(game_loop);
}

void game_loop(void)
{

	if (count_active_players()==0)
	{
		stack_jump(game_over);
	}
	
	stack_jump(round_loop);
}

JUMPTBL rl_tbl[]=
{
	one_on_one,
	two_on_two,
	tournament_play
};

void round_loop(void)
{
	stack_jump(rl_tbl[mode_of_play]);
}

void one_on_one(void)
{
	WORD pcount;

	p1_preload=bkgd_preload=0;		// clear preload flags

	round_1_vs();

	check_battle_milestone();

	if ((pcount=count_active_players())==0)
	{
		stack_jump(game_over);
	}

	if (pcount !=2)
	{
		clear_combo_ram();
		gstate=GS_ROUND_INTRO;						/* game state: intro (1 player) */
		if (set_drone_ochar()==SYSTEM_CARRY_SET)
		{
			stack_jump(game_over);
		}
	}

	/* PLAY1: */
	/* play 1 round */
	f_play3=1;
	round_results=play_1_round();

PLAY2:
	if (is_endurance_possible()==SYSTEM_CARRY_CLR) 
		goto PLAY3_PRE;
	
	(short)current_proc->a0=a0_next_endurance_guy();
	if ((short)current_proc->a0>=0)
	{
		spawn_endurance_guy(current_proc->a0);
		goto PLAY2;
	}

PLAY3_PRE:
	if ( play3_pa14!=NULL )
	{
		stack_jump(play3_pa14);
	}
	else
	{
		stack_jump(play3);
	}
}

void play3(void)
{
	WORD pcount;

	results_of_round(round_results);
	f_play3=0;

	if (p1_matchw==2 || p2_matchw==2)			/* anyone won 2 yet? */
	{
		inc_winners_wiar();
		update_win_counters();

		if ((clk_ones+clk_tens) != 0)			/* if time left, then allow finishing moves */
		{
			finish_him_sequence();
			set_winner_status();
		}
	}

	/* PLAY7: */
	f_thatsall=1;								/* flag: enough fighting already */
	bonus_count();

	if (p1_matchw!=2 && p2_matchw!=2)
	{
		stack_jump(round_loop);
	}

	end_of_match_chores();

	pcount=count_active_players();				// # of active players before buyin

	loser_in_buyin();								/* loser = buyin state */

	if ( pcount==1 && credits==0 && p1_state!=PS_ACTIVE && p2_state!=PS_ACTIVE)
	{
		check_enter_initials1();					// outta money for one player restart
		stack_jump(game_over);
	}

	check_enter_initials1();
	do_buyin();

	if (pcount!=1)
		check_enter_initials2();							/* if loser didn't buyin, check hstd */

	/* skip_inits */
	if ((WORD)current_drone_a0()==SP_END)
	{
		stack_jump(game_over);
	}

	current_proc->a1=mode_of_play;
	mode_of_play=0;

	if (current_proc->a1==1) 
	{
		stack_jump(buyin_to_1_on_1);
	}
		
	stack_jump(game_loop);
}

void buyin_to_1_on_1(void) 
{
	p1_wiar=p2_wiar=0;
	p1_rwon=p2_rwon=0;
	p1_state=p2_state=PS_PSEL;

	stack_jump(post_mode_select);

}

short stk_p1_char;
short stk_p2_char;
short stk_p3_char;
short stk_p4_char;

void two_on_two(void) 
{
	PROCESS *pa1,*pa10;
	WORD *pbar;
	short pchar;

	switch (round_num)
	{
		case 1:		// char_setup_2
			stk_p1_char=p1_char;
			stk_p2_char=p2_char;
			stk_p3_char=p3_char;
			stk_p4_char=p4_char;	// grav original chooses

			p1_char=stk_p2_char;	// round 2 = p2 vs p4
			p2_char=stk_p4_char;
			p3_char=stk_p1_char;	// round 2 = p1 vs p3
			p4_char=stk_p3_char;
			break;

		case 0:
		case 2:
		case 3:
		case 4:		// char_setup_1,3,4,5
		default:
			stk_p1_char=p1_char;
			stk_p2_char=p2_char;
			stk_p3_char=p3_char;
			stk_p4_char=p4_char;	// grav original chooses

			p2_char=stk_p3_char;		// round 1= 
			p3_char=stk_p2_char;
			break;
	}

	/* tot0 */
	p1_preload=bkgd_preload=0;		// clear preload flags

	round_1_vs();

	p1_heap_char=p2_heap_char=0xffff;	// force players to reinit

	round_results=play_1_round();

TOT1:
	switch (round_results)
	{
		case 2:			// tot_tie
			stack_jump(game_over);	
			LOCKUP;					// should never will go to TOT2
			break;
		case 0:			// tot_p1wins
			pa1=p2_proc;	// loser proc
			pchar=p4_char;			// replacement ochar
			pbar=&p2_bar;			// power bar we are waiting for
			break;
		case 1:			// tot_p2wins
			pa1=p1_proc;	// loser proc
			pchar=p3_char;			// replacement ochar
			pbar=&p1_bar;			// power bar we are waiting for
			break;
		case 3:			// tot_timeout
			f_thatsall=1;
			print_timeout_msg();
			
			round_results=round_is_over();
			goto TOT5;
			break;
	}

	/* TOT2: */
	if ((pa1->pdata.p_flags & PM_WINGMAN)==0)	// did we defeat a "wingman", 
	{
		pa10=spawn_wingman(pa1,pchar);
		pa10->pdata.p_flags|=PM_JOY;		// joystick dude flag
		wait_for_wingman(pbar);
		round_results=continue_fighting();
		goto TOT1;
	}

TOT5:
	p1_char=stk_p1_char;			// restore original choicses
	p2_char=stk_p2_char;			// goto --> end of round (bonus count)
	p3_char=stk_p3_char;
	p4_char=stk_p4_char;

	stack_jump(play3);
}

//********************************************************************8

WORD word1=1;
WORD word2=2;
WORD word3=3;
WORD word4=4;
WORD word5=5;
WORD word6=6;
WORD word7=7;
WORD word8=8;

WORD *tmatchups[]=
{
	&word1,
	&word2,		// 1st match = p1 vs. p2
	&word3,
	&word4,		// 2nd match = p3 vs. p4
	&word5,
	&word6,		// 3rd match = p5 vs. p6
	&word7,
	&word8,		// 4th match = p7 vs. p8
	&twinners[0],
	&twinners[1],	// 5th match = match 1 winner vs. match 2 winner
	&twinners[2],
	&twinners[3],	// 6th match = match 3 winner vs. match 4 winner
	&twinners[4],
	&twinners[5],	// 7th match = match 5 winner vs. match 6 winner
};

void tournament_play(void) 
{
	WORD stk_a10;
	WORD **ta11;
	WORD *ta0,*ta1;

	current_proc->a10=stk_a10=0;
	twinners[0]=twinners[1]=twinners[2]=twinners[3]=0;
	twinners[4]=twinners[5]=twinners[6]=0;

	/* tp3 */
	do
	{
		tournament_tree(stk_a10);

		ta11=&tmatchups[stk_a10];		// a11 ---> next match up !!
		ta0=*ta11;								// a0 ---> player ochar
		ta11++;									
		ta1=*ta11;								// a1 ---> to player ochar
		ta11++;									

		p1_char=p1_tour[*ta0-1];			// setup player 1
		p2_char=p1_tour[*ta1-1];			// setup player 2

		vs_or_ladder();
		p1_matchw=p2_matchw=round_num=0;		// reset for tournament

		round_results=play_1_round();

		/* ror_tournament */
		results_of_round(round_results);
		f_thatsall=1;			// flag: enough fighting

		bonus_count();

		switch (round_results)
		{
			case 0:			// tp_p1wins
				ta11=&tmatchups[stk_a10];		// a11 ---> next match up !!
				ta0=*ta11;
				ta11++;
				break;
			case 1:			// tp_p2wins
				ta11=&tmatchups[stk_a10];		// a11 ---> next match up !!
				ta11++;
				ta0=*ta11;
				ta11++;
				break;
			case 2:			// tp_tie
			case 3:			// tp_timeout
				stack_jump(game_over);
				LOCKUP;			// should never get here
				break;
		}

		/* tp7 */
		twinners[stk_a10]=*ta0;		// stuff winner's player

		stk_a10++;

		p1_heap_char=p2_heap_char=0xfff;
		
	}
	while(stk_a10!=7);			// more matches to play again

	/* tournament is over */
	tournament_tree(stk_a10);

	murder_myoinit_score();
	bogus_dlist;

	p1_char=p2_char=p1_tour[*ta0-1];
	p15_centered(get_winner_text(),SCX(0xc8),SCY(0x70)+8);
	process_sleep(0x40);

	murder_myoinit_score();

	stack_jump(end_of_tournament);
}

void round_1_vs(void) 
{
	if (round_num==0)
	{
		vs_or_ladder();
		p1_heap_char=p2_heap_char=0xffff;
	}


	return;
}

/******************************************************************************
 Function: void set_winner_status(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	set the winner status
******************************************************************************/
void set_winner_status(void)
{
	winner_status=(p1_bar==0)? 2:1;		// declare the winner
	return;
}

void play_1_match(void) 
{
	//-DHS- write code when needed
	LOCKUP;
}

void spawn_endurance_guy(WORD pa0) 
{
	if (round_results==0) 
	{
		spawn_wingman(p1_proc,pa0);
		wait_for_wingman(&p1_bar);
	}
	else
	{
		spawn_wingman(p2_proc,pa0);
		wait_for_wingman(&p2_bar);
	}

	round_results=continue_fighting();
	return;
}

WORD is_endurance_possible(void)
{
	short ta0;

	if (count_active_players()!=1)
		q_no;

	if (map_start==NULL)
		q_no;

	(short)current_proc->a0=ta0=*(map_start+map_position);	// set a0 for other routines, sloppy time
	if (ta0 & 0x80) 
	{
		current_proc->a14=0;
		if (p1_state!=PS_ACTIVE)
			current_proc->a14=1;

		if (current_proc->a14==round_results)
			q_yes;
	}

	q_no;
}

short a0_next_endurance_guy(void) 
{
	short *ta0;

	current_proc->a0&=0x7f;

	ta0=endurance_tables[current_proc->a0];

	((ADDRESS)current_proc->pdata.p_slave)+=1;		// next endur offset

	return(*(ta0+(ADDRESS)current_proc->pdata.p_slave));
}

void fatal_demo_loop(void) 
{
	//-DHS- write code

	LOCKUP;
}

/******************************************************************************
 Function: void game_over(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	Game over routines
******************************************************************************/
char txt_gameover[]="GAME OVER";
void game_over(void)
{
	gstate=GS_GAMEOVER;

	DONT_SHOW_SCORES;
	reset_map_variables();
	diff=0;

	p1_state=p2_state=0;				// take players out of game

	immune_mpp();
	MURDER;

	fast_fadeout_jsrp(0x20);

	MURDER;

	/* ending_gover_entry */
	if ( snd_module_status!= SMOD_GAMEOVER )
	{
		sound_module_load(SMOD_GAMEOVER);		// if already playing dont do it
		if ( f_music )
		{
			current_proc->a0=f_no_sfx;		// make sure music plays
			f_no_sfx=0;
			tsound(0x91);			// play correct tune
			f_no_sfx=current_proc->a0;
		}
	}

	clr_scrn();
	murder_myoinit_score();
	unimmune_mpp();
	MURDER;						// kill lingering multiparts

#if SMOKE_PAGE
	if (q_f_mileena()==SYSTEM_CARRY_CLR || q_f_ermac()==SYSTEM_CARRY_CLR || q_f_oldsz()==SYSTEM_CARRY_CLR)
	{
		/* game_over_exit */
		enter_ukk();
	}
	else
	{
		do_background(BKGD_LOST_MOD);
		p15_centered(txt_gameover,SCX(0xc8),SCY(0x58));
		process_sleep(0x40*5);
	}
#else
	do_background(BKGD_LOST_MOD);
	p15_centered(txt_gameover,SCX(0xc8),SCY(0x58));
	process_sleep(0x40*6);
#endif

	MURDER;
	stack_jump(gover_amode_entry);
}

/******************************************************************************
 Function: void clear_combo_ram(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	clear combo ram stuff (briefcase)
******************************************************************************/
void clear_combo_ram(void)
{
	combo_1=combo_2=combo_3=0;
	combo_4=combo_5=combo_6=0;

	return;
}

/******************************************************************************
 Function: void game_init(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	Initialize variables for starting a game
******************************************************************************/
void game_init(void)
{
	c_curback=-1;
	advance_curback();

	battle_num=1;

	c_1p_tries=1;

	f_secret=0;
	f_shadows=0;
	f_nosound=0;
	round_num=0;						/* round one !!! */
	winner_status=0;					/* no winner */
	coinflag=0;							/* flag: coin page not up */

	p1_xvel.pos=0;
	p1_obj=NULL;
	p1_proc=NULL;
	p1_state=0;
	p1_map.pos=0;
	p1_wiar=0;
	p1_rwon=0;

	p2_xvel.pos=0;
	p2_obj=NULL;
	p2_proc=NULL;
	p2_state=0;
	p2_map.pos=0;
	p2_wiar=0;
	p2_rwon=0;

	init_player_variables();

	return;
}

/******************************************************************************
 Function: void init_player_variables(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	Initialize variables for players 1 & 2
******************************************************************************/
inline void init_player_variables(void)
{

	p1_matchw=p2_matchw=0;
	p1_perfect=p2_perfect=0;

	p1_button=p2_button=bt_null;			/* disable buttons */

	return;
}

/******************************************************************************
 Function: WORD play_1_round(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: 0 --> player 1 won round
		  1 --> player 2 won round
		  2 --> tied *
		  3 --> timeout (clock hit zero)

 Description:	Master process routine to play 1 round

	Note: Due to the arcade assembly code making this code reentrant at multiple spots
		  we have split the code up into small chunks so that we can keep the correct flow
******************************************************************************/
WORD play_1_round(void)
{
	force_it();
	p1_name_char=p1_char;
	p2_name_char=p2_char;

	send_code_a3(0);						// shut up all sounds

	play3_pa14=NULL;					// reset return func, if used, will be set in try_briefcase

	SET_GAME_STATE(GS_PFSETUP);

	if (gstate!=GS_AMODE)
		murder_myoinit_score();					// amode = dont kill everyone

	/* play00 */
	object_initialize();
	setup_score_ram();
#if VS_OFF
	/* arcade display flow */
	DISPLAY_OFF;						// -VSON- remove if leave vs or ladder screen up
#endif

	ochar_force_curback();			// force backgrounds

	if ( round_num==0 && f_load==0)
	{
		file_load_message(3);
	}

	round_init();
	update_knotches();
	update_pmsg();
	update_win_counters();
	SHOW_SCORES;

	try_kombat_kodes();
	do_background(curback);				

	/* load fight sound effects */
	sound_module_load(SMOD_FIGHT);

	CREATE(PID_REPELL,repell);
	CREATE(PID_SCROLL,scroller);

	setup_players();
	start_shadows();

	f_load=0;
	DISPLAY_OFF;

	psxcd_seek_for_play_at((f_music==0)?TRK_FIGHT:get_background_tune(),(f_no_sfx==0)?2:300);

	SET_GAME_STATE(GS_ROUND_INTRO);

	/* PLAY1A: */
	f_sync=0;
	CREATE(PID_FX,round_intro_fx);
	process_sleep(1);

#if MK_EJBBUG
	if (df_fastrun!=0)
		goto SKIP_INTRO;
#endif

	fast_fadein_jsrp(0x020);

	while ( f_sync==0 )
		process_sleep(1);

SKIP_INTRO:
#if MK_EJBBUG
	DISPLAY_ON;
#endif

	/* POST_SEC_TRASH: */

	/* cheat code */
	if (df_nopower)
		p1_bar=p2_bar=1;

	/* cheat code */
	if (f_one_win)
		p1_matchw=p2_matchw=1;

	return(master_mercy_entry());
}

WORD master_mercy_entry(void)
{
	f_start=1;								/* flag: start fighting !! */

	if (gstate==GS_AMODE)
		process_suicide();

	/* clock countdown */
	SET_GAME_STATE(GS_FIGHTING);

	clk_ones=clk_tens=CLOCK_INIT_DIGIT;					/* set clock */

	while (TRUE)
	{
		/* CLOCK3: */
		update_clock_ones(entry_12,clk_ones);

		if (clk_tens<2)
			tsound(0x17);							/* tick sound */

		/* CLOCK5: */
		if (clk_ones+clk_tens!=0)
		{
			(LONG)current_proc->pa8=(mode_of_play==1) ? CLOCK_SPEED*2:CLOCK_SPEED;					/* clock speed counter */
			do
			{
				/* CLOCK4: */
				process_sleep(3);							/* sleep */

				if (p1_bar==0 || p2_bar==0)
				{
					/* round_is_over */
					return(round_is_over());
				}
			} while (--((LONG)current_proc->pa8) !=0);

			if (!f_no_clock)
				clk_ones--;

			if (clk_ones<0)
			{
				clk_ones=CLOCK_INIT_DIGIT;
#if CLOCK_ON
				if (!f_no_clock)
					clk_tens--;
#endif
				update_clock_tens(entry_11,clk_tens);
			}
		}
		else
		{
			/* round_timeout */
			clk_ones=clk_tens=0;					// zero clock

			/* prend */
			killfx();
			weed_out_cheaters();
			return(ROUND_TIMEOUT);
		}
	}
}

/******************************************************************************
 Function: void results_of_round(WORD results)

 By: David Schwartz

 Date: Sept 1994

 Parameters: results - results of round of fight

 Returns: None

 Description:	Acts on the results of the round based on:
		results =
			0 - player 1 won
			1 - player 2 won
			2 - tied
			3 - timeout (clock hit zero)
******************************************************************************/
void results_of_round(WORD results)
{
	if (results==ROUND_TIMEOUT)
	{
		f_thatsall=1;							// flag: enough fighting already
		print_timeout_msg();

		/* round_is_over */
		results=round_is_over();
	}

	switch (results)
	{
		case ROUND_P1_P2_TIE:
			return;
			break;
		case ROUND_P1_WON:
			winner_status=1;						// winner status=player 1
			p1_matchw++;							// add to his match wins
			p1_rwon++;								// add to his total rounds won
			break;
		case ROUND_P2_WON:
			winner_status=2;						// winner status=player 2
			p2_matchw++;							// add to his match wins
			p2_rwon++;								// add to his total rounds won
 			break;
	}


	/* results_retp */
	update_knotches();

	if (winner_status==1)
	{
		if (p1_matchw!=2 || p1_state!=PS_ACTIVE || count_active_players()==2)
			return;
	}
	else
	{
		if (p2_matchw!=2 || p2_state!=PS_ACTIVE || count_active_players()==2)
			return;
	}

	if (map_start==NULL)
		return;

	if (*(map_start+map_position+1)!=SP_END)
		return;							// player hasn't beat game yet

	/* game_finished */
	reset_map_variables();

	gstate=GS_GAMEOVER;						/* game over */

	shao_kahn_death_fx();
}

WORD round_is_over(void) 
{
	killfx();
	weed_out_cheaters();

	if (p1_bar==p2_bar)						/* tie game */
		return(ROUND_P1_P2_TIE);
	else return((p1_bar>p2_bar) ? ROUND_P1_WON:ROUND_P2_WON);		/* p1 or p2 won */
}

/******************************************************************************
 Function: void master_proc_mercy(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	mercy process
******************************************************************************/
void master_proc_mercy(void)
{
	f_start=f_thatsall=0;				// dont finish fight yet

	if (p1_bar!=0)
	{
		/* player 1 is the mericful one */
		current_proc->a10=p1_char;
		if (p1_wiar!=0)
			p1_wiar--;				// take "win in a row" from the winner
	}
	else
	{
		/* player 2 is the mericful one */
		current_proc->a10=p2_char;
		if (p2_wiar!=0)
			p2_wiar--;				// take "win in a row" from the winner
	}

	/* merc3 */
	restore_power(&p1_bar,&p2_matchw);
	restore_power(&p2_bar,&p1_matchw);			// more power to the loser

	if (clk_tens<4)
		clk_tens=4;				// make sure enough time to fight

	/* merc4 */
	update_knotches();
	update_pmsg();
	update_win_counters();
	SHOW_SCORES;

	/* fade down tune to prevent popping */
  	psxspu_start_cd_fade(FADE_TIME*2,0);

	(create_fx(FX_MERCY))->a10=current_proc->a10;			// pass merciful character

	gstate=GS_ROUND_INTRO;				// game state: intro;
	process_sleep(0x40);

	fastxfer(p1_proc,mercy_start);
	fastxfer(p2_proc,mercy_start);			// both players in stance animation

	process_sleep(0x40);

	winner_status=0;

	round_results=current_proc->a10=master_mercy_entry();
	stack_jump(play3);
}

void restore_power(WORD *pa5,WORD *pa6)
{
	if (*pa5==0)
	{
		*pa5=5;			// restore a little power
		*pa6=1;			// match wins = 1
	}

	return;
}

void mercy_start(void)
{
	back_to_normal();
	face_opponent(current_proc->pa8);
	disable_all_buttons;
	wait_for_start();
	reaction_exit(current_proc->pa8);
}

void wait_for_wingman(WORD *pbar) 
{
	current_proc->a14=0x40*3;
	do
	{
		process_sleep(1);
		if (*pbar!=0)
			break;
	}
	while(--current_proc->a10>0);

	return;
}

PROCESS *spawn_wingman(PROCESS *pa1,WORD pa5) 
{
	//-DHS- write code 
	LOCKUP;
	return(NULL);
}

/******************************************************************************
 Function: void check_enter_initials(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	check if its time to enter initials, if so
				setup variables and call the routine
******************************************************************************/
void check_enter_initials1(void)
{
	if (p1_state==0 || p2_state==0)
		return;

	check_enter_initials2();
}

void check_enter_initials2(void)
{
	if ( (p1_state+p2_state)==0 )
		return;								// nobody playing anymore = no initials

	if (p1_state!=PS_ACTIVE || (p1_state==PS_ACTIVE && p2_state!=PS_BUYIN))
	{
		if ( does_a0_cut_it(p1_wiar)==SYSTEM_CARRY_SET )
		{
	 		/* cei1, p1 enter initals */
	 		current_proc->pdata.p_flags=0;
	 		(ADDRESS)current_proc->pdata.p_anitab=p1_wiar;
	 		enter_initials();
	 		return;
		}
	}

	if ( p2_state!=PS_ACTIVE || (p2_state==PS_ACTIVE && p1_state!=PS_BUYIN))
	{
		if (does_a0_cut_it(p2_wiar)==SYSTEM_CARRY_SET)
		{
		 	/* cei2, p2 enter initals */
	 		current_proc->pdata.p_flags=1;
	 		(ADDRESS)current_proc->pdata.p_anitab=p2_wiar;
	 		enter_initials();
	 		return;
		}
	}

	return;
}

/******************************************************************************
 Function: void match_init(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	setup match
******************************************************************************/
void match_init(void)
{
	clear_combo_ram();
	f_gameflags=0;
}

/******************************************************************************
 Function: void round_init(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	Initialize things at the start of a round
******************************************************************************/
void round_init(void)
{
	int loop;

	round_num++;						/* next round */

	if (++c_three >= 4)					/* keep track of 1-2-3 */
		c_three=1;

	/* initialize switch queue ram */
	p1_bcq[0]=(LONG)(1);					/* set ptr to first entry */
	p1_jcq[0]=(LONG)(1);					/* set ptr to first entry */
	p1_boq[0]=(LONG)(1);					/* set ptr to first entry */
	p1_joq[0]=(LONG)(1);					/* set ptr to first entry */

	p2_bcq[0]=(LONG)(1);					/* set ptr to first entry */
	p2_jcq[0]=(LONG)(1);					/* set ptr to first entry */
	p2_boq[0]=(LONG)(1);					/* set ptr to first entry */
	p2_joq[0]=(LONG)(1);					/* set ptr to first entry */

	for (loop=1;loop<=SQS;loop++)
	{
		p1_bcq[loop]=0;
		p1_jcq[loop]=0;
		p1_boq[loop]=0;
		p1_joq[loop]=0;

		p2_bcq[loop]=0;
		p2_jcq[loop]=0;
		p2_boq[loop]=0;
		p2_joq[loop]=0;
	}

	recharge_bars();					/* set strength for players */

	clk_ones=clk_tens=9;				/* set clock */

	zero_round_init_words();

	zero_round_init_longs();

	/* rinit5 */
	l_sz_decoy[0]=0;
	l_sz_decoy[1]=0;
	l_morph[0]=0;
	l_morph[1]=0;
	l_slide[0]=0;
	l_slide[1]=0;
	l_liazap[0]=0;
	l_liazap[1]=0;
	l_net[0]=0;
	l_net[1]=0;
	l_angle[0]=0;
	l_angle[1]=0;
	f_aabuse=0;
	f_upcut_rec=0;
	f_unlim_run=0;
	lem4[0]=0;
	lem4[1]=0;
	l_jaxzap2[1]=0;
	l_jaxzap2[0]=0;

	l_throw_fan[0]=l_throw_fan[1]=0;
	l_decoy[0]=l_decoy[1]=0;
	l_flash[0]=l_flash[1]=0;
	l_spear[0]=l_spear[1]=0;
	l_block_fk[0]=l_block_fk[1]=0;
	l_mileena_roll[0]=l_mileena_roll[1]=0;
	l_puddle[0]=l_puddle[1]=0;
	f_gameflags=0;
	l_swat_gun[0]=l_swat_gun[1]=0;
	l_ermac_slam[0]=l_ermac_slam[1]=0;
	l_orb_slow[0]=l_orb_slow[1]=0;
	l_orb_fast[0]=l_orb_fast[1]=0;
	l_zap[0]=l_zap[1]=0;

	room7[0]=room7[1]=0;
	room8[0]=room8[1]=0;

	f_victory_start=0;
	f_no_bonus=0;

	CREATE(PID_FLASHER,flash_pmsg);

	return;
}

/******************************************************************************
 Function: void round_intro_fx(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters: None

 Returns: None

 Description:	Announce the start of a round and give clues away if needed
******************************************************************************/
void round_intro_fx(void)
{
	create_fx(FX_CHARGE_BARS);

//**********************************************************
#if 0
	/* starts the round # phrase */
	if (round_num<=4)
	{
		psxcd_seek_for_play_at(triple_sndtab[0x11+round_num-1],2);
		print_round_num();

#if _CD_VERSION_==1
		while (psxcd_elapsed_sectors()<150)
			process_sleep(1);
#endif
	}
	else
	{
		lm_printf_p1(pf_round_num,round_num);
		process_sleep(0x30);
	}
#endif
//**********************************************************

		//-DHS-psxcd_seek_for_play_at(get_background_tune(),2);
		print_round_num();

//**********************************************************
#if _CD_VERSION_==1
		while (psxcd_seeking_for_play() )
			process_sleep(1);
#endif

		if ( f_music==0 || test_gameflag(M_GF_NOMUSIC))
			play_generic_stream(TRK_FIGHT);
		else psxcd_play_at_andloop(get_background_tune(),cd_vol_tbl[f_music],(f_no_sfx==0)?2:300,0,
												get_background_tune(),cd_vol_tbl[f_music],450,3000);
		//psxcd_play_at(get_background_tune(),cd_vol_tbl[f_music],(f_no_sfx==0)?2:300);
//**********************************************************

		process_sleep(0x10);

#if _CD_VERSION_==1
		while (psxcd_elapsed_sectors()==0)
			process_sleep(1);
#endif

		create_fx(FX_FIGHT_INTRO);

		dallobj(OID_TEXT);						// erase round #

	process_suicide();
}

/******************************************************************************
 Function: void print_round_num(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	prints the round number
******************************************************************************/
void print_round_num(void)
{
	//if (round_num<=4)
	//	triple_sound(round_num+0x10);		// speech: ROUND #

	lm_printf_p1(pf_round_num,round_num);

#if 0
#if _CD_VERSION_==1
		while (psxcd_seeking_for_play() )
		{
			process_sleep(1);
			i++;
		};
#endif

		play_generic_stream(triple_sndtab[0x11+round_num-1]);

#if _CD_VERSION_==1
		while (psxcd_elapsed_sectors()==0)
		{
			process_sleep(1);
		}
#endif
#endif

	process_sleep(0x30);

	//dallobj(OID_TEXT);						// erase round #

	return;
}

/******************************************************************************
 Function: void check_battle_milestone(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	check for special event conditions as follows:
		1)	must be 2 player game
		2)	must be on battle 100
		3)  must be in round 1

******************************************************************************/
void check_battle_milestone(void)
{
	WORD ta0,ta1;

	if (count_active_players()==2 && battle_num==100 && round_num==0)
	{
		ta0=p1_state;
		ta1=p2_state;
		hidden_game_load();				// take it away mark & kent
		murder_myoinit();
		p1_state=ta0;
		p2_state=ta1;
	}

	return;
}

/******************************************************************************
 Function: void weed_out_cheaters(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters: None

 Returns: None

 Description:	Jump to GAME OVER if:
			a.	Both players are at full strength
			b.	5th or later round
******************************************************************************/
void weed_out_cheaters(void)
{
	if (((p1_bar+p2_bar)==FULL_STRENGTH*2) || round_num>=5)
	{
 		/* weed6 */
 		stack_jump(game_over);
	}

	return;
}

/******************************************************************************
 Function: void zero_round_init_words(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	initializes vars for a round to 0 (WORD type only)
******************************************************************************/
void zero_round_init_words(void)
{
	f_sans_throws=0;
	f_sans_block=0;
	f_shadows=0;
	f_fade=0;
	f_dark=0;
	f_norepell=0;
	tick=0;
	f_timeout=0;
	winner_status=0;
	f_start=0;
	f_novel=0;
	f_death=0;
	f_final_act=0;
	f_warnsound=0;
	f_thatsall=0;
	c_goro_dummy=0;
	c_kahn_dummy=0;
	f_pause=PAUSE_CLEAR;
	f_superjumps=0;
	f_no_combos=0;
	f_no_special_moves=0;
	f_p1_half_damage=f_p2_half_damage=0;
	return;
}

/******************************************************************************
 Function: void zero_round_init_longs(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	initializes vars for a round to 0 (LONG type only)
******************************************************************************/
void zero_round_init_longs(void)
{
	int i;

	for (i=0;i<HQS;i++)
		p1_hitq[i]=p2_hitq[i]=0;

	l_hp[0]=l_hp[1]=0;
	l_lp[0]=l_lp[1]=0;
	l_block[0]=l_block[1]=0;
	l_hk[0]=l_hk[1]=0;
	l_lk[0]=l_lk[1]=0;

	p1_xvel.pos=p2_xvel.pos=0;

	return;
}

/******************************************************************************
 Function: WORD count_active_players(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: # of active players

 Description:	Determines number of human players in game
******************************************************************************/
inline WORD count_active_players(void)
{
	WORD i=0;

	if (p1_state==PS_ACTIVE)
		i++;

	if (p2_state==PS_ACTIVE)
		i++;

	return(i);
}

/******************************************************************************
 Function: WORD set_drone_ochar(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: SYSTEM_CARRY_SET - end of game reached
		  SYSTEM_CARRY_CLR  - end of game not reached

 Description:	Determines who is the next drone opponent based on the player
				up's map position
******************************************************************************/
WORD set_drone_ochar(void)
{
	WORD *pchar;
	WORD ta0;

	pchar=drone_char_point_a6();
	ta0=current_drone_a0();

	if (ta0==SP_END)
		return(SYSTEM_CARRY_SET);

	if (map_start==NULL)
		return(SYSTEM_CARRY_CLR);	// no map = dont set ochar

	*pchar=ta0;
	return(SYSTEM_CARRY_CLR);
}


WORD *drone_char_point_a6(void)
{
	if (who_is_alone()==1)
	{
		return(&p2_char);
	}
	else
	{
		return(&p1_char);
	}
}

short current_drone_a0(void)
{
	short ta0;

	if (map_start==NULL)
		return(0);

	ta0=*(map_start+map_position);

	if (ta0 & 0x80) 
	{
		/* endurance stuff */
		(long)current_proc->pdata.p_slave=0;		// save offset here
		ta0&=0x7f;					// strip flag bit
		ta0=*endurance_tables[ta0];
	}

	return(ta0);		// next "enemy offset" to battle with
}

/******************************************************************************
 Function: WORD who_is_alone(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: 1 ----> player 1 is the only human
		  2 ----> player 2 is the only human

 Description:	Determines which player is human
******************************************************************************/
WORD who_is_alone(void)
{
	if (p1_state==PS_ACTIVE)
	{
		if (p2_state==PS_ACTIVE)					/* error if player 2 active */
			while (TRUE);
		return(1);
	}
	else if (p2_state==PS_ACTIVE)
			return(2);

	return(1);
}

/******************************************************************************
 Function: void inc_winners_wiar(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	Increment winners (WIAR variable)
******************************************************************************/
void inc_winners_wiar(void)
{
	WORD *pstate,*pwiar;

	if ((winner_status-1)!=0)
	{
		pstate=&p2_state;					/* player 2 wins in a row  */
		pwiar=&p2_wiar;
	}
	else
	{
		pstate=&p1_state;					/* player 1 wins in a row  */
		pwiar=&p1_wiar;
	}

	if (*pstate!=PS_ACTIVE)					/* winner active (ie..human)??? */
		*pwiar=0;							/* nope... */
	else (*pwiar)++;						/* human get another win in a row */
}

/******************************************************************************
 Function: void is_finish_him_allowed(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - allow finish him
 		  SYSTEM_CARRY_CLR - disallow finish him

 Description:	handle things for FINISH HIM
******************************************************************************/
WORD is_finish_him_allowed(void)
{
	switch (p1_char)
	{
		case FT_MOTARO:
		case FT_SK:
		case FT_NOOB:
			return(SYSTEM_CARRY_CLR);
			break;
	}

	switch (p2_char)
	{
		case FT_MOTARO:
		case FT_SK:
		case FT_NOOB:
			return(SYSTEM_CARRY_CLR);
			break;
	}

	if ((clk_ones+clk_tens)==0)
		return(SYSTEM_CARRY_CLR);	// no time left
	else return(SYSTEM_CARRY_SET);

}

/******************************************************************************
 Function: void finish_him_sequence(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters: None

 Returns: None

 Description:	handle things for FINISH HIM
******************************************************************************/
void finish_him_sequence(void)
{
	WORD fatal_timer;

	if (is_finish_him_allowed()==SYSTEM_CARRY_CLR)
		return;

	winner_status=3;						// flag: finish him mode

	f_no_special_moves=0;					// allow fatalities to happen

	finish_him_or_her();

	fatality_wait(0x28);

	/* over? */
	if (f_thatsall==0)
	{
		//play_finish_tune();							// proper "finish him" tune

		fatal_timer=fatality_wait(0x80);

		/* check if fatality started */
		if (fatal_timer!=0)
		{
			fatal_timer=0x40*8;
			do
			{
				process_sleep(1);		// wait until death blow complete or time up
			}
			while((f_death!=-1) && (--fatal_timer>0));
		}
	}

	return;
}

/******************************************************************************
 Function: WORD fatality_wait(WORD waittime)

 By: David Schwartz

 Date: Oct 1994

 Parameters: waittime - max time i will wait

 Returns: timed out = 0
		  fatality started > 0

 Description:	wait around for the FATALITY to start
******************************************************************************/
WORD fatality_wait(WORD waittime)
{
	do
	{
		current_proc->p_comp_flag=PC_FATAL_WAIT;
		process_sleep(1);
		current_proc->p_comp_flag=PC_CLEAR;

		/* fatality_waiting */
		if (f_death!=0)					// death blow happened
			return(waittime);

		if (f_thatsall!=0)				// ran outta game time
			return(0);

		if (f_unlim_fatal && f_victory_start==0)				// unlimited time, keep timer big
			waittime=20;

	}
	while(--waittime != 0);

	return(0);
}

/******************************************************************************
 Function: void end_of_match_chores(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters: None

 Returns: None

 Description:	do this when a match is over (2 out of 3)
******************************************************************************/
void end_of_match_chores(void)
{
	advance_curback();

	battle_num++;											// next battle please !!

	irqskye=0;			// keep background black

	round_num=c_three=f_mercy=0;

	dallobj(OID_TEXT);										// erase msg

	p1_matchw=p2_matchw=p1_perfect=p2_perfect=0;			// reset match wins and perfect for both dudes

	adv_winner_map();										// winner: move on!!
	diff_adjust();											// end of round diff adjust

	/* check "winning streak" reset counter, -SONY- needed for arcade only */

	return;

}

/******************************************************************************
 Function: void loser_in_buyin(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	stick loser in buyin state
******************************************************************************/
void loser_in_buyin(void)
{
	WORD *pstate;

	pstate=(winner_status==1)? &p2_state:&p1_state;

	if (*pstate==PS_ACTIVE)			// loser a human
		*pstate=PS_BUYIN;			// loser = human --> he is in buyin mode

	return;
}

/******************************************************************************
 Function: void print_timeout_msg(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	when a round times out we call this to interupt things & print "TIMES UP"
******************************************************************************/
char txt_times_up[]="TIME IS UP";
void print_timeout_msg(void)
{
	send_code_a3(0);				// stop music

	//send_code_a3(ST_SZ_ICE_UP);			// gong

	f_novel=f_timeout=1;

	scrolly.pos=0;
	stop_scrolling();

	MURDER;

	p15_centered(txt_times_up,SCX(0xc8),SCY(0x38)+16);

	/* rto6 */
	process_sleep(0x50);

	dallobj(OID_FX);

	return;
}

WORD tbl_ochar_curbacks[]=
{
	FT_MOTARO,BKGD_TOWER_MOD,		// motaro = curback = 4
	FT_SK,BKGD_PIT_MOD,
	FT_NOOB,BKGD_NOWALL_MOD,
	FT_OLDSZ,BKGD_LOST_MOD,
	FT_OLDSMOKE,BKGD_LOST_MOD,
	FT_ERMAC,BKGD_LOST_MOD,
	FT_MILEENA,BKGD_LOST_MOD,
	0
};

void ochar_force_curback(void) 
{
	WORD *ta7;

	if (count_active_players()==2)
		return;

	ta7=tbl_ochar_curbacks;
	
	while (*ta7!=0)
	{
		if ((*ta7==FT_ERMAC && f_ermac==1) ||		// only screw if character not active
		   (*ta7==FT_MILEENA && f_mileena==1) ||
		   (*ta7==FT_OLDSZ && f_oldsz==1)) 
		{
		   	ta7+=2;
		}
		else
		{
			if (*ta7==p1_char || *ta7==p2_char) 
			{
				curback=*(ta7+1);
				break;
			}
			ta7+=2;
		}
	}

	return;
}

/******************************************************************************
 Function: void reset_map_variable(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	reset map variables
******************************************************************************/
void reset_map_variables(void)
{
	map_position=0;
	map_start=0;
	perform=0;
	credits=GAME_CREDITS;			// # of 1 player credits
	return;
}

/******************************************************************************
 Function: void adv_winner_map(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters: None

 Returns: None

 Description:	advance winners map pointer to next position
******************************************************************************/
void adv_winner_map(void)
{
	WORD winner;

	if (count_active_players()!=2) 						// both human players -->skip
	{
		/* adv2 */
		winner=(p1_state==PS_ACTIVE)? 1:2;				// set which player won

		if (winner_status==winner) 						// check if winner is human
		{
			map_position++;
		}
		else
		{
			/* adv7 */
			c_1p_tries++;								// count # of 1p tries!!
		}
	}

	return;
}

short diff_advance_tbl[] = {1,1,1,2,3};

short diff_min_1[] =
{
	0,0,0,1,1,2,2,2,2,2,2,3,				// regular characters
	5,5,5,5,5,5								// bosses
};

short diff_min_2[] =
{
	0,0,1,2,2,2,2,2,3,3,4,4,				// regular characters
	5,5,5,5,5,5		   						// bosses
};

short diff_min_3[] =
{
	0,1,1,2,2,2,3,3,3,3,5,5,				// regular characters
	6,6,6,6,6,6		   						// bosses
};

short diff_min_4[] =
{
	1,2,3,3,3,4,5,6,6,6,7,7,				// regular characters
	7,8,8,8,8,8		   						// bosses
};

short diff_min_5[] =
{
	4,4,4,5,5,5,6,7,7,8,8,8,				// regular characters
	8,8,8,8,8,8		   						// bosses
};

short *diff_min_tables[] = {diff_min_1,diff_min_2,diff_min_3,diff_min_4,diff_min_5};


WORD ladderorder_a1(void) 
{
	return((ladder<=4)?ladder:1);
}

/******************************************************************************
 Function: void diff_adjust(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters: None

 Returns: None

 Description:	end of round adjustment of the variable @diff
******************************************************************************/
short ladder_diff_minimums[]={0,1,2,3};
short ladder_diff_slopes[]={1,2,2,3};

void diff_adjust(void)
{
	WORD *pstate;
	short adjtbl;
	WORD ta1;
	WORD ta0;

	pstate=(winner_status==1) ? &p1_state:&p2_state;

	if (*pstate==PS_ACTIVE)
	{
		/* winner is a human ---> get harder */
		if (count_active_players()==2)
			return;						// don't adjust if a two player game

		ta0=(++perform>=3) ? 9:perform-1;

		/* dadj3 */
		ta1=ladderorder_a1();
		diff+=ladder_diff_slopes[ta1];
		stuff_a0_diff();

	}
	else
	{
		/* winner is a drone */
		--perform;

		if (perform>3) 
		{
			/* wid3 */
			diff--;
		}
		else
		{
			diff=0;
		}

		stuff_a0_diff();

	}

	/* dadj5 */
	adjtbl=ladder_diff_minimums[ladderorder_a1()];

	if (diff<(adjtbl))
		diff=adjtbl;		// minimum diff level

	stuff_a0_diff();

	return;
}

void stuff_a0_diff(void) 
{
	if (diff<0)
		diff=0;

	if (diff>9)
		diff=9;				// to big=just use 9
	
	return;
}

/******************************************************************************
 Function: short get_adj_diff_word(short *difftbl)

 By: David Schwartz

 Date: Oct 1994

 Parameters: difftbl - diff adjust table to use

 Returns: delta diff

 Description:	get delta adjustment
******************************************************************************/
inline short get_adj_diff_word(short *difftbl)
{
	return(*(difftbl+get_adj_diff()));
}

/******************************************************************************
 Function: void amode_demo_game(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	attract mode game process
******************************************************************************/
void amode_demo_game(void)
{
	WORD ta1;

	ta1=f_nosound;
	game_init();
	f_nosound=ta1;

	object_initialize();
	setup_score_ram();
	get_fore_pal(SCORE_P);
	get_fore_pal(LOADING_P);

	curback=randu(11)-1;

	diff=5;
	stuff_a0_diff();

	play_1_round();

	wait_forever();
}

/******************************************************************************
 Function: void flash_pmsg(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	process to flash player messages, wait until there is one to flash
******************************************************************************/
void flash_pmsg(void)
{
	while (entry_5->disp_flag!=0 || entry_6->disp_flag!=0)
	{
		process_sleep(3);					// wait for legitimate message
	}

	flash_pmsg2();
}

/******************************************************************************
 Function: void flash_pmsg2(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	process to flash the player messages
******************************************************************************/
void flash_pmsg2(void)
{
	BYTE da10,da11;

	/* pmsg_blink_loop */
	while (TRUE)
	{
		da10=entry_6->disp_flag;
		da11=entry_5->disp_flag;

		if (p1_state!=PS_ACTIVE)
			entry_6->disp_flag=0;

		/* fpmsg5 */
		if (p2_state!=PS_ACTIVE)
			entry_5->disp_flag=0;

		/* fpmsg6 */
		process_sleep(14);

		/* fpmsg7 */
		if (entry_6->disp_flag==0)				// p1 still zero
			entry_6->disp_flag=da10;			// yes, then restore to normal

		/* fpmsg8 */
		if (entry_5->disp_flag==0)				// p2 still zero
			entry_5->disp_flag=da11;			// yes, then restore to normal

		/* fpmsg9 */
		process_sleep(14);
	}
}

/******************************************************************************
 Function: void finish_him_or_her(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	tell player to finish off the other dude
******************************************************************************/
void finish_him_or_her(void)
{
	OBJECT *pobj;

	pobj=(p1_bar==0)? p1_obj : p2_obj;

	switch (pobj->ochar)
	{
		case FT_LIA:
		case FT_SONYA:
		case FT_SG:
		case FT_KITANA:
		case FT_JADE:
		case FT_MILEENA:
			create_fx(FX_FINISH_HER);
			break;
		default:
			create_fx(FX_FINISH_HIM);
			break;
	}

	return;
}

/******************************************************************************
 Function: void advance_curback(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	change current background
******************************************************************************/

#define background_offsets &hidden_included[1]
WORD hidden_included[]=
{
	BKGD_LOST_MOD,		// frozen "lost" background
	BKGD_HADES_MOD,		// hell
	BKGD_DESERT_MOD,	// desert
	BKGD_WATER_MOD,		// water bridge
	BKGD_CAVE_MOD,		// cave
	BKGD_HADES_MOD,		// hell
	BKGD_DESERT_MOD,	// desert
	BKGD_WATER_MOD,		// water bridge
	BKGD_CAVE_MOD,		// cave

	BKGD_SUBWAY_MOD,	// subway
	BKGD_STREET_MOD,	// street
	BKGD_BANK_MOD,		//
	BKGD_ROOF_MOD,		// roof
	BKGD_TOWER_MOD,		// tower 
	BKGD_BRIDGE_MOD,	// bridge
	BKGD_SOUL_MOD,		// soul
	BKGD_BELL_MOD,		// bell tower
	BKGD_TEMPLE_MOD,	// church
	BKGD_GRAVE_MOD,		// grave
	BKGD_PIT_MOD,		// pit 3
};

void advance_curback(void)
{
	WORD *ta1;

	c_curback++;
	if (c_curback>=20)
		c_curback=0;

	ta1=((f_ermac+f_mileena+f_oldsz)==3)?hidden_included:background_offsets;

	curback=*(ta1+c_curback);

	return;
}

/******************************************************************************

 By: David Schwartz

 Date: Oct 1994

 Parameters: None

 Returns: adjustment diff

 Description:	retrieves the adjustment diff (0-4)
******************************************************************************/
short get_adj_diff(void)
{
	current_proc->a0=cmos_diff;

	if (current_proc->a0>4)
		current_proc->a0=2;
	return(current_proc->a0);				// return in (0-4) offset form
}

/******************************************************************************
 Function: short get_adj(WORD offset)

 By: David Schwartz

 Date: Jan 1995

 Parameters: adjustment # to get

 Returns: adjustment value

 Description:	gets an adjustment value
******************************************************************************/
short get_adj(WORD offset)
{
	return(3);
}

#if 0
/******************************************************************************
 Function: void barge_in_message(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	print that a player has intrupted game
******************************************************************************/
extern char txt_p1_entered[];
extern char txt_p2_entered[];

void barge_in_message(void)
{
	gstate=GS_BARGE;			// game start = barge!

	f_timeout=1;				// flag: this round was timedout
	MURDER;

	freeze_2_pages();
	DISPLAY_ON;

	lm_setup(pf_entered);

	if (p1_state==0)
		mk_printf(txt_p1_entered);
	else mk_printf(txt_p2_entered);

	(PROCESS *)current_proc->a11=CREATE(PID_FX,boonpal_stuff);
	((PROCESS *)current_proc->a11)->a11=(ADDRESS)fl_entered;

	//join_in_fade1(sans_boonpal);

	process_sleep(0x32);

	process_kill((PROCESS *)current_proc->a11);

	process_sleep(0x40);

	murder_myoinit_score();
	clr_scrn();

	return;
}
#endif

/******************************************************************************
 Function: void pause_setup(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	setup to pause the game
			1)	set pause flag
			2)	set process ID to pause class
			3)	dim screen
			4)	print pause message
******************************************************************************/
char txt_paused[]="PAUSED";
void pause_setup(WORD poffset)
{
	PROCESS *proc_ptr;

	f_pause=(poffset==PLAYER1_TYPE) ? PAUSE_P1_INIT:PAUSE_P2_INIT;	// game is now paused

	process_kill_class(PID_FADE,0xffff);

	current_proc->procid=PID_PAUSE;			// set id, so process can run during pause
	p15_centered(txt_paused,160,75);		// print pause message

	/* change all PID_FADE process to PID_PAUSE_FADE */
  psxspu_start_cd_fade(FADE_TIME,0);			// fade volume of cd

	//if (f_blast==0 && process_exist(PID_ROCK,0xffff)==NULL)
	//	pause_dimmer();						// fade screen, if not blasting through background, (pause bug)

	proc_ptr=(PROCESS *)&active_head;
	while (proc_ptr->plink != NULL)
	{
		proc_ptr=proc_ptr->plink;

		if (proc_ptr->procid==PID_FADE)
			proc_ptr->procid=PID_PAUSE_FADE;
	}

	return;
}

/******************************************************************************
 Function: void pause_hold(WORD poffset)

 By: David Schwartz

 Date: Mar 1995

 Parameters: poffset - offset of player that paused game

 Returns: None

 Description:	wait for pause to be released and then clear pause state
			1) kill pause message objects
			2) undim screen
			3) clear pause flag
******************************************************************************/
void pause_hold(WORD poffset)
{
	LONG mask;

	lights_on_hit();				// turn lights on to see pause message

	mask=(poffset==PLAYER1_TYPE) ? MASK_START:MASK_START<<16;

	/* pause music and kill effects */
	wess_seq_stopall();

	//psxspu_start_cd_fade(FADE_TIME/2,0);
	//process_sleep(1);
	psxcd_pause();

	f_pause-=2;

	/* wait for start to be released */
	while (swcurr.padvalue & mask)
	{
		process_sleep(1);
		psxcd_pause();
	}

	/* wait for start to be pressed */
	while ((swcurr.padvalue & mask)==0)
		process_sleep(1);

	/* start been pushed time to fight again */
	dallobj(OID_TEXT);								// kill all text

	//if ( f_blast==0 && process_exist(PID_ROCK,0xffff)==NULL)
	//	pause_undimmer();					// un fade screen

	f_pause-=2;							// move from pause to thaw state

	/* start music up */
	if ( f_music!=0 )
	{
		psxcd_restart(0);
		while ( psxcd_seeking_for_play() )
			process_sleep(1);

		psxspu_start_cd_fade(2000,cd_vol_tbl[f_music]);
	}

	return;
}

/******************************************************************************
 Function: void play_background_tune(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	start a background tune playing
******************************************************************************/
BYTE round_1_tunes[]=
{
	TRK_SUBWAY,				// 0 -
	TRK_STREET,				// 1 -
	TRK_BANK,				// 2 -
	TRK_ROOF,				// 3 -
	TRK_PIT,				// 4 -
	TRK_BRIDGE,				// 5 -
	TRK_SOUL,				// 6 -
	TRK_ROOF,				// 7 -
	TRK_CHURCH,				// 8 -
	TRK_GRAVE,				// 9 -
	TRK_SUBWAY,				// A -
	TRK_PIT,				// b -
	TRK_BANK,				// c -
	TRK_ROOF,				// d -
	TRK_HIDDEN,				// e -
	TRK_HIDDEN,				// f -
	TRK_HIDDEN,				// 10 -
	TRK_HIDDEN,				// 11 -
};

#if 0
BYTE round_23_tunes[]=
{
	TRK_SUBWAY,				// 0 -
	TRK_STREET,				// 1 -
	TRK_BANK,				// 2 -
	TRK_ROOF,				// 3 -
	TRK_PIT,				// 4 -
	TRK_BRIDGE,				// 5 -
	TRK_SOUL,				// 6 -
	TRK_ROOF,				// 7 -
	TRK_CHURCH,				// 8 -
	TRK_GRAVE,				// 9 -
	TRK_PIT,				// A -
	TRK_PIT,				// A -
	TRK_PIT,				// A -
	TRK_PIT,				// A -
	TRK_PIT,				// A -
	TRK_PIT,				// A -
	TRK_PIT,				// A -
	TRK_PIT,				// A -
};

BYTE finish_tunes[]=
{
	TRK_SUBWAY_FIN,				// 0 -
	TRK_STREET_FIN,				// 1 -
	TRK_BANK_FIN,				// 2 -
	TRK_ROOF_FIN,				// 3 -
	TRK_PIT_FIN,				// 4 -
	TRK_BRIDGE_FIN,				// 5 -
	TRK_SOUL_FIN,				// 6 -
	TRK_ROOF_FIN,				// 7 -
	TRK_CHURCH_FIN,				// 8 -
	TRK_GRAVE_FIN,				// 9 -
	TRK_PIT_FIN,				// A -
	TRK_PIT_FIN,				// B - HIDDEN
	TRK_PIT_FIN,				// C - HIDDEN
	TRK_PIT_FIN,				// D - HIDDEN
	TRK_PIT_FIN,				// E - HIDDEN
	TRK_PIT_FIN,				// F - HIDDEN
	TRK_PIT_FIN,				// 10 -	HIDDEN
	TRK_PIT_FIN,				// 11 -	HIDDEN
	TRK_PIT_FIN,				// 12 -	HIDDEN
	TRK_PIT_FIN,				// 13 -	HIDDEN
	TRK_PIT_FIN					// 14 -	HIDDEN
};
#endif

WORD winner_tunes[]=
{
	TUNE_END_TRAIN,
	TUNE_END_STREET,
	TUNE_END_BANK,
	TUNE_END_ROOF,
	TUNE_END_PIT,
	TUNE_END_BRIDGE,
	TUNE_END_SOUL,
	TUNE_END_ROOF,
	TUNE_END_CHURCH,
	TUNE_END_GRAVE,
	TUNE_END_PIT,
	TUNE_END_TRAIN,
	TUNE_END_PIT,
	TUNE_END_BANK,
	TUNE_END_ROOF,
	TUNE_END_FRENZY,
	TUNE_END_FRENZY,
	TUNE_END_FRENZY,
	TUNE_END_FRENZY,
};

WORD get_background_tune(void)
{
	/* start da tune */
	if ( curback==BKGD_MOCKPIT_MOD )
	{
		return(TRK_PIT);
	}
	else
	{
		if ( curback>BKGD_DESERT_MOD )
		{
			return(TRK_HIDDEN);
		}
		else
		{
			return(round_1_tunes[curback]);
		}
	}
}

void play_background_tune(void)
{
	play_generic_tune(get_background_tune());
	return;
}

void play_finish_tune(void)
{
	//play_generic_tune(finish_tunes[curback]);
}

void play_ending_chord(void)
{
	WORD song;
	/* music on */
	if (f_death!=-1)			 // if death blow = let gloom linger
	{
		/* start da tune */
		if ( sound_background>BKGD_PIT_MOD )
			song=winner_tunes[BKGD_PIT_MOD+1];
		else song=winner_tunes[sound_background];

		if ( f_music )
			wess_seq_trigger(song);
	}

	return;
}

void start_tune(void)
{
	WORD results;

	current_proc->p_comp_flag=999;				// quick hack before airport to prevent music during babality after mercy??????

	/* wait until fight intro stream done */
	do
	{
		results=psxcd_elapsed_sectors();
		process_sleep(1);
	} while ( results<250 );

	if ( winner_status!=3 && f_death==0)
	{
		psxcd_play_at_andloop(get_background_tune(),cd_vol_tbl[f_music],1100,2500,
													get_background_tune(),cd_vol_tbl[f_music],450,3000);
	}
	process_suicide();
}

/*************************************************************************
	BRIEF CASE STUFF
*************************************************************************/
LONG brief_case_codes[]=
{
	0x100100,		//   0 - r_throwing_disabled
	0x020020,		//   1 - r_blocking_disabled
	0x033000,		//   2 - r_p1_handicap
	0x000033,		//   3 - r_p2_handicap
	0x688422,		//   4 - r_dark_fighting
	0x460460,		//   5 - r_switcheroo
	0x987666,		//   6 - r_jackbot_easy
	0x707000,		//   7 - r_p1_supercap
	0x000707,		//   8 - r_p2_supercap
	0x642468,		//   9 - r_space_game
	0x969141,		//   a - r_wf_motaro
	0x033564,		//   b - r_wf_shao
	0x769342,		//   c - r_wf_noob
	0x282282,		//   d - r_nofear_clue
	0x987123,		//   e - r_no_powerbars
	0x788322,		//   f - r_uppercut_recovery
	0x466466,		//  10 - r_unlim_run
	0x985125,		//  11 - r_psycho_kombat
	0x123926,		//  12 - r_intro_message
	0x205205,		//  13 - r_sf_smoke
/*
* ultimate
*/
	0x000000,		//  14 - r_nopower (*)
	0x000000,		//  15 - r_silent_kombat (*)
	0x000000,		//  16 - easy randper
	0x000000,		//  17 - r_curback_subway (*)
	0x000000,		//  18 - r_curback_street (*)
	0x000000,		//  19 - r_curback_rooftop (*)
	0x000000,		//  1a - r_curback_ktower (*)
	0x000000,		//  1b - r_curback_bridge (*)
	0x000000,		//  1c - r_curback_soul (*)
	0x000000,		//  1d - r_curback_bell (*)
	0x000000,		//  1e - r_curback_temple (*)
	0x000000,		//  1f - r_curback_grave (*)
	0x000000,		//  20 - r_curback_pit3 (*)
	0x000000,		//  21 - r_curback_river
	0x000000,		//  22 - r_curback_hell (*)
	0x000000,		//  23 - r_curback_kkave (*)
	0x000000,		//  24 - r_curback_desert (*)
	0x000000,		//  25 - r_curback_subzero (*)
	0x000000,		//  26 - r_curback_noob (*)
	0x000000,		//  27 - revision #
	0x000000,		//  28 - r_throwing_encouraged
	0x000000,		//  29 - r_explode_2on2 (*)
	0x000000,		//  2a - r_explode_2on2 / no throws (*)
	0x000000,		//  2b - r_see_da_tour
	0x000000,		//  2c - r_bs_klue1
	0x000000,		//  2d - r_vinikour_quote
	0x000000,		//  2e - r_ferrier_quote
	0x000000,		//  2f - r_boon_quote

/*
 * SONY CODES
*/
	0x722722,		// 30 - disable combos
	0x321789,		// 31 - allow super jumps
	0x555556,		// 32 - no special moves
	0x390000,		// 33 - p1 inflict 1/2 damage
	0x000390,		// 34 - p2 inflict 1/2 damage
	0x390390,		// 35 - p1 & p2 inflict 1/2 damage
	0x975310,		// 36 - regen powerbar
	0x024689,		// 37 - super endurance
	0x040404,		// 38 - real kombat
	0
};

/******************************************************************************
 Function: void try_kombat_kodes(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	try brief case, search for secret codes
******************************************************************************/
void try_kombat_kodes(void)
{
	LONG ta5=0;						// a5-players attempt to open
	LONG *ta0;

	if (gstate==GS_AMODE)
		return;

	if (round_num!=1) 
	{
		do_round_jsrps();
		return;
	}

	ta5|=(combo_6<<(4*0));
	ta5|=(combo_5<<(4*1));
	ta5|=(combo_4<<(4*2));
	ta5|=(combo_3<<(4*3));
	ta5|=(combo_2<<(4*4));
	ta5|=(combo_1<<(4*5));

	/* walk down the table of codes */
	kode_offset=-1;		// assume failure 
	
	ta0=brief_case_codes;
	current_proc->a7=0;
	/* brief3 */
	while (*ta0!=0)
	{
		if (*ta0==ta5)
		{
			kode_offset=(short)current_proc->a7;
			break;
		}
		((short)current_proc->a7)++;
		ta0++;
	}

	/* briefcase_failure */
	combo_1=combo_2=combo_3=0;
	combo_4=combo_5=combo_6=0;

	do_round_jsrps();

	return;
}

/******************************************************************************
******************************************************************************/
extern char txt_space_game_intro[];

void hidden_game(void);


void hidden_game_load(void)
{
	gstate=GS_SPACE_GAME;
	amode_init_no_score();

	/* clear preload bs */
	p1_preload=0;
	bkgd_preload=0;

	sound_module_load(SMOD_HIDDEN);

	clr_scrn();
	mk3_gradient(GREEN_GRAD);
#if JAP_VERSION==1
	if (start_voice(VOICE_SPACE)==0)
		play_generic_tune(TRK_YOUWIN);		// SCA3(0x45)
#else
	play_generic_tune(TRK_YOUWIN);		// SCA3(0x45)
#endif

	pds_centered_soff(txt_space_game_intro,160,SCY(0x50)-8-10);

	while ( psxcd_seeking_for_play());
	DISPLAY_ON;
	fadein_jsrp(0x40*6);

#if JAP_VERSION==1
	stop_voice(VOICE_SPACE,1);
#endif

	generic_overlay_load(OVL_SINVADE);
	hidden_game();
}

/******************************************************************************
******************************************************************************/
/******************************************************************************
 Function: void player_heap_adj(void)

 By: David Schwartz

 Date: June 1995

 Parameters:  current_proc->pa8 - p1 or p2 object

 Returns: None

 Description:  this code is called to make sure that p1_obj heap and p2_obj heap
				point to two different locations.  This code should be called before
				any fatalites type load happens.  If p1 and p2 point to the same heap there
				is the potential that the dude who is going to die will need some art that is going
				to be loaded over by the dude who is about to commit fatality.  Therefore the caller
				should be the one that is about to have is heap info/art data updated!

				This routine also sets the finish_heap_ptr.  This ptr is the end of the victors heap.  The
				character_texture_load routine will take this ptr - length (4 byte aligned) and load the data
				correctly.  To do this one must insure that the winners victories, stances knockdowns, stumbles, and
				stunnes are near the beginning of the heap!
******************************************************************************/
void player_heap_adj(void)
{
	OBJECT *obj=current_proc->pa8;
	void *freeheap;

	if (p1_obj->oheap==p2_obj->oheap)				// heap ptrs are different, no problem will exists
	{
		freeheap=(obj->oheap==p1_heap)?p2_heap:p1_heap;		// determine which heap not being used
		(BYTE *)finish_heap_ptr=((BYTE *)freeheap)+PLAYER_HEAP_SIZE/2;
	}
	else (BYTE *)finish_heap_ptr=((BYTE *)obj->oheap)+PLAYER_HEAP_SIZE/2;

	return;
}



