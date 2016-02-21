/******************************************************************************
 File: mksound.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III sound Header
******************************************************************************/

#ifndef __mk_mksound_h__
#define __mk_mksound_h__

#include "mk3snd.h"

/*
 * 	DEFINITIONS
 */

/* sound priorities */
#define SP_WOOSH1	0x10*0x100
#define SP_GROUND	0x18*0x100
#define SP_BLOCK1	0x1f*0x100

#define SP_DONTIN	0x30*0x100		// 30s = dont interupt myself
#define SP_RUNRUN	0x31*0x100

#define SP_ONLY1	0x40*0x100		// 40s = special (dont overlap)
#define SP_WINDY1	0x41*0x100

#define SP_EFFECT	0x50*0x100
#define SP_HIT1		0x55*0x100
#define SP_VOICE1	0x60*0x100
#define SP_HIT2		0x61*0x100
#define SP_KAHNVC	0x62*0x100
#define SP_COIN		0x7f*0x100

/* group offsets */
#define GP_FEMALE	0
#define GP_MALE1	1
#define GP_MALE2	2
#define GP_MALE3	3
#define GP_MALE4	4
#define GP_ROBO		5
#define GP_KANG		6
#define GP_SHEEVA	 7
#define GP_SHAO_KAHN  8

/* tracks of the various tunes */
#define TRK_LADDER_STRT  			3		/* ladder start screen, ONEPLAY0.RAW */
#define TRK_ATTRACT		  			4		/* attract mode, SLOWATTR.RAW */
#define TRK_OPT_ATTRACT 			5		/* attract mode, FASTATTR.RAW */
#define TRK_VS						6		/* versus screen, VERSUSSC.RAW */
#define TRK_SELECT			 		7		/* select screen, SELECTSC.RAW */
#define TRK_BUYIN					8		/* buyin screen, BUYIN.RAW */
#define TRK_YOUWIN					9
#define TRK_BABYP1					10		/* babality ending part 1, FATAL1.RAW */
#define TRK_FATALP1					10		/* fatality ending part 1, FATAL1.RAW */
#define TRK_FATALP3 				11		/* fatality ending part 3, SKFATAL.RAW */
#define TRK_ANIMALP1				12	/* animality ending part 1, ANIMAL1.RAW */
#define TRK_ANIMALP3				13	/* animality ending part 3, ANIMALIT.RAW */
#define TRK_BABYP3					14	/* babality ending part 3, BABALITY.RAW */
#define TRK_FRIENDP1				15		/* friendship ending part 1, FRIEND1.RAW */
#define TRK_FRIENDP3				16		/* friendship ending part 3, SKFRIEND.RAW */
#define TRK_BANK					17	/* bank background, BANK.RAW */
#define TRK_THRUBANK				18		/* thru bank to city , THRUBANK.RAW */
#define TRK_PIT						19		/* the pit background, THEPIT.RAW */
#define TRK_SOUL				 	20		/* souls background, GREENFLA.RAW */
#define TRK_ROOF				 	21		/* city roof background, ROOF.RAW */
#define TRK_CHURCH					22	/* temple background, CHRUCH.RAW */
#define TRK_STREET					23		/* street background, STREET.RAW */
#define TRK_THRUSUB					24		/* thru subway to street, THRUSUBW.RAW */
#define TRK_SUBWAY					25		/* subway background, TRAIN.RAW */
#define TRK_HIDDEN					26		/* hidden background, SPECIALM.RAW */
#define TRK_GRAVE				 	27		/* graveyard background, GRAVEYAR.RAW */
#define TRK_BRIDGE					28	/* bridge background, BRIDGE.RAW */
#define TRK_KANO_WINS				33		/* all winning endings, STcharacter.RAW */
#define TRK_SONYA_WINS              30
#define TRK_JAX_WINS                35
#define TRK_INDIAN_WINS             45
#define TRK_SUBZERO_WINS            32
#define TRK_SWAT_WINS               43
#define TRK_LIA_WINS                40
#define TRK_ROBO1_WINS              44
#define TRK_ROBO2_WINS              36
#define TRK_LAO_WINS                39
#define TRK_TUSK_WINS               34
#define TRK_SHEEVA_WINS             31
#define TRK_SHANG_WINS              41
#define TRK_LIUKANG_WINS            38
#define TRK_SMOKE_WINS              37
#define TRK_MOTARO_WINS             42
#define TRK_SK_WINS                 29
#define TRK_FLAWLESS				62 //46		/* flawless victory, STFLAWLS.RAW */
#define TRK_KANO_MERCY				59	//61		/* all mercy endings, SMcharacter.RAW */
#define TRK_SONYA_MERCY             58	//60
#define TRK_JAX_MERCY               49	//50
#define TRK_INDIAN_MERCY            57	//59
#define TRK_SUBZERO_MERCY           51	//53
#define TRK_SWAT_MERCY              52	//54
#define TRK_LIA_MERCY               56	//58
#define TRK_ROBO1_MERCY             54	//56
#define TRK_ROBO2_MERCY             46	//47
#define TRK_LAO_MERCY               60	//62
#define TRK_TUSK_MERCY              55	//57
#define TRK_SHEEVA_MERCY            53	//55
#define TRK_SHANG_MERCY             47	//48
#define TRK_LIUKANG_MERCY           50	//52
#define TRK_SMOKE_MERCY             48	//49
#define TRK_MOTARO_MERCY            51

