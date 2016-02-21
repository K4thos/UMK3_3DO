/******************************************************************************
 File: mkcombo.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Combo Functions
******************************************************************************/

#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkobj.h"
#include "mkos.h"
#include "mkgame.h"
#include "mkinit.h"
#include "mkutil.h"
#include "mkani.h"
#include "mkjoy.h"
#include "mkpal.h"
#include "mkscore.h"
#include "mkstat.h"
#include "moves.h"
#include "mkcombo.h"
#include "mkguys.h"
#include "mkreact.h"
#include "valloc.h"

/******************************************************************************/
/* External Local References */
/******************************************************************************/

/* located in mkcombo.s */
extern WORD scom_axe_up[];
/******************************************************************************/

COMBO *ochar_elbow_combos[]=
{
	ct_kano_elbow2,
	ct_sonya_elbow2,
	ct_jax_elbow2,
	ct_ind_elbow2,
	ct_sz_elbow2,

	ct_swat_elbow2,
	ct_lia_elbow2,
	ct_sektor_elbow2,
	ct_cyrax_elbow2,
	ct_lao_elbow2,

	ct_tusk_elbow2,
	ct_sg_elbow2,
	ct_st_elbow2,
	ct_lk_elbow2,
	ct_smoke_elbow2,

	ct_kitana_elbow2,
	ct_jade_elbow2,
	ct_mileena_elbow2,

	ct_scorp_elbow2,
	ct_rep_elbow2,
	ct_ermac_elbow2,
	ct_osz_elbow2,
	ct_scorp_elbow2,
	ct_kano_elbow2
};


COMBO *ochar_knee_combos[]=
{
	ct_kano_knee2,	// 0
	ct_sonya_knee2,	// 1
	ct_jax_knee2,	// 3
	ct_ind_knee2,	// 2
	ct_sz_knee2,	// 4

	ct_swat_knee2,	// 5
	ct_lia_knee,	// 6
	ct_sektor_knee2,	// 7
	ct_cyrax_knee2,	// 8
	ct_lao_knee2,	// 9

	ct_tusk_knee2,	// 10
	ct_sg_knee2,	// 11
	ct_st_knee2,	// 12
	ct_lk_knee2,	// 13
	ct_smoke_knee2,	// 14

	ct_kitana_knee2,	  // 15
	ct_jade_knee2,	  // 16
	ct_mileena_knee2,  // 17

	ct_scorp_knee2,	     // 18
	ct_rep_knee2, 	     // 19
	ct_ermac_knee2,   	// 20
	ct_osz_knee2,    	// 21
	ct_kano_knee2,   	// 22
	ct_kano_knee2,   	// 23
};

/******************************************************************************
 Function: void do_knee(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: 	None

 Returns: None

 Description:	perform a knee kick
******************************************************************************/
void do_knee(void)
{
	WORD result;

	OBJECT *obj=current_proc->pa8;

	init_special(obj);

	result=striker(SI_STRIKER_NORMAL,1,ACT_KNEE,ANIM_KNEE,1,0xe);

	if (ochar_knee_combos[obj->ochar] != NULL && (!f_no_combos))
		process_combo_table(ochar_knee_combos[obj->ochar]);

	/* knee4 */
	current_proc->pdata.p_action=ACT_KNEE_SD;

	if (result==SYSTEM_CARRY_SET)
		process_sleep(15);

	retract_strike(6);

	return;
}

void knee_elbow_blocked(WORD pa0)
{
	process_sleep(0x10);
	retract_strike(pa0);
	return;
}

/******************************************************************************
 Function: void do_elbow(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters: None

 Returns: None

 Description:	perform an elbow attack
******************************************************************************/
BYTE ochar_elbow_animations[]=
{
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,

	0x10,
	0x10,
	0x10,
	0x10,
	0x10,

	0x10,
	0x10,
	0x10,
	0x10,
	0x10,

	0x10,
	0x10,
	0x80|0x13,	// mileena

	0x10,	// scorpion
	0x80|0x00,	// reptile
	0x80|0x03,	// ermac
	0x80|0x03,	// old subzero
	0x10,
	0x10,	
};

