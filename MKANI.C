/******************************************************************************
 File: mkani.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Animation Routines
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
#include "mkprop.h"
#include "mksound.h"
#include "valloc.h"

void beep_sound(void)
{
	ochar_sound(0x1a);
}

void robo_laugh_sound(void)
{
	ochar_sound(0x1c);
}

void camp_fire_sound(void)
{
	ochar_sound(0xa);
}

void motaro_leg_stance(void)
{
	current_proc->pdata.p_store1++;

	if (current_proc->pdata.p_store1>=2)
	{
		current_proc->pdata.p_store1=0;
		get_char_ani(ANIM_TABLE2,0);		// arcade mod so we can get correct ptr (if psx files changes make sure to update)
											// add a_stance2 to entry 0 of anitab2
	}
}

void stsw_sound(void)
{
	triple_sound(2);
	return;
}

void st_tumble_sound(void)
{
	ochar_sound(5);
	return;
}

void morph_swpal(void)
{
	LONG animframe;
	OBJECT *obj=current_proc->pa8;

	animframe=GET_LONG(current_proc->pa9);		// grab img which has pal
	animframe=(LONG)((ADDRESS*)COMPUTE_ADDR(obj->oheap,animframe));
	animframe=GET_LONG(animframe);
	animframe=(LONG)((OIMGTBL*)COMPUTE_ADDR(obj->oheap,animframe)+1);
	player_swpal(current_proc,(void *)(GET_LONG(animframe)),obj);
	return;
}

void boomerang_adjust(void)
{
	adjust_xy_a5(current_proc->pdata.p_slave,SCX(0x48),SCY(0x20));
	return;
}

extern WORD SKELRIP_P[];
extern void *finish_heap_ptr;
void useshak_bod_pal(void) 
{
	match_me_with_him();
	player_swpal(current_proc,SKELRIP_P,current_proc->pa8);

	(current_proc->pa8)->oheap=finish_heap_ptr;		// switch to correct heap usasge
	return;
}

extern void sk_ray1(void);
extern void sk_ray2(void);
extern void sk_ray3(void);
void spawn_bubble(void);

JUMPTBL calla_jumptbl[]=
{
	0,							// 0 not used
	sweep_sounds,				// 1
	lineup_number_2,			// 2
	delete_slave,				// 3
	rsnd_big_whoosh,			// 4
	player_normpal,				// 5
	sans_repell_3,				// 6
	sonya_zap_pal,				// 7
	rsnd_footstep,				// 8
	air_dragon,					// 9
	lower_dragon,				// 10
	slave_inviso,				//	11
	slave_viso,					//	12
	new_float_ani,				//	13
	shock_init,					//	14
	stun_speed_3,				//	15
	stun_speed_6,				//	16
	shock_check,				//	17
	set_a10_to_him,				//	18
	set_a10_to_slave,			//	19
	stsw_sound,					// 20
	st_tumble_sound,			// 21
	morph_swpal,				// 22
	motaro_leg_stance,			// 23
	rsnd_stab,					// 24
	rsnd_whoosh,				// 25
	rsnd_big_block,			// 26
	camp_fire_sound,			// 27
	beep_sound,				// 28
	robo_laugh_sound,	// 29
	sk_ray1,					// 30
	sk_ray2,					// 31
	sk_ray3,						// 32
	rsnd_small_block,	// 33
	boomerang_adjust,		// 34
	spawn_bubble,		// 35
	useshak_bod_pal,
};



/******************************************************************************
 Function: void find_ani_part2(short animnum)

 By: David Schwartz

 Date: Oct 1994

 Parameters: animnum - animation # to use (special move ANIM_TABLE2)

 Returns: current_proc->pa9 - ptr to start of 2nd part

 Description:	find the 2nd part of an animation:
			the 2nd part is defined as an animation that has the format as follows:
				frame 1		<--- 1st part
				frame 2
				...
				frame N
				0			<--- terminates 1st part
				frame 3		<--- 2nd part
				...
				frame N
				0			<--- terminates 2nd part
******************************************************************************/
void find_ani2_part2(short animnum)
{
	get_char_ani(ANIM_TABLE2,animnum);

	while ((GET_LONG(current_proc->pa9)++) != 0);

	return;
}

/******************************************************************************
 Function: void find_ani_part2(short animnum)

 By: David Schwartz

 Date: Oct 1994

 Parameters: animnum - animation # to use

 Returns: current_proc->pa9 - ptr to start of 2nd part

 Description:	find the 2nd part of an animation:
			the 2nd part is defined as an animation that has the format as follows:
				frame 1		<--- 1st part
				frame 2
				...
				frame N
				0			<--- terminates 1st part
				frame 3		<--- 2nd part
				...
				frame N
				0			<--- terminates 2nd part
******************************************************************************/
void find_ani_part2(short animnum)
{
	get_char_ani(ANIM_TABLE1,animnum);

	while ((GET_LONG(current_proc->pa9)++) != 0);

	return;
}

/******************************************************************************
 Function: void find_part2(void)

 By: David Schwartz

 Date: Oct 1994

 Parameters: curent_proc->pa9 - ptr to start of frames to look out

 Returns: current_proc->pa9 - ptr to start of next part

 Description:	find the next part of an animation:
******************************************************************************/
inline void find_part2(void)
{
	while ((GET_LONG(current_proc->pa9)++) != 0);

	return;
}

