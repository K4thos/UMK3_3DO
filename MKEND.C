/******************************************************************************
 File: mkend.c

 Date: Jan 1995

 (C) Williams Entertainment

 Mortal Kombat III Ending Stuff Routines
******************************************************************************/

#define FASTSHAO 1
/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkos.h"
#include "mkobj.h"
#include "mkfx.h"
#include "mkutil.h"
#include "mkani.h"
#include "mkgame.h"
#include "mkjoy.h"
#include "mkguys.h"
#include "mkinit.h"
#include "mkamode.h"
#include "mkend.h"
#include "mkmain.h"
#include "mkbio.h"
#include "mkpal.h"
#include "mktext.h"
#include "mkbuyin.h"
#include "mksound.h"
#include "mkbkgd.h"
#include "valloc.h"
#include "mkbonus.h"
#include "mkladder.h"
#include "mkboss.h"
#include "mkcanned.h"
#include "mkfade.h"
#include "mkfile.h"
#include "wessapi.h"

extern BYTE mock_heap[] __attribute__ ((aligned(4)));

void trick_buttons(int,int);

extern void *dlists_bogus[];

#if 0
/******************************************************************************
 Function: void arcade_page(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	give an arcade code
******************************************************************************/
char txt_arcade1[]="ULTIMATE MK3 ARCADE KODE";
char txt_end_gameover[]="GAME OVER";
extern void *ultimate_anims[];
extern void *smoke_symbol_table[];
LONG arcade_codes[]=
{
	0x227227,						// top of table most frequennt codes
	0x050050,
	0x833833,
	0x300003,
	0x880088,
	0x033330						// most infrequent codes
};

void arcade_page(void)
{
#if 0
	OBJECT *obj;
	WORD i;
	LONG sindex;
	LONG showcode;

	gstate=GS_UNLOCK_SMOKE;

	DISPLAY_OFF;
	clr_scrn();
	murder_myoinit_score();
	dlists=dlists_bogus;

	mk3_gradient(RED_GRAD);

	p15_centered(txt_arcade1,160,0xd0);

	/* draw border box */
	obj=make_solid_object(0x001c,110+6,SCY(0x15)+1);
	obj->header.t_xoffset|=0x80;			// set red solid
	obj->oypos.u.intpos=SCY(0x76);
	center_obj_x(obj);
	insert_object(obj,&objlst);
	(short)current_proc->a11=obj->oxpos.u.intpos;
	current_proc->a10=(ADDRESS)obj;
	(short)current_proc->a11+=2;

	/* pick code */
	if (randper(200)==SYSTEM_CARRY_SET)
		showcode=arcade_codes[randu(ARCADE_L3_PROB)-1];
	else
	{
		if (randper(450)==SYSTEM_CARRY_SET)
			showcode=arcade_codes[randu(ARCADE_L2_PROB)-1];
		else
		{
			showcode=arcade_codes[randu(ARCADE_L1_PROB)-1];
		}
	}

	/* draw the code */
	for (i=0;i<6;i++)
	{
		sindex=(showcode>>((5-i)*4)) & 0xf;			// -2 factor to simulate mult by 4

		current_proc->a1=(ADDRESS)COMPUTE_ADDR(ultimate_anims,smoke_symbol_table[sindex]);
		gso_dmawnz(obj,(ADDRESS)current_proc->a1,ultimate_anims,0);
		alloc_cache((OIMGTBL *)current_proc->a1,&ultimate_anims,obj);
		insert_object(obj,&objlst);
		obj->oypos.u.intpos=((OBJECT *)current_proc->a10)->oypos.u.intpos+2;
		obj->oxpos.u.intpos=(short)current_proc->a11;
		(short)current_proc->a11+=(2+obj->osize.u.xpos);
	}

	pds_centered(txt_end_gameover,160,SCY(0x10)+16);
	process_sleep(2);

	fadein_jsrp(3);			// pause after fade

	process_sleep(0x40*4);

	murder_myoinit_score();
	process_sleep(3);
	wess_seq_stopall();
#endif
	return;
}
#endif

