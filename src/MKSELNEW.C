/******************************************************************************
 File: mksel.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Player Select Routines
******************************************************************************/

/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */
#include "mkbkgd.h"
#include "mkobj.h"
#include "mkos.h"
#include "mkutil.h"
#include "mkani.h"
#include "mktext.h"
#include "mkinit.h"
#include "mkgame.h"
#include "psxcd.h"
#include "mkfile.h"
#include "mkmain.h"
#include "mkguys.h"
#include "mkpal.h"
#include "mkjoy.h"
#include "mksound.h"
#include "mkcanned.h"
#include "wessapi.h"
#include "valloc.h"
#include "mksel.h"

/* local stuff */
extern FNTSETUP pf_auto_msg_1[];
extern FNTSETUP pf_auto_msg_2[];
extern FNTSETUP coin_mess[];
extern FNTSETUP buyin_coin_mess[];
//extern FNTSETUP pf_iar_num[];
//extern FNTSETUP pf_new_champ[];
extern void *fl_cursor1[];
extern void *fl_cursor2[];
extern void *fl_cursor3[];
extern void *fl_cursor4[];
extern void *fl_auto[];
extern char *background_names[];

/* mkpsel.s */
extern void *psel_anims[];
extern void *a_badge_spin[];

#define HDKITANA HDSONYA
#define HDREPTILE HDSONYA
extern void *HDSONYA[];
extern void *HDJAX[];
extern void *HDINDIAN[];
#define HDJADE HDSONYA
#define HDSCORPION HDSONYA
extern void *HDKANO[];
#define HDMILEENA HDSONYA
#define HDERMAC HDSONYA
#define HDCLASSUB HDSONYA
extern void *HDSUBZERO[];
extern void *HDKETCHUP[];
extern void *HDBRIDE[];
extern void *HDSWAT[];
extern void *HDMUSTARD[];
extern void *HDLAO[];
extern void *HDTUSKAN[];
extern void *HDSHEGORO[];
extern void *HDSHANG[];
extern void *HDLIUKANG[];
extern void *BOX_09[];
extern void *HDSMOKE[];
extern void *BOX_01[];			// hidden questions

extern void *HDSHAO[];
extern void *HDMOTARO[];
extern void *HDQUESTION[];
extern void *BOX_02[];
extern void *BOX_03[];
extern void *BOX_04[];
extern void *BOX_05[];
extern void *BOX_06[];
extern void *BOX_07[];
extern void *BOX_08[];
extern void *SCURSOR[];
extern void *MKCOIN_01[];
extern void *MKCOIN_02[];
extern void *MKCOIN_03[];
extern void *MKCOIN_04[];
extern void *MKCOIN_05[];
extern void *MKCOIN_06[];
extern void *MKCOIN_07[];
extern void *MKCOIN_08[];
extern void *MKCOIN_09[];


extern void *HDSHAG_P[];
extern void *HDSUBZ_P[];
extern void *HDSWAT_P[];
extern void *HDBRDE_P[];
extern void *HDJAX_P[];
extern void *HDLAO_P[];
extern void *HDSOYN_P[];
extern void *HDKANO_P[];
extern void *HDMUST_P[];
extern void *HDKANG_P[];
extern void *HDGORO_P[];
extern void *HDINDY_P[];
extern void *HDKETC_P[];
extern void *HDSAND_P[];
extern void *HDMOTA_P[];
extern void *HDSMOK_P[];
extern void *HDSHAO_P[];

extern void *HDKANOD_P[];
extern void *HDSOYND_P[];
extern void *HDJAXD_P[];
extern void *HDINDYD_P[];
extern void *HDSUBZD_P[];
extern void *HDSWATD_P[];
extern void *HDBRDED_P[];
extern void *HDKETCD_P[];
extern void *HDMUSTD_P[];
extern void *HDLAOD_P[];
extern void *HDSANDD_P[];
extern void *HDGOROD_P[];
extern void *HDSHAGD_P[];
extern void *HDKANGD_P[];

extern void *psel_module[];
extern void *ladder_module[];
/******************************************************************************
 Function: void join_in_during_psel(WORD *pstate)

 By: David Schwartz

 Date: Jan 1995

 Parameters: pstate - player state of controler that joined in

 Returns: None

 Description:	Other player jumps in during play select, setup him up
******************************************************************************/
void join_in_during_psel(WORD *pstate)
{
	PROCESS *ptemp;

	tsound(0x93);				// join in sound -DHS- sound

	*pstate=PS_PSEL;

	ptemp=exprc_er(PID_MASTER);		// find central controller

	/* Note: this routine finds the PID_MASTER process that is already started.  We are modifying the arcade source to
			 make this possible from C.  Instead of reseting the stack and changing the wakeup/return address we will
			 set the p_comp_flag.  The PID_MASTER will recoginize this flag and call player_joinin.
	*/
	ptemp->p_comp_flag=PC_PSEL_JOIN;

	process_suicide();
}

MUGS mug_shot_table[]=
{
	{(OIMGTBL *)HDKITANA,MUG1_X+(X_SPACE*0),MUG1_Y+(Y_SPACE*0),FT_KITANA,0x2521},

	{(OIMGTBL *)HDREPTILE,MUG1_X+(X_SPACE*1),MUG1_Y+(Y_SPACE*0),FT_REPTILE,0x2521},
	{(OIMGTBL *)HDSONYA,MUG1_X+(X_SPACE*2),MUG1_Y+(Y_SPACE*0),FT_SONYA,0x2521},
	{(OIMGTBL *)HDJAX,MUG1_X+(X_SPACE*3),MUG1_Y+(Y_SPACE*0),FT_JAX,0x2521},
	{(OIMGTBL *)HDINDIAN,MUG1_X+(X_SPACE*4),MUG1_Y+(Y_SPACE*0),FT_INDIAN,0x2521},
	{(OIMGTBL *)HDJADE,MUG1_X+(X_SPACE*5),MUG1_Y+(Y_SPACE*0),FT_JADE,0x2521},
	{(OIMGTBL *)HDSCORPION,MUG1_X+(X_SPACE*6),MUG1_Y+(Y_SPACE*0),FT_SCORPION,0x2521},

	{(OIMGTBL *)HDKANO,MUG1_X+(X_SPACE*1),MUG1_Y+(Y_SPACE*1),FT_KANO,0x2521},
	{(OIMGTBL *)1,MUG1_X+(X_SPACE*2),MUG1_Y+(Y_SPACE*1),FT_MILEENA,0x2521},
	{(OIMGTBL *)2,MUG1_X+(X_SPACE*3),MUG1_Y+(Y_SPACE*1),FT_ERMAC,0x2521},
	{(OIMGTBL *)3,MUG1_X+(X_SPACE*4),MUG1_Y+(Y_SPACE*1),FT_OLDSZ,0x2521},
	{(OIMGTBL *)HDSUBZERO,MUG1_X+(X_SPACE*5),MUG1_Y+(Y_SPACE*1),FT_SUBZERO,0x2521},

	{(OIMGTBL *)HDKETCHUP,MUG1_X+(X_SPACE*1),MUG1_Y+(Y_SPACE*2),FT_ROBO1,0x2521},
	{(OIMGTBL *)HDBRIDE,MUG1_X+(X_SPACE*2),MUG1_Y+(Y_SPACE*2),FT_LIA,0x2521},
	{(OIMGTBL *)HDSWAT,MUG1_X+(X_SPACE*3),MUG1_Y+(Y_SPACE*2),FT_SWAT,0x2521},
	{(OIMGTBL *)HDMUSTARD,MUG1_X+(X_SPACE*4),MUG1_Y+(Y_SPACE*2),FT_ROBO2,0x2521},
	{(OIMGTBL *)HDLAO,MUG1_X+(X_SPACE*5),MUG1_Y+(Y_SPACE*2),FT_LAO,0x2521},

	{(OIMGTBL *)HDTUSKAN,MUG1_X+(X_SPACE*1),MUG1_Y+(Y_SPACE*3),FT_TUSK,0x2521},
	{(OIMGTBL *)HDSHEGORO,MUG1_X+(X_SPACE*2),MUG1_Y+(Y_SPACE*3),FT_SG,0x2521},
	{(OIMGTBL *)HDSHANG,MUG1_X+(X_SPACE*3),MUG1_Y+(Y_SPACE*3),FT_ST,0x2521},
	{(OIMGTBL *)HDLIUKANG,MUG1_X+(X_SPACE*4),MUG1_Y+(Y_SPACE*3),FT_LK,0x2521},
	{(OIMGTBL *)BOX_09,MUG1_X+(X_SPACE*5),MUG1_Y+(Y_SPACE*3),FT_SMOKE,0x2521},

	{NULL},
};