/******************************************************************************
 Function: void find_part_a14(WORD acount)

 By: David Schwartz

 Date: Oct 1994

 Parameters: acount - # of parts to find

 Returns: None

 Description:	find the acount part of an animation
******************************************************************************/
void find_part_a14(WORD acount)
{
	while (--acount>0)
		find_part2();

	return;
}

/******************************************************************************
 Function: void find_last_frame(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: Nine

 Returns: None

 Description:	get last frame of an animation
******************************************************************************/
void find_last_frame(void)
{
	/* get to end of list */
	while (GET_LONG(current_proc->pa9)++ !=0);

	(ADDRESS *)current_proc->pa9-=2;				// last frame of sequence

	return;
}

/******************************************************************************
 Function: void animate2(OBJECT *obj,ADDRESS *framelist)

 By: David Schwartz

 Date: Sept 1994

 Parameters: framelist - ptr to frame data table list

 Returns: None

 Description:	setup frame to display

 NOTE: we have kept the same structures for multipart ram as the
	   arcade game.  PLEASE NOTE THAT ALL IMAGES HAVE BEEN PUT
	   TOGETHER AND THUS THERE IS ONLY ONE ENTRY PER OBJECT.  IF THIS
	   SHOULD EVER CHANGE THEN THE ovcache field will need to be expanded to
	   handle each piece.  Also, code will need to be modified to deal with
	   this.

 Revisions:
	11/22/94 changed frame parameter to framelist parameter.  The table now points to a list
	 		 of pieces.  These pieces are then used to build up the image. This makes the
			 code work like the arcade version.

	11/21/94 added obj parameter so that routines that switch current_proc->pa8 and
			 the obj parameter can have current_proc->pa8 and obj to use set to two different
			 things.
******************************************************************************/
extern BYTE *soflor_addr;
void animate2(OBJECT *obj,ADDRESS *framelist)
{
	OIMGTBL *frame;
	P_RAM *pram_ptr;
	RECT frame_info;
	BYTE *temp;


	pram_ptr=(P_RAM *)obj->oimg;

	obj->oshape=framelist;						// define current shape

#if !MULTI_OFF
	while (((LONG)frame=GET_LONG(framelist)) != 0)
	{
#else
	(LONG)frame=GET_LONG(framelist);
#endif

		frame=(OIMGTBL *)COMPUTE_ADDR(obj->oheap,frame);

		pram_ptr->prsagptr=(ADDRESS)COMPUTE_ADDR(obj->oheap,frame->isagptr);

		obj->osize.yx=pram_ptr->prsize.yx=frame->size.yx;				// set size

		pram_ptr->prflags=obj->oflags & (M_FLIPH+M_FLIPV);			// set flip flags

		switch ((pram_ptr->prflags & 0x30)>>4)
		{
			case 0:	/* NO FLIPS */
				pram_ptr->prpos.u.xpos=frame->anixoff;
				pram_ptr->prpos.u.ypos=frame->aniyoff;
				break;
			case 1: /* H FLIPS */
				pram_ptr->prpos.u.xpos=pram_ptr->prsize.u.xpos-frame->anixoff;
				pram_ptr->prpos.u.ypos=frame->aniyoff;
				break;
			case 2: /* V FLIPS */
				pram_ptr->prpos.u.xpos=frame->anixoff;
				pram_ptr->prpos.u.ypos=pram_ptr->prsize.u.ypos-frame->aniyoff;
				break;
			case 3: /* HV FLIPS */
				pram_ptr->prpos.u.xpos=pram_ptr->prsize.u.xpos-frame->anixoff;
				pram_ptr->prpos.u.ypos=pram_ptr->prsize.u.ypos-frame->aniyoff;
				break;
		}

		/* transfer image to correct page */
		frame_info.w=pram_ptr->prsize.u.xpos;
		frame_info.h=pram_ptr->prsize.u.ypos;

		if ((obj->oflags & M_FIGHTER)==0)
		{
			/* vram manager call */
			obj->header.t_width=frame_info.w;
			obj->header.t_height=frame_info.h;
			obj->ovcache=alloc_vram_mem((void *)pram_ptr->prsagptr,&(obj->header),obj->ovcache);
		}
		else
		{

#if 1
/*************************************************
	COMPRESSION CODE
*************************************************/
			/* special case code for motaro, since he is so big we need to have him decomp
			 	 to a constant memory area.  It so happens the soul floor data area is available, so
			 	 we will use it.
		  */
			if ( obj->ochar==FT_MOTARO && gstate!=GS_BUYIN)
			{
				temp=soflor_addr;
				unbetterSimpleRLE(pram_ptr->prsagptr,&soflor_addr);
				soflor_addr=temp;
				pram_ptr->prsagptr=(ADDRESS)soflor_addr;
			}
			else
			{
				pram_ptr->prsagptr=(ADDRESS)uncompress_image((BYTE *)(pram_ptr->prsagptr));
			}
/*************************************************
	COMPRESSION CODE
*************************************************/
#endif

			if (obj==p1_obj)
			{
				frame_info.x=PLYR_1_BASE_X;
				frame_info.y=PLYR_1_BASE_Y;
			}
			else
			{
				frame_info.x=PLYR_2_BASE_X;
				frame_info.y=PLYR_2_BASE_Y;
			}

			/* load image and setup header for fighters */
			obj->header.tpage=LOADTPAGE((ADDRESS *)pram_ptr->prsagptr,TEXTURE_MODE,TRANS_RATE,frame_info.x,frame_info.y,(pram_ptr->prsize.u.xpos+3)&(~3),frame_info.h);
			obj->header.t_xoffset=(frame_info.x%128)*2;
			obj->header.t_yoffset=frame_info.y%256;
			obj->header.t_width=frame_info.w;
			obj->header.t_height=frame_info.h;
		}

#if !MULTI_OFF
		/* next part */
		pram_ptr++;											// move to next storage entry
		framelist++;												// move to next image entry
	}

	pram_ptr->prsagptr=(ADDRESS)0;					// terminate list
#endif

	return;
}

