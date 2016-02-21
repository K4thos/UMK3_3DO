/******************************************************************************
 File: mkscore.h

 By: David Schwartz

 Date: Dec 1994

 (C) Williams Entertainment

 Mortal Kombat III score Header
******************************************************************************/

#ifndef __mk_mkscore_h__
#define __mk_mkscore_h__

/*
 * 	DEFINITIONS
 */

#define P1_NAME_TXT_X	110
#define P1_NAME_TXT_Y	26
#define P2_NAME_TXT_X	176
#define P2_NAME_TXT_Y	26

#define POWERBAR_Y_SIZE		0x0007				// ysize of powerbar
#define POWERBAR_X_SIZE_MIN	0x0002				// min xside for powerbar display

#define FULL_TURBO 0x30							// turbo max bar
#define TURBOBAR_WIDTH	SCX(FULL_TURBO)

#define BACKBAR_WIDTH	SCX(168)

#define BACKBAR_Y		(18+SCY(0x19))
#define P1_BACKBAR_X	(SCX(0x14))
#define P2_BACKBAR_X	(SCRRGT-BACKBAR_WIDTH-P1_BACKBAR_X)

#define P1_TURBO_Y		(18+SCY(0x23))
#define P2_TURBO_Y		P1_TURBO_Y

#define P1_TURBO_X		(P1_BACKBAR_X)
#define P2_TURBO_X		(SCRRGT-TURBOBAR_WIDTH-P1_TURBO_X)

#define P1_MSG_Y		(18+SCY(0x000a))
#define P1_MSG_X		SCX(0x0015)

#define P2_MSG_Y		(18+SCY(0x000a))
#define P2_MSG_X		SCX(0x0164)

#define P1_SIC_Y		(18+SCY(0x000a))						// p1 "START/INSERT COIN"
#define P1_SIC_X		SCX(0x0014)

#define P2_SIC_Y		(18+SCY(0x000a))						// p2 "START/INSERT COIN"
#define P2_SIC_X		SCX(0x011d)

#define P1_K1_X			SCX(0x0018)
#define P1_K2_X			SCX(0x0026)+4
#define P2_K1_X			SCX(0x016b)
#define P2_K2_X			SCX(0x015d)-4

#define P_KNOTCH_Y		(18+SCY(0x002a))

#define CLOCK_TENS_Y	(18+SCY(0x0007))						// tens digit pos
#define CLOCK_TENS_X	SCX(0x00bb)

#define CLOCK_ONES_Y	(18+SCY(0x0007))						// ones digit pos
#define CLOCK_ONES_X	SCX(0x00c8)

#define DIGIT_SIZE 10

#define TURBOBAR_Y_SIZE	3

/* score ram entries */
#define p1_knotch1	(score_ram+0)
#define p1_knotch2	(score_ram+1)
#define p2_knotch1	(score_ram+2)
#define p2_knotch2	(score_ram+3)

#define entry_1		(score_ram+0)
#define entry_2		(score_ram+1)
#define entry_3		(score_ram+2)
#define entry_4		(score_ram+3)
#define entry_5		(score_ram+4)
#define entry_6		(score_ram+5)
#define entry_7		(score_ram+6)
#define entry_8		(score_ram+7)
#define entry_9		(score_ram+8)
#define entry_10	(score_ram+9)
#define entry_11	(score_ram+10)
#define entry_12	(score_ram+11)
#define entry_13	(score_ram+12)
#define entry_14	(score_ram+13)
#define entry_15	(score_ram+14)
#define entry_16	(score_ram+15)
#define entry_17	(score_ram+16)
#define entry_18	(score_ram+17)
#define entry_19	(score_ram+18)
#define entry_20	(score_ram+19)
#define entry_21	(score_ram+20)
#define entry_22	(score_ram+21)

/* vram indexes for control panel entries */
#define BAR_NULL	0x00			// control panel: nothing to display
#define MKMEDAL2	0x01			// control panel: win medal knotches

#define PWRBAR_LA	0x02			// control panel: power bar A
#define PWRBAR_RA	0x03			// control panel: power bar B

#define INSERT_COIN	0x04			// control panel: insert coin
#define PUSHSTART	0x05			// control panel: push start

