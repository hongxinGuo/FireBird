#include "stdafx.h"

#include"globedef.h"
#include "DayLine.h"

CDayLine::CDayLine() : CChinaStockHistoryData() {
  Reset();
}

void CDayLine::Reset(void) {
  CChinaStockHistoryData::Reset();
}

bool CDayLine::LoadExtendData(CSetDLExtendInfo* psetDLExtendInfo) {
  ASSERT(psetDLExtendInfo->IsOpen());
  m_lTransactionNumber = atol(psetDLExtendInfo->m_TransactionNumber);
  m_lTransactionNumberBelow5000 = atol(psetDLExtendInfo->m_TransactionNumberBelow5000);
  m_lTransactionNumberBelow50000 = atol(psetDLExtendInfo->m_TransactionNumberBelow50000);
  m_lTransactionNumberBelow200000 = atol(psetDLExtendInfo->m_TransactionNumberBelow200000);
  m_lTransactionNumberAbove200000 = atol(psetDLExtendInfo->m_TransactionNumberAbove200000);
  m_lAttackBuyVolume = atol(psetDLExtendInfo->m_AttackBuyVolume);
  m_lStrongBuyVolume = atol(psetDLExtendInfo->m_StrongBuyVolume);
  m_lAttackSellVolume = atol(psetDLExtendInfo->m_AttackSellVolume);
  m_lStrongSellVolume = atol(psetDLExtendInfo->m_StrongSellVolume);
  m_lUnknownVolume = atol(psetDLExtendInfo->m_UnknownVolume);
  m_lCanceledBuyVolume = atol(psetDLExtendInfo->m_CanceledBuyVolume);
  m_lCanceledSellVolume = atol(psetDLExtendInfo->m_CanceledSellVolume);
  m_lOrdinaryBuyVolume = atol(psetDLExtendInfo->m_OrdinaryBuyVolume);
  m_lAttackBuyBelow50000 = atol(psetDLExtendInfo->m_AttackBuyBelow50000);
  m_lAttackBuyBelow200000 = atol(psetDLExtendInfo->m_AttackBuyBelow200000);
  m_lAttackBuyAbove200000 = atol(psetDLExtendInfo->m_AttackBuyAbove200000);
  m_lOrdinarySellVolume = atol(psetDLExtendInfo->m_OrdinarySellVolume);
  m_lAttackSellBelow50000 = atol(psetDLExtendInfo->m_AttackSellBelow50000);
  m_lAttackSellBelow200000 = atol(psetDLExtendInfo->m_AttackSellBelow200000);
  m_lAttackSellAbove200000 = atol(psetDLExtendInfo->m_AttackSellAbove200000);

  m_lOrdinaryBuyVolumeBelow5000 = atoll(psetDLExtendInfo->m_OrdinaryBuyVolumeBelow5000);
  m_lOrdinaryBuyVolumeBelow10000 = atoll(psetDLExtendInfo->m_OrdinaryBuyVolumeBelow10000);
  m_lOrdinaryBuyVolumeBelow20000 = atoll(psetDLExtendInfo->m_OrdinaryBuyVolumeBelow20000);
  m_lOrdinaryBuyVolumeBelow50000 = atoll(psetDLExtendInfo->m_OrdinaryBuyVolumeBelow50000);
  m_lOrdinaryBuyVolumeBelow100000 = atoll(psetDLExtendInfo->m_OrdinaryBuyVolumeBelow100000);
  m_lOrdinaryBuyVolumeBelow200000 = atoll(psetDLExtendInfo->m_OrdinaryBuyVolumeBelow200000);
  m_lOrdinaryBuyVolumeAbove200000 = atoll(psetDLExtendInfo->m_OrdinaryBuyVolumeAbove200000);
  m_lOrdinarySellVolumeBelow5000 = atoll(psetDLExtendInfo->m_OrdinarySellVolumeBelow5000);
  m_lOrdinarySellVolumeBelow10000 = atoll(psetDLExtendInfo->m_OrdinarySellVolumeBelow10000);
  m_lOrdinarySellVolumeBelow20000 = atoll(psetDLExtendInfo->m_OrdinarySellVolumeBelow20000);
  m_lOrdinarySellVolumeBelow50000 = atoll(psetDLExtendInfo->m_OrdinarySellVolumeBelow50000);
  m_lOrdinarySellVolumeBelow100000 = atoll(psetDLExtendInfo->m_OrdinarySellVolumeBelow100000);
  m_lOrdinarySellVolumeBelow200000 = atoll(psetDLExtendInfo->m_OrdinarySellVolumeBelow200000);
  m_lOrdinarySellVolumeAbove200000 = atoll(psetDLExtendInfo->m_OrdinarySellVolumeAbove200000);

  m_lOrdinaryBuyNumberBelow5000 = atoll(psetDLExtendInfo->m_OrdinaryBuyNumberBelow5000);
  m_lOrdinaryBuyNumberBelow10000 = atoll(psetDLExtendInfo->m_OrdinaryBuyNumberBelow10000);
  m_lOrdinaryBuyNumberBelow20000 = atoll(psetDLExtendInfo->m_OrdinaryBuyNumberBelow20000);
  m_lOrdinaryBuyNumberBelow50000 = atoll(psetDLExtendInfo->m_OrdinaryBuyNumberBelow50000);
  m_lOrdinaryBuyNumberBelow100000 = atoll(psetDLExtendInfo->m_OrdinaryBuyNumberBelow100000);
  m_lOrdinaryBuyNumberBelow200000 = atoll(psetDLExtendInfo->m_OrdinaryBuyNumberBelow200000);
  m_lOrdinaryBuyNumberAbove200000 = atoll(psetDLExtendInfo->m_OrdinaryBuyNumberAbove200000);
  m_lOrdinarySellNumberBelow5000 = atoll(psetDLExtendInfo->m_OrdinarySellNumberBelow5000);
  m_lOrdinarySellNumberBelow10000 = atoll(psetDLExtendInfo->m_OrdinarySellNumberBelow10000);
  m_lOrdinarySellNumberBelow20000 = atoll(psetDLExtendInfo->m_OrdinarySellNumberBelow20000);
  m_lOrdinarySellNumberBelow50000 = atoll(psetDLExtendInfo->m_OrdinarySellNumberBelow50000);
  m_lOrdinarySellNumberBelow100000 = atoll(psetDLExtendInfo->m_OrdinarySellNumberBelow100000);
  m_lOrdinarySellNumberBelow200000 = atoll(psetDLExtendInfo->m_OrdinarySellNumberBelow200000);
  m_lOrdinarySellNumberAbove200000 = atoll(psetDLExtendInfo->m_OrdinarySellNumberAbove200000);

  m_lCanceledBuyVolumeBelow5000 = atoll(psetDLExtendInfo->m_CanceledBuyVolumeBelow5000);
  m_lCanceledBuyVolumeBelow10000 = atoll(psetDLExtendInfo->m_CanceledBuyVolumeBelow10000);
  m_lCanceledBuyVolumeBelow20000 = atoll(psetDLExtendInfo->m_CanceledBuyVolumeBelow20000);
  m_lCanceledBuyVolumeBelow50000 = atoll(psetDLExtendInfo->m_CanceledBuyVolumeBelow50000);
  m_lCanceledBuyVolumeBelow100000 = atoll(psetDLExtendInfo->m_CanceledBuyVolumeBelow100000);
  m_lCanceledBuyVolumeBelow200000 = atoll(psetDLExtendInfo->m_CanceledBuyVolumeBelow200000);
  m_lCanceledBuyVolumeAbove200000 = atoll(psetDLExtendInfo->m_CanceledBuyVolumeAbove200000);
  m_lCanceledSellVolumeBelow5000 = atoll(psetDLExtendInfo->m_CanceledSellVolumeBelow5000);
  m_lCanceledSellVolumeBelow10000 = atoll(psetDLExtendInfo->m_CanceledSellVolumeBelow10000);
  m_lCanceledSellVolumeBelow20000 = atoll(psetDLExtendInfo->m_CanceledSellVolumeBelow20000);
  m_lCanceledSellVolumeBelow50000 = atoll(psetDLExtendInfo->m_CanceledSellVolumeBelow50000);
  m_lCanceledSellVolumeBelow100000 = atoll(psetDLExtendInfo->m_CanceledSellVolumeBelow100000);
  m_lCanceledSellVolumeBelow200000 = atoll(psetDLExtendInfo->m_CanceledSellVolumeBelow200000);
  m_lCanceledSellVolumeAbove200000 = atoll(psetDLExtendInfo->m_CanceledSellVolumeAbove200000);

  return true;
}

