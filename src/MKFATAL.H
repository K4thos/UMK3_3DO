/******************************************************************************
 File: mkfatal.h

 By: David Schwartz

 Date: Jan 1995

 (C) Williams Entertainment

 Mortal Kombat III fatality header
******************************************************************************/

#ifndef __mk_mkfatal_h__
#define __mk_mkfatal_h__

/*
 * 	DEFINITIONS
 */


/*
 *	RAM
 */

/*
 *	PROTOTYPES
 */
void do_fatality_1(void);
void do_fatality_2(void);

void kitana_kiss(void);
void r_stretch(void);
void r_ex2on2(void);
void grow_n_shake(void);

void kitana_decap(void);

void jade_impale(void);
void r_impale_upcut(void);
void impale_call(void);
void down_the_staff(void);

void jade_shaker(void);
void skeleton_explode(void);
void jade_shake_loop(WORD pa0,WORD pa1);
void r_jade_stab(void);

void mileena_suck_kiss(void);
void bone_vomit_proc(void);
void r_kiss_suck(void);

void mileena_nails(void);
void nail_spawn_proc(void);
void nail_proc(void);
void r_scared_of_mileena(void);
void nails_blood_spawner(void);

void scorpion_hell(void);
void another_scorpion(void);

void scorpion_fire(void);
void scorpion_remove_mask(void);
void scorpion_flame(void);
void kludge_flame_ani(short pa0,short pa1);
void double_flame_ani(void);
void scorpion_remove_mask(void);

void reptile_tongue(void);
void grab_a_slice(WORD pa9);
void reptile_open_mouth(void);
void retract_tongue(void);
void head_ani_sleep(void);
void head_ani(void);
void tongue_in_ani(void);
void grab_reptile_obj(WORD pa9);
void r_tongue(void);
void reptile_remove_mask(void);

void reptile_vomit(void);
void r_prevomit(void);
void r_vomit(void);

void ermac_super_slam(void);

void ermac_decap_attack(void);

void osz_head_rip(void);
void r_head_rip(void);

void smoke_arm(void);
void open_wide(void);
void smoke_dropping(void);
void eat_this_shit(void);

void smoke_blowup_earth(void);
void kill_all_but_master(void);
void earth_bomb(void);
void fbomb_gravity2(OBJECT *pa8,long pa0);
void bgrav3(OBJECT *pa8);
#define fbomb_gravity(_la8) bgrav3(_la8)

void kang_mk_game(void);
void mk_game_cabinet(void);
void r_mk_game_crush(void);

void kang_fire(void);
void kang_reform(void);
void lk_skeleton_burn(void);

void st_suck(void);
void soul_afloat(void);
void green_shit(void);

void st_spike(void);
void st_spiked(void);
void appearing_spikes(void);

void sg_pound(void);
void chop_off_his_height(WORD pa6);

void sg_flesh_rip(void);
void about_2b_ripped(void);
void flesh_ripped_off(void);
void ripped_skeleton(void);

void kabal_scare(void);
void sacred_2_death(void);
void face_and_head_ani(void);
void face_ani(WORD pa0);
void next_head_ani(void);
void my_ghost(void);

void kabal_inflator(void);
void pumped(void);

void lao_slicer(void);
void slice_hat_proc(void);
void slice_speed_set(void);
void slice_on_by(WORD pa0);
void sob3(void);
void sob4(void);
void hat_sliced_xfer(void);
void lop_a_piece_off(void);
void lopped_piece(void);

void lao_tornado(void);
void tornado_sucked(void);
void normal_spin_intro(void);
void nado_sounds(void);

void cyrax_helecopter(void);
void hele_cutup(void);
void hele_slice(void);
void hele_sleep(void);
void hele_sound(void);

void cyrax_self_destruct(void);
void animate_a11(WORD pa0);

void jax_grow(void);
void r_jax_stomp(void);
void grow_victum(void);
void ground_scaled_jax(OIMGTBL *pa5);
short get_scaled_x_size(OIMGTBL *pa5);

void jax_slice(void);
void get_sliced_up(void);
void post_sliced_up(void);
void slice_fall(void);
void cutup_correct_pal(void);

void robo_flame_throw(void);

void lia_hair_spin(void);
void slide_behind_hair(void);
void hair_spin(void);

void lia_scream_rip(void);
void flesh_rip_sound(void);
void r_scream_ripped(void);
void rip_ani(void);
void initial_skeleton_shake(void);
void remaining_skel(void);
void skel_blood(void);
#define skinny_spawn create_fx(FX_SKINNY_BLOOD)

