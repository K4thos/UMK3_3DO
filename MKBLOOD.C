/******************************************************************************
 File: mkblood.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III blood Routines
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
#include "mkos.h"
#include "mkgame.h"
#include "mkpal.h"
#include "mkzap.h"
#include "valloc.h"
#include "mkblood.h"
#include "mkfatal.h"
#include "mksound.h"

extern void *blood_anims[];
extern void *a_face_blood[];
extern void *a_decap_spray[];
extern void *a_bigger[];
extern void *a_drip[];
extern void *a_drip_splat[];
extern void *a_rotate_12[];
extern void *a_rotate_standard[];
extern void *a_rotate_9[];
extern void *a_rotate_7[];
extern void *STAB1[];
extern void *BIGBLD1[];

extern WORD BLOOD_P[];
extern WORD OIL_P[];
extern WORD SHEEBL_P[];

JUMPTBL blood_procs[]=
{
	roundhouse_blood,
	upcutted_blood,
	face_blood_2,
	face_punch_blood,
	combo_blood,
	hat_blood,
	spider_blood,
	lion_blood,
	dino_blood,
	saw_blood,
	pit_spike_blood,
	boomerang_blood,
	spear_blood
};

/******************************************************************************
 Function: void create_blood_proc(WORD boffset)

 By: David Schwartz

 Date: Nov 1994

 Parameters: boffset - blood routine offset

 Returns: None

 Description:	create a blood routine process
******************************************************************************/
void create_blood_proc(WORD boffset)
{
	if (f_no_blood==0 && (sizeof(blood_procs)/sizeof(blood_procs[0]))>boffset) 					// blood turned off?
		CREATE(PID_BLOOD,blood_procs[boffset])->pdata.p_otherguy=current_proc->pdata.p_otherguy;
}

/******************************************************************************
 Function: void boomerang_blood(void)

 By: David Schwartz

 Date: Nov 1995

 Parameters: current_proc->pa8 - dude to bloody

 Returns: None

 Description:	boomerang blood
******************************************************************************/
void boomerang_blood(void)
{
	current_proc->a11=7;
	/* boom3 */
	do
	{
		CREATE(PID_BLOOD,boomb1);
		CREATE(PID_BLOOD,boomerang_drip);
		process_sleep(1);
	}
	while(--current_proc->a11>0);

	process_suicide();
}

void boomb1(void) 
{
	(OBJECT *)current_proc->a11=current_proc->pa8;
	get_jt_blood;
	a11_blood_lineup(SCX(0x0),SCY(0x20));
	(current_proc->pa8)->oyvel.pos=srandarc(SCY(0x10000));
	set_proj_vel(current_proc->pa8,SCX(0x10000)+randu(SCX(0x20000)),-1);

	flip_single(current_proc->pa8);

	insert_object(current_proc->pa8,&objlst);

	current_proc->pa9=a_bigger;
	framew(6);
	blood_death(current_proc->pa8);
	
}

void boomerang_drip(void) 
{
	(OBJECT *)current_proc->a11=current_proc->pa8;
	get_jt_blood;
	a11_blood_lineup(SCX(0x0),SCY(0x50));

	set_proj_vel(current_proc->pa8,SCX(0x20000)+randu(SCX(0x60000)),-1);

	(current_proc->pa8)->oyvel.pos=-(randu(SCY(0x40000))+SCY(0x30000));

	flip_single(current_proc->pa8);
	insert_object(current_proc->pa8,&objlst);

	current_proc->a0=8;
	current_proc->pa9=a_drip;
	(long)current_proc->a10=SCY(0x6000);
	blood_fall_splat_12_die();
}
/******************************************************************************
 Function: void saw_blood(void)

 By: David Schwartz

 Date: May 1995

 Parameters: current_proc->pa8 - dude to bloody

 Returns: None

 Description:	saw blood
******************************************************************************/
void saw_blood(void)
{
	current_proc->a11=3;
	do
	{
		CREATE(PID_BLOOD,sawb3);
		process_sleep(10);
	}
	while(--current_proc->a11>0);

	stack_jump(sawb3);
};

void sawb3(void)
{

	(OBJECT *)current_proc->a11=current_proc->pa8;
	(long)current_proc->a0=SCX(0x20000);		// xvel range
	(long)current_proc->a1=SCX(0x10000);		// xvel min
	(long)current_proc->a2=SCY(0x50000);		// yvel range
	(long)current_proc->a3=SCY(0x30000);		// yvel min
	current_proc->a4=0x00810001;
	current_proc->a5=0x00040000;
	spawn_drip_a10(2);

	CREATE(PID_BLOOD,saw_right);
	get_tg_blood;
	lineup_with_a11_offset(SCX(0x0005),SCY(0x0088));
	flip_single(current_proc->pa8);
	stack_jump(upcb3);
}

