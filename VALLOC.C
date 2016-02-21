/******************************************************************************
 File: valloc.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III VRAM ALLOCATION
******************************************************************************/

/*** C Include files ***/
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkos.h"
#include "valloc.h"

/*** module specific ram variable ***/
static VPAGEINFO dyn_vram_pages[MAX_DYNAMIC_TPAGE];
static VMEMINFO dyn_vram_mem[MAX_DYNAMIC_TPAGE][MIN_VRAM_ALLOC_SIZE][MIN_VRAM_ALLOC_SIZE];

static short dyn_page_count;
static short dyn_start_page;				// round robin variable, used to start search on different page each call (speed opt)

/******************************************************************************
 Function: WORD valloc_init(WORD *free_mem)

 By: David Schwartz

 Date: Jan 1995

 Parameters: free_mem - a list of texture pages to put in a dynamic memory (x,y),
 			 entry 0 is # of x,y pairs

 Returns:

 Description: 	initialize valloc/vfree routines and setup various data structures

 NOTE: there is no error checking done on the free_mem list.  Each entry in that list
 must be at the upper left hand corner of a valid tpage.  If not unpredictable results
 will happen when you use the various valloc/vfree functions
******************************************************************************/
WORD valloc_init(WORD *free_mem)
{
	short i,j,k;

	/* clear all dynamic vram memory */
	for (i=0;i<MIN_VRAM_ALLOC_SIZE;i++)
	{
		for (j=0;j<MIN_VRAM_ALLOC_SIZE;j++)
		{
			for (k=0;k<MAX_DYNAMIC_TPAGE;k++)
			{
				dyn_vram_mem[k][i][j].v_xyinfo=0;
			}
		}
	}

	dyn_page_count=0;
	i=*free_mem;
	free_mem++;

	/* init dynamic vram page lists */
	while (--i>=0)
	{
		if (dyn_page_count==MAX_DYNAMIC_TPAGE)
		{
#if VRAM_DEBUG
			vram_debug(VRAM_VALLOC_INIT_FAILED);
#endif
			return(VRAM_VALLOC_INIT_FAILED);
		}

		/* setup page info */
		dyn_vram_pages[dyn_page_count].vptr=&dyn_vram_mem[dyn_page_count][0][0];
		dyn_vram_pages[dyn_page_count].vbasex=*free_mem;
		dyn_vram_pages[dyn_page_count].vbasey=*(free_mem+1);
		dyn_vram_pages[dyn_page_count].vtpage=GetTPage(TEXTURE_MODE,TRANS_RATE,*free_mem,*(free_mem+1));

		/* next entry */
		dyn_page_count++;
		free_mem+=2;
 	}

	dyn_start_page=0;

	return(VRAM_VALLOC_OK);
}

/******************************************************************************
 Function: WORD valloc_rect(WORD width,WORD height,WORD *x_off,WORD *y_off)

 By: David Schwartz

 Date: Jan 1995

 Parameters:  width - width rectangle needed (pixels)
			  height - height of rectangle neede (pixels)

 Returns: tpage info
		  x_off - x offset in tpage
		  y_off - y offset in tpage

 Description:
******************************************************************************/
WORD valloc_rect(WORD width,WORD height,WORD *x_off,WORD *y_off)
{
	short i;
	short page=-1;

	/* normalize size */
	width=(width>>MIN_VRAM_ALLOC_SHIFT)+1;
	height=(height>>MIN_VRAM_ALLOC_SHIFT)+1;

	i=dyn_start_page;
	do
	{
		page=vscan_page(&dyn_vram_pages[i],width,height,x_off,y_off);
		i=(i+1)%dyn_page_count;

	}
	while(i!=dyn_start_page && page==-1);

	dyn_start_page=(dyn_start_page+1)%dyn_page_count;			// next round robin page

	if (page==-1)
	{
#if VRAM_DEBUG
		vram_debug(VRAM_VALLOC_NOSPACE);
#endif
		return(VRAM_VALLOC_NOSPACE);
	}
	else
	{
		return(page);
	}
}

