/******************************************************************************
 File: mkgame.h

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Game Equates
******************************************************************************/

#ifndef __mk_game_h__
#define __mk_game_h__

/*
 *	DEFINITIONS
 */

/**************************************************************************
			  GENERAL STUFF
***************************************************************************/
#define SQS	20							/* switch queue size */
#define HQS 6							/* hit queue size */

#define PLAYER1_TYPE 0 					/* player 1 */
#define PLAYER2_TYPE 1					/* player 2 */

#define FRONT_Z	0x50
#define BACK_Z 0x4f

#define CLOSE_TO_EDGE	SCX(0x068)			/* this constitutes being close to the edge */

#define GAME_CREDITS  5					// # of credits to start

/* shang morph values */
#define MORPH_OFF		0				/* no morphs allowed */
#define MORPH_QUASI		1				/* morph to other character only */
#define MORPH_NORMAL 	2				/* morph to anyone */


/**************************************************************************
			  STACK SWITCH EQUATES
***************************************************************************/
#define SW_OPEN 0 						/* open queue value */
#define SW_CLOSE 1 						/* close queue value */

#define SW_STACK_OPEN_OFFSET 0			/* offset to open offset stack */
#define SW_STACK_CLOSE_OFFSET 32			/* offset to close offset stack */

/**************************************************************************
 bits for f_gamesflags
**************************************************************************/
#define M_GF_NOMUSIC		0x0001
#define M_GF_THROWYES		0X0002
#define M_GF_EX2ON2			0x0004

/**************************************************************************
			  PLAYER EQUATES
***************************************************************************/
#define FT_KANO			0
#define FT_SONYA		1
#define FT_JAX			2
#define FT_INDIAN		3
#define FT_SUBZERO		4
#define FT_SWAT			5
#define FT_LIA			6
#define FT_ROBO1		7
#define FT_ROBO2		8
#define FT_LAO			9
#define FT_TUSK			10
#define FT_SG			11			// she goro
#define FT_ST			12			// shang tsu
#define FT_LK			13			// liu kang
#define FT_SMOKE		14			// smoke hidden robo ninja

#define FT_KITANA	15
#define FT_JADE		16
#define FT_MILEENA	17
#define FT_SCORPION	18
#define FT_REPTILE	19
#define FT_ERMAC	20
#define FT_OLDSZ	21
#define FT_OLDSMOKE	22
#define FT_NOOB		23
#define FT_MOTARO	24
#define FT_SK	  	25


/* special equates */
#define FT_ENDUR0	0x80
#define FT_ENDUR1	0x81
#define FT_ENDUR2	0x82

#define FT_ENDUR3	0x83	// normal 2 player
#define FT_ENDUR4	0x84	// normal 2 player
#define FT_ENDUR5	0x85	// normal 2 player
#define FT_ENDUR6	0x86	// normal 2 player
#define FT_ENDUR7	0x87	// normal 2 player
#define FT_ENDUR8	0x88	// normal 2 player
#define FT_ENDUR9	0x89	// normal 2 player
#define FT_ENDURA	0x8a	// normal 2 player
#define FT_ENDURB	0x8b	// normal 2 player
#define FT_ENDURC	0x8c	// normal 2 player
#define FT_ENDURD	0x8d	// normal 2 player
#define FT_ENDURE	0x8e	// normal 2 player
#define FT_ENDURF	0x8f	// normal 2 player

#define FT_ENDUR10	0x90	// normal 3 player
#define FT_ENDUR11	0x91	// normal 3 player
#define FT_ENDUR12	0x92	// normal 3 player
#define FT_ENDUR13	0x93	// normal 3 player

#define SP_END			0xffff
#define SP_ENDUR		0x1000


/* fatality equates */
#define FAT_FATALITY	1		// fatality type #1 = normal fatality
#define FAT_PIT			2		// fatality type #2 = pit
#define FAT_ANIMALITY	3		// fatality type #3 = animality
#define FAT_FRIEND		4		// fatality type #4 = friendship
#define FAT_BABY		5		// fatality type #5 = babality

/**************************************************************************
     process flag equates
**************************************************************************/
#define PB_JOY		0				/* flag: i am a joystick controlled guy */
#define PB_FINISH	1				/* flag: i get to finish other player off! */
#define PB_REACTING	2				/* flag: i am reacting to some attack */
#define PB_SITDUCK	3				/* flag: i am a sitting duck */
#define PB_SPECIAL	4				/* flag: i am doing a special move (no doubles) */
#define PB_ALT_PAL	5				/* flag: i am using an alternate palette !! */
#define PB_CORPSE	6				/* flag: i am a wasted drone corpse */
#define PB_EMPEROR	7				/* flag: i am the emperor */
#define PB_WINGMAN	8				/* flag: i am a "wingman" */
#define PB_SHANG	9				/* flag: i originally am shang tsung */

/**************************************************************************
     process flag masks equates
**************************************************************************/
#define PM_JOY		0x0001				/* mask: i am a joystick controlled guy */
#define PM_FINISH	0x0002				/* mask: i get to finish other player off! */
#define PM_REACTING	0x0004				/* mask: i am reacting to some attack */
#define PM_SITDUCK	0x0008				/* mask: i am a sitting duck */
#define PM_SPECIAL	0x0010				/* mask: i am doing a special move (no doubles) */
#define PM_ALT_PAL	0x0020				/* mask: i am using an alternate palette !! */
#define PM_CORPSE	0x0040				/* mask: i am a wasted drone corpse */
#define PM_EMPEROR	0x0080				/* mask: i am the emperor */
#define PM_WINGMAN	0x0100				/* flag: i am a "wingman" */
#define PM_SHANG	0x2000				/* flag: i originally am shang tsung */

