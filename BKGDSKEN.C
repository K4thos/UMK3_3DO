/******************************************************************************
 File: bkgdsken.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III SHAO KAHN ENDING Routines
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
void shao_storm_proc(void);

/************************************************************
 * 			S K  E N D  S T U F F 					*
 ************************************************************/

/********************
 Externals
 ********************/
extern void *a_sknado[];
extern void *SKNADO1[];
extern void *SKNADO2[];
extern void *SKNADO3[];
extern void *SKNADO4[];
extern void *SKNADO5[];
extern void *SKNADO6[];
extern void *SKNADO7[];
extern void *SKNADO8[];
extern void *sk_anims[];
extern void *SSTORM01[];
extern void *SSTORM03[];
extern void *SSTORM05[];
extern void *SSTORM07[];
extern void *SSTORM09[];
extern void *SSTORM11[];
extern void *a_storm_shao[];

/********************
 ********************/

ADDRESS *skend_perm_list[]=
{
	(ADDRESS*)SKNADO1,
	(ADDRESS*)SKNADO2,
	(ADDRESS*)SKNADO3,
	(ADDRESS*)SKNADO4,
	(ADDRESS*)SKNADO5,
	(ADDRESS*)SKNADO6,
	(ADDRESS*)SKNADO7,
	(ADDRESS*)SKNADO8,
	(ADDRESS*)SSTORM01,
	(ADDRESS*)SSTORM03,
	(ADDRESS*)SSTORM05,
	(ADDRESS*)SSTORM07,
	(ADDRESS*)SSTORM09,
	(ADDRESS*)SSTORM11,
	NULL
};

/******************************************************************************
 Function: void skdie_calla(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	setup the ending tornado
******************************************************************************/
DTBL_INITA skdie_ani_table[]=
{
	{5,SCX(0xd3)+0x11-6,SCY(0x2f)+0x12,a_sknado,&baklst5},
	{0}
};
void skdie_calla(void)
{
	CREATE(PID_BANI,shao_storm_proc);
	alloc_vram_perm_bg_list((ADDRESS *)skend_perm_list,&sk_anims);
	create_dumb_animators(skdie_ani_table,sk_anims);
	return;
}

/******************************************************************************
 Function: void shao_storm_proc(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	storm process
******************************************************************************/
void shao_storm_proc(void)
{
	OBJECT *obj;

	gso_dmawnz(obj,(ADDRESS)SSTORM01,sk_anims,0);
	set_xy_coordinates(obj,SCX(0xa8)+34-0x38,SCY(0x30)+20-0x34);
	alloc_cache(SSTORM01,&sk_anims,obj);
	insert_object(obj,&baklst6);

	current_proc->pa9=a_storm_shao;
	framew(7);
}

