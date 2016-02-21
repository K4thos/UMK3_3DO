/******************************************************************************
 File: mkbonus.c

 Date: Dec 1994

 (C) Williams Entertainment

 Mortal Kombat III bonus Routines
******************************************************************************/

#include "mk3snd.h"
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
#include "mkmain.h"
#include "mkblood.h"
#include "mkbonus.h"
#include "mkfx.h"
#include "mkfile.h"
#include "mktext.h"
#include "mksound.h"
#include "psxspu.h"
#include "mkfade.h"

/* local extern info */
extern char txt_tie[];
extern char txt_single_flawless[];
/******************************************************************************
 Function: void bonus_count(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	After fight bonus screen display
******************************************************************************/
char txt_tie[]="DRAW";
char txt_single_flawless[]="FLAWLESS VICTORY";

BYTE fatality_animations[][2]=
{
	{FX_FATAL_DRIP,FX_FATAL_LOAD},
	{FX_FATAL_DRIP,FX_FATAL_LOAD},
	{FX_ANIMALITY,FX_ANIMAL_LOAD},
	{FX_FRIENDSHIP,FX_FRIEND_LOAD},
	{FX_BABALITY,FX_BABY_LOAD},
	{FX_FATAL_DRIP,FX_FATAL_LOAD},
	{FX_FATAL_DRIP,FX_FATAL_LOAD}
};

short fatal_wait[]={225,225,425,450,520,225,225};

WORD winner_wait[]=
{
	2+30*75,			// kano
	2+130*75,			// sonya
	2+20*75,			// jax
	2+70*75,			// ind
	2+150*75,			// subz
	2+140*75,			// swat
	2+110*75,			// sindel
	2+80*75,			// robo1
	2+10*75,			// robo2
	2+40*75,			// lao
	2+0*75,			// kabal
	2+100*75,			// sheeva
	2+90*75,			// shang
	2+50*75,			// liu kang
	2+120*75,			// smoke

	2+120*75,			// kitana, //-DHS fill in with correct values
	2+120*75,			// jade
	2+120*75,			// mileena

	2+120*75,			// scorpion
	2+120*75,			// reptile
	2+120*75,			// ermac
	2+120*75,			// oldsz
	2+120*75,			// oldsmoke

	2+0*75,			// noob		//its offical you suck phrase
	2+60*75,			// motaro
	2+0*75,			// shao
};

