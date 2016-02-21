/******************************************************************************
 File: mkscore.c

 Date: Dec 1994

 (C) Williams Entertainment

 Mortal Kombat III vs Routines
******************************************************************************/

/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkos.h"
#include "mkutil.h"
#include "mkani.h"
#include "mkmain.h"
#include "mkguys.h"
#include "mkgame.h"
#include "mkfile.h"
#include "mkscore.h"
#include "mkdiag.h"

CPTPAGE control_panel_info[]=
{
	{0,0,0,0},							// control panel: nothing to display
	{0,11,13,13},							// control panel: win medal knotches

	{14,12,134,13},							// control panel: power bar LA
	{14,12,134,13},							// control panel: power bar LB

	{0,26,76,10},							// control panel: insert coin
	{179,0,74,11},							// control panel: push start

	{91,0,10,11},							// control panel: shadow num 0
	{101,0,6,10},							// control panel: shadow num 1
	{107,0,10,10},							// control panel: shadow num 2
	{117,0,9,10},							// control panel: shadow num 3
	{126,0,8,10},							// control panel: shadow num 4
	{134,0,9,10},							// control panel: shadow num 5
	{143,0,9,11},							// control panel: shadow num 6
	{152,0,9,10},							// control panel: shadow num 7
	{161,0,9,11},							// control panel: shadow num 8
	{170,0,9,10},							// control panel: shadow num 9
	{76,26,34,10},							// control panel: shadow wins

	{149,11,9,13},							// control panel: clock digit num 0
	{158,11,6,13},							// control panel: clock digit num 1
	{164,11,9,13},							// control panel: clock digit num 2
	{173,11,9,13},							// control panel: clock digit num 3
	{182,11,9,13},							// control panel: clock digit num 4
	{191,11,9,13},							// control panel: clock digit num 5
	{200,11,9,13},							// control panel: clock digit num 6
	{209,11,9,13},							// control panel: clock digit num 7
	{218,11,9,13},							// control panel: clock digit num 8
	{227,11,9,13},							// control panel: clock digit num 9

	{110,34,132,7},							// control panel: powerbar spacer
	{72,36,38,3}							// control panel: turbobar spacer
};


short name_wh[][2]=
{
	{26,7},							// p kano
	{30,7},							// p sonya
	{19,7},							// p jax
	{53,7},							// p nightwolf
	{44,7},							// p subzero
	{41,7},							// p stryker
	{31,7},							// p sindle
	{36,7},							// p sektor
	{32,7},							// p cyrax
	{48,7},							// p kunglao
	{30,7},							// p kabal
	{34,7},							// p sheeva
	{66,7},							// p shang tsung
	{43,7},							// p liu kang
	{32,7},							// p smoke
	{34,7},							// p kitana	
	{26,7},							// p jade	
	{40,7},							// p mileena
	{47,7},							// p scorpion
	{36,7},							// p reptile 
	{33,7},							// p ermac	 
	{44,7},							// p old sz
	{32,7},							// p smoke
	{62,7},							// p noob
	{39,7},							// p motaro
	{54,7},							// p shao kahn
	{54,7}							// p RAIN
};

short p2_powerbar_names[] =
{
	SCX(0x0153)-2,				// p kano
	SCX(0x014c)-2,				// p sonya
	SCX(0x015a)-2,				// p jax
	SCX(0x0132)-2,				// p nightwolf
	SCX(0x013c)-2,				// p subzero
	SCX(0x0140)-2,				// p stryker
	SCX(0x0149)-2,				// p sindle
	SCX(0x0149)-2,				// p sektor
	SCX(0x0149)-2,				// p cyrax
	SCX(0x0139)-2,				// p kunglao
	SCX(0x014f)-2,				// p kabal
	SCX(0x0149)-2,				// p sheeva
	SCX(0x0123)-2,				// p shang tsung
	SCX(0x013d)-2,				// p liu kang
	SCX(0x0149)-2,				// p smoke
	265-4,				// p kitana	
	273-4,				// p jade	
	259-4,				// p mileena
	252-4,				// p scorpion
	263-4,				// p reptile 
	266-4,				// p ermac	 
	SCX(0x013c)-2,				// p old subzero
	SCX(0x0149)-2,				// p old smoke
	SCX(0x0127)-2,				// p noob
	SCX(0x0144)-2,				// p motaro
	SCX(0x0132)-2,				// p shao
	SCX(0x132)-2				// p RAIN
};

