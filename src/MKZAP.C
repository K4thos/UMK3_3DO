/******************************************************************************
` File: mkzap.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Zap Routines
******************************************************************************/

/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkobj.h"
#include "mkos.h"
#include "mkgame.h"
#include "mkutil.h"
#include "mkani.h"
#include "mkpal.h"
#include "mkguys.h"
#include "mkboss.h"
#include "moves.h"
#include "mkdrone.h"
#include "mkslam.h"
#include "mkfx.h"
#include "mkjoy.h"
#include "mkreact.h"
#include "mkzap.h"
#include "mkprop.h"
#include "mksound.h"
#include "valloc.h"

JUMPTBL projectile_jumps [] =
{
	do_proj_kano_zap,				// 0x00
	do_proj_sonya_zap,				// 0x01
	do_proj_jax_zap1,				// 0x02
	do_proj_jax_zap2,				// 0x03
	do_proj_ind_zap,				// 0x04
	do_proj_sky_ice_on,			// 0x05
	do_proj_sky_ice_behind,			// 0x06
	do_proj_sky_ice_front,			// 0x07
	do_proj_sw_zap,			// 0x08
	do_proj_robo_zap,		// 0x09
	do_proj_robo_zap2,		// 0x0a
	do_proj_robo_net,		// 0x0b
	do_proj_sz_zap,			// 0x0c
	do_proj_lia_anglez,		// 0x0d
	do_proj_lao_zap,		// 0x0e
	do_proj_robo_bomb,		// 0x0f
	do_proj_robo_bomb_mid,	// 0x10
	do_proj_tusk_zap,		// 0x11
	do_proj_summon,			// 0x12
	do_proj_st_zap1,		// 0x13
	do_proj_st_zap2,		// 0x14
	do_proj_st_zap3,		// 0x15
	do_proj_lk_zap_hi,		// 0x16
	do_proj_lk_zap_lo,		// 0x17
	do_proj_sg_zap,			// 0x18
	do_proj_swat_bomb_hi,	// 0x19
	do_proj_swat_bomb_lo,	// 0x1a
	do_proj_lia_forward_zap,	// 0x1b
	do_proj_tusk_floor,		// 0x1c
	do_proj_sk_zap,			// 1d
	do_proj_smoke_spear,	// 1e
	do_proj_motaro_zap,		// 1f
	do_proj_kitana_zap,		//  20
	do_proj_jade_zap_med,		//  21
	do_proj_reptile_orb,		//  22
	do_proj_spit,			//  23
	do_proj_scorpion_spear,	//  24
	do_proj_jade_zap_hi,		//  25
	do_proj_jade_zap_lo,		//  26
	do_proj_jade_zap_ret,		//  27
	do_proj_reptile_orb_fast,	//  28
	do_proj_mileena_zap,		//  29
	do_proj_osz_zap,		//  2a
	do_proj_floor_ice,		//  2b
	do_proj_ermac_zap		//  2c
};

/******************************************************************************
 Function: void do_zap(WORD zindex)

 By: David Schwartz

 Date: Nov 1994

 Parameters: zindex zap table table index

 Returns: None

 Description:	all purpose zap routine
******************************************************************************/
void do_zap(WORD zindex)
{
	if (zindex>= (sizeof(projectile_jumps)/sizeof(projectile_jumps[0])))
	{
		reaction_exit(current_proc->pa8);
	}
	else
	{
		stack_jump(projectile_jumps[zindex]);
	}
	return;
}


//***********************************************************************
//***********************************************************************
void do_proj_ermac_zap(void)
{
	OBJECT *obj=current_proc->pa8;

	zap_init(ZINIT_SPEC_ACT,ACT_ERMAC_ZAP,0,obj);

	get_char_ani(ANIM_TABLE2,ANIM_OSZ_ZAP);

	if (q_is_he_react_fk()==SYSTEM_CARRY_CLR)
		animate_a0_frames(4,3);
	else animate_a0_frames(2,3);

	ochar_sound(9);

	get_char_ani(ANIM_TABLE1,ANIM_ZAP);		// MOD TO GET SK PROJECTILE ART
	get_proj_obj_m(obj);
	insert_object((OBJECT *)current_proc->a10,&objlst);

	adjust_xy_a5((OBJECT *)current_proc->a10,-SCX(0x28),-SCY(0x10));
	create_proj_proc(ermac_zap_proc);

	do_proj_sitting_duck(0x23);
	reaction_exit(obj);
}

WORD q_is_he_react_fk(void)
{
	if ((current_proc->pdata.p_otherproc)->pdata.p_action==ACT_REACT_FLIPK)
		q_yes;
	else q_no;
}

void ermac_zap_proc(void)
{
	OBJECT *obj=current_proc->pa8;

	set_proj_vel(obj,(q_is_he_react_fk()==SYSTEM_CARRY_CLR)?SCX(0xa0000):SCX(0xd0000),3);
	projectile_flight(current_proc,obj,0x19);

	ochar_sound(0xd);

	(short)current_proc->a7=obj->oxpos.u.intpos;
	multi_adjust_xy(obj,SCX(0x60),0);
	(short)current_proc->a10=obj->oxpos.u.intpos;
	obj->oxpos.u.intpos=(short)current_proc->a7;

	(short)current_proc->a11=obj->oypos.u.intpos+SCY(0x40);
	create_fx(FX_DAVE_EXPLODE);
	delete_proj_and_die(obj);
}

//***********************************************************************
//***********************************************************************
void do_proj_floor_ice(void)
{
	OBJECT *obj=current_proc->pa8;
	PROCESS *ptemp;

	zap_init(ZINIT_SPEC_ACT,ACT_FLOOR_ICE,0,obj);

	ochar_sound(0xe);

	animate2_a9(3,ANIM_OSZ_FLOORICE);

	ptemp=create_proj_proc(floor_ice_proc);
	ptemp->procid=p1p2_pick(obj,PID_FLOOR_ICE1,PID_FLOOR_ICE2);

	update_tsl(l_puddle,obj);

	do_proj_sitting_duck(0x20);
	reaction_exit(obj);
}

void floor_ice_proc(void)
{
	OBJECT *obj=current_proc->pa8;

	current_proc->pdata.p_action=0;

	obj->ozval=BACK_Z-1;

	find_ani2_part_a14(ANIM_OSZ_FLOORICE,3);
	mframew(3);		// part 1

	current_proc->a11=0x50;
	/* fip5 */
	do
	{
		process_sleep(1);
		if (q_is_he_a_boss()==SYSTEM_CARRY_CLR) 
		{
			if (winner_status!=0)
				break;
			if (proj_strike_check(obj,0x18)==SYSTEM_CARRY_SET) 
			{
				stack_jump(floor_ice_hit);
			}
		}
	}
	while(--current_proc->a11>0);

	stack_jump(ice_melt);
}

void floor_ice_hit(void) 
{
	OBJECT *obj=current_proc->pa8;

	//-DHS- code does nothing so skip
	(int)(current_proc->pdata.p_otherproc)->a10=leftmost_mpart(obj);
	(int)(current_proc->pdata.p_otherproc)->a11=rightmost_mpart(obj);

	// a11 = how much time remained
	/* fih6 */
	do
	{
		process_sleep(6);
	}
	while(get_his_action(current_proc)==ACT_SLIPPING);

	stack_jump(ice_melt);
}

void ice_melt(void) 
{
	OBJECT *obj=current_proc->pa8;

	dead_projectile(current_proc);
	find_ani2_part_a14(ANIM_OSZ_FLOORICE,4);
	mframew(4);
	delete_proj_and_die(obj);
}

//***********************************************************************
//***********************************************************************
void do_proj_osz_zap(void) 
{
	OBJECT *obj=current_proc->pa8;

	zap_init(ZINIT_SPEC_ACT,ACT_FORWARD_ICE,0,obj);

	ochar_sound(0xb);

	animate2_a9(0x4,ANIM_OSZ_ZAP);
	adjust_xy_a5(current_proc->pdata.p_slave,SCX(0x30),SCY(0x38));
	stack_jump(osz_forward_entry);
}

//***********************************************************************
//***********************************************************************
void do_proj_mileena_zap(void) 
{
	OBJECT *obj=current_proc->pa8;

	ochar_sound(0x8);
	if (am_i_airborn(current_proc->pa8)==SYSTEM_CARRY_SET) 
	{
		/* mileena_air_zap */
		zap_init(ZINIT_SPEC_ACT,ACT_MIL_AIR_ZAP,0,obj);

		find_ani2_part2(ANIM_THROW_SAI);
		do_next_a9_frame(obj);
		process_sleep(5);

		mframew(3);
		create_proj_proc(air_sai_proc)->pa8=obj;
		
		process_sleep(5);
		mframew(3);
		drop_down_land();
	}
	else
	{
		zap_init(ZINIT_SPEC_ACT,ACT_MILEENA_ZAP,0,obj);
		pose2_a9(0,ANIM_THROW_SAI);
		process_sleep(7);
		mframew(3);

		create_proj_proc(sai_proc)->pa8=obj;
		current_proc->pdata.p_action=ACT_PROJ_SD;

		process_sleep(0x25);
	}
	
	reaction_exit(obj);
}

void air_sai_proc(void) 
{
	sai3(SCX(0x1a),SCY(0x10));
}

void sai_proc(void) 
{
	sai3(SCX(0x3a),SCY(0x16));
}

void sai3(short pa0,short pa1) 
{	
	OBJECT *obj=current_proc->pa8;

	get_char_ani(ANIM_TABLE2,ANIM_SAI_FLY);
	current_proc->pa8=obj=get_proj_obj_m(obj);
	multi_adjust_xy(obj,pa0,pa1);
	insert_object(obj,&objlst);

	set_proj_vel(obj,SCX(0xb0000),4);

	current_proc->a10=3;
	projectile_flight(current_proc,obj,0x17);

	stop_a8(obj);
	find_ani2_part2(ANIM_SAI_FLY);
	mframew(3);
	delete_proj_and_die(obj);
} 

//***********************************************************************
//***********************************************************************
void do_proj_smoke_spear(void) 
{
	ssp2(smoke_spear_proc);
}

void do_proj_scorpion_spear(void) 
{
	ssp2(scorpion_spear_proc);
}

void ssp2(FUNC pa7) 
{
	OBJECT *obj=current_proc->pa8;
	PROCESS *pa0;

	zap_init(ZINIT_SPEC_ACT,ACT_SPEAR,0,obj);
	find_ani2_part_a14(ANIM_SCORP_THROWS,3);
	current_proc->a11=(ADDRESS)current_proc->pa9;

	if (get_his_action(current_proc)==ACT_REACTING || get_his_action(current_proc)==ACT_REACT_FLIPK)
		animate2_a9(1,ANIM_SCORP_THROWS);
	else animate2_a9(2,ANIM_SCORP_THROWS);
	get_single_spear_obj();
	pa0=create_proj_proc(pa7);
	(OBJECT *)pa0->a11=obj;
	pa0->pdata.p_store5=(ADDRESS)current_proc;

	set_proj_vel((OBJECT *)current_proc->a10,SCX(0xa0000),NEVER_ANI);

	current_proc->pdata.p_action=ACT_PROJ_SD;

	current_proc->p_comp_flag=PC_SCORP_WAIT;
	process_sleep(0x30);
	current_proc->p_comp_flag=PC_CLEAR;

	backwards_ani2(2,ANIM_SCORP_THROWS);
	current_proc->pdata.p_action=0;
	reaction_exit(obj);
}

void smoke_spear_proc(void) 
{
	ssp3(-SCX(0x48),SCY(0x15));
}

void scorpion_spear_proc(void) 
{
	ssp3(0,0);
}

void ssp3(short pa0,short pa1)
{
	OBJECT *obj=(OBJECT*)current_proc->a11;
	OBJECT *pa8=current_proc->pa8;
	OBJECT *oa10;
	WORD base_offset;

	obj->ochar=((OBJECT *)current_proc->a11)->ochar;
	
	current_proc->pa8=obj;
	find_ani2_part_a14(ANIM_SCORP_THROWS,4);
	oa10=(OBJECT*)current_proc->a10=get_proj_obj_m(obj);
	current_proc->pa8=pa8;
	oa10->header.t_width=0;
	oa10->header.t_height=0;
	insert_object((OBJECT *)current_proc->a10,&objlst);

	((OBJECT *)current_proc->a10)->ochar=obj->ochar;
	((OBJECT *)current_proc->a10)->oxvel.pos=pa8->oxvel.pos;

	match_ani_points(obj,oa10);

	current_proc->pa8=oa10;					// swap
	oa10=(OBJECT*)current_proc->a10=pa8;	
	pa8=current_proc->pa8;

	/* pa8 - mutli obj invis spear
	   current_proc->a10 (oa10) single obj vis spear
	   obj - scorp obj a11)
	*/
	adjust_xy_a5((OBJECT*)current_proc->a10,pa0,pa1);
	base_offset=oa10->header.t_xoffset;					// remember base offset

	/* ssp7 */
	do
	{
		(long)current_proc->a0=abs(pa8->oxpos.u.intpos-obj->oxpos.u.intpos);	// distacen from throw tip to thrower (A)

		if (obj->ochar==FT_SCORPION)
			(long)current_proc->a3=SCX(0xdf)-4;
		else (long)current_proc->a3=SCX(0xff);				// set clip value

		(long)current_proc->a3=abs((long)current_proc->a3-(long)current_proc->a0);			// offset into uv coord stuff from base

//-DHS rewrite to handle to length of rope and correct offset
		if (oa10->oflags & M_FLIPH) 
		{
			/* ssp6 */
//			current_proc->a2=oa10->header.t_xoffset;	// old uv offset
//			oa10->header.t_xoffset=(234-current_proc->a3)+base_offset;			// 234 size of scorpion spear
//			oa10->header.t_width=current_proc->a0;
//			oa10->oxpos.u.intpos-=((long)current_proc->a3-(long)current_proc->a2);
		}
		else
		{
			/* ssp8 */
//			current_proc->a2=oa10->header.t_xoffset-base_offset;		// prev u offset
//			oa10->header.t_xoffset=(current_proc->a3)+base_offset;	// set base u offset
//			oa10->oxpos.u.intpos+=((long)current_proc->a3-(long)current_proc->a2);
//			oa10->osize.u.xpos=current_proc->a0;					// set size of spear ????
//			oa10->header.t_width=current_proc->a0;			// set width to display

		}

		/* ssp9 */
		if (get_his_action(current_proc)!=ACT_REFLECT) 
		{
			if (proj_strike_check(pa8,0x14)==SYSTEM_CARRY_SET)
				sspear_hit();
		}

		/* spear_skip */
		process_sleep(1);
	}
	while(proj_onscreen_test((OBJECT*)current_proc->a10)==SYSTEM_CARRY_SET );

	delobjp((OBJECT*)current_proc->a10);

	delete_proj_and_die(pa8);
}

void sspear_hit(void) 
{
	OBJECT *obj=current_proc->pa8;
	PROCESS *ta0;

	dead_projectile(current_proc);
	process_kill(obj->oslink);
	delobjp(obj);

	obj=current_proc->pa8=(OBJECT*)current_proc->a10;
	stop_a8(obj);
	ta0=(PROCESS *)current_proc->pdata.p_store5;
	if (ta0->p_comp_flag!=PC_SCORP_WAIT)
		sspear_cancelled();

	if (f_block==1)
		sspear_blocked();

	fastxfer(ta0,scorp_rope_pull);
	ta0->a10=(ADDRESS)obj;
	ta0->pdata.p_slave=obj;
	process_suicide();
}

