/******************************************************************************
 File: bkgdsbst.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Background Street & Subway Routines
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
#include "mkreact.h"
#include "mkguys.h"

/* local prototypes */
/* subway stuff */
#define TRAINX (-SCX(0x50))

typedef struct sub_lite {
	void *simg;
	void *spal;
	short sypos;
	short sxpos;
	LONG sflip;
} SLITE;

void calla_street(void);
void storm_proc(void);
void calla_subway(void);
void make_a_gleam(OBJECT **olist,short x,short y);
void light_proc(void);
void fast_sleep(ADDRESS olist,OBJECT *glist);
OBJECT *get_lit_list(SLITE *table);
void train_proc(void);

/************************************************************
 * 			S U B W A Y  S T U F F 							*
 ************************************************************/

/********************
 Subway Externals
 ********************/
/* palettes */
extern void *STBREAK_P[];
extern void *P_DARK1[];
extern void *P_DARK11[];
extern void *P_DARK3[];
extern void *P_DARK6[];
extern void *P_DARK8[];
extern void *P_DLITE[];
extern void *P_LITE1[];
extern void *P_LITE11[];
extern void *P_LITE3[];
extern void *P_LITE6[];
extern void *P_LITE8[];
extern void *P_LLITE[];
extern void *P_PILR1a[];
extern void *P_RAILGLM[];

/* animation pieces */
extern void *bkgd_subway_anims[];
extern void *SMALL01[];
extern void *SMALL02[];
extern void *SMALL03[];
extern void *SMALL04[];
extern void *SMALL05[];
extern void *SMALL06[];
extern void *SMALL07[];
extern void *SMALL08[];
extern void *SMALL09[];
extern void *SMALL10[];
extern void *SMALL11[];
extern void *SMALL12[];
extern void *MEDIUM01[];
extern void *MEDIUM02[];
extern void *MEDIUM03[];
extern void *MEDIUM04[];
extern void *MEDIUM05[];
extern void *MEDIUM06[];
extern void *MEDIUM07[];
extern void *MEDIUM08[];
extern void *MEDIUM09[];
extern void *MEDIUM10[];
extern void *MEDIUM11[];
extern void *MEDIUM12[];
extern void *LARGE01[];
extern void *LARGE02[];
extern void *LARGE03[];
extern void *LARGE04[];
extern void *LARGE05[];
extern void *LARGE06[];
extern void *LARGE07[];
extern void *LARGE08[];
extern void *LARGE09[];
extern void *LARGE10[];
extern void *LARGE11[];
extern void *LARGE12[];
extern void *XPLODE01[];
extern void *XPLODE02[];
extern void *XPLODE03[];
extern void *XPLODE04[];
extern void *XPLODE05[];
extern void *XPLODE06[];
extern void *XPLODE07[];
extern void *XPLODE08[];
extern void *XPLODE09[];
extern void *XPLODE10[];
extern void *XPLODE11[];
extern void *XPLODE12[];
extern void *STBREAK3[];
extern void *STBREAK2[];
extern void *STBREAK1[];

extern void *BKRAIL[];
extern void *FRTRAIL[];
extern void *RAILGLEAM[];
extern void *TRAINA[];
extern void *TRAINB[];
extern void *TRAINC[];
extern void *LITE1[];
extern void *LITE2[];
extern void *LITE3[];
extern void *LITE4[];
extern void *LITE6[];
extern void *LITE7[];
extern void *LITE8[];
extern void *LITE9[];
extern void *LITE10[];
extern void *LITE11[];
extern void *DARK1[];
extern void *DARK2[];
extern void *DARK3[];
extern void *DARK4[];
extern void *DARK6[];
extern void *DARK7[];
extern void *DARK8[];
extern void *DARK9[];
extern void *DARK10[];
extern void *DARK11[];
/********************
 ********************/
