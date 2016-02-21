#include "psxcd.h"
#include "psxsnd.h"
#include "mk3snd.h"

#if _CD_ABS_OPEN_ == 1
 #include "psxcdabs.h"
#endif

static int mem_size = WMD_SIZE;
static char mem_pntr[WMD_SIZE];

// SRAM buffer management
static SampleBlock fighter1blk;
static SampleBlock fighter2blk;
static SampleBlock generaldatablk;

static void InitSoundBlocks(void);
static int  CheckSoundBlock(SampleBlock *sampblk, SampleBlock *chkblk);
static void DoSoundBlockClear(SampleBlock *sampblk);

typedef struct LcdList {
    char *lcdname;
}LcdList;

#if _CD_VERSION_ == 1

 #if _CD_ABS_OPEN_ == 1

    #define WESS_WMD_FILE (char *)_SNDDATA_MK3SND_WMD

LcdList lcdfightlist_arr[] = {
    {(char *) _KANO_KANO_LCD          },
    {(char *) _SONYA_SONYA_LCD        },
    {(char *) _JAX_JAX_LCD            },
    {(char *) _INDIAN_INDIAN_LCD      },
    {(char *) _SUBZERO_SUBZERO_LCD    },
    {(char *) _SWAT_SWAT_LCD          },
    {(char *) _LIA_LIA_LCD            },
    {(char *) _ROBO1_ROBO1_LCD        },
    {(char *) _ROBO2_ROBO2_LCD        },
    {(char *) _LAO_KUNGLAO_LCD        },
    {(char *) _TUSK_TUSK_LCD          },
    {(char *) _SHEEVA_SHEEVA_LCD      },
    {(char *) _SHANG_SHANG_LCD        },
    {(char *) _LKANG_LIUKANG_LCD      },
    {(char *) _ROBO3_SMOKE_LCD        },
    {(char *) _MOTARO_MOTARO_LCD      },
    {(char *) _SHAOKAHN_SKFIGHT_LCD   },
    {(char *) _KANO_NOOB_LCD          },
    {0}
};

LcdList lcdbaballist_arr[] = {
    {(char *) _KANO_BKANO_LCD         },
    {(char *) _SONYA_BSONYA_LCD       },
    {(char *) _JAX_BJAX_LCD           },
    {(char *) _INDIAN_BINDIAN_LCD     },
    {(char *) _SUBZERO_BSUBZERO_LCD   },
    {(char *) _SWAT_BSWAT_LCD         },
    {(char *) _LIA_BLIA_LCD           },
    {(char *) _ROBO1_BROBO1_LCD       },
    {(char *) _ROBO2_BROBO2_LCD       },
    {(char *) _LAO_BKUNGLAO_LCD       },
    {(char *) _TUSK_BTUSK_LCD         },
    {(char *) _SHEEVA_BSHEEVA_LCD     },
    {(char *) _SHANG_BSHANG_LCD       },
    {(char *) _LKANG_BLIUKANG_LCD     },
    {(char *) _ROBO3_BSMOKE_LCD       },
    {0},
    {0},
    {0},
    {0}
};

LcdList lcdanimalist_arr[] = {
    {(char *) _KANO_AKANO_LCD         },
    {(char *) _SONYA_ASONYA_LCD       },
    {(char *) _JAX_AJAX_LCD           },
    {(char *) _INDIAN_AINDIAN_LCD     },
    {(char *) _SUBZERO_ASUBZERO_LCD   },
    {(char *) _SWAT_ASWAT_LCD         },
    {(char *) _LIA_ALIA_LCD           },
    {(char *) _ROBO1_AROBO1_LCD       },
    {(char *) _ROBO2_AROBO2_LCD       },
    {(char *) _LAO_AKUNGLAO_LCD       },
    {(char *) _TUSK_ATUSK_LCD         },
    {(char *) _SHEEVA_ASHEEVA_LCD     },
    {(char *) _SHANG_ASHANG_LCD       },
    {(char *) _LKANG_ALIUKANG_LCD     },
    {(char *) _ROBO3_ASMOKE_LCD       },
    {0},
    {0},
    {0},
    {0}
};

