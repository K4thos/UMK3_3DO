/******************************************************************************
 File: mkfx.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Special Effects
******************************************************************************/

#ifndef __mk_mkfx_h__
#define __mk_mkfx_h__

/*
 * 	DEFINITIONS
 */

/* special effects offsets */
#define FX_SONYA_GLOBE		0x00
#define FX_TRAIL			0x01
#define FX_FLOOR_EXPLODE	0x02
#define FX_FREEZE_BLAST		0x03
#define FX_ROCKET_PUFF		0x04
#define FX_DAVE_EXPLODE		0x05
#define FX_SCREAM_SHAKER	0x06
#define FX_ROBO_DIZZY		0x07
#define FX_ROBO_TELE_EXPLODE		0x08
#define FX_ROBO_TELE_IMPLODE 0x09
#define FX_ROBO_BOMB_EXPLODE 0x0a
#define FX_FIGHT_INTRO		0x0b
#define FX_ROBO_CRUNCH_LIMB 0x0c
#define FX_COMBO			0x0d
#define FX_COMMENT			0x0e
#define FX_JAX_ZAP_PUFF		0x0f
#define FX_LINEUP_EXP		0x10
#define FX_FINISH_HIM		0x11
#define FX_FINISH_HER		0x12
#define FX_CHARGE_BARS		0x13
#define FX_SMOKE_GUY		0x14
#define FX_LASER_EXPLODE	0x15
#define FX_FATAL_DRIP		0x16
#define FX_FORDEN			0x17
#define FX_SWAT_BEXP		0x18
#define FX_SKINNY_BLOOD		0x19
#define FX_PIT_MULCH		0x1a
#define FX_SONYA_ANIMIAL	0x1b
#define FX_ANIMALITY		0x1c
#define FX_MERCY			0x1d
#define FX_INVISO_POOF		0x1e
#define FX_SMOKE			0x1f
#define FX_STAND_BLOOD		0x20
#define FX_JAX_STOMP		0x21
#define	FX_CYRAX_EXPLDE		0x22
#define FX_CYRAX_CUTUP		0x23
#define FX_LAO_NADO			0x24
#define FX_LIA_SPIN			0x25
#define FX_INFLATE_BOOM		0x26
#define FX_DANGER			0x27
#define FX_SPIKE_BLOOD		0x28
#define FX_SUBWAY_ROX		0x29
#define FX_FRIENDSHIP		0x2a
#define FX_BABALITY			0x2b
#define FX_SONYA_CRUSH		0x2c
#define FX_SZ_GUTS			0x2d
#define FX_CYRAX_COPTER		0x2e
#define FX_SPIT_GLOB		0x2f
#define FX_JADE_SMOKE		0x30
#define FX_MILEENA_BONE		0x31
#define FX_THROW_ENCOURAGE	0x32
#define FX_EX2ON2_BANG		0x33
#define FX_LAVA_DAN			0x34


/* sonya globs */
#define GLOB_AWAY_CNT 8
#define GLOB_TOWARD_CNT 8

typedef struct gexplode
{
	WORD g_cnt;			// how many pieces
	short g_x;			// x coord offset of dude
	short g_y;			// y coord offset of dude
	short g_grav;		// gravity
	void *g_simg;		// starting image
	void *g_anim;		// animation table
	short g_xvrange;	// x vel range (sll 8)
	short g_xvmin;		// x vel min (sll 8)
	short g_yvrange;	// y vel range (sll 8)
	short g_yvmin;		// y vel min (sll 8)
	WORD g_aspeed;		// animation speed
	WORD g_land;		// land routine
} GEXPLODE;

typedef struct gsexplode
{
	WORD g_cnt;			// how many pieces
	short g_x;			// x coord offset of dude
	short g_y;			// y coord offset of dude
	short g_grav;		// gravity
	short g_anim;		// animation index for ANIM TABLE 2
	short g_xvrange;	// x vel range (sll 8)
	short g_xvmin;		// x vel min (sll 8)
	short g_yvrange;	// y vel range (sll 8)
	short g_yvmin;		// y vel min (sll 8)
	WORD g_aspeed;		// animation speed
	WORD g_land;		// land routine
} GSEXPLODE;

/*
 *	RAM
 */

/*
 *	PROTOTYPES
 */
inline PROCESS *create_fx(WORD);
void shake_a11(WORD,WORD);

void fx_throw_encouraged(void);
void spit_glob(void);
void glob_proc(void);

void jade_smoke(void);
void jade_puff(void);
void puff_setup(void);

void fx_lava_dan(void);

void es_sub(void);

void cyrax_copter_guts(void);
void sz_rips_guts(void);
void sonya_crush_explode(void);