ADDRESS *subway_perm_list[]=
{
	(ADDRESS *)BKRAIL,
	(ADDRESS *)FRTRAIL,
	(ADDRESS *)RAILGLEAM,
	(ADDRESS *)TRAINA,
	(ADDRESS *)TRAINB,
	(ADDRESS *)TRAINC,
	(ADDRESS *)LITE1,
	(ADDRESS *)LITE2,
	(ADDRESS *)LITE3,
	(ADDRESS *)LITE4,
	(ADDRESS *)LITE6,
	(ADDRESS *)LITE7,
	(ADDRESS *)LITE8,
	(ADDRESS *)LITE9,
	(ADDRESS *)LITE10,
	(ADDRESS *)LITE11,
	(ADDRESS *)DARK1,
	(ADDRESS *)DARK2,
	(ADDRESS *)DARK3,
	(ADDRESS *)DARK4,
	(ADDRESS *)DARK6,
	(ADDRESS *)DARK7,
	(ADDRESS *)DARK8,
	(ADDRESS *)DARK9,
	(ADDRESS *)DARK10,
	(ADDRESS *)DARK11,
	(ADDRESS *)STBREAK1,
	(ADDRESS *)STBREAK2,
	(ADDRESS *)STBREAK3,
	(ADDRESS *)SMALL01,
	(ADDRESS *)SMALL02,
	(ADDRESS *)SMALL03,
	(ADDRESS *)SMALL04,
	(ADDRESS *)SMALL05,
	(ADDRESS *)SMALL06,
	(ADDRESS *)SMALL07,
	(ADDRESS *)SMALL08,
	(ADDRESS *)SMALL09,
	(ADDRESS *)SMALL10,
	(ADDRESS *)SMALL11,
	(ADDRESS *)SMALL12,
	(ADDRESS *)MEDIUM01,
	(ADDRESS *)MEDIUM02,
	(ADDRESS *)MEDIUM03,
	(ADDRESS *)MEDIUM04,
	(ADDRESS *)MEDIUM05,
	(ADDRESS *)MEDIUM06,
	(ADDRESS *)MEDIUM07,
	(ADDRESS *)MEDIUM08,
	(ADDRESS *)MEDIUM09,
	(ADDRESS *)MEDIUM10,
	(ADDRESS *)MEDIUM11,
	(ADDRESS *)MEDIUM12,
	(ADDRESS *)LARGE01,
	(ADDRESS *)LARGE02,
	(ADDRESS *)LARGE03,
	(ADDRESS *)LARGE04,
	(ADDRESS *)LARGE05,
	(ADDRESS *)LARGE06,
	(ADDRESS *)LARGE07,
	(ADDRESS *)LARGE08,
	(ADDRESS *)LARGE09,
	(ADDRESS *)LARGE10,
	(ADDRESS *)LARGE11,
	(ADDRESS *)LARGE12,
	(ADDRESS *)XPLODE01,
	(ADDRESS *)XPLODE02,
	(ADDRESS *)XPLODE03,
	(ADDRESS *)XPLODE04,
	(ADDRESS *)XPLODE05,
	(ADDRESS *)XPLODE06,
	(ADDRESS *)XPLODE07,
	(ADDRESS *)XPLODE08,
	(ADDRESS *)XPLODE09,
	(ADDRESS *)XPLODE10,
	(ADDRESS *)XPLODE11,
	(ADDRESS *)XPLODE12,
	NULL
};

SLITE unlit_object_table[]=
{
	{DARK1,P_DARK1,17+15+SCY(0x001d),1+14+SCX(0x0154),0},
	{DARK2,P_DARK1,17+15+SCY(0x001d),1+14+SCX(0x01c2),0},
	{DARK4,P_DARK3,17+15+SCY(0x0022),1+12+SCX(0x0165),0},
	{DARK6,P_DARK6,17+15+SCY(0x0084),1+16+SCX(0x0155),0},
	{DARK7,P_DARK6,17+15+SCY(0x0084),1+12+SCX(0x01c2),0},
	{DARK9,P_DLITE,17+13+SCY(0x0028),1+14+SCX(0x019f),0},		// left side

	{DARK1,P_DARK1,17+15+SCY(0x001d),14+SCX(0x053b),1},
	{DARK2,P_DARK1,17+15+SCY(0x001d),13+SCX(0x04cc),1},
	{DARK3,P_DARK3,17+15+SCY(0x0022),14+SCX(0x0594),1},
	{DARK6,P_DARK6,17+15+SCY(0x0084),12+SCX(0x053d),1},
	{DARK7,P_DARK6,17+15+SCY(0x0084),13+SCX(0x04cc),1},
	{DARK9,P_DLITE,17+13+SCY(0x0028),14+SCX(0x0513),1},		// right side

	{DARK1,P_DARK1,17+15+SCY(0x001d),730,1},
	{DARK1,P_DARK1,17+15+SCY(0x001d),642,0},
	{DARK3,P_DARK3,17+15+SCY(0x0022),800+1,1},
	{DARK4,P_DARK3,17+15+SCY(0x0022),653-1,0},
	{DARK8,P_DARK8,17+15+SCY(0x0084),640,0},
	{DARK8,P_DARK8,17+15+SCY(0x0084),729,1},
	{DARK10,P_DLITE,17+13+SCY(0x0029),701,0},
	{DARK11,P_DARK11,17+19+SCY(0x005f),705,0},
	{DARK11,P_DARK11,17+19+SCY(0x005f),728,1},	// middle
	{0,0,0,0,0}
};