LcdList lcdfatallist_arr[] = {
    {(char *) _KANO_FKANO_LCD         },
    {(char *) _SONYA_FSONYA_LCD       },
    {(char *) _JAX_FJAX_LCD           },
    {(char *) _INDIAN_FINDIAN_LCD     },
    {(char *) _SUBZERO_FSUBZERO_LCD   },
    {(char *) _SWAT_FSWAT_LCD         },
    {(char *) _LIA_FLIA_LCD           },
    {(char *) _ROBO1_FROBO1_LCD       },
    {(char *) _ROBO2_FROBO2_LCD       },
    {(char *) _LAO_FKUNGLAO_LCD       },
    {(char *) _TUSK_FTUSK_LCD         },
    {(char *) _SHEEVA_FSHEEVA_LCD     },
    {(char *) _SHANG_FSHANG_LCD       },
    {(char *) _LKANG_FLIUKANG_LCD     },
    {(char *) _ROBO3_FSMOKE_LCD       },
    {0},
    {0},
    {0},
    {0}
};

LcdList lcddatalist_arr[] = {
    {(char *) _SNDDATA_SELECT_LCD     },
    {(char *) _SNDDATA_HIDGAME_LCD    },
    {(char *) _SNDDATA_RAND_LCD       },
    {(char *) _SNDDATA_SMOKECOD_LCD   },
    {(char *) _SNDDATA_LADDER_LCD     },
    {(char *) _SNDDATA_GAMEOVER_LCD   },
    {(char *) _SNDDATA_BUYIN_LCD      },
    {(char *) _SNDDATA_VS_LCD         },
    {(char *) _SNDDATA_SKSPCIAL_LCD   },
    {(char *) _SNDDATA_SKPIT_LCD      },
    {(char *) _SNDDATA_SKBELL_LCD     },
    {(char *) _SNDDATA_SKTRAIN_LCD    },
    {(char *) _SNDDATA_SKSTREET_LCD   },
    {(char *) _SNDDATA_SKBRIDGE_LCD   },
    {(char *) _SNDDATA_SKBANK_LCD     },
    {(char *) _SNDDATA_SKROOF_LCD     },
    {(char *) _SNDDATA_SKSOUL_LCD     },
    {(char *) _SNDDATA_SKCHURCH_LCD   },
    {(char *) _SNDDATA_SKGRAVE_LCD    },
    {(char *) _SNDDATA_SKHID_LCD      },
    {0}
};
 #else

    #define WESS_WMD_FILE "\\SNDDATA\\MK3SND.WMD;1"

LcdList lcdfightlist_arr[] = {
    {"\\KANO\\KANO.LCD;1"          },
    {"\\SONYA\\SONYA.LCD;1"        },
    {"\\JAX\\JAX.LCD;1"            },
    {"\\INDIAN\\INDIAN.LCD;1"      },
    {"\\SUBZERO\\SUBZERO.LCD;1"    },
    {"\\SWAT\\SWAT.LCD;1"          },
    {"\\LIA\\LIA.LCD;1"            },
    {"\\ROBO1\\ROBO1.LCD;1"        },
    {"\\ROBO2\\ROBO2.LCD;1"        },
    {"\\LAO\\KUNGLAO.LCD;1"        },
    {"\\TUSK\\TUSK.LCD;1"          },
    {"\\SHEEVA\\SHEEVA.LCD;1"      },
    {"\\SHANG\\SHANG.LCD;1"        },
    {"\\LKANG\\LIUKANG.LCD;1"      },
    {"\\ROBO3\\SMOKE.LCD;1"        },
    {"\\SONYA\\SONYA.LCD;1"        },
    {"\\SONYA\\SONYA.LCD;1"        },
    {"\\SONYA\\SONYA.LCD;1"        },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\MOTARO\\MOTARO.LCD;1"      },
    {"\\SHAOKAHN\\SKFIGHT.LCD;1"   },
    {0}
};