/******************************************************************************
 Function: void do_first_a9_frame(void)

 By: David Schwartz

 Date: Aug 8, 1994

 Parameters: None

 Returns: None

 Description:	get frame # 1 and display first animation
******************************************************************************/
inline void do_first_a9_frame(short animnum)
{
	get_char_ani(ANIM_TABLE1,animnum);
	do_next_a9_frame(current_proc->pa8);

	return;
}

/******************************************************************************
 Function: WORD do_next_a9_frame(OBJECT *obj)

 By: David Schwartz

 Date: Aug 8, 1994

 Parameters: current_proc->pa9 - animation table ptr

 Returns: SYSTEM_CARRY_SET --> skip sleep flag
		  SYSTEM_CARRY_CLR --> don't skip sleep flag
		  SYSTEM_ZERO_SET --> end of animation reached!!
		  SYSTEM_ZERO_CLR --> not end of animation reached!!

 Description:	standard "advance to next frame" routine

 		ANIMATION COMMANDS MUST BE #'S THAT COULD NEVER BE OFFSETS INTO TABLES
		AS LONG AS THE COMMANDS ARE LESS THEN 20, THINGS WILL BE OK.  THIS IS THE
		CASE BECAUSE THE FIRST 20 BYTES (AT LEAST) IS THE ANIMATION TABLE PTR LIST
******************************************************************************/
WORD do_next_a9_frame(OBJECT *obj)
{
	LONG animframe;
	short offset,offset1;
	PROCESS *ptemp;
	OBJECT *objslave;

	animframe=GET_LONG(current_proc->pa9)++;

	while (animframe != ANI_END)
	{
		switch (animframe)
		{
			case ANI_JUMP:		// animation script "jump" command
				(ADDRESS *)current_proc->pa9=(ADDRESS *)COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->pa9));	// get new animation ptr
				break;

			case ANI_FLIP:		// animation script "flip" command
				flip_multi(obj);
				break;

			case ANI_ADJUSTX:		// animation script "adjust x" command
				offset=(short)(GET_LONG(current_proc->pa9)++);
				offset=offset*80/100;
				multi_adjust_xy(obj,(short)offset,0);
				break;

			case ANI_ADJUSTXY:	// animation script "adjust xy" command
				offset=(short)GET_WORD(current_proc->pa9)++;
				offset=offset*80/100;
				offset1=(short)GET_WORD(current_proc->pa9)++;
				offset1=offset1*85/100;
				multi_adjust_xy(obj,offset,offset1);
				break;

			case ANI_NOSLEEP:		// animation script "skip sleep" command
				animframe=GET_LONG(current_proc->pa9)++;		// grab frame which does not sleep
				animate2(obj,(ADDRESS *)COMPUTE_ADDR(obj->oheap,animframe));
				return(SYSTEM_CARRY_SET+SYSTEM_ZERO_CLR);		// flag: dont sleep please, no end of anim
				break;

			case ANI_CALLA:		// animation script call routine (next long)
				animframe=GET_LONG(current_proc->pa9)++;		// grab routine to call
				if (animframe!=0)
					calla_jumptbl[animframe]();
				break;

			case ANI_SOUND:		// animation script make a sound call
				animframe=GET_LONG(current_proc->pa9)++;		// sound call to make
				triple_sound(animframe);
				break;

			case ANI_OCHAR_JUMP:	// animation script ochar conditional jump
				animframe=GET_LONG(current_proc->pa9)++;		// grab ochar to check for
				if (animframe==obj->ochar)
					(ADDRESS *)current_proc->pa9=(ADDRESS *)COMPUTE_ADDR(obj->oheap,GET_LONG(current_proc->pa9));	// get new animation ptr
				else ((ADDRESS*)current_proc->pa9)++;
				break;

			case ANI_FLIP_V:		// animation script vertical flip
				flip_multi_v(obj);
				break;

			case ANI_OFFSET_XY:		// animation script offset xy
				/* xoffset */
				offset=(short)(GET_WORD(current_proc->pa9)++);
				offset=offset*80/100;
				if (obj->oflags & M_FLIPH)
					offset=-offset;
				obj->oxpos.u.intpos=offset+(current_proc->pdata.p_otherguy)->oxpos.u.intpos;

				offset=(short)(GET_WORD(current_proc->pa9)++);
				offset=offset*85/100;
				obj->oypos.u.intpos=offset+(current_proc->pdata.p_otherguy)->oypos.u.intpos;
				break;

			case ANI_SLADD:			// animation script add slave
				ptemp=gmo_proc((current_proc->pa8)->oheap);				// get process which holds data
				objslave=(OBJECT *)current_proc->a10=current_proc->pa8;		// a10 = object
				(current_proc->pa8)->oslink=ptemp;					// oslink = process
				current_proc->pa8=obj;

				animframe=GET_LONG(current_proc->pa9)++;			// grab image to make slave from
				objslave->ochar=obj->ochar;							// same ochar as thrower
				current_proc->pdata.p_slave=(OBJECT *)current_proc->a10;		// projectiles are slave objects

				objslave->ozval=FRONT_Z+1;

				match_ani_points(obj,objslave);						// lineup proj with thrower

				insert_object(objslave,&objlst);

				break;

			case ANI_SLANI:			// animation script slave animate
				current_proc->a10=(ADDRESS)current_proc->pdata.p_slave;
				match_ani_points(obj,(OBJECT *)current_proc->a10);		// always lineup slave
				do_next_a9_frame((OBJECT *)current_proc->a10);			// animate slave and then return
				current_proc->pa8=obj;
				break;

			case ANI_SLANI_SLEEP:			// animation script slave animate
				current_proc->a10=(ADDRESS)current_proc->pdata.p_slave;
				match_ani_points(obj,(OBJECT *)current_proc->a10);		// always lineup slave
				do_next_a9_frame((OBJECT *)current_proc->a10);			// animate slave and then return
				current_proc->pa8=obj;
				return(SYSTEM_CARRY_CLR+SYSTEM_ZERO_CLR);		// clear zero as well as carry and sleep
				break;

			case ANI_SWPAL:			// animation script switch to next img pal
				animframe=GET_LONG(current_proc->pa9);		// grab img which has pal
				animframe=(LONG)((ADDRESS*)COMPUTE_ADDR(obj->oheap,animframe));
				animframe=GET_LONG(animframe);
				animframe=(LONG)((OIMGTBL*)COMPUTE_ADDR(obj->oheap,animframe)+1);
				player_swpal(current_proc,(void *)(GET_LONG(animframe)),obj);
				break;

			case ANI_OCHAR_SOUND:
				animframe=GET_LONG(current_proc->pa9)++;		// grab img which has pal
				ochar_sound_stop(animframe);
				ochar_sound(animframe);
				break;
			default:
				animate2(obj,(ADDRESS *)COMPUTE_ADDR(obj->oheap,animframe));
				return(SYSTEM_CARRY_CLR+SYSTEM_ZERO_CLR);
		}
		animframe=GET_LONG(current_proc->pa9)++;				// get next frame
	}
 	return(SYSTEM_CARRY_CLR+SYSTEM_ZERO_SET);
}

