/******************************************************************************
 File: mkopt.c

 Date: April 1995

 (C) Williams Entertainment

 Mortal Kombat III SONY OPTION SCREENS ROUTINES
******************************************************************************/

/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif

#include "mkbkgd.h"
#include "mkobj.h"
#include "mkos.h"
#include "mkgame.h"
#include "mkutil.h"
#include "psxcd.h"
#include "mkani.h"
#include "mkopt.h"
#include "mkinit.h"
#include "mkjoy.h"
#include "valloc.h"
#include "mktext.h"
#include "mkdiag.h"
#include "mkfade.h"
#include "mkguys.h"
#include "mkpal.h"
#include "mksound.h"
#include "mkfile.h"

/* local externs */
extern void *a_opt_dragon[];
extern void *a_logo[];
extern void *a_credit[];
extern void *options_anims[];
extern void *POLE01[];
extern void *FRAME01[];

extern FNTSETUP pf_dave_smallc[];
extern FNTSETUP pf_15point[];

extern WORD YELLOW_p[];
extern WORD WHITE_p[];
extern WORD BLUE_p[];
extern WORD optextbg_PAL[];

extern void *options_module[];
extern void *optcoin_module[];

OPTPAGE option_text_list[]=
{
	{0,0,90,13},					// 0 - violence level
	{152,0,26,13},					// 1 - on
	{179,0,35,15},					// 2 - off
	{0,13,60,13},					// 3 - blood
	{61,13,107,15},					// 4 - difficulty
	{168,15,74,14},					// 5 - normal
	{0,28,100,14},					// 6 - very easy
	{54,28,46,14},					// 7 - easy
	{101,29,106,14},				// 8 - very hard
	{155,29,52,14},					// 9 - hard
	{208,29,39,14},					// a - rgb
	{0,42,44,13},					// b - ntsc
	{44,43,54,13},					// c - mono
	{104,43,96,14},					// d - stereo
	{213,43,43,13},					// e - exit
	{0,57,232,14},					// f - master vol control
	{0,71,102,13},					// 10 - music test
	{0,71,54,13},					// 11 - music
	{102,71,59,13},					// 12 - sound
	{102,71,109,13},				// 13 - sound test
	{0,84,88,15},					// 14 - sound fx
	{89,84,125,14},					// 15 - pan control
	{0,99,137,14},					// 16 - monitor type
	{137,98,73,9},					// 17 - sound test
	{137,107,71,9},					// 18 - music test
	{0,113,63,13},					// 19 - silent
	{63,113,59,13},					// 1a - quiet
	{124,118,69,14},				// 1b - rockin'
	{0,126,97,14},					// 1c - kranked!
	{172,132,79,10}, 				// 1d - high punch
	{61,144,71,10},				// 1e - lo punch
	{103,132,69,10},			// 1f - hi kick
	{0,144,61,9},					// 20 - lo kick
	{172,144,27,10},					// 21 - run
	{132,144,40,10},					// 22 - block
	{225,85,13,13},					// 23 - square
	{240,85,13,13},					// 24 - tri
	{240,71,13,13},					// 25 - circle
	{225,71,13,13},					// 26 - x
	{225,113,13,13},					// 27 - l1
	{240,113,13,13},					// 28 - l2
	{225,99,13,13},					// 29 - r1
	{240,99,13,13},					// 2a - r2
	{0,154,94,10},						// 2b - not assigned
	{97,154,135,14},					// 2c - shang morph
	{0,164,90,14},						// 2d - disabled
	{0,192,99,13},						// 2e - opponent
	{97,168,33,14},					// 2f - all
	{130,168,54,13},					// 30 - clock
	{103,182,102,15},					// 31 - free play
	{184,168,61,13},					// 32 - smoke
	{0,178,86,14},						// 33 - enabled
	{140,100,33,14},					// 34 - l/r
	{178,100,30,14},					// 35 - r/l
	{102,197,141,15},					// 36 - fataltity time
	{112,212,144,14},					// 37 - l round match
	{0,212,112,14},						// 38 - 1 hit death
	{0,226,127,13},							// 39 - level select
	{128,226,96,14},									// 3a - vs screen
	{66,240,54,13}									// 3b - voice
};


/***************************************************
 Joystick Remap Setup and Convert Tables
***************************************************/

OPTMAP opt_p1bridge[]=
{
	{B_TRI,&pad_tri[0]},
	{B_X,&pad_x[0]},
	{B_CIRCLE,&pad_circle[0]},
	{B_SQUARE,&pad_square[0]},
	{B_L1,&pad_l1[0]},
	{B_L2,&pad_l2[0]},
	{B_R1,&pad_r1[0]},
	{B_R2,&pad_r2[0]},
	{0,NULL}
};

OPTMAP opt_p2bridge[]=
{
	{B_TRI,&pad_tri[1]},
	{B_X,&pad_x[1]},
	{B_CIRCLE,&pad_circle[1]},
	{B_SQUARE,&pad_square[1]},
	{B_L1,&pad_l1[1]},
	{B_L2,&pad_l2[1]},
	{B_R1,&pad_r1[1]},
	{B_R2,&pad_r2[1]},
	{0,NULL}
};

