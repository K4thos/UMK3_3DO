/******************************************************************************
 File: mkbuyin.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III buyin module
******************************************************************************/

#ifndef __mk_mkbuyin_h__
#define __mk_mkbuyin_h__

/*
 * 	DEFINITIONS
 */

/* adjust menu index */
#define ADJ_DOWN	0x00				// adjust menu down routine
#define ADJ_UP		0x01				// adjust menu up routine

#define SLIDE_VEL	3
#define SLIDE_TIME	0x40

/*
 *	RAM
 */

/*
 *	PROTOTYPES
 */
void do_buyin(void);
void amenu2(WORD,OBJECT **);
void menu_monitor(void);
void move_menu_text_down(void);
void eliminate_buyins(void);
inline void elim2(WORD *,WORD *);
char *get_buyin_msg(FNTSETUP *,WORD,WORD);
char *get_ctc_msg(void);
void countdown_objects(void);
void shape_buyin_num(WORD num,OBJECT *obj_num);
void buyin_pick(short *,short *);

/*
 *	MACROS
 */
#define adjust_menu_down(_olst) amenu2(ADJ_DOWN,(_olst))
#define adjust_menu_up(_olst) amenu2(ADJ_UP,(_olst))

#endif /* __mk_mkbuyin_h__ */

