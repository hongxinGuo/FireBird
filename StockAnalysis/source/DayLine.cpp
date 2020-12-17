#include "stdafx.h"

#include"globedef.h"
#include "DayLine.h"

CDayLine::CDayLine() : CChinaStockHistoryData() {
  Reset();
}

void CDayLine::Reset(void) {
  CChinaStockHistoryData::Reset();
}

bool CDayLine::LoadChinaMarketExtendData(not_null<CSetDayLineExtendInfo*> psetDayLineExtendInfo) {
  ASSERT(psetDayLineExtendInfo->IsOpen());
  m_lTransactionNumber = atol(psetDayLineExtendInfo->m_TransactionNumber);
  m_lTransactionNumberBelow5000 = atol(psetDayLineExtendInfo->m_TransactionNumberBelow5000);
  m_lTransactionNumberBelow50000 = atol(psetDayLineExtendInfo->m_TransactionNumberBelow50000);
  m_lTransactionNumberBelow200000 = atol(psetDayLineExtendInfo->m_TransactionNumberBelow200000);
  m_lTransactionNumberAbove200000 = atol(psetDayLineExtendInfo->m_TransactionNumberAbove200000);
  m_lAttackBuyVolume = atol(psetDayLineExtendInfo->m_AttackBuyVolume);
  m_lStrongBuyVolume = atol(psetDayLineExtendInfo->m_StrongBuyVolume);
  m_lAttackSellVolume = atol(psetDayLineExtendInfo->m_AttackSellVolume);
  m_lStrongSellVolume = atol(psetDayLineExtendInfo->m_StrongSellVolume);
  m_lUnknownVolume = atol(psetDayLineExtendInfo->m_UnknownVolume);
  m_lCanceledBuyVolume = atol(psetDayLineExtendInfo->m_CanceledBuyVolume);
  m_lCanceledSellVolume = atol(psetDayLineExtendInfo->m_CanceledSellVolume);
  m_lOrdinaryBuyVolume = atol(psetDayLineExtendInfo->m_OrdinaryBuyVolume);
  m_lAttackBuyBelow50000 = atol(psetDayLineExtendInfo->m_AttackBuyBelow50000);
  m_lAttackBuyBelow200000 = atol(psetDayLineExtendInfo->m_AttackBuyBelow200000);
  m_lAttackBuyAbove200000 = atol(psetDayLineExtendInfo->m_AttackBuyAbove200000);
  m_lOrdinarySellVolume = atol(psetDayLineExtendInfo->m_OrdinarySellVolume);
  m_lAttackSellBelow50000 = atol(psetDayLineExtendInfo->m_AttackSellBelow50000);
  m_lAttackSellBelow200000 = atol(psetDayLineExtendInfo->m_AttackSellBelow200000);
  m_lAttackSellAbove200000 = atol(psetDayLineExtendInfo->m_AttackSellAbove200000);

  m_lOrdinaryBuyVolumeBelow5000 = atoll(psetDayLineExtendInfo->m_OrdinaryBuyVolumeBelow5000);
  m_lOrdinaryBuyVolumeBelow10000 = atoll(psetDayLineExtendInfo->m_OrdinaryBuyVolumeBelow10000);
  m_lOrdinaryBuyVolumeBelow20000 = atoll(psetDayLineExtendInfo->m_OrdinaryBuyVolumeBelow20000);
  m_lOrdinaryBuyVolumeBelow50000 = atoll(psetDayLineExtendInfo->m_OrdinaryBuyVolumeBelow50000);
  m_lOrdinaryBuyVolumeBelow100000 = atoll(psetDayLineExtendInfo->m_OrdinaryBuyVolumeBelow100000);
  m_lOrdinaryBuyVolumeBelow200000 = atoll(psetDayLineExtendInfo->m_OrdinaryBuyVolumeBelow200000);
  m_lOrdinaryBuyVolumeAbove200000 = atoll(psetDayLineExtendInfo->m_OrdinaryBuyVolumeAbove200000);
  m_lOrdinarySellVolumeBelow5000 = atoll(psetDayLineExtendInfo->m_OrdinarySellVolumeBelow5000);
  m_lOrdinarySellVolumeBelow10000 = atoll(psetDayLineExtendInfo->m_OrdinarySellVolumeBelow10000);
  m_lOrdinarySellVolumeBelow20000 = atoll(psetDayLineExtendInfo->m_OrdinarySellVolumeBelow20000);
  m_lOrdinarySellVolumeBelow50000 = atoll(psetDayLineExtendInfo->m_OrdinarySellVolumeBelow50000);
  m_lOrdinarySellVolumeBelow100000 = atoll(psetDayLineExtendInfo->m_OrdinarySellVolumeBelow100000);
  m_lOrdinarySellVolumeBelow200000 = atoll(psetDayLineExtendInfo->m_OrdinarySellVolumeBelow200000);
  m_lOrdinarySellVolumeAbove200000 = atoll(psetDayLineExtendInfo->m_OrdinarySellVolumeAbove200000);

  m_lOrdinaryBuyNumberBelow5000 = atoll(psetDayLineExtendInfo->m_OrdinaryBuyNumberBelow5000);
  m_lOrdinaryBuyNumberBelow10000 = atoll(psetDayLineExtendInfo->m_OrdinaryBuyNumberBelow10000);
  m_lOrdinaryBuyNumberBelow20000 = atoll(psetDayLineExtendInfo->m_OrdinaryBuyNumberBelow20000);
  m_lOrdinaryBuyNumberBelow50000 = atoll(psetDayLineExtendInfo->m_OrdinaryBuyNumberBelow50000);
  m_lOrdinaryBuyNumberBelow100000 = atoll(psetDayLineExtendInfo->m_OrdinaryBuyNumberBelow100000);
  m_lOrdinaryBuyNumberBelow200000 = atoll(psetDayLineExtendInfo->m_OrdinaryBuyNumberBelow200000);
  m_lOrdinaryBuyNumberAbove200000 = atoll(psetDayLineExtendInfo->m_OrdinaryBuyNumberAbove200000);
  m_lOrdinarySellNumberBelow5000 = atoll(psetDayLineExtendInfo->m_OrdinarySellNumberBelow5000);
  m_lOrdinarySellNumberBelow10000 = atoll(psetDayLineExtendInfo->m_OrdinarySellNumberBelow10000);
  m_lOrdinarySellNumberBelow20000 = atoll(psetDayLineExtendInfo->m_OrdinarySellNumberBelow20000);
  m_lOrdinarySellNumberBelow50000 = atoll(psetDayLineExtendInfo->m_OrdinarySellNumberBelow50000);
  m_lOrdinarySellNumberBelow100000 = atoll(psetDayLineExtendInfo->m_OrdinarySellNumberBelow100000);
  m_lOrdinarySellNumberBelow200000 = atoll(psetDayLineExtendInfo->m_OrdinarySellNumberBelow200000);
  m_lOrdinarySellNumberAbove200000 = atoll(psetDayLineExtendInfo->m_OrdinarySellNumberAbove200000);

  m_lCanceledBuyVolumeBelow5000 = atoll(psetDayLineExtendInfo->m_CanceledBuyVolumeBelow5000);
  m_lCanceledBuyVolumeBelow10000 = atoll(psetDayLineExtendInfo->m_CanceledBuyVolumeBelow10000);
  m_lCanceledBuyVolumeBelow20000 = atoll(psetDayLineExtendInfo->m_CanceledBuyVolumeBelow20000);
  m_lCanceledBuyVolumeBelow50000 = atoll(psetDayLineExtendInfo->m_CanceledBuyVolumeBelow50000);
  m_lCanceledBuyVolumeBelow100000 = atoll(psetDayLineExtendInfo->m_CanceledBuyVolumeBelow100000);
  m_lCanceledBuyVolumeBelow200000 = atoll(psetDayLineExtendInfo->m_CanceledBuyVolumeBelow200000);
  m_lCanceledBuyVolumeAbove200000 = atoll(psetDayLineExtendInfo->m_CanceledBuyVolumeAbove200000);
  m_lCanceledSellVolumeBelow5000 = atoll(psetDayLineExtendInfo->m_CanceledSellVolumeBelow5000);
  m_lCanceledSellVolumeBelow10000 = atoll(psetDayLineExtendInfo->m_CanceledSellVolumeBelow10000);
  m_lCanceledSellVolumeBelow20000 = atoll(psetDayLineExtendInfo->m_CanceledSellVolumeBelow20000);
  m_lCanceledSellVolumeBelow50000 = atoll(psetDayLineExtendInfo->m_CanceledSellVolumeBelow50000);
  m_lCanceledSellVolumeBelow100000 = atoll(psetDayLineExtendInfo->m_CanceledSellVolumeBelow100000);
  m_lCanceledSellVolumeBelow200000 = atoll(psetDayLineExtendInfo->m_CanceledSellVolumeBelow200000);
  m_lCanceledSellVolumeAbove200000 = atoll(psetDayLineExtendInfo->m_CanceledSellVolumeAbove200000);

  return true;
}