/******************************************************************************
 Function: void opt_main(void)

 By: David Schwartz

 Date: March 1995

 Parameters: None

 Returns: None

 Description:	main routine for the options
******************************************************************************/
extern void sony_graphics_setup(void);
extern void *option_text[];
void opt_main(void)
{
	OBJECT *obj;
	PROCESS *ptemp;
	PROCESS *pcube;

	current_proc->pdata.p_store5=gstate;		// save current state so we can restore it

	gstate=GS_OPTIONS;					// set game state so that we don't have multiple copies runned

	murder_myoinit();
	f_doscore=0;
	//file_load_message();

	/* load background textures so that no disk access */
	load_bkgd=BKGD_MK3_OPTION;
	sony_graphics_setup();
	bkgd_texture_load(bkgd_base_x,BACK_TEXTURE_BASE_Y);

	/* load texture graphics overlay for options */
	generic_overlay_load(OVL_OPTIONS);
	option_tpage=GetTPage(TEXTURE_MODE,TRANS_RATE,bkgd_base_x+128+128+128,BACK_TEXTURE_BASE_Y);		// -NOTE- ART FILES CHANGES MAKE SURE UPDATE TPAGE LOCATION

	/* generic sound effects load */
	sound_module_load(SMOD_SELECT);
	//play_generic_tune(TRK_OPT_ATTRACT);
	psxcd_play_at_andloop(TRK_OPT_ATTRACT,cd_vol_tbl[f_music],2,0,
												TRK_OPT_ATTRACT,cd_vol_tbl[f_music],2,3000);

	f_load=0;

	while (TRUE)
	{
		DISPLAY_OFF;
		murder_myoinit_score();

		load_bkgd=BKGD_MK3_OPTION;
		init_background_module(options_module);
		multi_plane();
		dlists=dlists_bogus;

		/* setup and create spinning cube */
		current_proc->pdata.p_store1=OPT_START;			// which cube face where on

		pcube=CREATE(PID_OPTION,opt_logo);
		pcube->a10=OPT_START;					// current cube face
		pcube->a11=FALSE;						// cube not spinning

		/* setup and create spinning dragon */
		gso_dmawnz_ns(obj,(ADDRESS)POLE01,options_anims,PFLAG_CLEAR);
		alloc_cache((ADDRESS)POLE01,&options_anims,obj);
		insert_object(obj,&objlst);
		obj->ozval=2;
		obj->oxpos.u.intpos=OPT_DRAGON_X;
		obj->oypos.u.intpos=OPT_DRAGON_Y;

		process_sleep(2);
		fadein_jsrp(0x16);

		ptemp=CREATE(PID_BANI,opt_dragon);
		ptemp->a10=DRAGON_SPIN;
		current_proc->pdata.p_store2=(ADDRESS)ptemp;			// keep ptr to dragon so that we can control him


		current_proc->a10=0;					// joy debounce
		/* main select loop */
		do
		{
			process_sleep(1);

			if (current_proc->a10!=0)
				current_proc->a10--;

			if (swcurr.padvalue & P1P2_ACTION)
			{
				(ADDRESS)ptemp=current_proc->pdata.p_store2;			// get dragon spin process and tell him to finish
				ptemp->a10=DRAGON_FINISH;
				break;
			}

			if ((swcurr.padvalue & P1P2_LEFT) && current_proc->a10==0 && pcube->a11==0 && current_proc->pdata.p_store1!=OPT_CREDIT)
			{
				/* turn cube to the left */
				current_proc->a10=12;
				if (current_proc->pdata.p_store1==0)
					current_proc->pdata.p_store1=OPT_MAX_SEL;
				else current_proc->pdata.p_store1--;

			 	pcube->a10=current_proc->pdata.p_store1;			// set new cube face
				tsound(0x5e);

			}
			else
			{
				if ((swcurr.padvalue & P1P2_RIGHT) && current_proc->a10==0 && pcube->a11==0 && current_proc->pdata.p_store1!=OPT_CREDIT)
				{
					/* turn cube to the right */
					current_proc->a10=12;
					if (current_proc->pdata.p_store1==OPT_MAX_SEL)
						current_proc->pdata.p_store1=0;
					else current_proc->pdata.p_store1++;

					pcube->a10=current_proc->pdata.p_store1;			// set new cube face
					tsound(0x5e);

				}
				else
				{
					if ( (swcurr.padvalue & P1P2_UP) && current_proc->a10==0 && pcube->a11==0)
					{
						if (current_proc->pdata.p_store1==OPT_START && f_cheat_menu!=0)
						{
							/* ONLY START CAN GOTO CREDITS w/ cheats enabled */
							current_proc->a10=12;
							current_proc->pdata.p_store1=pcube->a10=OPT_CREDIT;
							tsound(0x5e);
						}
					}
					else
					{
						if ((swcurr.padvalue & P1P2_DOWN) && current_proc->a10==0 && pcube->a11==0)
						{
							if (current_proc->pdata.p_store1==OPT_CREDIT)
							{
								/* ONLY CREDIT CAN GOTO START */
								current_proc->a10=12;
								current_proc->pdata.p_store1=pcube->a10=OPT_START;
								tsound(0x5e);
							}
						}
					}
				}
			}
		}
		while(1);

		/* selection made, do it */
		while (ptemp->a10!=DRAGON_STOP)
			process_sleep(1);					// wait for dragon to stop spinning

		process_sleep(30);
		fast_fadeout_jsrp(20);

		switch (current_proc->pdata.p_store1)
		{
			case OPT_START:			// ready to start playing
				gstate=current_proc->pdata.p_store5;
				murder_myoinit_score();
				return;
				break;
			case OPT_GAME:		// game options
				murder_myoinit_score();
				option_select(OPT_SCRN_GAME);
				break;
			case OPT_MUSIC:		// music options
				murder_myoinit_score();
				option_select(OPT_SCRN_MUSIC);
				break;
			case OPT_CONTROL:		// joystick configuration
				murder_myoinit_score();
				opt_pad_setup(opt_p1bridge,pad1_map_tbl);
				opt_pad_setup(opt_p2bridge,pad2_map_tbl);
				option_select(OPT_SCRN_CONTROLS);
				opt_pad_remap(opt_p1bridge,pad1_map_tbl);
				opt_pad_remap(opt_p2bridge,pad2_map_tbl);
				break;
			case OPT_CREDIT:
				murder_myoinit_score();
				if ( f_cheat_menu==1 )
					option_select(OPT_SCRN_CHEAT);
				else option_select(OPT_SCRN_CHEAT1);
				break;
		}
	}
	return;
}

