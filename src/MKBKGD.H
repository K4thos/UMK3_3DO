/******************************************************************************
 File: mkbkgd.h

 By: David Schwartz

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Background Header
******************************************************************************/

#ifndef __mk_bkgd_h__
#define __mk_bkgd_h__

/*
 * 	DEFINITIONS
 */

/* background modules */
#define BKGD_SUBWAY_MOD		0x00
#define BKGD_STREET_MOD		0x01
#define BKGD_BANK_MOD		0x02	
#define BKGD_ROOF_MOD		0x03
#define BKGD_TOWER_MOD		0x04
#define BKGD_BRIDGE_MOD		0x05
#define BKGD_SOUL_MOD		0x06
#define BKGD_BELL_MOD		0x07
#define BKGD_TEMPLE_MOD		0x08
#define BKGD_GRAVE_MOD		0x09
#define BKGD_PIT_MOD		0x0a
#define BKGD_WATER_MOD		0x0b
#define BKGD_HADES_MOD		0x0c
#define BKGD_CAVE_MOD		0x0d
#define BKGD_DESERT_MOD		0x0e
#define BKGD_MK3_BUYIN		0x0f
#define BKGD_LOST_MOD		0x10
#define BKGD_HIDDEN_MOD		0x11		// portal
#define BKGD_MK3_HSCORE		0x12
#define BKGD_LADDER_MOD		0x13
#define BKGD_VS_MOD			0x14
#define BKGD_MK3_COIN		0x15
#define BKGD_FIRE_MOD		0x16		// fire sky swirl
#define BKGD_NOWALL_MOD		0x17
#define BKGD_MK3_RGRAD		0x18
#define BKGD_MK3_GGRAD		0x19
#define BKGD_MK3_BGRAD		0x1a
#define BKGD_COIN_MODE			0x1b
#define BKGD_MK3_TITLE 		0x1c
#define BKGD_MK3_PSEL		0x1d
#define BKGD_MK3_LEGAL		0x1e
#define BKGD_SKDIE1_MOD		0x1f
#define BKGD_SKDIE2_MOD		0x20
#define BKGD_SKPIT_MOD		0x21
#define BKGD_MOCKPIT_MOD	0x22
#define BKGD_MK3_OPTION		0x23
#define BKGD_MK3_COINOPT	0x24





/* scroll parameters */
#define SCROLL_FAR 100	// SCX(100)
#define SCROLL_MIDZONE 150 // SCX(150)
#define SCROLL_MZ_OUT ((SCRRGT-SCROLL_MIDZONE)/2)
#define SCROLL_Y_DOWN SCY(0x18000)				/* down scroll rate */
#define SCROLL_BARRIER_PAD SCY(40)+4		/* upward padding for scrolling */

#define SCROLL_GRACE_SIZE SCX(40)			/* scroll grace pixel size */

#define END_LIST 0 							/* background command end of data */
#define SKIP_BAKMOD	1						/* background command to skip module */
#define FORCE_EXIT -1						/* background command to force exit, end of data */
#define CENTER_X -2							/* background command to center x */

#define NULL_IRQSKYE 444					/* null background color, leave only */

#define BACKGROUND_SLEEP_TIME 8				/* # of games ticks before updating background dislay lists */
#define BACKGROUND_WIDEST_BLOCK 250			/* widest block size */

#define DUMB_BACKGROUND 0xffffffff			/* dumb background, terminates background update for pass */

#define BLSTSTRT 0x8000
#define BLSTEND 0x8001

#define MAX_BACKGROUNDS 0x0b					/* maximum # of backgrounds in the game */

/* background block */
typedef struct o_block {
	WORD xoffset;						/* x offset within module */
	WORD yoffset;						/* y offset within module */
	WORD zdepth;						/* z priority within module */
	WORD flags;							/* block flags, bit 0 = hflip, bit 1 = v flip, upper byte=CLUT offset*/
	WORD hdr_index;						/* index into header array, indexes correct header (tile) */
} OBLOCK;

/* background header/tile */
typedef struct o_header {
	WORD tpage;							/* texture page */
	WORD t_xoffset;						/* x offset within texture page */
	WORD t_yoffset;						/* y offset within texture page */
	WORD t_width;						/* texture width */
	WORD t_height;						/* texture height */
} OHEADER;

/* background animation header */
typedef struct obg_header {
	WORD tpage;							/* texture page */
	WORD t_xoffset;						/* x offset within texture page */
	WORD t_yoffset;						/* y offset within texture page */
	WORD padding;
} OBGHEADER;

