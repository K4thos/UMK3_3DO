/******************************************************************************
 File: mkobj.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Object Handler Routines
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
#include "mkmain.h"
#include "mkinit.h"
#include "mkpal.h"
#include "valloc.h"

WORD objcnt;
WORD objmax=0;

extern WORD WHITE_p[];
/******************************************************************************
 Function: void object_initialize(void)

 By: David Schwartz

 Date: Aug 23, 1994

 Parameters: None

 Returns: None

 Description:	setup the display lists and variois object stuff and game init stuff
******************************************************************************/
void object_initialize(void)
{
	int loop;

	zero_word_init();				/* initialze all word values */

	zero_long_init();				/* initialze all long values */

	swtemp1.padvalue=swtemp2.padvalue=0;				/* all previous switches are opened */

	clear_pal();					/* initialize palette mananger */

	f_auto_erase=1;

	background_clear();				/* clear background vars and stuff */

	scrntlx=scrntl2x=SCRNSTX;			/* init screen top left */
	scrntly=scrntl2y=SCRNSTY;			/* init screen top left */

	scrnlrx=scrnlr2x=SCRNENDX;			/* init screen bottom right */
	scrnlry=scrnlr2y=SCRNENDY;			/* init screen bottom right */

	/* allocate perm palettes */
	get_fore_pal(SCORE_P);				// palette #2 score
	get_fore_pal(bpal_black_P);			// palette #3 black
	get_fore_pal(bpal_white_P);			// palette #6 white
	get_fore_pal(WHITE_p);			// palette #6 white
	get_fore_pal(bpal_flash1_P);		// pallete #11 flash palette #1
	get_fore_pal(bpal_flash2_P);		// pallete #12 flash palette #2
	get_fore_pal(bpal_flash3_P);		// pallete #13 flash palette #3

	/* setup object ram lists */
	obj_free=object_ram;

	for (loop=0;loop<NUM_OBJECTS-1;loop++)
		object_ram[loop].olink=&object_ram[loop+1];

	object_ram[NUM_OBJECTS-1].olink=NULL;

#if DEBUG
	objcnt=0;
#endif

	return;

}

/******************************************************************************
 Function: OBJECT *get_object(void)

 By: David Schwartz

 Date: Aug 1994

 Parameters: None

 Returns: ptr to allocated object
		  NULL - no object available

 Description:	retrieved an object off the free list & zeros out object list, if one is available, otherwise
				returns a NULL value
******************************************************************************/
OBJECT *get_object(void)
{
	OBJECT *new_obj;

	if ((new_obj=obj_free) !=NULL)
	{
		obj_free=obj_free->olink;
		memset(new_obj,0,sizeof(OBJECT));			/* clear structure */

		/* vcache object init */
		new_obj->ovcache=VCACHE_FAILED;
		new_obj->oscale.yx=0x01000100;


#if DEBUG
	objcnt++;
	if (objcnt>objmax)
		objmax=objcnt;
#endif

	}


	return(new_obj);
}

/******************************************************************************
 Function: void insert_object(OBJECT *obj,OBJECT **object_lst)

 By: David Schwartz

 Date: Aug 1994

 Parameters: obj - ptr to object to insert
			 obj_list - ptr to head of list to insert into

 Returns: None

 Description:	This routine will insert an obj into the object list (LINKED LIST)
				The sort criteria is 1)	ozval		2) oypos
******************************************************************************/
void insert_object(OBJECT *obj,OBJECT **obj_list)
{
	OBJECT *cur_obj,*prev_obj;

	cur_obj=(OBJECT *)obj_list;

	do
	{
		prev_obj=cur_obj;
		if ((cur_obj=cur_obj->olink) != NULL)
		{
			 /* sort first come last registered */
			if (obj->ozval > cur_obj->ozval) break;

			if ((obj->ozval == cur_obj->ozval) &&
				(obj->oypos.pos > cur_obj->oypos.pos)) break;
		}
	}
	while(cur_obj != NULL );

	/* insert new node */
	obj->olink=cur_obj;
	prev_obj->olink=obj;

	return;
}