/******************************************************************************
 Function: void opt_dragon(void)

 By: David Schwartz

 Date: March 1995

 Parameters: None

 Returns: None

 Description:	show the dragon part
******************************************************************************/
void opt_dragon(void)
{
	OBJECT *obj=current_proc->pa8;

	current_proc->pa9=a_opt_dragon;
	init_anirate(DRAGON_SPIN_SLOW);

	while (1)
	{
		switch (current_proc->a10)
		{
			case DRAGON_SPIN:
				if (GET_LONG(current_proc->pa9)==ANI_END)
					current_proc->pa9=a_opt_dragon;				// at end restart
				next_anirate();
				process_sleep(1);
				break;
			case DRAGON_FINISH:
				tsound(0x5f);
				init_anirate(DRAGON_SPIN_FAST);				// spin faster
				while (GET_LONG(current_proc->pa9)!=ANI_END)
				{
					next_anirate();							// spin until revolved around completely
					process_sleep(1);
				}

				/* spin one more time */
				tsound(0x5f);
				current_proc->pa9=a_opt_dragon;
				while (GET_LONG(current_proc->pa9)!=ANI_END)
				{
					next_anirate();							// spin until revolved around completely
					process_sleep(1);
				}

				current_proc->pa9=a_opt_dragon;
				frame_a9(obj);				// show first frame
				current_proc->a10=DRAGON_STOP;			// all done wait forever
			case DRAGON_STOP:
				wait_forever();
				break;
		}
	}
}

/******************************************************************************
 Function: void opt_logo(void)

 By: David Schwartz

 Date: March 1995

 Parameters: None

 Returns: None

 Description:	show the logo part
******************************************************************************/
void opt_logo(void)
{
	OBJECT *obj;

	current_proc->pdata.p_store1=current_proc->a10;			// get current face

	gso_dmawnz_ns(obj,(ADDRESS)FRAME01,options_anims,PFLAG_CLEAR);
	alloc_cache((ADDRESS)FRAME01,&options_anims,obj);
	insert_object(obj,&objlst);
	obj->ozval=0;
	obj->oxpos.u.intpos=OPT_CUBE_X;			// start at left edge
	obj->oypos.u.intpos=OPT_CUBE_Y;

	while (TRUE)
	{
		if (current_proc->a10==current_proc->pdata.p_store1)
			process_sleep(1);						// no change do nada
		else
		{
			/* time to change */
			current_proc->a11=1;				// set spinning flag

			if ( current_proc->a10==OPT_CREDIT || current_proc->pdata.p_store1==OPT_CREDIT)
			{
				current_proc->pa9=a_credit;

				if ( current_proc->pdata.p_store1==OPT_CREDIT)
					find_part2();

				framew(CUBE_SPIN);
			}
			else
			{
				/* determine spin direction */
				if (((current_proc->pdata.p_store1+1) & 3)==current_proc->a10)
				{
					/* spin right */
					current_proc->pa9=a_logo;
					find_part_a14(current_proc->pdata.p_store1+1);			// set at current
					framew(CUBE_SPIN);
				}
				else
				{
					/* spin left */
					current_proc->pa9=a_logo;
					find_part_a14(current_proc->a10+1+4);			// set at current
					framew(CUBE_SPIN);
				}
			}

			current_proc->pdata.p_store1=current_proc->a10;			// set new current
			current_proc->a11=0;									// done spinning, ready to do more
		}
	}
}

/******************************************************************************
 Function: void option_select(WORD scrnindex)

 By: David Schwartz

 Date: April 1995

 Parameters: scrnindex = which option screen
			0 - game configure
			1 - music configure
			2 - control configure

 Returns: None

 Description:	game selection options
******************************************************************************/
char *opt_screen_list[]=
{
	"GAME KONFIGURE",
	"SOUND AND MUSIC",
	"KONTROL KONFIGURE",
	"KONTROL KONFIGURE",
	"CHEATS",
	"CHEATS"
};

BYTE opt_dis_en[]={OIND_DISABLED,OIND_ENABLED,OIND_END};
BYTE opt_en_dis[]={OIND_ENABLED,OIND_DISABLED,OIND_END};
BYTE opt_off[]={OIND_OFF,OIND_END};
BYTE opt_off_on[]={OIND_OFF,OIND_ON,OIND_END};
BYTE opt_on_off[]={OIND_ON,OIND_OFF,OIND_END};
BYTE opt_diff[]={OIND_VEASY,OIND_EASY,OIND_MED,OIND_HARD,OIND_VHARD,OIND_END};
//BYTE opt_mtype[]={OIND_NTSC,OIND_RGB,OIND_END};
BYTE opt_music[]={OIND_SILENT,OIND_QUIET,OIND_ROCKIN,OIND_KRANKED,OIND_END};
BYTE opt_control[]={OIND_HP,OIND_LP,OIND_HK,OIND_LK,OIND_RUN,OIND_BLOCK,OIND_NOTASSIGN,OIND_END};
WORD opt_hard_log[]={MASK_JHIP,MASK_JLOP,MASK_JHIK,MASK_JLOK,MASK_JRUN,MASK_JBLOCK,0x0000};
BYTE opt_pan[]={OIND_OFF,OIND_LR,OIND_RL,OIND_END};
BYTE opt_shang[]={OIND_DISABLED,OIND_OPPONENT,OIND_ALL,OIND_END};

