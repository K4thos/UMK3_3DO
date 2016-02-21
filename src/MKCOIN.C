/******************************************************************************
 File: mkcoin.c

 Date: Nov 1995

 (C) Williams Entertainment

 Mortal Kombat III Coin Routines
******************************************************************************/

/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkos.h"
#include "mkgame.h"
#include "mkobj.h"
#include "mkutil.h"
#include "mkani.h"
#include "mkutil.h"
#include "mkinit.h"
#include "mkmain.h"
#include "valloc.h"

extern void *fl_coin_cursor[];
extern char txt_mkcoin1[];
extern char txt_mkcoin2a[];
extern char txt_mkcoin2b[];
extern char txt_mkcoin3a[];
extern char txt_mkcoin3b[];

WORD RED_p[]=				//-DHS- CONVERT TO SONY FORMAT
{
   16 ,0
   0x001f,0x0000,0x4000,0x4400,0x4800,0x5000,0x5400,0x5800
   0x5c00,0x6000,0x6400,0x6800,0x7000,0x7400,0x7800,0x7c00
};

//******************************************************************************
//******************************************************************************
void mode_select(void) 
{
	gstate=GS_MODE_SELECT;

	murder_myoinit_score();
	do_a11_background(BKGD_MK3_COIN);
	p15_coin(&baklst2,txt_mkcoin1,SCX(0x45),SCY(0x38));

	pds_coin(&baklst3,txt_mkcoin2b,SCX(0xc7),SCY(0x4a));
	p15_centered(txt_mk2coin2a,SCX(0xc7),SCY(0x32));
	current_proc->a14=(ADDRESS)objlst2;
	objlst2=NULL;
	tack_a14_on_a0(current_proc->a14,&baklst3);

	pds_coin(&baklst4,txt_mkcoin3b,SCX(0x148),SCY(0x4a));
	p15_centered(txt_mk2coin3a,SCX(0x148),SCY(0x32));
	current_proc->a14=(ADDRESS)objlst2;
	objlst2=NULL;
	tack_a14_on_a0(current_proc->a14,&baklst4);

	CREATE(PID_AMODE,amode_coin_page);

	current_proc->a11=(ADDRESS)fl_coin_cursor;
	CREATE(PID_AMODE,boonpal_stuff);

//	make_cursor_objects(coinpage_cursor,OID_AMODE,bpal_flash1_P);
//-DHS- have greg make a large size cursor

	current_proc->pa8=(OBJECT*)current_proc->a10;

	current_proc->a11=0;
	change_letter_colors(RED_p);

	/* msp2 */
	do
	{
		current_proc->a10=12;
		/* msp3 */
		do
		{
			process_sleep(1);
		}
		while(--current_proc->a10>0);

		if ((swcurr.padvalue & M_P1L) || (swcurr.padvalue & M_P2L)) 
		{
			/* msp_left */
			if (current_proc->a11!=0) 
			{
				change_letter_colors(WHITE_p);
				current_proc->a11--;
				change_letter_colors(RED_p);
				move_cursor(-SCX(0x82));
			}
		}
		else
		{
			if ((swcurr.padvalue & M_P1R) || (swcurr.padvalue & M_P2R)) 
			{
				/* msp_right */
				if (current_proc->a11!=2) 
				{
					change_letter_colors(WHITE_p);
					current_proc->a11--;
					change_letter_colors(RED_p);
					move_cursor(SCX(0x82));
				}
			}
			else
			{
				if ((swcurr.padvalue & M_P1START) || (swcurr.padvalue & M_P2START)) 
					break;
			}
		}
	}
	while(1);

	/* msp_picked */
	mode_of_play=current_proc->a11;
	MURDER;
	dallobj(OID_AMODE);

	current_proc->a10=10;
	/* mspp3 */
	do
	{
		change_letter_colors(WHITE_p);
		process_sleep(4);
		change_letter_colors(RED_p);
		process_sleep(4);
	}
	while(--current_proc->a10>0);

	process_sleep(8);
	return;
}

void move_cursor(short pa0) 
{
//-DHS- assume one cursor object, no lnks
	OBJECT *ta1;

	ta1=current_proc->pa8;
	ta1->oxpos.u.intpos+=pa0;

	return;
}