BYTE score_nums_tbl[]=
{
	X_SHADNUM0,
	X_SHADNUM1,
	X_SHADNUM2,
	X_SHADNUM3,
	X_SHADNUM4,
	X_SHADNUM5,
	X_SHADNUM6,
	X_SHADNUM7,
	X_SHADNUM8,
	X_SHADNUM9
};

BYTE clock_nums[] =
{
	COUNTDOWN_0,
	COUNTDOWN_1,
	COUNTDOWN_2,
	COUNTDOWN_3,
	COUNTDOWN_4,
	COUNTDOWN_5,
	COUNTDOWN_6,
	COUNTDOWN_7,
	COUNTDOWN_8,
	COUNTDOWN_9
};

/******************************************************************************
 Function: void setup_score_ram(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	init things so we can display the score area stuff
******************************************************************************/
void setup_score_ram(void)
{
	CPENTRY *sr_ptr;

	f_doscore=0;					// flag: dont do score area DMAS
	sr_ptr=score_ram;

	/* win knotches (4 entries, 1-2-3-4) */
	SET_SCORE_RAM(sr_ptr,P1_K1_X,P_KNOTCH_Y,MKMEDAL2,0);		// p1 notch #1 = NULL
	sr_ptr++;
	SET_SCORE_RAM(sr_ptr,P1_K2_X,P_KNOTCH_Y,MKMEDAL2,0);		// p1 notch #2 = NULL
	sr_ptr++;
	SET_SCORE_RAM(sr_ptr,P2_K1_X,P_KNOTCH_Y,MKMEDAL2,0);		// p2 notch #1 = NULL
	sr_ptr++;
	SET_SCORE_RAM(sr_ptr,P2_K2_X,P_KNOTCH_Y,MKMEDAL2,0);		// p2 notch #2 = NULL
	sr_ptr++;

	/* coin message/ player score (2 entries) (5-6) */
	SET_SCORE_RAM(sr_ptr,0,0,BAR_NULL,0);		// p1 = null entry
	sr_ptr++;
	SET_SCORE_RAM(sr_ptr,0,0,BAR_NULL,0);		// p2 = null entry
	sr_ptr++;

	/* character names in front of powerbars (2 entries) (7-8) */
	SET_SCORE_RAM(sr_ptr,2+SCX(0x19),SCY(0x1a),P1_NAME_SPEC,1);		// p1 name
	sr_ptr++;
	SET_SCORE_RAM(sr_ptr,p2_powerbar_names[p2_name_char],SCY(0x1a),P2_NAME_SPEC,1);		// p2 name
	sr_ptr++;

	/* solid color powerbars (2 entries) (9-10) */
	SET_SCORE_RAM(sr_ptr,P1_BACKBAR_X,BACKBAR_Y,P1_BACKPWR_SPEC,get_safe_powerbar(&p1_bar));		// p1 backdrop for powerbar
	sr_ptr++;
	SET_SCORE_RAM(sr_ptr,P2_BACKBAR_X+BACKBAR_WIDTH-1,BACKBAR_Y,P2_BACKPWR_SPEC,get_safe_powerbar(&p2_bar));		// p2 backdrop for powerbar
	sr_ptr++;

	/* clock digits (2 entries) (11-12) */
	SET_SCORE_RAM(sr_ptr,CLOCK_TENS_X,CLOCK_TENS_Y,COUNTDOWN_9,1);		// clock tens init with 9
	sr_ptr++;

	SET_SCORE_RAM(sr_ptr,CLOCK_ONES_X,CLOCK_ONES_Y,COUNTDOWN_9,1);		// clock ones init with 9
	sr_ptr++;

	/* score digits for player "# of wins" (6 entries) (13-14-15-16-17-18) */
	SET_SCORE_RAM(sr_ptr,P1_MSG_X+0*DIGIT_SIZE,P1_MSG_Y,X_SHADNUM0,1);		// p1 digit #1 (hundreds)
	sr_ptr++;
	SET_SCORE_RAM(sr_ptr,P1_MSG_X+1*DIGIT_SIZE,P1_MSG_Y,X_SHADNUM0,1);		// p1 digit #1 (tens)
	sr_ptr++;
	SET_SCORE_RAM(sr_ptr,P1_MSG_X+2*DIGIT_SIZE,P1_MSG_Y,X_SHADNUM0,1);		// p1 digit #1 (ones)
	sr_ptr++;

	SET_SCORE_RAM(sr_ptr,P2_MSG_X+0*DIGIT_SIZE,P2_MSG_Y,X_SHADNUM0,1);		// P2 digit #1 (hundreds)
	sr_ptr++;
	SET_SCORE_RAM(sr_ptr,P2_MSG_X+1*DIGIT_SIZE,P2_MSG_Y,X_SHADNUM0,1);		// P2 digit #1 (tens)
	sr_ptr++;
	SET_SCORE_RAM(sr_ptr,P2_MSG_X+2*DIGIT_SIZE,P2_MSG_Y,X_SHADNUM0,1);		// P2 digit #1 (ones)
	sr_ptr++;

	/* turbo bars (19-20) */
	SET_SCORE_RAM(sr_ptr,P1_TURBO_X+1,P1_TURBO_Y+1,P1_TURBO_SPEC,1);
	sr_ptr++;
	SET_SCORE_RAM(sr_ptr,P2_TURBO_X+TURBOBAR_WIDTH-1,P2_TURBO_Y+1,P2_TURBO_SPEC,1);
	sr_ptr++;

	/* backdrops for powerbars (2 entries) (21-22) */
	SET_SCORE_RAM(sr_ptr,P1_BACKBAR_X,BACKBAR_Y,PWRBAR_LA,1);		// p1 bar backdrop
	sr_ptr++;

	SET_SCORE_RAM(sr_ptr,P2_BACKBAR_X,BACKBAR_Y,PWRBAR_RA,1);		// p2 bar backdrop
	sr_ptr++;

	return;

}