bool CDayLine::SaveChinaMarketData(not_null<CSetDayLineBasicInfo*> psetDayLineBasicInfo) {
  ASSERT(psetDayLineBasicInfo->IsOpen());

  psetDayLineBasicInfo->m_Date = GetFormatedMarketDate();
  psetDayLineBasicInfo->m_Market = GetMarket();
  psetDayLineBasicInfo->m_StockCode = GetStakeCode();
  psetDayLineBasicInfo->m_StockName = GetStakeName();
  psetDayLineBasicInfo->m_LastClose = ConvertValueToString(GetLastClose(), 1000);
  psetDayLineBasicInfo->m_High = ConvertValueToString(GetHigh(), 1000);
  psetDayLineBasicInfo->m_Low = ConvertValueToString(GetLow(), 1000);
  psetDayLineBasicInfo->m_Open = ConvertValueToString(GetOpen(), 1000);
  psetDayLineBasicInfo->m_Close = ConvertValueToString(GetClose(), 1000);
  psetDayLineBasicInfo->m_Volume = ConvertValueToString(GetVolume());
  psetDayLineBasicInfo->m_Amount = ConvertValueToString(GetAmount());
  psetDayLineBasicInfo->m_UpAndDown = ConvertValueToString(GetUpDown());
  psetDayLineBasicInfo->m_UpDownRate = ConvertValueToString(GetUpDownRate());
  psetDayLineBasicInfo->m_ChangeHandRate = ConvertValueToString(GetChangeHandRate());
  psetDayLineBasicInfo->m_TotalValue = ConvertValueToString(GetTotalValue());
  psetDayLineBasicInfo->m_CurrentValue = ConvertValueToString(GetCurrentValue());
  psetDayLineBasicInfo->m_RS = ConvertValueToString(GetRS());
  psetDayLineBasicInfo->m_RSIndex = ConvertValueToString(GetRSIndex());
  psetDayLineBasicInfo->m_RSBackup = ConvertValueToString(GetRSBackup());

  return true;
}

