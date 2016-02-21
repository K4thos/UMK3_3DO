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
#include "mkos.h"
#include "mkgame.h"
#include "mkobj.h"
#include "mkutil.h"
#include "mkani.h"
#include "mkjoy.h"
#include "mkguys.h"
#include "mktext.h"
#include "mkinit.h"
#include "mkpal.h"
#include "mkmain.h"
#include "mksound.h"
#include "mkladder.h"
#include "mkvs.h"
#include "mksel.h"
#include "valloc.h"
#include "mkfade.h"

/* local externs */
extern void *ladder_anims[];
extern void *a_badge_spin[];
extern void *MKBRICK[];
extern void *MKBRICK2[];
extern void *UPEVILA[];
extern void *UPEVILB[];

extern void *HDSHANG[];
extern void *HDSUBZERO[];
extern void *HDSWAT[];
extern void *HDBRIDE[];
extern void *HDJAX[];
extern void *HDLAO[];
extern void *HDSONYA[];
extern void *HDKANO[];
extern void *HDMUSTARD[];
extern void *HDLIUKANG[];
extern void *HDSHEGORO[];
extern void *HDINDIAN[];
extern void *HDKETCHUP[];
extern void *HDTUSKAN[];
extern void *HDSMOKE[];
extern void *HDSHAO[];
extern void *HDMOTARO[];
extern void *MKCOIN_01[];
extern void *MKCOIN_02[];
extern void *MKCOIN_03[];
extern void *MKCOIN_04[];
extern void *MKCOIN_05[];
extern void *MKCOIN_06[];
extern void *MKCOIN_07[];
extern void *MKCOIN_08[];
extern void *MKCOIN_09[];

extern void *TXT_NOVICE[];
extern void *TXT_MASTER[];
extern void *TXT_WARRIOR[];

/*******************************************************************************
 MAP ROM TABLES
*******************************************************************************/
WORD map_1[]=
{
	FT_JAX,
	FT_TUSK,
	FT_INDIAN,
	FT_LAO,
	FT_KANO,
	FT_SONYA,
	FT_MOTARO,
	FT_SK,
	SP_END,

	FT_SG,
	FT_INDIAN,
	FT_SUBZERO,
	FT_LK,
	FT_TUSK,
	FT_ST,
	FT_KANO,
	FT_SONYA,
	FT_MOTARO,
	FT_SK,
	SP_END,

	FT_ST,
	FT_LK,
	FT_TUSK,
	FT_ROBO2,
	FT_LIA,
	FT_KANO,
	FT_LAO,
	FT_ROBO1,
	FT_SUBZERO,
	FT_SG,
	FT_MOTARO,
	FT_SK,
	SP_END
};

WORD map_2[]=
{
	FT_SONYA,
	FT_JAX,
	FT_INDIAN,
	FT_TUSK,
	FT_LAO,
	FT_KANO,
	FT_MOTARO,
	FT_SK,
	SP_END,

	FT_INDIAN,
	FT_SG,
	FT_SUBZERO,
	FT_KANO,
	FT_TUSK,
	FT_LK,
	FT_ST,
	FT_SONYA,
	FT_MOTARO,
	FT_SK,
	SP_END,

	FT_ST,
	FT_SG,
	FT_LK,
	FT_TUSK,
	FT_ROBO2,
	FT_LIA,
	FT_LAO,
	FT_ROBO1,
	FT_KANO,
	FT_SUBZERO,
	FT_MOTARO,
	FT_SK,
	SP_END
};

WORD map_3[]=
{
	FT_INDIAN,
	FT_TUSK,
	FT_SONYA,
	FT_JAX,
	FT_KANO,
	FT_SUBZERO,
	FT_MOTARO,
	FT_SK,
	SP_END,

	FT_SWAT,
	FT_SUBZERO,
	FT_INDIAN,
	FT_SG,
	FT_ST,
	FT_LK,
	FT_JAX,
	FT_SONYA,
	FT_MOTARO,
	FT_SK,
	SP_END,

	FT_ST,
	FT_LK,
	FT_SG,
	FT_TUSK,
	FT_LAO,
	FT_ROBO2,
	FT_LIA,
	FT_ROBO1,
	FT_SUBZERO,
	FT_KANO,
	FT_MOTARO	,
	FT_SK,
	SP_END
};

WORD map_4[]=
{
	FT_KANO,
	FT_SONYA,
	FT_JAX,
	FT_SG,
	FT_SWAT,
	FT_SUBZERO,
	FT_MOTARO,
	FT_SK,
	SP_END,

	FT_LAO,
	FT_KANO,
	FT_JAX,
	FT_ROBO2,
	FT_ROBO1,
	FT_SG,
	FT_LIA,
	FT_SONYA,
	FT_MOTARO,
	FT_SK,
	SP_END,

	FT_SONYA,
	FT_SG,
	FT_LIA,
	FT_INDIAN,
	FT_SUBZERO,
	FT_SWAT,
	FT_JAX,
	FT_LK,
	FT_TUSK,
	FT_ST,
	FT_MOTARO	,
	FT_SK,
	SP_END
};