/******************************************************************************
 Function: void update_win_counters(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	update p1 & p2 win counters
******************************************************************************/
void update_win_counters(void)
{
	/* update player 1*/
	if (p1_state==PS_ACTIVE)
	{
		uwc_stuff(p1_wiar,entry_13,P1_MSG_X+SCX(0x2d),P1_MSG_Y);

		entry_6->x0=P1_MSG_X;				// coordinate for player 1 "WINS:" image
		entry_6->y0=P1_MSG_Y;
	}

	/* update player 2*/
	if (p2_state==PS_ACTIVE)
	{
		uwc_stuff(p2_wiar,entry_16,P2_MSG_X-SCX(0x0e),P2_MSG_Y);

		entry_5->y0=P2_MSG_Y;				// coordinate for player 1 "WINS:" image

		if (p2_wiar>100)
			entry_5->x0=P2_MSG_X-SCX(0x3a)-DIGIT_SIZE;
		else entry_5->x0=P2_MSG_X-SCX(0x3a);
	}

	return;
}

/******************************************************************************
 Function: void uwc_stuff(WORD pwiar,CPENTRY *sr_ptr,WORD xpos,WORD ypos)

 By: David Schwartz

 Date: Dec 1994

 Parameters: pwiar - # of wins in a row
			 sr_ptr - ptr to start of score ram to fill
		  	 xpos - x position for message
			 ypos - y position for message

 Returns: None

 Description:	setup ram entry for wins counter stuff
******************************************************************************/
void uwc_stuff(WORD pwiar,CPENTRY *sr_ptr,WORD xpos,WORD ypos)
{
	short h_flag=0;
	WORD digit;
	short adjust;

	/* clear ones, tens, hundreds digit display */
	(sr_ptr+0)->disp_flag=0;
	(sr_ptr+1)->disp_flag=0;
	(sr_ptr+2)->disp_flag=0;

	/* make each digit prove it should be displayed */
	if ((digit=(pwiar/100))!=0)
	{
		h_flag=1;
		pwiar%=100;							// take off 100's

		/* update_score_digit (100's) */
		adjust=(digit==1)?2:0;				// if 1's digit then nudge coord
		SET_SCORE_RAM(sr_ptr,xpos+adjust,ypos,score_nums_tbl[digit],1);
		xpos+=DIGIT_SIZE;
	}

	sr_ptr++;								// next entry

	digit=(pwiar/10);
	//if ((digit=(pwiar/10))!=0) // || h_flag==1)
	{
		pwiar%=10;							// take off 10's

		/* update_score_digit (10's) */
		adjust=(digit==1)?2:0;				// if 1's digit then nudge coord
		SET_SCORE_RAM(sr_ptr,xpos+adjust,ypos,score_nums_tbl[digit],1);
		xpos+=DIGIT_SIZE;
	}

	sr_ptr++;								// next entry
	digit=pwiar;

	/* update_score_digit (1's) */
	adjust=(digit==1)?2:0;				// if 1's digit then nudge coord
	SET_SCORE_RAM(sr_ptr,xpos+adjust,ypos,score_nums_tbl[digit],1);
	xpos+=DIGIT_SIZE;

	return;
}

