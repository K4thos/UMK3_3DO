/******************************************************************************
 File: mkopt.h

 By: David Schwartz

 Date: Apr 1995

 (C) Williams Entertainment

 Mortal Kombat III Options Header
******************************************************************************/

#ifndef __mk_mkopt_h__
#define __mk_mkopt_h__

/*
 * 	DEFINITIONS
 */
#define OPT_START	0x00			// option - start the game
#define OPT_GAME	0x01			// option - game setup
#define OPT_CONTROL	0x02			// option - joy configuration
#define OPT_MUSIC	0x03			// option - music controls
#define OPT_MAX_SEL	0x03			// max # of option selections
#define OPT_CREDIT 0x04 		// option - secret credits

/* cube controls */
#define OPT_CUBE_X	114
#define OPT_CUBE_Y	130

#define CUBE_SPIN	0x3

/* dragon controls */
#define DRAGON_SPIN		0x00			// continue spinning
#define DRAGON_FINISH	0x01			// hurry up and finish
#define DRAGON_STOP		0x02			// stop moving

#define DRAGON_SPIN_SLOW	0x03		// normal spin speed
#define DRAGON_SPIN_FAST	0x01		// fast spin speed

#define OPT_DRAGON_X	98
#define OPT_DRAGON_Y	36

/* start of screen positions */
#define OPT_TOP_X_L		16
#define OPT_TOP_Y		96
#define OPT_TOP_X_C		238
#define OPT_EXIT_X	138
#define OPT_CC_X_L  152
#define OPT_CC_X1_C  60
#define OPT_CC_X2_C  (SCRRGT-60)

/* option joy control */
#define OPTION_EXIT P1P2_ACTION
#define OPT_P1MASK		(0x0000ffff)
#define OPT_P2MASK		(0xffff0000)

/* option screen index */
#define OPT_SCRN_GAME			0
#define OPT_SCRN_MUSIC			1
#define OPT_SCRN_CONTROLS		2
#define OPT_SCRN_CONTROLS2	3
#define OPT_SCRN_CHEAT		4
#define OPT_SCRN_CHEAT1		5

/* Indexes into option text */
#define OIND_VIOLENCE			0x00
#define OIND_ON					0x01
#define OIND_OFF				0x02
#define OIND_BLOOD				0x03
#define OIND_DIFF				0x04
#define OIND_MED				0x05
#define OIND_VEASY				0x06
#define OIND_EASY				0x07
#define OIND_VHARD				0x08
#define OIND_HARD				0x09
//#define OIND_RGB				0x0a
//#define OIND_NTSC				0x0b
#define OIND_MONO				0x0c
#define OIND_STEREO				0x0d
#define OIND_EXIT				0x0e
#define OIND_MASTER_VC			0x0f
//#define OIND_MUSIC_TEST			0x18
#define OIND_MUSIC				0x11
#define OIND_SOUND				0x12
//#define OIND_SOUND_TEST			0x17
#define OIND_SOUND_FX			0x14
#define OIND_PAN				0x15
#define OIND_MONITOR			0x16
#define OIND_SILENT				0x19
#define OIND_QUIET				0x1a
#define OIND_ROCKIN				0x1b
#define OIND_KRANKED			0x1c
#define OIND_HP					0x1d
#define OIND_LP					0x1e
#define OIND_HK					0x1f
#define OIND_LK					0x20
#define OIND_RUN				0x21
#define OIND_BLOCK				0x22
#define OIND_SQUARE			0x23
#define OIND_TRIANGLE   0x24
#define OIND_CIRCLE     0x25
#define OIND_X          0x26
#define OIND_L1         0x27
#define OIND_L2         0x28
#define OIND_R1         0x29
#define OIND_R2         0x2a
#define OIND_NOTASSIGN	0x2b
#define OIND_SHANG		0x2c		// shang morph
#define OIND_DISABLED	0x2d
#define OIND_OPPONENT	0x2e
#define OIND_ALL		0x2f
#define OIND_CLOCK		0x30
#define OIND_FREEPLAY	0x31
#define OIND_SMOKE		0x32
#define OIND_ENABLED	0x33
#define OIND_LR			0x34
#define OIND_RL			0x35
#define OIND_FATAL_TIME	0x36
#define OIND_ROUND		0x37
#define OIND_MATCH_DEAD	0x38
#define OIND_SELECT		0x39
#define OIND_VS				0x3a
#define OIND_VOICE		0x3b

