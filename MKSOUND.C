/******************************************************************************
 File: mksound.c

 Date: Dec 1994

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
#include "mkobj.h"
#include "mkgame.h"
#include "mkbonus.h"
#include "mksound.h"
#include "psxcd.h"
#include "psxsnd.h"

#define TS_FEET_LAND	RSND_FS3
#define TS_SK_SHOW_MERCY 0
#define TS_SK_WINS       0
#define TS_SK_VS         0

//*****
//JAX:
#define ST_JAX_CLANG	0

//CYRAX:
#define FT_ROBO_PROPELL	0

//SCORPION:
#define ST_SCORP_COME	0
#define ST_SCORP_GET	0
#define ST_SCORP_SPEAR	0
#define ST_SCORP_MASK	0
#define ST_SCORP_TELE	0
#define ST_SCORP_STAB	0

//ERMAC:
#define ST_ERMAC_TELE	0
#define ST_ERMAC_POOF	0
#define ST_ERMAC_HIT	0
#define ST_ERMAC_FLOOR	0

//REPTILE:
#define ST_REP_EXP		0
#define ST_REP_SLIDE	0
#define ST_REP_SPIT		0
#define ST_REP_INVISO	0
#define ST_REP_SIZZLE	0
#define ST_REP_OBJ		0
#define ST_REP_VOMIT	0

//OLDSZ:
#define ST_OSZ_ICEUP	0
#define ST_OSZ_IBLOCK	0
#define ST_OSZ_ICEHIT	0
#define ST_OSZ_SLIDE	0
#define ST_OSZ_FLOOR	0

//KITANA:
#define ST_KIT_THROW   	0
#define ST_KIT_FANH		0
#define ST_KIT_FANL		0
#define ST_KIT_KISS		0

//JADE:
#define ST_JADE_ARROW	0
#define ST_JADE_ZAP		0
#define ST_JADE_PROP	0
#define ST_JADE_TELE	0
#define ST_JADE_FLASH	0

//MILEENA:
#define ST_MIL_SAI		0
#define ST_MIL_HIT		0
#define ST_MIL_ROLL		0
#define ST_MIL_CLOUD	0

/***************************************************************************
***************************************************************************/
short gs_death[]=
{
	1,
   	GS_DEATH_GIRL,		// girl = eb
   	GS_DEATH_VP,			// vp = hey
   	GS_DEATH_DF,			// df = df
   	GS_DEATH_EB,	 		// eb
   	GS_DEATH_JH,	 		// 4 = eb
   	GS_DEATH_ROBO,	 		// 5 = hey
   	GS_DEATH_KANG,			// kang
	GS_DEATH_SG,			// sheeva
	GS_DEATH_SK			// shao kahn
};

short gs_shook[]=
{
	1,
	GS_SHOOK_GIRL,	   	// Girl #1: Wast #2
	GS_SHOOK_VP,		// VP: shook
	GS_SHOOK_DF,		// DF: shook
	GS_SHOOK_EB,		// EB: shook
	GS_SHOOK_JH,		// EB: shook
	GS_SHOOK_ROBO,	 	// robo: shook
	GS_SHOOK_KANG,	 	// kang: run (borrow)
	GS_SHOOK_SG,			// sheeva
	GS_SHOOK_SK			// shao kahn
};

short gs_run[]=
{
	1,
  	GS_RUN_GIRL,	  	// Girl #1: run
  	GS_RUN_VP,			// VP: run
  	GS_RUN_DF,			// DF: run
  	GS_RUN_EB,		// EB: run
  	GS_RUN_JH,		// JH: run
  	GS_RUN_ROBO,	 	// robo: run
  	GS_RUN_KANG,		// kang: run
	GS_RUN_SG,			// sheeva
	GS_RUN_SG			// shao kahn
};

short gs_tripped_voice[]=
{
	1,
	GS_TV_GIRL,	   	// Girl #1: tripped voice
	GS_TV_VP,			// VP: attack #1
	GS_TV_DF,			// DF: tripped
	GS_TV_EB,		// EB: tripped
	GS_TV_JH,		// JH: tripped
	0xffff,		 	// robo
	GS_TV_KANG,			// JH: tripped
	GS_TV_SG,			// sheeva
	GS_TV_SK			// shao kahn
};

short gs_face_hit_voice[]=
{
	3,
	GS_FHV_GIRL1,	  	// Girl: hit in face voice
	GS_FHV_GIRL2,	  	// Girl: hit in face voice
	GS_FHV_GIRL3,	  	// Girl: hit in face voice

	GS_FHV_VP1,		// VP: hit in face voice
	GS_FHV_VP2,		// VP: hit in face voice
	GS_FHV_VP3,		// VP: hit in face voice

	GS_FHV_DF1,	 	// DF: hit in face voice
	GS_FHV_DF2,	 	// DF: hit in face voice
	GS_FHV_DF3,	 	// DF: hit in face voice

	GS_FHV_EB1,	  	// EB: hit in face voice
	GS_FHV_EB2,	 	// EB: hit in face voice
	GS_FHV_EB3,	  	// EB: hit in face voice

	GS_FHV_JH1,	  	// JH: hit in face voice
	GS_FHV_JH2,	  	// JH: hit in face voice
	GS_FHV_JH3,	  	// JH: hit in face voice

	GS_FHV_ROBO1,	 	// robo: hit in face
	GS_FHV_ROBO2,	 	// robo: hit in face
	GS_FHV_ROBO3,		// robo: hit in face

	GS_FHV_KANG1,	  	// kang: hit in face voice
	GS_FHV_KANG2,	  	// kang: hit in face voice
	GS_FHV_KANG3,		// kang: hit in face voice

	GS_FHV_SG1,			// sheeva
	GS_FHV_SG2,			// sheeva
	GS_FHV_SG3,			// sheeva

	GS_FHV_SK1,			// shao kahn
	GS_FHV_SK2,			// shao kahn
	GS_FHV_SK3			// shao kahn

};

short gs_attack[]=
{
	2,						// 2 entries per group
	GS_ATK_GIRL1,	 	// Girl #1: attack #1
	GS_ATK_GIRL2,	 	// Girl #1: attack #2
	GS_ATK_VP1,	// VP: attack #1
	GS_ATK_VP2,	// VP: attack #2
	GS_ATK_DF1,		// DF: attack #1
	GS_ATK_DF2,		// DF: attack #2
	GS_ATK_EB1,	 	// EB: attack #1
	GS_ATK_EB2,	 	// EB: attack #2
	GS_ATK_JH1,	 	// JH: attack #1
	GS_ATK_JH2,	 	// JH: attack #2
	0xffff,		 				// robo attack #1
	0xffff,		 				// robo attack #2
	GS_ATK_KANG1,	 	// kang: attack #1
	GS_ATK_KANG2,		// kang: attack #2
	GS_ATK_SG1,			// sheeva
	GS_ATK_SG2,			// sheeva
	GS_ATK_SK1,			// shao kahn
	GS_ATK_SK2			// shao kahn
};

short gs_jump[]=
{
	1,
   	GS_JUMP_GIRL,	  	// Girl #1: jump
   	GS_JUMP_VP,		// VP: jump
   	GS_JUMP_DF,		// DF: jump
   	GS_JUMP_EB,		// EB: jump
   	GS_JUMP_JH,		// JH: jump
		0xffff,		 				// robo = sans
   	GS_JUMP_KANG,		// kang: jump
	GS_JUMP_SG,			// sheeva
	GS_JUMP_SK			// shao kahn
};

short gs_grab[]=
{
	1,
   	GS_GRAB_GIRL,	// girl grab
   	GS_GRAB_VP,		// VP: grab ??
   	GS_GRAB_DF,		// DF: grab ??
   	GS_GRAB_EB,		// EB: grab ??
   	GS_GRAB_JH,		// JH: grab ??
   	GS_GRAB_ROBO,				 	// robo: grab
   	GS_GRAB_KANG,		// kang: grab
	GS_GRAB_SG,			// sheeva
	GS_GRAB_SK			// shao kahn
};

