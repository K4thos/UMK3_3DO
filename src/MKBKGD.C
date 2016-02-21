/******************************************************************************
 File: mkbkgd.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Background Routines
******************************************************************************/

#define VERIFY_HACK 0			// 0-hack on 1-hack off

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
#include "mkfx.h"
#include "mkani.h"
#include "mkguys.h"
#include "mkinit.h"
#include "mkrepell.h"
#include "mkscore.h"
#include "mkmain.h"
#include "mkpal.h"
#include "mkfile.h"
#include "mksound.h"
#include "psxspu.h"
#include "valloc.h"


typedef struct stable
{
	long scrl_value;
    void (*scrl_func)(WORD);                            /* void function(WORD) */
} STABLE;

static STABLE scrolls_table[] =
{
	{0x10000,ss_10000},
	{0x20000,ss_20000},
	{0x28000,ss_28000},
	{0x30000,ss_30000},
	{0x40000,ss_40000},
	{0x60000,ss_60000},
	{0x70000,ss_70000},
	{0x80000,ss_80000},
	{0xa0000,ss_a0000},
	{0x00000,NULL}
};

/******************************************************************************
 Function: void do_background(WORD background)

 By: David Schwartz

 Date: Aug 23, 1994

 Parameters: background = background module to initiailze

 Returns: None

 Description:   This routine will do the follows:
	1)      Create the background process
	2)      Load the background data into VRAM
	3)      Setup all neccessary background structures/tables/variables
******************************************************************************/
void do_background(WORD background_mod)
{
	load_bkgd=background_mod;                       // set correct load background module #

	init_background_module(table_o_mods[load_bkgd]);

	process_sleep(1);

	CREATE(PID_BACKG,background_routine);
	return;
}

/******************************************************************************
 Function: void background_clear(void)

 By: David Schwartz

 Date: Aug 23, 1994

 Parameters: None

 Returns: None

 Description:   This routine does the following:
				1) clears background display lists
				2) sets background modules to dumb modules
				3) clears all world x positions
				4) stops all scrolling
******************************************************************************/
void background_clear(void)
{
	int l;

	/* clear background lists */
	baklst1=baklst2=baklst3=baklst4=NULL;
	baklst4=baklst5=baklst6=baklst7=NULL;
	baklst8=NULL;

	/* set background modules */
	for (l=0;l<(sizeof(bakmods)/sizeof(bakmods[0]));l++)
		bakmods[l]=(ADDRESS *)DUMB_BACKGROUND;

	/* clear background bits */
	for (l=0;l<BACK_BIT_SIZE/BACK_BIT_RESOLUTION;l++)
		bakbit1[l]=bakbit2[l]=bakbit3[l]=bakbit4[l]=bakbit5[l]=bakbit6[l]=bakbit7[l]=bakbit8[l]=0;

	/* set_all_worldtlx */
	worldtlx1.pos=worldtlx.pos=worldtlx2.pos=worldtlx3.pos=worldtlx4.pos=0;
	worldtlx5.pos=worldtlx6.pos=worldtlx7.pos=worldtlx8.pos=0;

	/* stop scrolling */
	stop_scrolling();

	return;
}

/* Format:
	WORD    TPAGE X START,# of DYN TPAGE
	WORD    x,y dyn tpage
	WORD    x,y dyn tpage
	WORD    x,y dyn tpage
*/

WORD bkgd_vmem_list[] =
{
	BACK_TEXTURE_BASE_X,3,
	0,256,
	128,256,
	256,256
};

WORD mock_vmem_list[] =
{
	BACK_TEXTURE_BASE_X+128,4,
	0,256,
	128,256,
	256,256,
	256+128,256
};

WORD vs_vmem_list[] =
{
	BACK_TEXTURE_BASE_X+128+128,5,
	0,256,
	128,256,
	256,256,
	256+128,256,
	256+128+128,256
};

#if 0
WORD sel_vmem_list[] =
{
	BACK_TEXTURE_BASE_X-128,2,
	0,256,
	128,256
};
#endif

/******************************************************************************
 Function: void sony_graphics_setup(void)

 By: David Schwartz

 Date: Apr 1995

 Parameters: None

 Returns: None

 Description:   This routine sets up the vcache system and loads the correct level background data
******************************************************************************/
void sony_graphics_setup(void)
{
	/* load in if memory isn't already setup with correct data */
	if (table_o_levels[load_bkgd]!=load_level)
	{
		/* load code and module data associated with level */
		level_overlay_load(table_o_levels[load_bkgd]);
		f_street=0;
	}

		/* SONY ROUTINE, START VRAM CACHE */
		switch (load_bkgd)
		{
#if 0
			case BKGD_MK3_PSEL:
				bkgd_base_x=sel_vmem_list[0];
				bkgd_base_tpage=GetTPage(TEXTURE_MODE,TRANS_RATE,bkgd_base_x,BACK_TEXTURE_BASE_Y);
				init_vram_cache(&sel_vmem_list[1]);                                                     // reset vram cache
				break;
#endif
			case BKGD_VS_MOD:
				bkgd_base_x=vs_vmem_list[0];
				bkgd_base_tpage=GetTPage(TEXTURE_MODE,TRANS_RATE,bkgd_base_x,BACK_TEXTURE_BASE_Y);
				init_vram_cache(&vs_vmem_list[1]);                                                      // reset vram cache
				break;
			case BKGD_MOCKPIT_MOD:
				bkgd_base_x=mock_vmem_list[0];
				bkgd_base_tpage=GetTPage(TEXTURE_MODE,TRANS_RATE,bkgd_base_x,BACK_TEXTURE_BASE_Y);
				init_vram_cache(&mock_vmem_list[1]);                                                    // reset vram cache
				break;
			default:
				bkgd_base_x=bkgd_vmem_list[0];
				bkgd_base_tpage=GetTPage(TEXTURE_MODE,TRANS_RATE,bkgd_base_x,BACK_TEXTURE_BASE_Y);
				init_vram_cache(&bkgd_vmem_list[1]);                                                    // reset vram cache
				break;
		}

		/* load control panel graphics names */
		load_control_page();
}

/******************************************************************************
 Function: void init_background_module(void *)

 By: David Schwartz

 Date: Aug 23, 1994

 Parameters: ptr to module to init

 Returns: None

 Description:   This routine initializes the background module and associated data
				(init_bakmods)
******************************************************************************/
void init_background_module(void *module_ptr)
{
	WORD rgb;
	WORD section_flag;
	LONG temp;
//	WORD clt_mode,clt_size;
	POS *world_ptr;
	FUNC init_func;
	ADDRESS **bakmod_ptr;
//	void *text_tbl;

	/* setup ram with correct code/modules/overlays etc */
	sony_graphics_setup();

	bakbit1[0]=bakbit2[0]=bakbit3[0]=bakbit4[0]=0;
	bakbit5[0]=bakbit6[0]=bakbit7[0]=bakbit8[0]=0;

	/* word #1 = background color */
	rgb=GET_WORD(module_ptr)++;                             /* get background color*/
	if (rgb != NULL_IRQSKYE)
		irqskye=rgb;

	/* word #2 = worldtly/ceiling_y */
	ceiling_y=GET_WORD(module_ptr)++;                       /* set ceiling y position */

	worldtly.pos=(LONG)ceiling_y<<16;

	/* word #3 = ground y offset */
	ground_y=(GET_WORD(module_ptr)++)+ceiling_y;                    /* set ground y position */

	/* word #4 = world x */
	temp=GET_WORD(module_ptr)++;

	if ((short)temp!=CENTER_X)      // this bgnd centered, if so skip
	{
		(long)temp=(long)temp<<16;

		/* set_all_worldtlx */
		worldtlx1.pos=worldtlx.pos=worldtlx2.pos=worldtlx3.pos=worldtlx4.pos=temp;
		worldtlx5.pos=worldtlx6.pos=worldtlx7.pos=worldtlx8.pos=temp;
	}

IBAK2:
	/* word 5 & 6 = scroll limits*/
	left_edge=GET_WORD(module_ptr)++;
	right_edge=GET_WORD(module_ptr)++;

	/* word 7 & 8 = call routine */
	init_func=(FUNC)(GET_LONG(module_ptr)++);               /* setup call routines */
	init_func();

	/* word 9 & 10 = scroll table */
	scrtab=(long *)(GET_LONG(module_ptr)++);                /* set scroll table */

	/* word 11 & 12 = display lists */
	dlists=(void *)(GET_LONG(module_ptr)++);                /* set display lists */

	bakmod_ptr=(ADDRESS **)(GET_LONG(module_ptr)++);        /* starting bak?mod to fill */
	section_flag=0;                                                                 /* start with load mods(0), then center x(1) */

	while (TRUE)
	{
IBAK3:
		temp=GET_LONG(module_ptr)++;

		/* main section */
		if (!section_flag)
		{
			if (temp==FORCE_EXIT)                                   /* abort loop */
				break;

			switch (temp)
			{
				case CENTER_X:                                          /* time to center planes */
					section_flag++;
					break;
				case SKIP_BAKMOD:                                       /* skip background plane */
					*bakmod_ptr++=(ADDRESS *)temp;
					break;
				default:
					*bakmod_ptr++=((ADDRESS *)module_ptr)-1;                        /* store ptr to background plane information (module ptr, x,y)*/
					((LONG *)module_ptr)++;                                         /* skip past x,y init pos */
					break;
			}
		}
		/* center x section */
		else
		{
			if (temp==END_LIST)
				break;

			world_ptr=(POS *)(GET_LONG(module_ptr)++);      /* get ptr to worldtlx var */

			world_ptr->pos= ((((OMODULE *)temp)->xsize-SCRRGT)/2)<<16;      /* calc middle of screen */
		}
	}

IBAK5:
	/* floor stuff */
	if ((temp=GET_LONG(module_ptr))!=0)
		setup_floor_info(module_ptr);

	/* load in module TEXTURE data, SONY ADDITION */
	bkgd_texture_load(bkgd_base_x,BACK_TEXTURE_BASE_Y);

	return;
}