/**************************************************************************
			  PLAYER STATE EQUATES
***************************************************************************/
#define PS_ACTIVE		0x01			/* player state: active */
#define PS_BUYIN		0x02			/* player state: buyin */
#define PS_PSEL			0x03			/* player state: selecting a fighter */
#define PS_POST_PSEL	0x04			/* player state: selected a fighter */
/* when adding entries, add to jump table, start_pstate_jumps */

/**************************************************************************
			  GAME STATE EQUATES
***************************************************************************/
#define GS_AMODE		0x01			/* game is in attract mode */
#define GS_FIGHTING		0x02
#define GS_BUYIN		0x03
#define GS_PSEL			0x04			/* fighter select mode */
#define GS_BONUS		0x05			/* bonus count state */
#define GS_PFSETUP		0x06			/* pre-fight setup state */
#define GS_ROUND_INTRO	0x07			/* round intro */
#define GS_DIAG			0x08			/* game is in diagnostics/audits/adjustments */
#define GS_VS			0x09			/* vs screen */
#define GS_INITIALS		0x0a			/* enter initials */
#define GS_GAMEOVER		0x0b			/* game over */
#define GS_OCTOPUS		0x0c			/* octopus mode */
#define GS_POST_PSEL	0x0d			/* post player select mode */
#define GS_BARGE		0x0e			/* player barging in mode */
#define GS_SEC_INTRO	0x0f			/* intro secret */
#define GS_DEBUG		0x10			/* debug state */
#define GS_PITFALL		0x11			/* falling down the pit */
#define GS_SPACE_GAME	0x12
#define GS_UNLOCK_HID	0x13
#define GS_MODE_SELECT	0x14			/* mode select "mode" */
#define GS_OPTIONS		0x15			/* options screen state */

/* when adding entries, add to jump table, start_gstate_jumps */

/**************************************************************************
			  JOYSTICK/BUTTON EQUATES
***************************************************************************/
#define SW_HI_PUNCH	0	/* if this changes look for all occurances */
#define SW_LO_PUNCH	1
#define SW_BLOCK	2
#define SW_HI_KICK	3
#define SW_LO_KICK	4
#define SW_RUN		5
#define SW_UP		6
#define SW_DOWN		7
#define SW_LEFT		8
#define SW_RIGHT	9

/**************************************************************************
			  process id's
***************************************************************************/
#define PID_GEORGE			0x00	/* G to the N to the P */

/* constant running procs */
#define PID_P1		1			// player 1 process (be careful if these
#define PID_P2		2			// player 2 process  variables are changed)
#define PID_P3				3	// player 2 process  variables are changed)
#define PID_P4				4	// player 2 process  variables are changed)
#define PID_MASTER			5	// master game flow process
#define PID_SCROLL			6	// scroll process
#define PID_BACKG			7	// background process
#define PID_BANI			8	// background object animator
#define PID_REPELL			9	// repell players proc
#define PID_FLASHER			0x0a	// player message flasher
#define PID_P1SEL			0x0b	// player 1 select process
#define PID_P2SEL			0x0c	// player 2 select process
#define PID_P3SEL			0x0d	// player 1 select process
#define PID_P4SEL			0x0e	// player 2 select process
#define PID_CRED			0x0f	// show credits proc
#define PID_INITIALS		0x10	// enter initials proc
#define PID_AMODE			0x11	// attract mode process
#define PID_CYCLE			0x12	// color cycler
#define PID_OCT				0x13	// octopus
#define PID_TARGET1			0x14	// robo rocket target
#define PID_TARGET2			0x15	// robo rocket target
#define PID_DANGER1			0x16	/* player 1 danger proc */
#define PID_DANGER2			0x17	/* player 2 danger proc */
#define PID_SMOKER			0x18	/* proc to constantly smoke a dude */
#define PID_1_PUFF			0x19	/* 1 puff o smoke */
#define PID_PONG_SPEED		0x1a	/* pong speed up */
#define PID_GREEN_SPLASH	0x1b	/* splash in dead pool */
#define PID_AUTO_CYCLE		0x1c	/* auto select color cycler */
#define PID_DARK			0x1d	/* dark fighting proc */
#define PID_COMB			0x1e	/* combo proc */
#define PID_R1				0x1f
#define PID_ROBO_BOMB		0x20
#define PID_SWITCHEROO		0x21
#define PID_PSYCHO			0x22
#define PID_HID_TIMER		0x23	//
#define PID_WINGS1			0x24	//
#define PID_WINGS2			0x25	//
#define PID_TOTREP			0x26	//
#define PID_UKK				0x27	//
#define PID_MODE			0x28	// game mode select proc
#define PID_SFLASH			0x29	// symbol flasher
#define PID_OLDSMOKE		0x2a	// old smoke transformation proc
#define PID_OPTION			0x30	/* option screen stuff (HOME) */
#define PID_REGENERATE		0x31