/******************************************************************************
 Function: void smoke_page(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	smoke page
******************************************************************************/
char txt_wrong[]="- INCORRECT CODE -";
char txt_from_this[]="FROM THIS POINT ON....";
char txt_smoke_is_act[]="SMOKE IS AT YOUR CONTROL";
extern char txt_smoke1[];
#if FINAL_BRIEF==0
WORD the_code[]={1,1,1,1,1,1,0xffff};
#else
WORD the_code[]={0,1,0,6,9,6,0xffff};
#endif
extern void *smoke_symbol_table[];
extern void *ultimate_anims[];
void smoke_page(void)
{
#if 0
	OBJECT *obj;
	OBJECT **ta3;
	WORD *ta5;
	OBJECT *sym[10];
	int cmask;
	int pmask;

#if 0
	if (f_smoke==1)
	{
		ultimate_exit();
		return;
	}
#endif

	gstate=GS_UNLOCK_SMOKE;

	clr_scrn();
	murder_myoinit_score();
	dlists=dlists_bogus;

	mk3_gradient(RED_GRAD);

	sound_module_load(SMOD_REWARD);

	p15_centered(txt_smoke1,SCX(0xc0),SCY(0x10)+16);

	obj=make_solid_object(0xfc00,116,SCY(0x15)+1);
	obj->oypos.u.intpos=SCY(0x76);
	center_obj_x(obj);
	insert_object(obj,&objlst);
	(short)current_proc->a11=obj->oxpos.u.intpos;
	current_proc->a10=(ADDRESS)obj;
	(short)current_proc->a11+=2;

	ta3=sym;			// a3 --> 1st symbol object

	(LONG)current_proc->pa9=6;
	/* smoke 3 */
	do
	{
		current_proc->a1=(ADDRESS)COMPUTE_ADDR(ultimate_anims,smoke_symbol_table[0]);
		gso_dmawnz(obj,(ADDRESS)current_proc->a1,ultimate_anims,0);
		alloc_cache((OIMGTBL *)current_proc->a1,&ultimate_anims,obj);
		insert_object(obj,&objlst);
		obj->oypos.u.intpos=((OBJECT *)current_proc->a10)->oypos.u.intpos+2;
		obj->oxpos.u.intpos=(short)current_proc->a11;
		(short)current_proc->a11+=(2+obj->osize.u.xpos);
		*ta3=obj;			// save object here
		ta3++;
		(LONG)obj->oslink=0;			// symbol offset
	}
	while(--((LONG)current_proc->pa9)>0 );

	//pds_centered(txt_end_gameover,SCX(0xc8),SCY(0x10)+16);

	DISPLAY_ON;
	fadein_jsrp(3);			// pause after fade

	/* ultimate_scan */
	CREATE(PID_SMOKE_TIMER,count_down_timer);

	(LONG)current_proc->pdata.p_joyport=tick;

	c_hp[0]=c_hp[1]=0;
	c_lp[0]=c_lp[1]=0;
	c_block[0]=c_block[1]=0;

	cmask=pmask=0;

	/* cproc7 */
	do
	{
		process_sleep(1);

		l_hp[0]=l_hp[1]=0;				// clear ram
		l_lp[0]=l_lp[1]=0;
		l_block[0]=l_block[1]=0;
		l_hk[0]=l_hk[1]=0;
		l_lk[0]=l_lk[1]=0;

		/* set buttons based on real buttons pressed, trick orig system */
		pmask=cmask;
		cmask=PadRead(SONY_PAD_ID);
		trick_buttons(cmask,pmask);

		setup_direction((MASK_JUP<<16)|MASK_JUP);			// p1 up
		current_proc->a0=0;
		smoke_check_button(sym[0]);					// entry 0
		smoke_check_button(sym[1]);				  // entry 1
		smoke_check_button(sym[2]);					// entry 2
		smoke_check_button(sym[3]);					// entry 3
		smoke_check_button(sym[4]);					// entry 4
		smoke_check_button(sym[5]);					// entry 5
	}
	while(process_exist(PID_SMOKE_TIMER,0xffff)!=NULL);

	/* decode */
	ta5=the_code;
	ta3=sym;			// a3 --> 1st symbol object

	/* decode3 */
	do
	{
		obj=(OBJECT*)(*ta3);
		if (*ta5!=(LONG)obj->oslink)
			break;
		ta5++;
		ta3++;
	}
	while(*ta5!=0xffff);

	if (*ta5==0xffff)
	{
	 	smoke_jackpot();
	}
	else
	{
		/* sans_ultimate */
		pds_centered(txt_wrong,SCX(0xc8),SCY(0xe0)+16);
		current_proc->pa8=sym[0];		// need a valid object
		rsnd_sk_bonus_win();
		process_sleep(0x70);
	}

#endif
	ultimate_exit();
	stack_jump(amode);			// restart attract mode
}

void ultimate_exit(void)
{
	murder_myoinit_score();
	process_sleep(3);
	return;
}

void smoke_jackpot(void)
{
	WORD ta10,ta11;

	tsound(0x7e);			// outstanding!

	process_sleep(0x40);

	ta10=4;
	/* ftb3 */
	do
	{
		tsound(0x00);
		tsound(0x01);
		white_flash();
		process_sleep(2);
	}
	while(--ta10>0);

	pds_centered(txt_from_this,SCX(0xc8),SCY(0xd0)+16);
	pds_centered(txt_smoke_is_act,SCX(0xc8),SCY(0xe0)+16);
	process_sleep(0x90);

	f_smoke=1;

	return;
}

void count_down_timer(void)
{
	OBJECT *obj;
	OBJECT *objnum;

	if ((objnum=get_object())==NULL)
		ermac(5);
	objnum->osize.yx=0x000a000a;
	objnum->opal=get_fore_pal(WHITE_p);
	objnum->ozval=2;
	shape_buyin_num(9,objnum);
	objnum->oypos.u.intpos=SCY(0xa0);
	center_obj_x(objnum);
	insert_object(objnum,&objlst);

	/* cdown3 */
	current_proc->a10=9;
	do
	{
		count_shape(objnum);

		if ((swcurr.u.p1pad & P1B5) || swcurr.u.p2pad & P2B5)
			process_sleep(4);
		else process_sleep(0x40);
	}
	while(--current_proc->a10>0);

	count_shape(objnum);
	process_sleep(4);

	process_suicide();
}


void count_shape(OBJECT *obj)
{
	shape_buyin_num(current_proc->a10,obj);
	return;
}

