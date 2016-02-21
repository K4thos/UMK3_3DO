/******************************************************************************
 File: mkhstd.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III High score Routines
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
#include "mkjoy.h"
#include "mkpal.h"
#include "mkguys.h"
#include "mktext.h"
#include "mkprop.h"
#include "mksound.h"
#include "valloc.h"
#include "mkbio.h"
#include "mkhstd.h"
#include "mkinit.h"
#include "mkmain.h"

extern HSCORE factory_initials[];

/******************************************************************************
 Function: void enter_initials(void)

 By: David Schwartz

 Date: May 1995

 Parameters: current_proc->pdata.p_flags (player offset 0-1)

 Returns: None

 Description:	high score to data inital entry
******************************************************************************/
extern char txt_p1_enter[];
extern char txt_p2_enter[];
extern char txt_enter_your[];
void enter_initials(void)
{
	short ta2x,ta2y;
	char ta0;
	PROCESS *ptemp;

	gstate=GS_INITIALS;

	/* load sound stuff */
	sound_module_load(SMOD_BUYIN);

	/* enter initals background */
	DONT_SHOW_SCORES;
	f_auto_erase=1;
	clr_scrn();
	murder_myoinit_score();
	process_sleep(2);
	dlists=dlists_bogus;

	DISPLAY_ON;

	process_sleep(3);
	mk3_gradient(RED_GRAD);
	play_generic_tune(TRK_YOUWIN);			// SCA3(0x45)

	if (current_proc->pdata.p_flags==0)
		pds_centered(txt_p1_enter,SCX(0xc8),SCY(0x10)+10);
	else pds_centered(txt_p2_enter,SCX(0xc8),SCY(0x10)+10);

	pds_centered(txt_enter_your,SCX(0xc8),SCY(0xd9)+16);

	/* alphabet */
	ta0='A';
	ta2x=ABC_STARTX;
	ta2y=ABC_STARTY;

	current_proc->a4=9;
	/* entin3 */
	do
	{
		print_1_character(ta0,ta2x,ta2y);
		if (--current_proc->a4==0)
		{
			current_proc->a4=9;
			ta2y+=ABC_SPACEY;
			ta2x-=ABC_SPACEX*9;
		}

		/* entin5 */
		ta2x+=ABC_SPACEX;
		ta0++;
	}
	while(ta0!=('Z'+1));

	ta0='-';					// print "rub" char
	print_1_character(ta0,ta2x,ta2y);

	process_sleep(3);

	move_object_list(&objlst2,&objlst);			// move all perm text to objlst1

	ptemp=CREATE(PID_INITIALS,get_initial_proc);
	ptemp->pdata.p_flags=current_proc->pdata.p_flags;
	current_proc->a10=(ADDRESS)ptemp;

	/* time down */
	current_proc->a11=0x40*10;
	/* entin8 */
	do
	{
		process_sleep(2);
		if (ptemp->p_comp_flag==PC_INIT_DONE)
		{
			/* entin9 */
			current_proc->a0=ptemp->pdata.p_store4;
			current_proc->a1=ptemp->pdata.p_store5;
			current_proc->a2=ptemp->pdata.p_store6;
			break;
		}
	}
	while(--current_proc->a11>0);

	if (current_proc->a11==0)
	{
		process_kill(ptemp);

		current_proc->a0=2;
		current_proc->a1=17;
		current_proc->a2=11;				// hmm crl
	}

	/* entina */
	current_proc->a3=(ADDRESS)current_proc->pdata.p_anitab;		// wiar
	insert_entry();

	if (current_proc->pdata.p_flags==0)
		p1_wiar=0;
	else p2_wiar=0;

	noflip=0;
	clr_scrn();
	murder_myoinit_score();
	return;
}

void print_1_character(char ta0,short ta2x,short ta2y)
{
	char txt[2];

	txt[0]=ta0;
	txt[1]=0;
	p15_centered_soff(txt,ta2x,ta2y);
	return;
}