/* layout for player multipart ram */
typedef struct p_ram {
	ADDRESS prsagptr;							// address to image data
	XYTYPE prsize;							// image size info Y:X
	WORD prflags;								// flip flags and other stuff (bits 0=hflip, bits1=vflip), padded
	XYTYPE prpos;							// y:x delta offsets from object position for image
} P_RAM;

/* background module header definition */
typedef struct o_module {
	WORD xsize;							/* width of module */
	WORD ysize;							/* height of module */
	LONG blockcnt;						/* # of blocks in module */
	OBLOCK *block_ptr;					/* ptr to array of blocks */
	OHEADER *header_ptr;				/* ptr to array of headers */
	WORD **clut_ptr;					/* ptr to table of cluts */
} OMODULE;

typedef struct oimgtbl {
	void *isagptr;				/* offset to actual image data */
	XYTYPE size;				/* x,y size of object */
	short anixoff;				/* animation x offset */
	short aniyoff;				/* animation y offset */
//	WORD iflags;				/* image flags */
} OIMGTBL;

/* object init table definition */
typedef struct oinit_tbl {
	OIMGTBL *oimgptr;				/* ptr to image data */
	void *opalptr;				/* ptr to palette data */
	WORD oid;					/* object id */
	short oxoff;				/* x offset */
	short oyoff;				/* y offset */
	short ozdepth;				/* z depth */
} OINIT_TBL;

/* dumb animator w init table struct */
typedef struct dtbl_init {
	WORD ani_speed;
	void *img;
	short x;
	short y;
	void *a_list;
	struct object **baklst;
} DTBL_INIT;

/* dumb animator init table struct */
typedef struct dtbl_inita {
	WORD ani_speed;
	short x;
	short y;
	void *a_list;
	struct object **baklst;
} DTBL_INITA;


/* Object Structure */
typedef struct object {
	struct object *olink;			/* link to next object, THIS MUST BE FIRST FILED IN STRUCTURE */
	//void *oplink;					/* ptr to misc data */
	struct process *oslink;			/* process slave link */
#define oplink oslink
/* ograv must be the first field after the above links, else mod copy_obj routine */
	POS ograv;						/* gravity */
	POS oxvel;						/* x velocity 16.16 */
	POS oyvel;						/* y velocity 16.16 */
	POS oxpos;						/* x position of object (16.16) */
	POS oypos;						/* y position of object (16.16) */
	short ozval;					/* z position of object (16.0) (priority with list) */
	LONG oblockmask;				/* mask to access correct bit with block bit array */
	LONG oflags2;					/* game flags, A7 */
	OHEADER header;					/* object display information */
	WORD oid;						/* object id */
	ADDRESS *oshape;				/* multipart shape */
	WORD ochar;						/* character id */
	void *oimg;						/* pointer to image data table	*/
	WORD opal;						/* palette id #, A8 */
	WORD oconst;					/* const color */
	XYTYPE osize;					/* size of object y:x, A9 */
	XYTYPE oscale;					/* scale factor y:x */
	WORD oflags;					/* DMA control, flip stuff, A11 */
	WORD ovcache;					/* entry in vcache */
	void *osag;						/* ptr to source address, A12 */
	void *oheap;					/* ptr to start of heap for object */
#if MULTI_OFF
	P_RAM opinfo;					/* mod from arcade, multi part objs are really one part */
#endif
} OBJECT;

/* plane table information structure */
typedef struct plane_tbl {
	LONG *bakbit;						/* ptr to LONG array of object display bits */
	OBJECT **baklst;					/* ptr to a ptr to bakground list for plane */
	POS *world_y;						/* ptr to world y variable for plane */
	POS *world_x;						/* ptr to world x variable for plane */
	void *mod_ptr;						/* ptr to a ptr to module block */
} PLANETBL;

/* floor call */
typedef void (*FLRCALL)(long);

/*
 *	RAM
 */
__EXTERN__ WORD f_street;					/* indicate street floor uncompressed */
__EXTERN__ WORD load_bkgd;				/* current bkgd # to load */
__EXTERN__ WORD bkgd_base_x;
__EXTERN__ short f_only_t;				/* flag: only show background list _t's */
__EXTERN__ short ground_y;				/* ground level y coord */
__EXTERN__ short ceiling_y;				/* ceiling level y coord */
__EXTERN__ short left_edge;				/* left scroll limit */
__EXTERN__ short right_edge;				/* right scroll limit */
__EXTERN__ long *scrtab;				/* ptr to scroll tables */
__EXTERN__ void *dlists;				/* ptr to display lists tables */
__EXTERN__ void *bakmods[8];			/* array of ptrs to background modules */
__EXTERN__ WORD irqskye;				/* default background color */
__EXTERN__ void *dark_dlist;