/******************************************************************************
 Function: void zero_word_init(void)

 By: David Schwartz

 Date: Aug 23, 1994

 Parameters: None

 Returns: None

 Description:	sets to zero all words
******************************************************************************/
void zero_word_init(void)
{

	f_novel=f_auto_erase=f_shadows=noflip=0;
	f_skew=skew_y=skew_height=skew_stack=0;
	f_only_t=0;

	return;
}

/******************************************************************************
 Function: void zero_long_init(void)

 By: David Schwartz

 Date: Aug 23, 1994

 Parameters: None

 Returns: None

 Description:	sets to zero or NULL all longs
******************************************************************************/
void zero_long_init(void)
{
	call_every_tick=dlists=NULL;

	scrolly.pos=scrollx.pos=scrollx1.pos=scrollx2.pos=scrollx3.pos=scrollx4.pos=0;
	scrollx5.pos=scrollx6.pos=scrollx7.pos=scrollx8.pos=0;

	worldtly.pos=worldtlx.pos=0;

	skew_dx.pos=0;

	p1_shape=p2_shape=0;
	p1_obj=p2_obj=NULL;
	p1_xvel.pos=p2_xvel.pos=0;

	objlst=objlst2=objlst3=NULL;
	baklst9=baklst8=baklst7=baklst6=baklst5=baklst4=baklst3=baklst2=baklst1=NULL;

	return;
}

