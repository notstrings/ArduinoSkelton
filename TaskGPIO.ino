////////////////////////////////////////////////////////////////////////////////
// 信号制御タスク
// 　サブタスク:入力信号タスク
// 　　　　　　:出力信号タスク

////////////////////////////////////////////////////////////////////////////////
// 入力信号タスク

static void InitTaskISIG(struct tyTaskISIG *stpISIG)
{
	for(int idx = 0 ; idx < IPIN_MAX ; idx++){
    stpISIG->uwCrnt[idx] = digitalRead(uwIPinID[idx]);
    stpISIG->uwPrev[idx] = stpISIG->uwCrnt[idx];
    stpISIG->uwChng[idx] = 0;
    stpISIG->uwCont[idx] = 0;
    stpISIG->uwStat[idx] = 0;
    stpISIG->uwREdg[idx] = 0;
    stpISIG->uwFEdg[idx] = 0;
  }
}

static void ExecTaskISIG(struct tyTaskISIG *stpISIG)
{
	for(int idx = 0 ; idx < IPIN_MAX ; idx++){
    stpISIG->uwCrnt[idx] = digitalRead(uwIPinID[idx]);
		stpISIG->uwChng[idx] = stpISIG->uwCrnt[idx] != stpISIG->uwPrev[idx];
		if(stpISIG->uwChng[idx]){
			stpISIG->uwCont[idx] = uwIPinTM[idx];
		}
		if(stpISIG->uwCont[idx] <= 0){
			if(stpISIG->uwStat[idx] != stpISIG->uwCrnt[idx]){
				stpISIG->uwREdg[idx] |= (stpISIG->uwCrnt[idx] == HIGH);
				stpISIG->uwFEdg[idx] |= (stpISIG->uwCrnt[idx] != HIGH);
			}
			stpISIG->uwStat[idx] = stpISIG->uwCrnt[idx];
    }else{
			stpISIG->uwCont[idx] = stpISIG->uwCont[idx] - 1;
		}
    stpISIG->uwPrev[idx] = stpISIG->uwCrnt[idx];
  }
}

////////////////////////////////////////////////////////////////////////////////
// 入力信号タスク

#define OUT_PIN_STAT_STAY				      (0x0000)	// 状態保持
#define OUT_PIN_STAT_LEVEL_HIGH			  (0x0100)	// ハイレベル
#define OUT_PIN_STAT_LEVEL_LOW			  (0x0200)	// ローレベル
#define OUT_PIN_STAT_PULSE_UP			    (0x0300)	// パルスアップ
#define OUT_PIN_STAT_PULSE_UP_WAIT	  (0x0301)	// パルスアップ保持
#define OUT_PIN_STAT_PULSE_DOWN			  (0x0400)	// パルスダウン
#define OUT_PIN_STAT_PULSE_DOWN_WAIT	(0x0401)	// パルスダウン保持
#define OUT_PIN_STAT_BLINK				    (0x0500)	// 点滅
#define OUT_PIN_STAT_BLINK_REV			  (0x0501)	// 点滅反転

static void InitTaskOSIG(struct tyTaskOSIG *stpOSIG)
{
	for(int idx = 0 ; idx < OPIN_MAX ; idx++){
    stpOSIG->uwStat[idx] = 0;
    stpOSIG->uwWait[idx] = 0;
  }
}