void *hidden_info[]=
{
	HDMILEENA,	// 0 = mug shot
	&f_mileena,	// 1 = flag to obey
	HDERMAC,
	&f_ermac,
	HDCLASSUB,
	&f_oldsz,
};


PDDATA p1_cdata=
{
	1,					// 0
	fl_cursor1,		// 1
	&swcurr.u.p1pad,		// 2
	&p1_char,			// 3
	&p2_char,			// 4
	psel_drone,		// 5
	mug_shot_table,		// 6
	M_P1ACTION,		// 7

	bpal_flash1_P,		// 0
	OID_CURSOR1,		// 1
	0x0d,			// 2
	PID_P1,			// 3
	0x25,			// 4
	0
};


PDDATA p2_cdata=
{
	2,		   	// 0
	fl_cursor2,			   	// 1
	&swcurr.u.p2pad,			   	// 2
	&p2_char,				   	// 3
	&p1_char,				   	// 4
	psel_drone,			   	// 5
	mug_shot_table+6,	// 6
	M_P2ACTION,				// 7

	bpal_flash2_P,				// 0
	OID_CURSOR2,				// 1
	0x22,				// 2
	PID_P2,					// 3
	SCRRGT-0x24,				// 4
	9
};

PDDATA p1_cteam=
{
	1,		// 0
	fl_cursor1,		// 1
	&swcurr.u.p1pad,			   	// 2
	&p1_char,			// 3
	&p2_char,			// 4
	psel_drone,		// 5
	mug_shot_table,		// 6
	M_P1ACTION,		// 7

	bpal_flash1_P,		// 0
	OID_CURSOR1,		// 1
	0x0d,			// 2
	PID_P1,			// 3
	0x15,			// 4 - world x offset
	0x01, 			// 5 - z position of drone
};

PDDATA p2_cteam=
{
	2,			   	// 0
	fl_cursor2,			   	// 1
	&swcurr.u.p1pad,			   	// 2
	&p2_char,				   	// 3
	&p1_char,				   	// 4
	psel_drone,			   	// 5
	mug_shot_table+(2),	// 6
	M_P1ACTION,				// 7

	bpal_flash2_P,				// 0
	OID_CURSOR2,				// 1
	0x0d,					// 2
	PID_P2,					// 3
	0x35,					// 4 - world x offset
	0x00,					// 5 - z position of drone
};

PDDATA p3_cteam=
{
	3,				// 0
	fl_cursor3,			   	// 1
	&swcurr.u.p2pad,			   	// 2
	&p3_char,				   	// 3
	&p1_char,				   	// 4
	psel_drone,			   	// 5
	mug_shot_table+(4),	// 6
	M_P2ACTION,				// 7

	bpal_flash3_P,				// 0
	OID_CURSOR3,				// 1
	0x22,					// 2
	PID_P3,					// 3
	SCRRGT-0x35,   				// 4 - world x offset
	0x00,				// 5 - z position of drone
};

PDDATA p4_cteam=
{
	4,		   	// 0
	fl_cursor4,			   	// 1
	&swcurr.u.p2pad,			   	// 2
	&p4_char,				   	// 3
	&p1_char,			   	// 4
	psel_drone,			   	// 5
	mug_shot_table+((32+32+32)*6),	// 6
	M_P2ACTION,				// 7

	bpal_flash4_P,				// 0
	OID_CURSOR4,				// 1
	0x22,					// 2
	PID_P4,					// 3
	SCRRGT-0x15,   				// 4 - world x offset
	0x00,					// 5 - z position of drone
};

ADDRESS *psel_vram_list[] =
{
	(ADDRESS *)HDSHANG,
	(ADDRESS *)HDSUBZERO,
	(ADDRESS *)HDSWAT,
	(ADDRESS *)HDBRIDE,
	(ADDRESS *)HDJAX,
	(ADDRESS *)HDLAO,
	(ADDRESS *)HDSONYA,
	(ADDRESS *)HDKANO,
	(ADDRESS *)HDMUSTARD,
	(ADDRESS *)HDLIUKANG,
	(ADDRESS *)HDSHEGORO,
	(ADDRESS *)HDINDIAN,
	(ADDRESS *)HDKETCHUP,
	(ADDRESS *)HDTUSKAN,
	(ADDRESS *)HDSHAO,
	(ADDRESS *)HDMOTARO,
	(ADDRESS *)HDSMOKE,
	(ADDRESS *)HDQUESTION,
	(ADDRESS *)BOX_01,
	(ADDRESS *)BOX_02,
	(ADDRESS *)BOX_03,
	(ADDRESS *)BOX_04,
	(ADDRESS *)BOX_05,
	(ADDRESS *)BOX_06,
	(ADDRESS *)BOX_07,
	(ADDRESS *)BOX_08,
	(ADDRESS *)BOX_09,
	(ADDRESS *)SCURSOR,
	(ADDRESS *)MKCOIN_01,
	(ADDRESS *)MKCOIN_02,
	(ADDRESS *)MKCOIN_03,
	(ADDRESS *)MKCOIN_04,
	(ADDRESS *)MKCOIN_05,
	(ADDRESS *)MKCOIN_06,
	(ADDRESS *)MKCOIN_07,
	(ADDRESS *)MKCOIN_08,
	(ADDRESS *)MKCOIN_09,
	NULL
};


char txt_subway[]="KOMBAT ZONE: THE SUBWAY";
char txt_steet[]="KOMBAT ZONE: THE STREET";
char txt_bank[]="KOMBAT ZONE: THE BANK";
char txt_roof[]="KOMBAT ZONE: ROOFTOP";
char txt_tower[]="KOMBAT ZONE: THE BALCONY";
char txt_bridge[]="KOMBAT ZONE: THE BRIDGE";
char txt_soul[]="KOMBAT ZONE: SOUL CHAMBER";
char txt_bell[]="KOMBAT ZONE: SHAO KAHN TOWER";
char txt_temple[]="KOMBAT ZONE: THE TEMPLE";
char txt_grave[]="KOMBAT ZONE: GRAVEYARD";
char txt_pit[]="KOMBAT ZONE: THE PIT 3";
char txt_hidden[]="KOMBAT ZONE: THE HIDDEN PORTAL";
char txt_select_back[]="SELECT KOMBAT ZONE";
char *background_names[]=
{
	txt_subway,
	txt_steet,
	txt_bank,
	txt_roof,
	txt_tower,
	txt_bridge,
	txt_soul,
	txt_bell,
	txt_temple,
	txt_grave,
	txt_pit,
	txt_hidden
};

