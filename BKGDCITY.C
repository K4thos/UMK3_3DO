/******************************************************************************
 File: bkgdcity.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III City Routines
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
void calla_city(void);
void city_storm_proc(void);

/************************************************************
 * 			C I T Y/R O O F  S T U F F 						*
 ************************************************************/
/********************
 Externals
 ********************/
extern void *city_anims[];
extern void *a_burn[];
extern void *a_storm_city[];
extern void *BURNING1[];
extern void *BURNING2[];
extern void *BURNING3[];
extern void *BURNING4[];
extern void *BURNING5[];
extern void *BURNING6[];
extern void *BURNING7[];
extern void *BURNING8[];
extern void *CSTORM01[];
extern void *CSTORM03[];
extern void *CSTORM05[];
extern void *CSTORM07[];
extern void *CSTORM09[];
extern void *CSTORM11[];

extern void *CITY_SMALL01[];
extern void *CITY_SMALL02[];
extern void *CITY_SMALL03[];
extern void *CITY_SMALL04[];
extern void *CITY_SMALL05[];
extern void *CITY_SMALL06[];
extern void *CITY_SMALL07[];
extern void *CITY_SMALL08[];
extern void *CITY_SMALL09[];
extern void *CITY_SMALL10[];
extern void *CITY_SMALL11[];
extern void *CITY_SMALL12[];
extern void *CITY_MEDIUM01[];
extern void *CITY_MEDIUM02[];
extern void *CITY_MEDIUM03[];
extern void *CITY_MEDIUM04[];
extern void *CITY_MEDIUM05[];
extern void *CITY_MEDIUM06[];
extern void *CITY_MEDIUM07[];
extern void *CITY_MEDIUM08[];
extern void *CITY_MEDIUM09[];
extern void *CITY_MEDIUM10[];
extern void *CITY_MEDIUM11[];
extern void *CITY_MEDIUM12[];
extern void *CITY_LARGE01[];
extern void *CITY_LARGE02[];
extern void *CITY_LARGE03[];
extern void *CITY_LARGE04[];
extern void *CITY_LARGE05[];
extern void *CITY_LARGE06[];
extern void *CITY_LARGE07[];
extern void *CITY_LARGE08[];
extern void *CITY_LARGE09[];
extern void *CITY_LARGE10[];
extern void *CITY_LARGE11[];
extern void *CITY_LARGE12[];
extern void *CITY_XPLODE01[];
extern void *CITY_XPLODE02[];
extern void *CITY_XPLODE03[];
extern void *CITY_XPLODE04[];
extern void *CITY_XPLODE05[];
extern void *CITY_XPLODE06[];
extern void *CITY_XPLODE07[];
extern void *CITY_XPLODE08[];
extern void *CITY_XPLODE09[];
extern void *CITY_XPLODE10[];
extern void *CITY_XPLODE11[];
extern void *CITY_XPLODE12[];

/********************
 ********************/
DTBL_INITA burn_table [] =
{
	{5,SCX(0x0043)+32-4,SCY(0x0096)+49-16,a_burn,&baklst4},
	{0}
};

