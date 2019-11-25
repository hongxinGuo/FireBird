#include "stdafx.h"

#include "DayLine.h"

CDayLine::CDayLine() : CObject() {
  Reset();
}

CDayLine::CDayLine(CDayLine& oneDl) {
  m_time = oneDl.m_time;
  m_lDay = oneDl.m_lDay;
  m_wMarket = oneDl.m_wMarket;
  m_lLastClose = oneDl.m_lLastClose;
  m_lOpen = oneDl.m_lOpen;
  m_lHigh = oneDl.m_lHigh;
  m_lLow = oneDl.m_lLow;
  m_lClose = oneDl.m_lClose;
  m_llVolume = oneDl.m_llVolume;
  m_llAmount = oneDl.m_llAmount;
  m_dUpDown = oneDl.m_dUpDown;
  m_dUpDownRate = oneDl.m_dUpDownRate;
  m_dChangeHandRate = oneDl.m_dChangeHandRate;
  m_llTotalValue = oneDl.m_llTotalValue;
  m_llCurrentValue = oneDl.m_llCurrentValue;
  m_lTransactionNumber = oneDl.m_lTransactionNumber;
  m_lTransactionNumberBelow5000 = oneDl.m_lTransactionNumberBelow5000;
  m_lTransactionNumberBelow50000 = oneDl.m_lTransactionNumberBelow50000;
  m_lTransactionNumberBelow200000 = oneDl.m_lTransactionNumberBelow200000;
  m_lTransactionNumberAbove200000 = oneDl.m_lTransactionNumberAbove200000;
  m_lAttackBuyVolume = oneDl.m_lAttackBuyVolume;
  m_lStrongBuyVolume = oneDl.m_lStrongBuyVolume;
  m_lAttackSellVolume = oneDl.m_lAttackSellVolume;
  m_lStrongSellVolume = oneDl.m_lStrongSellVolume;
  m_lUnknownVolume = oneDl.m_lUnknownVolume;
  m_lCancelBuyVolume = oneDl.m_lCancelBuyVolume;
  m_lCancelSellVolume = oneDl.m_lCancelSellVolume;
  m_dRelativeStrong = oneDl.m_dRelativeStrong;
  m_lOrdinaryBuyVolume = oneDl.m_lOrdinaryBuyVolume;
  m_lAttackBuyBelow50000 = oneDl.m_lAttackBuyBelow50000;
  m_lAttackBuyBelow200000 = oneDl.m_lAttackBuyBelow200000;
  m_lAttackBuyAbove200000 = oneDl.m_lAttackBuyAbove200000;
  m_lOrdinarySellVolume = oneDl.m_lOrdinarySellVolume;
  m_lAttackSellBelow50000 = oneDl.m_lAttackSellBelow50000;
  m_lAttackSellBelow200000 = oneDl.m_lAttackSellBelow200000;
  m_lAttackSellAbove200000 = oneDl.m_lAttackSellAbove200000;
  m_d3DayRS = oneDl.m_d3DayRS;
  m_d5DayRS = oneDl.m_d5DayRS;
  m_d10DayRS = oneDl.m_d10DayRS;
  m_d30DayRS = oneDl.m_d30DayRS;
  m_d60DayRS = oneDl.m_d60DayRS;
  m_d120DayRS = oneDl.m_d120DayRS;
}