/******************************************************************************
 Function: void animate_a9(WORD anispeed,WORD animnum)

 By: David Schwartz

 Date: Nov 1994

 Parameters: anisleed - time between animation updates
			 animnum - anim offset to run through (ANIM_TABLE1)

 Returns: None

 Description:	animate throught a given sequence
******************************************************************************/
void animate_a9(WORD anispeed,WORD animnum)
{
	get_char_ani(ANIM_TABLE1,animnum);
	mframew(anispeed);
	return;
}

/******************************************************************************
 Function: void animate2_a9(WORD anispeed,WORD animnum)

 By: David Schwartz

 Date: Nov 1994

 Parameters: anispeed - time between animation updates (a0[32..16])
			 animnum - anim offset to run through (ANIM_TABLE2) (a0[15..0])

 Returns: None

 Description:	animate throught a given sequence
******************************************************************************/
void animate2_a9(WORD anispeed,WORD animnum)
{
	get_char_ani(ANIM_TABLE2,animnum);
	mframew(anispeed);
	return;
}

/******************************************************************************
 Function: void pose_a9(WORD offset,WORD animnum)

 By: David Schwartz

 Date: Nov 1994

 Parameters: offset - offset frame to pose on (a0[32..16])
			 animnum - anim offset to run through (ANIM_TABLE1) (a0[15..0])

 Returns: None

 Description:	animate throught a given sequence
******************************************************************************/
void pose_a9(WORD offset,WORD animnum)
{
	get_char_ani(ANIM_TABLE1,animnum);
	((ADDRESS *)current_proc->pa9)+=offset;
	do_next_a9_frame(current_proc->pa8);
	return;
}

/******************************************************************************
 Function: void pose_him_a9(WORD offset,WORD animnum)

 By: David Schwartz

 Date: Mar 1995

 Parameters: offset - offset frame to pose on (a0[32..16])
			 animnum - anim offset to run through (ANIM_TABLE1) (a0[15..0])

 Returns: None

 Description:	animate him throught a given sequence
******************************************************************************/
void pose_him_a9(WORD offset,WORD animnum)
{
	PROCESS *ta13;

	ta13=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	pose_a9(offset,animnum);
	current_proc=ta13;

	return;
}

/******************************************************************************
 Function: void pose2_a9(WORD offset,WORD animnum)

 By: David Schwartz

 Date: Nov 1994

 Parameters: offset - offset frame to pose on
			 animnum - anim offset to run through (ANIM_TABLE2)

 Returns: None

 Description:	animate throught a given sequence
******************************************************************************/
void pose2_a9(WORD offset,WORD animnum)
{
	get_char_ani(ANIM_TABLE2,animnum);
	((ADDRESS *)current_proc->pa9)+=offset;
	do_next_a9_frame(current_proc->pa8);
	return;
}

/******************************************************************************
 Function: void pose_him(WORD animnum)

 By: David Schwartz

 Date: Nov 1994

 Parameters: animnum - anim offset to run through (ANIM_TABLE1)

 Returns: None

 Description:	pose the other fighter
******************************************************************************/
void pose_him_a0(WORD animnum)
{
	PROCESS *ptemp;

	ptemp=current_proc;							// pretend I am him
	current_proc=current_proc->pdata.p_otherproc;

	if ((short)animnum>=0)								// neg, we already got an frame
		get_char_ani(ANIM_TABLE1,animnum);		// pass other proc new anim ptr

	do_next_a9_frame(current_proc->pa8);

	current_proc=ptemp;
	return;
}