#define TRK_FIGHT					61	//79		/* fight, ROUND5.RAW */
#define TRK_RND1_FIGHT	 			33 //80		/* round 1 fight, ROUND1.RAW */
#define TRK_RND2_FIGHT	 			33 //81		/* round 2 fight, ROUND2.RAW */
#define TRK_RND3_FIGHT	 			33 //82		/* round 3 fight, ROUND3.RAW */
#define TRK_RND4_FIGHT	 			33 //83		/* round 4 fight, ROUND4.RAW */

/* wins with flawless streams */
#define TRK_KANO_FWINS			 33 //	(63-30+33)		/* all winning endings, STcharacter.RAW */
#define TRK_SONYA_FWINS      30	// 	(63-30+30)
#define TRK_JAX_FWINS        35 //	 (63-30+35)
#define TRK_INDIAN_FWINS     45 // 	(63-30+45)
#define TRK_SUBZERO_FWINS    32 //	 (63-30+32)
#define TRK_SWAT_FWINS       43 // 	(63-30+43)
#define TRK_LIA_FWINS        40 //	 (63-30+40)
#define TRK_ROBO1_FWINS      44 // 	(63-30+44)
#define TRK_ROBO2_FWINS      36 //	 (63-30+36)
#define TRK_LAO_FWINS        39 // 	(63-30+39)
#define TRK_TUSK_FWINS       34	//	 (63-30+34)
#define TRK_SHEEVA_FWINS     31	// 	(63-30+31)
#define TRK_SHANG_FWINS      41	//	 (63-30+41)
#define TRK_LIUKANG_FWINS    38	// 	(63-30+38)
#define TRK_SMOKE_FWINS      37	//	 (63-30+37)
#define TRK_MOTARO_FWINS	 	 42	// 	(63-30+42)

#if JAP_VERSION==1
#define TRK_VSTORY  32
#define TRK_VBIO		33
#define TRK_VEND    34
#endif

/* smod definitions */
#define SMOD_SELECT 0x01
#define SMOD_FIGHT 0x02
#define SMOD_REWARD 0x03
#define SMOD_HIDDEN 0x04
#define SMOD_SK_DEATH	0x5
#define SMOD_LADDER 0x6
#define SMOD_CAST		0x7
#define SMOD_BUYIN	0x8
#define SMOD_VS			0x9
#define SMOD_GAMEOVER	0xa


#if JAP_VERSION==1
#define VOICE_SKSTRY1	0			// green scrn with SK
#define VOICE_SKSTRY2	1			// lia float
#define VOICE_SKSTRY3	2			// lia & sk earth
#define VOICE_SKSTRY4	3			// red portal
#define VOICE_SKSTRY5	4			// soul suck
#define VOICE_SKSTRY6	5			// warriors
#define VOICE_SKSTRY7	6			// motaros
#define VOICE_SNSTRY	7			// sonya story
#define VOICE_SZSTRY	8			// subzero story
#define VOICE_LKSTRY	9			// liu kang and kung lao story
#define VOICE_BIOKN		10			// bios
#define VOICE_BIOSN		11
#define VOICE_BIOJX		12
#define VOICE_BIONW		13
#define VOICE_BIOSZ		14
#define VOICE_BIOSW		15
#define VOICE_BIOSI		16
#define VOICE_BIOR1		17
#define VOICE_BIOR2		18
#define VOICE_BIOKL		19
#define VOICE_BIOKA		20
#define VOICE_BIOSG		21
#define VOICE_BIOST		22
#define VOICE_BIOLK		23
#define VOICE_GOD		24			// supreme player screen
#define VOICE_SPACE		25			// space game
#define VOICE_ENDKN1		26			// endings
#define VOICE_ENDKN2		27
#define VOICE_ENDSN1		28
#define VOICE_ENDSN2		29
#define VOICE_ENDJX1		30
#define VOICE_ENDJX2		31
#define VOICE_ENDNW1		32
#define VOICE_ENDNW2		33
#define VOICE_ENDSZ1		34
#define VOICE_ENDSZ2		35
#define VOICE_ENDSW1		36
#define VOICE_ENDSW2		37
#define VOICE_ENDSI1		38
#define VOICE_ENDSI2		39
#define VOICE_ENDR11		40
#define VOICE_ENDR12		41
#define VOICE_ENDR21		42
#define VOICE_ENDR22		43
#define VOICE_ENDKL1		44
#define VOICE_ENDKL2		45
#define VOICE_ENDKA1		46
#define VOICE_ENDKA2		47
#define VOICE_ENDSG1		48
#define VOICE_ENDSG2		49
#define VOICE_ENDST1		50
#define VOICE_ENDST2		51
#define VOICE_ENDLK1		52
#define VOICE_ENDLK2		53
#define VOICE_ENDSM1		54
#define VOICE_ENDSM2		55
#endif

