#include "stdafx.h"
#include "globedef.h"
#include"Accessory.h"
#include"RTData.h"
#include"StockID.h"

#include"Accessory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CRTData::Reset(void) {
  m_lDataSource = __INVALID_RT_WEB_DATA__;
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
  m_llCurrentValue = m_llTotalValue = 0;
  m_lBuy = 0;
  m_lSell = 0;
  for (int i = 0; i < 5; i++) {
    m_lPBuy.at(i) = 0;
    m_lVBuy.at(i) = 0;
    m_lPSell.at(i) = 0;
    m_lVSell.at(i) = 0;
  }
  m_fActive = false;

  m_mapNeteaseSymbolToIndex[_T("time")] = 1;
  m_mapNeteaseSymbolToIndex[_T("code")] = 2;
  m_mapNeteaseSymbolToIndex[_T("name")] = 3;
  m_mapNeteaseSymbolToIndex[_T("type")] = 4;
  m_mapNeteaseSymbolToIndex[_T("symbol")] = 5;
  m_mapNeteaseSymbolToIndex[_T("status")] = 6;
  m_mapNeteaseSymbolToIndex[_T("update")] = 7;
  m_mapNeteaseSymbolToIndex[_T("open")] = 10;
  m_mapNeteaseSymbolToIndex[_T("yestclose")] = 11;
  m_mapNeteaseSymbolToIndex[_T("high")] = 12;
  m_mapNeteaseSymbolToIndex[_T("low")] = 13;
  m_mapNeteaseSymbolToIndex[_T("price")] = 14;
  m_mapNeteaseSymbolToIndex[_T("volume")] = 15;
  m_mapNeteaseSymbolToIndex[_T("precloseioev")] = 16;
  m_mapNeteaseSymbolToIndex[_T("bid1")] = 20;
  m_mapNeteaseSymbolToIndex[_T("bid2")] = 21;
  m_mapNeteaseSymbolToIndex[_T("bid3")] = 22;
  m_mapNeteaseSymbolToIndex[_T("bid4")] = 23;
  m_mapNeteaseSymbolToIndex[_T("bid5")] = 24;
  m_mapNeteaseSymbolToIndex[_T("bidvol1")] = 30;
  m_mapNeteaseSymbolToIndex[_T("bidvol2")] = 31;
  m_mapNeteaseSymbolToIndex[_T("bidvol3")] = 32;
  m_mapNeteaseSymbolToIndex[_T("bidvol4")] = 33;
  m_mapNeteaseSymbolToIndex[_T("bidvol5")] = 34;
  m_mapNeteaseSymbolToIndex[_T("ask1")] = 40;
  m_mapNeteaseSymbolToIndex[_T("ask2")] = 41;
  m_mapNeteaseSymbolToIndex[_T("ask3")] = 42;
  m_mapNeteaseSymbolToIndex[_T("ask4")] = 43;
  m_mapNeteaseSymbolToIndex[_T("ask5")] = 44;
  m_mapNeteaseSymbolToIndex[_T("askvol1")] = 50;
  m_mapNeteaseSymbolToIndex[_T("askvol2")] = 51;
  m_mapNeteaseSymbolToIndex[_T("askvol3")] = 52;
  m_mapNeteaseSymbolToIndex[_T("askvol4")] = 53;
  m_mapNeteaseSymbolToIndex[_T("askvol5")] = 54;
  m_mapNeteaseSymbolToIndex[_T("percent")] = 60;
  m_mapNeteaseSymbolToIndex[_T("updown")] = 61;
  m_mapNeteaseSymbolToIndex[_T("arrow")] = 62;
  m_mapNeteaseSymbolToIndex[_T("turnover")] = 63;
}

CRTData::CRTData(void) : CObject() {
  Reset();
}

/////////////////////////////////////////////////////////////////////////////
// CRTData diagnostics

#ifdef _DEBUG
void CRTData::AssertValid() const {
  CObject::AssertValid();
}

