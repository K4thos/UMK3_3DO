/******************************************************************************
 File: mkmark.c

 Date: Jan 1995

 (C) Williams Entertainment

 Mortal Kombat III Hidden Game
******************************************************************************/

/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkos.h"
#include "mkobj.h"
#include "mkfx.h"
#include "mkutil.h"
#include "mkani.h"
#include "mkgame.h"
#include "mkguys.h"
#include "mkmark.h"
#include "mkmain.h"
#include "mkpal.h"
#include "mkamode.h"
#include "mkjoy.h"
#include "mkinit.h"
#include "mksound.h"
#include "valloc.h"
#include "mkbio.h"

/* local prototypes */
void game_over_w(void);
void game_over_win_proc(void);
void game_over_d(void);
void gopr(void);
void game_over_entry(void);
void gameover(void);
void animate_player_ships(void);
void create_player_ships(void);
void create_player_1_ship(void);
void create_player_2_ship(void);
void move_player_ships(void);
void mpss1(short pa0,OBJECT *pa8);
void watch_buttons_or_missiles(void);
OBJECT *wte1(OBJECT *pa0);
void start_enemy_missile(void);
void watch_enemy_missile(void);
void start_player_1_missile(OBJECT **pa2);
void start_player_2_missile(OBJECT **pa2);
void sp1m1(OBJECT *pa0);
short read_player_1_joystick(void);
short read_player_2_joystick(void);
short read_player_1_button(void);
short read_player_2_button(void);
WORD are_we_hitting_player(void);
void kill_player(void);
OBJECT *kilj1(OBJECT *pa8);
void toast(void);
void restart_player_1(void);
void restart_player_2(void);
WORD are_we_shot(void);
WORD are_we_shot_by_p1(void);
WORD are_we_shot_by_p2(void);
WORD dooc(void);
void awsp11(OBJECT *pa9);
WORD do_objects_collide(OBJECT *pa9);
void create_enemy_dispatchers(void);
void move_wave(void);
WORD follow_path_11(PATHPTR *pa10);
WORD follow_path(long pa0,long pa1,PATHPTR *pa10);
WORD fp_9d(void);
void dispatch_grunts(void);
void create_baiters(void);
void baiter_enemy(void);
void be_9(WORD flag);
short move_to_point(short pa0,short pa1,short pa2);
void create_explosionb_a4(short pa4x,short pa4y);
void ce4b(void);
void create_explosions_a4(short pa4x,short pa4y);
void ce4s(void);
long safe32mult(long pa0,long pa1);
void make_stars(void);
OBJECT *makestar(void);

/* local externs */
extern void *hidden_anims[];
extern OIMGTBL ship1;
extern OIMGTBL ship2;
extern OIMGTBL ship3;
extern OIMGTBL ship4;
extern OIMGTBL enemy1;
extern OIMGTBL enemy3;
extern OIMGTBL enemy5;
extern OIMGTBL enemy9;
extern OIMGTBL expld1;
extern OIMGTBL expld2;
extern OIMGTBL expld3;
extern OIMGTBL expld4;
extern OIMGTBL expld5;
extern OIMGTBL expld6;
extern OIMGTBL expld7;
extern OIMGTBL expld8;
extern OIMGTBL star;

WORD missile_count;
/******************************************************************************
 Function: void hidden_game(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:	smoke page
******************************************************************************/
extern char txt_space_game_intro[];
void hidden_game(void)
{
	WORD ta10,ta11;

//*******************************************************************************
//*******************************************************************************
	/* setup via loaded */
#if 0
	gstate=GS_SPACE_GAME;
	amode_init_no_score();

	sound_module_load(SMOD_HIDDEN);

	clr_scrn();
	mk3_gradient(GREEN_GRAD);
	sony_graphics_setup();
	pds_centered_soff(txt_space_game_intro,160,SCY(0x50)-8);

	send_code_a3(0x45);

	DISPLAY_ON;
	fadein_jsrp(0x40*5);
#endif
//*******************************************************************************
//*******************************************************************************

	/* start the game */
	psxcd_play_at(TRK_HIDDEN,cd_vol_tbl[f_music],5*75);

	murder_myoinit_score();
	dlists=dlists_bogus;
	CREATE(PID_PONG_SPEED,make_stars);
	count_left=44;
	irqskye=0;
	p1_button=p2_button=0;
	p1_state=p2_state=0;
	ok_exit=0;
	missile_count=0;

	player_1_missile=player_1_missile2=player_1_missile3=player_1_missile4=NULL;
	player_2_missile=player_2_missile2=player_2_missile3=player_2_missile4=NULL;
	wave_x_position=0;
	wave_y_position=SCY(40);
	shoot_randper=dive_randper=3;
	create_player_ships();
	create_enemy_dispatchers();

	ta10=60*60/2;
	/* hg1x */
	do
	{
		ta11=3;
		/* hg1 */
		do
		{
			move_player_ships();
			watch_buttons_or_missiles();
			process_sleep(1);

			if ( ok_exit )
				goto HGSH9;

		} while (--ta11>0 );
		animate_player_ships();

	} while (--ta10>0 );

	game_over_d();
	process_sleep(0x40*3);

HGSH9:
	murder_myoinit_score();
	dlists=dlists_bogus;

	f_nosound=0;
	return;
}