ADDRESS *city_perm_list[]=
{
	(ADDRESS *)BURNING1,
	(ADDRESS *)BURNING2,
	(ADDRESS *)BURNING3,
	(ADDRESS *)BURNING4,
	(ADDRESS *)BURNING5,
	(ADDRESS *)BURNING6,
	(ADDRESS *)BURNING7,
	(ADDRESS *)BURNING8,
	(ADDRESS *)CSTORM01,
	(ADDRESS *)CSTORM03,
	(ADDRESS *)CSTORM05,
	(ADDRESS *)CSTORM07,
	(ADDRESS *)CSTORM09,
	(ADDRESS *)CSTORM11,
	(ADDRESS *)CITY_SMALL01,
	(ADDRESS *)CITY_SMALL02,
	(ADDRESS *)CITY_SMALL03,
	(ADDRESS *)CITY_SMALL04,
	(ADDRESS *)CITY_SMALL05,
	(ADDRESS *)CITY_SMALL06,
	(ADDRESS *)CITY_SMALL07,
	(ADDRESS *)CITY_SMALL08,
	(ADDRESS *)CITY_SMALL09,
	(ADDRESS *)CITY_SMALL10,
	(ADDRESS *)CITY_SMALL11,
	(ADDRESS *)CITY_SMALL12,
	(ADDRESS *)CITY_MEDIUM01,
	(ADDRESS *)CITY_MEDIUM02,
	(ADDRESS *)CITY_MEDIUM03,
	(ADDRESS *)CITY_MEDIUM04,
	(ADDRESS *)CITY_MEDIUM05,
	(ADDRESS *)CITY_MEDIUM06,
	(ADDRESS *)CITY_MEDIUM07,
	(ADDRESS *)CITY_MEDIUM08,
	(ADDRESS *)CITY_MEDIUM09,
	(ADDRESS *)CITY_MEDIUM10,
	(ADDRESS *)CITY_MEDIUM11,
	(ADDRESS *)CITY_MEDIUM12,
	(ADDRESS *)CITY_LARGE01,
	(ADDRESS *)CITY_LARGE02,
	(ADDRESS *)CITY_LARGE03,
	(ADDRESS *)CITY_LARGE04,
	(ADDRESS *)CITY_LARGE05,
	(ADDRESS *)CITY_LARGE06,
	(ADDRESS *)CITY_LARGE07,
	(ADDRESS *)CITY_LARGE08,
	(ADDRESS *)CITY_LARGE09,
	(ADDRESS *)CITY_LARGE10,
	(ADDRESS *)CITY_LARGE11,
	(ADDRESS *)CITY_LARGE12,
	(ADDRESS *)CITY_XPLODE01,
	(ADDRESS *)CITY_XPLODE02,
	(ADDRESS *)CITY_XPLODE03,
	(ADDRESS *)CITY_XPLODE04,
	(ADDRESS *)CITY_XPLODE05,
	(ADDRESS *)CITY_XPLODE06,
	(ADDRESS *)CITY_XPLODE07,
	(ADDRESS *)CITY_XPLODE08,
	(ADDRESS *)CITY_XPLODE09,
	(ADDRESS *)CITY_XPLODE10,
	(ADDRESS *)CITY_XPLODE11,
	(ADDRESS *)CITY_XPLODE12,
	NULL
};

/******************************************************************************
 Function: void calla_city(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	setup the citys background
******************************************************************************/
void calla_city(void)
{
	alloc_vram_perm_bg_list((ADDRESS *)city_perm_list,&city_anims);
	CREATE(PID_BANI,city_storm_proc);
	create_dumb_animators(burn_table,&city_anims);

	return;
}

/******************************************************************************
 Function: void city_storm_proc(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	storm process
******************************************************************************/
void city_storm_proc(void)
{
	OBJECT *obj;

	gso_dmawnz(obj,(ADDRESS)CSTORM01,city_anims,0);
	set_xy_coordinates(obj,SCX(0xa8)+34,SCY(0x30)+20);
	alloc_cache(CSTORM01,&city_anims,obj);
	insert_object(obj,&baklst7);

	current_proc->pa9=a_storm_city;
	framew(7);
}

/************************************************************
 * 			B A N K
 ************************************************************/
/********************
 Externals
 ********************/
extern void *BANK_STBREAK3[];
extern void *BANK_STBREAK2[];
extern void *BANK_STBREAK1[];
extern void *bank_anims[];

ADDRESS *bank_perm_list[]=
{
	(ADDRESS *)BANK_STBREAK1,
	(ADDRESS *)BANK_STBREAK2,
	(ADDRESS *)BANK_STBREAK3,
	NULL
};

/******************************************************************************
 Function: void calla_bank(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	setup the banks background
******************************************************************************/
void calla_bank(void)
{
	alloc_vram_perm_bg_list((ADDRESS *)bank_perm_list,&bank_anims);
	return;
}