bool CDayLine::AppendChinaMarketData(not_null<CSetDayLineBasicInfo*> psetDayLineBasicInfo) {
  ASSERT(psetDayLineBasicInfo->IsOpen());
  psetDayLineBasicInfo->AddNew();
  SaveChinaMarketData(psetDayLineBasicInfo);
  psetDayLineBasicInfo->Update();

  return true;
}

bool CDayLine::LoadChinaMarketBasicData(not_null<CSetDayLineBasicInfo*> psetDayLineBasicInfo) {
  ASSERT(psetDayLineBasicInfo->IsOpen());
  m_lDate = psetDayLineBasicInfo->m_Date;
  m_wMarket = psetDayLineBasicInfo->m_Market;
  m_strStakeCode = psetDayLineBasicInfo->m_StockCode;
  m_strStakeName = psetDayLineBasicInfo->m_StockName;
  m_lLastClose = atof(psetDayLineBasicInfo->m_LastClose) * 1000;
  m_lOpen = atof(psetDayLineBasicInfo->m_Open) * 1000;
  m_lHigh = atof(psetDayLineBasicInfo->m_High) * 1000;
  m_lLow = atof(psetDayLineBasicInfo->m_Low) * 1000;
  m_lClose = atof(psetDayLineBasicInfo->m_Close) * 1000;
  m_dUpDown = atof(psetDayLineBasicInfo->m_UpAndDown);
  m_llVolume = atoll(psetDayLineBasicInfo->m_Volume);
  m_llAmount = atoll(psetDayLineBasicInfo->m_Amount);
  m_dUpDownRate = atof(psetDayLineBasicInfo->m_UpDownRate);
  m_dChangeHandRate = atof(psetDayLineBasicInfo->m_ChangeHandRate);
  m_llTotalValue = atoll(psetDayLineBasicInfo->m_TotalValue);
  m_llCurrentValue = atoll(psetDayLineBasicInfo->m_CurrentValue);
  m_dRS = atof(psetDayLineBasicInfo->m_RS);
  m_dRSIndex = atof(psetDayLineBasicInfo->m_RSIndex);
  m_dRSBackup = atof(psetDayLineBasicInfo->m_RSBackup);
  CalculateRSLogarithm1(m_dRS);
  return true;
}

