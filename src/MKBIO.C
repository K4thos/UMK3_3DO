/******************************************************************************
 File: mkbio.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Bios Setup
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
#include "mkutil.h"
#include "mkani.h"
#include "mkbio.h"
#include "mkfile.h"
#include "mktext.h"
#include "mkvs.h"
#include "mksound.h"

/* local externs */

/******************************************************************************
 Function: void ultimate_bio(WORD p_a0)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  p_a0 - bio to show

 Returns: None

 Description:  lets see a history of our fighter
******************************************************************************/
extern char txt_kitana_bio[];
extern char txt_jade_bio[];
extern char txt_scorpion_bio[];
extern char txt_reptile_bio[];
char *bio_titles[]=
{
	txt_kitana_bio,
	txt_kitana_bio,
	txt_kitana_bio,
	txt_kitana_bio,
	txt_kitana_bio,
	txt_kitana_bio,
	txt_kitana_bio,
	txt_kitana_bio,
	txt_kitana_bio,
	txt_kitana_bio,
	txt_kitana_bio,
	txt_kitana_bio,
	txt_kitana_bio,
	txt_kitana_bio,
	txt_kitana_bio,
	txt_kitana_bio,
	txt_jade_bio,
	txt_kitana_bio,
	txt_scorpion_bio,
	txt_reptile_bio,
	txt_kitana_bio,
	txt_kitana_bio,
	txt_kitana_bio,
	txt_kitana_bio,
};

extern FNTSETUP pf_bio[];

void ultimate_bio(WORD p_a0)
{

	character_texture_load(p_a0,CHAR_VS,p1_heap,SYNC_LOAD);
	current_proc->a0=p_a0;
	slide_in_left(p1_heap);

	process_sleep(4);

	lm_setup(pf_bio);
	freeze_2_pages();
	background_clear();
	f_auto_erase=0;
	text_draw=1;
	mk_printf(bio_titles[p_a0]);
	text_draw=0;

	process_sleep(1);
	noflip=1;

	process_sleep(0x10);
	//-DHS- send_code_a3(0x25);
	process_sleep(0x40*10);
	//-DHS- send_code_a3(0x27);
	process_sleep(0x40*2);

	noflip=0;
	f_auto_erase=1;

	murder_myoinit_score();
	clr_scrn();
	process_sleep(10);
	return;
}

void slide_in_left(void *p_a0) 
{
	WORD t_a0=current_proc->a0;

	DISPLAY_ON;
	murder_myoinit_score();					// display hell background
	do_background(BKGD_FIRE_MOD);

	build_huge_left(p_a0);			// slide in dude
	process_sleep(0x1a);

	f_novel=1;			// stop
	triple_sound(t_a0+0x28);		// say the name

	return;
}

/******************************************************************************
 Function: void mk3_gradient(WORD pa0)

 By: David Schwartz

 Date: May 1995

 Parameters:  None

 Returns: None

 Description:  display fighter gradients
******************************************************************************/
extern void *gradient_txt[];
void mk3_gradient(WORD pa0)
{
	load_bkgd=BKGD_MK3_RGRAD+pa0;								// load correct gradient
	init_background_module(table_o_mods[load_bkgd]);
	vram_texture_load(bkgd_base_x,BACK_TEXTURE_BASE_Y,(ADDRESS*)gradient_txt);		// place textures in memory
	multi_plane();
	return;
}