WORD map_5[]=
{
	FT_LK,
	FT_ST,
	FT_SG,
	FT_TUSK,
	FT_LAO,
	FT_ROBO2,
	FT_MOTARO,
	FT_SK,
	SP_END,

	FT_LIA,
	FT_ROBO2,
	FT_LAO,
	FT_TUSK,
	FT_SG,
	FT_ST,
	FT_LK,
	FT_SONYA,
	FT_MOTARO,
	FT_SK,
	SP_END,

	FT_SG,
	FT_LK,
	FT_ST,
	FT_LAO,
	FT_SONYA,
	FT_SWAT,
	FT_ROBO1,
	FT_ST,
	FT_TUSK,
	FT_LIA,
	FT_MOTARO	,
	FT_SK,
	SP_END
};

WORD map_6[]=
{
	FT_LIA,
	FT_ROBO2,
	FT_JAX,
	FT_INDIAN,
	FT_SONYA,
	FT_KANO,
	FT_MOTARO,
	FT_SK,
	SP_END,

	FT_SONYA,
	FT_LAO,
	FT_TUSK,
	FT_ROBO2,
	FT_JAX,
	FT_SG,
	FT_ST,
	FT_KANO,
	FT_MOTARO,
	FT_SK,
	SP_END,

	FT_KANO,
	FT_SONYA,
	FT_SUBZERO,
	FT_INDIAN,
	FT_JAX,
	FT_SWAT,
	FT_ROBO1,
	FT_SG,
	FT_TUSK,
	FT_LIA,
	FT_MOTARO,
	FT_SK,
	SP_END
};


WORD *rom_maps_tbl[]=
{
	map_1,
	map_2,
	map_3,
	map_4,
	map_5,
	map_6
};

WORD e_kitana_jade[]=
{
	FT_KITANA,
	FT_JADE,
	0xffff
};

WORD e_mileena_jade[]=
{
 	FT_MILEENA,
 	FT_JADE,
 	0xffff
};

WORD e_ermac_jade[]=
{
	FT_ERMAC,
	FT_JADE,
	0xffff
};

WORD e_noob_ermac[]=
{
	FT_NOOB,
	FT_ERMAC,
	0xffff
};

WORD e_osz_osm[]=
{
	FT_OLDSZ,
	FT_OLDSMOKE,
	0xffff
};

WORD e_mega_endurance[]=
{
  	FT_NOOB,
	FT_OLDSZ,
	FT_OLDSMOKE,
	FT_ERMAC,
	FT_MILEENA,
	0xffff
};

WORD e_scorpion_jade_kit[]=
{
	FT_SCORPION,
	FT_JADE,
	FT_KITANA,
	0xffff
};

WORD e_reptile_indian_lia[]=
{
	FT_REPTILE,
	FT_INDIAN,
	FT_LIA,
	0xffff
};	

WORD e_scorp_swat_st[]=
{
	FT_SCORPION,
	FT_SWAT,
	FT_ST,
	0xffff
};	

WORD e_ermac_mileena_osz[]=
{
	FT_ERMAC,
	FT_MILEENA,
	FT_OLDSZ,
	0xffff
};	

WORD e_jade_reptile[]=
{
	FT_JADE,
	FT_REPTILE,
	0xffff
};

WORD e_kitana_scorpion[]=
{
	FT_KITANA,
	FT_SCORPION,
	0xffff
};

WORD e_kano_sonya[]=
{
	FT_KANO,
	FT_SONYA,
	0xffff
};

WORD e_jax_ind[]=
{
	FT_JAX,
	FT_INDIAN,
	0xffff
};

WORD e_sz_swat[]=
{
	FT_SUBZERO,
	FT_SWAT,
	0xffff
};

WORD e_lia_robo1[]=
{
	FT_LIA,
	FT_ROBO1,
	0xffff
};

WORD e_robo2_lao[]=
{
  	FT_ROBO2,
	FT_LAO,
	0xffff
};

WORD e_sg_st[]=
{
	FT_SG,
	FT_ST,
	0xffff
};

WORD e_lk_smoke[]=
{
	FT_LK,
	FT_SMOKE,
	0xffff
};

WORD e_ermac_mileena[]=
{
	FT_ERMAC,
	FT_MILEENA,
	0xffff
};

WORD e_osz_sz[]=
{
	FT_OLDSZ,
	FT_SUBZERO,
	0xffff
};

WORD e_kano_kano[]=
{
	FT_KANO,
	FT_KANO,
	0xffff
};

/* mhe tables */
OBJECT *mhe_list[]=
{
	(OBJECT *)&baklst2,
	(OBJECT *)&baklst6,
	(OBJECT *)&baklst4
};


short mhe_close_ladder_y[]=
{
   -SCY(0x200)-SCY(96)*2,
   -SCY(0x200)-SCY(96)*4,
   -SCY(0x200)
};

long mhe_x_vels[]=
{
	SCX(0x20000),
	SCX(0),
	-SCX(0x20000)
};

long mhe_y_vels[]=
{
		-SCY(0x10000),
		SCY(0),
		-SCY(0x20000),
};