/* 100 = special effects procs */
#define PID_FX				0x100	/* generic special f/x */
#define PID_FADE			0x101	/* fade proc */
#define PID_GROW			0x102	/* fx grower proc */
#define PID_SHAKER			0x103	/* earth shaker proc */
#define PID_TRAIL1			0x104	/* body trail (shadow kick) */
#define PID_SCREAM_WAVE		0x105	/* lia scream wave process */
#define PID_SHADOW1			0x106	/* shadow "borrow" proc for player 1 */
#define PID_SHADOW2			0x107	/* shadow "borrow" proc for player 2 */
#define PID_SELSPEECH		0x108	/* player select speech proc */
#define PID_MPO				0x109	/* multipart object process */
#define PID_SMOKE			0x10a	/* puff of smoke */
#define PID_EXPLODE			0x10b	/* explosion */
#define PID_BOLT			0x10c	/* robo dizzy bolt */
#define PID_POOF			0x10d	/* smoke poof proc */
#define PID_SONIC			0x10e	/* sonic wave proc */
#define PID_FORDEN			0x10f	/* forden peeker */
#define PID_BLAST			0x110	/* background blast proc */
#define PID_BONES			0x111
#define PID_ROBO_LIMB		0x112
#define PID_ROCK			0x113
#define PID_GRAVP			0x114	/* gravity piece */
#define PID_HAT_RING		0x115
#define PID_COMMENT			0x116
#define PID_COMBO_MSG		0x117
#define PID_TARGET			0x118
#define PID_FADEPAL			0x119
#define PID_FX_CHARGE		0x11a
#define PID_FBLAST			0x11b
#define PID_MERCY			0x11c
#define PID_TRAIL2			0x11d	/* body trail (shadow kick) */
#define PID_BOOM_TRAIL		0x11e	// boomerang trail
#define PID_JADE_FLASH		0x11f	// jade flasher proc
#define PID_OPTMUSIC		0x120
#define PID_OPTSOUND		0x121
#define PID_OPTCONTROL		0x122

/* 200 = procs which control prop objects */
#define PID_BLOOD			0x201			/* blood spirts */
#define PID_HEAD			0x202			/* decapped head */
#define PID_SUMMON			0x203
#define PID_DECOY1			0x204
#define PID_DECOY2			0x205
#define PID_BLADE			0x206
#define PID_DEAD_BLADE		0x207

#define PID_ENCOURAGE	0X208
#define OID_ENCOURAGE	PID_ENCOURAGE

#define PID_BLOOD_GLOB	0x209

/* 300 = switch spawned procs */
#define PID_SWITCH1			0x300			/* switch spawned process !! */
#define PID_SWITCH2			0x301			/* switch spawned process !! */

/* 400 = paused procs */
#define PID_PAUSE			0x400			/* master pause proc */
#define PID_PAUSE_FADE		0x401			/* pause fade routines */

/* 700 = player 1 projectile procs */
#define PID_PROJ1			0x700			/* generic projectile !! */
#define PID_HAT1			0x701			/* kung lao hat proc #1 */
#define PID_ORB1			0x702			/* reptile orb */
#define PID_ICE1			0x703			/* ice proc #1 */
#define PID_P1ROCKET1		0x704
#define PID_P1ROCKET2		0x705
#define PID_FAN1			0x706
#define PID_FLOOR_ICE1		0x707

/* 800 = player 2 projectile procs */
#define PID_PROJ2			0x800			/* generic projectile !! */
#define PID_HAT2			0x801			/* kung lao hat proc #2 */
#define PID_ORB2			0x802			/* reptile orb */
#define PID_ICE2			0x803			/* ice proc #2 */
#define PID_P2ROCKET1		0x804
#define PID_P2ROCKET2		0x805
#define PID_FAN2			0x706
#define PID_FLOOR_ICE2		0x707

/* 7000 = sound procs */
#define PID_VOLUME			0x7000		/* volume control proc */

/* 8000 = coin switch PIDs */
#define PID_LC				0x8c00		/* left coin	 */
#define PID_CC				0x8c02		/* center coin	 */
#define PID_RC				0x8c04		/* right coin	 */
#define PID_XC				0x8c06		/* fourth coin slot */
#define PID_SLAM			0x8c08	 	/* slam tilt process */
#define PID_COINCTR			0x8c0a		/* coin counter process */
#define PID_DBV				0x8c0c		// dbv coin in
#define PID_PRINT			0x800c		/* background printer process */
#define PID_DIAG			0x800d		/* diagnostics process */
#define PID_SECBUST			0x800e		/* sec bust proc */

/**************************************************************************
*                       object id's
**************************************************************************/
#define OID_BLOOD 		PID_BLOOD
#define OID_P1			PID_P1
#define OID_P2			PID_P2
#define OID_P3			PID_P3
#define OID_P4			PID_P4
#define OID_FX			PID_FX				/* special f/x */
#define OID_HEAD		PID_HEAD
#define OID_GROW		PID_GROW
#define OID_BONES		PID_BONES
#define OID_1_PUFF		PID_1_PUFF
#define OID_R1			PID_R1
#define OID_COMBO_MSG	PID_COMBO_MSG
#define OID_OPTMUSIC	PID_OPTMUSIC
#define OID_OPTSOUND	PID_OPTSOUND
#define OID_SHADOW1		PID_SHADOW1
#define OID_SHADOW2		PID_SHADOW2
#define OID_OPTION		PID_OPTION