char txt_select_msg[]="SELECT YOUR FIGHTER";
char txt_select_team[]="SELECT YOUR TEAMS";
char txt_select_tour[]="TOURNAMENT KOMBAT";
//char txt_new_champ[]="CHAMPION";
//char txt_iar_num[]="%d WINS";

/******************************************************************************
 Function: void player_select(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: ptr to map to use

 Description:	player select screen setup
******************************************************************************/
void player_select(void)
{
	MUGS *mptr;
	OBJECT *obj;
	ADDRESS mug;

#if MK_EJBBUG
//	if (df_fastrun!=0)
//		return;
#endif

	f_load=0;
	f_speech=0;
	f_shadow_fighting=0;
	murder_myoinit();
	f_doscore=0;				// dont display score
	f_fighter_load=0;			// fighter data not loaded yet
	init_player_variables();
	//nosounds();
	wess_seq_stopall();

	//file_load_message();

	load_bkgd=BKGD_MK3_PSEL;
	init_background_module(psel_module);
	multi_plane();
	alloc_vram_perm_bg_list((ADDRESS *)psel_vram_list,&psel_anims);		// load in perms frames

	irqskye=0x1f;
	dlists=dlists_bogus;

	/* load sound stuff */
	sound_module_load(SMOD_SELECT);

//***************************************************************************
#if _CD_VERSION_ == 0
	/* load in player select textures overlay */
	generic_overlay_load(OVL_SELECT);
	f_fighter_load=1;
	//play_generic_tune(TRK_SELECT);				// SCA3(0x32), player select tune
#else
	async_overlay_load(OVL_SELECT);
#endif
//***************************************************************************
	DISPLAY_OFF;

	f_start=0;					// flag: dont fight !!
	p1_char=p2_char=p3_char=p4_char=0xffff;			// null the characters

	/* display mugshots */
	mptr=mug_shot_table;
	(OBJECT *)current_proc->pa9=obj_free;
	(OBJECT *)current_proc->pdata.p_store1=obj_free;		// last object save here

	/* psel3 */
	while (mptr->m_mug!=NULL)
	{
		mug=(ADDRESS)mptr->m_mug;

		if (mug<=3)
		{
			if (*((LONG *)hidden_info[(mug-1)*2])==1)
				mug=*((LONG *)hidden_info[(mug-1)*2+1]);
			else mug=(ADDRESS)BOX_01;
		}

		/* psel_hidden */
		gso_dmawnz_ns(obj,mug,psel_anims,PFLAG_CLEAR);
		(OBJECT *)((OBJECT *)current_proc->pa9)->oslink=obj;			// link mugs together
		((OBJECT *)current_proc->pa9)=obj;
		set_xy_coordinates(obj,mptr->m_xpos,mptr->m_ypos);

		(void *)obj->oshape=(void  *)((LONG)(mptr->m_char));				// char id

		alloc_cache((OIMGTBL *)mug,&psel_anims,obj);

		clip_psel_box(mptr->m_pad,obj);

		insert_object(obj,&objlst);

		mptr++;
	}

	/* psel2 */
	obj->oslink=NULL;					// last object = 0 list

//	if (curback<=10)
//		pds_centered(background_names[curback],SCX(0xc8),SCY(0xec)+12);

	/* psela */
//	position_badge();

	switch (mode_of_play)
	{
		case 0:
			p15_centered(txt_select_msg,SCX(0xc8),SCY(0xb)+8);
			break;
		case 1:
			p15_centered(txt_select_team,SCX(0xc8),SCY(0xb)+8);
			break;
		case 2:
			p15_centered(txt_select_tour,SCX(0xc8),SCY(0xb)+8);
			break;
	}

	DISPLAY_ON;

	if ( f_music )
	{
		current_proc->a0=f_no_sfx;		// make sure music plays
		f_no_sfx=0;
		tsound(0x90);			// play correct tune
		f_no_sfx=current_proc->a0;
	}

	create_cursor_procs();

	psel6();
	return;
}

/******************************************************************************
 Function: void player_joinin(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	player select 2nd player joinin setup
******************************************************************************/
void player_joinin(void)
{
	create_cursor_procs();
//**********************************************
//psel6(); ARCADE MOD: Since this is called from PSEL6 we only want to return, see note in PSEL6
//**********************************************

	return;
}

/******************************************************************************
 Function: void psel6(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	player select main common routine, moves currsor, etc
******************************************************************************/
void psel6(void)
{
	/* psel6 */
	current_proc->a11=(mode_of_play) ?0x40*5 :0x40*15;
	current_proc->p_comp_flag=PC_CLEAR;				// mod from arcade, will be used to sinify if player joined in

	/* psel5 */
	while (exprc(PID_P1SEL)!=NULL || exprc(PID_P2SEL)!=NULL || exprc(PID_P3SEL)!=NULL || exprc(PID_P4SEL)!=NULL)
	{
		current_proc->p_comp_flag=PC_PSEL_WAIT;
		process_sleep(3);

		/* NOTE: this is a mod from the arcade.  The arcade changes the stack and return address of this
				 process when it is sleeping if another player joins in.  We cannont due this easily in C
				 so we will use the p_comp_flag.  If it is set then we will call the player_joinin routine.
				 This will make everything ok.
		*/
		if (current_proc->p_comp_flag==PC_PSEL_JOIN)
		{
			current_proc->p_comp_flag=PC_CLEAR;
			current_proc->a11=0x40*5;
			player_joinin();
		}

		/* waiting_for_psel */
		if (--current_proc->a11==0)
		{
			/* psel9 */
			force_psel_choice(PID_P1SEL);		// timeout, force choice now
			force_psel_choice(PID_P2SEL);
			force_psel_choice(PID_P3SEL);
			force_psel_choice(PID_P4SEL);
			break;
		}

//***************************************************************************
#if _CD_VERSION_ == 1
		if (f_fighter_load==0 && !psxcd_async_on())
		{
			/* done loading fighter data */
			f_fighter_load=1;							// only do this once and let drones appear

			/* close file io stuff */
			module_os_close(module_int);				// clean up after yer self

			/* play music */
			//play_generic_tune(TRK_SELECT);				// SCA3(0x32), player select tune
		}
#endif
//***************************************************************************
	}

	/* psel4 */
	process_sleep(0x20);

	p1_state=((short)p1_char!=-1) ? PS_ACTIVE:0;		// activate dudes

	p2_state=((short)p2_char!=-1) ? PS_ACTIVE:0;

	gstate=GS_POST_PSEL;					// game state: post player select

	process_sleep(0x30);

	dallobj(OID_CURSOR1);
	dallobj(OID_CURSOR2);					// delete cursor objects

	/* secret cheat code activation */
	if (f_level_select)
		select_background();

	process_sleep(0x20);

	dallobj(OID_TEXT);			// remove old text
	MURDER;

	wess_seq_stopall();			// kill looping sound effects
	return;
}

void clip_psel_box(WORD pa0,OBJECT *obj) 
{
	return;
}