/******************************************************************************
 Function: display_object_lists(XYTYPE *world,OBJECT *object_ptr)

 By: David Schwartz

 Date: Sept 1, 1994

 Parameters: world - y:x position of the top left corner
			 object_ptr - ptr to the object lists to display

 Returns: None

 Description:	sets to zero or NULL all longs
******************************************************************************/
void display_object_lists(XYTYPE *world,OBJECT *object_ptr)
{
	XYTYPE objpos;
	P_RAM *pram_ptr;
	WORD oflags2;
	LONG scx,scy;

	while (object_ptr->olink !=NULL)
	{
			object_ptr=object_ptr->olink;

			oflags2=object_ptr->oflags2;

			/* add velocities */
			if (!f_novel && (f_pause<PAUSE_OFF || f_pause==PAUSE_P1_INIT || f_pause==PAUSE_P2_INIT))
			{
				object_ptr->oxpos.pos+=object_ptr->oxvel.pos;
				object_ptr->oypos.pos+=object_ptr->oyvel.pos;
			}

			/* set object position */
			objpos.u.xpos=object_ptr->oxpos.u.intpos;
			objpos.u.ypos=object_ptr->oypos.u.intpos;

			/* scroll object */
			if (!(oflags2 & M_NOSCROLL))
			{
				objpos.u.xpos -=world->u.xpos;
				objpos.u.ypos -=world->u.ypos;
			}
			else
			{
				if ((oflags2 & M_YLOCK))
					objpos.u.ypos -=world->u.ypos;
			}

		if ( objpos.u.ypos>-250 && objpos.u.ypos<512 )
		{

			/* multi object code check */
			if (oflags2 & M_MULTIPART)
			{
/*****************************
 * MULTI PART OBJECT
 *****************************/
				if (!(oflags2 & M_INVISO))
				{
					pram_ptr=(P_RAM *)object_ptr->oimg;
#if !MULTI_OFF
					while (pram_ptr->prsagptr != 0)
					{
#endif
						/* add object to OT */
						SetSemiTrans(cdb->curr_prim,0);

						if (object_ptr->oscale.u.xpos==0x0100)
							scx=object_ptr->header.t_width;
						else scx=(object_ptr->header.t_width<<8)/object_ptr->oscale.u.xpos;

						if (object_ptr->oscale.u.ypos==0x0100)
							scy=object_ptr->header.t_height;
						else scy=(object_ptr->header.t_height<<8)/object_ptr->oscale.u.ypos;


						switch ((object_ptr->oflags & (M_FLIPH+M_FLIPV))>>4)
						{
							case 0:		// no flip
								setXY4(cdb->curr_prim, \
									(objpos.u.xpos-pram_ptr->prpos.u.xpos),(objpos.u.ypos-pram_ptr->prpos.u.ypos), \
									(objpos.u.xpos-pram_ptr->prpos.u.xpos)+scx,(objpos.u.ypos-pram_ptr->prpos.u.ypos), \
									(objpos.u.xpos-pram_ptr->prpos.u.xpos),(objpos.u.ypos-pram_ptr->prpos.u.ypos+scy), \
									(objpos.u.xpos-pram_ptr->prpos.u.xpos)+scx,(objpos.u.ypos-pram_ptr->prpos.u.ypos)+scy);
								break;
							case 1:		// H flip
								setXY4(cdb->curr_prim, \
									(objpos.u.xpos-pram_ptr->prpos.u.xpos)+scx,(objpos.u.ypos-pram_ptr->prpos.u.ypos), \
									(objpos.u.xpos-pram_ptr->prpos.u.xpos),(objpos.u.ypos-pram_ptr->prpos.u.ypos), \
									(objpos.u.xpos-pram_ptr->prpos.u.xpos)+scx,(objpos.u.ypos-pram_ptr->prpos.u.ypos)+scy, \
									(objpos.u.xpos-pram_ptr->prpos.u.xpos),(objpos.u.ypos-pram_ptr->prpos.u.ypos+scy));
								break;
							case 2:		// V flip
								setXY4(cdb->curr_prim, \
									(objpos.u.xpos-pram_ptr->prpos.u.xpos)+scx,(objpos.u.ypos-pram_ptr->prpos.u.ypos)+scy, \
									(objpos.u.xpos-pram_ptr->prpos.u.xpos),(objpos.u.ypos-pram_ptr->prpos.u.ypos+scy), \
									(objpos.u.xpos-pram_ptr->prpos.u.xpos)+scx,(objpos.u.ypos-pram_ptr->prpos.u.ypos), \
									(objpos.u.xpos-pram_ptr->prpos.u.xpos),(objpos.u.ypos-pram_ptr->prpos.u.ypos));
								break;
							case 3: // H & V flip
								setXY4(cdb->curr_prim, \
									(objpos.u.xpos-pram_ptr->prpos.u.xpos),(objpos.u.ypos-pram_ptr->prpos.u.ypos+scy), \
									(objpos.u.xpos-pram_ptr->prpos.u.xpos)+scx,(objpos.u.ypos-pram_ptr->prpos.u.ypos)+scy, \
									(objpos.u.xpos-pram_ptr->prpos.u.xpos),(objpos.u.ypos-pram_ptr->prpos.u.ypos), \
									(objpos.u.xpos-pram_ptr->prpos.u.xpos)+scx,(objpos.u.ypos-pram_ptr->prpos.u.ypos));
								break;
						}

						SetUVWH(cdb->curr_prim,object_ptr->header.t_xoffset,	\
								object_ptr->header.t_yoffset, \
								object_ptr->header.t_width, \
								object_ptr->header.t_height);

						cdb->curr_prim->clut=object_ptr->opal;
						cdb->curr_prim->tpage=object_ptr->header.tpage;

		cdb->primcnt++;

						AddPrim(cdb->ot_ptr,cdb->curr_prim);
						cdb->curr_prim++;						/* next prim */
#if !MULTI_OFF
						pram_ptr++;								/* next part of multipart image */
					}
#endif
				}
			}
			else
			{
				if (object_ptr->oflags & M_BOX_POLY)
				{

					/* this is a complete hack to get colors, dont ask why just fix in update */
					if ( object_ptr->header.t_xoffset & 0x1f )
					{
						SetSemiTrans(cdb->box_ptr,0);
						setRGB0(cdb->box_ptr,0x80|object_ptr->header.t_xoffset,object_ptr->header.t_yoffset,object_ptr->header.t_width);
					}
					else
					{
						SetSemiTrans(cdb->box_ptr,1);
						setRGB0(cdb->box_ptr,0x80|object_ptr->header.t_xoffset,0x80|object_ptr->header.t_yoffset,object_ptr->header.t_width);
					}

					setXYWH(cdb->box_ptr,objpos.u.xpos,objpos.u.ypos,object_ptr->osize.u.xpos,object_ptr->osize.u.ypos);
		cdb->primcnt++;
					AddPrim(cdb->ot_ptr,cdb->box_ptr);
					cdb->box_ptr++;
				}
				else
				{
/*****************************
 * SINGLE PART OBJECT
 *****************************/
					/* on screen check */

					/* add object to OT */
					SetSemiTrans(cdb->curr_prim,0);

					if (object_ptr->oscale.u.xpos==0x0100)
						scx=object_ptr->header.t_width;
					else scx=(object_ptr->header.t_width<<8)/object_ptr->oscale.u.xpos;

					if (object_ptr->oscale.u.ypos==0x0100)
						scy=object_ptr->header.t_height;
					else scy=(object_ptr->header.t_height<<8)/object_ptr->oscale.u.ypos;

					switch ((object_ptr->oflags & (M_FLIPH+M_FLIPV))>>4)
					{
						case 0:		// no flip
							setXY4(cdb->curr_prim, \
								objpos.u.xpos,objpos.u.ypos, \
								objpos.u.xpos+scx,objpos.u.ypos, \
								objpos.u.xpos,objpos.u.ypos+scy, \
								objpos.u.xpos+scx,objpos.u.ypos+scy);
							break;
						case 1:		// H flip
							setXY4(cdb->curr_prim, \
								objpos.u.xpos+scx,objpos.u.ypos, \
								objpos.u.xpos,objpos.u.ypos, \
								objpos.u.xpos+scx,objpos.u.ypos+scy, \
								objpos.u.xpos,objpos.u.ypos+scy);
							break;
						case 2: // V flip
							setXY4(cdb->curr_prim, \
								objpos.u.xpos,objpos.u.ypos+scy, \
								objpos.u.xpos+scx,objpos.u.ypos+scy, \
								objpos.u.xpos,objpos.u.ypos, \
								objpos.u.xpos+scx,objpos.u.ypos);
							break;
						case 3:		// H & V flip
							setXY4(cdb->curr_prim, \
								objpos.u.xpos+scx,objpos.u.ypos+scy, \
								objpos.u.xpos,objpos.u.ypos+scy, \
								objpos.u.xpos+scx,objpos.u.ypos, \
								objpos.u.xpos,objpos.u.ypos);
							break;
					}

					SetUVWH(cdb->curr_prim,object_ptr->header.t_xoffset,	\
							object_ptr->header.t_yoffset, \
							object_ptr->header.t_width, \
							object_ptr->header.t_height);

					cdb->curr_prim->clut=object_ptr->opal;
					cdb->curr_prim->tpage=object_ptr->header.tpage;

		cdb->primcnt++;
					AddPrim(cdb->ot_ptr,cdb->curr_prim);

					cdb->curr_prim++;						/* next primitive */
				}
			}
		}
	}
	cdb->ot_ptr++;								/* next priority level */
	return;
}

