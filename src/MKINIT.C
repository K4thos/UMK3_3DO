/******************************************************************************
 File: mkinit.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Initialization and Main Loop Routines
******************************************************************************/
#define CHECKSUM 0

/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
//#include <libgs.h>
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkobj.h"
#include "mkos.h"
#include "mkfile.h"
#include "mkgame.h"
#include "mkinit.h"
#include "mkmain.h"
#include "mkutil.h"
#include "mkguys.h"
#include "mkani.h"
#include "mkjoy.h"
#include "mkdiag.h"
#include "mkamode.h"
#include "moves.h"
#include "mkreact.h"
#include "mkscore.h"
#include "mkbug.h"
#include "valloc.h"
#include "mktext.h"
#include "mkdiag.h"
#include "psxsnd.h"
#include "wessapi.h"
#include "mksound.h"
#include "mkpal.h"
#include "stdio.h"

extern WORD f_white;
WORD loading_pal;
extern WORD LOADING_P[];
extern FNTSETUP pf_15point_soff[];
typedef struct
{
	WORD pid;									/* process id */
	JUMPTBL proc_func;							/* function to excute */
} SWTAB;

static SWTAB swtab[64] =
{
	{0,NULL},								/* open 0 */
	{0,NULL},								/* open 1 */
	{PID_SWITCH1,p1_button5_open},			/* open 2, PLAYER 1 RUN */
	{PID_SWITCH1,p1_button2_open},			/* open 3, PLAYER 1 BLOCK */
	{PID_SWITCH1,p1_button0_open},			/* open 4, PLAYER 1 HI PUNCH */
	{PID_SWITCH1,p1_button3_open},			/* open 5, PLAYER 1 HI KICK */
	{PID_SWITCH1,p1_button4_open},			/* open 6, PLAYER 1 LOW KICK */
	{PID_SWITCH1,p1_button1_open},			/* open 7, PLAYER 1 LOW PUNCH */
	{0,NULL},								/* open 8 */
	{0,NULL},								/* open 9 */
	{0,NULL},								/* open a */
	{PID_SWITCH1,p1_start_open},			/* open b, clear pause thaw state */
	{0,NULL},								/* open c */
	{0,NULL},								/* open d */
	{0,NULL},								/* open e */
	{0,NULL},								/* open f */
	{0,NULL},								/* open 10 */
	{0,NULL},								/* open 11 */
	{PID_SWITCH2,p2_button5_open},			/* open 12, PLAYER 2 RUN */
	{PID_SWITCH2,p2_button2_open},			/* open 13, PLAYER 2 BLOCK */
	{PID_SWITCH2,p2_button0_open},			/* open 14, PLAYER 2 HI PUNCH */
	{PID_SWITCH2,p2_button3_open},			/* open 15, PLAYER 2 HI KICK */
	{PID_SWITCH2,p2_button4_open},			/* open 16, PLAYER 2 LOW KICK */
	{PID_SWITCH2,p2_button1_open},			/* open 17, PLAYER 2 LOW PUNCH */
	{0,NULL},								/* open 18 */
	{0,NULL},								/* open 19 */
	{0,NULL},								/* open 1a */
	{PID_SWITCH2,p2_start_open},			/* open 1b, clear pause thaw state */
	{0,NULL},								/* open 1c */
	{0,NULL},								/* open 1d */
	{0,NULL},								/* open 1e */
	{0,NULL},								/* open 1f */
	{PID_SWITCH1,p_profile},				/* close 20/0, poor mans profile */
	{PID_SWITCH1,p_grid},					/* close 21/1, grid */
	{PID_SWITCH1,p1_button5},				/* close 22/2, PLAYER 1 RUN */
	{PID_SWITCH1,p1_button2},				/* close 23/3, PLAYER 1 BLOCK */
	{PID_SWITCH1,p1_button0},				/* close 24/4, PLAYER 1 HI PUNCH */
	{PID_SWITCH1,p1_button3},				/* close 25/5, PLAYER 1 HI KICK */
	{PID_SWITCH1,p1_button4},				/* close 26/6, PLAYER 1 LOW KICK */
	{PID_SWITCH1,p1_button1},				/* close 27/7, PLAYER 1 LOW PUNCH */
	{PID_SWITCH1,p_colbox},					/* close 28/8, collision box */
	{PID_SWITCH1,p_profile},				/* close 29/9, poor mans profile */
	{PID_SWITCH1,p_grid},					/* close 2a/a, grid */
	{PID_SWITCH1,p1_start_button},			/* close 2b/b, PLAYER 1 START */
	{PID_SWITCH1,p1_joy_up},  				/* close 2c/c, PLAYER 1 JOYSTICK UP */
	{PID_SWITCH1,p1_joy_right},				/* close 2d/d, PLAYER 1 JOYSTICK RIGHT */
	{PID_SWITCH1,p1_joy_down},				/* close 2e/e, PLAYER 1 JOYSTICK DOWN */
	{PID_SWITCH1,p1_joy_left},				/* close 2f/f, PLAYER 1 JOYSTICK LEFT */
	{PID_SWITCH2,p_profile},				/* close 30/10, poor mans profile */
	{PID_SWITCH2,p_grid},					/* close 31/11, grid */
	{PID_SWITCH2,p2_button5},				/* close 32/12, PLAYER 2 RUN */
	{PID_SWITCH2,p2_button2},				/* close 33/13, PLAYER 2 BLOCK */
	{PID_SWITCH2,p2_button0},				/* close 34/14, PLAYER 2 HI PUNCH */
	{PID_SWITCH2,p2_button3},				/* close 35/15, PLAYER 2 HI KICK */
	{PID_SWITCH2,p2_button4},				/* close 36/16, PLAYER 2 LOW KICK */
	{PID_SWITCH2,p2_button1},				/* close 37/17, PLAYER 2 LOW PUNCH */
	{PID_SWITCH2,p_colbox},					/* close 38/18, collision box */
	{PID_SWITCH2,p_profile},				/* close 39/19, poor mans profile */
	{PID_SWITCH2,p_grid},					/* close 3a/1a, grid */
	{PID_SWITCH2,p2_start_button},			/* close 3b/1b, PLAYER 2 START */
	{PID_SWITCH2,p2_joy_up},  				/* close 3c/1c, PLAYER 2 JOYSTICK UP */
	{PID_SWITCH2,p2_joy_right},				/* close 3d/1d, PLAYER 2 JOYSTICK RIGHT */
	{PID_SWITCH2,p2_joy_down},				/* close 3e/1e, PLAYER 2 JOYSTICK DOWN */
	{PID_SWITCH2,p2_joy_left}				/* close 3f/1f, PLAYER 2 JOYSTICK LEFT */

};

