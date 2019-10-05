#include "stdafx.h"
#include "globedef.h"

#include "RTData.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CStockRTData::Reset(void) {
  m_time = 0;
  // 必须初始化m_strStockCode的长度为6.
  m_strStockCode = "";
  m_wMarket = 0;
  m_lLastClose = 0;
  m_lOpen = 0;
  m_lHigh = 0;
  m_lLow = 0;
  m_lNew = 0;
  m_lVolume = 0;
  m_lAmount = 0;
  m_lBuy = 0;
  m_lSell = 0;
  for (int i = 0; i < 5; i++) {
    m_lPBuy.at(i) = 0;
    m_lVBuy.at(i) = 0;
    m_lPSell.at(i) = 0;
    m_lVSell.at(i) = 0;
  }
  m_fActive = false;
}

CStockRTData::CStockRTData(void) : CObject() {
  Reset();
}

/////////////////////////////////////////////////////////////////////////////
// CStockRTData diagnostics

#ifdef _DEBUG
void CStockRTData::AssertValid() const
{
  CObject::AssertValid();
}

void CStockRTData::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////////
//
//
// 参数:
//		data : 另一个实时数据单位.
//
//
/////////////////////////////////////////////////////////////////////////////////
bool CStockRTData::SetData(CStockRTData& data) {
  m_time = data.m_time;
  m_wMarket = data.m_wMarket;
  m_strStockCode = data.m_strStockCode;
  m_strStockName = data.m_strStockName;
  m_lLastClose = data.m_lLastClose;
  m_lOpen = data.m_lOpen;
  m_lHigh = data.m_lHigh;
  m_lLow = data.m_lLow;
  m_lNew = data.m_lNew;
  m_lBuy = data.m_lBuy;
  m_lSell = data.m_lSell;

  if (data.m_lVolume != 0) {
    m_lVolume = data.m_lVolume;
    m_lAmount = data.m_lAmount;
  }

  for (int i = 0; i < 5; i++) {
    m_lPBuy.at(i) = data.m_lPBuy.at(i);
    m_lVBuy.at(i) = data.m_lVBuy.at(i);
    m_lPSell.at(i) = data.m_lPSell.at(i);
    m_lVSell.at(i) = data.m_lVSell.at(i);
  }

  return(true);
}