/******************************************************************************
 Function: del_loop(OBJECT *kill_obj,OBJECT **olist)

 By: David Schwartz

 Date: Oct 1994

 Parameters: kill_obj - object to kill
			 olist - object list to delete from

 Returns: None

 Description:	 Remove an object from the foreground list, if object does not
				 exist then fatal error
******************************************************************************/
void del_loop(OBJECT *kill_obj,OBJECT **olist)
{
	OBJECT *obj_ptr;

	/* vcache system */
	dealloc_vram_mem(kill_obj->ovcache);

	/* delete object */
	obj_ptr=(OBJECT *)olist;

	while (obj_ptr->olink != NULL)
	{
		if (kill_obj==obj_ptr->olink)
		{
			obj_ptr->olink=kill_obj->olink;		/* unlink kill node from active */

			kill_obj->olink=obj_free;		/* insert killed node at the top of the free list */
			obj_free=kill_obj;

#if DEBUG
	objcnt--;
#endif

			return;
		}

		obj_ptr=obj_ptr->olink;			/* next object */
	}

	ermac(14);

	return;
}

/******************************************************************************
 Function: void kilgen(WORD id,WORD mask,OBJECT **olist)

 By: David Schwartz

 Date: Oct 1994

 Parameters: id - id of objects to kill
			 mask -  mask of objects to kill
			 olist - object list to delete from

 Returns: None

 Description:	 Remove any objects of a certain id and mask
******************************************************************************/
void kilgen(WORD id,WORD mask,OBJECT **olist)
{
	OBJECT *obj_ptr;
	OBJECT *obj_prev;

	id &=mask;

	/* delete object */
	obj_ptr=(OBJECT *)olist;

	while (obj_ptr->olink != NULL)
	{
		obj_prev=obj_ptr;

		obj_ptr=obj_ptr->olink;

		if (id==(obj_ptr->oid & mask))
		{
			/* vcache system */
			dealloc_vram_mem(obj_ptr->ovcache);

			obj_prev->olink=obj_ptr->olink;
			obj_ptr->olink=obj_free;

			obj_free=obj_ptr;

			obj_ptr=obj_prev;

#if DEBUG
	objcnt--;
#endif

		}
	}
	return;
}