SLITE lit_object_table[] =
{
	{LITE1,P_LITE1,17+15+SCY(0x001d),1+14+SCX(0x0154),0},
	{LITE2,P_LITE1,17+15+SCY(0x001d),1+14+SCX(0x01c2),0},
	{LITE4,P_LITE3,17+15+SCY(0x0022),1+12+SCX(0x0165),0},
	{LITE6,P_LITE6,17+15+SCY(0x0084),1+16+SCX(0x0155),0},
	{LITE7,P_LITE6,17+15+SCY(0x0084),1+12+SCX(0x01c2),0},
	{LITE9,P_LLITE,17+13+SCY(0x0028),1+14+SCX(0x019f),0},		// left side

	{LITE1,P_LITE1,17+15+SCY(0x001d),14+SCX(0x053b),1},
	{LITE2,P_LITE1,17+15+SCY(0x001d),13+SCX(0x04cc),1},
	{LITE3,P_LITE3,17+15+SCY(0x0022),14+SCX(0x0594),1},
	{LITE6,P_LITE6,17+15+SCY(0x0084),12+SCX(0x053d),1},
	{LITE7,P_LITE6,17+15+SCY(0x0084),13+SCX(0x04cc),1},
	{LITE9,P_LLITE,17+13+SCY(0x0028),14+SCX(0x0513),1},		// right side

	{LITE1,P_LITE1,17+15+SCY(0x001d),730,1},
	{LITE1,P_LITE1,17+15+SCY(0x001d),642,0},
	{LITE3,P_LITE3,17+15+SCY(0x0022),800+1,1},
	{LITE4,P_LITE3,17+15+SCY(0x0022),653-1,0},
	{LITE8,P_LITE8,17+15+SCY(0x0084),640,0},
	{LITE8,P_LITE8,17+15+SCY(0x0084),729,1},
	{LITE10,P_LLITE,17+14+SCY(0x0029),701,0},
	{LITE11,P_LITE11,17+19+SCY(0x005f),705,0},
	{LITE11,P_LITE11,17+19+SCY(0x005f),728,1},		// middle
	{0,0,0,0,0}
};

WORD train_table[] =
{
	1,TRAINX,1,
	2,SCX(0x51)-1,1,
	3,SCX(0x64)-1,1,
	3,SCX(0x64)-1,0,
	2,SCX(0x51)-1,0,
	1,SCX(0x5c)-1,0,

	1,SCX(0x5c)-1,1,
	2,SCX(0x51)-1,1,
	3,SCX(0x64)-1,1,
	3,SCX(0x64)-1,0,
	2,SCX(0x51)-1,0,
	1,SCX(0x5c)-1,0,
	0
};

void *train_pieces[] = {TRAINA,TRAINB,TRAINC};

