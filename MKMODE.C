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
#include "mkbkgd.h"
#include "mkobj.h"
#include "mkutil.h"
#include "mkani.h"
#include "mkutil.h"
#include "mkinit.h"
#include "mkfx.h"
#include "mkpal.h"
#include "mkjoy.h"
#include "mktext.h"
#include "mkmain.h"
#include "mksound.h"
#include "mkmode.h"
#include "valloc.h"

extern void amode(void);

extern void *fl_coin_cursor[];
extern char txt_mkcoin1[];
extern char txt_mkcoin2a[];
extern char txt_mkcoin2b[];
extern char txt_mkcoin3a[];
extern char txt_mkcoin3b[];
extern char txt_freeplay[];
extern FNTSETUP pf_credit_mode[];

char txt_choooz[]="SELECT MODE OF PLAY";

extern WORD RED_p[];

WORD RED_p[]=				
{
	16,0,
	0x0000, 0x8000, 0x000F, 0x0010, 0x0011, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
	0x0018, 0x0019, 0x001B, 0x001C, 0x001D, 0x001E
};

//******************************************************************************
//******************************************************************************
extern void *coin_mode_anims[];
extern void *coin_cursor[];

void mode_select(void) 
{
	WORD padvalue1;
	WORD padvalue2;
	WORD ta9;
	OBJECT *obj;

	DISPLAY_ON;
	gstate=GS_MODE_SELECT;
	coinpage_setup();

	/* credit stuff */
	p15_centered(txt_choooz,SCX(0xc8),SCY(0xd)+5);

	if ( f_freeplay )
			p15_centered(txt_freeplay,160,36+0x7e);
	else lm_printf_p1(pf_credit_mode,credits);


	CREATE(PID_MODE,mode_mood_sounds);
	CREATE(PID_AMODE,boonpal_stuff)->a11=(ADDRESS)fl_coin_cursor;

	obj=get_single_obj((LONG)coin_cursor,&coin_mode_anims,bpal_flash1_P,1);
	alloc_cache(coin_cursor,&coin_mode_anims,obj);
	obj->oid=OID_AMODE;
	insert_object(obj,&objlst);
	current_proc->pa8=(OBJECT*)current_proc->a10=obj;
	obj->oxpos.u.intpos=6;
	obj->oypos.u.intpos=0x26;

	current_proc->a11=0;
	change_letter_colors(RED_p);

	current_proc->pdata.p_anicount=0x40*15;			// time to choose

	/* msp2 */
	do
	{
		current_proc->a10=12;
		/* msp3 */
		do
		{
			process_sleep(1);

			if (--current_proc->pdata.p_anicount==0)
				mode_select_timeout();

		}
		while(--current_proc->a10>0);

		padvalue1=0;
		padvalue2=0;
		/* msp4 */
		if (p1_state+p2_state==0)
		{
			padvalue1=swcurr.u.p1pad;
			padvalue2=swcurr.u.p2pad;
		}
		else
		{
			if (look_or_ignore(p1_state,p2_state)==SYSTEM_CARRY_SET)
				padvalue1=swcurr.u.p1pad;

			if (look_or_ignore(p2_state,p1_state)==SYSTEM_CARRY_SET)
				padvalue2=swcurr.u.p2pad;
		}

		/* look_at_p1p2 */
		if ((padvalue1 & M_P1L) || (padvalue2 & M_P2L)) 
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
			if ((padvalue1 & M_P1R) || (padvalue2 & M_P2R)) 
			{
				/* msp_right */
				if (current_proc->a11!=2) 
				{
					change_letter_colors(WHITE_p);
					current_proc->a11++;
					change_letter_colors(RED_p);
					move_cursor(SCX(0x82));
				}
			}
			else
			{
				if (padvalue1 & M_P1START) 
				{
					ta9=0;
					break;
				}
			
				if (padvalue2 & M_P2START) 
				{
					ta9=1;
					break;
				}
			}
		}
	}
	while(1);

	/* msp_picked */
	mode_of_play=current_proc->a11;
	MURDER;
	dallobj(OID_AMODE);

	f_nosound=1;
	tsound(0x6f);					// -DHS- sound effect

	CREATE(PID_MODE,mode_shaker);

	if (ta9==0)
		p1_state=PS_PSEL;
	else p2_state=PS_PSEL;

	current_proc->a10=8;
	/* mspp3 */
	do
	{
		shift_letters(-SCX(6));
		process_sleep(3);
		shift_letters(SCX(6));
		process_sleep(3);
	}
	while(--current_proc->a10>0);

	process_sleep(0x30);
	MURDER;
	current_proc->a10=0;

	return;
}

