/******************************************************************************
 File: mkblood.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III blood Header
******************************************************************************/

#ifndef __mk_mkblood_h__
#define __mk_mkblood_h__

/* 
 * 	DEFINITIONS
 */
#define BLOOD_ROUNDHOUSE	0x00
#define BLOOD_UPPERCUT		0x01
#define BLOOD_FACE2			0x02
#define BLOOD_FACE_PUNCH	0x03
#define BLOOD_COMBO			0x04
#define BLOOD_HAT			0x05
#define BLOOD_SPIDER		0x06
#define BLOOD_LION			0x07
#define BLOOD_DINO_BLOOD	0x08
#define BLOOD_SAW_BLOOD		0x09
#define BLOOD_PIT			0x0a
#define BLOOD_BOOMER		0x0b
#define BLOOD_SPEAR			0x0c

/* 
 *	RAM 
 */

/*
 *	PROTOTYPES 
 */
void create_blood_proc(WORD);

void boomerang_blood(void);
void boomb1(void);
void boomerang_drip(void);
void spear_blood(void);
void spearb1(void);
void spear_drip(void);

void saw_blood(void);
void sawb3(void);
void saw_right(void);

void dino_blood(void);
void dblood3(void);
void dino_right(void);

void lion_blood(void);
void lion_right(void);

void spider_blood(void);
void spider_sounds(void);

void hat_blood(void);
void hatb1(void);
void hatb2(void);
void hatb_drip(void);
void drip_entry(void);

void pit_spike_blood(void);

void upcutted_blood(void);
void upcut_right(void);
void upcb3(void);

void lineup_with_a11_offset(short pa0x,short pa0y);

void roundhouse_blood(void);
void rhouse_right(void);
void rhb3(void);

void combo_blood(void);

void face_blood_2(void);
void face_blood(void);

void face_punch_blood(void);

void spawn_drip_a10(WORD pa10);
void spawnd3(void);

void tobias_drip_neg(void);
void tobias_drip(void);
void tdrip2(void);
void tdrip_getobj(void);

void blood_fall(void);
void gbld3(void *pa5);

void drip_fall(void);
void drip_fall_2(void);

void a11_blood_lineup(short ta0,short ta1);
void blood_fall_splat_12_die(void);


/*
 *	MACROS
 */
#define blood_death(_la8) delobjp(_la8),process_suicide()

#define spawn_drip_neg \
		current_proc->a7=(ADDRESS)tobias_drip_neg,spawnd3()

#define spawn_drip \
		current_proc->a7=(ADDRESS)tobias_drip,spawnd3()

#define get_tg_blood gbld3(STAB1)
#define get_jt_blood gbld3(BIGBLD1)

#endif /* __mk_mkblood_h__ */

