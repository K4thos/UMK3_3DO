/*
 *          Movie Sample Program
 *
 *      Copyright (C) 1993 by Sony Corporation
 *          All rights Reserved
 *
 *   Version    Date
 *  ------------------------------------------
 *  1.00        Jul,14,1994 yutaka
 *  1.10        Sep,01,1994 suzu
 *  1.20        Oct,24,1994 yutaka
 *                  (anim sub routine化)
 *  1.30        Nov,01,1994 ume
 *                  (描画オフセット設定、
 *                  サイズが１６の倍数以外のイメージに対応)
 *
 *****************RELEASE NOTE****************************
 * \psx\sample\movie\animをこのmain.cで置き換えて下さい。
 * makefile.makでsim.oをリンクしているのをやめて下さい。
 * ディスクはDTL-S2160を使用して下さい。
 * 再生フレームの調整はFRAME_SIZEではなくEND_FRAMEでやって下さい
 * FRAME_SIZEは、0xfffffffを入れて下さい。end_callbackは０を
 * 指定して下さい。
 *
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>
#include <r3000.h>
#include <asm.h>
#include <kernel.h>
#include <libsn.h>

/*#define EMULATE*/

#ifdef EMULATE
#define StGetNext   StGetNextS
#define StFreeRing  StFreeRingS
#endif

#define IS_RGB24    1   /* 0:RGB16, 1:RGB24 */

#if IS_RGB24==1
#define PPW 3/2         /* １ショートワードに何ピクセルあるか */
#define MODE    3       /* 24bit モードでデコード */
#else
#define PPW 1           /* １ショートワードに何ピクセルあるか */
#define MODE    2       /* 16bit モードでデコード */
#endif

#define END_FRAME     260   /* アニメーションを終わらせるフレーム */
static CdlLOC loc;                      /* CDROMを再生させるポイント */

#define FILENAME    "\\MOVIE.STR;1"       /* ファイルネーム */
#define OFFY 0                          /* 描画オフセット(Y 座標) */
#define OFFX 0                          /* 描画オフセット(X 座標、偶数) */

#define bound(val, n)   ((((val) - 1) / (n) + 1) * (n))
#define bound16PPW(val)   (bound((val),16*PPW))
#define bound16(val)   (bound((val),16))

static int isFirstSlice;                /* 最左端スライスを示すフラグ */

/*
 *  デコード環境
 */
typedef struct {
    u_long  *vlcbuf[2]; /* VLC バッファ（ダブルバッファ） */
    int vlcid;      /* 現在 VLC デコード中バッファの ID */
    u_short *imgbuf;    /* デコード画像バッファ（シングル）*/
    RECT    rect[2];    /* 転送エリア（ダブルバッファ） */
    int rectid;     /* 現在転送中のバッファ ID */
    RECT    slice;      /* １回の DecDCTout で取り出す領域 */
    int isdone;     /* １フレーム分のデータができたか */
} DECENV;

static DECENV   dec;        /* デコード環境の実体 */
static int      Rewind_Switch;  /* CDが終りまでいくと１になる */
static int      Clear_Flag; /* 画面の解像度が変わり画面のクリアが
                   必要な時に１になる */

CdlFILE     fp;     /* ファイルの位置・サイズ情報 */

/*
 * コールバック関数型
 */
typedef int (*CallbackFunc)();

/*
 * static 関数宣言
 */
static strInit(CdlLOC *loc, int frame_size,
    CallbackFunc callback, CallbackFunc endcallback);
static int strCallback();
static strNextVlc(DECENV *dec);
static u_long *strNext(DECENV *dec);
static strSync(DECENV *dec, int mode);
static strKickCD(CdlLOC *loc);
static strSetDefDecEnv(DECENV *dec, int x0, int y0, int x1, int y1);
static void anim(char *fileName);

main()
{
	struct EXEC exec;

    /* アニメーション それ以外 共通の初期化 */
    ResetCallback();
    CdInit();
    PadInit(0);     /* PAD をリセット */
    ResetGraph(0);      /* GPU をリセット */
    SetGraphDebug(0);   /* デバッグレベル設定 */

    /* フォントをロード */
    FntLoad(960, 256);
    SetDumpFnt(FntOpen(32, 32, 320, 200, 0, 512));

    anim(FILENAME);         /* アニメーションサブルーチン */

	PadStop();
	StopCallback();
	ResetGraph(3);

	_96_remove();
	_96_init();
	LoadExec("cdrom:\\MK3.EXE;1", 0x801fff00, 0);

	return(0);
}

/*
 *  アニメーションサブルーチン フォアグラウンドプロセス
 */