OPTLIST game_opts[]=
{
	{NULL,NULL,&cmos_diff,OPT_TOP_Y+20*0,OPT_TOP_X_L,OPT_TOP_X_C,OIND_DIFF,opt_diff,option_toggle,0},					// difficulty
	{NULL,NULL,&f_no_violence,OPT_TOP_Y+20*1,OPT_TOP_X_L,OPT_TOP_X_C,OIND_VIOLENCE,opt_on_off,option_toggle,0},	// violence
	{NULL,NULL,&f_no_blood,OPT_TOP_Y+20*2,OPT_TOP_X_L,OPT_TOP_X_C,OIND_BLOOD,opt_on_off,option_toggle,0},			// blood
	{NULL,NULL,&f_shang_morph,OPT_TOP_Y+20*3,OPT_TOP_X_L,OPT_TOP_X_C,OIND_SHANG,opt_shang,option_toggle,0},			// monitor type
	{NULL,NULL,&f_no_clock,OPT_TOP_Y+20*4,OPT_TOP_X_L,OPT_TOP_X_C,OIND_CLOCK,opt_en_dis,option_toggle,0},					// difficulty
	{NULL,NULL,&f_no_vs,OPT_TOP_Y+20*5,OPT_TOP_X_L,OPT_TOP_X_C,OIND_VS,opt_en_dis,option_toggle,0},			// vs screen on off
	{NULL,NULL,NULL,OPT_TOP_Y+30*4,OPT_EXIT_X,OPT_EXIT_X,OIND_EXIT,NULL,option_exit,0}							// exit
};

OPTLIST cheat_opts[]=
{
	{NULL,NULL,&f_freeplay,OPT_TOP_Y+20*0,OPT_TOP_X_L,OPT_TOP_X_C,OIND_FREEPLAY,opt_off_on,option_toggle,0},	// violence
	{NULL,NULL,&f_smoke,OPT_TOP_Y+20*1,OPT_TOP_X_L,OPT_TOP_X_C,OIND_SMOKE,opt_off_on,option_toggle,0},			// blood
	{NULL,NULL,&f_unlim_fatal,OPT_TOP_Y+20*2,OPT_TOP_X_L,OPT_TOP_X_C,OIND_FATAL_TIME,opt_on_off,option_toggle,0},			// blood
	{NULL,NULL,&f_level_select,OPT_TOP_Y+20*3,OPT_TOP_X_L,OPT_TOP_X_C,OIND_SELECT,opt_off_on,option_toggle,0},			// blood
	{NULL,NULL,NULL,OPT_TOP_Y+30*4,OPT_EXIT_X,OPT_EXIT_X,OIND_EXIT,NULL,option_exit,0}							// exit
};

OPTLIST cheat_opts1[]=
{
	{NULL,NULL,&f_freeplay,OPT_TOP_Y+20*0,OPT_TOP_X_L,OPT_TOP_X_C,OIND_FREEPLAY,opt_off_on,option_toggle,0},	// violence
	{NULL,NULL,&f_smoke,OPT_TOP_Y+20*1,OPT_TOP_X_L,OPT_TOP_X_C,OIND_SMOKE,opt_off_on,option_toggle,0},			// blood
	{NULL,NULL,&f_unlim_fatal,OPT_TOP_Y+20*2,OPT_TOP_X_L,OPT_TOP_X_C,OIND_FATAL_TIME,opt_on_off,option_toggle,0},			// blood
	{NULL,NULL,&f_level_select,OPT_TOP_Y+20*3,OPT_TOP_X_L,OPT_TOP_X_C,OIND_SELECT,opt_off_on,option_toggle,0},			// blood
	{NULL,NULL,&f_one_win,OPT_TOP_Y+20*4,OPT_TOP_X_L,OPT_TOP_X_C,OIND_ROUND,opt_off_on,option_toggle,0},			// blood
	{NULL,NULL,&df_nopower,OPT_TOP_Y+20*5,OPT_TOP_X_L,OPT_TOP_X_C,OIND_MATCH_DEAD,opt_off_on,option_toggle,0},			// blood
	{NULL,NULL,NULL,OPT_TOP_Y+30*4,OPT_EXIT_X,OPT_EXIT_X,OIND_EXIT,NULL,option_exit,0}							// exit
};


#if JAP_VERSION==1
OPTLIST sound_opts[]=
{
	{NULL,NULL,&f_stereo,OPT_TOP_Y+20*0,OPT_TOP_X_L,OPT_TOP_X_C,OIND_STEREO,opt_off_on,option_toggle_sound,0},			// sound on/off
	{NULL,NULL,&f_no_pan,OPT_TOP_Y+20*1,OPT_TOP_X_L,OPT_TOP_X_C,OIND_PAN,opt_pan,option_toggle_sound,0},			// pan control on/off, l/r r/l
	{NULL,NULL,&f_no_sfx,OPT_TOP_Y+20*2,OPT_TOP_X_L,OPT_TOP_X_C,OIND_SOUND,opt_on_off,option_toggle,0},		// sound fx on/off
	{NULL,NULL,&f_voice,OPT_TOP_Y+20*3,OPT_TOP_X_L,OPT_TOP_X_C,OIND_VOICE,opt_off_on,option_toggle,0},		// sound fx on/off
	{NULL,NULL,&f_music,OPT_TOP_Y+20*4,OPT_TOP_X_L,OPT_TOP_X_C,OIND_MUSIC,opt_music,option_toggle_sound,0},		// music silent,quiet,rockin,kranked
	{NULL,NULL,NULL,OPT_TOP_Y+30*4,OPT_EXIT_X,OPT_EXIT_X,OIND_EXIT,NULL,option_exit,0}							// exit
};
#else
OPTLIST sound_opts[]=
{
	{NULL,NULL,&f_stereo,OPT_TOP_Y+30*0,OPT_TOP_X_L,OPT_TOP_X_C,OIND_STEREO,opt_off_on,option_toggle_sound,0},			// sound on/off
	{NULL,NULL,&f_no_pan,OPT_TOP_Y+30*1,OPT_TOP_X_L,OPT_TOP_X_C,OIND_PAN,opt_pan,option_toggle_sound,0},			// pan control on/off, l/r r/l
	{NULL,NULL,&f_no_sfx,OPT_TOP_Y+30*2,OPT_TOP_X_L,OPT_TOP_X_C,OIND_SOUND,opt_on_off,option_toggle,0},		// sound fx on/off
	{NULL,NULL,&f_music,OPT_TOP_Y+30*3,OPT_TOP_X_L,OPT_TOP_X_C,OIND_MUSIC,opt_music,option_toggle_sound,0},		// music silent,quiet,rockin,kranked
	{NULL,NULL,NULL,OPT_TOP_Y+30*4,OPT_EXIT_X,OPT_EXIT_X,OIND_EXIT,NULL,option_exit,0}							// exit
};
#endif