extern ADDRESS *check_start;
extern ADDRESS *check_end;
LONG checkinit;
LONG checkcur;
ADDRESS checksum(void)
{
	ADDRESS value=0;
	ADDRESS *ptr;

	ptr=check_start;

	while ( ptr<check_end )
	{
		value+=*ptr++;
	}

	return(value);
}

extern void floor_code_setup(void);
WORD primmax=0;
extern WORD watchdog;

/******************************************************************************
 Function: void main(void)

 By: David Schwartz

 Date: Aug 8, 1994

 Parameters: None

 Returns: None

 Description:	This is were the game starts.
******************************************************************************/
void main(void)
{
	WORD swindex;
	WORD swpid;
	FUNC swproc_func;
	FUNC disp_func;
	ADDRESS *display_list;
	LONG *prnt;

#if PROFILE
	int vstart,vend;
#endif

POWERUP:

WARMBOOT:
	system_marker=0;

	df_fastrun=1;
	f_no_clock=1;

	CPRT("MORTAL KOMBAT III LIVES!");

#if MK_EJBBUG
	pmax=0;
#endif

#if COLLISION_BOX
	f_colbox=0;
#endif

//***************************************************************
//REMOVE SONY DEBUG PRINTF STATEMENTS FROM LIBRARIES, PRETTY LAME BUT WILL
//HOPEFULLY FIX THE CRASH BUG!!!!!
	prnt=(LONG *)printf;					// get start of printf function
	*prnt=0x03e00008;							// patch code jr ra
	*(prnt+1)=0x00000000;					// patch code nop
//***************************************************************

	ResetCallback();
	PadInit(0);											/* reset pad */
	//ResetGraph(0);
	//SetGraphDebug(0);

	/* Setup file system */
	module_io_init();
	psxcd_init();

	/* WMS Sound System Init */
	PsxSoundInit((char *)p1_heap);
	wess_master_sfx_vol_set(96);

	/* load sound stuff */
	sound_module_load(SMOD_SELECT);		// prime the pump

	VSync(0);			// init vscync stuff

	/* Initialize Game System and Stacks Switches */
	main_init();

	/* -DHS- determine what to do, activate hidden characters */
	f_mileena=1;
	f_ermac=1;
	f_oldsz=1;

MAIN_GO:
	system_marker=SYSTEM_INIT_MARKER;
	coinflag=0;							/* not on coinpage, yet */

	/* create attact mode process */
	CREATE(PID_AMODE,amode);

#if CHECKSUM
	checkinit=checksum();
#endif


	/* MAIN GAME LOOP (EXEC) */
	while (TRUE)
	{
/****************************************************************************
		 START VBLANK CODE STUFF (mkdisp.asm)
 ****************************************************************************/
		POLLING;

#if CHECKSUM
		if ( checkinit!=(checkcur=checksum()) )
		{
			LOCKUP;
		}
#endif

#if DEBUG
	if (cdb->primcnt>primmax)
		primmax=cdb->primcnt;
#endif
		/* perform page flipping if needed */
		cdb  = (cdb==db)? db+1: db;					/* swap double buffer ID */
		cdb->curr_prim=cdb->obj_prim;				/* setup prim ptr */
		cdb->primcnt=0;
		cdb->box_ptr=cdb->box_prim;					/* setup prim ptr for boxes */
		cdb->ot_ptr=cdb->ot+1;						/* set ot ptr */
	 	ClearOTag(cdb->ot+1, OTSIZE-1);				/* clear ordering table, leave clear primitive */

		/* setup image compression base, based on gstate */
		idcomp_ptr=idcomp_base;

		/* prof start */
#if PROFILE
		prof_start=GetRCnt(RCntCNT1);
		vstart=tick;
#endif


/****************************************************************************
		 START MAIN LOOP CODE STUFF (mkinit.asm)
 ****************************************************************************/
		/* unstack switch processes */
		current_proc=(PROCESS *)&active_head;
		while (swstack != swstmn)
		{
			if (free_head==NULL) break;				/* if no free processes save for next loop */

			swstack--;								/* pop stack */

			swindex=*swstack;						/* retrieve index */

			swproc_func=swtab[swindex].proc_func;	/* create process if not NULL */
			if (swproc_func != NULL && f_pause<PAUSE_OFF)
			{
				swpid=swtab[swindex].pid;
				CREATE(swpid,swproc_func);
			}
		}

		process_dispatch();

#ifdef MK_EJBBUG
		if (current_proc->plink != NULL)
			while (TRUE);
#endif /* MK_EJBBUG */

		/* do sound update */

		/* player bar stuff */
		update_bar(&p1_bar,&p1_bar_view);				/* update player 1 strength bar */
		raise_turbo_bar(&p1_turbo,&p1_turbo_view,&p1_dont_raise);

		update_bar(&p2_bar,&p2_bar_view);				/* update player 2 strength bar */
		raise_turbo_bar(&p2_turbo,&p2_turbo_view,&p2_dont_raise);

		position_p2_bar();

		/* joystick switch process */
		current_proc=(PROCESS *)&active_head;

/****************************************************************************
		 END MAIN LOOP CODE STUFF (mkinit.asm)
 ****************************************************************************/
		/* palette transfers */
 		transfer_palettes();

		/* routine to run every vblank */
		if (call_every_tick!=NULL)
			call_every_tick();

		if (f_pause<PAUSE_OFF || f_pause==PAUSE_P1_INIT || f_pause==PAUSE_P2_INIT)
		{
  			/* gravity & keep in bounds */
			if (p1_obj !=NULL)
				gravity_n_bounds(p1_obj,left_edge+LEFT_PADDING,right_edge+(SCRRGT-RIGHT_PADDING));

			if (p2_obj !=NULL)
				gravity_n_bounds(p2_obj,left_edge+LEFT_PADDING,right_edge+(SCRRGT-RIGHT_PADDING));

			/* update scrolling */
			worldtlx8.pos+=scrollx8.pos;
			worldtlx7.pos+=scrollx7.pos;
			worldtlx6.pos+=scrollx6.pos;
			worldtlx5.pos+=scrollx5.pos;
			worldtlx4.pos+=scrollx4.pos;
			worldtlx3.pos+=scrollx3.pos;
			worldtlx2.pos+=scrollx2.pos;
			worldtlx1.pos+=scrollx1.pos;

			worldtlx.pos+=scrollx.pos;
			worldtly.pos+=scrolly.pos;

		/* setup floor buffers */
		floor_code_setup();
		}

#if CD_DEBUG
		f_cdbug=1;
		dallobj(OID_CD);
		lm_setup(pf_15point_soff);
		fnt_state.fnt_posy=96;
		fnt_state.fnt_posx=32;
		printf_p1("%d   ",cdl_intr);
		fnt_state.fnt_posy=120;
		fnt_state.fnt_posx=32;
		printf_p1("%d   ",cdl_stat);
		fnt_state.fnt_posy=144;
		fnt_state.fnt_posx=32;
		printf_p1("%d   ",cdl_com);
		fnt_state.fnt_posy=96;
		fnt_state.fnt_posx=232;
		printf_p1("%d   ",cdl_errcount);
		fnt_state.fnt_posy=120;
		fnt_state.fnt_posx=232;
		printf_p1("%d   ",cdl_errintr);
		fnt_state.fnt_posy=144;
		fnt_state.fnt_posx=232;
		printf_p1("%d   ",cdl_errstat);
		fnt_state.fnt_posy=168;
		fnt_state.fnt_posx=232;
		printf_p1("%d   ",cdl_errcom);
		fnt_state.fnt_posy=192;
		fnt_state.fnt_posx=32;
		printf_p1("%d   ",g_place);
		fnt_state.fnt_posy=192+24;
		fnt_state.fnt_posx=32;
		printf_p1("%d   ",g_marker);
		f_cdbug=0;
#endif

		/* transfer display lists to primitives */
		if (dlists != NULL)
		{
			dlists_ptr=dlists;

			disp_world.u.ypos=worldtly.u.intpos;

			while ((display_list=(*((ADDRESS **)dlists_ptr)++)) != 0)
			{
				if ((long)display_list==-1)
				{
					disp_func=(FUNC)GET_LONG(dlists_ptr)++;			// get func to execute
					disp_func();
				}
				else
				{
					disp_world.u.xpos=*((WORD *)(GET_LONG(dlists_ptr)++));

					display_object_lists(&disp_world,(OBJECT *)display_list);
				}
			}
		}

		if (f_doscore!=0)
			update_control_panel();


//******************************************************************
	/* display bogus NOW LOADING Message */
	if ( f_load )
	{
		switch ( f_load )
		{
			case 1: //VS
				setXYWH(cdb->curr_prim,114,50,92,14);
				SetUVWH(cdb->curr_prim,92,LOADING_BASE_Y+30,92,14);
				break;
			case 4: //LADDER
				setXYWH(cdb->curr_prim,114,79,92,14);
				SetUVWH(cdb->curr_prim,92,LOADING_BASE_Y+30,92,14);
				break;
			case 2:	// SELECT
				setXYWH(cdb->curr_prim,114,SCY(0xec)+12,92,14);
				SetUVWH(cdb->curr_prim,92,LOADING_BASE_Y+30,92,14);
				break;
			case 3: // MAIN
				setXYWH(cdb->curr_prim,83,108,153,23);
				SetUVWH(cdb->curr_prim,92,LOADING_BASE_Y,153,23);
				break;
		}

		cdb->curr_prim->clut=get_fore_pal(LOADING_P);	//loading_pal;
		cdb->curr_prim->tpage=load_tpage;
		SetSemiTrans(cdb->curr_prim,0);
		cdb->primcnt++;
		AddPrim(cdb->ot_ptr,cdb->curr_prim);
		cdb->curr_prim++;						/* next prim */
	}
//******************************************************************

#if DEBUG_GRID
	  		/* Debug Grid */
		if (f_gridon!=0)
			debug_grid();
#endif

#if PROFILE
		prof_draw_start=prof_end=GetRCnt(RCntCNT1);
#endif

		DrawSync(0);									// wait for everything to finish


#if PROFILE
		prof_draw_end=GetRCnt(RCntCNT1);
		vend=tick-vstart;
		prof_draw_end+=(vend*240);
		profiler();
#endif


		/* Object Sort Routine, synced to irq_timer needed here */
		do
		{
			yzsort();
		}
		while (irq_timer==0);


		if (noflip==0)
		{
			PutDrawEnv(&cdb->draw); 		/* update drawing environment */
			PutDispEnv(&cdb->disp); 		/* update display environment */
		}

//    	setRGB0(&cdb->draw, GET_RED(irqskye)*8, GET_GREEN(irqskye)*8, GET_BLUE(irqskye)*8); /* set background color */
	  	setRGB0(&cdb->clear, GET_RED(irqskye)*8, GET_GREEN(irqskye)*8, GET_BLUE(irqskye)*8); /* set background color */

		if ( f_white==1 )
		{
			/* generate a white screen */
			DrawPrim((LONG *)&cdb->clear);
		}
		else
		{
//			cdb->draw.isbg=f_auto_erase;

			if (f_auto_erase)
				DrawOTag(cdb->ot);				/* draw */
			else DrawOTag(cdb->ot+1);			/* draw w/no clear */
		}
	}

	return;
}