/******************************************************************************
 Function: void update_clock_digit(CPENTRY *sr_ptr,WORD xpos,WORD ypos,WORD digit)

 By: David Schwartz

 Date: Dec 1994

 Parameters: sr_ptr - score ram entry
			 xpos - xpos of digit
			 ypos - ypos of digit
			 digit - digit to display

 Returns: None

 Description:	update a clock digit
******************************************************************************/
void update_clock_digit(CPENTRY *sr_ptr,WORD xpos,WORD ypos,WORD digit)
{
	short adjust;

	adjust=(digit==1)?2:0;				// if 1's digit then nudge coord
	SET_SCORE_RAM(sr_ptr,xpos+adjust,ypos,clock_nums[digit],1);
}

/******************************************************************************
 Function: void update_knotches(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	updates the ram so the correct "knotches" state
                is represented in the score area.
******************************************************************************/
void update_knotches(void)
{
	switch (p1_matchw)
	{
		case 0:
			p1_knotch1->disp_flag=0;
			p1_knotch2->disp_flag=0;
			break;
		case 1:
			p1_knotch1->disp_flag=1;
			p1_knotch2->disp_flag=0;
			break;
		case 2:
			p1_knotch1->disp_flag=1;
			p1_knotch2->disp_flag=1;
			break;
	}

	switch (p2_matchw)
	{
		case 0:
			p2_knotch1->disp_flag=0;
			p2_knotch2->disp_flag=0;
			break;
		case 1:
			p2_knotch1->disp_flag=1;
			p2_knotch2->disp_flag=0;
			break;
		case 2:
			p2_knotch1->disp_flag=1;
			p2_knotch2->disp_flag=1;
			break;
	}
}

/******************************************************************************
 Function: void qwerty(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	update the powerbar dither every vblank
******************************************************************************/
void qwerty(void)
{

#if 0			// routine not needed since, we are using hardware to do what that do here
	if (entry_21->cpentry==PWRBAR_LA)
	{
		entry_21->cpentry=PWRBAR_LA;	//b
		entry_22->cpentry=PWRBAR_RA;
	}
	else
	{
		entry_21->cpentry=PWRBAR_LA;
		entry_22->cpentry=PWRBAR_RA;
	}
#endif

	return;
}