void mode_select_timeout(void) 
{
	murder_myoinit_score();
	stack_jump(amode);
}

WORD look_or_ignore(WORD pa0,WORD pa7) 
{
	if (pa0==0) 
	{
		/* not_in */
		if (pa7==PS_BUYIN)
			return(SYSTEM_CARRY_CLR);
	}
	else
	{
		if (pa0==PS_ACTIVE) 
		{
			/* meactive */
			if (pa7==0 || pa7==PS_BUYIN)
				return(SYSTEM_CARRY_CLR);
		}
	}

	/* look */
	return(SYSTEM_CARRY_SET);
}

void move_cursor(short pa0) 
{
	OBJECT *ta1;

	ta1=current_proc->pa8;
	ta1->oxpos.u.intpos+=pa0;

	tsound(0x6e);				// cursor move sound --DHS- sound
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

void shift_letters(short pa1)  
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
		(current_proc->pa8)->oxpos.u.intpos+=pa1;
		current_proc->pa8=(current_proc->pa8)->olink;
	}
	while(current_proc->pa8!=NULL);

	current_proc->pa8=pa8;

	return;	
}

void mode_shaker(void) 
{
	shake_a11(8,8);
	process_suicide();
}

WORD mood_sounds[]=	//-DHS- sounds
{
	0x79,			
	0x7a,
	0x7b,	
	0x61,
	0x62,
	0x95,
	0x64,
	0x65,
	0x7d
};

void mode_mood_sounds(void)
{
	play_generic_tune(TRK_SELECT);			// start tune

	/* mms2 */
	do
	{
		current_proc->a10=0;
		do
		{
			process_sleep(0x40*4);
			triple_sound(mood_sounds[current_proc->a10]);
			current_proc->a10++;
		}
		while(current_proc->a10!=9);
	}
	while(1);
}	