/******************************************************************************
 Function: void bkgd_texture_load(WORD tsx,WORD tsy)

 By: David Schwartz

 Date: Dec 1994

 Parameters: tsx - start x position in frame buffer
			 tsy - start y position in frame buffer
			 text_tbl_size - # of entires in text_tbl
			 text_tbl - list of ptr to address of texture data

 Returns: None

 Description:   Load a list of 256 x 256 (8 bit) textures into the frame starting at tsx,tsy.
				The routine will only load if the current level loaded does not match the load_bkgd level
******************************************************************************/
void bkgd_texture_load(WORD tsx,WORD tsy)
{
	WORD results;
	LONG entry;

	if (table_o_levels[load_bkgd]!=load_level && table_o_levels[load_bkgd]!=LVL_NONE)
	{
		/* set new level & load data into VRAM */
		load_level=table_o_levels[load_bkgd];

#if VERIFY_HACK
		if ( !bkgd_preload )
			results=texture_level_load(load_level,SYNC_LOAD);
		else results=0;

		if (results==0)
			vram_texture_load(tsx,tsy,(ADDRESS *)p2_heap);
#else
HACK:
		if ( !bkgd_preload )
			results=texture_level_load(load_level,SYNC_LOAD);
		else results=0;

		if ( results==0 )
		{
			entry=GET_LONG(p2_heap);
			if (entry>7)
			{
				bkgd_preload=0;
				goto HACK;
			}

			vram_texture_load(tsx,tsy,(ADDRESS *)p2_heap);
		}

#endif
		bkgd_preload=0;
	}
	else
	{
		load_level=table_o_levels[load_bkgd];
	}

	return;
}

/******************************************************************************
 Function: void vram_texture_load(WORD tsx,WORD tsy,ADDRESS *text_tbl)

 By: David Schwartz

 Date: Dec 1994

 Parameters: tsx - start x position in frame buffer
						 tsy - start y position in frame buffer
						 text_tbl - list of ptr to address of texture data

 Returns: None

 Description:   Load a list of 256 x 256 (8 bit) textures into the frame starting at tsx,tsy.
				The routine will only load if the current level loaded does not match the curback's level
******************************************************************************/
void vram_texture_load(WORD tsx,WORD tsy,ADDRESS *text_tbl)
{
	WORD l;
	RECT frame;
	void *text_info;
	LONG text_tbl_size;
	void *text_heap=text_tbl;
	void *idcomp_temp=idcomp_ptr;                                   // save value

	/* set new level & load data */
	load_level=table_o_levels[load_bkgd];

	/* setup start of data info */
	text_tbl_size=GET_LONG(text_tbl)++;                                     // # of tpages to load

	/* load all of the tpages into vram memory */
	frame.w=128;
	frame.h=256;
	frame.x=tsx;
	frame.y=tsy;
	for (l=0;l<text_tbl_size;l++)
	{
		(ADDRESS *)text_info=GET_LONG(text_tbl)++;                      /* get ptr to textyre data */
		text_info=COMPUTE_ADDR(text_heap,(ADDRESS *)text_info);         // compute abs addr

#if BACKGROUND_COMPRESS
		(BYTE *)idcomp_ptr=(l & 1)?(BYTE *)(&player_heap[PLAYER_HEAP_SIZE]-75*1024):(BYTE *)(&player_heap[PLAYER_HEAP_SIZE]-75*1024*2); // use last 75K Pages
		text_info=(void*)uncompress_image((BYTE *)text_info);
#endif
		LOADIMAGE(&frame,text_info);                            /* load data */

		frame.x +=128;                                                  /* move to next texture pos */

#if BACKGROUND_COMPRESS
		DrawSync(0);                                            // make sure we dont overwrite
#endif
	}

	DrawSync(0);                            // make sure everything gets loaded before we allow heap ram to get trashed

#if BACKGROUND_COMPRESS
	idcomp_ptr=idcomp_temp;
#endif
	return;
}

/******************************************************************************
 Function: void setup_floor_info(void *modptr)

 By: David Schwartz

 Date: Nov 1994

 Parameters: None

 Returns: None

 Description:   setup floor info parameters
******************************************************************************/
void unbetterSimpleRLE(BYTE *src, BYTE **dst);
extern BYTE *subflor_addr;
extern BYTE *hellflor_addr;
void setup_floor_info(void *modptr)
{
	BYTE *srcfloor;
	WORD *temp;

	/* #1 - sag */
	srcfloor=(BYTE *)GET_LONG(modptr)++;                    // centered

	if ( load_bkgd==BKGD_STREET_MOD)                                        // hack to make floor space less for SBST module
	{
		if ( f_street==0 )
		{		
			temp=(WORD*)subflor_addr;
			unbetterSimpleRLE(srcfloor,&subflor_addr);
			subflor_addr=srcfloor=(BYTE*)temp;
			f_street=1;
		}
		else
		{
			srcfloor=subflor_addr;
		}
	}

#if 0
	if ( load_bkgd==BKGD_CAVE_MOD)                                        // hack to make floor space less for SBST module
	{
		if ( f_street==0 )
		{		
			temp=(WORD*)hellflor_addr;
			unbetterSimpleRLE(srcfloor,&hellflor_addr);
			hellflor_addr=srcfloor=(BYTE*)temp;
			f_street=1;
		}
		else
		{
			srcfloor=hellflor_addr;
		}
	}
#endif

	/* #2 - set const:pal */
	(LONG)temp=GET_LONG(modptr)++;
	skew_constpal=get_fore_pal(temp);

	skew_oc=0;                                                                                              // off center = 0

	/* #3 - height of floor */
	skew_height=GET_LONG(modptr)++;                                                 // floor height

	skew_y=worldtly.u.intpos+(BUFFER_HEIGHT-10)-skew_height;        // align floor with bottom of screen

	/* #4 - scroll x to use */
	skew_scroll=(ADDRESS *)(GET_LONG(modptr)++);

	/* #5 - scroll call routine */
	skew_calla=(FLRCALL)GET_LONG(modptr)++;

	/* transfer floor into src buffer */
	//memcpy(floor_src_buffer,srcfloor,FLOOR_X*FLOOR_Y);            // copy memory and set skew address
	//skew_sag=floor_src_buffer+(FLOOR_X/3);                                        // center 1 pixels/byte, ( 3 scrns wide so center is screen 1)
	skew_sag=srcfloor+(FLOOR_X/3);                                  // center 1 pixels/byte, ( 3 scrns wide so center is screen 1)


	f_skew=1;

	return;
}

/******************************************************************************
 Function: void use_next_y(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: update disp_world.ypos
		  update dlists_ptr

 Description:   This routines updates the y scroll value during display via dlists_ptr info
******************************************************************************/
void use_next_y(void)
{
	POS *world_ptr;

	world_ptr=(POS *)GET_LONG(dlists_ptr)++;

	disp_world.u.ypos=world_ptr->u.intpos;

	return;
}

/******************************************************************************
 Function: void use_worldtly(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: update disp_world.ypos

 Description:   This routines updates the y scroll value based on the worldtly
******************************************************************************/
void use_worldtly(void)
{
	disp_world.u.ypos=worldtly.u.intpos;

	return;
}

/******************************************************************************
 Function: void use_worldtly_t(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: update disp_world.ypos

 Description:   This routines updates the y scroll value based on the worldtly_t
******************************************************************************/
void use_worldtly_t(void)
{
	if (f_pause<PAUSE_OFF || f_pause==PAUSE_P1_INIT || f_pause==PAUSE_P2_INIT)
		worldtly_t.u.intpos+=scrolly.u.intpos;          // do y scrolling for temp planes

	disp_world.u.ypos=worldtly_t.u.intpos;

	return;
}

/******************************************************************************
 Function: void check_only_t(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns:  update dlists_ptr if needed

 Description:   check to see if we use temp lists only
******************************************************************************/
void check_only_t(void)
{
	if (f_only_t!=0)
	dlists_ptr=dlists_end;

	return;
}

/******************************************************************************
 Function: void background_routine(void)

 By: David Schwartz

 Date: Aug 23, 1994

 Parameters: None

 Returns: None

 Description:   This routine updates the backgrounds
******************************************************************************/
void background_routine(void)
{
	current_proc->a10=BACKGROUND_SLEEP_TIME;
	while (TRUE)
	{
		multi_plane();
		process_sleep(current_proc->a10);
	};
}

/******************************************************************************
 Function: void multi_plane(void)

 By: David Schwartz

 Date: Aug 23, 1994

 Parameters: None

 Returns: None

 Description:   This routine processes the multiple planes
******************************************************************************/
void multi_plane(void)
{
	static PLANETBL plane_table_info[] =
	{
		{&bakbit1[0],&baklst1,&worldtly,&worldtlx1,&bak1mods},
		{&bakbit2[0],&baklst2,&worldtly,&worldtlx2,&bak2mods},
		{&bakbit3[0],&baklst3,&worldtly,&worldtlx3,&bak3mods},
		{&bakbit4[0],&baklst4,&worldtly,&worldtlx4,&bak4mods},
		{&bakbit5[0],&baklst5,&worldtly,&worldtlx5,&bak5mods},
		{&bakbit6[0],&baklst6,&worldtly,&worldtlx6,&bak6mods},
		{&bakbit7[0],&baklst7,&worldtly,&worldtlx7,&bak7mods},
		{&bakbit8[0],&baklst8,&worldtly,&worldtlx8,&bak8mods}
	};

	int l;
	PLANETBL *plane;
	LONG bmod;
	XYTYPE disptl,displr;
//	OMODULE *mod_ptr;
//	XYTYPE mod_xysize;

	for (l=0;l<(sizeof(plane_table_info)/sizeof(plane_table_info[0]));l++)
	{
		plane=&plane_table_info[l];
		bmod=(LONG)(*((LONG *)plane->mod_ptr));

		if (bmod==DUMB_BACKGROUND)                      /* if DUMB BACKGROUND , end list processing */
			break;

		if (bmod != SKIP_BAKMOD)                        /* process plane check */
		{
			/* execute bgnd_ud1 routine */
			disptl.u.xpos=displr.u.xpos=plane->world_x->u.intpos;
			disptl.u.ypos=displr.u.ypos=plane->world_y->u.intpos;

			disptl.u.xpos=disptl.u.xpos+scrntlx-DISPLAY_PADDING_X;
			disptl.u.ypos=disptl.u.ypos+scrntly-DISPLAY_PADDING_Y;

			displr.u.xpos=displr.u.xpos+scrnlrx+DISPLAY_PADDING_X;
			displr.u.ypos=displr.u.ypos+scrnlry+DISPLAY_PADDING_Y;

			/* remove objects that aren't on screen */
			display_delete(plane->baklst,&disptl,&displr);

			/* add any tiles that aren't on display list */
			display_modify(plane->baklst,(void *)bmod,plane->bakbit,&disptl,&displr);
		}
	}
	return;
}

