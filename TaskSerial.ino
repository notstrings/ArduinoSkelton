// ////////////////////////////////////////////////////////////////////////////
// シリアル通信タスク(アスキー通信)

void InitTaskSCIF(struct tyTaskSCIF *stpTaskSCIF)
{
  stpTaskSCIF->uwStat = 0;
  stpTaskSCIF->iBuff = 0;
  stpTaskSCIF->cBuff = 0;
  MEMSET(stpTaskSCIF->aBuff, 0, char, ARRAYSZ(stpTaskSCIF->aBuff));
}

void ExecTaskSCIF(struct tyTaskSCIF *stpTaskSCIF)
{
  switch(stpTaskSCIF->uwStat){
    default:
    case 0: // 初端文字待機
      while (Serial.available() > 0) {
        stpTaskSCIF->cBuff = (char)Serial.read();
        stpTaskSCIF->iBuff = 0;
        stpTaskSCIF->aBuff[(stpTaskSCIF->iBuff)++] = stpTaskSCIF->cBuff;
        if (stpTaskSCIF->cBuff == '#') {
          stpTaskSCIF->uwStat += 1;
          break;
        }
      }
      break;
    case 1: // 終端文字待機
      while (Serial.available() > 0) {
        stpTaskSCIF->cBuff = (char)Serial.read();
        stpTaskSCIF->iBuff = stpTaskSCIF->iBuff;
        stpTaskSCIF->aBuff[(stpTaskSCIF->iBuff)++] = stpTaskSCIF->cBuff;
        if (stpTaskSCIF->cBuff == '\n') {
          stpTaskSCIF->uwStat += 1;
          break;
        }
      }
      break;
    case 2: // コマンド実行
      // #EXEC00 //////////////////////
      if (strncmp("#EXEC00", stpTaskSCIF->aBuff, 7) == 0) {
        int num = 0;
        int val = 0;
        sscanf(&(stpTaskSCIF->aBuff)[8],  "%2d", &num);
        sscanf(&(stpTaskSCIF->aBuff)[11], "%2d", &val);
        switch (num) {
          case 0: EnQueReqTaskMain(&stTaskMain, (uint16_t)val); break;
        }
        SerialPrintF(Serial, "#EXEC00:%02d,%02d\n", num, val);
      }
      // 初端待機へ ////////////////////
      stpTaskSCIF->uwStat = 0;
      stpTaskSCIF->iBuff = 0;
      break;
  }
}