/******************************************************************************
 Function: void update_bar(WORD *bar,WORD *bar_view,WORD *bram,WORD type)

 By: David Schwartz

 Date: Aug 8, 1994

 Parameters: *bar - ptr to player bar real strength variable
			 *bar_view - ptr to player bar strength view variable
			 *bram = ptr to player dont raise turbo flag
			  type = 1=raise_turbo bar 0=update_bar

 Returns: None

 Description:	update player's strength bar
******************************************************************************/
void update_bar_common(WORD *bar,WORD *bar_view,WORD *bram,WORD type)
{
	if ( f_pause>=PAUSE_OFF )
		return;

	/* raise_turbo_bar */
	if (type)
	{
		if (*bram)
		{
			(*bram)--;
		}
		else
		{
			(*bar)++;
			if (*bar>FULL_TURBO)
				*bar=FULL_TURBO;
		}
	}

	/* update bar */
	if (*bar & 0x8000)	return;					/* neg means effect going on, don't touch */

	if (*bar != 0)
	{
		/* upsb1, real power > 0 */
		if (*bar_view != *bar)
		{
			/* upsb4 */
			if (*bar_view<*bar)
				(*bar_view)++;			// viewable is smaller --> add
			else (*bar_view)--;			// viewable is biggger --> sub
		}

		/* upsb3 */
		if (*bar_view<3)
			*bar_view=3;
	}
	else
	{
		/* real power = 0 */
		if (*bar_view==0)					// viewable = 0 ---> stuff
			return;

		if ((--(*bar_view))<3)				// drop bar towards 0
			*bar_view=0;					// view<3 then zero it

		return;
	}
}