WORD *endurance_tables[]=
{
	e_noob_ermac,		// >80 = noob & ermac
	e_osz_osm,		// >81 = old sz & old smoke
	e_mega_endurance,	// >82 = old sz & old smoke

	e_jade_reptile,		// >83 =
	e_kitana_scorpion,	// >84 =
	e_kano_sonya,		// >85 =
	e_jax_ind,		// >86 =
	e_sz_swat,		// >87 =
	e_lia_robo1,		// >88 =
	e_robo2_lao,		// >89 =
	e_sg_st,			// >8a =
	e_lk_smoke,		// >8b =
	e_kitana_jade,		// >8c =

	e_ermac_jade,		// >8d = secret characters
	e_osz_sz,		// >8e = secret characters
	e_ermac_mileena,		// >8f = secret characters

	e_scorpion_jade_kit,	// >90
	e_reptile_indian_lia,	// >91
	e_scorp_swat_st,		// >92
	e_ermac_mileena_osz,	// >93
};

MHE_ORD mhe_table[]=
{
	/* medium_1st entry 0 */
	{(OBJECT *)&baklst3,0,med_to_hard},	// med
	{(OBJECT *)&baklst6,-2,hard_to_easy},	// hard
	{(OBJECT *)&baklst5,2,NULL},			// easy

	/* hard_1st entry 3 */
	{(OBJECT *)&baklst6,0,hard_to_med},		// hard
	{(OBJECT *)&baklst3,2,hard_to_easy},	// med
	{(OBJECT *)&baklst5,4,NULL},			// easy

	/* easy_1st entry 6 */
	{(OBJECT *)&baklst5,0,easy_to_hard},	// easy
	{(OBJECT *)&baklst6,-4,hard_to_med},	// hard
	{(OBJECT *)&baklst3,-2,NULL}			// med
};

MHE_ORD *mhe_ladder_order[]=
{
	medium_1st,
	hard_1st,
	easy_1st
};

#if OLD_LADDER
ADDRESS *ladder_perm_vram_list[]=
{
	(ADDRESS *)MKBRICK,
	(ADDRESS *)MKBRICK2,
	(ADDRESS *)UPEVILA,
	(ADDRESS *)UPEVILB,
	(ADDRESS *)LHDSHANG,
	(ADDRESS *)LHDSUBZERO,
	(ADDRESS *)LHDSWAT,
	(ADDRESS *)LHDBRIDE,
	(ADDRESS *)LHDJAX,
	(ADDRESS *)LHDLAO,
	(ADDRESS *)LHDSONYA,
	(ADDRESS *)LHDKANO,
	(ADDRESS *)LHDMUSTARD,
	(ADDRESS *)LHDLIUKANG,
	(ADDRESS *)LHDSHEGORO,
	(ADDRESS *)LHDINDIAN,
	(ADDRESS *)LHDKETCHUP,
	(ADDRESS *)LHDTUSKAN,
	(ADDRESS *)LHDSMOKE,
	(ADDRESS *)LHDSHAO,
	(ADDRESS *)LHDMOTARO,
	(ADDRESS *)LMKCOIN_01,
	(ADDRESS *)LMKCOIN_02,
	(ADDRESS *)LMKCOIN_03,
	(ADDRESS *)LMKCOIN_04,
	(ADDRESS *)LMKCOIN_05,
	(ADDRESS *)LMKCOIN_06,
	(ADDRESS *)LMKCOIN_07,
	(ADDRESS *)LMKCOIN_08,
	(ADDRESS *)LMKCOIN_09,
	(ADDRESS *)TXT_NOVICE,
	(ADDRESS *)TXT_MASTER,
	(ADDRESS *)TXT_WARRIOR,
	NULL
};
#endif

ADDRESS *ladder_perm_vram_list[]=
{
	(ADDRESS *)MKBRICK,
	(ADDRESS *)MKBRICK2,
	(ADDRESS *)UPEVILA,
	(ADDRESS *)UPEVILB,
	(ADDRESS *)HDSHANG,
	(ADDRESS *)HDSUBZERO,
	(ADDRESS *)HDSWAT,
	(ADDRESS *)HDBRIDE,
	(ADDRESS *)HDJAX,
	(ADDRESS *)HDLAO,
	(ADDRESS *)HDSONYA,
	(ADDRESS *)HDKANO,
	(ADDRESS *)HDMUSTARD,
	(ADDRESS *)HDLIUKANG,
	(ADDRESS *)HDSHEGORO,
	(ADDRESS *)HDINDIAN,
	(ADDRESS *)HDKETCHUP,
	(ADDRESS *)HDTUSKAN,
	(ADDRESS *)HDSMOKE,
	(ADDRESS *)HDSHAO,
	(ADDRESS *)HDMOTARO,
	(ADDRESS *)MKCOIN_01,
	(ADDRESS *)MKCOIN_02,
	(ADDRESS *)MKCOIN_03,
	(ADDRESS *)MKCOIN_04,
	(ADDRESS *)MKCOIN_05,
	(ADDRESS *)MKCOIN_06,
	(ADDRESS *)MKCOIN_07,
	(ADDRESS *)MKCOIN_08,
	(ADDRESS *)MKCOIN_09,
	(ADDRESS *)TXT_NOVICE,
	(ADDRESS *)TXT_MASTER,
	(ADDRESS *)TXT_WARRIOR,
	NULL
};

