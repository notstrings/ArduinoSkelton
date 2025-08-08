// ////////////////////////////////////////////////////////////////////////////
// Arduinoスケルトンプログラム
// 更新履歴
// 2025XXXX初版作成

#include "ArduinoSkelton.h"

struct tyTaskMain stTaskMain;
struct tyTaskSCIF stTaskSCIF;
struct tyTaskGPIO stTaskGPIO;

void setup() {
  Serial.begin(38400);
  pinMode(PNUM_LED, OUTPUT);
  while (!Serial);
}

void loop() {
  InitTaskMain(&stTaskMain);    // メインタスク
  InitTaskSCIF(&stTaskSCIF);    // シリアル通信タスク
  InitTaskGPIO(&stTaskGPIO);    // GPIOタスク
  while(1){
    ExecTaskMain(&stTaskMain);  // メインタスク
    ExecTaskSCIF(&stTaskSCIF);  // シリアル通信タスク
    ExecTaskGPIO(&stTaskGPIO);  // GPIOタスク
  }
}
