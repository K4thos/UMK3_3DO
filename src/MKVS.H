/******************************************************************************
 File: mkvs.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III vs Player Header
******************************************************************************/

#ifndef __mk_mkvs_h__
#define __mk_mkvs_h__

/*
 * 	DEFINITIONS
 */
#define SSPEED SCX(0x60000)
#define SDIST (SCX(0xb0))


typedef struct vpiece {
	short v_p1x;				// player 1 x off
	short v_p1y;				// player 1 y off
	short v_p2x;				// player 2 x off
	short v_p2y;				// player 2 y off
} VPIECE;

/*
 *	RAM
 */

/*
 *	PROTOTYPES
 */
void vs_or_ladder(void);

void mk_vs(void);
void mk_battle(void);

void mk_bat_signal_f(void);
void mk_bat_signal(void);

void do_mk_number(WORD pa0);

void combination_proc(void);
void slide_in_huge_players();
void build_huge_left(void *pheap);
inline void setup_direction(LONG pa4);
void start_off_screen(OBJECT *);
void check_button(OBJECT *obj);

OBJECT *make_a_mk_light(short pa9x,short pa9y);

void create_1_piece(void *pa7,short pa0,short pa1,short pa2);
void cd_preload_fade(void);
void cd_preload_p1(void);
void cd_preload_bkgd(void);

/* externs */
extern void *vs_anims[];
extern void *a_mklight[];
extern void *a_vs_s[];
extern void *a_vs_v[];
extern void *symbol_table[];
extern void *BURNV1[];
extern void *BURNS1[];
extern void *BATTLE0[];
extern void *BATTLE[];
extern void *MKLIGHT1[];
extern void *BOX2[];
extern void *I_DRAGON[];

extern VPIECE kano_pieces[];
extern VPIECE sonya_pieces[];
extern VPIECE jax_pieces[];
extern VPIECE ind_pieces[];
extern VPIECE sz_pieces[];
extern VPIECE swat_pieces[];
extern VPIECE lia_pieces[];
extern VPIECE robo1_pieces[];
extern VPIECE robo2_pieces[];
extern VPIECE lao_pieces[];
extern VPIECE tusk_pieces[];
extern VPIECE sg_pieces[];
extern VPIECE st_pieces[];
extern VPIECE lk_pieces[];
extern VPIECE smoke_pieces[];
extern VPIECE kitana_pieces[];		
extern VPIECE jade_pieces[];  		
extern VPIECE mileena_pieces[];		
extern VPIECE scorpion_pieces[];	
extern VPIECE reptile_pieces[];		
extern VPIECE ermac_pieces[];		
extern VPIECE oldsz_pieces[];		
extern VPIECE oldsmoke_pieces[];	
extern VPIECE noob_pieces[];		

/*
 *	MACROS
 */

#endif /* __mk_mkvs_h__ */