static void ExecTaskOSIG(struct tyTaskOSIG *stpOSIG)
{
	for(int idx = 0 ; idx < OPIN_MAX ; idx++){
		switch(stpOSIG->uwStat[idx]){
			case OUT_PIN_STAT_STAY:							// STAY //////////////////////
				// do nothing
				break;
			case OUT_PIN_STAT_LEVEL_HIGH:				// LEVEL HIGH ////////////////
				digitalWrite(uwOPinID[idx], HIGH);											// ON
				stpOSIG->uwStat[idx] = 0;																// 停止
				break;
			case OUT_PIN_STAT_LEVEL_LOW:				// LEVEL LOW /////////////////
				digitalWrite(uwOPinID[idx], LOW);										  	// OFF
				stpOSIG->uwStat[idx] = 0;																// 停止
				break;
			case OUT_PIN_STAT_PULSE_UP:					// UP PULSE //////////////////
				digitalWrite(uwOPinID[idx], HIGH);											// ON
				stpOSIG->uwWait[idx] = uwOPinTM[idx];										// 継続時間設定
				stpOSIG->uwStat[idx] = OUT_PIN_STAT_PULSE_UP_WAIT;			// 待ちへ
				break;
			case OUT_PIN_STAT_PULSE_UP_WAIT:		// UP PULSE DOWN WAIT
				if(stpOSIG->uwWait[idx]){
					stpOSIG->uwWait[idx] -= 1;														// カウントダウン
					break;
				}
				digitalWrite(uwOPinID[idx], LOW);												// OFF
				stpOSIG->uwStat[idx] = 0;																// 保持状態
				break;
			case OUT_PIN_STAT_PULSE_DOWN:				// DOWN PULSE ////////////////
				digitalWrite(uwOPinID[idx], LOW);												// OFF
				stpOSIG->uwWait[idx] = uwOPinTM[idx];										// 継続時間設定
				stpOSIG->uwStat[idx] = OUT_PIN_STAT_PULSE_DOWN_WAIT;		// 待ちへ
				break;
			case OUT_PIN_STAT_PULSE_DOWN_WAIT:	// DOWN PULSE UP WAIT
				if(stpOSIG->uwWait[idx]){
					stpOSIG->uwWait[idx] -= 1;														// カウントダウン
					break;
				}
				digitalWrite(uwOPinID[idx], HIGH);											// ON
				stpOSIG->uwStat[idx] = 0;																// 保持状態
				break;
			case OUT_PIN_STAT_BLINK:						// BLINK /////////////////////
				if(stpOSIG->uwWait[idx]){
					stpOSIG->uwWait[idx] -= 1;														// カウントダウン
					break;
				}
				digitalWrite(uwOPinID[idx], HIGH);											// ON
				stpOSIG->uwWait[idx] = uwOPinTM[idx];										// 継続時間設定
				stpOSIG->uwStat[idx] = OUT_PIN_STAT_BLINK_REV;					// 待ちへ
				break;
			case OUT_PIN_STAT_BLINK_REV:				// BLINK REVERSE
				if(stpOSIG->uwWait[idx]){
					stpOSIG->uwWait[idx] -= 1;														// カウントダウン
					break;
				}
				digitalWrite(uwOPinID[idx], LOW);												// OFF
				stpOSIG->uwWait[idx] = uwOPinTM[idx];										// 継続時間設定
				stpOSIG->uwStat[idx] = OUT_PIN_STAT_BLINK;							// 待ちへ
				break;
			default:														// ERROR /////////////////////
				// do nothing
				break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// GPIOタスク本体

void InitTaskGPIO(struct tyTaskGPIO *stpTaskGPIO)
{
	InitTaskISIG(&(stpTaskGPIO->stTaskISIG));
	InitTaskOSIG(&(stpTaskGPIO->stTaskOSIG));
}

void ExecTaskGPIO(struct tyTaskGPIO *stpTaskGPIO)
{
	stpTaskGPIO->lCrnt = millis();
	if ((stpTaskGPIO->lCrnt - stpTaskGPIO->lPrev) >= 10) {
		ExecTaskISIG(&(stpTaskGPIO->stTaskISIG));
		ExecTaskOSIG(&(stpTaskGPIO->stTaskOSIG));
		stpTaskGPIO->lPrev = stpTaskGPIO->lCrnt;
	}
}

int GetSigLV(struct tyTaskGPIO *stpTaskGPIO, int idx)
{
	return stpTaskGPIO->stTaskISIG.uwStat[idx];
}

int GetSigED(struct tyTaskGPIO *stpTaskGPIO, int idx)
{
	int ret = 0;
	ret |= ( stpTaskGPIO->stTaskISIG.uwREdg[idx] ? 0x01 : 0x00 );
	ret |= ( stpTaskGPIO->stTaskISIG.uwFEdg[idx] ? 0x02 : 0x00 );
	stpTaskGPIO->stTaskISIG.uwREdg[idx] = 0;
	stpTaskGPIO->stTaskISIG.uwFEdg[idx] = 0;
	return ret;
}

int SetSigMD(struct tyTaskGPIO *stpTaskGPIO, int idx, int req)
{
	int ret = 0;												// 返却値
	switch( req ){
		case OUT_PIN_REQ_NONE:	// NONE ////////////////////
			// do nothing
			break;
		case OUT_PIN_REQ_ON:	// ON //////////////////////
			stpTaskGPIO->stTaskOSIG.uwStat[idx] = OUT_PIN_STAT_LEVEL_HIGH;	// LEVEL HIGH
			stpTaskGPIO->stTaskOSIG.uwWait[idx] = 0;												// 継続時間設定
			digitalWrite(uwOPinID[idx], HIGH);															// ON
			break;
		case OUT_PIN_REQ_OFF:	// OFF /////////////////////
			stpTaskGPIO->stTaskOSIG.uwStat[idx] = OUT_PIN_STAT_LEVEL_LOW;		// LEVEL LOW
			stpTaskGPIO->stTaskOSIG.uwWait[idx] = 0;												// 継続時間設定
			digitalWrite(uwOPinID[idx], LOW);																// OFF
			break;
		case OUT_PIN_REQ_PULSE:	// PULSE ON ////////////////
			stpTaskGPIO->stTaskOSIG.uwStat[idx] = OUT_PIN_STAT_PULSE_UP;		// UP PULSE
			stpTaskGPIO->stTaskOSIG.uwWait[idx] = 0;												// 継続時間設定
			digitalWrite(uwOPinID[idx], HIGH);															// ON
			break;
		case OUT_PIN_REQ_NULSE: // PULSE OFF ///////////////
			stpTaskGPIO->stTaskOSIG.uwStat[idx] = OUT_PIN_STAT_PULSE_DOWN;	// DOWN PULSE
			stpTaskGPIO->stTaskOSIG.uwWait[idx] = 0;												// 継続時間設定
			digitalWrite(uwOPinID[idx], LOW);																// OFF
			break;
		case OUT_PIN_REQ_BLINK:	// BLINK ///////////////////
			stpTaskGPIO->stTaskOSIG.uwStat[idx] = OUT_PIN_STAT_BLINK;				// BLINK
			stpTaskGPIO->stTaskOSIG.uwWait[idx] = 0;												// 継続時間設定
			digitalWrite(uwOPinID[idx], HIGH);															// ON
			break;
		default:				// OTHER ///////////////////
			ret = -1;																												// エラー
			break;
	}
	return ret;
}
