// ////////////////////////////////////////////////////////////////////////////
// Arduinoスケルトンプログラム

#ifndef __XXX_H__
#define __XXX_H__

#include "misc.h"

// メインタスク
struct tyTaskMain {
  uint16_t  uwStat;
  uint16_t  uwMode;
  uint16_t  uwStep;
	uint16_t	uwReqQue[16];
	uint16_t	uwReqQueStt;
	uint16_t	uwReqQueEnd;
	uint16_t	uwReqQueCnt;
};

// シリアル通信タスク
struct tyTaskSCIF {
  uint16_t  uwStat;
  int  			iBuff;
  char 			cBuff;
  char 			aBuff[128];
};

// GPIOタスク
#define PNUM_LED 					(13)   	// ピン番号:死活確認用内蔵LED
#define PNUM_BTN 					(30)   	// ピン番号:死活確認用外付BTN
#define PIDX_LED		  		(0)			// ピン位置:LED
#define PIDX_BTN		  		(0)			// ピン位置:BTN
static const uint16_t uwIPinID[] = {
	PNUM_BTN
};
static const uint16_t uwIPinTM[] = {
	10
};
static const uint16_t uwOPinID[] = {
	PNUM_LED
};
static const uint16_t uwOPinTM[] = {
	100
};
#define IPIN_MAX ((int)(ARRAYSZ(uwIPinID)))
#define OPIN_MAX ((int)(ARRAYSZ(uwOPinID)))
#define OUT_PIN_REQ_NONE		(0)			// 無制御
#define OUT_PIN_REQ_ON			(1)			// HIGH LEVEL
#define OUT_PIN_REQ_OFF			(2)			// LOW  LEVEL
#define OUT_PIN_REQ_PULSE		(3)			// PULSE UP
#define OUT_PIN_REQ_NULSE		(4)			// PULSE DOWN
#define OUT_PIN_REQ_BLINK		(5)			// BLINK
struct tyTaskISIG {
	uint16_t	uwCrnt[IPIN_MAX];				// 入力ピン現状態
	uint16_t	uwPrev[IPIN_MAX];				// 入力ピン前状態
	uint16_t	uwChng[IPIN_MAX];				// 入力ピン変化
	uint16_t	uwCont[IPIN_MAX];				// 入力ピン継続
	uint16_t	uwStat[IPIN_MAX];		    // 入力ピン安定状態
	uint16_t	uwREdg[IPIN_MAX];			  // 立上エッジ検出フラグ
	uint16_t	uwFEdg[IPIN_MAX];			  // 立下エッジ検出フラグ
};
struct tyTaskOSIG {
	uint16_t	uwStat[OPIN_MAX];				// 出力制御状態
	uint16_t	uwWait[OPIN_MAX];				// 出力制御時間
};
struct tyTaskGPIO {
	uint16_t uwStat = 0;
	long int lCrnt = 0;
	long int lPrev = 0;
	struct tyTaskISIG stTaskISIG;
	struct tyTaskOSIG stTaskOSIG;
};

#endif