/******************************************************************************
 Function: void main_init(void)

 By: David Schwartz

 Date: Aug 8, 1994

 Parameters: None

 Returns: None

 Description:	Main system initialize routine.
******************************************************************************/
void main_init(void)
{
	BLK_FILL clr;

#ifdef SONY_PSX
	init_sony_system();					/* init game platform */
#endif

	idcomp_base=image_dcomp_buffer;		/* init decompression buffer base */

	swstack=swstmn;						/* initialize switch stack */
	curback=0;							/* set current background to 0 */
	text_draw=0;

	/* clear overlay stuff */
	load_bkgd=0xffff;					// set to nada loaded
	load_level=LVL_NONE;				// no data loaded at level_loadaddr

	/* clear screen */
	SetBlockFill(&clr);
	setRGB0(&clr,0,0,0);
	setXY0(&clr,BUF0_X_COORD,BUF0_Y_COORD);
	setWH(&clr,BUFFER_WIDTH,BUFFER_HEIGHT);
	DrawPrim((u_long *)&clr);

	SetBlockFill(&clr);
	setRGB0(&clr,0,0,0);
	setXY0(&clr,BUF1_X_COORD,BUF1_Y_COORD);
	setWH(&clr,BUFFER_WIDTH,BUFFER_HEIGHT);
	DrawPrim((u_long *)&clr);

	randomize(GetRCnt(RCntCNT1),GetRCnt(RCntCNT1));

#if CD_DEBUG
	g_marker=0;
	g_place=0;
#endif

	POLLING;

	dipinit();							/* init dipswitch settings */

	process_initialize();				/* init process list */

	object_initialize();				/* init object list */

	/* load permanent art into VRAM */
	load_perm_art();
	f_load=0;
	p1_heap_char=p2_heap_char=0xffff;
	f_shang_morph=MORPH_QUASI;

#ifdef SONY_PSX
//	EnterCriticalSection();
//	EnableEvent(vbEvent);
//	ExitCriticalSection();

	DISPLAY_ON;
#endif

	return;
}

