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

#define MUG1_X	(SCX(0x1f)+1-3)
#define MUG1_Y	(SCY(0x1b)+7)
#define X_SPACE	(SCX(49))
#define Y_SPACE	(SCY(55))

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

typedef struct 
{ 
	WORD pd_num;
	void *pd_fldata;
	WORD *pd_joy;
	WORD *pd_p1char;
	WORD *pd_p2char;
	FUNC pd_func;
	MUGS *pd_mug;
	WORD pd_action;

	WORD *pd_pal;
	WORD pd_oid;
	WORD pd_y;
	WORD pd_pid;
	WORD pd_x;
	WORD pd_zval;
} PDDATA;


/* auto select directions */
#define MVUP	1
#define MVDN	2
#define MVLF	3
#define MVRT	4

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
void create_psel_proc(WORD pa0,WORD pa4,WORD *pa5,PDDATA *pa6,FUNC pa7);
inline void force_psel_choice(WORD);
void player_select_proc(void);
void clip_psel_box(WORD pa0,OBJECT *obj);
void cmove9(void);
void curs3(WORD);
WORD is_move_legal(WORD *);
void new_cursor_calla(WORD *,short,short);
#define new_cursor_position(_tbl,_xoff,_yoff) new_cursor_calla(_tbl,_xoff,_yoff)
WORD a0_ochar_to_position(WORD ochar);
void show_credits(void);

void print_credits(void);
OBJECT *make_ochar_psel_guy(WORD pa1,short pax);
void psel_drone(void);
void psd4(WORD);
void select_speech(void);
void cursor_sound(WORD p1_snd,WORD p2_snd);
void flash_sleep(void *);

void auto_player_select(void);
void new_auto_position(WORD *pa2,short pa3,short pa4);

void replace_drone(void);


void position_badge(void);
void badge_spinner(void);

void select_background(void);


/* externs */

/*
 *	MACROS
 */
#define p_mychar p_store6
#define p_hischar p_otherproc
#define p_cursor_num p_otherguy

#endif /* __mk_mksel_h__ */