LcdList lcdbaballist_arr[] = {
    {"\\KANO\\BKANO.LCD;1"         },
    {"\\SONYA\\BSONYA.LCD;1"       },
    {"\\JAX\\BJAX.LCD;1"           },
    {"\\INDIAN\\BINDIAN.LCD;1"     },
    {"\\SUBZERO\\BSUBZERO.LCD;1"   },
    {"\\SWAT\\BSWAT.LCD;1"         },
    {"\\LIA\\BLIA.LCD;1"           },
    {"\\ROBO1\\BROBO1.LCD;1"       },
    {"\\ROBO2\\BROBO2.LCD;1"       },
    {"\\LAO\\BKUNGLAO.LCD;1"       },
    {"\\TUSK\\BTUSK.LCD;1"         },
    {"\\SHEEVA\\BSHEEVA.LCD;1"     },
    {"\\SHANG\\BSHANG.LCD;1"       },
    {"\\LKANG\\BLIUKANG.LCD;1"     },
    {"\\ROBO3\\BSMOKE.LCD;1"       },
    {"\\SONYA\\SONYA.LCD;1"        },
    {"\\SONYA\\SONYA.LCD;1"        },
    {"\\SONYA\\SONYA.LCD;1"        },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0}
};

LcdList lcdanimalist_arr[] = {
    {"\\KANO\\AKANO.LCD;1"         },
    {"\\SONYA\\ASONYA.LCD;1"       },
    {"\\JAX\\AJAX.LCD;1"           },
    {"\\INDIAN\\AINDIAN.LCD;1"     },
    {"\\SUBZERO\\ASUBZERO.LCD;1"   },
    {"\\SWAT\\ASWAT.LCD;1"         },
    {"\\LIA\\ALIA.LCD;1"           },
    {"\\ROBO1\\AROBO1.LCD;1"       },
    {"\\ROBO2\\AROBO2.LCD;1"       },
    {"\\LAO\\AKUNGLAO.LCD;1"       },
    {"\\TUSK\\ATUSK.LCD;1"         },
    {"\\SHEEVA\\ASHEEVA.LCD;1"     },
    {"\\SHANG\\ASHANG.LCD;1"       },
    {"\\LKANG\\ALIUKANG.LCD;1"     },
    {"\\ROBO3\\ASMOKE.LCD;1"       },
    {"\\SONYA\\SONYA.LCD;1"        },
    {"\\SONYA\\SONYA.LCD;1"        },
    {"\\SONYA\\SONYA.LCD;1"        },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0}
};

LcdList lcdfatallist_arr[] = {
    {"\\KANO\\FKANO.LCD;1"         },
    {"\\SONYA\\FSONYA.LCD;1"       },
    {"\\JAX\\FJAX.LCD;1"           },
    {"\\INDIAN\\FINDIAN.LCD;1"     },
    {"\\SUBZERO\\FSUBZERO.LCD;1"   },
    {"\\SWAT\\FSWAT.LCD;1"         },
    {"\\LIA\\FLIA.LCD;1"           },
    {"\\ROBO1\\FROBO1.LCD;1"       },
    {"\\ROBO2\\FROBO2.LCD;1"       },
    {"\\LAO\\FKUNGLAO.LCD;1"       },
    {"\\TUSK\\FTUSK.LCD;1"         },
    {"\\SHEEVA\\FSHEEVA.LCD;1"     },
    {"\\SHANG\\FSHANG.LCD;1"       },
    {"\\LKANG\\FLIUKANG.LCD;1"     },
    {"\\ROBO3\\FSMOKE.LCD;1"       },
    {"\\SONYA\\SONYA.LCD;1"        },
    {"\\SONYA\\SONYA.LCD;1"        },
    {"\\SONYA\\SONYA.LCD;1"        },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {"\\KANO\\KANO.LCD;1"          },
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0}
};