/******************************************************************************
 Function: short vscan_page(VPAGEINFO *vptr,WORD width,WORD height,WORD *x_off,WORD *y_off)

 By: David Schwartz

 Date: Jan 1995

 Parameters:  vpptr - ptr to page info
 			  width - width rectangle needed (pixels)
			  height - height of rectangle neede (pixels)

 Returns: tpage info for found entry (-1 no area found on tpage)
		  x_off - x offset in tpage
		  y_off - y offset in tpage

 Description: 	scan an entire page to see if the requested block will fit
******************************************************************************/
short vscan_page(VPAGEINFO *vpptr,WORD width,WORD height,WORD *x_off,WORD *y_off)
{
	short xcount;
	short xstart,ystart;
	VMEMINFO *vmptr;

	vmptr=vpptr->vptr;							// retrieve ptr to start of vram alloc block page

	xstart=ystart=xcount=0;

	/* scan through page */
	while (ystart<(MIN_VRAM_ALLOC_SIZE-height))
	{
		/* see if start block is unused */
		if ((vmptr+(xstart+(ystart<<MIN_VRAM_ALLOC_SHIFT)))->v_xyinfo!=0)
		{
			/* block isn't available, skip to next start point search */
			xstart+=(vmptr+(xstart+(ystart<<MIN_VRAM_ALLOC_SHIFT)))->u.v_x;				// skip past allocated info
			if (xstart>=(MIN_VRAM_ALLOC_SIZE-width))
			{
				xstart=0;										// reset to start of line
				ystart++;										// move down to next line
			}
		}
		else
		{
			/* search for valid rectangle */
			if ((xcount=vsearch_rect((vmptr+(xstart+(ystart<<MIN_VRAM_ALLOC_SHIFT))),width,height))!=0)
			{
				/* search failed, so skip width of block */
				xstart+=xcount;
				if (xstart>=(MIN_VRAM_ALLOC_SIZE-width))
				{
					xstart=0;										// reset to start of line
					ystart++;										// move down to next line
				}
			}
			else
			{
				/* mark memory as being used */
				vmark_rect((vmptr+(xstart+(ystart<<MIN_VRAM_ALLOC_SHIFT))),width,height);
				/* we found a spot to place object */
				*x_off=xstart<<MIN_VRAM_ALLOC_SHIFT;				// compute abs offset
				*y_off=ystart<<MIN_VRAM_ALLOC_SHIFT;				// compute abs offset
				return(vpptr->vtpage);
			}
		}
	}

	return(-1);						// no room on tpage
}

/******************************************************************************
 Function: short vsearch_rect(VMEMINFO *vmptr,WORD width,WORD height);

 By: David Schwartz

 Date: Jan 1995

 Parameters:  vmptr - start of memory to search
			  width - width of memory needed
			  height - height of memory needed

 Returns: None

 Description: 	This routine scans the start of the memory to see if the rectangular
				area is free.  If it is it returns 0, if it isn't it returns the X coord
				of where a block was used.  This will allow us to skip that amount of searches
				since that rectangle is invalid.
******************************************************************************/
short vsearch_rect(VMEMINFO *vmptr,WORD width,WORD height)
{
	WORD i,j;

	for (j=0;j<height;j++)
	{
		for (i=0;i<width;i++ )
		{
			if ((vmptr+(i+(j<<MIN_VRAM_ALLOC_SHIFT)))->v_xyinfo!=0)
				return(i+1);				// return count in width that cant be used
		}
	}

	return(0);					// we have found our rect, notify caller
}