bool CDayLine::SaveAmericaMarketData(not_null<CSetAmericaStakeDayLine*> psetAmericaStakeDayLine) {
  ASSERT(psetAmericaStakeDayLine->IsOpen());

  psetAmericaStakeDayLine->m_Date = GetFormatedMarketDate();
  psetAmericaStakeDayLine->m_Market = GetMarketString();
  psetAmericaStakeDayLine->m_Symbol = GetStakeCode();
  psetAmericaStakeDayLine->m_Ticker = GetStakeName();
  psetAmericaStakeDayLine->m_LastClose = ConvertValueToString(GetLastClose(), 1000);
  psetAmericaStakeDayLine->m_High = ConvertValueToString(GetHigh(), 1000);
  psetAmericaStakeDayLine->m_Low = ConvertValueToString(GetLow(), 1000);
  psetAmericaStakeDayLine->m_Open = ConvertValueToString(GetOpen(), 1000);
  psetAmericaStakeDayLine->m_Close = ConvertValueToString(GetClose(), 1000);
  psetAmericaStakeDayLine->m_Volume = ConvertValueToString(GetVolume());
  psetAmericaStakeDayLine->m_Amount = ConvertValueToString(GetAmount());
  psetAmericaStakeDayLine->m_UpAndDown = ConvertValueToString(GetUpDown());
  psetAmericaStakeDayLine->m_UpDownRate = ConvertValueToString(GetUpDownRate());
  psetAmericaStakeDayLine->m_ChangeHandRate = ConvertValueToString(GetChangeHandRate());
  psetAmericaStakeDayLine->m_TotalValue = ConvertValueToString(GetTotalValue());
  psetAmericaStakeDayLine->m_CurrentValue = ConvertValueToString(GetCurrentValue());
  psetAmericaStakeDayLine->m_RS = ConvertValueToString(GetRS());
  psetAmericaStakeDayLine->m_RSIndex = ConvertValueToString(GetRSIndex());
  psetAmericaStakeDayLine->m_RSBackup = ConvertValueToString(GetRSBackup());

  return true;
}

bool CDayLine::AppendAmericaMarketData(not_null<CSetAmericaStakeDayLine*> psetAmericaStakeDayLine) {
  ASSERT(psetAmericaStakeDayLine->IsOpen());
  psetAmericaStakeDayLine->AddNew();
  SaveAmericaMarketData(psetAmericaStakeDayLine);
  psetAmericaStakeDayLine->Update();

  return true;
}