void game_over_w(void)
{
	CREATE(PID_PONG_SPEED,game_over_win_proc);
	return;
}

void game_over_win_proc(void)
{
	nosounds();
	process_sleep(2);
	tsound(0x6d);
	stack_jump(game_over_entry);
}

void game_over_d(void)
{
	CREATE(PID_PONG_SPEED,gopr);
	return;
}

void gopr(void)
{
	nosounds();
	process_sleep(2);
	tsound(0x82);
	stack_jump(game_over_entry);
}

void game_over_entry(void)
{
	f_nosound=1;
	process_sleep(0x40*3);
	gameover();
	process_suicide();
}

void gameover(void)
{
	ok_exit=1;
	return;
}

void animate_player_ships(void)
{

	if ( (current_proc->pa8=player_1_ship)!=NULL )
	{
		if ( (current_proc->pa8)->oimg==&ship1 )
			animate(SINGLE_ANI,&ship2,(current_proc->pa8)->oflags);
		else animate(SINGLE_ANI,&ship1,(current_proc->pa8)->oflags);
	}

	/* aps_2 */
	if ( (current_proc->pa8=player_2_ship)!=NULL )
	{
		if ( (current_proc->pa8)->oimg==&ship3 )
			animate(SINGLE_ANI,&ship4,(current_proc->pa8)->oflags);
		else animate(SINGLE_ANI,&ship3,(current_proc->pa8)->oflags);
	}

	return;
}

void create_player_ships(void)
{
	player_1_ships=player_2_ships=3;
	create_player_1_ship();
	create_player_2_ship();
	return;
}

void create_player_2_ship(void)
{
	OBJECT *obj;

	gso_dmawnz(obj,(ADDRESS)&ship3,hidden_anims,0);
	alloc_cache(&ship3,&hidden_anims,obj);
	set_xy_coordinates(obj,SCX(0x140),SCY(0xe0));
	player_2_ship=obj;
	insert_object(obj,&objlst);
	return;
}

void create_player_1_ship(void)
{
	OBJECT *obj;

	gso_dmawnz(obj,(ADDRESS)&ship1,hidden_anims,0);
	alloc_cache(&ship1,&hidden_anims,obj);
	set_xy_coordinates(obj,SCX(0x50),SCY(0xe0));
	player_1_ship=obj;
	insert_object(obj,&objlst);
	return;
}

void move_player_ships(void)
{
	short ta0;

	ta0=read_player_1_joystick();
	ta0<<=1;
	if ( player_1_ship!=NULL )
		mpss1(ta0,player_1_ship);

	ta0=read_player_2_joystick();
	ta0<<=1;
	if ( player_2_ship!=NULL )
		mpss1(ta0,player_2_ship);

	return;
}

void mpss1(short pa0,OBJECT *pa8)
{
	short ta14;

	ta14=pa8->oxpos.u.intpos;
	if ( ta14<=5 )
	{
		if ( pa0<0 )
			pa0=0;
	}

	/* rmm1 */
	if ( ta14>=SCX(386)-16 )
	{
		if ( pa0>0 )
			pa0=0;
	}

	pa8->oxvel.u.intpos=pa0;

	return;
}