static void anim(char *fileName)
{
    DISPENV disp;
    DRAWENV draw;

    int frame = 0;      /* フレームカウンタ */
    int id;         /* 表示バッファ ID */
    volatile u_long  enc_count; /* カウンタ */

    u_char  result[8], ret;     /* CD-ROM status */
    CdlLOC  ppos, lpos;     /* CD-ROM position */
    CdlFILE file;
    int fn;
    u_char  param[8];

    isFirstSlice = 1;

    /* ファイルをサーチ */
    if (CdSearchFile(&file, fileName) == 0) {
        StopCallback();
        PadStop();
        exit();
    }

    loc.minute = file.pos.minute;
    loc.second = file.pos.second;
    loc.sector = file.pos.sector;

    /* 初期化 */
    strSetDefDecEnv(&dec, OFFX*PPW, OFFY, OFFX*PPW, 240 + OFFY);

    strInit(&loc, 0xfffffff, strCallback, 0);

    /* 次のフレームのデータをとってきて VLC のデコードを行なう */
    /* 結果は、dec.vlcbuf[dec.vlcid] に格納される。*/
    strNextVlc(&dec);

    while (1) {
        /* VLC の完了したデータを送信 */
        DecDCTin(dec.vlcbuf[dec.vlcid], MODE);

        /* 最初のデコードブロックの受信の準備をする */
        DecDCTout(dec.imgbuf,
            bound16PPW(dec.slice.w)*bound16(dec.slice.h)/2);

        /* 次のフレームのデータの VLC デコード */
        strNextVlc(&dec);

        /* データが出来上がるのを待つ */
        strSync(&dec, 0);

        /* V-BLNK を待つ */
        VSync(0);

        /* 表示バッファをスワップ */
        /* 表示バッファは、描画バッファの反対側なことに注意 */
        id = dec.rectid? 0: 1;
        SetDefDispEnv(&disp, dec.rect[id].x - OFFX*PPW,
            dec.rect[id].y - OFFY,
            dec.rect[id].w, dec.rect[id].h);
        SetDefDrawEnv(&draw, dec.rect[id].x, dec.rect[id].y,
            dec.rect[id].w, dec.rect[id].h);

#if IS_RGB24==1
        disp.isrgb24 = IS_RGB24;
        disp.disp.w = disp.disp.w*2/3;
#endif
        PutDispEnv(&disp);
        PutDrawEnv(&draw);
        SetDispMask(1);       /* 表示許可 */

#if IS_RGB24==0
        FntFlush(-1);
#endif

        if(Rewind_Switch == 1)
        break;

        if(PadRead(1) & PADk)
            /* ストップボタンが押されたらアニメーションを抜ける */
            break;
    }

    /* アニメーション後処理 */
    param[0] = CdlModeSpeed;
    CdControlB(CdlSetmode,param,0);
    DecDCToutCallback(0);
    CdDataCallback(0);
    CdReadyCallback(0);
    CdControlB(CdlPause,0,0);
}

/*
 * デコード環境を初期化
 * imgbuf, vlcbuf は最適なサイズを割り当てるべきです。
 */
static strSetDefDecEnv(DECENV *dec, int x0, int y0, int x1, int y1)
{
    static u_long   vlcbuf0[320/2*256];     /* 大きさ適当 */
    static u_long   vlcbuf1[320/2*256];     /* 大きさ適当 */
    static u_short  imgbuf[16*PPW*240*2];       /* 短柵１個 */

    dec->vlcbuf[0] = vlcbuf0;
    dec->vlcbuf[1] = vlcbuf1;
    dec->vlcid     = 0;
    dec->imgbuf    = imgbuf;
    dec->rectid    = 0;
    dec->isdone    = 0;
    setRECT(&dec->rect[0], x0, y0, 640*PPW, 240);
    setRECT(&dec->rect[1], x1, y1, 640*PPW, 240);
    setRECT(&dec->slice,   x0, y0,  16*PPW, 240);
}

/*
 * ストリーミング環境を初期化
 */
#define RING_SIZE   32      /* リングバッファサイズ */

static u_long   sect_buff[RING_SIZE*SECTOR_SIZE];/* リングバッファ */

static u_long   sect_buff0[RING_SIZE*SECTOR_SIZE];/* リングバッファ */

static strInit(CdlLOC *loc, int frame_size,
    CallbackFunc callback, CallbackFunc endcallback)
{
    DecDCTReset(0);     /* MDEC をリセット */
    Rewind_Switch = 0;  /* 巻き戻し０ */

    /* MDECが１デコードブロックを処理した時のコールバックを定義する */
    DecDCToutCallback(callback);

    /* リングバッファの設定 */
    StSetRing(sect_buff, RING_SIZE);

    /* ストリーミングをセットアップ */
    /* 最後までいったら endcallback() がコールバックされる */
    StSetStream(IS_RGB24, 1, frame_size, 0, endcallback);

    /* 最初の１フレームをとってくる */
    strKickCD(loc);
}


/*
 * バックグラウンドプロセス
 * (DecDCTout() が終った時に呼ばれるコールバック関数)
 */
