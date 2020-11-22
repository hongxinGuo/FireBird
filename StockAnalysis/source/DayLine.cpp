#include "stdafx.h"

#include"globedef.h"
#include "DayLine.h"

CDayLine::CDayLine() : CChinaStockHistoryData() {
  Reset();
}

void CDayLine::Reset(void) {
  CChinaStockHistoryData::Reset();
}

bool CDayLine::LoadExtendData(not_null<CSetDayLineExtendInfo*> psetDayLineExtendInfo) {
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

bool CDayLine::SaveData(not_null<CSetDayLineBasicInfo*> psetDayLineBasicInfo) {
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

bool CDayLine::AppendData(not_null<CSetDayLineBasicInfo*> psetDayLineBasicInfo) {
  ASSERT(psetDayLineBasicInfo->IsOpen());
  psetDayLineBasicInfo->AddNew();
  SaveData(psetDayLineBasicInfo);
  psetDayLineBasicInfo->Update();

  return true;
}

bool CDayLine::LoadBasicData(not_null<CSetDayLineBasicInfo*> psetDayLineBasicInfo) {
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ���������ݡ���������������ʷ���ݸ�ʽ��
//
// ��ʵʱ���������ƣ����ּ۸�ԷŴ�һǧ�����Գ����ʹ洢���������ݿ�ʱ��DECIMAL(10,3)���ʹ洢��
// �ַ�������ʽΪ��2019-07-10,600000,�ֶ�����,���̼�,��߼�,��ͼ�,���̼�,ǰ���̼�,�ǵ�ֵ,�ǵ�����,������,�ɽ�����,�ɽ����,����ֵ,��ͨ��ֵ\r\n
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
  while ((*pCurrentPos != 0x02c)) { // ��ȡ���ڣ�ֱ����������
    if ((*pCurrentPos == 0x0d) || (*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000) || (i > 30)) { // ��������س������С��ַ������������߶�ȡ��20���ַ�
      return false; // ���ݳ�����������
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

  if (*pCurrentPos != 0x027) return(false); // ���ǵ�����(')�����ݳ�����������
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

  if (GetLastClose() == 0) { // �����ǵ�����
    SetUpDownRate(0.0); // ����������̼�Ϊ�㣨û���ף������ǵ���Ҳ��Ϊ�㡣
  }
  else {
    // ��Ҫ�Ŵ�1000 * 100�������̼۱�ʵ��ֵ��1000������¼���ǰٷֱȣ�ҲҪ����100����
    SetUpDownRate(((double)(GetUpDown() * 100000.0)) / GetLastClose());
  }

  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  SetChangeHandRate(buffer2);

  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  SetVolume(buffer2); // ������ǹ���

  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  SetAmount(buffer2);

  // ����ֵ��������������ʽ����Ҫ�����ж�
  if (!ReadOneValueOfNeteaseDayLine(pCurrentPos, buffer2, iCount)) return false;
  SetTotalValue(buffer2); // ����ֵ�ĵ�λΪ��Ԫ

  // ��ͨ��ֵ�����ö��Ž������ʶ�����ʹ��ReadOneValueFromNeteaseDayLine����
  // ��ͨ��ֵ��������ʽ�����֣��ʶ���Ҫ�����ж���
  i = 0;
  while (*pCurrentPos != 0x00d) {
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000) || (i > 30)) return false; // ���ݳ�����������
    buffer2[i++] = *pCurrentPos++;
    iCount++;
  }
  pCurrentPos++;
  iCount++;
  buffer2[i] = 0x000;
  SetCurrentValue(buffer2); // ��ͨ��ֵ�ĵ�λΪ��Ԫ��
  // \r���������Ӧ����\n
  if (*pCurrentPos++ != 0x0a) return false; // ���ݳ�����������
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
  while ((pBuffer.at(lCurrentPos) != 0x02c)) { // ��ȡ���ڣ�ֱ����������
    if ((pBuffer.at(lCurrentPos) == 0x0d) || (pBuffer.at(lCurrentPos) == 0x00a) || (pBuffer.at(lCurrentPos) == 0x000) || (i > 30)) { // ��������س������С��ַ������������߶�ȡ��20���ַ�
      return false; // ���ݳ�����������
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

  if (pBuffer.at(lCurrentPos) != 0x027) return(false); // ���ǵ�����(')�����ݳ�����������
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

  if (GetLastClose() == 0) { // �����ǵ�����
    SetUpDownRate(0.0); // ����������̼�Ϊ�㣨û���ף������ǵ���Ҳ��Ϊ�㡣
  }
  else {
    // ��Ҫ�Ŵ�1000 * 100�������̼۱�ʵ��ֵ��1000������¼���ǰٷֱȣ�ҲҪ����100����
    SetUpDownRate(((double)(GetUpDown() * 100000.0)) / GetLastClose());
  }

  if (!ReadOneValueOfNeteaseDayLine2(pBuffer, buffer2, lCurrentPos)) return false;
  SetChangeHandRate(buffer2);

  if (!ReadOneValueOfNeteaseDayLine2(pBuffer, buffer2, lCurrentPos)) return false;
  SetVolume(buffer2); // ������ǹ���

  if (!ReadOneValueOfNeteaseDayLine2(pBuffer, buffer2, lCurrentPos)) return false;
  SetAmount(buffer2);

  // ����ֵ��������������ʽ����Ҫ�����ж�
  if (!ReadOneValueOfNeteaseDayLine2(pBuffer, buffer2, lCurrentPos)) return false;
  SetTotalValue(buffer2); // ����ֵ�ĵ�λΪ��Ԫ

  // ��ͨ��ֵ�����ö��Ž������ʶ�����ʹ��ReadOneValueFromNeteaseDayLine����
  // ��ͨ��ֵ��������ʽ�����֣��ʶ���Ҫ�����ж���
  i = 0;
  while (pBuffer.at(lCurrentPos) != 0x00d) {
    if ((pBuffer.at(lCurrentPos) == 0x00a) || (pBuffer.at(lCurrentPos) == 0x000) || (i > 30)) return false; // ���ݳ�����������
    buffer2[i++] = pBuffer.at(lCurrentPos++);
  }
  lCurrentPos++;
  buffer2[i] = 0x000;
  SetCurrentValue(buffer2); // ��ͨ��ֵ�ĵ�λΪ��Ԫ��
  // \r���������Ӧ����\n
  if (pBuffer.at(lCurrentPos++) != 0x0a) return false; // ���ݳ�����������

  return true;
}

bool CDayLine::IsActive(void) {
  if ((GetClose() != 0) && (GetLastClose() != 0)) {
    return true;
  }
  else return false;
}