void saw_right(void)
{
	get_tg_blood;
	lineup_with_a11_offset(SCX(0x0000),SCY(0x0088));
	stack_jump(upcb3);
}

/******************************************************************************
 Function: void dino_blood(void)

 By: David Schwartz

 Date: May 1995

 Parameters: current_proc->pa8 - dude to bloody

 Returns: None

 Description:	dino blood
******************************************************************************/
void dino_blood(void)
{
	CREATE(PID_BLOOD,dblood3);
	process_sleep(8);
	stack_jump(dblood3);
};

void dblood3(void)
{
	(OBJECT *)current_proc->a11=current_proc->pa8;
	(long)current_proc->a0=SCX(0x20000);		// xvel range
	(long)current_proc->a1=SCX(0x10000);		// xvel min
	(long)current_proc->a2=SCY(0x50000);		// yvel range
	(long)current_proc->a3=SCY(0x30000);		// yvel min
	current_proc->a4=0x00410001;
	current_proc->a5=0x00040000;
	spawn_drip_a10(3);

	CREATE(PID_BLOOD,dino_right);
	get_tg_blood;
	lineup_with_a11_offset(SCX(0x0005),SCY(0x0048));
	flip_single(current_proc->pa8);
	stack_jump(upcb3);
}

void dino_right(void)
{
	get_tg_blood;
	lineup_with_a11_offset(SCX(0x0000),SCY(0x0048));
	stack_jump(upcb3);
}

/******************************************************************************
 Function: void lion_blood(void)

 By: David Schwartz

 Date: May 1995

 Parameters: current_proc->pa8 - dude to bloody

 Returns: None

 Description:	lion blood
******************************************************************************/
void lion_blood(void)
{
	(OBJECT *)current_proc->a11=current_proc->pa8;
	(long)current_proc->a0=SCX(0x20000);		// xvel range
	(long)current_proc->a1=SCX(0x10000);		// xvel min
	(long)current_proc->a2=SCY(0x50000);		// yvel range
	(long)current_proc->a3=SCY(0x30000);		// yvel min
	current_proc->a4=0x00710001;
	current_proc->a5=0x00040000;
	spawn_drip_a10(3);

	CREATE(PID_BLOOD,lion_right);
	get_tg_blood;
	lineup_with_a11_offset(SCX(0x0005),SCY(0x0078));
	flip_single(current_proc->pa8);
	stack_jump(upcb3);
}

void lion_right(void)
{
	get_tg_blood;
	lineup_with_a11_offset(SCX(0x0000),SCY(0x0078));
	stack_jump(upcb3);
}

/******************************************************************************
 Function: void spider_blood(void)

 By: David Schwartz

 Date: May 1995

 Parameters: current_proc->pa8 - dude to bloody

 Returns: None

 Description:	spider blood
******************************************************************************/
void spider_blood(void)
{
	CREATE(PID_FX,spider_sounds);
	current_proc->a11=4;

	/* spid3 */
	do
	{
#if PAL_VERSION==1
		if (current_proc->a11 & 1)
	       create_blood_proc(BLOOD_ROUNDHOUSE);
		create_blood_proc(BLOOD_UPPERCUT);
		create_blood_proc(BLOOD_HAT);
		process_sleep(0x1c);
#else
		if ((current_proc->pdata.p_otherguy)->ochar==FT_SMOKE||(current_proc->pdata.p_otherguy)->ochar==FT_OLDSMOKE)
		{
			if (current_proc->a11 & 1)
	           create_blood_proc(BLOOD_ROUNDHOUSE);
		   create_blood_proc(BLOOD_UPPERCUT);
		   create_blood_proc(BLOOD_HAT);
		   process_sleep(0x1c);
		}
		else
		{
	      create_blood_proc(BLOOD_ROUNDHOUSE);
		   create_blood_proc(BLOOD_UPPERCUT);
		   create_blood_proc(BLOOD_HAT);
		   process_sleep(0x1c);
	  }
#endif
	}
	while(--current_proc->a11>0);

	process_suicide();
}

