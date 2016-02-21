/******************************************************************************
 File: mkbkmod.s

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Background Module Composition Tables 
******************************************************************************/


/* 
 *	Format of Module Table 

 *	WORD	autoerase color
 *	WORD	ground y position
 *	WORD	initial world y coord
 *	WORD	initial world x coord
 *	WORD	scroll left limit
 *	WORD	scroll right limit
 *	PTR	ptr to a function that will initiate various processes for the background module 
 *	PTR	ptr to a table of longs used to specify scroll rates
 *	PTR	ptr to a table of two longs per record. [object list for module, worldtlx variable to use]
 *	PTR	ptr to first background mod variable to fill
 *	
 *	FOLLOWING DATA REPEATED DEPENDING ON BACKGROUND:
 *	PTR	command to skip background level 
 *	
 *	PTR	ptr to background module (OMODULE) data
 *	
 *	PTR	command to center x coord of background modules
 *	
 *	PTR,PTR	list of background modules and world x variables used to center
 */	


/******************************************************************************
 *
 * MODULE:	ARENA (MK II)
 *
 ******************************************************************************/