/******************************************************************************
 Function: void dipinit(void)

 By: David Schwartz

 Date: Aug 8, 1994

 Parameters: None

 Returns: None

 Description:	Routine to initialize system according to dip switches (SECRET CODE SETTINGS)
******************************************************************************/
void dipinit(void)
{
	f_no_violence=0;
	f_no_blood=0;
	diff=4;
	cmos_diff=2;
	f_freeplay=0;					// no free play
	f_no_clock=0;
	f_smoke=0;
	f_cheat_menu=0;
	df_nopower=0;
	f_unlim_fatal=0;
	f_level_select=0;
	f_one_win=0;
	f_shang_morph=MORPH_QUASI;
	credits=GAME_CREDITS;
	f_no_sfx=0;
	f_music=2;
#if JAP_VERSION
	f_voice=1;			// voice effects on
#endif
	f_no_pan=0;				// pan normal (OFF)
	f_stereo=1;				// stero on
	f_no_vs=0;
	f_blast=0;

	/* configure sound driver */
	sound_driver_config();

	return;
}

/******************************************************************************
 Function: void wipeout(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description: wipes the system clear of all other processes, objects	 	*
	  		and coordinates.						 			*
	  		it returns with the display system and auto-erase on,		*
	  		color ram cleared, and the bit map wiped clean.		 	*
	  		this is nice to use when switching between stuff.		 	*
******************************************************************************/
void wipeout(void)
{
	WORD pa0;

	coinflag=on_hstd=0;

	process_kill_class(0,0x8000);			// kill all procs except coin

	object_initialize();		// reinit object lists

	pa0=tick;
	while (tick==pa0);			// wait for new vblank

	clr_scrn();
	process_sleep(1);
	clr_scrn();
	process_sleep(1);
	irqskye=0;
	DISPLAY_ON;

	return;
}