void sspear_cancelled(void) 
{
	ADDRESS ta10=current_proc->a10;

	takeover_him(r_null_speared);
	current_proc->a10=ta10;
	stack_jump(sspear_blocked);
}

void sspear_blocked(void)
{
	WORD ta11,ta9;


	stuff_rope_flag2(1);		// white
	stuff_rope_flag2(0);		
	stuff_rope_flag2(1);
	stuff_rope_flag2(0);
	stuff_rope_flag2(1);
	stuff_rope_flag2(0);
	delobjp((OBJECT*)current_proc->a10);
	process_suicide();
}

extern WORD CUM_P[];

void stuff_rope_flag2(WORD pa0)
{
	void *pal;

	switch (pa0)
	{
		case 0:
			pal=(((OBJECT*)current_proc->a10)->ochar==FT_SCORPION) ? SPEAR_P:CUM_P;
			break;
		case 1:
			pal=bpal_white_P;
			break;
	}
	swpal(pal,(OBJECT*)current_proc->a10);
	process_sleep(3);
	return;
}

void scorp_rope_pull(void)
{
	OBJECT *obj=current_proc->pa8;
	OBJECT *oa10=(OBJECT*)current_proc->a10;
	ADDRESS ta10;

	current_proc->pdata.p_action=ACT_ROPE_PULL;

	find_ani2_part_a14(ANIM_ROBO_TSPEAR,3);
	((ADDRESS*)current_proc->pa9)+=2;
	current_proc->pa8=oa10;
	animate(SINGLE_ANI,(OIMGTBL *)COMPUTE_ADDR(oa10->oheap,GET_LONG(current_proc->pa9)),oa10->oflags);
	current_proc->pa8=obj;

	current_proc->pdata.p_store8=oa10->header.t_xoffset;		// base offset in tpage for rope

	pose_him_a0(ANIM_KDOWN);			// him --> 1st frame of knocked down
	current_proc->a11=(ADDRESS)current_proc->pdata.p_otherguy;

	stop_him(current_proc->pdata.p_otherguy);
	/* ground_him */
	(current_proc->pdata.p_otherguy)->oypos.u.intpos=(current_proc->pdata.p_otherproc)->pdata.p_ganiy;
	current_proc->a10=(ADDRESS)oa10;

	((OBJECT*)current_proc->a11)->ozval=BACK_Z-1;		// speared dude
	oa10->ozval=BACK_Z;			// spear object
	/* proj_in_front */
	obj->ozval=FRONT_Z;


	/* define new action */
	(current_proc->pdata.p_otherproc)->pdata.p_action=ACT_SPEARED_SD;

	lineup_rope_with_him();
	pull_clip();

	ta10=6;
	/* sspull4 */
	do
	{
		double_shaker(-2);
		double_shaker(2);
	}
	while(--ta10>0);

	get_char_ani(ANIM_TABLE2,ANIM_ROBO_TSPEAR);
	find_part2();
	do_next_a9_frame(obj);
	process_sleep(4);
	
	xfer_otherguy(tugged_in_by_spear);
	process_sleep(3);

	ochar_sound(randu(2)-1);			//-DHS- new sound

	init_anirate(3);

	do
	{
		process_sleep(1);
		next_anirate();
		lineup_rope_with_him();
		pull_clip();

		if ((((OBJECT*)current_proc->a10)->oflags & M_FLIPH)==0)
			((OBJECT*)current_proc->a10)->oxpos.u.intpos=obj->oxpos.u.intpos;

	}
	while(((OBJECT*)current_proc->a11)->oxvel.pos!=0);

	delobjp((OBJECT*)current_proc->a10);
	current_proc->pdata.p_slave=NULL;

	update_tsl(l_spear,obj);
	reaction_exit(obj);
}

void pull_clip(void) 
{
	OBJECT *ta8=current_proc->pa8;
	long ta4;

	ta4=abs(get_x_dist-((OBJECT*)current_proc->a10)->osize.u.xpos);
	current_proc->pa8=(OBJECT*)current_proc->a10;
	clip_scorpion_rope(0,ta4);
	current_proc->pa8=ta8;
	return;
}

void clip_scorpion_rope(short pa3,short pa4) 
{
	OBJECT *obj=current_proc->pa8;

	if (obj->oflags & M_FLIPH) 
	{
		obj->oxpos.u.intpos+=(pa3-pa4);
		obj->header.t_xoffset=pa4;
	}
	else
	{
		obj->oxpos.u.intpos+=(pa4-pa3);
		obj->header.t_xoffset=pa4;
	}

	return;
}

void lineup_rope_with_him(void)
{
	OBJECT *obj=current_proc->pa8;
	OBJECT *oa10=(OBJECT *)current_proc->a10;

	(short)current_proc->a7=oa10->oypos.u.intpos;
	lineup_1pwm(oa10,current_proc->pdata.p_otherguy);
	current_proc->pa8=oa10;
	flip_single(oa10);
	oa10->oypos.u.intpos=(short)current_proc->a7;
	current_proc->pa8=obj;

#if 0
	/* clip rope */
	(short)current_proc->a0=abs(250-get_x_dist);

	((OBJECT *)current_proc->a10)->header.t_xoffset=current_proc->pdata.p_store8+(short)current_proc->a0;
	((OBJECT *)current_proc->a10)->header.t_width=(250-(short)current_proc->a0);

	if ((((OBJECT *)current_proc->a10)->oflags & M_FLIPH)==0)
		((OBJECT *)current_proc->a10)->oxpos.u.intpos+=(short)current_proc->a0;
#endif

	adjust_xy_a5(oa10,SCX(0x10),0);
	return;
}

void get_single_spear_obj(void) 
{
	OBJECT *obj=current_proc->pa8;
	OBJECT *pa8=current_proc->pa8;

	borrow_ochar_sound(2,FT_SCORPION);

	gso_dmawnz(obj,(ADDRESS)COMPUTE_ADDR(pa8->oheap,GET_LONG(current_proc->a11)),obj->oheap,0);
	alloc_cache((OIMGTBL *)COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->a11)),obj->oheap,obj);
	(OBJECT *)current_proc->a10=obj;
	insert_object(obj,&objlst);
	current_proc->pa8=pa8;
	lineup_1pwm(obj,pa8);

	adjust_xy_a5(obj,-SCX(0xd0),SCY(0x12));
	return;
}

void double_shaker(short pa1)
{
	multi_adjust_xy((OBJECT*)current_proc->a10,0,pa1);		// shake him
	multi_adjust_xy((OBJECT*)current_proc->a11,0,pa1);		// shake rope
	process_sleep(2);
	return;
}

void tugged_in_by_spear(void)
{
	towards_x_vel(current_proc,current_proc->pa8,SCX(0x80000));
	set_no_block(current_proc->pa8);

	/* papa4 */
	do
	{
		process_sleep(1);
	}
	while(get_x_dist>SCX(0x40));

	current_proc->pdata.p_action=ACT_SPEARED_SD;

	stop_me(current_proc->pa8);
	pose_a9(0,ANIM_DIZZY);
	init_anirate(8);

	current_proc->a11=8*8;
	/* papa6 */
	do
	{
		process_sleep(1);
		next_anirate();
		if (is_he_airborn()==SYSTEM_CARRY_SET)
			reaction_exit(current_proc->pa8);
	}
	while(--current_proc->a11>0);

	reaction_exit(current_proc->pa8);
}

//***********************************************************************
//***********************************************************************
void do_proj_spit(void) 
{
	OBJECT *obj=current_proc->pa8;

	zap_init(ZINIT_SPEC_ACT,ACT_SPIT,0,obj);

	ochar_sound(8);
	animate2_a9(3,ANIM_REP_SPIT);
	create_proj_proc(spit_proc);

	current_proc->pdata.p_action=ACT_PROJ_SD;

	process_sleep(0x1f);
	mframew(4);
	reaction_exit(obj);
}

void spit_proc(void) 
{
	find_ani2_part_a14(ANIM_REP_SPIT,3);

	spit_prezap(SCX(0x5e),SCY(0x1c),SCX(0x34),SCY(0xc));
	spit_prezap(SCX(0x96),SCY(0x1c),SCX(0x34),SCY(0xc));

	set_proj_vel(current_proc->pa8,SCX(0x80000),3);
	(void *)current_proc->a6=NULL;
	projectile_flight_call(current_proc->pa8,0x12);

	spit_prezap_hit();
}

void spit_prezap_hit(void) 
{
	OBJECT *obj=current_proc->pa8;

	stop_a8(obj);
	dead_projectile(current_proc);

	(OBJECT *)current_proc->a10=current_proc->pdata.p_otherguy;
	get_char_ani(ANIM_TABLE2,ANIM_REP_SPITG);
	create_fx(FX_SPIT_GLOB);

	match_me_with_him();
	flip_multi(obj);
	multi_adjust_xy(obj,-SCX(0xc0),SCY(0));

	find_ani2_part_a14(ANIM_REP_SPIT,4);
	mframew(3);
	delete_proj_and_die(obj);
}

void spit_prezap(short pa1x,short pa1y,short pa2x,short pa2y) 
{
	do_next_a9_frame(current_proc->pa8);

	if (strike_check_box(current_proc->pa8,0x12,pa1x,pa1y,pa2x,pa2y)==SYSTEM_CARRY_SET)
		spit_prezap_hit();
	return;
}

//***********************************************************************
//***********************************************************************
void do_proj_reptile_orb_fast(void) 
{
	update_tsl(l_orb_fast,current_proc->pa8);

	zap_init(ZINIT_SPEC_ACT,ACT_FAST_ORB,0,current_proc->pa8);
	orb3(SCX(0x70000));
}

void do_proj_reptile_orb(void)  
{
	update_tsl(l_orb_fast,current_proc->pa8);

	zap_init(ZINIT_SPEC_ACT,ACT_FAST_ORB,0,current_proc->pa8);
	orb3(SCX(0x30000));
}

void orb3(long pa11) 
{
	animate2_a9(3,ANIM_REP_RORB);

	(long)(create_proj_proc(orb_proc)->a10)=pa11;

	process_sleep(8);

	mframew(4);

	reaction_exit(current_proc->pa8);
}

void orb_proc(void) 
{
	find_ani2_part_a14(ANIM_REP_RORB,3);

	set_proj_vel(current_proc->pa8,(long)current_proc->a10,3);
	current_proc->a6=(ADDRESS)orb_calla;
	current_proc->a10=3;
	projectile_flight_call(current_proc->pa8,0x11);

	stop_a8(current_proc->pa8);
	ochar_sound(5);

	find_ani2_part_a14(ANIM_REP_RORB,4);
	mframew(3);
	delete_proj_and_die(current_proc->pa8);
}

void orb_calla(void) 
{
	if (--current_proc->a10==0) 
	{
		current_proc->a10=3;
		ochar_sound(0x12);
	}

	return;
}

//***********************************************************************
//***********************************************************************
void do_proj_jade_zap_ret(void) 
{
	jzap3(1);
}

void do_proj_jade_zap_lo(void) 
{
	jzap3(SCY(0x5000));
}

void do_proj_jade_zap_hi(void) 
{
	jzap3(-SCY(0xc000));
}

void do_proj_jade_zap_med(void) 
{
	jzap3(0);
}

void jzap3(long pa0) 
{
	zap_init(ZINIT_SPEC_ACT,ACT_JADE_ZAP,0,current_proc->pa8);

	ochar_sound(0);
	ochar_sound(3);
	ochar_sound(4);

	get_char_ani(ANIM_TABLE2,ANIM_JADE_ZAP);
	
	do_next_a9_frame(current_proc->pa8);
	process_sleep(6);
	mframew(4);

	set_proj_vel(current_proc->pdata.p_slave,SCX(0x90000),-1);

	(long)(create_proj_proc(boomerang_proc)->pdata.p_store8)=pa0;
	current_proc->pdata.p_action=ACT_PROJ_SD;
	process_sleep(0x20);
	mframew(3);
	reaction_exit(current_proc->pa8);
}

void boomerang_proc(void) 
{
	current_proc->pdata.p_store2=2;

	get_char_ani(ANIM_TABLE2,ANIM_BOOMERANG);
	set_proj_vel(current_proc->pa8,SCX(0x90000),2);
	current_proc->a6=(ADDRESS)boomerang_call;
	projectile_flight_call(current_proc->pa8,0x14);

	if (f_block==1) 
	{
		/* boomerang_blocked */
		init_anirate(2);
		(current_proc->pa8)->oyvel.pos=-SCY(0x30000);
		(current_proc->pa8)->oxvel.pos*=-1;

	}
	else
	{
		dead_projectile(current_proc);
		(current_proc->pa8)->ozval=FRONT_Z;
	}

	/* boom8, boom4 */
	do
	{
		next_anirate();
		process_sleep(1);
	}
	while(proj_onscreen_test(current_proc->pa8)==SYSTEM_CARRY_SET);
	
	delete_proj_and_die(current_proc->pa8);


}

void boomerang_call(void) 
{
	boomerang_trail();
	if (current_proc->pdata.p_store8==0)
		return;

	if (current_proc->pdata.p_store8==1) 
	{
		/* boom_return_check */
		if (proj_onscreen_test(current_proc->pa8)==SYSTEM_CARRY_SET)
			return;
		(current_proc->pa8)->oxvel.pos*=-1;
		current_proc->pdata.p_store1=(ADDRESS)boomerang_trail;
	}
	else
	{
		(current_proc->pa8)->oyvel.pos+=(long)(current_proc->pdata.p_store8);
	}
	
	return;
}

void boomerang_trail(void) 
{
	if (--current_proc->pdata.p_store2==0) 
	{
		CREATE(PID_BOOM_TRAIL,boom_trail_proc);
		current_proc->pdata.p_store2=2;
	}

	return;
}

void boom_trail_proc(void) 
{
	OBJECT *obj;

	(ADDRESS)obj=current_proc->a10=(ADDRESS)current_proc->pa8;
	obj->ochar=FT_JADE;
	get_char_ani(ANIM_TABLE2,ANIM_BOOMERANG);
	find_part2();

	gso_dmawnz(obj,(ADDRESS)COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->pa9)),obj->oheap,0);
	alloc_cache((OIMGTBL *)COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->pa9)++),obj->oheap,obj);
	lineup_1pwm(obj,(OBJECT *)current_proc->a10);
	insert_object(obj,&objlst);
	framew(3);
	delobjp(obj);
	process_suicide();
}

//***********************************************************************
//***********************************************************************
void do_proj_kitana_zap(void) 
{
	ochar_sound(0);

	update_tsl(l_throw_fan,current_proc->pa8);

	if (am_i_airborn(current_proc->pa8)==SYSTEM_CARRY_SET) 
	{
		/* kit_zap_air */
		zap_init(ZINIT_AIR,0,0,current_proc->pa8);
		current_proc->pdata.p_action=ACT_AIR_FAN;

		get_char_ani(ANIM_TABLE2,ANIM_KIT_AIRFAN);
		mframew(2);
		match_ani_points(current_proc->pa8,current_proc->pdata.p_slave);
		adjust_xy_a5(current_proc->pdata.p_slave,SCX(0),SCY(0x18));
		create_proj_proc(fan_proc);
		mframew(3);
		drop_down_land();
		reaction_exit(current_proc->pa8);
	}
	else
	{
		zap_init(ZINIT_SPEC_ACT,ACT_KITANA_ZAP,0,current_proc->pa8);

		animate_a9(3,ANIM_ZAP);
		match_ani_points(current_proc->pa8,current_proc->pdata.p_slave);
		adjust_xy_a5(current_proc->pdata.p_slave,SCX(0),SCY(0x18));
		create_proj_proc(fan_proc);
		mframew(3);
		current_proc->pdata.p_action=ACT_PROJ_SD;
		process_sleep(0x20);
		mframew(3);
		reaction_exit(current_proc->pa8);
	}
}