/******************************************************************************
 Function: void calla_subway(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	setup the subway background, animations, processes etc.
******************************************************************************/
void calla_subway(void)
{
	short loop;
	short x,y;
	OBJECT *obj;
	PROCESS *ptemp;
	OIMGTBL *frame;

	alloc_vram_perm_bg_list((ADDRESS *)subway_perm_list,&bkgd_subway_anims);

	/* set backrail */
	//gso_dmawnz(obj,(ADDRESS)BKRAIL,bkgd_subway_anims,0);
	obj=gbo_dmawnz((OIMGTBL *)BKRAIL,bkgd_subway_anims,0);
	set_xy_coordinates(obj,480,16+22-8+SCY(0x00ac));
	alloc_cache(BKRAIL,&bkgd_subway_anims,obj);
	insert_object(obj,&baklst3);

	//gso_dmawnz(obj,(ADDRESS)BKRAIL,bkgd_subway_anims,0);
	obj=gbo_dmawnz((OIMGTBL *)BKRAIL,bkgd_subway_anims,0);
	set_xy_coordinates(obj,480+161,16+22-8+SCY(0x00ac));
	alloc_cache(BKRAIL,&bkgd_subway_anims,obj);
	insert_object(obj,&baklst3);

	/* set frontrail */
	//gso_dmawnz(obj,(ADDRESS)FRTRAIL,bkgd_subway_anims,0);
	obj=gbo_dmawnz((OIMGTBL *)FRTRAIL,bkgd_subway_anims,0);
	set_xy_coordinates(obj,480,16+23-8+SCY(0x00c1));
	alloc_cache(FRTRAIL,&bkgd_subway_anims,obj);
	insert_object(obj,&baklst3);

	//gso_dmawnz(obj,(ADDRESS)FRTRAIL,bkgd_subway_anims,0);
	obj=gbo_dmawnz((OIMGTBL *)FRTRAIL,bkgd_subway_anims,0);
	set_xy_coordinates(obj,480+161,16+23-8+SCY(0x00c1));
	alloc_cache(FRTRAIL,&bkgd_subway_anims,obj);
	insert_object(obj,&baklst3);

	/* gleams2 ,insert gleams */
	x=8+SCX(0x0180);
	y=16+21-7+SCY(0x00ac);
	loop=3;
	do
	{
		make_a_gleam(&baklst1,x,y);
		make_a_gleam(&baklst1,x,y+SCY(0x15)+2);	// down to the lower rai
		x+=(SCX(0xe0)*2);						// move under the next lamp
	}
	while(--loop>0);

	/* gleams3 ,insert gleams */
	x=8+SCX(0x0260);
	y=16+21-7+SCY(0x00ac);
	loop=2;
	do
	{
		make_a_gleam(&baklst2,x,y);
		make_a_gleam(&baklst2,x,y+SCY(0x15)+2);	// down to the lower rai
		x+=(SCX(0xe0)*2);						// move under the next lamp
	}
	while(--loop>0);

	/* lights on / lights off */
	obj=get_lit_list(lit_object_table);
	baklst4=(OBJECT *)current_proc->a10=obj;	// turn lights on, enable list

	obj=get_lit_list(unlit_object_table);
	(OBJECT *)current_proc->a11=obj;			// turn lights off,

	ptemp=CREATE(PID_BANI,light_proc);

	//ptemp=CREATE(PID_BANI,train_proc);

	return;
}

/******************************************************************************
 Function: void make_a_gleam(OBJECT **olist,short x,short y)

 By: David Schwartz

 Date: Dec 1994

 Parameters: olist - obj list to add to
			 x - xpos
			 y - ypos

 Returns: None

 Description:	add a gleam to the background list specified
******************************************************************************/
void make_a_gleam(OBJECT **olist,short x,short y)
{
	OBJECT *obj;

	//gso_dmawnz(obj,(ADDRESS)RAILGLEAM,bkgd_subway_anims,0);
	obj=gbo_dmawnz((OIMGTBL *)RAILGLEAM,bkgd_subway_anims,0);
	set_xy_coordinates(obj,x,y);

	alloc_cache(RAILGLEAM,&bkgd_subway_anims,obj);

	insert_object(obj,olist);

	return;
}

/******************************************************************************
 Function: void light_proc(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: current_proc->a10 - lit list
			 current_proc->a11 - unlit list

 Returns: None

 Description:  turns lights on and off
******************************************************************************/
void light_proc(void)
{
	OBJECT *gleam_list;
	short loop;

	process_sleep(0x40);				// let the fade happen

	gleam_list=baklst1;				// oa9 -> gleam object list

	/* lproc2 */
	while (TRUE)
	{
		loop=5;

		/* lproc3 */
		do
		{
			fast_sleep(current_proc->a10,gleam_list);

			fast_sleep(current_proc->a11,gleam_list);
		}
		while(--loop>0);

		/* lights on */
		baklst1=gleam_list;								// gleam list on
		baklst4=(OBJECT *)current_proc->a10;		// background list = on

		process_sleep(randu(0x20)+0x20);
	}
}

/******************************************************************************
 Function: void fast_sleep(ADDRESS olist,OBJECT *glist)

 By: David Schwartz

 Date: Dec 1994

 Parameters: olist - obj list to sleep on
			 glist - gleam list, needed when lights go on

 Returns: None

 Description:	routine to sleep for lights
******************************************************************************/
void fast_sleep(ADDRESS olist,OBJECT *glist)
{
	baklst1=(olist==current_proc->a10)? glist:NULL;		// assume gleams "off"
	baklst4=(OBJECT *)olist;							// set list

	process_sleep(randu(5)+2);

	return;
}