/******************************************************************************
 Function: void smoke_check_button(OBJECT *obj)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	start offscreen
******************************************************************************/
WORD *ultimate_button_ram_spots[]=
{
	l_hp,		// symbol #1
	l_lp,		// symbol #2
	l_block,	// symbol #3
	l_lk,		// symbol #4
	l_hk,		// symbol #5
	l_hp+1,		// symbol #6
	l_lp+1,		// symbol #7
	l_block+1,	// symbol #8
	l_lk+1,		// symbol #9
	l_hk+1		// symbol #10
};

int mask_table[]=
{
	0x00040004,				// BUTTON 0 (L1)
	0x00080008,				// BUTTON 1 (R1)
	0x00800080,				// BUTTON 2 (SQUARE)
	0x00100010,				// BUTTON 3 (TRIANGLE)
	0x00200020,				// BUTTON 4 (CIRCLE)
	0x00400040				// BUTTON 5 (X)
};

void trick_buttons(int curr,int prev)
{
	int i;
	WORD *ta0;

	for ( i=0;i<6;i++)
	{
		ta0=ultimate_button_ram_spots[i];
		if ( ((curr^prev) & mask_table[i]) && (prev & mask_table[i]) )
			*ta0=1;
	}

	return;
}

void smoke_check_button(OBJECT *obj)
{
	WORD *ta0,*ta2;
	long offset;

	current_proc->pa8=obj;

	ta0=ultimate_button_ram_spots[current_proc->a0];

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

		current_proc->a1=(ADDRESS)COMPUTE_ADDR(ultimate_anims,smoke_symbol_table[offset]);
		swpal((void *)GET_LONG((((OIMGTBL *)current_proc->a1)+1)) ,obj);
		animate(SINGLE_ANI,(OIMGTBL*)current_proc->a1,obj->oflags);
	}
	current_proc->a0++;
	return;
}

/******************************************************************************
 Function: void shao_kahn_death_fx(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	ultimate death seq
******************************************************************************/
void shao_kahn_death_fx(void)
{
	ADDRESS ta1,ta2,ta3,ta4,ta5,ta6,ta7;
	WORD winchar;
#if JAP_VERSION==1
		TriggerPlayAttr trigger;
#endif

	p1_obj->oid|=0x8000;
	p2_obj->oid|=0x8000;

	kill_ocp(0,0x8000);				// kill all object except players

	if (p1_char==FT_SK)				// protect kahn process
	{
		p1_proc->procid|=0x8000;
		MURDER;
		p1_proc->procid&=0x7fff;
		fastxfer(p1_proc,wait_forever);		// disable sk proc
		winchar=p2_char;
	}
	else
	{
		p2_proc->procid|=0x8000;
		MURDER;
		p2_proc->procid&=0x7fff;
		fastxfer(p2_proc,wait_forever);		// disable sk proc
		winchar=p1_char;
	}

	scrolly.pos=0;
	stop_scrolling();					// stop screen
	stop_me(p1_obj);
	stop_me(p2_obj);

	swpal(bpal_black_P,p1_obj);				// both players --> black
	swpal(bpal_black_P,p2_obj);

	stop_scrolling();
	ta1=(ADDRESS)baklst1;
	ta2=(ADDRESS)baklst2;
	ta3=(ADDRESS)baklst3;
	ta4=(ADDRESS)baklst4;
	ta5=(ADDRESS)baklst5;
	ta6=(ADDRESS)baklst6;
	ta7=(ADDRESS)baklst7;

	baklst1=NULL;
	baklst2=NULL;
	baklst3=NULL;
	baklst4=NULL;
	baklst5=NULL;
	baklst6=NULL;
	baklst7=NULL;

	/* psycho flash !!! */
	current_proc->a10=7;
	do
	{
		tsound(0x7f);
		kahn_flash(0,bpal_white_P);
		kahn_flash(0xffff,bpal_black_P);
	}
	while(--current_proc->a10>0);

	if (p1_char==FT_SK)
	{
		current_proc->pa8=p1_obj;
		current_proc->a11=(ADDRESS)p1_proc;
		current_proc->a0=(ADDRESS)p2_obj;
	}
	else
	{
		current_proc->pa8=p2_obj;
		current_proc->a11=(ADDRESS)p2_proc;
		current_proc->a0=(ADDRESS)p1_obj;
	}

	/* skdie5, fade from white to normal */
	set_inviso((OBJECT *)current_proc->a0);		// make winner disappear

	nosounds();
	process_sleep(1);
	tsound(0);
	tsound(1);						// ka boom
	set_inviso(current_proc->pa8);	// all white = dont show sk
	process_sleep(4);

	baklst1=(OBJECT *)ta1;
	baklst2=(OBJECT *)ta2;
	baklst3=(OBJECT *)ta3;
	baklst4=(OBJECT *)ta4;
	baklst5=(OBJECT *)ta5;
	baklst6=(OBJECT *)ta6;
	baklst7=(OBJECT *)ta7;

#if FASTSHAO==1
	if ( p2_char==FT_SK )
	{
		p1_char=p1_heap_char=FT_SK;		 		// setup for player 1 being shao
		p2_obj->oheap=p1_obj->oheap;			// switch heap ptrs to player 1
		memcpy(p1_heap,p2_heap,350000);		//
	}
#endif

	fastxfer((PROCESS*)current_proc->a11,shao_kahn_dying);
	shake_a11(0x3,0x40);

	fadein_white_jsrp();

	tsound(0x87);
	tsound(0x88);

#if FASTSHAO==1			// preload background texture data into heap
	ta1=0;
	texture_level_load(table_o_levels[BKGD_SKDIE1_MOD],ASYNC_LOAD);
	process_sleep(0x50);
	while ( psxcd_async_on() )
		process_sleep(1);
	module_os_close(module_int);
	bkgd_preload=1;
#else
	process_sleep(0x50);
#endif

#if FASTSHAO==0
	//**************			// make shao kahn art load
	p1_heap_char=p2_heap_char=0xffff;
	//**************
#endif
	init_and_multi_plane(BKGD_SKDIE1_MOD);
	shake_a11(0x3,0x40);
	process_sleep(0x40);

	tsound(0x87);
	tsound(0x88);

//LOAD SHAO DATA
	sk_die_pit(0x30,0x6,0x40);

	tsound(0x87);
	tsound(0x88);
	sk_die_tower(0x20,0x6,0x40);
	sk_die_pit(0x20,0x8,0x40);

	sk_die_tower(0x18,0x6,0x40);
	sk_die_pit(0x18,0x8,0x40);

	tsound(0x87);
	tsound(0x88);

	sk_die_tower(0x10,0x6,0x40);
	sk_die_pit(0x10,0x8,0x40);

	current_proc->pdata.p_anirate=4;
	/* fastsk3 */
	do
	{
		sk_die_tower(0x8,0x6,0x40);
		sk_die_pit(0x8,0x8,0x40);
	}
	while(--current_proc->pdata.p_anirate>0);

	init_and_multi_plane(BKGD_SKDIE2_MOD);

	wess_seq_stopall();

	current_proc->a11=3;
	/* last_flash */
	do
	{
		tsound(0x00);
		tsound(0x01);
		white_flash();
		process_sleep(8);
	}
	while(--current_proc->a11>0);

	irqskye=0x7fff;			//0x0fff;
	process_sleep(3);

	fadein_white_jsrp();

	play_generic_tune(TRK_YOUWIN);			// SCA3(0x45)

	process_sleep(0x60);

	/* set decomp base to decomp area */
	idcomp_base=image_dcomp_buffer;

	p1_char=p2_char=winchar;					// set to correct winner
	victory_message();

	sound_module_load(SMOD_GAMEOVER);
	if ( f_music )
	{
		current_proc->a0=f_no_sfx;
		f_no_sfx=0;
#if JAP_VERSION==1
		trigger.mask=TRIGGER_VOLUME;
		trigger.volume=0x28;
		wess_seq_trigger_special(TUNE_GAMEOVER,&trigger);
#else
		tsound(0x91);				// play game over music
#endif
		f_no_sfx=current_proc->a0;
	}

	murder_myoinit_score();
	clr_scrn();
	process_sleep(8);
	f_shadow_fighting=0;

	end_of_tournament();
}