/******************************************************************************
 Function: void mk_ladder(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	The cool ladder screen
******************************************************************************/
extern BYTE sel_ladder_heap[] __attribute__ ((aligned(4)));
void mk_ladder(void)
{
	murder_myoinit_score();
	f_doscore=0;

	do_background(BKGD_LADDER_MOD);
	alloc_vram_perm_bg_list((ADDRESS *)ladder_perm_vram_list,&ladder_anims);		// load in perms frames

	/* load sound stuff */
	sound_module_load(SMOD_LADDER);

	/* set decomp base  */
	idcomp_base=(LONG *)sel_ladder_heap;						/* pt to player 2 area */

	if ( map_start==NULL )
		play_generic_tune(TRK_LADDER_STRT);				// only play when selecting new ladder

	f_load=0;

	f_novel=1;

	if (map_start!=NULL)			// have we set out map yet??,
	{
		setup_close_ladder();
	}
	else
	{
		/* lad2 */
		ladder_base=rom_maps_tbl[randu(6)-1];

		setup_distant_ladder();
		ladder_switch_scan();
		map_position=perform=0;
		init_map_start();

		spin_the_mk_badge();
		pan_to_pick_tower();
		zoom_in();
		pan_down();
		set_match_1_diff();			// set diff
	}

	/* lad1p */
	send_code_a3(TS_ONEPLAY_END);

	//process_sleep(0x26);

	/* CD IMPROVEMENT
		1) PRELOAD PLAYER 1 DATA INTO HEAP
		2) PRELOAD BKGD TEXTURE DATA INTO HEAP2
	*/
	set_drone_ochar();
	cd_preload_fade();
	cd_preload_p1();
	cd_preload_bkgd();


	/* set decomp base to decomp area */
	idcomp_base=image_dcomp_buffer;

#if VS_OFF
	/* arcade display flow */
	murder_myoinit_score();
#else
	/* keep screen up with loading message */
	send_code_a3(0);
	file_load_message(4);
	process_sleep(2);
	freeze_2_pages();
	murder_myoinit_score();
#endif

	return;
}

/******************************************************************************
 Function: void setup_close_ladder(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
void setup_close_ladder(void)
{
	long ta1;
	OBJECT *oa1;
	OBJECT *obj;

	switch (current_proc->a11=ladder_order)
	{
		case 0:
			build_left_tower();
			break;
		case 1:
			build_middle_tower();
			break;
		case 2:
			build_right_tower();
			break;
	}

	ladder_scale.yx=0x01000100;			// full scale

	if ((ta1=map_position-1)<0)
		ta1=0;							// position 1&0 --> bottom

	/* scl4 */
	ta1*=SCY(96);					// blocks are SCY(96) pixels tall

	ladder_y.u.intpos=mhe_close_ladder_y[current_proc->a11]+ta1+32;		// lineup at the point we are at !!
	ladder_x.pos=(SCRRGT/2)<<16;
	call_every_tick=ladder_irq;				// ladder IRQ ---> go !!

	oa1=get_current_1st_obj();
	/* scl5 */
	do
	{
		oa1->oyvel.pos=0;				// stop all other vels
		obj=(OBJECT *)oa1->oslink;					// ptr to assoc mug
		obj->oyvel.pos=0;				// clear mugshot velocity also
		oa1=oa1->olink;
	}
	while(oa1!=NULL);
	f_novel=0;							// and allow normal vel

	gso_dmawnz_ns_coor(obj,(current_proc->a5=get_player_ochar_a5()),ladder_anims,0,SCX(0x88),182);
	alloc_cache((void *)current_proc->a5,&ladder_anims,obj);
	insert_object(obj,&objlst2);
	(OBJECT *)current_proc->pa9=obj;
	obj=solid_backdrop_box();
	insert_object(obj,&objlst2);
	process_sleep(0x12);

	if (map_position!=0)				// dont move on first pos
	{
		(short)current_proc->a10=obj->oypos.u.intpos;
		obj->oyvel.pos=-SCY(0x30000);
		((OBJECT *)current_proc->pa9)->oyvel.pos=-SCY(0x30000);

		/* scl7 */
		do
		{
			process_sleep(1);
		}
		while(abs(obj->oypos.u.intpos-(short)current_proc->a10)<SCY(96));

		obj->oyvel.pos=0;
		((OBJECT *)current_proc->pa9)->oyvel.pos=0;
	}

	/* scl8 */
	process_sleep(0x20);
	return;
}

