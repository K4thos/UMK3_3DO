/******************************************************************************
 File: mkamode.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III ATTRACT MODE ROUTINES
******************************************************************************/

/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif

#include "mkbkgd.h"
#include "mkobj.h"
#include "mkamode.h"
#include "mkos.h"
#include "mkgame.h"
#include "mkutil.h"
#include "mkani.h"
#include "mkmain.h"
#include "valloc.h"
#include "mktext.h"
#include "mkvs.h"
#include "mkdiag.h"
#include "mkfade.h"
#include "mkrepell.h"
#include "mkscore.h"
#include "mksound.h"
#include "mkbio.h"
#include "mkinit.h"
#include "mkend.h"
#include "mkhstd.h"

/* local extern info */
extern void sony_graphics_setup(void);
extern void enter_initials(void);
void legal_page(void);
void mode_select(void);

extern FNTSETUP pf_15point[];
extern WORD f_load;
/******************************************************************************
 Function: void amode(void)

 By: David Schwartz

 Date: Aug 11, 1994

 Parameters: None

 Returns: None

 Description:	This is the main attract mode process
******************************************************************************/
void amode(void)
{
	int i;

	amode_init();
	gstate=GS_AMODE;

	/* amode_loop */
	do
	{
		/* load sound stuff */
		sound_module_load(SMOD_SELECT);

		midway_presents();

		legal_page();
		title_page();
		//-DHS- fast_edit_sequence();
		quote();
		print_rankings();
		amode_demo_fight();
		
		ultimate_bio(FT_KITANA);
		ultimate_bio(FT_JADE);
		print_rankings();
		amode_demo_fight();

		ultimate_bio(FT_REPTILE);
		ultimate_bio(FT_SCORPION);
	}
	while(1);

}

/******************************************************************************
 Function: void quote(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	words of wisdom
******************************************************************************/
extern char txt_quote1[];
extern char txt_quote2[];
char *quote_table[]=
{
	txt_quote1,
	txt_quote2
};
void quote(void)
{
	sony_graphics_setup();
	amode_init_no_score();
	pds_centered(quote_table[randu(2)-1],SCX(0xc8),SCY(0x70));
	fadein_jsrp(0x40*2);
}

/******************************************************************************
 Function: void midway_presents(void);

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	midway presents
******************************************************************************/
char txt_midway_presents[]="WILLIAMS ENTERTAINMENT PRESENTS";
void midway_presents(void)
{
	short r,g,b;

	nosounds();

	load_bkgd=BKGD_MK3_TITLE;				// set system up to for title screen stuff (don't want music intrupted later)
	sony_graphics_setup();
	bkgd_texture_load(bkgd_base_x,BACK_TEXTURE_BASE_Y);
	play_generic_tune(TRK_ATTRACT);			// send_code_a3(0x3e)

	process_sleep(1);

	f_load=0;
	amode_init_no_score();
	pds_centered(txt_midway_presents,SCX(0xc8),SCY(0x70));
	fadein_jsrp(0x40*2);

	return;

	f_auto_erase=0;
	db[0].draw.isbg=1;
	db[1].draw.isbg=1;
	r=0x0;
	b=0x0;
	g=0x0;

	do
	{
		process_sleep(4);
		do
		{
       	 	b = (b+32)%256;
			do
			{
       	 		g = (g+32)%256;
				do
				{
       	 			r = (r+32)%256;
					process_sleep(2);
       				setRGB0(&cdb->draw, r, g, b); /* set background color */
				}
				while(r!=0 );
			}
			while(g!=0 );
		}
		while(b!=0 );
	}
	while(1);

	return;
}

/******************************************************************************
 Function: void legal_page(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	legal page
******************************************************************************/
extern void *legal_module[];
void legal_page(void)
{

	/* legal_page */
	object_initialize();
	load_bkgd=BKGD_MK3_LEGAL;
	init_background_module(legal_module);
	multi_plane();

	process_sleep(0x40*5);
	smooth_black_retp();

	return;
}

/******************************************************************************
 Function: void title_page(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	title page
******************************************************************************/
extern void *title_module[];
void title_page(void)
{
	object_initialize();

	load_bkgd=BKGD_MK3_TITLE;
	init_background_module(title_module);
	multi_plane();

	process_sleep(0x40*5);

	smooth_black_retp();

	return;
}