static int strCallback()
{
    int mod;

#if IS_RGB24==1
    extern StCdIntrFlag;
    if(StCdIntrFlag) {
        StCdInterrupt();    /* RGB24の時はここで起動する */
        StCdIntrFlag = 0;
    }
#endif

    /* デコード結果をフレームバッファに転送 */
    LoadImage(&dec.slice, (u_long *)dec.imgbuf);

    /* 短柵矩形領域をひとつ右に更新 */
    if (isFirstSlice && (mod = dec.rect[dec.rectid].w % (16*PPW))) {
        dec.slice.x += mod*PPW;
        isFirstSlice = 0;
    }
    else {
        dec.slice.x += 16*PPW;
    }

    /* まだ足りなければ、*/
    if (dec.slice.x < dec.rect[dec.rectid].x + dec.rect[dec.rectid].w) {
        /* 次の短柵を受信 */
        DecDCTout(dec.imgbuf, bound16PPW(dec.slice.w)*bound16(dec.slice.h)/2);
    }
    /* １フレーム分終ったら、*/
    else {
        /* 終ったことを通知 */
        dec.isdone = 1;
        isFirstSlice = 1;

        /* ID を更新 */
        dec.rectid = dec.rectid? 0: 1;
        dec.slice.x = dec.rect[dec.rectid].x;
        dec.slice.y = dec.rect[dec.rectid].y;

        /*DecDCTout(dec.imgbuf, dec.slice.w*dec.slice.h/2);*/
    }
}


/* 次のフレームのデータをよみだし VLC の解凍を行なう
 * エラーならばリングバッファをクリアして次のフレームを獲得する
 * VLCがデコードされたらリングバッファのそのフレームの領域は
 * 必要ないのでリングバッファのフレームの領域を解放する
 */
static strNextVlc(DECENV *dec)
{
    int cnt = WAIT_TIME;
    u_long  *next, *strNext();

    while ((next = strNext(dec)) == 0) {        /* get next frame */
        if (--cnt == 0)
            return(-1);
    }

    dec->vlcid = dec->vlcid? 0: 1;          /* swap ID */
    DecDCTvlc(next, dec->vlcbuf[dec->vlcid]);   /* VLC decode */
    StFreeRing(next);               /* free used frame */
    return(0);
}


/*
 * 次の１フレームのMOVIEフォーマットのデータをリングバッファからとってくる
 * データが正常だったら リングバッファの先頭アドレスを
 * 正常でなければ NULLを返す
 */
typedef struct {
    u_short id;         /* always 0x0x0160 */
    u_short type;
    u_short secCount;
    u_short nSectors;
    u_long  frameCount;
    u_long  frameSize;

    u_short width;
    u_short height;

    u_long  headm;
    u_long  headv;
} CDSECTOR;             /* CD-ROM STR 構造体 */

static u_long *strNext(DECENV *dec)
{
    u_long      *addr;
    CDSECTOR    *sector;
    int     cnt = WAIT_TIME;
    static  int      width  = 0;            /* 画面の横と縦 */
    static  int      height = 0;

    while(StGetNext(&addr,(u_long**)&sector)) {
        if (--cnt == 0)
          return(0);
    }

    /* セクタヘッダにあるヘッダと実際のデータのヘッダが
        一致していなければ エラーを返す */

    if(addr[0] != sector->headm || addr[1] != sector->headv) {
        printf("header is wrong!\n");
        StFreeRing(addr);
        return(0);
    }

    if(sector->frameCount == END_FRAME)
      {
        Rewind_Switch = 1;
      }

    /* 画面の解像度が 前のフレームと違うならば ClearImage を実行 */
    if(width != sector->width || height != sector->height) {

        RECT    rect;
        setRECT(&rect, 0, 0, 640 * PPW, 480);
        ClearImage(&rect, 0, 0, 0);

        width  = sector->width;
        height = sector->height;
    }

    /* ミニヘッダに合わせてデコード環境を変更する */
    dec->rect[0].w = dec->rect[1].w = width*PPW;
    dec->rect[0].h = dec->rect[1].h = height;
    dec->slice.h   = height;

    return(addr);
}

static strSync(DECENV *dec, int mode)
{
    volatile u_long cnt = WAIT_TIME;

    while (dec->isdone == 0) {
        if (--cnt == 0) {
            /* timeout: 強制的に切替える */
            printf("time out in decoding !\n");
            dec->isdone = 1;
            dec->rectid = dec->rectid? 0: 1;
            dec->slice.x = dec->rect[dec->rectid].x;
            dec->slice.y = dec->rect[dec->rectid].y;
        }
    }
    dec->isdone = 0;
}


/*
 * CDROMをlocからREADする。
 */
static strKickCD(CdlLOC *loc)
{
    /* 目的地まで Seek する */
    while (CdControl(CdlSeekL, (u_char *)loc, 0) == 0);

    /* ストリーミングモードを追加してコマンド発行 */
    while(CdRead2(0x100|CdlModeSpeed|CdlModeRT) == 0);
}

