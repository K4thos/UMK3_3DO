/******************************************************************************
 File: bkgdsoul.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Soul Routines
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
#include "mkfx.h"
#include "mkani.h"
#include "mkinit.h"
#include "mkpal.h"
#include "mksound.h"
#include "valloc.h"

/* Prototypes */
void calla_soul(void);
void soul_na_do(void);

/************************************************************
 * 			S O U L  S T U F F 							*
 ************************************************************/

/********************
 Externals
 ********************/
extern void *SONADO_P[];
extern void *soul_anims[];
extern void *a_soul_na_do[];
extern void *SONADO1[];
extern void *SONADO2[];
extern void *SONADO3[];
extern void *SONADO4[];
extern void *SONADO5[];
extern void *SONADO6[];
extern void *SONADO7[];
extern void *SONADO8[];
extern void *SONADO9[];
extern void *SONADO10[];
extern void *SONADO11[];
extern void *SONADO12[];

extern void *SOUL_STBREAK3[];
extern void *SOUL_STBREAK2[];
extern void *SOUL_STBREAK1[];

/********************
 ********************/


ADDRESS *soul_perm_list[] =
{
	(ADDRESS *)SONADO1,
	(ADDRESS *)SONADO2,
	(ADDRESS *)SONADO3,
	(ADDRESS *)SONADO4,
	(ADDRESS *)SONADO5,
	(ADDRESS *)SONADO6,
	(ADDRESS *)SONADO7,
	(ADDRESS *)SONADO8,
	(ADDRESS *)SONADO9,
	(ADDRESS *)SONADO10,
	(ADDRESS *)SONADO11,
	(ADDRESS *)SONADO12,
	(ADDRESS *)SOUL_STBREAK1,
	(ADDRESS *)SOUL_STBREAK2,
	(ADDRESS *)SOUL_STBREAK3,
	NULL
};

/******************************************************************************
 Function: void calla_soul(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	setup the souls background
******************************************************************************/
void calla_soul(void)
{
	OBJECT *obj;
	PROCESS *ptemp;

	alloc_vram_perm_bg_list((ADDRESS *)soul_perm_list,&soul_anims);

	/* set soul tornado */
	gso_dmawnz(obj,(ADDRESS)SONADO1,soul_anims,0);
	set_xy_coordinates(obj,0x24b,0x4e);
	alloc_cache(SONADO1,&soul_anims,obj);
	insert_object(obj,&baklst3);

	ptemp=CREATE(PID_BANI,soul_na_do);
	ptemp->pa8=obj;

	return;
}

/******************************************************************************
 Function: void soul_na_do(void)

 By: David Schwartz

 Date:  Jan 1995

 Parameters: None

 Returns: None

 Description:	animate the tornado
******************************************************************************/
void soul_na_do(void)
{
	while (TRUE)
	{
		current_proc->pa9=a_soul_na_do;
		framew(4);
	}
}

/* Prototypes */
void calla_tower(void);

/************************************************************
 * 			B A L C O N Y & T O W E R  S T U F F 			*
 ************************************************************/

/*****************
 Externals
*****************/
extern void *tower_anims[];
extern void *a_torch[];
extern void *BALFIRE1[];
extern void *BALFIRE2[];
extern void *BALFIRE3[];
extern void *BALFIRE4[];
extern void *BALFIRE5[];
extern void *BALFIRE6[];

extern void *TOWER_SMALL01[];
extern void *TOWER_SMALL02[];
extern void *TOWER_SMALL03[];
extern void *TOWER_SMALL04[];
extern void *TOWER_SMALL05[];
extern void *TOWER_SMALL06[];
extern void *TOWER_SMALL07[];
extern void *TOWER_SMALL08[];
extern void *TOWER_SMALL09[];
extern void *TOWER_SMALL10[];
extern void *TOWER_SMALL11[];
extern void *TOWER_SMALL12[];
extern void *TOWER_MEDIUM01[];
extern void *TOWER_MEDIUM02[];
extern void *TOWER_MEDIUM03[];
extern void *TOWER_MEDIUM04[];
extern void *TOWER_MEDIUM05[];
extern void *TOWER_MEDIUM06[];
extern void *TOWER_MEDIUM07[];
extern void *TOWER_MEDIUM08[];
extern void *TOWER_MEDIUM09[];
extern void *TOWER_MEDIUM10[];
extern void *TOWER_MEDIUM11[];
extern void *TOWER_MEDIUM12[];
extern void *TOWER_LARGE01[];
extern void *TOWER_LARGE02[];
extern void *TOWER_LARGE03[];
extern void *TOWER_LARGE04[];
extern void *TOWER_LARGE05[];
extern void *TOWER_LARGE06[];
extern void *TOWER_LARGE07[];
extern void *TOWER_LARGE08[];
extern void *TOWER_LARGE09[];
extern void *TOWER_LARGE10[];
extern void *TOWER_LARGE11[];
extern void *TOWER_LARGE12[];
extern void *TOWER_XPLODE01[];
extern void *TOWER_XPLODE02[];
extern void *TOWER_XPLODE03[];
extern void *TOWER_XPLODE04[];
extern void *TOWER_XPLODE05[];
extern void *TOWER_XPLODE06[];
extern void *TOWER_XPLODE07[];
extern void *TOWER_XPLODE08[];
extern void *TOWER_XPLODE09[];
extern void *TOWER_XPLODE10[];
extern void *TOWER_XPLODE11[];
extern void *TOWER_XPLODE12[];