/* when adding more proc's for projectiles make sure you update
* wait_for_prj routine */

#define OID_ROBO_LIMB	0x200
#define OID_BOLT		0x300

/* 700 = projectiles */
#define OID_SBZAP1		0x700				//
#define OID_SBZAP2		0x701				//
#define OID_SLOW_PROJ	0x709				// slow reptile projectile
#define OID_HAT2		0x70a				// hathead projectile

/* 800 = non-game related */
#define OID_CBOX		0x800				// collision box
#define OID_TEXT		0x801
#define OID_CURSOR1		0x802				// player 1 select cursor
#define OID_CURSOR2		0x803				// player 2 select cursor
#define OID_CURSOR3		0x804				// player 1 select cursor
#define OID_CURSOR4		0x805				// player 2 select cursor
#define OID_BUYIN		0x806				// buyin object
#define OID_VBOX		0x807
#define OID_AMODE		0x808
#define OID_PROFILE		0x809				// profiler object
#define OID_CD				0x80a

/**************************************************************************
     action equates
**************************************************************************/

/* projectile attacks */
#define ACT_KANO_ZAP	   0x01		// kano zap
#define ACT_SONYA_ZAP	   0x02		// sonya zap
#define ACT_JAX_ZAP		   0x03		// jax zap
#define ACT_JAX_ZAP2	   0x04		// jax zap
#define ACT_IND_ZAP		   0x05		// indian zap
#define ACT_SKY_ICE		   0x06		// sz zap in the sky
#define ACT_SW_ZAP		   0x07		// fire gun
#define ACT_ROBO_ZAP2	   0x08		// robo tracker rocket
#define ACT_ROBO_ZAP	   0x09		// robo zap
#define ACT_ROBO_NET	   0x0a		// projectile net
#define ACT_LIA_ANGLEZ	   0x0b		// lia angle zap !!
#define ACT_LAO_ZAP		   0x0c		// kung lao zap
#define ACT_ROBO_BOMB	   0x0d		// robo bomb
#define ACT_TUSK_ZAP	   0x0e		// tusk zap
#define ACT_FLOOR_ZAP	   0x0f		// floor zap
#define ACT_ST_ZAP1		   0x10		// shang tsung zap
#define ACT_ST_ZAP2		   0x11		// shang tsung zap
#define ACT_ST_ZAP3		   0x12		// shang tsung zap
#define ACT_LK_ZAP_HI	   0x13		//
#define ACT_LK_ZAP_LO	   0x14		//
#define ACT_LK_ZAP_AIR	   0x15		//
#define ACT_TUSK_ZAP_AIR   0x16		//
#define ACT_SG_ZAP		   0x17		//
#define ACT_SWAT_BOMB_HI   0x18		//
#define ACT_SWAT_BOMB_LO   0x19		//
#define ACT_FORWARD_ICE	   0x1a
#define ACT_LIA_FORWARD		0x1b
#define ACT_FLOOR_BLADE		0x1c
#define ACT_SPEAR			0x1d
#define ACT_KITANA_ZAP		0x1e	//
#define ACT_AIR_FAN			0x1f	//
#define ACT_JADE_ZAP		0x20	//
#define ACT_REPTILE_ORB		0x21	//
#define ACT_FAST_ORB		0x22	//
#define ACT_SPIT			0x23	// reptile spit
#define ACT_MILEENA_ZAP		0x24	//
#define ACT_MIL_AIR_ZAP		0x25	//
#define ACT_FLOOR_ICE		0x26	//
#define ACT_ERMAC_ZAP		0x27	//

/* stationary attacks */
#define ACT_ATTACKS		0x100	// action catagory: attacks
#define ACT_HIPUNCH		0x101	// trying to punch hi
#define ACT_LOPUNCH		0x102	// trying to punch low
#define ACT_HIKICK		0x103	// trying to kick high
#define ACT_LOKICK		0x104	// trying to kick low
#define ACT_RHOUSE		0x105	// trying round house kick
#define ACT_DUCK_KICKH	0x106	// duck kick
#define ACT_DUCK_KICKL	0x107	// duck kick low
#define ACT_DUCKPUNCH	0x108	// duck punch low
#define ACT_KNEE		0x109	// knee attack
#define ACT_ELBOW		0x10a	// elbow
#define ACT_JUPKICK		0x10b	// jumpup kick
#define ACT_JUPPUNCH	0x10c	// jumpup punch
#define ACT_SWEEP		0x10d	// sweep kicking
#define ACT_UPPERCUT	0x10e	// uppercut punch
#define ACT_LIA_SCREAM	0x10f	//
#define ACT_LEG_GRAB	0x110	// sonya leg grab move
#define ACT_SHAKE		0x111	// kano shaker
#define ACT_NOOGY		0x112	// noogy punch
#define ACT_QUAKE		0x113	// jax-quake
#define ACT_AXE_HORZ	0x114	// horizontal axe swipe
#define ACT_AXE_UP		0x115	// up ax
#define ACT_COMBO		0x116	// in middle of combo mode
#define ACT_KANO_SWIPE	0x117	// kano swipe up
#define ACT_SG_QUAKE	0x118
#define ACT_LAOSPIN		0x119
#define ACT_ROPE_PULL	0x11a
#define ACT_FAN_LIFT   	0x111b
#define ACT_SWAT_GUN   	0x111c
#define ACT_ERMAC_SLAM 	0x11d

