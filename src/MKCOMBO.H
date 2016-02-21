/******************************************************************************
 File: mkcombo.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III combo Player Header
******************************************************************************/

#ifndef __mk_mkcombo_h__
#define __mk_mkcombo_h__

/* 
 * 	DEFINITIONS
 */
#define CB_HIPUNCH	0x00
#define CB_LOPUNCH	0x01
#define CB_BLOCK	0x02
#define CB_HIKICK	0x03
#define CB_LOKICK	0x04

#define CB_JUMP_ADDRESS	0x1111


#define STK_0		0x0000	// hit in face
#define STK_1		0x0100	// stumble away
#define STK_2		0x0200	// knocked on back
#define STK_3		0x0300	// knocked high on back
#define STK_4		0x0400	// NAILED !
#define STK_5		0x0500	// uppercutted

#define C_SPEED1	0x0100
#define C_SPEED2	0x0200
#define C_SPEED3	0x0300

#define LAST_OPTION	0x8000
#define B_LAST_OPTION	15
#define M_LAST_OPTION	LAST_OPTION

typedef struct combohdr {
	WORD	c_time;
	WORD	c_attack;		// attack animation
	WORD 	c_speed;		// anim speed, button #
	WORD	c_retr;			// retract ani: [offset,p#]
	WORD	c_stk;			// strike offset
	WORD	c_pad;			// padding for alignment
	JUMPTBL c_next;			// success ptr to next combo table entry
} COMBO;

#define c_req	c_stk		// requirement routine

#define REQ_NONE	0	// requirement: none
#define REQ_STKAWAY	1	// requirement: joystick away
#define REQ_STKTOW	3	// requirement: joystick towards
#define REQ_STKDOWN	4

#define RETRACT_ELBOW	0x1002
#define RETRACT_KNEE	0x1302

#define ATK_ELBOW1	0x1001
#define ATK_ELBOW2	0x1003
#define ATK_ELBOW3	0x1004
#define ATK_ELBOW4	0x1005
#define ATK_KNEE1	0x1301
#define ATK_KNEE2	0x1303
#define ATK_KNEE3	0x1304
#define ATK_KNEE4	0x1305
#define ATK_HIKICK	0x1101
#define ATK_LOKICK	0x1201
#define ATK_ROUNDH	0x1501



/* 
 *	RAM 
 */

/*
 *	PROTOTYPES 
 */
void do_knee(void);
void do_elbow(void);
void knee_elbow_blocked(WORD pa0);

void process_combo_table(COMBO *);
WORD combo_scan_a11(COMBO **ctbl);
inline void a9_combo_ani(WORD);
void clear_combo_butn(void);
WORD check_axe_up_combo(void);
WORD combo_is_stick_away(void);
WORD combo_is_stick_down(void);
WORD combo_is_stick_up(void);
WORD combo_is_stick_toward(void);
WORD combo_q_no(void);
WORD set_his_half_damage(void);
WORD set_his_quarter_damage(void);

/* combo tables externs (mkscombo.s) */
extern COMBO ct_kano_elbow2[];
extern COMBO ct_sonya_elbow2[];
extern COMBO ct_jax_elbow2[];
extern COMBO ct_ind_elbow2[];
extern COMBO ct_sz_elbow2[];
extern COMBO ct_swat_elbow2[];
extern COMBO ct_lia_elbow2[];
extern COMBO ct_sektor_elbow2[];
extern COMBO ct_cyrax_elbow2[];
extern COMBO ct_lao_elbow2[];
extern COMBO ct_tusk_elbow2[];
extern COMBO ct_sg_elbow2[];
extern COMBO ct_st_elbow2[];
extern COMBO ct_lk_elbow2[];
extern COMBO ct_smoke_elbow2[];
extern COMBO ct_kitana_elbow2[];
extern COMBO ct_jade_elbow2[];
extern COMBO ct_mileena_elbow2[];
extern COMBO ct_scorp_elbow2[];
extern COMBO ct_rep_elbow2[];
extern COMBO ct_ermac_elbow2[];
extern COMBO ct_osz_elbow2[];
extern COMBO ct_scorp_elbow2[];
extern COMBO ct_kano_elbow2[];
extern COMBO ct_kano_knee2[];
extern COMBO ct_sonya_knee2[];
extern COMBO ct_jax_knee2[];
extern COMBO ct_ind_knee2[];
extern COMBO ct_sz_knee2[];
extern COMBO ct_swat_knee2[];
extern COMBO ct_lia_knee[];
extern COMBO ct_sektor_knee2[];
extern COMBO ct_cyrax_knee2[];
extern COMBO ct_lao_knee2[];
extern COMBO ct_tusk_knee2[];
extern COMBO ct_sg_knee2[];
extern COMBO ct_st_knee2[];
extern COMBO ct_lk_knee2[];
extern COMBO ct_smoke_knee2[];
extern COMBO ct_kitana_knee2[];
extern COMBO ct_jade_knee2[];
extern COMBO ct_mileena_knee2[];
extern COMBO ct_scorp_knee2[];
extern COMBO ct_rep_knee2[];
extern COMBO ct_ermac_knee2[];
extern COMBO ct_osz_knee2[];
extern COMBO ct_kano_knee2[];
extern COMBO ct_kano_knee2[];

/*
 *	MACROS
 */

#endif /* __mk_mkcombo_h__ */