/******************************************************************************
 Function: void update_pmsg(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	Update player messages
******************************************************************************/
void update_pmsg(void)
{
	PROCESS *ptemp;

	/* player 1 message */
	if (p1_state==PS_ACTIVE)
	{
		SET_SCORE_RAM(entry_6,SCX(0x0034),18+SCY(0x000c),X_SHADWINS,1);
	}
	else
	{
		entry_13->disp_flag=0;
		entry_14->disp_flag=0;
		entry_15->disp_flag=0;
		SET_SCORE_RAM(entry_6,P1_SIC_X,P1_SIC_Y,start_or_insertcoin(),1);
	}

	/* player 2 message */
	if (p2_state==PS_ACTIVE)
	{
		SET_SCORE_RAM(entry_5,SCX(0x012d),18+SCY(0x000c),X_SHADWINS,1);
	}
	else
	{
		entry_16->disp_flag=0;
		entry_17->disp_flag=0;
		entry_18->disp_flag=0;
		SET_SCORE_RAM(entry_5,P2_SIC_X,P2_SIC_Y,start_or_insertcoin(),1);
	}

	if ((ptemp=process_exist(PID_FLASHER,0xffff))!=NULL)
		fastxfer(ptemp,flash_pmsg2);
}

/******************************************************************************
 Function: WORD start_or_insertcoin(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: display index

 Description:	determine which message to display based on # of credits
******************************************************************************/
WORD start_or_insertcoin(void)
{
#if 0
	if (cr_strtp()==FALSE)
		return(INSERT_COIN);
	else return(PUSHSTART);
#endif
	return(PUSHSTART);
}

/******************************************************************************
 Function: void recharge_bars(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	give both players full power and cancel any bar drop values
******************************************************************************/
void recharge_bars(void)
{
	p1_bar=p1_bar_view=FULL_STRENGTH;
	p2_bar=p2_bar_view=FULL_STRENGTH;

	p1_turbo=p1_turbo_view=FULL_TURBO;
	p2_turbo=p2_turbo_view=FULL_TURBO;

	position_p2_bar();

	return;
}

/******************************************************************************
 Function: void position_p2_bar(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	pos p2 turbo and power bar
******************************************************************************/
void position_p2_bar(void)
{
	p2_bar_xpos=FULL_STRENGTH-p2_bar_view+P2_BACKBAR_X+1;

	p2_turbo_xpos=FULL_TURBO-p2_turbo_view+P2_TURBO_X+1;

	return;
}