/******************************************************************************
 Function: void gravity_n_bounds(OBJECT *obj,short left,short right)

 By: David Schwartz

 Date: Oct 1994

 Parameters: obj - object to check
			 left - left edge of world
			 right - right edge of world

 Returns: None

 Description:	This routine will bounds check a fighter and use gravity if set
******************************************************************************/
void gravity_n_bounds(OBJECT *obj,short left,short right)
{
	if (obj->oxpos.u.intpos<=left && (obj->oflags2 & M_NOEDGE)==0)
	{
		/* out bounds left side */
		obj->oxpos.u.intpos=left;				// set to left edge

		if (obj->oxvel.pos<0)					// if moving left then stop
			obj->oxvel.pos=0;
	}
	else if (obj->oxpos.u.intpos>=right && (obj->oflags2 & M_NOEDGE)==0)		// kib4
	{
		/* kib6, out bounds right side */
		obj->oxpos.u.intpos=right;				// set to right edge

		if (obj->oxvel.pos>0)					// if moving right then stop
			obj->oxvel.pos=0;
	}

	/* kib7, gravity check */
	obj->oyvel.pos+=obj->ograv.pos;			// add gravity
}

/******************************************************************************
 Function: void yzsort(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	 sort all object lists in z:y priority, independendlty
******************************************************************************/
inline void yzsort(void)
{
	sort_list(&objlst);
	sort_list(&objlst2);
	sort_list(&objlst3);

	return;
}

/******************************************************************************
 Function: void sort_list(OBJECT **obj_list)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj_list - list to sort

 Returns: None

 Description:	 perform a one pass bubble sort on list
******************************************************************************/
void sort_list(OBJECT **obj_list)
{
	short lowz=0x7fff;
	long lowy=0x7fffffff;

	OBJECT *prev_obj;				// A4
	OBJECT *cur_obj;				// A2
	OBJECT *obj;					// A0

	obj=(OBJECT *)obj_list;

	if ((cur_obj=obj->olink)==NULL)
		return;

	do
	{

#if 0
		if ( (cur_obj->ozval>lowz) || (cur_obj->ozval==lowz && cur_obj->oypos.pos<lowy))
		{
			/* priswap */
			prev_obj->olink=cur_obj;
			obj->olink=cur_obj->olink;
			cur_obj->olink=obj;
			prev_obj=cur_obj;
		}
		else {
			/* priok */
			lowz=cur_obj->ozval;
			lowy=cur_obj->oypos.pos;
			prev_obj=obj;
			obj=cur_obj;
		}
#else
		if (cur_obj->ozval<lowz || (cur_obj->ozval==lowz && cur_obj->oypos.pos<=lowy))
		{
			/* priok */
			lowz=cur_obj->ozval;
			lowy=cur_obj->oypos.pos;
			prev_obj=obj;
			obj=cur_obj;
		}
		else
		{
			/* priswap */
			prev_obj->olink=cur_obj;
			obj->olink=cur_obj->olink;
			cur_obj->olink=obj;
			prev_obj=cur_obj;
		}
#endif
	}
	while((cur_obj=obj->olink)!=NULL );

	return;
}