/* body propelling attacks */
#define ACT_BODY_PROPELL	0x200
#define ACT_FLYKICK			0x200	// flying punch
#define ACT_FLYPUNCH		0x201	// flying punch
#define ACT_SZ_DECOY		0x202	//
#define ACT_HOVER			0x203	// lia hover move
#define ACT_SBIKE  			0x204	// sonya bike
#define ACT_JAX_DASH		0x205	// jax dash punch
#define ACT_LAO_TELE		0x206	// kung lao teleport
#define ACT_SQUARE			0x207	//
#define ACT_ROBO_TELE		0x208	// robo tele
#define ACT_ROBO_AIRGRAB	0x209	//
#define ACT_TELE_EXPLODE	0x20a	//
#define ACT_KANO_ROLL		0x20b	// kano cannonball roll attack
#define ACT_LAO_ANGLE		0x20c	//
#define ACT_BIKE			0x20d	//
#define ACT_SUPERKANG		0x20e	//
#define ACT_SG_POUNCE		0x20f	//
#define ACT_SLIDE			0x210	// slide
#define ACT_ZOOM			0x211
#define ACT_STICK_SWEEP		0x212
#define ACT_TUSK_BLUR		0x213
#define ACT_IND_CHARGE	0x214
#define ACT_SCORP_TELE		0x215
#define ACT_REPTILE_DASH	0x216
#define ACT_JADE_PROP		0x217
#define ACT_MILEENA_TELE	0x218
#define ACT_MILEENA_ROLL	0x219
#define ACT_KANO_UPBALL		0x21a

/* non-attacking actions */
#define ACT_NONATTACKS 	0x300
#define ACT_BACKUP	   	0x301	// getting backup from crouch position
#define ACT_DUCK	   	0x302	// duck
#define ACT_STANCE	   	0x303	// stance
#define ACT_LAND	   	0x304	// landing on ground
#define ACT_WALKF	   	0x305	// walk forward
#define ACT_POSTATTACK 	0x306	// just finished an attack
#define ACT_WALKB	   	0x307	// walk backward
#define ACT_ANGLE_JUMP 	0x308	// jumping at an angle
#define ACT_GETUP	   	0x309	// getup from a fall
#define ACT_JUMPUP	   	0x30a	// jump straight up
#define ACT_POUNCE		0x30b
#define ACT_INVISO		0x30c	// reptile turning invisible

/* specific actions */
#define ACT_STUNNED		0x400	// stunned
#define ACT_MORPH		0x401	// shang morph
#define ACT_REFLECT		0x402	// indian projectile reflect move
#define ACT_BACKBREAK	0x403	// jax back breaker move

/* unrelated actions */
#define ACT_PROJ_DEAD	0x500	// dead projectile
#define ACT_IGNORE_PROJ	0x501	// ignoring projectile
#define ACT_DEAD		0x502	// i am a dead man
#define ACT_REACTING	0x503	// reacting to a hit
#define ACT_BODYSLAM	0x504	// body slam
#define ACT_PROJ_PASSED	0x505	// passed projectile
#define ACT_LEGGED		0x506
#define ACT_REACT_FLIPK	0x507
#define ACT_PITTING		0x508	// knocking someone into the pit
#define ACT_PUNCHED		0x509
#define ACT_POST_AIRSTK	0x50a

/* sitting duck states */
#define ACT_SWEEP_SD	0x600	// sweep sitting duck mode
#define ACT_KNEE_SD		0x601	// post knee sitting duck
#define ACT_KROLL_SD	0x602	// sitting duck: kano roll
#define ACT_UPCUT_SD	0x603	// post uppercut sitting duck
#define ACT_PROJ_SD		0x604	// post proj thrower sitting duck
#define ACT_ICHARGE_SD	0x605	// indian charge sitting duck
#define ACT_UPCUTTED	0x606	// flying in the air uppercutted
#define ACT_CAUGHT_NET	0x607
#define ACT_LEG_SD		0x608	// sonya leg grab duck
#define ACT_HITFALL_SD	0x609	// falling from a hit sitting duck
#define ACT_ELBOW_SD	0x60a	// post elbow sitting duck
#define ACT_RET_KICK	0x60b	// retract a kick
#define ACT_FK_SD		0x60c	// flip kick sitting duck
#define ACT_SPIN_SD		0x60d	// post spin sitting duck
#define ACT_REF_SD		0x60e	// reflect sitting duck
#define ACT_LAO_SD		0x60f	// kung lao superkick sd
#define ACT_FROZEN		0x610	// frozen
#define ACT_SBIKE_SD	0x611	// sonya bike sitting duck
#define ACT_SUPERK_SD	0x612	//
#define ACT_SLIDE_SD	0x613	//
#define ACT_STSW_SD		0x614	// stick sweep sitting duk !!
#define ACT_SCREAMED	0x615	// caught in lia scream
#define ACT_ROBO_TELED	0x616	//
#define ACT_BLURRED		0x617	//
#define ACT_KSWIPE_SD		0x618	// kano swipe sd
#define ACT_AIRPUNCHED		0x619	//
#define ACT_SBIKE_KICKED	0x61a	//
#define ACT_NOOGY_SD		0x61b	//
#define ACT_SSD				0x61c	//
#define ACT_LK_BIKE_SD		0x61d	//
#define ACT_ZOOM_SD			0x61e	// kabal zoom sitting duck
#define ACT_TUSK_BLUR_SD	0x61f	//
#define ACT_DIZZY			0x620	//
#define ACT_SG_QUAKE_SD		0x621	//
#define ACT_ROBO_TELE_SD	0x622
#define ACT_SPEARED_SD		0x623
#define ACT_WAVE_SD			0x624	//
#define ACT_ORBBED			0x625	//
#define ACT_SPIT_ON			0x626	//
#define ACT_MIL_TELE_SD		0x627	// mileena tele sd
#define ACT_SLIPPING		0x628	//
#define ACT_SWAT_GUN_SD		0x629	//
#define ACT_ER_SLAM_SD		0x62a	//
#define ACT_LAO_SPINNED		0x62b	// lao spinned
#define ACT_JAXDASH_SD		0x62c	//
#define ACT_UPBALL_SD		0x62d
#define ACT_SLAM_BOUNCE		0x62e
#define ACT_SG_PUNCE_SD		0x62f	//

