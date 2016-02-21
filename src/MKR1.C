/******************************************************************************
 File: mkr1.c

 Date: Mar 1995

 (C) Williams Entertainment

 Mortal Kombat III round intro jsrps
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
#include "mkbkgd.h"
#include "mkani.h"
#include "mkjoy.h"
#include "mkmain.h"
#include "mkpal.h"
#include "mkscore.h"
#include "mktext.h"
#include "mksound.h"
#include "mkr1.h"
#include "mkbuyin.h"
#include "mkbonus.h"
/* Local Externs */
extern void *fl_r1[];

#define REGEN_POWER	2			// # of power ticks to give back during regeneration
#define REGEN_SLEEP 30

/******************************************************************************
 Function: void do_round_jsrps(void)

 By: David Schwartz

 Date: March 1995

 Parameters:  None

 Returns: None

 Description:	round introduction display code
******************************************************************************/
char txt_intro_message[]="THERE IS NO KNOWLEDGE THAT IS NOT POWER";
char txt_psycho_kombat[]="PSYCHO KOMBAT";
char txt_uppercut_recovery[]="FAST UPPERCUT RECOVERY ENABLED";
char txt_unlim_run[]="UNLIMITED RUN";
char txt_no_special_moves[]="SPECIAL MOVES DISABLED";
char txt_p1_half_damage[]="PLAYER 1 INFLICTS HALF DAMAGE";
char txt_p2_half_damage[]="PLAYER 2 INFLICTS HALF DAMAGE";
char txt_p1p2_half_damage[]="INFLICT HALF DAMAGE";
char txt_wf_smoke[]="WINNER OF THIS ROUND BATTLES SMOKE";
char txt_wf_noob[]="WINNER OF THIS ROUND BATTLES NOOB SAIBOT";
char txt_wf_shao[]="WINNER OF THIS ROUND BATTLES SHAO KAHN";
char txt_wf_motaro[]="WINNER OF THIS ROUND BATTLES MOTARO";
char txt_rellim[]="YOU ARE ENTERING THE LAND OF RELLIM";
char txt_no_powerbars[]="NO POWERBARS";
char txt_nofear_clue[]="NO FEAR = EB BUTTON, SKYDIVE, MAX COUNTDOWN";
char txt_jackbot[]="HOLD FLIPPERS DURING CASINO RUN";
char txt_switcheroo[]="QUASI-RANDPER KOMBAT";
char txt_regenerate[]="REGENERATING POWERBARS";
char txt_super_endur[]="SUPER ENDURANCE";
char txt_real_kombat[]="REAL KOMBAT";
char txt_dark_fighting[]="DARK KOMBAT";
char txt_no_blocks[]="BLOCKING DISABLED";
char txt_no_throws[]="THROWING DISABLED";
char txt_super_jumps[]="SUPER RUN JUMPS";
char txt_no_combos[]="COMBO SYSTEM DISABLED";
char txt_p1_handicap[]="PLAYER 1 HALF POWER";
char txt_p1_supercap[]="PLAYER 1 QUARTER POWER";
char txt_p2_handicap[]="PLAYER 2 HALF POWER";
char txt_p2_supercap[]="PLAYER 2 QUARTER POWER";
char txt_boon_quote[]="WHATCHA GUN DO ? - E.B.";
char txt_vinikour_quote[]="DON'T JUMP AT ME - MXV";
char txt_ferrier_quote[]="SKUNKY !! - E.F.";
char txt_bs_klue1[]="RAIN CAN BE FOUND IN THE GRAVEYARD";
char txt_see_da_tour[]="SEE THE MORTAL KOMBAT LIVE TOUR !!";
char txt_curback_subway[]="KOMBAT ZONE: SUBWAY";
char txt_curback_street[]="KOMBAT ZONE: STREET";
char txt_curback_rooftop[]="KOMBAT ZONE: ROOFTOP";
char txt_curback_ktower[]="KOMBAT ZONE: KAHN'S TOWER";
char txt_curback_bridge[]="KOMBAT ZONE: THE BRIDGE";
char txt_curback_soul[]="KOMBAT ZONE: SOUL CHAMBER";
char txt_curback_bell[]="KOMBAT ZONE: BELL TOWER";
char txt_curback_temple[]="KOMBAT ZONE: KOMBAT TEMPLE";
char txt_curback_grave[]="KOMBAT ZONE: THE GRAVEYARD";
char txt_curback_pit3[]="KOMBAT ZONE: THE PIT 3";
char txt_curback_river[]="KOMBAT ZONE: RIVER KOMBAT";
char txt_curback_hell[]="KOMBAT ZONE: SCORPION'S LAIR";
char txt_curback_kkave[]="KOMBAT ZONE: KAHN'S KAVE";
char txt_curback_desert[]="KOMBAT ZONE: JADE'S DESERT";
char txt_curback_subzero[]="KOMBAT ZONE: SCISLAC BUSOREZ";
char txt_curback_noob[]="KOMBAT ZONE: NOOB SAIBOT DORFEN";
char txt_silent_message[]="SILENT KOMBAT";
char txt_p12_nopower[]="UNIKORIV REFERRI: SANS POWER";
char rom_name[]="SONY REV 3.0";
char txt_yes_throws[]="THROWING ENCOURAGED";
char txt_ex2on2[]="EXPLOSIVE KOMBAT";
char txt_explode_no_throw[]="EXPLOSIVE KOMBAT AND THROWING DISABLED";


