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
#include "mkinit.h"
#include "mkmain.h"
#include "mksound.h"
#include "mkvs.h"
#include "valloc.h"
#include "mkfade.h"
#include "mkfile.h"
#include "psxspu.h"

extern void *vs_module[];
extern OIMGTBL *battle_digits[];

/******************************************************************************
 Function: void vs_or_ladder(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	The cool vs screen
******************************************************************************/
void vs_or_ladder(void)
{

	if (gstate!=GS_AMODE)
	{
#if MK_EJBBUG
		if (df_fastrun!=0)
			return;
#endif

		if (count_active_players()==1)
		{
			mk_ladder();
			return;
		}
		SET_GAME_STATE(GS_VS);
	}


	/* game option to allow skipping of vs screen */
	if ( f_no_vs )
	{
		file_load_message(2);
		process_sleep(2);
		freeze_2_pages();
		murder_myoinit_score();
		idcomp_ptr=idcomp_base=image_dcomp_buffer;
		return;
	};

	/* vs3 */

	/* loopit, not_highest_battle */
	murder_myoinit();
	irqskye=0;


	/* load sound stuff */
	sound_module_load(SMOD_VS);


	/* load background data */
	load_bkgd=BKGD_VS_MOD;
	init_background_module(vs_module);
	multi_plane();
	dlists=dlists_bogus;


	/* load character 1 and character 2 data */
	character_texture_load(p1_char,CHAR_VS,p1_heap,SYNC_LOAD);
	character_texture_load(p2_char,CHAR_VS,p2_heap,SYNC_LOAD);

	/* set decomp base to p2 data area */
	idcomp_base=(LONG *)(&player_heap[PLAYER_HEAP_SIZE]-200000);

	//play_generic_stream(TRK_VS);			// SCA3(0x35), mortal sans!!

	f_load=0;
	DISPLAY_OFF;

	CREATE(PID_FX,mk_bat_signal);
	CREATE(PID_FX,mk_bat_signal_f);
	CREATE(PID_FX,mk_battle);
	CREATE(PID_FX,mk_vs);
	CREATE(PID_COMB,combination_proc);

	process_sleep(1);

	fadein_jsrp(0);
	slide_in_huge_players();

	nosounds();
	if ( f_music!=0 )
	{
		current_proc->a0=f_no_sfx;
		f_no_sfx=0;
		tsound(0x8f);
		f_no_sfx=current_proc->a0;
	}

	process_sleep(0xb0);

	/* CD IMPROVEMENT
		1) PRELOAD PLAYER 1 DATA INTO HEAP
		2) PRELOAD BKGD TEXTURE DATA INTO HEAP2
	*/
	cd_preload_fade();
	cd_preload_p1();
	cd_preload_bkgd();

	dallprc(PID_COMB);				// stop combinations proc


	/* set decomp base to decomp area */
	idcomp_base=image_dcomp_buffer;

#if VS_OFF
	/* arcade display flow */
	process_sleep(4);
	murder_myoinit_score();
	clr_scrn();
	process_sleep(0x08);
#else
	/* keep screen up with loading message */
	file_load_message(1);
	process_sleep(2);
	freeze_2_pages();
	murder_myoinit_score();
#endif

	/* vs_exit */
	return;
}

/******************************************************************************
 Function: void mk_vs(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	Vs burn fx stuff
******************************************************************************/
void mk_vs(void)
{
	OBJECT *obj;

	gso_dmawnz_ns(obj,(ADDRESS)BURNV1,vs_anims,0);
	set_xy_coordinates(obj,SCX(0xab),SCY(0x50)+4);
	alloc_cache(BURNV1,&vs_anims,obj);
	insert_object(obj,&objlst);
	current_proc->pa9=a_vs_v;
	framew(2);

	gso_dmawnz_ns(obj,(ADDRESS)BURNS1,vs_anims,0);
	set_xy_coordinates(obj,SCX(0xd3),SCY(0x50)+4);
	alloc_cache(BURNS1,&vs_anims,obj);
	insert_object(obj,&objlst);
	current_proc->pa9=a_vs_s;
	framew(2);

	process_suicide();
}