#define OIND_END				0xff

/* valid buttons for SONY PSX */
#define M_TRI			0x0010
#define M_X				0x0040
#define M_CIRCLE	0x0020
#define M_SQUARE	0x0080
#define M_L2			0x0002
#define M_L1			0x0008
#define M_R2			0x0000
#define M_R1			0x0004

#define B_TRI			4
#define B_X       6
#define B_CIRCLE  5
#define B_SQUARE  7
#define B_L1      3
#define B_L2      1
#define B_R1      2
#define B_R2      0

/* opt control lookup values */
#define OCL_HP			0
#define OCL_LP      1
#define OCL_HK      2
#define OCL_LK      3
#define OCL_RUN     4
#define OCL_BLOCK   5
#define OCL_OFF     6

/* list structures for options */
typedef struct optlst
{
	OBJECT *lobj;				// label object
	OBJECT *cobj;				// choice object
	WORD *var;					// ptr to varible that will store results
	short y;					// line text is located on
	short xlabel;				// x position of label
	short xchoice;				// x posiiton of choices
	WORD lab_index;				// label index into graphics info list
	BYTE *choicetbl;			// ptr to a list of choices
	FUNC toggle_func;		// toggle function
	WORD index;					// index into choice table
} OPTLIST;

typedef struct optpage
{
	BYTE u0,v0;						// offset within tpage (u0 even)
	BYTE w,h;						// width & height of sprite
} OPTPAGE;

typedef struct optmap
{
	WORD b_index;				// button index value  (hardware bit value)
	WORD *storage;			// variable to store remap
} OPTMAP;

/* color scheme */
#define OPT_NORMAL_COLOR  optextbg_PAL
#define OPT_HILITE_COLOR	YELLOW_p

/* active process for test */
#define OPT_ACTIVE_NONE			0x00
#define OPT_ACTIVE_MUSIC		PID_OPTMUSIC
#define OPT_ACTIVE_SOUND		PID_OPTSOUND

#define OPT_MAX_MUSIC			5							// max # of songs
#define OPT_MAX_SOUND			50						// max # of sfx

/*
 *	RAM
 */
__EXTERN__ WORD option_tpage;
__EXTERN__ WORD opt_mnum;						// current music #
__EXTERN__ WORD opt_snum;						// sound #
__EXTERN__ WORD opt_active;					// (0=none active, 1=music active, 2=sound active)
__EXTERN__ WORD opt_done;				// flag: option select done

/*
 *	PROTOTYPES
 */
void opt_main(void);
void opt_dragon(void);
void opt_logo(void);

void option_select(WORD scrnindex);
void option_toggle(void);
void option_exit(void);
void option_test_toggle(WORD aflag,WORD max);
void option_mtest(void);
void option_stest(void);
void soundtest_proc(void);
void option_toggle_sound(void);

WORD init_option_table(OPTLIST *otbl);
OBJECT *get_option_object(short x,short y,WORD lindex);
void opt_disp_num(WORD sndnum,WORD x,WORD y,WORD id);
void opt_pad_setup(OPTMAP *omap, WORD *padmap);
void opt_pad_remap(OPTMAP *omap, WORD *padmap);
void opt_proc(void);
void opt_p2control(void);

/*
 *	MACROS
 */
#define center_opt_obj(_ta8) (_ta8)->oxpos.u.intpos=(_ta8)->oxvel.u.intpos-(_ta8)->header.t_width/2;

/* sony psx hardware joypad */
#define pad_tri			l_hp
#define pad_x				l_lk
#define pad_circle	l_hk
#define pad_square	l_lp
#define pad_r1			l_block
#define pad_r2			l_up
#define pad_l1			l_down
#define pad_l2			l_left

#endif /* __mk_mkopt_h__ */

