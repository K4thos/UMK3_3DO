/******************************************************************************
 File: mkbuyin.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III buyin Routines
******************************************************************************/

/* INCLUDES */
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkobj.h"
#include "mkos.h"
#include "mkinit.h"
#include "mkgame.h"
#include "mkutil.h"
#include "mkani.h"
#include "mkjoy.h"
#include "mkmain.h"
#include "mkpal.h"
#include "mktext.h"
#include "mkbuyin.h"
#include "mksound.h"
#include "mkdiag.h"
#include "psxcd.h"
/* local extern info */
extern FNTSETUP pf_player1[];
extern FNTSETUP pf_player2[];
extern FNTSETUP pf_pmessage[];
extern FNTSETUP pf_credit_info1[];
extern FNTSETUP pf_credit_info2[];
extern void *fl_num[];
extern char txt_start_to_continue[];
extern char txt_press_join[];
extern char txt_insert_start[];
extern char txt_prepare[];

extern void *buyin_module[];
extern WORD WHITE_p[];
/******************************************************************************
 Function: void do_buyin(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters: None

 Returns: None

 Description:	main game loop goes here to run the buyin count
				down while there is only one player on the game.
******************************************************************************/
char txt_freeplay[]="FREEPLAY";
void do_buyin(void)
{
	PROCESS *ptemp;
	OBJECT *prev_obj,*obj;
	char *cptr;
	int loop,blaze,count;

	if (p1_state!=PS_BUYIN && p2_state!=PS_BUYIN)			// anybody need buyin?
		return;

	gstate=GS_BUYIN;				// game state = buyin

	if (p1_state==PS_BUYIN)
		p1_wiar=0;
	else p2_wiar=0;					// zero wins in a row

	MURDER;							// stop all other procs!

	/* load sound stuff */
	sound_module_load(SMOD_BUYIN);

	process_sleep(4);				// allow flash_msg to settle in


	/* freeze the screen */
	freeze_2_pages();

	background_clear();

	p1_obj=p2_obj=NULL;
	f_shadows=0;

	DONT_SHOW_SCORES;

	load_bkgd=BKGD_MK3_BUYIN;
	init_background_module(buyin_module);
	multi_plane();

	/* credit status */
	if (p1_state!=PS_BUYIN)
	{
		/* print kredit info in player 1 side */
		lm_setup(pf_pmessage);

		if ( f_freeplay )
				p15_centered(txt_freeplay,84,184);
		else lm_printf_p1(pf_credit_info1,credits);

	}
	else
	{
		/* print kredit info in player 2 side */
		lm_setup(pf_pmessage);

		if ( f_freeplay )
				p15_centered(txt_freeplay,244,184);
		else lm_printf_p1(pf_credit_info2,credits);
	}

	/* player 1 text */
	if (p1_state==PS_BUYIN)			// p1 in buyin --> he gets text
	{
		lm_printf(pf_player1);
		cptr=get_buyin_msg(pf_pmessage,p1_state,0);
		p15_centered(cptr,84,163+8-3);
		move_menu_text_down();
	}

	/* player 2 text */
	if (p2_state==PS_BUYIN)			// p2 in buyin -- he gets text
	{
		lm_printf(pf_player2);
		cptr=get_buyin_msg(pf_pmessage,p2_state,1);
		p15_centered(cptr,244,163+8-3);

		move_menu_text_down();
	}

	/* buy2 */
	ptemp=CREATE(PID_FX,boonpal_stuff);
	(void *)ptemp->a11=fl_num;

	/* buy3 */

	/* NOTE: since are list are sorted in reverse order as the arcade
	   this routine has been modified.  The 100 zval are at the start
	   of the list instead of the end.  So we need to link the lists in
	   a different matter, to get the same results.
	*/
	obj=baklst1;
	do
	{
		prev_obj=obj;
		obj=obj->olink;
	}
	while(obj->ozval==100);		// zpos = 100 --> "thickness peice",

	baklst3=baklst1;			// list 3 = thickness pieces

	prev_obj->olink=NULL;		// end the NON-100 list
	baklst1=obj;


	/* buy4 */
	obj=baklst2;
	do
	{
		prev_obj=obj;
		obj=obj->olink;
	}
	while(obj->ozval==100);		// zpos = 100 --> "thickness peice",

	baklst4=baklst2;			// list 4 = thickness pieces

	prev_obj->olink=NULL;		// end the NON-100 list
	baklst2=obj;

	/* move each door piece offscreen */
	adjust_menu_up(&baklst1);
	adjust_menu_up(&baklst3);
	adjust_menu_down(&baklst2);
	adjust_menu_down(&baklst4);

	l_hp[0]=SLIDE_TIME+3;				// set ticks till we stop menu

	call_every_tick=menu_monitor;

	f_novel=0;						// allow menu movement

	DISPLAY_ON;

	tsound(3);				// doors closing sound

	/* buyin2 */
	do
	{
		process_sleep(1);
	}
	while(f_novel==0);				// still sliding menu in ?

	tsound(4);					// door slam sound

	f_auto_erase=1;						// we want autoerase

	delolist(&baklst3);
	delolist(&baklst4);

	shake_a11(0x0003,0x0005);			// shake a bit

	process_sleep(0x20);

	play_generic_tune(TRK_BUYIN);				 // send_code_a3(0x38);			// buyin tune

//****************************************************
	count=0;
	while (psxcd_seeking_for_play() && ++count<5*55 )
		process_sleep(1);			// DHS, try to fix lockup bug???????
//****************************************************

	current_proc->pdata.p_anicount=3;			// # of times u r allowed to reset buyin timer

COINS_WERE_ADDED:
	current_proc->pdata.p_anirate=credits;	// latest # of credits

	f_start_pressed=0;								// let player reset with start!!

	loop=9;
	/* tick tick tick */

	/* bcount3 */
	do
	{
		shape_buyin_num(loop,(OBJECT *)(current_proc->pdata.p_store1));
		blaze=0x60/2;					// tick timer speed

		/* bcount4 */
		do
		{
			process_sleep(2);

			if (credits!=current_proc->pdata.p_anirate)	// same # creds as last time we checked
				goto COINS_WERE_ADDED;							// no, update the message

			if (f_start_pressed!=0)
			{
				f_start_pressed=0;				// reset start pressed
				if (current_proc->pdata.p_anicount!=0)
				{
					(current_proc->pdata.p_anicount)--;
					goto COINS_WERE_ADDED;						// reset clock
				}
			}

			/* bcount5 */
			if (((swcurr.u.p1pad & (P1B0|P1B1|P1B2|P1B3|P1B4|P1B5)) && blaze>5) ||
			   ((swcurr.u.p2pad & (P2B0|P2B1|P2B2|P2B3|P2B4|P2B5)) && blaze>5))
				blaze=5;

			/* bcount6 */
		}
		while(--blaze>0);
	}
	while (--loop>=0);

	eliminate_buyins();

	/* buyin retp */
	return;
}

