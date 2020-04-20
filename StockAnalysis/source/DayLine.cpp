#include "stdafx.h"

#include"globedef.h"
#include "DayLine.h"

CDayLine::CDayLine() : CObject() {
  Reset();
}

bool CDayLine::LoadData(CSetDayLineExtendInfo& setDayLineExtendInfo) {
  ASSERT(setDayLineExtendInfo.IsOpen());
  m_lTransactionNumber = atol(setDayLineExtendInfo.m_TransactionNumber);
  m_lTransactionNumberBelow5000 = atol(setDayLineExtendInfo.m_TransactionNumberBelow5000);
  m_lTransactionNumberBelow50000 = atol(setDayLineExtendInfo.m_TransactionNumberBelow50000);
  m_lTransactionNumberBelow200000 = atol(setDayLineExtendInfo.m_TransactionNumberBelow200000);
  m_lTransactionNumberAbove200000 = atol(setDayLineExtendInfo.m_TransactionNumberAbove200000);
  m_lAttackBuyVolume = atol(setDayLineExtendInfo.m_AttackBuyVolume);
  m_lStrongBuyVolume = atol(setDayLineExtendInfo.m_StrongBuyVolume);
  m_lAttackSellVolume = atol(setDayLineExtendInfo.m_AttackSellVolume);
  m_lStrongSellVolume = atol(setDayLineExtendInfo.m_StrongSellVolume);
  m_lUnknownVolume = atol(setDayLineExtendInfo.m_UnknownVolume);
  m_lCancelBuyVolume = atol(setDayLineExtendInfo.m_CancelBuyVolume);
  m_lCancelSellVolume = atol(setDayLineExtendInfo.m_CancelSellVolume);
  m_lOrdinaryBuyVolume = atol(setDayLineExtendInfo.m_OrdinaryBuyVolume);
  m_lAttackBuyBelow50000 = atol(setDayLineExtendInfo.m_AttackBuyBelow50000);
  m_lAttackBuyBelow200000 = atol(setDayLineExtendInfo.m_AttackBuyBelow200000);
  m_lAttackBuyAbove200000 = atol(setDayLineExtendInfo.m_AttackBuyAbove200000);
  m_lOrdinarySellVolume = atol(setDayLineExtendInfo.m_OrdinarySellVolume);
  m_lAttackSellBelow50000 = atol(setDayLineExtendInfo.m_AttackSellBelow50000);
  m_lAttackSellBelow200000 = atol(setDayLineExtendInfo.m_AttackSellBelow200000);
  m_lAttackSellAbove200000 = atol(setDayLineExtendInfo.m_AttackSellAbove200000);

  m_lOrdinaryBuyVolumeBelow5000 = atoll(setDayLineExtendInfo.m_OrdinaryBuyVolumeBelow5000);
  m_lOrdinaryBuyVolumeBelow10000 = atoll(setDayLineExtendInfo.m_OrdinaryBuyVolumeBelow10000);
  m_lOrdinaryBuyVolumeBelow20000 = atoll(setDayLineExtendInfo.m_OrdinaryBuyVolumeBelow20000);
  m_lOrdinaryBuyVolumeBelow50000 = atoll(setDayLineExtendInfo.m_OrdinaryBuyVolumeBelow50000);
  m_lOrdinaryBuyVolumeBelow100000 = atoll(setDayLineExtendInfo.m_OrdinaryBuyVolumeBelow100000);
  m_lOrdinaryBuyVolumeBelow200000 = atoll(setDayLineExtendInfo.m_OrdinaryBuyVolumeBelow200000);
  m_lOrdinaryBuyVolumeAbove200000 = atoll(setDayLineExtendInfo.m_OrdinaryBuyVolumeAbove200000);
  m_lOrdinarySellVolumeBelow5000 = atoll(setDayLineExtendInfo.m_OrdinarySellVolumeBelow5000);
  m_lOrdinarySellVolumeBelow10000 = atoll(setDayLineExtendInfo.m_OrdinarySellVolumeBelow10000);
  m_lOrdinarySellVolumeBelow20000 = atoll(setDayLineExtendInfo.m_OrdinarySellVolumeBelow20000);
  m_lOrdinarySellVolumeBelow50000 = atoll(setDayLineExtendInfo.m_OrdinarySellVolumeBelow50000);
  m_lOrdinarySellVolumeBelow100000 = atoll(setDayLineExtendInfo.m_OrdinarySellVolumeBelow100000);
  m_lOrdinarySellVolumeBelow200000 = atoll(setDayLineExtendInfo.m_OrdinarySellVolumeBelow200000);
  m_lOrdinarySellVolumeAbove200000 = atoll(setDayLineExtendInfo.m_OrdinarySellVolumeAbove200000);
  m_lOrdinaryBuyNumberBelow5000 = atoll(setDayLineExtendInfo.m_OrdinaryBuyNumberBelow5000);
  m_lOrdinaryBuyNumberBelow10000 = atoll(setDayLineExtendInfo.m_OrdinaryBuyNumberBelow10000);
  m_lOrdinaryBuyNumberBelow20000 = atoll(setDayLineExtendInfo.m_OrdinaryBuyNumberBelow20000);
  m_lOrdinaryBuyNumberBelow50000 = atoll(setDayLineExtendInfo.m_OrdinaryBuyNumberBelow50000);
  m_lOrdinaryBuyNumberBelow100000 = atoll(setDayLineExtendInfo.m_OrdinaryBuyNumberBelow100000);
  m_lOrdinaryBuyNumberBelow200000 = atoll(setDayLineExtendInfo.m_OrdinaryBuyNumberBelow200000);
  m_lOrdinaryBuyNumberAbove200000 = atoll(setDayLineExtendInfo.m_OrdinaryBuyNumberAbove200000);
  m_lOrdinarySellNumberBelow5000 = atoll(setDayLineExtendInfo.m_OrdinarySellNumberBelow5000);
  m_lOrdinarySellNumberBelow10000 = atoll(setDayLineExtendInfo.m_OrdinarySellNumberBelow10000);
  m_lOrdinarySellNumberBelow20000 = atoll(setDayLineExtendInfo.m_OrdinarySellNumberBelow20000);
  m_lOrdinarySellNumberBelow50000 = atoll(setDayLineExtendInfo.m_OrdinarySellNumberBelow50000);
  m_lOrdinarySellNumberBelow100000 = atoll(setDayLineExtendInfo.m_OrdinarySellNumberBelow100000);
  m_lOrdinarySellNumberBelow200000 = atoll(setDayLineExtendInfo.m_OrdinarySellNumberBelow200000);
  m_lOrdinarySellNumberAbove200000 = atoll(setDayLineExtendInfo.m_OrdinarySellNumberAbove200000);
  return true;
}