RECT box;
void get_initial_proc(void)
{
	WORD ta2x,ta2y;
	OBJECT *letlist[2];

	current_proc->pdata.p_store8=(current_proc->pdata.p_flags==0)? P1_BMASK:P2_BMASK;
	current_proc->pdata.p_joyport=(current_proc->pdata.p_flags==0)? &swcurr.u.p1pad:&swcurr.u.p2pad;

	/* get initials */
	current_proc->a11=0;
	letlist[0]=letlist[1]=NULL;
	current_proc->pa8=NULL;						// box object
	setRECT(&box,ABC_STARTX-12,ABC_STARTY-6,0x14,0x15);

	ta2x=SCX(0xa0);
	ta2y=SCY(0xa8)+6;
geti1:
	if ( letlist[0]!=NULL )
	{
		delobjp(letlist[0]);		// delete old letter
		letlist[0]=NULL;
		process_sleep(1);
	}
	letlist[0]=get_one_initial(&ta2x,&ta2y,0);
	if ( check_for_rub()==SYSTEM_CARRY_SET )
		goto geti1;

	current_proc->pdata.p_store4=current_proc->a11;
	ta2x+=SCX(0x13);
geti2:
	if ( letlist[1]!=NULL )
	{
		delobjp(letlist[1]);			// delete old letter
		letlist[1]=NULL;
		process_sleep(1);
	}

	letlist[1]=get_one_initial(&ta2x,&ta2y,1);
	if ( check_for_rub()==SYSTEM_CARRY_SET )
		goto geti1;

	current_proc->pdata.p_store5=current_proc->a11;
	ta2x+=SCX(0x13);
	get_one_initial(&ta2x,&ta2y,2);
	if ( check_for_rub()==SYSTEM_CARRY_SET )
		goto geti2;

	current_proc->pdata.p_store6=current_proc->a11;

	/* geti3 */
	while ( 1 )
	{
		current_proc->p_comp_flag=PC_INIT_DONE;
		process_sleep(0x40);
	}
}

/* pa2x,y - pos to print select letter, letnum - which letter we are getting */
/* returns - new created letter object */
OBJECT *get_one_initial(WORD *pa2x,WORD *pa2y,WORD letnum)
{
	OBJECT *obj;

	box_entry();						// move cursor

	/* check for rub */
	if ( current_proc->a11==26 )
	{
		if ( letnum!=0 )
			*pa2x-=SCX(0x13);				// move back position

		button_wait();
		return(NULL);
	}
	else
	{
		/* print character */
		obj=current_proc->pa8;
		print_1_character(current_proc->a11+'A',*pa2x,*pa2y);
		current_proc->pa8=obj;
		obj=objlst2;					// get newly create character
		move_object_list(&objlst2,&objlst);			// move new char to front list
		button_wait();
		return(obj);
	}
}

/* wait until button has been released */
void button_wait(void)
{
	/* butw2 */
	do
	{
		process_sleep(1);
	} while ((*current_proc->pdata.p_joyport) & current_proc->pdata.p_store8);

	erasebox();

	return;
}

/* determine if rub character has been selected */
WORD check_for_rub(void)
{
	if ( current_proc->a11==26 )
	{
		tsound(0x1c);
		return(SYSTEM_CARRY_SET);
	}
	else return(SYSTEM_CARRY_CLR);
}