void fx_babality(void);
void baby_letter(void);
void baby_fall(long pa0);

void fx_friendship(void);
void ship_proc(void);
void fani3(void *pa9);
void friendship_speech(void);

void subway_rox(void);
void do_rox(WORD pa0,WORD pa6,void *pa11);
void subway_rock_fall(void);

void inflate_boom(void);
void spike_blood_explode(void);

void lia_spin_fx(void);
void hair_blood(void);
void hair3(void);
void hair_explode(void);

void lao_nado_pieces(void);
void nado_spawn(void);
void nado_blood_explode(void);
void nado_pieces(void);

void cyrax_fatal_explode(void);
void stand_blood_explode(void);
void cbx3(void);
void cyrax_cutup_blood(void);
void jax_stomp_explode(void);

void fx_mercy(void);
void new_mercy_pal(void);
void mercy_fader(void);

void sonya_animal_fx(void);

void pit_mulch(void);
void pit_blood_explode(void);
void pit_mulch_burst(void);
void pit_blood_explode_proc(void);

void skinny_blood(void);
void fatality_drip(void);

void fx_animality(void);

void swat_bomb_explode(void);
void laser_explode(void);
void smoke_a_guy(void);

void mileena_bone(void);
void one_bone_proc(void);
void ex2on2_kombat_bang(void);



void smoke_smoke(void);
void smoke_smoke_1(void);

void smoke_a_1(void);
void puff_ani2(void);

void fx_forden_a11(void);
void forden_peek(WORD pa11);

void fx_inviso_poof(void);

void sonya_proj_globs(void);
void get_glob_obj(long,long,long,long);
void glob_fly(void);
void glob_away(void);
void glob_toward(void);

void trail_proc(void);
short get_jc_pid(void);
void shadow_dummy_proc(void);
WORD get_trail_pid(OBJECT *pa11); 
WORD get_shadow_oid(OBJECT *pa11); 

void floor_explode(void);
void gravity_explode(GEXPLODE *,void *);
void spawn_1_piece(GEXPLODE *,short,short,WORD,void *);
void special_gravity_explode(GSEXPLODE *);
void special_spawn_1_piece(GSEXPLODE *,short,short,WORD);

void gravity_piece(void);
void do_land_routine(void);

void gravity_a11(void);
void blast_shaker(void);
void land_small_rock(void);
#define land_med_rock land_small_rock
void land_large_rock(void);
inline void rock_bounce(long,long);
inline void rock_bounce_nosound(long min,long range);
void single_flight(long,long,long,WORD);
void split_bounce(void);
void spit_in_half(void);
void rock_crumble(void);
void land_body_part(void);
void land_subway_rock(void);
void land_ex2on2(void);
void land_pit_bottom(void);

void scream_shaker(void);

void freeze_blast(void);

void robo_bomb_explode(void);

void robo_tele_explode(void);
void robo_tele_implode(void);
void get_explode_obj(void);

void robo_dizzy_bolts(void);
void dave_explode(void);
void rocket_puff(void);
void one_dizzy_bolt(void);
void robo_dizzy_abort(void);
WORD is_robo_still_dizzy(void);

void fx_fight_intro(void);
void fx_finish_him(void);
void fx_finish_her(void);
void grow_in_grow_out(WORD pa0,OIMGTBL *pa5);
void grow_in_spin_out(WORD pa0,OIMGTBL *pa5,void *pa9);
void center_scale(OBJECT *obj,long pa9);

void lineup_explode(void);

void white_flash_proc(void);
void white_flash(void);

void jax_zap_puff(void);
OBJECT *puff_init(WORD zval);

void fx_combo(void);
void hits_msg(void);
void setup_combo_message(short pa4x,short pa4y,void *pa5,WORD pa6,WORD pa11);
OBJECT *a3_combo_number_obj(WORD pa3,short pa4x,short pa4y,void *pa5);
void slide_combo_msg(void);

void fx_charge_bars(void);
void charge_stuff(void);

void fx_comment(void);

void blood_explode(void);
void danger_danger(void);
void danger_y_adjust(short pa1);

void robo_crunch_limbs(void);
void squeeze_out_left(void);
void squeeze_out_right(void);
void sol4(long pa0);
OBJECT *get_a_limb(WORD pa0);


/*
 *	MACROS
 */
#define get_glob_and_vel get_glob_obj(SCX(0x60000),SCX(0x20000),SCY(0x80000),SCY(0x50000))


/* gravity a11 yvel */
#define gravity_a11_yvel(_obj,_yvel) \
		(_obj)->oyvel.pos=(_yvel),gravity_a11()

#endif /* __mk_mkfx_h__ */

