/******************************************************************************
 File: filename (this file)

 Date: date

 Source: arcade filename

 (C) Williams Entertainment

 -COMMENT-
******************************************************************************/

/************************************
Revisions:
	DHS	11/20/95	Created file
************************************/
#ifndef __mkcoin_h__
#define __mkcoin_h__


/*************************************************************************
 * DEFINES
 *************************************************************************/ 
#define TREEYSP	(SCY(0x20))
#define TREEXSP	(SCX(0x5a))

#define TREEORGX (SCX(0x30))
#define TREEORGY (SCX(0x10))

#define TREEORG2X (TREEORGX+TREEXSP)
#define TREEORG2Y (TREEORGY+SCY(0x10))

#define TREEORG3X (TREEORG2X+TREEXSP)
#define TREEORG3Y (TREEORG2Y+SCY(0x20))

#define TREEORG4X (TREEORG3X+TREEXSP)
#define TREEORG4Y (TREEORG3Y+SCY(0x30))

typedef struct 
{ 
	short t_x;
	short t_y;
	WORD *t_wrd;
} TREES;

/************************************************************************
 * RAM
 ************************************************************************/
__EXTERN__ WORD twinners[7];

/************************************************************************
 * PROTOTYPES
 ************************************************************************/
void mode_select(void);
void move_cursor(short pa0);
void change_letter_colors(WORD *pa1);
void pds_coin(OBJECT **pa10,char *pa8,short x,short y);

void tournament_tree(WORD pa10); 
void ttext_flasher(void);
void ttf_sleep(OBJECT *pa0);


/************************************************************************
 * MACROS
 ************************************************************************/

#endif /* __mkcoin_h_ */