void do_elbow(void)
{

	OBJECT *obj=current_proc->pa8;

	init_special(obj);

	if (ochar_elbow_animations[obj->ochar] & 0x80) 
		current_proc->pa9=get_char_ani(ANIM_TABLE2,ochar_elbow_animations[obj->ochar] & 0x7f);
	else current_proc->pa9=get_char_ani(ANIM_TABLE1,ochar_elbow_animations[obj->ochar] & 0x7f);

	if (striker(SI_STRIKER_NORMAL,1,ACT_ELBOW,0xffff,5,0xf)==SYSTEM_CARRY_SET)
	{
		if (ochar_elbow_combos[obj->ochar]!=NULL && (!f_no_combos))
		{
			process_combo_table(ochar_elbow_combos[obj->ochar]);
		}
		else
		{
			/* elbow4 */
			current_proc->pdata.p_action=ACT_ELBOW_SD;
			process_sleep(10);
		}
	}

	/* elbow5 */
	retract_strike_act(ACT_ELBOW_SD,3);

	return;
}

/******************************************************************************
 Function: void process_combo_table(COMBO *ctbl)

 By: David Schwartz

 Date: Feb 1995

 Parameters: 	None

 Returns: None

 Description:	process a combo table and perform if correct
******************************************************************************/
void process_combo_table(COMBO *ctbl)
{
	WORD results;

	current_proc->a10=0;			// previouis button offset
	current_proc->pdata.p_action=ACT_COMBO;			// define my action, combo mode

	clear_combo_butn();

COMB0:	/* comb0 */
	current_proc->a0=ctbl->c_time & 0xff;		// time to button/anim speed, mask off "last option" bits

	if (am_i_joy()==SYSTEM_CARRY_CLR)
	{
		process_sleep(4);

		if (diff<2)
			return;				// easy == no cpu combo's
	}
	else
	{
		/* combj */
		current_proc->pdata.p_anicount=current_proc->a0;
		process_sleep(3);

		current_proc->pdata.p_anicount-=3;

		/* comb1 */
		do
		{
			process_sleep(1);

			if ((results=combo_scan_a11(&ctbl))==SYSTEM_CARRY_SET)
				goto COMB8;
		}
		while(--current_proc->pdata.p_anicount>0);

		if (results==SYSTEM_CARRY_SET)
			goto COMB8;					// !! SUCCESS

		goto COMBO_2_LATE;
	}

	/* success -> do next animation */
COMB8:
	if (is_he_airborn()==SYSTEM_CARRY_CLR)
		stop_him(current_proc->pdata.p_otherguy);		// he is airborn dont stop him

	/* comb2 */
	clear_combo_butn();

	current_proc->a10=current_proc->a14;

	if (ctbl->c_attack==CB_JUMP_ADDRESS)
		(ctbl->c_next)();

	/* comba */
	a9_combo_ani(ctbl->c_attack);

	mframew((ctbl->c_speed)>>8);			// anim speed

	if (strike_check(combo_strike_table[(ctbl->c_stk)>>8],0)==SYSTEM_CARRY_SET)
	{
		zero_turbo_bar();		// avoid sprint abuse

		(COMBO *)current_proc->a7=ctbl;

#if SONY_PSX
		/* -SONY- conv note code spcae is 0x8000000 or greater */
#endif
		if ((LONG)ctbl->c_next >= 0x80000000)
		{
			ctbl=(COMBO *)(ctbl->c_next);
			goto COMB0;					// neg = process it
		}

		process_sleep((LONG)(ctbl->c_next));
		goto COMB9;
	}
	else
	{
		/* combo_miss */
		process_sleep(0x10);
	}

COMBO_2_LATE:
	process_sleep(5);

COMBO_EXIT:
	a9_combo_ani(ctbl->c_retr);

COMB9:
	mframew(4);

	reaction_exit(current_proc->pa8);
}

FUNCW requirement_table[]=
{
	combo_is_stick_away,		// 1
	check_axe_up_combo,	// 2
	combo_is_stick_toward,	// 3
	combo_is_stick_down,		// 4
	set_his_half_damage,
	combo_is_stick_up,
	combo_q_no,
	set_his_quarter_damage
};