void fan_proc(void) 
{
	OBJECT *obj=current_proc->pa8;

	p1p2_pid_stuff(current_proc,obj,PID_FAN1,PID_FAN2);
	find_ani_part_a14(ANIM_ZAP,4);

	set_proj_vel(obj,SCX(0x80000),1);
	(ADDRESS *)current_proc->a6=NULL;
	projectile_flight_call(obj,0x13);

	dead_projectile(current_proc);
	obj->ozval=FRONT_Z;

	if (f_block==0) 
	{
		/* fan hit --> saw at the guy */
		ochar_sound(1);

		stop_a8(obj);

		do
		{
			(long)current_proc->a1=obj->oypos.pos;
			match_me_with_him();
			flip_multi(obj);
			obj->oypos.pos=(long)current_proc->a1;

			multi_adjust_xy(obj,-SCX(0x30),0);
			do_next_a9_frame(obj);
			do_next_a9_frame(obj);

			process_sleep(1);
		}
		while((current_proc->pdata.p_otherproc)->p_comp_flag==PC_RHAT_WAKE);

		set_proj_vel(obj,SCX(0xb0000),5);
	}

	/* fan_blocked */
	ochar_sound(3);

	init_anirate(1);
	obj->oyvel.pos=-SCY(0x50000);
	obj->oxvel.pos*=-1;

	/* fanpc */
	do
	{
		next_anirate();
		process_sleep(1);
	}
	while(proj_onscreen_test(obj)==SYSTEM_CARRY_SET);

	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void do_proj_motaro_zap(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	Motaro zap
******************************************************************************/
void do_proj_motaro_zap(void)
{
	OBJECT *obj=current_proc->pa8;
	void *ta9;

	zap_init(ZINIT_SPEC,0,0,obj);

	get_char_ani(ANIM_TABLE1,ANIM_MOT_ZAP);
	mframew(4);
	process_sleep(4);

	ta9=current_proc->pa9;
	find_part2();
	local_ochar_sound(5);
	create_proj_proc(motaro_zap_proc);
	current_proc->pa9=ta9;

	mframew(5);

	return;			// special case code for boss, since boss code returns to processing code
}

void motaro_zap_proc(void)
{
	OBJECT *obj=current_proc->pa8;

	set_proj_vel(obj,SCX(0xa0000),3);
	(FUNC)current_proc->a6=mot_zap_call;
	projectile_flight_call(obj,0x3);

	make_lineup_explode(obj,SCX(0),SCY(0));
	delete_proj_and_die(obj);
}

void mot_zap_call(void)
{
	(current_proc->pa8)->oyvel.pos+=SCY(0x5000);
	return;
}

/******************************************************************************
 Function: void do_proj_sk_zap(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	Shao Kahn zap
******************************************************************************/
void do_proj_sk_zap(void)
{
	OBJECT *obj=current_proc->pa8;
	void *ta9;

	zap_init(ZINIT_SPEC,0,0,obj);

	animate_a9(3,ANIM_ZAP);
	ta9=current_proc->pa9;

	ochar_sound(1);

	get_char_ani(ANIM_TABLE1,ANIM_SK_PROJECT);
	(OBJECT *)current_proc->a10=get_proj_obj_m(current_proc->pa8);
	insert_object((OBJECT *)current_proc->a10,&objlst);
	adjust_xy_a5((OBJECT *)current_proc->a10,-SCX(0x14),-SCY(0x6));

	create_proj_proc(sk_zap_proc);

	process_sleep(0x20);

	current_proc->pa9=ta9;

	mframew(4);				// straigten backup

	return;
	//reaction_exit(obj);
}

void sk_zap_proc(void)
{
	OBJECT *obj=current_proc->pa8;

	set_proj_vel(obj,SCX(0xa0000),3);
	projectile_flight(current_proc,obj,4);
	(short)current_proc->a7=obj->oxpos.u.intpos;
	multi_adjust_xy(obj,SCX(0x60),SCY(0));
	(short)current_proc->a10=obj->oxpos.u.intpos;
	obj->oxpos.u.intpos=(short)current_proc->a7;

	current_proc->a11=obj->oypos.u.intpos+SCY(0x40);
	create_fx(FX_DAVE_EXPLODE);

	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void do_proj_tusk_floor(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	kabal floor saw
******************************************************************************/
void do_proj_tusk_floor(void)
{
	void *ta9;
	OBJECT *ta10;

	init_special_act(ACT_FLOOR_BLADE,current_proc->pa8);
	local_ochar_sound(6);

	animate2_a9(4,ANIM_TUSK_GLOW);

	process_sleep(8);

	ta9=current_proc->pa9;
	(OBJECT *)current_proc->a10=ta10=current_proc->pdata.p_slave;
	current_proc->pdata.p_slave=NULL;
	get_char_ani(ANIM_TABLE2,ANIM_TUSK_BLADE);
	get_proj_obj_m(current_proc->pa8);
	create_proj_proc(blade_proc)->pdata.p_store1=(ADDRESS)current_proc;
	current_proc->pa9=ta9;
	(OBJECT *)current_proc->a10=ta10;
	current_proc->pdata.p_slave=ta10;

	process_sleep(0x18);

	mframew(3);
	reaction_exit(current_proc->pa8);
}

void blade_proc(void)
{
	OBJECT *obj=current_proc->pa8;

	current_proc->procid=PID_BLADE;
	obj->oypos.u.intpos=ground_y;			// place blade on ground

	flip_multi(obj);
	obj->oxpos.u.intpos=worldtlx.u.intpos+SCRRGT/2;
	multi_adjust_xy(obj,-SCX(0xc0),0);
	insert_object(obj,&objlst);

	get_char_ani(ANIM_TABLE2,ANIM_TUSK_BLADE);
	set_proj_vel(obj,SCX(0x80000),2);				// start moving

	local_ochar_sound(5);				// saw sound

	current_proc->a10=0x10;
	/* blade5 */
	do
	{
		process_sleep(1);
		saw_strike_check();
		next_anirate();
	}
	while(--current_proc->a10>0);

	/* blade7 */
	do
	{
		process_sleep(1);
		saw_strike_check();
		if (proj_onscreen_test(obj)==SYSTEM_CARRY_CLR)
			delete_proj_and_die(obj);
		next_anirate();
	}
	while(1);
}

void saw_strike_check(void)
{
	OBJECT *obj=current_proc->pa8;
	short ta6;

	if (q_is_he_a_boss()==SYSTEM_CARRY_SET)
		return;

	if (((PROCESS *)current_proc->pdata.p_store1)->pdata.p_flags & PM_REACTING)
		delete_proj_and_die(obj);


	if (strike_check_a0(obj,0x13)==SYSTEM_CARRY_CLR)
		return;

	/* saw_hit */
	current_proc->procid=PID_DEAD_BLADE;

	local_ochar_sound(5);
	stop_a8(obj);

	ta6=obj->oypos.u.intpos;
	lineup_a0_onto_a1(obj,current_proc->pdata.p_otherguy);
	multi_adjust_xy(obj,0,0);			// right under him
	obj->oypos.u.intpos=ta6;

	/* sawh5 */
	do
	{
		process_sleep(1);
		next_anirate();
	}
	while(q_is_he_reacting()==SYSTEM_CARRY_SET);

	set_proj_vel(obj,SCX(0x100000),-1);
	/* sawh3 */
	do
	{
		process_sleep(1);
		next_anirate();
	}
	while(proj_onscreen_test(obj)==SYSTEM_CARRY_SET);

	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void do_proj_lia_forward_zap(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	lia forward spit zap
******************************************************************************/
void do_proj_lia_forward_zap(void)
{
	OBJECT *obj=current_proc->pa8;
	PROCESS *pa0;

	zap_init(ZINIT_SPEC_ACT,ACT_LIA_FORWARD,0,obj);

	local_ochar_sound(2);			// fireball sound

	if (get_his_action(current_proc)==ACT_REACT_FLIPK)
	{
		current_proc->a11=1;			// fast version
		animate_a9(1,ANIM_ZAP);
	}
	else
	{
		current_proc->a11=0;			// normal version
		animate_a9(3,ANIM_ZAP);
	}

	find_ani_part2(ANIM_ZAP);
	get_proj_obj_m(obj);
	pa0=create_proj_proc(lia_forward_proc);

	if (current_proc->a11==1)
		pa0->a11=SCX(0xa0000);
	else pa0->a11=SCX(0x80000);

	current_proc->pdata.p_action=ACT_PROJ_SD;
	process_sleep(0x20);

	backwards_ani(4,ANIM_ZAP);

	reaction_exit(obj);
}

void lia_forward_proc(void)
{
	OBJECT *obj=current_proc->pa8;

	multi_adjust_xy(obj,SCX(0x8),SCY(0x20));
	insert_object(obj,&objlst);

	if (current_proc->a11==SCX(0x80000))
		animate_a0_frames(3,1);
	else animate_a0_frames(1,1);

	if (local_strike_check_box(0x15,SCX(0x4b),SCY(0x6),SCX(0x28),SCY(0x13))==SYSTEM_CARRY_CLR)
	{
		init_anirate(4);
		set_proj_vel(obj,current_proc->a11,4);
		projectile_flight(current_proc,obj,0x15);
	}

	/* lia_forward_hit */
	local_ochar_sound(3);

	lineup_a0_onto_a1(obj,current_proc->pdata.p_otherguy);
	flip_multi(obj);
	multi_adjust_xy(obj,-SCX(0x80),0);
	obj->oypos.u.intpos=ground_y-SCY(0xe0);

	shake_a11(0x4,0x8);
	stop_a8(obj);
	find_ani_part2(ANIM_PROJECTILE);
	((ADDRESS *)current_proc->pa9)+=3;
	mframew(2);
	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void do_proj_sz_zap(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	subzero ice zap
******************************************************************************/
void do_proj_sz_zap(void)
{
	OBJECT *obj=current_proc->pa8;

	zap_init(ZINIT_SPEC_ACT,ACT_FORWARD_ICE,0,obj);

	local_ochar_sound(0);

	animate_a9(3,ANIM_ZAP);

	adjust_xy_a5(current_proc->pdata.p_slave,SCX(0x30),SCY(0x38));		// adjust ice to sz hands

	stack_jump(osz_forward_entry);
}

void osz_forward_entry(void)
{
	ice_collision_check(SCX(0x60),SCY(0x2d),SCX(0x28),SCY(0x10));
	ice_collision_check(SCX(0x60),SCY(0x2d),SCX(0x28),SCY(0x10));
	ice_collision_check(SCX(0x88),SCY(0x2d),SCX(0x50),SCY(0x10));
	ice_collision_check(SCX(0xa4),SCY(0x2d),SCX(0x6c),SCY(0x10));
	ice_collision_check(SCX(0xb5),SCY(0x2d),SCX(0x66),SCY(0x10));

	create_proj_proc(sz_zap_proc);

	sz_post_zap();
}

void sz_post_zap(void)
{
	process_sleep(0x10);

	if ((current_proc->pa8)->ochar==FT_OLDSZ)
		find_ani2_part_a14(ANIM_OSZ_ZAP,3);
	else find_ani_part_a14(ANIM_ZAP,4);
	mframew(4);
	reaction_exit(current_proc->pa8);
}

void ice_collision_check(short pa0x,short pa0y,short pa1x,short pa1y)
{
	OBJECT *pa8=current_proc->pa8;

	current_proc->pa8=(OBJECT *)current_proc->a10;
	do_next_a9_frame(current_proc->pa8);
	current_proc->pa8=pa8;
	if (local_strike_check_box(0x13,pa0x,pa0y,pa1x,pa1y)==SYSTEM_CARRY_CLR)
	{
		process_sleep(3);
		return;
	}

	/* sz_prezap_hit */
	hob_ochar_sound(4,3);

	create_proj_proc(sz_zap_hit);
	sz_post_zap();
}

void sz_zap_proc(void)
{
	init_anirate(4);
	set_proj_vel(current_proc->pa8,SCX(0xa0000),-1);
	projectile_flight(current_proc,current_proc->pa8,0x13);
	stack_jump(sz_zap_hit);
}

void sz_zap_hit(void)
{
	OBJECT *obj=current_proc->pa8;
	short ta7;

	hob_ochar_sound(4,3);

	dead_projectile(current_proc);
	stop_a8(current_proc->pa8);

	ta7=obj->oypos.u.intpos;
	match_me_with_him();
	flip_multi(obj);
	multi_adjust_xy(obj,-SCX(0x90),0);
	obj->oypos.u.intpos=ta7;

	find_ani_part_a14(ANIM_ZAP,3);

	mframew(4);
	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void do_proj_robo_bomb_mid(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	robo bomb drop mid
******************************************************************************/
void do_proj_robo_bomb_mid(void)
{
	bomb3(robo_bomb_mid);
}

/******************************************************************************
 Function: void do_proj_robo_bomb(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	robo bomb drop full
******************************************************************************/
void do_proj_robo_bomb(void)
{
	bomb3(robo_bomb_full);
}

/******************************************************************************
 Function: void bomb3(FUNC pa11)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa11 - routine

 Returns: None

 Description:	robo bomb drop common stuff
******************************************************************************/
void bomb3(FUNC pa11)
{
	PROCESS *pa0;
	OBJECT *obj=current_proc->pa8;
	void *ta9;

	zap_init(ZINIT_SPEC_ACT,ACT_ROBO_BOMB,0,obj);

	robo_open_chest(obj);

	/* count # of bombs out there */
	current_proc->a3=0;
	pa0=(PROCESS *)&active_head;

	/* bomb5 */
	do
	{
		if (pa0->procid==PID_ROBO_BOMB)
			current_proc->a3++;

		pa0=pa0->plink;
	}
	while(pa0!=NULL);

	/* bomb4 */
	if (current_proc->a3<2 && process_exist(p1p2_pick(obj,PID_PROJ1,PID_PROJ2),0xffff)==NULL)			// only allow 2 bombs
	{
		ta9=current_proc->pa9;
		get_char_ani(ANIM_TABLE2,ANIM_ROBO_BOMB);
		current_proc->a11=(ADDRESS)current_proc->pa9;
		(OBJECT *)current_proc->a10=get_proj_obj_m(obj);
		insert_object((OBJECT *)current_proc->a10,&objlst);
		current_proc->pa9=ta9;

		a5_front_plus_1((OBJECT *)current_proc->a10);
		adjust_xy_a5((OBJECT *)current_proc->a10,SCX(0x00),SCY(0x28));

		create_proj_proc(pa11);				// let another proc handle things
		i_am_a_sitting_duck();
		process_sleep(0x16);
	}
	else
	{
		/* bomb_dud */
		i_am_a_sitting_duck();
		process_sleep(0x12);
	}
	robo_close_chest(obj);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void do_proj_swat_bomb_lo(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	swat bomb lo
******************************************************************************/
void do_proj_swat_bomb_lo(void)
{
	zap_init(ZINIT_SPEC_ACT,ACT_SWAT_BOMB_LO,0,current_proc->pa8);
	current_proc->a11=0;
	stack_jump(bomb33);
}

/******************************************************************************
 Function: void do_proj_swat_bomb_hi(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	swat bomb hi
******************************************************************************/
void do_proj_swat_bomb_hi(void)
{
	zap_init(ZINIT_SPEC_ACT,ACT_SWAT_BOMB_HI,0,current_proc->pa8);
	current_proc->a11=1;
	stack_jump(bomb33);
}

/******************************************************************************
 Function: void bomb33(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	swat bomb
******************************************************************************/
void bomb33(void)
{
	OBJECT *obj=current_proc->pa8;
	OBJECT *obomb;
	void *ta9;

	get_char_ani(ANIM_TABLE2,ANIM_SWAT_BOMB);
	animate_a0_frames(0x0003,0x0003);

	group_sound(0);				// haya
	ochar_sound(1);				// throw bomb sound

	ta9=current_proc->pa9;
	find_ani2_part2(ANIM_SWAT_BOMB);

	obomb=get_proj_obj_m(obj);
	adjust_xy_a5(obomb,SCX(0x20),SCY(0x20));
	insert_object(obomb,&objlst);
	current_proc->pa9=ta9;

	create_proj_proc(swat_bomb_proc);

	i_am_a_sitting_duck();

	mframew(4);
	process_sleep(0x10);

	backwards_ani2(3,ANIM_SWAT_BOMB);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void swat_bomb_proc(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	swat bomb process
******************************************************************************/
void swat_bomb_proc(void)
{
	find_ani2_part2(ANIM_SWAT_BOMB);

	if (current_proc->a11==0)
		(current_proc->pa8)->oyvel.pos=-SCY(0x30000);
	else (current_proc->pa8)->oyvel.pos=-SCY(0x60000);

	current_proc->a6=(ADDRESS)bomb_call;
	set_proj_vel(current_proc->pa8,SCX(0x80000),4);
	projectile_flight_call(current_proc->pa8,0x10);

	make_lineup_explode(current_proc->pa8,SCX(0),SCY(0));

	ochar_sound(0);			// bomb sound
	delete_proj_and_die(current_proc->pa8);
}

/******************************************************************************
 Function: void bomb_call(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	swat bomb call
******************************************************************************/
void bomb_call(void)
{
	(current_proc->pa8)->oyvel.pos+=SCY(0x6000);
	return;
}

/******************************************************************************
 Function: void make_lineup_explode(OBJECT *pa8,short pa11x,short pa11y)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa8 - object
		     pa11x - x adjust
			 pa11y - y adjust

 Returns: None

 Description:	lineup obj with fx
******************************************************************************/
void make_lineup_explode(OBJECT *pa8,short pa11x,short pa11y)
{
	if (pa8->oflags & M_FLIPH)
		pa11x=-pa11x;

	/* makel3 */
	current_proc->a11=(pa11y<<16)|(pa11x & 0x0000ffff);
	current_proc->a10=((pa8->oypos.u.intpos)<<16)|(pa8->oxpos.u.intpos & 0x0000ffff);
	create_fx(FX_LINEUP_EXP);

	return;
}

/******************************************************************************
 Function: void do_proj_sg_zap(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	she goro zap
******************************************************************************/
void do_proj_sg_zap(void)
{
	OBJECT *obj=current_proc->pa8;

	zap_init(ZINIT_SPEC_ACT,ACT_SG_ZAP,0,obj);

	local_ochar_sound(4);

	get_char_ani(ANIM_TABLE1,ANIM_ZAP);
	mframew(2);

	local_ochar_sound(3);
	create_proj_proc(sg_zap_proc);

	do_next_a9_frame(obj);

	do_proj_sitting_duck(0x2a);
	reaction_exit(obj);
}

/******************************************************************************
 Function: void sg_zap_proc(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	she goro zap process
******************************************************************************/
void sg_zap_proc(void)
{
	OBJECT *obj=current_proc->pa8;
	void *ta5;

	set_inviso(obj);

#if 0
	current_proc->pa9=a_small_explode;
	ta5=(void *)COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->pa9));
#endif

	get_char_ani(ANIM_TABLE2,ANIM_SG_SMEXPLODE);
	ta5=(void *)COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->pa9));
	((OIMGTBL *)ta5)++;
	swpal((void *)GET_LONG(ta5),obj);

	current_proc->pdata.p_action=ACT_SG_ZAP;

	multi_adjust_xy(obj,SCX(0x30),SCY(0x30));

	current_proc->pdata.p_store2=4;

	current_proc->a6=(ADDRESS)sg_trail_spawn;
	set_proj_vel(obj,SCX(0xa0000),NEVER_ANI);
	projectile_flight_call(obj,0x11);
	process_sleep(4);
	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void sg_trail_spawn(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	she goro zap trail spawning process
******************************************************************************/
void sg_trail_spawn(void)
{
	PROCESS *ptemp;
	void *heap;

	heap=(current_proc->pa8)->oheap;
	if (--current_proc->pdata.p_store2==0)
	{
		current_proc->pdata.p_store2=4;
		ptemp=CREATE(PID_POOF,sg_zap_trail);
		(void *)ptemp->a10=heap;
	}

	return;
}

/******************************************************************************
 Function: void sg_zap_trail(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	she goro zap trail process
******************************************************************************/
void sg_zap_trail(void)
{
	OBJECT *obj=current_proc->pa8;
	void *heap;

	(ADDRESS)heap=current_proc->a10;
	current_proc->a11=(ADDRESS)obj;
	gso_dmawnz(obj,(ADDRESS)COMPUTE_ADDR(heap,GET_LONG(current_proc->pa9)),heap,0);
	alloc_cache((OIMGTBL *)COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->pa9)),obj->oheap,obj);
	lineup_1pwm(obj,(OBJECT *)current_proc->a11);

	obj->ozval=1;
	insert_object(obj,&objlst);

	frame_a9(obj);
	process_sleep(2);

	obj->ozval=0;
	framew(2);

	delobjp(obj);

	process_suicide();
}

/******************************************************************************
 Function: void robo_bomb_mid(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	robo bomb process mid velocity
******************************************************************************/
void robo_bomb_mid(void)
{
	rbomb4(get_bomb_vel()>>1);
}

/******************************************************************************
 Function: void robo_bomb_full(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	robo bomb process full velocity
******************************************************************************/
void robo_bomb_full(void)
{
	rbomb4(get_bomb_vel());
}

/******************************************************************************
 Function: void rbomb4(long pa0)

 By: David Schwartz

 Date: feb 1995

 Parameters: pa0 - velocity

 Returns: None

 Description:	robo bomb process
******************************************************************************/
void rbomb4(long pa0)
{
	OBJECT *obj=current_proc->pa8;

	current_proc->procid=PID_ROBO_BOMB;		// set id

	set_proj_vel(obj,pa0,(WORD)pa0);

	current_proc->pdata.p_ganiy=ground_y-SCY(0x15);	// set ground pos

	init_anirate(3);
	get_char_ani(ANIM_TABLE2,ANIM_ROBO_BOMB);
	obj->oyvel.pos=0;
	bomb_gravity(obj);

	bomb_gravity2(obj,-SCY(0x50000));
	bomb_gravity2(obj,-SCY(0x30000));

	bgrav9(obj);
}

/******************************************************************************
 Function: void bomb_gravity2(OBJECT *obj,long vel)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	robo bomb gravity double time
******************************************************************************/
void bomb_gravity2(OBJECT *obj,long vel)
{
	obj->oyvel.pos=vel;		// up again
	obj->oxvel.pos>>=1;		// half speed
	current_proc->a11<<=1;	// double time

	bomb_gravity(obj);
}

/******************************************************************************
 Function: void bomb_gravity(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	robo bomb gravity time
******************************************************************************/
void bomb_gravity(OBJECT *obj)
{
	/* bgrav3 */
	do
	{
		process_sleep(1);
		next_anirate();
		current_proc->a11--;
		obj->oyvel.pos+=SCY(0x8000);		// gravity
	}
	while(obj->oyvel.pos<0 || obj->oypos.u.intpos<current_proc->pdata.p_ganiy);

	/* klang sound */
	current_proc->a0=8;			// klang sound 1

	current_proc->pdata.p_store2^=1;

	if (current_proc->pdata.p_store2!=0)
		current_proc->a0=9;			// klang sound 2

	local_ochar_sound(current_proc->a0);

	if ((long)current_proc->a11<0)
		bgrav9(obj);

	return;
}

/******************************************************************************
 Function: void bgrav9(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	robo bomb explode time
******************************************************************************/
void bgrav9(OBJECT *obj)
{
	short ta11;

	stop_a8(obj);

	ta11=0x40*1;
	do
	{
		process_sleep(1);
		next_anirate();
	}
	while(--ta11>0);

	if (f_death)					// no explode during fatality
		process_suicide();

	if (q_is_he_a_boss()==SYSTEM_CARRY_SET)
		delete_proj_and_die(obj);

	create_fx(FX_ROBO_BOMB_EXPLODE);
	local_ochar_sound(0xa);

	/* bomb collision check */
//	if ((current_proc->pdata.p_otherguy)->ochar==FT_MOTARO)
//		delete_proj_and_die(obj);

	set_inviso(obj);
	obj->ochar=FT_ROBO2;

	ta11=5;
	/* bgravb */
	do
	{
		process_sleep(1);
		if (strike_check_a0(obj,0x15)==SYSTEM_CARRY_SET)
			delete_proj_and_die(obj);
	}
	while(--ta11>0);

	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: long get_bomb_vel(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	compute robot bomb velocity
******************************************************************************/
long get_bomb_vel(void)
{
	long xdist;
	OBJECT *obj=current_proc->pa8;

	xdist=abs(obj->oxpos.u.intpos-((current_proc->pdata.p_otherguy)->oxpos.u.intpos));
	xdist<<=16;
	xdist/=0x20;					// a1=distance/time=speed
	current_proc->a11=0x20;
	return(xdist);
}

/******************************************************************************
 Function: void do_proj_robo_zap2(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	robo zap2
******************************************************************************/
void do_proj_robo_zap2(void)
{
	zap_init(ZINIT_SPEC_ACT,ACT_ROBO_ZAP2,0,current_proc->pa8);

	local_ochar_sound(0x0c);				// target sound

	rzap3(rocket2_proc);
}

/******************************************************************************
 Function: void do_proj_robo_zap(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	robo zap
******************************************************************************/
void do_proj_robo_zap(void)
{
	zap_init(ZINIT_SPEC_ACT,ACT_ROBO_ZAP,0,current_proc->pa8);

	rzap3(rocket1_proc);
}

/******************************************************************************
 Function: void rzap3(FUNC routine)

 By: David Schwartz

 Date: Jan 1995

 Parameters: routine - routine to execute for proj

 Returns: None

 Description:	robo zap common routine
******************************************************************************/
void rzap3(FUNC routine)
{
	OBJECT *obj=current_proc->pa8;

	robo_open_chest_fast(obj);

	setup_proj_obj(obj);

	((OBJECT *)current_proc->a10)->ozval=0;

	adjust_xy_a5((OBJECT *)current_proc->a10,SCX(7),SCY(45));

	create_proj_proc(routine);

	i_am_a_sitting_duck();

	process_sleep(0x20);

	robo_close_chest(obj);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void rocket1_proc(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	rocket1 projectile
******************************************************************************/
void rocket1_proc(void)
{
	OBJECT *obj=current_proc->pa8;

	p1p2_pid_stuff(current_proc,obj,PID_P1ROCKET1,PID_P2ROCKET1);

	ochar_sound(3);

	get_char_ani(ANIM_TABLE1,ANIM_PROJECTILE);
	find_part_a14(4);				// flat rocket animation

	if (q_his_react_flag_set()==SYSTEM_CARRY_SET)
		set_proj_vel(obj,SCX(0x70000),3);			// he reacting, fast rocket
	else set_proj_vel(obj,SCX(0x40000),3);

	current_proc->pdata.p_store3=0;		// puff counter

	(FUNC)current_proc->a6=rocket1_flight_call;
	projectile_flight_call(obj,0x12);

	rocket_explode_fx(obj);
}

/******************************************************************************
 Function: void rocket1_flight_call(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	rocket1 flight call
******************************************************************************/
void rocket1_flight_call(void)
{
	OBJECT *obj=current_proc->pa8;
	long temp;

	rocket_smoke(3);

	temp=obj->oxvel.pos;
	temp=abs(temp+(temp>>4));

	if (temp>SCX(0xe0000))
		temp=SCX(0xe0000);

	set_proj_vel(obj,temp,(WORD)(obj->oxvel.pos>>4));

	return;
}

/******************************************************************************
 Function: void rocket2_proc(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	rocket2 projectile
******************************************************************************/
void rocket2_proc(void)
{
	OBJECT *obj=current_proc->pa8;
	OBJECT *objtarget;
	void *ta9;
	ADDRESS target;

	p1p2_pid_stuff(current_proc,obj,PID_P1ROCKET2,PID_P2ROCKET2);

	ochar_sound(4);				// curvy rocket sound

	current_proc->pdata.p_store4=0x40*2;

	set_proj_vel(obj,SCX(0x60000),3);

	current_proc->a10=0;			// current animation
	current_proc->pa9=0;
	current_proc->a11=0x12;
	tell_world_stk(0x12);

	current_proc->pdata.p_store3=0;			// puff counter
	current_proc->pdata.p_store2=0;			// acell routine
	(WORD)current_proc->pdata.p_store1=rocket_routines[0].r_accelcnt;
	a8_front_plus_1(obj);				// pos rocket

	/* setup target */
	ta9=current_proc->pa9;
	get_char_ani(ANIM_TABLE2,ANIM_ROBO_TARGET);
	target=(ADDRESS)COMPUTE_ADDR(obj->oheap,(void *)GET_LONG(current_proc->pa9));
	gso_dmawnz(objtarget,target,obj->oheap,0);			// create target
	alloc_cache((void *)target,objtarget->oheap,objtarget);
	insert_object(objtarget,&objlst);
	current_proc->pdata.p_store5=5;							// ani speed
	((OBJECT *)current_proc->pdata.p_store6)=objtarget;		// target obj
	((void *)current_proc->pdata.p_store7)=current_proc->pa9;		// target ani
	a8_front_plus_1(objtarget);					// target in front
	current_proc->pa8=obj;
	current_proc->pa9=ta9;
	update_target();

	/* rocket2_loop */
	do
	{
		process_sleep(1);

		update_target();
		rocket_smoke(point_rocket(obj));

		next_anirate();

		if (--current_proc->pdata.p_store1==0)
		{
			/* newacc */
			current_proc->pdata.p_store2++;
			if (rocket_routines[current_proc->pdata.p_store2].r_accelcnt==0)
				rocket_explode(obj);
			else current_proc->pdata.p_store1=rocket_routines[current_proc->pdata.p_store2].r_accelcnt;
		}
		else
		{
			(rocket_routines[current_proc->pdata.p_store2].r_routine)();	// call aceel routine
		}
	}
	while(TRUE);
}


RROCKET rocket_routines[] =
{
	{rr_nothing,SCY(0x3333),0x08},
	{rr_up,SCY(0xa000),0x10},
	{rocket_hunt,SCY(0x3333),0x3333}
};

/******************************************************************************
 Function: void rocket_hunt(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	rocket hunt for target
******************************************************************************/
void rocket_hunt(void)
{
	OBJECT *obj=current_proc->pa8;
	OBJECT *obj_other=current_proc->pdata.p_otherguy;
	long xval,yval,hypo;

	do
	{
		/* set dir vector */
		xval=obj_other->oxpos.u.intpos-obj->oxpos.u.intpos;
		yval=obj_other->oypos.u.intpos-obj->oypos.u.intpos+SCY(0x40);		// 0x40 factor, about dudes chest

		/* if close to enemy --> boom!! */
		hypo=get_rough_hypotenuse(xval,yval);

		if (hypo<SCX(010))
			rocket_explode(obj);

		xval*=SCX(0x40000);				// master normal vel
		yval*=SCX(0x40000);				// master normal vel

		hypo=get_rough_hypotenuse(xval,yval);
		hypo>>=16;
		xval/=hypo;					// normalize vector
		yval/=hypo;					// normalize vector

		xval+=(obj->oxvel.pos-(obj->oxvel.pos>>4));	// take off 1/8 of old vel & add in new vector
		yval+=(obj->oyvel.pos-(obj->oyvel.pos>>4));

		hypo=get_rough_hypotenuse(xval,yval);
		hypo>>=16;
		obj->oxvel.pos=(xval/hypo)*5;					// normalize vector	& store vel
		obj->oyvel.pos=(yval/hypo)*5;					// normalize vector & store vel

		process_sleep(1);
		update_target();
		rocket_smoke(point_rocket(obj));

		next_anirate();

		if (f_death)
			break;				// fatality started = exit
	}
	while(--current_proc->pdata.p_store4!=0);

	rocket_explode(obj);
}

/******************************************************************************
 Function: void update_target(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: current_proc->pdata.p_store5 = ani speed
			 current_proc->pdata.p_store6 = target obj
			 current_proc->pdata.p_store7 = target ani

 Returns: None

 Description:	update target on otherdude
******************************************************************************/
void update_target(void)
{
	OBJECT *ta8;
	void *ta9;

	(OBJECT *)current_proc->a10=current_proc->pdata.p_otherguy;
	ta8=current_proc->pa8;
	ta9=current_proc->pa9;

	current_proc->pa8=((OBJECT*)current_proc->pdata.p_store6);
	current_proc->pa9=((void *)current_proc->pdata.p_store7);

	if (--current_proc->pdata.p_store5==0)
	{
		frame_a9(current_proc->pa8);
		current_proc->pdata.p_store5=5;
	}

	lineup_1pwm(current_proc->pa8,(OBJECT *)current_proc->a10);
	multi_adjust_xy(current_proc->pa8,-SCX(0x10),SCY(0x20));

	current_proc->pa8=ta8;
	current_proc->pa9=ta9;

	return;
}

/******************************************************************************
 Function: void rocket_smoke(WORD soffset)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	rocket smoke routine
******************************************************************************/
void rocket_smoke(WORD soffset)
{
	PROCESS *ptemp;

	current_proc->pdata.p_store3^=1;

	if (current_proc->pdata.p_store3!=0)
	{
		ptemp=create_fx(FX_ROCKET_PUFF);
		ptemp->a11=soffset;
	}

	return;
}

/******************************************************************************
 Function: void rr_up(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	rocket up routine
******************************************************************************/
void rr_up(void)
{
	OBJECT *obj=current_proc->pa8;
	long temp;

	temp=((long)(rocket_routines[current_proc->pdata.p_store2].r_grav)) & 0xffff;
	obj->oyvel.pos-=temp;
	return;
}

/******************************************************************************
 Function: void rr_down(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	rocket down routine
******************************************************************************/
void rr_down(void)
{
	OBJECT *obj=current_proc->pa8;
	long temp;

	temp=((long)(rocket_routines[current_proc->pdata.p_store2].r_grav)) & 0xffff;
	obj->oyvel.pos+=temp;

	return;
}

/******************************************************************************
 Function: void rr_nothing(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	do nothing
******************************************************************************/
void rr_nothing(void)
{
	return;
}

/******************************************************************************
 Function: void rocket_explode(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: obj - rocket object to blow up

 Returns: None

 Description:	rocket explode routine
******************************************************************************/
void rocket_explode(OBJECT *obj)
{
	short xpos,ypos;

	if (((current_proc->pdata.p_otherguy)->ochar==FT_MOTARO || get_his_action(current_proc)==ACT_REFLECT) && strike_check_a0_test(obj,0x12)==SYSTEM_CARRY_SET)
	{
		/* rocket2_reflect */
		delete_target_obj();
		benedict_arnold_projectile(current_proc->pdata.p_otherguy);		// new originator

		xpos=obj->oxpos.u.intpos;
		ypos=obj->oypos.u.intpos;

		match_ani_points(current_proc->pdata.p_otherguy,obj);

		obj->oxpos.u.intpos=xpos;
		obj->oypos.u.intpos=ypos;
		rocket2_proc();
	}
	else
	{
		/* rexp3 */
		proj_strike_check(obj,0x12);
		delete_target_obj();
		rocket_explode_fx(obj);
	}
}

/******************************************************************************
 Function: void delete_target_obj(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:	delete target
******************************************************************************/
void delete_target_obj(void)
{
	OBJECT *ta8=current_proc->pa8;

	current_proc->pa8=((OBJECT*)current_proc->pdata.p_store6);
	delobjp(current_proc->pa8);
	current_proc->pa8=ta8;
	return;
}

/******************************************************************************
 Function: void rocket_explode_fx(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: obj - rocket object to blow up

 Returns: None

 Description:	rocket explode special effects routine
******************************************************************************/
void rocket_explode_fx(OBJECT *obj)
{
	PROCESS *ptemp;

	local_ochar_sound(10);

	ptemp=create_fx(FX_DAVE_EXPLODE);
	ptemp->a10=obj->oxpos.u.intpos;
	ptemp->a11=obj->oypos.u.intpos;

	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: WORD point_rocket(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: smoke dir

 Description:	point the rocket
******************************************************************************/
WORD point_rocket(OBJECT *obj)
{
	WORD flags;
	long temp;

	flags=M_FLIPH;					// neg = we want flipped
	if (obj->oxvel.pos>0)
		flags=0;					// pos = we want un-flipped

	if ((obj->oflags & M_FLIPH)!=flags)	// are we faciong correct direction
		flip_multi(obj);			// no reverse

	(long)current_proc->a0=abs(obj->oxvel.pos);	// grab current vel
	(long)current_proc->a1=abs(obj->oyvel.pos);

	/* 12 o'clock ??? */
	current_proc->a5=0;
	temp=(long)current_proc->a0;
	temp=(((temp<<1)+temp)+(temp<<2))>>1;		// x*7/2
	if ((long)current_proc->a1<temp)			// y <=x*7/2, keep checking
	{
		/* 1 o'clock */
		current_proc->a5++;
		if ((long)current_proc->a1<(long)current_proc->a0) 		// y<=x, keep checking
		{
			/* 2 o'clock */
			current_proc->a5++;

			temp=(long)current_proc->a1;
			temp=(((temp<<1)+temp)+(temp<<2))>>1;		// y*7/2
			if ((long)current_proc->a0>temp) 		// x<=y*7/2 ---> keep checking
			{
				/* 3 o'clock */
				current_proc->a5++;
			}
		}
	}

	/* point3 */
	if (obj->oyvel.pos>0)
		current_proc->a5+=4;			// zero || neg y --> point at 3 oclock

	/* point_up */
	(void *)temp=current_proc->pa9;
	get_char_ani(ANIM_TABLE1,ANIM_PROJECTILE);

	find_part_a14(current_proc->a5+1);
	current_proc->a7=(ADDRESS)current_proc->pa9;
	current_proc->pa9=(void *)temp;

	if (current_proc->a7!=current_proc->a10)
	{
		(ADDRESS)current_proc->pa9=current_proc->a7;
		current_proc->a10=current_proc->a7;
	}

	return((WORD)current_proc->a5);
}

/******************************************************************************
 Function: void do_proj_robo_net(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	robo net routine
******************************************************************************/
void do_proj_robo_net(void)
{
	void *ta9;
	OBJECT *obj=current_proc->pa8;
	OBJECT *onet;

	zap_init(ZINIT_SPEC_ACT,ACT_ROBO_NET,0,obj);

	robo_open_chest(obj);

	update_tsl(l_net,obj);

	ochar_sound(0x05);			// net sound

	ta9=current_proc->pa9;
	get_char_ani(ANIM_TABLE2,ANIM_ROBO_NET);
	current_proc->a11=(ADDRESS)current_proc->pa9;

	onet=get_proj_obj_m(obj);
	insert_object(onet,&objlst);
	current_proc->pa9=ta9;

	onet->ozval=FRONT_Z+1;		// behind robo

	adjust_xy_a5(onet,-SCX(0x18),SCY(0x30));
	create_proj_proc(net_proc);		// let another proc handle things

	i_am_a_sitting_duck();

	process_sleep(0x1a);

	get_char_ani(ANIM_TABLE2,ANIM_ROBO_CHEST);
	backwards_ani(4,-1);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void net_proc(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	net procees
******************************************************************************/
void net_proc(void)
{
	OBJECT *obj=current_proc->pa8;

	get_char_ani(ANIM_TABLE2,ANIM_ROBO_NET);

	obj->oyvel.pos=SCY(0x8000);		// head downwards

	set_proj_vel(obj,SCX(0x60000),2);

	projectile_flight(current_proc,obj,0x11);

	stop_a8(obj);
	find_ani2_part2(ANIM_ROBO_NET);
	init_anirate(4);
	(OBJECT *)current_proc->a10=current_proc->pdata.p_otherguy;

	/* netgrab3 */
	do
	{
		match_ani_points((OBJECT *)current_proc->a10,obj);
		flip_multi(obj);

		multi_adjust_xy(obj,-SCX(0x20),SCY(0x42));
		process_sleep(1);
		next_anirate();
	}
	while((current_proc->pdata.p_otherproc)->p_comp_flag==PC_NET_WAKE);	// mod from orig code

	/* kill any stumble sound from other dude */
	kill_group_sound(8,current_proc->pdata.p_otherguy);

	/* net explode */
	local_ochar_sound(0x15);
	find_ani2_part2(ANIM_ROBO_NET);
	find_part2();
	mframew(2);
	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void robo_open_chest(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	robot chest opens
******************************************************************************/
void robo_open_chest(OBJECT *obj)
{
	ochar_sound(0x0b);		// open chest sound

	get_char_ani(ANIM_TABLE2,ANIM_ROBO_CHEST);
	mframew(4);

	return;
}

void robo_open_chest_fast(OBJECT *obj)
{
	ochar_sound(0x0b);		// open chest sound
	get_char_ani(ANIM_TABLE2,ANIM_ROBO_CHEST);

	if (q_his_react_flag_set()==SYSTEM_CARRY_SET)
	{
		mframew(2);
	}
	else
	{
		mframew(4);
	}

	return;

}

/******************************************************************************
 Function: void robo_close_chest(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	robot chest closes
******************************************************************************/
void robo_close_chest(OBJECT *obj)
{
	get_char_ani(ANIM_TABLE2,ANIM_ROBO_CHEST);
	backwards_ani(4,-1);
	return;
}

/******************************************************************************
 Function: void lk_zap_air(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	kano zap air routine
******************************************************************************/
void lk_zap_air(void)
{
	OBJECT *obj=current_proc->pa8;

	zap_init(ZINIT_AIR,0,0,obj);
	current_proc->pdata.p_action=ACT_LK_ZAP_AIR;

	local_ochar_sound(0);

	get_char_ani(ANIM_TABLE2,ANIM_LK_ZAP_AIR);
	mframew(2);
	lk_zap_entry();
}

/******************************************************************************
 Function: void do_proj_lk_zap_lo(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	kano zap lo routine
******************************************************************************/
void do_proj_lk_zap_lo(void)
{
	OBJECT *obj=current_proc->pa8;

	if (am_i_airborn(obj)==SYSTEM_CARRY_SET)
		lk_zap_air();

	zap_init(ZINIT_SPEC_ACT,ACT_LK_ZAP_LO,0,obj);

	local_ochar_sound(0);

	animate2_a9(0x0004,ANIM_LK_ZAP_LO);
	lk_zap_entry();
}

/******************************************************************************
 Function: void do_proj_lk_zap_hi(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	kano zap hi routine
******************************************************************************/
void do_proj_lk_zap_hi(void)
{
	OBJECT *obj=current_proc->pa8;

	if (am_i_airborn(obj)==SYSTEM_CARRY_SET)
		lk_zap_air();

	zap_init(ZINIT_SPEC_ACT,ACT_LK_ZAP_HI,0,obj);

	local_ochar_sound(0);

	animate_a9(0x0004,ANIM_ZAP);
	lk_zap_entry();
}

/******************************************************************************
 Function: void lk_zap_entry(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	kano zap routine
******************************************************************************/
void lk_zap_entry(void)
{
	OBJECT *obj=current_proc->pa8;
	WORD results;

	current_proc->pa8=current_proc->pdata.p_slave;
	results=local_strike_check_box(0x11,SCX(0x39),SCY(0x0009),SCX(0x0029),SCY(0x20));
	current_proc->pa8=obj;
	if (results==SYSTEM_CARRY_SET)
	 	lk_prezap_hit();
	 else
	 {
	 	if (lk_prezap(SCX(0x80),SCY(0xe),SCX(0x71),SCY(0x13))==SYSTEM_CARRY_SET)
	 		lk_prezap_hit();
	 	else
	 	{
	 		if (lk_prezap(SCX(0xa4),SCY(0x10),SCX(0x5f),SCY(0x13))==SYSTEM_CARRY_SET)
	 			lk_prezap_hit();
	 		else
	 		{
	 			if (lk_prezap(SCX(0xd0),SCY(0x10),SCX(0x71),SCY(0x13))==SYSTEM_CARRY_SET)
	 				lk_prezap_hit();
	 			else create_proj_proc(lk_zap_proc);
	 		}
	 	}
	}

	/* lkzap5 */
	if (am_i_airborn(obj)==SYSTEM_CARRY_SET)
	{
		/* post_air */
		get_char_ani(ANIM_TABLE2,ANIM_LK_ZAP_AIR);
		find_part_a14(3);
		mframew(2);
		drop_down_land();
	}
	else
	{
		i_am_a_sitting_duck();

		if (am_i_short()==SYSTEM_CARRY_SET)
		{
			process_sleep(0x18);
			get_char_ani(ANIM_TABLE2,ANIM_LK_ZAP_LO);
			find_part_a14(3);
			mframew(4);
		}
		else
		{
			process_sleep(0x10);
			find_ani_part_a14(ANIM_ZAP,3);
			mframew(4);
		}
	}

	reaction_exit(obj);
}

/******************************************************************************
 Function: void lk_zap_proc(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	zap process
******************************************************************************/
void lk_zap_proc(void)
{
	set_proj_vel(current_proc->pa8,SCX(0x80000),4);
	projectile_flight(current_proc,current_proc->pa8,0x11);

	hob_ochar_sound(0x0001,0x0003);

	make_dragon_explode();
	delete_proj_and_die(current_proc->pa8);
}

/******************************************************************************
 Function: WORD lk_prezap(short pa1x,short pa1x,short pa2x,short pa2y)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa1x,y - offset of coll box
			 pa2x,y	- size of coll box

 Returns: SYSTEM_CARRY_SET - hit
		  SYSTEM_CARRY_CLR - miss

 Description:	prezap routine
******************************************************************************/
WORD lk_prezap(short pa1x,short pa1y,short pa2x,short pa2y)
{
	OBJECT *obj=current_proc->pa8;
	WORD results;

	do_next_a9_frame(obj);
	process_sleep(4);
	current_proc->pa8=current_proc->pdata.p_slave;
	results=local_strike_check_box(0x11,pa1x,pa1y,pa2x,pa2y);
	current_proc->pa8=obj;
	return(results);
}

/******************************************************************************
 Function: void lk_prezap_hit(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	prezap hit
******************************************************************************/
void lk_prezap_hit(void)
{
	OBJECT *ta8;

	ta8=current_proc->pa8;
	(ADDRESS)current_proc->pa8=current_proc->a10;
	make_dragon_explode();
	current_proc->pa8=ta8;
	delete_slave();

	return;
}

/******************************************************************************
 Function: void make_dragon_explode(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	dragon explode routine
******************************************************************************/
void make_dragon_explode(void)
{
	OBJECT *obj=current_proc->pa8;

	ochar_sound(1);

	make_lineup_explode(obj,abs(rightmost_mpart(obj)-leftmost_mpart(obj)),SCY(0x0010));
	return;
}

/******************************************************************************
 Function: void do_proj_st_zap1(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	shang tsu zap 1
******************************************************************************/
void do_proj_st_zap1(void)
{
	stz1(ACT_ST_ZAP1,1);
}

/******************************************************************************
 Function: void do_proj_st_zap2(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	shang tsu zap 2
******************************************************************************/
void do_proj_st_zap2(void)
{
	stz1(ACT_ST_ZAP2,2);
}

/******************************************************************************
 Function: void do_proj_st_zap3(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	shang tsu zap 3
******************************************************************************/
void do_proj_st_zap3(void)
{
	stz1(ACT_ST_ZAP3,3);
}

/******************************************************************************
 Function: void stz1(WORD pa1,WORD pa11)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa1 - action
			 pa11 - shot count

 Returns: None

 Description:	shang tsu zap common routine
******************************************************************************/
void stz1(WORD pa1,WORD pa11)
{
	OBJECT *obj=current_proc->pa8;

	zap_init(ZINIT_SPEC_ACT,pa1,0,obj);

	st_zap_jsrp(0);
	if (--pa11!=0)
	{
		process_sleep(8);

		st_zap_jsrp(3);
		if (--pa11!=0)
		{
			process_sleep(8);
			st_zap_jsrp(3);
		}
	}

	/* stz2 */
	i_am_a_sitting_duck();
	process_sleep(8);

	get_char_ani(ANIM_TABLE1,ANIM_ZAP);
	((ADDRESS *)current_proc->pa9)++;
	do_next_a9_frame(obj);
	process_sleep(4);
	((ADDRESS *)current_proc->pa9)-=2;
	do_next_a9_frame(obj);
	process_sleep(4);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void st_zap_jsrp(WORD pa10)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa10 - anim offset

 Returns: None

 Description:	shang tsu zap fire routine
******************************************************************************/
void st_zap_jsrp(WORD pa10)
{
	local_ochar_sound(3);
	get_char_ani(ANIM_TABLE1,ANIM_ZAP);
	((ADDRESS *)current_proc->pa9)+=pa10;
	mframew(3);
	create_proj_proc(skull_proc);
	update_tsl(l_zap,current_proc->pa8);
	return;
}

/******************************************************************************
 Function: void skull_proc(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: none

 Returns: None

 Description:	shang tsu skull routine
******************************************************************************/
void skull_proc(void)
{
	get_char_ani(ANIM_TABLE1,ANIM_PROJECTILE);

	tell_world_stk(0x11);

	if ((current_proc->pdata.p_otherguy)->ochar!=FT_MOTARO) 		// motaro = reflect
	{
		if (get_his_action(current_proc)!=ACT_REFLECT)
		{
			if (local_strike_check_box(0x11,SCX(0x57),SCY(0x15),SCX(0x52),SCY(0x16))==SYSTEM_CARRY_SET)
			{
				/* pre_skull_hit */
				make_lineup_explode(current_proc->pa8,SCX(0x42),SCY(0x23));
				goto SKULL_DIE;
			}

		}
	}

	/* skull3 */
	set_proj_vel(current_proc->pa8,SCX(0x80000),4);
	projectile_flight(current_proc,current_proc->pa8,0x11);

	/* skull_hit */
	make_lineup_explode(current_proc->pa8,SCX(0x92),SCY(0x23));

SKULL_DIE:
	hob_ochar_sound(0x0002,0x0006);
	delete_proj_and_die(current_proc->pa8);
}

/******************************************************************************
 Function: void do_proj_summon(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	shang tsu summon
******************************************************************************/
void do_proj_summon(void)
{
	PROCESS *ptemp;
	OBJECT *obj=current_proc->pa8;

	zap_init(ZINIT_SPEC_ACT,ACT_FLOOR_ZAP,0,obj);

	animate2_a9(3,ANIM_ST_SUMMON);

	ochar_sound(0x01);

	ptemp=CREATE(PID_SUMMON,master_summon_proc);
	stuff_others_a0(ptemp);

	mframew(3);
	process_sleep(0x10);
	mframew(2);

	reaction_exit(obj);
}

/******************************************************************************
 Function: inline void stuff_others_a0(PROCESS *pa0)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa0 - process to update

 Returns: None

 Description:	stuff other info into process
******************************************************************************/
inline void stuff_others_a0(PROCESS *pa0)
{
	pa0->pdata.p_otherguy=current_proc->pdata.p_otherguy;
	pa0->pdata.p_otherproc=current_proc->pdata.p_otherproc;

	return;
}

/******************************************************************************
 Function: void master_summon_proc(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	summons process
******************************************************************************/
void master_summon_proc(void)
{
	OBJECT *obj=current_proc->pa8;

	if (is_he_right(current_proc)==SYSTEM_CARRY_SET)
	{
		(long)current_proc->a11=SCRRGT-SCX(0x70)+obj->oxpos.u.intpos;
		(long)current_proc->a10=-SCX(0x60);
	}
	else
	{
		(long)current_proc->a11=-(SCRRGT-SCX(0x70))+(obj->oxpos.u.intpos);
		(long)current_proc->a10=SCX(0x60);
	}

	summon_spawn();
	summon_spawn();
	summon_spawn();
	process_suicide();
}

/******************************************************************************
 Function: void summon_spawn(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	summons spawning process
******************************************************************************/
void summon_spawn(void)
{
	PROCESS *ptemp;

	ptemp=CREATE(PID_SUMMON,summon_proc);
	stuff_others_a0(ptemp);
	process_sleep(0x12);
	(long)current_proc->a11+=(long)current_proc->a10;
	return;
}

/******************************************************************************
 Function: void summon_proc(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	summons process
******************************************************************************/
void summon_proc(void)
{
	ADDRESS ta11;
	OBJECT *obj;

	ta11=current_proc->a11;
	get_char_ani(ANIM_TABLE2,ANIM_ST_FIRE);
	current_proc->a11=(ADDRESS)current_proc->pa9;
	get_proj_obj_m(current_proc->pa8);
	current_proc->a11=ta11;
	((OBJECT *)current_proc->a10)->oxpos.u.intpos=(short)ta11;

	((OBJECT *)current_proc->a10)->oypos.u.intpos=ground_y-SCY(0x10);
	insert_object((OBJECT *)current_proc->a10,&objlst);
	current_proc->pa8=(OBJECT *)current_proc->a10;

	mframew(4);
	CREATE(PID_SUMMON,summon_flame_animator);

	get_char_ani(ANIM_TABLE2,ANIM_ST_FIRE);
	current_proc->a11=(ADDRESS)current_proc->pa9;
	get_proj_obj_m(current_proc->pa8);
	match_ani_points(current_proc->pa8,(OBJECT *)current_proc->a10);
	insert_object((OBJECT *)current_proc->a10,&objlst);
	find_part2();
	find_part2();
	obj=current_proc->pa8=(OBJECT *)current_proc->a10;

	current_proc->pdata.p_action=ACT_FLOOR_ZAP;

	/* flight loop */
	obj->oyvel.pos=-SCY(0x80000);
	init_anirate(4);
	current_proc->pdata.p_store1=0;

	/* sum5 */
	do
	{
		process_sleep(1);
		if (current_proc->pdata.p_store1==0 && q_is_he_a_boss()==SYSTEM_CARRY_CLR)	// boss=no hit
		{
			if (strike_check_a0(obj,0x12)==SYSTEM_CARRY_SET)
				current_proc->pdata.p_store1=1;
		}

		/* sum7 */
		next_anirate();
	}
	while(lowest_mpart(obj)>worldtly.u.intpos);

	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void summon_flame_animator(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	summons flame animator process
******************************************************************************/
void summon_flame_animator(void)
{
	OBJECT *obj=current_proc->pa8;

	obj->ochar=FT_ST;

	local_ochar_sound(0);

	shake_a11(0x0006,0x0006);

	mframew(4);

	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void do_proj_tusk_zap(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	tusk zap routine
******************************************************************************/
void do_proj_tusk_zap(void)
{
	OBJECT *obj=current_proc->pa8;

	if (am_i_airborn(obj)==SYSTEM_CARRY_SET)
	{
		/* tusk air zap */
		zap_init(ZINIT_AIR,ACT_TUSK_ZAP_AIR,0,obj);
		current_proc->pdata.p_action=ACT_TUSK_ZAP_AIR;

		local_ochar_sound(1);

		get_char_ani(ANIM_TABLE2,ANIM_TUSK_AIR_ZAP);
		mframew(2);
		setup_proj_obj(obj);
		adjust_xy_a5((OBJECT *)current_proc->a10,0,SCY(0x10));
		create_proj_proc(photon_proc);

		away_x_vel(current_proc,obj,SCX(0x60000));
		process_sleep(6);
		away_x_vel(current_proc,obj,SCX(0x40000));
		mframew(4);
		drop_down_land_jump();
	}
	else
	{
		/* tusk_ground_zap */
		zap_init(ZINIT_SPEC_ACT,ACT_TUSK_ZAP,0,obj);

		animate_a9(3,ANIM_ZAP);
		setup_proj_obj(obj);
		adjust_xy_a5((OBJECT *)current_proc->a10,SCX(0x10),SCY(0x37));
		create_proj_proc(photon_proc);

		local_ochar_sound(1);

		i_am_a_sitting_duck();

		mframew(4);
		delete_slave();

		mframew(5);

		reaction_exit(obj);
	}
}

/******************************************************************************
 Function: void photon_proc(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	fire photons
******************************************************************************/
void photon_proc(void)
{
	PROCESS *ptemp;

	get_char_ani(ANIM_TABLE1,ANIM_PROJECTILE);
	set_proj_vel(current_proc->pa8,SCX(0xa0000),3);
	projectile_flight(current_proc,current_proc->pa8,0x11);

	ptemp=create_fx(FX_DAVE_EXPLODE);
	ptemp->a10=(current_proc->pa8)->oxpos.u.intpos;
	ptemp->a11=(current_proc->pa8)->oypos.u.intpos;

	delete_proj_and_die(current_proc->pa8);
}

/******************************************************************************
 Function: void do_proj_kano_zap(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	kano zap routine
******************************************************************************/
void do_proj_kano_zap(void)
{
	OBJECT *obj=current_proc->pa8;

	zap_init(ZINIT_SPEC_ACT,ACT_KANO_ZAP,0,obj);

	ochar_sound(0x01);			// knife throw sound

	pose_a9(0x0000,ANIM_ZAP);
	process_sleep(3);

	setup_proj_obj(obj);
	double_mframew(3);
	create_proj_proc(kano_zap_proc);

	mframew(4);

	update_tsl(l_zap,obj);

	do_proj_sitting_duck(0x18);
	reaction_exit(obj);
}

/******************************************************************************
 Function: void kano_zap_proc(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	kano zap routine
******************************************************************************/
void kano_zap_proc(void)
{
	OBJECT *obj=current_proc->pa8;


	if (local_strike_check_box(0x15,SCX(0x60),SCY(0x3),SCX(0x44),SCY(0x22))==SYSTEM_CARRY_SET)
	{
		multi_adjust_xy(obj,SCX(0x38),SCY(0x20));
	}
	else
	{
		/* kzap2 */
		find_ani_part2(ANIM_PROJECTILE);

		set_proj_vel(obj,SCX(0x80000),1);

		projectile_flight(current_proc,obj,0x15);

		stop_a8(obj);

		multi_adjust_xy(obj,SCX(13),SCY(0));
	}

	/* kzap7 */
	hob_ochar_sound(0x0002,0x0003);

	dead_projectile(current_proc);

	find_ani_part_a14(ANIM_PROJECTILE,3);

	mframew(4);

	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void do_proj_ind_zap(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	ind zap routine
******************************************************************************/
void do_proj_ind_zap(void)
{
	OBJECT *obj=current_proc->pa8;
	OBJECT *obj_slave;
	ADDRESS a10;

	zap_init(ZINIT_SPEC_ACT,ACT_IND_ZAP,0,obj);

	ochar_sound(0);

	animate_a9((q_his_react_flag_set()==SYSTEM_CARRY_SET)?1:3,ANIM_ZAP);

	obj_slave=current_proc->pdata.p_slave;
	a10=current_proc->a10;
	setup_proj_obj(obj);
	create_proj_proc(ind_zap_proc);
	current_proc->a10=a10;
	current_proc->pdata.p_slave=obj_slave;

	i_am_a_sitting_duck();

	mframew(3);
	do_proj_sitting_duck(0x1);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void ind_zap_proc(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:	ind zap process
******************************************************************************/
void ind_zap_proc(void)
{
	OBJECT *obj=current_proc->pa8;

	find_ani_part_a14(ANIM_ZAP,0x03);
	do_next_a9_frame(obj);

	ochar_sound(1);

	set_proj_vel(obj,SCX(0xa0000),4);

	projectile_flight(current_proc,obj,0x12);

	hob_ochar_sound(0x0003,0x0004);

	stop_a8(obj);

	find_ani_part_a14(ANIM_ZAP,4);

	mframew(4);

	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void do_proj_lia_anglez(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	lia anglez
******************************************************************************/
void do_proj_lia_anglez(void)
{
	OBJECT *obj=current_proc->pa8;
	WORD tp_action;

	tp_action=current_proc->pdata.p_action;

	zap_init(ZINIT_AIR,0,0,obj);
	current_proc->pdata.p_action=ACT_LIA_ANGLEZ;

	local_ochar_sound(2);

	get_char_ani(ANIM_TABLE2,ANIM_LIA_ANGLE_ZAP);
	mframew(2);

	(OBJECT *)current_proc->a11=current_proc->pdata.p_slave;
	create_proj_proc(angle_zap_proc);
	current_proc->pdata.p_slave=(OBJECT *)current_proc->a11;		// restore my flash

	update_tsl(l_liazap,obj);				// mark down this event

	i_am_a_sitting_duck();

	mframew(5);				// flash face on and off
	delete_slave();			// remove flashing face
	mframew(2);				// back to normal
	obj->oyvel.pos=SCY(0x10000);	// head start down

	if (tp_action != ACT_HOVER)
		land_on_yer_feet();
	else do_body_propell(PROP_LIA_AFLOAT);

}

/******************************************************************************
 Function: void angle_zap_proc(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	angle  zap routine
******************************************************************************/
void angle_zap_proc(void)
{
	OBJECT *obj=current_proc->pa8;

	setup_proj_obj(obj);

	obj=current_proc->pa8=(OBJECT *)current_proc->a10;
	get_char_ani(ANIM_TABLE1,ANIM_PROJECTILE);

	angle_zap_jsrp(obj,0x12);
	angle_zap_jsrp(obj,0x12);
	angle_zap_jsrp(obj,0x13);
	multi_adjust_xy(obj,SCX(9),SCY(9));
	angle_zap_jsrp(obj,0x13);

	obj->oyvel.pos=SCY(0x80000);
	set_proj_vel(obj,SCX(0x80000),4);
	current_proc->a6=(ADDRESS)angle_zap_call;
	projectile_flight_call(obj,0x14);

	angle_zap_hit(obj);
}

/******************************************************************************
 Function: void angle_zap_call(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	angle zap call routine
******************************************************************************/
void angle_zap_call(void)
{
	OBJECT *obj=current_proc->pa8;

	if ((ground_y-obj->oypos.u.intpos)>SCY(0x88))
		return;						// haven't hit ground

	obj->oypos.u.intpos=ground_y-SCY(0x98)+16;			// place on ground

	local_ochar_sound(3);

	angle_zap_explode(obj);
}

/******************************************************************************
 Function: void angle_zap_explode(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	angle zap explode routine
******************************************************************************/
void angle_zap_explode(OBJECT *obj)
{
	shake_a11(0x0004,0x0008);
	stop_a8(obj);
	find_ani_part2(ANIM_PROJECTILE);
	mframew(2);
	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void angle_zap_jsrp(OBJECT *obj,WORD strkoff)

 By: David Schwartz

 Date: Jan 1995

 Parameters: obj - proj obj
			 strkoff -strike offset

 Returns: None

 Description:	angle zap collision routine
******************************************************************************/
void angle_zap_jsrp(OBJECT *obj,WORD strkoff)
{
	short loop=4;

	do_next_a9_frame(obj);
	do
	{
		if (strike_check_a0(obj,strkoff)==SYSTEM_CARRY_SET)
			angle_zap_hit(obj);
		else process_sleep(1);
	}
	while(--loop>0);

	return;
}

/******************************************************************************
 Function: void angle_zap_hit(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: obj - proj obj

 Returns: None

 Description:	angle zap hit routine
******************************************************************************/
void angle_zap_hit(OBJECT *obj)
{
	if (f_block==0)
		local_ochar_sound(3);
	else local_ochar_sound(4);

	if (obj->oflags & M_FLIPH)
		obj->oxpos.u.intpos=leftmost_mpart(obj);
	else obj->oxpos.u.intpos=rightmost_mpart(obj);

	obj->oypos.u.intpos=lowest_mpart(obj);

	multi_adjust_xy(obj,-SCX(0x80),-SCY(0x80));

	angle_zap_explode(obj);
}

/******************************************************************************
 Function: void do_proj_sonya_zap(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	sonya zap routine
******************************************************************************/
void do_proj_sonya_zap(void)
{
	OBJECT *obj=current_proc->pa8;

	zap_init(ZINIT_SPEC_ACT,ACT_SONYA_ZAP,0,obj);

	ochar_sound(0);

	get_char_ani(ANIM_TABLE1,ANIM_ZAP);

	mframew(3);

	create_proj_proc(sonya_zap_proc);

	do_proj_sitting_duck(0x23);

	reaction_exit(obj);
}
/******************************************************************************
 Function: void lao_zap_call(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	lao zap call routine during flight, adjust gravity
******************************************************************************/
void lao_zap_call(void)
{
	if (--current_proc->pdata.p_store2 ==0)
	{
		CREATE(PID_HAT_RING,hat_ring_proc);
		current_proc->pdata.p_store2=2;
	}

	/* lzc5 */
	(current_proc->pa8)->oyvel.pos+=SCY(0x3000);
	return;
}

/******************************************************************************
 Function: void hat_ring_proc(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	lao zap hat ring fx process
******************************************************************************/
void hat_ring_proc(void)
{
	OBJECT *obj=current_proc->pa8;

	(OBJECT *)current_proc->a10=obj;

	obj->ochar=FT_LAO;
	get_char_ani(ANIM_TABLE2,ANIM_LAO_HAT_RING);

	gso_dmawnz(obj,(ADDRESS)COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->pa9)),obj->oheap,0);

	lineup_1pwm(obj,(OBJECT *)current_proc->a10);

	insert_object(obj,&objlst);

	framew(4);

	delobjp(obj);

	process_suicide();
}

/******************************************************************************
 Function: void lao_hat_proc(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	lao zap routine
******************************************************************************/
void lao_hat_proc(void)
{
	OBJECT *obj=current_proc->pa8;
	short temp;

	(ADDRESS)current_proc->pa9=ANIM_ZAP;
	find_ani_part_a14(ANIM_ZAP,4);					// hat fly ani
	current_proc->a10=(ADDRESS)current_proc->pa9;
	do_next_a9_frame(obj);							// 1st frame
	(ADDRESS)current_proc->pa9=current_proc->a10;

	obj->oyvel.pos=-SCY(0x20000);					// proj yvel
	set_proj_vel(obj,SCX(0x80000),4);				// proj xvel

	current_proc->pdata.p_store2=1;

	(FUNC)current_proc->a6=lao_zap_call;
	projectile_flight_call(obj,0x11); 				// return = hit

	/* hatp8, hat hit !!! */
	dead_projectile(current_proc);
	obj->ozval=FRONT_Z;

	if (f_block==0)
	{
		/* hat hit --> saw at the guy */
		local_ochar_sound(2);			// hat hit sound

		stop_a8(obj);

		/* saw3 */
		do
		{
			temp=obj->oypos.u.intpos;
			match_me_with_him();
			flip_multi(obj);
			obj->oypos.u.intpos=temp;

			multi_adjust_xy(obj,-SCX(0x68),0);
			next_anirate();

			process_sleep(1);
		}
		while((current_proc->pdata.p_otherproc)->p_comp_flag==PC_RHAT_WAKE);

		set_proj_vel(obj,SCX(0xb0000),5);
	}

	/* hat blocked */
	local_ochar_sound(3);				// hat blocked sound

	find_ani_part_a14(ANIM_ZAP,5);			// angle fly

	init_anirate(4);

	obj->oyvel.pos=-SCY(0x50000);

	flip_multi(obj);
	multi_adjust_xy(obj,-SCX(0xa0),-SCY(0x08));

	obj->oxvel.pos=-obj->oxvel.pos;

	/* hatpc */
	do
	{
		next_anirate();
		process_sleep(1);
	}
	while(proj_onscreen_test(obj)==SYSTEM_CARRY_SET);

	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void do_proj_lao_zap(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	lao zap routine
******************************************************************************/
void do_proj_lao_zap(void)
{
	OBJECT *obj=current_proc->pa8;

	zap_init(ZINIT_SPEC_ACT,ACT_LAO_ZAP,0,obj);

	local_ochar_sound(1);			// hat throw

	animate_a9(0x0003,ANIM_ZAP);

	create_proj_proc(lao_hat_proc);


	mframew(3);

	mframew(5);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void sonya_zap_proc(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	sonya zap routine
******************************************************************************/
void sonya_zap_proc(void)
{
	OBJECT *obj=current_proc->pa8;

	if (proj_strike_check(obj,0x13)==SYSTEM_CARRY_SET)
	{
		multi_adjust_xy(obj,-SCX(0x10),SCY(0));
	}
	else
	{
		get_char_ani(ANIM_TABLE1,ANIM_PROJECTILE);

		set_proj_vel(obj,SCX(0x80000),3);

		projectile_flight(current_proc,obj,0x12);

		stop_a8(obj);

		multi_adjust_xy(obj,SCX(13),SCY(0));
	}

	create_fx(FX_SONYA_GLOBE);

	hob_ochar_sound(0x0001,0x0002);

	dead_projectile(current_proc);

	set_proj_vel(obj,SCX(0x20000),0);

	find_ani_part2(ANIM_PROJECTILE);

	mframew(4);

	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void do_proj_sw_zap(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	sw zap routine
******************************************************************************/
void do_proj_sw_zap(void)
{
	OBJECT *obj=current_proc->pa8;

	zap_init(ZINIT_SPEC_ACT,ACT_SW_ZAP,0,obj);

	animate_a9(0x0003,ANIM_ZAP);
	create_proj_proc(swat_proj_proc);

	do_proj_sitting_duck(0x18);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void swat_proj_proc(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	swat proj routine
******************************************************************************/
void swat_proj_proc(void)
{
	OBJECT *obj=current_proc->pa8;

	find_ani_part2(ANIM_ZAP);		// part 2 = bullets ani

	do_next_a9_frame(obj);
	if (proj_strike_check(obj,0x13)==SYSTEM_CARRY_SET)	// prezap check
		sw_proj_hit(obj);
	process_sleep(3);

	do_next_a9_frame(obj);
	if (proj_strike_check(obj,0x14)==SYSTEM_CARRY_SET)	// prezap check
		sw_proj_hit(obj);
	process_sleep(3);

	do_next_a9_frame(obj);
	if (proj_strike_check(obj,0x14)==SYSTEM_CARRY_SET)	// prezap check
		sw_proj_hit(obj);
	process_sleep(3);

	set_proj_vel(obj,SCX(0xa0000),4);

	projectile_flight(current_proc,obj,0x12);

	sw_proj_hit(obj);
}

/******************************************************************************
 Function: void sw_proj_hit(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters:  obj - proj object

 Returns: None

 Description:	swat projectile object hit routine
******************************************************************************/
void sw_proj_hit(OBJECT *obj)
{
	stop_a8(obj);

	hit_proj_lineup_x(obj,-SCX(0xfa));		// lineup with guy getting hit

	find_ani_part_a14(ANIM_ZAP,3);			// part 3 = bullets hit ani

	mframew(3);

	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void do_proj_jax_zap2(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	jax zap 2 routine
******************************************************************************/
void do_proj_jax_zap2(void)
{
	OBJECT *obj=current_proc->pa8;

	zap_init(ZINIT_SPEC_ACT,ACT_JAX_ZAP2,0,obj);

	jax_zap_jsrp(obj);
	do_next_a9_frame(obj);
	process_sleep(6);

	local_ochar_sound(0);			// jax fire sound

	create_proj_proc(jax_zap_proc);

	current_proc->pdata.p_store4=1;			// flag: projectile #2

	away_x_vel(current_proc,obj,SCX(0x40000));

	do_next_a9_frame(obj);

	process_sleep(0x20);				// even LONGER DELAY

	update_tsl(l_jaxzap2,obj);

	stop_me(obj);

	mframew(7);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void do_proj_sky_ice_behind(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	subzero sky ice behind routine
******************************************************************************/
void do_proj_sky_ice_behind(void)
{
	if (is_he_right(current_proc)==SYSTEM_CARRY_SET)
		(long)current_proc->a11=SCX(0x60);
	else (long)current_proc->a11=-SCX(0x60);

	doice5();
}

/******************************************************************************
 Function: void do_proj_sky_ice_front(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	subzero sky ice front routine
******************************************************************************/
void do_proj_sky_ice_front(void)
{
	if (is_he_right(current_proc)==SYSTEM_CARRY_CLR)
		(long)current_proc->a11=SCX(0x60);
	else (long)current_proc->a11=-SCX(0x60);

	doice5();
}

/******************************************************************************
 Function: void do_proj_sky_ice_on(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	subzero sky ice on routine
******************************************************************************/
void do_proj_sky_ice_on(void)
{
	current_proc->a11=0;
	doice5();
}

/******************************************************************************
 Function: void doice5(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	common sky ice routine
******************************************************************************/
void doice5(void)
{
	OBJECT *obj=current_proc->pa8;

	zap_init(ZINIT_SPEC_ACT,ACT_SKY_ICE,0,obj);

	ochar_sound(0);

	animate2_a9(0x0003,ANIM_SZ_SKYZAP);

	mframew(2);			// start: animate ice intro

	create_proj_proc(sky_ice_proc);	// let another proc handle things

	mframew(3);			// finish: animate ice into the sky
	delete_slave();

	backwards_ani2(3,ANIM_SZ_SKYZAP);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void sky_ice_proc(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	sky ice process, up down ice blast
******************************************************************************/
void sky_ice_proc(void)
{
	OBJECT *obj=current_proc->pa8;
	short stop_pos;

	find_part2();

	obj->oxpos.u.intpos=(current_proc->pdata.p_otherguy)->oxpos.u.intpos+(short)current_proc->a11;	// adjust for on behind or front,

	do_next_a9_frame(obj);		// pose: 1st ice fall

	obj->oypos.u.intpos=ceiling_y-SCY(0x1b0);	// way above ceiling

	process_sleep(8);			// delay actual drop

	ochar_sound(1);

	obj->oypos.u.intpos+=SCY(0x100);		// just above ceiling

	obj->oyvel.pos=SCY(0xb0000);			// heading down
	stop_pos=ground_y-SCY(0xc0);

	do
	{
		process_sleep(1);
		if (proj_strike_check(obj,0x12)==SYSTEM_CARRY_SET)
		{
			/* ice_hit */
			hob_ochar_sound(0x0002,0x0003);
			obj->oyvel.pos=SCY(0x40000);		// heading down slow while breaking up

			/* ice_break_up */
			mframew(3);
			delete_proj_and_die(obj);
		}
	}
	while(obj->oypos.u.intpos<stop_pos);

	/* ice hit ground */
	stop_a8(obj);
	obj->oypos.u.intpos=stop_pos;			// exactly on ground

	/* ice_break_up */
	mframew(3);
	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void do_proj_jax_zap1(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	jax zap 1 routine
******************************************************************************/
void do_proj_jax_zap1(void)
{
	OBJECT *obj=current_proc->pa8;

	zap_init(ZINIT_SPEC_ACT,ACT_JAX_ZAP,0,obj);

	jax_zap_jsrp(obj);

	update_tsl(l_jaxzap2,obj);

	do_proj_sitting_duck(0x16);

	reaction_exit(obj);
}

/******************************************************************************
 Function: void zap_init(WORD zindex,WORD actnum,WORD mflip,OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: zindex - routine to run
 			 actnum - action to set (a1)
			 mflip - flip projectile flag (a10)
 			 obj - dude about to launch a projectile (a8)

 Returns: None

 Description:	general zap init routine
******************************************************************************/
void zap_init(WORD zindex,WORD actnum,WORD mflip,OBJECT *obj)
{
	switch (zindex)
	{
		case ZINIT_AIR:
			air_init_special(obj);
			break;
		case ZINIT_SPEC_ACT:
			init_special_act(actnum,obj);
			break;
		case ZINIT_SPEC:
			init_special(obj);
			break;
	}

	/* zinit3 */
	if (mflip==1)
		flip_multi(obj);

	return;
}

/******************************************************************************
 Function: void jax_zap_jsrp(OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - dude about to launch a projectile

 Returns: None

 Description:	jaz general zap routine init
******************************************************************************/
void jax_zap_jsrp(OBJECT *obj)
{
	do_first_a9_frame(ANIM_ZAP);

	process_sleep(6);

	local_ochar_sound(0);				// jax fire sound

	do_next_a9_frame(obj);

	process_sleep(4);

	create_proj_proc(jax_zap_proc);

	current_proc->pdata.p_store4=0;		// flag: projectile 1

	away_x_vel(current_proc,obj,SCX(0x40000));		// recoil

	mframew(4);

	stop_me(obj);

	return;
}

/******************************************************************************
 Function: void do_proj_sitting_duck(WORD sleeptime)

 By: David Schwartz

 Date: Nov 1994

 Parameters: sleeptime - amount of time to sleep

 Returns: None

 Description:	sitting duck after projectile routine
******************************************************************************/
void do_proj_sitting_duck(WORD sleeptime)
{
	current_proc->pdata.p_action=ACT_PROJ_SD;			// set I am a sitting duck
	process_sleep(sleeptime);
}

/******************************************************************************
 Function: PROCESS *create_proj_proc(FUNC routine)

 By: David Schwartz

 Date: Nov 1994

 Parameters: routine - routine for process to execute

 Returns: created process

 Description:	create a projectile process
******************************************************************************/
PROCESS *create_proj_proc(FUNC routine)
{
	PROCESS *ptemp;

	ptemp=CREATE(p1p2_pick(current_proc->pa8,PID_PROJ1,PID_PROJ2),routine);

	ptemp->pdata.p_otherguy=current_proc->pdata.p_otherguy;
	ptemp->pdata.p_otherproc=current_proc->pdata.p_otherproc;
	ptemp->pa8=(OBJECT *)current_proc->a10;

	ptemp->pdata.p_joyport=NULL;			// init for drones
	current_proc->pdata.p_slave=NULL;

	return(ptemp);
}

/******************************************************************************
 Function: void jax_zap_proc(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	jaxes zap process
******************************************************************************/
void jax_zap_proc(void)
{
	OBJECT *obj=current_proc->pa8;

	get_char_ani(ANIM_TABLE1,ANIM_PROJECTILE);

	do_next_a9_frame(obj);

	tell_world_stk(0x13);

	if (strike_check_a0(obj,0x13)==SYSTEM_CARRY_SET)
		jax_zap_hit(obj);

	process_sleep(3);

	current_proc->pdata.p_store5=3;

	set_proj_vel(obj,SCX(0x80000),4);

	current_proc->a6=(ADDRESS)jax_proj_calla;

	projectile_flight_call(obj,0x12);

	jax_zap_hit(obj);
}

/******************************************************************************
 Function: void jax_zap_hit(OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	jax projectile hit
******************************************************************************/
void jax_zap_hit(OBJECT *obj)
{
	if (f_block==0)
		shake_a11(0x0005,0x0005);

	/* jax_zap_blocked */
	hob_ochar_sound(0x0001,0x0002);
	stop_a8(obj);
	match_me_with_him();

	multi_adjust_xy(current_proc->pa8,-SCX(0xb3),SCY(0));

	if (current_proc->pdata.p_store4==1)
	{
		/* 2nd missle --> raise up */
		multi_adjust_xy(current_proc->pa8,-SCX(0x1a),-SCY(0x25));
	}

	/* jzhit4 */
	find_ani_part_a14(ANIM_PROJECTILE,2);

	mframew(4);

	delete_proj_and_die(obj);
}

/******************************************************************************
 Function: void jax_proj_calla(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:	proj call for jax during strike check
******************************************************************************/
void jax_proj_calla(void)
{
	if (--current_proc->pdata.p_store5==0)
	{
		create_fx(FX_JAX_ZAP_PUFF);
		current_proc->pdata.p_store5=3;
	}

	return;
}

/******************************************************************************
 Function: void set_proj_vel(OBJECT *obj,long vel,WORD anirate)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - projectile object to update
			 vel - speed of projectile
			 anirate - animation rate

 Returns: None

 Description:	set a projectiles velocity
******************************************************************************/
void set_proj_vel(OBJECT *obj,long vel,WORD anirate)
{
	if (obj->oflags & M_FLIPH)
		obj->oxvel.pos=-vel;
	else obj->oxvel.pos=vel;

	init_anirate(anirate);

	return;
}

/******************************************************************************
 Function: WORD projectile_flight_call(OBJECT *obj,WORD strkoff);

 By: David Schwartz

 Date: Nov 1994

 Parameters: strkoff - strike offset

 Returns: SYSTEM_CARRY_SET - projectile hit
		  SYSTEM_CARRY_CLR - projectile missed/went off screen

 Description:	fly a projectile until offscreen or hit
******************************************************************************/
WORD projectile_flight_call(OBJECT *obj,WORD strkoff)
{
	WORD results;

	(ADDRESS)current_proc->pdata.p_store1=current_proc->a6;
	tell_world_stk(strkoff);

	/* pflt3 */
	do
	{
		process_sleep(1);

		next_anirate();

//		if (proj_onscreen_test(obj)==SYSTEM_CARRY_CLR)
//			delete_proj_and_die(obj);

		if ((ADDRESS *)current_proc->pdata.p_store1!=NULL)
			((FUNC)current_proc->pdata.p_store1)();

		/* pflt5 */
		if (proj_onscreen_test(obj)==SYSTEM_CARRY_CLR)
			delete_proj_and_die(obj);

		if (((current_proc->pdata.p_otherguy)->ochar==FT_MOTARO || get_his_action(current_proc)==ACT_REFLECT) && 	// motaro reflects
		   (strike_check_a0_test(obj,strkoff)==SYSTEM_CARRY_SET))
		{
			projectile_reflect(obj);
			results=SYSTEM_CARRY_CLR;
		}
		else
		{
			tell_world_stk(strkoff);
			results=proj_strike_check(obj,strkoff);
		}
	}
	while(results==SYSTEM_CARRY_CLR);

	return(results);
}

/******************************************************************************
 Function: void projectile_reflect(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: obj - projectile

 Returns: None

 Description:	reflect the projectile to attack the sender
******************************************************************************/
void projectile_reflect(OBJECT *obj)
{
	benedict_arnold_projectile(obj);

	set_proj_vel(obj,abs(obj->oxvel.pos),current_proc->pdata.p_anirate);		// turn proj around

	return;
}

/******************************************************************************
 Function: void benedict_arnold_projectile(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: obj - projectile

 Returns: None

 Description:	perform the traitorous act, change the projectile target
******************************************************************************/
void benedict_arnold_projectile(OBJECT *obj)
{
	PROCESS *ptemp;
	short ta3,ta2;

	ptemp=current_proc->pdata.p_otherproc;
	current_proc->pdata.p_otherguy=ptemp->pdata.p_otherguy;
	current_proc->pdata.p_otherproc=ptemp->pdata.p_otherproc;

	ta3=get_frontmost_point(obj);
	flip_multi(obj);
	(short)current_proc->a4=get_frontmost_point(obj);
	ta3=-abs((short)current_proc->a4-ta3);		// we ALWAYS want negative here

	(short)current_proc->a4=rightmost_mpart(obj);
	ta2=leftmost_mpart(obj);
	ta2=abs((short)current_proc->a4-ta2);

	ta3+=ta2;						// add in the x size of the proj
	multi_adjust_xy(obj,ta3,0);

	return;
}

WORD is_jade_protected(void) 
{
	OBJECT *pa1=current_proc->pdata.p_otherguy;
	PROCESS *pa0;

	if (pa1->ochar!=FT_JADE)
		q_no;

	pa0=(PROCESS *)&active_head;
	/* ijp2 */
	do
	{
		if (pa0->procid==PID_JADE_FLASH && pa0->pa8==pa1)
			q_yes;

		pa0=pa0->plink;
	}
	while(pa0!=NULL);

	q_no;
}

/******************************************************************************
 Function: short get_frontmost_point(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: obj - projectile obj

 Returns: None

 Description:	get the frontmost point of projectile
******************************************************************************/
short get_frontmost_point(OBJECT *obj)
{
	if (obj->oflags & M_FLIPH)
		return(leftmost_mpart(obj));
	else return(rightmost_mpart(obj));
}

/******************************************************************************
 Function: void delete_proj_and_die(OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - projectile to die

 Returns: None

 Description:	kill process and associated objects for projectile
******************************************************************************/
void delete_proj_and_die(OBJECT *obj)
{
	process_kill(obj->oslink);

	delobjp(obj);

	process_suicide();
}

/******************************************************************************
 Function: void tell_world_stk(WORD pa0)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa0 - strike parm

 Returns: 	None

 Description:	inform which strike is going to happen
******************************************************************************/
void tell_world_stk(WORD pa0)
{
	current_proc->pdata.p_joyport=(WORD *)get_char_stk(current_proc->pa8,pa0);
	return;
}

/******************************************************************************
 Function: void setup_proj_obj(OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: 	current_proc->a11 - animation ptr
			current_proc->a10 - proj object

 Description:	setup a object for a projectile
******************************************************************************/
void setup_proj_obj(OBJECT *obj)
{
	ADDRESS anif;

	anif=(ADDRESS)current_proc->pa9;

	get_char_ani(ANIM_TABLE1,ANIM_PROJECTILE);
	current_proc->a11=(ADDRESS)current_proc->pa9;

	get_proj_obj_m(obj);

	insert_object((OBJECT *)current_proc->a10,&objlst);

	(ADDRESS)current_proc->pa9=anif;

	return;
}

/******************************************************************************
 Function: inline void i_am_a_sitting_duck(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa0 - strike parm

 Returns: 	None

 Description:	inform i am a dead duck
******************************************************************************/
inline void i_am_a_sitting_duck(void)
{
	current_proc->pdata.p_action=ACT_PROJ_SD;
	return;
}

/******************************************************************************
 Function: WORD proj_strike_check(OBJECT *obj,WORD strkoff)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - projectile to die
			 strkoff -  strike offset

 Returns: SYSTEM_CARRY_SET - projectile hit
		  SYSTEM_CARRY_CLR - projectile missed

 Description:	check to see if a projectile hit
******************************************************************************/
WORD proj_strike_check(OBJECT *obj,WORD strkoff)
{
	if (is_he_motaro()==SYSTEM_CARRY_SET || is_jade_protected()==SYSTEM_CARRY_SET)
		return(SYSTEM_CARRY_CLR);
	else return(strike_check_a0(obj,strkoff));
}

/******************************************************************************
 Function: WORD proj_onscreen_test(OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - projectile to die
			 strkoff -  strike offset

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:	check to see if a projectile is on screen
******************************************************************************/
WORD proj_onscreen_test(OBJECT *obj)
{
	if (obj->oflags2 & M_MULTIPART)
	{
		/* multipart object */
		if (obj->oxvel.pos<0)
		{
			/* pot3, check right pt */
			if (rightmost_mpart(obj)<=worldtlx.u.intpos)
				return(SYSTEM_CARRY_CLR);
			else return(SYSTEM_CARRY_SET);
		}
		else
		{
			/* pot5, check left pt */
			if (leftmost_mpart(obj)<=(worldtlx.u.intpos+SCRRGT))
				return(SYSTEM_CARRY_SET);
			else return(SYSTEM_CARRY_CLR);
		}
	}
	else
	{
		/* single part object */
		if (obj->oxvel.pos<0)
		{
			/* check right pt */
			if (worldtlx.u.intpos<=(obj->oxpos.u.intpos+obj->osize.u.xpos))
				return(SYSTEM_CARRY_SET);
			else return(SYSTEM_CARRY_CLR);
		}
		else
		{
			/* check left pt */
			if (obj->oxpos.u.intpos<=(worldtlx.u.intpos+SCRRGT))
				return(SYSTEM_CARRY_SET);
			else return(SYSTEM_CARRY_CLR);
		}
	}
}


#if 0
WORD ochar_proj_oids[][2] =
{
	{PID_PROJ1,PID_PROJ2},
	{PID_PROJ1,PID_PROJ2},
	{PID_PROJ1,PID_PROJ2},
	{PID_PROJ1,PID_PROJ2},
	{PID_PROJ1,PID_PROJ2},

	{PID_PROJ1,PID_PROJ2},
	{PID_PROJ1,PID_PROJ2},
	{PID_PROJ1,PID_PROJ2},
	{PID_PROJ1,PID_PROJ2},
	{PID_PROJ1,PID_PROJ2},

	{PID_PROJ1,PID_PROJ2},
	{PID_PROJ1,PID_PROJ2},
	{PID_PROJ1,PID_PROJ2},
	{PID_PROJ1,PID_PROJ2},
	{PID_PROJ1,PID_PROJ2}
};
#endif

/******************************************************************************
 Function: OBJECT *get_proj_obj_m(OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: obj - object created

 Description:	create an object for the projectile
******************************************************************************/
OBJECT *get_proj_obj_m(OBJECT *obj)
{
	PROCESS *ptemp;
	OBJECT *objslave;

	ptemp=gmo_proc(obj->oheap);				// get process which holds data
	objslave=(OBJECT *)current_proc->a10=current_proc->pa8;		// a10 = object
	(current_proc->pa8)->oslink=ptemp;					// oslink = process
	current_proc->pa8=obj;

	objslave->ochar=obj->ochar;
	current_proc->pdata.p_slave=objslave;

	objslave->ozval=FRONT_Z+1;

	if (obj->oid==OID_P1)
		objslave->oid=PID_PROJ1;		//ochar_proj_oids[obj->ochar][0];
	else objslave->oid=PID_PROJ2;		//ochar_proj_oids[obj->ochar][1];

	match_ani_points(obj,objslave);

	current_proc->a10=(ADDRESS)objslave;

	return(objslave);
}

/******************************************************************************
 Function: void hit_proj_lineup_x(OBJECT *obj,short xadj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: obj - proj to adjust
			 xadj - x adjust value

 Returns: None

 Description:	lineup up projectile with hit obj in x dir
******************************************************************************/
void hit_proj_lineup_x(OBJECT *obj,short xadj)
{
	short y;

	y=obj->oypos.u.intpos;
	hit_proj_lineup_xy(obj,xadj,0);
	obj->oypos.u.intpos=y;
	return;
}

/******************************************************************************
 Function: void hit_proj_lineup_xy(OBJECT *obj,short xadj,short yadj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: obj - proj to adjust
			 xadj - x adjust value
			 yadj - y adjust value

 Returns: None

 Description:	lineup up projectile with hit obj in x&y dir
******************************************************************************/
void hit_proj_lineup_xy(OBJECT *obj,short xadj,short yadj)
{
	match_me_with_him();
	flip_multi(obj);
	multi_adjust_xy(obj,xadj,yadj);
	return;
}

/******************************************************************************
 Function: WORD is_he_motaro(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - other dude is motaro
		  SYSTEM_CARRY_CLR - other dude is not motaro

 Description:	check to see if other dude is motaro
******************************************************************************/
WORD is_he_motaro(void)
{
	if ((current_proc->pdata.p_otherguy)->ochar==FT_MOTARO)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: WORD q_his_react_flag_set(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - other dude react flag set
		  SYSTEM_CARRY_CLR - other dude react flag not set

 Description:	check to see if other dude is reacting
******************************************************************************/
WORD q_his_react_flag_set(void)
{
	if ((current_proc->pdata.p_otherproc)->pdata.p_flags & PM_REACTING)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: WORD local_strike_check_box(WORD stk,short pa0x,pa0y,pa1x,pa1y)

 By: David Schwartz

 Date: May 1995

 Parameters: stk - strike offset
			 pa0 - top left corner
			 pa1 - lower right corner

 Returns: SYSTEM_CARRY_SET - hit
		  SYSTEM_CARRY_CLR - miss

 Description:	check to see if proj hit
******************************************************************************/
WORD local_strike_check_box(WORD stk,short pa0x,short pa0y,short pa1x,short pa1y)
{
	if (is_he_motaro()==SYSTEM_CARRY_SET || is_jade_protected()==SYSTEM_CARRY_SET)
		return(SYSTEM_CARRY_CLR);

	return(strike_check_box(current_proc->pa8,stk,pa0x,pa0y,pa1x,pa1y));
}