short gs_slam[]=
{
	1,
	GS_SLAM_GIRL,	// girl throw voice
	GS_SLAM_VP,		// VP: throw voice
	GS_SLAM_DF,		// DF: throw voice
	GS_SLAM_EB,		// EB: throw voice
	GS_SLAM_JH,		// JH: throw voice
	GS_SLAM_ROBO,	 	// robo: throw voice
	GS_SLAM_KANG,		// kang: throw voice
	GS_SLAM_SG,			// sheeva
	GS_SLAM_SK			// shao kahn
};

short gs_wasted[]=
{
	2,
	GS_WST_GIRL1,	  	// Girl #1: Wast #1
	GS_WST_GIRL2,	  	// Girl #1: Wast #2
	GS_WST_VP1,			// VP: Wast #1
	GS_WST_VP2,			// VP: Wast #2
	GS_WST_DF1,			// DF: Wast
	GS_WST_DF2,			// DF: Wast
	GS_WST_EB1,		  	// EB: Wast #1
	GS_WST_EB2,		  	// EB: Wast #2
	GS_WST_JH1,		  	// JH: Wast #1
	GS_WST_JH2,		  	// JH: Wast #2
	GS_WST_ROBO1,	 	// robo: wasted
	GS_WST_ROBO2,	 	// robo: wasted
	GS_WST_KANG1, 	// kang: Wast #1
	GS_WST_KANG2, 	// kang: Wast #2
	GS_WST_SG1,			// sheeva
	GS_WST_SG2,			// sheeva
	GS_WST_SK1,			// shao kahn
	GS_WST_SK2			// shao kahn
};

short *group_table[]=
{
	gs_attack,			// 0 = quick attacks
	gs_jump, 			// 1 = jump
	gs_wasted,			// 2 = wasted
	gs_grab,  			// 3 = grab
	gs_slam,  			// 4 = throw voice
	gs_tripped_voice,	// 5 =
	gs_face_hit_voice,	// 6 =
	gs_run,				// 7 =
	gs_shook,			// 8 =
	gs_death			// 9
};

short ochar_voice_groups[]=
{
 	GP_MALE3,	//
	GP_FEMALE,	//
	GP_MALE3,	//
	GP_MALE2,	//
	GP_MALE4,	//
	GP_MALE1,	//
	GP_FEMALE,	//
	GP_ROBO,		//
	GP_ROBO,		//
	GP_MALE4,	//
	GP_MALE1,	//
	GP_SHEEVA,	//
	GP_MALE3,	//
	GP_KANG,		//
	GP_ROBO,	//
 
   	GP_FEMALE,	
   	GP_FEMALE,	
   	GP_FEMALE,	
   	GP_MALE1,	
   	GP_MALE2,	
   	GP_MALE3,	
   	GP_MALE4,	
   	GP_MALE1,	
   	GP_MALE2,
	-1,
	GP_SHAO_KAHN
};

/******************************************************************************
 Function: void group_sound(WORD offset)

 By: David Schwartz

 Date: Apr 1995

 Parameters: offset - group offset

 Returns: None

 Description:
******************************************************************************/
void group_sound(WORD offset)
{
	short *stbl;
	OBJECT *obj=current_proc->pa8;


	stbl=get_group_table(offset);
	current_proc->a2=GET_WORD(stbl)++;				// # of entries
	current_proc->a3=current_proc->a2;

	(short)current_proc->a1=ochar_voice_groups[obj->ochar];		// a1 = my group (offset into groups data)

	if (((short)current_proc->a1)<0)
		return;									// neg = no group for me
	else
	{
		stbl+=(current_proc->a2*current_proc->a1);	// # of words to skip to get to our group data
	}

	/* gsnd5 */
	next_table_sound(current_proc->a3,stbl);

	return;
}

/******************************************************************************
 Function: void kill_group_sound(WORD offset,OBJECT *obj)

 By: David Schwartz

 Date: Apr 1995

 Parameters: offset - group offset
						 obj - group to kill from

 Returns: None

 Description:
******************************************************************************/
void kill_group_sound(WORD offset,OBJECT *obj)
{
	short *stbl;

	stbl=get_group_table(offset);
	current_proc->a2=GET_WORD(stbl)++;				// # of entries
	current_proc->a3=current_proc->a2;

	(short)current_proc->a1=ochar_voice_groups[obj->ochar];		// a1 = my group (offset into groups data)

	if (((short)current_proc->a1)<0)
		return;									// neg = no group for me
	else
	{
		stbl+=(current_proc->a2*current_proc->a1);	// # of words to skip to get to our group data
	}

	/* gsnd5 */
	do
	{
		wess_seq_stop(*stbl);						// kill all sounds for that group
		stbl++;
	} while ( --current_proc->a3>0 );

	return;
}

/******************************************************************************
 Function: void his_group_sound(WORD offset)

 By: David Schwartz

 Date: Apr 1995

 Parameters: offset - group offset

 Returns: None

 Description:
******************************************************************************/
void his_group_sound(WORD offset)
{
	OBJECT *ta8;

	ta8=current_proc->pa8;
	current_proc->pa8=current_proc->pdata.p_otherguy;
	group_sound(offset);
	current_proc->pa8=ta8;

	return;
}


/******************************************************************************
RANDOM SELECT SOUND EFFECTS
******************************************************************************/
short rsnd_fs[]=
{
	RSND_FS1,	// 5 - foot step #1
	RSND_FS2,	// 6 - foot step #2
	RSND_FS3,	// 7 - foot step #3
	RSND_FS4	// 8 - foot step #4
};
void rsnd_footstep(void)
{
	next_table_sound(4,rsnd_fs);
}

short rsnd_skb[]=
{
	ST_SK_PATHETIC, 	// 1 - Shao Kahn: That was Pathetic
	ST_SK_NEVER_WIN,	// 2 - Shao Kahn: You will never win
	ST_SK_STILL_TRYING,	// 3 - Shao Kahn: You're still trying to win?
	ST_SK_DONT_LAUGH,		// 4 - Shao Kahn: Don't make me laugh
	ST_SK_DONT_LAUGH	// 5 - Shao Kahn: I win
};
void rsnd_sk_bonus_win(void)
{
	next_table_sound(5,rsnd_skb);
	return;
}


short rsnd_bb[]=
{
	RSND_BB1,			// block a Major hit
	RSND_BB2			// block a Major hit
};
void rsnd_big_block(void)
{
	next_table_sound(2,rsnd_bb);
}

short rsnd_sb[]=
{
	RSND_SB1,	// block
	RSND_SB2	// block
};
void rsnd_small_block(void)
{
	next_table_sound(2,rsnd_sb);
}

short rsnd_lsmack[]=
{
	RSND_SM1,		// 13 - face hit #1
	RSND_SM2		// 14 - face hit #2
};
void rsnd_smack(void)
{
	next_table_sound(2,rsnd_lsmack);
}

short rsnd_msmack[]=
{
	RSND_MSM1,
	RSND_MSM2,
	RSND_MSM3
};
void rsnd_med_smack(void)
{
	next_table_sound(3,rsnd_msmack);
}

short rsnd_bsmack[]=
{
	RSND_BSM1,		// 2 - Old big hit #1
	RSND_BSM2		// 3 - Old big hit #2
};
void rsnd_big_smack(void)
{
	next_table_sound(2,rsnd_bsmack);
}

short rsnd_rcks[]=
{
	RSND_ROCK1,		//      Rock #1
	RSND_ROCK2,		//      Rock #2
	RSND_ROCK3,		//      Rocks #1
	RSND_ROCK4,		//      Rocks #2
	RSND_ROCK5		//      Rocks #3
};
void rsnd_rocks(void)
{
	next_table_sound(5,rsnd_rcks);
}