/* current_proc->a11 - init offset value */
/* box -> box size and position */
void box_entry(void)
{
	WORD ta8,ta10,ta3;
	XYTYPE boxsize;

	boxsize.u.xpos=SCX(0x1a);					// set size of box
	boxsize.u.ypos=SCY(0x1c)-2;

	/* boxm1 */
	ta8=ta10=0;					// debounce counters

	/* boxm2 */
	boxme(&box);							// draw a box and set current_proc->pa8 to start of box pieces

	/* boxm3 */
	do
	{
		process_sleep(1);
		if ( ta10>0 )
			ta10--;

		/* boxm4 */
		if ( ta8>0 )
			ta8--;

		/* boxm5 */
		if ( ((*current_proc->pdata.p_joyport) & P_JMASK))
		{
			/* joystick switch made */
			if ( ta8==0 )
			{
				ta8=8;

				if ( (*current_proc->pdata.p_joyport) & MASK_JUP )
				{
					/* move up */
					ta3=current_proc->a11;
					if ( (ta3/ABC_PER_LINE)!=0 ) 		// move is legal
					{
						valid_move();
						box.y-=boxsize.u.ypos;
						current_proc->a11-=ABC_PER_LINE;		// move offset pos
						boxme(&box);
					}
				}

				if ( (*current_proc->pdata.p_joyport) & MASK_JDOWN )
				{
					/* move down */
					ta3=current_proc->a11;
					if ( (ta3/ABC_PER_LINE)<2)
					{
						valid_move();
						box.y+=boxsize.u.ypos;
						current_proc->a11+=ABC_PER_LINE;
						boxme(&box);
					}
				}

				if ( (*current_proc->pdata.p_joyport) & MASK_JLEFT )
				{
					/* move left */
					ta3=current_proc->a11;
					if ( (ta3 % ABC_PER_LINE)!=0)
					{
						valid_move();
						box.x-=boxsize.u.xpos;
						current_proc->a11--;
						boxme(&box);
					}
				}

				if ( (*current_proc->pdata.p_joyport) & MASK_JRIGHT)
				{
					/* move right */
					ta3=current_proc->a11;
					if ( (ta3 % ABC_PER_LINE)!=(ABC_PER_LINE-1) )
					{
						valid_move();
						box.x+=boxsize.u.xpos;
						current_proc->a11++;
						boxme(&box);
					}
				}
			}
		}
		else
		{
			/* check for button pressed for exit */
			if ( ((*current_proc->pdata.p_joyport) & current_proc->pdata.p_store8) && ta10==0 )
			{
				ta10=120;
				valid_move();
				boxme(&box);
				return;
			}
		}
	} while (1);
}

void boxme(RECT *box)
{
	OBJECT *obj;
	OBJECT *objbase;

	/* draw top portion of box */
	current_proc->pa8=objbase=get_object();
	objbase->oid=OID_CURSOR1;
	objbase->oypos.u.intpos=box->y;
	objbase->oxpos.u.intpos=box->x;
	objbase->osize.u.ypos=3;
	objbase->osize.u.xpos=box->w;
	objbase->oflags|=M_BOX_POLY;
	objbase->header.t_xoffset=0xff;			// set rgb color
	objbase->header.t_yoffset=0x80;
	objbase->header.t_width=0;
	insert_object(objbase,&objlst);

	/* draw bottom */
	((OBJECT*)objbase->oslink)=obj=get_object();
	obj->oid=OID_CURSOR1;
	obj->oypos.u.intpos=box->y+box->h;
	obj->oxpos.u.intpos=box->x;
	obj->osize.u.ypos=3;
	obj->osize.u.xpos=box->w;
	obj->oflags|=M_BOX_POLY;
	obj->header.t_xoffset=0xff;			// set rgb color
	obj->header.t_yoffset=0x80;
	obj->header.t_width=0x0;
	insert_object(obj,&objlst);
	objbase=obj;

	/* draw right */
	((OBJECT*)objbase->oslink)=obj=get_object();
	obj->oid=OID_CURSOR1;
	obj->oypos.u.intpos=box->y;
	obj->oxpos.u.intpos=box->x+box->w;
	obj->osize.u.ypos=box->h+3;
	obj->osize.u.xpos=3;
	obj->oflags|=M_BOX_POLY;
	obj->header.t_xoffset=0xff;			// set rgb color
	obj->header.t_yoffset=0x80;
	obj->header.t_width=0x0;
	insert_object(obj,&objlst);
	objbase=obj;

	/* draw left */
	((OBJECT*)objbase->oslink)=obj=get_object();
	obj->oid=OID_CURSOR1;
	obj->oypos.u.intpos=box->y;
	obj->oxpos.u.intpos=box->x;
	obj->osize.u.ypos=box->h;
	obj->osize.u.xpos=3;
	obj->oflags|=M_BOX_POLY;
	obj->header.t_xoffset=0xff;			// set rgb color
	obj->header.t_yoffset=0x80;
	obj->header.t_width=0x0;
	insert_object(obj,&objlst);
	obj->oslink=NULL;							// term list

	return;
}