bool CDayLine::LoadAmericaMarketData(not_null<CSetAmericaStakeDayLine*> psetAmericaStakeDayLine) {
  ASSERT(psetAmericaStakeDayLine->IsOpen());
  m_lDate = psetAmericaStakeDayLine->m_Date;
  m_strMarket = psetAmericaStakeDayLine->m_Market;
  m_strStakeCode = psetAmericaStakeDayLine->m_Symbol;
  m_strStakeName = psetAmericaStakeDayLine->m_Ticker;
  m_lLastClose = atof(psetAmericaStakeDayLine->m_LastClose) * 1000;
  m_lOpen = atof(psetAmericaStakeDayLine->m_Open) * 1000;
  m_lHigh = atof(psetAmericaStakeDayLine->m_High) * 1000;
  m_lLow = atof(psetAmericaStakeDayLine->m_Low) * 1000;
  m_lClose = atof(psetAmericaStakeDayLine->m_Close) * 1000;
  m_dUpDown = atof(psetAmericaStakeDayLine->m_UpAndDown);
  m_llVolume = atoll(psetAmericaStakeDayLine->m_Volume);
  m_llAmount = atoll(psetAmericaStakeDayLine->m_Amount);
  m_dUpDownRate = atof(psetAmericaStakeDayLine->m_UpDownRate);
  m_dChangeHandRate = atof(psetAmericaStakeDayLine->m_ChangeHandRate);
  m_llTotalValue = atoll(psetAmericaStakeDayLine->m_TotalValue);
  m_llCurrentValue = atoll(psetAmericaStakeDayLine->m_CurrentValue);
  m_dRS = atof(psetAmericaStakeDayLine->m_RS);
  m_dRSIndex = atof(psetAmericaStakeDayLine->m_RSIndex);
  m_dRSBackup = atof(psetAmericaStakeDayLine->m_RSBackup);
  return true;
}

bool CDayLine::SaveForexDayLine(not_null<CSetForexDayLine*> psetForexDayLine) {
  ASSERT(psetForexDayLine->IsOpen());

  psetForexDayLine->m_Date = GetFormatedMarketDate();
  psetForexDayLine->m_Exchange = GetMarketString();
  psetForexDayLine->m_Symbol = GetStakeCode();
  psetForexDayLine->m_LastClose = ConvertValueToString(GetLastClose(), 1000);
  psetForexDayLine->m_High = ConvertValueToString(GetHigh(), 1000);
  psetForexDayLine->m_Low = ConvertValueToString(GetLow(), 1000);
  psetForexDayLine->m_Open = ConvertValueToString(GetOpen(), 1000);
  psetForexDayLine->m_Close = ConvertValueToString(GetClose(), 1000);
  psetForexDayLine->m_Volume = ConvertValueToString(GetVolume());

  return true;
}

bool CDayLine::AppendForexDayLine(not_null<CSetForexDayLine*> psetForexDayLine) {
  ASSERT(psetForexDayLine->IsOpen());
  psetForexDayLine->AddNew();
  SaveForexDayLine(psetForexDayLine);
  psetForexDayLine->Update();

  return true;
}