void sweep_sounds(void)
{
	rsnd_big_whoosh();
	group_sound(0);
}

short rsnd_bhit[]=
{
   	RSND_BHT1, 		// 10 - body hit #1
   	RSND_BHT2		// 11 - body hit #2
};
void rsnd_body_hit(void)
{
	next_table_sound(2,rsnd_bhit);
}

short rsnd_gnd[]=
{
   	RSND_GND1,  	//2c - fall #1
   	RSND_GND2,  	//2d - fall #2
   	RSND_GND3,  	//2e - fall #3
   	RSND_GND4  	//2f - fall #4
};
void rsnd_ground(void)
{
	next_table_sound(4,rsnd_gnd);
}

short rsnd_whsh[]=
{
	RSND_WHSH1, 	//24 - whoosh #1
	RSND_WHSH2, 	//25 - whoosh #2
	RSND_WHSH3,	//26 - whoosh #3
	RSND_WHSH4 	//21 - big whoosh #2
};
void rsnd_whoosh(void)
{
	next_table_sound(4,rsnd_whsh);
}

short rsnd_bwhsh[]=
{
	RSND_BWHSH1, 	// 20 - big whoosh #1
	RSND_BWHSH2 	// 22 - big whoosh #3
};
void rsnd_big_whoosh(void)
{
	next_table_sound(2,rsnd_bwhsh);
}

short rsnd_eboom[]=
{
	RSND_EBOOM1,
	RSND_EBOOM2,
	RSND_EBOOM3
};
void rsnd_enemy_boom(void)
{
	next_table_sound(3,rsnd_eboom);
}

short rsnd_splsh[]=
{
	RSND_SPLISH1,
	RSND_SPLISH2,
	RSND_SPLISH3
};
void rsnd_splish(void)
{
	next_table_sound(3,rsnd_splsh);
}

short rsnd_stb[]=
{
	RSND_STAB1,
	RSND_STAB2,
	RSND_STAB3,
	RSND_STAB4
};
void rsnd_stab(void)
{
	next_table_sound(4,rsnd_stb);
}

short rsnd_klng[]=
{
	RSND_KLANG1,
	RSND_KLANG2
};
void rsnd_klang(void)
{
	next_table_sound(2,rsnd_klng);
}

void rsnd_react_voice(void)
{
	group_sound(6);
	return;
}