void end_of_tournament(void) 
{
	//-DHS- select_yer_ending();
	mk3_cast_o_characters();
	mk3_design_team(arcade_team_table);
	mk3_design_team(sony_team_table);
	mk3_thank_you();

	stack_jump(game_over);
}

void sk_die_tower(WORD pa10,WORD pa11h,WORD pa11l)
{
	init_and_multi_plane(BKGD_SKDIE1_MOD);
	shake_a11(pa11h,pa11l);
	process_sleep(pa10);
	return;
}

void sk_die_pit(WORD pa10,WORD pa11h,WORD pa11l)
{
	init_and_multi_plane(BKGD_SKPIT_MOD);
	CREATE(PID_FX,shao_kahn_dying2);
	shake_a11(pa11h,pa11l);
	process_sleep(pa10);
	return;
}

void init_and_multi_plane(WORD pa0)
{
	murder_myoinit_score();
	load_bkgd=pa0;
	init_background_module(table_o_mods[load_bkgd]);
	multi_plane();

	/* change to mock heap so we dont run out of memory */
	idcomp_base=(LONG *)mock_heap;

	return;
}

void rayman(void);

short ray_table[]=
{
	ANIM_SK_DEATHA1,0x3b,0xffb8,
	ANIM_SK_DEATHB1,0x38,0x2e,
	ANIM_SK_DEATHC1,0xff8c,0xffb6,
	ANIM_SK_DEATHD1,0xff5e,0x5b,
	ANIM_SK_DEATHE1,0xff60,0x10,
	ANIM_SK_DEATHF1,0x34,0x7b,
	ANIM_SK_DEATHG1,0xfff9,0xffba
};
void shao_kahn_dying2(void)
{
	WORD i;

	if (p1_char==FT_SK)
		setup_player_1();
	else setup_player_2();

	round_num++;


	/* create flying flames DEATHA-F */
	current_proc->a11=7;
	do
	{
		CREATE(PID_BANI,rayman)->a11=current_proc->a11-1;
	} while (--current_proc->a11>0);

	/* mskd5 */
	dallprc(PID_P1);
	dallprc(PID_P2);
	sk_die_setup();
	find_ani_part_a14(ANIM_SK_DIE,3);
	do_next_a9_frame(current_proc->pa8);
	mframew(5);

	wait_forever();
}