/******************************************************************************
 Function: void display_delete(OBJECT **baklst,XYTYPE tl,XTYPE lr)

 By: David Schwartz

 Date: Aug 23, 1994

 Parameters: baklst - ptr to planes display list of objects
			 tl - top left y:x coord of view screen
			 lr - lower right y:x coord of view screen

 Returns: None

 Description:   This routine will scan through the current plane display list
				and remove any object's collision box that does not fall anywhere
				within the view screen.

******************************************************************************/
#if DEBUG
extern WORD objcnt;
#endif

void display_delete(OBJECT **baklst,XYTYPE *tl,XYTYPE *lr)
{
	OBJECT *current_obj;
	OBJECT *prev_obj;

	current_obj=(OBJECT *)baklst;                           /* set current to start of list */

	while (current_obj->olink != NULL)
	{
		prev_obj=current_obj;
		current_obj=current_obj->olink;

		if ((current_obj->oxpos.u.intpos>lr->u.xpos) ||
		   (current_obj->oypos.u.intpos>lr->u.ypos) ||
		   (current_obj->oxpos.u.intpos+current_obj->header.t_width<tl->u.xpos) ||
		   (current_obj->oypos.u.intpos+current_obj->header.t_height<tl->u.ypos))
		{
			*((LONG *)current_obj->oplink) &=~current_obj->oblockmask;      /* clear block bit, not in display list */

			if (current_obj->opal !=0)                                      /* free up palette that object uses */
				free_pal(current_obj->opal);

			prev_obj->olink=current_obj->olink;             /* remove object and reconnect node */

			current_obj->olink=obj_free;    /* add removed object to free list */
			obj_free=current_obj;

#if DEBUG
	objcnt--;
#endif

			current_obj=prev_obj;                   /* place current ptr to prev */
		}
	}
	return;
}

/******************************************************************************
 Function: void display_modify(OBJECT **baklst,void *bak_info,LONG *bakbit,XYTYPE *tl,XYTYPE *lr)

 By: David Schwartz

 Date: Aug, 1994

 Parameters: baklst - ptr to ptr to display list for plane
			 bak_info - ptr to module info and (xy start pos for module)
			 bakbit - ptr to array of longs (1 bit per object) on plane, indicates object already added
			 tl - top left corner of screen (Y:X)
			 lr - lower right corner of screen (Y:X)

 Returns: None

 Description:   Scan a module lists to find out which modules contain blocks
				which need to be checked for addition to the display list.
******************************************************************************/
void display_modify(OBJECT **baklst,void *bak_info,LONG *bakbit,XYTYPE *tl,XYTYPE *lr)
{
	OMODULE *mod_ptr;
	XYTYPE mod_xysize;
	XYTYPE mod_start,mod_end;
	XYTYPE topl,lowerr;
//	long ytemp;

	mod_ptr=(OMODULE *)(GET_LONG(bak_info)++);
	mod_xysize.yx=GET_LONG(mod_ptr);

	/* set module start position */
	mod_start.u.xpos=GET_WORD(bak_info)++;
	mod_start.u.ypos=GET_WORD(bak_info)++;

	if (mod_start.u.xpos==(short)BLSTSTRT)
		mod_start.u.xpos=-mod_xysize.u.xpos;
	else if (mod_start.u.xpos==(short)BLSTEND)
			mod_start.u.xpos=0;

	/* gotnewx */
	if (mod_start.u.ypos==(short)BLSTSTRT)
		mod_start.u.ypos=-mod_xysize.u.ypos;
	else if (mod_start.u.ypos==(short)BLSTEND)
			mod_start.u.ypos=0;

	/* set module end position */
	mod_end.u.xpos=mod_start.u.xpos+mod_xysize.u.xpos;
	mod_end.u.ypos=mod_start.u.ypos+mod_xysize.u.ypos;

	/* check if any part of module is viewable */
	if  (!((mod_end.u.xpos<tl->u.xpos) ||
		(mod_end.u.ypos<tl->u.ypos) ||
		(mod_start.u.xpos>lr->u.xpos) ||
		(mod_start.u.ypos>lr->u.ypos)))
		{
			topl.u.xpos=tl->u.xpos-mod_start.u.xpos;        /* convert screen coords to relative mod screen coords */
			topl.u.ypos=tl->u.ypos-mod_start.u.ypos;

			lowerr.u.xpos=lr->u.xpos-mod_start.u.xpos;      /* convert screen coords to relative mod screen coords */
			lowerr.u.ypos=lr->u.ypos-mod_start.u.ypos;

			display_add(baklst,mod_ptr,bakbit,&topl,&lowerr,&mod_start);
		}

	return;
}

/******************************************************************************
 Function: void display_add(OBJECT **baklst,OMODULE *bakmod,LONG *bakbit,XYTYPE *tl,XYTYPE *lr,XYTYPE *start)

 By: David Schwartz

 Date: Aug, 1994

 Parameters: baklst - ptr to ptr to display list for plane
			 bakmod - ptr to module list
			 bakbit - ptr to array of longs (1 bit per object) on plane, indicates object already added
			 tl - top left corner of screen (Y:X)
			 lr - lower right corner of screen (Y:X)
			 start - module start position (Y:X)

 Returns: None

 Description:   parse through module block list and adds any modules that are not
				on display list that fit on active screen area.
******************************************************************************/
void display_add(OBJECT **baklst,OMODULE *bakmod,LONG *bakbit,XYTYPE *tl,XYTYPE *lr,XYTYPE *start)
{
	static LONG bit_mask_table [] =
	{
		0x00000001,0x00000002,0x00000004,0x00000008,
		0x00000010,0x00000020,0x00000040,0x00000080,
		0x00000100,0x00000200,0x00000400,0x00000800,
		0x00001000,0x00002000,0x00004000,0x00008000,
		0x00010000,0x00020000,0x00040000,0x00080000,
		0x00100000,0x00200000,0x00400000,0x00800000,
		0x01000000,0x02000000,0x04000000,0x08000000,
		0x10000000,0x20000000,0x40000000,0x80000000
	};

	WORD done=FALSE;
	short xpos;
	WORD block_num=0;
	LONG blk_mask;
	WORD blk_index;
	WORD blk_cnt;
	OBLOCK *blk_ptr;
	OHEADER *hdr_ptr,*hdr_info;

	blk_ptr=bakmod->block_ptr;
	hdr_ptr=bakmod->header_ptr;

	xpos =tl->u.xpos-BACKGROUND_WIDEST_BLOCK;
	blk_cnt=bakmod->blockcnt;

	/* find first block that could be on screen */
	while (!done && block_num<blk_cnt)
	{
		if ((short)blk_ptr->xoffset >= xpos)
			done=TRUE;
		else
		{
			blk_ptr++;
			block_num++;
		}
	}

	if (!done) return;                                      /* if not done then no blocks on screen to exit */

	/* loop one scan until x coord greater the tl or no blocks left */
	while (block_num<blk_cnt)
	{
		blk_index=block_num/BACK_BIT_RESOLUTION;
		blk_mask=bit_mask_table[block_num % BACK_BIT_RESOLUTION];

		/* if block not on display list then check for on active screen */
		if (!(*(bakbit+blk_index) & blk_mask))
		{
			if ((short)blk_ptr->xoffset>tl->u.xpos)         /* check if x to big */
				break;
			else
			{
				hdr_info=hdr_ptr+blk_ptr->hdr_index;

				if (!(((short)(blk_ptr->xoffset+hdr_info->t_width)<tl->u.xpos) ||
					((short)blk_ptr->yoffset>lr->u.ypos) ||
					((short)(blk_ptr->yoffset+hdr_info->t_height)<tl->u.ypos)))

					if (add_block(baklst,bakbit+blk_index,blk_ptr,hdr_info,start,blk_mask,bakmod->clut_ptr))
						return;                                                                 /* no objects left */
			}

		}

		/* move to next block */
		block_num++;
		blk_ptr++;
	}

	/* loop two scan until x coord greater then lr or no blocks left */
	while (block_num<blk_cnt)
	{
		blk_index=block_num/BACK_BIT_RESOLUTION;
		blk_mask=bit_mask_table[block_num % BACK_BIT_RESOLUTION];

		/* if block not on display list then check for on active screen */
		if (!(*(bakbit+blk_index) & blk_mask))
		{
			if ((short)blk_ptr->xoffset>lr->u.xpos)         /* check if x to big then exit */
				return;

			hdr_info=hdr_ptr+blk_ptr->hdr_index;

			if (!(((short)(blk_ptr->xoffset+hdr_info->t_width)<tl->u.xpos) ||
				((short)blk_ptr->yoffset>lr->u.ypos) ||
				((short)(blk_ptr->yoffset+hdr_info->t_height)<tl->u.ypos)))

					if (add_block(baklst,bakbit+blk_index,blk_ptr,hdr_info,start,blk_mask,bakmod->clut_ptr))
						return;                                                                 /* no objects left */
		}

		/* move to next block */
		block_num++;
		blk_ptr++;
	}
	return;
}