bool CDayLine::SaveData(CSetDLBasicInfo* psetDLBasicInfo) {
  ASSERT(psetDLBasicInfo->IsOpen());

  psetDLBasicInfo->m_Date = GetFormatedMarketDate();
  psetDLBasicInfo->m_Market = GetMarket();
  psetDLBasicInfo->m_StockCode = GetStockCode();
  psetDLBasicInfo->m_StockName = GetStockName();
  psetDLBasicInfo->m_LastClose = ConvertValueToString(GetLastClose(), 1000);
  psetDLBasicInfo->m_High = ConvertValueToString(GetHigh(), 1000);
  psetDLBasicInfo->m_Low = ConvertValueToString(GetLow(), 1000);
  psetDLBasicInfo->m_Open = ConvertValueToString(GetOpen(), 1000);
  psetDLBasicInfo->m_Close = ConvertValueToString(GetClose(), 1000);
  psetDLBasicInfo->m_Volume = ConvertValueToString(GetVolume());
  psetDLBasicInfo->m_Amount = ConvertValueToString(GetAmount());
  psetDLBasicInfo->m_UpAndDown = ConvertValueToString(GetUpDown());
  psetDLBasicInfo->m_UpDownRate = ConvertValueToString(GetUpDownRate());
  psetDLBasicInfo->m_ChangeHandRate = ConvertValueToString(GetChangeHandRate());
  psetDLBasicInfo->m_TotalValue = ConvertValueToString(GetTotalValue());
  psetDLBasicInfo->m_CurrentValue = ConvertValueToString(GetCurrentValue());
  psetDLBasicInfo->m_RS = ConvertValueToString(GetRS());
  psetDLBasicInfo->m_RSIndex = ConvertValueToString(GetRSIndex());
  psetDLBasicInfo->m_RSBackup = ConvertValueToString(GetRSBackup());

  return true;
}