/******************************************************************************
 Function: void create_cursor_procs(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description: create a cursor proc for any player who is in the game.
******************************************************************************/
void create_cursor_procs(void)
{

	switch (mode_of_play)
	{
		case 3:			// cursor_tournament
			p1_state=p2_state=PS_ACTIVE;
		case 0:			// cursor_1_on_1
			create_psel_proc(PID_P1SEL,0,&p1_state,&p1_cdata,player_select_proc);
			create_psel_proc(PID_P2SEL,0,&p2_state,&p2_cdata,player_select_proc);
			break;
		case 1:			// cursor_2_on_2
			p1_state=p2_state=PS_ACTIVE;
			create_psel_proc(PID_P1SEL,0,&p1_state,&p1_cteam,player_select_proc);
			create_psel_proc(PID_P2SEL,0,&p1_state,&p2_cteam,player_select_proc);
			create_psel_proc(PID_P3SEL,1,&p2_state,&p3_cteam,player_select_proc);
			create_psel_proc(PID_P4SEL,1,&p2_state,&p4_cteam,player_select_proc);
			break;
	}

	return;
}

void create_psel_proc(WORD pa0,WORD pa4,WORD *pa5,PDDATA *pa6,FUNC pa7) 
{
	PROCESS *ta0;

	if (*pa5!=PS_PSEL) 
	{
		if (*pa5==PS_ACTIVE) 
		{
			*pa5=PS_PSEL;

			ta0=create_if_gone(pa0,pa7);
			if (ta0!=NULL) 
			{
				ta0->pdata.p_store8=(ADDRESS)pa6;
				ta0->pdata.p_hit=pa4;
			}
		}
	}

	return;

}

/******************************************************************************
 Function: inline void force_psel_choice(WORD pid)

 By: David Schwartz

 Date: Jan 1995

 Parameters: pid - id of process to force to choose

 Returns: None

 Description: force process id to pick a player
******************************************************************************/
inline void force_psel_choice(WORD pid)
{
	PROCESS *ptemp;

	if ((ptemp=exprc(pid))!=NULL)
		fastxfer(ptemp,cmove9);

	return;
}



/******************************************************************************
 Function: void player_select_proc()

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description: player 1 select
******************************************************************************/

WORD upsans[]={0,1,2,3,4,5,6};
WORD dnsans[]={0,6,17,18,19,20,21,21};
WORD lfsans[]={0,7,12,17,17,17,17};
WORD rtsans[]={6,11,16,21,21,21,21};

WORD up_change[]=
{
	0 ,		// 0 -
	1 ,		// 1 -
	2 ,		// 2 -
	3 ,		// 3 -
	4 ,		// 4 -
	5 ,		// 5 -
	6 ,		// 6 -

	1 ,		// 7 -
	2 ,		// 8 -
	3 ,		// 9 -
	4 ,		// 10 -
	5 ,		// 11 -

	7 ,		// 12 -
	8 ,		// 13 -
	9 ,		// 14 -
	10,		// 15 -
	11,		// 16 -

	12,		// 17 -
	13,		// 18 -
	14,		// 19 -
	15,		// 20 -
	16,		// 21 -
};

WORD down_change[]=
{
	0 ,	        // 0 -
	7 ,		// 1 -
    8 ,			// 2 -
	9 ,		// 3 -
	10,		// 4 -
	11,		// 5 -
	6 ,		// 6 -

	12,		// 7 -
	13,		// 8 -
	14,		// 9 -
	15,		// 10 -
	16,		// 11 -

	17,		// 12 -
	18,		// 13 -
	19,		// 14 -
	20,		// 15 -
	21,		// 16 -

	17,		// 17 -
	18,		// 18 -
	19,		// 19 -
	20,		// 20 -
	21,		// 21 -
};

WORD left_change[]=
{
	0 ,	        // 0 -
	0 ,		// 1 -
    1 ,		// 2 -
	2 ,		// 3 -
	3 ,		// 4 -
	4 ,		// 5 -
	5 ,		// 6 -

	7 ,		// 7 -
	7 ,		// 8 -
	8 ,		// 9 -
	9 ,		// 10 -
	10,		// 11 -

	12,		// 12 -
	12,		// 13 -
	13,		// 14 -
	14,		// 15 -
	15,		// 16 -

	17,		// 17 -
	17,		// 18 -
	18,		// 19 -
	19,		// 20 -
	20,		// 21 -
};

WORD right_change[]=
{
	1 ,	        // 0 -
	2 ,		// 1 -
    3 ,		// 2 -
	4 ,		// 3 -
	5 ,		// 4 -
	6 ,		// 5 -
	6 ,		// 6 -

	8 ,		// 7 -
	9 ,		// 8 -
	10,		// 9 -
	11,		// 10 -
	11,		// 11 -

	13,		// 12 -
	14,		// 13 -
	15,		// 14 -
	16,		// 15 -
	16,		// 16 -

	18,		// 17 -
	19,		// 18 -
	20,		// 19 -
	21,		// 20 -
	21,		// 21 -
};