OPTLIST control1_opts[]=
{
	{NULL,NULL,&pad_tri[0],OPT_TOP_Y+14*0,OPT_CC_X_L,OPT_CC_X1_C,OIND_TRIANGLE,opt_control,option_toggle,0},					// difficulty
	{NULL,NULL,&pad_square[0],OPT_TOP_Y+14*1,OPT_CC_X_L,OPT_CC_X1_C,OIND_SQUARE,opt_control,option_toggle,0},					// difficulty
	{NULL,NULL,&pad_circle[0],OPT_TOP_Y+14*2,OPT_CC_X_L,OPT_CC_X1_C,OIND_CIRCLE,opt_control,option_toggle,0},					// difficulty
	{NULL,NULL,&pad_x[0],OPT_TOP_Y+14*3,OPT_CC_X_L,OPT_CC_X1_C,OIND_X,opt_control,option_toggle,0},					// difficulty
	{NULL,NULL,&pad_r2[0],OPT_TOP_Y+14*4,OPT_CC_X_L,OPT_CC_X1_C,OIND_R2,opt_control,option_toggle,0},					// difficulty
	{NULL,NULL,&pad_r1[0],OPT_TOP_Y+14*5,OPT_CC_X_L,OPT_CC_X1_C,OIND_R1,opt_control,option_toggle,0},					// difficulty
	{NULL,NULL,&pad_l2[0],OPT_TOP_Y+14*6,OPT_CC_X_L,OPT_CC_X1_C,OIND_L2,opt_control,option_toggle,0},					// difficulty
	{NULL,NULL,&pad_l1[0],OPT_TOP_Y+14*7,OPT_CC_X_L,OPT_CC_X1_C,OIND_L1,opt_control,option_toggle,0},					// difficulty
	{NULL,NULL,NULL,OPT_TOP_Y+30*4,OPT_CC_X1_C-24,OPT_CC_X1_C-24,OIND_EXIT,NULL,option_exit,0}							// exit
};

OPTLIST control2_opts[]=
{
	{NULL,NULL,&pad_tri[1],OPT_TOP_Y+14*0,OPT_CC_X_L,OPT_CC_X2_C,OIND_TRIANGLE,opt_control,option_toggle,0},					// difficulty
	{NULL,NULL,&pad_square[1],OPT_TOP_Y+14*1,OPT_CC_X_L,OPT_CC_X2_C,OIND_SQUARE,opt_control,option_toggle,0},					// difficulty
	{NULL,NULL,&pad_circle[1],OPT_TOP_Y+14*2,OPT_CC_X_L,OPT_CC_X2_C,OIND_CIRCLE,opt_control,option_toggle,0},					// difficulty
	{NULL,NULL,&pad_x[1],OPT_TOP_Y+14*3,OPT_CC_X_L,OPT_CC_X2_C,OIND_X,opt_control,option_toggle,0},					// difficulty
	{NULL,NULL,&pad_r2[1],OPT_TOP_Y+14*4,OPT_CC_X_L,OPT_CC_X2_C,OIND_R2,opt_control,option_toggle,0},					// difficulty
	{NULL,NULL,&pad_r1[1],OPT_TOP_Y+14*5,OPT_CC_X_L,OPT_CC_X2_C,OIND_R1,opt_control,option_toggle,0},					// difficulty
	{NULL,NULL,&pad_l2[1],OPT_TOP_Y+14*6,OPT_CC_X_L,OPT_CC_X2_C,OIND_L2,opt_control,option_toggle,0},					// difficulty
	{NULL,NULL,&pad_l1[1],OPT_TOP_Y+14*7,OPT_CC_X_L,OPT_CC_X2_C,OIND_L1,opt_control,option_toggle,0},					// difficulty
	{NULL,NULL,NULL,OPT_TOP_Y+30*4,OPT_CC_X2_C-24,OPT_CC_X2_C-24,OIND_EXIT,NULL,option_exit,0}							// exit
};

OPTLIST *opt_tables[]={game_opts,sound_opts,control1_opts,control2_opts,cheat_opts,cheat_opts1};

void option_select(WORD scrnindex)
{
	OBJECT *obj;

	load_bkgd=BKGD_MK3_COINOPT;
	init_background_module(optcoin_module);
	multi_plane();

	p15_centered(opt_screen_list[scrnindex],160,16);

	obj=(OBJECT *)&objlst2;
	while ( obj->olink!=NULL )
	{
		obj=obj->olink;
		if ( obj->oid==OID_TEXT )
			obj->oid=OID_OPTION;
	}

	f_novel=1;								// disable velocities
	opt_done=0;								// # of process using routine

	/* activate 2nd player controls */
	current_proc->pdata.p_store7=OPT_P1MASK;
	if (scrnindex==OPT_SCRN_CONTROLS)
	{
		CREATE(PID_OPTCONTROL,opt_p2control);
	}
	else
	{
		current_proc->pdata.p_store7|=OPT_P2MASK;
	}

	current_proc->a11=scrnindex;
	opt_proc();

	while ( opt_done!=0 )
		process_sleep(1);

	fadeout_jsrp(0x30);						// fade down

	return;
}

