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
  m_llVolume = 0;
  m_llAmount = 0;
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

  if (data.m_llVolume != 0) {
    m_llVolume = data.m_llVolume;
    m_llAmount = data.m_llAmount;
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
  m_llVolume = 0;
  m_llAmount = 0;
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
  m_llVolume = data.m_llVolume;
  m_llAmount = data.m_llAmount;
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
// 从网络文件file中读取新浪制式实时数据，返回值是所读数据是否出现格式错误。
//
//  新浪实时行情站点：http://hq.sinajs.cn/
// var hq_str_sh601006=”大秦铁路,27.55,27.25,26.91,27.55,26.20,26.91,26.92,
//                     22114263,589824680,4695,26.91,57590,26.90,14700,26.89,14300,
//                     26.88,15100,26.87,3100,26.92,8900,26.93,14230,26.94,25150,26.95,15220,26.96,2008-01-11,15:05:32”;
//
// 这个字符串由许多数据拼接在一起，不同含义的数据用逗号隔开了，按照程序员的思路，顺序号从0开始。
// 0：”大秦铁路”，股票名字；
// 1：”27.55″，今日开盘价；
// 2：”27.25″，昨日收盘价；
// 3：”26.91″，当前价格；
// 4：”27.55″，今日最高价；
// 5：”26.20″，今日最低价；
// 6：”26.91″，竞买价，即“买一”报价；
// 7：”26.92″，竞卖价，即“卖一”报价；
// 8：”22114263″，成交的股票数，由于股票交易以一百股为基本单位，所以在使用时，通常把该值除以一百；
// 9：”589824680″，成交金额，单位为“元”，为了一目了然，通常以“万元”为成交金额的单位，所以通常把该值除以一万；
// 10：”4695″，“买一”申请4695股，即47手；
// 11：”26.91″，“买一”报价；
// 12：”57590″，“买二”
// 13：”26.90″，“买二”
// 14：”14700″，“买三”
// 15：”26.89″，“买三”
// 16：”14300″，“买四”
// 17：”26.88″，“买四”
// 18：”15100″，“买五”
// 19：”26.87″，“买五”
// 20：”3100″，“卖一”申报3100股，即31手；
// 21：”26.92″，“卖一”报价
// (22, 23), (24, 25), (26, 27), (28, 29)分别为“卖二”至“卖四的情况”
// 30：”2008 - 01 - 11″，日期；
// 31：”15:05 : 32″，时间；
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CStockRTData::ReadSinaData(char*& pCurrentPos, long& lTotalRead)
{
  static char buffer1[200];
  char buffer2[7];
  static char buffer3[200];
  static CString strHeader = _T("var hq_str_s");

  m_fActive = false;    // 初始状态为无效数据
  strncpy_s(buffer1, pCurrentPos, 12); // 读入“var hq_str_s"
  buffer1[12] = 0x000;
  CString str1;
  str1 = buffer1;
  if (strHeader.Compare(str1) != 0) { // 数据格式出错
    return false;
  }
  pCurrentPos += 12;
  lTotalRead += 12;

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
  lTotalRead += 1;

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
  lTotalRead += 6;

  strncpy_s(buffer1, pCurrentPos, 2); // 读入'="'
  if (buffer1[0] != '=') {
    return false;
  }
  if (buffer1[1] != '"') {
    return false;
  }
  pCurrentPos += 2;
  lTotalRead += 2;
  strncpy_s(buffer1, pCurrentPos, 2);
  if (buffer1[0] == '"') { // 没有数据
    if (buffer1[1] != ';') {
      return false;
    }
    lTotalRead += 2;
    pCurrentPos += 2;
    if (*pCurrentPos++ != 0x00a) {
      return false; // 确保是字符 \n
    }
    lTotalRead++;
    return true;
  }
  if ((buffer1[0] == 0x00a) || (buffer1[0] == 0x000)) {
    return false;
  }
  if ((buffer1[1] == 0x00a) || (buffer1[1] == 0x000)) {
    return false;
  }
  pCurrentPos += 2;
  lTotalRead += 2;

  int i = 2;
  while (*pCurrentPos != 0x02c) { // 读入剩下的中文名字（第一个字在buffer1中）
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
      return false;
    }
    buffer1[i++] = *pCurrentPos++;
    lTotalRead++;
  }
  buffer1[i] = 0x000;
  m_strStockName = buffer1; // 设置股票名称

  pCurrentPos++;
  lTotalRead++;

  // 读入开盘价。放大一千倍后存储为长整型。其他价格亦如此。
  if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lOpen, lTotalRead)) {
    return false;
  }
  // 读入前收盘价
  if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lLastClose, lTotalRead)) {
    return false;
  }
  // 读入当前价
  if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lNew, lTotalRead)) {
    return false;
  }
  // 读入最高价
  if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lHigh, lTotalRead)) {
    return false;
  }
  // 读入最低价
  if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lLow, lTotalRead)) {
    return false;
  }
  // 读入竞买价
  if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lBuy, lTotalRead)) {
    return false;
  }
  // 读入竞卖价
  if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lSell, lTotalRead)) {
    return false;
  }
  // 读入成交股数。成交股数存储实际值
  if (!ReadSinaOneValue(pCurrentPos, m_llVolume, lTotalRead)) {
    return false;
  }
  // 读入成交金额
  if (!ReadSinaOneValue(pCurrentPos, m_llAmount, lTotalRead)) {
    return false;
  }
  // 读入买一--买五的股数和价格
  for (int j = 0; j < 5; j++) {
    // 读入数量
    if (!ReadSinaOneValue(pCurrentPos, m_lVBuy.at(j), lTotalRead)) {
      return false;
    }
    // 读入价格
    if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lPBuy.at(j), lTotalRead)) {
      return false;
    }
  }
  // 读入卖一--卖五的股数和价格
  for (int j = 0; j < 5; j++) {
    // 读入数量
    if (!ReadSinaOneValue(pCurrentPos, m_lVSell.at(j), lTotalRead)) {
      return false;
    }
    // 读入价格
    if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lPSell.at(j), lTotalRead)) {
      return false;
    }
  }
  // 读入成交日期和时间
  i = 0;
  while (*pCurrentPos != ',') { // 读入日期。格式为:yyyy-mm-dd
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
      return false;
    }
    buffer3[i++] = *pCurrentPos++;
    lTotalRead++;
  }
  pCurrentPos++;
  lTotalRead++;

  buffer3[i++] = ' '; // 添加一个空格，分隔日期和时间
  while (*pCurrentPos != ',') { // 读入时间，格式为hh:mm:ss
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
      return false;
    }
    buffer3[i++] = *pCurrentPos++;
    lTotalRead++;
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
    lTotalRead++;
  }
  lTotalRead++;
  m_fActive = true;

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个INT64值，遇到逗号结束。返回值在llReturnValue中
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CStockRTData::ReadSinaOneValue(char*& pCurrentPos, INT64& llReturnValue, long& lTotalRead) {
  INT64 llTemp;
  static char buffer3[200];

  if (!ReadSinaOneValue(pCurrentPos, buffer3, lTotalRead)) {
    return false;
  }
  llTemp = atoll(buffer3);
  if (llTemp < 0) return false;
  if (llTemp > 0) llReturnValue = llTemp;
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个值，遇到逗号结束。返回值在lReturnValue中
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CStockRTData::ReadSinaOneValue(char*& pCurrentPos, long& lReturnValue, long& lTotalRead) {
  long lTemp;
  static char buffer3[200];

  if (!ReadSinaOneValue(pCurrentPos, buffer3, lTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) lReturnValue = lTemp;
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个值，遇到逗号结束。
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CStockRTData::ReadSinaOneValue(char*& pCurrentPos, char* buffer, long& lTotalRead)
{
  int i = 0;
  while (*pCurrentPos != ',') {
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) return false;
    buffer[i++] = *pCurrentPos++;
  }
  buffer[i] = 0x000;
  pCurrentPos++;
  i++;
  lTotalRead += i;
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入浮点数，小数点后保留三位，不足就加上0.，多于三位就抛弃。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CStockRTData::ReadSinaOneValueExceptPeriod(char*& pCurrentPos, long& lReturnValue, long& lTotalRead) {
  long lTemp;
  static char buffer3[200];

  if (!ReadSinaOneValueExceptPeriod(pCurrentPos, buffer3, lTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) lReturnValue = lTemp;
  return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入浮点数，小数点后保留三位，不足就加上0.，多于三位就抛弃。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CStockRTData::ReadSinaOneValueExceptPeriod(char*& pCurrentPos, char* buffer, long& lCounter)
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

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 从网络文件file中读取腾讯制式实时数据，返回值是所读数据是否出现格式错误。
//
// 要获取最新行情，访问数据接口：http://qt.gtimg.cn/q=sz002818
//
// v_sz000001 = "51~平安银行~000001~15.59~15.90~15.75~1046363~518391~527971~
//              15.58~2365~15.57~802~15.56~1855~15.55~2316~15.54~320~15.59~661~15.60~15381~15.61~3266~15.62~450~15.63~520~~
//              20190930154003~-0.31~-1.95~15.89~15.57~15.59/1046363/1645828527~1046363~164583~0.54~11.27~~
//              15.89~15.57~2.01~3025.36~3025.38~1.15~17.49~14.31~
//              0.73~-12617~15.73~9.82~12.19~~~1.24~164582.85~0.00~0~~GP-A~68.91~~0.82";
//
// 0: 不明
// 1 : 名字
// 2 : 代码
// 3 : 现价
// 4 : 昨收
// 5 : 今开
// 6 : 成交量（手）
// 7 : 外盘
// 8 : 内盘 （第一行结束）
// 9 : 买一
// 10 : 买一量（手）
// 11 - 18 : 买二 - 买五
// 19 : 卖一
// 20 : 卖一量
// 21 - 28 : 卖二 - 卖五
// 29 : 最近逐笔成交 ( 第二行结束）
// 30 : 时间
// 31 : 涨跌
// 32 : 涨跌 %
// 33 : 最高
// 34 : 最低
// 35 : 价格 / 成交量（手） / 成交额
// 36 : 成交量（手）
// 37 : 成交额（万）
// 38 : 换手率
// 39 : 市盈率
// 40 : /       (第三行结束）
// 41 : 最高
// 42 : 最低
// 43 : 振幅
// 44 : 流通市值
// 45 : 总市值
// 46 : 市净率
// 47 : 涨停价
// 48 : 跌停价   （第四行结束）
// 49 ：        之后的这些数字不清楚其含义
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CStockRTData::ReadTengxunData(char*& pCurrentPos, long& lTotalRead)
{
  static char buffer1[200];
  char buffer2[7];
  static char buffer3[200];
  static CString strHeader = _T("v=s");
  long lTemp = 0;
  INT64 llTemp = 0;

  m_fActive = false;    // 初始状态为无效数据
  strncpy_s(buffer1, pCurrentPos, 3); // 读入“v=s"
  buffer1[3] = 0x000;
  CString str1;
  str1 = buffer1;
  if (strHeader.Compare(str1) != 0) { // 数据格式出错
    return false;
  }
  pCurrentPos += 3;
  lTotalRead += 3;

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
  lTotalRead += 1;

  // 读入六位股票代码
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
  lTotalRead += 6;

  strncpy_s(buffer1, pCurrentPos, 2); // 读入'="'
  if (buffer1[0] != '=') {
    return false;
  }
  if (buffer1[1] != '"') {
    return false;
  }
  pCurrentPos += 2;
  lTotalRead += 2;

  // 读入不明代码（51或者1）
  if (!ReadTengxunOneValue(pCurrentPos, lTemp, lTotalRead)) {
    return false;
  }

  int i = 0;
  while (*pCurrentPos != '~') { // 读入股票的中文名字
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
      return false;
    }
    buffer1[i++] = *pCurrentPos++;
    lTotalRead++;
  }
  buffer1[i] = 0x000;
  m_strStockName = buffer1; // 设置股票名称
  pCurrentPos++;
  lTotalRead++;

  // 读入六位股票代码
  if (!ReadTengxunOneValue(pCurrentPos, lTemp, lTotalRead)) {
    return false;
  }
  if (lTemp != m_iStockCode) return false;

  // 读入现在成交价。放大一千倍后存储为长整型。其他价格亦如此。
  if (!ReadTengxunOneValue(pCurrentPos, m_lNew, lTotalRead)) {
    return false;
  }
  // 读入前收盘价
  if (!ReadTengxunOneValue(pCurrentPos, m_lLastClose, lTotalRead)) {
    return false;
  }
  // 读入开盘价
  if (!ReadTengxunOneValue(pCurrentPos, m_lOpen, lTotalRead)) {
    return false;
  }

  // 读入成交手数。成交股数存储实际值
  if (!ReadTengxunOneValue(pCurrentPos, llTemp, lTotalRead)) {
    return false;
  }
  m_llVolume = llTemp * 100; 
  // 读入外盘
  if (!ReadTengxunOneValue(pCurrentPos, lTemp, lTotalRead)) {
    return false;
  }
  // 读入内盘
  if (!ReadTengxunOneValue(pCurrentPos, lTemp, lTotalRead)) {
    return false;
  }
  // 读入买一至买五的价格和手数
  for (int j = 0; j < 5; j++) {
    // 读入买一价
    if (!ReadTengxunOneValue(pCurrentPos, m_lPBuy.at(j), lTotalRead)) {
      return false;
    }
    // 读入买一数量（手）
    if (!ReadTengxunOneValue(pCurrentPos, lTemp, lTotalRead)) {
      return false;
    }
    if (lTemp > 0) m_lVBuy.at(j) = lTemp * 100;
  }
  // 读入卖一至卖五的价格和手数
  for (int j = 0; j < 5; j++) {
    // 读入买一价
    if (!ReadTengxunOneValue(pCurrentPos, m_lPSell.at(j), lTotalRead)) {
      return false;
    }
    // 读入买一数量（手）
    if (!ReadTengxunOneValue(pCurrentPos, lTemp, lTotalRead)) {
      return false;
    }
    if (lTemp > 0) m_lVSell.at(j) = lTemp * 100;
  }
  // 最近逐笔成交
  if (!ReadTengxunOneValue(pCurrentPos, lTemp, lTotalRead)) {
    return false;
  }

  // 读入成交日期和时间
  if (!ReadTengxunOneValue(pCurrentPos, buffer3, lTotalRead)) {
    return false;
  }

  tm tm_;
  int year, month, day, hour, minute, second;
  sscanf_s(buffer3, "%4d%2d%2d%2d%2d%2d", &year, &month, &day, &hour, &minute, &second);
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
    lTotalRead++;
  }
  lTotalRead++;
  m_fActive = true;

  return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个INT64值，遇到~号结束。返回值在llReturnValue中
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CStockRTData::ReadTengxunOneValue(char*& pCurrentPos, INT64& llReturnValue, long& lTotalRead) {
  INT64 llTemp;
  static char buffer3[200];

  if (!ReadTengxunOneValue(pCurrentPos, buffer3, lTotalRead)) {
    return false;
  }
  llTemp = atoll(buffer3);
  if (llTemp < 0) return false;
  if (llTemp > 0) llReturnValue = llTemp;
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个值，遇到~号结束。返回值在lReturnValue中
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CStockRTData::ReadTengxunOneValue(char*& pCurrentPos, long& lReturnValue, long& lTotalRead) {
  long lTemp;
  static char buffer3[200];

  if (!ReadTengxunOneValue(pCurrentPos, buffer3, lTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) lReturnValue = lTemp;
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个值，遇到~号结束。
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CStockRTData::ReadTengxunOneValue(char*& pCurrentPos, char* buffer, long& lTotalRead)
{
  int i = 0;
  while (*pCurrentPos != '~') {
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) return false;
    buffer[i++] = *pCurrentPos++;
  }
  buffer[i] = 0x000;
  pCurrentPos++;
  i++;
  lTotalRead += i;
  return true;
}