/*
 *	RAM
 */
__EXTERN__ LONG snd_module_status;
__EXTERN__ WORD sound_background;

/*
 *	PROTOTYPES
 */
extern short triple_sndtab[];
extern short cd_vol_tbl[];

void group_sound(WORD);
void kill_group_sound(WORD,OBJECT *);
void his_group_sound(WORD);

void rsnd_splish(void);
void rsnd_stab(void);
void rsnd_enemy_boom(void);
void rsnd_klang(void);
void rsnd_footstep(void);
void rsnd_big_block(void);
void rsnd_small_block(void);
void rsnd_smack(void);
void rsnd_med_smack(void);
void rsnd_big_smack(void);
void rsnd_rocks(void);
void sweep_sounds(void);
void rsnd_ground(void);
void rsnd_whoosh(void);
void rsnd_big_whoosh(void);
void rsnd_body_hit(void);
void rsnd_react_voice(void);

void next_table_sound(WORD pa0,short *pa1);
void triple_sound(WORD pa0);
void tsnd0(short *pa0,WORD pan);

void psound_for_him(WORD offset);
void nosounds(void);
void send_code_a3(WORD pa3);

void hob_ochar_sound(WORD hit_off,WORD block_off);
void hit_or_blocked_sound(WORD hit_off,WORD block_off);

void ochar_sound(WORD);
void ochar_sound_stop(WORD);
void his_ochar_sound(WORD offset);
void rsnd_ochar_sound(WORD pa0l,WORD pa0h);
void borrow_ochar_sound(WORD pa0,WORD pa1);

void play_generic_tune(WORD trk);
void play_generic_stream(WORD trk);

void sound_module_fight(void);
void sound_module_load(WORD mod);

void rsnd_sk_bonus_win(void);

WORD sound_pan_value(void);
void sound_driver_config(void);

#if JAP_VERSION==1
void stop_voice(WORD,WORD);
WORD start_voice(WORD);
#endif
/*
 *	MACROS
 */
#define tsound(_off) triple_sound((_off))

#define local_group_sound(_pa0) group_sound(_pa0)

#define get_group_table(_ta0) group_table[_ta0]

#endif /* __mk_mksound_h__ */


/*
JAX:
ST_JAX_CLANG	2e9	(robo_clang)

CYRAX:
FT_ROBO_PROPELL	(0x52c1)

SCORPION:
ST_SCORP_COME	528f		; come here
ST_SCORP_GET	5294		; get over here
ST_SCORP_SPEAR	27f6		; spear throw
ST_SCORP_MASK	52bc		; remov emask
ST_SCORP_TELE	375			; tele sound
ST_SCORP_STAB	e6			; spear stab

ERMAC:
ST_ERMAC_TELE	375			; tele sound
ST_ERMAC_POOF	2f3			; inviso poof
ST_ERMAC_HIT	25d			; zap hit
ST_ERMAC_FLOOR	2805		; floor rumble

REPTILE:
ST_REP_EXP		5271		; orb explode
ST_REP_SLIDE	343			; slide
ST_REP_SPIT		527b		; spit acid
ST_REP_INVISO	2f3			; inviso poof
ST_REP_SIZZLE	2891		; acid sizzle
ST_REP_OBJ		526c		; orb move
ST_REP_VOMIT	33e			; vomit sound

OLDSZ:
ST_OSZ_ICEUP	217			; ice up sound
ST_OSZ_IBLOCK	285			; ice blocked
ST_OSZ_ICEHIT	20d			; ice hit
ST_OSZ_SLIDE	343			; slide
ST_OSZ_FLOOR	5299		; floor freeze

KITANA:
ST_KIT_THROW	522b		; throw fan
ST_KIT_FANH		5230		; fan hit
ST_KIT_FANL		5235		; fan lift
ST_KIT_KISS		2d5			; kiss (sonya)

JADE:
ST_JADE_ARROW	226			; jade arrow appear
ST_JADE_ZAP		348			; jade zap hit
ST_JADE_PROP	52a8		; jade prop sound
ST_JADE_TELE	375			; sg tele
ST_JADE_FLASH	52b2

MILEENA:
ST_MIL_SAI		5249		; sai throw
ST_MIL_HIT		25d			; sai hit
ST_MIL_ROLL		339			; roll
ST_MIL_CLOUD	2841		; energy cloud
*/