#include"stdafx.h"
#include"globedef.h"

#include"ReadString.h"

CString ReadString(CWebDataPtr pWebData) {
  char buffer[100];
  int i = 0;
  CString str = _T("");

  if (pWebData->GetCurrentPosData() != '"') return str; // error
  pWebData->IncreaseCurrentPos();
  while (pWebData->GetCurrentPosData() != '"') {
    buffer[i++] = pWebData->GetCurrentPosData();
    pWebData->IncreaseCurrentPos();
  }
  pWebData->IncreaseCurrentPos();
  buffer[i] = 0x000;
  str = buffer;
  return str;
}