/******************************************************************************
******************************************************************************/
short triple_sndtab[]=	//-DHS- redo table
{
	TS_EXP_ST1,			// 0 - Explode thru street
	TS_EXP_ST2,			// 1 - Explode thru street
	TS_RNDHOUSE,			// 2 - roundhouse whoosh
	TS_BIGDOOR_CLOSE,		// 3 - big door closing
	TS_BIGDOOR_SLAM,		// 4 - big door slam shut
	TS_WIND,				// 5 - wind
	0x8000|TS_WHOOSH1,			// 6 - whoosh #1
	0x8000|TS_WHOOSH3,			// 7 - whoosh #3
	TS_SUB_APPROACH, 		// 8 - subway approaching
	TS_SUB_STEADY,	  		// 9 - subway steady state
	TS_SUB_GOING,	  		// a - subway going away
	TS_CROWD_OOO,	  		// b - crowd: "Ooooo"
	TS_P1_CURS,		  	// c - p1 cursor
	TS_P2_CURS,		  	// d - p2 cursor
	TS_P1_PICKED,	  		// e - p1 picked
	TS_P2_PICKED,	  		// f - p2 picked
	TRK_FIGHT,			// 10 - Shao Kahn: Fight! #3
	TRK_RND1_FIGHT,			// 11 - Shao Kahn: Round One
	TRK_RND2_FIGHT,			// 12 - Shao Kahn: Round Two
	TRK_RND3_FIGHT,			// 13 - Shao Kahn: Round Three
	TRK_RND4_FIGHT,			// 14 - Shao Kahn: Round Four
	TS_SK_FIN_HIM,			// 15 - Shao Kahn: Finish Him!
	TS_SK_FIN_HER,			// 16 - Shao Kahn: Finish Her!
	TS_CLOCK_TICK,	 		// 17 - clock time down tik !!
	0x8000|TS_FEET_LAND,	// 18 - players feet landing on ground
	TS_MAP_ZOOM,			// 19 - 1 player map zoom sound
	TS_COIN_SND,			// 1a - coin sound, -DHS- NEW SOUND
	TS_DF_TOASTY,			// 1b - DF: toasty
	TS_DF_FROSTY,			// 1c - DF: frosty
	TS_DF_CRISPY,			// 1d - DF: crispy
	0x8000|TS_SHOCK, 				// 1e - shock sound !!

	0x8000|TS_FLAME,				// 1f - flame throw
	0x8000|TS_BURNING, 			// 20 - burning flames!!
	0x8000|TS_BONES,	 			// 21 - bones fall

	TS_PIT_MACH1,			// 22 - pit machine sound
	TS_PIT_MACH2,			// 23 - pit machine sound
	0x8000|TS_CRUNCH1,				// 24 - crunch #1
	0x8000|TS_CRUNCH2,				// 25 - crunch #2
	TS_ANIM_SIZZLE,			// 26 - animality sizzle
	TS_ANIM_MORPH,			// 27 - animality morph

	TS_SK_KANO,				// 28 - Shao Kahn: Kano
	TS_SK_SONYA,			// 29 - Shao Kahn: Sonya
	TS_SK_JAX,				// 2a - Shao Kahn: Jax
	TS_SK_NIGHTWOLF, 		// 2b - Shao Kahn: Night Wolf
	TS_SK_SUBZERO,			// 2c - Shao Kahn: Sub-Zero
	TS_SK_STRYKER,			// 2d - Shao Kahn: Stryker
	TS_SK_SINDEL,			// 2e - Shao Kahn: Sindel
	TS_SK_SEKTOR,			// 2f - Shao Kahn: Sektor
	TS_SK_CYRAX,			// 30 - Shao Kahn: Cyrax
	TS_SK_LAO,				// 31 - Shao Kahn: Kung Lao
	TS_SK_KABAL,			// 32 - Shao Kahn: Kabal
	TS_SK_SHEEVA,			// 33 - Shao Kahn: Sheeva
	TS_SK_ST,				// 34 - Shao Kahn: Shang Tsung
	TS_SK_LKANG,			// 35 - Shao Kahn: Liu Kang
	TS_SK_SMOKE,			// 36 - Shao Kahn: Smoke
	TS_SK_MOTARO,			// 37 - Shao Kahn: Motaro
//37 - Shao Kahn: Kitana
//38 - Shao Kahn: Jade
//39 - Shao Kahn: Mileena
//3a - Shao Kahn: Scorpion
//3b - Shao Kahn: Reptile
//3c - Shao Kahn: Ermac
//3d - Shao Kahn: Sub-Zero
//3e - Shao Kahn: Smoke
	0,						// 38 - Shao Kahn:
	TS_SK_CHOOSE,			// 39 - Shak Kahn: Choose your destiny
	TS_ANIM_ROAR,			// 3a - Huge Animal Roar!
	TS_PLAY_BUY,			// 3b - player buys in
	TS_ANIM_SIZZLE1,		// 3c - anim sizzle 1

	TRK_KANO_WINS,						// 3d - Shao Kahn: Kano wins
	TRK_SONYA_WINS,						// 3e - Shao Kahn: Sonya wins
	TRK_JAX_WINS,						// 3f - Shao Kahn: Jax wins
	TRK_INDIAN_WINS,						// 3f - Shao Kahn: Nightwolf wins
	TRK_SUBZERO_WINS,						// 41 - Shao Kahn: Sub-Zero wins
	TRK_SWAT_WINS,						// 42 - Shao Kahn: Stryker wins
	TRK_LIA_WINS,						// 43 - Shao Kahn: Sindel wins
	TRK_ROBO1_WINS,						// 44 - Shao Kahn: Sector wins
	TRK_ROBO2_WINS,						// 45 - Shao Kahn: Cyrax wins
	TRK_LAO_WINS,						// 46 - Shao Kahn: Kung Lao wins
	TRK_TUSK_WINS,						// 47 - Shao Kahn: Cabal wins
	TRK_SHEEVA_WINS,						// 48 - Shao Kahn: Sheeva wins
	TRK_SHANG_WINS,						// 49 - Shao Kahn: Shang Tsung wins
	TRK_LIUKANG_WINS,						// 4a - Shao Kahn: Liu Kang wins
	TRK_SMOKE_WINS,						// 4d - Shao Kahn: Smoke wins
	TRK_MOTARO_WINS,						// 4c - Shao Kahn: Motaro wins
	TRK_SK_WINS,						// 4d - Shao Kahn: I wins

	TRK_KANO_MERCY,											// 4e - Shao Kahn: Kano shows mercy
	TRK_SONYA_MERCY,											// 4f - Shao Kahn: Sonya shows mercy
	TRK_JAX_MERCY,												// 50 - Shao Kahn: Jax shows mercy
	TRK_INDIAN_MERCY,										// 51 - Shao Kahn: Indian shows mercy
	TRK_SUBZERO_MERCY,										// 52 - Shao Kahn: Sub-Zero shows mercy
	TRK_SWAT_MERCY,											// 53 - Shao Kahn: Stryker shows mercy
	TRK_LIA_MERCY,												// 54 - Shao Kahn: Sindel shows mercy
	TRK_ROBO1_MERCY,											// 55 - Shao Kahn: Sektor shows mercy
	TRK_ROBO2_MERCY,											// 56 - Shao Kahn: Cyrax shows mercy
	TRK_LAO_MERCY,												// 57 - Shao Kahn: Kung Lao shows mercy
	TRK_TUSK_MERCY,											// 58 - Shao Kahn: Cabal shows mercy
	TRK_SHEEVA_MERCY,										// 59 - Shao Kahn: Sheeva shows mercy
	TRK_SHANG_MERCY,											// 5a - Shao Kahn: Shang Tsung shows mercy
	TRK_LIUKANG_MERCY,										// 5b - Shao Kahn: Liu Kang shows mercy
	TRK_SMOKE_MERCY,											// 5c - Shao Kahn: Smoke shows mercy

	TS_MUSIC_MERCY,			// 5d - mercy music hit
	TS_ROCK_MOVE,			// 5e - option screen rock move
	TS_DRAGON_SPIN,			// 5f - option screen dragon spin
	TS_ANIM_BONUS,			// 60 - animality bonus

	TS_SK_FLAWLESS,		// 61 - Shao Kahn: Flawless Victory
	TS_SK_FATALITY,		// 62 - Shao Kahn: Fatality!
	0,				//TS_SK_MERCY,		// 63 - Shao Kahn: Mercy
	TS_SK_ANIMALITY,	// 64 - Shao Kahn: Animality
	TS_SK_BABALITY,		// 65 - Shao Kahn: Babality
	TS_SK_CRISPY,		// 66 - Shao Kahn: Crispy
	TS_SK_FROSTY,		// 67 - Shao Kahn: Frosty
	TS_SK_FRIEND,		// 68 - Shao Kahn: Friendship
	TS_SK_FRIEND1,		// 69 - Shao Kahn: Friendship!?!
	TS_SK_AGAIN,		// 6a - Shao Kahn: Again?
	0,					// 6b - Shao Kahn: Toasty High
	TS_SK_TOASTYL,		// 6c - Shao Kahn: Toasty Low
	TS_SK_OMAW,			// 6d - Shao Kahn: Oh maw
	0,					// 6e
	0,					// 6f
	0x8000|TS_NASTY_GOO,		// 70 - nasty goo
	TS_DANGER,			// 71 - danger danger
	TS_FLAWLESS_TONE,	// 72

	0,					// 73 -
	TS_SK_SUBERB,		// 74 - Shao Kahn: Superb
	TS_SK_WELL_DONE,	// 75 - Shao Kahn: Well Done
	TS_SK_VS,			// 76 - Shao Kahn: ... versus ...
	TS_SK_WINS,			// 77 - Shao Kahn: ... wins
	TS_SK_SHOW_MERCY,	// 78 - Shao Kahn: ... shows mercy
	TS_SK_LAUGH1,		// 79 - Shao Kahn: Laugh #1
	TS_SK_LAUGH2,		// 7a - Shao Kahn: Laugh #2
	TS_SK_LAUGH3,		// 7b - Shao Kahn: Laugh #3

	TS_SK_EXCELLENT,			// 7c - Shao Kahn: Excellent
	TS_SK_NEVER_WIN, 			// 7d - Shao Kahn: You will never win
	TS_SK_OUTSTANDING,			// 7e - Shao Kahn: Outstanding
	TS_SK_LAST_HIT,				// 7f - Shao Kahn: Last Hit

	TS_HGAME_FIRE,				// 80 - hidden game fire
	RSND_EBOOM2,	//TS_HGAME_EXP,				// 81 - hidden game explode
	ST_SK_PATHETIC,				// 82
	TS_KLANG1,					// 83

	TS_SUB_APPROACH,				// 84 - subway approaching
	TS_SUB_STEADY,				// 85 - subway steady state
	TS_SUB_GOING,				// 86 - subway going away

	TS_FINAL_DEATH,				// 87 - Final Death Quake Loop
	TS_SK_DEATH,				// 88 - Shao Kahn Death Scream
	TS_DEATH_QUAKE,				// 89 - Final Death Quake Loop Crescendo (3x)
	0x8000|TS_DOG,						// 8a - dog
	0x8000|TS_DOG_OUCH,				// 8b - dog ouch
	0x8000|TS_BABY_POOF,				// 8c - babality poof
	0x8000|TS_BABY_CRY,				// 8d - baby cry
	TS_CHURCH_BELL,				// 8e - bell - bell
	TUNE_VERSUS,			// 8f - vs tune
	TUNE_SELECT,			// 90 - select
	TUNE_GAMEOVER,			// 91 - game over

	TRK_KANO_FWINS,					// 92 - Shao Kahn: Kano wins
	TRK_SONYA_FWINS,					// 93 - Shao Kahn: Sonya wins
	TRK_JAX_FWINS,						// 94 - Shao Kahn: Jax wins
	TRK_INDIAN_FWINS,				// 95 - Shao Kahn: Nightwolf wins
	TRK_SUBZERO_FWINS,				// 96 - Shao Kahn: Sub-Zero wins
	TRK_SWAT_FWINS,					// 97 - Shao Kahn: Stryker wins
	TRK_LIA_FWINS,						// 98 - Shao Kahn: Sindel wins
	TRK_ROBO1_FWINS,					// 99 - Shao Kahn: Sector wins
	TRK_ROBO2_FWINS,					// 9a - Shao Kahn: Cyrax wins
	TRK_LAO_FWINS,						// 9b - Shao Kahn: Kung Lao wins
	TRK_TUSK_FWINS,					// 9c - Shao Kahn: Cabal wins
	TRK_SHEEVA_FWINS,				// 9d - Shao Kahn: Sheeva wins
	TRK_SHANG_FWINS,					// 9e - Shao Kahn: Shang Tsung wins
	TRK_LIUKANG_FWINS,				// 9f - Shao Kahn: Liu Kang wins
	TRK_SMOKE_FWINS,					// a0 - Shao Kahn: Smoke wins
	TRK_MOTARO_WINS,				// a1 - Shao Kahn: Motaro wins
	TRK_SK_WINS						// a2 - Shao Kahn: I wins
};

short *triple_end = triple_sndtab+sizeof(triple_sndtab);