void CDayLine::CalculateRSLogarithm(double dRS) {
  double dLog50 = log10(50.0);
  double dLog100 = log10(100.0);
  double dLog = dLog100 - dLog50;
  if ((dRS - 50) > 0) {
    m_dRSLogarithm = 50 + (log10(dRS) - dLog50) * 50 / dLog;
  }
  else if ((dRS - 50) < 0) {
    m_dRSLogarithm = 50 - (log10(100 - dRS) - dLog50) * 50 / dLog;
  }
  else m_dRSLogarithm = 50;
}

void CDayLine::Reset(void) {
  m_lDay = 0;		// 类型
  m_time = 0;
  m_wMarket = 0;
  m_strStockCode = _T("");
  m_strStockName = _T("");
  m_lLastClose = m_lOpen = m_lHigh = m_lLow = m_lClose = 0;							// 收盘价
  m_llVolume = 0;
  m_llAmount = 0;
  m_dUpDown = 0;
  m_dUpDownRate = 0;
  m_llCurrentValue = m_llTotalValue = 0;
  m_dChangeHandRate = 0;

  m_lTransactionNumber = 0; //
  m_lTransactionNumberBelow5000 = 0; //
  m_lTransactionNumberBelow50000 = 0; //
  m_lTransactionNumberBelow200000 = 0; //
  m_lTransactionNumberAbove200000 = 0; //

  m_lAttackBuyVolume = m_lStrongBuyVolume = m_lAttackSellVolume = m_lStrongSellVolume = 0;
  m_lUnknownVolume = m_lCancelBuyVolume = m_lCancelSellVolume = 0;
  m_dRelativeStrong = m_dRSLogarithm = m_dRelativeStrongIndex = m_dRelativeStrongBackup = 0.0;
  m_lOrdinaryBuyVolume = m_lAttackBuyBelow50000 = m_lAttackBuyBelow200000 = m_lAttackBuyAbove200000 = 0;
  m_lOrdinarySellVolume = m_lAttackSellBelow50000 = m_lAttackSellBelow200000 = m_lAttackSellAbove200000 = 0;
  m_d3DayRS = m_d5DayRS = m_d10DayRS = m_d30DayRS = m_d60DayRS = m_d120DayRS = 0.0;
}