void move_menu_text_down(void)
{
	move_object_list(&objlst2,&objlst);
	adjust_menu_down(&objlst);
	return;
}
/******************************************************************************
 Function: void amenu2(WORD aindex,OBJECT **olst)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	adjust menu display stuff up or down
******************************************************************************/
void amenu2(WORD aindex,OBJECT **olst)
{
	OBJECT *obj;
	long vel,pos;

	switch (aindex)
	{
		case ADJ_DOWN:		// adjust_menu_down
			vel=-SLIDE_VEL*0x10000;
			pos=(SLIDE_TIME*SLIDE_VEL);
			break;

		case ADJ_UP:		// adjust_menu_up
			vel=SLIDE_VEL*0x10000;
			pos=-(SLIDE_TIME*SLIDE_VEL);
			break;
	}

	obj=(OBJECT *)olst;

	/* amenu2 */
	while((obj=obj->olink)!=NULL)
	{
		obj->oflags2&=(~M_NOSCROLL);
		obj->oypos.u.intpos+=pos;			// adjust 1 piece at a time
		obj->oyvel.pos=vel;				// give piece a velocity
	}

	return;
}

/******************************************************************************
 Function: void menu_monitor(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	wait until menu timer up, then stop velocity
******************************************************************************/
void menu_monitor(void)
{
	if (--l_hp[0]==0)
		f_novel=1;								// flag: stop menu movement

	return;
}

/******************************************************************************
 Function: void eliminate_buyins(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:	eliminate any players who are currently in buyin state
******************************************************************************/
void eliminate_buyins(void)
{
	if (mode_of_play==1)
	{
		/* eliminate_2_on_2 */
		p1_state=PS_ACTIVE;
		p2_state=PS_ACTIVE;
	}
	else
	{
		elim2(&p1_state,&p1_wiar);
		elim2(&p2_state,&p2_wiar);
	}
	return;
}