void opt_p2control(void)
{
	current_proc->a11=OPT_SCRN_CONTROLS2;
	current_proc->pdata.p_store7=OPT_P2MASK;
	opt_proc();
	process_suicide();
}

/******************************************************************************
 Function: void opt_proc(void)

 By: David Schwartz

 Date: April 1995

 Parameters:
				current_proc->pdata.p_store7 - joystick mask
								(OPT_P1MASK - player 1 joystick only)
								(OPT_P2MASK - player 2 joystick only)
 				current_proc->a11 = which option screen table to use
					0 = game configure
					1 = music configure
					2 = p1 control configure
					3 = p2 control configure
					4 = cheat screen

 Returns: None

 Description:	game selection options
******************************************************************************/
void opt_proc(void)
{
	OPTLIST *opt,*opt_table;
	OBJECT *obj;

	opt=opt_table=opt_tables[current_proc->a11];

	/* setup all parameters, display and stuff for options */
	current_proc->a11=init_option_table(opt);			// max # of entries
	current_proc->pdata.p_store1=0;								// current entry pos
	current_proc->a10=0;													// debounce

	/* hilight palette of first choice */
	obj=opt->cobj;
	delpal(obj);
	obj->opal=get_fore_pal(OPT_HILITE_COLOR);

	opt_done=1;
	current_proc->pdata.p_store6=1;
	while (current_proc->pdata.p_store6 && opt_done!=0)
	{
		process_sleep(1);

		if ( current_proc->a10!=0 )
		{
				current_proc->a10--;
		}

		/* check for cursor movement */
		if ((swcurr.padvalue & P1P2_UP & current_proc->pdata.p_store7) && current_proc->a10==0)
		{
			current_proc->a10=12;
			obj=opt->cobj;							// get object

			/* move up */
			if ( obj!=NULL )
			{
				delpal(obj);							// change back to original color
				obj->opal=get_fore_pal(OPT_NORMAL_COLOR);
			}

			if (current_proc->pdata.p_store1==0)
				current_proc->pdata.p_store1=current_proc->a11-1;
			else current_proc->pdata.p_store1--;

			opt=opt_table+current_proc->pdata.p_store1;		// update opt ptr & object
			obj=opt->cobj;

			if ( obj!=NULL )
			{
				delpal(obj);							// set hilite color
				obj->opal=get_fore_pal(OPT_HILITE_COLOR);
			}

			tsound(0x0c);
		}
		else
		{
			if ((swcurr.padvalue & P1P2_DOWN & current_proc->pdata.p_store7) && current_proc->a10==0)
			{
				current_proc->a10=12;
				obj=opt->cobj;							// get object

				/* move down */
				if ( obj!=NULL )
				{
					delpal(obj);							// set to orig color
					obj->opal=get_fore_pal(OPT_NORMAL_COLOR);
				}

				current_proc->pdata.p_store1++;

				if (current_proc->pdata.p_store1==current_proc->a11)
					current_proc->pdata.p_store1=0;

				opt=opt_table+current_proc->pdata.p_store1;		// update opt ptr & object
				obj=opt->cobj;

				if ( obj!=NULL )
				{
					delpal(obj);							// set hilite color
					obj->opal=get_fore_pal(OPT_HILITE_COLOR);
				}

				tsound(0x0c);
			}
		}

		current_proc->pdata.p_store4=(ADDRESS)opt_table;		// setup transfer vars
		current_proc->pdata.p_store5=(ADDRESS)opt;
		current_proc->pa8=obj;
		opt->toggle_func();
		(ADDRESS)opt_table=current_proc->pdata.p_store4;		// update transfer vars
		(ADDRESS)opt=current_proc->pdata.p_store5;
		obj=current_proc->pa8;
	}

	opt_done=0;

	/* unhilite object */
	obj=opt->cobj;
	delpal(obj);
	obj->opal=get_fore_pal(OPT_NORMAL_COLOR);

	return;
}

/******************************************************************************
 Function: void option_exit(void)

 By: David Schwartz

 Date: April 1995

 Parameters:  current_proc->pdata.p_store5 - current option table entry
							current_proc->pdata.p_store4 - base option table

 Returns: None

 Description:	see if its time to exit
******************************************************************************/
void option_exit(void)
{
	OPTLIST *opt;

	opt=(OPTLIST *)current_proc->pdata.p_store5;

	/* check for exit */
	if ((swcurr.padvalue & OPTION_EXIT & current_proc->pdata.p_store7))
		current_proc->pdata.p_store6=0;

	return;
}

/******************************************************************************
 Function: void option_toggle(void)

 By: David Schwartz

 Date: April 1995

 Parameters:  current_proc->pdata.p_store5 - current option table entry
							current_proc->pdata.p_store4 - base option table

 Returns: None

 Description:	std option toggle stuff, will toggle through a list based on joy input
******************************************************************************/
void option_toggle_sound(void)
{
	option_toggle();
	sound_driver_config();			// repeatedly set sound driver, a waste but no big deal
	return;
}