void player_select_proc(void)
{
	PROCESS *ptemp;
	OBJECT *obj;
	FNTTPAGE *fptr;
	PDDATA *pa0;
	WORD jpad;
	MUGS *pa4;

	pa0=(PDDATA*)current_proc->pdata.p_store8;
	ptemp=CREATE(pa0->pd_pid,pa0->pd_func);
	current_proc->pdata.p_store7=(ADDRESS)ptemp;

	ptemp->pdata.p_store8=(ADDRESS)pa0;

	current_proc->pdata.p_joyport=pa0->pd_joy;			// look at p1 controls

	(WORD *)(current_proc->pdata.p_mychar)=pa0->pd_p1char;	// pointer to my char
	(WORD *)(current_proc->pdata.p_hischar)=pa0->pd_p2char;	// pointer to his char


	ptemp=CREATE(PID_FX,boonpal_stuff);
	ptemp->a11=(ADDRESS)pa0->pd_fldata;						// flash letters
	(PROCESS *)current_proc->pdata.p_store2=ptemp;		// save cursor flash


//-DHS-	stuff_number_shape(current_proc->pdata.p_hit);

	pa4=pa0->pd_mug;

	current_proc->pdata.p_store5=pa0->pd_oid;
	*((WORD *)current_proc->pdata.p_mychar)=pa4->m_char;

	/* curs2 */

	(OBJECT *)current_proc->pa9=obj_free;
	(OBJECT *)current_proc->a10=obj_free;

	/* curs4 */
	obj=get_single_obj((LONG)SCURSOR,&psel_anims,pa0->pd_pal,1);

	alloc_cache(SCURSOR,&psel_anims,obj);

	/* curs4 */
	obj->oid=current_proc->pdata.p_store4;			// set oid of cur
	(OBJECT *)(((OBJECT *)current_proc->pa9)->oslink)=obj;		// link cursor

	obj->oxpos.u.intpos=pa4->m_xpos-2;
	obj->oypos.u.intpos=pa4->m_ypos-3;
	obj->ozval=2;

	insert_object(obj,&objlst);
	(OBJECT *)current_proc->pa9=obj;

	/* now do the player # indicator part of the cursor */
	if ((obj=get_object())==NULL)
		ermac(5);
	obj->osize.yx=0x000a000a;

	obj->opal=get_fore_pal(WHITE_p);

	obj->oid=current_proc->pdata.p_store4;
	obj->ozval=2;

	/* load correct number */
	fptr=font15_num+(LONG)current_proc->pdata.p_cursor_num;
	current_proc->a5=pa0->pd_y;

	/* curs5, setup object for display */
	obj->header.tpage=font_lists[FONT_15].f_tpage;
	obj->header.t_xoffset=fptr->u0;
	obj->header.t_yoffset=fptr->v0;
	obj->header.t_width=fptr->w;
	obj->header.t_height=fptr->h;

	(OBJECT *)((OBJECT *)(current_proc->pa9))->oslink=obj;

	/* set coords */
	obj->oxpos.u.intpos=pa4->m_xpos+22-3-8+5;
	obj->oypos.u.intpos=pa4->m_ypos+current_proc->a5-10;

	insert_object(obj,&objlst);

	obj->oslink=NULL;
	current_proc->pa8=obj=(OBJECT *)current_proc->a10;				// a8 =1st object of cursor

	if (mode_of_play==1) 
	{
		current_proc->a10=0xffff;
		if (current_proc->procid==PID_P2SEL)
			current_proc->a10=PID_P1SEL;

		if (current_proc->procid==PID_P4SEL)
			current_proc->a10=PID_P3SEL;

		if (current_proc->a10!=0xffff) 
		{
			do
			{
				process_sleep(1);
			}
			while(process_exist(current_proc->a10,0xffff)!=NULL);
		}
	}

	/*********************************/
	/* cmove2, scan joystick buttons */
	/*********************************/

	process_sleep(0x20);
	current_proc->a10=0;			// joy debounce

	/* cmove3 */
	while (TRUE)
	{
		current_proc->p_comp_flag=PC_CURSOR_LOOP;
		process_sleep(1);
		current_proc->p_comp_flag=PC_CLEAR;

		/* psel_cursor_loop */
		if (current_proc->a10!=0)
			current_proc->a10--;

		/* cmove4 */
		if (current_proc->procid==PID_P1SEL)
		{
			if ((*current_proc->pdata.p_joyport) & pa0->pd_action)
			{
				cmove9();
			}
		}
		else
		{
			if ((*current_proc->pdata.p_joyport) & pa0->pd_action)
			{
				cmove9();
			}
		}

		/* cmove41 */
		if ((jpad=JOYSTICK_IN_A0(current_proc))!=0 && current_proc->a10==0) 	// any joy movement and debounce speed
		{
			current_proc->a10=12;					// debounce speed
			current_proc->a1=*((WORD *)(current_proc->pdata.p_mychar));	// get current ochar

			/* joystick up */
			if ((jpad & MASK_JUP) && is_move_legal(upsans)==SYSTEM_CARRY_SET)
			{
				new_cursor_position(up_change,0,-Y_SPACE);
			}
			else
			{
				/* joystick down */
				if ((jpad & MASK_JDOWN) && is_move_legal(dnsans)==SYSTEM_CARRY_SET)
				{
					new_cursor_position(down_change,0,Y_SPACE);
				}
				else
				{
					/* joystick left */
					if ((jpad & MASK_JLEFT) && is_move_legal(lfsans)==SYSTEM_CARRY_SET)
					{
						new_cursor_position(left_change,-X_SPACE,0);
					}
					else
					{
						/* joystick right */
						if ((jpad & MASK_JRIGHT) && is_move_legal(rtsans)==SYSTEM_CARRY_SET)
						{
							new_cursor_position(right_change,X_SPACE,0);
						}
					}
				}
			}
		}
	}
}

extern WORD HDKITA2_P[];
extern WORD HDJADE2_P[];
extern WORD HDMILE2_P[];
extern WORD HDSCOR2_P[];
extern WORD HDREPT2_P[];
extern WORD HDRMAC2_P[];
extern WORD HDZERP2_P[];
extern WORD black_p[];

void *ochar_picked_palettes[]=
{
	HDKANOD_P,
	HDSOYND_P,
	HDJAXD_P,
	HDINDYD_P,
	HDSUBZD_P,
	HDSWATD_P,
	HDBRDED_P,
	HDKETCD_P,
	HDMUSTD_P,
	HDLAOD_P,
	HDSANDD_P,
	HDGOROD_P,
	HDSHAGD_P,
	HDKANGD_P,
	HDSMOK_P,

	HDKITA2_P,
	HDJADE2_P,
	HDMILE2_P,

	HDSCOR2_P,
	HDREPT2_P,
	HDRMAC2_P,
	HDZERP2_P,

	black_p,
	black_p,
	black_p,
};

/******************************************************************************
 Function: void cmove9(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description: cursor select button has been pushed, lets finish it
******************************************************************************/
void cmove9(void)
{
	PROCESS *ptemp=NULL;
	PDDATA *pa0;

	(WORD *)current_proc->a2=((WORD *)current_proc->pdata.p_mychar);		// my choice
	current_proc->a0=*((WORD *)current_proc->a2);		// my choice

	if (current_proc->procid==PID_P1SEL)
	{
		if ( !(f_speech & 1) )
		{
			ptemp=CREATE(PID_SELSPEECH,select_speech);
			ptemp->a10=current_proc->a0;
			f_speech|=1;
		}
	}
	else
	{
		if ( !(f_speech & 2) )
		{
			ptemp=CREATE(PID_SELSPEECH,select_speech);
			ptemp->a10=current_proc->a0;
			f_speech|=2;
		}
	}

	ptemp=exprc_er(PID_MASTER);			// a0=master process else
	current_proc->a1=ptemp->pdata.p_store1;		// a1= 1st mugshot object

	current_proc->a0=current_proc->pdata.p_store1=*((WORD *)current_proc->a2);

	current_proc->a2=current_proc->a1;
	/* cmovea */
	do
	{
		current_proc->a1=current_proc->a2;
		(void *)current_proc->a4=(void *)(((OBJECT *)current_proc->a1)->oshape);
		(void *)current_proc->a2=((OBJECT *)current_proc->a1)->oslink;
	}
	while(current_proc->a0!=current_proc->a4);

	/* cmoveb, flash the mugshot */
	current_proc->pa8=(OBJECT *)current_proc->a1;

	cursor_sound(0xc,0xd);

	current_proc->a6=*(pa0->pd_p1char);
	current_proc->a0=pa0->pd_pid;

	/* look2 */
#if SONY_PSX
	/* wait until fighter data has been loaded before victory */
	while (!f_fighter_load)
		process_sleep(1);

	process_sleep(2);				// give other procs chances to setup
#endif

	ptemp=exprc_er(current_proc->a0);
	fastxfer(ptemp,psel_victory_animation);

	current_proc->a0=current_proc->a1;

	if ((ptemp=process_exist(current_proc->a0,-1))!=NULL)			// other proc flashing?
	{
		if (ptemp->p_comp_flag==PC_FLASH_WAKE) 								// is he flashing a cursor to, yes
		{
			if (current_proc->pa8==ptemp->pa8)
			{
				dallobj(ptemp->pdata.p_store4);						// kill his cursor objects
				process_kill(ptemp);								// yes, sorry there can be only one
			}
		}
	}

	/* look3 */
	current_proc->a11=6;

	/* look4 */
	do
	{
		flash_sleep(bpal_white_P);								// mod from arcade, flash white then flash normal color

		(OIMGTBL *)current_proc->a0=((OIMGTBL *)((current_proc->pa8)->oimg))+1;
		current_proc->a0=GET_LONG(current_proc->a0);
		flash_sleep((void *)current_proc->a0);		// mod from arcade, flash normal pal

	}
	while(--(current_proc->a11)>0 );

	swpal(ochar_picked_palettes[current_proc->pdata.p_store1],current_proc->pa8);		// dim down

	dallobj(current_proc->pdata.p_store4);

	switch (mode_of_play)
	{
		case 0:			// picked_1_on_1
			if (current_proc->procid==PID_P1SEL)
				p1_state=PS_POST_PSEL;
			else p2_state=PS_POST_PSEL;
			break;
		case 1:			// picked_2_on_2
			if (current_proc->procid==PID_P1SEL || current_proc->procid==PID_P3SEL)
				process_suicide();

			if (current_proc->procid==PID_P2SEL)
				p1_state=PS_POST_PSEL;

			if (current_proc->procid==PID_P4SEL)
				p2_state=PS_POST_PSEL;
			break;
		case 3:			// picked tournament
			p1_tour[(WORD)current_proc->pdata.p_hit]=*((WORD *)current_proc->pdata.p_mychar);
			current_proc->a0=current_proc->pdata.p_hit+2;
			if (current_proc->a0==8 || current_proc->a0==9)
				process_suicide();

			current_proc->pdata.p_hit=current_proc->a0;

			stuff_number_shape(current_proc->a0);

			replace_drone();

			break;
	}


	/* cmoved */
	process_suicide();
}

