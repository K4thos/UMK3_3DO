/******************************************************************************
 File: mkutil.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Utility and Game Support Functions
******************************************************************************/

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
#include "mkscore.h"
#include "mkguys.h"
#include "mkfile.h"
#include "mkmain.h"
#include "mksound.h"
#include "mkfx.h"
#include "moves.h"
#include "valloc.h"
#include "psxsnd.h"

void clear_pdata(void) 
{
	memset(&(current_proc->pdata),0,sizeof(PDATA));
	return;
}

/******************************************************************************
 Function: void back_to_normal(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters:  None

 Returns: None

 Description:   player characters call this routine to clear any funky states
				or falgs to get normal again

				1) i can now be flipped
				2) dont ignore my block button
				3) rid of slave objects
******************************************************************************/
void back_to_normal(void)
{
	PROCESS *ptemp;

	if (current_proc->pdata.p_hit>=2)
	{
		ptemp=create_fx(FX_COMBO);                              // combo message
		ptemp->a11=current_proc->pdata.p_hit;   // pass # of hits
		ptemp->a10=((((current_proc->pdata.p_damage)<<16) /FULL_STRENGTH)*0x64)>>16;    // pass % of damage this combo did
	}

	/* btn3 */
	current_proc->pdata.p_hit=0;                    // hit count=0
	current_proc->pdata.p_block=0;                  // block count = 0
	current_proc->pdata.p_power=0;                  // power = 0
	current_proc->pdata.p_damage=0;                 // reset combo damage
	current_proc->pdata.p_action=0;                 // no action
	current_proc->pdata.p_stk=0;                    // I am no longer striking

	ochar_bank_bit(current_proc->pa8);
	player_normpal();

	delete_slave();

	current_proc->pa8->oflags2 &=~(M_NOFLIP|M_NOBLOCK|M_NOCOL|M_NOEDGE|M_HALF_DAMAGE|M_QUARTER_DAMAGE);              // clear flip & block flags

	if ((current_proc->pa8->oflags2 & M_INVISO)==0)         // if inviso dont show shadow
		current_proc->pa8->oflags2 |=M_SHADOW;

	current_proc->pa8->oflags &=~M_FLIPV;

	current_proc->pdata.p_flags &=~(PM_SPECIAL|PM_REACTING|PM_SITDUCK|PM_FINISH);   // not doing a special or reacting to an attack

	return;
}

/******************************************************************************
 Function: void advance_him(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: current_proc->a11=animate ptr

 Returns: None

 Description:   advance other dude one frame
******************************************************************************/
void advance_him(void)
{
	PROCESS *ptemp;
	ADDRESS animtemp;

	ptemp=current_proc;                                     // save a13

	animtemp=(ADDRESS)current_proc->pa9;                            // save a9

	current_proc=current_proc->pdata.p_otherproc;           // set a13-->otherproc

	(ADDRESS)current_proc->pa9=ptemp->a11;                          // set otherguys animptr

	do_next_a9_frame((OBJECT *)ptemp->a10);                         // use other guys object

	ptemp->a11=(ADDRESS)current_proc->pa9;                          // save otherguys animptr

	current_proc=ptemp;

	(ADDRESS)current_proc->pa9=animtemp;

	return;
}

/******************************************************************************
 Function: void add_combo_damage(WORD pa14)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:   add combo damage to other player
******************************************************************************/
void add_combo_damage(WORD pa14)
{
	PROCESS *pa0=current_proc->pdata.p_otherproc;

	pa0->pdata.p_damage+=pa14;

	return;
}

/******************************************************************************
 Function: void decp3(PROCESS *pa0)

 By: David Schwartz

 Date: Mar 1995

 Parameters: pa0 - process to update

 Returns: None

 Description:   common dec from phit for combo stuff
******************************************************************************/
void decp3(PROCESS *pa0)
{
	if (pa0->pdata.p_hit!=0)
		pa0->pdata.p_hit--;

	return;
}

/******************************************************************************
 Function: inline void inc_his_p_hit(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:   inc combo hit counter for otherdude
******************************************************************************/
inline void inc_his_p_hit(void)
{
	(current_proc->pdata.p_otherproc)->pdata.p_hit++;

	return;
}

/******************************************************************************
 Function: void sans_repell_3(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:   disable repell for 3 loops
******************************************************************************/
void sans_repell_3(void)
{
	f_norepell=3;
}

/******************************************************************************
 Function: void clr_scrn(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:   clear both screen buffers
******************************************************************************/
void clr_scrn(void)
{
#if SONY_PSX
	DrawSync(0);                                                                    // wait for everything to finish

	cdb  = (cdb==db)? db+1: db;                                     /* swap double buffer ID */
	cdb->curr_prim=cdb->obj_prim;                           /* setup prim ptr */
	cdb->box_ptr=cdb->box_prim;                                     /* setup prim ptr for boxes */
	cdb->ot_ptr=cdb->ot+1;                                          /* set ot ptr */
	ClearOTag(cdb->ot+1, OTSIZE-1);                         /* clear ordering table, leave clear primitive */
	PutDrawEnv(&cdb->draw);                 /* update drawing environment */
	PutDispEnv(&cdb->disp);                 /* update display environment */
	DrawOTag(cdb->ot);                                                              /* draw */
	DrawSync(0);                                                                    // wait for everything to finish

	cdb  = (cdb==db)? db+1: db;                                     /* swap double buffer ID */
	cdb->curr_prim=cdb->obj_prim;                           /* setup prim ptr */
	cdb->box_ptr=cdb->box_prim;                                     /* setup prim ptr for boxes */
	cdb->ot_ptr=cdb->ot+1;                                          /* set ot ptr */
	ClearOTag(cdb->ot+1, OTSIZE-1);                         /* clear ordering table, leave clear primitive */
	PutDrawEnv(&cdb->draw);                 /* update drawing environment */
	PutDispEnv(&cdb->disp);                 /* update display environment */
	DrawOTag(cdb->ot);                                                              /* draw */
	DrawSync(0);                                                                    // wait for everything to finish
#endif

	return;
}

/******************************************************************************
 Function: void boonpal_stuff(void *)

 By: David Schwartz

 Date: Jan 1995

 Parameters: current_proc->a11 - ptr to flash info

 Returns: None

 Description:   flash a color. This routine is slightly mod from the arcade.
				The arcade is able to assign in color within a palette to a text
				while the sony only can assign a palette to text.  So we have modified
				the routine to search for one of the flash paletes (bpal_flash1,2,3)
				Once found it will perform the flash through color #1 of the palette.
				Since these palettes are continuoulsy changing, we do not care what the
				initial values are.

Format: flptr:
	dw      ptr to palette
	dh      sleep time between updates
	dh      color
	dh      color
	dh      ...
	dh      END_STUFF

Format of Palette:
	dw      # of colors in pal
	dh      color, color ... ,color
******************************************************************************/
void boonpal_stuff(void)
{
	void *fl_ptr;
	WORD *pal;
	void *fl_top;
	WORD sleep;
	WORD palid;

	(ADDRESS)fl_ptr=current_proc->a11;

	/* nrgbs0 */
	(LONG)pal=GET_LONG(fl_ptr)++;           // get palette addr
	sleep=GET_WORD(fl_ptr)++;                       // get sleep time

	if ((palid=findpal(pal))==UNKNOWN_PAL)
		ermac(9);

	/* nrgbs1 */
	fl_top=fl_ptr;

	/* nrgbs2 */
	while (TRUE)
	{
		/* nrgbs3 */
		do
		{
			/* write color to palette */
			*(pal+3)=GET_WORD(fl_ptr)++;                            // stick rgb in color #0 (3 half word) and inc to next rgb value
			*(pal+4)=*(pal+5)=*(pal+6)=*(pal+7)=*(pal+8)=*(pal+9)=*(pal+10)=*(pal+11)=*(pal+3);
			*(pal+12)=*(pal+13)=*(pal+14)=*(pal+15)=*(pal+16)=*(pal+17)=*(pal+18)=*(pal+19)=*(pal+3);
			*(pal+20)=*(pal+21)=*(pal+22)=*(pal+23)=*(pal+24)=*(pal+25)=*(pal+26)=*(pal+27)=*(pal+3);

			/* setup xfer */
			palset_xfer((FORE_CLUT_START_X+((palid%CLUT_FACTOR)<<CLUT_SHIFT_SIZE)),(FORE_CLUT_START_Y+(palid/CLUT_FACTOR)),pal);

			process_sleep(sleep);

		}
		while(*((WORD *)fl_ptr)!=END_STUFF);

		fl_ptr=fl_top;                  // start over again
	}
}

