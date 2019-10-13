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
// �������ļ�file�ж�ȡ������ʽʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
//
//  ����ʵʱ����վ�㣺http://hq.sinajs.cn/
// var hq_str_sh601006=��������·,27.55,27.25,26.91,27.55,26.20,26.91,26.92,
//                     22114263,589824680,4695,26.91,57590,26.90,14700,26.89,14300,
//                     26.88,15100,26.87,3100,26.92,8900,26.93,14230,26.94,25150,26.95,15220,26.96,2008-01-11,15:05:32��;
//
// ����ַ������������ƴ����һ�𣬲�ͬ����������ö��Ÿ����ˣ����ճ���Ա��˼·��˳��Ŵ�0��ʼ��
// 0����������·������Ʊ���֣�
// 1����27.55�壬���տ��̼ۣ�
// 2����27.25�壬�������̼ۣ�
// 3����26.91�壬��ǰ�۸�
// 4����27.55�壬������߼ۣ�
// 5����26.20�壬������ͼۣ�
// 6����26.91�壬����ۣ�������һ�����ۣ�
// 7����26.92�壬�����ۣ�������һ�����ۣ�
// 8����22114263�壬�ɽ��Ĺ�Ʊ�������ڹ�Ʊ������һ�ٹ�Ϊ������λ��������ʹ��ʱ��ͨ���Ѹ�ֵ����һ�٣�
// 9����589824680�壬�ɽ�����λΪ��Ԫ����Ϊ��һĿ��Ȼ��ͨ���ԡ���Ԫ��Ϊ�ɽ����ĵ�λ������ͨ���Ѹ�ֵ����һ��
// 10����4695�壬����һ������4695�ɣ���47�֣�
// 11����26.91�壬����һ�����ۣ�
// 12����57590�壬�������
// 13����26.90�壬�������
// 14����14700�壬��������
// 15����26.89�壬��������
// 16����14300�壬�����ġ�
// 17����26.88�壬�����ġ�
// 18����15100�壬�����塱
// 19����26.87�壬�����塱
// 20����3100�壬����һ���걨3100�ɣ���31�֣�
// 21����26.92�壬����һ������
// (22, 23), (24, 25), (26, 27), (28, 29)�ֱ�Ϊ���������������ĵ������
// 30����2008 - 01 - 11�壬���ڣ�
// 31����15:05 : 32�壬ʱ�䣻
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CStockRTData::ReadSinaData(char*& pCurrentPos, long& lTotalRead)
{
  static char buffer1[200];
  char buffer2[7];
  static char buffer3[200];
  static CString strHeader = _T("var hq_str_s");

  m_fActive = false;    // ��ʼ״̬Ϊ��Ч����
  strncpy_s(buffer1, pCurrentPos, 12); // ���롰var hq_str_s"
  buffer1[12] = 0x000;
  CString str1;
  str1 = buffer1;
  if (strHeader.Compare(str1) != 0) { // ���ݸ�ʽ����
    return false;
  }
  pCurrentPos += 12;
  lTotalRead += 12;

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
  lTotalRead += 1;

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
  lTotalRead += 6;

  strncpy_s(buffer1, pCurrentPos, 2); // ����'="'
  if (buffer1[0] != '=') {
    return false;
  }
  if (buffer1[1] != '"') {
    return false;
  }
  pCurrentPos += 2;
  lTotalRead += 2;
  strncpy_s(buffer1, pCurrentPos, 2);
  if (buffer1[0] == '"') { // û������
    if (buffer1[1] != ';') {
      return false;
    }
    lTotalRead += 2;
    pCurrentPos += 2;
    if (*pCurrentPos++ != 0x00a) {
      return false; // ȷ�����ַ� \n
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
  while (*pCurrentPos != 0x02c) { // ����ʣ�µ��������֣���һ������buffer1�У�
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
      return false;
    }
    buffer1[i++] = *pCurrentPos++;
    lTotalRead++;
  }
  buffer1[i] = 0x000;
  m_strStockName = buffer1; // ���ù�Ʊ����

  pCurrentPos++;
  lTotalRead++;

  // ���뿪�̼ۡ��Ŵ�һǧ����洢Ϊ�����͡������۸�����ˡ�
  if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lOpen, lTotalRead)) {
    return false;
  }
  // ����ǰ���̼�
  if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lLastClose, lTotalRead)) {
    return false;
  }
  // ���뵱ǰ��
  if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lNew, lTotalRead)) {
    return false;
  }
  // ������߼�
  if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lHigh, lTotalRead)) {
    return false;
  }
  // ������ͼ�
  if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lLow, lTotalRead)) {
    return false;
  }
  // ���뾺���
  if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lBuy, lTotalRead)) {
    return false;
  }
  // ���뾺����
  if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lSell, lTotalRead)) {
    return false;
  }
  // ����ɽ��������ɽ������洢ʵ��ֵ
  if (!ReadSinaOneValue(pCurrentPos, m_llVolume, lTotalRead)) {
    return false;
  }
  // ����ɽ����
  if (!ReadSinaOneValue(pCurrentPos, m_llAmount, lTotalRead)) {
    return false;
  }
  // ������һ--����Ĺ����ͼ۸�
  for (int j = 0; j < 5; j++) {
    // ��������
    if (!ReadSinaOneValue(pCurrentPos, m_lVBuy.at(j), lTotalRead)) {
      return false;
    }
    // ����۸�
    if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lPBuy.at(j), lTotalRead)) {
      return false;
    }
  }
  // ������һ--����Ĺ����ͼ۸�
  for (int j = 0; j < 5; j++) {
    // ��������
    if (!ReadSinaOneValue(pCurrentPos, m_lVSell.at(j), lTotalRead)) {
      return false;
    }
    // ����۸�
    if (!ReadSinaOneValueExceptPeriod(pCurrentPos, m_lPSell.at(j), lTotalRead)) {
      return false;
    }
  }
  // ����ɽ����ں�ʱ��
  i = 0;
  while (*pCurrentPos != ',') { // �������ڡ���ʽΪ:yyyy-mm-dd
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
      return false;
    }
    buffer3[i++] = *pCurrentPos++;
    lTotalRead++;
  }
  pCurrentPos++;
  lTotalRead++;

  buffer3[i++] = ' '; // ���һ���ո񣬷ָ����ں�ʱ��
  while (*pCurrentPos != ',') { // ����ʱ�䣬��ʽΪhh:mm:ss
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
// ����һ��INT64ֵ���������Ž���������ֵ��llReturnValue��
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
// ����һ��ֵ���������Ž���������ֵ��lReturnValue��
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
// ����һ��ֵ���������Ž�����
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
// ���븡������С���������λ������ͼ���0.��������λ��������
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
// ���븡������С���������λ������ͼ���0.��������λ��������
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
  lCounter += i; // ���1������Ҫ��������Ķ���

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ļ�file�ж�ȡ��Ѷ��ʽʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
//
// Ҫ��ȡ�������飬�������ݽӿڣ�http://qt.gtimg.cn/q=sz002818
//
// v_sz000001 = "51~ƽ������~000001~15.59~15.90~15.75~1046363~518391~527971~
//              15.58~2365~15.57~802~15.56~1855~15.55~2316~15.54~320~15.59~661~15.60~15381~15.61~3266~15.62~450~15.63~520~~
//              20190930154003~-0.31~-1.95~15.89~15.57~15.59/1046363/1645828527~1046363~164583~0.54~11.27~~
//              15.89~15.57~2.01~3025.36~3025.38~1.15~17.49~14.31~
//              0.73~-12617~15.73~9.82~12.19~~~1.24~164582.85~0.00~0~~GP-A~68.91~~0.82";
//
// 0: ����
// 1 : ����
// 2 : ����
// 3 : �ּ�
// 4 : ����
// 5 : ��
// 6 : �ɽ������֣�
// 7 : ����
// 8 : ���� ����һ�н�����
// 9 : ��һ
// 10 : ��һ�����֣�
// 11 - 18 : ��� - ����
// 19 : ��һ
// 20 : ��һ��
// 21 - 28 : ���� - ����
// 29 : �����ʳɽ� ( �ڶ��н�����
// 30 : ʱ��
// 31 : �ǵ�
// 32 : �ǵ� %
// 33 : ���
// 34 : ���
// 35 : �۸� / �ɽ������֣� / �ɽ���
// 36 : �ɽ������֣�
// 37 : �ɽ����
// 38 : ������
// 39 : ��ӯ��
// 40 : /       (�����н�����
// 41 : ���
// 42 : ���
// 43 : ���
// 44 : ��ͨ��ֵ
// 45 : ����ֵ
// 46 : �о���
// 47 : ��ͣ��
// 48 : ��ͣ��   �������н�����
// 49 ��        ֮�����Щ���ֲ�����京��
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

  m_fActive = false;    // ��ʼ״̬Ϊ��Ч����
  strncpy_s(buffer1, pCurrentPos, 3); // ���롰v=s"
  buffer1[3] = 0x000;
  CString str1;
  str1 = buffer1;
  if (strHeader.Compare(str1) != 0) { // ���ݸ�ʽ����
    return false;
  }
  pCurrentPos += 3;
  lTotalRead += 3;

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
  lTotalRead += 1;

  // ������λ��Ʊ����
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
  lTotalRead += 6;

  strncpy_s(buffer1, pCurrentPos, 2); // ����'="'
  if (buffer1[0] != '=') {
    return false;
  }
  if (buffer1[1] != '"') {
    return false;
  }
  pCurrentPos += 2;
  lTotalRead += 2;

  // ���벻�����루51����1��
  if (!ReadTengxunOneValue(pCurrentPos, lTemp, lTotalRead)) {
    return false;
  }

  int i = 0;
  while (*pCurrentPos != '~') { // �����Ʊ����������
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
      return false;
    }
    buffer1[i++] = *pCurrentPos++;
    lTotalRead++;
  }
  buffer1[i] = 0x000;
  m_strStockName = buffer1; // ���ù�Ʊ����
  pCurrentPos++;
  lTotalRead++;

  // ������λ��Ʊ����
  if (!ReadTengxunOneValue(pCurrentPos, lTemp, lTotalRead)) {
    return false;
  }
  if (lTemp != m_iStockCode) return false;

  // �������ڳɽ��ۡ��Ŵ�һǧ����洢Ϊ�����͡������۸�����ˡ�
  if (!ReadTengxunOneValue(pCurrentPos, m_lNew, lTotalRead)) {
    return false;
  }
  // ����ǰ���̼�
  if (!ReadTengxunOneValue(pCurrentPos, m_lLastClose, lTotalRead)) {
    return false;
  }
  // ���뿪�̼�
  if (!ReadTengxunOneValue(pCurrentPos, m_lOpen, lTotalRead)) {
    return false;
  }

  // ����ɽ��������ɽ������洢ʵ��ֵ
  if (!ReadTengxunOneValue(pCurrentPos, llTemp, lTotalRead)) {
    return false;
  }
  m_llVolume = llTemp * 100; 
  // ��������
  if (!ReadTengxunOneValue(pCurrentPos, lTemp, lTotalRead)) {
    return false;
  }
  // ��������
  if (!ReadTengxunOneValue(pCurrentPos, lTemp, lTotalRead)) {
    return false;
  }
  // ������һ������ļ۸������
  for (int j = 0; j < 5; j++) {
    // ������һ��
    if (!ReadTengxunOneValue(pCurrentPos, m_lPBuy.at(j), lTotalRead)) {
      return false;
    }
    // ������һ�������֣�
    if (!ReadTengxunOneValue(pCurrentPos, lTemp, lTotalRead)) {
      return false;
    }
    if (lTemp > 0) m_lVBuy.at(j) = lTemp * 100;
  }
  // ������һ������ļ۸������
  for (int j = 0; j < 5; j++) {
    // ������һ��
    if (!ReadTengxunOneValue(pCurrentPos, m_lPSell.at(j), lTotalRead)) {
      return false;
    }
    // ������һ�������֣�
    if (!ReadTengxunOneValue(pCurrentPos, lTemp, lTotalRead)) {
      return false;
    }
    if (lTemp > 0) m_lVSell.at(j) = lTemp * 100;
  }
  // �����ʳɽ�
  if (!ReadTengxunOneValue(pCurrentPos, lTemp, lTotalRead)) {
    return false;
  }

  // ����ɽ����ں�ʱ��
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
// ����һ��INT64ֵ������~�Ž���������ֵ��llReturnValue��
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
// ����һ��ֵ������~�Ž���������ֵ��lReturnValue��
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
// ����һ��ֵ������~�Ž�����
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