/******************************************************************************
 Function: void vmark_rect(VMEMINFO *vmptr,WORD width,WORD height);

 By: David Schwartz

 Date: Jan 1995

 Parameters:  vmptr - start of memory to mark
			  width - width of memory to mark
			  height - height of memory to mark

 Returns: None

 Description: 	This routine marks memory to indicate that it is in use.  It marks
			    the memory by supplying the xy dim of the allocated block
******************************************************************************/
void vmark_rect(VMEMINFO *vmptr,WORD width,WORD height)
{
	short i,j;
	VMEMINFO mark;

	mark.u.v_x=width;				// set mark info
	mark.u.v_y=height;

	/* mark memory */
	for (j=0;j<height;j++)
	{
		for (i=0;i<width;i++)
		{
			(vmptr+(i+(j<<MIN_VRAM_ALLOC_SHIFT)))->v_xyinfo=mark.v_xyinfo;
		}
	}

	return;
}

/******************************************************************************
 Function: WORD vfree_rect(WORD tpage,WORD x_off,WORD y_off)

 By: David Schwartz

 Date: Jan 1995

 Parameters:  tpage - which tpage to free from
			  x_off - xoffset within tpage
			  y_off - yoffset within tpage

 Returns: status of free
			VRAM_VALLOC_OK - no problems
			VRAM_VFREE_FAILED - tried to free a 0 size block

 Description: 	this routine will free a piece of vram memory.  Based on the tpage
				and x & y offsets, the routine will retrieve clear the marker area in
				vram_memory.
******************************************************************************/
WORD vfree_rect(WORD tpage,WORD x_off,WORD y_off)
{
	short xs,ys;
	short i,j;
	VMEMINFO *vmptr;
	VPAGEINFO *vpptr;

	/* scan page list to get correct ptr to vram mem */
	xs=0;
	vpptr=dyn_vram_pages;

	while (xs<dyn_page_count && vpptr->vtpage!=tpage)
	{
		xs++;
		vpptr++;
	};

	if (xs==dyn_page_count)
	{
#if VRAM_DEBUG
		vram_debug(VRAM_VFREE_TPAGE);
#endif
		return(VRAM_VFREE_TPAGE);
	}

	/* pos ptr to first block & retrieve size of allocated block */
	vmptr=vpptr->vptr;									// get ptr to vmem
	vmptr+=((y_off)+(x_off>>MIN_VRAM_ALLOC_SHIFT));

	xs=vmptr->u.v_x;				// xsize of alloc block
	ys=vmptr->u.v_y;				// ysize of alloc block


	if (xs==0 || ys==0)
	{
#if VRAM_DEBUG
		vram_debug(VRAM_VFREE_ZERO);
#endif
		return(VRAM_VFREE_ZERO);
	}

	/* erase block */
	for (j=0;j<ys;j++,vmptr+=(MIN_VRAM_ALLOC_SIZE-xs))
	{
		for (i=0;i<xs;i++,vmptr++)
		{
			vmptr->v_xyinfo=0;			// zero block
		}
	}

	return(VRAM_VALLOC_OK);
}

#if VRAM_DEBUG
/******************************************************************************
 Function: void vram_debug(WORD error)

 By: David Schwartz

 Date: Jan 1995

 Parameters: None

 Returns: None

 Description: 	debug error messages
******************************************************************************/
void vram_debug(WORD error)
{
	volatile short wait;

	switch (error)
	{
		case VRAM_VFREE_TPAGE:
			CPRT("TPAGE NOT FOUND DURING VFREE");
			break;
		case VRAM_VFREE_ZERO:
			CPRT("TRYING TO FREE ZERO SIZED BLOCK");
			break;
		case VRAM_VALLOC_INIT_FAILED:
			CPRT("RAN OUT OF TPAGES IN INIT");
			break;
		case VRAM_VALLOC_NOSPACE:
			CPRT("NO SPACE IN VRAM TO FIT REQUESTED RECT");
			break;
		default:
			CPRT("UNKNOWN VRAM ALLOC ERROR!");
			break;
	}

	wait=0;
	while (wait==0)
	{
		POLLING;
	}
}
#endif /* VRAM_DEBUG */

/*********************************************************************
						 VRAM MEMORY CACHE MANAGER
 *********************************************************************/