/******************************************************************************
 Function: void flash_sleep(void *pal)

 By: David Schwartz

 Date: Jan 1995

 Parameters: pal - palette to flash

 Returns: None

 Description: flash a palette and sleep a bit
******************************************************************************/
void flash_sleep(void *pal)
{
	swpal(pal,current_proc->pa8);
	current_proc->p_comp_flag=PC_FLASH_WAKE;
	process_sleep(2);
	current_proc->p_comp_flag=PC_CLEAR;
	return;
}

/******************************************************************************
 Function: WORD is_move_legal(WORD *dirtbl)

 By: David Schwartz

 Date: Jan 1995

 Parameters: dirtbl - ptr of words to legal moves
			 current_proc->a1 - my current ochar

 Returns: SYSTEM_CARRY_SET - move is legal
		  SYSTEM_CARRY_CLR - move is not legal

 Description: determine if a joystick move is legal
******************************************************************************/
WORD is_move_legal(WORD *dirtbl)
{
	WORD ochar;
	ochar=a0_ochar_to_position(current_proc->a1);

	if ((GET_WORD(dirtbl)++)==ochar)		// tbl entry 0
		return(SYSTEM_CARRY_CLR);
	if ((GET_WORD(dirtbl)++)==ochar)		// tbl entry 1
		return(SYSTEM_CARRY_CLR);
	if ((GET_WORD(dirtbl)++)==ochar)		// tbl entry 2
		return(SYSTEM_CARRY_CLR);
	if ((GET_WORD(dirtbl)++)==ochar)		// tbl entry 3
		return(SYSTEM_CARRY_CLR);
	if ((GET_WORD(dirtbl)++)==ochar)		// tbl entry 4
		return(SYSTEM_CARRY_CLR);
	if ((GET_WORD(dirtbl)++)==ochar)		// tbl entry 4
		return(SYSTEM_CARRY_CLR);
	if ((GET_WORD(dirtbl)++)==ochar)		// tbl entry 4
		return(SYSTEM_CARRY_CLR);

	return(SYSTEM_CARRY_SET);
}

/******************************************************************************
 Function: WORD a0_ochar_to_position(WORD ochar)

 By: David Schwartz

 Date: Jan 1995

 Parameters: ochar - character to search for

 Returns: position value in mugshot table

 Description: determine character position in mugshot table
******************************************************************************/
WORD a0_ochar_to_position(WORD ochar)
{
	short loop=-1;

	while(mug_shot_table[++loop].m_char!=ochar);

	return(loop);
}

/******************************************************************************
 Function: void new_cursor_calla(WORD *tbl,short xoff,short yoff)

 By: David Schwartz

 Date: Jan 1995

 Parameters: tbl - pos change tbl for new spot
			 xoff - coord delta x
			 yoff - coord delta y

 Returns: None

 Description: handle moving cursor to a new position
******************************************************************************/
void new_cursor_calla(WORD *tbl,short xoff,short yoff)
{
	OBJECT *obj;
	short xadj;

	current_proc->a1=*(tbl+a0_ochar_to_position(current_proc->a1));			// new position

	xadj=xoff;
	/* skip center spot */
	if ( (current_proc->a1==8 && f_mileena==0) || (current_proc->a1==9 && f_ermac==0) || (current_proc->a1==10 && f_oldsz==0))
	{
		if (tbl==up_change)
		{
			yoff<<=1;				// double dy
			current_proc->a1-=6; // force new spot
		}
		else
		{
			if (tbl==down_change)
			{
				yoff<<=1;				// double dy
				current_proc->a1+=5; // force new spot
			}
			else
			{
				if (tbl==left_change)
				{
					xadj=xoff;

				   	if (current_proc->a1==10) 
				   	{
				   		if (f_oldsz)
							goto NCP3;

						xadj+=xoff;
						current_proc->a1--;
				   	}

				   	if (current_proc->a1==9) 
				   	{
				   		if (f_ermac)
							goto NCP3;

						xadj+=xoff;
						current_proc->a1--;
				   	}

				   	if (current_proc->a1==8) 
				   	{
				   		if (f_mileena)
							goto NCP3;

						xadj+=xoff;
						current_proc->a1--;
				   	}
				}
				else
				{
					if (tbl==right_change)
					{
						xadj=xoff;

				   		if (current_proc->a1==8) 
				   		{
				   			if (f_mileena)
								goto NCP3;

							xadj+=xoff;
							current_proc->a1++;
				   		}

				   		if (current_proc->a1==9) 
				   		{
				   			if (f_ermac)
								goto NCP3;

							xadj+=xoff;
							current_proc->a1++;
				   		}

				   		if (current_proc->a1==10) 
				   		{
				   			if (f_oldsz)
								goto NCP3;

							xadj+=xoff;
							current_proc->a1++;
				   		}

					}
				}
			}

		}
	}

NCP3:
	/* ncp3, position --> ochar */
	xoff=xadj;

	*((WORD *)current_proc->pdata.p_mychar)=mug_shot_table[current_proc->a1].m_char;

	cursor_sound(0xc,0xd);

	obj=current_proc->pa8;
	do
	{
		obj->oxpos.u.intpos+=xoff;
		obj->oypos.u.intpos+=yoff;
		obj=(OBJECT *)(obj->oslink);
	}
	while(obj!=NULL);

	replace_drone();
}

void replace_drone(void) 
{
	PROCESS *pa5,*pa13;

	/* replace drone */
	pa5=(PROCESS*)current_proc->pdata.p_store7;


	pa13=current_proc;
	current_proc=pa5;
	delete_slave();
	current_proc=pa13;

	process_kill((current_proc->pa8)->oslink);
	delobjp(current_proc->pa8);

	pa13=process_create(PID_P1,psel_drone);
	pa13->pdata.p_store8=pa5->pdata.p_store8;		// pass info to new prpc
	current_proc->pdata.p_store7=(ADDRESS)pa13;

	process_kill(pa5);

	return;
}