void rayman(void)
{
	OBJECT *obj;
	OBJECT *oa11=current_proc->pa8;
	WORD aninum=current_proc->a11;
	void *animframe;
	short *tbl;

	tbl=ray_table+(aninum<<1)+aninum;
	/* get frame */
	get_char_ani(ANIM_TABLE1,*tbl);
	find_part2();
	animframe=current_proc->pa9;
	animframe=(void*)COMPUTE_ADDR(oa11->oheap,GET_LONG(animframe));

	/* setup object */
	gso_dmawnz(obj,(ADDRESS)animframe,oa11->oheap,0);
	alloc_cache((OIMGTBL *)animframe,oa11->oheap,obj);
	obj->ozval=20+FRONT_Z;
	lineup_1pwm(obj,oa11);
	multi_adjust_xy(obj,*(tbl+1),*(tbl+2));
	insert_object(obj,&objlst);

	/* setup animation ptr */
	get_char_ani(ANIM_TABLE1,*tbl);
	find_part2();

	/* animate forever */
	framew(5);

	wait_forever();
}

void shao_kahn_dying(void)
{
	sk_die_setup();

	current_proc->pdata.p_flags|=PM_ALT_PAL;
	player_normpal();
	ground_player(current_proc->pa8);

	get_char_ani(ANIM_TABLE1,ANIM_SK_DIE);
	animate_a0_frames(0x5,0x7);

	find_ani_part2(ANIM_SK_DIE);
	mframew(5);
	wait_forever();
}

void sk_die_setup(void)
{
	clear_inviso(current_proc->pa8);
	(current_proc->pa8)->oxpos.u.intpos=SCRRGT/2+worldtlx.u.intpos;
	return;
}

void kahn_flash(WORD pa0,void *pa1)
{
	irqskye=pa0;
	swpal(pa1,p1_obj);
	swpal(pa1,p2_obj);
	process_sleep(4);
	return;
}

void sk_ray1(void)
{
	PROCESS *ptemp;

	ptemp=CREATE(PID_BANI,sk_ray_setup);
	ptemp->a11=ANIM_SK_DEATHA1;
	(short)(ptemp->pdata.p_store1)=0;	//0x3a;
	(short)(ptemp->pdata.p_store2)=0;	//(short)0xffb2;

	return;
}

void sk_ray2(void)
{
	PROCESS *ptemp;

	ptemp=CREATE(PID_BANI,sk_ray_setup);
	ptemp->a11=ANIM_SK_DEATHE1;
	(short)(ptemp->pdata.p_store1)=0;	//0x3a;
	(short)(ptemp->pdata.p_store2)=0;	//(short)0xffb2;

	return;

}

void sk_ray3(void)
{
	PROCESS *ptemp;

	ptemp=CREATE(PID_BANI,sk_ray_setup);
	ptemp->a11=ANIM_SK_DEATHC1;
	(short)(ptemp->pdata.p_store1)=0;	//0x3a;
	(short)(ptemp->pdata.p_store2)=0;	//(short)0xffb2;

	ptemp=CREATE(PID_BANI,sk_ray_setup);
	ptemp->a11=ANIM_SK_DEATHD1;
	(short)(ptemp->pdata.p_store1)=0;	//0x3a;
	(short)(ptemp->pdata.p_store2)=0;	//(short)0xffb2;

	return;
}

void sk_ray_setup(void)
{
	OBJECT *obj;
	OBJECT *oa11=current_proc->pa8;
	void *animframe;
	WORD aninum=current_proc->a11;

	/* setup first death ray animation */
	get_char_ani(ANIM_TABLE1,aninum);
	animframe=current_proc->pa9;
	animframe=(void*)COMPUTE_ADDR(oa11->oheap,GET_LONG(animframe));

	/* setup object */
	gso_dmawnz(obj,(ADDRESS)animframe,oa11->oheap,0);
	alloc_cache((OIMGTBL *)animframe,oa11->oheap,obj);
	obj->ozval=20+FRONT_Z;
	lineup_1pwm(obj,oa11);
	multi_adjust_xy(obj,(short)current_proc->pdata.p_store1,(short)current_proc->pdata.p_store2);
	insert_object(obj,&objlst);

	/* setup animation ptr */
	get_char_ani(ANIM_TABLE1,aninum);

	/* animate forever */
	framew(5);

	wait_forever();
}
//***********************************************************************
extern char *end_winner_text[];
void victory_message(void)
{
	(char *)current_proc->a0=get_winner_text();
	current_proc->pdata.p_store1=0x19;
	winner_msg_rise();
	process_sleep(0xa0);

	current_proc->a0=0;
	current_proc->a1=0x13;
	msg_rise();
	process_sleep(0xa0);

	current_proc->a0=1;
	current_proc->a1=0xe;
	msg_rise();
	process_sleep(0xf0);

	return;
}

void msg_rise(void)
{
	current_proc->pdata.p_store1=current_proc->a1;
	current_proc->a0=(ADDRESS)end_winner_text[current_proc->a0];
	winner_msg_rise();
	return;
}

extern FNTSETUP pf_rise[];
void winner_msg_rise(void)
{
	OBJECT *ta0,*ta5,*ta1,*ta2;

	lm_setup(pf_rise);
	mk_printf((char *)current_proc->a0);

	f_novel=1;

	set_objlst2_vel(-(0x80000));

	f_novel=0;
	process_sleep(current_proc->pdata.p_store1);

	f_novel=1;
	set_objlst2_vel(0);

	ta0=objlst2;
	/* rise4 */
	do
	{
		ta5=ta0;
		ta0=ta0->olink;
	}
	while(ta0!=NULL);

	ta1=objlst;
	ta2=objlst2;

	objlst=objlst2=NULL;

	ta5->olink=ta1;
	objlst=ta2;

	return;
}

