/******************************************************************************
 File: mkmark.h

 By: David Schwartz

 Date: Jan 1995

 (C) Williams Entertainment

 Mortal Kombat III hidden game header
******************************************************************************/

#ifndef __mk_mkmark_h__
#define __mk_mkmark_h__

/*
 * 	DEFINITIONS
 */
typedef struct pathptr
{
	long p_xvel;			// xvel
	long p_yvel;			// yvel
	WORD p_time;			// time
	WORD p_shot;			// shot %
} PATHPTR;

typedef struct baitpos
{
	short b_xpos;
	short b_ypos;
	OIMGTBL *b_frame;
} BAITPOS;


#define BPAL_YELLOW 0x7fe0
#define BPAL_RED 0x001f
/*
 *	RAM
 */
__EXTERN__ WORD player_1_ships;
__EXTERN__ OBJECT *player_1_ship;
__EXTERN__ OBJECT *player_1_missile;
__EXTERN__ OBJECT *player_1_missile2;
__EXTERN__ OBJECT *player_1_missile3;
__EXTERN__ OBJECT *player_1_missile4;
__EXTERN__ WORD player_1_bstate;

__EXTERN__ WORD player_2_ships;
__EXTERN__ OBJECT *player_2_ship;
__EXTERN__ OBJECT *player_2_missile;
__EXTERN__ OBJECT *player_2_missile2;
__EXTERN__ OBJECT *player_2_missile3;
__EXTERN__ OBJECT *player_2_missile4;
__EXTERN__ WORD player_2_bstate;

__EXTERN__ short wave_x_position;
__EXTERN__ short wave_y_position;
__EXTERN__ short wave_x_velocity;
__EXTERN__ short wave_y_velocity;
__EXTERN__ WORD dive_randper;
__EXTERN__ WORD shoot_randper;
__EXTERN__ WORD count_left;
__EXTERN__ OBJECT *last_star;
__EXTERN__ WORD ok_exit;

/*
 *	PROTOTYPES
 */
void hidden_game(void);


/*
 *	MACROS
 */
//#define safe32mult(_la0,_la1) ((_la0)*(_la1))

#define be_99	be_9(0)
#define be_9d	be_9(1)

#endif /* __mk_mkmark_h__ */