/////////////////////////////////////////////////////////////////////////////////
//
//
// 参数:
//		data : 另一个实时数据单位.
//
//
/////////////////////////////////////////////////////////////////////////////////
bool CStockRTData::SetDataAll(CStockRTData& data) {
  m_time = 0;
  m_wMarket = 0;
  m_lLastClose = 0;
  m_lOpen = 0;
  m_lHigh = 0;
  m_lLow = 0;
  m_lNew = 0;
  m_lVolume = 0;
  m_lAmount = 0;
  m_lBuy = 0;
  m_lSell = 0;

  for (int i = 0; i < 5; i++) {
    m_lPBuy.at(i) = 0;
    m_lVBuy.at(i) = 0;
    m_lPSell.at(i) = 0;
    m_lVSell.at(i) = 0;
  }

  m_time = data.m_time;
  m_wMarket = data.m_wMarket;
  m_strStockCode = data.m_strStockCode;
  m_strStockName = data.m_strStockName;
  m_lLastClose = data.m_lLastClose;
  m_lOpen = data.m_lOpen;
  m_lHigh = data.m_lHigh;
  m_lLow = data.m_lLow;
  m_lNew = data.m_lNew;
  m_lVolume = data.m_lVolume;
  m_lAmount = data.m_lAmount;
  m_lBuy = data.m_lBuy;
  m_lSell = data.m_lSell;

  for (int i = 0; i < 5; i++) {
    m_lPBuy.at(i) = data.m_lPBuy.at(i);
    m_lVBuy.at(i) = data.m_lVBuy.at(i);
    m_lPSell.at(i) = data.m_lPSell.at(i);
    m_lVSell.at(i) = data.m_lVSell.at(i);
  }
  return(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 从网络文件file中读取实时数据，返回值是所读数据是否出现格式错误。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CStockRTData::ReadData(char*& pCurrentPos, long& iTotalRead)
{
  static char buffer1[200];
  char buffer2[7];
  static char buffer3[200];
  static CString strHeader = _T("var hq_str_s");
  long lTemp = 0;
  INT64 llTemp = 0;

  m_fActive = false;    // 初始状态为无效数据
  strncpy_s(buffer1, pCurrentPos, 12); // 读入“var hq_str_s"
  buffer1[12] = 0x000;
  CString str1;
  str1 = buffer1;
  if (strHeader.Compare(str1) != 0) { // 数据格式出错
    return false;
  }
  pCurrentPos += 12;
  iTotalRead += 12;

  if (*pCurrentPos == 'h') { // 上海股票
    m_wMarket = __SHANGHAI_MARKET__; // 上海股票标识
  }
  else if (*pCurrentPos == 'z') {
    m_wMarket = __SHENZHEN_MARKET__; // 深圳股票标识
  }
  else {
    return false;
  }
  pCurrentPos++;
  iTotalRead += 1;

  strncpy_s(buffer2, pCurrentPos, 6);
  buffer2[6] = 0x000;
  m_strStockCode = buffer2;
  switch (m_wMarket) {
  case __SHANGHAI_MARKET__:
    m_strStockCode = _T("sh") + m_strStockCode; // 由于上海深圳股票代码有重叠，故而所有的股票代码都带上市场前缀。上海为sh
    break;
  case __SHENZHEN_MARKET__:
    m_strStockCode = _T("sz") + m_strStockCode;// 由于上海深圳股票代码有重叠，故而所有的股票代码都带上市场前缀。深圳为sz
    break;
  default:
    return false;
  }
  m_iStockCode = atoi(buffer2);
  pCurrentPos += 6;
  iTotalRead += 6;

  strncpy_s(buffer1, pCurrentPos, 2); // 读入'="'
  if (buffer1[0] != '=') {
    return false;
  }
  if (buffer1[1] != '"') {
    return false;
  }
  pCurrentPos += 2;
  iTotalRead += 2;
  strncpy_s(buffer1, pCurrentPos, 2);
  if (buffer1[0] == '"') { // 没有数据
    if (buffer1[1] != ';') {
      return false;
    }
    iTotalRead += 2;
    pCurrentPos += 2;
    if (*pCurrentPos++ != 0x00a) {
      return false; // 确保是字符 \n
    }
    iTotalRead++;
    return true;
  }
  if ((buffer1[0] == 0x00a) || (buffer1[0] == 0x000)) {
    return false;
  }
  if ((buffer1[1] == 0x00a) || (buffer1[1] == 0x000)) {
    return false;
  }
  pCurrentPos += 2;
  iTotalRead += 2;

  int i = 2;
  while (*pCurrentPos != 0x02c) { // 读入剩下的中文名字（第一个字在buffer1中）
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
      return false;
    }
    buffer1[i++] = *pCurrentPos++;
    iTotalRead++;
  }
  buffer1[i] = 0x000;
  m_strStockName = buffer1; // 设置股票名称

  pCurrentPos++;
  iTotalRead++;

  // 读入开盘价。放大一千倍后存储为长整型。其他价格亦如此。
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lOpen = lTemp;
  // 读入前收盘价
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lLastClose = lTemp;
  // 读入当前价
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lNew = lTemp;
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lHigh = lTemp;
  // 读入最低价
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lLow = lTemp;
  // 读入竞买价
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lBuy = lTemp;
  // 读入竞卖价
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lSell = lTemp;

  // 读入成交股数。成交股数存储实际值
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  llTemp = atoll(buffer3); // 读入的是股数，存储也使用股，故而需要将此变量设为INT64。
  if (llTemp < 0) return false;
  if (llTemp > 0) m_lVolume = llTemp;
  // 读入成交金额
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  llTemp = atoll(buffer3);
  if (llTemp < 0) return false;
  if (llTemp > 0) m_lAmount = llTemp;
  // 读入买一数量
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVBuy.at(0) = lTemp;
  // 读入买一价格
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPBuy.at(0) = lTemp;
  // 读入买二数量
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVBuy.at(1) = lTemp;
  // 读入买二价格
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPBuy.at(1) = lTemp;
  // 读入买三数量
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVBuy.at(2) = lTemp;
  // 读入买三价格
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPBuy.at(2) = lTemp;
  // 读入买四数量
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVBuy.at(3) = lTemp;
  // 读入买四价格
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPBuy.at(3) = lTemp;
  // 读入买五数量
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVBuy.at(4) = lTemp;
  // 读入买五价格
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPBuy.at(4) = lTemp;
  // 读入卖一数量
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVSell.at(0) = lTemp;
  // 读入卖一价格
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPSell.at(0) = lTemp;
  // 读入卖二数量
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVSell.at(1) = lTemp;
  // 读入卖二价格
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPSell.at(1) = lTemp;
  // 读入卖三数量
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVSell.at(2) = lTemp;
  // 读入卖三价格
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPSell.at(2) = lTemp;
  // 读入卖四数量
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVSell.at(3) = lTemp;
  // 读入卖四价格
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPSell.at(3) = lTemp;
  // 读入卖五数量
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVSell.at(4) = lTemp;
  // 读入卖五价格
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPSell.at(4) = lTemp;
  // 读入成交日期和时间
  i = 0;
  while (*pCurrentPos != ',') {
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
      return false;
    }
    buffer3[i++] = *pCurrentPos++;
    iTotalRead++;
  }

  pCurrentPos++;
  iTotalRead++;

  buffer3[i++] = ' ';
  while (*pCurrentPos != ',') {
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
      return false;
    }
    buffer3[i++] = *pCurrentPos++;
    iTotalRead++;
  }
  buffer3[i] = 0x000;

  tm tm_;
  int year, month, day, hour, minute, second;
  sscanf_s(buffer3, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
  tm_.tm_year = year - 1900;
  tm_.tm_mon = month - 1;
  tm_.tm_mday = day;
  tm_.tm_hour = hour;
  tm_.tm_min = minute;
  tm_.tm_sec = second;
  tm_.tm_isdst = 0;
  m_time = mktime(&tm_);

  while (*pCurrentPos++ != 0x00a) {
    if (*pCurrentPos == 0x000) {
      return false;
    }
    iTotalRead++;
  }
  iTotalRead++;
  m_fActive = true;

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个值，遇到逗号结束。
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CStockRTData::ReadOneValue(char*& pCurrentPos, char* buffer, long& lCounter)
{
  int i = 0;
  while (*pCurrentPos != ',') {
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) return false;
    buffer[i++] = *pCurrentPos++;
  }
  buffer[i] = 0x000;
  pCurrentPos++;
  i++;
  lCounter += i;
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入浮点数，小数点后保留三位，不足就加上0.，多于三位就抛弃。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CStockRTData::ReadOneValueExceptperiod(char*& pCurrentPos, char* buffer, long& lCounter)
{
  int i = 0;
  bool fFoundPoint = false;
  int iCount = 0;
  while ((*pCurrentPos != ',') && (iCount < 3)) {
    if (fFoundPoint) iCount++;
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) return false;
    if (*pCurrentPos == '.') {
      fFoundPoint = true;
      pCurrentPos++;
    }
    else buffer[i++] = *pCurrentPos++;
  }

  if (fFoundPoint && (iCount < 3)) {
    int jCount = i;
    for (int j = iCount; j < 3; j++) {
      buffer[jCount++] = '0';
    }
    buffer[jCount] = 0x000;
  }
  else {
    buffer[i] = 0x000;
  }

  while (*pCurrentPos != ',') {
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) return false;
    i++;
    pCurrentPos++;
  }
  pCurrentPos++;
  i++;
  if (fFoundPoint) i++;
  lCounter += i; // 多加1，是需要加上少算的逗号

  return true;
}