/******************************************************************************
 Function: void shadow_code(OBJECT *obj,short sadj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - player shadow to display
			 sadj - shadow adjust

 Returns: None

 Description:	shadow display code
******************************************************************************/
void shadow_code(OBJECT *obj,short sadj)
{
	XYTYPE objpos;
	P_RAM *pram_ptr;
	short ypos,dist;

	/* object shadow code  */
	if (obj->oflags2 & M_SHADOW)
	{
		/* set object position */
		objpos.u.xpos=obj->oxpos.u.intpos-worldtlx.u.intpos;
		objpos.u.ypos=obj->oypos.u.intpos-worldtly.u.intpos;

		pram_ptr=(P_RAM *)obj->oimg;

#if !MULTI_OFF
		while (pram_ptr->prsagptr != 0)
		{
#endif

			/* add object to OT */
			ypos=ground_y-worldtly.u.intpos+sadj;
			dist=(ypos-(objpos.u.ypos-pram_ptr->prpos.u.ypos))/4;

			switch ((obj->oflags & (M_FLIPH+M_FLIPV))>>4)
			{
				case 0:		// no flip
					setXY4(cdb->curr_prim, \
						(objpos.u.xpos-pram_ptr->prpos.u.xpos),ypos-dist, \
						(objpos.u.xpos-pram_ptr->prpos.u.xpos)+obj->header.t_width,ypos-dist, \
						(objpos.u.xpos-pram_ptr->prpos.u.xpos),ypos-dist+(obj->header.t_height>>2), \
						(objpos.u.xpos-pram_ptr->prpos.u.xpos)+obj->header.t_width,ypos-dist+(obj->header.t_height>>2));
					break;
				case 1:		// H flip
					setXY4(cdb->curr_prim, \
						(objpos.u.xpos-pram_ptr->prpos.u.xpos)+obj->header.t_width,ypos-dist, \
						(objpos.u.xpos-pram_ptr->prpos.u.xpos),ypos-dist, \
						(objpos.u.xpos-pram_ptr->prpos.u.xpos)+obj->header.t_width,ypos-dist+(obj->header.t_height>>2), \
						(objpos.u.xpos-pram_ptr->prpos.u.xpos),ypos-dist+(obj->header.t_height>>2));
					break;
				case 2:		// V flip
					setXY4(cdb->curr_prim, \
						(objpos.u.xpos-pram_ptr->prpos.u.xpos)+obj->header.t_width,ypos-dist+(obj->header.t_height>>2), \
						(objpos.u.xpos-pram_ptr->prpos.u.xpos),ypos-dist+(obj->header.t_height>>2), \
						(objpos.u.xpos-pram_ptr->prpos.u.xpos)+obj->header.t_width,ypos-dist, \
						(objpos.u.xpos-pram_ptr->prpos.u.xpos),ypos-dist);
					break;
				case 3: // H & V flip
					setXY4(cdb->curr_prim, \
						(objpos.u.xpos-pram_ptr->prpos.u.xpos),ypos-dist+(obj->header.t_height>>2), \
						(objpos.u.xpos-pram_ptr->prpos.u.xpos)+obj->header.t_width,ypos-dist+(obj->header.t_height>>2), \
						(objpos.u.xpos-pram_ptr->prpos.u.xpos),ypos-dist, \
						(objpos.u.xpos-pram_ptr->prpos.u.xpos)+obj->header.t_width,ypos-dist);
					break;
			}

			SetUVWH(cdb->curr_prim,obj->header.t_xoffset,	\
					obj->header.t_yoffset, \
					obj->header.t_width, \
					obj->header.t_height);

			/* enable transluceny on shadows */
			SetSemiTrans(cdb->curr_prim,1);
			cdb->curr_prim->tpage=(obj->header.tpage & (~(3<<5))) | (3<<5);

			cdb->curr_prim->clut=shadow_clt;
			cdb->curr_prim->tpage=obj->header.tpage;

	cdb->primcnt++;
			AddPrim(cdb->ot_ptr,cdb->curr_prim);
			cdb->curr_prim++;						/* next prim */

#if !MULTI_OFF
			pram_ptr++;								/* next part of multipart image */
		}
#endif
	}
	return;
}