/******************************************************************************
 Function: void psel_drone(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description: display fighter that cursor currently on, removing any old processes in the process
******************************************************************************/
void psel_drone(void)
{
	PDDATA *pa0;
	OBJECT *obj;

#if SONY_PSX
	/* wait until fighter data has been loaded */
	while (!f_fighter_load)
		process_sleep(1);
#endif

	wait_forever();

	pa0=(PDDATA *)current_proc->pdata.p_store8;
	current_proc->procid=pa0->pd_pid;

	obj=make_ochar_psel_guy(*(pa0->pd_p1char),SCRRGT-SCX(0x30));
	obj->ozval=0x1000+pa0->pd_zval;
	obj->oxpos.u.intpos=pa0->pd_x;

	if (pa0->pd_joy==&swcurr.u.p2pad)
		flip_multi(obj);

	current_proc->pdata.p_slave=NULL;

	stance_setup();
	current_proc->pdata.p_ganiy=obj->oypos.u.intpos;
	next_anirate();

	/* psel_drone_loop */
	while (TRUE)
	{
		process_sleep(1);

		next_anirate();
		if (obj->oid==2)			// player 2 check pal
		{
			/* player 2 --> check if alt palette is needed */
			if ((obj->oflags2 & M_UGLY_PAL) == 0)
			{
				if (p1_char==obj->ochar) 		// player 1 same char as me ?, yes
				{
					obj->oflags2|=M_UGLY_PAL;

					/* psd9 */
					player_swpal(current_proc,character_palettes_2[obj->ochar],obj);
				}
			}
			else
			{
				/* psd8 */
				if (p1_char!=obj->ochar)		// player 1 same char as me ?, no
				{
					obj->oflags2&=(~M_UGLY_PAL);
					player_swpal(current_proc,character_palettes_1[obj->ochar],obj);
				}
			}
		}
	}
}

OBJECT *make_ochar_psel_guy(WORD pa1,short pax) 
{
	return(NULL);
}

/******************************************************************************
 Function: void select_speech(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: current_proc->a10=fighter

 Returns: None

 Description: make a speech call for selected fighter
******************************************************************************/
void select_speech(void)
{
	PROCESS *ptemp;

	current_proc->p_comp_flag=PC_SPEECH_WAKE;
	process_sleep(8);
	current_proc->p_comp_flag=PC_CLEAR;

SELSP1:
	if ((ptemp=process_exist(PID_SELSPEECH,-1))!=0) 		// dont intrupt another process
	{
		if (ptemp->p_comp_flag!=PC_SPEECH_WAKE)
		{
			process_sleep(1);
			goto SELSP1;
		}
	}

	triple_sound(current_proc->a10+0x28);
	process_sleep(0x30);

	process_suicide();
}

/******************************************************************************
 Function: void cursor_sound(WORD p1_snd,WORD p2_snd)

 By: David Schwartz

 Date: Feb 1995

 Parameters: p1_snd - player 1 sound
			 p2_snd - player 2 sound

 Returns: None

 Description: make a cursor sound
******************************************************************************/
void cursor_sound(WORD p1_snd,WORD p2_snd)
{
	if (current_proc->procid==PID_P1SEL)
		triple_sound(p1_snd);
	else triple_sound(p2_snd);

	return;
}

/******************************************************************************
******************************************************************************/
BYTE p1_kano[]={MVRT,MVDN,MVDN,MVDN,MVUP,MVUP,0};
BYTE p1_sonya[]={MVRT,MVRT,MVRT,MVRT,MVLF,MVLF,0};
BYTE p1_jax[]={MVRT,MVRT,MVRT,MVLF,MVRT,MVLF,MVRT,0};
BYTE p1_ind[]={MVRT,MVRT,MVRT,MVRT,MVRT,MVLF,0};
BYTE p1_sz[]={MVRT,MVDN,MVDN,MVRT,MVRT,MVRT,MVRT,MVUP,0};
BYTE p1_swat[]={MVRT,MVDN,MVDN,MVRT,MVRT,0};
BYTE p1_lia[]={MVRT,MVDN,MVDN,MVRT,0};
BYTE p1_robo1[]={MVRT,MVDN,MVDN,MVDN,MVUP,0};
BYTE p1_robo2[]={MVRT,MVRT,MVRT,MVRT,MVRT,MVDN,MVDN,MVLF,0};
BYTE p1_lao[]={MVRT,MVRT,MVRT,MVRT,MVRT,MVDN,MVDN,0};
BYTE p1_tusk[]={MVRT,MVDN,MVDN,MVDN,0};
BYTE p1_sg[]={MVRT,MVDN,MVDN,MVDN,MVRT,0};
BYTE p1_st[]={MVRT,MVDN,MVDN,MVDN,MVRT,MVRT,0};
BYTE p1_lk[]={MVRT,MVDN,MVDN,MVDN,MVRT,MVRT,MVRT,0};
BYTE p1_smoke[]={MVRT,MVDN,MVDN,MVDN,MVRT,MVRT,MVRT,MVRT,0};
BYTE p1_kit[]={MVRT,MVDN,MVUP,MVRT,MVLF,MVLF,0};
BYTE p1_jad[]={MVRT,MVRT,MVRT,MVRT,MVRT,MVDN,MVUP,0};
BYTE p1_mil[]={MVRT,MVDN,MVRT,MVUP,MVDN,0};
BYTE p1_scorp[]={MVRT,MVRT,MVRT,MVRT,MVRT,MVDN,MVUP,MVRT,0};
BYTE p1_rep[]={MVRT,MVDN,MVDN,MVDN,MVUP,MVUP,MVUP,0};
BYTE p1_ermac[]={MVRT,MVRT,MVRT,MVDN,0};
BYTE p1_osz[]={MVRT,MVRT,MVRT,MVRT,MVDN,0};

BYTE p2_kano[]={MVLF,MVLF,MVLF,MVLF,MVLF,MVDN,0};
BYTE p2_sonya[]={MVLF,MVLF,MVLF,MVLF,0};
BYTE p2_jax[]={MVLF,MVLF,MVLF,MVLF,MVRT,0};
BYTE p2_ind[]={MVLF,MVDN,MVUP,MVLF,0};
BYTE p2_sz[]={MVLF,MVDN,MVDN,MVDN,MVUP,MVUP,0};
BYTE p2_swat[]={MVLF,MVDN,MVDN,MVLF,MVLF,0};
BYTE p2_lia[]={MVLF,MVDN,MVDN,MVLF,MVLF,MVLF,0};
BYTE p2_robo1[]={MVLF,MVLF,MVLF,MVLF,MVLF,MVDN,MVDN,0};
BYTE p2_robo2[]={MVLF,MVDN,MVDN,MVLF,0};
BYTE p2_lao[]={MVLF,MVDN,MVDN,MVDN,MVUP,0};
BYTE p2_tusk[]={MVLF,MVLF,MVLF,MVLF,MVLF,MVDN,MVDN,MVDN,0};
BYTE p2_sg[]={MVLF,MVLF,MVLF,MVLF,MVLF,MVDN,MVDN,MVDN,MVRT,0};
BYTE p2_st[]={MVLF,MVDN,MVDN,MVDN,MVLF,MVLF,0};
BYTE p2_lk[]={MVLF,MVDN,MVDN,MVDN,MVLF,0};
BYTE p2_smoke[]={MVLF,MVDN,MVDN,MVDN,MVLF,MVRT,0};
BYTE p2_kit[]={MVLF,MVLF,MVLF,MVLF,MVLF,MVLF,0};
BYTE p2_jad[]={MVLF,MVLF,MVLF,MVRT,MVRT,0};
BYTE p2_mil[]={MVLF,MVLF,MVLF,MVLF,MVDN,0};
BYTE p2_scorp[]={MVLF,MVDN,MVDN,MVUP,MVUP,MVRT,0};
BYTE p2_rep[]={MVLF,MVLF,MVLF,MVLF,MVLF,0};
BYTE p2_ermac[]={MVLF,MVLF,MVLF,MVDN,0};
BYTE p2_osz[]={MVLF,MVDN,MVDN,MVUP,0};