void CDayLine::operator =(CDayLine& oneDl) {
  m_lDay = oneDl.m_lDay;
  m_lLastClose = oneDl.m_lLastClose;
  m_lOpen = oneDl.m_lOpen;
  m_lHigh = oneDl.m_lHigh;
  m_lLow = oneDl.m_lLow;
  m_lClose = oneDl.m_lClose;
  m_llVolume = oneDl.m_llVolume;
  m_llAmount = oneDl.m_llAmount;
  m_dUpDown = oneDl.m_dUpDown;
  m_dUpDownRate = oneDl.m_dUpDownRate;
  m_dChangeHandRate = oneDl.m_dChangeHandRate;
  m_llTotalValue = oneDl.m_llTotalValue;
  m_llCurrentValue = oneDl.m_llCurrentValue;
  m_lTransactionNumber = oneDl.m_lTransactionNumber;
  m_lTransactionNumberBelow5000 = oneDl.m_lTransactionNumberBelow5000;
  m_lTransactionNumberBelow50000 = oneDl.m_lTransactionNumberBelow50000;
  m_lTransactionNumberBelow200000 = oneDl.m_lTransactionNumberBelow200000;
  m_lTransactionNumberAbove200000 = oneDl.m_lTransactionNumberAbove200000;
  m_lAttackBuyVolume = oneDl.m_lAttackBuyVolume;
  m_lStrongBuyVolume = oneDl.m_lStrongBuyVolume;
  m_lAttackSellVolume = oneDl.m_lAttackSellVolume;
  m_lStrongSellVolume = oneDl.m_lStrongSellVolume;
  m_lUnknownVolume = oneDl.m_lUnknownVolume;
  m_lCancelBuyVolume = oneDl.m_lCancelBuyVolume;
  m_lCancelSellVolume = oneDl.m_lCancelSellVolume;
  m_dRelativeStrong = oneDl.m_dRelativeStrong;
  m_lOrdinaryBuyVolume = oneDl.m_lOrdinaryBuyVolume;
  m_lAttackBuyBelow50000 = oneDl.m_lAttackBuyBelow50000;
  m_lAttackBuyBelow200000 = oneDl.m_lAttackBuyBelow200000;
  m_lAttackBuyAbove200000 = oneDl.m_lAttackBuyAbove200000;
  m_lOrdinarySellVolume = oneDl.m_lOrdinarySellVolume;
  m_lAttackSellBelow50000 = oneDl.m_lAttackSellBelow50000;
  m_lAttackSellBelow200000 = oneDl.m_lAttackSellBelow200000;
  m_lAttackSellAbove200000 = oneDl.m_lAttackSellAbove200000;
  m_d3DayRS = oneDl.m_d3DayRS;
  m_d5DayRS = oneDl.m_d5DayRS;
  m_d10DayRS = oneDl.m_d10DayRS;
  m_d30DayRS = oneDl.m_d30DayRS;
  m_d60DayRS = oneDl.m_d60DayRS;
  m_d120DayRS = oneDl.m_d120DayRS;
}

bool CDayLine::LoadData(CSetDayLineInfo& setDayLineInfo) {
  m_lTransactionNumber = atol(setDayLineInfo.m_TransactionNumber);
  m_lTransactionNumberBelow5000 = atol(setDayLineInfo.m_TransactionNumberBelow5000);
  m_lTransactionNumberBelow50000 = atol(setDayLineInfo.m_TransactionNumberBelow50000);
  m_lTransactionNumberBelow200000 = atol(setDayLineInfo.m_TransactionNumberBelow200000);
  m_lTransactionNumberAbove200000 = atol(setDayLineInfo.m_TransactionNumberAbove200000);
  m_lAttackBuyVolume = atol(setDayLineInfo.m_AttackBuyVolume);
  m_lStrongBuyVolume = atol(setDayLineInfo.m_StrongBuyVolume);
  m_lAttackSellVolume = atol(setDayLineInfo.m_AttackSellVolume);
  m_lStrongSellVolume = atol(setDayLineInfo.m_StrongSellVolume);
  m_lUnknownVolume = atol(setDayLineInfo.m_UnknownVolume);
  m_lCancelBuyVolume = atol(setDayLineInfo.m_CancelBuyVolume);
  m_lCancelSellVolume = atol(setDayLineInfo.m_CancelSellVolume);
  m_lOrdinaryBuyVolume = atol(setDayLineInfo.m_OrdinaryBuyVolume);
  m_lAttackBuyBelow50000 = atol(setDayLineInfo.m_AttackBuyBelow50000);
  m_lAttackBuyBelow200000 = atol(setDayLineInfo.m_AttackBuyBelow200000);
  m_lAttackBuyAbove200000 = atol(setDayLineInfo.m_AttackBuyAbove200000);
  m_lOrdinarySellVolume = atol(setDayLineInfo.m_OrdinarySellVolume);
  m_lAttackSellBelow50000 = atol(setDayLineInfo.m_AttackSellBelow50000);
  m_lAttackSellBelow200000 = atol(setDayLineInfo.m_AttackSellBelow200000);
  m_lAttackSellAbove200000 = atol(setDayLineInfo.m_AttackSellAbove200000);

  return true;
}

void CDayLine::Reset(void) {
  m_lDay = 0;		// 类型
  m_time = 0;
  m_wMarket = 0;
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
  m_lUnknownVolume = m_lCancelBuyVolume = m_lCancelSellVolume = m_dRelativeStrong = 0;
  m_lOrdinaryBuyVolume = m_lAttackBuyBelow50000 = m_lAttackBuyBelow200000 = m_lAttackBuyAbove200000 = 0;
  m_lOrdinarySellVolume = m_lAttackSellBelow50000 = m_lAttackSellBelow200000 = m_lAttackSellAbove200000 = 0;
  m_d3DayRS = m_d5DayRS = m_d10DayRS = m_d30DayRS = m_d60DayRS = m_d120DayRS = 0;
}