void bonus_count(void)
{
	WORD *pa4;
	WORD *pa5;
	WORD *pbar;
	ADDRESS ta11;
	WORD count;
	PROCESS *ptemp;

	gstate=GS_BONUS;					// game state = bonus

	kill_lingerings();					// kill fx/dangers/etc...

	while ( f_no_bonus)				// wait until fatality code say go
		process_sleep(1);

//********************************************
	ptemp=process_exist(PID_MERCY,0xffff);
	if ( ptemp!=NULL  )
	{
		if ( ptemp->p_comp_flag==999 )
		{
			process_kill(ptemp);
		}
	}
//********************************************


	/* load after fight sound effects */
	psxspu_start_cd_fade(2*FADE_TIME,0);

	if ( p1_bar!=p2_bar )
	{
		p15_centered(get_winner_text(),SCX(0xc8),SCY(0x38)+8);
		change_oid_list(&objlst2,OID_TEXT,OID_FX);		// change oid of PLAYER WINS TEXT
	}

	process_sleep(0x10);

	if ( f_death==0 )
		play_ending_chord();

	count=0;
	while(psxspu_get_cd_fade_status() && (++count)<4*55)		// wait until fade done to prevent popping
		process_sleep(1);

	process_sleep(1);
	if ( f_death!=0 )
	{
		current_proc->a0=(f_final_act==0)? 0:f_final_act-1;
		current_proc->pa8=(winner_status==1)?p1_obj:p2_obj;
		/* load graphic data */

		psxspu_set_cd_vol(0); 					// make sure volume is zero so no delay can happen
		psxcd_stop();

		count=0;
		while ( psxcd_waiting_for_pause() && ++count<2*55 )
		{
			process_sleep(1);
		}

		ta11=(ADDRESS)special_fx_load(fatality_animations[current_proc->a0][1]);
	}

	if (p1_bar!=p2_bar)
	{
//********************************************************************
//  ANNOUNCE THE WINNER
//********************************************************************
		if (get_winner_ochar()==FT_SK)
		{
			current_proc->pa8=(p1_char==FT_SK)?p1_obj:p2_obj;		// make sure point to an object for panning driver call
			rsnd_sk_bonus_win();
			process_sleep(0x28);
		}
		else
		{
			 if ( (p1_bar==FULL_STRENGTH && p1_state==PS_ACTIVE )|| (p2_bar==FULL_STRENGTH && p2_state==PS_ACTIVE))
			 {
					/* winner and flawless */
					psxcd_play_at(30,(f_no_sfx)?0:cd_vol_tbl[3],winner_wait[get_winner_ochar()]);
			 }
			 else
			 {
					/* winner */
					psxcd_play_at(31,(f_no_sfx)?0:cd_vol_tbl[3],winner_wait[get_winner_ochar()]);
			 }
		}

//********************************************************************
		/* flawless check */

		/* bonus3 */
		process_sleep(2);

		wait_for_bonusv(170+winner_wait[get_winner_ochar()]);							// make sure name is said completely

		if ((winner_status==1 && p1_state==PS_ACTIVE) ||
		   (winner_status==2 && p2_state==PS_ACTIVE))
		{
			/* flawless victory ? */
			if (winner_status==1)
			{
				pbar=&p1_bar;
				pa4=&p1_perfect;			// winner perfect
				pa5=&p2_perfect;			// loser perfect
			}
			else
			{
				pbar=&p2_bar;
				pa4=&p2_perfect;			// winner perfect
				pa5=&p1_perfect;			// loser perfect
			}

			/* bonus5 */
			*pa5=~0;						// loser = no chance for double perfect

			if (*pbar==FULL_STRENGTH)		// check for perfection
			{
				/* win5 */
				*pa4=1;			// flag: perfect round
				(char *)current_proc->pa8=txt_single_flawless;

				/* win51 */
				pds_centered((char *)current_proc->pa8,SCX(0xc8),SCY(0x60)+16);

//********************************************************************
//  ANNOUNCE FLAWLESS
//********************************************************************
//				play_generic_stream(TRK_FLAWLESS);			// tsound(0x61)
//#if _CD_VERSION_ == 1
//					while ( psxcd_elapsed_sectors()<20)
//						process_sleep(1);								// wait until sound has started
//#endif
//********************************************************************
//  ANNOUNCE FLAWLESS
//********************************************************************
				wait_for_bonusv(340+winner_wait[get_winner_ochar()]);							// make sure name is said completely
  			psxspu_start_cd_fade(FADE_TIME,0);					// kill volume so next tune happens quicker
			}
		}
		else
		{
  		psxspu_start_cd_fade(FADE_TIME/2,0);					// kill volume so next tune happens quicker
			/* stay awhile for a computer victory, NEED for streams */
			process_sleep(0x28);
		}

		/* win6 , fatality ? */
		if (f_death!=0)
		{
			dallobj(OID_TEXT);

			current_proc->a0=(f_final_act==0)? 0:f_final_act-1;
			current_proc->pa8=(winner_status==1)?p1_obj:p2_obj;

			/* load graphic data */
//			current_proc->a11=(ADDRESS)special_fx_load(fatality_animations[current_proc->a0][1]);

			/* display graphic data */
			current_proc->a11=ta11;
			create_fx(fatality_animations[current_proc->a0][0]);		// fatality drip fx

			process_sleep(2);
			wait_for_bonusv(fatal_wait[current_proc->a0]);
  		psxspu_start_cd_fade(FADE_TIME,0);					// kill volume so next tune happens quicker
		}
	}
	else
	{
		/* bonus_count_draw */
		p15_centered(txt_tie,SCX(0xc8),SCY(0x38)+16);
		process_sleep(0x40);
	}

	/* bonus exit */
	if (p1_matchw!=2 && p2_matchw!=2)
	{
		fast_fadeout_jsrp(0x20);
		freeze_2_pages();
		MURDER;

		/* amode display reset */
		f_doscore=0;
		clr_scrn();
		f_auto_erase=1;
		murder_myoinit();
		clr_scrn();
		/* amode display reset */
	}
	else
	{
		/* bonus7 */
		process_sleep(0x25);
	}

	send_code_a3(0);

	irqskye=0;

	return;
}

/******************************************************************************
 Function: void kill_lingerings(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	kill lingering objects and processes that aren't needed
******************************************************************************/
void kill_lingerings(void)
{
	dallobj(PID_DANGER1);
	dallprc(PID_DANGER1);

	dallobj(PID_DANGER2);
	dallprc(PID_DANGER2);

	dallprc(PID_FX);
	dallobj(OID_FX);
	dallobj(OID_TEXT);

	return;
}