/******************************************************************************
 Function: void mk_battle(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	Vs Battle fx stuff
******************************************************************************/
void mk_battle(void)
{
	OBJECT *obj;
	WORD b0,b1,b2;

	gso_dmawnz_ns(obj,(ADDRESS)BATTLE,vs_anims,0);
	obj->oypos.u.intpos=10+4;
	center_obj_x(obj);
	alloc_cache(BATTLE,&vs_anims,obj);
	insert_object(obj,&objlst);							// text battle

	current_proc->a0=(battle_num<1000)? battle_num:999;

	/* mkbat3 */
	b0=current_proc->a0/100;			// b0 = 100s
	current_proc->a0%=100;
	b1=current_proc->a0/10;				// b1 = 10s
	b2=current_proc->a0%10;				// b2 = 1s

	/* 3 digits */
	if (b0!=0)
	{
		do_mk_number(b0);
		(OBJECT *)current_proc->a10=current_proc->pa8;

		do_mk_number(b1);
		(OBJECT *)current_proc->pa9=current_proc->pa8;

		do_mk_number(b2);

		b0=(current_proc->pa8)->osize.u.xpos;
		b1=((OBJECT *)current_proc->pa9)->osize.u.xpos;
		b2=((OBJECT *)current_proc->a10)->osize.u.xpos;

		current_proc->a3=SCRRGT/2-((b0+b1+b2)>>1);

		((OBJECT *)current_proc->a10)->oxpos.u.intpos=(short)(current_proc->a3);
		current_proc->a3+=b2;
		((OBJECT *)current_proc->pa9)->oxpos.u.intpos=(short)(current_proc->a3);
		current_proc->a3+=b1;
		(current_proc->pa8)->oxpos.u.intpos=(short)(current_proc->a3);
	}
	else
	{
		/* 2 digits */
		if (b1!=0)
		{
			do_mk_number(b1);
			(OBJECT *)current_proc->pa9=current_proc->pa8;

			do_mk_number(b2);

			b0=(current_proc->pa8)->osize.u.xpos;
			b1=((OBJECT *)current_proc->pa9)->osize.u.xpos;

			current_proc->a3=SCRRGT/2-((b0+b1)>>1);

			((OBJECT *)current_proc->pa9)->oxpos.u.intpos=(short)(current_proc->a3);
			current_proc->a3+=b1;
			(current_proc->pa8)->oxpos.u.intpos=(short)(current_proc->a3);
		}
		else
		{
			/* 1 digit */
			do_mk_number(b2);

			b0=(current_proc->pa8)->osize.u.xpos;
			current_proc->a3=SCRRGT/2-((b0)>>1);
			(current_proc->pa8)->oxpos.u.intpos=(short)(current_proc->a3);
		}
	}

	process_suicide();
}

void do_mk_number(WORD pa0)
{
	OBJECT *obj;

	gso_dmawnz_ns(obj,(ADDRESS)BATTLE0,vs_anims,0);
	set_xy_coordinates(obj,SCX(0xc8),SCY(0x20)+4+3);
	alloc_cache(BATTLE0,&vs_anims,obj);
	insert_object(obj,&objlst);

	animate(SINGLE_ANI,battle_digits[pa0],obj->oflags);
}

/******************************************************************************
 Function: void mk_bat_signal_f(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	Bat singnal flip
******************************************************************************/
void mk_bat_signal_f(void)
{
	OBJECT *obj;

	obj=make_a_mk_light(SCX(0x13d),4);
	flip_single(obj);

	/* signal4 */
	current_proc->pa9=a_mklight;
	framew(4);

	process_suicide();
}

/******************************************************************************
 Function: void mk_bat_signal(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	Bat singnal flip
******************************************************************************/
void mk_bat_signal(void)
{
	OBJECT *obj;

	obj=make_a_mk_light(0xfffe,4);

	/* signal4 */
	current_proc->pa9=a_mklight;
	framew(4);

	process_suicide();
}

/******************************************************************************
 Function: OBJECT *make_a_mk_light(short pa9x,short pa9y)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa9x - x coord of light
			 pa9y - y coord of light

 Returns: None

 Description:	Bat singnal flip
******************************************************************************/
OBJECT *make_a_mk_light(short pa9x,short pa9y)
{
	OBJECT *obj;

	gso_dmawnz_ns(obj,(ADDRESS)MKLIGHT1,vs_anims,0);
	set_xy_coordinates(obj,pa9x,pa9y);
	alloc_cache(BATTLE0,&vs_anims,obj);
	insert_object(obj,&objlst);

	return(obj);
}

VPIECE *ochar_pieces[] =
{
	kano_pieces,
	sonya_pieces,
	jax_pieces,
	ind_pieces,
	sz_pieces,
	swat_pieces,

	lia_pieces,
	robo1_pieces,
	robo2_pieces,
	lao_pieces,
	tusk_pieces,

	sg_pieces,
	st_pieces,
	lk_pieces,
	smoke_pieces,

	kitana_pieces,		// 15
	jade_pieces,  		// 16
	mileena_pieces,		// 17
	scorpion_pieces,	// 18
	reptile_pieces,		// 19
	ermac_pieces,		// 20
	oldsz_pieces,		// 21
	oldsmoke_pieces,	// 22
	noob_pieces			// 23
};