/******************************************************************************
 Function: void mframew(WORD sleeptime)

 By: David Schwartz

 Date: Sept 1994

 Parameters: sleep time between frames
			current_proc->pa9 - table of animations
			current_proc->pa8 - 1st object on list

 Returns: None

 Description:	animate through a list of multipart frames
******************************************************************************/
void mframew(WORD sleeptime)
{
	OBJECT *obj=current_proc->pa8;
	WORD carry;
	while (((carry=do_next_a9_frame(obj)) & SYSTEM_ZERO_MASK) != SYSTEM_ZERO_SET)
	{
		if ((carry & SYSTEM_CARRY_MASK) == SYSTEM_CARRY_CLR)
			process_sleep(sleeptime);
	}
}

/******************************************************************************
 Function: void double_mframew(WORD sleeptime)

 By: David Schwartz

 Date: Sept 1994

 Parameters: sleep time between frames
			current_proc->pa9 - obj #1 anim ptr
			current_proc->pa8 - obj #1
			current_proc->a11 - obj #2 animptr
			current_proc->a10 - obj #2

 Returns: None

 Description:	animate through a list of multipart frames for two objects
******************************************************************************/
void double_mframew(WORD sleeptime)
{
	while ((double_next_a9() & SYSTEM_ZERO_MASK) != SYSTEM_ZERO_SET)
			process_sleep(sleeptime);
}

/******************************************************************************
 Function: void mframew_him(WORD sleeptime)

 By: David Schwartz

 Date: Nov 1994

 Parameters: sleep time between frames
			 current_proc->a11 = animptr

 Returns: None

 Description:	animate through a list of multipart frames for the other guy
******************************************************************************/
void mframew_him(WORD sleeptime)
{
	ADDRESS animtemp;
	OBJECT *obj=current_proc->pa8;

	animtemp=(ADDRESS)current_proc->pa9;
	(ADDRESS)current_proc->pa9=current_proc->a11;
	current_proc->pa8=current_proc->pdata.p_otherguy;
	mframew(sleeptime);
	current_proc->pa8=obj;
	current_proc->a11=(ADDRESS)current_proc->pa9;
	(ADDRESS)current_proc->pa9=animtemp;

	return;
}

/******************************************************************************
 Function: void ground_multi(OBJECT *obj)

 By: David Schwartz

 Date: Jan 1995

 Parameters: obj - 1st object on list

 Returns: None

 Description:   plant a dudes feet on da ground
******************************************************************************/
void ground_multi(OBJECT *obj)
{
	long pos;

	pos=ground_y-lowest_mpart(obj);			// lowest multipart obj

	obj->oypos.u.intpos+=pos;		// adjust each object
	return;
}

/******************************************************************************
 Function: void multi_adjust_xy(OBJECT *obj,short xadj,short yadj)

 By: David Schwartz

 Date: Nov 1994

 Parameters: obj - dude to adjust
 			 xadj - amount to adjust him in x
			 xadj - amount to adjust him in x

 Returns: None

 Description: 	adjust the dudes animation pt
******************************************************************************/
void multi_adjust_xy(OBJECT *obj,short xadj,short yadj)
{
	if (obj->oflags & M_FLIPH)
		obj->oxpos.u.intpos-=xadj;
	else obj->oxpos.u.intpos+=xadj;

	obj->oypos.u.intpos+=yadj;

	return;
}

/******************************************************************************
 Function: void match_ani_points(OBJECT *pa8,OBJECT *pa0)

 By: David Schwartz

 Date: Nov 1994

 Parameters: pa8 - object ot match with (a8)
			 pa0 - object with changes (a0)

 Returns: None

 Description - set a multipart object's animation x to match
               another object's ani x,y
******************************************************************************/
void match_ani_points(OBJECT *pa8,OBJECT *pa0)
{
	OIMGTBL *iptr;

	pa0->oxpos.pos=pa8->oxpos.pos;
	pa0->oypos.pos=pa8->oypos.pos;

	if (pa0->oflags2 & M_MULTIPART)
	{
		/* multipart */
		if ((pa8->oflags & M_FLIPH)!=(pa0->oflags & M_FLIPH))
			flip_multi(pa0);
	}
	else
	{
		/* match_single */
		if ((pa8->oflags & M_FLIPH) != (pa0->oflags & M_FLIPH))		// same direction ?
			pa0->oflags^=M_FLIPH;			// reverse flip bit

		/* maps1 */
		iptr=(OIMGTBL *)pa0->oimg;

		if (pa0->oflags & M_FLIPH)
			pa0->oxpos.u.intpos-=(iptr->size.u.xpos-(-iptr->anixoff));
		else pa0->oxpos.u.intpos-=iptr->anixoff;

		if (pa0->oflags & M_FLIPV)
			pa0->oypos.u.intpos-=iptr->aniyoff;
		else pa0->oypos.u.intpos+=iptr->aniyoff;
	}

	return;
}