/******************************************************************************
 Function: inline char *get_winner_text(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	returns ptr to winners text
******************************************************************************/
char txt_kano_wins[]="KANO WINS";
char txt_sonya_wins[]="SONYA WINS";
char txt_jax_wins[]="JAX WINS";
char txt_sal_wins[]="NIGHTWOLF WINS";
char txt_sz_wins[]="SUB-ZERO WINS";
char txt_swat_wins[]="STRYKER WINS";
char txt_lia_wins[]="SINDEL WINS";
char txt_robo1_wins[]="SEKTOR WINS";
char txt_robo2_wins[]="CYRAX WINS";
char txt_lao_wins[]="KUNG LAO WINS";
char txt_tusk_name_wins[]="KABAL WINS";
char txt_sg_name_wins[]="SHEEVA WINS";
char txt_st_name_wins[]="SHANG TSUNG WINS";
char txt_lk_name_wins[]="LIU KANG WINS";
char txt_smoke_wins[]="SMOKE WINS";
char txt_kitana_wins[]="KITANA WINS";
char txt_jade_wins[]="JADE WINS";
char txt_mileena_wins[]="MILEENA WINS";
char txt_scorpion_wins[]="SCORPION WINS";
char txt_rain_wins[]="RAIN WINS";
char txt_reptile_wins[]="REPTILE WINS";
char txt_ermac_wins[]="ERMAC WINS";
char txt_motaro_wins[]="MOTARO WINS";
char txt_shao_kahn_wins[]="SHAO KAHN WINS";
char txt_noob_wins[]="NOOB SAIBOT WINS";
char txt_boon_w[]="BOON WINS";
char txt_jc_t[]="JOHHNY CAGE TRANSFORMATION ACTIVATED";
char txt_hyperf[]="HYPER FATAILITY";

char *ochar_winner_text[]=
{
	txt_kano_wins,
	txt_sonya_wins,
	txt_jax_wins,
	txt_sal_wins,
	txt_sz_wins,
	txt_swat_wins,
	txt_lia_wins,
	txt_robo1_wins,
	txt_robo2_wins,
	txt_lao_wins,
	txt_tusk_name_wins,
	txt_sg_name_wins,
	txt_st_name_wins,
	txt_lk_name_wins,
	txt_smoke_wins,
	txt_kitana_wins,
	txt_jade_wins,
	txt_mileena_wins,
	txt_scorpion_wins,
	txt_reptile_wins,
	txt_ermac_wins,
	txt_sz_wins,
	txt_smoke_wins,
	txt_noob_wins,
	txt_motaro_wins,
	txt_motaro_wins,
};

char txt_team1_wins[]="TEAM 1 WINS";
char txt_team2_wins[]="TEAM 2 WINS";

inline char *get_winner_text(void)
{
	if (mode_of_play==1 )
		return((winner_status==1)?txt_team1_wins:txt_team2_wins);

	return(ochar_winner_text[get_winner_ochar()]);
}

WORD get_winner_ochar(void)
{
	PROCESS *pa1;
	OBJECT *pa0;

	switch (mode_of_play)
	{
		case 0:			// gwo_1_on_1
		case 2:			// gwo_tournament
			if (p1_obj==NULL)
				return((winner_status==1)?p1_char:p2_char);

			if (winner_status==1)
			{
				pa0=p1_obj;
				pa1=p1_proc;
			}
			else
			{
				pa0=p2_obj;
				pa1=p2_proc;
			}

			if (pa1->pdata.p_flags & PM_SHANG)
				return(FT_ST);
			else return(pa0->ochar);

			break;
		case 1:			// gwo_2_on_2
			return((winner_status==1)?p1_obj->ochar:p2_obj->ochar);
			break;
	}

	return((winner_status==1)?p1_char:p2_char);		// default
}

/******************************************************************************
 Function: void wait_for_bonusv(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	wait for shao kahn voice to finish
******************************************************************************/
void wait_for_bonusv(WORD sector)
{
	current_proc->a10=0x44*5;
	/* wfb3 */
	do
	{
		if ( psxcd_elapsed_sectors()<=sector)
			process_sleep(1);
		else break;
	}
	while(--current_proc->a10>0);

	return;
}
