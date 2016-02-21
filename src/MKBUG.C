/******************************************************************************
 File: mkvs.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III vs Routines
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
#include "mkinit.h"
#include "mkutil.h"
#include "mkani.h"


/******************************************************************************
 Function: void debug_grid(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	display debug grid on screen
******************************************************************************/
void debug_grid(void) 
{
#if DEBUG_GRID
	int test;

	/* green horiz lines */
	for (test=0;test<11;test++)
	{
		SetLineF2(&cdb->lines[test]);
		setRGB0(&cdb->lines[test],0,128,0);
		setXY2(&cdb->lines[test],0,test*43/2+16,320,test*43/2+16);
	cdb->primcnt++;
		AddPrim(cdb->ot_ptr,&cdb->lines[test]); 
	}

	/* blue vert lines */
	for (test=11;test<21;test++)
	{
		SetLineF2(&cdb->lines[test]);
		setRGB0(&cdb->lines[test],0,0,128);
		setXY2(&cdb->lines[test],(test-11)*32,0,(test-11)*32,240);
	cdb->primcnt++;
		AddPrim(cdb->ot_ptr,&cdb->lines[test]); 
	}
#endif /* DEBUG_GRID */

	return;
}

/******************************************************************************
 Function: void profiler(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	display profile of program
******************************************************************************/
void profiler(void) 
{
#if PROFILE
		if (f_profile==1) 
		{
			setRGB0(cdb->box_ptr,0,128,0);			// base line (1 vblank)
			setXYWH(cdb->box_ptr,12,32,12+240/2,6);
	cdb->primcnt++;
			AddPrim(cdb->ot_ptr,cdb->box_ptr); 
			cdb->box_ptr++;

			setRGB0(cdb->box_ptr,128,128,0);			// base line (2 vblank)
			setXYWH(cdb->box_ptr,12+240/2,32,12+240/2,6);
	cdb->primcnt++;
			AddPrim(cdb->ot_ptr,cdb->box_ptr); 
			cdb->box_ptr++;

			setRGB0(cdb->box_ptr,0,0,128);			// current line CPU
			setXYWH(cdb->box_ptr,12,24,12+(prof_end-prof_start)/2,8);
	cdb->primcnt++;
			AddPrim(cdb->ot_ptr,cdb->box_ptr); 
			cdb->box_ptr++;

			setRGB0(cdb->box_ptr,80,45,14);			// current line drawing
			setXYWH(cdb->box_ptr,12+(prof_end-prof_start)/2,24,12+(prof_draw_end-prof_draw_start)/2,8);
	cdb->primcnt++;
			AddPrim(cdb->ot_ptr,cdb->box_ptr); 
			cdb->box_ptr++;


			if ((prof_draw_end-prof_start)>prof_max)
				prof_max=prof_draw_end-prof_start;

			setRGB0(cdb->box_ptr,128,0,0);			// max line
			setXYWH(cdb->box_ptr,12,38,12+prof_max/2,8);
	cdb->primcnt++;
			AddPrim(cdb->ot_ptr,cdb->box_ptr); 
			cdb->box_ptr++;

		}
#endif
	return;
}

/******************************************************************************
 Function: void draw_collision_box(XYTYPE *tl,XYTYPE *lr,WORD index)

 By: David Schwartz

 Date: Dec 1994

 Parameters: tl - top left of box
			 lr - lower right of box
			 index - 0=attacker 1=victim

 Returns: None

 Description:	draw a collision box
******************************************************************************/
void draw_collision_box(XYTYPE *tl,XYTYPE *lr,WORD index) 
{
#if COLLISION_BOX
	OBJECT *obj;

	if (f_colbox) 
	{
		if (index==0)
			kilobj2(OID_CBOX,0xffff);

		obj=get_object();
		obj->oid=OID_CBOX;
		obj->oypos.u.intpos=tl->u.ypos;
		obj->osize.u.ypos=abs(tl->u.ypos-lr->u.ypos);

		obj->oxpos.u.intpos=tl->u.xpos;
		obj->osize.u.xpos=abs(tl->u.xpos-lr->u.xpos);

		obj->oflags|=M_BOX_POLY;

		if (index==0) 
		{
			obj->header.t_xoffset=0;
			obj->header.t_yoffset=0;
			obj->header.t_width=128;
		}
		else
		{
			obj->header.t_xoffset=128;
			obj->header.t_yoffset=0;
			obj->header.t_width=0;
		}

		insert_object(obj,&objlst2);				// put on list #2, in front of fighters
	}
#endif
	return;	
}