/******************************************************************************
 Function: void p[1|2]_button[0..4][_open]

 By: David Schwartz

 Date: Aug 8, 1994

 Parameters: None

 Returns: None

 Description:	Various player switch processes.  These functions are expected to
				commit suicide at the end.  They will never return from the queue
				and jump routine!!!!!
******************************************************************************/
void p1_button0_open(void)
{
	queue_and_jump(SW_HI_PUNCH,p1_boq,PLAYER1_TYPE,SW_OPEN);
	process_suicide();
}

void p1_button1_open(void)
{
	queue_and_jump(SW_LO_PUNCH,p1_boq,PLAYER1_TYPE,SW_OPEN);
	process_suicide();
}

void p1_button2_open(void)
{
	queue_and_jump(SW_BLOCK,p1_boq,PLAYER1_TYPE,SW_OPEN);
	process_suicide();
}

void p1_button3_open(void)
{
	queue_and_jump(SW_HI_KICK,p1_boq,PLAYER1_TYPE,SW_OPEN);
	process_suicide();
}

void p1_button4_open(void)
{
	queue_and_jump(SW_LO_KICK,p1_boq,PLAYER1_TYPE,SW_OPEN);
	process_suicide();
}

void p1_button5_open(void)
{
	queue_and_jump(SW_RUN,p1_boq,PLAYER1_TYPE,SW_OPEN);
	process_suicide();
}

void p2_button0_open(void)
{
	queue_and_jump(SW_HI_PUNCH,p2_boq,PLAYER2_TYPE,SW_OPEN);
	process_suicide();
}

void p2_button1_open(void)
{
	queue_and_jump(SW_LO_PUNCH,p2_boq,PLAYER2_TYPE,SW_OPEN);
	process_suicide();
}

void p2_button2_open(void)
{
	queue_and_jump(SW_BLOCK,p2_boq,PLAYER2_TYPE,SW_OPEN);
	process_suicide();
}

void p2_button3_open(void)
{
	queue_and_jump(SW_HI_KICK,p2_boq,PLAYER2_TYPE,SW_OPEN);
	process_suicide();
}

void p2_button4_open(void)
{
	queue_and_jump(SW_LO_KICK,p2_boq,PLAYER2_TYPE,SW_OPEN);
	process_suicide();
}

void p2_button5_open(void)
{
	queue_and_jump(SW_RUN,p2_boq,PLAYER2_TYPE,SW_OPEN);
	process_suicide();
}

void p1_button0(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_HI_PUNCH,p1_bcq);

	queue_and_jump(SW_HI_PUNCH,p1_bcq,PLAYER1_TYPE,SW_CLOSE);
	process_suicide();
}

void p1_button1(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_LO_PUNCH,p1_bcq);

	queue_and_jump(SW_LO_PUNCH,p1_bcq,PLAYER1_TYPE,SW_CLOSE);
	process_suicide();
}

void p1_button2(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_BLOCK,p1_bcq);

	queue_and_jump(SW_BLOCK,p1_bcq,PLAYER1_TYPE,SW_CLOSE);
	process_suicide();
}

void p1_button3(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_HI_KICK,p1_bcq);

	queue_and_jump(SW_HI_KICK,p1_bcq,PLAYER1_TYPE,SW_CLOSE);
	process_suicide();
}

void p1_button4(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_LO_KICK,p1_bcq);

	queue_and_jump(SW_LO_KICK,p1_bcq,PLAYER1_TYPE,SW_CLOSE);
	process_suicide();
}

void p1_button5(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_RUN,p1_bcq);

	queue_and_jump(SW_RUN,p1_bcq,PLAYER1_TYPE,SW_CLOSE);
	process_suicide();
}

void p2_button0(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_HI_PUNCH,p2_bcq);

	queue_and_jump(SW_HI_PUNCH,p2_bcq,PLAYER2_TYPE,SW_CLOSE);
	process_suicide();
}

void p2_button1(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_LO_PUNCH,p2_bcq);

	queue_and_jump(SW_LO_PUNCH,p2_bcq,PLAYER2_TYPE,SW_CLOSE);
	process_suicide();
}

void p2_button2(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_BLOCK,p2_bcq);

	queue_and_jump(SW_BLOCK,p2_bcq,PLAYER2_TYPE,SW_CLOSE);
	process_suicide();
}

void p2_button3(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_HI_KICK,p2_bcq);

	queue_and_jump(SW_HI_KICK,p2_bcq,PLAYER2_TYPE,SW_CLOSE);
	process_suicide();
}