/******************************************************************************
 Function: int highest_mpart(OBJECT *obj)

 By: David Schwartz

 Date: Sept 1994

 Parameters: obj - object to find highest point

 Returns: coordinate of highest point (lowest value)

 Description: 	finds the object on a multipart object thats highest off the ground (lowest value)
******************************************************************************/
int highest_mpart(OBJECT *obj)
{
	int highest=0x7fff;
	P_RAM *pramptr;
	short ypos;


	pramptr=(P_RAM *)obj->oimg;
	ypos=obj->oypos.u.intpos;

#if !MULTI_OFF
	while (pramptr->prsagptr != 0)
	{
#endif

		if ((ypos-pramptr->prpos.u.ypos)<highest)
			highest=ypos-pramptr->prpos.u.ypos;

#if !MULTI_OFF
		pramptr++;
	}
#endif

	return(highest);
}

/******************************************************************************
 Function: int lowest_mpart(OBJECT *obj)

 By: David Schwartz

 Date: Sept 1994

 Parameters: obj - object to find lowest point

 Returns: coordinate of lowest point (highest value)

 Description: 	finds the object on a multipart object thats lowest off the ground (highest value)
******************************************************************************/
int lowest_mpart(OBJECT *obj)
{
	int lowest=0;
	P_RAM *pramptr;
	short ypos;

	pramptr=(P_RAM *)obj->oimg;
	ypos=obj->oypos.u.intpos;

#if !MULTI_OFF
	while (pramptr->prsagptr != 0)
	{
#endif

		if ((ypos-pramptr->prpos.u.ypos+pramptr->prsize.u.ypos)>lowest)
			lowest=ypos-pramptr->prpos.u.ypos+pramptr->prsize.u.ypos;

#if !MULTI_OFF
		pramptr++;
	}
#endif

	return(lowest);
}

/******************************************************************************
 Function: int leftmost_mpart(OBJECT *obj)

 By: David Schwartz

 Date: Sept 1994

 Parameters: obj - object to find left pt

 Returns: leftmost coordinate

 Description: 	finds the object on a multipart object thats the leftmost (smallest value)
******************************************************************************/
int leftmost_mpart(OBJECT *obj)
{
	int leftmost=0x7fff;
	P_RAM *pramptr;
	short xpos;

	pramptr=(P_RAM *)obj->oimg;
	xpos=obj->oxpos.u.intpos;

#if !MULTI_OFF
	while (pramptr->prsagptr != 0)
	{
#endif

		if ((xpos-pramptr->prpos.u.xpos)<leftmost)
			leftmost=xpos-pramptr->prpos.u.xpos;

#if !MULTI_OFF
		pramptr++;
	}
#endif

	return(leftmost);
}

/******************************************************************************
 Function: int rightmost_mpart(OBJECT *obj)

 By: David Schwartz

 Date: Sept 1994

 Parameters: obj - object to find right point

 Returns: rightmost coordinate

 Description: 	finds the object on a multipart object the rightmost (biggest x)
******************************************************************************/
int rightmost_mpart(OBJECT *obj)
{
	int rightmost=-5000;
	P_RAM *pramptr;
	short xpos;


	pramptr=(P_RAM *)obj->oimg;
	xpos=obj->oxpos.u.intpos;

#if !MULTI_OFF
	while (pramptr->prsagptr != 0)
	{
#endif

		if ((xpos-pramptr->prpos.u.xpos+pramptr->prsize.u.xpos)>rightmost)
			rightmost=xpos-pramptr->prpos.u.xpos+pramptr->prsize.u.xpos;

#if !MULTI_OFF
		pramptr++;
	}
#endif

	return(rightmost);
}

/******************************************************************************
 Function: void backwards_ani(WORD sleeptime,WORD animnum)

 By: David Schwartz

 Date: Sept 1994

 Parameters: sleeptime - time to sleep between frames
			 animnum - animation number to traverse (ANIM_TABLE1)

 Returns: None

 Description:	start animation & animate backwards through a list of multipart frames
******************************************************************************/
void backwards_ani(WORD sleeptime,WORD animnum)
{
	if ((short)animnum>=0)
		get_char_ani(ANIM_TABLE1,animnum);
	bani2(current_proc->pa8,sleeptime);
}

/******************************************************************************
 Function: void backwards_ani2(WORD sleeptime,WORD animnum)

 By: David Schwartz

 Date: Sept 1994

 Parameters: sleeptime - time to sleep between frames (a0)
			 animnum - animation number to traverse (ANIM_TABLE2) (a9)

 Returns: None

 Description:	start animation & animate backwards through a list of multipart frames
******************************************************************************/
void backwards_ani2(WORD sleeptime,WORD animnum)
{
	if ((short)animnum>=0)
		get_char_ani(ANIM_TABLE2,animnum);
	bani2(current_proc->pa8,sleeptime);
}

/******************************************************************************
 Function: void bani2(OBJECT *obj,WORD sleeptime)

 By: David Schwartz

 Date: Sept 1994

 Parameters: sleeptime - time to sleep between frames

 Returns: None

 Description:	animate backwards through a list of multipart frames
******************************************************************************/
void bani2(OBJECT *obj,WORD sleeptime)
{
	(ADDRESS *)current_proc->a10=(ADDRESS *)current_proc->pa9;

	/* get to end of list */
	while (GET_LONG(current_proc->pa9) !=0)
		((ADDRESS *)current_proc->pa9)++;

	/* animate backwards */
	do
	{
		--((LONG *)current_proc->pa9);
		animate2(obj,(ADDRESS *)COMPUTE_ADDR(current_proc->pa8->oheap,GET_LONG(current_proc->pa9)));
		process_sleep(sleeptime);
	}
	while((ADDRESS *)current_proc->pa9 != (ADDRESS *)current_proc->a10);
}