bool CDayLine::SaveData(CSetDayLineBasicInfo& setDayLineBasicInfo) {
  ASSERT(setDayLineBasicInfo.IsOpen());

  setDayLineBasicInfo.m_Day = GetDay();
  setDayLineBasicInfo.m_Market = GetMarket();
  setDayLineBasicInfo.m_StockCode = GetStockCode();
  setDayLineBasicInfo.m_StockName = GetStockName();
  setDayLineBasicInfo.m_LastClose = ConvertValueToString(GetLastClose(), 1000);
  setDayLineBasicInfo.m_High = ConvertValueToString(GetHigh(), 1000);
  setDayLineBasicInfo.m_Low = ConvertValueToString(GetLow(), 1000);
  setDayLineBasicInfo.m_Open = ConvertValueToString(GetOpen(), 1000);
  setDayLineBasicInfo.m_Close = ConvertValueToString(GetClose(), 1000);
  setDayLineBasicInfo.m_Volume = ConvertValueToString(GetVolume());
  setDayLineBasicInfo.m_Amount = ConvertValueToString(GetAmount());
  setDayLineBasicInfo.m_UpAndDown = ConvertValueToString(GetUpDown());
  setDayLineBasicInfo.m_UpDownRate = ConvertValueToString(GetUpDownRate());
  setDayLineBasicInfo.m_ChangeHandRate = ConvertValueToString(GetChangeHandRate());
  setDayLineBasicInfo.m_TotalValue = ConvertValueToString(GetTotalValue());
  setDayLineBasicInfo.m_CurrentValue = ConvertValueToString(GetCurrentValue());
  setDayLineBasicInfo.m_RelativeStrong = ConvertValueToString(GetRelativeStrong());
  setDayLineBasicInfo.m_RelativeStrongIndex = ConvertValueToString(GetRelativeStrongIndex());
  setDayLineBasicInfo.m_RelativeStrongBackup = ConvertValueToString(GetRelativeStrongBackup());

  return true;
}

bool CDayLine::AppendData(CSetDayLineBasicInfo& setDayLineBasicInfo) {
  ASSERT(setDayLineBasicInfo.IsOpen());
  setDayLineBasicInfo.AddNew();
  SaveData(setDayLineBasicInfo);
  setDayLineBasicInfo.Update();

  return true;
}

bool CDayLine::LoadData(CSetDayLineBasicInfo& setDayLineBasicInfo) {
  ASSERT(setDayLineBasicInfo.IsOpen());
  m_lDay = setDayLineBasicInfo.m_Day;
  m_wMarket = setDayLineBasicInfo.m_Market;
  m_strStockCode = setDayLineBasicInfo.m_StockCode;
  m_lLastClose = atof(setDayLineBasicInfo.m_LastClose) * 1000;
  m_lOpen = atof(setDayLineBasicInfo.m_Open) * 1000;
  m_lHigh = atof(setDayLineBasicInfo.m_High) * 1000;
  m_lLow = atof(setDayLineBasicInfo.m_Low) * 1000;
  m_lClose = atof(setDayLineBasicInfo.m_Close) * 1000;
  m_dUpDown = atof(setDayLineBasicInfo.m_UpAndDown);
  m_llVolume = atoll(setDayLineBasicInfo.m_Volume);
  m_llAmount = atoll(setDayLineBasicInfo.m_Amount);
  m_dUpDownRate = atof(setDayLineBasicInfo.m_UpDownRate);
  m_dChangeHandRate = atof(setDayLineBasicInfo.m_ChangeHandRate);
  m_llTotalValue = atoll(setDayLineBasicInfo.m_TotalValue);
  m_llCurrentValue = atoll(setDayLineBasicInfo.m_CurrentValue);
  m_dRelativeStrong = atof(setDayLineBasicInfo.m_RelativeStrong);
  m_dRelativeStrongIndex = atof(setDayLineBasicInfo.m_RelativeStrongIndex);
  m_dRelativeStrongBackup = atof(setDayLineBasicInfo.m_RelativeStrongBackup);
  CalculateRSLogarithm(m_dRelativeStrong);
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
  long lDay = 0;
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
  sscanf_s(buffer3, "%04d-%02d-%02d", &year, &month, &day);
  tm_.tm_year = year - 1900;
  tm_.tm_mon = month - 1;
  tm_.tm_mday = day;
  tm_.tm_hour = 15;
  tm_.tm_min = 0;
  tm_.tm_sec = 0;
  tm_.tm_isdst = 0;
  SetTime(mktime(&tm_));
  lDay = year * 10000 + month * 100 + day;
  SetDay(lDay);
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
    //ASSERT(strcmp(buffer2, "None") == 0);
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

bool CDayLine::IsActive(void) {
  if ((GetClose() != 0) && (GetLastClose() != 0)) {
    return true;
  }
  else return false;
}