/******************************************************************************
 Function: void init_vram_cache(WORD *vmem_list)

 By: David Schwartz

 Date: Dec 1994

 Parameters: vmem_list ptr to vram data

 Returns: None

 Description: 	resets vram cache manager & inits memory manager
******************************************************************************/
void init_vram_cache(WORD *vmem_list)
{
	int i;

	valloc_init(vmem_list);

	/* init cache */
	for (i=0;i<MAX_VRAMMEM;i++)
		memset(vram_cache+i,0,sizeof(VRAMMEM));

	return;
}

/******************************************************************************
 Function: WORD alloc_vram_mem(void *img,OHEADER *tinfo)

 By: David Schwartz

 Date: Dec 1994

 Parameters: img - img to load
			 tinfo - ptr to return area for data
			 tinfo->t_width - width of object
			 tinfo->t_height - height of object

 Returns: texture info in *tinfo
		  cache entry number (0-MAX_VRAMMEM-1)
		  VCACHE_FAILED  - unable to alloc in cache (TRAP)

 Description: 	searches the cache to see if img has been loaded
				if found
						inc cnt if not permanent
					returns back header info
				not found
					allocates vram and load image
					inc cnt
					retruns back header info

NOTE: if there is not enough cache entries, function will TRAP!
******************************************************************************/
WORD alloc_vram_mem(void *img,OHEADER *tinfo,WORD ventry)
{
	WORD i=0;
	WORD size;
	WORD vx,vy;
	ADDRESS *vptr;
	VRAMMEM *vmem;
	WORD vfind=VCACHE_FAILED;			// find first available vmem, in case img not already loaded

	while (i<MAX_VRAMMEM)
	{
		vmem=vram_cache+i;

		if (vfind==VCACHE_FAILED && vmem->v_cnt==0)	// find avail entry
			vfind=i;

		if (vmem->v_imgptr==img && vmem->v_cnt>0)
		{
			if (ventry==i)					// find entry current allocated to object, do nada
				return(i);
			else dealloc_vram_mem(ventry);	// release prev image

			/* found entry */
			*tinfo=vmem->v_textinfo;		// copy texture info

			if (vmem->v_cnt!=VRAM_PERM)		// if not perm, inc count
				vmem->v_cnt++;

			return(i);
		}
		else i++;
	}

	/* no entry found */
	if (vfind==VCACHE_FAILED)
		return(VCACHE_FAILED);
	else
	{
		/* release prev image */
		dealloc_vram_mem(ventry);

		/* setup a new entry */
		vmem=vram_cache+vfind;

		vmem->v_cnt=1;						// entry has one obj using it

		/* memory allocation code */

#if 0
		/* NOTE THE ONE FACTOR IS BECAUSE OF THE SONY TEXTURE PAGE FLIP BUG CANT BE LEFT CORNER ALIGNED */
		/* two factor because you cant load an even width texture on an odd boundary */
		tinfo->tpage=valloc_rect(tinfo->t_width+2,tinfo->t_height+1,&(tinfo->t_xoffset),&(tinfo->t_yoffset));

		tinfo->t_xoffset+=2;
		tinfo->t_yoffset++;
#endif

		tinfo->tpage=valloc_rect(tinfo->t_width+2,tinfo->t_height,&(tinfo->t_xoffset),&(tinfo->t_yoffset));
		tinfo->t_xoffset+=2;

		/* load data */
		vx=((tinfo->tpage&0x000f)<<6)+((tinfo->t_xoffset)/2);
		vy=((tinfo->tpage&0x0010)<<4)+tinfo->t_yoffset;

		vmem->v_textinfo=*tinfo;			// return info has all data, set cache


/*************************************************
	COMPRESSION CODE
*************************************************/
		vptr=(ADDRESS *)uncompress_image((BYTE *)(img));
/*************************************************
	COMPRESSION CODE
*************************************************/
		vmem->v_imgptr=img;

		LOADTPAGE(vptr,TEXTURE_MODE,TRANS_RATE,vx,vy,(tinfo->t_width+3)&(~3),tinfo->t_height);

		return(vfind);
	}
}