/******************************************************************************
 Function: void next_table_sound(WORD pa0,short *pa1)

 By: David Schwartz

 Date: Apr 1995

 Parameters: pa0 - # of entries in table
			 pa1 - ptr to table of effects

 Returns: None

 Description:	rnd pick a sound effect call from the supplied table
******************************************************************************/
#if DEBUG
WORD f_david_test=0;
#endif
void next_table_sound(WORD pa0,short *pa1)
{
	short *sndptr;

	//if (f_nosound || f_no_sfx)
	if (f_no_sfx)
		return;

	sndptr=pa1+((randu(pa0)-1));				// 1 short/sound effect

	tsnd0(sndptr,sound_pan_value());
}

void triple_sound(WORD pa0)
{
	short *sndptr;
	short snd;

	//if (f_nosound || f_no_sfx)
	if (f_no_sfx)
		return;

	sndptr=triple_sndtab+(pa0);				// get ptr to sound entry (2 short/sound effect)

	if (sndptr<triple_end)
		tsnd0(sndptr,(*sndptr & 0x8000)? sound_pan_value():64);

	return;
}

void tsnd0(short *pa0,WORD pan)
{
	TriggerPlayAttr trigger;

	trigger.mask=TRIGGER_PAN;
	trigger.pan=pan;

	/* play sound effect */
#if DEBUG
	/* trap sound calls with no load info */
	if ( f_david_test )
	{
		if (wess_dig_in_seq_sizeof(*pa0))
		{
			LOCKUP;				// breakpoiunt
		}
	}
#endif

	wess_seq_trigger_special(*pa0 & 0x7fff,&trigger);

	return;
}

/******************************************************************************
 Function: void psound_for_him(WORD offset)

 By: David Schwartz

 Date: Apr 1995

 Parameters: offset - sound offset num

 Returns: None

 Description:	lets a player make a sound call for another player by pretending he
				is him for an instance
******************************************************************************/
void psound_for_him(WORD offset)
{
	PROCESS *pa13;

	pa13=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	triple_sound(offset);
	current_proc=pa13;

	return;
}

/******************************************************************************
 Function: void nosounds(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	cancells all sound calls, zeros all priorities and kills any tunes running
******************************************************************************/
void nosounds(void)
{
	send_code_a3(0);
	return;
}

/******************************************************************************
 Function: void send_code_a3(WORD pa3)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa3 - sound effect # (0=shut up system)

 Returns: None

 Description:	play a sound effect directly or shut up the system
******************************************************************************/
void send_code_a3(WORD pa3)
{
	//if (f_nosound || f_no_sfx)

	if (pa3==0)
	{
		wess_seq_stopall();
		psxcd_stop();
	}
	else
	{
		if (!f_no_sfx)
			wess_seq_trigger(pa3);
	}
	return;
}

/******************************************************************************
 Function: void play_generic_tune(WORD trk)

 By: David Schwartz

 Date: May 1995

 Parameters: trk - track # to play

 Returns: None

 Description:	play a track off the CD
******************************************************************************/
short cd_vol_tbl[]=
{
	0,
	0x1000,
	0x2000,
	0x3cff
};
void play_generic_tune(WORD trk)
{
	//if (f_nosound || f_music==0)

	/* start da tune */
	psxcd_play_at(trk,cd_vol_tbl[f_music],2);

	return;
}

/* used for 'Sound Effects' */
void play_generic_stream(WORD trk)
{
	/* start da tune */
	if ( f_no_sfx )
		psxcd_play_at(trk,0,2);
	else psxcd_play_at(trk,cd_vol_tbl[3],2);
}

/******************************************************************************
 Function: void hob_ochar_sound(WORD hit_off,WORD block_off)

 By: David Schwartz

 Date: Jan 1995

 Parameters: hit_off (a0[31..16])
			 block_off  (a0[15..0])

 Returns: None

 Description:
******************************************************************************/
void hob_ochar_sound(WORD hit_off,WORD block_off)
{

	if (f_block)
		ochar_sound(block_off);
	else ochar_sound(hit_off);

	return;
}

/******************************************************************************
 Function: void hit_or_blocked_sound(WORD hit_off,WORD block_off)

 By: David Schwartz

 Date: Jan 1995

 Parameters: hit_off (a0[31..16])
			 block_off  (a0[15..0])

 Returns: None

 Description:
******************************************************************************/
void hit_or_blocked_sound(WORD hit_off,WORD block_off)
{

	if (f_block)
		triple_sound(block_off);
	else triple_sound(hit_off);

	return;
}

/******************************************************************************
******************************************************************************/
short st_kano[]=
{
 	ST_KANO_AX,				//0 - Kano ax
	ST_KANO_SKNIFE,			//1 - Kano spinning knife
	ST_KANO_SKNIFE_HIT,		//2 - Kano knife hit
	ST_KANO_SKNIFE_BLK,		//3 - Kano knife blocked
	ST_KANO_BALL,		  	//4 - Kano ball trajectory
	FT_KANO_LASER,		 	//5 - kano laser (borrow)
	FT_KANO_LASER_BURN,	  	//6 - kano laser burn
	FT_KANO_SKELETON_RIP,	//7 - skeleton rip (borrow)
	FT_KANO_SKIN_FALL		//8 - skin falling to ground
};

short st_sonya[]=
{
   	ST_SONYA_RINGS,		  	//0 - sonya rings
   	ST_SONYA_RINGS_HIT,		//1 - sonya rings hit
   	ST_SONYA_RINGS_BLK,	 	//2 - sonya rings blocked
   	ST_SONYA_BIKE,		 	//3 - sonya bike kick
   	ST_SONYA_FLY,			//4 - Sonya Fly
   	ST_SONYA_BIKE_VOICE,	//5 - Sonya bike kick voice (nwtowtdutb)
	FT_SONYA_BLOW_KISS,		//6 - fatal blow kiss
	FT_SONYA_QUAKE,			//7 -
};

short st_jax[]=
{
   	ST_JAX_CANNON,			//0 - Jax Cannon fire
   	ST_JAX_ZAP_HIT,			//1 - jax zap hit
   	ST_JAX_ZAP_BLK,		//2 - jax zap blocked
   	ST_JAX_BLURR,	 		//3 - jax blurr
   	ST_JAX_GROUND,	 		//4 - jax ground smash
   	ST_JAX_SLAM,			//5 - jax slamming you on ground
	ST_JAX_GOTCHA,			//6 - gotcha!!
	FT_JAX_ARM_MORPH,		//7 - arm morph
	FT_JAX_CRUNCH1,			//8 - crunch!
	FT_JAX_CRUNCH2,			//9 - crunch #1
	FT_JAX_QUAKE,			//a - death quake
	ST_JAX_BREAK,			//b - bax break
	ST_ROBO_CLANG1,			//c - robo metal ball clang 1
};

short st_subzero[]=
{
   	ST_SZ_ICE_UP,	   		//0 - ice up
   	ST_SZ_ICE_DOWN,	   	//1 - ice down
   	ST_SZ_ICE_HIT,	 	  	//2 - ice hit
   	ST_SZ_ICE_BLOCK,   	//3 - ice blocked
   	ST_SZ_ICE_HIT2,	   	//4 - ice hit
   	ST_SZ_ICE_DECOY,   	//5 - hit by decoy
   	ST_SZ_SLIDE,		 	//6 - slide
	FT_SZ_BLOW,				// 7 - blow
	FT_SZ_FROZE				// 8 - froze dude
};

short st_swat[]=
{
	ST_SWAT_BOMB_EXP,  	//0 - swat bomb explode (borrow)
	ST_SWAT_BOMB_THROW,   	//1 - swat bomb throw (borrow)
	ST_SWAT_ZOOM,			//2 - swat zoom (borrow)
	ST_SWAT_STICK,			//3 - swat stick sweep kick (borrow)
	ST_SWAT_ZOOM_THROW, 	//4 - swat zoom throw
	ST_SWAT_BANG, 			//5 - swat gun bang
	FT_SWAT_TASER, 		//6 - taser shock sound
	FT_SWAT_TASER_FIRE,  	//7 - swat taser gun FIRE !
	FT_SWAT_WHISTLE
};