void do_round_jsrps(void)
{
	if (kode_offset>=0) 
	{
		switch (kode_offset)
		{
			case 0:		// throwing disabled
				f_sans_throws=1;
				msg1(txt_no_throws);
				break;
			case 1:		// blocking disabled
				f_sans_block=1;
				msg1(txt_no_blocks);
				break;
			case 2:		// p1 handicap
				CREATE(PID_FX,p1_hadicap_proc);
				msg1(txt_p1_handicap);
				break;
			case 3:		// p2 handicap
				CREATE(PID_FX,p2_hadicap_proc);
				msg1(txt_p2_handicap);
				break;
			case 4:		// dark fighting
				CREATE(PID_DARK,dark_fight_proc);
				msg1(txt_dark_fighting);
				break;
			case 5:		// switcheroo
			case 22:	// switcheroo
				CREATE(PID_SWITCHEROO,switcheroo_proc);
				msg1(txt_switcheroo);
				break;
			case 6:		// jackbot 
				msg1(txt_jackbot);
				break;
			case 7:		// p1 supercap
				CREATE(PID_FX,p1_supercap_proc);
				msg1(txt_p1_supercap);
				break;
			case 8:		// p2 supercap
				CREATE(PID_FX,p2_supercap_proc);
				msg1(txt_p2_supercap);
				break;
			case 9:		// space game
				CREATE(PID_MASTER,space_game);
				msg1(txt_rellim);
				break;
			
			case 10:	// winner fight motaro
				wf3(txt_wf_motaro,post_wf_motaro);
				break;
			case 11:	// winner fights shao
				wf3(txt_wf_shao,post_wf_shao);
				break;
			case 12:	// winner fight noob
				wf3(txt_wf_noob,post_wf_noob);
				break;
			case 13:	// nofear
				msg1(txt_nofear_clue);
				break;
			case 14:	// no powerbars
				DONT_SHOW_SCORES;
				msg1(txt_no_powerbars);
				break;
			case 15:	// uppercut recover
				f_upcut_rec=1;
				msg1(txt_uppercut_recovery);
				break;
			case 16:	// unlim run
				f_unlim_run=1;
				msg1(txt_unlim_run);
				break;
			case 17:	// psycho kombat
				f_upcut_rec=1;
				f_unlim_run=1;
				f_sans_block=1;

				CREATE(PID_PSYCHO,psycho_score_onoff);
				CREATE(PID_SWITCHEROO,switcheroo_proc);
				CREATE(PID_DARK,dark_fight_proc);
				msg1(txt_psycho_kombat);
				break;
			case 18:	// intro message
				msg1(txt_intro_message);
				break;
			case 19:	// sf smoke
				wf3(txt_wf_smoke,post_wf_smoke);
				break;
			
			case 20:	// nopower
				p1_matchw=p2_matchw=1;
				CREATE(PID_FX,p1_nopower_proc);
				CREATE(PID_FX,p2_nopower_proc);
				msg1(txt_p12_nopower);
				break;
			case 21:	// silent kombat
				set_gameflag(M_GF_NOMUSIC);
				msg1(txt_silent_message);
				break;
			case 23:	// subway
				r_force_curback(txt_curback_subway,BKGD_SUBWAY_MOD);
				break;
			case 24:	// street
				r_force_curback(txt_curback_street,BKGD_STREET_MOD);
				break;
			case 25:	// rooftop
				r_force_curback(txt_curback_rooftop,BKGD_ROOF_MOD);
				break;
			case 26:	// ktower
				r_force_curback(txt_curback_ktower,BKGD_TOWER_MOD);
				break;
			case 27:	// bridge
				r_force_curback(txt_curback_bridge,BKGD_BRIDGE_MOD);
				break;
			case 28:	// soul
				r_force_curback(txt_curback_soul,BKGD_SOUL_MOD);
				break;
			case 29:	// bell
				r_force_curback(txt_curback_bell,BKGD_BELL_MOD);
				break;
			
			case 30:	// temple
				r_force_curback(txt_curback_temple,BKGD_TEMPLE_MOD);
				break;
			case 31:	// grave
				r_force_curback(txt_curback_grave,BKGD_GRAVE_MOD);
				break;
			case 32:	// pit3
				r_force_curback(txt_curback_pit3,BKGD_PIT_MOD);
				break;
			case 33:	// river
				r_force_curback(txt_curback_river,BKGD_WATER_MOD);
				break;
			case 34:	// hell
				r_force_curback(txt_curback_hell,BKGD_HADES_MOD);
				break;
			case 35:	// cave
				r_force_curback(txt_curback_kkave,BKGD_CAVE_MOD);
				break;
			case 36:	// desert
				r_force_curback(txt_curback_desert,BKGD_DESERT_MOD);
				break;
			case 37:	// subzero
				r_force_curback(txt_curback_subzero,BKGD_LOST_MOD);
				break;
			case 38:	// noob
				r_force_curback(txt_curback_noob,BKGD_NOWALL_MOD);
				break;
			case 39:	// rev num
				msg1(rom_name);
				break;
			
			case 40:	// throw encouraged
				set_gameflag(M_GF_THROWYES);
				msg1(txt_yes_throws);
				break;
			case 41:	// explode 2 on 2
				if (mode_of_play!=1) 
				{
					set_gameflag(M_GF_EX2ON2);
					msg1(txt_ex2on2);
				}
				break;
			case 42:	// explode no throw
				if (mode_of_play!=1) 
				{
					f_sans_throws=1;
					set_gameflag(M_GF_EX2ON2);
					msg1(txt_explode_no_throw);
				}
				break;
			case 43:	// see da tour
				msg1(txt_see_da_tour);
				break;
			case 44:	// bs klue1
				msg1(txt_bs_klue1);
				break;
			case 45:	// vinikour quote
				msg1(txt_vinikour_quote);
				break;
			case 46:	// ferrie quote
				msg1(txt_ferrier_quote);
				break;
			case 47:	// boon quote
				msg1(txt_boon_quote);
				break;
			case 48:	// disable combos
				f_no_combos=1;
				msg1(txt_no_combos);
				break;
			case 49:	// allow super jumps
				f_superjumps=1;
				msg1(txt_super_jumps);
				break;

			case 50:	// no special moves
				f_no_special_moves=1;
				msg1(txt_no_special_moves);
				break;
			case 51:	// p1 inflict 1/2 damage
				f_p1_half_damage=1;
				msg1(txt_p1_half_damage);
				break;
			case 52:	// p2 imflict 1/2 damage
				f_p2_half_damage=1;
				msg1(txt_p2_half_damage);
				break;
			case 53:	// p1 & p2 inflict half damage
				f_p1_half_damage=1;
				f_p2_half_damage=1;
				msg1(txt_p1p2_half_damage);
				break;
			case 54:	// regenreate powerbar
				CREATE(PID_REGENERATE,regenerate_proc);
				msg1(txt_regenerate);
				break;
			case 55:	// super endurance
				f_unlim_run=1;
				f_p1_half_damage=1;
				f_p2_half_damage=1;
				CREATE(PID_REGENERATE,regenerate_proc);
				msg1(txt_super_endur);
				break;
			case 56:	// real kombat
				f_no_special_moves=1;
				CREATE(PID_REGENERATE,regenerate_proc);
				msg1(txt_real_kombat);
				break;
		}
	}

	return;
}


