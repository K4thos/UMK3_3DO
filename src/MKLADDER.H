/******************************************************************************
 File: mkladder.h

 By: David Schwartz

 Date: Mar 1995

 (C) Williams Entertainment

 Mortal Kombat III ladder Player Header
******************************************************************************/

#ifndef __mk_mkladder_h__
#define __mk_mkladder_h__

/* 
 * 	DEFINITIONS
 */
#define PANDOWN	SCY(0x30000)

#define ENTRIES_EASY	8
#define ENTRIES_MED		10
#define ENTRIES_HARD	12

#define LAST_HARD_ENTRY	((ENTRIES_EASY+ENTRIES_MED+ENTRIES_HARD-1)+1+1)
#define LAST_MED_ENTRY	((ENTRIES_EASY+ENTRIES_MED-1)+1)
#define LAST_EASY_ENTRY	(ENTRIES_EASY-1)

typedef struct mhe_ord 
{
	OBJECT *olist;			// object list
	short 	bskip;			// # of blocks to skip
	FUNC routine;			// routine to run
} MHE_ORD;


/* 
 *	RAM 
 */
__EXTERN__ XYTYPE ladder_scale;
__EXTERN__ POS ladder_y;
__EXTERN__ POS ladder_y_vel;
__EXTERN__ POS ladder_x;
__EXTERN__ POS ladder_x_vel;
__EXTERN__ short ladder_left_x;
__EXTERN__ short ladder_right_x;
__EXTERN__ LONG ladder_order;			//-DHS- change 16 bit -> 32 bit
__EXTERN__ WORD *ladder_base;
__EXTERN__ short ladder;

/*
 *	PROTOTYPES 
 */
void mk_ladder(void);

void setup_close_ladder(void);
void setup_distant_ladder(void);
void build_middle_tower(void);
void build_left_tower(void);
void build_right_tower(void);
void ladder_switch_scan(void);
void spin_the_mk_badge(void); 
void pan_to_pick_tower(void);
void zoom_in(void);
void pan_down(void);
OBJECT *solid_backdrop_box(void);
ADDRESS get_player_ochar_a5(void);
void create_base_object(void);
void move_dragon_logo(short pa1);
OBJECT *get_current_1st_obj(void);
OBJECT *get_current_objlst(void);
void base_offset_obj(ADDRESS pa5,short pa6,short pa7);
void base_insert(void);
void ladder_irq(void);
void process_a_ladder(MHE_ORD *pa0,short pa2);
void get_a_brick(WORD pa7,OBJECT **olist);

void easy_to_hard(void);
#define hard_to_med easy_to_hard

void med_to_hard(void);
#define hard_to_easy med_to_hard

void set_match_1_diff(void);
void init_map_start(void);
void ladder_badge_spinner(void);

/*
 *	MACROS
 */
#define medium_1st (&mhe_table[0])
#define hard_1st (&mhe_table[3])
#define easy_1st (&mhe_table[6])

#endif /* __mk_mkladder_h__ */