bool CDayLine::SaveData(CSetDayLine& setDayLine) {
  ASSERT(setDayLine.IsOpen());

  setDayLine.m_Day = GetDay();
  setDayLine.m_Market = GetMarket();
  setDayLine.m_StockCode = GetStockCode();
  setDayLine.m_StockName = GetStockName();
  setDayLine.m_LastClose = ConvertValueToString(GetLastClose(), 1000);
  setDayLine.m_High = ConvertValueToString(GetHigh(), 1000);
  setDayLine.m_Low = ConvertValueToString(GetLow(), 1000);
  setDayLine.m_Open = ConvertValueToString(GetOpen(), 1000);
  setDayLine.m_Close = ConvertValueToString(GetClose(), 1000);
  setDayLine.m_Volume = ConvertValueToString(GetVolume());
  setDayLine.m_Amount = ConvertValueToString(GetAmount());
  setDayLine.m_UpAndDown = ConvertValueToString(GetUpDown());
  setDayLine.m_UpDownRate = ConvertValueToString(GetUpDownRate());
  setDayLine.m_ChangeHandRate = ConvertValueToString(GetChangeHandRate());
  setDayLine.m_TotalValue = ConvertValueToString(GetTotalValue());
  setDayLine.m_CurrentValue = ConvertValueToString(GetCurrentValue());
  setDayLine.m_RelativeStrong = ConvertValueToString(GetRelativeStrong());

  return true;
}

bool CDayLine::AppendData(CSetDayLine& setDayLine) {
  ASSERT(setDayLine.IsOpen());
  setDayLine.AddNew();
  SaveData(setDayLine);
  setDayLine.Update();

  return true;
}

bool CDayLine::LoadData(CSetDayLine& setDayLine) {
  m_lDay = setDayLine.m_Day;
  m_wMarket = setDayLine.m_Market;
  m_strStockCode = setDayLine.m_StockCode;
  m_lLastClose = atof(setDayLine.m_LastClose) * 1000;
  m_lOpen = atof(setDayLine.m_Open) * 1000;
  m_lHigh = atof(setDayLine.m_High) * 1000;
  m_lLow = atof(setDayLine.m_Low) * 1000;
  m_lClose = atof(setDayLine.m_Close) * 1000;
  m_dUpDown = atof(setDayLine.m_UpAndDown);
  m_llVolume = atoll(setDayLine.m_Volume);
  m_llAmount = atoll(setDayLine.m_Amount);
  m_dUpDownRate = atof(setDayLine.m_UpDownRate);
  m_dChangeHandRate = atof(setDayLine.m_ChangeHandRate);
  m_llTotalValue = atoll(setDayLine.m_TotalValue);
  m_llCurrentValue = atoll(setDayLine.m_CurrentValue);
  m_dRelativeStrong = atof(setDayLine.m_RelativeStrong);
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读取一条日线数据。采用网易日线历史数据格式。
//
// 与实时数据相类似，各种价格皆放大一千倍后以长整型存储。存入数据库时以DECIMAL(10,3)类型存储。
//
// 字符串的制式为：2019-07-10,600000,浦东银行,收盘价,最高价,最低价,开盘价,前收盘价,涨跌值,涨跌比率,换手率,成交股数,成交金额,总市值,流通市值\r\n
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDayLine::ProcessDayLineData(CString strStockCode, char*& pCurrentPos, long& lLength) {
  long iCount = 0;
  static char buffer2[200], buffer3[100];
  long i = 0;
  tm tm_;
  int year = 0, month = 0, day = 0;
  long lDay = 0;
  CString str;
  i = 0;
  while (*pCurrentPos != 0x2c) {
    if ((*pCurrentPos == 0x0d) || (*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
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

  if (*pCurrentPos != 0x27) return(false); // 不是逗号，数据出错，放弃载入
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
    ASSERT(0); // 股票代码制式出错
    return false;
  }
  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  str = buffer2;
  SetStockName(str);

  if (!ReadOneValueExceptPeriodOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  SetClose(buffer2);

  if (!ReadOneValueExceptPeriodOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  SetHigh(buffer2);

  if (!ReadOneValueExceptPeriodOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  SetLow(buffer2);

  if (!ReadOneValueExceptPeriodOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  SetOpen(buffer2);

  if (!ReadOneValueExceptPeriodOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  SetLastClose(buffer2);

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
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) return false; // 数据出错，放弃载入
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

  lLength = iCount;

  return true;
}