/******************************************************************************
 Function: void dealloc_vram_mem(WORD vcache_entry)

 By: David Schwartz

 Date: Dec 1994

 Parameters: vcache entry - index into cache of entry used

 Returns: None

 Description: 	removes one from cnt of entry supplied and
				if cnt goes to zero, unallocates block if not
				perm
******************************************************************************/
void dealloc_vram_mem(WORD vcache_entry)
{
	VRAMMEM *vmem;
	OHEADER *vhead;
	WORD size;

	if (vcache_entry==VCACHE_FAILED)				// only dealloc inited stuff
		return;

	vmem=vram_cache+vcache_entry;

	if (vmem->v_cnt!=VRAM_PERM) 					// if perm ignore dealloc
	{
		if ((--vmem->v_cnt)==0)
		{
			vmem->v_imgptr=NULL;					// clear key

			/* cnt of objs using image is none, release vram mem */
			vhead=&(vmem->v_textinfo);

			/* release vram memory */

			/* -1 factor is to deal with the fact that we allocate the texture 1 less then then
			   xoffset, however in fixing the sony flip bug we allocated 1 more */
			vfree_rect(vhead->tpage,vhead->t_xoffset-2,vhead->t_yoffset);
		}
	}

	return;
}

/******************************************************************************
 Function: void flush_vram_mem(void)

 By: David Schwartz

 Date: Dec 1994

 Parameters: None

 Returns: None

 Description: 	looks at all vram cache entries and flushes any non perm vram
******************************************************************************/
void flush_vram_mem(void)
{
	short i=0;
	VRAMMEM *vmem;
	OHEADER *vhead;

	while (i<MAX_VRAMMEM)
	{
		vmem=vram_cache+i;

		if (vmem->v_cnt!=VRAM_PERM && vmem->v_cnt!=0) 					// if perm ignore dealloc
		{
			/* flush and non perm non zero entry */
			vmem->v_cnt=0;

			vmem->v_imgptr=NULL;					// clear key

			/* cnt of objs using image is none, release vram mem */
			vhead=&(vmem->v_textinfo);

			/* release vram memory */
			/* -1 factor is to deal with the fact that we allocate the texture 1 less then then
			   xoffset, however in fixing the sony flip bug we allocated 1 more */
			vfree_rect(vhead->tpage,vhead->t_xoffset-2,vhead->t_yoffset);
		}

		i++;						// next entry
	}
	return;
}

/******************************************************************************
 Function: WORD alloc_vram_perm(OIMGTBL *permimg,void *heapptr,WORD vx,WORD vy)

 By: David Schwartz

 Date: Dec 1994

 Parameters: permimg - image info to perm load
			 heapptr - start of heap for image (addr base)
			 vx - x pos in vram to load image
			 vy - y pos in vram to load image

 Returns: vcache entry to load data

 Description: 	find an empty slot in vram cache mem and allocates a perm
				entry based on supplies data

NOTE: if there is not enough cache entries, function will TRAP!
******************************************************************************/
WORD alloc_vram_perm(OIMGTBL *permimg,void *heapptr,WORD vx,WORD vy)
{
	short i=0;
	VRAMMEM *vmem;
	OHEADER *vtinfo;
	ADDRESS *frame;

	while (i<MAX_VRAMMEM)
	{
		vmem=vram_cache+i;

		if (vmem->v_cnt==0)							// find avail entry
		{
			vtinfo=&(vmem->v_textinfo);				// ptr to header info

			vmem->v_cnt=VRAM_PERM;					// set entry as permanent

			/* setup header info */
			(ADDRESS *)(vmem->v_imgptr)=frame=(ADDRESS *)COMPUTE_ADDR(heapptr,(LONG)(permimg->isagptr));		// get img ptr
			vtinfo->t_width=permimg->size.u.xpos;				   // set width of image
			vtinfo->t_height=permimg->size.u.ypos;				   // set height of image
			vtinfo->t_xoffset=(vx%128)*2;						   // set x offset within tpage
			vtinfo->t_yoffset=(vy%256);							   // set y offset within tpage

/* if ever used need to implement compression */
			LOCKUP;

			/* load tpage */
			vtinfo->tpage=LOADTPAGE(frame,TEXTURE_MODE,TRANS_RATE,vx,vy,(vtinfo->t_width+3)&(~3),vtinfo->t_height);

			return(i);
		}
		else i++;
	}

	ermac(15);
	return(VCACHE_FAILED);
}

