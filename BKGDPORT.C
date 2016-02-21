/******************************************************************************
 File: bkgdpotr.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Portal Routines
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

/************************************************************
 * 			P O R T A L  S T U F F 					*
 ************************************************************/

/********************
 Externals
 ********************/
extern void *portal_anims[];
extern void *a_portal_zap[];
extern void *PORTAL01[];
extern void *PORTAL02[];
extern void *PORTAL03[];
extern void *PORTAL04[];
extern void *PORTAL05[];
extern void *PORTAL06[];
extern void *PORTAL07[];
extern void *PORTAL08[];
extern void *PORTAL09[];
extern void *PORTAL11[];
extern void *PORTAL12[];
extern void *PORTAL13[];
extern void *PORTAL14[];
extern void *PORTAL17[];

/********************
 ********************/
void portal_zap(void);
void mk3_calla(void);

ADDRESS *portal_perm_list[]=
{
	(ADDRESS*)PORTAL01,
	(ADDRESS*)PORTAL02,
	(ADDRESS*)PORTAL03,
	(ADDRESS*)PORTAL04,
	(ADDRESS*)PORTAL05,
	(ADDRESS*)PORTAL06,
	(ADDRESS*)PORTAL07,
	(ADDRESS*)PORTAL08,
	(ADDRESS*)PORTAL09,
	(ADDRESS*)PORTAL11,
	(ADDRESS*)PORTAL12,
	(ADDRESS*)PORTAL13,
	(ADDRESS*)PORTAL14,
	(ADDRESS*)PORTAL17,
	NULL
};

/******************************************************************************
 Function: void mk3_calla(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	setup the citys background
******************************************************************************/
void mk3_calla(void)
{
	alloc_vram_perm_bg_list((ADDRESS *)portal_perm_list,&portal_anims);
	CREATE(PID_BANI,portal_zap);
	return;
}

void portal_zap(void)
{
	OBJECT *obj;

	gso_dmawnz(obj,(ADDRESS)PORTAL01,portal_anims,0);
	set_xy_coordinates(obj,SCX(0xb0)+0x6,SCY(0x5c)-4);
	alloc_cache(PORTAL01,&portal_anims,obj);
	insert_object(obj,&baklst1);

	/* pzap3 */
	do
	{
		current_proc->pa9=a_portal_zap;
		framew(3);
		(long)current_proc->a11=obj->oypos.pos;
		obj->oypos.u.intpos=SCY(0x4000);
		process_sleep(0x30);
		obj->oypos.pos=(long)current_proc->a11;
	}
	while(1);
}
