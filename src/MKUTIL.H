/******************************************************************************
 File: mkutil.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Utility Header Functions
******************************************************************************/

#ifndef __mk_util_h__
#define __mk_util_h__

/*
 *	DEFINITIONS
 */
#define ONE_SECOND	55						/* # of ticks per second */
#define GNULL 22							/* null gravity */

/* palette flags for fighter setup */
#define PFLAG_CLEAR		0x0000					// dont load palette for object
#define PFLAG_PALLOAD	0x0001					// load palette for object
#define PFLAG_FIGHTER	0x0002					// fighter being dealt with
#define PFLAG_FIGHTER2	0x0004					// fighter 2 being dealt with

/* strike flags */
#define SF_SQUEEZE		1					// flag: squeeze
#define SB_SQUEEZE		0					// flag: squeeze
#define SF_DUCK			2					// flag: must duck/block to block

#define SFM_SQUEEZE		2					// mask: squeeze
#define SBM_SQUEEZE		1					// mask: squeeze
#define SFM_DUCK		4					// mask: must duck/block to block

/* takeover_him routine indexes */
#define TH_NORMAL	0x00					// takeover_him_routine: normal ops
#define TH_SR		0x01					// takeover_him_routine: sr routine

typedef struct s_strktbl {
	short sxoff;							// lower right xoffset of collision box
	short syoff;							// lower right yoffset of collision box
	short sxsize;							// x size of collision box
	short sysize;							// y size of collision box
	WORD sroutine;							// strike routine
	WORD sdamage;							// damage
	WORD sflags;							// various flags for strike
} STRKTBL;

/*
 *	RAM
 */
__EXTERN__ WORD	timecnt;					/* # of timer ticks */
__EXTERN__ long rand_num;					/* random # */

/*
 *	PROTOTYPES
 */
void clear_pdata(void);

void back_to_normal(void);
void advance_him(void);
void add_combo_damage(WORD pa14);
void decp3(PROCESS *pa0);
inline void inc_his_p_hit(void);

void sans_repell_3(void);
void clr_scrn(void);
long get_rough_hypotenuse(long,long);
void boonpal_stuff(void);
void slave_inviso(void);
void slave_viso(void);
void center_about_x(OBJECT *,short);
PROCESS *exprc_er(WORD);
inline void no_edge_limits(void);
WORD closest_edge_a5(void);
void get_my_dfe(void);
void get_his_dfe(void);
long edge_pick_a0(long pa0);
void screen_a5(OBJECT *);
void world_a5(OBJECT *);
void get_screen_coordinates(OBJECT *,short *,short *);
inline int get_my_height(void);
void no_edge_both_players(void);
void wait_for_start(void);
void wait_for_his_dog(long);
void update_tsl(WORD *,OBJECT *);
inline WORD am_i_short(void);
WORD is_he_short(void);
inline WORD is_he_airborn(void);
inline WORD is_he_joy(void);
inline WORD am_i_airborn(OBJECT *);
WORD is_he_blocking(WORD *,WORD);
inline WORD am_i_joy(void);
WORD am_i_close_to_edge(PROCESS *);
inline void killfx(void);
void face_opponent(OBJECT *);
inline WORD is_he_right(PROCESS *);
inline WORD is_he_left(PROCESS *);
void delete_slave(void);
void murder_myoinit_score(void);
void murder_myoinit(void);
void fastxfer(PROCESS *,JUMPTBL);
PROCESS *create_and_kill(WORD,FUNC);
PROCESS *create_if_gone(WORD,FUNC);
void kill_ocp(WORD,WORD);
void change_oid_list(OBJECT **olist,WORD oldid,WORD newid);
void koc2(OBJECT **,WORD,WORD);
void swpal(void *,OBJECT *);
void wait_forever(void);
void shake2(OBJECT *,WORD,WORD,WORD,WORD);
void takeover_him_routine(WORD,JUMPTBL);
WORD get_tsl(WORD *,OBJECT *);
void damage_to_me(WORD,OBJECT *);
inline void half_x_vel(OBJECT *);
ADDRESS get_his_char_ani(WORD,short);
void adjust_him_xy(short,short);
void match_him_with_me_f();
void match_him_with_me();
void match_me_with_him(void);
void get_his_a11_ani(WORD);
PROCESS *gmo_proc(void *);
void multi_dummy_proc(void);
OBJECT *get_multi_obj(ADDRESS *,void *,void *,WORD);
OBJECT *object_setup(OIMGTBL *,void *,WORD);
OBJECT *gbo_dmawnz(OIMGTBL *frame,void *heapptr,void *palram);
void grab_screen(void);
void sync_wait_dma_q(void);
void move_object_list(OBJECT **,OBJECT **);
void delolist(OBJECT **);
void lineup_on_a9(short pa9);
void center_around_me(void);
void new_scroll_init(void);
void start_shadows(void);
void lineup_1pwm(OBJECT *,OBJECT *);
void gravity_ani(WORD,long,long);
inline WORD hit_ground_yet(OBJECT *);
OBJECT *get_single_obj(LONG,void *,void *,WORD);
void immune_mpp(void);
void unimmune_mpp(void);
void randomize(short,short);
short nrand(void);
long random(void);
LONG randu(LONG);
long srandarc(LONG);
WORD randper(WORD);
WORD is_he_facing_me(void);
void flight_call(OBJECT *obj,long initx,long inity,long gravity,short anispeed,long anioffset);
void flight_loop(OBJECT *obj);
void towards_x_vel(PROCESS *proc,OBJECT *obj,long initx);
inline void away_x_vel(PROCESS *proc,OBJECT *obj,long initx);