/******************************************************************************
 Function: void r_psycho_kombat(void)

 By: David Schwartz

 Date: May 1995

 Parameters:  None

 Returns: None

 Description:	pyscho combat
******************************************************************************/
void psycho_score_onoff(void)
{
	do
	{
		DONT_SHOW_SCORES;
		process_sleep(pso3());
		SHOW_SCORES;
		process_sleep(pso3());
	}
	while(1);
}

WORD pso3(void)
{
	if (winner_status!=0)
	{
		SHOW_SCORES;
		process_suicide();
	}

	return(randu_minimum(0x20,0x20));
}

/******************************************************************************
 Function: void r_sf_????(void)

 By: David Schwartz

 Date: May 1995

 Parameters:  None

 Returns: None

 Description:  fight certain characters
******************************************************************************/
void wf3(char *pa8,FUNC pa14)
{
	if (f_play3==0)
		return;

	kode_offset=0;
	clear_combo_ram();

	msg1(pa8);

	play3_pa14=pa14;			// set return addr after fight

	return;
}

/******************************************************************************
 Function: void p1_hadicap_proc(void)

 By: David Schwartz

 Date: March 1995

 Parameters:  None

 Returns: None

 Description:	p1 handicap
******************************************************************************/
void p1_supercap_proc(void)
{
	hand2(0,FULL_STRENGTH/4);
}

