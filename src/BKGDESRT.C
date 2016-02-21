/******************************************************************************
 File: bkgdesrt.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Desert Routines
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
void make_cyrax_sand(void);

/************************************************************
 * 			G R A V E  S T U F F 							*
 ************************************************************/

/********************
 Externals
 ********************/
extern void *desert_anims[];
extern void *a_cyrax[];
extern void *CYRXSAND1[];
extern void *CYRXSAND2[];
extern void *CYRXSAND3[];
extern void *CYRXSAND4[];

/********************
 ********************/
ADDRESS *desert_perm_list[]=
{
	(ADDRESS *)CYRXSAND1,
	(ADDRESS *)CYRXSAND2,
	(ADDRESS *)CYRXSAND3,
	(ADDRESS *)CYRXSAND4,
	NULL
};

DTBL_INIT cyrax_table [] =
{
	{7,CYRXSAND1,-SCX(0xb0)+24,SCY(0x008a)+24,a_cyrax,&baklst5},
	{0}
};

/******************************************************************************
 Function: void make_cyrax_sand(void)

 By: David Schwartz

 Date: Nov 1995

 Parameters: None

 Returns: None

 Description:	setup the desert background
******************************************************************************/
void make_cyrax_sand(void)
{
	alloc_vram_perm_bg_list((ADDRESS *)desert_perm_list,&desert_anims);

	create_dumb_animators_w(cyrax_table,&desert_anims);

	return;
}