void watch_buttons_or_missiles(void)
{
	OBJECT **ta2;
	short ta0;

	if ( player_1_missile!=NULL )
		player_1_missile=wte1(player_1_missile);

	/* wbom1 */
	if ( player_1_missile2!=NULL )
		player_1_missile2=wte1(player_1_missile2);

	/* wbom2 */
	if ( player_2_missile!=NULL )
		player_2_missile=wte1(player_2_missile);

	/* wbom3 */
	if ( player_2_missile2!=NULL )
		player_2_missile2=wte1(player_2_missile2);

	/* wbom4 */
	if ( player_1_missile3!=NULL )
		player_1_missile3=wte1(player_1_missile3);

	/* wbom5 */
	if ( player_1_missile4!=NULL )
		player_1_missile4=wte1(player_1_missile4);

	/* wbom6 */
	if ( player_2_missile3!=NULL )
		player_2_missile3=wte1(player_2_missile3);

	/* wbom7 */
	if ( player_2_missile4!=NULL )
		player_2_missile4=wte1(player_2_missile4);

	/* wbom8 */
	ta2=&player_1_missile;
	if ( *ta2!=NULL )
	{
		ta2=&player_1_missile2;
		if ( *ta2!=NULL )
		{
			ta2=&player_1_missile3;
			if ( *ta2!=NULL )
			{
				ta2=&player_1_missile4;
				if ( *ta2!=NULL )
				{
					goto WP2R;
				}
			}
		}
	}

	/* wp1s */
	if ( player_1_ship!=NULL )
	{
		ta0=read_player_1_button();
		if ( ta0==0 )
		{
			player_1_bstate=ta0;
		}
		else
		{
			if ( player_1_bstate==0 )
			{
				start_player_1_missile(ta2);
				player_1_bstate=1;
			}
			else player_1_bstate=ta0;
		}
	}

WP2R:
	ta2=&player_2_missile;
	if ( *ta2!=NULL )
	{
		ta2=&player_2_missile2;
		if ( *ta2!=NULL )
		{
			ta2=&player_2_missile3;
			if ( *ta2!=NULL )
			{
				ta2=&player_2_missile4;
				if ( *ta2!=NULL )
				{
					goto WP3R;
				}
			}
		}
	}

	/* wp2s */
	if ( player_2_ship!=NULL )
	{
		ta0=read_player_2_button();
		if ( ta0==0 )
		{
			player_2_bstate=ta0;
		}
		else
		{
			if ( player_2_bstate==0 )
			{
				start_player_2_missile(ta2);
				player_2_bstate=1;
			}
			else player_2_bstate=ta0;
		}
	}

WP3R:
	return;
}

OBJECT *wte1(OBJECT *pa0)
{
	if ( pa0->oypos.u.intpos<0 )
	{
		missile_count--;
		delobj(pa0);
		return(NULL);
	}
	else
	{
		return(pa0);
	}
}

void start_enemy_missile(void)
{
	OBJECT *obj=current_proc->pa8;

	if ( missile_count<(BOXPRIM_SIZE-7) )
	{
		make_solid_object(0x001c,3,6);
		set_xy_coordinates(current_proc->pa8,obj->oxpos.u.intpos+SCX(4),obj->oypos.u.intpos);
		insert_object(current_proc->pa8,&objlst);
		(current_proc->pa8)->oyvel.u.intpos=2;
		CREATE(PID_PONG_SPEED,watch_enemy_missile);
		current_proc->pa8=obj;
		missile_count++;
	}

	return;
}

void watch_enemy_missile(void)
{
	OBJECT *obj=current_proc->pa8;

	/* wem1 */
	do
	{
		if (obj->oypos.u.intpos>SCY(260))
		{
			/* wem9 */
			missile_count--;
			delobj(obj);
			process_suicide();
		}

		if (are_we_hitting_player())
		{
			kill_player();

			/* wem9 */
			missile_count--;
			delobj(obj);
			process_suicide();
		}

		/* wem2 */
		process_sleep(1);
	}
	while(1);
}

void start_player_1_missile(OBJECT **pa2)
{
	tsound(0x80);
	make_solid_object(BPAL_YELLOW,3,6);
	sp1m1(player_1_ship);
	*pa2=current_proc->pa8;
	return;
}

void start_player_2_missile(OBJECT **pa2)
{
	tsound(0x80);
	make_solid_object(BPAL_YELLOW,3,6);
	sp1m1(player_2_ship);
	*pa2=current_proc->pa8;
	return;
}