void p1_hadicap_proc(void)
{
	hand2(0,FULL_STRENGTH/2);
}

void p2_supercap_proc(void)
{
	hand2(1,FULL_STRENGTH/4);
}

void p2_hadicap_proc(void)
{
	hand2(1,FULL_STRENGTH/2);
}

void p1_nopower_proc(void) 
{
	hand2(0,1);
}

void p2_nopower_proc(void) 
{
	hand2(1,1);
}

/******************************************************************************
 Function: void hand2(WORD pa11,WORD pa10)

 By: David Schwartz

 Date: March 1995

 Parameters:  pa11 - 0=pwr bar0 1=pwr bar1
			  pa10 - strength to set

 Returns: None

 Description:	deal with handicap
******************************************************************************/
void hand2(WORD pa11,WORD pa10)
{
	PROCESS *ptemp;

	ptemp=wait_for_charge();

	if (pa11==0)
	{
		ptemp->a10=pa10;
	}
	else
	{
		ptemp->a11=pa10;
	}

	process_suicide();
}

PROCESS *wait_for_charge(void)
{
	PROCESS *ptemp;
	do
	{
		process_sleep(5);
		ptemp=process_exist(PID_FX_CHARGE,-1);
	}
	while(ptemp==NULL);
	return(ptemp);
}

/******************************************************************************
 Function: void msg1(char *msg)

 By: David Schwartz

 Date: March 1995

 Parameters:  None

 Returns: None

 Description:	print round message
******************************************************************************/
void msg1(char *msg)
{
	OBJECT *ta0;

	p7_centered(msg,SCX(0xc8),SCY(0xf0)+8);

	ta0=(OBJECT *)&objlst2;

	/* msg3 */
	while((ta0=ta0->olink)!=NULL)
	{
		ta0->oid=OID_R1;				// change all text id --> oid_r1
		//delpal(ta0);		// mod arcade to allow blinking palettes
		//ta0->opal=get_fore_pal(bpal_white_P);
	}

	CREATE(PID_R1,msg_round1);

	return;
}

/******************************************************************************
 Function: void msg_round1(void)

 By: David Schwartz

 Date: March 1995

 Parameters:  None

 Returns: None

 Description:	flash letters for round intro
******************************************************************************/
void flash_r1(int t)
{
	OBJECT *obj;
	short pos;

	pos=(t==1)?600:-600;

	obj=(OBJECT *)&objlst2;
	while ( obj->olink!=NULL )
	{
		obj=obj->olink;
		if ( obj->oid==OID_R1 )
			obj->oypos.u.intpos+=pos;
	}

	return;
}