void freeze_2_pages(void);
void slave_in_front(void);

void wait_for_landing(void);
void back_to_shang_check(void);
WORD am_i_shang();
void back_to_shang_form(void);

void air_dragon(void);
void lower_dragon(void);

WORD get_my_matchw(void);
WORD get_his_matchw(void);
OBJECT *make_solid_object(WORD pa0,WORD pa3x,WORD pa3y);

void player_heap_adj(void);

/* externals mkstk.s */
extern STRKTBL **strike_tables[];
extern STRKTBL *combo_strike_table[];

/*
 *	MACROS
 */
#define gsagof(_i,_size,_sag) \
		{ \
		(_size)=(_i)->size.yx; \
		(_sag)=(_i)->isagptr; \
		}

#define stop_a8(_i) (_i)->oxvel.pos=(_i)->oyvel.pos=0

#define MURDER process_kill_class(0,0x8000)

#define distance_off_ground(_proc,_obj) ((_proc)->pdata.p_ganiy-(_obj)->oypos.u.intpos)

#define distance_from_ground(_obj) (ground_y-(_obj)->oypos.u.intpos)

/* get ptr to correct strike table entry */
#define get_char_stk(_obj,_offset) (*(strike_tables[(_obj)->ochar]+(_offset)))

/* get current strength of object */
#define get_my_strength(_obj) ((p1_obj==(_obj)) ? p1_bar:p2_bar)

/* edge setting fucntions */
#define set_noedge(_pa8) ((_pa8)->oflags2|=M_NOEDGE)
#define set_his_noedge set_noedge(current_proc->pdata.p_otherguy)
#define clear_noedge(_pa8) ((_pa8)->oflags2&=(~M_NOEDGE))

/* set half damage */
#define set_half_damage(_pa8) ((_pa8)->oflags2|=M_HALF_DAMAGE)

/* clear half damage */
#define clear_half_damage(_pa8) ((_pa8)->oflags2&=(~M_HALF_DAMAGE))

/* set half damage */
#define set_quarter_damage(_pa8) ((_pa8)->oflags2|=M_QUARTER_DAMAGE)

/* clear half damage */
#define clear_quarter_damage(_pa8) ((_pa8)->oflags2&=(~M_QUARTER_DAMAGE))


/* delete every process with id, no masking */
#define dallprc(_id) process_kill_class(_id,-1)

