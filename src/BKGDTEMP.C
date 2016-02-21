/******************************************************************************
 File: bkgdtemp.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Temple Routines
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


/* local prototypes */
void temple_calla(void);
void candle_flame(void);

/************************************************************
 * 			T E M P L E  S T U F F 							*
 ************************************************************/
/*****************
 Externals
*****************/
extern void *temple_anims[];
extern void *a_tflame[];
extern void *a_candle_flame[];
extern void *SKTORCH1[];
extern void *SKTORCH2[];
extern void *SKTORCH3[];
extern void *SKTORCH4[];
extern void *SKTORCH5[];
extern void *SKTORCH6[];
extern void *wik1[];
extern void *wik2[];
extern void *wik3[];
extern void *wik4[];
extern void *KANDLE1[];
/*****************
*****************/

DTBL_INITA tony_candle_table[] =
{
	{4,0xd5+2,0x2c-3,a_tflame,&baklst7},
	{4,0x14d+2,0x2c-3,a_tflame,&baklst7},
	{4|0x8000,0x26d+2,0x2c-3,a_tflame,&baklst7},
	{4|0x8000,0x2e4+2,0x2c-3,a_tflame,&baklst7},
	{0}
};

ADDRESS *temple_perm_list[] =
{
	(ADDRESS *)SKTORCH1,
	(ADDRESS *)SKTORCH2,
	(ADDRESS *)SKTORCH3,
	(ADDRESS *)SKTORCH4,
	(ADDRESS *)SKTORCH5,
	(ADDRESS *)SKTORCH6,
	(ADDRESS *)wik1,
	(ADDRESS *)wik2,
	(ADDRESS *)wik3,
	(ADDRESS *)wik4,
	(ADDRESS *)KANDLE1,
	NULL
};

/******************************************************************************
 Function: void temple_calla(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	setup the bridge background
******************************************************************************/
void temple_calla(void)
{
	OBJECT *obj;

	alloc_vram_perm_bg_list((ADDRESS *)temple_perm_list,&temple_anims);

	current_proc->a10=10;
	(long)current_proc->a11=-1;

	do
	{
		/* candd */
		gso_dmawnz(obj,(ADDRESS)KANDLE1,temple_anims,0);
		obj->oxpos.u.intpos=(short)current_proc->a11+8+2;
		obj->oypos.u.intpos=0xb2+6;	//+36;
		alloc_cache(KANDLE1,temple_anims,obj);
		insert_object(obj,&baklst6);

		/* flame */
		(OBJECT *)current_proc->pa9=obj;
		gso_dmawnz(obj,(ADDRESS)wik1,temple_anims,0);
		alloc_cache(wik1,temple_anims,current_proc->pa8);
		insert_object(current_proc->pa8,&baklst6);

		obj->oxpos.u.intpos=((OBJECT *)current_proc->pa9)->oxpos.u.intpos+SCX(7);
		obj->oypos.u.intpos=((OBJECT *)current_proc->pa9)->oypos.u.intpos-SCY(12);

		CREATE(PID_BANI,candle_flame);
		(long)current_proc->a11+=SCX(0x96);
	}
	while(--current_proc->a10>0);

	create_dumb_animators(tony_candle_table,&temple_anims);

	return;
}

/******************************************************************************
 Function: void candle_flame(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	animate the candles on the temple bkgd
******************************************************************************/
void candle_flame(void)
{
	process_sleep(randu(10));

	current_proc->pa9=a_candle_flame;
	framew(3+randu(2));

	wait_forever();
}