void spider_sounds(void)
{
	death_scream;
	current_proc->a11=4;

	/* ssnd */
	do
	{
		tsound(0x24);
		process_sleep(0x10);
		tsound(0x25);
	}
	while(--current_proc->a11>0);

	process_suicide();
}

void spear_blood(void) 
{
	current_proc->a11=12;
	do
	{
		CREATE(PID_BLOOD,spearb1);
		CREATE(PID_BLOOD,spear_drip);
		process_sleep(1);
	}
	while(--current_proc->a11>0);

	process_suicide();
}
/******************************************************************************
 Function: void hat_blood(void)

 By: David Schwartz

 Date: May 1995

 Parameters: current_proc->pa8 - dude to bloody

 Returns: None

 Description:	hat blood
******************************************************************************/
void hat_blood(void)
{
	current_proc->a11=2;
	do
	{
		CREATE(PID_BLOOD,hatb1);
		CREATE(PID_BLOOD,hatb_drip);
		process_sleep(1);
	}
	while(--current_proc->a11>0);

	process_suicide();
}

void spearb1(void)
{
	(OBJECT *)current_proc->a11=current_proc->pa8;
	get_jt_blood;
	a11_blood_lineup(SCX(0x10),SCY(0x18));
	stack_jump(hatb2);
}

void hatb1(void)
{
	(OBJECT *)current_proc->a11=current_proc->pa8;
	get_jt_blood;
	a11_blood_lineup(SCX(0x10),SCY(0x38));
	stack_jump(hatb2);
}	

void hatb2(void)
{
	(current_proc->pa8)->oyvel.pos=srandarc(SCY(0x10000));

	set_proj_vel(current_proc->pa8,SCX(0x10000)+randu(SCX(0x20000)),-1);

	flip_single(current_proc->pa8);

	insert_object(current_proc->pa8,&objlst);

	current_proc->pa9=a_bigger;
	framew(6);
	blood_death(current_proc->pa8);
}

void spear_drip(void)
{
	(OBJECT *)current_proc->a11=current_proc->pa8;
	get_jt_blood;
	a11_blood_lineup(SCX(0x10),SCY(0x30));

	set_proj_vel(current_proc->pa8,SCX(0x20000)+randu(SCX(0x60000)),-1);

	stack_jump(drip_entry);
}

void hatb_drip(void)
{
	(OBJECT *)current_proc->a11=current_proc->pa8;
	get_jt_blood;
	a11_blood_lineup(SCX(0x10),SCY(0x60));

	set_proj_vel(current_proc->pa8,SCX(0x20000)+randu(SCX(0x60000)),-1);

	stack_jump(drip_entry);
}

void drip_entry(void)
{
	(current_proc->pa8)->oyvel.pos=-(randu(SCY(0x40000))+SCY(0x30000));

	flip_single(current_proc->pa8);
	insert_object(current_proc->pa8,&objlst);

	current_proc->a0=8;
	current_proc->pa9=a_drip;
	(long)current_proc->a10=SCY(0x6000);
	blood_fall_splat_12_die();
}

/******************************************************************************
 Function: void pit_spike_blood(void)

 By: David Schwartz

 Date: May 1995

 Parameters: current_proc->pa8 - dude to bloody

 Returns: None

 Description:	pit blood
******************************************************************************/
void pit_spike_blood(void)
{
	current_proc->a11=4;
	do
	{
		CREATE(PID_BLOOD,sawb3);
		process_sleep(8);
	}
	while(--current_proc->a11>0);

	process_suicide();
}
/******************************************************************************
 Function: void upcutted_blood(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: current_proc->a11 - dude to bloody

 Returns: None

 Description:	uppercut blood
******************************************************************************/
void upcutted_blood(void)
{
	(ADDRESS)current_proc->a11=current_proc->pa8;
	(long)current_proc->a0=SCX(0x20000);		// xvel range
	(long)current_proc->a1=SCX(0x10000);		// xvel min
	(long)current_proc->a2=SCY(0x50000);		// yvel range
	(long)current_proc->a3=SCY(0x30000);		// yvel min
	current_proc->a4=0x00010001;
	current_proc->a5=0x00040000;
	spawn_drip_a10(3);

	CREATE(PID_BLOOD,upcut_right);
	get_tg_blood;
	lineup_with_a11_offset(SCX(0x0005),SCY(0x0018));
	flip_single(current_proc->pa8);
	stack_jump(upcb3);
}

void upcut_right(void)
{
	get_tg_blood;
	lineup_with_a11_offset(SCX(0x0000),SCY(0x0018));
	stack_jump(upcb3);
}