/******************************************************************************
 Function: void setup_distant_ladder(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
char txt_ladder[]="CHOOSE YOUR DESTINY";
void setup_distant_ladder(void)
{
	OBJECT *obj;
	OBJECT *oa9;

	tsound(0x39);					// shao: choose your destiny

	pds_centered(txt_ladder,SCX(0xc8),SCY(0x8)+16);
	ladder_order=1;

	ladder_scale.yx=0x04000400;
	ladder_y.pos=SCY(0x140000)+(0x180000);

	ladder_x.pos=(SCRRGT/2)<<16;
	ladder_x_vel.pos=0;
	ladder_left_x=0;
	ladder_right_x=0;

	build_middle_tower();
	build_left_tower();
	build_right_tower();

	call_every_tick=ladder_irq;

#if OLD_LADDER
	gso_dmawnz_ns_coor(obj,(ADDRESS)LMKCOIN_01,ladder_anims,0,SCX(0xb3+0x68)+1,SCY(0xd0)+16);	// start at novice
	alloc_cache(LMKCOIN_01,&ladder_anims,obj);
#endif
	gso_dmawnz_ns_coor(obj,(ADDRESS)MKCOIN_01,ladder_anims,0,SCX(0xb3+0x68)+1,SCY(0xd0)+16);	// start at novice
	alloc_cache(MKCOIN_01,&ladder_anims,obj);
	insert_object(obj,&objlst2);
	oa9=(OBJECT *)current_proc->pa9=current_proc->pa8;

	gso_dmawnz_ns(obj,(ADDRESS)TXT_NOVICE,ladder_anims,0);
	alloc_cache(TXT_NOVICE,&ladder_anims,obj);
	obj->oypos.u.intpos=oa9->oypos.u.intpos+SCY(0x10)+2;
	obj->ozval=oa9->ozval+1;

	obj->oxpos.u.intpos=(oa9->oxpos.u.intpos+(oa9->osize.u.xpos)/2)-(obj->osize.u.xpos)/2;		// center spot
	insert_object(obj,&objlst2);
	current_proc->pa8=oa9;
	current_proc->pa9=(void*)obj;
#if OLD_LADDER
	CREATE(PID_BANI,ladder_badge_spinner);		// a8=text/a9=coin
#endif
	CREATE(PID_BANI,badge_spinner);		// a8=text/a9=coin

	return;
}

/******************************************************************************
 Function: void build_middle_tower(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
void build_middle_tower(void)
{
	WORD *ta7;

	ta7=ladder_base+LAST_HARD_ENTRY;

	current_proc->a10=ENTRIES_HARD-1;			// # of entries
	get_a_brick(*ta7,&baklst1);
	ta7--;
	insert_object(current_proc->pa8,&baklst6);

	/* lad3 */
	do
	{
		(OBJECT *)current_proc->pa9=current_proc->pa8;
		get_a_brick(*ta7,&baklst1);
		ta7--;

		(current_proc->pa8)->olink=NULL;
		((OBJECT *)current_proc->pa9)->olink=current_proc->pa8;
	}
	while(--current_proc->a10>0);

	return;
}

/******************************************************************************
 Function: void build_left_tower(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
void build_left_tower(void)
{
	WORD *ta7;

	ta7=ladder_base+LAST_MED_ENTRY;
	current_proc->a10=ENTRIES_MED-1;			// # of entries

	get_a_brick(*ta7,&baklst2);
	ta7--;
	insert_object(current_proc->pa8,&baklst3);

	/* lad5 */
	do
	{
		(OBJECT *)current_proc->pa9=current_proc->pa8;
		get_a_brick(*ta7,&baklst2);
		ta7--;

		(current_proc->pa8)->olink=NULL;
		((OBJECT *)current_proc->pa9)->olink=current_proc->pa8;
	}
	while(--current_proc->a10>0);

	return;
}

/******************************************************************************
 Function: void build_right_tower(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:
******************************************************************************/
void build_right_tower(void)
{
	WORD *ta7;

	ta7=ladder_base+LAST_EASY_ENTRY;
	current_proc->a10=ENTRIES_EASY-1;			// # of entries

	get_a_brick(*ta7,&baklst4);
	ta7--;
	insert_object(current_proc->pa8,&baklst5);

	/* lad7 */
	do
	{
		(OBJECT *)current_proc->pa9=current_proc->pa8;
		get_a_brick(*ta7,&baklst4);
		ta7--;

		(current_proc->pa8)->olink=NULL;
		((OBJECT *)current_proc->pa9)->olink=current_proc->pa8;
	}
	while(--current_proc->a10>0);

	return;
}

/******************************************************************************
 Function: void ladder_switch_scan(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	scan switches for ladder
******************************************************************************/
void ladder_switch_scan(void)
{
	WORD *pa3;

	current_proc->a11=2;				// start pos = center
	current_proc->a10=0;				// debounce

	/* lad91 */
	if (p1_state==PS_ACTIVE)
	{
		current_proc->pdata.p_store1=M_P1ACTION;
		pa3=&swcurr.u.p1pad;
	}
	else
	{
		current_proc->pdata.p_store1=M_P2ACTION;
		pa3=&swcurr.u.p2pad;
	}

	/* lad92 */
	current_proc->a0=0x40*5;
	do
	{
		current_proc->pdata.p_store3=current_proc->a0;
		process_sleep(1);

		if (current_proc->a10!=0)
		{
			--current_proc->a10;
			if (current_proc->a10>0)
				goto LAD97;
		}

		/* lad93 */
		if ((*pa3) & current_proc->pdata.p_store1)
			return;								// action button exit

		if ((*pa3) & M_PJOY)
		{
			/* stick left */
			current_proc->a10=10;					// debounce speed
			if ((*pa3) & MASK_JLEFT)
			{
				if (current_proc->a11!=0)
				{
					current_proc->a11--;
					move_dragon_logo(-84);
				}
			}
			else
			{
				/* lad96, stick right */
				if ((*pa3) & MASK_JRIGHT)
				{
					if (current_proc->a11 !=2)
					{
						current_proc->a11++;
					   	move_dragon_logo(84);
					}
				}
			}
		}
LAD97:
		current_proc->a0=current_proc->pdata.p_store3;
	}
	while(--current_proc->a0>0);

	return;
}

