#ifndef _PSXSND_H
#define _PSXSND_H

#include "wessapi.h"
#include "seqload.h"
#include "lcdload.h"

#include "soundhw.h"

#define WMD_SIZE 43000
#define SRAM_START 4112
#define SRAM_FIGHTER_1 221312
#define SRAM_FIGHTER_2 370752

#define SOUNDLOADDATA_SELECT     0
#define SOUNDLOADDATA_HIDGAME    1
#define SOUNDLOADDATA_RAND       2
#define SOUNDLOADDATA_SMOKECOD   3
#define SOUNDLOADDATA_LADDER     4
#define SOUNDLOADDATA_GAMEOVER   5
#define SOUNDLOADDATA_BUYIN      6
#define SOUNDLOADDATA_VS         7
#define SOUNDLOADDATA_SKSPCIAL   8
#define SOUNDLOADDATA_SKPIT      9
#define SOUNDLOADDATA_SKBELL     10
#define SOUNDLOADDATA_SKTRAIN    11
#define SOUNDLOADDATA_SKSTREET   12
#define SOUNDLOADDATA_SKBRIDGE   13
#define SOUNDLOADDATA_SKBANK     14
#define SOUNDLOADDATA_SKROOF     15
#define SOUNDLOADDATA_SKSOUL     16
#define SOUNDLOADDATA_SKCHURCH   17
#define SOUNDLOADDATA_SKGRAVE    18
#define SOUNDLOADDATA_SKHID      19

extern int sram_pos;

extern unsigned long PsxSoundInit(char *ptrtowmd);
extern void PsxSoundExit(void);

extern void PsxSoundLoadClear(void);
extern void PsxSoundLoadData(int lcdnum);
extern void PsxSoundLoadFighter1(int lcdnum);
extern void PsxSoundLoadFighter2(int lcdnum);
extern void PsxSoundLoadFighter1Babality(int lcdnum);
extern void PsxSoundLoadFighter2Babality(int lcdnum);
extern void PsxSoundLoadFighter1Animality(int lcdnum);
extern void PsxSoundLoadFighter2Animality(int lcdnum);
extern void PsxSoundLoadFighter1Fatality(int lcdnum);
extern void PsxSoundLoadFighter2Fatality(int lcdnum);

#endif