/* blocks */
#define ACT_BLOCKS		0x700
#define ACT_BLOCKHI		0x700				// trying to block high
#define ACT_BLOCKLO		0x701				// trying to duck block

// when adding more, add entries to the tables in drone.asm

/*
 *	MORTAL KOMBAT GAME SPECIFIC RAM
 */
#if CD_DEBUG
__EXTERN__ WORD f_cdbug;
__EXTERN__ WORD g_marker;
__EXTERN__ WORD g_place;
#endif
__EXTERN__ WORD p1_preload;
__EXTERN__ WORD bkgd_preload;

__EXTERN__ WORD f_no_bonus;				// prevent async bug that can occur, if set bonus routine delayed until fatal stuff say go
__EXTERN__ WORD f_freeplay;				// set if game is in freeplay
__EXTERN__ WORD credits;					// # of credits in game

__EXTERN__ WORD	gstate;					/* game state variable */
__EXTERN__ WORD	switch_escape;

__EXTERN__ WORD coinflag;				/* on coin page, yet */
__EXTERN__ WORD on_hstd;				/* on high score apge on not */

__EXTERN__ WORD b0;						// emul reg b0
__EXTERN__ WORD b1;						// emul reg b1

/* PLAYER 1 RAM */
__EXTERN__ WORD p3_char;
__EXTERN__ WORD p4_char;

__EXTERN__ WORD	p1_state;				/* player 1 state */
__EXTERN__ WORD p1_heap_char;			/* char # associated with heap */
__EXTERN__ ADDRESS p1_shape;
__EXTERN__ OBJECT *p1_obj;				/* player 1 object */
__EXTERN__ WORD *p1_button;				/* player 1 button table ptr */
__EXTERN__ PROCESS *p1_proc;			/* player 1 process */
__EXTERN__ WORD p1_char;
__EXTERN__ WORD p1_name_char;
__EXTERN__ POS p1_xvel;				/* player 1 requested xvel */
__EXTERN__ WORD p1_bar;					/* player 1 strength bar */
__EXTERN__ WORD p1_turbo;				/* player 1 turbo bar */
__EXTERN__ WORD p1_perfect;
__EXTERN__ WORD p1_matchw;				/* player 1 wins this match */
__EXTERN__ POS	p1_map;					/* player 1 map position */
__EXTERN__ LONG p1_bcq[SQS+1];			/* player 1 button close queue */
__EXTERN__ LONG p1_jcq[SQS+1];			/* player 1 joystick close queue */
__EXTERN__ LONG p1_boq[SQS+1];			/* player 1 button open queue */
__EXTERN__ LONG p1_joq[SQS+1];			/* player 1 joystick open queue */
__EXTERN__ WORD p1_dont_raise;

/* PLAYER 2 RAM */
__EXTERN__ WORD	p2_state;				/* player 2 state */
__EXTERN__ WORD p2_heap_char;			/* char # associated with heap */
__EXTERN__ ADDRESS p2_shape;
__EXTERN__ OBJECT *p2_obj;				/* player 2 object */
__EXTERN__ WORD *p2_button;			/* player 2 button table ptr */
__EXTERN__ PROCESS *p2_proc;			/* player 2 process */
__EXTERN__ WORD p2_char;
__EXTERN__ WORD p2_name_char;
__EXTERN__ POS p2_xvel;				/* player 2 requested xvel */
__EXTERN__ WORD p2_bar;					/* player 2 strength bar */
__EXTERN__ WORD p2_turbo;			/* player 2 turbo bar */
__EXTERN__ WORD p2_perfect;
__EXTERN__ WORD p2_matchw;				/* player 2 wins this match */
__EXTERN__ POS	p2_map;					/* player 2 map position */
__EXTERN__ LONG p2_bcq[SQS+1];			/* player 2 button close queue */
__EXTERN__ LONG p2_jcq[SQS+1];			/* player 2 joystick close queue */
__EXTERN__ LONG p2_boq[SQS+1];			/* player 2 button open queue */
__EXTERN__ LONG p2_joq[SQS+1];			/* player 2 joystick open queue */
__EXTERN__ WORD p2_dont_raise;