extern FNTSETUP pf_dave_smallc_soff[];
void pds_coin(OBJECT **pa10,char *pa8,short x,short y)
{
	OBJECT *ta11;

	ta11=objlst2;

	objlst2=NULL;
//	pds_centered(pa8,x,y);
	lm_setup(pf_dave_smallc_soff);
	fnt_state.fnt_posx=x;
	fnt_state.fnt_posy=y;
	mk_printf(pa8);

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

void tack_a14_on_a0(OBJECT *pa14,OBJECT *pa0) 
{
	OBJECT *pa1;

	do
	{
		pa1=pa0;
		pa0=pa0->olink;
	}
	while(pa0!=NULL);

	pa1->olink=pa14;
	pa14->olink=NULL;

	return;
}

void coinpage_setup(void) 
{
	murder_myoinit_score();
	do_background(BKGD_COIN_MODE);

	p15_coin(&baklst2,txt_mkcoin1,SCX(0x45)-4,SCY(0x38)+20);

	pds_coin(&baklst3,txt_mkcoin2b,SCX(0xc7)-2,SCY(0x4a)+12);

	p15_centered(txt_mkcoin2a,SCX(0xc7)-2,SCY(0x32)+12);
	current_proc->a14=(ADDRESS)objlst2;
	objlst2=NULL;
	tack_a14_on_a0((OBJECT *)current_proc->a14,(OBJECT *)&baklst3);

	pds_coin(&baklst4,txt_mkcoin3b,SCX(0x148)-4,SCY(0x4a)+12);
	p15_centered(txt_mkcoin3a,SCX(0x148)-6,SCY(0x32)+12);
	current_proc->a14=(ADDRESS)objlst2;
	objlst2=NULL;
	tack_a14_on_a0((OBJECT *)current_proc->a14,(OBJECT *)&baklst4);

	CREATE(PID_MODE,player_prompts_proc);		// not needed flash print messages at bottom
	//-DHS- CREATE(PID_MODE,modesel_coin_page);			// not needed 

	return;
}

extern char txt_p1_ic[];
extern char txt_p2_ic[];
extern char txt_p1_ps[];
extern char txt_p2_ps[];
void player_prompts_proc(void) 
{
	char *ta10,*ta11;

	ta10=(f_freeplay==0)?txt_p1_ic:txt_p1_ps;
	ta11=(f_freeplay==0)?txt_p2_ic:txt_p2_ps;

	/* print_messages */
	pds_coin(&baklst6,ta10,SCX(0x50),SCY(0xcd)+6);
	pds_coin(&baklst7,ta11,SCX(0x13c),SCY(0xcd)+6);

	current_proc->a10=(ADDRESS)baklst6;
	current_proc->a11=(ADDRESS)baklst7;

	/* prom6 */
	do
	{
		baklst6=baklst7=NULL;
		process_sleep(10);
		(ADDRESS)baklst6=current_proc->a10;	
		(ADDRESS)baklst7=current_proc->a11;	
		process_sleep(10);
	}
	while(1);
}

//******************************************************************************
//******************************************************************************
char txt_ques[]="? ? ? ? ? ";
char txt_player_x[]="P%d-";

extern void *dlist_tournament[];

extern WORD word1;
extern WORD word2;
extern WORD word3;
extern WORD word4;
extern WORD word5;
extern WORD word6;
extern WORD word7;
extern WORD word8;

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


char n_kano[]="KANO";
char n_sonya[]=	"SONYA";
char n_jax[]="JAX";
char n_ind[]="NIGHTWOLF";
char n_sz[]="SUBZERO";
char n_swat[]="STRYKER";
char n_lia[]="SINDEL";
char n_robo1[]="SEKTOR";
char n_robo2[]="CYRAX";
char n_lao[]="KUNG LAO";
char n_tusk[]="KABAL";
char n_sg[]="SHEEVA";
char n_st[]="SHANG";
char n_lk[]="LIU KANG";
char n_smoke[]=	"SMOKE";
char n_kitana[]="KITANA";
char n_jade[]="JADE";
char n_mileena[]="MILEENA";
char n_scorpion[]="SCORPION";
char n_reptile[]="REPTILE";
char n_ermac[]="ERMAC";
char n_noob[]="NOOB";

char *tnames_table[]=
{
	n_kano,
	n_sonya,
	n_jax,
	n_ind,
	n_sz,

	n_swat,
	n_lia,
	n_robo1,
	n_robo2,
	n_lao,

	n_tusk,
	n_sg,
	n_st,
	n_lk,
	n_smoke,

	n_kitana,
	n_jade,
	n_mileena,
	n_scorpion,
	n_reptile,

	n_ermac,
	n_sz,
	n_smoke,
	n_noob,
};


extern void *vs_module[];
char txt_tournament[]="ULTIMATE MK3 TOURNAMENT";

void tournament_tree(WORD pa10) 
{
	TREES *ta11;
	WORD ta0,ta8;
	OBJECT **ta1;
	OBJECT *obj;

	DISPLAY_OFF;

	current_proc->pdata.p_flags=pa10;
	murder_myoinit_score();
	load_bkgd=BKGD_VS_MOD;
	init_background_module(vs_module);
	multi_plane();
	dlists=dlist_tournament;
	process_sleep(2);

	play_generic_tune(TRK_SELECT);		

	freeze_2_pages();
	background_clear();
	f_auto_erase=0;
	text_draw=1;

	ta8=0;			// match counter
	ta11=tree_structure;
	/* tree3 */
	while (ta11->t_x!=0)
	{
		ta0=*(ta11->t_wrd);
		if (ta0==0)
			pds_left(txt_ques,ta11->t_x,ta11->t_y);
		else 
		{
			p7_left_p1(txt_player_x,ta11->t_x,ta11->t_y,ta0);
			pds_left(tnames_table[p1_tour[ta0-1]],fnt_state.fnt_posx,fnt_state.fnt_posy);
		}
			

		if ((ta8>>1)==pa10)
			ta1=&baklst2;
		else ta1=&baklst3;

//		move_object_list(&objlst2,ta1);
		ta8++;
		ta11++;
	}

	/* treex */
	obj=make_solid_object(0x1f,SCX(0x190),SCY(0x10)+16);
	set_xy_coordinates(obj,0,0);
	insert_object(obj,&objlst);

	p7_centered(txt_tournament,SCX(0xc8),SCX(0x5)+8);

	obj=make_solid_object(0x1f,SCX(0x190),SCY(0x10)+17);
	set_xy_coordinates(obj,0,SCY(0xf8));
	insert_object(obj,&objlst);
	text_draw=0;

	process_sleep(3);
	noflip=1;

	DISPLAY_ON;
	process_sleep(10);

	CREATE(PID_FX,ttext_flasher);
	process_sleep(0x40*4);

	noflip=0;
	f_auto_erase=1;

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