void option_toggle(void)
{
	OBJECT *obj;
	OPTLIST *opt,*opt_table;

	opt_table=(OPTLIST *)current_proc->pdata.p_store4;
	opt=(OPTLIST *)current_proc->pdata.p_store5;
	obj=current_proc->pa8;

	/* check for option change */
	if ((swcurr.padvalue & P1P2_RIGHT & current_proc->pdata.p_store7) && current_proc->a10==0 && opt->choicetbl!=NULL)
	{

		current_proc->a10=12;

		/* toggle forward in options */
		opt->index++;								// next option
		if ((current_proc->a0=*((opt->choicetbl)+opt->index))==OIND_END)		// if at end reset to start
		{
			opt->index=0;
			current_proc->a0=*opt->choicetbl;
		}

		/* update graphics */
		obj->header.t_xoffset=option_text_list[current_proc->a0].u0;
		obj->header.t_yoffset=option_text_list[current_proc->a0].v0;
		obj->header.t_width=option_text_list[current_proc->a0].w;
		obj->header.t_height=option_text_list[current_proc->a0].h;

		/* center object */
		center_opt_obj(obj);

		/* update variable */
		if ( opt->var!=NULL )
			*opt->var=opt->index;

	}
	else
	{
		if ((swcurr.padvalue & P1P2_LEFT & current_proc->pdata.p_store7) && current_proc->a10==0 && opt->choicetbl!=NULL)
		{
			current_proc->a10=12;

			/* toggle backward in options */
			if (opt->index==0)
			{
				while (*((opt->choicetbl)+opt->index)!=OIND_END)
					opt->index++;
			}

			opt->index--;

			current_proc->a0=*((opt->choicetbl)+opt->index);

			/* update graphics */
			obj->header.t_xoffset=option_text_list[current_proc->a0].u0;
			obj->header.t_yoffset=option_text_list[current_proc->a0].v0;
			obj->header.t_width=option_text_list[current_proc->a0].w;
			obj->header.t_height=option_text_list[current_proc->a0].h;

			/* center object */
			center_opt_obj(obj);

			/* update variable */
			if ( opt->var!=NULL )
				*opt->var=opt->index;
		}
	}

	/* update vars to other routines will know about changes */
	current_proc->pdata.p_store4=(ADDRESS)opt_table;
	current_proc->pdata.p_store5=(ADDRESS)opt;
	current_proc->pa8=obj;
}

/******************************************************************************
 Function: WORD init_option_table(OPTLIST *otbl)

 By: David Schwartz

 Date: April 1995

 Parameters:  otbl - ptr to a table of options

 Returns: # of options in table (includeing exit)

 Description:	setup a screen for options
	Option table must be terminated by using NULL in 'var' field
******************************************************************************/
WORD init_option_table(OPTLIST *otbl)
{
	OBJECT *obj;		// temp obj ptr
	WORD index=0;

	while (otbl->lab_index!=OIND_EXIT)
	{
		/* set choice index to current variable value */
		otbl->index=*(otbl->var);

		/* Create Label Object */
		obj=get_option_object(otbl->xlabel,otbl->y,otbl->lab_index);
		obj->opal=get_fore_pal(OPT_NORMAL_COLOR);
		insert_object(obj,&objlst2);
		otbl->lobj=obj;							// link object into struct

		/* Create Choice Object */
		if ( otbl->choicetbl!=NULL )
		{
			obj=get_option_object(otbl->xchoice,otbl->y,*(otbl->choicetbl+otbl->index));
			center_opt_obj(obj);
			obj->opal=get_fore_pal(OPT_NORMAL_COLOR);
			insert_object(obj,&objlst2);
			otbl->cobj=obj;							// link object into struct
		}

		otbl++;							// next entry
		index++;
	}

	/* setup exit object */
	obj=get_option_object(otbl->xlabel,otbl->y,otbl->lab_index);
	obj->opal=get_fore_pal(OPT_NORMAL_COLOR);
	insert_object(obj,&objlst2);
	otbl->cobj=obj;							// link object into struct

	return(index+1);
}

/******************************************************************************
 Function: OBJECT *get_option_object(short x, short y, WORD lindex)

 By: David Schwartz

 Date: April 1995

 Parameters: x,y - pos of object
			 lindex - index into option_text_list to get graphic data

 Returns: None

 Description:	setup a text object for the option screens
******************************************************************************/
OBJECT *get_option_object(short x,short y,WORD lindex)
{
	OBJECT *obj;

	obj=get_object();

	obj->ozval=1;
	obj->oxvel.u.intpos=obj->oxpos.u.intpos=x;
	obj->oyvel.u.intpos=obj->oypos.u.intpos=y;
	obj->oid=OID_OPTION;

	/* setup header stuff */
	obj->header.tpage=option_tpage;
	obj->header.t_xoffset=option_text_list[lindex].u0;
	obj->header.t_yoffset=option_text_list[lindex].v0;
	obj->header.t_width=option_text_list[lindex].w;
	obj->header.t_height=option_text_list[lindex].h;

	return(obj);
}

#if 0
/******************************************************************************
 Function: void option_mtest(void)

 By: David Schwartz

 Date: April 1995

 Parameters: None

 Returns: None

 Description:	music test
******************************************************************************/
void option_mtest(void)
{
	option_test_toggle(OPT_ACTIVE_MUSIC,OPT_MAX_MUSIC);
	return;
}

/******************************************************************************
 Function: void option_stest(void)

 By: David Schwartz

 Date: April 1995

 Parameters: None

 Returns: None

 Description:	sound test
******************************************************************************/
void option_stest(void)
{
	option_test_toggle(OPT_ACTIVE_SOUND,OPT_MAX_SOUND);
	return;
}