/* delete all objects with id (check all object lists), no masking */
#define dallobj(_id) kill_ocp(_id,-1)

/* get other players action */
#define get_his_action(_proc) (((_proc)->pdata.p_otherproc)->pdata.p_action)

/* set the no block bit */
#define set_no_block(_obj) ((_obj)->oflags2 |=M_NOBLOCK)

/* set the no flip bit */
#define set_noflip(_obj) ((_obj)->oflags2 |=M_NOFLIP)

/* set the no scroll bit */
#define set_noscroll(_obj) ((_obj)->oflags2 |=M_NOSCROLL)

/* clear the no scroll bit */
#define clear_noscroll(_obj) ((_obj)->oflags2 &=(~M_NOSCROLL))

/* set the bank_bit bit */
#define set_bank_bit(_obj) ((_obj)->oflags2 |=M_BANK1)

/* clear the bank_bit bit */
#define clear_bank_bit(_obj) ((_obj)->oflags2 &=(~M_BANK1))

/* set the no col bit */
#define set_nocol(_obj) ((_obj)->oflags2 |=M_NOCOL)

/* clear the no col bit */
#define clear_nocol(_obj) ((_obj)->oflags2 &=(~M_NOCOL))

/* is he close to edge */
#define is_he_close_to_edge(_cproc) am_i_close_to_edge((_cproc)->pdata.p_otherguy)

/* clear shadow bit */
#define clear_shadow_bit(_obj) (_obj)->oflags2 &=(~M_SHADOW)

/* set shadow bit */
#define set_shadow_bit(_obj) (_obj)->oflags2 |=(M_SHADOW)

/* clear no flip*/
#define clear_noflip(_obj) (_obj)->oflags2 &=(~M_NOFLIP)

/* set inviso */
#define set_inviso(_obj) clear_shadow_bit(_obj),(_obj)->oflags2 |=(M_INVISO)

/* clear inviso */
#define clear_inviso(_obj) (_obj)->oflags2 &=(~M_INVISO)

/* set ignore y */
#define set_ignore_y(_obj) (_obj)->oflags2 |=(M_IGNORE_Y)

/* clear ignore y */
#define clear_ignore_y(_obj) (_obj)->oflags2 &=(~M_IGNORE_Y)

/* takeover_him routine calling macros */
#define takeover_him(_jump) takeover_him_routine(TH_NORMAL,(_jump))

#define takeover_him_sr(_jump) takeover_him_routine(TH_SR,(_jump))

/* sans_repell */
#define sans_repell (f_norepell=0x40)

/* boss check stuff */
#define is_he_a_boss(_obj) am_i_a_boss((_obj))

/* make a process unkillable */
#define immune_a0(_proc) ((_proc)->procid |=0x8000)

/* make a process killable */
#define unimmune_a0(_proc) ((_proc)->procid &=0x7fff)

/* cause damage to other guy */
#define damage_to_him(_damage,_obj) damage_to_me((_damage),(_obj))

/* adjust_him_a0 */
#define adjust_him_a0(_a0) adjust_him_xy((_a0)&0xffff,((_a0)&0xffff0000)>>16)

/* adjust_him_x */
#define adjust_him_x(_a0) adjust_him_xy((_a0)&0xffff,0)

/* xfer_otherguy */
#define xfer_otherguy(_func) fastxfer(current_proc->pdata.p_otherproc,(_func))

/* delobj */
#define delobj(_obj) del_loop((_obj),&objlst)
#define delobj2(_obj) del_loop((_obj),&objlst2)

/* delobj_lista4 */
#define delobj_lista4(_obj,_olist) del_loop((_obj),(_olist))

/* delobjp(_proc) */
#define delobjp(_obj) delpal((_obj)),delobj((_obj))
#define delobjp2(_obj) delpal((_obj)),delobj2((_obj))

/* gso_dmawnz, no dma flags to worry about, bypass setting */
#define gso_dmawnz(_obj,_frm,_heap,_pal) (_obj)=get_single_obj((_frm),(_heap),(_pal),PFLAG_PALLOAD)