BYTE *p1_movements[]=
{
	p1_kano,
	p1_sonya,
	p1_jax,
	p1_ind,
	p1_sz,
	p1_swat,
	p1_lia,
	p1_robo1,
	p1_robo2,
	p1_lao,
	p1_tusk,
	p1_sg,
	p1_st,
	p1_lk,
	p1_smoke,
	p1_kit,
	p1_jad,
	p1_mil,
	p1_scorp,
	p1_rep,
	p1_ermac,
	p1_osz,
};

BYTE *p2_movements[]=
{
	p2_kano,
	p2_sonya,
	p2_jax,
	p2_ind,
	p2_sz,
	p2_swat,
	p2_lia,
	p2_robo1,
	p2_robo2,
	p2_lao,
	p2_tusk,
	p2_sg,
	p2_st,
	p2_lk,
	p2_smoke,
	p2_kit,
	p2_jad,
	p2_mil,
	p2_scorp,
	p2_rep,
	p2_ermac,
	p2_osz,
};


/******************************************************************************
 Function: void auto_player_select(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description: auto player select
******************************************************************************/
void auto_player_select(void)
{
	PROCESS *ta0;
	OBJECT *ta8;

	if ((ta0=process_exist(PID_MASTER,-1))!=NULL)
	{
		if (ta0->p_comp_flag==PC_PSEL_WAIT)
			ta0->a11=0x40*5;				// tell him to wait nol longer
	}

	/* aps0 */
	if ((ta0=process_exist(PID_AUTO_CYCLE,-1))==NULL)
	{
		ta0=CREATE(PID_AUTO_CYCLE,boonpal_stuff);
		ta0->a11=(ADDRESS)fl_auto;
	}

	/* aps1 */
	//tsound(0xf8);

	/* asp6 */
	ta8=current_proc->pa8;
	if (current_proc->pdata.p_mychar==FT_KITANA)
		lm_printf(pf_auto_msg_1);
	else lm_printf(pf_auto_msg_2);
	current_proc->pa8=ta8;

	process_sleep(4);

	/* asp_try_again */
	do
	{
		current_proc->a0=randu(22)-1;
		if ((current_proc->a0==FT_MILEENA && f_mileena==0) || (current_proc->a0==FT_ERMAC && f_ermac==0) || (current_proc->a0==FT_OLDSZ && f_oldsz==0))
			continue;
		else break;
	}
	while(1);

	/* asp9 */
	(BYTE *)current_proc->a11=(current_proc->pdata.p_mychar==FT_KITANA) ? p1_movements[(current_proc->a0)] : p2_movements[(current_proc->a0)];

	/* aps3 */
	while (TRUE)
	{
		(BYTE)current_proc->a0=*((BYTE *)current_proc->a11);
		((BYTE *)current_proc->a11)++;

		switch ((BYTE)current_proc->a0)
		{
			case 0:
				process_sleep(9);
				stack_jump(cmove9);
				break;
			case MVUP:		// auto_move_up
				new_auto_position(up_change,0,-Y_SPACE);
				break;
			case MVDN:	// auto_move_down
				new_auto_position(down_change,0,Y_SPACE);
				break;
			case MVRT:	// auto_move_right
				new_auto_position(right_change,X_SPACE,0);
				break;
			case MVLF:	// auto_move_left
				new_auto_position(left_change,-X_SPACE,0);
				break;
		}
	}
	return;
}

/******************************************************************************
 Function: void new_auto_position(WORD *pa2,short pa3,short pa4)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa2 - pos change table
			 pa3 - coord dx
			 pa4 - coord dy

 Returns: None

 Description: select new position
******************************************************************************/
void new_auto_position(WORD *pa2,short pa3,short pa4)
{
	current_proc->a1=*((WORD *)current_proc->pdata.p_mychar);		// a1 = my current ochar
	new_cursor_calla(pa2,pa3,pa4);
	process_sleep(0x0a);

	return;
}

/******************************************************************************
******************************************************************************/

/******************************************************************************
 Function: void position_badge(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	print the winners badge and spin the coin
******************************************************************************/
void position_badge(void)
{
	OBJECT *obj;

	if (winner_status==0)
		return;

	if (winner_status==1)
	{
		if (p1_state!=PS_ACTIVE)
			return;
	}
	else
	{
		if (p2_state!=PS_ACTIVE)
			return;							// winner drone: no stinkin badge
	}

	/* mint3 */
	if (winner_status==1)
	{
		gso_dmawnz_ns_coor(obj,(ADDRESS)MKCOIN_01,psel_anims,0,SCX(0x000d),(0x0010)+3);
	}
	else
	{
		gso_dmawnz_ns_coor(obj,(ADDRESS)MKCOIN_01,psel_anims,0,SCX(0x0153),(0x0010)+3);
	}

	alloc_cache(MKCOIN_01,&psel_anims,obj);
	insert_object(obj,&objlst);
	CREATE(PID_BANI,badge_spinner);

#if 0
	/* print wins in a row */
	if (winner_status==1)
	{
		current_proc->a6=SCX(0x22);				// x pos for p1
		ta7=&p1_wiar;
	}
	else
	{
		current_proc->a6=SCX(0x16c);				// x pos for p2
		ta7=&p2_wiar;
	}

	if (*ta7==1)
	{
		lm_setup(pf_new_champ);
		fnt_state.fnt_posx=current_proc->a6;
		mk_printf(txt_new_champ);
	}
	else
	{
		lm_setup(pf_iar_num);
		fnt_state.fnt_posx=current_proc->a6;
		printf_p1(txt_iar_num,*ta7);
	}
#endif
	return;

}

/******************************************************************************
 Function: void badge_spinner(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	process to spin the pretty coin
******************************************************************************/
void badge_spinner(void)
{
	current_proc->pa9=a_badge_spin;
	framew(5);

	wait_forever();
}

/******************************************************************************
******************************************************************************/
extern FNTSETUP pf_bk_setup[];
/******************************************************************************
 Function: void select_background(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	select a valid background
******************************************************************************/
void select_background(void)
{
	OBJECT *obj;
	WORD jpad;
	WORD oldback=0xffff;

	/* setup joy */
	dallobj(OID_TEXT);			// remove old text

	p15_centered(txt_select_back,SCX(0xc8),SCY(0xb)+8);

	obj=(OBJECT *)&objlst2;
	while((obj=obj->olink)!=NULL)
		obj->oid=OID_R1;				// change all text id --> oid_r1

	current_proc->a10=0;	// debounce
	current_proc->pdata.p_store5=0x60*5;					// time to select kombat zone

	do
	{
		jpad=swcurr.u.p1pad|swcurr.u.p2pad;
		if (current_proc->a10!=0)
			current_proc->a10--;

		if ((jpad & MASK_JLEFT) && current_proc->a10==0)
		{
			/* down # */
			current_proc->a10=12;

			curback--;
			if ((short)curback<0)
				curback=MAX_BACKGROUNDS;

		}
		else
		{
			if ((jpad & MASK_JRIGHT) && current_proc->a10==0)
			{
				/* up # */
				current_proc->a10=12;

				curback++;

				if (curback>MAX_BACKGROUNDS)
					curback=0;
			}
		}

		if (oldback!=curback)
		{
			oldback=curback;
			dallobj(OID_TEXT);
			lm_setup(pf_bk_setup);
			mk_printf(background_names[curback]);
		}
		process_sleep(1);
	}
	while((jpad & (P1B0|P1B2|P1B3|P1SF2|P1B1|P1B4))==0 && (--current_proc->pdata.p_store5>0));

	tsound(0xd);

	dallobj(OID_TEXT);
	dallobj(OID_R1);
	return;
}