void CRTData::Dump(CDumpContext& dc) const {
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
bool CRTData::SetData(CRTData& data) {
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
  m_llCurrentValue = data.m_llCurrentValue;
  m_llTotalValue = data.m_llTotalValue;

  for (int i = 0; i < 5; i++) {
    m_lPBuy.at(i) = data.m_lPBuy.at(i);
    m_lVBuy.at(i) = data.m_lVBuy.at(i);
    m_lPSell.at(i) = data.m_lPSell.at(i);
    m_lVSell.at(i) = data.m_lVSell.at(i);
  }

  return(true);
}

bool CRTData::Compare(CRTDataPtr pRTData) {
  time_t ttDiff = m_time - pRTData->GetTransactionTime();
  if (ttDiff != 0) { TRACE("����ʱ�䲻ƥ��: %d\n", ttDiff); }
  if (ttDiff != 0) {
    if (m_llVolume != pRTData->GetVolume()) {
      TRACE("�ɽ�������ƥ��: %d\n", m_llVolume - pRTData->GetVolume());
    }
  }
  return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ļ�file�ж�ȡ������ʽʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
//
//  ����ʵʱ����վ�㣺http://hq.sinajs.cn/list=sh601006
// var hq_str_sh601006=��������·,27.55,27.25,26.91,27.55,26.20,26.91,26.92,
//                     22114263,589824680,4695,26.91,57590,26.90,14700,26.89,14300,
//                     26.88,15100,26.87,3100,26.92,8900,26.93,14230,26.94,25150,26.95,15220,26.96,2008-01-11,15:05:32,00��;
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
// 31����15:05:32�壬ʱ�䣻
// 32����00����  ��������
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CRTData::ReadSinaData(CSinaRTWebData* pSinaRTWebData) {
  static char buffer1[100];
  char buffer2[7];
  static char buffer3[100];
  static CString strHeader = _T("var hq_str_s");
  long lStockCode = 0;

  try {
    m_fActive = false;    // ��ʼ״̬Ϊ��Ч����
    strncpy_s(buffer1, pSinaRTWebData->m_pCurrentPos, 12); // ���롰var hq_str_s"
    buffer1[12] = 0x000;
    CString str1;
    str1 = buffer1;
    if (strHeader.Compare(str1) != 0) { // ���ݸ�ʽ����
      return false;
    }
    pSinaRTWebData->IncreaseCurrentPos(12);

    if (*pSinaRTWebData->m_pCurrentPos == 'h') { // �Ϻ���Ʊ
      m_wMarket = __SHANGHAI_MARKET__; // �Ϻ���Ʊ��ʶ
    }
    else if (*pSinaRTWebData->m_pCurrentPos == 'z') {
      m_wMarket = __SHENZHEN_MARKET__; // ���ڹ�Ʊ��ʶ
    }
    else {
      return false;
    }
    pSinaRTWebData->IncreaseCurrentPos();

    strncpy_s(buffer2, pSinaRTWebData->m_pCurrentPos, 6);
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
    lStockCode = GetValue(buffer2);
    pSinaRTWebData->IncreaseCurrentPos(6);

    strncpy_s(buffer1, pSinaRTWebData->m_pCurrentPos, 2); // ����'="'
    if (buffer1[0] != '=') {
      return false;
    }
    if (buffer1[1] != '"') {
      return false;
    }
    pSinaRTWebData->IncreaseCurrentPos(2);
    strncpy_s(buffer1, pSinaRTWebData->m_pCurrentPos, 2);
    if (buffer1[0] == '"') { // û������
      if (buffer1[1] != ';') {
        return false;
      }
      pSinaRTWebData->IncreaseCurrentPos(2);
      if (*pSinaRTWebData->m_pCurrentPos != 0x00a) {
        return false;
      }
      pSinaRTWebData->IncreaseCurrentPos();
      m_fActive = false;
      return true;  // �ǻ�Ծ��Ʊû��ʵʱ���ݣ��ڴ˷��ء�
    }
    if ((buffer1[0] == 0x00a) || (buffer1[0] == 0x000)) {
      return false;
    }
    if ((buffer1[1] == 0x00a) || (buffer1[1] == 0x000)) {
      return false;
    }
    pSinaRTWebData->IncreaseCurrentPos(2);

    int i = 2;
    while (*pSinaRTWebData->m_pCurrentPos != 0x02c) { // ����ʣ�µ��������֣���һ������buffer1�У�
      if ((*pSinaRTWebData->m_pCurrentPos == 0x00a) || (*pSinaRTWebData->m_pCurrentPos == 0x000)) {
        return false;
      }
      buffer1[i++] = *pSinaRTWebData->m_pCurrentPos;
      pSinaRTWebData->IncreaseCurrentPos();
    }
    buffer1[i] = 0x000;
    m_strStockName = buffer1; // ���ù�Ʊ����

    pSinaRTWebData->IncreaseCurrentPos();

    // ���뿪�̼ۡ��Ŵ�һǧ����洢Ϊ�����͡������۸�����ˡ�
    if (!ReadSinaOneValueExceptPeriod(pSinaRTWebData, m_lOpen)) {
      return false;
    }
    // ����ǰ���̼�
    if (!ReadSinaOneValueExceptPeriod(pSinaRTWebData, m_lLastClose)) {
      return false;
    }
    // ���뵱ǰ��
    if (!ReadSinaOneValueExceptPeriod(pSinaRTWebData, m_lNew)) {
      return false;
    }
    // ������߼�
    if (!ReadSinaOneValueExceptPeriod(pSinaRTWebData, m_lHigh)) {
      return false;
    }
    // ������ͼ�
    if (!ReadSinaOneValueExceptPeriod(pSinaRTWebData, m_lLow)) {
      return false;
    }
    // ���뾺���
    if (!ReadSinaOneValueExceptPeriod(pSinaRTWebData, m_lBuy)) {
      return false;
    }
    // ���뾺����
    if (!ReadSinaOneValueExceptPeriod(pSinaRTWebData, m_lSell)) {
      return false;
    }
    // ����ɽ��������ɽ������洢ʵ��ֵ
    if (!ReadSinaOneValue(pSinaRTWebData, m_llVolume)) {
      return false;
    }
    // ����ɽ����
    if (!ReadSinaOneValue(pSinaRTWebData, m_llAmount)) {
      return false;
    }
    // ������һ--����Ĺ����ͼ۸�
    for (int j = 0; j < 5; j++) {
      // ��������
      if (!ReadSinaOneValue(pSinaRTWebData, m_lVBuy.at(j))) {
        return false;
      }
      // ����۸�
      if (!ReadSinaOneValueExceptPeriod(pSinaRTWebData, m_lPBuy.at(j))) {
        return false;
      }
    }
    // ������һ--����Ĺ����ͼ۸�
    for (int j = 0; j < 5; j++) {
      // ��������
      if (!ReadSinaOneValue(pSinaRTWebData, m_lVSell.at(j))) {
        return false;
      }
      // ����۸�
      if (!ReadSinaOneValueExceptPeriod(pSinaRTWebData, m_lPSell.at(j))) {
        return false;
      }
    }
    // ����ɽ����ں�ʱ��
    if (!ReadSinaOneValueExceptPeriod(pSinaRTWebData, buffer1)) {
      return false;
    }
    CString strTime;
    strTime = buffer1;
    strTime += ' '; //���һ���ո������������ת��
    if (!ReadSinaOneValueExceptPeriod(pSinaRTWebData, buffer3)) {
      return false;
    }
    strTime += buffer3;
    m_time = ConvertBufferToTime("%04d-%02d-%02d %02d:%02d:%02d", strTime.GetBuffer());

    // ��������ݽ�Ϊ��Ч���ݣ����������ݵĽ�β�����ɡ�
    while (*pSinaRTWebData->m_pCurrentPos != 0x00a) { // Ѱ���ַ�'\n'���س�����
      pSinaRTWebData->IncreaseCurrentPos();
      if (*pSinaRTWebData->m_pCurrentPos == 0x000) {
        return false;
      }
    }
    pSinaRTWebData->IncreaseCurrentPos(); // �����ַ�'\n'
    // �жϴ�ʵʱ�����Ƿ���Ч�������ڴ��жϣ�������ǽ�����Ч��Ʊ������٣����еĹ�Ʊ�����ݣ�����ֵ��Ϊ�㣬�����ɽ��ջ��Ʊ��ʱ��Ҫʵʱ��������Ч�ģ���
    // 0.03�汾����֮ǰ�Ķ�û�����жϣ�0.04�汾����ʹ�ò��жϵ����ְɡ�
    // ��ϵͳ׼�����ǰ���ж����˻�Ծ��Ʊ����ֻʹ�óɽ�ʱ��һ��ʶ���Ȼ���ڷǻ�Ծ��Ʊ�����С�
    if (IsDataTimeIn2Weeks()) m_fActive = true;
    else m_fActive = false;

    return true;
  }
  catch (exception & e) {
    TRACE(_T("ReadSinaData�쳣\n"));
    return false;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ��INT64ֵ���������Ž���������ֵ��llReturnValue��
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CRTData::ReadSinaOneValue(CSinaRTWebData* pSinaRTWebData, INT64& llReturnValue) {
  INT64 llTemp;
  static char buffer3[200];

  if (!ReadSinaOneValue(pSinaRTWebData, buffer3)) {
    return false;
  }
  llTemp = GetValue(buffer3);
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
bool CRTData::ReadSinaOneValue(CSinaRTWebData* pSinaRTWebData, long& lReturnValue) {
  long lTemp;
  static char buffer3[200];

  if (!ReadSinaOneValue(pSinaRTWebData, buffer3)) {
    return false;
  }
  lTemp = GetValue(buffer3);
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
bool CRTData::ReadSinaOneValue(CSinaRTWebData* pSinaRTWebData, char* buffer) {
  int i = 0;
  try {
    while (*pSinaRTWebData->m_pCurrentPos != ',') {
      if ((*pSinaRTWebData->m_pCurrentPos == 0x00a) || (*pSinaRTWebData->m_pCurrentPos == 0x000)) return false;
      buffer[i++] = *pSinaRTWebData->m_pCurrentPos;
      pSinaRTWebData->IncreaseCurrentPos();
    }
    buffer[i] = 0x000;
    // ���','�š�
    pSinaRTWebData->IncreaseCurrentPos();

    return true;
  }
  catch (exception & e) {
    TRACE(_T("ReadSinaOneValue�쳣\n"));
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���븡������С���������λ������ͼ���0.��������λ��������
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CRTData::ReadSinaOneValueExceptPeriod(CSinaRTWebData* pSinaRTWebData, long& lReturnValue) {
  long lTemp;
  static char buffer3[200];

  if (!ReadSinaOneValue(pSinaRTWebData, buffer3)) {
    return false;
  }
  lTemp = GetValue(buffer3) * 1000;
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
bool CRTData::ReadSinaOneValueExceptPeriod(CSinaRTWebData* pSinaRTWebData, char* buffer) {
  int i = 0;
  bool fFoundPoint = false;
  int iCount = 0;
  try {
    while ((*pSinaRTWebData->m_pCurrentPos != ',') && (iCount < 3)) {
      if (fFoundPoint) iCount++;
      if ((*pSinaRTWebData->m_pCurrentPos == 0x00a) || (*pSinaRTWebData->m_pCurrentPos == 0x000)) return false;
      if (*pSinaRTWebData->m_pCurrentPos == '.') {
        fFoundPoint = true;
      }
      else buffer[i++] = *pSinaRTWebData->m_pCurrentPos;
      pSinaRTWebData->IncreaseCurrentPos();
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

    while (*pSinaRTWebData->m_pCurrentPos != ',') {
      if ((*pSinaRTWebData->m_pCurrentPos == 0x00a) || (*pSinaRTWebData->m_pCurrentPos == 0x000)) return false;
      i++;
      pSinaRTWebData->IncreaseCurrentPos();
    }
    pSinaRTWebData->IncreaseCurrentPos();

    return true;
  }
  catch (exception & e) {
    TRACE(_T("ReadSinaOneValueExceptPeriod�쳣\n"));
    return false;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ļ�file�ж�ȡ��Ѷ��ʽʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
//
// Ҫ��ȡ�������飬�������ݽӿڣ�http://qt.gtimg.cn/q=sz002818
//
// v_sz000001="51~ƽ������~000001~15.59~15.90~15.75~1046363~518391~527971~
//             15.58~2365~15.57~802~15.56~1855~15.55~2316~15.54~320~15.59~661~15.60~15381~15.61~3266~15.62~450~15.63~520~~
//             20190930154003~-0.31~-1.95~15.89~15.57~15.59/1046363/1645828527~1046363~164583~0.54~11.27~~
//             15.89~15.57~2.01~3025.36~3025.38~1.15~17.49~14.31~
//             0.73~-12617~15.73~9.82~12.19~~~1.24~164582.85~0.00~0~~GP-A~68.91~~0.82";
//
// 0: �г����Ϻ�Ϊ1������Ϊ51����
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
// 44 : ��ͨ��ֵ(��λΪ���ڣ�
// 45 : ����ֵ����λΪ���ڣ�
// 46 : �о���
// 47 : ��ͣ��
// 48 : ��ͣ��   �������н�����
// 49 ��        ֮�����Щ���ֲ�����京��
//
//
// ��Ѷʵʱ�����У��ɽ����ĵ�λΪ�֣��޷��ﵽ��������ľ��ȣ��ɣ����ʶ�ֻ����Ϊ���ݲ���֮�á�
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CRTData::ReadTengxunData(CTengxunRTWebData* pTengxunRTWebData) {
  static char buffer1[200];
  char buffer2[7];
  static char buffer3[200];
  static CString strHeader = _T("v_s");
  long lTemp = 0;
  INT64 llTemp = 0;
  double dTemp = 0.0;
  long lStockCode = 0;

  try {
    m_fActive = false;    // ��ʼ״̬Ϊ��Ч����
    strncpy_s(buffer1, pTengxunRTWebData->m_pCurrentPos, 3); // ���롰v_s"
    buffer1[3] = 0x000;
    CString str1;
    str1 = buffer1;
    if (strHeader.Compare(str1) != 0) { // ���ݸ�ʽ����
      return false;
    }
    pTengxunRTWebData->IncreaseCurrentPos(3);
    if (*pTengxunRTWebData->m_pCurrentPos == 'h') { // �Ϻ���Ʊ
      m_wMarket = __SHANGHAI_MARKET__; // �Ϻ���Ʊ��ʶ
    }
    else if (*pTengxunRTWebData->m_pCurrentPos == 'z') {
      m_wMarket = __SHENZHEN_MARKET__; // ���ڹ�Ʊ��ʶ
    }
    else {
      return false;
    }
    pTengxunRTWebData->IncreaseCurrentPos();

    // ��λ��Ʊ����
    strncpy_s(buffer2, pTengxunRTWebData->m_pCurrentPos, 6);
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
    lStockCode = atoi(buffer2);
    pTengxunRTWebData->IncreaseCurrentPos(6);

    strncpy_s(buffer1, pTengxunRTWebData->m_pCurrentPos, 2); // ����'="'
    if (buffer1[0] != '=') {
      return false;
    }
    if (buffer1[1] != '"') {
      return false;
    }
    pTengxunRTWebData->IncreaseCurrentPos(2);

    // �г���ʶ���루51Ϊ���У�1Ϊ���У�
    if (!ReadTengxunOneValue(pTengxunRTWebData, lTemp)) {
      return false;
    }
#ifdef DEBUG
    if (lTemp == 1) ASSERT(m_wMarket == __SHANGHAI_MARKET__);
    else if (lTemp == 51) ASSERT(m_wMarket == __SHENZHEN_MARKET__);
    else ASSERT(0); // ����
#endif
    if (!ReadTengxunOneValue(pTengxunRTWebData, buffer1)) {
      return false;
    }
    m_strStockName = buffer1; // ���ù�Ʊ����
    // ��λ��Ʊ����
    if (!ReadTengxunOneValue(pTengxunRTWebData, lTemp)) {
      return false;
    }
    if (lTemp != lStockCode) return false;

    // ���ڳɽ��ۡ��Ŵ�һǧ����洢Ϊ�����͡������۸�����ˡ�
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }
    m_lNew = dTemp * 1000;
    // ǰ���̼�
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }
    m_lLastClose = dTemp * 1000;
    // ���̼�
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }
    m_lOpen = dTemp * 1000;
    // �ɽ��������ɽ������洢ʵ��ֵ
    if (!ReadTengxunOneValue(pTengxunRTWebData, llTemp)) {
      return false;
    }
    m_llVolume = llTemp * 100;
    // ����
    if (!ReadTengxunOneValue(pTengxunRTWebData, lTemp)) {
      return false;
    }
    // ����
    if (!ReadTengxunOneValue(pTengxunRTWebData, lTemp)) {
      return false;
    }
    // ������һ������ļ۸������
    for (int j = 0; j < 5; j++) {
      // ���̼۸�
      if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
        return false;
      }
      m_lPBuy.at(j) = dTemp * 1000;

      // �����������֣�
      if (!ReadTengxunOneValue(pTengxunRTWebData, lTemp)) {
        return false;
      }
      m_lVBuy.at(j) = lTemp * 100;
    }
    // ������һ������ļ۸������
    for (int j = 0; j < 5; j++) {
      //�������̼۸�
      if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
        return false;
      }
      m_lPSell.at(j) = dTemp * 1000;
      // �����������֣�
      if (!ReadTengxunOneValue(pTengxunRTWebData, lTemp)) {
        return false;
      }
      m_lVSell.at(j) = lTemp * 100;
    }
    // �����ʳɽ�
    if (!ReadTengxunOneValue(pTengxunRTWebData, lTemp)) {
      return false;
    }
    // �ɽ����ں�ʱ��.��ʽΪ��yyyymmddhhmmss
    if (!ReadTengxunOneValue(pTengxunRTWebData, buffer3)) {
      return false;
    }
    m_time = ConvertBufferToTime("%04d%02d%02d%02d%02d%02d", buffer3);
    // �ǵ�
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }
    // �ǵ���
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }
    // ��߼�
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }
    // ��ͼ�
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }
    // �ɽ���/�ɽ���/�ɽ����
    if (!ReadTengxunOneValue(pTengxunRTWebData, buffer3)) {
      return false;
    }
    // �ɽ�����
    if (!ReadTengxunOneValue(pTengxunRTWebData, lTemp)) {
      return false;
    }
    // �ɽ�����Ԫ��
    if (!ReadTengxunOneValue(pTengxunRTWebData, llTemp)) {
      return false;
    }
    // ������
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }
    // ��ӯ��
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }
    // ����
    if (!ReadTengxunOneValue(pTengxunRTWebData, buffer3)) {
      return false;
    }
    // ��߼�
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }
    // ��ͼ�
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }
    // ���
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }
    // ��ͨ��ֵ����λΪ����Ԫ��
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }
    m_llCurrentValue = dTemp * 100000000;
    // ����ֵ����λΪ����Ԫ��
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }
    m_llTotalValue = dTemp * 100000000;
    // �о���
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }
    // ��ͣ��
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }
    // ��ͣ��
    if (!ReadTengxunOneValue(pTengxunRTWebData, dTemp)) {
      return false;
    }

    while (*pTengxunRTWebData->m_pCurrentPos != 0x00a) {
      pTengxunRTWebData->IncreaseCurrentPos();
      if (*pTengxunRTWebData->m_pCurrentPos == 0x000) {
        return false;
      }
    }
    pTengxunRTWebData->IncreaseCurrentPos();
    if (!IsDataTimeIn2Weeks()) { // �������ʱ����һ��ǰ
      m_fActive = false;
    }
    else if ((m_lOpen == 0) && (m_llVolume == 0) && (m_lHigh == 0) && (m_lLow == 0)) { // ��Ѷ�ǻ�Ծ��Ʊ��m_lNew��Ϊ�㣬�ʶ�����ʹ������Ϊ�ж�����
      m_fActive = false; // ��Ѷ�ǻ�Ծ��Ʊ��ʵʱ����Ҳ�������е��ֶΣ��ʶ��ڴ�ȷ����Ϊ�ǻ�Ծ
    }
    else m_fActive = true;

    return true;
  }
  catch (exception & e) {
    TRACE(_T("ReadTengxunData�쳣\n"));
    return false;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ļ�file�ж�ȡ������ʽʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
// ��ʼ��Ϊ��һ��{��������Ϊ�����ڶ���}��������������账���򱻴�����ַ�Ϊ','�����û�������ˣ��򱻴�����ַ�Ϊ' '��
//
// Ҫ��ȡ�������飬�������ݽӿڣ�http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
// ����ʵʱ����ȱ�ٹؼ��Եĳɽ����һ��ʶ��޷���Ϊ�������ݣ�ֻ����Ϊ�����á�
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CRTData::ReadNeteaseData(CNeteaseRTWebData* pNeteaseRTWebData) {
  long lIndex = 0;
  CString strValue = _T("");
  char* pTestCurrentPos = pNeteaseRTWebData->m_pCurrentPos;
  char bufferTest[2000];
  char* pTestCurrentPos1 = pNeteaseRTWebData->m_pCurrentPos - 1;
  bool fFind = false;

  int i = 0;
  while ((*pTestCurrentPos != '}') && (i < 1900)) {
    bufferTest[i++] = *pTestCurrentPos++;
  }
  bufferTest[i] = 0x000;

  try {
    m_fActive = false;    // ��ʼ״̬Ϊ��Ч����
    // ���ǰ׺�ַ���"0601872")��ֱ��ʹ����������
    while (!fFind) {
      if ((*pNeteaseRTWebData->m_pCurrentPos == '"')
          && (*(pNeteaseRTWebData->m_pCurrentPos + 1) == ':')
          && (*(pNeteaseRTWebData->m_pCurrentPos + 2) == '{')) {
        fFind = true;
        pNeteaseRTWebData->IncreaseCurrentPos(3);
      }
      else pNeteaseRTWebData->IncreaseCurrentPos();
    }
    do {
      if (GetNeteaseIndexAndValue(pNeteaseRTWebData, lIndex, strValue)) {
        SetValue(lIndex, strValue);
      }
      else throw exception();
    } while ((lIndex != 63) && (*pNeteaseRTWebData->m_pCurrentPos != '}'));  // ����turnover(63)���������ַ�'}'
    // ������'}'�ͽ�����
    if (*pNeteaseRTWebData->m_pCurrentPos == '}') {
      pNeteaseRTWebData->IncreaseCurrentPos();
    }

    if (!IsDataTimeIn2Weeks()) { // �ǻ�Ծ��Ʊ��updateʱ��Ϊ0��ת��Ϊtime_tʱΪ-1.
      m_fActive = false;
    }
    else {
      if ((m_lOpen == 0) && (m_llVolume == 0) && (m_lHigh == 0) && (m_lLow == 0)) {
        m_fActive = false; // ���׷ǻ�Ծ��Ʊ��ʵʱ����Ҳ�������е��ֶΣ��ʶ��ڴ�ȷ����Ϊ�ǻ�Ծ
      }
      else m_fActive = true;
    }
    return true;
  }
  catch (exception & e) {
    TRACE(_T("ReadNeteaseData�쳣\n"));
    m_fActive = false;
    // ����˴������ݣ�Ѱ����һ�����ݵ���ʼ����
    while ((*pNeteaseRTWebData->m_pCurrentPos != '{') && (pNeteaseRTWebData->m_lCurrentPos < pNeteaseRTWebData->m_lByteRead)) {
      pNeteaseRTWebData->IncreaseCurrentPos();
    }
    return true; // �����棬�����˴������ݣ���������
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ��INT64ֵ������~�Ž���������ֵ��llReturnValue��
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CRTData::ReadTengxunOneValue(CTengxunRTWebData* pTengxunRTWebData, INT64& llReturnValue) {
  INT64 llTemp;
  static char buffer3[200];

  if (!ReadTengxunOneValue(pTengxunRTWebData, buffer3)) {
    return false;
  }
  llTemp = atoll(buffer3);
  if (llTemp < 0) return false;
  if (llTemp > 0) llReturnValue = llTemp;
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ��������ֵ������~�Ž���������ֵ��dReturnValue��
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CRTData::ReadTengxunOneValue(CTengxunRTWebData* pTengxunRTWebData, double& dReturnValue) {
  double dTemp;
  static char buffer3[200];

  if (!ReadTengxunOneValue(pTengxunRTWebData, buffer3)) {
    return false;
  }
  dTemp = atof(buffer3);
  dReturnValue = dTemp;
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ��������ֵ������~�Ž���������ֵ��lReturnValue��
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CRTData::ReadTengxunOneValue(CTengxunRTWebData* pTengxunRTWebData, long& lReturnValue) {
  long lTemp;
  static char buffer3[200];

  if (!ReadTengxunOneValue(pTengxunRTWebData, buffer3)) {
    return false;
  }
  lTemp = atol(buffer3);
  if (lTemp < 0) return false;
  if (lTemp > 0) lReturnValue = lTemp;
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ���ַ���������~�Ž����������buffer�С�
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CRTData::ReadTengxunOneValue(CTengxunRTWebData* pTengxunRTWebData, char* buffer) {
  int i = 0;
  try {
    while (*pTengxunRTWebData->m_pCurrentPos != '~') {
      if ((*pTengxunRTWebData->m_pCurrentPos == 0x00a) || (*pTengxunRTWebData->m_pCurrentPos == 0x000)) return false;
      buffer[i++] = *pTengxunRTWebData->m_pCurrentPos;
      pTengxunRTWebData->IncreaseCurrentPos();
    }
    buffer[i] = 0x000;
    pTengxunRTWebData->IncreaseCurrentPos();
    return true;
  }
  catch (exception & e) {
    TRACE("ReadTengxunOneValue�����쳣\n");
    return false;
  }
}

long CRTData::GetNeteaseSymbolIndex(CString strSymbol) {
  long lIndex = 0;
  try {
    lIndex = m_mapNeteaseSymbolToIndex.at(strSymbol);
  }
  catch (exception & e) {
    TRACE(_T("GetNeteaseSymbolIndex�쳣: %s\n"), strSymbol);
    lIndex = 0;
  }
  return lIndex;
}

bool CRTData::GetNeteaseIndexAndValue(CNeteaseRTWebData* pNeteaseRTWebData, long& lIndex, CString& strValue) {
  char buffer[100];
  int i = 0;
  CString strIndex;
  bool fFind = false;
  char* pTestCurrentPos;
  char* p = pNeteaseRTWebData->m_pCurrentPos - 1;
  char bufferTest[100];

  try {
    while (*pNeteaseRTWebData->m_pCurrentPos != '"') {
      pNeteaseRTWebData->IncreaseCurrentPos();
    }
    pNeteaseRTWebData->IncreaseCurrentPos();

    pTestCurrentPos = pNeteaseRTWebData->m_pCurrentPos;
    while ((*pTestCurrentPos != '}') && (*pTestCurrentPos != ',') && (i < 99)) {
      bufferTest[i++] = *pTestCurrentPos++;
    }
    bufferTest[i] = 0x000;

    i = 0;
    while ((*pNeteaseRTWebData->m_pCurrentPos != '"') && (*pNeteaseRTWebData->m_pCurrentPos != ':') && (*pNeteaseRTWebData->m_pCurrentPos != ',')) {
      buffer[i++] = *pNeteaseRTWebData->m_pCurrentPos;
      pNeteaseRTWebData->IncreaseCurrentPos();
    }
    if (*pNeteaseRTWebData->m_pCurrentPos != '"') {
      TRACE(_T("δ������ȷ�ַ�'\"'"));
      return false;
    }
    buffer[i] = 0x000;
    strIndex = buffer;
    lIndex = GetNeteaseSymbolIndex(strIndex);
    // ���"\""�ַ�
    pNeteaseRTWebData->IncreaseCurrentPos();
    if (*pNeteaseRTWebData->m_pCurrentPos != ':') {
      TRACE(_T("δ������ȷ�ַ�':'"));
      return false;
    }
    pNeteaseRTWebData->IncreaseCurrentPos();
    if (*pNeteaseRTWebData->m_pCurrentPos != ' ') {
      TRACE(_T("δ������ȷ�ַ�' '"));
      return false;
    }
    pNeteaseRTWebData->IncreaseCurrentPos();

    if (*pNeteaseRTWebData->m_pCurrentPos == '"') {
      fFind = true;
      pNeteaseRTWebData->IncreaseCurrentPos();
    }
    else fFind = false;

    i = 0;
    if (fFind) {
      while ((*pNeteaseRTWebData->m_pCurrentPos != '"') && (*pNeteaseRTWebData->m_pCurrentPos != ',')) {
        buffer[i++] = *pNeteaseRTWebData->m_pCurrentPos;
        pNeteaseRTWebData->IncreaseCurrentPos();
      }
      if (*pNeteaseRTWebData->m_pCurrentPos != '"') {
        TRACE(_T("δ������ȷ�ַ�'\"'"));
        return false;
      }
      buffer[i] = 0x000;
      strValue = buffer;
      pNeteaseRTWebData->IncreaseCurrentPos();
    }
    else {
      while ((*pNeteaseRTWebData->m_pCurrentPos != ',') && (*pNeteaseRTWebData->m_pCurrentPos != '}')) {
        buffer[i++] = *pNeteaseRTWebData->m_pCurrentPos;
        pNeteaseRTWebData->IncreaseCurrentPos();
      }
      buffer[i] = 0x000;
      strValue = buffer;
    }
    return true;
  }
  catch (exception & e) {
    TRACE(_T("GetNeteaseIndexAndValue Exception\n"));
    lIndex = 0;
    strValue = _T("");
    return false;
  }
}

bool CRTData::SetValue(long lIndex, CString strValue) {
  CString str1, str;
  try {
    switch (lIndex) {
    case 1: // time
    break;
    case 2: // code
    ASSERT(strValue.GetLength() == 7);
    str = strValue.Left(1);
    if (str.Compare(_T("0")) == 0) {
      str1 = _T("sh");
    }
    else str1 = _T("sz");
    m_strStockCode = str1 + strValue.Right(6);
    break;
    case 3: // name
    m_strStockName = strValue;
    break;
    case 4: // type
    break;
    case 5: // symbol
    break;
    case 6: // status
    break;
    case 7: // update
    m_time = ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strValue);
    break;
    case 10: // open
    m_lOpen = atof(strValue) * 1000;
    break;
    case 11: // yestclose
    m_lLastClose = atof(strValue) * 1000;
    break;
    case 12: // high
    m_lHigh = atof(strValue) * 1000;
    break;
    case 13: // low
    m_lLow = atof(strValue) * 1000;
    break;
    case 14: // price
    m_lNew = atof(strValue) * 1000;
    break;
    case 15: // volume
    m_llVolume = atol(strValue);
    break;
    case 20: // bid1
    m_lPBuy[0] = atof(strValue) * 1000;
    break;
    case 21: // bid2
    m_lPBuy[1] = atof(strValue) * 1000;
    break;
    case 22: // bid3
    m_lPBuy[2] = atof(strValue) * 1000;
    break;
    case 23: // bid4
    m_lPBuy[3] = atof(strValue) * 1000;
    break;
    case 24: // bid5
    m_lPBuy[4] = atof(strValue) * 1000;
    break;
    case 30: // bidvol1
    m_lVBuy[0] = atol(strValue);
    break;
    case 31: // bidvol2
    m_lVBuy[1] = atol(strValue);
    break;
    case 32: // bidvol3
    m_lVBuy[2] = atol(strValue);
    break;
    case 33: // bidvol4
    m_lVBuy[3] = atol(strValue);
    break;
    case 34: // bidvol5
    m_lVBuy[4] = atol(strValue);
    break;
    case 40: // ask1
    m_lPSell[0] = atof(strValue) * 1000;
    break;
    case 41: // ask2
    m_lPSell[1] = atof(strValue) * 1000;
    break;
    case 42: // ask3
    m_lPSell[2] = atof(strValue) * 1000;
    break;
    case 43: // ask4
    m_lPSell[3] = atof(strValue) * 1000;
    break;
    case 44: // ask5
    m_lPSell[4] = atof(strValue) * 1000;
    break;
    case 50: // askvol1
    m_lVSell[0] = atol(strValue);
    break;
    case 51: // askvol2
    m_lVSell[1] = atol(strValue);
    break;
    case 52: // askvol3
    m_lVSell[2] = atol(strValue);
    break;
    case 53: // askvol4
    m_lVSell[3] = atol(strValue);
    break;
    case 54: // askvol5
    m_lVSell[4] = atol(strValue);
    break;
    case 60: // percent
    case 61: // updown
    case 62: // arrow
    case 63: // turnover
    break;
    default:
    // ������
    break;
    }
    return true;
  }
  catch (exception & e) {
    TRACE(_T("SetValue�쳣\n"));
    return false;
  }
}

bool CRTData::IsDataTimeIn2Weeks(void) {
  if (m_time < (gl_systemTime.Gett_time() - 14 * 3600 * 24)) {
    return false;
  }
  else if (m_time > (gl_systemTime.Gett_time() + 3600)) return false;
  else return true;
}