short st_indian[]=
{
  	ST_IND_ARROW_APPEAR,	 	//0 - arrow appear
  	ST_IND_ARROW_FIRE,	 	//1 - arrow fire
  	ST_IND_BURN_AXE,		  	//2 - burning axe
  	ST_IND_ARROW_HIT,		//3 - arrow hit
  	ST_IND_ARROW_BLK,		//4 - arrow blocked
  	ST_IND_AXE_UP,			//5 - indian axe up
  	ST_IND_KNEE_BREAK,		//6 - knee break
  	ST_IND_REFLECT,			//7 - reflector
  	ST_IND_SHADOW_SHLD,		//8 - shadow shoulder
  	ST_IND_AXE_HIT,			//9 - axe hit
  	FT_IND_LIGHT_START,		//a - ind light start
  	FT_IND_LIGHT_LOOP,		//b - ind light loop
  	FT_IND_LIGHT_END,		//c - ind light end
  	FT_IND_LIGHT_HIT_AXE,	//d - lightning hit axe
  	FT_IND_LIGHT_HIT_HIM	//e - lightning hit him!
};

short st_lia[]=
{
   	ST_LIA_HAIR,		   	  	//0 - hair throw sound
   	ST_LIA_SCREAM,			  	//1 - scream trap !!
   	ST_LIA_FIREBALL,	   	 	//2 - lia fireball
   	ST_LIA_FIREBALL_HIT,  	 	//3 - lia fireball hit
   	ST_LIA_FIREBALL_BLOCK,	 	//4 - lia fireball blocked
   	ST_LIA_FLY_UP,			  	//5 - lia fly up
   	ST_LIA_FLY_ACROSS,		 	//6 - lia fly across
		FT_LIA_HAIR,				//7 - lia hair tornado
		FT_LIA_QUAKE,				//8 - death quake loop borrow
		FT_LIA_TWFUN				//9 - that was fun
};

#define st_robo2 st_robo1
short st_robo1[]=
{
	ST_ROBO_ARM_OUT,			 	//0 - robo arm OUT
	ST_ROBO_ARM_THROW,		 	//1 - robo arm THROW
	ST_ROBO_ARM_IN,			 	//2 - robo arm IN
	ST_ROBO_FAST_ROCKET,		 	//3 - robo fast rocket
	ST_ROBO_CRUISE_ROCKET,  	//4 - robo criuse rocket
	ST_ROBO_NET,				 	//5 - robo net throw
	ST_ROBO_ZAP_HIT,				//6 - jax zap hit
	ST_ROBO_DROP,			 	//7 - robo drop through ground tele
	ST_ROBO_CLANG1,			 	//8 - robo metal ball clang 1
	ST_ROBO_CLANG2,			 	//9 - robo metal ball clang 2
	ST_ROBO_BALL_BOOM,		 	//a - robo metal ball BOOM
	ST_ROBO_CHEST_OPEN,		 	//b - robo chest open
	ST_ROBO_TARGET,			 	//c - robo target sound
	ST_ROBO_EXPLODE,			 	//d - robo explode himself
	ST_ROBO_IMPLODE,			 	//e - robo implode himself
	ST_ROBO_SPARK1,			 	//f - robo spark #1
	ST_ROBO_SPARK2,			 	//10 - robo spark #2
	ST_ROBO_SPARK3,				//11 - robo spark #3
	ST_ROBO_SPEECH1,			 	//12 - robo speech 1
	ST_ROBO_SPEECH2,			 	//13 - robo speech 2
	ST_ROBO_SPEECH3,			 	//14 - robo speech 3
	ST_ROBO_SPEECH4,				//15 - robo speech 3
	FT_ROBO_CRSH_OUT,				//16 - crusher comes out
	FT_ROBO_CRSH_DOWN,				//17 - crusher down !
	FT_ROBO_CRSH_SMOOSH1,			//18 - crusher smoosh
	FT_ROBO_CRSH_SMOOSH2,			//19 - spear throw -DHS- NEW SOUND (FOR SPEAR CALL SMOKE)
	FT_ROBO_BEEP,					//1a - beep
	FT_ROBO_SELF_DESTRUCT,			//1b - self destruct sounds !!
	FT_ROBO_DESTRUCT_LAFF,			//1c - self destruct laugh !!
	FT_ROBO_WHIRL,					//1d -
	FT_ROBO_QUAKE,					//1e - death quake loop borrow
	ST_ROBO_SPEAR,					//1f - spear stab
	FT_ROBO_BAT,					//20 - bat sound
	FT_ROBO_SLIDE,					//21 - slide whistle
	FT_ROBO_HORN,					//22 - horn
	FT_ROBO_PROPELL,				//23 - new sound
};

short st_lao[]=
{
  	ST_LAO_ANGLE,	  	//0 - kung lao angle kick
  	ST_LAO_HAT_THROW,  	//1 - lao hat throw
  	ST_LAO_HAT_HIT,	  	//2 - lao hat hit
  	ST_LAO_HAT_BLK,	  	//3 - lao hat bloked
  	ST_LAO_TELE,		  	//4 - lao tele (ejbpatch-borrowed from robo)
  	ST_LAO_THROW,	  	//5 - lao throw
	FT_LAO_TORNADO,			//6 - lao tornado
	FT_LAO_QUAKE,			//7 - death quake loop borrow
	FT_LAO_TORNADO_HIT		//8 - lao tornado
};

short st_tusk[]=
{
   	ST_TUSK_ZAP,		 	//0 - lia fireball hit
   	ST_TUSK_CANNON,		//1 - Jax Cannon fire (photon fire)
   	ST_TUSK_BLURR,	 	//2 - blurr (borrwed)
   	ST_TUSK_FLIP1,	 	//3 - kabal flip whoosh #1
   	ST_TUSK_FLIP2,		//4 - kabal flip whoosh #2
	ST_TUSK_SAW,			//5 - tusk saw
	ST_TUSK_REFLECT,		//6 - reflector
	FT_TUSK_BUBBLE,			//7 -
	FT_TUSK_GROW,			//8 -
	FT_TUSK_HEAD_SCREAM,	//9 - scary head scream
	FT_TUSK_BURN			//a - burning axe
};

short st_sg[]=
{
	ST_SG_TELE,		  	//0 - sg tele (borrow)
	ST_SG_POUNCE,	 	//1 - she pounce / jax slamming you on ground
	ST_SG_ZAP_HIT,		//2 - sg zap hit (borrow)
	ST_SG_ZAP,			//3 - sg zap (borrow)
	ST_SG_ZAP_START,	//4 - sg start zap (borrow)
	FT_SG_KLANG,		//5 - klang 2 (the sequel) !!
	FT_SG_SPIN			//6 - spin plate
};

short st_st[]=
{
   	ST_ST_CANNON,		//0 - Jax Cannon fire
   	ST_ST_SUMMON,	 	//1 - summon
   	ST_ST_FIRE_HIT,		//2 - fireball hit
   	ST_ST_SKULL,		//3 - sonya rings (skull fire)
   	ST_ST_MORPH,		//4 - morph
   	ST_ST_FLIP,		 	//5 - shang tsung flip tumble sound
   	ST_ST_FIRE_BLK,		//6 - fireball block
	FT_ST_SWORD,		//7 - sword rise
	FT_ST_CLOUD,		//8 - energy cloud
	FT_ST_BONES			//9 - bonz
};

