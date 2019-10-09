//////////////////////////////////////////////////////////////////////////////////////////////////

//#include"stdafx.h"

#include"globedef.h"
#include"Thread.h"

#include"Market.h"

#include"SetDayLineInfo.h"
#include"SetDayLineToday.h"
#include"SetOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMarket::CMarket(void) : CObject() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("�г�����ֻ�������һ��ʵ��\n");
    ASSERT(0);
  }

  m_fPermitResetSystem = true; // ����ϵͳ�����ñ�ʶ��Ψ���˱�ʶ������ϵͳ���á���ʼʱ����Ϊ�棺��������ϵͳ��

  Reset();
}

CMarket::~CMarket() {
}

void CMarket::Reset(void)
{
  m_mapActiveStockToIndex.clear();
  m_vActiveStock.clear();
  m_lTotalActiveStock = 0; // ��ʼʱ��Ʊ���еĹ�Ʊ����Ϊ��

  m_fLoadedSelectedStock = false;
  m_fSystemReady = false;    // �г���ʼ״̬Ϊδ���úá�
  m_fCurrentStockChanged = false;
  m_fCurrentEditStockChanged = false;

  m_pCurrentStock = nullptr;

  m_lTotalMarketBuy = m_lTotalMarketSell = 0;

  time_t ttime;
  time(&ttime);
  tm tm_;
  localtime_s(&tm_, &ttime);
  if (tm_.tm_hour >= 15) { // �й���Ʊ�г��Ѿ�����
    m_fTodayStockCompiled = true; // ���к��ִ�б�ϵͳ������Ϊ�Ѿ���������չ�Ʊ�����ˡ�
  }
  else m_fTodayStockCompiled = false;

  m_lRelativeStrongEndDay = m_lRelativeStrongStartDay = m_lLastLoginDay = 19900101;

  m_lCountLoopRTDataInquiring = 0;

  m_fResetm_ItStock = true;

  m_fTodayTempDataLoaded = false;

  m_fCheckTodayActiveStock = true;  //��鵱�ջ�Ծ��Ʊ������Ϊ�档

  m_fUpdatedStockCodeDataBase = false;

  m_fCalculatingRS = false;

  m_fGetRTStockData = true;
  m_fReadingTengxunRTData = false; // Ĭ��״̬�²���ȡ��Ѷʵʱ����
  m_iCountDownDayLine = 3;    // 400ms��ʱ��100msÿ�Σ�
  m_iCountDownSlowReadingRTData = 3; // 400����ÿ��

  // ���ɹ�Ʊ�����
  CreateTotalStockContainer();
}

#ifdef _DEBUG
void CMarket::AssertValid() const
{
  CObject::AssertValid();
}