/******************************************************************************
 Function: void update_control_panel

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	This routine updates the control panel.  The panel consists of:
			1)	p1 & p2 win knotches
			2)	p1 & p2 coin messages (start pause etc)
			3)	p1 & p2 character names
			4)	p1 & p2 power bars
			5)	clock digits (10's 1's)
			6)	p1 wins (100's 10's 1's)
			7)	p2 wins (100's 10's 1's)
******************************************************************************/
void update_control_panel(void)
{
	short loop;
	short sindex=0;
	CPENTRY *sr_ptr;
	SPRT *sprt;
	CPTPAGE *cp;
	RECT area;

	setRECT(&area,0,0,255,255);
	/* change default tpage directive */
	SetDrawMode(&(cdb->dm_score),0,0,control_floor_tpage,&area);
	AddPrim(cdb->ot_ptr,&(cdb->dm_score));
	cdb->primcnt++;
	cdb->ot_ptr++;								/* next priority level */

	for (loop=0;loop<SCORE_RAM_CNT;loop++)
	{
		sr_ptr=score_ram+loop;

		if (sr_ptr->disp_flag!=0)
		{
			switch (sr_ptr->cpentry)
			{
				case P1_BACKPWR_SPEC:
					cp=control_panel_info+ENERGY_FILL_BAR;
					setXYWH(cdb->curr_prim,sr_ptr->x0+2,sr_ptr->y0+1,(p1_bar_view*80/100)-1,POWERBAR_Y_SIZE);
					cdb->curr_prim->clut=score_pal;
					cdb->curr_prim->tpage=control_floor_tpage;
					setUVWH(cdb->curr_prim,cp->u0,cp->v0,cp->w,cp->h);
					SetSemiTrans(cdb->curr_prim,0);
					cdb->primcnt++;
					AddPrim(cdb->ot_ptr,cdb->curr_prim);
					cdb->curr_prim++;						/* next prim */
					break;

				case P2_BACKPWR_SPEC:
					cp=control_panel_info+ENERGY_FILL_BAR;
					setXYWH(cdb->curr_prim,sr_ptr->x0+1-(p2_bar_view*80/100),sr_ptr->y0+1,(p2_bar_view*80/100)-1,POWERBAR_Y_SIZE);
					cdb->curr_prim->clut=score_pal;
					cdb->curr_prim->tpage=control_floor_tpage;
					setUVWH(cdb->curr_prim,cp->u0,cp->v0,cp->w,cp->h);
					SetSemiTrans(cdb->curr_prim,0);
					cdb->primcnt++;
					AddPrim(cdb->ot_ptr,cdb->curr_prim);
					cdb->curr_prim++;						/* next prim */
					break;

				case P1_TURBO_SPEC:
					cp=control_panel_info+TURBO_FILL_BAR;
					setXYWH(cdb->curr_prim,sr_ptr->x0,sr_ptr->y0,(p1_turbo_view*80/100),TURBOBAR_Y_SIZE);
					cdb->curr_prim->clut=score_pal;
					cdb->curr_prim->tpage=control_floor_tpage;
					setUVWH(cdb->curr_prim,cp->u0,cp->v0,cp->w,cp->h);
					SetSemiTrans(cdb->curr_prim,0);
					cdb->primcnt++;
					AddPrim(cdb->ot_ptr,cdb->curr_prim);
					cdb->curr_prim++;						/* next prim */
					break;

				case P2_TURBO_SPEC:
					cp=control_panel_info+TURBO_FILL_BAR;
					setXYWH(cdb->curr_prim,sr_ptr->x0-(p2_turbo_view*80/100),sr_ptr->y0,(p2_turbo_view*80/100),TURBOBAR_Y_SIZE);
					cdb->curr_prim->clut=score_pal;
					cdb->curr_prim->tpage=control_floor_tpage;
					setUVWH(cdb->curr_prim,cp->u0,cp->v0,cp->w,cp->h);
					SetSemiTrans(cdb->curr_prim,0);
					cdb->primcnt++;
					AddPrim(cdb->ot_ptr,cdb->curr_prim);
					cdb->curr_prim++;						/* next prim */
					break;

				case P1_NAME_SPEC:
					sprt=&(cdb->cp[sindex++]);
					sprt->clut=score_pal;
					sprt->u0=P1_NAME_TXT_X;
					sprt->v0=P1_NAME_TXT_Y;
					sprt->w=name_wh[p1_name_char][0];
					sprt->h=name_wh[p1_name_char][1];
					sprt->x0=sr_ptr->x0;
					sprt->y0=sr_ptr->y0+18;
					SetSemiTrans(sprt,0);
					cdb->primcnt++;
					AddPrim(cdb->ot_ptr,sprt);							// add to order table
					break;
				case P2_NAME_SPEC:
					sprt=&(cdb->cp[sindex++]);
					sprt->clut=score_pal;
					sprt->u0=P2_NAME_TXT_X;
					sprt->v0=P2_NAME_TXT_Y;
					sprt->w=name_wh[p2_name_char][0];
					sprt->h=name_wh[p2_name_char][1];
					sprt->x0=sr_ptr->x0+2;
					sprt->y0=sr_ptr->y0+18;
					SetSemiTrans(sprt,0);
					cdb->primcnt++;
					AddPrim(cdb->ot_ptr,sprt);							// add to order table
					break;
				case BAR_NULL:
					break;
				case PWRBAR_RA:			// special to flip texture
					cp=control_panel_info+sr_ptr->cpentry-1;
					setXY4(cdb->curr_prim, \
							sr_ptr->x0+cp->w,sr_ptr->y0, \
							sr_ptr->x0,sr_ptr->y0, \
							sr_ptr->x0+cp->w,sr_ptr->y0+cp->h, \
							sr_ptr->x0,sr_ptr->y0+cp->h);
					cdb->curr_prim->clut=score_pal;
					cdb->curr_prim->tpage=control_floor_tpage;
					SetUVWH(cdb->curr_prim,cp->u0,cp->v0,cp->w,cp->h);
					SetSemiTrans(cdb->curr_prim,1);
					cdb->primcnt++;
					AddPrim(cdb->ot_ptr,cdb->curr_prim);
					cdb->curr_prim++;						/* next prim */
					break;

				default:
					sprt=&(cdb->cp[sindex++]);
					cp=control_panel_info+sr_ptr->cpentry;

					sprt->clut=score_pal;
					sprt->u0=cp->u0;
					sprt->v0=cp->v0;
					sprt->w=cp->w;
					sprt->h=cp->h;
					sprt->x0=sr_ptr->x0;
					sprt->y0=sr_ptr->y0;

					if (sr_ptr==entry_21 || sr_ptr==entry_22) 			// set semi trans for score bar
						SetSemiTrans(sprt,1);
					else SetSemiTrans(sprt,0);

					cdb->primcnt++;
					AddPrim(cdb->ot_ptr,sprt);							// add to order table
					break;
			}
		}
	}

	cdb->ot_ptr++;								/* next priority level */

	return;
}