/******************************************************************************
 Function: void spin_the_mk_badge(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	spin the neat dragon
******************************************************************************/
void spin_the_mk_badge(void)
{
	OBJECT *ta9;

	send_code_a3(TS_P1_PICKED);

	(void *)ta9=current_proc->pa9;			// save diff text

	dallprc(PID_BANI);					// stop badge rotator dude
	current_proc->pa9=a_badge_spin;
	current_proc->a10=0x40;

	/* lad9a */
	do
	{
		process_sleep(1);
		frame_a9(current_proc->pa8);
	}
	while(--current_proc->a10>0);
	delobjp2(current_proc->pa8);				// delete badge obj
	delobjp2(ta9);								// delete diff text

	triple_sound(current_proc->a11+0x7c);			// shao kahn comment
	process_sleep(8);
	return;
}

/******************************************************************************
 Function: void pan_to_pick_tower(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	pan to the correct tower
******************************************************************************/
void pan_to_pick_tower(void)
{
	OBJECT *obj;

	ladder_x_vel.pos=mhe_x_vels[current_proc->a11];

	/* wait for top birck to be centered */
	if (current_proc->a11!=1) 				// hard --> we are already there
	{
		obj=get_current_1st_obj();

		/* ladd7 */
		do
		{
			process_sleep(1);
		}
		while(abs(obj->oxpos.u.intpos-SCX(0xb0))>SCX(3));

		ladder_x_vel.pos=0;
		ladder_x.pos=(SCRRGT/2)<<16;
		ladder_order=current_proc->a11;
	}

	return;
}

/******************************************************************************
 Function: void zoom_in(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	pan to the correct tower
******************************************************************************/
void zoom_in(void)
{
	XYTYPE ta9;

	OBJECT *obj;

	obj=get_current_1st_obj();
	ladder_y.pos=obj->oypos.pos;				// set new ladder "y"

	process_sleep(0x10);

	ladder_y_vel.pos=mhe_y_vels[current_proc->a11];

	tsound(0x19);

	/* ZOOM IN ! */
	ta9.u.xpos=0x0400;
	ta9.u.ypos=0x0400;
	/* zoom3 */
	do
	{
		process_sleep(1);
		ladder_scale.yx=ta9.yx;
		ta9.u.xpos-=0x0010;
		ta9.u.ypos-=0x0010;
	}
	while(ta9.yx!=0x01000100);

	ladder_x_vel.pos=0;
	ladder_y_vel.pos=0;					// stop moving already
	ladder_scale.yx=ta9.yx;
	process_sleep(2);					// ok now everyone is full scale

	call_every_tick=NULL;

	return;
}

/******************************************************************************
 Function: void pan_down(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	pan down
******************************************************************************/
void pan_down(void)
{
	create_base_object();

	current_proc->a5=get_player_ochar_a5();
	base_offset_obj(current_proc->a5,SCX(0x20),-64);

	(OBJECT*)current_proc->pa9=current_proc->pa8;
	solid_backdrop_box();
	base_insert();

	current_proc->a11=0x10;			// button exit timer !!

	f_novel=0;

	/* pandown4 */
	do
	{
		process_sleep(1);

		if (p1_state==PS_ACTIVE)
			current_proc->a1=swcurr.u.p1pad & M_P1ACTION;
		else current_proc->a1=swcurr.u.p2pad & M_P2ACTION;

		if (current_proc->a1!=0)
		{
			if (--current_proc->a11==0)
				break;
		}
	}
	while(((OBJECT *)current_proc->a10)->oypos.u.intpos>SCY(0xa7));

	/* pandown9 */
	f_novel=1;
	return;
}

/******************************************************************************
 Function: OBJECT *solid_backdrop_box(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	draw a solid back box
******************************************************************************/
OBJECT *solid_backdrop_box(void)
{
	OBJECT *obj;

	obj=get_object();

	obj->oflags|=M_BOX_POLY;

	/* set color */
	obj->header.t_xoffset=0x7f;			// set red
	obj->header.t_yoffset=0;
	obj->header.t_width=0;

	obj->osize.u.xpos=42;		// set size of box
	obj->osize.u.ypos=54;

	obj->oxpos.u.intpos=((OBJECT *)current_proc->pa9)->oxpos.u.intpos-2;
	obj->oypos.u.intpos=((OBJECT *)current_proc->pa9)->oypos.u.intpos-2;		// set coord

	current_proc->pa8=obj;

	return(obj);
}

