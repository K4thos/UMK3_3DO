/******************************************************************************
 File: bkgdgrav.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Grave Yard Routines
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
void grave_calla(void);
void moon_proc(void);

/************************************************************
 * 			G R A V E  S T U F F 							*
 ************************************************************/

/********************
 Externals
 ********************/
extern void *grave_anims[];
extern void *a_moon[];
extern void *MOONA1[];
extern void *MOONA2[];
extern void *MOONA3[];
extern void *MOONA4[];
extern void *MOONA5[];
extern void *MOONA6[];
extern void *MOONA7[];
extern void *MOONA8[];
extern void *MOONA9[];
extern void *MOONA10[];
extern void *MOONA11[];
extern void *MOONA12[];
extern void *MOONA13[];
extern void *MOONA14[];
extern void *MOONA15[];
extern void *MOONA16[];
extern void *MOONA17[];
extern void *MOONA18[];
extern void *MOONA19[];
extern void *MOONA20[];
extern void *MOONA21[];
extern void *MOONA22[];
extern void *MOONA23[];
extern void *MOONA24[];
extern void *MOONA25[];
extern void *MOONA26[];
extern void *MOONA27[];
extern void *MOONA28[];
extern void *BOON[];
extern void *BERAN[];
extern void *TOBIAS[];
extern void *MICHIC[];
extern void *GOSKIE[];
extern void *VOGEL[];
extern void *LYDY[];
extern void *MAY[];
extern void *MILLER[];
extern void *SCHWARTZ[];
extern void *GUIDO[];
extern void *ABBOT[];
/********************
 ********************/

void *tomb_stones1[]=
{
	BOON,
	BERAN,
	TOBIAS,
	GOSKIE
};

void *tomb_stones2[]=
{
	GOSKIE,
	BOON,
	VOGEL,
	MICHIC
};

void *tomb_stones3[]=
{
	BERAN,
	MILLER,
	TOBIAS,
	MICHIC
};


void *tomb_stones4[]=
{
	GUIDO,
	MAY,
	SCHWARTZ,
	MILLER
};

void *tomb_stones5[]=
{
	GUIDO,
	BOON,
	SCHWARTZ,
	TOBIAS
};

void *tomb_stones6[]=
{
	MILLER,
	MAY,
	VOGEL,
	BERAN
};

ADDRESS *tomb_table[]=
{
	(ADDRESS *)tomb_stones1,(ADDRESS *)tomb_stones2,(ADDRESS *)tomb_stones3,(ADDRESS *)tomb_stones4,(ADDRESS *)tomb_stones5,(ADDRESS *)tomb_stones6
};

short tomb_spots[]=
{
	-SCX(0x100),
	SCX(0x00),
	SCX(0x100),
	SCX(0x200),
	SCX(0x300)
};

ADDRESS *grave_perm_list[]=
{
	(ADDRESS *)MOONA2,
	(ADDRESS *)MOONA4,
	(ADDRESS *)MOONA6,
	(ADDRESS *)MOONA8,
	(ADDRESS *)MOONA10,
	(ADDRESS *)MOONA12,
	(ADDRESS *)MOONA14,
	(ADDRESS *)MOONA16,
	(ADDRESS *)MOONA18,
	(ADDRESS *)MOONA20,
	(ADDRESS *)MOONA22,
	(ADDRESS *)MOONA24,
	(ADDRESS *)MOONA26,
	(ADDRESS *)MOONA28,
	(ADDRESS *)BOON,
	(ADDRESS *)BERAN,
	(ADDRESS *)TOBIAS,
	(ADDRESS *)MICHIC,
	(ADDRESS *)GOSKIE,
	(ADDRESS *)VOGEL,
	(ADDRESS *)LYDY,
	(ADDRESS *)MAY,
	(ADDRESS *)MILLER,
	(ADDRESS *)SCHWARTZ,
	(ADDRESS *)GUIDO,
	(ADDRESS *)ABBOT,
	NULL
};

/******************************************************************************
 Function: void grave_calla(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	setup the graveyard background
******************************************************************************/
void grave_calla(void)
{
	OBJECT *obj;
	PROCESS *ptemp;
	ADDRESS *tombtbl;

	alloc_vram_perm_bg_list((ADDRESS *)grave_perm_list,&grave_anims);

	/* set moon process */
	gso_dmawnz_ns(obj,(ADDRESS)MOONA28,grave_anims,0);
	set_xy_coordinates(obj,0x60,0x20-6);
	alloc_cache(MOONA28,&grave_anims,obj);

	ptemp=CREATE(PID_BANI,moon_proc);
	ptemp->pa8=obj;
	insert_object(obj,&baklst8);

	current_proc->a11=4;
	tombtbl=tomb_table[randu(6)-1]; 	// add our names
	//tombtbl=tomb_table[0];

	/* tomb3 */
	do
	{
		gso_dmawnz(obj,(ADDRESS)(tombtbl[current_proc->a11-1]),grave_anims,0);
		obj->oxpos.u.intpos=worldtlx1.u.intpos+tomb_spots[current_proc->a11-1];
		obj->oypos.u.intpos=ground_y-SCY(0x58)-1;
		alloc_cache(tombtbl[current_proc->a11-1],&grave_anims,obj);

		insert_object(obj,&baklst9);
	}
	while(--current_proc->a11>0);

	return;
}

/******************************************************************************
 Function: void moon_proc(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	moon procedure
******************************************************************************/
void moon_proc(void)
{
	while (1)
	{
		process_sleep((WORD)randu_minimum(0x40,0x40));

		current_proc->pa9=a_moon;
		framew(8);
	}
}