short st_lk[]=
{
	ST_LK_FIRE,			//0 - lk fireball
	ST_LK_FIRE_HIT,		//1 - lk fireball hit
	ST_LK_SUPER_KICK,	//2 - super kick
	ST_LK_FIRE_BLK,		//3 - lk fireball blocked
	ST_LK_BIKE,		 	//4 - bike kick jibberish
	ST_LK_FLY_YELL,		//5 - flying kick yell
	FT_LK_FLAME_MORPH,	//6 - flame morph
	FT_LK_RUN,			//7 - kang: run (borrow)
	FT_LK_SLAM			//8 - jax slamming you on ground
};

short st_motaro[]=
{
  	ST_MOT_EFF1, 			//0 -
  	ST_MOT_EFF2, 			//1 -
  	ST_MOT_EFF3, 			//2 -
  	ST_MOT_HIT1, 			//3 - motaro hit 1
  	ST_MOT_HIT2, 			//4 - motaro hit 2
	ST_MOT_FIRE				//5 - fire
};

short st_sk[]=
{
	ST_SK_SHOULDER,		//0 - shadow shoulder (borrow)
	ST_SK_PHOTON,		//1 - fire photon (borrow from kabal)
	ST_SK_KLANG1,		//2 - klang 1 !!
	ST_SK_KLANG2,		//3 - klang 2 (the sequel) !!
	ST_SK_LAUGH1,		//4 - Shao Kahn: Laugh #1
	ST_SK_LAUGH2,		//5 - Shao Kahn: Laugh #2
	ST_SK_LAUGH3,		//6 - Shao Kahn: Laugh #3
	ST_SK_PATHETIC,		//7 - Shao Kahn: That was Pathetic
	ST_SK_NEVER_WIN,		//8 - Shao Kahn: You will never win
	ST_SK_STILL_TRYING,		//9 - Shao Kahn: You're still trying to win?
	ST_SK_DONT_LAUGH		//a - Shao Kahn: Don't make me laugh
};

short st_nj[]=		
{
	ST_SCORP_COME,			// 0 - Scorpion: Come here !!
	ST_SCORP_GET,			// 1 - Scorpion: Get over here !!
	ST_SCORP_SPEAR	,			// 2 - spear throw
	ST_OSZ_IBLOCK,			// 3 - ice blocked
	ST_OSZ_ICEHIT,			// 4 - ice hit
	ST_REP_EXP,			// 5 - orb explode
	ST_REP_SLIDE,			// 6 - slide
	ST_REP_INVISO,			// 7 - inviso
	ST_REP_SPIT,			// 8 - spit
	ST_ERMAC_POOF,			// 9 - inviso poof
	ST_SCORP_MASK,			// a - remove mask !!!
	ST_OSZ_ICEUP,			// b - ice up
	ST_SCORP_TELE,			// c - scorp tele sound
	ST_ERMAC_HIT,			// d - ermac zap hit (borrow)
	ST_OSZ_FLOOR,			// e - floor freeze
	ST_ERMAC_FLOOR,			// f - ermac floor rumble
	ST_REP_SIZZLE,			// 10 - acid sizzle hit
	ST_SCORP_STAB,			// 11 - spear stab
	ST_REP_OBJ,			// 12 - orb move
	ST_REP_VOMIT,			// 13 - vomit start sound
};

short st_fn[]=		
{
	ST_KIT_THROW,			//  0 - throw fan
	ST_KIT_FANH,			//  1 - fan hit
	ST_KIT_FANL,			//  2 - fan lift
	ST_KIT_KISS,			//  3 - Sonya blow kiss borrow
	ST_JADE_ARROW,			//  4 - arrow appear
	ST_JADE_ZAP,			//  5 - jade zap hit
	ST_JADE_PROP,			//  6 - jade prop sound
	ST_JADE_TELE,			//  7 - sg tele
	ST_MIL_SAI	,			//  8 - mileena sai throw
	ST_MIL_HIT	,			//  9 - mileena sai hit
	ST_MIL_ROLL	,			//  a - mileena roll
	ST_MIL_CLOUD,			//  b - energy cloud
	ST_JADE_FLASH,			//  c - jade flashy
};

short *ochar_sound_tables[]=
{
	st_kano,
	st_sonya,
	st_jax,
	st_indian,
	st_subzero,
	st_swat,
	st_lia,
	st_robo1,
	st_robo2,
	st_lao,
	st_tusk,
	st_sg,
	st_st,
	st_lk,
	st_robo2,

	st_fn,
	st_fn,
	st_fn,

	st_nj,
	st_nj,
	st_nj,
	st_nj,
	st_nj,
	st_nj,

	st_motaro,
	st_sk,
};

/******************************************************************************
 Function: void ochar_sound(WORD offset)

 By: David Schwartz

 Date: Jan 1995

 Parameters: offset - offset into char data sound effect #

 Returns: None

 Description:	play a character specific sound effect #
******************************************************************************/
void ochar_sound(WORD offset)
{
	short *stbl;

	//if (f_nosound || f_no_sfx)
	if (f_no_sfx)
		return;

	stbl=ochar_sound_tables[(current_proc->pa8)->ochar]+(offset);
	tsnd0(stbl,sound_pan_value());

	return;
}

void ochar_sound_stop(WORD offset)
{
	short *stbl;

	//if (f_nosound || f_no_sfx)
	if (f_no_sfx)
		return;

	stbl=ochar_sound_tables[(current_proc->pa8)->ochar]+(offset);
	wess_seq_stop(*stbl);

	return;
}

/******************************************************************************
 Function: void his_ochar_sound(WORD offset)

 By: David Schwartz

 Date: Jan 1995

 Parameters: offset - snd effect offset within char table

 Returns: None

 Description:	act as the otherdude to get the correct sound
******************************************************************************/
void his_ochar_sound(WORD offset)
{
	WORD ta1;

	ta1=(current_proc->pa8)->ochar;
	(current_proc->pa8)->ochar=(current_proc->pdata.p_otherguy)->ochar;
	ochar_sound(offset);
	(current_proc->pa8)->ochar=ta1;
	return;
}

/******************************************************************************
 Function: void rsnd_ochar_sound(WORD pa0l,WORD pa0h)

 By: David Schwartz

 Date: Jan 1995

 Parameters: pa0l - # of entries to choose from
			 pa0h - base offset

 Returns: None

 Description:	play a rnd sound effect for a character in the correct range
******************************************************************************/
void rsnd_ochar_sound(WORD pa0l,WORD pa0h)
{
	ochar_sound(pa0h+randu(pa0l)-1);
}

/******************************************************************************
 Function: void borrow_ochar_sound(WORD pa0,WORD pa1)

 By: David Schwartz

 Date: Jan 1995

 Parameters: pa0 - snd effect offset within char table
			 pa1 - dude to borrow from

 Returns: None

 Description:	borrow sound effect from other dude
******************************************************************************/
void borrow_ochar_sound(WORD pa0,WORD pa1)
{
	WORD ta1;

	ta1=(current_proc->pa8)->ochar;
	(current_proc->pa8)->ochar=pa1;
	ochar_sound(pa0);
	(current_proc->pa8)->ochar=ta1;
	return;
}