void upcb3(void)
{
	insert_object(current_proc->pa8,&objlst);
	current_proc->pa9=a_decap_spray;
	framew(3);
	blood_death(current_proc->pa8);
}

/******************************************************************************
 Function: void lineup_with_a11_offset(short pa0x,short pa0y)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa0 x,y : adjustment to pos

 Returns: None

 Description:	uppercut blood
******************************************************************************/
void lineup_with_a11_offset(short pa0x,short pa0y)
{
	lineup_1pwm(current_proc->pa8,(OBJECT *)current_proc->a11);

	/* adjust_blood_a0 */
	multi_adjust_xy(current_proc->pa8,pa0x,pa0y);

	return;
}

/******************************************************************************
 Function: void roundhouse_blood(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: current_proc->a11 - dude to bloody

 Returns: None

 Description:	uppercut blood
******************************************************************************/
void roundhouse_blood(void)
{
	(ADDRESS)current_proc->a11=current_proc->pa8;
	(long)current_proc->a0=SCX(0x20000);		// xvel range
	(long)current_proc->a1=SCX(0x10000);		// xvel min
	(long)current_proc->a2=SCY(0x50000);		// yvel range
	(long)current_proc->a3=SCY(0x30000);		// yvel min

	(long)current_proc->a4=(-SCY(0x0010))<<16;
	(long)current_proc->a4|=SCX(0x000);
	current_proc->a5=0x00040000;
	(long)current_proc->a10=3;					// #of drops /2
	spawn_drip_a10(3);

	CREATE(PID_BLOOD,rhouse_right);
	get_tg_blood;
	lineup_with_a11_offset(-SCX(0x000a),SCY(0x0018));
	flip_single(current_proc->pa8);
	stack_jump(rhb3);
}

void rhouse_right(void)
{
	get_tg_blood;
	lineup_with_a11_offset(-SCX(0x0010),SCY(0x0018));
	stack_jump(rhb3);
}

void rhb3(void)
{
	insert_object(current_proc->pa8,&objlst);
	current_proc->pa9=a_decap_spray;
	framew(3);
	blood_death(current_proc->pa8);
}

/******************************************************************************
 Function: void combo_blood(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: current_proc->a11 - dude to bloody

 Returns: None

 Description:	combo blood
******************************************************************************/
void combo_blood(void)
{
	(ADDRESS)current_proc->a11=current_proc->pa8;

	current_proc->a6=2;
	/* comb3 */
	do
	{
		CREATE(PID_BLOOD,face_blood);
	}
	while(--current_proc->a6>0);

	(long)current_proc->a0=SCX(0x20000);		// xvel range
	(long)current_proc->a1=SCX(0x30000);		// xvel min
	(long)current_proc->a2=SCY(0x40000);		// yvel range
	(long)current_proc->a3=SCY(0x30000);		// yvel min
	current_proc->a4=0x00010001;
	current_proc->a5=0x00060001;
	spawn_drip_neg;
	spawn_drip_neg;
	process_suicide();
}

/******************************************************************************
 Function: void face_blood_2(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: current_proc->a11 - dude to bloody

 Returns: None

 Description:	hit in the face hard blood
******************************************************************************/
void face_blood_2(void)
{
	(ADDRESS)current_proc->a11=current_proc->pa8;

	current_proc->a6=4;
	/* comb3 */
	do
	{
		CREATE(PID_BLOOD,face_blood);
	}
	while(--current_proc->a6>0);

	(long)current_proc->a0=SCX(0x20000);		// xvel range
	(long)current_proc->a1=SCX(0x30000);		// xvel min
	(long)current_proc->a2=SCY(0x40000);		// yvel range
	(long)current_proc->a3=SCY(0x30000);		// yvel min
	current_proc->a4=0x00010001;
	current_proc->a5=0x00060001;
	spawn_drip_neg;
	spawn_drip_neg;
	spawn_drip_neg;
	spawn_drip_neg;
	process_suicide();
}

/******************************************************************************
 Function: void face_blood(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: current_proc->a11 - dude to bloody

 Returns: None

 Description:	hit in the face hard blood
******************************************************************************/
void face_blood(void)
{
	get_jt_blood;

	lineup_with_a11_offset(SCX(0x0000),-SCY(0x0010));
	insert_object(current_proc->pa8,&objlst);

	(current_proc->pa8)->oyvel.pos=-randu(SCY(0x12000));

	set_proj_vel(current_proc->pa8,-(SCX(0x30000)+randu(SCX(0x20000))),-1);

	current_proc->pa9=a_bigger;
	framew(6);
	blood_death(current_proc->pa8);
}