extern ADDRESS ochar_order_mugs[];
/******************************************************************************
 Function: ADDRESS get_player_ochar_a5(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: ptr to correct char face

 Description:	retrieve the correct player face
******************************************************************************/
ADDRESS get_player_ochar_a5(void)
{
	if (p1_state==PS_ACTIVE)
		return(ochar_order_mugs[p1_char]);
	else return(ochar_order_mugs[p2_char]);
}

/******************************************************************************
 Function: void create_base_object(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: current_proc->a10 last object
		  current_proc->pa8 object

 Description:	create the base object of the mountain
******************************************************************************/
void create_base_object(void)
{
	OBJECT *obj;

	obj=get_current_1st_obj();
	/* cbo3 */
	do
	{
		(OBJECT *)current_proc->a10=obj;
		obj=obj->olink;
	}
	while(obj!=NULL);


	gso_dmawnz_ns(obj,(ADDRESS)MKBRICK2,ladder_anims,0);
	alloc_cache(MKBRICK2,&ladder_anims,obj);
	obj->oyvel.pos=-PANDOWN;
	obj->oxpos.u.intpos=((OBJECT *)current_proc->a10)->oxpos.u.intpos;
	obj->oypos.u.intpos=((OBJECT *)current_proc->a10)->oypos.u.intpos+((OBJECT *)current_proc->a10)->osize.u.ypos;	// pos under last
	obj->olink=NULL;			// new end of list
	((OBJECT *)current_proc->a10)->olink=obj;
	(OBJECT *)current_proc->a10=obj;			// new last obj

	/* create mountain objects */
	base_offset_obj((ADDRESS)UPEVILA,-SCX(0x75),SCY(0x10));
	base_offset_obj((ADDRESS)UPEVILB,SCX(0x6c)+4,SCY(0x10)+4);

	return;
}

/******************************************************************************
 Function: void move_dragon_logo(short pa1)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa1 - amount to move dragon logo

 Returns: None

 Description:	move the dragon logo to the correct screen pos
******************************************************************************/
extern OIMGTBL *emh_images[];
void move_dragon_logo(short pa1)
{
	OBJECT *ta8=current_proc->pa8;

	/* update diff text */
	current_proc->pa8=(OBJECT *)current_proc->pa9;
	animate(SINGLE_ANI,emh_images[current_proc->a11],(current_proc->pa8)->oflags);
	current_proc->pa8=ta8;

	(current_proc->pa8)->oxpos.u.intpos+=pa1;
	((OBJECT*)current_proc->pa9)->oxpos.u.intpos+=pa1;

	send_code_a3(TS_P1_CURS);
	return;
}

/******************************************************************************
 Function: OBJECT *get_current_1st_obj(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: ptr to first object on correct list

 Description:	retieve ptr to first object on current list
******************************************************************************/
OBJECT *get_current_1st_obj(void)
{
	OBJECT *obj;

	obj=get_current_objlst();
	return(obj->olink);
}

/******************************************************************************
 Function: OBJECT *get_current_obj(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: ptr to head to current list

 Description:	retieve current object list
******************************************************************************/
OBJECT *get_current_objlst(void)
{
	return(mhe_table[current_proc->a11].olist);
}

/******************************************************************************
 Function: void base_offset_obj(ADDRESS pa5,short pa6,short pa7)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa5 - frame to setup
			 pa6 - x offset
			 pa7 - y offset

 Returns: current_proc->a8  - ptr to newly created object

 Description:	retieve current object list
******************************************************************************/
void base_offset_obj(ADDRESS pa5,short pa6,short pa7)
{
	OBJECT *obj;

	gso_dmawnz_ns(obj,pa5,ladder_anims,0);
	alloc_cache((void *)pa5,ladder_anims,obj);

	obj->olink=NULL;
	obj->ozval=500;
	obj->oxpos.u.intpos=((OBJECT *)current_proc->a10)->oxpos.u.intpos+pa6;
	obj->oypos.u.intpos=((OBJECT *)current_proc->a10)->oypos.u.intpos+pa7;

	base_insert();

	return;
}

/******************************************************************************
 Function: void base_insert(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	insert onto display list
******************************************************************************/
void base_insert(void)
{
	OBJECT *obj=current_proc->pa8;

	obj->oyvel.pos=-PANDOWN;
	insert_object(obj,(OBJECT **)get_current_objlst());

	return;
}