void sp1m1(OBJECT *pa0)
{
	set_xy_coordinates(current_proc->pa8,pa0->oxpos.u.intpos+SCX(4),pa0->oypos.u.intpos);
	insert_object(current_proc->pa8,&objlst);
	(current_proc->pa8)->oyvel.u.intpos=-5;
	missile_count++;
	return;
}

short read_player_1_joystick(void)
{
	short ta0=0;

	if (swcurr.u.p1pad & MASK_JLEFT)
		ta0--;

	if (swcurr.u.p1pad & MASK_JRIGHT)
		ta0++;

	return(ta0);
}

short read_player_2_joystick(void)
{
	short ta0=0;

	if (swcurr.u.p2pad & MASK_JLEFT)
		ta0--;

	if (swcurr.u.p2pad & MASK_JRIGHT)
		ta0++;

	return(ta0);
}

short read_player_1_button(void)
{
	short ta0=0;

	if (swcurr.u.p1pad & P1B0)
		ta0++;

	return(ta0);
}

short read_player_2_button(void)
{
	short ta0=0;

	if (swcurr.u.p2pad & P1B0)
		ta0++;

	return(ta0);
}

WORD are_we_hitting_player(void)
{
	WORD ta11=0;

	if (player_1_ship!=NULL)
	{
		if (do_objects_collide(player_1_ship)==SYSTEM_CARRY_SET)
			ta11++;
	}

	/* awn1 */
	if (player_2_ship!=NULL)
	{
		if (do_objects_collide(player_2_ship)==SYSTEM_CARRY_SET)
			ta11+=2;
	}

	current_proc->a0=ta11;

	return(ta11);
}

void kill_player(void)
{
	WORD ta11=current_proc->a0;

	if (ta11 & 1)
	{
		/* kp_p1 */
		tsound(0x81);
		CREATE(PID_FX,toast);

		if (player_1_ship!=NULL)
		{
			player_1_ship=kilj1(player_1_ship);

			if (--player_1_ships>0)
			{
				CREATE(PID_PONG_SPEED,restart_player_1);
			}
		}
	}

	/* kp_p2 */
	if (ta11 & 2)
	{
		tsound(0x81);
		CREATE(PID_FX,toast);

		if (player_2_ship!=NULL)
		{
			player_2_ship=kilj1(player_2_ship);

			if (--player_2_ships>0)
			{
				CREATE(PID_PONG_SPEED,restart_player_2);
			}
		}
	}

	/* kp_p3 */
	if (player_1_ships<=0 || player_2_ships<=0)
	{
		game_over_d();
	}

	return;
}

OBJECT *kilj1(OBJECT *pa8)
{
	create_explosionb_a4(pa8->oxpos.u.intpos,pa8->oypos.u.intpos);
	delobj(pa8);
	return(NULL);
}

void toast(void)
{
	process_sleep(0x30);
	tsound(0x1b);
	process_suicide();
}

void restart_player_1(void)
{
	process_sleep(100);
	create_player_1_ship();
	process_suicide();
}

void restart_player_2(void)
{
	process_sleep(100);
	create_player_2_ship();
	process_suicide();
}