/******************************************************************************
 Function: void face_punch_blood(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: current_proc->a11 - dude to bloody

 Returns: None

 Description:	hit in the face blood
******************************************************************************/
void face_punch_blood(void)
{
	current_proc->a11=(ADDRESS)current_proc->pa8;

	if (random()>0)
	{
		(long)current_proc->a0=1;						// xvel range
		(long)current_proc->a1=SCX(0x30000);		// xvel min
		(long)current_proc->a2=SCY(0x20000);		// yvel range
		(long)current_proc->a3=SCY(0x20000);		// yvel min
		current_proc->a4=SCY(0x0010)<<16|(-SCX(0x10));
		current_proc->a5=0x00040002;
		spawn_drip_neg;
	}

	/* face3 */
	get_tg_blood;
	lineup_1pwm(current_proc->pa8,(OBJECT *)current_proc->a11);
	flip_single(current_proc->pa8);

	current_proc->a0=((OBJECT*)current_proc->a11)->ochar;
	if (current_proc->a0==FT_JAX)
		multi_adjust_xy(current_proc->pa8,SCX(0xc),SCY(0x15));
	else multi_adjust_xy(current_proc->pa8,SCX(0x8),SCY(0x10));

	(current_proc->pa8)->oyvel.pos=-SCY(0x8000);

	set_proj_vel(current_proc->pa8,SCX(0x10000),-1);
	insert_object(current_proc->pa8,&objlst);

	current_proc->pa9=a_face_blood;
	framew(3);
	blood_death(current_proc->pa8);
}

/******************************************************************************
 Function: void spawn_drip_a10(WORD pa10)

 By: David Schwartz

 Date: Mar 1995

 Parameters: current_proc->a0 - xvel range
			 current_proc->a1 - xvel min
			 current_proc->a2 - yvel range
			 current_proc->a3 - yvel min
			 current_proc->a4 - y:x coord adjust
			 current_proc->a5 - anim speed:blood offset
			 pa10 - # of times to initiate blood
 Returns: None

 Description:	setup blood objects and process
******************************************************************************/
void spawn_drip_a10(WORD pa10)
{
	do
	{
		spawn_drip;
	  	spawn_drip_neg;
	}
	while(--pa10>0);
}

void spawnd3(void)
{
	PROCESS *ptemp;

	ptemp=CREATE(PID_BLOOD,(FUNC)current_proc->a7);
	ptemp->pdata.p_store1=current_proc->a0;
	ptemp->pdata.p_store2=current_proc->a1;
	ptemp->pdata.p_store3=current_proc->a2;
	ptemp->pdata.p_store4=current_proc->a3;
	ptemp->pdata.p_store5=current_proc->a4;
	ptemp->pdata.p_store6=current_proc->a5;

	return;
}

/******************************************************************************
 Function: void tobias_drip_neg(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	Drip some blood
******************************************************************************/
void *drip_ani_table[]=
{
	a_rotate_12,
	a_rotate_standard,
	a_rotate_9,
	a_rotate_7
};

void tobias_drip_neg(void)
{
	tdrip_getobj();
	set_proj_vel(current_proc->pa8,-((long)current_proc->a0),current_proc->a1);
	stack_jump(tdrip2);
}

void tobias_drip(void)
{
	tdrip_getobj();
	set_proj_vel(current_proc->pa8,((long)current_proc->a0),current_proc->a1);
	stack_jump(tdrip2);
}

void tdrip2(void)
{
	insert_object(current_proc->pa8,&objlst);

	current_proc->a0=(current_proc->pdata.p_store6>>16) & 0xffff;
	current_proc->pa9=drip_ani_table[current_proc->pdata.p_store6 & 0xffff];
	current_proc->a10=SCY(0x4000);
	drip_fall_2();
}

void tdrip_getobj(void)
{
	get_tg_blood;

	lineup_with_a11_offset(current_proc->pdata.p_store5 & 0xffff,(current_proc->pdata.p_store5)>>16);

	(current_proc->pa8)->oyvel.pos=-(current_proc->pdata.p_store4+randu(current_proc->pdata.p_store3));

	(long)current_proc->a0=current_proc->pdata.p_store2+randu(current_proc->pdata.p_store1);

	return;
}