void p2_button4(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_LO_KICK,p2_bcq);

	queue_and_jump(SW_LO_KICK,p2_bcq,PLAYER2_TYPE,SW_CLOSE);
	process_suicide();
}

void p2_button5(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_RUN,p2_bcq);

	queue_and_jump(SW_RUN,p2_bcq,PLAYER2_TYPE,SW_CLOSE);
	process_suicide();
}

void p1_start_button(void)
{
	start_entry(&p1_state,0);
}

void p1_joy_up(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_UP,p1_jcq);

	queue_and_jump(SW_UP,p1_jcq,PLAYER1_TYPE,SW_CLOSE);
}

void p1_joy_right(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_RIGHT,p1_jcq);

	queue_and_jump(SW_RIGHT,p1_jcq,PLAYER1_TYPE,SW_CLOSE);
}

void p1_joy_down(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_DOWN,p1_jcq);

	queue_and_jump(SW_DOWN,p1_jcq,PLAYER1_TYPE,SW_CLOSE);
}

void p1_joy_left(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_LEFT,p1_jcq);

	queue_and_jump(SW_LEFT,p1_jcq,PLAYER1_TYPE,SW_CLOSE);
}

void p2_start_button(void)
{
	start_entry(&p2_state,1);
}

void p2_joy_up(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_UP,p2_jcq);

	queue_and_jump(SW_UP,p2_jcq,PLAYER2_TYPE,SW_CLOSE);
}

void p2_joy_right(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_RIGHT,p2_jcq);

	queue_and_jump(SW_RIGHT,p2_jcq,PLAYER2_TYPE,SW_CLOSE);
}

void p2_joy_down(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_DOWN,p2_jcq);

	queue_and_jump(SW_DOWN,p2_jcq,PLAYER2_TYPE,SW_CLOSE);
}

void p2_joy_left(void)
{
	if (gstate==GS_AMODE)
		home_code_que(SW_LEFT,p2_jcq);

	queue_and_jump(SW_LEFT,p2_jcq,PLAYER2_TYPE,SW_CLOSE);
}

void p1_start_open(void)
{
	if (f_pause==PAUSE_P1_THAW)
		f_pause=0;

	process_suicide();
}

void p2_start_open(void)
{
	if (f_pause==PAUSE_P2_THAW)
		f_pause=0;

	process_suicide();
}

void p_grid(void)
{
	process_suicide();
#if DEBUG_GRID
	f_gridon^=1;
#else
	f_gridon=0;
#endif
	process_suicide();
}

void p_profile(void)
{
#if 1
#if PROFILE
	prof_max=0;
	f_profile^=1;
#else
	f_profile=0;
#endif
	process_suicide();
#endif
}

void p_colbox(void)
{
	//p2_bar=0;
	//process_suicide();

#if SCREEN_SHOTS
	while (!((swcurr.u.p1pad & P1_BUTTON_MASK) || (swcurr.u.p2pad & P2_BUTTON_MASK)))
		POLLING;

	process_suicide();
#endif

#if COLLISION_BOX
	kilobj2(OID_CBOX,0xffff);
	f_colbox^=1;
#else
	f_colbox=0;
#endif
	process_suicide();
}

WORD *last_switch_ram[]=
{
	l_hp,c_hp,			// 0
	l_lp,c_lp,	   		// 1
	l_block,c_block,	// 2
	l_hk,c_hk,			// 3
	l_lk,c_lk,			// 4
	l_run,c_run,		// 5
	l_up,c_up,			// 6
	l_down,c_down,		// 7
	l_left,c_left,		// 8
	l_right,c_right 	// 9
};