/******************************************************************************
 Function: int add_block(OBJECT **baklst,LONG *bakbit,OBLOCK bakblk,OHEADER *hdr_info,XYTYPE *start,LONG blkmask,WORD **blkclt)

 By: David Schwartz

 Date: Aug 23, 1994

 Parameters: baklst = ptr to ptr to display list
			 bakbit = ptr to background display bits for current block
			 bakblk = ptr to background block info
			 hdr_info = ptr to block tile information (header info)
			 start = Y:X start position of module
			 blkmask = bit mask to set block is on display list
			 blkclt = ptr to clut tables

 Returns: TRUE = object not allocated
		  FALSE = object allocated

 Description:   This routine does the following:
				1) retrive an object block, returns TRUE if no available
				2) updates object block with info
				3) adds object block onto display list
******************************************************************************/
int add_block(OBJECT **baklst,LONG *bakbit,OBLOCK *bakblk,OHEADER *hdr_info,XYTYPE *start,LONG blkmask,WORD **blkclt)
{
	OBJECT *obj;
//	int temp;
//	ADDRESS *temp1;

	if ((obj=get_object())==NULL)
		return(TRUE);

	(LONG *)obj->oplink=bakbit;                             /* set oplink to be ptr to display list bit */
	*bakbit |=blkmask;                                      /* set display bit to on */

	obj->oblockmask=blkmask;                        /* set mask */

	obj->oxpos.pos=(start->u.xpos+bakblk->xoffset)<<16;     /* set screen position */
	obj->oypos.pos=(start->u.ypos+bakblk->yoffset)<<16;

	obj->ozval=bakblk->zdepth;                                                      /* set z depth, for priority */

	/* set texture info */
	obj->header.tpage=(hdr_info->tpage<<1)+bkgd_base_tpage;
	obj->header.t_xoffset=hdr_info->t_xoffset;
	obj->header.t_yoffset=hdr_info->t_yoffset;
	obj->osize.u.xpos=obj->header.t_width=hdr_info->t_width;
	obj->osize.u.ypos=obj->header.t_height=hdr_info->t_height;

	/* set texture flips */
	obj->oflags=(bakblk->flags & (M_FLIPH+M_FLIPV));

	/* set CLUT info */
	obj->opal=get_back_pal((*(blkclt+((bakblk->flags & M_PAL)>>8))));

	/* insert object onto display list ZY sort*/
	insert_object(obj,baklst);

	return(FALSE);
}

void *bgnd_move_table[]=
{
	&baklst8,&baklst8_t,                    // from , to
	&baklst7,&baklst7_t,
	&baklst6,&baklst6_t,
	&baklst5,&baklst5_t,
	&baklst4,&baklst4_t,
	&baklst3,&baklst3_t,
	&baklst2,&baklst2_t,
	&baklst1,&baklst1_t,
	&objlst,&objlst_t,
	&worldtlx8,&worldtlx8_t,
	&worldtlx7,&worldtlx7_t,
	&worldtlx6,&worldtlx6_t,
	&worldtlx5,&worldtlx5_t,
	&worldtlx4,&worldtlx4_t,
	&worldtlx3,&worldtlx3_t,
	&worldtlx2,&worldtlx2_t,
	&worldtlx1,&worldtlx1_t,
	&worldtlx,&worldtlx_t,
	NULL,NULL
};

/******************************************************************************
 Function: void background_blast(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description:   transition when blasting through one background
			    to another
******************************************************************************/
extern void *bkgd_subway_anims[];
extern void *STBREAK3[];
extern void *STBREAK2[];
extern void *STBREAK1[];
extern void *bank_anims;
extern void *BANK_STBREAK3[];
extern void *BANK_STBREAK2[];
extern void *BANK_STBREAK1[];
extern void *soul_anims;
extern void *SOUL_STBREAK3[];
extern void *SOUL_STBREAK2[];
extern void *SOUL_STBREAK1[];

extern void *hell_anims;
extern void *HELL_STBREAK3[];
extern void *HELL_STBREAK2[];
extern void *HELL_STBREAK1[];

void background_blast(void)
{
	OBJECT *olist,*obj,*oa9;
	void *blist;
	void *fromlst,*tolst;
	short temp;

  psxspu_start_cd_fade(FADE_TIME,0);

	stop_scrolling();

	/* block #1 = centered about player who is crashing through */
	oa9=current_proc->pa8;                                          // a9, plyer obj that kicked nailed
	(OBJECT *)current_proc->pa9=oa9;                        // pass parm for exploding floor

	switch (load_level)
	{
		case LVL_SUBWAY_STREET:
			obj=get_ceiling_block1(STBREAK1);
			break;
		case LVL_BANK:
			obj=get_ceiling_block1(BANK_STBREAK1);
			break;
		case LVL_SOUL:
			obj=get_ceiling_block1(SOUL_STBREAK1);
			break;
		case LVL_HADES_CAVE:
			obj=get_ceiling_block1(HELL_STBREAK1);
			break;
	}

	center_about_x(obj,oa9->oxpos.u.intpos);                // lineup with victim
	insert_object(obj,&objlst);
	(OBJECT *)current_proc->a10=obj;                        // a10 - 1st ceiling block

	/* blast0, build blocks left */
	temp=((OBJECT *)current_proc->a10)->oxpos.u.intpos;     // a5, current pos
	do
	{
		switch (load_level)
		{
			case LVL_SUBWAY_STREET:
				obj=get_ceiling_block1(STBREAK1);
				break;
			case LVL_BANK:
				obj=get_ceiling_block1(BANK_STBREAK1);
				break;
			case LVL_SOUL:
				obj=get_ceiling_block1(SOUL_STBREAK1);
				break;
			case LVL_HADES_CAVE:
				obj=get_ceiling_block1(HELL_STBREAK1);
				break;
		}

		obj->oypos.u.intpos=((OBJECT *)current_proc->a10)->oypos.u.intpos;
		obj->oxpos.u.intpos=temp-obj->osize.u.xpos;
		insert_object(obj,&objlst);
		temp=obj->oxpos.u.intpos;                               // new current xpos
	}
	while(temp-worldtlx.u.intpos>(-0x10));          // build until pieces off screen

	/* blast1, build blocks right */
	temp=((OBJECT *)current_proc->a10)->oxpos.u.intpos;     // a5, current pos
	do
	{
		switch (load_level)
		{
			case LVL_SUBWAY_STREET:
				obj=get_ceiling_block1(STBREAK1);
				break;
			case LVL_BANK:
				obj=get_ceiling_block1(BANK_STBREAK1);
				break;
			case LVL_SOUL:
				obj=get_ceiling_block1(SOUL_STBREAK1);
				break;
			case LVL_HADES_CAVE:
				obj=get_ceiling_block1(HELL_STBREAK1);
				break;
		}

		obj->oypos.u.intpos=((OBJECT *)current_proc->a10)->oypos.u.intpos;
		obj->oxpos.u.intpos=temp+obj->osize.u.xpos;
		insert_object(obj,&objlst);
		temp=obj->oxpos.u.intpos;                               // new current xpos
	}
	while((temp-worldtlx.u.intpos)<SCRRGT);         // build until pieces off screen

	/* broken pieces */
	switch (load_level)
	{
		case LVL_SUBWAY_STREET:
			obj=get_ceiling_block2(STBREAK2);
			break;
		case LVL_BANK:
			obj=get_ceiling_block2(BANK_STBREAK2);
			break;
		case LVL_SOUL:
			obj=get_ceiling_block2(SOUL_STBREAK2);
			break;
		case LVL_HADES_CAVE:
			obj=get_ceiling_block1(HELL_STBREAK2);
			break;
	}

	obj->ozval=1;
	obj->oxpos.u.intpos=oa9->oxpos.u.intpos-obj->osize.u.xpos;
	(OBJECT *)(current_proc->pdata.p_store1)=obj;

	switch (load_level)
	{
		case LVL_SUBWAY_STREET:
			obj=get_ceiling_block3(STBREAK3);
			break;
		case LVL_BANK:
			obj=get_ceiling_block3(BANK_STBREAK3);
			break;
		case LVL_SOUL:
			obj=get_ceiling_block3(SOUL_STBREAK3);
			break;
		case LVL_HADES_CAVE:
			obj=get_ceiling_block1(HELL_STBREAK3);
			break;
	}
	obj->ozval=1;
	obj->oxpos.u.intpos=oa9->oxpos.u.intpos;
	(OBJECT *)(current_proc->pdata.p_store2)=obj;

	/* wait for the floor to scroll offscreen */
	scrolly.pos=-(0xa0000);

	/* blast2 */
	do
	{
		process_sleep(1);
	}
	while((skew_height-(ceiling_y-worldtly.u.intpos))>0);   // do until higher then floor height

	free_pal(skew_constpal);                        // delete old floor palette!!
	dallprc(PID_COMBO_MSG);
	dallobj(OID_COMBO_MSG);

	/* pull player objects off "objlst" */
	delobj(p1_obj);                         // take off objlst
	delobj(p2_obj);                         // take off objlst

	olist=obj_free->olink;          // pt to p2 obj->olink (p1)
	obj_free=olist->olink;                  // pt to p1 obj->olink (free)
								// take player obj's off free list

	screen_a5(p2_obj);
	screen_a5(p1_obj);

	/* object list block move */
	blist=bgnd_move_table;

	/* blast3 */
	while ((fromlst=(void *)(GET_LONG(blist)++))!=NULL)             // grab "from" list
	{
		tolst=(void *)(GET_LONG(blist)++);                                      // grab "to" list

		*((ADDRESS *)tolst)=*((ADDRESS*)fromlst);               // get "from" & stuff "to"
		*((ADDRESS*)fromlst)=NULL;                                              // clear "from"
	}

	/* blast4, both player objects --> objlst */
	worldtly_t.pos=worldtly.pos;                            // pass along y

	p2_obj->olink=NULL;
	p1_obj->olink=p2_obj;
	objlst=p1_obj;                                  // create object list P1 & P2 only

	load_bkgd=current_proc->pdata.p_store6;
	init_background_module(table_o_mods[load_bkgd]);
	no_edge_limits();                                               // no limits just yet

	/* plot the new background objects */
	f_only_t=1;                                                             // avoid "plot while on screen race

	dlists=(void*)current_proc->pdata.p_store7;
	multi_plane();

	worldtly.u.intpos+=(short)current_proc->pdata.p_store5;                         // start underneath upper background

	world_a5(p2_obj);                                               // new world coords
	world_a5(p1_obj);

	f_only_t=0;                                                             // ok to plot now

	process_sleep(1);

	/* disable interfering procs */
	dallprc(PID_REPELL);
	dallprc(PID_SCROLL);

	/* blast5 */
	do
	{
		process_sleep(1);
	}
	while(oa9->oypos.u.intpos>232); // wait for player to reach cieling/floor

	/* put broken pieces on object list */
	insert_object((OBJECT *)(current_proc->pdata.p_store1),&objlst_t);
	insert_object((OBJECT *)(current_proc->pdata.p_store2),&objlst_t);

	(OBJECT *)current_proc->pa9=oa9;                        // restore reg a9, needed later
	create_fx(FX_FLOOR_EXPLODE);

	/* blast8, wait for scroll to reach broken floor */
	do
	{
		process_sleep(1);
		temp=ceiling_y+SCY(63)+10;                                      // 63 factor size of stbreak
	}
	while(worldtly.u.intpos>temp);          // part1 = zoom up to floor

	scrolly.pos=0;                                                                  // stop scrolling

	temp-=worldtly.u.intpos;

	worldtly.u.intpos-=temp;
	worldtly_t.u.intpos-=temp;                      // set both worldy's to match

	/* blast9 */
	scrolly.pos=-(0x20000);                         // scroll up slow
	do
	{
		process_sleep(1);
	}
	while(worldtly.u.intpos>ceiling_y);     // part 2 = scroll up slowly

	scrolly.pos=0;                                          // stop scrolling
	worldtly.u.intpos=ceiling_y;

	CREATE(PID_REPELL,repell);
	CREATE(PID_SCROLL,scroller);
	CREATE(PID_BACKG,background_routine);
//***********
	process_exist(PID_FLASHER,-1);
	CREATE(PID_FLASHER,flash_pmsg);
	update_pmsg();
	update_win_counters();
//***********

	curback=(WORD)current_proc->pdata.p_store6;                     // new current background

	blist=table_o_mods[curback];            // reset edge limits
	left_edge=(short)(*((short *)blist+4));
	right_edge=(short)(*((short *)blist+5));

	blist=(void *)((short *)blist+6);       // skip past word data
	dlists=(void *)(*((ADDRESS *)blist+2)); // activate prpoper dlists

	/* blastA */
	delolist(&baklst8_t);                           // delete objects on temp list
	delolist(&baklst7_t);
	delolist(&baklst6_t);
	delolist(&baklst5_t);
	delolist(&baklst4_t);
	delolist(&baklst3_t);
	delolist(&baklst2_t);
	delolist(&baklst1_t);
	delolist(&objlst_t);

	process_sleep(0x20);

	switch (curback)
	{
		case BKGD_STREET_MOD:
			play_generic_tune(TRK_THRUSUB);
			break;
		case BKGD_ROOF_MOD:
		case BKGD_TOWER_MOD:
			play_generic_tune(TRK_THRUBANK);
		case BKGD_CAVE_MOD:
			play_generic_tune(TRK_BANK);
			break;
	}

	process_sleep(0x20);		// should add up to 0x40

	process_suicide();
}