/* GAME VARIABLES AND FLAGS */
__EXTERN__ short f_shadow_fighting;			// shadowing fighting
__EXTERN__ short f_p1_half_damage;		// p1 inflicts half damage
__EXTERN__ short f_p2_half_damage;		// p2 inflicts half damage
__EXTERN__ short f_no_special_moves;	// disable speical moves
__EXTERN__ short f_no_vs;							// disable vs screen coming up
__EXTERN__ short f_superjumps;			// allow long super jumps
__EXTERN__ short f_no_combos;			// disable combos
__EXTERN__ short f_no_clock;				// if set clock disabled
__EXTERN__ short f_shang_morph;			/* shang morph, 0=NONE, 1=QUASI, 2=NORMAL */
__EXTERN__ short f_one_win;				/* need only 1 one to move on */
__EXTERN__ short f_unlim_fatal;			/* infinite time to comit fatalities */
__EXTERN__ short f_level_select;		/* if set allow level select */
__EXTERN__ short f_cheat_menu;			/* allow cheat menu features */
__EXTERN__ short f_pause;				/* flag: TRUE: game paused */
__EXTERN__ short f_nosound;				/* TRUE, sound disabled */
__EXTERN__ short f_timeout;				/* flag: round timeout */
__EXTERN__ short f_doscore;				/* flag: display scores/bars/timer */
__EXTERN__ short f_death;				/* flag: death blow achieved */
__EXTERN__ short f_norepell;				/* flag: don't repell players */
__EXTERN__ short f_start;				/* flag: start fightin' */
__EXTERN__ short f_auto_erase;			/* flag: do auto erase */
__EXTERN__ short f_novel;				/* flag: no velocities */
__EXTERN__ short f_warnsound;			/* flag: warning sound has been made */
__EXTERN__ short f_nopmsg;				/* flag: dont print player message */
__EXTERN__ short f_start_pressed;		/* flag: a start button was pressed */
__EXTERN__ short f_hey;					/* jon hey yell short */
__EXTERN__ short f_no_violence;			/* flag: no violence */
__EXTERN__ short f_no_blood;				/* flag: no blood */
__EXTERN__ short f_fade;					/* flag: background is faded */
__EXTERN__ short f_thatsall;				/* flag: thats all, round is over! */
__EXTERN__ short f_shadows;				/* flag: do shadows */
__EXTERN__ short f_show_ranking;			/* flag: show rankings */
__EXTERN__ short f_no_pan;				/* flag: panning on=0,off=1 */
__EXTERN__ short f_music;			/* flag: music silent=0, quiet=1 rockin=2 kranked=3 */

__EXTERN__ short f_mileena;
__EXTERN__ short f_ermac;
__EXTERN__ short f_oldsz;

#if JAP_VERSION
__EXTERN__ short f_voice;			/* flag: is voice enabled */
#endif

__EXTERN__ short f_no_sfx;				/* flag: sound effect on=0, off=1*/
__EXTERN__ short f_stereo;				/* flag: stereo=0, mono=1 */

__EXTERN__ WORD curback;				/* current background */
__EXTERN__ short cmos_diff;				/* current game difficulty */
__EXTERN__ short diff;				/* current game difficulty */
__EXTERN__ short perform;				/* human performance vs drone */
__EXTERN__ WORD round_num;				/* round # */
__EXTERN__ WORD winner_status;			/* 1=player,2=2, 3=finish him */
__EXTERN__ WORD *map_start;				/* ptr to mountain map data to use */
__EXTERN__ WORD map_position;			/* current position in mountain map */
__EXTERN__ WORD battle_num;				/* current battle we are fighting */
__EXTERN__ WORD p1_wiar;				/* player 1 wins in a row */
__EXTERN__ WORD p2_wiar;				/* player 2 wins in a row */
__EXTERN__ WORD p1_rwon;				/* player 1 total rounds won */
__EXTERN__ WORD p2_rwon;				/* player 2 total rounds won */
__EXTERN__ WORD p1_hitq[HQS];				/* player 1 hit queue */
__EXTERN__ WORD p2_hitq[HQS];				/* player 2 hit queue */
__EXTERN__ WORD silhoette;				/* matches sans silhoette */

__EXTERN__ WORD c_three;				/* counter: threes */
__EXTERN__ WORD c_drone_kill;			/* count: drone kill count down */
__EXTERN__ WORD c_amodeloop;			/* counter: attact mode looper */
//__EXTERN__ WORD c_amode_bio;			/* counter: attract mode biography */
__EXTERN__ WORD f_secret;

/*
* time markers for special events
*/
__EXTERN__ WORD	l_hp[2];					/* p1:p2 last high punch */
__EXTERN__ WORD	l_lp[2];					/* p1:p2 last low punch */
__EXTERN__ WORD	l_block[2];				/* p1:p2 block */
__EXTERN__ WORD	l_hk[2];					/* p1:p2 high kick */
__EXTERN__ WORD	l_lk[2];					/* p1:p2 low kick */
__EXTERN__ WORD	l_up[2];					/* p1:p2 up */
__EXTERN__ WORD	l_down[2];					/* p1:p2 down */
__EXTERN__ WORD	l_left[2];					/* p1:p2 left */
__EXTERN__ WORD	l_right[2];					/* p1:p2 right */
__EXTERN__ WORD	l_run[2];					/* p1:p2 run */