/******************************************************************************
 Function: queue_and_jump

 By: David Schwartz

 Date: Aug 8, 1994

 Parameters: WORD swoffset - switch offset (0-8) (a0)
			 LONG *b_queue - array button queue to queue switch offset (a1)
			 WORD poffset - player offset (0-1) (a2)
			 WORD swtype - open/close (0/1) (a3)

 Returns: process will terminate in suicide

 Description:	0)	determine if player active, if not process commits suicide
 				1)	queues up a switch equate
				2)	loads up a player's info into process ram
				3)	compute jump of switch address and jump
				4)	if returns then commit suicide
******************************************************************************/
void queue_and_jump(WORD swoffset,LONG *b_queue,WORD poffset,WORD swtype)
{
	PROCESS *ptemp;
	WORD *pbutton;
	JUMPTBL *sw_ptr;

	/* check if player active otherwise ignore input */
	if (poffset==PLAYER1_TYPE)
		pdie2(p1_state,0,swtype,swoffset);
	else pdie2(p2_state,1,swtype,swoffset);

	switch_queue(swoffset,b_queue);			/* queue switch */

	/* load info based on player type */
	ptemp=current_proc;
	ptemp->a0=(ADDRESS)swoffset;
	ptemp->a2=(ADDRESS)poffset;

	if (poffset==PLAYER1_TYPE)
	{
		ptemp->pdata.p_otherguy=p2_obj;

		ptemp->pdata.p_otherproc=p2_proc;

		(ADDRESS)ptemp->pdata.p_store4=(ADDRESS)p1_proc;

		ptemp->pdata.p_joyport=p1_proc->pdata.p_joyport;

		ptemp->pa8=p1_obj;
	}
	else
	{
		ptemp->pdata.p_otherguy=p1_obj;

		ptemp->pdata.p_otherproc=p1_proc;

		(ADDRESS)ptemp->pdata.p_store4=p2_proc;

		ptemp->pdata.p_joyport=p2_proc->pdata.p_joyport;

		ptemp->pa8=p2_obj;
	}

	if (swtype==SW_CLOSE)
	{
		/* button table jump?? */
		if (poffset==PLAYER1_TYPE)
		{
			pbutton=p1_button;
			ptemp=p1_proc;
		}
		else
		{
			pbutton=p2_button;
			ptemp=p2_proc;
		}

		if ((*(pbutton+swoffset)) != JI_JOY_IGNORE)
		{
			ptemp->joyindex=*(pbutton+swoffset);				// set joystick routine index var
			fastxfer(ptemp,joy_proc);								// transfer player to start of joystick routine
		}

		/* update "time since last" ram */
		if (poffset==PLAYER1_TYPE)
		{
			/* player 1 update */
			pbutton=*(last_switch_ram+swoffset*2);		// last ram
			*pbutton=tick;

			pbutton=*(last_switch_ram+swoffset*2+1);	// combo ram
			*pbutton=tick;

		}
		else
		{
			/* player 2 update */
			pbutton=(*(last_switch_ram+swoffset*2))+1;		// last ram
			*pbutton=tick;

			pbutton=(*(last_switch_ram+swoffset*2+1))+1;	// combo ram
			*pbutton=tick;
		}
	}

	/* qaj6 */
	if (!f_no_special_moves)
	{
		if (swtype==SW_CLOSE)
			sw_ptr=*(switch_close_jumps+swoffset);
		else sw_ptr=*(switch_open_jumps+swoffset);

		if (*(sw_ptr+(current_proc->pa8)->ochar) != NULL)
			(*(sw_ptr+(current_proc->pa8)->ochar))();		// address to jump to
	}

	process_suicide();						/* kill self if every gets back to this point */
}

/******************************************************************************
 Function: switch_queue(WORD swoffset,LONG *bqueue)

 By: David Schwartz

 Date: Aug 8, 1994

 Parameters: WORD swoffset - switch offset (0-8)
			 LONG b_queue - array button queue to queue switch offset

 Returns: NONE

 Description:	adds a switch entry to the current queue
******************************************************************************/
void switch_queue(WORD swoffset,LONG *b_queue)
{
	LONG	index;

	index=b_queue[0];								/* next entry */
	b_queue[index++]=(LONG)((swoffset<<16)|tick);	/* stuff info, [switch,time] */

	if (index>=SQS+1) index=1;						/* update index, if end set to start */
	b_queue[0]=index;

	return;
}

/******************************************************************************
 Function: void shadow_p1p2(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	display shadows for player 1 and player 2
******************************************************************************/
void shadow_p1p2(void)
{
	if (f_shadows==1)							// shadows allowed?
	{
		shadow_code(p1_obj,p1_shadadj);

		shadow_code(p2_obj,p2_shadadj);

		cdb->ot_ptr++;								/* next priority level */
	}
}

/******************************************************************************
 Function: void pdie2(WORD pb0,WORD pb1,WORD pa3)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pb0 - pstate
			 pb1 - PLAYER1 =0 PLAYER2=1
			 pa3 - switch open/close (0/1) (a3)

 Returns: None

 Description:	routine to die if the player who hit the button is not in game
******************************************************************************/
void pdie2(WORD pb0,WORD pb1,WORD pa3,WORD swoffset)
{
	WORD *pbutton;

	if (gstate==GS_UNLOCK_HID)
		goto VS_SCREEN_BUTTON;

	if (pb0!=PS_ACTIVE)
		process_suicide();

	if (gstate==GS_VS)
	{
VS_SCREEN_BUTTON:

		if (pa3!=0)
		{
			pbutton=(pb1==PLAYER1_TYPE) ? *(last_switch_ram+swoffset*2):(*(last_switch_ram+swoffset*2))+1;	// "last ram to update "
			*pbutton=1;
		}

		process_suicide();
	}
	else
	{
		if (gstate!=GS_FIGHTING)
			process_suicide();
	}

	return;
}