void msg_round1(void)
{
	WORD t;

	flash_r1(1);				// premove message off screen

	t=0x50/8;
	do
	{
		flash_r1(0);					// ON
		process_sleep(6);
		flash_r1(1);
		process_sleep(6);			// OFF
	} while ( --t>0 );

	//pa0=CREATE(PID_R1,boonpal_stuff);
	//pa0->a11=(ADDRESS)fl_r1;
	//process_sleep(0x50);

	dallobj(OID_R1);
	dallprc(PID_R1);

	process_suicide();
}

/******************************************************************************
 Function: void switcheroo_proc(void)

 By: David Schwartz

 Date: March 1995

 Parameters:  None

 Returns: None

 Description:	rander fighter change process
******************************************************************************/
void switcheroo_proc(void)
{
	do
	{
		process_sleep(8);
	}
	while(f_start==0);			// wait until we are fighting

	current_proc->pdata.p_store1=8;
	current_proc->pdata.p_store2=8;

	do
	{
		if (p1_bar!=0 && p2_bar!=0)		// zero power = dont switch
		{
			if (f_thatsall!=0)
				process_suicide();				// stop upon these events

			process_sleep(2);

			current_proc->pa8=p1_obj;
			current_proc->a5=(ADDRESS)p1_proc;
			switcheroo_check(&(current_proc->pdata.p_store1));

			current_proc->pa8=p2_obj;
			current_proc->a5=(ADDRESS)p2_proc;
			switcheroo_check(&(current_proc->pdata.p_store2));
		}
	}
	while(1);
}

void switcheroo_check(ADDRESS *pa11)
{
	PROCESS *a13=current_proc;
	long a0;

	a0=*pa11;

	if (a0!=0)
	{
		a0-=2;
		if (a0<0)
			a0=0;

		/* sw2 */
		*pa11=a0;

		if (a0!=0)
			return;
	}

	/* sw3 */
	current_proc=(PROCESS *)current_proc->a5;
	if (am_i_airborn(current_proc->pa8)==SYSTEM_CARRY_SET)
	{
		current_proc=a13;
		return;				// no switch in air
	}

	if (current_proc->pdata.p_flags & PM_SPECIAL)
	{
		current_proc=a13;
		return;				// no switch during special
	}

	if (current_proc->pdata.p_flags & PM_REACTING)
	{
		current_proc=a13;
		return;				// no switch during reacting
	}

	fastxfer(current_proc,switcheroo_wake);

	*pa11=randu_minimum(0x40,0x40*4);

	current_proc=a13;
	return;
}

/******************************************************************************
 Function: void regenerate_proc(void)

 By: David Schwartz

 Date: March 1995

 Parameters:  None

 Returns: None

 Description:	keep power bars growing back to full health
******************************************************************************/
void regenerate_proc(void)
{
	do
	{
		process_sleep(1);
	}
	while(f_start==0);				// wait till fights starts

	while (p1_bar!=0 && p2_bar!=0 && f_thatsall==0)
	{
		p1_bar+=REGEN_POWER;
		if (p1_bar>FULL_STRENGTH)
			p1_bar=FULL_STRENGTH;

		p2_bar+=REGEN_POWER;
		if (p2_bar>FULL_STRENGTH)
			p2_bar=FULL_STRENGTH;

		process_sleep(REGEN_SLEEP);				// give power at some time intervals
	}

	process_suicide();
}

/******************************************************************************
 Function: void dark_fight_proc(void)

 By: David Schwartz

 Date: March 1995

 Parameters:  None

 Returns: None

 Description:	fight in the dark
******************************************************************************/
void dark_fight_proc(void)
{
	do
	{
		process_sleep(1);
	}
	while(f_start==0);				// wait till fights starts

	f_dark=0;						// lights out
	(void *)current_proc->a10=dark_dlist=dlists;				// grab current dlists
	process_sleep(8);

	/* dark2 */
	do
	{
		lights_out();

		/* dark3 */
		do
		{
			current_proc->a0=1;
			dark_sleep();
		}
		while(f_dark==0);			// someone hit lights on

		/* hit = lights on !! */
		lights_on();

		/* dark4 */
		do
		{
			if (f_dark==0)
				break;			// already slept --> go dark
			current_proc->a0=f_dark;
			f_dark=0;
			dark_sleep();
			lights_out();
		}
		while(1);
	}
	while(1);
}