/******************************************************************************
 Function: OBJECT *gcb3(void *frame)

 By: David Schwartz

 Date: Jan 1995

 Parameters: frame - img to draw

 Returns: obj - newly created object

 Description:   grab a ceiling block and display it
******************************************************************************/
OBJECT *gcb3(void *frame)
{
	OBJECT *obj;

	switch (load_level)
	{
		case LVL_SUBWAY_STREET:
			gso_dmawnz(obj,(ADDRESS)frame,bkgd_subway_anims,0);
			alloc_cache(frame,&bkgd_subway_anims,obj);
			break;
		case LVL_CITY:
			gso_dmawnz(obj,(ADDRESS)frame,bank_anims,0);
			alloc_cache(frame,&bank_anims,obj);
			break;
		case LVL_SOUL:
			gso_dmawnz(obj,(ADDRESS)frame,soul_anims,0);
			alloc_cache(frame,&soul_anims,obj);
			break;
		case LVL_HADES_CAVE:
	 		gso_dmawnz(obj,(ADDRESS)frame,hell_anims,0);
	 		alloc_cache(frame,&hell_anims,obj);
			break;
		default:
			wait_forever();
	}

	obj->oypos.u.intpos=ceiling_y-SCY(0x60)+12;
	obj->ozval=0;


	return(obj);
}

/******************************************************************************
 Function: void calla_return(void)

 By: David Schwartz

 Date:

 Parameters: None

 Returns: None

 Description:   Nothing
******************************************************************************/
void calla_return(void)
{
	return;
}

/******************************************************************************
 Function: void skew_7(long scroll)

 By: David Schwartz

 Date: Dec 1994

 Parameters: scroll value to work with

 Returns: None

 Description:   skew adjust for floor scroll/128
******************************************************************************/
void skew_7(long scroll)
{
	skew_dx.pos-=(scroll>>7);                               // dx per line adjust with scroll
	return;
}

/******************************************************************************
 Function: void garbage_straight(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   create a straight flying piece of garbage
******************************************************************************/
void garbage_straight(void)
{
	get_paper_object();

	do
	{
		paper_start_position();
		paper_glide();
		paper_glide();
		paper_fly_offscreen(0,0);
	}
	while(1);
}

/******************************************************************************
 Function: void garbage_loop(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   create a loop flying piece of garbage
******************************************************************************/
void garbage_loop(void)
{
	OBJECT *obj;
	POS ta1;
	POS ta2;

	get_paper_object();
	obj=current_proc->pa8;

	/* ploop1 */
	do
	{
		paper_start_position();
		paper_glide();

		(long)current_proc->a10=-SCY(0x2800);           // dy
		/* ploop2 */
		do
		{
			ta1.pos=-SCX(0x4000);           // dx

			if (abs(obj->oxvel.pos)<=SCX(0x8000))
				(long)current_proc->a10=SCY(0x5000);            // reached peak --> head downwards

			/* ploop3 */
			ta2.pos=(long)current_proc->a10;
			paper_loop_sleep(&ta1.pos,&ta2.pos);
		}
		while(ta1.pos>-SCX(0x20000));

		/* ploop4 */
		do
		{
			ta1.pos=SCX(0x3000);
			ta2.pos=SCY(0x3000);
			paper_loop_sleep(&ta1.pos,&ta2.pos);

			if (ta2.pos>SCY(0x30000))
				ta2=SCY(0x30000);

			/* ploop5 */
		}
		while(ta1.pos<SCX(0x30000));

		paper_fly_offscreen(SCX(0x3000),-SCY(0x2000));
	}
	while(TRUE);
}

/******************************************************************************
 Function: void paper_fly_offscreen(short pa1,short pa2)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa1 - x grav
			 pa2 - y grav

 Returns: None

 Description:   have paper go offscreen
******************************************************************************/
void paper_fly_offscreen(short pa1,short pa2)
{
	long ta1,ta2;

	do
	{
		ta1=pa1;
		ta2=pa2;
		paper_loop_sleep(&ta1,&ta2);
		next_anirate();
	}
	while((current_proc->pa8)->oxpos.u.intpos<(SCRRGT+worldtlx1.u.intpos));

	return;
}

/******************************************************************************
 Function: void paper_loop_sleep(long *pa1,long *pa2)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa1 - ptr to x grav
			 pa2 - ptr to y grav

 Returns: pa1 - current xvel
		  pa2 - current yvel

 Description:   sleep and affect speed routine for flying papers
******************************************************************************/
void paper_loop_sleep(long *pa1,long *pa2)
{
	OBJECT *obj=current_proc->pa8;

	obj->oyvel.pos+=(*pa2);
	obj->oxvel.pos+=(*pa1);

	process_sleep(1);
	next_anirate();

	*pa2=obj->oyvel.pos;
	*pa1=obj->oxvel.pos;

	return;
}

/******************************************************************************
 Function: void paper_glide(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   glide through the wind
******************************************************************************/
void paper_glide(void)
{
	OBJECT *obj=current_proc->pa8;

	obj->oyvel.pos=srandarc(SCY(0x20000))-SCY(0x4000);

	current_proc->a10=randu_minimum(0x20,0x08);

	/* glid3 */
	do
	{
		process_sleep(1);
		next_anirate();
	}
	while(--current_proc->a10>0);

	return;
}

/******************************************************************************
 Function: void get_paper_object(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: current_proc->pa9 - ptr to animation

 Returns: None

 Description:   setup a paper object
******************************************************************************/
void get_paper_object(void)
{
	OBJECT *obj;

	process_sleep(randu(0x30));

	init_anirate(4);

	(ADDRESS)current_proc->a5=(ADDRESS)COMPUTE_ADDR((ADDRESS)current_proc->a10,GET_LONG(current_proc->pa9));
	gso_dmawnz(obj,(ADDRESS)current_proc->a5,(void *)(current_proc->a10),0);
	alloc_cache((void *)current_proc->a5,current_proc->a10,obj);
	insert_object(obj,&baklst4);

	return;
}

/******************************************************************************
 Function: void paper_start_position(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: current_proc->pa9 - ptr to animation

 Returns: None

 Description:   setup a paper object start position
******************************************************************************/
void paper_start_position(void)
{
	a4_to_world_coord(-0x10,(short)randu_minimum(SCY(0x40),SCY(0x80)));

	(current_proc->pa8)->oxvel.pos=(long)randu_minimum(SCX(0x40000),SCX(0x40000));

	return;
}

/******************************************************************************
 Function: void a4_to_world_coord(short pa0,short pa1)

 By: David Schwartz

 Date: Feb 1995

 Parameters: pa0 - x start pos
			 pa1 - y start pos

 Returns: None

 Description:   convert screen to world pos
******************************************************************************/
inline void a4_to_world_coord(short pa0,short pa1)
{
	set_xy_coordinates(current_proc->pa8,worldtlx1.u.intpos+pa0,worldtly.u.intpos+pa1);

	return;
}