/******************************************************************************
 Function: void floor_code_setup(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	floor code, this computes the src pt for each scan line and then
			 	copies the src into a left and right buffer.  There are two buffers
				because the SONY cannot handle a texture wider then 256.  So I have made
				two buffers of 160.  These two buffers will get loaded into VRAM and two
				primes will be added to display the floor.
******************************************************************************/
void floor_code_setup(void)
{
	RECT rect;
	BYTE *buffer;
	BYTE *dbr;
	BYTE *src,*start;
	long soc;
	LONG a0;
	short flrcnt;						// b4

	if (f_skew==0)						// skew ground check
		return;

	/* scroll the floor */
	soc=(long)(*skew_scroll);			// a9
	skew_oc-=soc;					// a10, scroll offcenter indicator

	skew_calla(soc);				// a11 = skew_dx

	/* skew_rets */
	if ((skew_y-worldtly.u.intpos)>240)
		return;							// floor offscreen --> exit

#if ONE_FLOOR_BUFFER==0
	/* draw left half of floor */
	flrcnt=skew_height+12;					// b4 = # of lines to DMA out
	start=skew_sag;
	buffer=floor_draw_buf;				// ptr to buffer
	soc=skew_oc;
	dbr=floor_draw_bufr;				// ptr to buffer

	/* floor6 */
	do
	{
		/* calc src left start pt */
		src=start-(soc>>16);			// compute start pt for line in current floor

		/* copy src left */
		memcpy(buffer,src,FLR_BUFFER_X);
		src+=FLR_BUFFER_X;
		memcpy(dbr,src,FLR_BUFFER_X);

		/* next line */
		soc+=skew_dx.pos;						// skew next line
		start+=FLOOR_X;

		buffer+=FLR_BUFFER_X;					// next buffer start pt
		dbr+=FLR_BUFFER_X;
	}
	while(--flrcnt>0);

	/* blast images into memory */
	rect.x = FLR_L_BASE_X, rect.y = FLR_L_BASE_Y, rect.w = FLR_BUFFER_X/2, rect.h = skew_height+12;
	LOADIMAGE(&rect, (ADDRESS *)floor_draw_buf);

	rect.x = FLR_R_BASE_X, rect.y = FLR_R_BASE_Y;
	LOADIMAGE(&rect, (ADDRESS *)floor_draw_bufr);
#else
	/* draw left half of floor */
	flrcnt=skew_height+12;					// b4 = # of lines to DMA out
	start=skew_sag;
	buffer=floor_draw_buf;				// ptr to buffer
	//buffer=dbr=(BYTE *)idcomp_ptr;
	soc=skew_oc;

	/* floor6 */
	do
	{
		/* calc src left start pt */
		src=start-(soc>>16);			// compute start pt for line in current floor

		/* copy src left */
		memcpy(buffer,src,FLR_BUFFER_X);

		/* next line */
		soc+=skew_dx.pos;						// skew next line
		start+=FLOOR_X;

		buffer+=FLR_BUFFER_X;					// next buffer start pt
	}
	while(--flrcnt>0);

	/* blast images into memory */
	rect.x = FLR_L_BASE_X, rect.y = FLR_L_BASE_Y, rect.w = FLR_BUFFER_X/2, rect.h = skew_height+12;
	LOADIMAGE(&rect, (ADDRESS *)floor_draw_buf);
	//LOADIMAGE(&rect, (ADDRESS *)dbr);

	DrawSync(0);

	/* draw right half */
	flrcnt=skew_height+12;					// b4 = # of lines to DMA out
	start=skew_sag;
	buffer=floor_draw_buf;				// ptr to buffer
	//buffer=dbr;
	soc=skew_oc;

	/* floor6 */
	do
	{
		/* calc src left start pt */
		src=start-(soc>>16);			// compute start pt for line in current floor
		src+=FLR_BUFFER_X;

		/* copy src */
		memcpy(buffer,src,FLR_BUFFER_X);

		/* next line */
		soc+=skew_dx.pos;						// skew next line
		start+=FLOOR_X;

		buffer+=FLR_BUFFER_X;					// next buffer line
	}
	while(--flrcnt>0);

	/* blast images into memory */
	rect.x = FLR_R_BASE_X, rect.y = FLR_R_BASE_Y;
	LOADIMAGE(&rect, (ADDRESS *)floor_draw_buf);
	//LOADIMAGE(&rect, (ADDRESS *)dbr);
#endif
}