void set_objlst2_vel(long pa1)
{
	OBJECT *oa0;

	oa0=objlst2;

	/* rise3 */
	do
	{
		oa0->oyvel.pos=pa1;
		oa0=oa0->olink;
	}
	while(oa0!=NULL);

	return;
}

/******************************************************************************
 Function: void mk3_cast_o_characters(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	introduce the cast!
******************************************************************************/
char txt_mk3_cast[]="MK3 CAST OF CHARACTERS";
extern void *cast_table[];
void mk3_cast_o_characters(void)
{
	void *ta11;
	OBJECT *obj;
	WORD ta11_p1,ta11_p2;
	OBJECT *obj1=NULL,*obj2=NULL;
	PROCESS *p1,*p2;

	DISPLAY_ON;
	murder_myoinit();
	DONT_SHOW_SCORES;

	do_background(BKGD_ROOF_MOD);

	p15_centered(txt_mk3_cast,SCX(0xc8),SCY(0x30));
	process_sleep(0x70);

	dallobj(OID_TEXT);
	process_sleep(0x10);

	ta11=cast_table;

	while (GET_LONG(ta11)!=0)
	{
		/* coc3, print text of character names */
		while (GET_LONG(ta11)!=0)
		{
			(ADDRESS)current_proc->pa8=GET_LONG(ta11)++;		// get txt
			(WORD)current_proc->a0=GET_WORD(ta11)++;			// get x pos
			(WORD)current_proc->a1=GET_WORD(ta11)++;			// get y pos
			pds_centered((char*)current_proc->pa8,(short)current_proc->a0,(short)current_proc->a1);
		}

		((ADDRESS *)ta11)++;				// skip zero termination entry

		/* coc7, get dudes out here */
		p1_char=GET_WORD(ta11)++;
		character_texture_load(p1_char,CHAR_NORMAL,p1_heap,ASYNC_LOAD);
		while ( psxcd_async_on() )
			process_sleep(1);
		module_os_close(module_int);			// make sure to close async file
		obj1=current_proc->pa8=make_player_obj(p1_char,SHADOW_MODE,PLAYER1_TYPE,p1_heap,0);
		(current_proc->pa8)->ochar=p1_char;
		get_player_palette(character_palettes_1);
		ta11_p1=GET_WORD(ta11)++;

		if (GET_WORD(ta11)==0xffff)
		{
			GET_LONG(ta11)++;				// dummy entry skip
		}
		else
		{
			p2_char=GET_WORD(ta11)++;
			character_texture_load(p2_char,CHAR_NORMAL,p2_heap,ASYNC_LOAD);
			while ( psxcd_async_on() )
				process_sleep(1);
			module_os_close(module_int);			// make sure to close async file
			obj2=current_proc->pa8=make_player_obj(p2_char,SHADOW_MODE,PLAYER2_TYPE,p2_heap,0);
			get_player_palette(character_palettes_1);
			(current_proc->pa8)->ochar=p2_char;
			ta11_p2=GET_WORD(ta11)++;
		}

		if ( obj1!=NULL )
		{
			current_proc->pa8=obj1;
			insert_object(current_proc->pa8,&objlst);
			p1=CREATE(PID_FX,take_a_bow);
			p1->a11=ta11_p1;
			obj1=NULL;
		}

		if ( obj2!=NULL )
		{
			current_proc->pa8=obj2;
			insert_object(current_proc->pa8,&objlst);
			p2=CREATE(PID_FX,take_a_bow);
			p2->a11=ta11_p2;
			obj2=NULL;
		}

		if ( p2!=NULL )
		{
			p1->pdata.p_otherguy=p2->pa8;
			p1->pdata.p_otherproc=p2;
			p2->pdata.p_otherguy=p1->pa8;
			p2->pdata.p_otherproc=p1;
		}
		else
		{
			p1->pdata.p_otherguy=p1->pa8;
			p1->pdata.p_otherproc=p1;
		}

		p1=p2=NULL;

		if ( GET_LONG(ta11)!=0 )
		{
			process_sleep(0x150);
			dallobj(OID_TEXT);
		}
		else break;
	}

	/* coc8 */
	process_sleep(0x50);
	MURDER;
	process_sleep(0x70);
	murder_myoinit_score();
	process_sleep(8);
	return;
}

void take_a_bow(void)
{
	OBJECT *obj=current_proc->pa8;

	current_proc->pdata.p_slave=NULL;

	ground_ochar();
	current_proc->pdata.p_ganiy=obj->oypos.u.intpos;
	obj->oypos.u.intpos+=0x90;
	obj->oxpos.u.intpos=worldtlx.u.intpos+(WORD)current_proc->a11;

	get_char_ani(ANIM_TABLE1,ANIM_JUMPUP);
	((ADDRESS *)current_proc->pa9)++;
	do_next_a9_frame(obj);

	obj->ograv.pos=0;
	obj->oyvel.pos=-SCY(0x90000);

	/* btf6 */
	do
	{
		process_sleep(1);
	}
	while(obj->oypos.u.intpos>current_proc->pdata.p_ganiy);

	/* tab7 */
	do
	{
		process_sleep(1);
		next_anirate();
		obj->oyvel.pos+=SCY(0xc000);
	}
	while(obj->oypos.u.intpos<current_proc->pdata.p_ganiy);

	tsound(0x18);			// feet land on ground
	ground_player(obj);
	stop_a8(obj);

	CREATE(PID_FX,wake_me_up)->a11=(ADDRESS)current_proc;		// pass my proc

	stack_jump(ending_victory_animation);
}

