/******************************************************************************
 File: valloc.h

 By: David Schwartz

 Date: Jan  1995

 (C) Williams Entertainment

 Mortal Kombat III VALLOC header
******************************************************************************/

#ifndef __mk_valloc_h__
#define __mk_valloc_h__

/*
 * 	DEFINITIONS
 */
#define VRAM_DEBUG	1

#define MIN_VRAM_ALLOC_SIZE	16		// each tpage is divided into this many number of squares
#define MIN_VRAM_ALLOC_SHIFT 4		// shift to simulate the divide of (MIN_VRAM_ALLOC_SIZE)

#define MAX_DYNAMIC_TPAGE	6		// max # of tpages supported for dynamic memory



/* error stuff */
#define VRAM_VALLOC_OK	0			// no problems
#define VRAM_VALLOC_INIT_FAILED -1	// init failed
#define VRAM_VFREE_ZERO -2			// tried to free a zero size block
#define VRAM_VFREE_TPAGE -3			// tpage does not exist
#define VRAM_VALLOC_NOSPACE -4		// no space in vram

/* structure for each square in the tpage map */
typedef union vmeminfo {
	WORD v_xyinfo;				// y:x
	struct {
		BYTE v_x;				// width of allocated block this piece is containted int
		BYTE v_y;				// height of allocated block this piece is containted int
	} u;
} VMEMINFO;

/* structure for each tpage entry */
typedef struct vpageinfo {
	VMEMINFO *vptr;				// ptr to an arry for the tpage
	WORD vtpage;				// base tpage info
	WORD vbasex;				// base x pos in vram
	WORD vbasey;				// base y pos in vram
} VPAGEINFO;


/* VRAM memory allocation structure */
typedef struct v_vrammem {
	void *v_imgptr;							// ptr to start addr of image
	WORD v_cnt;								// to # objects using entry (-1 = permanent)
	OHEADER v_textinfo;						// texture info for image
} VRAMMEM;

#define MAX_VRAMMEM 160						// # of entries in manager cache
#define VRAM_PERM 0xffff					// vram cache entry is permanent
#define VCACHE_FAILED 0xffff				// indictate no cache

/*
 *	RAM
 */
__EXTERN__ VRAMMEM vram_cache[MAX_VRAMMEM];	/* cache of entries in vram memory manager */

/*
 *	PROTOTYPES
 */
WORD valloc_init(WORD *free_mem);
WORD valloc_rect(WORD width,WORD height,WORD *x_off,WORD *y_off);
WORD vfree_rect(WORD tpage,WORD x_off,WORD y_off);
void vram_debug(WORD);
short vscan_page(VPAGEINFO *vptr,WORD width,WORD height,WORD *x_off,WORD *y_off);
short vsearch_rect(VMEMINFO *vmptr,WORD width,WORD height);
void vmark_rect(VMEMINFO *vmptr,WORD width,WORD height);

/* vram cache prototypes */
void init_vram_cache(WORD *);
WORD alloc_vram_mem(void *,OHEADER *,WORD);
void dealloc_vram_mem(WORD);
WORD alloc_vram_perm(OIMGTBL *,void *,WORD,WORD);
WORD alloc_vram_perm_bg(OIMGTBL *,void *);
void alloc_vram_perm_bg_list(ADDRESS *,void *);
void flush_vram_mem(void);

/*
 *	MACROS
 */
#define alloc_cache(_img,_heap,_obj) \
		(_obj)->header.t_width=((OIMGTBL *)(_img))->size.u.xpos, \
		(_obj)->header.t_height=((OIMGTBL *)(_img))->size.u.ypos, \
		(_obj)->ovcache=alloc_vram_mem((void *)COMPUTE_ADDR((_heap),(LONG)(((OIMGTBL *)(_img))->isagptr)),&((_obj)->header),(_obj)->ovcache)

#endif /* __mk_valloc_h__ */