/******************************************************************************
 Function: void wind_sound(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: None

 Returns: None

 Description:   sound the wind
******************************************************************************/
void wind_sound(void)
{
	process_sleep(8);                               // make sure sound data is loaded before we start
	while (TRUE)
	{
		tsound(5);                      // wind sound
		process_sleep(0xc8);
	}
}

/******************************************************************************
 Function: void dumb_animator(void)

 By: David Schwartz

 Date: Feb 1995

 Parameters: current_proc->a10 - sleep time between each frame
			 current_proc->pa9 - table of frames

 Returns: None

 Description:   dumb animator routine that continueously update the same animation (single object)
******************************************************************************/
void dumb_animator(void)
{
	while (1)
	{
		(void *)current_proc->a11=current_proc->pa9;
		framew(current_proc->a10);
		current_proc->pa9=(void *)current_proc->a11;
	}
}

/******************************************************************************
 Function: inline void stop_scrolling(void)

 By: David Schwartz

 Date: Aug 23, 1994

 Parameters: None

 Returns: None

 Description:   This routine will do the follows:
	1)      clear all scrolling x values
******************************************************************************/
inline void stop_scrolling(void)
{
	scrollx.pos=scrollx1.pos=scrollx2.pos=scrollx3.pos=scrollx4.pos=0;
	scrollx5.pos=scrollx6.pos=scrollx7.pos=scrollx8.pos=0;
}

/******************************************************************************
 Function: scroll_a9(void)

 By: David Schwartz

 Date: May 1995

 Parameters: None

 Returns: None

 Description:   This routine will do the follows:
******************************************************************************/
void scroll_a9(void)
{
	LONG pa9;

	scrolly.pos=0;
	worldtly.pos=ceiling_y<<16;

	/* scrol9 */
	do
	{
		pa9=(ADDRESS)current_proc->pa9;
		scroll_to_x_a9();
		(ADDRESS)current_proc->pa9=pa9;
		process_sleep(1);
	}
	while(1);
}


void scroll_to_x_a9(void)
{
	short ta9;

	ta9=(short)((long)current_proc->pa9);

	if (abs(ta9-worldtlx.u.intpos)<6)
		stop_scrolling();
	else
	{
		if (ta9>worldtlx.u.intpos)
			kcen_right();
		else
		{
			if (ta9<worldtlx.u.intpos)
				kcen_left();
			else stop_scrolling();
		}
	}

	return;
}

/******************************************************************************
 Function: void scroll_center(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: current_proc->pa8 - dude to follow

 Returns: None

 Description:   This routine will do the follows:
			center camera around dude
******************************************************************************/
void scroll_center(void)
{
	scrolly.pos=0;

	worldtly.pos=ceiling_y<<16;

	/* scrc3 */
	do
	{
		keep_centered_on_a8();
		handle_y_scrolling();
		process_sleep(1);
	}
	while(TRUE);
}

/******************************************************************************
 Function: void keep_centered_on_a8(void)

 By: David Schwartz

 Date: Jan 1995

 Parameters: current_proc->pa8 - dude to center on

 Returns: None

 Description:   This routine will do the centering
******************************************************************************/
void keep_centered_on_a8(void)
{
	OBJECT *obj=current_proc->pa8;

	(short)current_proc->a1=worldtlx.u.intpos+(SCRRGT/2);

	if (abs(obj->oxpos.u.intpos-(short)current_proc->a1)>=6)        // if < then already centered, stop scroll
	{
		if (obj->oxpos.u.intpos>(short)current_proc->a1)
		{
			kcen_right();
			return;
		}
		else if (obj->oxpos.u.intpos<(short)current_proc->a1)
		{
			kcen_left();
			return;
		}
	}

	/* kcen3 */
	stop_scrolling();

	return;
}

void kcen_right(void)
{
	if (abs(right_edge-worldtlx.u.intpos)>6)
	{
		ss_40000(0);
	}
	else
	{
		stop_scrolling();
	}

	return;
}

void kcen_left(void)
{
	if (abs(left_edge-worldtlx.u.intpos)>6)
	{
		ss_40000(1);
	}
	else
	{
		stop_scrolling();
	}
	return;
}

/******************************************************************************
 Function: void scroller(void)

 By: David Schwartz

 Date: Aug 23, 1994

 Parameters: None

 Returns: None

 Description:   This process keeps the two players on the screen at all times.
				i.e. it is the camera routine.
******************************************************************************/
void scroller(void)
{
	POS leftpos,rightpos,distance,centerpt;
	short centerscrn;
	OBJECT *p_a8,*p_a9;

	/* Wait, until game has setup */
	process_sleep(4);

	stop_scrolling();

	while (TRUE)
	{
		process_sleep(1);

		/* determine left/right position of two fighters */
		if (p2_obj->oxpos.pos>p1_obj->oxpos.pos)
		{
			leftpos.pos=p1_obj->oxpos.pos;                                  // a0
			rightpos.pos=p2_obj->oxpos.pos;                                 // a1
			p_a9=p1_obj;
			p_a8=p2_obj;
		}
		else
		{
			leftpos.pos=p2_obj->oxpos.pos;                                  // a0
			rightpos.pos=p1_obj->oxpos.pos;                                 // a1
			p_a8=p1_obj;
			p_a9=p2_obj;
		}

		/* calc distance/placement info */
		distance.pos=rightpos.pos-leftpos.pos;                          // a3
		centerpt.pos=leftpos.pos+(distance.pos>>1);                     // a2

		/* X SCROLLING, determine far or close routine */
		if (distance.u.intpos <= SCROLL_FAR)
		{
			/* dudes are close together */
			centerscrn=worldtlx.u.intpos+SCRRGT/2;                  // a3

			if (abs(centerpt.u.intpos-centerscrn) > (SCROLL_MIDZONE/2))
				if (centerscrn>leftpos.u.intpos)
				{
					if (worldtlx.u.intpos>left_edge)                        /* scroll to far left */
						scroll_choose(TRUE);
					else stop_scrolling();
				}
				else
				{
					if (worldtlx.u.intpos<right_edge)                       /* scroll to far right */
						scroll_choose(FALSE);
					else stop_scrolling();
				}
			else stop_scrolling();
		}
		else
		{
			/* dudes are far apart (just keep'em on screen) */
			if (p_a8->oflags2 & M_NOEDGE)
				rightpos.pos=leftpos.pos;			// ignore a8 player

			if (p_a9->oflags2 & M_NOEDGE)
				leftpos.pos=rightpos.pos;			// ignore a9 player

			if (leftpos.u.intpos < (worldtlx.u.intpos+SCROLL_GRACE_SIZE))
			{
					if (worldtlx.u.intpos>left_edge)                        /* scroll to far left */
						scroll_choose(TRUE);
					else stop_scrolling();
			}
			else
			{
				if (rightpos.u.intpos > (worldtlx.u.intpos+SCRRGT-SCROLL_GRACE_SIZE))
				{
					if (worldtlx.u.intpos<right_edge)                       /* scroll to far right */
						scroll_choose(FALSE);
					else stop_scrolling();
				}
				else stop_scrolling();
			}
		}

		handle_y_scrolling();
	}

	return;
}

void handle_y_scrolling(void) 
{
	/***************
	 * y scrolling *
	 ***************/
	(short)current_proc->a6=(p1_obj->oflags2 & M_IGNORE_Y) ? p1_proc->pdata.p_ganiy : p1_obj->oypos.u.intpos;       // if scroller ignore, then tell scroller player grounded

	(short)current_proc->a7=(p2_obj->oflags2 & M_IGNORE_Y) ? p2_proc->pdata.p_ganiy : p2_obj->oypos.u.intpos;       // if scroller ignore, then tell scroller player grounded

	/* SCRL5 */
	if ((worldtly.u.intpos+SCROLL_BARRIER_PAD+16)>=(short)current_proc->a6||
		(worldtly.u.intpos+SCROLL_BARRIER_PAD+16)>=(short)current_proc->a7)
	{
		/* SCRL7, scroll up */
		if (worldtly.u.intpos <= (ceiling_y-(short)SCY(0x10)))                          /* check highest allowed scroll */
			scrolly.pos=0;
		else
		{
			if (abs(worldtly.u.intpos+SCROLL_BARRIER_PAD+16-(short)current_proc->a6)<2 ||   /* how far away from limit for p1 */
				abs(worldtly.u.intpos+SCROLL_BARRIER_PAD+16-(short)current_proc->a7)<2)                 /* how far away from limit for p2 */
				scrolly.pos=0;                                                                          /* to close --> avoid jittering */
			else scrolly.pos=-SCROLL_Y_DOWN;
		}
	}
	else
	{
		/* test to see if we are too low --> adjust back up */
		if (worldtly.u.intpos==ceiling_y)
			scrolly.pos=0;
		else
		{
			if (worldtly.u.intpos>ceiling_y)
			{
				/* SCRL8 */
				scrolly.pos=0;                                          /* stop scrolling */
				worldtly.u.intpos=ceiling_y;            /* stuff exactly where we want to be */
			}
			else
			{
				scrolly.pos=SCROLL_Y_DOWN;                              /* scroll down */
			}
		}
	}
	return;
}


/* scroller for scorpion teleport */
void scorp_tele_scroll(void) 
{
	PROCESS *t_a0;
	OBJECT *t_a2;

	t_a0=(PROCESS *)current_proc->pdata.p_store2;
	t_a2=(OBJECT *)current_proc->pdata.p_store1;		// dude to follow
	do
	{
		if (t_a0->pdata.p_action!=ACT_SCORP_TELE) 
		{
			stack_jump(scroller);
		}
		
		if (t_a2->oxpos.u.intpos<worldtlx.u.intpos+SCROLL_GRACE_SIZE) 
		{
			/* scorp_left */
			if (worldtlx.u.intpos>left_edge)                        /* scroll to far left */
				ss_20000(TRUE);
			else stop_scrolling();
		}
		else
		{
			if (t_a2->oxpos.u.intpos>worldtlx.u.intpos+SCRRGT-SCROLL_GRACE_SIZE) 
			{
				/* scorp_right */
				if (worldtlx.u.intpos<right_edge)                       /* scroll to far right */
					ss_20000(FALSE);
				else stop_scrolling();
			}
			else stop_scrolling();
		}

		/* sts5 */
		scrolly.pos=0;			// no vertical movement
	}
	while(TRUE);
}