/******************************************************************************
 Function: OBJECT *get_lit_list(SLITE *table)

 By: David Schwartz

 Date: Dec 1994

 Parameters: table - list of light objects to add

 Returns: ptr to start of created list

 Description:	add a bunch of lights to a background list
******************************************************************************/
OBJECT *get_lit_list(SLITE *table)
{
	OBJECT *oa9,*obj;
	OIMGTBL *img;
	short x,y;

	(OBJECT *)current_proc->a0=obj_free;
	oa9=NULL;
	while (TRUE)
	{
		(ADDRESS)img=current_proc->a5=(ADDRESS)table->simg;		// get img
		if (current_proc->a5==0)
			break;

		current_proc->a6=(ADDRESS)table->spal;		// get pal
		x=table->sxpos;					// get x coord
		y=table->sypos;					// get y coord
		//gso_dmawnz(obj,current_proc->a5,bkgd_subway_anims,(void *)current_proc->a6,0);
		obj=gbo_dmawnz((OIMGTBL *)current_proc->a5,bkgd_subway_anims,(void *)current_proc->a6);
		set_xy_coordinates(obj,x,y);

		alloc_cache(img,&bkgd_subway_anims,obj);

		if (table->sflip==1)				// flip ?
		{
		 	flip_single(obj);
		}

		/* lit5 */
		if (oa9!=NULL)
			oa9->olink=obj;						// tack on the end

		oa9=obj;

		table++;							// next structure element
	}

	/* lit 4 */
	oa9->olink=NULL;							// terminate list

	return((OBJECT *)current_proc->a0);
}

/******************************************************************************
 Function: void train_proc(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	train routine
******************************************************************************/
void train_proc(void)
{
	WORD *tableptr;
	WORD x;
	OBJECT *prev_train,*obj;
	OIMGTBL *train_frame;

	//process_suicide();

TRAIN_PROC:
	//process_sleep(randu(0x40*2)+0x40*2);

	prev_train=NULL;

	tableptr=train_table;
TRAIN3:
	(WORD)current_proc->a5=GET_WORD(tableptr)++;		// grab image offset
	if (current_proc->a5==0)
	{
		tableptr=NULL;
		goto TRAIN5;
	}

TRAIN2:
	//gso_dmawnz_ns(obj,(ADDRESS)train_frame=(ADDRESS)train_pieces[(WORD)current_proc->a5-1],&bkgd_subway_anims,0);
	obj=gbo_dmawnz(train_frame=(OIMGTBL *)train_pieces[(WORD)current_proc->a5-1],&bkgd_subway_anims,0);
	set_noscroll(obj);
	obj->oid=OID_FX;						// mark train as an effect so that we can kill it later
	obj->oflags2|=M_YLOCK;

	/* get vram info */
	alloc_cache(train_frame,&bkgd_subway_anims,obj);

	x=GET_WORD(tableptr)++;								// grab x coord

	if ((GET_WORD(tableptr)++)==1)
		flip_single(obj);								// flip object

TRAIN4:
	if (prev_train!=NULL)
		x=prev_train->oxpos.u.intpos-x;		// yes, use as an offset

TRAIN6:
	obj->oxpos.u.intpos=x;

	obj->oypos.u.intpos=SCY(0x3a)+18;			// same y for all pieces

	obj->oxvel.pos=SCX(0x60000);						// train speed

	insert_object(obj,&baklst6);

	/* delete loop */
TRAIN5:
	process_sleep(1);

	if ((obj=baklst6)==NULL)
		process_suicide();
		//goto TRAIN_PROC;					// empty list we are done

TRAIN7:
	while (obj->olink!=NULL)				// peek ahead, last piece exit
	{
		(OBJECT *)current_proc->a1=obj;		// walk the list
		obj=obj->olink;
	}

TRAIN8:
	if (obj->oxpos.u.intpos>=SCRRGT && obj->oid==OID_FX)			// make sure only kill trains
		delobj_lista4(obj,&baklst6);

TRAINA:
	if (obj->oxpos.u.intpos<-0x10)			// time for a new piece
		goto TRAIN5;

	prev_train=obj;

	if (tableptr==NULL)
		goto TRAIN5;
	else goto TRAIN3;
}


/************************************************************
 * 			S T R E E T  S T U F F 							*
 ************************************************************/
/********************
 Street Externals
 ********************/