LcdList lcddatalist_arr[] = {
    {"\\SNDDATA\\SELECT.LCD;1"     },
    {"\\SNDDATA\\HIDGAME.LCD;1"    },
    {"\\SNDDATA\\RAND.LCD;1"       },
    {"\\SNDDATA\\SMOKECOD.LCD;1"   },
    {"\\SNDDATA\\LADDER.LCD;1"     },
    {"\\SNDDATA\\GAMEOVER.LCD;1"   },
    {"\\SNDDATA\\BUYIN.LCD;1"      },
    {"\\SNDDATA\\VS.LCD;1"         },
    {"\\SNDDATA\\SKSPCIAL.LCD;1"   },
    {"\\SNDDATA\\SKPIT.LCD;1"      },
    {"\\SNDDATA\\SKBELL.LCD;1"     },
    {"\\SNDDATA\\SKTRAIN.LCD;1"    },
    {"\\SNDDATA\\SKSTREET.LCD;1"   },
    {"\\SNDDATA\\SKBRIDGE.LCD;1"   },
    {"\\SNDDATA\\SKBANK.LCD;1"     },
    {"\\SNDDATA\\SKROOF.LCD;1"     },
    {"\\SNDDATA\\SKSOUL.LCD;1"     },
    {"\\SNDDATA\\SKCHURCH.LCD;1"   },
    {"\\SNDDATA\\SKGRAVE.LCD;1"    },
    {"\\SNDDATA\\SKHID.LCD;1"      },
    {0}
};
 #endif

#else

    #define WESS_WMD_FILE "SNDDATA\\MK3SND.WMD"

LcdList lcdfightlist_arr[] = {
    {"KANO\\KANO.LCD"          },
    {"SONYA\\SONYA.LCD"        },
    {"JAX\\JAX.LCD"            },
    {"INDIAN\\INDIAN.LCD"      },
    {"SUBZERO\\SUBZERO.LCD"    },
    {"SWAT\\SWAT.LCD"          },
    {"LIA\\LIA.LCD"            },
    {"ROBO1\\ROBO1.LCD"        },
    {"ROBO2\\ROBO2.LCD"        },
    {"LAO\\KUNGLAO.LCD"        },
    {"TUSK\\TUSK.LCD"          },
    {"SHEEVA\\SHEEVA.LCD"      },
    {"SHANG\\SHANG.LCD"        },
    {"LKANG\\LIUKANG.LCD"      },
    {"ROBO3\\SMOKE.LCD"        },
    {"MOTARO\\MOTARO.LCD"      },
    {"SHAOKAHN\\SKFIGHT.LCD"   },
    {"KANO\\NOOB.LCD"          },
    {0}
};

LcdList lcdbaballist_arr[] = {
    {"KANO\\BKANO.LCD"         },
    {"SONYA\\BSONYA.LCD"       },
    {"JAX\\BJAX.LCD"           },
    {"INDIAN\\BINDIAN.LCD"     },
    {"SUBZERO\\BSUBZERO.LCD"   },
    {"SWAT\\BSWAT.LCD"         },
    {"LIA\\BLIA.LCD"           },
    {"ROBO1\\BROBO1.LCD"       },
    {"ROBO2\\BROBO2.LCD"       },
    {"LAO\\BKUNGLAO.LCD"       },
    {"TUSK\\BTUSK.LCD"         },
    {"SHEEVA\\BSHEEVA.LCD"     },
    {"SHANG\\BSHANG.LCD"       },
    {"LKANG\\BLIUKANG.LCD"     },
    {"ROBO3\\BSMOKE.LCD"       },
    {0},
    {0},
    {0},
    {0}
};

LcdList lcdanimalist_arr[] = {
    {"KANO\\AKANO.LCD"         },
    {"SONYA\\ASONYA.LCD"       },
    {"JAX\\AJAX.LCD"           },
    {"INDIAN\\AINDIAN.LCD"     },
    {"SUBZERO\\ASUBZERO.LCD"   },
    {"SWAT\\ASWAT.LCD"         },
    {"LIA\\ALIA.LCD"           },
    {"ROBO1\\AROBO1.LCD"       },
    {"ROBO2\\AROBO2.LCD"       },
    {"LAO\\AKUNGLAO.LCD"       },
    {"TUSK\\ATUSK.LCD"         },
    {"SHEEVA\\ASHEEVA.LCD"     },
    {"SHANG\\ASHANG.LCD"       },
    {"LKANG\\ALIUKANG.LCD"     },
    {"ROBO3\\ASMOKE.LCD"       },
    {0},
    {0},
    {0},
    {0}
};