/******************************************************************************
 Function: void alloc_vram_perm_bg_list(ADDRESS *bglist,void *heap)

 By: David Schwartz

 Date: Feb 1995

 Parameters: bglist - list of images to alloc
			 heap -  ptr to start of data heap for images

 Returns: None

 Description: 	This routine reads through a 0 terminated lists and allocs a vcache entry
******************************************************************************/
void alloc_vram_perm_bg_list(ADDRESS *bglist,void *heap)
{
	while (*bglist!=NULL)
	{
		alloc_vram_perm_bg((OIMGTBL *)(GET_LONG(bglist)),heap);
		bglist++;
	}

}

/******************************************************************************
 Function: WORD alloc_vram_perm_bg(OIMGTBL *permimg,void *heapptr)

 By: David Schwartz

 Date: Dec 1994

 Parameters: permimg - image info to perm load
			 heapptr - start of heap for image (addr base)
			 vx - x pos in vram to load image
			 vy - y pos in vram to load image

 Returns: vcache entry to load data

 Description: 	find an empty slot in vram cache mem and allocates a perm
				background animation entry based on supplies data.  This routine
				does not load the image into memory since it should already be there.
				The isagptr of the permimg info is different then that of the
				normal image.  The ptr now pts to an OHEADER entry.  This entry will contain
				all information located for the bg ani piece.  This routine must be called for all
				ani piece that use this structure, before that are requested to draw.  If this happens then
				the program will work fine.  If it does not happen, and a request to draw is made using alloc_vram_mem
				before the perm image is setup the program will crash and burn since the isagptr is different in alloc_vram_mem.

 Structure Info:
	Background animation OIMGTBL entry setup:

	bg_anim_label
		dw	bgheader_ptr
		dh	width,height,anixoff,aniyoff
		dw	palptr [optional]

	bgheader_ptr
		dh	tpage offset,tpage xoff,tpage yoff,0

NOTE: if there is not enough cache entries, function will TRAP!
******************************************************************************/
WORD alloc_vram_perm_bg(OIMGTBL *permimg,void *heapptr)
{
	short i=0;
	VRAMMEM *vmem;
	OHEADER *vtinfo;
	OBGHEADER *bghead;

	while (i<MAX_VRAMMEM)
	{
		vmem=vram_cache+i;

		if (vmem->v_cnt==0)							// find avail entry
		{
			vtinfo=&(vmem->v_textinfo);				// ptr to header info

			vmem->v_cnt=VRAM_PERM;					// set entry as permanent

			/* setup header info */
			(ADDRESS *)(vmem->v_imgptr)=(ADDRESS *)bghead=(ADDRESS *)COMPUTE_ADDR(heapptr,(LONG)(permimg->isagptr));
			vtinfo->t_width=permimg->size.u.xpos;				   // set width of image
			vtinfo->t_height=permimg->size.u.ypos;				   // set height of image

			/* setup tpage info */
			vtinfo->t_xoffset=bghead->t_xoffset;						   	// set x offset within tpage
			vtinfo->t_yoffset=bghead->t_yoffset;							// set y offset within tpage
			vtinfo->tpage=(bghead->tpage<<1)+bkgd_base_tpage;				// factor in base background tpage

			return(i);
		}
		else i++;
	}

	ermac(15);
	return(VCACHE_FAILED);
}