/******************************************************************************
 Function: void slide_in_huge_players(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	show the cool players coming together to fight
******************************************************************************/
void slide_in_huge_players(void)
{
	VPIECE *vtbl;
	void *iptr;

	send_code_a3(TS_MAP_ZOOM);	// send_code_a3(0x451) Mortal Sans Pre-whoosh buildup

	f_novel=1;

	/* player 1 */
	build_huge_left(p1_heap);

	/* player 2 */
	vtbl=(VPIECE *)p2_heap;
	(VPIECE *)iptr=vtbl+1;			// get to piece info
	while (GET_LONG(iptr)!=0)
	{
		/* slide5 */
		create_1_piece(iptr,vtbl->v_p2x,vtbl->v_p2y,1);
		((ADDRESS *)iptr)+=2;
	}

	/* slide6 */
	f_novel=0;

	process_sleep((SDIST/(SSPEED/0x10000))-8);

	f_novel=1;

	return;
}

void build_huge_left(void *pheap) 
{
	VPIECE *vtbl;
	void *iptr;

	vtbl=(VPIECE *)pheap;
	(VPIECE *)iptr=vtbl+1;			// get to piece info
	while (GET_LONG(iptr)!=0)
	{
		/* slide3 */
		create_1_piece(iptr,vtbl->v_p1x,vtbl->v_p1y,0);
		((ADDRESS *)iptr)+=2;
	}

	return;
}

/******************************************************************************
 Function: void create_1_piece(void *pa7,short pa0,short pa1,short pa2)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa7 - ptr to piece data
			 pa0 - x offset
			 pa1 - y offset
			 pa2 - flip flag (0=no(player1) 1=yes(player2))

 Returns: None

 Description:	create object for piece to display
******************************************************************************/
void create_1_piece(void *pa7,short pa0,short pa1,short pa2)
{
	PROCESS *ptemp;
	OBJECT *obj;
	void *pal;

	if ( pa2==0 )
	{
		current_proc->pa9=(void *)COMPUTE_ADDR(p1_heap,(GET_LONG(pa7)));
		ptemp=gmo_proc(p1_heap);
	}
	else
	{
		current_proc->pa9=(void *)COMPUTE_ADDR(p2_heap,(GET_LONG(pa7)));
		ptemp=gmo_proc(p2_heap);
	}

	obj=current_proc->pa8;

	/* alt pal check */
	if (pa2==1 && p1_char==p2_char)
	{
		pal=(void *)(GET_LONG(pa7+1));			// get palette ptr
		if (pal!=NULL)
			swpal(pal,obj);
	}

	/* cone3 */
	if (pa2)
	{
		flip_multi(obj);
		obj->oxvel.pos=-SSPEED;
		obj->oxpos.u.intpos=(pa0*SCF_X/100)-(-SDIST);
		obj->oypos.u.intpos=pa1*SCF_Y/100;
	}
	else
	{
		obj->oxvel.pos=SSPEED;
		obj->oxpos.u.intpos=(pa0*SCF_X/100)-(SDIST);
		obj->oypos.u.intpos=pa1*SCF_Y/100;
	}

	set_noscroll(obj);

	obj->ozval=0;

	insert_object(obj,&objlst);

	return;
}

/******************************************************************************
 Function: void combination_proc(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	secret combo process
******************************************************************************/
void combination_proc(void)
{
	ADDRESS *ta3;
	OBJECT *obj;

	gso_dmawnz_ns(obj,(ADDRESS)BOX2,vs_anims,0);
	alloc_cache(BOX2,&vs_anims,obj);

	obj->ozval=1;

	obj->oypos.u.intpos=SCY(0xe9)+0x10-4;
	center_obj_x(obj);
	start_off_screen(obj);
	current_proc->pa9=(OBJECT *)current_proc->pa8;

	/* symbols */
	ta3=&(current_proc->pdata.p_store1);
	current_proc->a7=6;
	current_proc->a6=SCX(2)+2;

	/* cproc5 */
	do
	{
		gso_dmawnz_ns(obj,(ADDRESS)I_DRAGON,vs_anims,0);
		alloc_cache(I_DRAGON,&vs_anims,obj);
		obj->oxpos.u.intpos=((OBJECT *)current_proc->pa9)->oxpos.u.intpos+current_proc->a6;
		obj->oypos.u.intpos=((OBJECT *)current_proc->pa9)->oypos.u.intpos+2;

		*ta3=(ADDRESS)current_proc->pa8;			// save object here
		ta3++;

		current_proc->a6+=19;				// next symbol to the right

		(LONG)obj->oslink=0;

		/* soff3 */
		insert_object(obj,&objlst);

		obj->ozval=2;
	}
	while(--current_proc->a7>0);

	if (gstate==GS_AMODE)
		process_suicide();

	f_novel=0;
	c_hp[0]=c_hp[1]=0;
	c_lp[0]=c_lp[1]=0;
	c_block[0]=c_block[1]=0;
	c_lk[0]=c_lk[1]=0;
	c_hk[0]=c_hk[1]=0;

	process_sleep(2);

	/* cproc 7 */
	while (1)
	{
		l_hp[0]=l_hp[1]=0;
		l_lp[0]=l_lp[1]=0;
		l_block[0]=l_block[1]=0;
		l_lk[0]=l_lk[1]=0;
		l_hk[0]=l_hk[1]=0;

		process_sleep(1);

		setup_direction(MASK_JUP<<0);			// p1 up
		current_proc->a0=0;
		check_button((OBJECT *)current_proc->pdata.p_store1);
		check_button((OBJECT *)current_proc->pdata.p_store2);
		check_button((OBJECT *)current_proc->pdata.p_store3);

		setup_direction(MASK_JUP<<16);			// p2 up
		check_button((OBJECT *)current_proc->pdata.p_store4);
		check_button((OBJECT *)current_proc->pdata.p_store5);
		check_button((OBJECT *)current_proc->pdata.p_store6);
	}
}