/******************************************************************************
 Function: void refresh_score(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	reinit the score panel stuff
******************************************************************************/
void refresh_score(void)
{
	setup_score_ram();
	SHOW_SCORES;
	update_knotches();
	update_pmsg();
	update_win_counters();
	if ( current_proc->pa8==p1_obj )
		load_p1_name();
	else load_p2_name();
	return;
}

/******************************************************************************
 Function: void load_control_page(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	load control page texture page and the p1 & p2 char names
******************************************************************************/
extern ADDRESS *name_anim_imgs[];
extern void *combo_heap[];
extern void *damage_s[];
extern void *hit_s[];
extern void *DANGER[];
extern void *TOASTY[];

ADDRESS *combo_perm_list[]=
{
	(ADDRESS *)damage_s,
	(ADDRESS *)hit_s,
	(ADDRESS *)DANGER,
	NULL
};

ADDRESS *misc_perm_list[]=
{
	(ADDRESS *)TOASTY,
	NULL
};

void load_control_page(void)
{
	WORD pchar;

	load_p1_name();
	load_p2_name();

	/* make combo text permanent */
	pchar=bkgd_base_tpage;					// save bkgd tpage
	bkgd_base_tpage=control_floor_tpage;		// fool routine to where bkgds are
	alloc_vram_perm_bg_list((ADDRESS *)combo_perm_list,&combo_heap);
	bkgd_base_tpage=pchar;					// restore bkgd tpage

	/* make misc (toasty) text permanent */
	pchar=bkgd_base_tpage;					// save bkgd tpage
	bkgd_base_tpage=load_tpage;		// fool routine to where bkgds are
	alloc_vram_perm_bg_list((ADDRESS *)misc_perm_list,&combo_heap);
	bkgd_base_tpage=pchar;					// restore bkgd tpage

	DrawSync(0);

	return;
}

void load_p1_name(void)
{
	WORD pchar;
	ADDRESS *src;
	RECT img;
	/* load p1 Name */
	pchar=p1_name_char;
	if (pchar>FT_SK)
		pchar=0;
	setRECT(&img,CP_TEXT_X+P1_NAME_TXT_X/2,(CP_TEXT_Y+P1_NAME_TXT_Y),((name_wh[pchar][0]+3)&(~3))/2,name_wh[pchar][1]);

	src=(ADDRESS*)uncompress_image((BYTE *)(name_anim_imgs[pchar]));
	LOADIMAGE(&img,src);
}

void load_p2_name(void)
{
	WORD pchar;
	ADDRESS *src;
	RECT img;
	/* load p2 Name */
	pchar=p2_name_char;
	if (pchar>FT_SK)
		pchar=0;
	setRECT(&img,CP_TEXT_X+P2_NAME_TXT_X/2,(CP_TEXT_Y+P2_NAME_TXT_Y),((name_wh[pchar][0]+3)&(~3))/2,name_wh[pchar][1]);

	src=(ADDRESS*)uncompress_image((BYTE *)(name_anim_imgs[pchar]));
	LoadImage(&img,src);
}