/* skewing floor related ram */
__EXTERN__ WORD f_skew;				// flag: skew the ground
__EXTERN__ POS skew_dx;
__EXTERN__ long skew_oc;			// how far off (and direction) center
__EXTERN__ short skew_y; 			// starting y position of floor
__EXTERN__ short skew_height;		// floor height
__EXTERN__ WORD skew_stack;			// # of times to stack floor
__EXTERN__ WORD skew_constpal;		// skew: const:pal
__EXTERN__ FLRCALL skew_calla;			// skew calla
__EXTERN__ BYTE *skew_sag;		// skew ground image pointer
__EXTERN__ ADDRESS *skew_scroll;	// pointer to which scroller skew uses


/*
 *	PROTOTYPES
 */
void do_background(WORD background);
void background_clear(void);
void init_background_module(void *);
void bkgd_texture_load(WORD tsx,WORD tsy);
void vram_texture_load(WORD tsx,WORD tsy,ADDRESS *text_tbl);
void setup_floor_info(void *modptr);
void use_next_y(void);
void use_worldtly(void);
void use_worldtly_t(void);
void check_only_t(void);
void background_routine(void);
void multi_plane(void);
void display_delete(OBJECT **baklst,XYTYPE *tl,XYTYPE *lr);
void display_modify(OBJECT **baklst,void *bak_info,LONG *bakbit,XYTYPE *tl,XYTYPE *lr);
void display_add(OBJECT **baklst,OMODULE *bakmod,LONG *bakbit,XYTYPE *tl,XYTYPE *lr,XYTYPE *start);
int add_block(OBJECT **baklst,LONG *bakbit,OBLOCK *bakblk,OHEADER *hdr_info,XYTYPE *start,LONG blkmask,WORD **blkclt);
void background_blast(void);
OBJECT *gcb3(void *frame);
void calla_return(void);
void skew_7(long scroll);
void wind_sound(void);
void garbage_straight(void);
void garbage_loop(void);
void paper_fly_offscreen(short pa1,short pa2);
void paper_loop_sleep(long *pa1,long *pa2);
void paper_glide(void);
void get_paper_object(void);
void paper_start_position(void);
inline void a4_to_world_coord(short pa0,short pa1);
void dumb_animator(void);
inline void stop_scrolling(void);
void scroll_a9(void);
void scroll_to_x_a9(void);
void scroll_center(void);
void scorp_tele_scroll(void);
void keep_centered_on_a8(void);
void kcen_right(void);
void kcen_left(void);
void scroller(void);
void handle_y_scrolling(void);
void scroll_choose(WORD neg_flag);
void ss_10000(WORD neg_flag);
void ss_20000(WORD neg_flag);
void ss_28000(WORD neg_flag);
void ss_30000(WORD neg_flag);
void ss_40000(WORD neg_flag);
void ss_60000(WORD neg_flag);
void ss_70000(WORD neg_flag);
void ss_a0000(WORD neg_flag);
void ss_80000(WORD neg_flag);
void create_dumb_animators_w(DTBL_INIT *tbl,void *heap);
void create_dumb_animators(DTBL_INITA *tbl,void *heap);

void pit_scroll_proc(void);
void bridge_list_scroll(OBJECT **pa0,long pa1);
void bridge_list_fixed(OBJECT **pa0,short pa5);
void saw_spin_proc(void);
void build_pit_saw(void);

/* IN FILE MKBT.S */
extern void *table_o_mods[];
extern BYTE table_o_levels[];

/* external display list setups */
extern void *dlists_bogus[];
extern void *dlists_objlst[];
extern void *dlists_end[];

/*
 *	MACROS
 */
#define bak1mods bakmods[0]
#define bak2mods bakmods[1]
#define bak3mods bakmods[2]
#define bak4mods bakmods[3]
#define bak5mods bakmods[4]
#define bak6mods bakmods[5]
#define bak7mods bakmods[6]
#define bak8mods bakmods[7]


#define get_ceiling_block3(_lab) gcb3(_lab)
#define get_ceiling_block2(_lab) gcb3(_lab)
#define get_ceiling_block1(_lab) gcb3(_lab)

#endif /* __mk_bkgd_h__ */