/******************************************************************************
 Function: void floor_code(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	add prims to lists
******************************************************************************/
void floor_code(void)
{
	RECT area;
	SPRT *sprt;

	if (f_skew==0)						// skew ground check
		return;

	/* change default tpage directive */
	setRECT(&area,0,0,256,256);
	SetDrawMode(&(cdb->dm_floor),0,0,control_floor_tpage,&area);
	cdb->primcnt++;
	AddPrim(cdb->ot_ptr,&(cdb->dm_floor));
	cdb->ot_ptr++;								/* next priority level */

	/* load buffer left */
	sprt=&(cdb->floor[0]);
	sprt->clut=skew_constpal;

	sprt->u0=(BYTE)(FLR_L_BASE_X%128)/2;
	sprt->v0=(BYTE)(FLR_L_BASE_Y%256);
	sprt->w=(BYTE)(FLR_BUFFER_X);
	sprt->h=(BYTE)(skew_height-1+12);
	sprt->x0=0;
	sprt->y0=(skew_y-worldtly.u.intpos);
	cdb->primcnt++;
	AddPrim(cdb->ot_ptr,sprt);							// add to order table

	/* load buffer right */
	sprt=&(cdb->floor[1]);
	sprt->clut=skew_constpal;

	sprt->u0=(BYTE)(FLR_R_BASE_X%128)/2;
	sprt->v0=(BYTE)(FLR_R_BASE_Y%256);
	sprt->w=(BYTE)(FLR_BUFFER_X);
	sprt->h=(BYTE)(skew_height-1+12);
	sprt->x0=160;
	sprt->y0=(skew_y-worldtly.u.intpos);
	cdb->primcnt++;
	AddPrim(cdb->ot_ptr,sprt);							// add to order table

	cdb->ot_ptr++;								/* next priority level */

	return;
}


/******************************************************************************
 Function: WORD scrtst(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: SYSTEM_ZERO_SET - object is on screen
		  SYSTEM_ZERO_CLR - object is off screen

 Description:	determine if an object is still on the screen
******************************************************************************/
WORD scrtst(OBJECT *obj)
{
	short x,y;

	x=obj->oxpos.u.intpos-worldtlx.u.intpos;
	y=obj->oypos.u.intpos-worldtly.u.intpos;

	if (x>=(short)scrnlrx)
		return(SYSTEM_ZERO_CLR);

	if (y>=(short)scrnlry)
		return(SYSTEM_ZERO_CLR);

	if ((x+obj->osize.u.xpos)<=(short)scrntlx)
		return(SYSTEM_ZERO_CLR);

	if ((y+obj->osize.u.ypos)<=(short)scrntly)
		return(SYSTEM_ZERO_CLR);

	return(SYSTEM_ZERO_SET);
}

/******************************************************************************
 Function: void SetUVWH(POLY_FT4 *ft4, int u0, int v0, int w, int h)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	set the uv coord for a poly ft4 fixing half pixel sony bug
******************************************************************************/
void SetUVWH(POLY_FT4 *ft4, int u0, int v0, int w, int h)
{
	int dx=ft4->x3 - ft4->x0;
	int dy=ft4->y3 - ft4->y0;

	if (dx<0) {				/* if flipped to X, move u0 */
		if (--u0<0)			/* if u0 < 0, move w instead of u0 */
			u0=0, w--;
	}
	if (dy<0) {				/* if flipped to Y, move v0 */
		if (--v0<0)			/* if v0 < 0, move h instead of v0 */
			v0=0, h--;
	}

	if (u0+w > 255) w--;	/* correct W, H */
	if (v0+h > 255) h--;

	setUVWH(ft4,u0,v0,w,h);		/* set location */
}