LcdList lcdfatallist_arr[] = {
    {"KANO\\FKANO.LCD"         },
    {"SONYA\\FSONYA.LCD"       },
    {"JAX\\FJAX.LCD"           },
    {"INDIAN\\FINDIAN.LCD"     },
    {"SUBZERO\\FSUBZERO.LCD"   },
    {"SWAT\\FSWAT.LCD"         },
    {"LIA\\FLIA.LCD"           },
    {"ROBO1\\FROBO1.LCD"       },
    {"ROBO2\\FROBO2.LCD"       },
    {"LAO\\FKUNGLAO.LCD"       },
    {"TUSK\\FTUSK.LCD"         },
    {"SHEEVA\\FSHEEVA.LCD"     },
    {"SHANG\\FSHANG.LCD"       },
    {"LKANG\\FLIUKANG.LCD"     },
    {"ROBO3\\FSMOKE.LCD"       },
    {"SONYA\\FSONYA.LCD"       },
    {"SONYA\\FSONYA.LCD"       },
    {"SONYA\\FSONYA.LCD"       },
    {"ROBO1\\FROBO1.LCD"       },
    {"ROBO1\\FROBO1.LCD"       },
    {"ROBO1\\FROBO1.LCD"       },
    {"ROBO1\\FROBO1.LCD"       },
    {"ROBO1\\FROBO1.LCD"       },
    {"ROBO1\\FROBO1.LCD"       },
    {"ROBO1\\FROBO1.LCD"       },
    {"ROBO1\\FROBO1.LCD"       },
};

LcdList lcddatalist_arr[] = {
    {"SNDDATA\\SELECT.LCD"     },
    {"SNDDATA\\HIDGAME.LCD"    },
    {"SNDDATA\\RAND.LCD"       },
    {"SNDDATA\\SMOKECOD.LCD"   },
    {"SNDDATA\\LADDER.LCD"     },
    {"SNDDATA\\GAMEOVER.LCD"   },
    {"SNDDATA\\BUYIN.LCD"      },
    {"SNDDATA\\VS.LCD"         },
    {"SNDDATA\\SKSPCIAL.LCD"   },
    {"SNDDATA\\SKPIT.LCD"      },
    {"SNDDATA\\SKBELL.LCD"     },
    {"SNDDATA\\SKTRAIN.LCD"    },
    {"SNDDATA\\SKSTREET.LCD"   },
    {"SNDDATA\\SKBRIDGE.LCD"   },
    {"SNDDATA\\SKBANK.LCD"     },
    {"SNDDATA\\SKROOF.LCD"     },
    {"SNDDATA\\SKSOUL.LCD"     },
    {"SNDDATA\\SKCHURCH.LCD"   },
    {"SNDDATA\\SKGRAVE.LCD"    },
    {"SNDDATA\\SKHID.LCD"      },
    {0}
};

#endif

static int psxsettings[SNDHW_TAG_MAX*2] = {
        SNDHW_TAG_DRIVER_ID, PSX_ID,
        SNDHW_TAG_SOUND_EFFECTS, 1,
        SNDHW_TAG_MUSIC, 1,
        SNDHW_TAG_DRUMS, 1,
        SNDHW_TAG_END, 0
    };

static int *settings_list[2] = {
        psxsettings,
        0
    };

int sram_pos = SRAM_START;

void InitSoundBlocks(void)
{
    fighter1blk.numsamps = 0;
    fighter2blk.numsamps = 0;
    generaldatablk.numsamps = 0;
}

int CheckSoundBlock(SampleBlock *sampblk, SampleBlock *chkblk)
{
    int numtochk;

    if(sampblk!=chkblk)
    {
        numtochk = chkblk->numsamps;

        while(numtochk--)
        {
            if(sampblk->sampindx[sampblk->numsamps]==
               chkblk->sampindx[numtochk])
            {
                wess_dig_set_sample_position(chkblk->sampindx[numtochk],
                                             (char *)((unsigned long)chkblk->samppos[numtochk]<<3));
                return(1);
            }
        }
    }

    return(0);
}

void DoSoundBlockClear(SampleBlock *sampblk)
{
    while(sampblk->numsamps)
    {
        sampblk->numsamps--;

        if(CheckSoundBlock(sampblk,&fighter1blk)) continue;

        if(CheckSoundBlock(sampblk,&fighter2blk)) continue;

        if(CheckSoundBlock(sampblk,&generaldatablk)) continue;

        //if no other buffers have the sound, clear position
        wess_dig_set_sample_position(sampblk->sampindx[sampblk->numsamps],
                                     (char *)0);

    }
}