/******************************************************************************
 Function: void scroll_choose(WORD neg_flag)

 By: David Schwartz

 Date: Sept 1994

 Parameters: neg_flag = should scroll results be negative or positive

 Returns: None

 Description:   This routine will decide which scroll routine to execute and then
				execute that routine.
******************************************************************************/
void scroll_choose(WORD neg_flag)
{
	long high_vel;
	long done;
	STABLE *stbl_ptr;

	high_vel = (abs(p1_xvel.pos) > abs(p2_xvel.pos)) ? abs(p1_xvel.pos): abs(p2_xvel.pos);          /* get fastest moving dude */

	if (high_vel != 0)
	{
		stbl_ptr=scrolls_table;
		done=FALSE;
		while (!done)
		{
			/* scrr3 */
			if (stbl_ptr->scrl_value==0)                    /* player speed to fast, big problem */
			{
				stbl_ptr--;                                     // last entry = use fastest scroll speed
				done=TRUE;
			}
			else
			{
				/* scrr7 */
				if (stbl_ptr->scrl_value>=high_vel)     /* find scroll routine thats speed is faster then players */
					done=TRUE;
				else stbl_ptr++;
			}
		};

		/* scrr6 */
		stbl_ptr->scrl_func(neg_flag);                  /* execute scroll routine */
	}
	else ss_28000(neg_flag);                                        /* execute scroll routine, no guys moving, panning */

	return;
}

/******************************************************************************
 Function: ss_10000(WORD neg_flag)

 By: David Schwartz

 Date: Sept 1994

 Parameters: neg_flag = should scroll results be pos/neg

 Return: None

 Description: move by 1/2 table values
******************************************************************************/
void ss_10000(WORD neg_flag)
{
	long *scrtab_ptr;

	scrtab_ptr=scrtab;
	if (neg_flag==0)
	{
		scrollx8.pos=(*scrtab_ptr++)/2;
		scrollx7.pos=(*scrtab_ptr++)/2;
		scrollx6.pos=(*scrtab_ptr++)/2;
		scrollx5.pos=(*scrtab_ptr++)/2;
		scrollx4.pos=(*scrtab_ptr++)/2;
		scrollx3.pos=(*scrtab_ptr++)/2;
		scrollx2.pos=(*scrtab_ptr++)/2;
		scrollx1.pos=(*scrtab_ptr++)/2;
		scrollx.pos=(*scrtab_ptr)/2;
	}
	else
	{
		scrollx8.pos=-(*scrtab_ptr++)/2;
		scrollx7.pos=-(*scrtab_ptr++)/2;
		scrollx6.pos=-(*scrtab_ptr++)/2;
		scrollx5.pos=-(*scrtab_ptr++)/2;
		scrollx4.pos=-(*scrtab_ptr++)/2;
		scrollx3.pos=-(*scrtab_ptr++)/2;
		scrollx2.pos=-(*scrtab_ptr++)/2;
		scrollx1.pos=-(*scrtab_ptr++)/2;
		scrollx.pos=-(*scrtab_ptr)/2;
	}

	return;
}

/******************************************************************************
 Function: ss_20000(WORD neg_flag)

 By: David Schwartz

 Date: Sept 1994

 Parameters: neg_flag = should scroll results be pos/neg

 Return: None

 Description:   move by table values
******************************************************************************/
void ss_20000(WORD neg_flag)
{
	long *scrtab_ptr;

	scrtab_ptr=scrtab;
	if (neg_flag==0)
	{
		scrollx8.pos=(*scrtab_ptr++);
		scrollx7.pos=(*scrtab_ptr++);
		scrollx6.pos=(*scrtab_ptr++);
		scrollx5.pos=(*scrtab_ptr++);
		scrollx4.pos=(*scrtab_ptr++);
		scrollx3.pos=(*scrtab_ptr++);
		scrollx2.pos=(*scrtab_ptr++);
		scrollx1.pos=(*scrtab_ptr++);
		scrollx.pos=(*scrtab_ptr);
	}
	else
	{
		scrollx8.pos=-(*scrtab_ptr++);
		scrollx7.pos=-(*scrtab_ptr++);
		scrollx6.pos=-(*scrtab_ptr++);
		scrollx5.pos=-(*scrtab_ptr++);
		scrollx4.pos=-(*scrtab_ptr++);
		scrollx3.pos=-(*scrtab_ptr++);
		scrollx2.pos=-(*scrtab_ptr++);
		scrollx1.pos=-(*scrtab_ptr++);
		scrollx.pos=-(*scrtab_ptr);
	}

	return;
}

/******************************************************************************
 Function: ss_28000(WORD neg_flag)

 By: David Schwartz

 Date: Sept 1994

 Parameters: neg_flag = should scroll results be pos/neg

 Return: None

 Description:   move by 5/4 table values
******************************************************************************/
void ss_28000(WORD neg_flag)
{
	long *scrtab_ptr;

	scrtab_ptr=scrtab;
	if (neg_flag==0)
	{
		scrollx8.pos=(*scrtab_ptr)/4+(*scrtab_ptr++);
		scrollx7.pos=(*scrtab_ptr)/4+(*scrtab_ptr++);
		scrollx6.pos=(*scrtab_ptr)/4+(*scrtab_ptr++);
		scrollx5.pos=(*scrtab_ptr)/4+(*scrtab_ptr++);
		scrollx4.pos=(*scrtab_ptr)/4+(*scrtab_ptr++);
		scrollx3.pos=(*scrtab_ptr)/4+(*scrtab_ptr++);
		scrollx2.pos=(*scrtab_ptr)/4+(*scrtab_ptr++);
		scrollx1.pos=(*scrtab_ptr)/4+(*scrtab_ptr++);
		scrollx.pos=(*scrtab_ptr)/4+(*scrtab_ptr);
	}
	else
	{
		scrollx8.pos=-((*scrtab_ptr)/4+(*scrtab_ptr++));
		scrollx7.pos=-((*scrtab_ptr)/4+(*scrtab_ptr++));
		scrollx6.pos=-((*scrtab_ptr)/4+(*scrtab_ptr++));
		scrollx5.pos=-((*scrtab_ptr)/4+(*scrtab_ptr++));
		scrollx4.pos=-((*scrtab_ptr)/4+(*scrtab_ptr++));
		scrollx3.pos=-((*scrtab_ptr)/4+(*scrtab_ptr++));
		scrollx2.pos=-((*scrtab_ptr)/4+(*scrtab_ptr++));
		scrollx1.pos=-((*scrtab_ptr)/4+(*scrtab_ptr++));
		scrollx.pos=-((*scrtab_ptr)/4+(*scrtab_ptr));
	}
	return;
}

/******************************************************************************
 Function: ss_30000(WORD neg_flag)

 By: David Schwartz

 Date: Sept 1994

 Parameters: neg_flag = should scroll results be pos/neg

 Return: None

 Description:   move by 3/2 table values
******************************************************************************/
void ss_30000(WORD neg_flag)
{
	long *scrtab_ptr;

	scrtab_ptr=scrtab;
	if (neg_flag==0)
	{
		scrollx8.pos=(*scrtab_ptr)/2+(*scrtab_ptr++);
		scrollx7.pos=(*scrtab_ptr)/2+(*scrtab_ptr++);
		scrollx6.pos=(*scrtab_ptr)/2+(*scrtab_ptr++);
		scrollx5.pos=(*scrtab_ptr)/2+(*scrtab_ptr++);
		scrollx4.pos=(*scrtab_ptr)/2+(*scrtab_ptr++);
		scrollx3.pos=(*scrtab_ptr)/2+(*scrtab_ptr++);
		scrollx2.pos=(*scrtab_ptr)/2+(*scrtab_ptr++);
		scrollx1.pos=(*scrtab_ptr)/2+(*scrtab_ptr++);
		scrollx.pos=(*scrtab_ptr)/2+(*scrtab_ptr);
	}
	else
	{
		scrollx8.pos=-((*scrtab_ptr)/2+(*scrtab_ptr++));
		scrollx7.pos=-((*scrtab_ptr)/2+(*scrtab_ptr++));
		scrollx6.pos=-((*scrtab_ptr)/2+(*scrtab_ptr++));
		scrollx5.pos=-((*scrtab_ptr)/2+(*scrtab_ptr++));
		scrollx4.pos=-((*scrtab_ptr)/2+(*scrtab_ptr++));
		scrollx3.pos=-((*scrtab_ptr)/2+(*scrtab_ptr++));
		scrollx2.pos=-((*scrtab_ptr)/2+(*scrtab_ptr++));
		scrollx1.pos=-((*scrtab_ptr)/2+(*scrtab_ptr++));
		scrollx.pos=-((*scrtab_ptr)/2+(*scrtab_ptr));
	}

	return;
}

/******************************************************************************
 Function: ss_40000(WORD neg_flag)

 By: David Schwartz

 Date: Sept 1994

 Parameters: neg_flag = should scroll results be pos/neg

 Return: None

 Description:   move by 2 times table value
******************************************************************************/
void ss_40000(WORD neg_flag)
{
	long *scrtab_ptr;

	scrtab_ptr=scrtab;
	if (neg_flag==0)
	{
		scrollx8.pos=(*scrtab_ptr++)*2;
		scrollx7.pos=(*scrtab_ptr++)*2;
		scrollx6.pos=(*scrtab_ptr++)*2;
		scrollx5.pos=(*scrtab_ptr++)*2;
		scrollx4.pos=(*scrtab_ptr++)*2;
		scrollx3.pos=(*scrtab_ptr++)*2;
		scrollx2.pos=(*scrtab_ptr++)*2;
		scrollx1.pos=(*scrtab_ptr++)*2;
		scrollx.pos=(*scrtab_ptr)*2;
	}
	else
	{
		scrollx8.pos=-(*scrtab_ptr++)*2;
		scrollx7.pos=-(*scrtab_ptr++)*2;
		scrollx6.pos=-(*scrtab_ptr++)*2;
		scrollx5.pos=-(*scrtab_ptr++)*2;
		scrollx4.pos=-(*scrtab_ptr++)*2;
		scrollx3.pos=-(*scrtab_ptr++)*2;
		scrollx2.pos=-(*scrtab_ptr++)*2;
		scrollx1.pos=-(*scrtab_ptr++)*2;
		scrollx.pos=-(*scrtab_ptr)*2;
	}

	return;
}