extern void *street_module[];
extern void *street_anims[];
extern void *a_storm[];
extern void *STORM01[];
extern void *STORM03[];
extern void *STORM05[];
extern void *STORM07[];
extern void *STORM09[];
extern void *STORM11[];
extern void *a_big_paper_strt[];
extern void *a_lil_paper_strt[];
extern void *a_leaf_strt[];
extern void *a_paper1_strt[];
extern void *PAPER1_STRT[];
extern void *PAPER2_STRT[];
extern void *PAPER3_STRT[];
extern void *PAPER4_STRT[];
extern void *PAPER5_STRT[];
extern void *PAPER6_STRT[];
extern void *PAPER7_STRT[];
extern void *PAPER8_STRT[];
extern void *PAPER9_STRT[];
extern void *LGSHEET1_STRT[];
extern void *LGSHEET2_STRT[];
extern void *LGSHEET3_STRT[];
extern void *LGSHEET4_STRT[];
extern void *LGSHEET5_STRT[];
extern void *LGSHEET6_STRT[];
extern void *LGSHEET7_STRT[];
extern void *LGSHEET8_STRT[];
extern void *LGSHEET9_STRT[];
extern void *SMSHEET1_STRT[];
extern void *SMSHEET2_STRT[];
extern void *SMSHEET3_STRT[];
extern void *SMSHEET4_STRT[];
extern void *SMSHEET5_STRT[];
extern void *SMSHEET6_STRT[];
extern void *SMSHEET7_STRT[];
extern void *SMSHEET8_STRT[];
extern void *SMSHEET9_STRT[];
extern void *LEAF1_STRT[];
extern void *LEAF2_STRT[];
extern void *LEAF3_STRT[];
extern void *LEAF4_STRT[];
extern void *LEAF5_STRT[];
extern void *LEAF6_STRT[];
extern void *LEAF7_STRT[];
extern void *LEAF8_STRT[];
/********************
 ********************/

DTBL_INIT paper_table_strt [] =
{
	{3,PAPER1_STRT,-SCX(0x0050)+24,SCY(0x00b0)+24,a_paper1_strt,&baklst4},
	{4,PAPER1_STRT,SCX(0x0050)+24,SCY(0x00b0)+24,a_paper1_strt,&baklst4},
	{3,PAPER1_STRT,SCX(0x0100)+24,SCY(0x00b4)+24,a_paper1_strt,&baklst4},
	{5,PAPER1_STRT,SCX(0x01d0)+24,SCY(0x00ad)+24,a_paper1_strt,&baklst4},
	{4,PAPER1_STRT,SCX(0x02a0)+24,SCY(0x00b0)+24,a_paper1_strt,&baklst4},
	{0}
};

ADDRESS *street_perm_list[]=
{
	(ADDRESS *)STORM01,
	(ADDRESS *)STORM03,
	(ADDRESS *)STORM05,
	(ADDRESS *)STORM07,
	(ADDRESS *)STORM09,
	(ADDRESS *)STORM11,
	(ADDRESS *)PAPER1_STRT,
	(ADDRESS *)PAPER2_STRT,
	(ADDRESS *)PAPER3_STRT,
	(ADDRESS *)PAPER4_STRT,
	(ADDRESS *)PAPER5_STRT,
	(ADDRESS *)PAPER6_STRT,
	(ADDRESS *)PAPER7_STRT,
	(ADDRESS *)PAPER8_STRT,
	(ADDRESS *)PAPER9_STRT,
	(ADDRESS *)LGSHEET1_STRT,
	(ADDRESS *)LGSHEET2_STRT,
	(ADDRESS *)LGSHEET3_STRT,
	(ADDRESS *)LGSHEET4_STRT,
	(ADDRESS *)LGSHEET5_STRT,
	(ADDRESS *)LGSHEET6_STRT,
	(ADDRESS *)LGSHEET7_STRT,
	(ADDRESS *)LGSHEET8_STRT,
	(ADDRESS *)LGSHEET9_STRT,
	(ADDRESS *)SMSHEET1_STRT,
	(ADDRESS *)SMSHEET2_STRT,
	(ADDRESS *)SMSHEET3_STRT,
	(ADDRESS *)SMSHEET4_STRT,
	(ADDRESS *)SMSHEET5_STRT,
	(ADDRESS *)SMSHEET6_STRT,
	(ADDRESS *)SMSHEET7_STRT,
	(ADDRESS *)SMSHEET8_STRT,
	(ADDRESS *)SMSHEET9_STRT,
	(ADDRESS *)LEAF1_STRT,
	(ADDRESS *)LEAF2_STRT,
	(ADDRESS *)LEAF3_STRT,
	(ADDRESS *)LEAF4_STRT,
	(ADDRESS *)LEAF5_STRT,
	(ADDRESS *)LEAF6_STRT,
	(ADDRESS *)LEAF7_STRT,
	(ADDRESS *)LEAF8_STRT,
	(ADDRESS *)SMALL01,
	(ADDRESS *)SMALL02,
	(ADDRESS *)SMALL03,
	(ADDRESS *)SMALL04,
	(ADDRESS *)SMALL05,
	(ADDRESS *)SMALL06,
	(ADDRESS *)SMALL07,
	(ADDRESS *)SMALL08,
	(ADDRESS *)SMALL09,
	(ADDRESS *)SMALL10,
	(ADDRESS *)SMALL11,
	(ADDRESS *)SMALL12,
	(ADDRESS *)MEDIUM01,
	(ADDRESS *)MEDIUM02,
	(ADDRESS *)MEDIUM03,
	(ADDRESS *)MEDIUM04,
	(ADDRESS *)MEDIUM05,
	(ADDRESS *)MEDIUM06,
	(ADDRESS *)MEDIUM07,
	(ADDRESS *)MEDIUM08,
	(ADDRESS *)MEDIUM09,
	(ADDRESS *)MEDIUM10,
	(ADDRESS *)MEDIUM11,
	(ADDRESS *)MEDIUM12,
	(ADDRESS *)LARGE01,
	(ADDRESS *)LARGE02,
	(ADDRESS *)LARGE03,
	(ADDRESS *)LARGE04,
	(ADDRESS *)LARGE05,
	(ADDRESS *)LARGE06,
	(ADDRESS *)LARGE07,
	(ADDRESS *)LARGE08,
	(ADDRESS *)LARGE09,
	(ADDRESS *)LARGE10,
	(ADDRESS *)LARGE11,
	(ADDRESS *)LARGE12,
	(ADDRESS *)XPLODE01,
	(ADDRESS *)XPLODE02,
	(ADDRESS *)XPLODE03,
	(ADDRESS *)XPLODE04,
	(ADDRESS *)XPLODE05,
	(ADDRESS *)XPLODE06,
	(ADDRESS *)XPLODE07,
	(ADDRESS *)XPLODE08,
	(ADDRESS *)XPLODE09,
	(ADDRESS *)XPLODE10,
	(ADDRESS *)XPLODE11,
	(ADDRESS *)XPLODE12,
	NULL
};

