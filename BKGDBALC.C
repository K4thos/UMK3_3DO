/******************************************************************************
 File: bkgdbalc.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Balcony Routines
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
void calla_tower(void);

/************************************************************
 * 			B A L C O N Y & T O W E R  S T U F F 			*
 ************************************************************/
#if 0
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
#endif