/******************************************************************************
 Function: void slave_viso(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:   set current_proc slave inviso flag
******************************************************************************/
void slave_viso(void)
{
	clear_inviso(current_proc->pdata.p_slave);
	return;
}

/******************************************************************************
 Function: void slave_inviso(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:   set current_proc slave inviso flag
******************************************************************************/
void slave_inviso(void)
{
	set_inviso(current_proc->pdata.p_slave);
	return;
}

/******************************************************************************
 Function: long get_rough_hypotenuse(long xval,long yval)

 By: David Schwartz

 Date: Jan 1995

 Parameters: xval - x value
			 yval - y value

 Returns: hypotenuse

 Description:    returns a rouch estimate of a hyoptenuse (a14)
******************************************************************************/
long get_rough_hypotenuse(long xval,long yval)
{
	xval=abs(xval);
	yval=abs(yval);

	if (xval>yval)
		return((yval>>1)+xval);
	else return((xval>>1)+yval);
}

/******************************************************************************
 Function: inline void center_about_x(OBJECT *obj,short xpos)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:   center object about x
******************************************************************************/
inline void center_about_x(OBJECT *obj,short xpos)
{
	obj->oxpos.u.intpos=xpos-(obj->osize.u.xpos>>1);
	return;
}

/******************************************************************************
 Function: PROCESS *exprc_er(WORD pid)

 By: David Schwartz

 Date: Dec 1994

 Parameters: pid - process to find, if it does nto exist then it erros

 Returns: None

 Description:   find a process with the correct id
******************************************************************************/
PROCESS *exprc_er(WORD pid)
{
	PROCESS *ptemp;

	ptemp=process_exist(pid,0xffff);

	if (ptemp==NULL)
		ermac(4);

	return(ptemp);
}

/******************************************************************************
 Function: inline void no_edge_limits(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:   cancel edge limits
******************************************************************************/
inline void no_edge_limits(void)
{
	left_edge=-0x500;
	right_edge=0x500;

	return;
}

/******************************************************************************
 Function: void closest_edge_a5(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: current_proc->a5 - closest edge
		  closest edge

 Description:   get distances from either edge
******************************************************************************/
WORD closest_edge_a5(void)
{
	get_my_dfe();

	if (current_proc->a5>current_proc->a6)
		current_proc->a5=current_proc->a6;

	return((WORD)current_proc->a5);
}

/******************************************************************************
 Function: void get_my_dfe(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: current_proc->a5 - left edge
		  current_proc->a6 - right edge

 Description:   get distances from either edge
******************************************************************************/
void get_my_dfe(void)
{
	/* get_my_dfe */
	current_proc->a5=abs((current_proc->pa8)->oxpos.u.intpos-left_edge);
	current_proc->a6=abs(right_edge+SCRRGT-((current_proc->pa8)->oxpos.u.intpos));
	return;
}

void get_his_dfe(void) 
{
	/* get_my_dfe */
	current_proc->a5=abs((current_proc->pdata.p_otherguy)->oxpos.u.intpos-left_edge);
	current_proc->a6=abs(right_edge+SCRRGT-((current_proc->pdata.p_otherguy)->oxpos.u.intpos));
	return;
}

/******************************************************************************
 Function: long edge_pick_a0(long pa0)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa0 - edge value

 Returns: None

 Description:   set value depending on which edge
******************************************************************************/
long edge_pick_a0(long pa0)
{
	if (current_proc->a6>current_proc->a5)
		return(pa0);
	else return(-pa0);
}

/******************************************************************************
 Function: WORD is_he_blocking(WORD *duckflag,WORD sflags)

 By: David Schwartz

 Date: Oct 1994

 Parameters: duckflag - set if ducking
			 sflags - strike info flags

 Returns: SYSTEM_CARRY_SET - yes (blocking)
		  SYSTEM_CARRY_CLR - no (no blocking)

 Description:   answers the question: is the other dude trying to block this punch?
******************************************************************************/
WORD is_he_blocking(WORD *duckflag,WORD sflags)
{
	*duckflag=0;                                                                    // assume "no blocking"

	if (f_sans_block)
		return(SYSTEM_CARRY_CLR);                       // no blocking allowed

	if (is_he_joy()==SYSTEM_CARRY_CLR)
	{
		/* other dude is a drone */
		if ((current_proc->pdata.p_otherproc)->pdata.p_action==ACT_BLOCKHI)
			return(SYSTEM_CARRY_SET);

		if ((current_proc->pdata.p_otherproc)->pdata.p_action==ACT_BLOCKLO)
		{
			*duckflag=1;                                                                    // he's ducking --> flag it!!
			return(SYSTEM_CARRY_SET);
		}
		else return(SYSTEM_CARRY_CLR);
	}
	else
	{
		/* other dude is a joystick */
		if (is_he_airborn()==SYSTEM_CARRY_SET)                          // cant block while airborne
			return(SYSTEM_CARRY_CLR);

		if (((current_proc->pdata.p_otherguy)->oflags2 & M_NOBLOCK) || check_his_block_bit()==SYSTEM_CARRY_CLR)
			return(SYSTEM_CARRY_CLR);                                               // in "no block situation" or not blocking

		/* block code */
		if (*((current_proc->pdata.p_otherproc)->pdata.p_joyport) & MASK_JDOWN)
		{
			*duckflag=1;
			return(SYSTEM_CARRY_SET);
		}
		else
		{
			/* standing up block */
			if (sflags & 2)
				return(SYSTEM_CARRY_CLR);                                       // requires "duck" block and we are doing a stand block
			else return(SYSTEM_CARRY_SET);
		}
	}
}