/******************************************************************************
 Function: void elim2(WORD *pstate,WORD *pwiar)

 By: David Schwartz

 Date: Nov 1994

 Parameters: pstate - ptr to player state
			 pwair - ptr to player wins in a row

 Returns: None

 Description:	common routine for eliminate_buyins
******************************************************************************/
inline void elim2(WORD *pstate,WORD *pwiar)
{
	if (*pstate==PS_BUYIN)
	{
		*pstate=0;				// yes were history
		*pwiar=0;				// zero wins in a row
	}

	return;
}

/******************************************************************************
 Function: char *get_buyin_msg(FNTSETUP *fptr,WORD pstate,WORD player)

 By: David Schwartz

 Date: Jan 1995

 Parameters: fptr - font setup info
 			 pstate - current state
	player - wich player 0=p1 1=p2

 Returns: ptr to message to print

 Description:	get correct message to display
******************************************************************************/
char *get_buyin_msg(FNTSETUP *fptr,WORD pstate,WORD player)
{
	char *cptr;

	lm_setup(fptr);

	switch (pstate)
	{
		case 0:					// gb_join_in
			cptr=(credits!=0 || f_freeplay) ? txt_press_join:txt_insert_start;
			break;
		case 1:					// gb_active
			cptr=txt_prepare;
			break;
		case 2:					// gb_buyin
#if 0
			/* print credits */
			if ( player==0 )
			{
				if ( f_freeplay )
						p7_centered(txt_freeplay,84,200);
				else lm_printf_p1(pf_credit_info1,credits);
			}
			else
			{
				if ( f_freeplay )
						p7_centered(txt_freeplay,244,200);
				else lm_printf_p1(pf_credit_info2,credits);
			}
#endif

			if (credits!=0 || f_freeplay)
			{
				cptr=txt_start_to_continue;
				countdown_objects();
			}
			else
			{
				cptr=txt_insert_start;
				countdown_objects();
			}
			break;

		default:					// gb_messup
			cptr=txt_insert_start;
			break;
	}

	return(cptr);
}

/******************************************************************************
 Function: void countdown_objects(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:	display countdown to zero
******************************************************************************/
void countdown_objects(void)
{
	OBJECT *obj;
	short x,y;

	if ((obj=get_object())==NULL)
		ermac(5);

	obj->osize.yx=0x000a000a;
	obj->opal=get_fore_pal(WHITE_p);

	obj->ozval=2;
	buyin_pick(&x,&y);
	set_xy_coordinates(obj,x,y);
	insert_object(obj,&objlst2);

#if 0
	current_proc->pdata.p_store2=(ADDRESS)obj;
	if ((obj=get_object())==NULL)
		ermac(5);
	obj->osize.yx=0x000a000a;
	obj->opal=get_fore_pal(bpal_black_P);
	obj->ozval=0;
	buyin_pick(&x,&y);
	set_xy_coordinates(obj,x+0x0002,y+0x0002);
	insert_object(obj,&objlst2);
	current_proc->pdata.p_store1=(ADDRESS)obj;
#endif

	shape_buyin_num(9,obj);
	current_proc->pdata.p_store1=(ADDRESS)obj;

	return;
}

/******************************************************************************
 Function: void buyin_pick(short *x,short *y)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: x - xpos
		  y - ypos

 Description:	get correct countdown num pos
******************************************************************************/
void buyin_pick(short *x,short *y)
{
	if (p1_state==PS_BUYIN)
	{
		*x=84-7;
		*y=190+32-10;
	}
	else
	{
		*x=237;
		*y=190+32-10;
	}

	return;
}

/******************************************************************************
 Function: void shape_buyin_num(WORD num,OBJECT *obj_num)

 By: David Schwartz

 Date: Jan 1995

 Parameters: num - number to display
			 obj_num - object of number

 Returns: None

 Description:	display current countdown value and shadow
******************************************************************************/
void shape_buyin_num(WORD num,OBJECT *obj_num)
{
	FNTTPAGE *fptr;

	fptr=font15_num+num;

	/* load correct number */
	obj_num->header.tpage=font_lists[FONT_15].f_tpage;
	obj_num->header.t_xoffset=fptr->u0;
	obj_num->header.t_yoffset=fptr->v0;
	obj_num->header.t_width=fptr->w;
	obj_num->header.t_height=fptr->h;

	return;
}