/*****************
*****************/

DTBL_INITA tower_table [] =
{
	{5,0xc9,0x51,a_torch,&baklst8},
	{5,0x299,0x53,a_torch,&baklst8},
	{0}
};

ADDRESS *tower_perm_list[]=
{
	(ADDRESS *)BALFIRE1,
	(ADDRESS *)BALFIRE2,
	(ADDRESS *)BALFIRE3,
	(ADDRESS *)BALFIRE4,
	(ADDRESS *)BALFIRE5,
	(ADDRESS *)BALFIRE6,
	(ADDRESS *)TOWER_SMALL01,
	(ADDRESS *)TOWER_SMALL02,
	(ADDRESS *)TOWER_SMALL03,
	(ADDRESS *)TOWER_SMALL04,
	(ADDRESS *)TOWER_SMALL05,
	(ADDRESS *)TOWER_SMALL06,
	(ADDRESS *)TOWER_SMALL07,
	(ADDRESS *)TOWER_SMALL08,
	(ADDRESS *)TOWER_SMALL09,
	(ADDRESS *)TOWER_SMALL10,
	(ADDRESS *)TOWER_SMALL11,
	(ADDRESS *)TOWER_SMALL12,
	(ADDRESS *)TOWER_MEDIUM01,
	(ADDRESS *)TOWER_MEDIUM02,
	(ADDRESS *)TOWER_MEDIUM03,
	(ADDRESS *)TOWER_MEDIUM04,
	(ADDRESS *)TOWER_MEDIUM05,
	(ADDRESS *)TOWER_MEDIUM06,
	(ADDRESS *)TOWER_MEDIUM07,
	(ADDRESS *)TOWER_MEDIUM08,
	(ADDRESS *)TOWER_MEDIUM09,
	(ADDRESS *)TOWER_MEDIUM10,
	(ADDRESS *)TOWER_MEDIUM11,
	(ADDRESS *)TOWER_MEDIUM12,
	(ADDRESS *)TOWER_LARGE01,
	(ADDRESS *)TOWER_LARGE02,
	(ADDRESS *)TOWER_LARGE03,
	(ADDRESS *)TOWER_LARGE04,
	(ADDRESS *)TOWER_LARGE05,
	(ADDRESS *)TOWER_LARGE06,
	(ADDRESS *)TOWER_LARGE07,
	(ADDRESS *)TOWER_LARGE08,
	(ADDRESS *)TOWER_LARGE09,
	(ADDRESS *)TOWER_LARGE10,
	(ADDRESS *)TOWER_LARGE11,
	(ADDRESS *)TOWER_LARGE12,
	(ADDRESS *)TOWER_XPLODE01,
	(ADDRESS *)TOWER_XPLODE02,
	(ADDRESS *)TOWER_XPLODE03,
	(ADDRESS *)TOWER_XPLODE04,
	(ADDRESS *)TOWER_XPLODE05,
	(ADDRESS *)TOWER_XPLODE06,
	(ADDRESS *)TOWER_XPLODE07,
	(ADDRESS *)TOWER_XPLODE08,
	(ADDRESS *)TOWER_XPLODE09,
	(ADDRESS *)TOWER_XPLODE10,
	(ADDRESS *)TOWER_XPLODE11,
	(ADDRESS *)TOWER_XPLODE12,
	NULL
};

/******************************************************************************
 Function: void calla_tower(void)

 By: David Schwartz

 Date:  Jan 1995

 Parameters: None

 Returns: None

 Description:	init balcony/tower stuff
******************************************************************************/
void calla_tower(void)
{
	OBJECT *obj;
	PROCESS *ptemp;

	alloc_vram_perm_bg_list((ADDRESS *)tower_perm_list,&tower_anims);

	create_dumb_animators(tower_table,&tower_anims);

	return;
}