/******************************************************************************
 Function: inline WORD is_he_joy(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:   answers the question: is the other dude joystick controlled
******************************************************************************/
inline WORD is_he_joy(void)
{
	if ((current_proc->pdata.p_otherproc)->pdata.p_flags & PM_JOY)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: inline WORD am_i_joy(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:   answers the question: is the other current fighter joystick controlled
******************************************************************************/
inline WORD am_i_joy(void)
{
	if (current_proc->pdata.p_flags & PM_JOY)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: WORD am_i_facing_him(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:   answers the question with a carry, is the current proc facing the other fighter?
******************************************************************************/
WORD am_i_facing_him(void)
{
	WORD mask=0;

	if (is_he_right(current_proc)==SYSTEM_CARRY_CLR)                // assume: no flip needed if other to right
		mask=M_FLIPH;

	if ((current_proc->pa8->oflags & M_FLIPH)==mask)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: inline WORD is_he_airborn(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters: None

 Returns: SYSTEM_CARRY_SET - yes
		  SYSTEM_CARRY_CLR - no

 Description:   answers the question: is the other dude in the air
******************************************************************************/
inline WORD is_he_airborn(void)
{
	OBJECT *odude;

	odude=current_proc->pdata.p_otherguy;

	if ((odude->oyvel.pos !=0) || (odude->oypos.u.intpos != (current_proc->pdata.p_otherproc)->pdata.p_ganiy))
		return(SYSTEM_CARRY_SET);                                                                       // dude is not on ground or moving in y dir
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: void get_screen_coordinates(OBJECT *obj,short *x,short *y)

 By: David Schwartz

 Date: Dec 1994

 Parameters: obj - screen coords of object

 Returns: x - xpos
		  y - ypos o

 Description:   get screen coords of an object
******************************************************************************/
void get_screen_coordinates(OBJECT *obj,short *x,short *y)
{
	*x=obj->oxpos.u.intpos-worldtlx.u.intpos;
	*y=obj->oypos.u.intpos-worldtly.u.intpos;

	return;
}

/******************************************************************************
 Function: void world_a5(OBJECT *obj)

 By: David Schwartz

 Date: Dec 1994

 Parameters: obj - place object in world

 Returns: None

 Description:   place an object in the world
******************************************************************************/
void world_a5(OBJECT *obj)
{
	obj->oxpos.u.intpos+=worldtlx.u.intpos;
	obj->oypos.u.intpos+=worldtly.u.intpos;

	return;
}

/******************************************************************************
 Function: void screen_a5(OBJECT *obj)

 By: David Schwartz

 Date: Dec 1994

 Parameters: obj - place object on the screen

 Returns: None

 Description:   place an object on the screen
******************************************************************************/
void screen_a5(OBJECT *obj)
{
	obj->oxpos.u.intpos-=worldtlx.u.intpos;
	obj->oypos.u.intpos-=worldtly.u.intpos;

	return;
}

/******************************************************************************
 Function: inline int get_my_height(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: highest pt of fighter

 Description:   compute how tall I am
******************************************************************************/
inline int get_my_height(void)
{
	return(ground_y-highest_mpart(current_proc->pa8));
}

/******************************************************************************
 Function: void no_edge_both_players(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:   disable edge flag for both players
******************************************************************************/
void no_edge_both_players(void)
{
	set_noedge(p1_obj);
	set_noedge(p2_obj);
	return;
}

/******************************************************************************
 Function: void wait_for_start(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:   fighters wait here for the fight to start !!!
******************************************************************************/
void wait_for_start(void)
{
	stance_setup();                         // get stance ani/ani speed

	do
	{
		process_sleep(1);
		next_anirate();
	}
	while(!f_start);
}

/******************************************************************************
 Function: void wait_for_landing(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:   wait for him to land
******************************************************************************/
void wait_for_landing(void)
{
	do
	{
		process_sleep(1);

		if (get_his_action(current_proc)==ACT_FROZEN)
			break;
	}
	while(is_he_airborn()==SYSTEM_CARRY_SET);

	/* wfl_abort */
	return;
}

/******************************************************************************
 Function: void wait_for_his_dog(long dog)

 By: David Schwartz

 Date: Nov 1994

 Parameters:  dog - distance off ground to wait before resuming

 Returns: None

 Description:   wait until other player is within dog parameter
******************************************************************************/
void wait_for_his_dog(long dog)
{
	PROCESS *ptemp=current_proc->pdata.p_otherproc;
	OBJECT *otemp=ptemp->pa8;

	do
	{
		process_sleep(1);
	}
	while((ptemp->pdata.p_ganiy-otemp->oypos.u.intpos)>dog);

	return;
}

/******************************************************************************
 Function: void update_tsl(WORD *tsl,OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: tsl - base addr ptr for tick ram to update
			 obj - current object ptr

 Returns: None

 Description:   update fighters last tick for current button op
******************************************************************************/
void update_tsl(WORD *tsl,OBJECT *obj)
{
	if (obj->oid!=PID_P1)                                                           // not p1 so point to p2 ram
		tsl++;

	*tsl=tick;                                                                                      // update this players ram

	return;
}

/******************************************************************************
 Function: inline WORD am_i_short(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: SYSTEM_CARRY_SET - I am short
		  SYSTEM_CARRY_CLR - I am tall

 Description:   determines if I am short or not
******************************************************************************/
inline WORD am_i_short(void)
{
	if (get_my_height()<SCY(0x67))
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: WORD is_he_short(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET - I am short
		  SYSTEM_CARRY_CLR - I am tall

 Description:   determines if I am short or not
******************************************************************************/
WORD is_he_short(void)
{
	PROCESS *ta13;
	OBJECT *ta8;
	WORD results;

	ta13=current_proc;
	ta8=ta13->pa8;
	results=am_i_short();
	current_proc=ta13;
	ta13->pa8=ta8;

	return(results);
}

/******************************************************************************
 Function: WORD am_i_close_to_edge(PROCESS *mproc)

 By: David Schwartz

 Date: Nov 1994

 Parameters:  None

 Returns: SYSTEM_CARRY_SET - to close to edge
		  SYSTEM_CARRY_CLR - far enough from edge

 Description:   check to see if I am close to the edge
******************************************************************************/
WORD am_i_close_to_edge(PROCESS *mproc)
{

	get_my_dfe();

	if (is_he_right(mproc)==SYSTEM_CARRY_CLR)
		current_proc->a5=current_proc->a6;

	(long)current_proc->a0=0x40000;                                 // create faster stumbles

	if (current_proc->a5<CLOSE_TO_EDGE)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: inline void killfx(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters:  None

 Returns: None

 Description:   kill all fx procs and objects1
******************************************************************************/
inline void killfx(void)
{
	dallobj(OID_FX);
	dallprc(PID_FX);
	return;
}

/******************************************************************************
 Function: void face_opponent(OBJECT *obj)

 By: David Schwartz

 Date: Sept 1994

 Parameters:  obj - object

 Returns: None

 Description:   make sure I am pointing at the other dude
******************************************************************************/
void face_opponent(OBJECT *obj)
{
	WORD flag=M_FLIPH;

	if (is_he_right(current_proc)==SYSTEM_CARRY_SET)        // attacker to the right of me ??
		flag=0;                                                         // yes, i should not be flipped

	if (flag != (obj->oflags & M_FLIPH))
		flip_multi(obj);                                        // no, correct me
}

/******************************************************************************
 Function: WORD is_he_right(PROCESS *ptemp)

 By: David Schwartz

 Date: Sept 1994

 Parameters: ptemp -- process to check against

 Returns: SYSTEM_CARRY_SET - other dude to right
		  SYSTEM_CARRY_CLR - other dude to left

 Description:   answers question: is the other dude to the right of me?
******************************************************************************/
inline WORD is_he_right(PROCESS *ptemp)
{

	if ((ptemp->pdata.p_otherguy)->oxpos.pos > (ptemp->pa8->oxpos.pos))
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: WORD is_he_left(PROCESS *ptemp)

 By: David Schwartz

 Date: Sept 1994

 Parameters: ptemp -- process to check against

 Returns: SYSTEM_CARRY_SET - other dude to left
		  SYSTEM_CARRY_CLR - other dude to right

 Description:   answers question: is the other dude to the left of me?
******************************************************************************/
inline WORD is_he_left(PROCESS *ptemp)
{
	if (is_he_right(ptemp)==SYSTEM_CARRY_SET)
		return(SYSTEM_CARRY_CLR);
	else return(SYSTEM_CARRY_SET);
}


/******************************************************************************
 Function: void delete_slave(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters:  None

 Returns: None

 Description:   delete a multipart slave object
******************************************************************************/
void delete_slave(void)
{
	OBJECT *slave;
	if ((slave=current_proc->pdata.p_slave)!=NULL)
	{
		//-DHS- check for correctness
		if (slave->oflags2 & M_MULTIPART)
			process_kill((PROCESS *)slave->oslink);

		delobjp(slave);

		current_proc->pdata.p_slave=NULL;                               // clear slave ram
	}

	return;
}

/******************************************************************************
 Function: murder_myoinit_score(void)

 By: David Schwartz

 Date: Aug 8, 1994

 Parameters: None

 Returns: None

 Description:   Calls murder_myoinit and then set the second palette to the vogel score palette
******************************************************************************/
extern WORD LOADING_P[];
extern WORD loading_pal;
void murder_myoinit_score(void)
{
	murder_myoinit();
	setup_score_ram();

	score_pal=get_fore_pal(SCORE_P);
	loading_pal=get_fore_pal(LOADING_P);

	return;
}

/******************************************************************************
 Function: murder_myoinit(void)

 By: David Schwartz

 Date: Aug 8, 1994

 Parameters: None

 Returns: None

 Description:   Kill every process except "indestrictables"
******************************************************************************/
void murder_myoinit(void)
{
	/* vram cache system flush */
	flush_vram_mem();                                       // flush all non perm vram */

	object_initialize();                            /* setup object ram */

	MURDER;

	return;
}

/******************************************************************************
 Function: void         fastxfer(PROCESS *pxfer,JUMPTBL jumpptr)

 By: David Schwartz

 Date: Oct 1994

 Parameters: pxfer - process to change
			 jumpptr - func to execute

 Returns: None

 Description:   xfer the execution of another process to the "jumpptr".  In doing
				this we need to reset the stack to the start and tell the process
				to wake up right away
******************************************************************************/
void fastxfer(PROCESS *pxfer,JUMPTBL jumpptr)
{
	pxfer->ptime=1;                                                                                         // set sleep time to start process right away
	pxfer->p_context.c_frameptr=pxfer->p_context.c_stackptr=(ADDRESS)&(pxfer->p_context.c_stack[STKSIZE-3]);        //set start of stack for process
	pxfer->p_context.c_wakeup=(ADDRESS)jumpptr;                                     // set wakeup point
	pxfer->p_comp_flag=PC_CLEAR;                                                                            // addition to fix bug of xfer in middle of move
	return;
}

/******************************************************************************
 Function: PROCESS *create_and_kill(WORD pid,FUNC routine)

 By: David Schwartz

 Date: Nov 1994

 Parameters:  pid - process id
			  routine - routine process should execute

 Returns: ptr to process that was created

 Description:   creates a process and kills any old ones with the same id
******************************************************************************/
PROCESS *create_and_kill(WORD pid,FUNC routine)
{
	dallprc(pid);
	return(CREATE(pid,routine));
}

/******************************************************************************
 Function: PROCESS *create_if_gone(WORD pid,FUNC routine)

 By: David Schwartz

 Date: Jan 1995

 Parameters:  pid - process id
			  routine - routine process should execute

 Returns: ptr to process that was created

 Description:   creates a process if it's id does not already exist
******************************************************************************/
PROCESS *create_if_gone(WORD pid,FUNC routine)
{
	PROCESS *ptemp;

	ptemp=process_exist(pid,-1);

	if (ptemp==NULL)
		return(CREATE(pid,routine));
	else return(ptemp);
}

/******************************************************************************
 Function: void kill_ocp(WORD id,WORD mask)

 By: David Schwartz

 Date: Oct 1994

 Parameters:  id - id of object
			  mask - mask bits

 Returns: None

 Description:   traverse the three object lists and kill a class of objects
******************************************************************************/
void kill_ocp(WORD id,WORD mask)
{
	koc2(&objlst,id,mask);                                                  // traverse object list 1
	koc2(&objlst2,id,mask);                                                 // traverse object list 2
	koc2(&objlst3,id,mask);                                                 // traverse object list 3

	return;
}

/******************************************************************************
 Function: void koc2(OBJECT **olist,WORD id,WORD mask)

 By: David Schwartz

 Date: Oct 1994

 Parameters: olist - list to traverese
			 id - id of object
			 mask - mask bits

 Returns: None

 Description:   kill a class of objects and delete then from the pallete count they were on
******************************************************************************/
#if DEBUG
extern WORD objcnt;
#endif

void koc2(OBJECT **olist,WORD id,WORD mask)
{
	OBJECT *obj_ptr;
	OBJECT *obj_prev;

	obj_ptr=(OBJECT *)olist;

	while (obj_ptr->olink != NULL)
	{
		obj_prev=obj_ptr;                                                       /* save previous */

		obj_ptr=obj_ptr->olink;                                         /* point to next object */

		if ((obj_ptr->oid & mask)==id)
		{
			free_pal(obj_ptr->opal);                        // dec palette count

			obj_prev->olink=obj_ptr->olink;                 // unlink kill node from active */

			obj_ptr->olink=obj_free;                                // insert killed node into free area */

			obj_free=obj_ptr;                                               // set free head

			obj_ptr=obj_prev;                                               // go back to prev
#if DEBUG
	objcnt--;
#endif
		}
	}

	return;
}

/******************************************************************************
 Function: void change_oid_list(OBJECT **olist,WORD oldid,WORD newid)

 By: David Schwartz

 Date: Jan 1995

 Parameters: olist - list to traverese
			 oldid - id of object to change
			 newid - new id for object

 Returns: None

 Description:   change all oldid's to newid's in the specified list
******************************************************************************/
void change_oid_list(OBJECT **olist,WORD oldid,WORD newid)
{
	OBJECT *obj_ptr;

	obj_ptr=(OBJECT *)olist;

	while (obj_ptr->olink != NULL)
	{
		obj_ptr=obj_ptr->olink;                                         /* point to next object */

		if (obj_ptr->oid ==oldid)
			obj_ptr->oid=newid;
	}

	return;
}

/******************************************************************************
 Function: void swpal(void *charpal,OBJECT *obj)

 By: David Schwartz

 Date: Sept 1994

 Parameters: charpal - ptr to palette to use
			 obj - object to switch palette

 Returns: None

 Description:   routine to switch the palette of an object
******************************************************************************/
void swpal(void *charpal,OBJECT *obj)
{
	delpal(obj);
	if ((obj->opal=get_fore_pal((WORD *)charpal))==0)
		ermac(8);

	return;
}

/******************************************************************************
 Function: void wait_forever(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: None

 Returns: None

 Description:   have process wait forever
******************************************************************************/
void wait_forever(void)
{
	while(1)
	{
		POLLING;
		current_proc->p_comp_flag=PC_WAITF_WAKE;
		process_sleep(0x40);
		current_proc->p_comp_flag=PC_CLEAR;
	}
}

/******************************************************************************
 Function: void shake2(OBJECT *obj,WORD xpixel,WORD ypixel,WORD shakes,WORD sleep)

 By: David Schwartz

 Date: Nov 1994

 Parameters:  obj - dude to shake
			  xpixel - # of pixels to move in x (a0[0..15])
			  ypixel - # of pixels to move in y (a0[16..31])
			  shakes - # of times to shake (a1)
			  sleep - sleep time between shakes (a2)

 Returns: None

 Description:   shake specified dude up
******************************************************************************/
void shake2(OBJECT *obj,WORD xpixel,WORD ypixel,WORD shakes,WORD sleep)
{
	do
	{
		obj->oxpos.u.intpos+=xpixel;
		obj->oypos.u.intpos+=ypixel;
		process_sleep(sleep);

		obj->oxpos.u.intpos-=xpixel;
		obj->oypos.u.intpos-=ypixel;
		process_sleep(sleep);
	}
	while(--shakes>0);

	return;
}

/******************************************************************************
 Function: void takeover_him_routine(th_index,JUMPTBL xferpt)

 By: David Schwartz

 Date: Nov 1994

 Parameters: xferpt - routine to set other player to

 Returns: None

 Description:   routine to do the chores of taking over someone
******************************************************************************/
void takeover_him_routine(WORD th_index,JUMPTBL xferpt)
{
	PROCESS *ptemp;

	if (th_index==TH_SR)
		sans_repell;

	stop_him(current_proc->pdata.p_otherguy);
	disable_his_buttons;

	/* put on disguise */
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;

	current_proc->pdata.p_action=0;                                                 // clear action

	current_proc->pdata.p_flags|=PM_REACTING;                               // flag: reacting

	player_normpal();

	delete_slave();

	/* end disguise */
	current_proc=ptemp;

	fastxfer(current_proc->pdata.p_otherproc,xferpt);

	return;
}


/******************************************************************************
 Function: WORD get_tsl(WORD *mtime,OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: mtime - ptr to p1/p2 location for last time check ram (a0)
			 obj - dude who wants to know (a8)

 Returns: time elapsed

 Description:   get the amount of time (ticks) that has elapsed
		since the last time we updated this ram.
******************************************************************************/
WORD get_tsl(WORD *mtime,OBJECT *obj)
{
	if (obj->oid==PID_P2)
		mtime++;                                        // point to player 2 ram

	if (*mtime==0)                                  // when last time ram set, 0=never
		return(0x40*30);                        // default setting = 30 seconds
	else return(tick-*mtime);
}

/******************************************************************************
 Function: void damage_to_me(WORD damage,OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: damage - amount of damage (a10)
			 obj - dude to take damage(a8)

 Returns: time elapsed

 Description:   dude call this routine to give away awards to other player
******************************************************************************/
void damage_to_me(WORD damage,OBJECT *obj)
{
	if (obj==p1_obj)
		bar_reducer(PLAYER1_TYPE,&damage);
	else bar_reducer(PLAYER2_TYPE,&damage);

	return;
}

/******************************************************************************
 Function: inline void half_x_vel(OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - dude to half vel (a8)

 Returns: time elapsed

 Description:   halve a dudes velocity, or the current objects
******************************************************************************/
inline void half_x_vel(OBJECT *obj)
{
	if (p1_obj==obj)
		p1_xvel.pos>>=1;
	else
	{
		if (p2_obj==obj)
			p2_xvel.pos>>=1;
		else obj->oxvel.pos>>=1;
	}

	return;
}

/******************************************************************************
 Function: ADDRESS get_his_char_ani(WORD anitab,short animnum)

 By: David Schwartz

 Date: Nov 1994

 Parameters: anitab - which table (ANIM_TABLE1,ANIMTABLE2)
			 animnum - animation number

 Returns: animation ptr

 Description:   set the other dudes animation
******************************************************************************/
ADDRESS get_his_char_ani(WORD anitab,short animnum)
{
	PROCESS *ptemp;
	ADDRESS animptr;
	void *ta9;

	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;           // pretend I am him
	ta9=current_proc->pa9;                                                  // save otherdudes a9 reg
	get_char_ani(anitab,animnum);
	animptr=(ADDRESS)current_proc->pa9;
	current_proc->pa9=ta9;                                                  // restore otherdudes a9 reg
	current_proc=ptemp;

	return(animptr);
}

/******************************************************************************
 Function: void adjust_him_xy(short xadj,short yadj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: xadj - amount to adjust him in x
			 xadj - amount to adjust him in x

 Returns: None

 Description:   set the other dudes animation
******************************************************************************/
void adjust_him_xy(short xadj,short yadj)
{
	multi_adjust_xy(current_proc->pdata.p_otherguy,xadj,yadj);
	return;
}

/******************************************************************************
 Function: void match_him_with_me_f(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:   Match his animation points with my and flip
******************************************************************************/
void match_him_with_me_f(void)
{
	match_ani_points(current_proc->pa8,current_proc->pdata.p_otherguy);
	flip_multi(current_proc->pdata.p_otherguy);
	return;
}

/******************************************************************************
 Function: void match_him_with_me(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:   Match his animation points with mine
******************************************************************************/
void match_him_with_me(void)
{
	match_ani_points(current_proc->pa8,current_proc->pdata.p_otherguy);
	return;
}

/******************************************************************************
 Function: void match_me_with_him(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:   Match my animation points with his
******************************************************************************/
void match_me_with_him(void)
{
	match_ani_points(current_proc->pdata.p_otherguy,current_proc->pa8);
	return;
}

/******************************************************************************
 Function: PROCESS *gmo_proc(void *heapptr)

 By: David Schwartz

 Date: Nov 1994

 Parameters: current_proc->pa9 - frame with all of the info
			 heapptr - ptr to start of object data

 Returns: current_proc->a0 - dummy process created (also returns value)

 Description -  Get multipart object using a process for the multipart ram
******************************************************************************/
PROCESS *gmo_proc(void *heapptr)
{
	LONG animframe;
#if !MULTI_OFF
	PROCESS *ptemp;

	ptemp=CREATE(PID_MPO,multi_dummy_proc);                         // grab a process for ram

	ptemp->ptime=0x1000;                                                            // don't allow wakeup
#endif

	animframe=GET_LONG(current_proc->pa9);
	animframe=(LONG)COMPUTE_ADDR(heapptr,animframe);

#if MULTI_OFF
	get_multi_obj((ADDRESS *)animframe,0,heapptr,PFLAG_PALLOAD);
	return(NULL);
#else
	get_multi_obj((ADDRESS *)animframe,&(ptemp->pdata),heapptr,PFLAG_PALLOAD);
	return(ptemp);
#endif
}

/******************************************************************************
 Function: void multi_dummy_proc(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description -  dummy object to wait for enternity to be killed
******************************************************************************/
void multi_dummy_proc(void)
{
	while (TRUE)
	{
		current_proc->p_comp_flag=PC_MULTI_DUMMY;
		process_sleep(0x1000);
	}
}

/******************************************************************************
 Function: OBJECT *get_multi_obj(ADDRESS *framelist,void *mram,void *heapptr,WORD getpal)

 By: David Schwartz

 Date: Nov 1994

 Parameters: framelist - frame #1
			 mram - multipart ram to use
			 heapptr - base address for obj data
			 getpal - "should I get a palette" flag (0x0001) (PFLAG_PALLOAD)
					  "am i a fighter" flag (hack mod) (0x0002) (PFLAG_FIGHTER)
					  "am i fighter 2)	(PFLAG_FIGHTER2)

 Returns: newly created object

 Description -  get a multipart object
******************************************************************************/
OBJECT *get_multi_obj(ADDRESS *framelist,void *mram,void *heapptr,WORD getpal)
{
	OBJECT *obj;
	LONG frame;

	frame=GET_LONG(framelist);
	frame=(LONG)COMPUTE_ADDR(heapptr,frame);

	current_proc->pa8=obj=object_setup((OIMGTBL *)frame,mram,getpal & PFLAG_PALLOAD);
	obj->oheap=heapptr;

	obj->oflags2|=M_MULTIPART;                              // set multipart bit
	if (getpal & PFLAG_FIGHTER)
	{
		obj->oflags|=M_FIGHTER;
		if (getpal & PFLAG_FIGHTER2)            // need to fix a bug in pselect
			p2_obj=obj;
		else p1_obj=obj;
	}
	else obj->oflags&=(~M_FIGHTER);                         // sony flag, tells vram alloc not a fighter

#if MULTI_OFF
	(P_RAM *)obj->oimg=&(obj->opinfo);
#else
	obj->oimg=mram;                                                 // multipart ram
#endif

	animate2(obj,framelist);

	return(obj);
}

/******************************************************************************
 Function: OBJECT *get_single_obj(LONG frame,void *heapptr,void *pal,WORD palflag)

 By: David Schwartz

 Date: Nov 1994

 Parameters: frame - single obj frame
			 heapptr - start of anim info
			 pal - palette to load
			 palflag - retrieve a pal

 Returns: object structure

 Description -  get a single object
******************************************************************************/
OBJECT *get_single_obj(LONG frame,void *heapptr,void *pal,WORD palflag)
{
	OBJECT *obj;

	//-SONY- assumes code space start at 0x80000000
#if SONY_PSX
#endif

	if (frame<0x80000000)                                                   // only compute address if value is an offset
		frame=(LONG)COMPUTE_ADDR(heapptr,frame);

	obj=object_setup((OIMGTBL *)frame,pal,palflag & PFLAG_PALLOAD);         // flag: get a  pal

	obj->oheap=heapptr;

	(LONG)obj->oimg=frame;

	obj->osag=((OIMGTBL *)frame)->isagptr;                  // pass sag
	obj->osize.yx=((OIMGTBL *)frame)->size.yx;              // pass size

	return(obj);
}

/******************************************************************************
 Function: OBJECT *object_setup(OIMGTBL *frame,void *mram,WORD getpal)


 By: David Schwartz

 Date: Nov 1994

 Parameters: frame - frame #1
			 palram - pal to use
			 getpal -  flag to get a palette (0-no 1-yes)

 Returns: current_proc->pa8 = created object
		  create object

 Description -  setup object
******************************************************************************/
OBJECT *object_setup(OIMGTBL *frame,void *palram,WORD getpal)
{
	OBJECT *obj;

	if ((obj=current_proc->pa8=get_object())==NULL)
		ermac(5);

	/* gobj3 */
	if (getpal!=0)                          // check to get a pal
	{
	//-SONY- SONY PSX CODE DEPENDENT at 0x80000000
#if SONY_PSX
#endif
		if ((long)((frame+1)->isagptr)<0)
			palram=(frame+1)->isagptr;

		obj->opal=get_fore_pal((WORD *)palram);
	}

	/* gobj8 */
	obj->ozval=0;                                                   // initial z

	return(obj);
}

/******************************************************************************
 Function: OBJECT *gbo_dmawnz(OIMGTBL *frame,void *heapptr,void *palram)


 By: David Schwartz

 Date: Nov 1994

 Parameters: frame - frame #1
			 palram - pal to use

 Returns: current_proc->pa8 = created object
		  create object

 Description -  setup object using a background palette
******************************************************************************/
OBJECT *gbo_dmawnz(OIMGTBL *frame,void *heapptr,void *palram)
{
	OBJECT *obj;

	if ((obj=current_proc->pa8=get_object())==NULL)
		ermac(5);

	obj->oheap=heapptr;

	/* gbo3 */
	//-NOTE- SONY PSX CODE DEPENDENT at 0x80000000
#if SONY_PSX
#endif
	if ((long)((frame+1)->isagptr)<0)
		palram=(frame+1)->isagptr;

	obj->opal=get_back_pal((WORD *)palram);

	obj->ozval=0;                                                   // initial z
	obj->oimg=(void *)frame;
	obj->osag=frame->isagptr;
	obj->osize.yx=frame->size.yx;

	obj->oflags&=0xfff0;

	return(obj);
}

/******************************************************************************
 Function: void get_his_a11_ani(WORD animnum)

 By: David Schwartz

 Date: Nov 1994

 Parameters: animnum - animation offset number

 Returns: current_proc->a10 - otherguys object
		  current_proc->a11 - animation ptr


 Description:   setup the other guys animation
******************************************************************************/
void get_his_a11_ani(WORD animnum)
{
	ADDRESS animtemp;

	animtemp=(ADDRESS)current_proc->pa9;

	current_proc->a11=get_his_char_ani(ANIM_TABLE1,animnum);
	current_proc->a10=(ADDRESS)current_proc->pdata.p_otherguy;

	(ADDRESS)current_proc->pa9=animtemp;

	return;
}

/******************************************************************************
 Function: void grab_screen(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:   grab screen does following:
				1) turns off autoerase
				2) zeros all object lists
				3) returns with a screwed up system (beware)
******************************************************************************/
void grab_screen(void)
{
	int loop;

	f_auto_erase=0;

	/* zero object lists */
	baklst1=baklst2=baklst3=baklst4=baklst5=baklst6=NULL;
	objlst=objlst2=objlst3=baklst7=baklst8=baklst9=NULL;

	/* free up objects */
	obj_free=object_ram;

	for (loop=0;loop<NUM_OBJECTS-1;loop++)
		object_ram[loop].olink=&object_ram[loop+1];

	object_ram[NUM_OBJECTS-1].olink=NULL;

	return;
}

/******************************************************************************
 Function: void sync_wait_dma_q(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:   Wait until all dmas in que have been processed
******************************************************************************/
void sync_wait_dma_q(void)
{
	WORD curtick=irq_timer;

	while (curtick==irq_timer);             // wait until next vblank

	//-NOTE- if we have a dma que then need a test for dma finished!
	DrawSync(0);

	curtick=irq_timer;
	while (curtick==irq_timer);

	return;
}

/******************************************************************************
 Function: void move_object_list(OBJECT **objsrc,OBJECT **objdest)

 By: David Schwartz

 Date: Nov 1994

 Parameters: objsrc - list to move from
			 objdest - list to move to

 Returns: None

 Description:   move a list of object list from one objlist to another
******************************************************************************/
void move_object_list(OBJECT **objsrc,OBJECT **objdest)
{
	OBJECT *src,*dest;

	src=((OBJECT *)objsrc)->olink;
	dest=(OBJECT *)objdest;

	while (dest->olink !=NULL)
		dest=dest->olink;                       // find end of the to list

	dest->olink=src;                                                // tak onto "to" list
	((OBJECT *)objsrc)->olink=NULL;                 // clear "from" list

	return;
}

/******************************************************************************
 Function: void delolist(OBJECT **obj_list)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj_list - list to delete

 Returns: None

 Description:   deletes and zeros an object list.
******************************************************************************/
void delolist(OBJECT **obj_list)
{
	OBJECT *obj,*kilobj;

	obj=((OBJECT *)obj_list)->olink;

	((OBJECT *)obj_list)->olink=NULL;                       // null the list

	while (obj!=NULL)
	{
		kilobj=obj;
		obj=obj->olink;                         // save next link

		delpal(kilobj);                         // delete the palette 1st

		kilobj->olink=obj_free;         // object --> top o free list
		obj_free=kilobj;                        // object is top o free list
	}

	return;
}

/******************************************************************************
 Function: void lineup_1pwm(OBJECT *obj_single,OBJECT *obj_multi)

 By: David Schwartz

 Date: Dec 1994

 Parameters: obj_single - single part object (a0)
			 obj_multi - multi part object (a1)

 Returns: None

 Description:   Lineup a 1 part obj (a0) with a multipart obj (a1)
******************************************************************************/
void lineup_1pwm(OBJECT *obj_single,OBJECT *obj_multi)
{
	short x,y;
	OIMGTBL *iptr;

	obj_single->oxpos.pos=obj_multi->oxpos.pos;
	obj_single->oypos.pos=obj_multi->oypos.pos;

	obj_single->oflags=(obj_multi->oflags & M_FLIPH) | (obj_single->oflags & (~M_FLIPH));           // match flip bits

	/* ani_adjust_single */
	iptr=(OIMGTBL *)obj_single->oimg;

	x=iptr->anixoff;                        // grab animation points
	y=iptr->aniyoff;

	if (obj_single->oflags & M_FLIPH)
		x=-x+iptr->size.u.xpos;                 // flip x

	if (obj_single->oflags & M_FLIPV)
		y=-y;

	obj_single->oxpos.u.intpos-=x;

	obj_single->oypos.u.intpos-=y;

	return;
}

/******************************************************************************
 Function: void lineup_on_a9(short pa9)

 By: David Schwartz

 Date: May 1995

 Parameters: pa9 - position to lineup with

 Returns: None

 Description:   lineup camera with pa9
******************************************************************************/
void lineup_on_a9(short pa9)
{
	PROCESS *ptemp;

	new_scroll_init();
	ptemp=CREATE(PID_SCROLL,scroll_a9);
	((long)ptemp->pa9)=(long)pa9;
	return;
}

/******************************************************************************
 Function: void center_around_me(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:   center camera around me
******************************************************************************/
void center_around_me(void)
{

	new_scroll_init();
	CREATE(PID_SCROLL,scroll_center);
	return;
}

/******************************************************************************
 Function: void new_scroll_init(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description:   setup for a new scroller
******************************************************************************/
void new_scroll_init(void)
{
	dallprc(PID_SCROLL);
	dallprc(PID_SHAKER);

	//worldtly.pos=0;                                       // removed to fix one frame glitch one camera is switched,
														// dont see why it was there in the first place

	return;
}

/******************************************************************************
 Function: void start_shadows(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:   enable shadows
******************************************************************************/
void start_shadows(void)
{
	f_shadows=1;                                            // flag: do shadows

	p1_obj->oflags2|=M_SHADOW;                      // set shadow bit

	p2_obj->oflags2|=M_SHADOW;                      // set shadow bit

	shadow_clt=get_fore_pal(SHADOW_P);                      /* load shadow palette */

	return;
}

/******************************************************************************
 Function: void gravity_ani(WORD anispeed,long inity,long gravity)

 By: David Schwartz

 Date: Dec 1994

 Parameters: anispeed - animation speed (a0)
			 inity - inital y vel (a1)
			 gravity - gravity (a10)

			 current_proc->a6 - rountine to call everytick (0=none)
			 current_proc->pdata.p_ganiy = ground level

 Returns: None

 Description:   gravity animation loop
******************************************************************************/
void gravity_ani(WORD anispeed,long inity,long gravity)
{
	OBJECT *obj=current_proc->pa8;
	short done;

	if (obj->oflags2 & M_MULTIPART)
	{
		(ADDRESS *)current_proc->pdata.p_store4=(ADDRESS *)current_proc->a6;                    // save routine to call every

		if (inity!=0)                                           // if zero vel dont stuff
			obj->oyvel.pos=inity;

		/* gani1 */
		init_anirate(anispeed);                         // setup animation speed

		/* gani2 */
		done=0;
		do
		{
			process_sleep(1);

			if ((ADDRESS *)current_proc->pdata.p_store4!=NULL)
				((FUNC)current_proc->pdata.p_store4)();

			obj->oyvel.pos+=gravity;                                // gravity

			if (obj->oyvel.pos>=0 && hit_ground_yet(obj)==SYSTEM_CARRY_SET)
				done=1;
		}
		while(done==0);

		stop_me(obj);
		ground_player(obj);

		current_proc->pdata.p_hit=0;
	}
	else
	{
		/* gravani_single */
		if (inity!=GNULL)
			obj->oyvel.pos=inity;                           // send 'em skywards

		/* gani5 */
		if (anispeed!=GNULL)
			init_anirate(anispeed);                         // setup ani speed

		/* gani6 */
		if ((ADDRESS)current_proc->a6!=GNULL)
			(ADDRESS *)current_proc->pdata.p_store4=(ADDRESS *)current_proc->a6;            // save routine to call

		/* gani7 */
		done=0;
		do
		{
			process_sleep(1);

			if ((ADDRESS)current_proc->pa9!=GNULL)
				next_anirate();

			/* gani8 */
			obj->oyvel.pos+=gravity;                                // single obj =  gravity by hand

			if (obj->oyvel.pos>=0 && hit_ground_yet(obj)==SYSTEM_CARRY_SET)
				done=1;
		}
		while(done==0);

		stop_a8(obj);
	}

	return;
}

/******************************************************************************
 Function: WORD hit_ground_yet(OBJECT *obj)

 By: David Schwartz

 Date: Dec 1994

 Parameters: obj - dude to check

 Returns: SYSTEM_CARRY_SET - hit ground
		  SYSTEM_CARRY_CLR - still in air

 Description:   hit the ground yet
******************************************************************************/
inline WORD hit_ground_yet(OBJECT *obj)
{
	if ((obj->oypos.u.intpos+obj->osize.u.ypos)<current_proc->pdata.p_ganiy-8)
		return(SYSTEM_CARRY_CLR);
	else return(SYSTEM_CARRY_SET);
}

short or1;
static short or2;
static short addend;
void randomize(short seed1,short seed2)
{
	or1=seed1;
	or2=seed2;
	addend=seed1*seed2/3;
}

short nrand(void)
{
	short ret;

	ret=(((or1^((or2<<5)+(addend<<3)-1))-or2)^addend);
	addend++;
	if ( addend>or2 )
		addend++;

	or1=or2;
	or2=ret;

	return(ret);
}

/******************************************************************************
 Function: long random(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: 32 bit random #

 Description:   generate a random number
******************************************************************************/
long random(void)
{
	//rand_num=rand()*rand();
	rand_num=nrand()*nrand();
	return(rand_num);

}

/******************************************************************************
 Function: LONG randu(LONG range)

 By: David Schwartz

 Date: Dec 1994

 Parameters: range

 Returns: random number from 1-range

 Description:   ranged random #
******************************************************************************/
LONG randu(LONG range)
{
	LONG rnd;
	LONG result;

	if ( range==0 )
		return(0);

	rnd=(LONG)random();

	result=(rnd % range)+1;

	return(result);
}

/******************************************************************************
 Function: long srandarc(LONG pa0)

 By: David Schwartz

 Date: Dec 1994

 Parameters: pa0 - =/- range

 Returns: signed random number in range +/- pa0

 Description:   signed ranged random # +-range
******************************************************************************/
long srandarc(LONG pa0)
{
	long a0;

	a0=randu(pa0<<1);
	a0-=(long)pa0;
	return(a0);
}

/******************************************************************************
 Function: WORD randper(WORD prob)

 By: David Schwartz

 Date: Dec 1994

 Parameters: prob =probability of event (0-1000) p(a0=1000) = 1; p(a0=1) = 1/1000.

 Returns: SYSTEM_CARRY_SET - if prob true
		  SYSTEM_CARRY_CLR - if prob false

 Description:   random percent routine
******************************************************************************/
WORD randper(WORD prob)
{
	LONG rnd;
	LONG result;

	rnd=(LONG)random();

	//result=rnd*1000;
	//result=((result & 0x0000ffff));
	result=rnd>>7;
	result=result%1000;

	if (result<prob)
		return(SYSTEM_CARRY_SET);
	else return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: void back_to_shang_check(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   check to see if it is time to revert to shang
******************************************************************************/
void back_to_shang_check(void)
{
	if (am_i_shang()==SYSTEM_CARRY_CLR || (current_proc->pa8)->ochar==FT_ST)
		return;

	if (get_tsl(l_morph,current_proc->pa8)<0x40*8)
		return;

	/* in normal morph dont allow shang to morph back if projectile is out */
	if ( f_shang_morph==MORPH_NORMAL)
	{
		if (process_exist(PID_BLADE,0xfffe)!=NULL)                      // check blade and dead blade
			return;

		if (process_exist(PID_GRAVP,0xffff)!=NULL)                      // check blade and dead blade
			return;

		if (process_exist(PID_POOF,0xffff)!=NULL)                       // check for explosion from smoke
			return;

		if (process_exist(PID_SONIC,0xffff)!=NULL)                       // check for sonic
			return;

		if ((current_proc->pdata.p_otherproc)->pdata.p_action==ACT_BLURRED )                    // dont switch if other dude is blurred
			return;

		if (process_exist(PID_ROBO_BOMB,0xffff)!=NULL)                  // check robo bomb
			return;

		if (process_exist(PID_SCREAM_WAVE,0xffff)!=NULL)                        // check lia scream
			return;

		if (process_exist(PID_BLOOD_GLOB,0xffff)!=NULL)                 // check lia scream
			return;

		if (process_exist(PID_FBLAST,0xffff)!=NULL)                     // check lia scream
			return;

		if ( (current_proc->pa8)->oid==OID_P1)
		{
			 if (process_exist(PID_DECOY1,-1)!=NULL || process_exist(PID_PROJ1,0xff00)!=NULL )
				return;

			 if (process_exist(PID_FLOOR_ICE1,-1)!=NULL)
				return;

		}
		else
		{
			 if (process_exist(PID_DECOY2,-1)!=NULL || process_exist(PID_PROJ2,0xff00)!=NULL )
				return;

			 if (process_exist(PID_FLOOR_ICE2,-1)!=NULL)
				return;

		}
	}

	back_to_shang_form();
}

/******************************************************************************
 Function: WORD am_i_shang(OBJECT *pa8)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa8 - object to test for shang

 Returns: SYSTEM_CARRY_SET - current obj is shang
		  SYSTEM_CARRY_CLR - current obj is not shang

 Description:   check to see if it is time to revert to shang
******************************************************************************/
WORD am_i_shang()
{
	if (current_proc->pdata.p_flags & PM_SHANG)
		return(SYSTEM_CARRY_SET);			// yes
	else return(SYSTEM_CARRY_CLR);                               // nope
}

/******************************************************************************
 Function: void back_to_shang_form(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   change current object back to shang tsu
******************************************************************************/
void back_shang_ani(void)
{
	do_next_a9_frame(current_proc->pa8);
	((ADDRESS*)current_proc->pa9)-=2;
	process_sleep(4);
	return;
}

extern short ochar_ground_offsets[];
void back_to_shang_form(void)
{
	OBJECT *obj=current_proc->pa8;
	WORD ochar;
	void *animframe;
	PROCESS *ptemp;

	init_special_act(ACT_MORPH,obj);

	clear_inviso(obj);

	borrow_ochar_sound(4,FT_ST);            // morph sound

	ochar=obj->ochar;
	obj->ochar=FT_ST;
	ochar_bank_bit(obj);

	if ( ochar==FT_SMOKE )
	{
		ptemp=active_head;
		while ( ptemp!=NULL )
		{
			if ( ptemp->procid==PID_SMOKER && current_proc->pa8==ptemp->pa8 )
			{
				process_kill(ptemp);                            // kill smoke process
				break;
			}

			ptemp=ptemp->plink;
		}
	}

	/* load shang fighter data */
	if (f_shang_morph==MORPH_NORMAL)
	{
		newchar_load(FT_ST);
	}
	else
	{
		obj->oheap=(p2_obj==obj)?p2_heap:p1_heap;                       // switch to shang heap for return from quasi morph
	}

	animframe=get_char_ani(ANIM_TABLE2,ochar);              // get proper anim table
	((ADDRESS*)animframe)+=3;
	animframe=(void*)COMPUTE_ADDR(obj->oheap,GET_LONG(animframe));
	animframe=(void*)COMPUTE_ADDR(obj->oheap,GET_LONG(animframe));
	((OIMGTBL *)animframe)++;
	player_swpal(current_proc,(ADDRESS *)GET_LONG(animframe),current_proc->pa8);

	((ADDRESS*)current_proc->pa9)+=4;

	/* adjust y for the morph */
	obj->oypos.u.intpos+=(ochar_ground_offsets[ochar]-ochar_ground_offsets[FT_ST]);

	back_shang_ani();
	//back_shang_ani();
	back_shang_ani();

	/* switch to shang pal */
	obj->ochar=FT_ST;
	ochar_bank_bit(obj);
	player_normpal();
	do_first_a9_frame(ANIM_STANCE);
	ground_ochar();
	current_proc->pdata.p_ganiy=obj->oypos.u.intpos;
	process_sleep(4);

	reaction_exit(obj);
}

/*************************************************************************************
 Function: void air_dragon(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   air dragon
******************************************************************************/
void air_dragon(void)
{
	multi_adjust_xy(current_proc->pdata.p_slave,SCX(0x28),-SCY(0x28));
	return;
}

/******************************************************************************
 Function: void lower_dragon(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   lower dragon
******************************************************************************/
void lower_dragon(void)
{
	multi_adjust_xy(current_proc->pdata.p_slave,SCX(0x1d),SCY(0x30));
	return;
}

/******************************************************************************
 Function: void away_x_vel(PROCESS *proc,OBJECT *obj,long initx)

 By: David Schwartz

 Date: Nov 1994

 Parameters: initx - initial x velocity (16.16)
			 obj - current object
			 proc - current proc

 Returns: None

 Description:	left or right velocity
******************************************************************************/
inline void away_x_vel(PROCESS *proc,OBJECT *obj,long initx)
{
	if (is_he_right(proc)==SYSTEM_CARRY_SET)
		set_x_vel(obj,-initx);
	else set_x_vel(obj,initx);
}

/******************************************************************************
 Function: void towards_x_vel(PROCESS *proc,OBJECT *obj,long initx)

 By: David Schwartz

 Date: Nov 1994

 Parameters: initx - initial x velocity (16.16)
			 obj - current object
			 proc - current process

 Returns: None

 Description:	left or right velocity
******************************************************************************/
inline void towards_x_vel(PROCESS *proc,OBJECT *obj,long initx)
{
	if (is_he_right(proc)==SYSTEM_CARRY_CLR)
		set_x_vel(obj,-initx);
	else set_x_vel(obj,initx);
}

/******************************************************************************
 Function: void flight_call(OBJECT *obj,long initx,long inity,long gravity,short anispeed,long anioffset)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - ptr to object
 			 initx - inital x velocity (a0)
			 inity - initial y velocity (a1)
			 gravity - gravity to use (a2)
			 anispeed - animation speed (sleep ticks) (a3)
			 anioffset - animation offset to use (a9)

			 current_proc->a6 - routine to call (NULL=NONE)

 Returns: None

 Description:	flight routines for people to fly
******************************************************************************/
void flight_call(OBJECT *obj,long initx,long inity,long gravity,short anispeed,long anioffset)
{

	if (inity != DONT_TOUCH)
		obj->oyvel.pos=inity;

	/* flgt2 */
	if (gravity != DONT_TOUCH)
		obj->ograv.pos=gravity;

	/* flgt3 */
	if (initx != DONT_TOUCH)
		away_x_vel(current_proc,obj,initx);

	/* flgt6 */
	/* setup animation (a9) */
	if (anioffset>=0) 								// neg offset = animation already setup
	{
		if ((anioffset>>16)==0)
			get_char_ani(ANIM_TABLE1,anioffset & 0xfff);	// pick correct table
		else get_char_ani(ANIM_TABLE2,anioffset & 0xffff);
	}

	/* flgt5 */
	if (anispeed!=DONT_TOUCH)
		init_anirate(anispeed);								// set things up to animate

	(ADDRESS *)current_proc->pdata.p_store4=(ADDRESS *)current_proc->a6;	// routine to call every tick!!

	/* check if starting underground */
	if (obj->oypos.u.intpos>current_proc->pdata.p_ganiy)		// check if start below ground level
		obj->oypos.u.intpos=current_proc->pdata.p_ganiy;		// yes, set to ground level

	/* flight loop */
	do
	{
		process_sleep(1);

		if ((ADDRESS *)current_proc->pdata.p_store4!=NULL)
			((FUNC)current_proc->pdata.p_store4)();

		next_anirate();
	}
	while(obj->oypos.u.intpos<current_proc->pdata.p_ganiy);		// keep falling until below ground

	obj->ograv.pos=0;										// stop gravity
	stop_me(obj);
	ground_player(obj);

	return;
}


/******************************************************************************
 Function: void flight_loop(OBJECT *obj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - ptr to object
			 current_proc->pdata.pstore4 - routine to call (NULL=NONE)

 Returns: None

 Description:	flight loop routine
 ******************************************************************************/
 void flight_loop(OBJECT *obj)
 {
	/* flight loop */
	do
	{
		process_sleep(1);

		if ((ADDRESS *)current_proc->pdata.p_store4!=NULL)
			((FUNC)current_proc->pdata.p_store4)();

		next_anirate();
	}
	while(obj->oypos.u.intpos<current_proc->pdata.p_ganiy);		// keep falling until below ground

	obj->ograv.pos=0;										// stop gravity
	stop_me(obj);
	ground_player(obj);

	return;
}

/******************************************************************************
 Function: WORD is_he_facing_me(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: SYSTEM_CARRY_SET = yes
		  SYSTEM_CARRY_CLR = no

 Description:   is other dude facing me
******************************************************************************/
WORD is_he_facing_me(void)
{
	PROCESS *pa13;
	WORD result;

	pa13=current_proc;

	current_proc=current_proc->pdata.p_otherproc;

	result=am_i_facing_him();

	current_proc=pa13;

	return(result);
}

/******************************************************************************
 Function: void freeze_2_pages(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:   freeze display pages
******************************************************************************/
void freeze_2_pages(void)
{
	f_novel=1;

	scrolly.pos=0;

	stop_scrolling();                       // no scrolling

	process_sleep(2);                       // time 4 both pages to show same thing

	f_auto_erase=0;

	process_sleep(2);

	//DISPLAY_OFF;

	sync_wait_dma_q();

	grab_screen();

	return;
}

/******************************************************************************
 Function: void slave_in_front(void)

 By: David Schwartz

 Date: Mar 1995

 Parameters: None

 Returns: None

 Description:   slave in front of master
******************************************************************************/
void slave_in_front(void)
{
	(current_proc->pdata.p_slave)->ozval=100;
	return;
}

/******************************************************************************
 Function: void unimmune_mpp(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:   allow multi part objs to be killed
******************************************************************************/
void unimmune_mpp(void)
{
	PROCESS *pa0;

	pa0=(PROCESS *)&active_head;

	/* binit5 */
	do
	{
		if ((pa0->procid & 0x7fff)==PID_MPO)
			unimmune_a0(pa0);
		pa0=pa0->plink;
	}
	while(pa0!=NULL);

	return;
}

/******************************************************************************
 Function: void immune_mpp(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:   dont allow multi part objs to be killed
******************************************************************************/
void immune_mpp(void)
{
	PROCESS *pa0;

	pa0=(PROCESS *)&active_head;

	/* binit3 */
	do
	{
		if (pa0->procid ==PID_MPO)
			immune_a0(pa0);
		pa0=pa0->plink;
	}
	while(pa0!=NULL);

	return;
}

WORD get_my_matchw(void)
{
	if (current_proc->pa8==p1_obj)
		return(p1_matchw);
	else
	{
		return(p2_matchw);
	}
}

WORD get_his_matchw(void)
{
	if (current_proc->pa8!=p1_obj)
		return(p1_matchw);
	else
	{
		return(p2_matchw);
	}
}

/******************************************************************************
 Function: OBJECT *make_solid_object(WORD pa0,WORD pa3x,WORD pa3y)

 By: David Schwartz

 Date: May 1995

 Parameters: pa0 - color
			 pa3x,y - size of object

 Returns: object ptr (current_proc->pa8)

 Description:   create a solid color object
******************************************************************************/
OBJECT *make_solid_object(WORD pa0,WORD pa3x,WORD pa3y)
{
	OBJECT *obj;

	obj=get_object();

	obj->oflags|=M_BOX_POLY;

	/* set color */
	obj->header.t_xoffset=GET_RED(pa0);                     // set red
	obj->header.t_yoffset=GET_GREEN(pa0);                   // set green
	obj->header.t_width=GET_BLUE(pa0);                              // set blue

	obj->osize.u.xpos=pa3x;         // set size of box
	obj->osize.u.ypos=pa3y;

	current_proc->pa8=obj;

	return(obj);
}