void PsxSoundLoadClear(void)
{
    DoSoundBlockClear(&fighter1blk);
    DoSoundBlockClear(&fighter2blk);
    DoSoundBlockClear(&generaldatablk);
    sram_pos = SRAM_START;
}

void PsxSoundLoadData(int lcdnum)
{
    sram_pos += wess_dig_lcd_load(lcddatalist_arr[lcdnum].lcdname,(void *)sram_pos, &generaldatablk, 0);
}

void PsxSoundLoadFighter1(int lcdnum)
{
    DoSoundBlockClear(&fighter1blk);
    wess_dig_lcd_load(lcdfightlist_arr[lcdnum].lcdname,(void *)SRAM_FIGHTER_1, &fighter1blk, 1);
}

void PsxSoundLoadFighter2(int lcdnum)
{
    DoSoundBlockClear(&fighter2blk);
    wess_dig_lcd_load(lcdfightlist_arr[lcdnum].lcdname,(void *)SRAM_FIGHTER_2, &fighter2blk, 1);
}

void PsxSoundLoadFighter1Babality(int lcdnum)
{
    DoSoundBlockClear(&fighter1blk);
    wess_dig_lcd_load(lcdbaballist_arr[lcdnum].lcdname,(void *)SRAM_FIGHTER_1, &fighter1blk, 1);
}

void PsxSoundLoadFighter2Babality(int lcdnum)
{
    DoSoundBlockClear(&fighter2blk);
    wess_dig_lcd_load(lcdbaballist_arr[lcdnum].lcdname,(void *)SRAM_FIGHTER_2, &fighter2blk, 1);
}

void PsxSoundLoadFighter1Animality(int lcdnum)
{
    DoSoundBlockClear(&fighter1blk);
    wess_dig_lcd_load(lcdanimalist_arr[lcdnum].lcdname,(void *)SRAM_FIGHTER_1, &fighter1blk, 1);
}

void PsxSoundLoadFighter2Animality(int lcdnum)
{
    DoSoundBlockClear(&fighter2blk);
    wess_dig_lcd_load(lcdanimalist_arr[lcdnum].lcdname,(void *)SRAM_FIGHTER_2, &fighter2blk, 1);
}

void PsxSoundLoadFighter1Fatality(int lcdnum)
{
    DoSoundBlockClear(&fighter1blk);
    wess_dig_lcd_load(lcdfatallist_arr[lcdnum].lcdname,(void *)SRAM_FIGHTER_1, &fighter1blk, 1);
}

void PsxSoundLoadFighter2Fatality(int lcdnum)
{
    DoSoundBlockClear(&fighter2blk);
    wess_dig_lcd_load(lcdfatallist_arr[lcdnum].lcdname,(void *)SRAM_FIGHTER_2, &fighter2blk, 1);
}

unsigned long PsxSoundInit(char *ptrtowmd)
{
    //int i;
    #if _CD_VERSION_ == 1
     PsxCd_File *fileptr;
    #else
     int fileint;
     unsigned long filesize;
    #endif

    InitSoundBlocks();

    wess_init();
    psx_set_mute_release(250);

    #if _CD_VERSION_ == 1
        psxcd_init();
        fileptr = psxcd_open(WESS_WMD_FILE);
        //i = CdPosToInt(&fileptr->file.pos);
        psxcd_read(ptrtowmd,fileptr->file.size,fileptr);
        psxcd_close(fileptr);
    #else
        fileint = PCopen(WESS_WMD_FILE,0,0);
        filesize = PClseek(fileint, 0, 2);
        PClseek(fileint, 0, 0);
        PCread(fileint, ptrtowmd, filesize);
        PCclose(fileint);
    #endif

    wess_load_module(ptrtowmd,mem_pntr,mem_size,settings_list);

    wess_seq_loader_init(wess_get_master_status(),WESS_WMD_FILE,NoOpenSeqHandle);

    wess_dig_lcd_loader_init(wess_get_master_status());

    return(wess_seq_range_load(0,NUMBER_OF_SEQUENCES,wess_get_wmd_end()));
}

void PsxSoundExit(void)
{
    wess_exit(YesRestore);
}