/* combo timers */
__EXTERN__ WORD	c_hp[2];					/* p1:p2 last high punch */
__EXTERN__ WORD	c_lp[2];					/* p1:p2 last low punch */
__EXTERN__ WORD	c_block[2];				/* p1:p2 block */
__EXTERN__ WORD	c_hk[2];					/* p1:p2 high kick */
__EXTERN__ WORD	c_lk[2];					/* p1:p2 low kick */
__EXTERN__ WORD	c_up[2];					/* p1:p2 up */
__EXTERN__ WORD	c_down[2];					/* p1:p2 down */
__EXTERN__ WORD	c_left[2];					/* p1:p2 left */
__EXTERN__ WORD	c_right[2];					/* p1:p2 right */
__EXTERN__ WORD	c_run[2];					/* p1:p2 run */


__EXTERN__ WORD	c_p1p2_fastblk[2];		/* p1:p2 fast block counter */

__EXTERN__ WORD	c_kahn_dummy;			/* kahn dummy counter */
__EXTERN__ WORD	c_goro_dummy;			/* goro dummy counter */
__EXTERN__ WORD	c_sk_taunt;				/* sk taunt counter */
__EXTERN__ WORD	f_final_act;			/* flag: death blow achieved */
__EXTERN__ WORD c_1p_tries;				/* counter: 1 player tries */

__EXTERN__ short df_fastrun;
__EXTERN__ short df_nopower;
__EXTERN__ short df_p1joy;
__EXTERN__ short df_p2joy;
__EXTERN__ short df_quiet;
__EXTERN__ short df_nodamage;

__EXTERN__ WORD f_colbox;				/* flag: show collision boxes (DEBUGGING) */

__EXTERN__ WORD f_block;				/* flag: set if strike check detects a block */

__EXTERN__ WORD p1_bar_view;			/* bar size viewable */
__EXTERN__ WORD p2_bar_view;			/* bar size viewable */
__EXTERN__ WORD p2_bar_xpos;			/* right justify pos for bars */

__EXTERN__ WORD p1_turbo_view;			/* turbo size viewable */
__EXTERN__ WORD p2_turbo_view;			/* turbo size viewable */
__EXTERN__ WORD p2_turbo_xpos;			/* right justify pos for turbos */

__EXTERN__ short clk_tens;				/* clock tens digit */
__EXTERN__ short clk_ones;				/* clock ones digit */

__EXTERN__ short p1_shadadj;				/* player 1 shadow y adjustment ! */
__EXTERN__ short p2_shadadj;				/* player 2 shadow y adjustment ! */

__EXTERN__ WORD displayon;				/* display on or off */

__EXTERN__ WORD shadow_clt;				/* clt id for shadows */


__EXTERN__ WORD f_sans_throws;			// throwing disabled
__EXTERN__ short kode_offset;
__EXTERN__ WORD f_sans_block;			// blocking disabled
__EXTERN__ WORD f_mercy;				// mercy

/* zeroed every round */
__EXTERN__ WORD l_sz_decoy[2];			/* p1:p2 last subzero decoy freeze */
__EXTERN__ WORD l_morph[2];				/* p1:p2 last morph */
__EXTERN__ WORD l_slide[2];				/* p1:p2 last slide */

__EXTERN__ WORD l_liazap[2];
__EXTERN__ WORD l_net[2];
__EXTERN__ WORD l_angle[2];
__EXTERN__ WORD f_aabuse;
__EXTERN__ WORD f_upcut_rec;
__EXTERN__ WORD f_unlim_run;
__EXTERN__ WORD lem4[2];
__EXTERN__ WORD l_jaxzap2[2];
__EXTERN__ WORD l_throw_fan[2];
__EXTERN__ WORD l_decoy[2];
__EXTERN__ WORD l_flash[2];
__EXTERN__ WORD l_spear[2];
__EXTERN__ WORD l_block_fk[2];
__EXTERN__ WORD l_mileena_roll[2];
__EXTERN__ WORD l_puddle[2];
__EXTERN__ LONG f_gameflags;
__EXTERN__ WORD l_swat_gun[2];
__EXTERN__ WORD l_ermac_slam[2];
__EXTERN__ WORD l_orb_slow[2];
__EXTERN__ WORD l_orb_fast[2];
__EXTERN__ WORD l_zap[2];

__EXTERN__ WORD room7[2];
__EXTERN__ WORD room8[2];

__EXTERN__ WORD f_dark;
__EXTERN__ WORD f_smoke;
//round_zero_end,0,1

__EXTERN__ WORD mode_of_play;
__EXTERN__ WORD p1_tour[16];			// tournament players (16 slots)
__EXTERN__ WORD twinners[7];			// tounrament winners
__EXTERN__ WORD c_curback;				// counter for current background


__EXTERN__ WORD comboram[10];			// combination lock ram
#define combo_1 (comboram[0])			// combination lock ram 1
#define combo_2 (comboram[1])			// combination lock ram 2
#define combo_3 (comboram[2])			// combination lock ram 3
#define combo_4 (comboram[3])			// combination lock ram 4
#define combo_5 (comboram[4])			// combination lock ram 5
#define combo_6 (comboram[5])			// combination lock ram 6
#define combo_7 (comboram[6])			// combination lock ram 6
#define combo_8 (comboram[7])			// combination lock ram 7
#define combo_9 (comboram[8])			// combination lock ram 8
#define combo_10 (comboram[9])			// combination lock ram 9

#endif /* __mk_game_h__ */
