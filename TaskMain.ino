// ////////////////////////////////////////////////////////////////////////////
// メインタスク

void InitTaskMain(struct tyTaskMain *stpTaskMain)
{
  stpTaskMain->uwStat = 0;
  stpTaskMain->uwMode = 0;
  stpTaskMain->uwStep = 0;
	RING_INIT( uint16_t, stpTaskMain->uwReqQue,
	           stpTaskMain->uwReqQueStt, stpTaskMain->uwReqQueEnd, stpTaskMain->uwReqQueCnt,
	           ARRAYSZ(stpTaskMain->uwReqQue) );
}

void ExecTaskMain(struct tyTaskMain *stpTaskMain)
{
  uint16_t uwReq;
  switch(stpTaskMain->uwStat){
    case 0: // INIT
      SerialPrintF(Serial, "Skelton Program Start\n");
      SetGPIOMD(&stTaskGPIO, PIDX_LED, OUT_PIN_REQ_BLINK);
      stpTaskMain->uwStat += 1;
      break;
    case 1: // IDLE
      if(DeQueReqTaskMain(stpTaskMain, &uwReq)==0){
        switch(uwReq){
          case 0: SetGPIOMD(&stTaskGPIO, PIDX_LED, OUT_PIN_REQ_ON);    break;
          case 1: SetGPIOMD(&stTaskGPIO, PIDX_LED, OUT_PIN_REQ_OFF);   break;
          case 2: SetGPIOMD(&stTaskGPIO, PIDX_LED, OUT_PIN_REQ_PULSE); break;
          case 3: SetGPIOMD(&stTaskGPIO, PIDX_LED, OUT_PIN_REQ_NULSE); break;
        }
      }
      if(GetGPIOED(&stTaskGPIO,PIDX_BTN)){
        SerialPrintF(Serial, "EDGE\n");
      }
      break;
    default:
      stpTaskMain->uwStat = 0;
      break;
  }
}

int DeQueReqTaskMain(struct tyTaskMain *stpTaskMain, uint16_t *uwReq)
{
	int ret = 1;
	if( RING_CAN_DEQUEUE( uint16_t, stpTaskMain->uwReqQue,
                        stpTaskMain->uwReqQueStt, stpTaskMain->uwReqQueEnd, stpTaskMain->uwReqQueCnt,
                        ARRAYSZ(stpTaskMain->uwReqQue), 1 )
	){
		RING_DEQUEUE( uint16_t, stpTaskMain->uwReqQue,
		              stpTaskMain->uwReqQueStt, stpTaskMain->uwReqQueEnd, stpTaskMain->uwReqQueCnt,
		              ARRAYSZ(stpTaskMain->uwReqQue), *uwReq);
		ret = 0;
	}
	return ret;
}

int EnQueReqTaskMain(struct tyTaskMain *stpTaskMain, uint16_t uwReq)
{
	int ret = 1;
	if( RING_CAN_ENQUEUE( uint16_t, stpTaskMain->uwReqQue,
                        stpTaskMain->uwReqQueStt, stpTaskMain->uwReqQueEnd, stpTaskMain->uwReqQueCnt,
                        ARRAYSZ(stpTaskMain->uwReqQue), 1 )
	){
		RING_ENQUEUE( uint16_t, stpTaskMain->uwReqQue,
		              stpTaskMain->uwReqQueStt, stpTaskMain->uwReqQueEnd, stpTaskMain->uwReqQueCnt,
		              ARRAYSZ(stpTaskMain->uwReqQue), uwReq);
		ret = 0;
	}
	return ret;
}
