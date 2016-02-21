/******************************************************************************
 File: bkgdbrig.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Bridge Routines
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
void bridge_calla(void);

/************************************************************
 * 			B R I D G E S T U F F 							*
 ************************************************************/
/********************
 Externals
 ********************/
extern void *a_big_paper[];
extern void *a_lil_paper[];
extern void *a_leaf[];
extern void *a_paper1[];
extern void *bridge_anims[];
extern void *PAPER1[];
extern void *PAPER2[];
extern void *PAPER3[];
extern void *PAPER4[];
extern void *PAPER5[];
extern void *PAPER6[];
extern void *PAPER7[];
extern void *PAPER8[];
extern void *PAPER9[];
extern void *LGSHEET1[];
extern void *LGSHEET2[];
extern void *LGSHEET3[];
extern void *LGSHEET4[];
extern void *LGSHEET5[];
extern void *LGSHEET6[];
extern void *LGSHEET7[];
extern void *LGSHEET8[];
extern void *LGSHEET9[];
extern void *SMSHEET1[];
extern void *SMSHEET2[];
extern void *SMSHEET3[];
extern void *SMSHEET4[];
extern void *SMSHEET5[];
extern void *SMSHEET6[];
extern void *SMSHEET7[];
extern void *SMSHEET8[];
extern void *SMSHEET9[];
extern void *LEAF1[];
extern void *LEAF2[];
extern void *LEAF3[];
extern void *LEAF4[];
extern void *LEAF5[];
extern void *LEAF6[];
extern void *LEAF7[];
extern void *LEAF8[];
/********************
 ********************/

DTBL_INIT paper_table [] = 
{
	{3,PAPER1,-SCX(0x0050)+8,SCY(0x00b0)+16,a_paper1,&baklst4},
	{4,PAPER1,SCX(0x0050)+8,SCY(0x00b0)+16,a_paper1,&baklst4},
	{3,PAPER1,SCX(0x0100)+8,SCY(0x00b4)+16,a_paper1,&baklst4},
	{5,PAPER1,SCX(0x01d0)+8,SCY(0x00ad)+16,a_paper1,&baklst4},
	{4,PAPER1,SCX(0x02a0)+8,SCY(0x00b0)+16,a_paper1,&baklst4},
	{0}
};

ADDRESS *bridge_perm_list[] = 
{
	(ADDRESS *)PAPER1,
	(ADDRESS *)PAPER2,
	(ADDRESS *)PAPER3,
	(ADDRESS *)PAPER4,
	(ADDRESS *)PAPER5,
	(ADDRESS *)PAPER6,
	(ADDRESS *)PAPER7,
	(ADDRESS *)PAPER8,
	(ADDRESS *)PAPER9,
	(ADDRESS *)LGSHEET1,
	(ADDRESS *)LGSHEET2,
	(ADDRESS *)LGSHEET3,
	(ADDRESS *)LGSHEET4,
	(ADDRESS *)LGSHEET5,
	(ADDRESS *)LGSHEET6,
	(ADDRESS *)LGSHEET7,
	(ADDRESS *)LGSHEET8,
	(ADDRESS *)LGSHEET9,
	(ADDRESS *)SMSHEET1,
	(ADDRESS *)SMSHEET2,
	(ADDRESS *)SMSHEET3,
	(ADDRESS *)SMSHEET4,
	(ADDRESS *)SMSHEET5,
	(ADDRESS *)SMSHEET6,
	(ADDRESS *)SMSHEET7,
	(ADDRESS *)SMSHEET8,
	(ADDRESS *)SMSHEET9,
	(ADDRESS *)LEAF1,
	(ADDRESS *)LEAF2,
	(ADDRESS *)LEAF3,
	(ADDRESS *)LEAF4,
	(ADDRESS *)LEAF5,
	(ADDRESS *)LEAF6,
	(ADDRESS *)LEAF7,
	(ADDRESS *)LEAF8,
	NULL
};

/******************************************************************************
 Function: void bridge_calla(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	setup the bridge background
******************************************************************************/
void bridge_calla(void)
{
	short loop=2;
	PROCESS *ptemp;

	alloc_vram_perm_bg_list((ADDRESS *)bridge_perm_list,&bridge_anims);

	CREATE(PID_BANI,wind_sound);

	do
	{
		ptemp=CREATE(PID_BANI,garbage_straight);
		ptemp->pa9=a_big_paper;
		(void *)ptemp->a10=&bridge_anims;

		ptemp=CREATE(PID_BANI,garbage_straight);
		ptemp->pa9=a_lil_paper;
		(void *)ptemp->a10=&bridge_anims;

		ptemp=CREATE(PID_BANI,garbage_straight);
		ptemp->pa9=a_leaf;
		(void *)ptemp->a10=&bridge_anims;
		ptemp=CREATE(PID_BANI,garbage_straight);
		ptemp->pa9=a_leaf;
		(void *)ptemp->a10=&bridge_anims;
		ptemp=CREATE(PID_BANI,garbage_straight);
		ptemp->pa9=a_leaf;
		(void *)ptemp->a10=&bridge_anims;

		ptemp=CREATE(PID_BANI,garbage_loop);
		ptemp->pa9=a_big_paper;
		(void *)ptemp->a10=&bridge_anims;

		ptemp=CREATE(PID_BANI,garbage_loop);
		ptemp->pa9=a_lil_paper;
		(void *)ptemp->a10=&bridge_anims;
	}
	while(--loop>0);

	create_dumb_animators_w(paper_table,&bridge_anims);

	return;
}