void wake_me_up(void)
{
	process_sleep(0xc0);
	fastxfer((PROCESS *)current_proc->a11,run_away);
	process_suicide();
}

void run_away(void)
{
	OBJECT *obj=current_proc->pa8;

	delete_slave();

	if ((obj->oflags & M_FLIPH)==0)
		flip_multi(obj);

	/* tab3 */
	if (obj->oxpos.u.intpos>=(worldtlx.u.intpos+SCRRGT/2))
	{
		(long)current_proc->a11=SCX(0x80000);
		flip_multi(obj);
	}
	else
	{
		(long)current_proc->a11=-SCX(0x80000);
	}

	/* rleft, exit stage right or left */
	get_char_ani(ANIM_TABLE1,ANIM_RUN);
	init_anirate(3);

	current_proc->a10=0x40;
	do
	{
		process_sleep(1);
		obj->oxvel.pos=(long)current_proc->a11;
		next_anirate();
	}
	while(--current_proc->a10>0);
	stop_a8(obj);
	delobjp(obj);
	process_suicide();
}
/******************************************************************************
 Function: void mk3_thank_you(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	thank all!
******************************************************************************/
extern char txt_execs[];
//extern char txt_dcs[];
extern char txt_ssr[];
extern char txt_voices[];
extern char txt_testers[];
extern char txt_thanx_alot[];
extern char txt_mk3_design[];
extern char txt_scea[];

THANKX mk3_thanx[]=
{
#if PAL_VERSION
	{txt_execs,0xd8*6/5},
	/*{txt_mk3_design,0x140},*/
	{txt_ssr,0x60*6/5},
	{txt_voices,0x160*6/5},
	{txt_testers,0x170*6/5},
	{txt_scea,0x60*6/5},
	{txt_thanx_alot,0x120*6/5},
	{(char *)0xffffffff,0}
#else
	{txt_execs,0xd8},
	/*{txt_mk3_design,0x140},*/
	{txt_ssr,0x60},
	{txt_voices,0x160},
	{txt_testers,0x170},
	{txt_scea,0x60},
	{txt_thanx_alot,0x120},
	{(char *)0xffffffff,0}
#endif
};

void mk3_thank_you(void)
{
	THANKX *ta11;

	clr_scrn();
	murder_myoinit_score();
	process_sleep(4);

	do_background(BKGD_TOWER_MOD);
	CREATE(PID_FX,text_deleter);


	ta11=mk3_thanx;
	/* des9 */
	do
	{
		if ( ta11->txt==(char *)0xffffffff)
			break;

		if ( ta11->txt!=NULL )
		{
			pds_centered_soff(ta11->txt,SCX(0xc8),(0xff));
			set_letter_vel();
		}

		/* des5 */
		process_sleep(ta11->sleep);
		ta11++;
	} while (1);

	/* des9 */
	process_sleep(0xc0);
	MURDER;

	fadeout_jsrp(10);

	return;
}

void set_letter_vel(void)
{
	OBJECT *oa0;
	OBJECT *oa3;
	OBJECT *oa4,*oa5;

	f_novel=1;

	oa0=objlst2;
	if ( oa0!=NULL )
	{
		oa3=oa0;
		objlst2=NULL;

		/* scrp4 */
		do
		{
			oa0->oyvel.pos=-(0x10000);
			oa0->ozval=100;
		} while ((oa0=oa0->olink)!=NULL);

		oa4=objlst;
		if (oa4!=NULL )
		{
			/* scrp5 */
			do
			{
				oa5=oa4;
				oa4=oa4->olink;
			} while (oa4!=NULL);
			oa5->olink=oa3;
		}
		else objlst=oa3;
	}

	/* slv9 */
	f_novel=0;

	return;
}

void text_deleter(void)
{
	OBJECT *oa0,*oa2;
	do
	{
		process_sleep(8);
		oa0=objlst;
		if ( oa0!=NULL )
		{
			/* objjd */
			do
			{
				oa2=oa0->olink;
				if ( oa0->oypos.u.intpos<-12 )
					delobjp(oa0);

				/* objjc */
				oa0=oa2;
			} while (oa0!=NULL  );
		}

	} while (1);
}

/******************************************************************************
 Function: void mk3_design_team(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	show the guys
******************************************************************************/
extern char txt_david[];
extern char txt_greg[];
extern char txt_markm[];
extern char txt_markg[];
extern char txt_rob[];
extern char txt_scott[];
extern char txt_ed[];
extern char txt_johnt[];
extern char txt_steve[];
extern char txt_dan[];
extern char txt_tonyg[];
extern char txt_dave[];
extern char txt_johnv[];

extern OIMGTBL a_DAVE;
extern OIMGTBL a_GREG;
extern OIMGTBL a_GUIDO;
extern OIMGTBL a_MARK;
extern OIMGTBL a_ROB;
extern OIMGTBL a_SCOTT;