void CMarket::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc);
}
#endif //_DEBUG

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ʼ�����п��ܵĹ�Ʊ����أ�ֻ��CMarket�ĳ�ʼ��������һ�Ρ�
// ���������Ҫ����ȫ�ֱ�����ʼ����֧�֣��ʶ�gl_ChinaStockMarketʵ����Ҫ��������ȫ�ֱ��������
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::CreateTotalStockContainer(void)
{
  char buffer[10]{};

  StockIDPtr pStockID = nullptr;
  int iCount = 0;

  // ���֮ǰ�����ݣ�����еĻ�����Resetʱ�������������оʹ������ݣ���
  m_vChinaMarketAStock.clear();
  m_mapChinaMarketAStock.clear();

  // �����Ϻ���Ʊ����
  for (int i = 600000; i < 602000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHANGHAI_MARKET__); // �Ϻ�����
    pStockID->SetIndex(iCount);
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++; // ʹ���±������µ�ӳ��
  }

  // ���������Ʊ����
  for (int i = 603000; i < 604000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHANGHAI_MARKET__); // �Ϻ�����
    pStockID->SetIndex(iCount);
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++; // ʹ���±������µ�ӳ��
  }

  // ���ɿƴ����Ʊ����
  for (int i = 688000; i < 689000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHANGHAI_MARKET__); // �Ϻ��ƴ���
    pStockID->SetIndex(iCount);
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++; // ʹ���±������µ�ӳ��
  }

  // ����B�ɹ�Ʊ����
  for (int i = 900000; i < 901000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHANGHAI_MARKET__); // �Ϻ�B��
    pStockID->SetIndex(iCount);
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++; // ʹ���±������µ�ӳ��
  }

  // �����Ϻ�ָ������
  for (int i = 0; i < 1000; i++) {
    CString str = _T("sh");
    sprintf_s(buffer, 10, "%06d", i);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHANGHAI_MARKET__); // �Ϻ�ָ��
    pStockID->SetIndex(iCount);
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++; // ʹ���±������µ�ӳ��
  }

  ///////////////////////////////////////////////
  // �������������Ʊ����
  for (int i = 0; i < 2000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHENZHEN_MARKET__); // ��������
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++;// ʹ���±������µ�ӳ��
  }

  // ����������С���Ʊ����
  for (int i = 2000; i < 3000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHENZHEN_MARKET__); // ������С��
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++;// ʹ���±������µ�ӳ��
  }

  // ����B�ɹ�Ʊ����
  for (int i = 200000; i < 201000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHENZHEN_MARKET__); // ����B��
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++;// ʹ���±������µ�ӳ��
  }

  // ���ɴ�ҵ���Ʊ����
  for (int i = 300000; i < 301000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHENZHEN_MARKET__); // ���ڴ�ҵ��
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++;// ʹ���±������µ�ӳ��
  }

  // ��������ָ��
  for (int i = 399000; i < 400000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(__SHENZHEN_MARKET__); // ����ָ��
    m_vChinaMarketAStock.push_back(pStockID);
    m_mapChinaMarketAStock[pStockID->GetStockCode()] = iCount++;// ʹ���±������µ�ӳ��
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���ɹ�Ʊ������ַ��������ڲ�ѯ�˹�Ʊ�ڵ�ǰ�г��Ƿ��ڻ�Ծ״̬�������Ƿ���ڴ˹�Ʊ���룩
// ����ʵʱ��ʽ
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CMarket::CreateSinaRTDataInquiringStr(CString& str) {
  static bool fCreateStr = false;
  static int siCounter = 0;

  const long siTotalStock = m_vChinaMarketAStock.size();

  str = m_vChinaMarketAStock.at(siCounter)->GetStockCode();
  siCounter++;
  if (siCounter == siTotalStock) {
    siCounter = 0;
    return true; // ��ѯ��ͷ��
  }

  for (int i = 1; i < 900; i++) {  // ÿ����ѯ900����Ʊ.
    if (siCounter == siTotalStock) {
      siCounter = 0;
      return true; // ��ѯ��ͷ��
    }
    str += _T(",");
    str += m_vChinaMarketAStock.at(siCounter)->GetStockCode();
    siCounter++;
  }
  siCounter -= 2; // ��������Ϊ�˷�ֹ��Ե���ݴ��󣬹ʱ�Ե���ݲ�ѯ����(�������û��Ҫ�ˣ�ʵʱ���ݻ���û���������

  return false; // ��δ�������й�Ʊ
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���ɹ�Ʊ������ַ��������ڲ�ѯ�˹�Ʊ�ڵ�ǰ�г��Ƿ��ڻ�Ծ״̬�������Ƿ���ڴ˹�Ʊ���룩
// ��Ѷʵʱ��ʽ
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::CreateTengxunRTDataInquiringStr(CString& str) {
  static bool fCreateStr = false;
  static int siCounter = 0;

  const long siTotalStock = m_vChinaMarketAStock.size();

  str = m_vChinaMarketAStock.at(siCounter)->GetStockCode();
  siCounter++;
  if (siCounter == siTotalStock) {
    siCounter = 0;
    return true; // ��ѯ��ͷ��
  }

  for (int i = 1; i < 900; i++) {  // ÿ����ѯ900����Ʊ.
    if (siCounter == siTotalStock) {
      siCounter = 0;
      return true; // ��ѯ��ͷ��
    }
    str += _T(",");
    str += m_vChinaMarketAStock.at(siCounter)->GetStockCode();
    siCounter++;
  }
  siCounter -= 2; // ��������Ϊ�˷�ֹ��Ե���ݴ��󣬹ʱ�Ե���ݲ�ѯ����(�������û��Ҫ�ˣ�ʵʱ���ݻ���û���������

  return false; // ��δ�������й�Ʊ
}

bool CMarket::GetSinaStockRTData(void)
{
  static int iCountUp = 0;
  char* pCurrentPos = nullptr;
  CStockRTDataPtr pRTData = nullptr;
  long i = 0;
  INT64 iTotalNumber = 0;

  if (!gl_ThreadStatus.IsSinaRTDataReadingInProcess()) {
    if (gl_ThreadStatus.IsSinaRTDataReceived()) {
      if (gl_stSinaRTDataInquire.fError == false) { //����ͨ��һ��˳����
        iTotalNumber = gl_stSinaRTDataInquire.lByteRead;
        pCurrentPos = gl_stSinaRTDataInquire.buffer;
        long  iCount = 0;
        while (iCount < iTotalNumber) { // ����ʵʱ���ݻ���û�д��󣬲���Ҫ�׵����һ�������ˡ�
          pRTData = make_shared<CStockRTData>();
          if (pRTData->ReadSinaData(pCurrentPos, iCount)) {
            i++;
            gl_systemDequeData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
          }
          else {
            TRACE("����ʵʱ��������,�׵�����\n");
            CString str;
            str = _T("����ʵʱ��������");
            gl_systemMessage.PushInformationMessage(str);
            iCount = iTotalNumber; // ��������ݿ��ܳ����⣬�׵����á�
          }
        }
        TRACE("����%d������ʵʱ����\n", i);
        // ������յ���ʵʱ����
        ProcessRTData();
      }
      else {  // ����ͨ�ų��ִ���
        TRACE("Error reading http file ��hq.sinajs.cn\n");
        CString str;
        str = _T("Error reading http file ��hq.sinajs.cn");
        gl_systemMessage.PushInformationMessage(str);
      }
    }

    CString strTemp = _T("");

    // ������һ�������˹�Ʊʵʱ����
    if (m_fCheckTodayActiveStock || !SystemReady()) { // �������Ѱ�ҽ��ջ�Ծ��Ʊ�ڼ䣨9:10--9:29, 11:31--12:59),��ʹ��ȫ�ֹ�Ʊ��
      gl_stSinaRTDataInquire.strInquire = m_strSinaRTStockSource; // ���ò�ѯ����ʵʱ���ݵ��ַ���ͷ
      if (CreateSinaRTDataInquiringStr(strTemp)) {
        if (++m_lCountLoopRTDataInquiring >= 3) {  // ��������ȫ���Ʊ��
          if (!SystemReady()) { // ���ϵͳ��δ���úã�����ʾϵͳ׼��
            gl_systemMessage.PushInformationMessage(_T("���ϵͳ��ʼ��"));
          }
          SetSystemReady(true); // ���еĹ�Ʊʵʱ���ݶ���ѯһ�飬���ջ�Ծ��Ʊ���Ѿ��������ʶ����Խ������������ˡ�
        }
      }
      gl_stSinaRTDataInquire.strInquire += strTemp;
    }
    else { // ����ʱʹ�ý��ջ�Ծ��Ʊ��
      gl_stSinaRTDataInquire.strInquire = m_strSinaRTStockSource;
      GetInquiringStockStr(gl_stSinaRTDataInquire.strInquire);
    }
    gl_ThreadStatus.SetSinaRTDataReceived(false);
    gl_ThreadStatus.SetSinaRTDataReadingInProcess(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
    AfxBeginThread(ThreadReadingSinaRTDataProc, nullptr);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// ��Ѷʵʱ�����ȡ������ֻ�е�ϵͳ׼����Ϻ󣬷���ִ�д˺�����
//
// Ŀǰ��Ѷʵʱ���������еĳɽ���������Ϊÿ�֣��޷��ﵽ����ʵʱ���ݵ�ÿ�ɾ��ȣ��ʶ�ֻʹ���������ʵ����ݡ�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::GetTengxunStockRTData(void)
{
  static int iCountUp = 0;
  char* pCurrentPos = nullptr;
  CStockRTDataPtr pRTData = nullptr;
  long i = 0;
  INT64 iTotalNumber = 0;

  if (!gl_ThreadStatus.IsTengxunRTDataReadingInProcess()) {
    if (gl_ThreadStatus.IsTengxunRTDataReceived()) {
      if (gl_stTengxunRTDataInquire.fError == false) { //����ͨ��һ��˳����
        iTotalNumber = gl_stTengxunRTDataInquire.lByteRead;
        pCurrentPos = gl_stTengxunRTDataInquire.buffer;
        long  iCount = 0;
        /*
        while (iCount < iTotalNumber) { // ��Ѷʵʱ���ݻ���û�д��󣬲���Ҫ�׵����һ�������ˡ�
          pRTData = make_shared<CStockRTData>();
          if (pRTData->ReadTengxunData(pCurrentPos, iCount)) {
            i++;
            gl_systemDequeData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
          }
          else {
            TRACE("��Ѷʵʱ��������,�׵�����\n");
            CString str;
            str = _T("��Ѷʵʱ��������");
            gl_systemMessage.PushInformationMessage(str);
            iCount = iTotalNumber; // ��������ݿ��ܳ����⣬�׵����á�
          }
        }
        */
        TRACE("����%i�ֽ���Ѷʵʱ����\n", iTotalNumber);
        // ������յ���ʵʱ����
        //ProcessRTData();
      }
      else {  // ����ͨ�ų��ִ���
        TRACE("Error reading http file ��qt.gtimg.cn\n");
        CString str;
        str = _T("��Ѷʵʱ����ͨ�Ŵ���");
        gl_systemMessage.PushInformationMessage(str);
      }
    }

    CString strTemp = _T("");

    // ������һ����Ѷ��Ʊʵʱ����
    gl_stTengxunRTDataInquire.strInquire = m_strTengxunRTStockSource;
    GetInquiringStockStr(gl_stTengxunRTDataInquire.strInquire);
    gl_ThreadStatus.SetTengxunRTDataReceived(false);
    gl_ThreadStatus.SetTengxunRTDataReadingInProcess(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
    AfxBeginThread(ThreadReadingTengxunRTDataProc, nullptr);
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����������߹�Ʊ������ַ��������ڲ�ѯ�˹�Ʊ�ڵ�ǰ�г��Ƿ��ڻ�Ծ״̬�������Ƿ���ڴ˹�Ʊ���룩
//
//  �˺����Ǽ��m_vChinaMarketAStock��Ʊ��
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::CreateNeteaseDayLineInquiringStr(CString& str, CString& strStartDay) {
  static int iStarted = 0;
  static int siCounter = 0;

  long lTotalStock;

  StockIDPtr pStockID;

  lTotalStock = m_vChinaMarketAStock.size();

  bool fFound = false;
  int iCount = 0;
  CString strTemp;
  while (!fFound && (iCount < 1000)) {
    if (!m_vChinaMarketAStock.at(siCounter)->IsDayLineNeedUpdate()) { // �������ݲ���Ҫ���¡���ϵͳ��ʼʱ�����ô�m_fDayLineNeedUpdate��ʶ
      // TRACE("%S ���������������\n", static_cast<LPCWSTR>(m_vChinaMarketAStock.at(siCounter)->m_strStockCode));
      siCounter++;
      iCount++;
      if (siCounter == lTotalStock) {
        siCounter = 0;
      }
    }
    else if (m_vChinaMarketAStock.at(siCounter)->GetIPOStatus() == __STOCK_NULL__) {	// ��δʹ�ù��Ĺ�Ʊ���������ѯ��������
      m_vChinaMarketAStock.at(siCounter)->SetDayLineNeedUpdate(false); // �˹�Ʊ�������ϲ���Ҫ�����ˡ�
      // TRACE("��Ч��Ʊ���룺%S, �����ѯ��������\n", static_cast<LPCWSTR>(m_vChinaMarketAStock.at(siCounter)->m_strStockCode));
      iCount++;
      siCounter++;
      if (siCounter == lTotalStock) {
        siCounter = 0;
      }
    }
    else if (m_vChinaMarketAStock.at(siCounter)->GetDayLineEndDay() >= gl_systemTime.GetLastTradeDay()) { // ��һ�����յ����������Ѿ��洢����ʱ�Ѿ������һ�����������ˣ������ٴδ���
      m_vChinaMarketAStock.at(siCounter)->SetDayLineNeedUpdate(false); // �˹�Ʊ�������ϲ���Ҫ�����ˡ�
      // TRACE("%S �������ݱ����Ѹ���\n", static_cast<LPCWSTR>(m_vChinaMarketAStock.at(siCounter)->m_strStockCode));
      siCounter++;
      iCount++;
      if (siCounter == lTotalStock) {
        siCounter = 0;
      }
    }
    else {
      fFound = true;
    }
  }

  if (iCount >= 1000) { //  û���ҵ���Ҫ�������ߵĹ�Ʊ
    TRACE("δ�ҵ������������ʷ���ݵĹ�Ʊ\n");
    return false;
  }

  // �ҵ���������������ʷ���ݵĹ�Ʊ��siCounterΪ������
  StockIDPtr pID = m_vChinaMarketAStock.at(siCounter);
  switch (pID->GetMarket()) { // ת����������������������ʽ���Ϻ�Ϊ��0��������Ϊ��1����
  case __SHANGHAI_MARKET__: // �Ϻ��г���
  case __SHANGHAI_MAIN__: // �Ϻ����壿
  case __SHANGHAI_INDEX__: // �Ϻ�ָ��
  case __SHANGHAI_3BAN__: // �Ϻ�3��
  case __SHANGHAI_KECHUANG__: // �Ϻ��ƴ���
  case __SHANGHAI_B_SHARE__: // �Ϻ�B��
    str += '0'; // �Ϻ��г���ʶ
    break;
  case __SHENZHEN_MARKET__: // �����г���
  case __SHENZHEN_INDEX__: // ����ָ��
  case __SHENZHEN_3BAN__: // ɽ����С��
  case __SHENZHEN_B_SHARE__: // ����B��
  case __SHENZHEN_CHUANGYE__: // ���ڴ�ҵ��
  case __SHENZHEN_MAIN__: // ��������
    str += '1'; // �����г���ʶ
    break;
  default: // Խ��
    ASSERT(0);
  }
  char buffer[30];
  SetDownLoadingStockCodeStr(pID->GetStockCode());
  str += pID->GetStockCode().Right(6); // ȡ��Ʊ������ұ���λ���֡�
  tm tm_;
  tm_.tm_year = pID->GetDayLineEndDay() / 10000 - 1900;
  tm_.tm_mon = pID->GetDayLineEndDay() / 100 - (tm_.tm_year + 1900) * 100 - 1;
  tm_.tm_mday = pID->GetDayLineEndDay() - (tm_.tm_year + 1900) * 10000 - (tm_.tm_mon + 1) * 100;
  tm_.tm_hour = 12;
  tm_.tm_min = 0;
  tm_.tm_sec = 0;
  time_t tTime;
  tTime = mktime(&tm_);
  tTime += 3600 * 24; // ����һ�졣
  localtime_s(&tm_, &tTime);
  const long lDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday; // ���ߵ���ʼ�������������ڵĺ�һ�졣
  sprintf_s(buffer, "%8d", lDay);
  strStartDay = buffer;
  siCounter++;
  if (siCounter == lTotalStock) {
    siCounter = 0;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����������������ӿڣ���ȡ�������ݡ�
// ÿ�ζ�ȡһ����Ʊ��
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::GetNetEaseStockDayLineData(void)
{
  CString strRead;
  static bool sfFoundStock = true;
  CDayLinePtr pDayLine = nullptr;

  if (!gl_ThreadStatus.IsNeteaseDayLineReadingInProcess()) {
    if (sfFoundStock) {
      if ((gl_stDayLineInquire.fError == false) && gl_ThreadStatus.IsDayLineDataReady()) { //����ͨ��һ��˳����
        TRACE("��Ʊ%s��������Ϊ%d�ֽ�\n", GetDownLoadingStockCodeStr(), gl_stDayLineInquire.lByteRead);
        ASSERT(gl_stDayLineInquire.lByteRead < 2048 * 1024);
        // ����ǰ��Ʊ��������
        ProcessDayLineData(gl_stDayLineInquire.buffer, gl_stDayLineInquire.lByteRead);
      }
      else {
        if (gl_stDayLineInquire.lByteRead > 0) {
          TRACE("Error reading http file ��quotes.money.163.com/service/\n");
          CString str;
          str = _T("Error reading http file ��quotes.money.163.com/service/");
          gl_systemMessage.PushInformationMessage(str);
          gl_stDayLineInquire.lByteRead = 0;
        }
      }
    }

    CString strTemp;
    char buffer2[200];
    CString strStartDay;

    // ׼�������������������ʽ
    strRead = m_strNeteaseDayLineStockSource;
    sfFoundStock = CreateNeteaseDayLineInquiringStr(strRead, strStartDay);
    if (sfFoundStock) {
      strRead += _T("&start=");
      strRead += strStartDay;
      strRead += _T("&end=");
      sprintf_s(buffer2, "%8d", gl_systemTime.GetDay());
      strRead += buffer2;
      strRead += m_strNeteaseDayLinePostfix;

      gl_stDayLineInquire.strInquire = strRead;
      gl_ThreadStatus.SetDayLineDataReady(false);
      gl_ThreadStatus.SetNeteaseDayLineReadingInProcess(true); // ���������һ��(�߳���Ҳ���ã����Է��߳����ڻ����ӳٵ����ٴν��루�߳��˳�ʱ������˱�ʶ��
      // ����̵߳��������Բ��û���ģʽ����������ֱ�ӵ���
      AfxBeginThread(ThreadReadingNeteaseDayLineProc, nullptr);
      return true;
    }
    else return false;
  }
  return false;
}

/////////////////////////////////////////////////////////////////////////
//
//	�õ���ʱ��ƫ������09:30Ϊ0��15:00Ϊ240,����Ϊ1����
//
//
////////////////////////////////////////////////////////////////////////
long CMarket::GetMinLineOffset(CStockID sID, time_t Time) {
  ASSERT(Time >= 0);
  tm tmTemp{};
  time_t t = 0;
  long lIndex = 0;

  localtime_s(&tmTemp, &Time);
  tmTemp.tm_hour = (9 - 8);			// time_t, tmʹ�õ��ǹ��ʱ�׼ʱ(UTC),�ʱ���ʱ��09��30��UTC��01��30��Ҫ��ȥ8Сʱ
  tmTemp.tm_min = 30;
  tmTemp.tm_sec = 0;
  t = mktime(&tmTemp);
  lIndex = (Time - t) / 60;
  if (lIndex < 0) lIndex = 0;
  if ((lIndex >= 120) && (lIndex < 209)) lIndex = 119;
  if (lIndex >= 210) lIndex -= 90;
  if (lIndex >= 240) lIndex = 239;

  ASSERT((lIndex >= 0) && (lIndex < 240));
  return(lIndex);
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//		�ж�pStock�Ƿ�Ϊ����A�ɡ�
//		����A�ɴ�����6��ͷ������A�ɴ�����00��ͷ��
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::IsAStock(CStockPtr pStock) {
  ASSERT(pStock != nullptr);

  return(IsAStock(pStock->GetStockCode()));
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//		�ж�strStockCode�Ƿ�Ϊ����A������Ĺ�Ʊ���롣
//		����A�ɴ�����600��601��ͷ������A�ɴ�����000��001��ͷ��
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::IsAStock(CString strStockCode) {
  if ((strStockCode[0] == 's') && (strStockCode[1] == 'h') && (strStockCode[2] == '6') && (strStockCode[3] == '0')) {
    if ((strStockCode[4] == '0') || (strStockCode[4] == '1')) {
      return true;
    }
  }
  else {
    if ((strStockCode[0] == 's') && (strStockCode[1] == 'z') && (strStockCode[2] == '0') && (strStockCode[3] == '0')) {
      if ((strStockCode[4] == '0') || (strStockCode[4] == '2')) {
        return true;
      }
    }
  }
  return(false);
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//		�жϴ����Ƿ�Ϊ�����Ʊ��
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::IsStock(CString strStockCode, CStockPtr& pStock) {
  if ((pStock = GetStockPtr(strStockCode)) != nullptr) {
    return(true);
  }
  else {
    pStock = NULL;
    return(false);
  }
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////
INT64 CMarket::GetTotalAttackBuyAmount(void) {
  INT64 lAmount = 0;
  for (auto pStock : m_vActiveStock) {
    if (IsAStock(pStock)) {
      lAmount += pStock->GetAttackBuyAmount();
    }
  }
  return(lAmount);
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////
INT64 CMarket::GetTotalAttackSellAmount(void) {
  INT64 lAmount = 0;
  for (auto pStock : m_vActiveStock) {
    if (IsAStock(pStock)) {
      lAmount += pStock->GetAttackSellAmount();
    }
  }
  return(lAmount);
}

void CMarket::SetReadingSinaRTDataTime(clock_t tt)
{
  CSingleLock singleLock(&m_ReadingSinaRTDataTimeLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_ReadingSinaRTDataTime = tt;
    singleLock.Unlock();
  }
}

clock_t CMarket::GetReadingSinaRTDataTime(void)
{
  CSingleLock singleLock(&m_ReadingSinaRTDataTimeLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    clock_t tt = m_ReadingSinaRTDataTime;
    singleLock.Unlock();
    return tt;
  }
}

void CMarket::SetReadingTengxunRTDataTime(clock_t tt)
{
  CSingleLock singleLock(&m_ReadingTengxunRTDataTimeLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_ReadingTengxunRTDataTime = tt;
    singleLock.Unlock();
  }

}

clock_t CMarket::GetReadingTengxunRTDataTime(void)
{
  CSingleLock singleLock(&m_ReadingTengxunRTDataTimeLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    clock_t tt = m_ReadingTengxunRTDataTime;
    singleLock.Unlock();
    return tt;
  }
}

void CMarket::SetReadingNeteaseDayDataTime(clock_t tt)
{
  CSingleLock singleLock(&m_ReadingNeteaseDayDataTimeLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_ReadingNeteaseDayDataTime = tt;
    singleLock.Unlock();
  }
}

clock_t CMarket::GetReadingNeteaseDayDataTime(void)
{
  CSingleLock singleLock(&m_ReadingNeteaseDayDataTimeLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    clock_t tt = m_ReadingNeteaseDayDataTime;
    singleLock.Unlock();
    return tt;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ���ݵȣ���SchedulingTask�������á�
// ��ʵʱ�����ݴ�����е����ݷֱ��ŵ����Թ�Ʊ��ʵʱ�����С���������µĹ�Ʊ������������Ӧ���¹�Ʊ��
// �ַ�����ʱ��ֻ�ַ��µģ�����ʱ������֮ǰ���ݵģ�ʵʱ���ݡ�
//
// �˺����õ�������ȫ�ֱ��������Ƿ������߳�Ϊ�á������߳�Ŀǰ����ֻ���������Ʊ�Ĺҵ������
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::ProcessRTData(void)
{
  // ��������ʵʱ���ݣ����ɵ��յĻ�Ծ��Ʊ�г�
  CStockPtr pStock;
  CStockRTDataPtr pRTDataCompact = nullptr;
  const long lTotalNumber = gl_systemDequeData.GetRTDataDequeSize();

  for (int iCount = 0; iCount < lTotalNumber; iCount++) {
    CStockRTDataPtr pRTData = gl_systemDequeData.PopRTData();
    //ASSERT(pRTData->IsActive());  // ������Ӧ������Զ��Ч�ġ�
    if (pRTData->IsActive()) { // ��ʵʱ������Ч��
      long lIndex = 0;
      if (m_mapActiveStockToIndex.find(pRTData->GetStockCode()) == m_mapActiveStockToIndex.end()) { // �µĹ�Ʊ���룿
        pStock = make_shared<CStock>();
        pStock->SetActive(true);
        pStock->SetMarket(pRTData->GetMarket());
        pStock->SetStockCode(pRTData->GetStockCode());
        pStock->SetStockName(pRTData->GetStockName());
        pStock->SetCode(pRTData->GetCode());
        pStock->SetLastClose(pRTData->GetLastClose());
        pStock->SetOpen(pRTData->GetOpen());
        AddStockToMarket(pStock); // ��Ӵ˹�������������������Ŀǰ��m_lTotalActiveStaock��ֵ��
        ASSERT(m_vActiveStock.size() == m_lTotalActiveStock);
        pStock->PushRTData(pRTData);
        lIndex = m_mapChinaMarketAStock[pStock->GetStockCode()];
        m_vChinaMarketAStock.at(lIndex)->SetStockName(pStock->GetStockName());
        m_vChinaMarketAStock.at(lIndex)->SetActive(true); // ���ս��յ������ݣ�
        long lIndex2 = m_mapActiveStockToIndex.at(pRTData->GetStockCode());
        m_vActiveStock.at(lIndex2)->SetTransactionTime(pRTData->GetTransactionTime());  // �������½��ܵ�ʵʱ���ݵ�ʱ��
        m_vChinaMarketAStock.at(lIndex)->SetDayLineNeedUpdate(true);
        // ����˹�Ʊ������δʹ�ù���������Ϊ��ʹ�á�
        // ͣ�ƺ�Ĺ�Ʊ��Ҳ�ܽ��յ�ʵʱ���ݣ�ֻ��������ֻ�����̼ۣ�������Ϊ�㡣�������������Ч���ݡ�
        m_vChinaMarketAStock.at(lIndex)->SetIPOStatus(__STOCK_IPOED__);
        ASSERT(m_vChinaMarketAStock.at(lIndex)->GetStockCode().Compare(pStock->GetStockCode()) == 0);
      }
      else {
        lIndex = m_mapActiveStockToIndex.at(pRTData->GetStockCode());
        ASSERT(lIndex <= m_lTotalActiveStock);
        if (pRTData->GetTransactionTime() > m_vActiveStock.at(lIndex)->GetTransactionTime()) { // �µ����ݣ�
          m_vActiveStock.at(lIndex)->PushRTData(pRTData); // �洢�µ����������ݳ�
          m_vActiveStock.at(lIndex)->SetTransactionTime(pRTData->GetTransactionTime());   // �������½��ܵ�ʵʱ���ݵ�ʱ��
        }
      }
    }
  }
  gl_ThreadStatus.SetRTDataNeedCalculate(true); // ���ý��յ�ʵʱ���ݱ�ʶ

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����ÿ�β�ѯʵʱ��Ʊ���ݵ��ַ���
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////
int CMarket::GetInquiringStockStr(CString& str)
{
  int iCount = 1;

  if (m_fResetm_ItStock) {
    m_itStock = m_vActiveStock.begin();
    m_fResetm_ItStock = false;
  }

  str += (*m_itStock++)->GetStockCode();  // �õ���һ����Ʊ����
  while ((m_itStock != m_vActiveStock.end()) && (iCount < 900)) { // ÿ������ѯ��Ϊ900����Ʊ
    iCount++;
    str += ',';
    str += (*m_itStock++)->GetStockCode();
  }

  if (m_itStock == m_vActiveStock.end()) {
    m_itStock = m_vActiveStock.begin();
  }
  else {
    // ��һ�������ܹ�����ס��Ե
    m_itStock--;
  }
  return iCount;
}

////////////////////////////////////////////////////////////////////////////////////////
//
// �ɹ����߳�ThreadCalculatingRTDataProc���ã�ע��ȫ�ֱ�����ʹ��
//
//
/////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::CalculateRTData(void)
{
  for (auto pStock : m_vActiveStock) {
    ASSERT(pStock != nullptr);
    pStock->CalculateRTData();
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// ������������߷������϶�ȡ�Ĺ�Ʊ�������ݡ�
// ������ʽΪ�� ����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::ProcessDayLineData(char* buffer, long lLength) {
  long iCount = 0;
  char* pCurrentPos = buffer;
  char* pTestPos = buffer;
  vector<CDayLinePtr> vTempDayLine;

  if (lLength == 0) { // û�����ݶ��룿����״̬�ǲ�ѯ�Ĺ�ƱΪ��Ч�������ڣ�����
    return false;
  }

  while (*pCurrentPos != 0X0d) {
    if ((*pCurrentPos == 0x0a) || (*pCurrentPos == 0x000)) {
      return false;
    }
    pCurrentPos++;
    iCount++;
  }
  pCurrentPos++;
  if (*pCurrentPos != 0x0a) return false;
  iCount++;
  pCurrentPos++;
  iCount++;
  shared_ptr<CDayLine> pDayLine;
  long lIndex = -1;

  pTestPos = buffer;
  pTestPos += iCount;
  ASSERT(*pTestPos == *pCurrentPos);
  if (iCount == lLength) {// ��Ч��Ʊ���룬����ֻ��ǰ׺˵����û��ʵ����Ϣ�����������ˣ������Ѿ������ˣ��������¹����еĵ�һ��
    lIndex = m_mapChinaMarketAStock.at(m_strCurrentStockDownLoading);
    // ASSERT(!m_vChinaMarketAStock[lIndex]->m_fActive); ��һ����ƱIPO����δ����ʱ����Ʊ������ڵ�û���������ݡ�ȡ���˶����жϡ�
    // ��Щ��Ʊ�����к���ֱ��չ�����������������������ݲ��ٸ��¡���������������ô˹�ƱΪ��Ч����
    if (m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay() == 19900101) { // �����ʼ���߽������ڴ���û�б��������˹�Ʊ������δ��ʹ�ù�
      m_vChinaMarketAStock.at(lIndex)->SetIPOStatus(__STOCK_NULL__);   // �˹�Ʊ������δʹ�á�
      //TRACE("��Ч��Ʊ���룺%s\n", static_cast<LPCWSTR>(m_strCurrentStockDownLoading));
    }
    else { // �Ѿ����еĹ�Ʊ
      if (m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay() + 100 < gl_systemTime.GetDay()) {
        m_vChinaMarketAStock.at(lIndex)->SetIPOStatus(__STOCK_DELISTED__);   // �˹�Ʊ�����Ѿ����С�
      }
      //TRACE("%S û�пɸ��µ���������\n", static_cast<LPCWSTR>(gl_strCurrentStockDownLoading));
    }
    m_vChinaMarketAStock.at(lIndex)->SetDayLineNeedUpdate(false); // ������Ҫ������������
    return false;
  }

  lIndex = 0;
  long iTemp = 0;
  CString strTemp;
  pTestPos = buffer;
  pTestPos += iCount;
  ASSERT(*pTestPos == *pCurrentPos);
  while (iCount < lLength) {
    pDayLine = make_shared<CDayLine>();
    if (!ProcessOneItemDayLineData(pDayLine, pCurrentPos, iTemp)) { // ����һ����������
      TRACE("%s �������ݳ���\n", pDayLine->GetStockCode());
      // ������ݴ����������
      vTempDayLine.clear();
      return false; // ���ݳ�����������
    }
    iCount += iTemp;
    pTestPos = buffer;
    pTestPos += iCount;
    ASSERT(*pTestPos == *pCurrentPos);
    if (m_mapActiveStockToIndex.find(pDayLine->GetStockCode()) == m_mapActiveStockToIndex.end()) { // �µĹ�Ʊ���룿
      // �����¹�Ʊ
      auto pStock = make_shared<CStock>();
      pStock->SetActive(true);
      pStock->SetDayLineLoaded(false);
      pStock->SetMarket(pDayLine->GetMarket());
      pStock->SetStockCode(pDayLine->GetStockCode());
      pStock->SetStockName(pDayLine->GetStockName());
      strTemp = pStock->GetStockCode().Right(6); // ��ȡ��Ʊ�����ұߵ���������
      pStock->SetCode(atoi(strTemp.GetBuffer()));
      AddStockToMarket(pStock);
      lIndex = m_lTotalActiveStock - 1;
      ASSERT(m_vActiveStock.size() == m_lTotalActiveStock);
      // ����ȫ�ֹ�Ʊ����Ϣ����ʱRTData��ȫ���޷��������еĹ�Ʊ��Ϣ��
      long lIndexTemp = m_mapChinaMarketAStock.at(pStock->GetStockCode());
      m_vChinaMarketAStock.at(lIndexTemp)->SetStockCode(pStock->GetStockCode());
      m_vChinaMarketAStock.at(lIndexTemp)->SetStockName(pStock->GetStockName());
    }
    else lIndex = m_mapActiveStockToIndex.at(pDayLine->GetStockCode());
    //if ((pDayLine->m_lClose != 0) && (pDayLine->m_lOpen != 0)) { // ���������Ч����洢���������ݣ����к��������������Ч�ģ���
    vTempDayLine.push_back(pDayLine); // �ݴ�����ʱvector�У���Ϊ�����������ݵ�ʱ��˳���ǵߵ��ģ����µ�����ǰ��
  /*}
  else {
    int i111 = 0;
  }*/
  }
  strTemp = pDayLine->GetStockCode();
  strTemp += _T("�����������.");
  gl_systemMessage.PushDayLineInfoMessage(strTemp);
  m_vChinaMarketAStock.at(m_mapChinaMarketAStock.at(pDayLine->GetStockCode()))->SetDayLineNeedUpdate(false); // ��������������ϣ�����Ҫ�����������ˡ�
  if ((vTempDayLine.at(0)->GetDay() + 100) < gl_systemTime.GetDay()) { // ��ȡ���Ĺ�Ʊ���������������ղ����ϸ��µ���������գ������˻��������������һ���µ�ʱ��۲졣
    m_vChinaMarketAStock.at(m_mapChinaMarketAStock.at(pDayLine->GetStockCode()))->SetIPOStatus(__STOCK_DELISTED__); // �����л���ͣ���ס�
  }
  else {
    m_vChinaMarketAStock.at(m_mapChinaMarketAStock.at(pDayLine->GetStockCode()))->SetIPOStatus(__STOCK_IPOED__); // �������׹�Ʊ
  }
  ASSERT(lIndex >= 0);
  m_vActiveStock.at(lIndex)->SetDayLineLoaded(true);
  m_vActiveStock.at(lIndex)->SetDayLineNeedSavingFlag(true); // ���ô洢���߱�ʶ
  m_vActiveStock.at(lIndex)->m_vDayLine.clear(); // �����������������ݣ�����еĻ���
  // ������������ʱ��Ϊ�������
  for (int i = vTempDayLine.size() - 1; i >= 0; i--) {
    pDayLine = vTempDayLine.at(i);
    if (pDayLine->GetDay() < gl_systemTime.GetDay()) { // ��Ҫ�洢�����������ݣ���������������ʵʱ�������ɣ�.
      // ���¹ɵ�һ������ʱ��������ֻ��һ�죬�����������ӵ��ˣ���������������Ϊ�ա�����ʱע�⡣
      // �����ǵ�ȡgl_lLastTradeDay��֮ǰ���������ݣ��ʶ��¹ɵ����������϶�Ϊ�ա�
      m_vActiveStock.at(lIndex)->m_vDayLine.push_back(pDayLine);
    }
  }
  vTempDayLine.clear();

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ȡһ���������ݡ���������������ʷ���ݸ�ʽ��
//
// ��ʵʱ���������ƣ����ּ۸�ԷŴ�һǧ�����Գ����ʹ洢���������ݿ�ʱ��DECIMAL(10,3)���ʹ洢��
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::ProcessOneItemDayLineData(CDayLinePtr pDayLine, char*& pCurrentPos, long& lLength) {
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
      return false; // ���ݳ�����������
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
  pDayLine->SetTime(mktime(&tm_));
  lDay = year * 10000 + month * 100 + day;
  pDayLine->SetDay(lDay);
  //TRACE("%d %d %d\n", year, month, day);

  if (*pCurrentPos != 0x27) return(false); // ���ݳ�����������
  pCurrentPos++;
  iCount++;

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  str = buffer2;
  pDayLine->SetStockCode(m_strCurrentStockDownLoading);
  str = m_strCurrentStockDownLoading.Left(2);
  if (str == _T("sh")) {
    pDayLine->SetMarket(__SHANGHAI_MARKET__);
  }
  else if (str == _T("sz")) {
    pDayLine->SetMarket(__SHENZHEN_MARKET__);
  }
  else {
    ASSERT(0); // ��Ʊ������ʽ����
    return false;
  }
  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  str = buffer2;
  pDayLine->SetStockName(str);

  if (!ReadOneValueExceptperiod(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetClose(atol(buffer2));

  if (!ReadOneValueExceptperiod(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetHigh(atol(buffer2));

  if (!ReadOneValueExceptperiod(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetLow(atol(buffer2));

  if (!ReadOneValueExceptperiod(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetOpen(atol(buffer2));

  if (!ReadOneValueExceptperiod(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetLastClose(atol(buffer2));

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  if (pDayLine->GetOpen() == 0) {
    //ASSERT(strcmp(buffer2, "None") == 0);
    pDayLine->SetUpDown(0);
  }
  else pDayLine->SetUpDown(atof(buffer2));

  if (pDayLine->GetLastClose() == 0) { // �����ǵ�����
    pDayLine->SetUpDownRate(0); // ����������̼�Ϊ�㣨û���ף������ǵ���Ҳ��Ϊ�㡣
  }
  else {
    // ��Ҫ�Ŵ�1000 * 100�������̼۱�ʵ��ֵ��1000������¼���ǰٷֱȣ�ҲҪ����100����
    pDayLine->SetUpDownRate(((double)(pDayLine->GetUpDown() * 100000.0)) / pDayLine->GetLastClose());
  }

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetChangeHandRate(atof(buffer2));

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetVolume(atoll(buffer2)); // ������ǹ���

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetAmount(atoll(buffer2));

  if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
  pDayLine->SetTotalValue(atoll(buffer2)); // ����ֵ�ĵ�λΪ��Ԫ

  i = 0;
  while (*pCurrentPos != 0x0d) {
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) return false; // ���ݳ�����������
    buffer2[i++] = *pCurrentPos++;
    iCount++;
  }
  pCurrentPos++;
  iCount++;
  buffer2[i] = 0x000;
  pDayLine->SetCurrentValue(atoll(buffer2)); // ��ͨ��ֵ�ĵ�λΪ��Ԫ��
  if (*pCurrentPos++ != 0x0a) return false; // ���ݳ�����������
  iCount++;

  lLength = iCount;

  return true;
}

bool CMarket::ReadOneValue(char*& pCurrentPos, char* buffer, long& iReadNumber)
{
  int i = 0;

  while (*pCurrentPos != 0x2c) { // ����һ������ǰ���ַ����뻺����. 0x2c���Ƕ��š�
    if ((*pCurrentPos == 0x0d) || (*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
      return false; // ���ݳ�����������
    }
    buffer[i++] = *pCurrentPos++;
  }
  buffer[i] = 0x000;
  pCurrentPos++;
  i++;
  iReadNumber += i;
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���븡������С���������λ������ͼ���0.��������λ���������������ֵ�Ŵ�һǧ����
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::ReadOneValueExceptperiod(char*& pCurrentPos, char* buffer, long& lCounter)
{
  int i = 0;
  bool fFoundPoint = false;
  int iCount = 0;
  while ((*pCurrentPos != ',') && (iCount < 3)) {
    if (fFoundPoint) iCount++;
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x00d) || (*pCurrentPos == 0x000)) return false;
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
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x00d) || (*pCurrentPos == 0x000)) return false;
    i++;
    pCurrentPos++;
  }
  pCurrentPos++;
  i++;
  if (fFoundPoint) i++;
  lCounter += i; // ���1������Ҫ��������Ķ���

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//
// ��Լÿ100�������һ��
//
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SchedulingTask(void)
{
  static time_t s_timeLast = 0;

  gl_systemTime.CalculateTime();      // ����ϵͳ����ʱ��

  //����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
  if (gl_systemTime.Gett_time() > s_timeLast) {
    SchedulingTaskPerSecond(gl_systemTime.Gett_time() - s_timeLast);
    s_timeLast = gl_systemTime.Gett_time();
  }

  // ץȡʵʱ����(���˺�/����Ѷ��
  if (!gl_fExiting && m_fGetRTStockData && (m_iCountDownSlowReadingRTData <= 0)) {
    GetSinaStockRTData(); // ÿ400����(100X4)����һ��ʵʱ���ݡ����˵�ʵʱ�����������Ӧʱ�䲻����100���루30-70֮�䣩����û�г��ֹ����ݴ���

    // ��ȡ��Ѷʵʱ�������ݡ� ������Ѷʵʱ����Ĺ�������Ϊ�֣�û�������Ϣ�������޷�������ʵʱ�������ݶԽӣ����˾���Ϊ�ɣ����ʶ���ʱ����
    if (m_fReadingTengxunRTData && SystemReady()) {
      GetTengxunStockRTData();  // ֻ�е�ϵͳ׼����Ϻ󣬷���ִ�ж�ȡ��Ѷʵʱ�������ݵĹ���
    }

    // ���Ҫ�����ٶ�ȡʵʱ���ݣ������ö�ȡ����Ϊÿ����һ��
    if (!m_fMarketOpened && SystemReady()) m_iCountDownSlowReadingRTData = 1000; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
    else m_iCountDownSlowReadingRTData = 3;  // ����4��

  }
  m_iCountDownSlowReadingRTData--;

  // ϵͳ׼������֮����Ҫ��ɵĸ����
  if (SystemReady()) {
    // װ��֮ǰ�洢��ϵͳ�������ݣ�����еĻ���
    if (!m_fTodayTempDataLoaded) { // �˹���������һ�Ρ�
      LoadTodayTempData();
      m_fTodayTempDataLoaded = true;
    }

    // ץȡ��������
    if (!gl_fExiting && m_fGetDayLineData) {
      GetNetEaseStockDayLineData();
    }
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ʱ���Ⱥ�����ÿ��һ�Ρ�
//
// ��������֮���п��ܳ��ֻ����������洢��ʱʵʱ���ݵĹ����߳������ʵʱ���ݵĹ����߳�֮��Ͳ�����ͬʱ���У�
// �ʶ����еĶ�ʱ����Ҫ����ʱ�����ӳ��������У�����ִ��ÿ����һ�ε�������ִ��ÿ����һ�ε����������ܹ���֤���������������ִ�С�
//
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SchedulingTaskPerSecond(long lSecondNumber)
{
  static int i10SecondsCounter = 9; // ʮ��һ�εļ�����
  static int i1MinuteCounter = 59;  // һ����һ�εļ�����
  static int i5MinuteCounter = 299; // �����һ�εļ�����
  const long lTime = gl_systemTime.GetTime();

  // ����ÿ�����һ�ε�����
  if (i5MinuteCounter <= 0) {
    i5MinuteCounter = 299;

    // ��ҹ�������ø��ֱ�ʶ
    if (lTime <= 1500 && !m_fPermitResetSystem) {  // ����㵽���ʮ��֣�����ϵͳ��ʶ
      m_fPermitResetSystem = true;
      CString str;
      str = _T(" ����ϵͳ���ñ�ʶ");
      gl_systemMessage.PushInformationMessage(str);
    }

    // ����ʱÿ����Ӵ洢һ�ε�ǰ״̬������һ�����ô�ʩ����ֹ�˳�ϵͳ��Ͷ��������е����ݣ�����̫Ƶ����
    if (m_fMarketOpened && m_fSystemReady && !gl_ThreadStatus.IsCalculatingRTData()) {
      if (((lTime > 93000) && (lTime < 113600)) || ((lTime > 130000) && (lTime < 150000))) { // �洢��ʱ�����ϸ��ս���ʱ����ȷ��(�м������ڼ�Ҫ�洢һ�Σ��ʶ���11:36����ֹ��
        CString str;
        str = _T(" �洢��ʱ����");
        gl_systemMessage.PushInformationMessage(str);
        UpdateTempRTData();
      }
    }
  } // ÿ�����һ�ε�����
  else i5MinuteCounter -= lSecondNumber;

  // ����ÿ����һ�ε��������еĶ�ʱ����Ҫ����ʱ�����ӳ��������У�����ִ��ÿ����һ�ε�������ִ��ÿ����һ�ε����������ܹ���֤���������������ִ�С�
  if (i1MinuteCounter <= 0) {
    i1MinuteCounter = 59; // ���ü�����

    // �ŵ�ʮ��������ϵͳ
    // �����ڴ�ʱ������������������Ļ����׳������ݲ�ȫ�����⡣
    if (m_fPermitResetSystem) { // �����������ϵͳ
      if ((lTime >= 91300) && (lTime <= 91400) && ((gl_systemTime.GetDayOfWeek() > 0) && (gl_systemTime.GetDayOfWeek() < 6))) { // �����վŵ�ʮ�������ϵͳ
        gl_fResetSystem = true;     // ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
        m_fSystemReady = false;
      }
    }

    // �ŵ��ʮ����ٴ�����ϵͳ
    if (m_fPermitResetSystem) { // �����������ϵͳ
      if ((lTime >= 92500) && (lTime <= 93000) && ((gl_systemTime.GetDayOfWeek() > 0) && (gl_systemTime.GetDayOfWeek() < 6))) { // �����վŵ�ʮ�������ϵͳ
        gl_fResetSystem = true;     // ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
        m_fSystemReady = false;
        m_fPermitResetSystem = false; // ���첻����������ϵͳ��
      }
    }

    // �ж��й���Ʊ�г�����״̬
    if ((lTime < 91500) || (lTime > 150130) || ((lTime > 113500) && (lTime < 125500))) { //�������������ʮ���г����׽�����
      m_fMarketOpened = false;
    }
    else if ((gl_systemTime.GetDayOfWeek() == 0) || (gl_systemTime.GetDayOfWeek() == 6)) { //�����������ձ��С��ṹtm��0--6��ʾ��������������
      m_fMarketOpened = false;
    }
    else m_fMarketOpened = true;

    // �ڿ���ǰ��������ͣʱ��ѯ���й�Ʊ�أ��ҵ������Ծ��Ʊ��
    if (((lTime >= 91500) && (lTime < 92900)) || ((lTime >= 113100) && (lTime < 125900))) {
      m_fCheckTodayActiveStock = true;
    }
    else m_fCheckTodayActiveStock = false;

    // ��������һ�ֿ�ʼ������ʵʱ���ݡ�
    if ((lTime >= 150100) && !IsTodayStockCompiled()) {
      if (SystemReady()) {
        AfxBeginThread(ThreadCompileTodayStocks, nullptr);
        SetTodayStockCompiledFlag(true);
      }
    }

    // �ж��Ƿ��������������ʷ����
    if (IsTotalStockDayLineChecked() && !m_fUpdatedStockCodeDataBase) { // ������й�Ʊ�������Ҵ洢���߽����ݿ���߳��Ѿ����н���
      if (!gl_ThreadStatus.IsSavingDayLineInProcess()) { // ��������������ݿ��̲߳��ǻ�Ծ״̬����ֹͣ�������ݲ�ѯ��
        // �����������ݿ��̴߳��ڻ�Ծ��ʱ����������û�д洢������ֹͣ��ѯ���̣���ѯ�����ܹ���������̣߳�
        m_fUpdatedStockCodeDataBase = true;
        TRACE("������ʷ���ݸ������\n");
        CString str;
        str = _T("������ʷ���ݸ������");
        gl_systemMessage.PushInformationMessage(str);
        SaveStockCodeDataBase();  // ���¹�Ʊ�����ݿ�
        ASSERT(m_setSavingDayLineOnly.IsOpen());
        m_setSavingDayLineOnly.Close(); // �ر�������ʷ���ݴ洢��¼��
        m_fGetDayLineData = false; // ������Ҫ���µ��������϶����¹��ˣ�����ִ�и���������������
      }
    }
    else {
      if (!gl_ThreadStatus.IsSavingDayLineInProcess() && m_fGetDayLineData) {
        gl_ThreadStatus.SetSavingDayLineInProcess(true);
        AfxBeginThread(ThreadSavingDayLineProc, nullptr);
      }
    }
  } // ÿһ����һ�ε�����
  else i1MinuteCounter -= lSecondNumber;

  // ����ÿʮ����һ�ε�����
  if (i10SecondsCounter <= 0) {
    i10SecondsCounter = 9;
    // do something
  } // ÿʮ����һ�ε�����
  else i10SecondsCounter -= lSecondNumber;

  // ����ʵʱ���ݣ�ÿ����һ�Ρ�Ŀǰ����ʵʱ����Ϊÿ3����һ�θ��£��ʶ������ٿ��ˡ�
  if (SystemReady() && !gl_ThreadStatus.IsSavingTempData()) { // ��ϵͳ�洢��ʱ����ʱ����ͬʱ����ʵʱ���ݣ��������׳���ͬ�����⡣
    if (gl_ThreadStatus.IsRTDataNeedCalculate()) {
      gl_ThreadStatus.SetCalculatingRTData(true);
      AfxBeginThread(ThreadCalculatingRTDataProc, nullptr);
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// �õ���Ʊ�ļ��
//
//////////////////////////////////////////////////////////////////////////////////////////////
CString CMarket::GetStockName(CString strStockCode) {
  if (m_mapActiveStockToIndex.find(strStockCode) != m_mapActiveStockToIndex.end()) {
    long lIndex = m_mapActiveStockToIndex.at(strStockCode);
    return (m_vActiveStock.at(lIndex)->GetStockName());
  }
  else return _T("");
}

////////////////////////////////////////////////////////////////////////////////
//
//	ͨ����Ʊ����õ���Ʊ��������
//	���û�ҵ��Ļ�����ֵΪ�١�
//
//
////////////////////////////////////////////////////////////////////////////////
bool CMarket::GetStockIndex(CString strStockCode, long& lIndex) {
  if (m_mapActiveStockToIndex.find(strStockCode) == m_mapActiveStockToIndex.end()) {
    lIndex = -1;
    return false;
  }
  else lIndex = m_mapActiveStockToIndex.at(strStockCode);
  return true;
}

////////////////////////////////////////////////////////////////////////////////
//
//	ͨ����Ʊ����õ���Ʊ��ָ�롣
//	���û�ҵ��Ļ����ؿ�ָ��
//
//
////////////////////////////////////////////////////////////////////////////////
CStockPtr CMarket::GetStockPtr(CString strStockCode) {
  long lIndex = -1;

  if (m_mapActiveStockToIndex.find(strStockCode) != m_mapActiveStockToIndex.end()) {
    lIndex = m_mapActiveStockToIndex[strStockCode];
    return (m_vActiveStock.at(lIndex));
  }
  else return nullptr;
}

CStockPtr CMarket::GetStockPtr(long lIndex) {
  ASSERT((lIndex >= 0) && (lIndex < m_lTotalActiveStock));
  return m_vActiveStock.at(lIndex);
}

void CMarket::AddStockToMarket(CStockPtr pStock) {
  m_mapActiveStockToIndex[pStock->GetStockCode()] = m_lTotalActiveStock++;
  m_vActiveStock.push_back(pStock);
}

bool CMarket::GetStockIDPtr(CString strStockCode, StockIDPtr& pStockIDPtr)
{
  long lIndex = 0;
  if (m_mapChinaMarketAStock.find(strStockCode) == m_mapChinaMarketAStock.end()) { // δ�ҵ�
    pStockIDPtr = nullptr;
    return false;
  }
  lIndex = m_mapChinaMarketAStock.at(strStockCode);
  pStockIDPtr = m_vChinaMarketAStock.at(lIndex);
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
// ���õ�ǰ�����Ĺ�Ʊ
//
// ������Ӧ�Ĺ�Ʊָ�룬װ�����������ݡ�
//
/////////////////////////////////////////////////////////////////////////
void CMarket::SetShowStock(CStockPtr pStock)
{
  if (m_pCurrentStock != pStock) {
    m_pCurrentStock = pStock;
    m_fCurrentStockChanged = true;
    m_pCurrentStock->SetDayLineLoaded(false);
    AfxBeginThread(ThreadLoadDayLineProc, 0);
  }
}

bool CMarket::IsCurrentStockChanged(void) {
  if (m_fCurrentStockChanged) {
    m_fCurrentStockChanged = false;
    return true;
  }
  else return false;
}

//////////////////////////////////////////////////////////////////////////////////////
//
//	ͨ����Ʊ������г��������õ�ǰѡ���Ʊ
//
//////////////////////////////////////////////////////////////////////////////////////
void CMarket::SetShowStock(CString strStockCode) {
  m_pCurrentStock = GetStockPtr(strStockCode);
  ASSERT(m_pCurrentStock != NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	�����ߴ������ݿ⣮Ĭ�����ݿ�Ϊ�ա�
//  �˺����������̵߳��ã���Ҫע������ͬ�����⡣
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SaveDayLine(CSetDayLine* psetDayLine, CSetStockCode* psetStockCode, CStockPtr pStock, vector<CDayLinePtr>& vectorDayLine, bool fReversed) {
  long lIndex = 0;
  psetDayLine->m_pDatabase->BeginTrans();
  if (fReversed) {
    for (int i = vectorDayLine.size() - 1; i >= 0; i--) { // �����ǵ���洢�ģ���Ҫ��β����ʼ�洢
      auto pDayLine = vectorDayLine.at(i);
      lIndex = m_mapChinaMarketAStock.at(pStock->GetStockCode());
      if (m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay() >= pDayLine->GetDay()) continue; // �洢�����������ݲ��ô洢
      SaveOneRecord(psetDayLine, pDayLine);
    }
  }
  else {
    for (int i = 0; i < vectorDayLine.size(); i++) { // ����������洢�ģ���Ҫ��ͷ����ʼ�洢
      auto pDayLine = vectorDayLine.at(i);
      lIndex = m_mapChinaMarketAStock.at(pStock->GetStockCode());
      if (m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay() >= pDayLine->GetDay()) continue; // �洢�����������ݲ��ô洢
      SaveOneRecord(psetDayLine, pDayLine);
    }
  }
  psetDayLine->m_pDatabase->CommitTrans();

  // ���������������ں���ʼ��������
  bool fSaved = false;
  if (fReversed) {
    if (m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay() < vectorDayLine.at(0)->GetDay()) {
      m_vChinaMarketAStock.at(lIndex)->SetDayLineStartDay(vectorDayLine.at(0)->GetDay());
      m_vChinaMarketAStock.at(lIndex)->SetDayLineEndDay(vectorDayLine.at(vectorDayLine.size() - 1)->GetDay());
      fSaved = true;
    }
  }
  else {
    if (m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay() < vectorDayLine.at(vectorDayLine.size() - 1)->GetDay()) {
      m_vChinaMarketAStock.at(lIndex)->SetDayLineStartDay(vectorDayLine.at(0)->GetDay());
      m_vChinaMarketAStock.at(lIndex)->SetDayLineEndDay(vectorDayLine.at(vectorDayLine.size() - 1)->GetDay());
      fSaved = true;
    }
  }

  if (fSaved) {
    psetStockCode->m_pDatabase->BeginTrans();
    psetStockCode->AddNew();
    psetStockCode->m_Counter = m_vChinaMarketAStock.at(lIndex)->GetIndex();
    psetStockCode->m_StockType = m_vChinaMarketAStock.at(lIndex)->GetMarket();
    psetStockCode->m_StockCode = m_vChinaMarketAStock.at(lIndex)->GetStockCode();
    psetStockCode->m_StockName = m_vChinaMarketAStock.at(lIndex)->GetStockName();
    psetStockCode->m_DayLineStartDay = m_vChinaMarketAStock.at(lIndex)->GetDayLineStartDay();
    psetStockCode->m_DayLineEndDay = m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay();
    psetStockCode->m_IPOed = m_vChinaMarketAStock.at(lIndex)->GetIPOStatus();
    psetStockCode->Update();
    psetStockCode->m_pDatabase->CommitTrans();
  }

  return true;
}

bool CMarket::SaveOneRecord(CSetDayLine* psetDayLine, CDayLinePtr pDayLine) {
  psetDayLine->AddNew();
  psetDayLine->m_Time = pDayLine->GetDay();
  psetDayLine->m_Market = pDayLine->GetMarket();
  psetDayLine->m_StockCode = pDayLine->GetStockCode();
  psetDayLine->m_StockName = pDayLine->GetStockName();
  psetDayLine->m_LastClose = (double)pDayLine->GetLastClose() / 1000;
  psetDayLine->m_High = (double)pDayLine->GetHigh() / 1000;
  psetDayLine->m_Low = (double)pDayLine->GetLow() / 1000;
  psetDayLine->m_Open = (double)pDayLine->GetOpen() / 1000;
  psetDayLine->m_Close = (double)pDayLine->GetClose() / 1000;
  psetDayLine->m_Volume = pDayLine->GetVolume();
  psetDayLine->m_Amount = pDayLine->GetAmount();
  psetDayLine->m_UpAndDown = pDayLine->GetUpDown();
  psetDayLine->m_UpDownRate = pDayLine->GetUpDownRate();
  psetDayLine->m_ChangeHandRate = pDayLine->GetChangeHandRate();
  psetDayLine->m_TotalValue = pDayLine->GetTotalValue();
  psetDayLine->m_CurrentValue = pDayLine->GetCurrentValue();

  psetDayLine->m_RelativeStrong = pDayLine->GetRelativeStrong();

  /*
  psetDayLine->m_TransactionNumber = pDayLine->GetTransactionNumber();
  psetDayLine->m_TransactionNumberBelow5000 = pDayLine->GetTransactionNumberBelow5000();
  psetDayLine->m_TransactionNumberBelow50000 = pDayLine->GetTransactionNumberBelow50000();
  psetDayLine->m_TransactionNumberBelow200000 = pDayLine->GetTransactionNumberBelow200000();
  psetDayLine->m_TransactionNumberAbove200000 = pDayLine->GetTransactionNumberAbove200000();

  psetDayLine->m_OrdinaryBuyVolume = pDayLine->GetOrdinaryBuyVolume();
  psetDayLine->m_OrdinarySellVolume = pDayLine->GetOrdinarySellVolume();
  psetDayLine->m_AttackBuyVolume = pDayLine->GetAttackBuyVolume();
  psetDayLine->m_AttackSellVolume = pDayLine->GetAttackSellVolume();
  psetDayLine->m_StrongBuyVolume = pDayLine->GetStrongBuyVolume();
  psetDayLine->m_StrongSellVolume = pDayLine->GetStrongSellVolume();
  psetDayLine->m_UnknownVolume = pDayLine->GetUnknownVolume();
  psetDayLine->m_CancelBuyVolume = pDayLine->GetCancelBuyVolume();
  psetDayLine->m_CancelSellVolume = pDayLine->GetCancelSellVolume();

  psetDayLine->m_AttackBuyBelow50000 = pDayLine->GetAttackBuyBelow50000();
  psetDayLine->m_AttackBuyBelow200000 = pDayLine->GetAttackBuyBelow200000();
  psetDayLine->m_AttackBuyAbove200000 = pDayLine->GetAttackBuyAbove200000();
  psetDayLine->m_AttackSellBelow50000 = pDayLine->GetAttackSellBelow50000();
  psetDayLine->m_AttackSellBelow200000 = pDayLine->GetAttackSellBelow200000();
  psetDayLine->m_AttackSellAbove200000 = pDayLine->GetAttackSellAbove200000();
  */
  psetDayLine->Update();

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	���������ݴ������ݿ⣮
//  �˺����ɹ����߳�ThreadDayLineSaveProc���ã�������Ҫʹ��ȫ�ֱ�����
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SaveDayLineData(void) {
  CSetStockCode setStockCode;

  setStockCode.Open();
  long lIndex = 0;

  for (auto pStock : m_vActiveStock) {
    if (pStock->IsDayLineNeedSaving()) {
      lIndex = m_mapChinaMarketAStock.at(pStock->GetStockCode());
      if (pStock->m_vDayLine.size() > 0) { // �¹ɵ�һ������ʱ������ֻ�洢���ڽ�����������ݣ������������ǿյģ��ʶ���Ҫ�ж�һ��
        SaveDayLine(&m_setSavingDayLineOnly, &setStockCode, pStock, pStock->m_vDayLine, false);
      }
      else {
        CString str1 = m_vChinaMarketAStock.at(lIndex)->GetStockCode();
        str1 += _T(" �¹�����,û����������");
        gl_systemMessage.PushDayLineInfoMessage(str1);
      }
      pStock->m_vDayLine.clear();
      pStock->SetDayLineLoaded(false);
      CString str = m_vChinaMarketAStock.at(lIndex)->GetStockCode();
      str += _T("�������ϴ洢���");
      gl_systemMessage.PushDayLineInfoMessage(str);
      pStock->SetDayLineNeedSavingFlag(false);
    }
    if (gl_fExiting) {
      break; // ������������˳�����ֹͣ�洢��
    }
  }

  setStockCode.Close();

  return(true);
}

bool CMarket::ClearAllDayLineVector(void)
{
  for (auto pStock : m_vActiveStock) {
    pStock->m_vDayLine.clear();
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	��ʵʱ���ݴ������ݿ⣮Ĭ�����ݿ�Ϊ�ա�
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SaveRTData(void) {
  CSetRealTimeData setRTData;
  setRTData.m_strFilter = _T("[ID] = 1");

  setRTData.Open();
  setRTData.m_pDatabase->BeginTrans();
  for (auto pStock : m_vActiveStock) {
    if (pStock != nullptr) {
      if (pStock->IsActive()) {
        pStock->SaveRealTimeData(&setRTData);
      }
    }
  }
  setRTData.m_pDatabase->CommitTrans();
  setRTData.Close();
  return(true);
}

bool CMarket::IsTotalStockDayLineChecked(void) {
  for (auto pStockID : m_vChinaMarketAStock) {
    if (pStockID->IsDayLineNeedUpdate()) return false;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////////
//
// ������ս��յ���ʵʱ���ݣ��������߸��������ݣ����ǿ�ȡ������������̣���
//
// ֻ��������������ʷ���ݺ󣬷���ִ�д���ʵʱ���ݣ�����������й�Ʊ����������
//
//
//////////////////////////////////////////////////////////////////////////////////
long CMarket::CompileCurrentTradeDayStocks(long lCurrentTradeDay) {
  long lClose = 0, lLastClose = 0;
  char buffer[20];
  CString strDay;
  CSetDayLine setDayKLine;
  CSetDayLineInfo setDayLineInfo;
  long lIndex = 0;
  long iCount = 0;

  CString str;
  str = _T("��ʼ�������½����յ�ʵʱ����");
  gl_systemMessage.PushInformationMessage(str);

  // �洢��ǰ�����յ�����
  _ltoa_s(lCurrentTradeDay, buffer, 10);
  strDay = buffer;
  setDayKLine.m_strFilter = _T("[Time] =");
  setDayKLine.m_strFilter += strDay;
  setDayKLine.Open();
  setDayKLine.m_pDatabase->BeginTrans();
  while (!setDayKLine.IsEOF()) {
    setDayKLine.Delete();
    setDayKLine.MoveNext();
  }
  setDayKLine.m_pDatabase->CommitTrans();

  setDayKLine.m_pDatabase->BeginTrans();
  for (auto pStock : m_vActiveStock) {
    if (pStock == nullptr) {
      gl_systemMessage.PushInformationMessage(_T("��ǰ��Ծ��Ʊ�д���nullptr"));
      continue; // ����λ�á�Ӧ�ò����ڡ�
    }
    if ((pStock->GetHigh() == 0) && (pStock->GetLow() == 0) && (pStock->GetAmount() == 0)
      && (pStock->GetVolume() == 0) && (pStock->GetNew() == 0)) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
      continue;
    }
    iCount++;
    lIndex = m_mapChinaMarketAStock.at(pStock->GetStockCode());
    m_vChinaMarketAStock.at(lIndex)->SetDayLineEndDay(lCurrentTradeDay);
    m_vChinaMarketAStock.at(lIndex)->SetIPOStatus(__STOCK_IPOED__); // ������һ�Ρ���ֹ�¹ɹ�Ʊ��������û����ʷ���ݶ�������Ϊ�����ڡ�
    setDayKLine.AddNew();
    setDayKLine.m_Time = lCurrentTradeDay;
    setDayKLine.m_Market = pStock->GetMarket();
    setDayKLine.m_StockName = pStock->GetStockName();
    setDayKLine.m_StockCode = pStock->GetStockCode();
    lLastClose = pStock->GetLastClose();
    setDayKLine.m_LastClose = static_cast<double>(pStock->GetLastClose()) / 1000;
    setDayKLine.m_Open = static_cast<double>(pStock->GetOpen()) / 1000;
    setDayKLine.m_High = static_cast<double>(pStock->GetHigh()) / 1000;
    setDayKLine.m_Low = static_cast<double>(pStock->GetLow()) / 1000;
    lClose = pStock->GetNew();
    setDayKLine.m_Close = static_cast<double>(pStock->GetNew()) / 1000;
    setDayKLine.m_UpAndDown = static_cast<double>(lClose - lLastClose) / 1000;
    if (lLastClose == 0) { // �����е�һ��Ĺ�Ʊ
      setDayKLine.m_UpDownRate = 0;
    }
    else {
      setDayKLine.m_UpDownRate = (static_cast<double>(lClose - lLastClose)) * 100.0 / lLastClose;
    }

    setDayKLine.m_Volume = pStock->GetVolume();
    setDayKLine.m_Amount = pStock->GetAmount();
    setDayKLine.m_TotalValue = 0;
    setDayKLine.m_CurrentValue = 0;
    setDayKLine.m_RelativeStrong = pStock->GetRelativeStrong();
    /*
    setDayKLine.m_TransactionNumber = pStock->GetTransactionNumber();
    setDayKLine.m_TransactionNumberBelow5000 = pStock->GetTransactionNumberBelow5000();
    setDayKLine.m_TransactionNumberBelow50000 = pStock->GetTransactionNumberBelow50000();
    setDayKLine.m_TransactionNumberBelow200000 = pStock->GetTransactionNumberBelow200000();
    setDayKLine.m_TransactionNumberAbove200000 = pStock->GetTransactionNumberAbove200000();
    setDayKLine.m_CancelBuyVolume = pStock->GetCancelBuyVolume();
    setDayKLine.m_CancelSellVolume = pStock->GetCancelSellVolume();
    setDayKLine.m_AttackBuyVolume = pStock->GetAttackBuyVolume();
    setDayKLine.m_AttackSellVolume = pStock->GetAttackSellVolume();
    setDayKLine.m_StrongBuyVolume = pStock->GetStrongBuyVolume();
    setDayKLine.m_StrongSellVolume = pStock->GetStrongSellVolume();
    setDayKLine.m_UnknownVolume = pStock->GetUnknownVolume();
    setDayKLine.m_OrdinaryBuyVolume = pStock->GetOrdinaryBuyVolume();
    setDayKLine.m_OrdinarySellVolume = pStock->GetOrdinarySellVolume();
    setDayKLine.m_AttackBuyBelow50000 = pStock->GetAttackBuyBelow50000();
    setDayKLine.m_AttackBuyBelow200000 = pStock->GetAttackBuyBelow200000();
    setDayKLine.m_AttackBuyAbove200000 = pStock->GetAttackBuyAbove200000();
    setDayKLine.m_AttackSellBelow50000 = pStock->GetAttackSellBelow50000();
    setDayKLine.m_AttackSellBelow200000 = pStock->GetAttackSellBelow200000();
    setDayKLine.m_AttackSellAbove200000 = pStock->GetAttackSellAbove200000();
    */
    setDayKLine.Update();
  }
  setDayKLine.m_pDatabase->CommitTrans();
  setDayKLine.Close();

  // �洢�������ɵ�������DayLineInfo���С�
  setDayLineInfo.m_strFilter = _T("[Time] =");
  setDayLineInfo.m_strFilter += strDay;
  setDayLineInfo.Open();
  setDayLineInfo.m_pDatabase->BeginTrans();
  while (!setDayLineInfo.IsEOF()) {
    setDayLineInfo.Delete();
    setDayLineInfo.MoveNext();
  }
  setDayLineInfo.m_pDatabase->CommitTrans();

  setDayLineInfo.m_pDatabase->BeginTrans();
  for (auto pStock : m_vActiveStock) {
    if ((pStock->GetHigh() == 0) && (pStock->GetLow() == 0) && (pStock->GetAmount() == 0)
      && (pStock->GetVolume() == 0) && (pStock->GetNew() == 0)) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
      continue;
    }
    setDayLineInfo.AddNew();
    setDayLineInfo.m_Day = lCurrentTradeDay;
    setDayLineInfo.m_StockCode = pStock->GetStockCode();

    setDayLineInfo.m_TransactionNumber = pStock->GetTransactionNumber();
    setDayLineInfo.m_TransactionNumberBelow5000 = pStock->GetTransactionNumberBelow5000();
    setDayLineInfo.m_TransactionNumberBelow50000 = pStock->GetTransactionNumberBelow50000();
    setDayLineInfo.m_TransactionNumberBelow200000 = pStock->GetTransactionNumberBelow200000();
    setDayLineInfo.m_TransactionNumberAbove200000 = pStock->GetTransactionNumberAbove200000();
    setDayLineInfo.m_CancelBuyVolume = pStock->GetCancelBuyVolume();
    setDayLineInfo.m_CancelSellVolume = pStock->GetCancelSellVolume();
    setDayLineInfo.m_AttackBuyVolume = pStock->GetAttackBuyVolume();
    setDayLineInfo.m_AttackSellVolume = pStock->GetAttackSellVolume();
    setDayLineInfo.m_StrongBuyVolume = pStock->GetStrongBuyVolume();
    setDayLineInfo.m_StrongSellVolume = pStock->GetStrongSellVolume();
    setDayLineInfo.m_UnknownVolume = pStock->GetUnknownVolume();
    setDayLineInfo.m_OrdinaryBuyVolume = pStock->GetOrdinaryBuyVolume();
    setDayLineInfo.m_OrdinarySellVolume = pStock->GetOrdinarySellVolume();
    setDayLineInfo.m_AttackBuyBelow50000 = pStock->GetAttackBuyBelow50000();
    setDayLineInfo.m_AttackBuyBelow200000 = pStock->GetAttackBuyBelow200000();
    setDayLineInfo.m_AttackBuyAbove200000 = pStock->GetAttackBuyAbove200000();
    setDayLineInfo.m_AttackSellBelow50000 = pStock->GetAttackSellBelow50000();
    setDayLineInfo.m_AttackSellBelow200000 = pStock->GetAttackSellBelow200000();
    setDayLineInfo.m_AttackSellAbove200000 = pStock->GetAttackSellAbove200000();
    setDayLineInfo.Update();
  }
  setDayLineInfo.m_pDatabase->CommitTrans();
  setDayLineInfo.Close();

  gl_systemMessage.PushInformationMessage(_T("���½�����ʵʱ���ݴ������"));
  return iCount;
}

//////////////////////////////////////////////////////////////////////////////////
//
// �����մ���õ����ݴ��������ݿ��У��Ա���һϵͳ����ʱ����װ�롣
// �ƺ�Ӧ����һ����ʱ�����̵߳���ʽ���ڡ�
//
// �о�֮��
//
//////////////////////////////////////////////////////////////////////////////////
bool CMarket::SaveTodayTempData(void) {
  CSetDayLineToday setDayLineToday;

  // �洢�������ɵ�������DayLineToday���С�
  setDayLineToday.Open();
  setDayLineToday.m_pDatabase->BeginTrans();
  while (!setDayLineToday.IsEOF()) {
    setDayLineToday.Delete();
    setDayLineToday.MoveNext();
  }
  setDayLineToday.m_pDatabase->CommitTrans();
  setDayLineToday.m_pDatabase->BeginTrans();
  for (auto pStock : m_vActiveStock) {
    if ((pStock->GetHigh() == 0) && (pStock->GetLow() == 0) && (pStock->GetAmount() == 0)
      && (pStock->GetVolume() == 0) && (pStock->GetNew() == 0)) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
      continue;
    }
    ASSERT(pStock->GetVolume() == pStock->GetOrdinaryBuyVolume() + pStock->GetOrdinarySellVolume() + pStock->GetAttackBuyVolume()
      + pStock->GetAttackSellVolume() + pStock->GetStrongBuyVolume() + pStock->GetStrongSellVolume() + pStock->GetUnknownVolume());
    setDayLineToday.AddNew();
    setDayLineToday.m_Time = gl_systemTime.GetDay();
    setDayLineToday.m_Market = pStock->GetMarket();
    setDayLineToday.m_StockName = pStock->GetStockName();
    setDayLineToday.m_StockCode = pStock->GetStockCode();

    setDayLineToday.m_Volume = pStock->GetVolume();
    setDayLineToday.m_TransactionNumber = pStock->GetTransactionNumber();
    setDayLineToday.m_TransactionNumberBelow5000 = pStock->GetTransactionNumberBelow5000();
    setDayLineToday.m_TransactionNumberBelow50000 = pStock->GetTransactionNumberBelow50000();
    setDayLineToday.m_TransactionNumberBelow200000 = pStock->GetTransactionNumberBelow200000();
    setDayLineToday.m_TransactionNumberAbove200000 = pStock->GetTransactionNumberAbove200000();
    setDayLineToday.m_CancelBuyVolume = pStock->GetCancelBuyVolume();
    setDayLineToday.m_CancelSellVolume = pStock->GetCancelSellVolume();
    setDayLineToday.m_AttackBuyVolume = pStock->GetAttackBuyVolume();
    setDayLineToday.m_AttackSellVolume = pStock->GetAttackSellVolume();
    setDayLineToday.m_StrongBuyVolume = pStock->GetStrongBuyVolume();
    setDayLineToday.m_StrongSellVolume = pStock->GetStrongSellVolume();
    setDayLineToday.m_UnknownVolume = pStock->GetUnknownVolume();
    setDayLineToday.m_OrdinaryBuyVolume = pStock->GetOrdinaryBuyVolume();
    setDayLineToday.m_OrdinarySellVolume = pStock->GetOrdinarySellVolume();
    setDayLineToday.m_AttackBuyBelow50000 = pStock->GetAttackBuyBelow50000();
    setDayLineToday.m_AttackBuyBelow200000 = pStock->GetAttackBuyBelow200000();
    setDayLineToday.m_AttackBuyAbove200000 = pStock->GetAttackBuyAbove200000();
    setDayLineToday.m_AttackSellBelow50000 = pStock->GetAttackSellBelow50000();
    setDayLineToday.m_AttackSellBelow200000 = pStock->GetAttackSellBelow200000();
    setDayLineToday.m_AttackSellAbove200000 = pStock->GetAttackSellAbove200000();
    setDayLineToday.Update();
  }
  setDayLineToday.m_pDatabase->CommitTrans();
  setDayLineToday.Close();

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// �����ݴ�Ľ��յ�ǰ����״̬�����÷����ĳ�ʼ̬���������ڿ���ʱϵͳ�˳�ʱ����������ʧ�������ѷ���������
//
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::LoadTodayTempData(void) {
  CStockPtr pStock = nullptr;
  CSetDayLineToday setDayLineToday;
  CStockRTDataPtr pRTData;

  ASSERT(!m_fTodayTempDataLoaded);
  ASSERT(!gl_ThreadStatus.IsCalculatingRTData());    // ִ�д˳�ʼ������ʱ������ʵʱ���ݵĹ����̱߳���û�����С�
  // ��ȡ�������ɵ�������DayLineToday���С�
  setDayLineToday.Open();
  if (!setDayLineToday.IsEOF()) {
    if (setDayLineToday.m_Time == gl_systemTime.GetDay()) { // ����ǵ�������飬�����룬�������
      while (!setDayLineToday.IsEOF()) {
        if ((pStock = GetStockPtr(setDayLineToday.m_StockCode)) != nullptr) {
          // ��Ҫ����m_lUnknownVolume = pRTData->m_lVolume - setDayLineToday.m_Volume + setDayLineToday.m_UnknownVolume
          // ����һ��ִ�м���ʵʱ����ʱ��ֻ�ǳ�ʼ��ϵͳ��������������m_lUnknownVolume += pRTData->GetVolume
          // �ʶ��˴��������㡣
          ASSERT(!pStock->IsStartCalculating()); // ȷ��û�п�ʼ����ʵʱ����
          pStock->SetUnknownVolume(setDayLineToday.m_UnknownVolume - setDayLineToday.m_Volume);  // ��Ҫ�������m_lUnknownVolume

          pStock->SetTransactionNumber(setDayLineToday.m_TransactionNumber);
          pStock->SetTransactionNumberBelow5000(setDayLineToday.m_TransactionNumberBelow5000);
          pStock->SetTransactionNumberBelow50000(setDayLineToday.m_TransactionNumberBelow50000);
          pStock->SetTransactionNumberBelow200000(setDayLineToday.m_TransactionNumberBelow200000);
          pStock->SetTransactionNumberAbove200000(setDayLineToday.m_TransactionNumberAbove200000);
          pStock->SetCancelBuyVolume(setDayLineToday.m_CancelBuyVolume);
          pStock->SetCancelSellVolume(setDayLineToday.m_CancelSellVolume);
          pStock->SetAttackBuyVolume(setDayLineToday.m_AttackBuyVolume);
          pStock->SetAttackSellVolume(setDayLineToday.m_AttackSellVolume);
          pStock->SetStrongBuyVolume(setDayLineToday.m_StrongBuyVolume);
          pStock->SetStrongSellVolume(setDayLineToday.m_StrongSellVolume);
          pStock->SetOrdinaryBuyVolume(setDayLineToday.m_OrdinaryBuyVolume);
          pStock->SetOrdinarySellVolume(setDayLineToday.m_OrdinarySellVolume);
          pStock->SetAttackBuyBelow50000(setDayLineToday.m_AttackBuyBelow50000);
          pStock->SetAttackBuyBelow200000(setDayLineToday.m_AttackBuyBelow200000);
          pStock->SetAttackBuyAbove200000(setDayLineToday.m_AttackBuyAbove200000);
          pStock->SetAttackSellBelow50000(setDayLineToday.m_AttackSellBelow50000);
          pStock->SetAttackSellBelow200000(setDayLineToday.m_AttackSellBelow200000);
          pStock->SetAttackBuyAbove200000(setDayLineToday.m_AttackSellAbove200000);
        }
        setDayLineToday.MoveNext();
      }
    }
  }
  setDayLineToday.Close();

  return true;
}

///////////////////////////////////////////////////////////////////////////////////
//
// ����lDay���������ǿ��, lDay�ĸ�ʽΪ��YYYYMMDD,�� 19990605.
// �����߰��ǵ����к�,�����ǿ�������ڶ����е�λ��
//
//////////////////////////////////////////////////////////////////////////////////
bool CMarket::CalculateOneDayRelativeStrong(long lDay) {
  vector<StockIDPtr> vStockID;
  vector<int> vIndex;
  vector<double> vRelativeStrong;
  int iTotalAShare = 0;
  CString strSQL;
  CString strDay;
  char  pch[30];
  int iStockNumber = 0, j = 0;
  CTime ctTime;
  CSetDayLine setDayKLine;
  const long lYear = lDay / 10000;
  const long lMonth = lDay / 100 - lYear * 100;
  const long lDayOfMonth = lDay - lYear * 10000 - lMonth * 100;
  char buffer[100];

  for (j = 0; j < 30; j++) pch[j] = 0x000;

  _ltoa_s(lDay, pch, 10);
  strDay = pch;
  setDayKLine.m_strSort = _T("[UpDownRate]");
  setDayKLine.m_strFilter = _T("[Time] =");
  setDayKLine.m_strFilter += strDay;
  setDayKLine.Open();
  if (setDayKLine.IsEOF()) { // ���ݼ�Ϊ�գ���������û�н���
    setDayKLine.Close();
    return false;
  }
  setDayKLine.m_pDatabase->BeginTrans();
  iStockNumber = 0;
  while (!setDayKLine.IsEOF()) {
    if (gl_ChinaStockMarket.IsAStock(setDayKLine.m_StockCode)) {
      long lIndex = m_mapChinaMarketAStock.at(setDayKLine.m_StockCode);
      vStockID.push_back(m_vChinaMarketAStock.at(lIndex));
      vIndex.push_back(iStockNumber); // ��A�ɵ�������¼�������С�
      iTotalAShare++;
    }
    iStockNumber++;
    setDayKLine.MoveNext();
  }

  setDayKLine.MoveFirst();
  int iCount = 0;
  int iBefore = 0;
  while (iCount < vIndex.size()) { // ֻ�����Ծ��Ʊ�����ǿ��
    for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) { // ��������ȥ�������ݿ�,�������A�ɵĹ�Ʊ
      setDayKLine.MoveNext();
    }
    setDayKLine.Edit();
    if (((static_cast<double>(setDayKLine.m_Low) / setDayKLine.m_LastClose) < 0.88)
      || ((static_cast<double>(setDayKLine.m_High) / setDayKLine.m_LastClose) > 1.12)) { // ��Ȩ���¹����е�
      setDayKLine.m_RelativeStrong = 50; // �¹����л��߳�Ȩ��Ϣ��������˹�
    }
    else if ((fabs(setDayKLine.m_High - setDayKLine.m_Close) < 0.0001)
      && ((static_cast<double>(setDayKLine.m_Close) / setDayKLine.m_LastClose) > 1.095)) { // ��ͣ��
      setDayKLine.m_RelativeStrong = 100;
    }
    else if ((fabs(setDayKLine.m_Close - setDayKLine.m_Low) < 0.0001)
      && ((static_cast<double>(setDayKLine.m_Close) / setDayKLine.m_LastClose) < 0.905)) { // ��ͣ��
      setDayKLine.m_RelativeStrong = 0;
    }
    else {
      setDayKLine.m_RelativeStrong = (static_cast<double>(iCount) * 100) / iTotalAShare;
    }
    setDayKLine.Update();
    iBefore = vIndex.at(iCount++);
    setDayKLine.MoveNext(); // �Ƶ���һ�����ݡ�
    iBefore++; // ������Ҳͬʱ��һ��
  }
  setDayKLine.m_pDatabase->CommitTrans();
  setDayKLine.Close();

  vStockID.clear();
  vIndex.clear();
  vRelativeStrong.clear();

  sprintf_s(buffer, "%4d��%2d��%2d�յĹ�Ʊ���ǿ�ȼ������", lYear, lMonth, lDayOfMonth);
  CString strTemp;
  strTemp = buffer;
  gl_systemMessage.PushDayLineInfoMessage(strTemp);    // ����ͬ�����Ʊ�����Ϣ

  return(true);
}

bool CMarket::SaveStockCodeDataBase(void)
{
  CSetStockCode setStockCode;

  setStockCode.Open();
  setStockCode.m_pDatabase->BeginTrans();
  while (!setStockCode.IsEOF()) {
    setStockCode.Delete();
    setStockCode.MoveNext();
  }
  setStockCode.m_pDatabase->CommitTrans();
  setStockCode.m_pDatabase->BeginTrans();
  for (auto pStockID : gl_ChinaStockMarket.m_vChinaMarketAStock) {
    setStockCode.AddNew();
    CString str;
    setStockCode.m_Counter = pStockID->GetIndex();
    setStockCode.m_StockType = pStockID->GetMarket();
    setStockCode.m_StockCode = pStockID->GetStockCode();
    if (pStockID->GetStockName() != _T("")) {   // ����˹�ƱID�����µ����֣�
      setStockCode.m_StockName = pStockID->GetStockName(); // ��洢�µ�����
    }
    if (pStockID->GetIPOStatus() == __STOCK_IPOED__) { // ����˹�Ʊ�ǻ�Ծ��Ʊ
      if (pStockID->GetDayLineEndDay() < (gl_systemTime.GetDay() - 100)) { // ����˹�Ʊ��������ʷ�����Ѿ�����һ�����ˣ������ô˹�Ʊ״̬Ϊ������
        setStockCode.m_IPOed = __STOCK_DELISTED__;
      }
      else {
        setStockCode.m_IPOed = pStockID->GetIPOStatus();
      }
    }
    else {
      setStockCode.m_IPOed = pStockID->GetIPOStatus();
    }
    setStockCode.m_DayLineStartDay = pStockID->GetDayLineStartDay();
    setStockCode.m_DayLineEndDay = pStockID->GetDayLineEndDay();
    setStockCode.Update();
  }
  setStockCode.m_pDatabase->CommitTrans();
  setStockCode.Close();

  return true;
}

void CMarket::LoadStockCodeDataBase(void)
{
  CSetStockCode setStockCode;

  setStockCode.Open();
  // װ���Ʊ�������ݿ�
  while (!setStockCode.IsEOF()) {
    long lIndex = 1;
    lIndex = m_mapChinaMarketAStock.at(setStockCode.m_StockCode);
    if (setStockCode.m_StockCode != _T("")) {
      m_vChinaMarketAStock.at(lIndex)->SetStockCode(setStockCode.m_StockCode);
    }
    if (setStockCode.m_StockName != _T("")) {
      CString str = setStockCode.m_StockName; // ��str�м����һ�£��Ϳ��Զ�ȡUniCode��ʽ��m_StockName�ˡ�
      m_vChinaMarketAStock.at(lIndex)->SetStockName(str);
    }
    if (setStockCode.m_IPOed != __STOCK_NOT_CHECKED__) { // ����˹�Ʊ�����Ѿ��������������ù�ƱĿǰ״̬���������á�
      m_vChinaMarketAStock.at(lIndex)->SetIPOStatus(setStockCode.m_IPOed);
    }
    m_vChinaMarketAStock.at(lIndex)->SetDayLineStartDay(setStockCode.m_DayLineStartDay);
    if (m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay() < setStockCode.m_DayLineEndDay) { // ��ʱһ����Ʊ���ж����¼������������Ϊ׼��
      m_vChinaMarketAStock.at(lIndex)->SetDayLineEndDay(setStockCode.m_DayLineEndDay);
    }
    // ���ٸ����������ݱ��ϸ�������Ҫ�µĹ�Ʊ���������еĹ�Ʊ����ѯһ�飬�Է�ֹ�����¹�Ʊ�����ϵĹ�Ʊ���»�Ծ������
    if (gl_systemTime.GetLastTradeDay() <= m_vChinaMarketAStock.at(lIndex)->GetDayLineEndDay()) { // ������������Ϊ���ջ�����һ�������յ����ݡ�
      m_vChinaMarketAStock.at(lIndex)->SetDayLineNeedUpdate(false); // �������ݲ���Ҫ����
    }
    if (setStockCode.m_IPOed == __STOCK_NULL__) { // ��Ч���벻�������������
      m_vChinaMarketAStock.at(lIndex)->SetDayLineNeedUpdate(false);
    }
    if (setStockCode.m_IPOed == __STOCK_DELISTED__) { // ���й�Ʊ���������������
      m_vChinaMarketAStock.at(lIndex)->SetDayLineNeedUpdate(false);
    }
    setStockCode.MoveNext();
  }
  setStockCode.Close();
}

///////////////////////////////////////////////////////////////////////////////////
//
// ����ѡ�����ݿ�
//
//
//
//////////////////////////////////////////////////////////////////////////////////
bool CMarket::UpdateOptionDataBase(void)
{
  CSetOption setOption;
  setOption.Open();
  setOption.m_pDatabase->BeginTrans();
  if (setOption.IsEOF()) {
    setOption.AddNew();
    setOption.m_RelativeStrongEndDay = gl_ChinaStockMarket.GetRelativeStrongEndDay();
    setOption.m_RalativeStrongStartDay = gl_ChinaStockMarket.GetRelativeStrongStartDay();
    setOption.m_LastLoginDay = gl_systemTime.GetDay();
    setOption.Update();
  }
  else {
    setOption.Edit();
    setOption.m_RelativeStrongEndDay = gl_ChinaStockMarket.GetRelativeStrongEndDay();
    setOption.m_RalativeStrongStartDay = gl_ChinaStockMarket.GetRelativeStrongStartDay();
    setOption.m_LastLoginDay = gl_systemTime.GetDay();
    setOption.Update();
  }
  setOption.m_pDatabase->CommitTrans();
  setOption.Close();
  return false;
}

void CMarket::LoadOptionDataBase(void)
{
  CSetOption setOption;
  setOption.Open();
  if (setOption.IsEOF()) {
    gl_ChinaStockMarket.SetRelativeStrongEndDay(19900101);
    gl_ChinaStockMarket.SetRelativeStrongEndDay(19900101);
    gl_ChinaStockMarket.SetLastLoginDay(19900101);
  }
  else {
    if (setOption.m_RelativeStrongEndDay == 0) {
      gl_ChinaStockMarket.SetRelativeStrongEndDay(19900101);
    }
    else {
      gl_ChinaStockMarket.SetRelativeStrongEndDay(setOption.m_RelativeStrongEndDay);
    }
    if (setOption.m_RalativeStrongStartDay == 0) {
      gl_ChinaStockMarket.SetRelativeStrongStartDay(19900101);
    }
    else {
      gl_ChinaStockMarket.SetRelativeStrongStartDay(setOption.m_RalativeStrongStartDay);
    }
    if (setOption.m_LastLoginDay == 0) {
      gl_ChinaStockMarket.SetLastLoginDay(19900101);
    }
    else {
      gl_ChinaStockMarket.SetLastLoginDay(setOption.m_LastLoginDay);
    }
  }

  // �ж��Ƿ���Ҫ��ȡ������ʷ����
  if (m_lLastLoginDay >= gl_systemTime.GetLastTradeDay()) m_fGetDayLineData = false;
  else m_fGetDayLineData = true;

  setOption.Close();
}

bool CMarket::UpdateTempRTData(void)
{
  if (!gl_ThreadStatus.IsSavingTempData()) {
    gl_ThreadStatus.SetSavingTempData(true);
    AfxBeginThread(ThreadSavingTempRTDataProc, nullptr);
  }

  return false;
}

bool CMarket::OpenSavingDayLineRecord(void)
{
  // ����������ʷ���ݵļ�¼����Զ���ڴ�״̬��Ϊ�˼���)
  if (!m_setSavingDayLineOnly.IsOpen()) {
    CString str = _T("[ID] = 1"); // ����������Ϊ����Index��
    m_setSavingDayLineOnly.m_strFilter = str; // �������ã����������е����ݶ��룬�˷�ʱ��
    m_setSavingDayLineOnly.Open(); // ��Զ�򿪣����ڴ洢���յ���������ʷ���ݡ�
  }

  return true;
}