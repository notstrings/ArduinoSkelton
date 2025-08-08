// ////////////////////////////////////////////////////////////////////////////
// 雑多な定義

// 書式ありシリアル出力
void SerialPrintF(Stream &SerialStrm, const char *format, ...)
{
  char sChar[256];
  va_list args;
  va_start(args, format);
  vsnprintf(sChar, sizeof(sChar), format, args);
  va_end(args);
  SerialStrm.print(sChar);
}