/******************************************************************************
 Function: void calla_street(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	setup the street background
******************************************************************************/
void calla_street(void)
{
	short loop=2;
	PROCESS *ptemp;

	alloc_vram_perm_bg_list((ADDRESS *)street_perm_list,&street_anims);

	CREATE(PID_BANI,storm_proc);

	CREATE(PID_BANI,wind_sound);

	do
	{
		ptemp=CREATE(PID_BANI,garbage_straight);
		ptemp->pa9=a_big_paper_strt;
		(void *)ptemp->a10=&street_anims;

		ptemp=CREATE(PID_BANI,garbage_straight);
		ptemp->pa9=a_lil_paper_strt;
		(void *)ptemp->a10=&street_anims;

		ptemp=CREATE(PID_BANI,garbage_straight);
		ptemp->pa9=a_leaf_strt;
		(void *)ptemp->a10=&street_anims;
		ptemp=CREATE(PID_BANI,garbage_straight);
		ptemp->pa9=a_leaf_strt;
		(void *)ptemp->a10=&street_anims;
		ptemp=CREATE(PID_BANI,garbage_straight);
		ptemp->pa9=a_leaf_strt;
		(void *)ptemp->a10=&street_anims;

		ptemp=CREATE(PID_BANI,garbage_loop);
		ptemp->pa9=a_big_paper_strt;
		(void *)ptemp->a10=&street_anims;

		ptemp=CREATE(PID_BANI,garbage_loop);
		ptemp->pa9=a_lil_paper_strt;
		(void *)ptemp->a10=&street_anims;
	}
	while(--loop>0);

	create_dumb_animators_w(paper_table_strt,&street_anims);
}

/******************************************************************************
 Function: void storm_proc(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	storm process
******************************************************************************/
void storm_proc(void)
{
	OBJECT *obj;

	gso_dmawnz(obj,(ADDRESS)STORM01,street_anims,0);
	set_xy_coordinates(obj,0x140+SCX(0x70)-8,-SCY(11)+40-0x10 );
	alloc_cache(STORM01,&street_anims,obj);
	insert_object(obj,&baklst6);

	current_proc->pa9=a_storm;
	framew(6);
}