#define X_SHADNUM0	0x06			// control panel: shadow num 0
#define X_SHADNUM1	0x07			// control panel: shadow num 1
#define X_SHADNUM2	0x08			// control panel: shadow num 2
#define X_SHADNUM3	0x09			// control panel: shadow num 3
#define X_SHADNUM4	0x0a			// control panel: shadow num 4
#define X_SHADNUM5	0x0b			// control panel: shadow num 5
#define X_SHADNUM6	0x0c			// control panel: shadow num 6
#define X_SHADNUM7	0x0d			// control panel: shadow num 7
#define X_SHADNUM8	0x0e			// control panel: shadow num 8
#define X_SHADNUM9	0x0f			// control panel: shadow num 9
#define X_SHADWINS	0x10			// control panel: shadow wins

#define COUNTDOWN_0	0x11			// control panel: clock digit num 0
#define COUNTDOWN_1	0x12			// control panel: clock digit num 1
#define COUNTDOWN_2	0x13			// control panel: clock digit num 2
#define COUNTDOWN_3	0x14			// control panel: clock digit num 3
#define COUNTDOWN_4	0x15			// control panel: clock digit num 4
#define COUNTDOWN_5	0x16			// control panel: clock digit num 5
#define COUNTDOWN_6	0x17			// control panel: clock digit num 6
#define COUNTDOWN_7	0x18			// control panel: clock digit num 7
#define COUNTDOWN_8	0x19			// control panel: clock digit num 8
#define COUNTDOWN_9	0x1a			// control panel: clock digit num 9
#define ENERGY_FILL_BAR	0x1b
#define TURBO_FILL_BAR	0x1c

#define P1_BACKPWR_SPEC	0xff		// control panel flag: for special power bar p1 backdrop
#define P2_BACKPWR_SPEC	0xfe		// control panel flag: for special power bar p2 backdrop
#define P1_TURBO_SPEC	0xfd		// control panel flag: for special power bar p1 turbo bar
#define P2_TURBO_SPEC	0xfc		// control panel flag: for special power bar p2 turbo bar
#define	P1_NAME_SPEC	0xfb		// control panel flag: for special name
#define	P2_NAME_SPEC	0xfa		// control panel flag: for special name

/* score ram index */
#define SR_P1_KNOTCH1	0x00
#define SR_P1_KNOTCH2	0x01
#define SR_P2_KNOTCH1	0x02
#define SR_P2_KNOTCH2	0x03


/* control panel tpage info */
typedef struct cptpage
{
	BYTE u0,v0;						// offset within tpage (u0 even)
	BYTE w,h;						// width & height of sprite
} CPTPAGE;

typedef struct cpentry
{
	short x0,y0;					// position of piece
	BYTE  cpentry;					// index into cp ram description table
	BYTE  disp_flag;				// 0=dont display, otherwise size
} CPENTRY;

/*
 *	RAM
 */
__EXTERN__ CPENTRY score_ram[SCORE_RAM_CNT];
__EXTERN__ WORD score_pal;						/* address of score pal */
__EXTERN__ WORD boon_pal;						/* address of boon pal */


/*
 *	PROTOTYPES
 */
void setup_score_ram(void);
void recharge_bars(void);
void update_win_counters(void);
void uwc_stuff(WORD,CPENTRY *,WORD,WORD);
void update_clock_digit(CPENTRY *,WORD,WORD,WORD);
void update_knotches(void);
void update_pmsg(void);
WORD start_or_insertcoin(void);
void qwerty(void);
void update_control_panel(void);
void position_p2_bar(void);
void refresh_score(void);
void load_control_page(void);
void load_p1_name(void);
void load_p2_name(void);

/* score texture (mkcontro.s) */
extern void *cptpage_TXT[];

/*
 *	MACROS
 */
/* set score ram info */
#define SET_SCORE_RAM(_p,_x,_y,_i,_df) \
		((_p)->x0=(_x)),((_p)->y0=(_y)),((_p)->cpentry=(_i)),((_p)->disp_flag=(_df))

/* get_safe_powerbar, keep bar at min size */
#define get_safe_powerbar(_pb) (((*(_pb))>POWERBAR_X_SIZE_MIN) ? (*(_pb)):POWERBAR_X_SIZE_MIN)

/* update ones digit */
#define update_clock_ones(_sr,_d) update_clock_digit((_sr),CLOCK_ONES_X,CLOCK_ONES_Y,(_d))

/* update tens digit */
#define update_clock_tens(_sr,_d) update_clock_digit((_sr),CLOCK_TENS_X,CLOCK_TENS_Y,(_d))

#endif /* __mk_mkscore_h__ */