/******************************************************************************
 Function: WORD combo_scan_a11(COMBO **ctbl)

 By: David Schwartz

 Date: Feb 1995

 Parameters: ctbl - (a11) ptr to a ptr combo table

 Returns: SYSTEM_CARRY_SET - combo match
		  SYSTEM_CARRY_CLR - no combo match

 Description:	scan a combo table and detrmine if valid combo started
******************************************************************************/
WORD combo_scan_a11(COMBO **ctbl)
{
	COMBO *ta11=*ctbl;
	WORD *cbutton;

	/* cscan1 */
	do
	{
		current_proc->a14=ta11->c_speed & 0x000f;		// this will be out "LAST" button

		cbutton=*(last_switch_ram+(current_proc->a14*2)+1);	// combo ram

		if (p2_obj==current_proc->pa8)
			cbutton++;

		/* comb5 */
		if (*cbutton!=0) 		// button presses since last sleep, combo time
		{
			/* make sure no other buttons pressed */
			current_proc->a2=0;
			current_proc->a1=(p1_obj==current_proc->pa8)?0:1;		// index into combo time ram

			/* loop unroll from arcade */
			if (c_hp[current_proc->a1] && current_proc->a10!=0)
				current_proc->a2++;
			if (c_lp[current_proc->a1] && current_proc->a10!=1)
				current_proc->a2++;
			if (c_block[current_proc->a1] && current_proc->a10!=2)
				current_proc->a2++;
			if (c_hk[current_proc->a1] && current_proc->a10!=3)
				current_proc->a2++;
			if (c_lk[current_proc->a1] && current_proc->a10!=4)
				current_proc->a2++;

			/* comb7, after loop */
			if (current_proc->a2>1)
				goto COMBOF;

			current_proc->a0=ta11->c_req & 0xff;

			if (current_proc->a0!=0)
			{
				current_proc->a0--;

				if (requirement_table[current_proc->a0]()==SYSTEM_CARRY_CLR)		// test requirement
					goto COMBOF;
			}

			/* cscan8 */
			*ctbl=ta11;
			return(SYSTEM_CARRY_SET);
		}

COMBOF:
		if ((ta11->c_time & M_LAST_OPTION))
			break;
		ta11++;
	}
	while(1);

	/* cscan9 */
	return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: inline void a9_combo_ani(WORD pa9)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa9 - anim index & offset

 Returns: None

 Description:	combo anim setup
******************************************************************************/
inline void a9_combo_ani(WORD pa9)
{
	WORD t_a9;

	t_a9=pa9>>8;
	pa9&=0x00ff;

	if (t_a9 & 0x80)
		find_ani2_part_a14(t_a9 & 0x7f,pa9);
	else find_ani_part_a14(t_a9,pa9);

	return;
}

/******************************************************************************
 Function: void clear_combo_butn(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	combo ram clear
******************************************************************************/
void clear_combo_butn(void)
{
	current_proc->a0=(p1_obj==current_proc->pa8)?0:1;		// index into combo time ram

	/* loop unroll from arcade */
	c_hp[current_proc->a0]=0;
	c_lp[current_proc->a0]=0;
	c_block[current_proc->a0]=0;
	c_hk[current_proc->a0]=0;
	c_lk[current_proc->a0]=0;
	return;
}

/******************************************************************************
 Function: WORD check_axe_up_combo(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - condition matched (axe up)
		  SYSTEM_CARRY_CLR - condition failed

 Description:	see if combo is performed
******************************************************************************/
WORD check_axe_up_combo(void)
{
	(PROCESS *)current_proc->pdata.p_store4=current_proc;		// this is assumed

	return(stick_look_lr(P1B0,P2B0,scom_axe_up));
}

/************************************************
 COMBO STUB FOR CORRECT ROUTINES
************************************************/
WORD combo_q_no(void) 
{
	return(SYSTEM_CARRY_CLR);
}

WORD combo_is_stick_away(void)
{
	return(is_stick_away(current_proc));
}

WORD combo_is_stick_up(void)
{
	return(is_stick_up(current_proc));
}

WORD combo_is_stick_down(void)
{
	return(is_stick_down(current_proc));
}

WORD combo_is_stick_toward(void)
{
	return(is_stick_toward(current_proc));
}

WORD set_his_half_damage(void)
{
	if ((current_proc->pdata.p_otherproc)->pdata.p_hit>=3)
		set_half_damage(current_proc->pdata.p_otherguy);

	return(SYSTEM_CARRY_SET);			// always return 'YES'
}

WORD set_his_quarter_damage(void) 
{
	/* calla_for_him */
	set_half_damage(current_proc->pdata.p_otherguy);
	return(SYSTEM_CARRY_SET);			// always return 'YES'
}