/******************************************************************************
 Function: void animate_a0_frames(WORD sleep,WORD frames)

 By: David Schwartz

 Date: Sept 1994

 Parameters: sleep - time between animation updates (a0[31..16])
			 frames - # of frames to animate through (a0[15..0])

 Returns: None

 Description:	walks an animation script according to sleep and frames
******************************************************************************/
void animate_a0_frames(WORD sleep,WORD frames)
{
	do
	{
		if (do_next_a9_frame(current_proc->pa8)==SYSTEM_ZERO_SET)
			break;

		process_sleep(sleep);
	}
	while(--frames > 0);
}

/******************************************************************************
 Function: void init_anirate(WORD anispeed)

 By: David Schwartz

 Date: Sept 1994

 Parameters: anispeed - animation speed

 Returns: None

 Description: 	init p_anirate and zero p_anicount so we will animate
				on first call of next_anirate
******************************************************************************/
void init_anirate(WORD anispeed)
{
	current_proc->pdata.p_anirate=anispeed;			// set speed

	current_proc->pdata.p_anicount=(anispeed != NEVER_ANI) ? 1: anispeed;				// first call=animate, or never

	return;
}

/******************************************************************************
 Function: void next_anirate(void)

 By: David Schwartz

 Date: Sept 1994

 Parameters: current_proc->pa9 - animation frames to use
			 current_proc->pdata.p_anitab - base animation table

 Returns: None

 Description:	animate according to p_anirate value
******************************************************************************/
void next_anirate(void)
{
	OBJECT *obj=current_proc->pa8;

	if (--current_proc->pdata.p_anicount==0) 				// time to animate ??
	{
		current_proc->pdata.p_anicount=current_proc->pdata.p_anirate;	// reload count

		if (GET_LONG(current_proc->pa9) !=ANI_END)
		{
			if (obj->oflags2 & M_MULTIPART)
				do_next_a9_frame(obj);
			else frame_a9(obj);
		}
	}
}

/******************************************************************************
 Function: WORD frame_a9(OBJECT *obj)

 By: David Schwartz

 Date: Sept 1994

 Parameters: current_proc->pa9 - frame list entry
			 obj - object to use

 Returns: SYSTEM_CARRY_SET - end of list reached
		  SYSTEM_CARRY_CLEAR - end of list not reached

 Description:	single object frame animator
******************************************************************************/
//-NOTE- THIS ROUTINE WAS OPTIMIZED, ASSUMES THAT FRAME1 CODE WAS NEVER EXECUTED!!
//		ROUTINE WILL ERROR IF FRAME 1 CODE IS SUPPOSE TO EXECUTE
WORD frame_a9(OBJECT *obj)
{
	LONG animframe;
	WORD newflags;

	newflags=obj->oflags;									// get flags

	animframe=GET_LONG(current_proc->pa9)++;				// get a table entry

	/* -NOTE-, MODIFICATION TO ALLOW SWITCHING OF PALETTES */
	if (animframe==ANI_SWPAL)
	{
		animframe=GET_LONG(current_proc->pa9);		// grab img which has pal
		animframe=(LONG)((ADDRESS*)COMPUTE_ADDR(obj->oheap,animframe));
		((OIMGTBL *)animframe)++;
		swpal((void *)(GET_LONG(animframe)),obj);

		animframe=GET_LONG(current_proc->pa9)++;		// grab img to display

	}

	if (animframe>ANI_LASTCOM)
	{
		animate(SINGLE_ANI,(OIMGTBL *)COMPUTE_ADDR(obj->oheap,animframe),newflags);

		/* quick exit check */
		if ((animframe=GET_LONG(current_proc->pa9))>ANI_LASTCOM)
			return(SYSTEM_CARRY_CLR);
	}

	/* frame1 error check */
	if (animframe==ANI_FLIP)
		ermac(13);

	/* frame4 */
	if (animframe==ANI_END)								// end of list reached?
		return(SYSTEM_CARRY_SET);

	if (animframe==ANI_JUMP)							// jump command so redirect
		(ADDRESS *)current_proc->pa9=(ADDRESS *)COMPUTE_ADDR(obj->oheap,GET_LONG(((LONG *)current_proc->pa9+1)));	// get new animation ptr

	return(SYSTEM_CARRY_CLR);
}

/******************************************************************************
 Function: void flip_multi(OBJECT *obj)

 By: David Schwartz

 Date: Sept 1994

 Parameters: obj - 1st object on list

 Returns: None

 Description:	flip a multipart object
******************************************************************************/
void flip_multi(OBJECT *obj)
{
	obj->oflags ^=M_FLIPH;					// reverse flip bit
	animate2(obj,obj->oshape);

	return;
}

/******************************************************************************
 Function: void flip_multi_v(OBJECT *obj)

 By: David Schwartz

 Date: Sept 1994

 Parameters: obj - 1st object on list

 Returns: None

 Description:	flip a multipart object vertically
******************************************************************************/
void flip_multi_v(OBJECT *obj)
{
	obj->oflags ^=M_FLIPV;					// reverse flip bit
	animate2(obj,obj->oshape);

	return;
}