/******************************************************************************
 Function: ss_60000(WORD neg_flag)

 By: David Schwartz

 Date: Sept 1994

 Parameters: neg_flag = should scroll results be pos/neg

 Return: None

 Description:   move by 3 times table values
******************************************************************************/
void ss_60000(WORD neg_flag)
{
	long *scrtab_ptr;

	scrtab_ptr=scrtab;
	if (neg_flag==0)
	{
		scrollx8.pos=(*scrtab_ptr)*2+(*scrtab_ptr++);
		scrollx7.pos=(*scrtab_ptr)*2+(*scrtab_ptr++);
		scrollx6.pos=(*scrtab_ptr)*2+(*scrtab_ptr++);
		scrollx5.pos=(*scrtab_ptr)*2+(*scrtab_ptr++);
		scrollx4.pos=(*scrtab_ptr)*2+(*scrtab_ptr++);
		scrollx3.pos=(*scrtab_ptr)*2+(*scrtab_ptr++);
		scrollx2.pos=(*scrtab_ptr)*2+(*scrtab_ptr++);
		scrollx1.pos=(*scrtab_ptr)*2+(*scrtab_ptr++);
		scrollx.pos=(*scrtab_ptr)*2+(*scrtab_ptr);
	}
	else
	{
		scrollx8.pos=-((*scrtab_ptr)*2+(*scrtab_ptr++));
		scrollx7.pos=-((*scrtab_ptr)*2+(*scrtab_ptr++));
		scrollx6.pos=-((*scrtab_ptr)*2+(*scrtab_ptr++));
		scrollx5.pos=-((*scrtab_ptr)*2+(*scrtab_ptr++));
		scrollx4.pos=-((*scrtab_ptr)*2+(*scrtab_ptr++));
		scrollx3.pos=-((*scrtab_ptr)*2+(*scrtab_ptr++));
		scrollx2.pos=-((*scrtab_ptr)*2+(*scrtab_ptr++));
		scrollx1.pos=-((*scrtab_ptr)*2+(*scrtab_ptr++));
		scrollx.pos=-((*scrtab_ptr)*2+(*scrtab_ptr));
	}
	return;
}

/******************************************************************************
 Function: ss_70000(WORD neg_flag)

 By: David Schwartz

 Date: Sept 1994

 Parameters: neg_flag = should scroll results be pos/neg

 Return: None

 Description:   move by 7/2 tables values
******************************************************************************/
void ss_70000(WORD neg_flag)
{
	long *scrtab_ptr;

	scrtab_ptr=scrtab;
	if (neg_flag==0)
	{
		scrollx8.pos=(*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr++);
		scrollx7.pos=(*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr++);
		scrollx6.pos=(*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr++);
		scrollx5.pos=(*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr++);
		scrollx4.pos=(*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr++);
		scrollx3.pos=(*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr++);
		scrollx2.pos=(*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr++);
		scrollx1.pos=(*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr++);
		scrollx.pos=(*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr);
	}
	else
	{
		scrollx8.pos=-((*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr++));
		scrollx7.pos=-((*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr++));
		scrollx6.pos=-((*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr++));
		scrollx5.pos=-((*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr++));
		scrollx4.pos=-((*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr++));
		scrollx3.pos=-((*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr++));
		scrollx2.pos=-((*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr++));
		scrollx1.pos=-((*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr++));
		scrollx.pos=-((*scrtab_ptr)/2+(*scrtab_ptr)*2+(*scrtab_ptr));
	}
	return;
}

/******************************************************************************
 Function: ss_a0000(WORD neg_flag)

 By: David Schwartz

 Date: Sept 1994

 Parameters: neg_flag = should scroll results be pos/neg

 Return: None

 Description:   move by 5 times table values
******************************************************************************/
void ss_a0000(WORD neg_flag)
{
	long *scrtab_ptr;

	scrtab_ptr=scrtab;
	if (neg_flag==0)
	{
		scrollx8.pos=(*scrtab_ptr)*4+(*scrtab_ptr++);
		scrollx7.pos=(*scrtab_ptr)*4+(*scrtab_ptr++);
		scrollx6.pos=(*scrtab_ptr)*4+(*scrtab_ptr++);
		scrollx5.pos=(*scrtab_ptr)*4+(*scrtab_ptr++);
		scrollx4.pos=(*scrtab_ptr)*4+(*scrtab_ptr++);
		scrollx3.pos=(*scrtab_ptr)*4+(*scrtab_ptr++);
		scrollx2.pos=(*scrtab_ptr)*4+(*scrtab_ptr++);
		scrollx1.pos=(*scrtab_ptr)*4+(*scrtab_ptr++);
		scrollx.pos=(*scrtab_ptr)*4+(*scrtab_ptr);
	}
	else
	{
		scrollx8.pos=-((*scrtab_ptr)*4+(*scrtab_ptr++));
		scrollx7.pos=-((*scrtab_ptr)*4+(*scrtab_ptr++));
		scrollx6.pos=-((*scrtab_ptr)*4+(*scrtab_ptr++));
		scrollx5.pos=-((*scrtab_ptr)*4+(*scrtab_ptr++));
		scrollx4.pos=-((*scrtab_ptr)*4+(*scrtab_ptr++));
		scrollx3.pos=-((*scrtab_ptr)*4+(*scrtab_ptr++));
		scrollx2.pos=-((*scrtab_ptr)*4+(*scrtab_ptr++));
		scrollx1.pos=-((*scrtab_ptr)*4+(*scrtab_ptr++));
		scrollx.pos=-((*scrtab_ptr)*4+(*scrtab_ptr));
	}

	return;
}

/******************************************************************************
 Function: ss_80000(WORD neg_flag)

 By: David Schwartz

 Date: Sept 1994

 Parameters: neg_flag = should scroll results be pos/neg

 Return: None

 Description:   move by 4 times table values
******************************************************************************/
void ss_80000(WORD neg_flag)
{
	long *scrtab_ptr;

	scrtab_ptr=scrtab;
	if (neg_flag==0)
	{
		scrollx8.pos=(*scrtab_ptr++)*4;
		scrollx7.pos=(*scrtab_ptr++)*4;
		scrollx6.pos=(*scrtab_ptr++)*4;
		scrollx5.pos=(*scrtab_ptr++)*4;
		scrollx4.pos=(*scrtab_ptr++)*4;
		scrollx3.pos=(*scrtab_ptr++)*4;
		scrollx2.pos=(*scrtab_ptr++)*4;
		scrollx1.pos=(*scrtab_ptr++)*4;
		scrollx.pos=(*scrtab_ptr)*4;
	}
	else
	{
		scrollx8.pos=-((*scrtab_ptr++)*4);
		scrollx7.pos=-((*scrtab_ptr++)*4);
		scrollx6.pos=-((*scrtab_ptr++)*4);
		scrollx5.pos=-((*scrtab_ptr++)*4);
		scrollx4.pos=-((*scrtab_ptr++)*4);
		scrollx3.pos=-((*scrtab_ptr++)*4);
		scrollx2.pos=-((*scrtab_ptr++)*4);
		scrollx1.pos=-((*scrtab_ptr++)*4);
		scrollx.pos=-((*scrtab_ptr)*4);
	}
	return;
}

/******************************************************************************
 Function: void create_dumb_animators_w(DTBL_INIT *tbl,void *heap)

 By: David Schwartz

 Date: Feb 1995

 Parameters: tbl - table of dumb animations to start
			 heap - base address

 Return: None

 Description:   start a table of dumb animations
******************************************************************************/
void create_dumb_animators_w(DTBL_INIT *tbl,void *heap)
{
	OBJECT *obj;
	PROCESS *ptemp;

	while (tbl->ani_speed !=0)
	{
		(void *)current_proc->a5=tbl->img;
		gso_dmawnz(obj,(ADDRESS)current_proc->a5,heap,0);
		alloc_cache((void *)current_proc->a5,heap,obj);

		current_proc->pa9=tbl->a_list;

		a4_to_world_coord(tbl->x,tbl->y);

		insert_object(obj,tbl->baklst);

		ptemp=CREATE(PID_BANI,dumb_animator);
		ptemp->a10=tbl->ani_speed;

		tbl++;
	}

	return;
}

/******************************************************************************
 Function: void create_dumb_animators(DTBL_INITA *tbl,void *heap)

 By: David Schwartz

 Date: Feb 1995

 Parameters: tbl - table of dumb animations to start
			 heap - base address

 Return: None

 Description:   start a table of dumb animations
******************************************************************************/
void create_dumb_animators(DTBL_INITA *tbl,void *heap)
{
	OBJECT *obj;
	PROCESS *ptemp;

	while (tbl->ani_speed !=0)
	{
		current_proc->pa9=tbl->a_list;
		(void *)current_proc->a5=COMPUTE_ADDR(heap,GET_LONG(current_proc->pa9)++);      // check for correctness

		gso_dmawnz(obj,(ADDRESS)current_proc->a5,heap,0);
		set_xy_coordinates(obj,tbl->x,tbl->y);
		alloc_cache((void *)current_proc->a5,heap,obj);

		if (tbl->ani_speed & 0x8000)
			flip_single(obj);

		/* cda3 */
		insert_object(obj,tbl->baklst);

		ptemp=CREATE(PID_BANI,dumb_animator);
		ptemp->a10=tbl->ani_speed & 0x7fff;

		tbl++;
	}

	return;
}

extern void *coin_mode_anims[];
extern void *coin_cursor[];
ADDRESS *coin_mode_perm_list[] =
{
	(ADDRESS *)coin_cursor,
	NULL
};

void calla_coin_mode(void) 
{
	alloc_vram_perm_bg_list((ADDRESS *)coin_mode_perm_list,&coin_mode_anims);
	return;
}