/******************************************************************************
 Function: void amode_demo_fight(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	show a small demo of what we got
******************************************************************************/
WORD amode_demo_matchups[]=
{
	FT_KITANA,FT_SCORPION,
	FT_JADE,FT_REPTILE,
	FT_KITANA,FT_KANO,
	FT_JADE,FT_SONYA,
	FT_SCORPION,FT_JAX,
	FT_REPTILE,FT_INDIAN,
	FT_KITANA,FT_SUBZERO,
	FT_JADE,FT_SWAT,
	FT_SCORPION,FT_LIA,
	FT_REPTILE,FT_ROBO1,
	FT_KITANA,FT_ROBO2,
	FT_JADE,FT_LAO,
	FT_SCORPION,FT_TUSK,
	FT_REPTILE,FT_SG,
	FT_KITANA,FT_ST,
	FT_JADE,FT_LK,
	FT_SCORPION,FT_SMOKE,
	FT_MILEENA,FT_ERMAC,
	FT_NOOB,FT_OLDSMOKE
};

void amode_demo_fight(void)
{
	WORD t_a0;

	amode_init_no_score();

	t_a0=(randu(19)-1)<<1;
	p1_char=amode_demo_matchups[t_a0];
	p2_char=amode_demo_matchups[t_a0+1];

	if ( !f_no_vs )
		vs_or_ladder();
	CREATE(PID_AMODE,amode_demo_game);
	process_sleep(6);
	CREATE(PID_REPELL,repell);

	process_sleep(0x40*12);

	MURDER;
	/* freeze_2_pages modified fixes wierd bugs */
	f_novel=1;
	scrolly.pos=0;
	p1_heap_char=p2_heap_char=0xffff;
	stop_scrolling();
	idcomp_ptr=idcomp_base=image_dcomp_buffer;			// reset buffer
	send_code_a3(0);
	play_ending_chord();
	process_sleep(0x30);

	amode_fade_retp();

	sound_module_load(SMOD_SELECT);

	return;
}

/******************************************************************************
 Function: void amode_init(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	Initialize game for attact mode
******************************************************************************/
void amode_init(void)
{
	WORD loop;

	mode_of_play=0;

	current_proc->pdata.p_hit=4;
	c_amodeloop=0;

	/* ainit4 */
	current_proc->pdata.p_action=randu(7)-1;

	update_pmsg();

	coinflag=0;

	/* setup ram for hidden codes */
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
	return;
}

/******************************************************************************
 Function: void amode_shutup(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	attact mode be quiet
******************************************************************************/
#if 0
void amode_shutup(void)
{
	nosounds();
	f_nosound=1;
	current_proc->pdata.p_hit=0;
	return;
}
#endif

/******************************************************************************
 Function: void amode_init_no_score(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	attact mode setup and dont show score board
******************************************************************************/
void amode_init_no_score(void)
{
	amode_oinit();
	DONT_SHOW_SCORES;
	f_load=0;
}

/******************************************************************************
 Function: void amode_oinit(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	attact mode setup
******************************************************************************/
void amode_oinit(void)
{
	murder_myoinit_score();

	dlists=dlists_bogus;

	irqskye=0;
	noflip=0;
	f_doscore=1;
	return;
}

/******************************************************************************
 Function: void amode_init_system(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	attact system setuyp
******************************************************************************/
void amode_init_system(void)
{
	murder_myoinit_score();
//	clr_scrn();

	irqskye=0;
	noflip=0;
//	f_doscore=1;
	f_auto_erase=1;
	DONT_SHOW_SCORES;
	nosounds();
	clr_scrn();
	process_sleep(2);
	return;
}

/******************************************************************************
 Function: void gover_amode_entry(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:	game over attract mode entry point,
******************************************************************************/
void gover_amode_entry(void)
{
	PROCESS *ptemp;

	ptemp=CREATE(PID_FADE,skydown);
	(long)(ptemp->pa9)=0x800;
	amode_fade_retp();

	amode_init();
	gstate=GS_AMODE;

	stack_jump(amode);
}

/******************************************************************************
 Function: void amode_fade_retp(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	attact mode fade and return to caller
******************************************************************************/
void amode_fade_retp(void)
{
	fadeout_jsrp(0x20);
	smooth_black_retp();
}

/******************************************************************************
 Function: void smooth_black_retp(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	smooth black fade transition
******************************************************************************/
void smooth_black_retp(void)
{
	amode_display_reset();

	process_sleep(0x6);

	return;
}

/******************************************************************************
 Function: void amode_display_reset(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	attact mode display reset routine
******************************************************************************/
void amode_display_reset(void)
{
	clr_scrn();
	murder_myoinit();
	f_doscore=0;					// score bars = no
	f_auto_erase=1;

	return;
}

