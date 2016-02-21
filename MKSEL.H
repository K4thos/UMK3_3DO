/******************************************************************************
 File: mksel.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III PLAYER SELECT HEADER
******************************************************************************/

#ifndef __mk_mksel_h__
#define __mk_mksel_h__

/*
 * 	DEFINITIONS
 */
#define MAX_MAP_SELECT	6					/* # of maps/mountains to select from */


#define CTHICK	4

#define X_SIZE	44
#define Y_SIZE	60

#define MUG1_X	(SCX(0x45)+1-3)
#define MUG1_Y	(SCY(0x25)+7)
#define X_SPACE	(SCX(53))
#define Y_SPACE	(SCY(65))

#define S2_OCHAR   2
#define S12_OCHAR  12
#define S6_OCHAR   6
#define S8_OCHAR   8

#define DRONE1_X (SCX(0x25)-8)
#define DRONE2_X (SCRRGT-SCX(0x24)+2)

typedef struct mugs
{
	OIMGTBL *m_mug;				// ptr to mug info for character
	short m_xpos;				// x pos of mug
	short m_ypos;				// y pos of mug
	WORD m_char;				// char id of character
	WORD m_pad;					// padding
} MUGS;


/* auto select directions */
#define A_UP	1
#define A_DOWN	2
#define A_LEFT	3
#define A_RIGHT	4

/*
 *	RAM
 */
__EXTERN__ volatile WORD f_fighter_load;				/* set when fighter data is loaded */
__EXTERN__ WORD f_speech;						/* fix double repeat name bug (1=p1 spoken, 2=p2 spoken) */

/*
 *	PROTOTYPES
 */
void join_in_during_psel(WORD *);
void player_select(void);
void player_joinin(void);
void psel6(void);
void create_cursor_procs(void);
inline void force_psel_choice(WORD);
void box_flip_proc(void);
void p1_select(void);
void p2_select(void);
void cmove9(void);
void curs3(WORD);
WORD is_move_legal(WORD *);
void new_cursor_calla(WORD *,short,short);
#define new_cursor_position(_tbl,_xoff,_yoff) new_cursor_calla(_tbl,_xoff,_yoff)
WORD a0_ochar_to_position(WORD ochar);
void show_credits(void);

void print_credits(void);
void psel_drone_1(void);
void psel_drone_2(void);
void psd4(WORD);
void select_speech(void);
void cursor_sound(WORD p1_snd,WORD p2_snd);
void flash_sleep(void *);

void auto_player_select(void);
void new_auto_position(WORD *pa2,short pa3,short pa4);

void position_badge(void);
void badge_spinner(void);

void select_background(void);


/* externs */

/*
 *	MACROS
 */
#define p_mychar p_store6
#define p_hischar p_otherproc

#endif /* __mk_mksel_h__ */