/******************************************************************************
 Function: void ladder_irq(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	routine to call on ladder screen every tick
******************************************************************************/
void ladder_irq(void)
{
	long ta10,ta11,ta3;
	MHE_ORD *ta0;

	ladder_y.pos+=ladder_y_vel.pos;			// y vel if any

	ladder_x.pos+=ladder_x_vel.pos;			// x vel if anx


	ta10=SCX(192)<<8;
	ta11=SCY(96)<<8;

	ta11=ta11/ladder_scale.u.xpos;
	ta10=ta10/ladder_scale.u.ypos;

	ta3=SCX(0x19e)<<8;
	ta3=ta3/ladder_scale.u.xpos;

	(long)current_proc->a3=ta3;
	(long)current_proc->a11=ta11;
	(long)current_proc->a10=ta10;

	ta0=mhe_ladder_order[ladder_order];
	(short)current_proc->a2=ladder_x.u.intpos;

	process_a_ladder(ta0,(short)current_proc->a2);

	ta0->routine();
	ta0++;
	process_a_ladder(ta0,(short)current_proc->a2);

	ta0->routine();
	ta0++;
	process_a_ladder(ta0,(short)current_proc->a2);
}

/******************************************************************************
 Function: void process_a_ladder(MHE_ORD *pa0,short pa2)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa0 - info table
			 pa2 - x position to center around

 Returns: None

 Description:	process tower
******************************************************************************/
void process_a_ladder(MHE_ORD *pa0,short pa2)
{
	OBJECT *obj;
	OBJECT *oa0;

	obj=*((OBJECT **)(pa0->olist));

	if (obj==NULL)
		return;								// empty list

	obj->oscale.yx=ladder_scale.yx;			// update to newest scale value
	obj->oypos.u.intpos=ladder_y.u.intpos+((short)current_proc->a11)*pa0->bskip;		// starting local

	obj->oxpos.u.intpos=pa2-(((short)current_proc->a10)>>1);		// center image on screen

	/* lq3 */
	do
	{
		obj->oscale.yx=ladder_scale.yx;		// update each object
		oa0=(OBJECT *)obj->oslink;					// ptr to assoc mug
		oa0->oscale.yx=ladder_scale.yx;

		oa0->oxpos.u.intpos=obj->oxpos.u.intpos+(short)((SCX(108)<<8)/ladder_scale.u.xpos)+1;
		oa0->oypos.u.intpos=obj->oypos.u.intpos+(short)((18<<8)/ladder_scale.u.ypos);

		(OBJECT *)current_proc->a7=obj;				// prev object
		obj=obj->olink;
		if (obj==NULL)
			break;

		obj->oxpos.u.intpos=((OBJECT *)current_proc->a7)->oxpos.u.intpos;
		obj->oypos.u.intpos=((OBJECT *)current_proc->a7)->oypos.u.intpos+(short)current_proc->a11;
	}
	while(1);

	/* lq9 */
	return;
}

/******************************************************************************
 Function: void get_a_brick(WORD pa7,OBJECT **olist)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa7 - char #
			 olist - object list to insert onto

 Returns: current_proc->pa8 - created object

 Description:	draw a brick
******************************************************************************/
void get_a_brick(WORD pa7,OBJECT **olist)
{
	OBJECT *obj;
	OBJECT *obj1;

	gso_dmawnz_ns(obj,(ADDRESS)MKBRICK,ladder_anims,0);
	alloc_cache(MKBRICK,&ladder_anims,obj);
	obj->oyvel.pos=-PANDOWN;

	gso_dmawnz_ns(obj1,ochar_order_mugs[pa7],ladder_anims,0);
	alloc_cache((void *)ochar_order_mugs[pa7],&ladder_anims,obj1);
	obj1->oyvel.pos=-PANDOWN;

	insert_object(obj1,olist);

	current_proc->pa8=obj;
	(OBJECT *)obj->oslink=obj1;

	return;
}

/******************************************************************************
 Function: void easy_to_hard(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	process easy to hard towers
******************************************************************************/
void easy_to_hard(void)
{
	(long)current_proc->a2=(ladder_x.pos>>16)-((short)current_proc->a3);
	return;
}

/******************************************************************************
 Function: void med_to_hard(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	process easy to hard towers
******************************************************************************/
void med_to_hard(void)
{
	(long)current_proc->a2=(ladder_x.pos>>16)+((short)current_proc->a3);
	return;
}

/******************************************************************************
 Function: void set_match_1_diff(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	set match difficulty
******************************************************************************/
short mhe_match_1_diffs[]={0,3,0};

void set_match_1_diff(void)
{
	diff=(ladder_order<=2)?mhe_match_1_diffs[ladder_order]:mhe_match_1_diffs[2];
	return;
}

/******************************************************************************
 Function: void init_map_start(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	start the map position
******************************************************************************/
void init_map_start(void)
{
	WORD *ta0;

	if ((ta0=ladder_base)==NULL)
		ta0=map_1;							// safety net

	/* im3 */
	switch (current_proc->a11)
	{
		case 1: // diff_is_hard
			ta0+=ENTRIES_MED+1;				// 1 factor to skip termin list
		case 0:	// diff_is_med
			ta0+=ENTRIES_EASY+1;				// 1 factor to skip termin list
		case 2: // diff_is_easy
			map_start=ta0;
			break;
	}

	return;
}

#if OLD_LADDER
/******************************************************************************
 Function: void ladder_badge_spinner(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	process to spin the pretty coin
******************************************************************************/
void ladder_badge_spinner(void)
{
	current_proc->pa9=a_lbadge_spin;
	framew(5);

	wait_forever();
}
#endif