/******************************************************************************
 Function: void sound_module_fight(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	Load Sound Select Module
******************************************************************************/
//-DHS- update this table
WORD bkgd_sound_mod[]=
{
	SOUNDLOADDATA_SKTRAIN,
	SOUNDLOADDATA_SKSTREET,
	SOUNDLOADDATA_SKBANK,
	SOUNDLOADDATA_SKROOF,
	SOUNDLOADDATA_SKPIT,
	SOUNDLOADDATA_SKBRIDGE,
	SOUNDLOADDATA_SKSOUL,
	SOUNDLOADDATA_SKBELL,
	SOUNDLOADDATA_SKCHURCH,
	SOUNDLOADDATA_SKGRAVE,
	SOUNDLOADDATA_SKPIT,
	SOUNDLOADDATA_SKHID,
	SOUNDLOADDATA_SKHID,
	SOUNDLOADDATA_SKHID,
	SOUNDLOADDATA_SKHID,
	SOUNDLOADDATA_SKHID,
	SOUNDLOADDATA_SKHID,
	SOUNDLOADDATA_SKHID,
	SOUNDLOADDATA_SKHID,
	SOUNDLOADDATA_SKHID,
	SOUNDLOADDATA_SKHID,
	SOUNDLOADDATA_SKHID,
	SOUNDLOADDATA_SKHID,
	SOUNDLOADDATA_SKHID,
	SOUNDLOADDATA_SKHID,
	SOUNDLOADDATA_SKHID,
	SOUNDLOADDATA_SKHID
};
void sound_module_fight(void)
{
	/* setup sounds for the round */
	PsxSoundLoadData(SOUNDLOADDATA_RAND);
	if ( p1_char==FT_SK || p2_char==FT_SK )
		PsxSoundLoadData(SOUNDLOADDATA_SKSPCIAL);
	else
	{
		PsxSoundLoadData(bkgd_sound_mod[curback]);
		sound_background=curback;
	}
	//PsxSoundLoadFighter1(p1_char);
	//PsxSoundLoadFighter2(p2_char);
}

/******************************************************************************
 Function: void sound_module_load(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	Load Sound Module
******************************************************************************/
//-DHS- update this table
BYTE module_table[]=
{
	SOUNDLOADDATA_SELECT,
	0,
	SOUNDLOADDATA_SMOKECOD,
	SOUNDLOADDATA_HIDGAME,
	0,		//SOUNDLOADDATA_KAHNDETH,
	SOUNDLOADDATA_LADDER,
	0,
	SOUNDLOADDATA_BUYIN,
	SOUNDLOADDATA_VS,
	SOUNDLOADDATA_GAMEOVER
};
void sound_module_load(WORD mod)
{
	//if (snd_module_status != mod && !f_nosound)
	if (snd_module_status != mod)
	{
		PsxSoundLoadClear();
		if (mod==SMOD_FIGHT)
			sound_module_fight();
		else PsxSoundLoadData(module_table[mod-1]);
		snd_module_status=mod;
	}
}
/******************************************************************************
 Function: void sound_pan_value(void)

 By: David Schwartz

 Date: Jun 1995

 Parameters: None

 Returns: None

 Description:	Compute the pan value (1-127) - 64 centered, base on pa8->center position
******************************************************************************/
WORD sound_pan_value(void)
{
	OBJECT *obj=current_proc->pa8;
	short pos;

	pos=obj->oxpos.u.intpos;	//-worldtlx.u.intpos;

	if (!(obj->oflags2 & M_NOSCROLL))
		pos-=worldtlx.u.intpos;

	if (pos<0)
		pos=0;

	if (pos>SCRRGT)
		pos=SCRRGT;

	return((WORD)32+((128*pos) / (SCRRGT/2) ) /4);	/*	  pan value
 																		   				32---------64---------96
																						 	left     center		 right
																							*/
}

void sound_driver_config(void)
{
	/* configure panning */
	wess_pan_mode_set(f_no_pan);			// set pan mode (0=off 1=l/r 2=r/l

	/* configure cd volume */
	wess_master_mus_vol_set(cd_vol_tbl[f_music]>>7);
#if _CD_VERSION_ == 1
	psxspu_set_cd_vol(cd_vol_tbl[f_music]);				// set cd music volume
	psxcd_set_stereo(f_stereo);
	psxcd_set_loop_volume(cd_vol_tbl[f_music]);		// set loop music
#endif

	/* configure stereo */

	return;
}

#if JAP_VERSION==1
typedef struct
{
	WORD	track;		// track #
	WORD	start;		// start sector
	WORD	end;		// end sector
} VOICE;

VOICE voice_tbl[]=
{

	{TRK_VSTORY,1050*0,1050*1-75},	// 0 story stuff
	{TRK_VSTORY,1050*1,1050*2-75},
	{TRK_VSTORY,1050*2,1050*3-75},
	{TRK_VSTORY,1050*3,1050*4-75},
	{TRK_VSTORY,1050*4,1050*5-75},
	{TRK_VSTORY,1050*5,1050*6-75},
	{TRK_VSTORY,1050*6,1050*7-75},
	{TRK_VSTORY,1050*7,1050*8-75},
	{TRK_VSTORY,1050*8,1050*9-75},
	{TRK_VSTORY,1050*9,1050*10-75},

	{TRK_VBIO,1200*0,1200*1},		// KANO
	{TRK_VBIO,1200*1,1200*2},   // SONYA
	{TRK_VBIO,1200*2,1200*3},		// JAX
	{TRK_VBIO,1200*3,1200*4},		// IND
	{TRK_VBIO,1200*4,1200*5},		// SZ
	{TRK_VBIO,1200*5,1200*6},		// SWAT
	{TRK_VBIO,1200*13,1200*14},		// LIA
	{TRK_VBIO,1200*6,1200*7},		// RO1
	{TRK_VBIO,1200*7,1200*8},		// RO2
	{TRK_VBIO,1200*8,1200*9},		// LAO
	{TRK_VBIO,1200*9,1200*10},	// TUSK
	{TRK_VBIO,1200*10,1200*11},	// SG
	{TRK_VBIO,1200*11,1200*12},	// ST
	{TRK_VBIO,1200*12,1200*13},	// LK

	{TRK_VEND,750*0,750*1},		// 24, GOD
	{TRK_VEND,750*1,750*2},		// SPACE
	{TRK_VEND,750*2,750*3},		// KANO
	{TRK_VEND,750*3,750*4},		// KANO
	{TRK_VEND,750*4,750*5},		// SONYA
	{TRK_VEND,750*5,750*6},		// SONYA
	{TRK_VEND,750*6,750*7},		// JAX
	{TRK_VEND,750*7,750*8},		// JAX
	{TRK_VEND,750*10,750*11},		// IND
	{TRK_VEND,750*11,750*12},		// IND
	{TRK_VEND,750*12,750*13},		// SZ
	{TRK_VEND,750*13,750*14},		// SZ
	{TRK_VEND,750*22,750*23},		// SWAT
	{TRK_VEND,750*23,750*24},		// SWAT
	{TRK_VEND,750*8,750*9},		// LIA
	{TRK_VEND,750*9,750*10},		// LIA
	{TRK_VEND,750*14,750*15},		// R1
	{TRK_VEND,750*15,750*16},		// R1
	{TRK_VEND,750*16,750*17},		// R2
	{TRK_VEND,750*17,750*18},		// R2
	{TRK_VEND,750*24,750*25},		// LAO
	{TRK_VEND,750*25,750*26},		// LAO
	{TRK_VEND,750*20,750*21},		// TUSK
	{TRK_VEND,750*21,750*22},		// TUSK
	{TRK_VEND,750*18,750*19},		// SG
	{TRK_VEND,750*19,750*20},		// SG
	{TRK_VEND,750*26,750*27},		// ST
	{TRK_VEND,750*27,750*28},		// ST
	{TRK_VEND,750*28,750*29},		// LK
	{TRK_VEND,750*29,750*30},		// LK
	{TRK_VEND,750*30,750*31},		// SMOKE
	{TRK_VEND,750*31,750*32}		// SMOKE
};

WORD start_voice(WORD voice)
{
	if (f_voice)
	{
		psxcd_play_at(voice_tbl[voice].track,cd_vol_tbl[3],voice_tbl[voice].start+2);
		return(1);
	}
	else
	{
		return(0);
	}
}

void stop_voice(WORD voice,WORD sleep)
{
	int max_delay=55*24;
	if (f_voice)
	{
		do
		{
			if (psxcd_elapsed_sectors()< voice_tbl[voice].end)
				process_sleep(1);
			else break;
		}
		while(--max_delay>0 );

		psxcd_stop();

	}
	else
	{
		if ( sleep!=0 )
			process_sleep(sleep);
	}

	return;
}
#endif