extern OIMGTBL T_DFORDEN;
extern OIMGTBL T_MICHIC;
extern OIMGTBL T_JOHNV;
extern OIMGTBL T_BERAN;
extern OIMGTBL T_ED;
extern OIMGTBL T_GOSKIE;
extern OIMGTBL T_TOBIAS;
extern void *FACES_anims[];
char txt_mk3_dteam[]="MK3 SONY DEVELOPMENT TEAM";
char txt_mk_design[]="MORTAL KOMBAT 3 DESIGN TEAM";
WORD team_fade_in[]={0,0,8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128,0xffff};
DTEAM sony_team_table[]=
{
	{txt_markm,160,SCY(0xdd)+20},
	{NULL,0,0},
	{&a_MARK,0,0},
	{NULL,-1,-1},

	{txt_greg,160,SCY(0xdd)+20},
	{NULL,0,0},
	{&a_GREG,100,-6},
	{NULL,-1,-1},

	{txt_david,160,SCY(0xdd)+20},
	{NULL,0,0},
	{&a_DAVE,48,-22},
	{NULL,-1,-1},

	{txt_markg,160,SCY(0xdd)+20},
	{NULL,0,0},
	{&a_GUIDO,-57,-29},
	{NULL,-1,-1},

	{txt_scott,160,SCY(0xdd)+20},
	{NULL,0,0},
	{&a_SCOTT,79,-60},
	{NULL,-1,-1},

	{txt_rob,160,SCY(0xdd)+20},
	{NULL,0,0},
	{&a_ROB,4,-70},
	{NULL,-1,-1},

	{&a_ROB,-2,-2},

};

#define TEAM_X (SCX(0x6a)-76)
#define TEAM_Y (SCY(0x66)-80)

DTEAM arcade_team_table[]=
{
	{txt_ed,SCX(0x90),SCY(0xdd)+20},
	{txt_johnt,SCX(0x108),SCY(0xdd)+20},
	{&T_ED,TEAM_X+0,TEAM_Y+0},
	{&T_TOBIAS,TEAM_X+SCX(101),TEAM_Y+SCY(9)},
	{NULL,-1,-1},

	{txt_steve,SCX(0xc8),SCY(0xdd)+20},
	{NULL,0,0},
	{&T_BERAN,TEAM_X+SCX(46),TEAM_Y-SCY(19)},
	{NULL,-1,-1},

	{txt_dan,SCX(0xc8),SCY(0xdd)+20},
	{NULL,0,0},
	{&T_DFORDEN,TEAM_X-SCX(50),TEAM_Y-SCY(34)},
	{NULL,-1,-1},

	{txt_tonyg,SCX(0x90)-10,SCY(0xdd)+20},
	{txt_dave,SCX(0x108)+10,SCY(0xdd)+20},
	{&T_GOSKIE,TEAM_X+SCX(16),TEAM_Y-SCY(59)},
	{&T_MICHIC,TEAM_X+SCX(79),TEAM_Y-SCY(61)},
	{NULL,-1,-1},

	{txt_johnv,SCX(0xc8),SCY(0xdd)+20},
	{NULL,0,0},
	{&T_JOHNV,TEAM_X+SCX(154),TEAM_Y-SCY(42)},
	{NULL,-1,-1},

	{&a_ROB,-2,-2},

};

void mk3_design_team(DTEAM *dptr)
{
	OBJECT *oa0;
	WORD *pal;
	PROCESS *ptemp;

	clr_scrn();
	murder_myoinit_score();
	process_sleep(4);
	sony_graphics_setup();
	level_overlay_load(LVL_DTEAM);
	dlists=dlists_bogus;

	if ( dptr==arcade_team_table )
		pds_centered(txt_mk_design,SCX(0xc8),SCY(0x18));
	else pds_centered(txt_mk3_dteam,SCX(0xc8),SCY(0x18));

	oa0=objlst2;
	/* team0 */
	do
	{
		oa0->oid=OID_FX;
		oa0=oa0->olink;
	}
	while(oa0!=NULL );

	process_sleep(0x30);

	current_proc->a10=10;

	/* team2 */
	do
	{
		/* print text */
		pds_centered((char *)dptr->t1,dptr->x1,dptr->y1-8);
		dptr++;
		if ( dptr->t1!=NULL )
			pds_centered((char *)dptr->t1,dptr->x1,dptr->y1-8);
		dptr++;

		/* draw face */
		while ( dptr->x1!=-1 )
		{
			gso_dmawnz_ns(oa0,(ADDRESS)dptr->t1,FACES_anims,0);
			alloc_cache((OIMGTBL *)dptr->t1,&FACES_anims,oa0);
			animate(SINGLE_ANI,(OIMGTBL*)dptr->t1,oa0->oflags);
			oa0->ozval=current_proc->a10;
			current_proc->a10--;
			insert_object(oa0,&objlst);
			set_xy_coordinates(oa0,64+dptr->x1,104+dptr->y1-8);

			/* cool face fadein */
			ptemp=CREATE(PID_FADE,fadeproc);
			pal=dptr->t1;
			((OIMGTBL *)pal)++;
			pal=(WORD *)GET_LONG(pal);
			(WORD *)ptemp->a11=team_fade_in;
			(LONG)ptemp->pa9=findpal(pal);
			(WORD *)ptemp->pa8=pal;              // palette local

			dptr++;
		};

		tsound(0x8e);

		process_sleep(0x70);
		dallobj(OID_TEXT);
		dptr++;
	}
	while(dptr->x1!=-2);

	/* team9 */
	tsound(0x8e);
	process_sleep(0xc0);
	dallobj(OID_TEXT);
	process_sleep(0x20);

	murder_myoinit_score();
	clr_scrn();
	process_sleep(8);

	return;
}

