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
  // �����ʼ��m_strStockCode�ĳ���Ϊ6.
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
// ����:
//		data : ��һ��ʵʱ���ݵ�λ.
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
// ����:
//		data : ��һ��ʵʱ���ݵ�λ.
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
// �������ļ�file�ж�ȡʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
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

  m_fActive = false;    // ��ʼ״̬Ϊ��Ч����
  strncpy_s(buffer1, pCurrentPos, 12); // ���롰var hq_str_s"
  buffer1[12] = 0x000;
  CString str1;
  str1 = buffer1;
  if (strHeader.Compare(str1) != 0) { // ���ݸ�ʽ����
    return false;
  }
  pCurrentPos += 12;
  iTotalRead += 12;

  if (*pCurrentPos == 'h') { // �Ϻ���Ʊ
    m_wMarket = __SHANGHAI_MARKET__; // �Ϻ���Ʊ��ʶ
  }
  else if (*pCurrentPos == 'z') {
    m_wMarket = __SHENZHEN_MARKET__; // ���ڹ�Ʊ��ʶ
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
    m_strStockCode = _T("sh") + m_strStockCode; // �����Ϻ����ڹ�Ʊ�������ص����ʶ����еĹ�Ʊ���붼�����г�ǰ׺���Ϻ�Ϊsh
    break;
  case __SHENZHEN_MARKET__:
    m_strStockCode = _T("sz") + m_strStockCode;// �����Ϻ����ڹ�Ʊ�������ص����ʶ����еĹ�Ʊ���붼�����г�ǰ׺������Ϊsz
    break;
  default:
    return false;
  }
  m_iStockCode = atoi(buffer2);
  pCurrentPos += 6;
  iTotalRead += 6;

  strncpy_s(buffer1, pCurrentPos, 2); // ����'="'
  if (buffer1[0] != '=') {
    return false;
  }
  if (buffer1[1] != '"') {
    return false;
  }
  pCurrentPos += 2;
  iTotalRead += 2;
  strncpy_s(buffer1, pCurrentPos, 2);
  if (buffer1[0] == '"') { // û������
    if (buffer1[1] != ';') {
      return false;
    }
    iTotalRead += 2;
    pCurrentPos += 2;
    if (*pCurrentPos++ != 0x00a) {
      return false; // ȷ�����ַ� \n
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
  while (*pCurrentPos != 0x02c) { // ����ʣ�µ��������֣���һ������buffer1�У�
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
      return false;
    }
    buffer1[i++] = *pCurrentPos++;
    iTotalRead++;
  }
  buffer1[i] = 0x000;
  m_strStockName = buffer1; // ���ù�Ʊ����

  pCurrentPos++;
  iTotalRead++;

  // ���뿪�̼ۡ��Ŵ�һǧ����洢Ϊ�����͡������۸�����ˡ�
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lOpen = lTemp;
  // ����ǰ���̼�
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lLastClose = lTemp;
  // ���뵱ǰ��
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
  // ������ͼ�
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lLow = lTemp;
  // ���뾺���
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lBuy = lTemp;
  // ���뾺����
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lSell = lTemp;

  // ����ɽ��������ɽ������洢ʵ��ֵ
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  llTemp = atoll(buffer3); // ������ǹ������洢Ҳʹ�ùɣ��ʶ���Ҫ���˱�����ΪINT64��
  if (llTemp < 0) return false;
  if (llTemp > 0) m_lVolume = llTemp;
  // ����ɽ����
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  llTemp = atoll(buffer3);
  if (llTemp < 0) return false;
  if (llTemp > 0) m_lAmount = llTemp;
  // ������һ����
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVBuy.at(0) = lTemp;
  // ������һ�۸�
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPBuy.at(0) = lTemp;
  // �����������
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVBuy.at(1) = lTemp;
  // ��������۸�
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPBuy.at(1) = lTemp;
  // ������������
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVBuy.at(2) = lTemp;
  // ���������۸�
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPBuy.at(2) = lTemp;
  // ������������
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVBuy.at(3) = lTemp;
  // �������ļ۸�
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPBuy.at(3) = lTemp;
  // ������������
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVBuy.at(4) = lTemp;
  // ��������۸�
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPBuy.at(4) = lTemp;
  // ������һ����
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVSell.at(0) = lTemp;
  // ������һ�۸�
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPSell.at(0) = lTemp;
  // ������������
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVSell.at(1) = lTemp;
  // ���������۸�
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPSell.at(1) = lTemp;
  // ������������
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVSell.at(2) = lTemp;
  // ���������۸�
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPSell.at(2) = lTemp;
  // ������������
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVSell.at(3) = lTemp;
  // �������ļ۸�
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPSell.at(3) = lTemp;
  // ������������
  if (!ReadOneValue(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lVSell.at(4) = lTemp;
  // ��������۸�
  if (!ReadOneValueExceptperiod(pCurrentPos, buffer3, iTotalRead)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) m_lPSell.at(4) = lTemp;
  // ����ɽ����ں�ʱ��
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
// ����һ��ֵ���������Ž�����
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
// ���븡������С���������λ������ͼ���0.��������λ��������
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
  lCounter += i; // ���1������Ҫ��������Ķ���

  return true;
}