WORD are_we_shot(void)
{
	if (are_we_shot_by_p1()==SYSTEM_CARRY_SET || are_we_shot_by_p2()==SYSTEM_CARRY_SET)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

WORD are_we_shot_by_p1(void)
{
	current_proc->a10=(ADDRESS)&player_1_missile;
	if (dooc()==SYSTEM_CARRY_SET)
		return(SYSTEM_CARRY_SET);

	current_proc->a10=(ADDRESS)&player_1_missile2;
	if (dooc()==SYSTEM_CARRY_SET)
		return(SYSTEM_CARRY_SET);

	current_proc->a10=(ADDRESS)&player_1_missile3;
	if (dooc()==SYSTEM_CARRY_SET)
		return(SYSTEM_CARRY_SET);

	current_proc->a10=(ADDRESS)&player_1_missile4;
	if (dooc()==SYSTEM_CARRY_SET)
		return(SYSTEM_CARRY_SET);

	return(SYSTEM_CARRY_CLR);
}

WORD are_we_shot_by_p2(void)
{
	current_proc->a10=(ADDRESS)&player_2_missile;
	if (dooc()==SYSTEM_CARRY_SET)
		return(SYSTEM_CARRY_SET);

	current_proc->a10=(ADDRESS)&player_2_missile2;
	if (dooc()==SYSTEM_CARRY_SET)
		return(SYSTEM_CARRY_SET);

	current_proc->a10=(ADDRESS)&player_2_missile3;
	if (dooc()==SYSTEM_CARRY_SET)
		return(SYSTEM_CARRY_SET);

	current_proc->a10=(ADDRESS)&player_2_missile4;
	if (dooc()==SYSTEM_CARRY_SET)
		return(SYSTEM_CARRY_SET);

	return(SYSTEM_CARRY_CLR);
}

/* current_proc->a10 - ptr to missile obj addr */
WORD dooc(void)
{
	OBJECT **ta10=(OBJECT **)current_proc->a10;

	if (*ta10!=NULL)
	{
		if (do_objects_collide(*ta10)==SYSTEM_CARRY_SET)
		{
			awsp11(*ta10);
			*ta10=NULL;
			return(SYSTEM_CARRY_SET);
		}
	}

	/* dc9n */
	return(SYSTEM_CARRY_CLR);
}

void awsp11(OBJECT *pa9)
{
	create_explosions_a4((current_proc->pa8)->oxpos.u.intpos,(current_proc->pa8)->oypos.u.intpos);
	delobj(pa9);
	missile_count--;
	return;
}

/* current_proc->pa8 - obj 1, pa9 - obj 2*/
/* SYSTEM_CARRY_SET =hit SYSTEM_CARRY_CLR - miss */
WORD do_objects_collide(OBJECT *pa9)
{
	XYTYPE o1tl,o1br;
	XYTYPE o2tl,o2br;

	o1tl.u.xpos=(current_proc->pa8)->oxpos.u.intpos;	// a0
	o1tl.u.ypos=(current_proc->pa8)->oypos.u.intpos;	// a1
	o1br.u.xpos=o1tl.u.xpos+(current_proc->pa8)->osize.u.xpos-1;	// a2
	o1br.u.ypos=o1tl.u.ypos+(current_proc->pa8)->osize.u.ypos-1;	// a3

	o2tl.u.xpos=(pa9)->oxpos.u.intpos;					// a4
	o2tl.u.ypos=(pa9)->oypos.u.intpos;					// a5
	o2br.u.xpos=o2tl.u.xpos+(pa9)->osize.u.xpos-1;		// a6
	o2br.u.ypos=o2tl.u.ypos+(pa9)->osize.u.ypos-1;		// a7

	if (o2br.u.xpos<o1tl.u.xpos ||
		o2br.u.ypos<o1tl.u.ypos ||
		o1br.u.xpos<o2tl.u.xpos ||
		o1br.u.ypos<o2tl.u.ypos)
		return(SYSTEM_CARRY_CLR);

	return(SYSTEM_CARRY_SET);
}

void create_enemy_dispatchers(void)
{
	create_baiters();
	CREATE(PID_PONG_SPEED,move_wave);
	return;
}

void move_wave(void)
{
	/* mw1 */
	do
	{
		process_sleep(2);
		if (++wave_x_position>SCX(155))
		{
			dive_randper+=2;
			shoot_randper+=5;
			if (shoot_randper>25)
			{
				shoot_randper+=25;
				dive_randper=shoot_randper;
			}

			/* mw2 */
			do
			{
				process_sleep(2);
				wave_x_position--;
			}
			while(wave_x_position>0);
		}
	}
	while(1);
}

WORD follow_path_11(PATHPTR *pa10)
{
	return(follow_path(0x10000,0x10000,pa10));
}

/*
	pa0 - scale factor for x speed
	pa1 - scale factor for y speed
	pa10 - address of path
*/
WORD follow_path(long pa0,long pa1,PATHPTR *pa10)
{
	OBJECT *obj=current_proc->pa8;

	/* fp_e0 */
	do
	{
		if (pa10->p_xvel==SCX(0xeac) && pa10->p_yvel==SCY(0xeac))
		{
			/* fpp9 */
			return(SYSTEM_CARRY_CLR);
		}

		obj->oxvel.pos=safe32mult(pa10->p_xvel,pa0);
		obj->oyvel.pos=safe32mult(pa10->p_yvel,pa1);
		//obj->oxvel.pos=pa10->p_xvel;
		//obj->oyvel.pos=pa10->p_yvel;
		current_proc->a11=pa10->p_time;

		/* fp_e2 */
		do
		{
			if (obj->oxpos.u.intpos<-SCX(40))
				obj->oxpos.u.intpos=-SCX(40);

			if (obj->oxpos.u.intpos>SCX(440))
				obj->oxpos.u.intpos=SCX(440);

			process_sleep(1);

			if (are_we_shot()==SYSTEM_CARRY_SET)
			{
				return(fp_9d());
			}

			if (are_we_hitting_player()!=0)
			{
				kill_player();
				return(fp_9d());
			}

			/* fp_e3 */
			if (randper(pa10->p_shot+shoot_randper)==SYSTEM_CARRY_SET)
			{
				start_enemy_missile();
				//start_enemy_missile();
			}

			/* fp_e4 */
		}
		while(--current_proc->a11>0);

		pa10++;
	}
	while(1);
}

WORD fp_9d(void)
{
	delobj(current_proc->pa8);
	return(SYSTEM_CARRY_SET);
}

BAITPOS baiter_pos_list[]=
{
	{SCX(0),SCY(20),&enemy3},
	{SCX(15),SCY(20),&enemy3},
	{SCX(30),SCY(20),&enemy3},
	{SCX(45),SCY(20),&enemy3},
	{SCX(60),SCY(20),&enemy3},
	{SCX(75),SCY(20),&enemy3},
	{SCX(90),SCY(20),&enemy3},
	{SCX(105),SCY(20),&enemy3},
	{SCX(120),SCY(20),&enemy3},
	{SCX(135),SCY(20),&enemy3},
	{SCX(150),SCY(20),&enemy3},
	{SCX(165),SCY(20),&enemy3},
	{SCX(180),SCY(20),&enemy3},
	{SCX(195),SCY(20),&enemy3},

	{SCX(0),SCY(40),&enemy9},
	{SCX(15),SCY(40),&enemy9},
	{SCX(30),SCY(40),&enemy9},
	{SCX(45),SCY(40),&enemy9},
	{SCX(60),SCY(40),&enemy9},
	{SCX(75),SCY(40),&enemy9},
	{SCX(90),SCY(40),&enemy9},
	{SCX(105),SCY(40),&enemy9},
	{SCX(120),SCY(40),&enemy9},
	{SCX(135),SCY(40),&enemy9},
	{SCX(150),SCY(40),&enemy9},
	{SCX(165),SCY(40),&enemy9},
	{SCX(180),SCY(40),&enemy9},
	{SCX(195),SCY(40),&enemy9},

	{SCX(0),SCY(60),&enemy5},
	{SCX(15),SCY(60),&enemy5},
	{SCX(30),SCY(60),&enemy5},
	{SCX(45),SCY(60),&enemy5},
	{SCX(60),SCY(60),&enemy5},
	{SCX(75),SCY(60),&enemy5},
	{SCX(90),SCY(60),&enemy5},
	{SCX(105),SCY(60),&enemy5},
	{SCX(120),SCY(60),&enemy5},
	{SCX(135),SCY(60),&enemy5},
	{SCX(150),SCY(60),&enemy5},
	{SCX(165),SCY(60),&enemy5},
	{SCX(180),SCY(60),&enemy5},
	{SCX(195),SCY(60),&enemy5},

	{SCX(50),SCY(0),&enemy1},
	{SCX(150),SCY(0),&enemy1},
	{-1,-1,NULL}
};

void create_baiters(void)
{
	BAITPOS *pa10;
	OBJECT *obj;

	pa10=baiter_pos_list;
	/* cb_1 */
	do
	{
		if (pa10->b_ypos<0)
			return;

		gso_dmawnz_ns(obj,(ADDRESS)pa10->b_frame,hidden_anims,0);
		alloc_cache(pa10->b_frame,&hidden_anims,obj);

		(short)obj->ochar=pa10->b_xpos;
		(short)obj->oid=pa10->b_ypos;
		set_xy_coordinates(obj,pa10->b_xpos+wave_x_position,pa10->b_ypos+wave_y_position);
		insert_object(obj,&objlst);
		CREATE(PID_PONG_SPEED,baiter_enemy);

		pa10++;
	}
	while(1);
}

PATHPTR be_pr[]=
{
	{SCX(0x08000),SCY(0x10000),1,20},
	{SCX(0x10000),SCY(0x10000),1,20},
	{SCX(0x18000),SCY(0x10000),2,20},
	{SCX(0x20000),SCY(0x10000),2,20},
	{SCX(0x28000),SCY(0x10000),4,20},
	{SCX(0x30000),SCY(0x10000),4,20},
	{SCX(0x28000),SCY(0x10000),8,20},
	{SCX(0x20000),SCY(0x10000),8,20},
	{SCX(0x18000),SCY(0x10000),10,20},
	{SCX(0x10000),SCY(0x10000),10,20},
	{SCX(0x08000),SCY(0x10000),12,20},
	{SCX(0x00000),SCY(0x10000),12,20},
	{-SCX(0x08000),SCY(0x10000),15,20},
	{-SCX(0x10000),SCY(0x10000),15,20},
	{-SCX(0x18000),SCY(0x10000),17,20},
	{-SCX(0x20000),SCY(0x10000),17,20},
	{-SCX(0x28000),SCY(0x10000),20,20},
	{-SCX(0x30000),SCY(0x10000),20,20},
	{-SCX(0x28000),SCY(0x10000),21,20},
	{-SCX(0x20000),SCY(0x10000),21,20},
	{-SCX(0x18000),SCY(0x10000),22,20},
	{-SCX(0x10000),SCY(0x10000),22,20},
	{SCX(0x00000),SCY(0x00000),1,20},
	{SCX(0x0eac),SCY(0x0eac),0,0}
};

void baiter_enemy(void)
{
	OBJECT *obj=current_proc->pa8;
	short ta4;

	/* be_e1 */
	do
	{
		obj->oxpos.u.intpos=wave_x_position+(short)obj->ochar;
		obj->oypos.u.intpos=wave_y_position+(short)obj->oid;
		obj->oxvel.u.intpos=wave_x_velocity;
		obj->oyvel.u.intpos=wave_y_velocity;
		if (are_we_shot()==SYSTEM_CARRY_SET)
			be_9d;

		if (are_we_hitting_player()!=0)
		{
			kill_player();
			be_9d;
		}

		/* be_e3 */
		process_sleep(1);
		if (randper(dive_randper)==SYSTEM_CARRY_SET)
		{
			if (follow_path(srandarc(SCX(0x20000)),SCY(0x12000)+srandarc(SCY(0x2000)),be_pr)==SYSTEM_CARRY_SET)
				be_99;
			else obj->oypos.u.intpos=-5;

			/* be_e5 */
			do
			{
				ta4=move_to_point(wave_x_position+(short)obj->ochar,wave_y_position+(short)obj->oid,2);
				if (ta4<2)
					break;

				process_sleep(1);
				if (are_we_shot()==SYSTEM_CARRY_SET)
					be_9d;

				if (are_we_hitting_player()!=0)
				{
					kill_player();
					be_9d;
				}

			}
			while(1);
		}
	}
	while(1);
}

void be_9(WORD flag)
{
	if (flag)
	{
		rsnd_enemy_boom();
		delobj(current_proc->pa8);
	}

	/* be_99 */
	if (--count_left==0)
	{
		game_over_w();
	}

	process_suicide();
}

/* pa0 - x dest pt, pa1 - y dest pt, pa2 - max x & y vel */
/* return - dist from dest */
short move_to_point(short pa0,short pa1,short pa2)
{
	OBJECT *obj=current_proc->pa8;
	short ta14,ta4;

	ta14=-(obj->oxpos.u.intpos-pa0);
	if (ta14>pa2)
		ta14=pa2;

	/* mtp1 */
	pa2=-pa2;
	if (pa2>ta14)
		ta14=pa2;

	/* mtp2 */
	pa2=-pa2;
	obj->oxpos.u.intpos+=ta14;
	ta4=abs(obj->oxpos.u.intpos-pa0);

	ta14=-(obj->oypos.u.intpos-pa1);
	if (ta14>pa2)
		ta14=pa2;

	/* mtp3 */
	pa2=-pa2;
	if (pa2>ta14)
		ta14=pa2;

	/* mtp4 */
	pa2=-pa2;
	obj->oypos.u.intpos+=ta14;
	ta4+=abs(obj->oypos.u.intpos-pa1);

	return(ta4);
}

void create_explosionb_a4(short pa4x,short pa4y)
{
	PROCESS *ptemp;
	XYTYPE temp;

	ptemp=CREATE(PID_PONG_SPEED,ce4b);
	temp.u.xpos=pa4x;
	temp.u.ypos=pa4y;
	(LONG)ptemp->pa9=temp.yx;
	return;
}

void ce4b(void)
{
	OBJECT *obj;
	XYTYPE temp;

	gso_dmawnz(obj,(ADDRESS)&expld1,hidden_anims,0);
	alloc_cache(&expld1,&hidden_anims,obj);
	temp.yx=(LONG)current_proc->pa9;
	set_xy_coordinates(obj,temp.u.xpos+2,temp.u.ypos+5);
	insert_object(obj,&objlst);
	process_sleep(2);

	set_xy_coordinates(obj,temp.u.xpos+2,temp.u.ypos+5);
 	animate(SINGLE_ANI,&expld2,obj->oflags);
	process_sleep(2);

 	animate(SINGLE_ANI,&expld3,obj->oflags);
	process_sleep(2);

 	animate(SINGLE_ANI,&expld4,obj->oflags);
	process_sleep(2);

 	animate(SINGLE_ANI,&expld5,obj->oflags);
	process_sleep(2);

	delobj(obj);
	process_suicide();
}

void create_explosions_a4(short pa4x,short pa4y)
{
	PROCESS *ptemp;
	XYTYPE temp;

	ptemp=CREATE(PID_PONG_SPEED,ce4s);
	temp.u.xpos=pa4x;
	temp.u.ypos=pa4y;
	(LONG)ptemp->pa9=temp.yx;
	return;
}

void ce4s(void)
{
	OBJECT *obj;
	XYTYPE temp;

	gso_dmawnz(obj,(ADDRESS)&expld6,hidden_anims,0);
	alloc_cache(&expld1,&hidden_anims,obj);
	temp.yx=(LONG)current_proc->pa9;
	set_xy_coordinates(obj,temp.u.xpos,temp.u.ypos);
	insert_object(obj,&objlst);
	process_sleep(2);

 	animate(SINGLE_ANI,&expld7,obj->oflags);
	process_sleep(2);

 	animate(SINGLE_ANI,&expld8,obj->oflags);
	process_sleep(2);

	delobj(obj);
	process_suicide();
}

long safe32mult(long pa0,long pa1)
{
	LONG sign;
	LONG a0_i,a0_f;
	LONG a1_i,a1_f;
	long result;

	sign=(pa0 & 0x80000000) ^ (pa1 & 0x80000000);

	pa0=abs(pa0);
	pa1=abs(pa1);

	a0_i=(pa0 & 0xffff0000)>>16;			// int part
	a1_i=(pa1 & 0xffff0000)>>16;

	a0_f=(pa0 & 0xffff);							// frac part
	a1_f=(pa1 & 0xffff);

	result=((a0_i*a1_i)<<16)+((a0_i*a1_f))+((a0_f*a1_i))+((a0_f*a1_f)>>16);
	if ( sign )
		result=-result;

	return(result);
}

void make_stars(void)
{
	OBJECT *obj;
	WORD ta4;

	last_star=makestar();
	last_star->oslink=NULL;

	/* amx2 */
	ta4=100;
	/* amx1 */
	do
	{
		makestar();
		(OBJECT *)(current_proc->pa8)->oslink=last_star;
		last_star=current_proc->pa8;
	} while (--ta4>0);

	/* amx5 */
	do
	{
		process_sleep(1);
		obj=last_star;
		/* amx4 */
		do
		{
			if ( obj->oypos.u.intpos>SCY(260) )
			{
				obj->oypos.u.intpos-=SCY(270);
				obj->oxpos.u.intpos=randu(SCX(400))-1;
			}
		} while ((obj=(OBJECT*)obj->oslink)!=NULL  );
	} while (1);
}

OBJECT *makestar(void)
{
	OBJECT *obj;

	gso_dmawnz(obj,(ADDRESS)&star,hidden_anims,0);
	alloc_cache(&star,&hidden_anims,obj);
	obj->ozval=-100;
	obj->oxpos.u.intpos=randu(SCX(400))-1;
	obj->oypos.u.intpos=randu(SCY(254))-1;
	insert_object(obj,&objlst);
	obj->oyvel.pos=randu(SCY(0x20000))+SCY(0x20000);

	return(obj);
}