/******************************************************************************
 Function: void fall_on_trax(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	die by train
******************************************************************************/
extern void train_proc(void);
void fall_on_trax(void)
{
	OBJECT *obj=current_proc->pa8;
	void *ta5;

	(short)current_proc->pdata.p_store6=obj->oxpos.u.intpos;
	group_sound(9);						// death scream

	dallprc(PID_SCROLL);
	dallprc(PID_REPELL);
	dallprc(PID_SMOKER);

	obj->oyvel.pos=-SCY(0x100000);			// zoom off screen
	get_char_ani(ANIM_TABLE1,ANIM_KDOWN);
	init_anirate(6);

	/* trax3 */
	do
	{
		process_sleep(1);
		next_anirate();
	}
	while(distance_off_ground(current_proc,obj)<SCY(0x100));

	stop_me(obj);
	clear_shadow_bit(obj);

	create_fx(FX_SUBWAY_ROX);
	process_sleep(0x50);

	current_proc->a10=(ADDRESS)current_proc->pa8;
#if 0
	ta5=ochar_mini_falls[obj->ochar];
	current_proc->pdata.p_store7=(ADDRESS)ta5;
	gso_dmawnz(obj,(ADDRESS)ta5,mini_anims,0);
	alloc_cache(ta5,&mini_anims,obj);
#endif
	pose_a9(2,ANIM_KDOWN);
	obj->oscale.yx=0x01c001c0;
	clear_shadow_bit(obj);
	obj->ochar=((OBJECT*)current_proc->a10)->ochar;
	obj->oid=((OBJECT*)current_proc->a10)->oid;

	if (current_proc->procid==PID_P1)
	{
		delobj(p1_obj);				// pull off objlst
		obj_free=obj_free->olink;			// remove from free list so it can be active again
		p1_obj=obj;
	}
	else
	{
		delobj(p2_obj);				// pull off objlst
		obj_free=obj_free->olink;			// remove from free list so it can be active again
		p2_obj=obj;					// stuff new obj
	}

	player_froze_pal();
	player_normpal();

	insert_object(obj,&baklst6);
	obj->oxpos.u.intpos=(short)current_proc->pdata.p_store6;
	obj->oypos.u.intpos=0;

	obj->oxpos.u.intpos=SCRRGT/2+worldtlx6.u.intpos;

	group_sound(2);

	obj->oyvel.pos=SCY(0x30000);			// fall down
	obj->ograv.pos=SCY(0x6000);

	CREATE(PID_BANI,train_sound_proc);

	/* trax8 */
	do
	{
		process_sleep(1);
	}
	while(obj->oypos.u.intpos<SCY(0x90));			// fall until hit ground

	shake_n_sound();
	stop_me(obj);
	process_sleep(0x18);

#if 0
	ta5=ochar_mini_stunned[obj->ochar];
	animate(SINGLE_ANI,ta5,obj->oflags);
#endif
	obj->oypos.u.intpos=SCY(0xe8)-obj->osize.u.ypos;
	pose_a9(0,ANIM_DIZZY);

	obj->oyvel.pos=-SCY(0x30000);
	do
	{
		process_sleep(1);
	} while ( obj->oypos.u.intpos>87);
	stop_a8(obj);

	get_char_ani(ANIM_TABLE1,ANIM_DIZZY);			// act a bit dizzy (arcade mod)
	init_anirate(5);
	current_proc->a10=0x15;
	do
	{
		next_anirate();
		process_sleep(1);
	} while (--current_proc->a10>0 );


	CREATE(PID_BANI,train_proc);

	current_proc->a10=0x1c;
	do
	{
		next_anirate();
		process_sleep(1);
	} while (--current_proc->a10>0 );

	group_sound(9);
	tsound(0x83);
#if 0
	(ADDRESS)ta5=current_proc->pdata.p_store7;
	animate(SINGLE_ANI,ta5,obj->oflags);
#endif
	pose_a9(2,ANIM_KDOWN);

	obj->oypos.u.intpos-=SCY(0x20);

#if 0
	if ( p1_obj==obj )
		p1_xvel.pos=obj->oxvel.pos=SCX(0x80000);
	else p2_xvel.pos=obj->oxvel.pos=SCX(0x80000);
#endif

	obj->oxvel.pos=SCX(0x80000);
	obj->oyvel.pos=-SCY(0x30000);
	obj->ograv.pos=SCY(0x6000);
	shake_a11(5,5);

	process_sleep(0x30);
	death_blow_complete();
	process_sleep(0x20);
	f_no_bonus=0;								// allow bonus routine to do its thing

	wait_forever();
}

void train_sound_proc(void)
{
	tsound(0x84);
	process_sleep(0x10);

	tsound(0x85);
	process_sleep(0x38);
	tsound(0x85);
	process_sleep(0x38);
	tsound(0x85);
	process_sleep(0x38);
	tsound(0x85);
	process_sleep(0x38);

	tsound(0x86);

	process_suicide();
}