bool CDayLine::AppendData(CSetDLBasicInfo* psetDLBasicInfo) {
  ASSERT(psetDLBasicInfo->IsOpen());
  psetDLBasicInfo->AddNew();
  SaveData(psetDLBasicInfo);
  psetDLBasicInfo->Update();

  return true;
}

bool CDayLine::LoadBasicData(CSetDLBasicInfo* psetDLBasicInfo) {
  ASSERT(psetDLBasicInfo->IsOpen());
  m_lDate = psetDLBasicInfo->m_Date;
  m_wMarket = psetDLBasicInfo->m_Market;
  m_strStockCode = psetDLBasicInfo->m_StockCode;
  m_strStockName = psetDLBasicInfo->m_StockName;
  m_lLastClose = atof(psetDLBasicInfo->m_LastClose) * 1000;
  m_lOpen = atof(psetDLBasicInfo->m_Open) * 1000;
  m_lHigh = atof(psetDLBasicInfo->m_High) * 1000;
  m_lLow = atof(psetDLBasicInfo->m_Low) * 1000;
  m_lClose = atof(psetDLBasicInfo->m_Close) * 1000;
  m_dUpDown = atof(psetDLBasicInfo->m_UpAndDown);
  m_llVolume = atoll(psetDLBasicInfo->m_Volume);
  m_llAmount = atoll(psetDLBasicInfo->m_Amount);
  m_dUpDownRate = atof(psetDLBasicInfo->m_UpDownRate);
  m_dChangeHandRate = atof(psetDLBasicInfo->m_ChangeHandRate);
  m_llTotalValue = atoll(psetDLBasicInfo->m_TotalValue);
  m_llCurrentValue = atoll(psetDLBasicInfo->m_CurrentValue);
  m_dRS = atof(psetDLBasicInfo->m_RS);
  m_dRSIndex = atof(psetDLBasicInfo->m_RSIndex);
  m_dRSBackup = atof(psetDLBasicInfo->m_RSBackup);
  CalculateRSLogarithm1(m_dRS);
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理一条日线数据。采用网易日线历史数据格式。
//
// 与实时数据相类似，各种价格皆放大一千倍后以长整型存储。存入数据库时以DECIMAL(10,3)类型存储。
// 字符串的制式为：2019-07-10,600000,浦东银行,收盘价,最高价,最低价,开盘价,前收盘价,涨跌值,涨跌比率,换手率,成交股数,成交金额,总市值,流通市值\r\n
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDayLine::ProcessNeteaseData(CString strStockCode, char*& pCurrentPos, INT64& lLength) {
  long iCount = 0;
  static char buffer2[200], buffer3[100];
  long i = 0;
  tm tm_;
  int year = 0, month = 0, day = 0;
  long lDate = 0;
  CString str;
  double dTemp = 0;

  i = 0;
  while ((*pCurrentPos != 0x02c)) { // 读取日期，直到遇到逗号
    if ((*pCurrentPos == 0x0d) || (*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000) || (i > 30)) { // 如果遇到回车、换行、字符串结束符或者读取了20个字符
      return false; // 数据出错，放弃载入
    }
    buffer3[i++] = *pCurrentPos++;
    iCount++;
  }
  pCurrentPos++;
  iCount++;
  buffer3[i] = 0x00;
  sscanf_s(buffer3, _T("%04d-%02d-%02d"), &year, &month, &day);
  tm_.tm_year = year - 1900;
  tm_.tm_mon = month - 1;
  tm_.tm_mday = day;
  tm_.tm_hour = 15;
  tm_.tm_min = 0;
  tm_.tm_sec = 0;
  tm_.tm_isdst = 0;
  SetTime(mktime(&tm_));
  lDate = year * 10000 + month * 100 + day;
  SetDate(lDate);
  //TRACE("%d %d %d\n", year, month, day);

  if (*pCurrentPos != 0x027) return(false); // 不是单引号(')，数据出错，放弃载入
  pCurrentPos++;
  iCount++;

  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  str = buffer2;
  SetStockCode(strStockCode);
  str = strStockCode.Left(2);
  if (str == _T("sh")) {
    SetMarket(__SHANGHAI_MARKET__);
  }
  else if (str == _T("sz")) {
    SetMarket(__SHENZHEN_MARKET__);
  }
  else {
    return false;
  }
  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  str = buffer2;
  SetStockName(str);

  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  dTemp = atof(buffer2);
  SetClose(dTemp * 1000);

  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  dTemp = atof(buffer2);
  SetHigh(dTemp * 1000);

  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  dTemp = atof(buffer2);
  SetLow(dTemp * 1000);

  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  dTemp = atof(buffer2);
  SetOpen(dTemp * 1000);

  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  dTemp = atof(buffer2);
  SetLastClose(dTemp * 1000);

  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  if (GetOpen() == 0) {
    //ASSERT(strcmp(buffer2, _T("None") == 0);
    SetUpDown(0.0);
  }
  else SetUpDown(buffer2);

  if (GetLastClose() == 0) { // 设置涨跌幅。
    SetUpDownRate(0.0); // 如果昨日收盘价为零（没交易），则涨跌幅也设为零。
  }
  else {
    // 需要放大1000 * 100倍。收盘价比实际值大1000倍，记录的是百分比，也要增大100倍。
    SetUpDownRate(((double)(GetUpDown() * 100000.0)) / GetLastClose());
  }

  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  SetChangeHandRate(buffer2);

  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  SetVolume(buffer2); // 读入的是股数

  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  SetAmount(buffer2);

  // 总市值的数据有两种形式，需要程序判定
  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  SetTotalValue(buffer2); // 总市值的单位为：元

  // 流通市值不是用逗号结束，故而不能使用ReadOneValueFromNeteaseDL函数
  // 流通市值的数据形式有两种，故而需要程序判定。
  i = 0;
  while (*pCurrentPos != 0x00d) {
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000) || (i > 30)) return false; // 数据出错，放弃载入
    buffer2[i++] = *pCurrentPos++;
    iCount++;
  }
  pCurrentPos++;
  iCount++;
  buffer2[i] = 0x000;
  SetCurrentValue(buffer2); // 流通市值的单位为：元。
  // \r后面紧跟着应该是\n
  if (*pCurrentPos++ != 0x0a) return false; // 数据出错，放弃载入
  iCount++;

  lLength += iCount;
  return true;
}

bool CDayLine::IsActive(void) {
  if ((GetClose() != 0) && (GetLastClose() != 0)) {
    return true;
  }
  else return false;
}