/******************************************************************************
 Function: void blood_fall(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: current_proc->a0 - ani speed
			 current_proc->a9 - ani
			 current_proc->a10 - gravity

 Returns: None

 Description:	blood be falling on my head
******************************************************************************/
void blood_fall(void)
{
	OBJECT *obj=current_proc->pa8;

	init_anirate(current_proc->a0);

	/* bldf3 */
	do
	{
		do
		{
			obj->oyvel.pos+=(long)current_proc->a10;
			next_anirate();
			process_sleep(1);
		}
		while(obj->oyvel.pos<0);		// heading up = loop
	}
	while(obj->oypos.u.intpos<(ground_y-SCY(0x14)));

	stop_a8(obj);

	return;
}

/******************************************************************************
 Function: void gbld3(void *pa5)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa5 - frame to init

 Returns: None

 Description:	setup a piece of blood
******************************************************************************/
WORD *ochar_blood_palettes[]=
{
	BLOOD_P,
	BLOOD_P,
	BLOOD_P,
	BLOOD_P,
	BLOOD_P,

	BLOOD_P,
	BLOOD_P,
	OIL_P,
	OIL_P,
	BLOOD_P,

	BLOOD_P,
	SHEEBL_P,
	BLOOD_P,
	BLOOD_P,
	OIL_P,

	BLOOD_P,
	BLOOD_P,
	BLOOD_P,
	BLOOD_P,
	SHEEBL_P,

	BLOOD_P,
	BLOOD_P,
	BLOOD_P,
	BLOOD_P,
	BLOOD_P,

	SHEEBL_P,
	BLOOD_P
};

void gbld3(void *pa5)
{
	OBJECT *obj;
	OBJECT *ta3;

	ta3=current_proc->pa8;				// save player object we are following

	gso_dmawnz(obj,(ADDRESS)pa5,blood_anims,0);
	alloc_cache(pa5,&blood_anims,obj);

	if (ta3->ochar<=FT_SK)
		(WORD *)current_proc->a0=ochar_blood_palettes[ta3->ochar];
	else (WORD *)current_proc->a0=ochar_blood_palettes[0];

	if ((WORD *)current_proc->a0!=BLOOD_P)
		swpal((void *)current_proc->a0,obj);			// not normal blood switch

	/* gbld5 */
	obj->ozval=FRONT_Z+1;

	obj->oflags=(obj->oflags & (~M_FLIPH)) | (((OBJECT *)current_proc->a11)->oflags & M_FLIPH);
	return;
}

/******************************************************************************
 Function: void drip_fall(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: current_proc->a0 = ani speed
			 current_proc->a9 = animation
			 current_proc->a10 = gravity

 Returns: None

 Description:	blood fall
******************************************************************************/
void drip_fall(void)
{
	current_proc->a0=6;
	current_proc->pa9=a_drip;
	(long)current_proc->a10=SCY(0x6000);

	stack_jump(drip_fall_2);
}

void drip_fall_2(void)
{
	blood_fall();
	(current_proc->pa8)->ozval=BACK_Z-1;

	current_proc->pa9=a_drip;
	find_part2();
	framew(6);

	if (current_proc->a11==0)
		process_suicide();

	/* blood_12_death */
	process_sleep(12);

	blood_death(current_proc->pa8);
}

/******************************************************************************
 Function: void a11_blood_lineup(short ta0,short ta1)

 By: David Schwartz

 Date: May 1995

 Parameters: current_proc->a11 = ani dude to lineup blood with
			 ta0 - x adj
			 ta1 - y adj

 Returns: None

 Description:	lineup blood with a11 dude then adjust
******************************************************************************/
void a11_blood_lineup(short ta0,short ta1)
{
	(current_proc->pa8)->oxpos.u.intpos=((OBJECT *)current_proc->a11)->oxpos.u.intpos;
	(current_proc->pa8)->oypos.u.intpos=((OBJECT *)current_proc->a11)->oypos.u.intpos;
	multi_adjust_xy(current_proc->pa8,ta0,ta1);
	return;
}

/******************************************************************************
 Function: void blood_fall_splat_12_die(void)

 By: David Schwartz

 Date: May 1995

 Parameters: current_proc->a0 = anirate

 Returns: None

 Description:	blood fall to ground splat make noise and die
******************************************************************************/
void blood_fall_splat_12_die(void)
{
	blood_fall();
	current_proc->pa9=a_drip;
	find_part2();

	rsnd_splish();

	framew(6);

	/* blood_12_death */
	process_sleep(12);

	blood_death(current_proc->pa8);
}