bool CDayLine::LoadForexDayLine(not_null<CSetForexDayLine*> psetForexDayLine) {
  ASSERT(psetForexDayLine->IsOpen());
  m_lDate = psetForexDayLine->m_Date;
  m_strMarket = psetForexDayLine->m_Exchange;
  m_strStakeCode = psetForexDayLine->m_Symbol;
  m_lLastClose = atof(psetForexDayLine->m_LastClose) * 1000;
  m_lOpen = atof(psetForexDayLine->m_Open) * 1000;
  m_lHigh = atof(psetForexDayLine->m_High) * 1000;
  m_lLow = atof(psetForexDayLine->m_Low) * 1000;
  m_lClose = atof(psetForexDayLine->m_Close) * 1000;
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
  SetStakeCode(strStockCode);
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
  SetStakeName(str);

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

  // 流通市值不是用逗号结束，故而不能使用ReadOneValueFromNeteaseDayLine函数
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

bool CDayLine::ProcessNeteaseData2(CString strStockCode, vector<char>& pBuffer, INT64& lCurrentPos) {
  static char buffer2[200], buffer3[100];
  long i = 0;
  tm tm_;
  int year = 0, month = 0, day = 0;
  long lDate = 0;
  CString str;
  double dTemp = 0;

  i = 0;
  while ((pBuffer.at(lCurrentPos) != 0x02c)) { // 读取日期，直到遇到逗号
    if ((pBuffer.at(lCurrentPos) == 0x0d) || (pBuffer.at(lCurrentPos) == 0x00a) || (pBuffer.at(lCurrentPos) == 0x000) || (i > 30)) { // 如果遇到回车、换行、字符串结束符或者读取了20个字符
      return false; // 数据出错，放弃载入
    }
    buffer3[i++] = pBuffer.at(lCurrentPos++);
  }
  lCurrentPos++;
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

  if (pBuffer.at(lCurrentPos) != 0x027) return(false); // 不是单引号(')，数据出错，放弃载入
  lCurrentPos++;

  if (!ReadOneValueOfNeteaseDayLine2(pBuffer, buffer2, lCurrentPos)) return false;
  str = buffer2;
  SetStakeCode(strStockCode);
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
  if (!ReadOneValueOfNeteaseDayLine2(pBuffer, buffer2, lCurrentPos)) return false;
  str = buffer2;
  SetStakeName(str);

  if (!ReadOneValueOfNeteaseDayLine2(pBuffer, buffer2, lCurrentPos)) return false;
  dTemp = atof(buffer2);
  SetClose(dTemp * 1000);

  if (!ReadOneValueOfNeteaseDayLine2(pBuffer, buffer2, lCurrentPos)) return false;
  dTemp = atof(buffer2);
  SetHigh(dTemp * 1000);

  if (!ReadOneValueOfNeteaseDayLine2(pBuffer, buffer2, lCurrentPos)) return false;
  dTemp = atof(buffer2);
  SetLow(dTemp * 1000);

  if (!ReadOneValueOfNeteaseDayLine2(pBuffer, buffer2, lCurrentPos)) return false;
  dTemp = atof(buffer2);
  SetOpen(dTemp * 1000);

  if (!ReadOneValueOfNeteaseDayLine2(pBuffer, buffer2, lCurrentPos)) return false;
  dTemp = atof(buffer2);
  SetLastClose(dTemp * 1000);

  if (!ReadOneValueOfNeteaseDayLine2(pBuffer, buffer2, lCurrentPos)) return false;
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

  if (!ReadOneValueOfNeteaseDayLine2(pBuffer, buffer2, lCurrentPos)) return false;
  SetChangeHandRate(buffer2);

  if (!ReadOneValueOfNeteaseDayLine2(pBuffer, buffer2, lCurrentPos)) return false;
  SetVolume(buffer2); // 读入的是股数

  if (!ReadOneValueOfNeteaseDayLine2(pBuffer, buffer2, lCurrentPos)) return false;
  SetAmount(buffer2);

  // 总市值的数据有两种形式，需要程序判定
  if (!ReadOneValueOfNeteaseDayLine2(pBuffer, buffer2, lCurrentPos)) return false;
  SetTotalValue(buffer2); // 总市值的单位为：元

  // 流通市值不是用逗号结束，故而不能使用ReadOneValueFromNeteaseDayLine函数
  // 流通市值的数据形式有两种，故而需要程序判定。
  i = 0;
  while (pBuffer.at(lCurrentPos) != 0x00d) {
    if ((pBuffer.at(lCurrentPos) == 0x00a) || (pBuffer.at(lCurrentPos) == 0x000) || (i > 30)) return false; // 数据出错，放弃载入
    buffer2[i++] = pBuffer.at(lCurrentPos++);
  }
  lCurrentPos++;
  buffer2[i] = 0x000;
  SetCurrentValue(buffer2); // 流通市值的单位为：元。
  // \r后面紧跟着应该是\n
  if (pBuffer.at(lCurrentPos++) != 0x0a) return false; // 数据出错，放弃载入

  return true;
}

bool CDayLine::IsActive(void) {
  if ((GetClose() != 0) && (GetLastClose() != 0)) {
    return true;
  }
  else return false;
}