void sw_plant_bomb(void);
void attach_obj_to_him(void);

void swat_taser(void);
void r_taser(void);

void sz_lift_n_freeze(void);
void frozen_half_ani(void);
void liftshake(void);
void lifts3(OBJECT *pa0,short pa2);
void freeze_into_boomer(void);

void sz_blow(void);
void r_ice_blow(void);

void ind_zap_kill(void);
void r_ind_lightning(void);

void ind_light(void);
void light_sound_proc(void);
void light_animator(void);

void sonya_kiss_crusher(void);
void crusher_orb(void);
void crush_stuggle(void);
void crush_duck(void);
void crush_blood(void);

void sonya_kiss(void);
void kiss_orb(void);
void kissani(void);
void orb_sleep_1(void);

void robo_skeleton_burn(void);
void scorp_skeleton_burn(void);
void sb_skeleton_burn(void);
void skburn3(void);
void skel_fire_proc(void);

void kano_skeleton(void);
void skin_fall(void);
void death_shake(void);

void kano_lazer(void);
void local_r_laser(void);

void fat_robo_crush(void);
void adjust_top_and_bottom(short pa0,short pa1);
void crush_him_more(short pa1,WORD pa9frm,short pa9off);
void crush_sleep_5(void);
void create_arm_obj(void);
void a10_frame_a9(void);

void shocker_shaker(WORD pa11);
void fatal_offset(OBJECT *obj,short pa0,short pa1);
void sans_repell_for_good(void);
void fatality_start_pause(void);
void init_death_blow(WORD pa1);
void death_blow_fader(void);
void make_db_tone(void);
void death_blow_complete(void);
void do_pit_fatality(void);
void gravity_ani_ysize(long pa1,long pa10);
void make_him_face_me(void);

void r_scared_of_skunk(void);
void r_scared_of_scorp(void);
void scared_pose(void);

/*
 *	MACROS
 */
#define adjust_piece(_pa6,_pa1) ((OBJECT *)(_pa6))->oypos.u.intpos+=(_pa1)

#define wfe_him takeover_him(wait_forever)

/* death_scream */
#define death_scream group_sound(9)

#define bogus_dlist dlists=dlists_bogus

#define local_victory_animation stack_jump(victory_animation)

#define make_him_invisible set_inviso(current_proc->pdata.p_otherguy)
#endif /* __mk_mkfatal_h__ */


/*
check ochar_reached:
check for umk3
check for dhs

KITANA KISS:
ANIM_MIL_KISS
ANIM_STRETCH (break down per character, yea) (per character), dont bother
FX_SWAT_BEXP
ts(0x60)
ts(0x24)
ts(0x25)

KITANA DECAP:
ANIM_FAN_DECAP
ochar_cutups

JADE IMPALE:
ANIM_ROD_IMPALE
rsnd_big_whoosh
ts(0x27)

JADE SHAKER:
ANIM_ROD_SHAKER
FX_SWAT_BEXP
rsnd_stab

SCROPION HELL:
tsound(0)
tsound(1)
rsnd_big_smack
rsnd_stab
tsound(0x24);

OSZ HEAD RIP: (DONE)
tsound(0x24);
tsound(0x25);
tsound(0x7b);

ERMAC DECAP: (test)
ANIM_SCORP_SUMMON
SWAT EXPLOSION STUFF
tsound(0x81);

MILEENA NAILS:
ANIM_SWALLOW_NAILS
ANIM_MIL_KISS
rsnd_stab

MILEENA SUCK KISS:
ANIM_MIL_KISS
ANIM_SUCK_PIT
SUCKED ANIMATIONS in femal ninja file
FX_MILEENA_BONE
tsound(0x60)
tsound(0x87)

SCORPION FIRE:
ANIM_SCORP_FIRE
ANIM_SCORP_FLAME
ANIM_SCORP_MASK_OFF
ochar_sound(a)
scorp_skeleton_burn
ts(0x1f)


REPTILE TONGUE:
ANIM_HI_TONGUE
ANIM_MED_TONGUE
ANIM_LO_TONGUE
ANIM_REP_CHOMP_HEAD
ochar_cutups
ANIM_SCORP_MASK_OFF
ts(0x25)
ts(0x24)


REPTILE VOMIT:
ANIM_VOMIT 
ANIM_SCORP_MASK_OFF
fx_smoke_guy
ochar_sound(0x10)
ochar_sound(0x13)

*/
