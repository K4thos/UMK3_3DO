/******************************************************************************
 File: mkglobal.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III GLOBAL RAM
******************************************************************************/

#define DECLARE_GLOBALS


#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif

#include "mkbkgd.h"
#include "mkos.h"
#include "mkinit.h"
#include "mkutil.h"
#include "mkobj.h"
#include "mkgame.h"
#include "mkcombo.h"
#include "moves.h"
#include "mkamode.h"
#include "mkmain.h"
#include "mksel.h"
#include "mkreact.h"
#include "mkjoy.h"
#include "mkdrone.h"
#include "mkguys.h"
#include "mkpal.h"
#include "mkvs.h"
#include "mkrepell.h"
#include "mkslam.h"
#include "mkblood.h"
#include "mkani.h"
#include "mkprop.h"
#include "mkzap.h"
#include "mksound.h"
#include "mkcanned.h"
#include "mkbonus.h"
#include "valloc.h"
#include "mkscore.h"
#include "mktext.h"
#include "mkfatal.h"
#include "mkanimal.h"
#include "mkfriend.h"
#include "mkend.h"
#include "mkmark.h"
#include "mkdiag.h"
#include "mkfade.h"
#include "mkladder.h"
#include "mkfile.h"
#include "mkbio.h"
#include "mkboss.h"
#include "mkstat.h"
#include "mkopt.h"
#include "mkmode.h"