/******************************************************************************
 Function: WORD double_next_a9(void)

 By: David Schwartz

 Date: Nov 1994

 Parameters: current_proc->pa8 - object #1
			 current_proc->pa9 - ani #1
			 current_proc->a10 - object #2
			 current_proc->a11 - ani #2


 Returns: SYSTEM_CARRY_SET --> skip sleep flag
		  SYSTEM_CARRY_CLR --> don't skip sleep flag
		  SYSTEM_ZERO_SET --> end of animation reached!!
		  SYSTEM_ZERO_CLR --> not end of animation reached!!

 Description:	double object animation routine
******************************************************************************/
WORD double_next_a9(void)
{
	ADDRESS animtemp,animframe;
	PROCESS *ptemp;
	WORD results;

	do_next_a9_frame(current_proc->pa8);

	/* load up second guy */
	animtemp=current_proc->a11;
	ptemp=current_proc;
	current_proc=current_proc->pdata.p_otherproc;
	animframe=(ADDRESS)current_proc->pa9;

	(ADDRESS)current_proc->pa9=animtemp;
	results=do_next_a9_frame((OBJECT *)ptemp->a10);
	animtemp=(ADDRESS)current_proc->pa9;

	/* restore stuff */
	(ADDRESS)current_proc->pa9=animframe;
	current_proc=ptemp;
	current_proc->a11=animtemp;

	return(results);
}

/******************************************************************************
 Function: void ganiof(OIMGTBL *img,WORD flags)

 By: David Schwartz

 Date: Sept 1994

 Parameters:  img - img data
			  flags - flag info

 Returns: current_proc->a6 = ani x off * 0x10000
		  current_proc->a7 = ani y off * 0x10000

 Description:	get animation offset
******************************************************************************/
void ganiof(OIMGTBL *img,WORD flags)
{
	switch ((flags & (M_FLIPH+M_FLIPV))>>4)
	{
		case 0:		// no flips
			(long)current_proc->a6=(img->anixoff)<<16;
			(long)current_proc->a7=(img->aniyoff)<<16;
			break;
		case 1:		// h flip
			(long)current_proc->a6=(img->size.u.xpos-img->anixoff-1)<<16;
			(long)current_proc->a7=(img->aniyoff)<<16;
			break;
		case 2:		// v flip
			(long)current_proc->a6=(img->anixoff)<<16;
			(long)current_proc->a7=(img->size.u.ypos-img->aniyoff-1)<<16;
			break;
		case 3:		// h & v flip
			(long)current_proc->a6=(img->size.u.xpos-img->anixoff-1)<<16;
			(long)current_proc->a7=(img->size.u.ypos-img->aniyoff-1)<<16;
			break;
	}
}

/******************************************************************************
 Function: void flip_single(OBJECT *obj)

 By: David Schwartz

 Date: Dec 1994

 Parameters:  obj - object

 Returns: None

 Description:  flip a single part object routine
******************************************************************************/
void flip_single(OBJECT *obj)
{
	obj->oflags^=M_FLIPH;
	animate(SINGLE_ANI0,obj->oimg,obj->oflags);
	return;
}

/******************************************************************************
 Function: void framew(WORD anisleep)

 By: David Schwartz

 Date: Dec 1994

 Parameters:  anisleep - sleep time between each frame

 Returns: None

 Description:  walk through a table of frames sleeping the same
******************************************************************************/
void framew(WORD anisleep)
{
	OBJECT *obj=current_proc->pa8;

FWLOOP:
	if (frame_a9(obj)==SYSTEM_CARRY_SET)
		goto FWDONE;
	process_sleep(anisleep);
	goto FWLOOP;

FWDONE:
	((ADDRESS*)current_proc->pa9)++;
	process_sleep(anisleep);

	return;
}

/******************************************************************************
 Function: void animate(WORD aniindex,OIMGTBL *frame,WORD newflag)

 By: David Schwartz

 Date: Sept 1994

 Parameters: aniindex - index to what routine to run
 			 frame - ptr to frame data (a1)
			 newflag - new flags to set for image  (a4)

 Returns: None

 Description:	single object animation subroutine
******************************************************************************/
void animate(WORD aniindex,OIMGTBL *frame,WORD newflags)
{
	WORD oldflags;
	OIMGTBL *oldimg;
	long oldx,oldy;
	OBJECT *obj=current_proc->pa8;
	ADDRESS ta6,ta7;

	ta7=current_proc->a7;			// preserve values
	ta6=current_proc->a6;


	oldflags=obj->oflags;					// a5
	oldimg=(OIMGTBL *)obj->oimg;			// a3

	/* only check for same frame is index is set correctly */
	if (aniindex==SINGLE_ANI && oldimg==frame && newflags==oldflags)
		return;

	/* anig1 */
	ganiof(oldimg,oldflags);			// get old animation offset (a6,a7)
	oldx=(long)current_proc->a6;
	oldy=(long)current_proc->a7;

	gsagof(frame,obj->osize.yx,obj->osag);				// set size, sag ptr
	ganiof(frame,newflags);

	/* gsagof sets osize, osag */
	obj->oflags=newflags;

	(OIMGTBL *)obj->oimg=frame;

	obj->oxpos.pos+=(oldx-(long)current_proc->a6);	// adjust for new pos
	obj->oypos.pos+=(oldy-(long)current_proc->a7);	// adjust for new pos

	/* vcache mem manager */
	alloc_cache(frame,obj->oheap,obj);

	/* anigx */
	current_proc->a7=ta7;			// restore values
	current_proc->a6=ta6;

	return;
}