/* gso_dmawnz_ns, no dma flags to worry about, bypass setting, ns=noscroll*/
#define gso_dmawnz_ns(_obj,_frm,_heap,_pal)  (_obj)=get_single_obj((_frm),(_heap),(_pal),PFLAG_PALLOAD), set_noscroll(_obj)

/* gso_dmawnz_coor, no dma flags to worry about, bypass setting & set coords*/
#define gso_dmawnz_coord(_obj,_frm,_heap,_pal,_x,_y)  \
				{ \
				(_obj)=get_single_obj((_frm),(_heap),(_pal),PFLAG_PALLOAD); \
				set_xy_coordinates((_obj),(_x),(_y)) \
				}

/* gso_dmawnz_ns_coor, no dma flags to worry about, bypass setting & set coords*/
#define gso_dmawnz_ns_coor(_obj,_frm,_heap,_pal,_x,_y)  \
				{ \
				(_obj)=get_single_obj((_frm),(_heap),(_pal),PFLAG_PALLOAD); \
				set_xy_coordinates((_obj),(_x),(_y)); \
				set_noscroll(_obj); \
				}


/* get single object without setting palette */
#define get_single_obj_np(_frm,_heap,_pal) get_single_obj((_frm),(_heap),(_pal),PFLAG_CLEAR)

#define gso_dmawnz_np(_obj,_frm,_heap,_pal) (_obj)=get_single_obj_np((_frm),(_heap),(_pal))

/* set xy coordinates */
#define set_xy_coordinates(_obj,_x,_y) \
		{ \
		(_obj)->oxpos.u.intpos=(_x); \
		(_obj)->oypos.u.intpos=(_y); \
		}

/* shake_him_up */
#define shake_him_up(_obj,_xp,_yp,_slp,_cnt) \
	 	shake2((_obj),(_xp),(_yp),(_cnt),(_slp))

/* shake_up_a8 */
#define shake_a8_up(_obj,_xp,_yp,_slp,_cnt) \
	 	shake2((_obj),(_xp),(_yp),(_cnt),(_slp))

/* randu_min */
#define randu_minimum(_la0,_la1) ((_la1)+randu(_la0)-1)

/* center obj x */
#define center_obj_x(_obj) center_about_x((_obj),SCRRGT/2)

/* pick p1 or p2 based on obj->oid */
#define p1p2_pick(_obj,_p1,_p2) ((_obj)->oid==OID_P2)?(_p2):(_p1)

/* a8_front_plus_1 */
#define a8_front_plus_1(_obj) (_obj)->ozval=FRONT_Z+1

/* a5_front_plus_1 */
#define a5_front_plus_1(_obj) (_obj)->ozval=FRONT_Z+1

/* a5_front_minus_1 */
#define a5_front_minus_1(_obj) (_obj)->ozval=BACK_Z-1

/* a8_back_minus_1 */
#define a8_back_minus_1(_obj) (_obj)->ozval=BACK_Z-1

/* a5_back_minus_1 */
#define a5_back_minus_1(_obj) (_obj)->ozval=BACK_Z-1

/* exprc */
#define exprc(_id) process_exist((_id),-1)

/* zero my p hit */
#define zero_my_p_hit(_a13) (_a13)->pdata.p_hit=0

/* set game state */
#define SET_GAME_STATE(gs) gstate=(gstate != GS_AMODE) ? gs:GS_AMODE

/* combo dec p hit routines */
#define dec_my_p_hit decp3(current_proc)
#define dec_his_p_hit decp3(current_proc->pdata.p_otherproc)

/* bogus dlist */
#define bogus_dlist dlists=dlists_bogus

#define set_gameflag(_t_a0) f_gameflags|=(_t_a0)
#define test_gameflag(_t_a0) (f_gameflags & (_t_a0))

/* flight */
#define flight(_obj,_initx,_inity,_gravity,_anispeed,_anioffset) \
			current_proc->a6=NULL,flight_call(_obj,_initx,_inity,_gravity,_anispeed,_anioffset)


#endif /* __mk_util_h__ */