/******************************************************************************
 Function: void option_test_toggle(WORD aflag,WORD max)

 By: David Schwartz

 Date: April 1995

 Parameters: None

 Returns: None

 Description:	sound test
******************************************************************************/
void option_test_toggle(WORD aflag,WORD max)
{
	OPTLIST *opt,*opt_table;

	opt_active=aflag;				// set sound active

	opt_table=(OPTLIST *)current_proc->pdata.p_store4;
	opt=(OPTLIST *)current_proc->pdata.p_store5;

	/* check for option change */
	if ((swcurr.padvalue & P1P2_RIGHT & current_proc->pdata.p_store7) && current_proc->a10==0)
	{

		current_proc->a10=12;

		/* toggle forward in options */
		opt->index++;								// next option
		if ( opt->index>=max)
			opt->index=0;

		/* update variable */
		*opt->var=opt->index;

	}
	else
	{
		if ((swcurr.padvalue & P1P2_LEFT & current_proc->pdata.p_store7) && current_proc->a10==0)
		{
			current_proc->a10=12;

			/* toggle backward in options */
			if ( opt->index==0 )
				opt->index=max-1;
			else opt->index--;

			/* update variable */
			*opt->var=opt->index;
		}
	}

	return;
}

#if 0
/******************************************************************************
 Function: void soundtest_proc(void)

 By: David Schwartz

 Date: April 1995

 Parameters: current_proc->a10 - x pos
						 current_proc->a11 - y pos
						 current_proc->a14 - ptr to variable to track

 Returns: None

 Description:	sound test process, this process deals with update the text
						and playing a sfx
******************************************************************************/
void soundtest_proc(void)
{
	WORD *sndsrc;
	OBJECT *obj;

	sndsrc=(WORD *)current_proc->a14;
	current_proc->pdata.p_store1=*sndsrc;

	/* print current snd num */
	opt_disp_num(current_proc->pdata.p_store1,current_proc->a10,current_proc->a11,current_proc->procid);

	while ( 1 )
	{
		sndsrc=(WORD *)current_proc->a14;

		if ( (*sndsrc)!=current_proc->pdata.p_store1 )
		{
			/* sound # changed, play new sfx, and print new # */
			kilobj2(current_proc->procid,-1);							// remove old text
			current_proc->pdata.p_store1=*sndsrc;
			opt_disp_num(*sndsrc,current_proc->a10,current_proc->a11,current_proc->procid);
		}

		obj=(OBJECT *)&objlst2;

		/* set text color, hilite or normal depending on activity */
		(WORD *)current_proc->a0=OPT_NORMAL_COLOR;
		if ( opt_active==current_proc->procid )
		{
			(WORD *)current_proc->a0=OPT_HILITE_COLOR;
			opt_active=OPT_ACTIVE_NONE;
		}

		/* change color */
		while ( obj->olink!=NULL )
		{
			 obj=obj->olink;
			 if ( obj->oid==current_proc->procid )
			 {
				 delpal(obj);
				 obj->opal=get_fore_pal((WORD *)current_proc->a0);
			 }
		}

		process_sleep(1);
	}

	process_suicide();
}
#endif

/******************************************************************************
 Function: void opt_disp_num(WORD sndnum,WORD x,WORD y,WORD id)

 By: David Schwartz

 Date: April 1995

 Parameters: sndnum - current sound number
						 x,y - position
						 id - id to change all OID_TEXT

 Returns: None

 Description:	display the current sound effect/music that we are going to play
******************************************************************************/
void opt_disp_num(WORD sndnum,WORD x,WORD y,WORD id)
{
	OBJECT *obj;

	/* set position */
 	lm_setup(pf_dave_smallc);
	fnt_state.fnt_posx=x;
	fnt_state.fnt_posy=y;

	/* print number */
	printf_p1("%d",sndnum);

	/* change ids */
	obj=(OBJECT *)&objlst2;
	while ( obj->olink!=NULL )
	{
		obj=obj->olink;
		if ( obj->oid==OID_TEXT )
			obj->oid=id;
	}

	return;
}
#endif

/******************************************************************************
 Function: void opt_pad_setup(OPTMAP *omap, WORD *padmap)

 By: David Schwartz

 Date: April 1995

 Parameters: omap - init table
						 index - 0=p1 1=p2

 Returns: None

 Description: read the current logical joystick values and convert them to viewable
							indecies for the option remap routines

 pad?_map_table[X] -> logical hardware value -> display object index
******************************************************************************/
void opt_pad_setup(OPTMAP *omap, WORD *padmap)
{
	WORD log_hard_val;

	while ( omap->storage!=NULL )
	{
		/* retrieve currentl logical hardware value from remap table */
		log_hard_val=*(padmap+omap->b_index);

		/* assign storage, dispaly object index with correct index type */
		switch ( log_hard_val )
		{
			case MASK_JHIP:
				*(omap->storage)=OCL_HP;
				break;
			case MASK_JLOP:
				*(omap->storage)=OCL_LP;
				break;
			case MASK_JHIK:
				*(omap->storage)=OCL_HK;
				break;
			case MASK_JLOK:
				*(omap->storage)=OCL_LK;
				break;
			case MASK_JRUN:
				*(omap->storage)=OCL_RUN;
				break;
			case MASK_JBLOCK:
				*(omap->storage)=OCL_BLOCK;
				break;
			default:
				*(omap->storage)=OCL_OFF;
				break;
		}
		omap++;
	}

	return;
}

/******************************************************************************
 Function: void opt_pad_remap(OPTMAP *omap, WORD *padmap)

 By: David Schwartz

 Date: April 1995

 Parameters: omap - init table
						 index - 0=p1 1=p2

 Returns: None

 Description: converts readable index into logical hardware value and stores into remap table

 display object index -> logical hardware value -> pad?_map_table[X]
******************************************************************************/
void opt_pad_remap(OPTMAP *omap, WORD *padmap)
{
	WORD log_hard_val;

	while ( omap->storage!=NULL )
	{
		/* retrieve current display index */
		log_hard_val=opt_hard_log[*(omap->storage)];

		/* assign storage, dispaly object index with correct index type */
		*(padmap+omap->b_index)=log_hard_val;

		omap++;
	}

	return;
}