void valid_move(void)
{
	tsound(0xc);
	erasebox();
	return;
}

void erasebox(void)
{
	OBJECT *obj=current_proc->pa8;

	while ( obj!=NULL )
	{
		delobj(obj);
		obj=(OBJECT*)(obj->oslink);
	}

	return;
}

//**********************************************************************
//**********************************************************************
extern WORD *all_palettes[];
void print_rankings(void)
{
	murder_myoinit_score();
	clr_scrn();

	DISPLAY_OFF;
	high_score_background();
	process_sleep(4);
	high_score_text();

	fadein_jsrp(0x55);

	process_sleep(0x40*10);
	return;

}

void high_score_background(void)
{
	load_bkgd=BKGD_MK3_HSCORE;
	init_background_module(table_o_mods[load_bkgd]);
	multi_plane();
	return;
}

char txt_rank_title[]="LONGEST WINNING STREAKS";
extern FNTSETUP pf_dave_smalll_soff[];
extern FNTSETUP pf_dave_smallr_soff[];
void high_score_text(void)
{
	short i;

	pds_centered_soff(txt_rank_title,160,16);
	process_sleep(2);

	for ( i=0;i<ENTRIES_PER_PAGE;i++ )
		print_line(i);

	return;
}

char txt_rank_num[]="%d.";
char txt_rank1[]="%s";
extern char txt_rank_wiar[];
void print_line(WORD i)
{
	/* print rank num */
	lm_setup(pf_dave_smalll_soff);
	fnt_state.fnt_posy=(i*(4+fnt_spacing.u.ypos))+40;			// start of correct line
	fnt_state.fnt_posx=40;
	printf_p1(txt_rank_num,i+1);				// 1 factor to start at numeral #1

	/* print name */
	lm_setup(pf_dave_smallr_soff);
	fnt_state.fnt_posx=60;
	fnt_state.fnt_posy=(i*(4+fnt_spacing.u.ypos))+40;			// start of correct line
	printf_s1(txt_rank1,factory_initials[i].hinit);

	/* print wins in a row */
	lm_setup(pf_dave_smalll_soff);
	fnt_state.fnt_posx=264;
	fnt_state.fnt_posy=(i*(4+fnt_spacing.u.ypos))+40;			// start of correct line
	printf_p1(txt_rank_wiar,factory_initials[i].wiar);

	return;
}

WORD does_a0_cut_it(WORD pa0)
{
	if ( factory_initials[ENTRIES_PER_PAGE-1].wiar<=pa0 ) 		// if WIAR>=factory then new inits
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/*
	current_proc->a0 - char #1 (offset)
	current_proc->a1 - char #2 (offset)
	current_proc->a2 - char #3 (offset)
	current_proc->a3 - wiar
*/
void insert_entry(void)
{
	short i,j;

	/* find position in list */
	i=0;
	while ( factory_initials[i].wiar>current_proc->a3)
		i++;

	/* move list from pos down to end */
	j=ENTRIES_PER_PAGE-1;
	while ( j!=i )
	{
		factory_initials[j]=factory_initials[j-1];
		j--;
	}

	/* insert to new entry */
	factory_initials[i].hinit[0]=(char)(((BYTE)current_proc->a0)+'A');
	factory_initials[i].hinit[1]=(char)(((BYTE)current_proc->a1)+'A');
	factory_initials[i].hinit[2]=(char)(((BYTE)current_proc->a2)+'A');
	factory_initials[i].wiar=(WORD)current_proc->a3;

	return;
}