void dark_sleep(void)
{
	/* darks3 */
	do
	{
		process_sleep(1);
		if (p1_bar==0 || p2_bar==0 || f_thatsall!=0)
		{
			/* dark_abort */
			lights_on();
			process_suicide();
		}
	}
	while(--current_proc->a0>0);

	return;
}

void lights_out(void)
{
	call_every_tick=dark_velocities;
	f_dark=0;
	dlists=NULL;
	return;
}

void lights_on(void)
{
	call_every_tick=qwerty;
	dlists=(void *)current_proc->a10;
	return;
}

void dark_velocities(void)
{
	ADDRESS *dptr;
	OBJECT *obj;

	dptr=(ADDRESS *)dark_dlist;
	dptr-=2;

	/* darkv2 */
	do
	{
		dptr+=2;
		obj=(OBJECT *)(*((ADDRESS **)dptr));

		if (obj==NULL)
		{
			qwerty();			// all velocites update --> qwerty
			return;
		}

		/* -SONY- SONY CODE SPACE AT 0x80000000 */
#if SONY_PSX
#endif

		if ((ADDRESS)obj!=0xffffffff)
		{
			/* darkv3 */
			while ((obj=obj->olink)!=NULL)
			{
				obj->oxpos.pos+=obj->oxvel.pos;
				obj->oypos.pos+=obj->oyvel.pos;
			}
		}
		else
		{
			if (*((FUNC*)(dptr+1))==use_next_y)
				dptr++;						// special case routine
		}
	}
	while(1);
}

void lights_on_hit(void)
{
	f_dark=0x18;				// flag to darkman
	return;
}

void lights_on_slam(void)
{
	f_dark=0x38;				// flag to darkman
	return;
}

/******************************************************************************
 Function: void space_game(void)

 By: David Schwartz

 Date: March 1995

 Parameters:  None

 Returns: None

 Description:	play the space game
******************************************************************************/
void space_game(void)
{
	process_sleep(0x40*2);
	hidden_game_load();
	murder_myoinit_score();
	CREATE(PID_AMODE,game_over);
	f_nosound=0;
	process_suicide();
}

/******************************************************************************
 Function: void post_wf_????(void)

 By: David Schwartz

 Date: March 1995

 Parameters:  None

 Returns: None

 Description:	play the space game
******************************************************************************/
void post_wf_smoke(void)
{
	postw2(FT_OLDSMOKE);
}

void post_wf_noob(void)
{
	postw2(FT_NOOB);
}

void post_wf_shao(void)
{
	postw2(FT_SK);
}

void post_wf_motaro(void)
{
	postw2(FT_MOTARO);
}

void postw2(WORD pa0)
{
	results_of_round(round_results);
	f_thatsall=1;					// flag: enough fighting already
	bonus_count();					// print winner is

	if (p1_bar==0)
	{
		/* p1 lost, out of game */
		p1_state=0;
		p1_char=p1_name_char=pa0;
	}
	else
	{
		/* p2 lost, out of game */
		p2_state=0;
		p2_char=p2_name_char=pa0;
	}

	p1_matchw=p2_matchw=0;

	murder_myoinit_score();
	p1_heap_char=p2_heap_char=0xffff;
	snd_module_status=0xffff;

	pa0=diff;
	p1_matchw=p2_matchw=0;
	special_game_init();
	round_results=play_1_round();
	results_of_round(round_results);
	diff=pa0;

	f_thatsall=1;
	bonus_count();

	special_game_init();
	advance_curback();

	if (p1_state!=PS_ACTIVE)
		p1_state=PS_BUYIN;
	else p2_state=PS_BUYIN;

	do_buyin();

	advance_curback();
	stack_jump(game_loop);
}

void special_game_init(void)
{
	WORD a1,a2;

	a1=p1_state;
	a2=p2_state;
	game_init();
	p1_state=a1;
	p2_state=a2;

	return;
}

void r_force_curback(char *pa8, WORD pa11) 
{
	curback=pa11;
	msg1(pa8);
}