WORD *button_ram_spots[]=
{
	l_hp,		// symbol #1
	l_hk,	//l_block,		// symbol #2
	l_lk,	//l_lk,		// symbol #3
	l_hp+1,		// symbol #4
	l_hk+1,		//l_block+1,   	// symbol #5
	l_lk+1	//l_lk+1		// symbol #6
};

/******************************************************************************
 Function: inline setup_direction(LONG pa4)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	setup direction
******************************************************************************/
inline void setup_direction(LONG pa4)
{
	(long)current_proc->a3=(swcurr.padvalue & pa4) ? -1:1;

	return;
}

/******************************************************************************
 Function: void start_off_screen(OBJECT *obj)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	start offscreen
******************************************************************************/
void start_off_screen(OBJECT *obj)
{
	insert_object(obj,&objlst);

	return;
}

/******************************************************************************
 Function: void check_button(OBJECT *obj)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	start offscreen
******************************************************************************/
void check_button(OBJECT *obj)
{
	WORD *ta0,*ta2;
	long offset;

	current_proc->pa8=obj;

	ta0=button_ram_spots[current_proc->a0];

	if (*ta0 != 0 )
	{
		ta2=&combo_1+current_proc->a0;		// a2 ---> combo ram to change

		offset=(long)obj->oslink+(long)current_proc->a3;
		if (offset<0)
			offset=9;
		if (offset>9)
			offset=0;

		(long)obj->oslink=offset;
		*ta2=offset;					// stuff intro combo ram as well

		switch (offset)		// stuff new offset into ram
		{
			case 0:
				c_hp[0]=offset;
				break;
			case 1:
				c_hp[1]=offset;
				break;
			case 2:
				c_lp[0]=offset;
				break;
			case 3:
				c_lp[1]=offset;
				break;
			case 4:
				c_block[0]=offset;
				break;
			case 5:
				c_block[1]=offset;
				break;
			case 6:
				c_hk[0]=offset;
				break;
			case 7:
				c_hk[1]=offset;
				break;
			case 8:
				c_lk[0]=offset;
				break;
			case 9:
				c_lk[1]=offset;
				break;
		}

		current_proc->a1=(ADDRESS)COMPUTE_ADDR(vs_anims,symbol_table[offset]);
		swpal((void *)GET_LONG((((OIMGTBL *)current_proc->a1)+1)) ,obj);
		animate(SINGLE_ANI,(OIMGTBL*)current_proc->a1,obj->oflags);
	}
	current_proc->a0++;
	return;
}

void cd_preload_fade(void)
{
	/* kill fade so loads are more async */
  psxspu_start_cd_fade(FADE_TIME,0);
  while(psxspu_get_cd_fade_status())
		process_sleep(1);
	psxcd_stop();
  while(psxcd_waiting_for_pause())
		process_sleep(1);

	return;
}

void cd_preload_p1(void)
{
	character_texture_load(p1_char,CHAR_NORMAL,p1_heap,ASYNC_LOAD);
	while ( psxcd_async_on() )
		process_sleep(1);
	module_os_close(module_int);
	p1_preload=1;

	return;
}

void cd_preload_bkgd(void)
{

	WORD save_curback;

	/* bkgd preload */
	save_curback=curback;
	ochar_force_curback();
	texture_level_load(table_o_levels[curback],ASYNC_LOAD);
	while ( psxcd_async_on() )
		process_sleep(1);
	module_os_close(module_int);
	bkgd_preload=1;

	curback=save_curback;

	return;
}
