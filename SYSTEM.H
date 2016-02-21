/******************************************************************************
 File: system.h

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Common System Defines
*******************************************************************************/

#ifndef __system_h__
#define __system_h__

/*
 *      Target System
 */
#define SONY_PSX 1

/*
 *      System State Defines
 */

/* clear these on final build */
#define POLL_TYPE 1                                     /* debuggin polling for sony box */
#define DEBUG 1
#define MK_EJBBUG 1                                     /* quick debugging stuff, ARCADE STUFF */
#define PAL_VERSION 0                                   /* set for EUROPE */
#define JAP_VERSION 0									/* set for JAPAN */

#define FINAL_BRIEF 1                                   /* SET ON FINAL VERSION/BETA */
/* also change psxcd.h & makefile for full opts & link file  */
/* clear these on final build */
#define CD_DEBUG        0                      /* if set let cd dbug happen */
#define VS_OFF  0                                                       /* CLEAR IF WANT LOAD MESSAGE ON TOP OF VS SCREEN & LADDER */
#define PROFILE 0                                                       /* enable the poormans profiler */
#define DEBUG_GRID      0                                               /* debug grid for display lineup */
#define CLOCK_ON        1                                               /* enable clock */
#define SHADOW_MODE 0                                           /* if set then have shadows */
#define COLLISION_BOX 0                                         /* if set show collisin boxes over fighters */
#define SCREEN_SHOTS    0                               /* set if want select to pause for screen shots */
#define SEL_BKGD        1                                               /* allow player to choose background cheat */
#define MULTI_OFF 1                                             /* if set, then mutli part objs are singles and alloc is different */
#define SMOKE_PAGE      0                                       /* set if want ultimate kombat code */
#define ONE_FLOOR_BUFFER        1               /* set for only one floor buffer */
#define BACKGROUND_COMPRESS 1                   /* if set then TPG files have been compressed */

/*
 *      Miscellaneous Defines
 */
#define SYSTEM_INIT_MARKER      0x01a2b3c4d                     /* system init mark */

#define SYSTEM_CARRY_SET        0x8000                          /* carry set bit on */
#define SYSTEM_CARRY_CLR        0x4000                          /* carry set bit off */
#define SYSTEM_CARRY_MASK       (SYSTEM_CARRY_SET+SYSTEM_CARRY_CLR)     /* create mask for carry bits */

#define SYSTEM_ZERO_SET 0x0002                          /* zero set bit on */
#define SYSTEM_ZERO_CLR 0x0001                          /* zero set bit off */
#define SYSTEM_ZERO_MASK        (SYSTEM_ZERO_SET+SYSTEM_ZERO_CLR)       /* create mask for zero bits */

#define STKSIZE (256)                                                   /* process stack size */

/*
 * Debugging Macros
 */
#if DEBUG
#define CPRT(s)                 //printf("%s\n",s)
#else
#define CPRT(s)
#endif /* DEBUG */

#if POLL_TYPE
#define POLLING pollhost()
#else
#define POLLING
#endif /* MK_EJBBUG */

/*
 *      Extract the low and high from a long
 */
#define LOWORD(theValue)        ((unsigned short)(theValue))
#define HIWORD(theValue)        ((unsigned short)((unsigned long)(theValue)>>16))

/*
 *      Make it easy to declare/initialize global variables
 *      example usage: __EXTERN__ long foo __INIT__(1);
 *      this becomes:
 *              extern long foo;                        for declaring access to variable(done may times)
 *      or:
 *              long foo=1;                                     for defining storage for the variable (done once)
 */
#ifdef DECLARE_GLOBALS                          /* declare variables as globals */
#define __EXTERN__                                      /* define storage for the variable */
#define __INIT__(initVal) = initVal     /* set compile time initial value */
#else                                                           /* declare variables as externs */
#define __EXTERN__ extern                       /* declare an extern reference to the variable */
#define __INIT__(initVal)                       /* ignore initial value */
#endif /* DECLARE GLOBALS */

/*
 *      Declare some useful types
 */
#define TRUE    1
#define FALSE   0

typedef void (*FUNC)();                         /* void function(void) */
typedef FUNC JUMPTBL;                           /* jump table definition */

typedef unsigned short (*FUNCW)();                      /* void function with a word return */

#define LOCKUP while(1) POLLING

/* scale macros */
#define SCF_X 80                                                        /* scale factor percent x */
#define SCF_Y 85                                                        /* scale factor percent y */

#define SCALE_FACTOR_X ((float)SCF_X/100)                       /* scale factor between sony and arcade X */
#define SCALE_FACTOR_Y ((float)SCF_Y/100)                       /* scale factor between sony and arcade Y */


#define SCX(_x) ((long)(SCALE_FACTOR_X*(long)(_x)))
#define SCY(_y) ((long)(SCALE_FACTOR_Y*(long)(_y)))

#define SCX_V(_x,_v) ((long)((_v)*(long)(_x)))
#define SCY_V(_y,_v) ((long)((_v)*(long)(_y)))


#endif /* __system_h__ */