void change_letter_colors(WORD *pa1) 
{
	OBJECT *ta2;
	OBJECT *pa8=current_proc->pa8;

	switch (current_proc->a11)
	{
		case 0:
			ta2=(OBJECT *)&baklst2;
			break;
		case 1:
			ta2=(OBJECT *)&baklst3;
			break;
		case 2:
			ta2=(OBJECT *)&baklst4;
			break;
	}

	current_proc->pa8=ta2->olink;
	do
	{
		swpal(pa1,current_proc->pa8);
		current_proc->pa8=(current_proc->pa8)->olink;
	}
	while(current_proc->pa8!=NULL);

	current_proc->pa8=pa8;

	return;
}

void pds_coin(OBJECT **pa10,char *pa8,short x,short y)
{
	OBJECT *ta11;

	ta11=objlst2;

	objlst2=NULL;
	pds_centered(pa8,x,y);

	*pa10=objlst2;
	objlst2=ta11;

	return;
}

void p15_coin(OBJECT **pa10,char *pa8,short x,short y)
{
	OBJECT *ta11;

	ta11=objlst2;

	objlst2=NULL;
	p15_centered(pa8,x,y);

	*pa10=objlst2;
	objlst2=ta11;

	return;
}

//******************************************************************************
//******************************************************************************
char txt_ques[]="? ? ? ? ? ";
char txt_player_x[]="PLAYER %d";


WORD word1=1;
WORD word2=2;
WORD word3=3;
WORD word4=4;
WORD word5=5;
WORD word6=6;
WORD word7=7;
WORD word8=8;


TREES tree_structure[]=
{
	{TREEORGX,TREEORGY+(TREEYSP*0),&word1},
	{TREEORGX,TREEORGY+(TREEYSP*1),&word2},
	{TREEORGX,TREEORGY+(TREEYSP*2),&word3},
	{TREEORGX,TREEORGY+(TREEYSP*3),&word4},
	{TREEORGX,TREEORGY+(TREEYSP*4),&word5},
	{TREEORGX,TREEORGY+(TREEYSP*5),&word6},
	{TREEORGX,TREEORGY+(TREEYSP*6),&word7},
	{TREEORGX,TREEORGY+(TREEYSP*7),&word8},

	{TREEORG2X,TREEORG2Y+(TREEYSP*0),&twinners[0]},
	{TREEORG2X,TREEORG2Y+(TREEYSP*2),&twinners[1]},
	{TREEORG2X,TREEORG2Y+(TREEYSP*4),&twinners[2]},
	{TREEORG2X,TREEORG2Y+(TREEYSP*6),&twinners[3]},

	{TREEORG3X,TREEORG3Y+(TREEYSP*0),&twinners[4]},
	{TREEORG3X,TREEORG3Y+(TREEYSP*4),&twinners[5]},

	{TREEORG4X,TREEORG4Y,&twinners[6]},
	{0,0,NULL}
};

void tournament_tree(WORD pa10) 
{
	TREES *ta11;
	WORD ta0,ta8;
	OBJECT *ta1;

	current_proc->pdata.p_flags=pa10;
	murder_myoinit_score();
	bogus_dlist;

	ta8=0;			// match counter
	ta11=tree_structure;
	/* tree3 */
	while (ta11->t_x!=0)
	{
		ta0=*(ta11->t_wrd);
		if (ta0==0)
			pds_centered(txt_ques,ta11->t_x,ta11->t_y);
		else pds_centered_p1(txt_player_x,ta11->t_x,ta11->t_y,ta0);

		if ((ta8>>2)==0)
			ta1=(OBJECT *)&baklst2;
		else ta1=(OBJECT *)&baklst1;

		move_object_list(&objlst2,ta1);
		pa8++;
		ta11++;
	}

	CREATE(PID_FX,ttext_flasher);
	process_sleep(0x40);
	return;
}

void ttext_flasher(void) 
{
	OBJECT *ta10;

	ta10=baklst2;
	do
	{
		ttf_sleep(ta10);
		ttf_sleep(NULL);
	}
	while(1);
}

void ttf_sleep(OBJECT *pa0) 
{
	baklst2=pa0;
	process_sleep(10);
	return;
}
