/******************************************************************************
 File: mkbrief.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Briefcase Setup
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
#include "mkbrief.h"

BCC brief_case_codes[]=
{
	{0x100100,bc_throwing_disabled},
	{0x222222,bc_blocking_disabled},
	{0x888000,bc_p1_handicap},
	{0x000888,bc_p2_handicap},
	{0x333333,bc_dark_fighting},
	{0x444444,bc_switcheroo},
	{0x987666,bc_jackbot},
	{0,NULL}
};

/******************************************************************************
 Function: void bc_p1_handicap(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  setup p1 for handicap
******************************************************************************/
void bc_p1_handicap(void)
{
	stuff_round_123(3);
	return;
}

/******************************************************************************
 Function: void bc_p2_handicap(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  setup p2 for handicap
******************************************************************************/
void bc_p2_handicap(void)
{
	stuff_round_123(4);
	return;
}

/******************************************************************************
 Function: void bc_throwing_disabled(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  no throwing allowed
******************************************************************************/
void bc_throwing_disabled(void)
{
	stuff_round_123(1);
	return;
}

/******************************************************************************
 Function: void bc_blocking_disabled(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  None

 Returns: None

 Description:  no blocking allowed
******************************************************************************/
void bc_blocking_disabled(void)
{
	stuff_round_123(2);
	return;
}

void bc_jackbot(void)
{
	stuff_round_123(7);
	return;
}

void bc_switcheroo(void)
{
	stuff_round_123(6);
	return;
}

void bc_dark_fighting(void)
{
	stuff_round_123(5);
	return;
}

/******************************************************************************
 Function: void stuff_round_123(WORD pa0)

 By: David Schwartz

 Date: Mar 1995

 Parameters:  pa0 - setup code

 Returns: None

 Description:  setup for speical codes
******************************************************************************/
void stuff_round_123(WORD pa0)
{
	round_1_jsrp=pa0;
	round_23_jsrp=pa0;
	return;
}
