//////////////////////////////////////////////////////////////////////////////////////////////////


//#include"stdafx.h"

#include"globedef.h"
#include"accessory.h"
#include"ClientThread.h"

#include"Market.h"

#include"SetDayLineInfo.h"
#include"SetOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMarket::CMarket ( void ) : CObject() {
  static int siInstance = 0;
  if (++siInstance > 1) {
    TRACE("�г�����ֻ�������һ��ʵ��\n");
    ASSERT(0);
  }
  Reset();
}

CMarket::~CMarket( ) {

   
}

void CMarket::Reset(void)
{
  m_mapActiveStockToIndex.clear();
  m_vActiveStock.clear();
  m_lTotalActiveStock = 0; // ��ʼʱ��Ʊ���еĹ�Ʊ����Ϊ��

  m_fLoadedSelectedStock = false;
  m_fMarketReady = false;    // �г���ʼ״̬Ϊδ���úá�
  m_fCurrentStockChanged = false;
  m_fCurrentEditStockChanged = false;

  m_pCurrentStock = nullptr;

  m_lTotalMarketBuy = m_lTotalMarketSell = 0;

  time_t ttime;
  time(&ttime);
  tm tm_;
  localtime_s(&tm_, &ttime);
  if (tm_.tm_hour > 15) { // �й���Ʊ�г��Ѿ�����
    m_fTodayStockCompiled = true; // ���к��ִ�б�ϵͳ������Ϊ�Ѿ���������չ�Ʊ�����ˡ�
  }
  else m_fTodayStockCompiled = false;

  m_lRelativeStrongEndDay = m_lRelativeStrongStartDay = 19900101;

  m_fResetm_ItStock = true;
  
  m_fCheckTodayActiveStock = true;  //��鵱�ջ�Ծ��Ʊ������Ϊ�档 
  
  m_fUpdatedStockCodeDataBase = false;

  m_fGetRTStockData = true;
  m_fGetDayLineData = true;
  m_fCountDownRT = true;      // ��ʼʱִ�����ٲ�ѯʵʱ���顣
  m_iCountDownDayLine = 2;    // 400ms��ʱ��200msÿ�Σ�
  m_iCountDownRT = 1;

  // ���ɹ�Ʊ�����
  CreateTotalStockContainer();


  CSetStockCode setStockCode;

  setStockCode.Open();
    // װ���Ʊ�������ݿ�
  while (!setStockCode.IsEOF()) {
    long lIndex = 1;
    lIndex = gl_mapTotalStockToIndex.at(setStockCode.m_StockCode);
    if (setStockCode.m_IPOed != __STOCK_NOT_CHECKED__) { // ����˹�Ʊ�����Ѿ��������������ù�ƱĿǰ״̬���������á�
      gl_vTotalStock.at(lIndex)->SetIPOStatus(setStockCode.m_IPOed);
    }
    gl_vTotalStock.at(lIndex)->SetDayLineStartDay(setStockCode.m_DayLineStartDay);
    if (gl_vTotalStock.at(lIndex)->GetDayLineEndDay() < setStockCode.m_DayLineEndDay) { // ��ʱһ����Ʊ���ж����¼������������Ϊ׼��
      gl_vTotalStock.at(lIndex)->SetDayLineEndDay(setStockCode.m_DayLineEndDay);
    }
    if (gl_vTotalStock.at(lIndex)->GetNewestDayLineDay() < setStockCode.m_NewestDayLineDay) { // ��ʱһ����Ʊ���ж����¼������������Ϊ׼��
      gl_vTotalStock.at(lIndex)->SetNewestDayLineDay(setStockCode.m_NewestDayLineDay);
    }
    // ���ٸ����������ݱ��ϸ�������Ҫ�µĹ�Ʊ���������еĹ�Ʊ����ѯһ�飬�Է�ֹ�����¹�Ʊ�����ϵĹ�Ʊ���»�Ծ������
    if (gl_systemTime.GetLastTradeDay() <= gl_vTotalStock.at(lIndex)->GetDayLineEndDay()) { // ������������Ϊ���ջ�����һ�������յ����ݡ�
      gl_vTotalStock.at(lIndex)->SetDayLineNeedUpdate(false); // �������ݲ���Ҫ����
    }
    if (setStockCode.m_IPOed == __STOCK_NULL__) { // ��Ч���벻�������������
      gl_vTotalStock.at(lIndex)->SetDayLineNeedUpdate(false);
    }
    if (setStockCode.m_IPOed == __STOCK_DELISTED__) { // ���й�Ʊ���������������
      gl_vTotalStock.at(lIndex)->SetDayLineNeedUpdate(false);
    }
    setStockCode.MoveNext();
  }
  setStockCode.Close();

  // ����������ʷ���ݵļ�¼����Զ���ڴ�״̬��Ϊ�˼���)
  CString str = _T("[ID] = 1"); // ����������Ϊ����Index��
  gl_setSavingDayLineOnly.m_strFilter = str; // �������ã����������е����ݶ��룬�˷�ʱ��
  if (!gl_setSavingDayLineOnly.IsOpen()) gl_setSavingDayLineOnly.Open(); // ��Զ�򿪣����ڴ洢���յ���������ʷ���ݡ�


  CSetOption setOption;
  setOption.Open();
  if (setOption.IsEOF()) {
    gl_ChinaStockMarket.SetRelativeStrongEndDay(19900101);
    gl_ChinaStockMarket.SetRelativeStrongEndDay(19900101);
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
  }
  setOption.Close();

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
  char buffer[10];

  StockIDPtr pStockID = nullptr;
  int iCount = 0;

  // ���֮ǰ�����ݣ�����еĻ�����Resetʱ�������������оʹ������ݣ���
  gl_vTotalStock.clear();
  gl_mapTotalStockToIndex.clear();

  // �����Ϻ���Ʊ����
  for (int i = 600000; i < 602000; i++) {
    CString str = _T("sh");
    _itoa_s(i, buffer, 10);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(1); // �Ϻ��г�
    pStockID->SetIndex(iCount);
    gl_vTotalStock.push_back(pStockID);
    gl_mapTotalStockToIndex[pStockID->GetStockCode()] = iCount++; // ʹ���±������µ�ӳ��
  }

  ///////////////////////////////////////////////
  // �������ڹ�Ʊ����
  for (int i = 0; i < 3000; i++) {
    CString str = _T("sz");
    sprintf_s(buffer, 10, "%06d", i);
    pStockID = make_shared<CStockID>();
    pStockID->SetIndex(iCount);
    str += buffer;
    pStockID->SetStockCode(str);
    pStockID->SetMarket(2); // �����г�
    gl_vTotalStock.push_back(pStockID);
    gl_mapTotalStockToIndex[pStockID->GetStockCode()] = iCount++;// ʹ���±������µ�ӳ��
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���ɹ�Ʊ������ַ��������ڲ�ѯ�˹�Ʊ�ڵ�ǰ�г��Ƿ��ڻ�Ծ״̬�������Ƿ���ڴ˹�Ʊ���룩
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CMarket::CreateRTDataInquiringStr(CString& str) {
  static bool fCreateStr = false;
  static int siCounter = 0;

  const long siTotalStock = gl_vTotalStock.size();

  str = gl_vTotalStock.at(siCounter)->GetStockCode();
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
    str += gl_vTotalStock.at(siCounter)->GetStockCode();
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

  if (!gl_systemStatus.IsRTDataReadingInProcess()) {
    if (gl_systemStatus.IsRTDataReceived()) {
      if (gl_stRTDataInquire.fError == false) { //����ͨ��һ��˳����
        iTotalNumber = gl_stRTDataInquire.lByteRead;
        pCurrentPos = gl_stRTDataInquire.buffer;
        long  iCount = 0;
        while (iCount < iTotalNumber) { // ����ʵʱ���ݻ���û�д��󣬲���Ҫ�׵����һ�������ˡ�
          pRTData = make_shared<CStockRTData>();
          if (pRTData->ReadData(pCurrentPos, iCount)) {
            i++;
            gl_systemDequeData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
          }
          else {
            TRACE("ʵʱ��������,�׵�����\n");
            CString str = gl_systemTime.GetTimeString();
            str += _T("ʵʱ��������");
            gl_systemMessage.PushInformationMessage(str);
            iCount = iTotalNumber; // ��������ݿ��ܳ����⣬�׵����á�
          }
        }
        TRACE("����%d��ʵʱ����\n", i);
        // ������յ���ʵʱ����
        ProcessRTData();
      }
      else {  // ����ͨ�ų��ִ���
        TRACE("Error reading http file ��hq.sinajs.cn\n");
        CString str = gl_systemTime.GetTimeString();
        str += _T("Error reading http file ��hq.sinajs.cn");
        gl_systemMessage.PushInformationMessage(str);
      }
    }

    bool fFinished = false;
    CString strTemp = _T("");

    // ������һ���ι�Ʊʵʱ����
    if (m_fCheckTodayActiveStock) {
      gl_stRTDataInquire.strInquire = gl_strRTStockSource;
      fFinished = CreateRTDataInquiringStr(strTemp);
      gl_stRTDataInquire.strInquire += strTemp;
      if (fFinished) m_fCheckTodayActiveStock = false;
    }
    else {
      SetMarketReadyFlag(true); // ���еĹ�Ʊʵʱ���ݶ���ѯһ�飬���ջ�Ծ��Ʊ���Ѿ��������ʶ����Խ������������ˡ�
      static bool sfShow = true;
      if (sfShow) {
        gl_systemMessage.PushInformationMessage(_T("���ϵͳ��ʼ��"));
        sfShow = false;
      }
      gl_stRTDataInquire.strInquire = gl_strRTStockSource;
      GetInquiringStockStr(gl_stRTDataInquire.strInquire);
    }
    gl_systemStatus.SetRTDataReceived(false);
    gl_systemStatus.SetRTDataReadingInProcess(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
    AfxBeginThread(ClientThreadReadingRTDataProc, nullptr);
  }

  if (iCountUp == 1000) {
    m_fCheckTodayActiveStock = true;
    iCountUp = 0;
  }
  else iCountUp++;

  return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���ɹ�Ʊ������ַ��������ڲ�ѯ�˹�Ʊ�ڵ�ǰ�г��Ƿ��ڻ�Ծ״̬�������Ƿ���ڴ˹�Ʊ���룩
// 
//  �˺����Ǽ��gl_vTotalStock��Ʊ��
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::CreateDayLineInquiringStr(CString& str, CString& strStartDay) {
  static int iStarted = 0;
  static int siCounter = 0;
    
  long lTotalStock;

  StockIDPtr pStockID;

  lTotalStock = gl_vTotalStock.size();

  bool fFound = false;
  int iCount = 0;
  CString strTemp;
  while (!fFound && (iCount < 1000)) {
    if (!gl_vTotalStock.at(siCounter)->IsDayLineNeedUpdate()) { // �������ݲ���Ҫ���¡���ϵͳ��ʼʱ�����ô�m_fDayLineNeedUpdate��ʶ
      // TRACE("%S ���������������\n", static_cast<LPCWSTR>(gl_vTotalStock.at(siCounter)->m_strStockCode));
      siCounter++;
      iCount++;
      if (siCounter == lTotalStock) {
        siCounter = 0;
      }
    }
    else if (gl_vTotalStock.at(siCounter)->GetIPOStatus() == __STOCK_NULL__) {	// ��δʹ�ù��Ĺ�Ʊ���������ѯ��������
      gl_vTotalStock.at(siCounter)->SetDayLineNeedUpdate(false); // �˹�Ʊ�������ϲ���Ҫ�����ˡ�
      // TRACE("��Ч��Ʊ���룺%S, �����ѯ��������\n", static_cast<LPCWSTR>(gl_vTotalStock.at(siCounter)->m_strStockCode));
      iCount++;
      siCounter++;
      if (siCounter == lTotalStock) {
        siCounter = 0;
      }
    }
    else if (gl_vTotalStock.at(siCounter)->GetDayLineEndDay() >= gl_systemTime.GetLastTradeDay()) { // ��һ�����յ����������Ѿ��洢����ʱ�Ѿ������һ�����������ˣ������ٴδ���
      gl_vTotalStock.at(siCounter)->SetDayLineNeedUpdate(false); // �˹�Ʊ�������ϲ���Ҫ�����ˡ�
      // TRACE("%S �������ݱ����Ѹ���\n", static_cast<LPCWSTR>(gl_vTotalStock.at(siCounter)->m_strStockCode));
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
  StockIDPtr pID = gl_vTotalStock.at(siCounter);
  switch (pID->GetMarket()) { // ת����������������������ʽ���Ϻ�Ϊ��0��������Ϊ��1����
  case 1: // �Ϻ��г���
    str += '0'; // �Ϻ��г���ʶ
    break;
  case 2: // �����г���
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
  long lDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday; // ���ߵ���ʼ�������������ڵĺ�һ�졣
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

  if (!gl_systemStatus.IsReadingInProcess()) {
    if (sfFoundStock) {
      if ((gl_stDayLineInquire.fError == false) && gl_systemStatus.IsDayLineDataReady()) { //����ͨ��һ��˳����
        TRACE("��Ʊ%s��������Ϊ%d�ֽ�\n", GetDownLoadingStockCodeStr(), gl_stDayLineInquire.lByteRead);
        ASSERT(gl_stDayLineInquire.lByteRead < 2048 * 1024);
        // ����ǰ��Ʊ��������
        ProcessDayLineData(gl_stDayLineInquire.buffer, gl_stDayLineInquire.lByteRead);
      }
      else {
        if (gl_stDayLineInquire.lByteRead > 0) {
          TRACE("Error reading http file ��quotes.money.163.com/service/\n");
          CString str = gl_systemTime.GetTimeString();
          str += _T("Error reading http file ��quotes.money.163.com/service/");
          gl_systemMessage.PushInformationMessage(str);
          gl_stDayLineInquire.lByteRead = 0;
        }
      }
    }

    CString strTemp;
    char buffer2[200];
    CString strStartDay;

    // ׼�������������������ʽ
    strRead = gl_strDayLineStockSource;
    sfFoundStock = CreateDayLineInquiringStr(strRead, strStartDay);
    if (sfFoundStock) {
      strRead += _T("&start=");
      strRead += strStartDay;
      strRead += _T("&end=");
      sprintf_s(buffer2, "%8d", gl_systemTime.GetDay());
      strRead += buffer2;
      strRead += gl_strDayLinePostfix;

      gl_stDayLineInquire.strInquire = strRead;
      gl_systemStatus.SetDayLineDataReady(false);
      gl_systemStatus.SetReadingInProcess(true); // ���������һ��(�߳���Ҳ���ã����Է��߳����ڻ����ӳٵ����ٴν��루�߳��˳�ʱ������˱�ʶ��
      // ����̵߳��������Բ��û���ģʽ����������ֱ�ӵ���
      AfxBeginThread(ClientThreadReadDayLineProc, nullptr);
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
long CMarket::GetMinLineOffset( CStockID sID, time_t Time ) {
	ASSERT( Time >= 0 );
	tm tmTemp{};
	time_t t = 0;
	long lIndex = 0;

	localtime_s( &tmTemp, &Time );
	tmTemp.tm_hour = (9 - 8);			// time_t, tmʹ�õ��ǹ��ʱ�׼ʱ(UTC),�ʱ���ʱ��09��30��UTC��01��30��Ҫ��ȥ8Сʱ
	tmTemp.tm_min = 30;
	tmTemp.tm_sec = 0;
	t = mktime( &tmTemp );
	lIndex = (Time - t)/60;
	if ( lIndex < 0 ) lIndex = 0;
	if ( (lIndex >= 120) && (lIndex < 209) ) lIndex = 119;
	if ( lIndex >= 210 ) lIndex -= 90;
	if ( lIndex >= 240 ) lIndex = 239;
	
	ASSERT( (lIndex >= 0) && (lIndex < 240) );
	return( lIndex );
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

  if ((pStock->GetStockCode()[0] == 's') && (pStock->GetStockCode()[1] == 'h') && (pStock->GetStockCode()[2] == '6')) {
    return true;
  }
  else {
    if ((pStock->GetStockCode()[0] == 's') && (pStock->GetStockCode()[1] == 'z') && (pStock->GetStockCode()[2] == '0') && (pStock->GetStockCode()[3] == '0')) {
      return true;
    }
  }
  return(false);
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//		�ж�strStockCode�Ƿ�Ϊ����A�ɵĹ�Ʊ���롣
//		����A�ɴ�����6��ͷ������A�ɴ�����00��ͷ��
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::IsAStock(CString strStockCode) {

  if ((strStockCode[0] == 's') && (strStockCode[1] == 'h') && (strStockCode[2] == '6')) {
    return true;
  }
  else {
    if ((strStockCode[0] == 's') && (strStockCode[1] == 'z') && (strStockCode[2] == '0') && (strStockCode[3] == '0')) {
      return true;
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
bool CMarket::IsStock( CString strStockCode, CStockPtr & pStock ) {
	if ( (pStock = GetStockPtr( strStockCode )) != nullptr ) {
		return( true );
	}
	else {
		pStock = NULL;
		return( false );
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////
INT64 CMarket::GetTotalAttackBuyAmount( void ) {
	INT64 lAmount = 0;
	for ( auto pStock : m_vActiveStock ) {
		if ( IsAStock( pStock ) ) {
			lAmount += pStock->GetAttackBuyAmount();
		}
	}
	return( lAmount );
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////
INT64 CMarket::GetTotalAttackSellAmount( void ) {
	INT64 lAmount = 0;
	for ( auto pStock : m_vActiveStock ) {
		if ( IsAStock( pStock ) ) {
			lAmount += pStock->GetAttackSellAmount();
		}
	}
	return( lAmount );
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ���ݵȣ���CMainFrame��OnTimer�������á�
//
// �˺����õ�������ȫ�ֱ��������Ƿ������߳�Ϊ�á������߳�Ŀǰ����ֻ���������Ʊ�Ĺҵ������
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::ProcessRTData(void)
{
  // ��������ʵʱ���ݣ����ɵ��յĻ�Ծ��Ʊ�г�
  CStockPtr pStock;
	CStockRTDataPtr pRTDataCompact = nullptr;
  long lTotalNumber = gl_systemDequeData.GetRTDataDequeSize();

  for (int i = 0; i < lTotalNumber; i++) {
    CStockRTDataPtr pRTData = gl_systemDequeData.PopRTData();
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
				pStock->UpdataCurrentStatus(pRTData);
        AddStockToMarket(pStock); // ��Ӵ˹�������������������Ŀǰ��m_lTotalActiveStaock��ֵ��
        ASSERT(m_vActiveStock.size() == m_lTotalActiveStock);
				pStock->PushRTData(pRTData);
        pStock->UpdataCurrentStatus(pRTData);
				lIndex = gl_mapTotalStockToIndex[pStock->GetStockCode()];
				gl_vTotalStock.at(lIndex)->SetStockName(pStock->GetStockName());
				gl_vTotalStock.at(lIndex)->SetActive(true); // ���ս��յ������ݣ�
        gl_vTotalStock.at(lIndex)->SetDayLineNeedUpdate(true);
        // ����˹�Ʊ������δʹ�ù���������Ϊ��ʹ�á�
        // ͣ�ƺ�Ĺ�Ʊ��Ҳ�ܽ��յ�ʵʱ���ݣ�ֻ��������ֻ�����̼ۣ�������Ϊ�㡣�������������Ч���ݡ�
        gl_vTotalStock.at(lIndex)->SetIPOStatus(__STOCK_IPOED__);
				ASSERT(gl_vTotalStock.at(lIndex)->GetStockCode().Compare(pStock->GetStockCode()) == 0);
      }
      else {
        lIndex = m_mapActiveStockToIndex.at(pRTData->GetStockCode());
        ASSERT(lIndex <= m_lTotalActiveStock);
        if (pRTData->GetTime() > m_vActiveStock.at(lIndex)->GetTime()) { // �µ����ݣ�
          m_vActiveStock.at(lIndex)->UpdataCurrentStatus(pRTData);
          m_vActiveStock.at(lIndex)->PushRTData(pRTData); // �洢�µ����������ݳ�
        }
      }
    }
  }
  gl_systemStatus.SetRTDataNeedCalculate(true);

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
// �ɹ����߳�ClientThreadCalculatingRTDataProc���ã�ע��ȫ�ֱ�����ʹ��
//
//
/////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::CalculateRTData(void)
{
  for ( auto pStock : m_vActiveStock) {
    ASSERT(pStock != nullptr);
    pStock->CalculateRTData();
    if (gl_fExiting) return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// ������������߷������϶�ȡ�Ĺ�Ʊ�������ݡ�
// ������ʽΪ�� ����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::ProcessDayLineData(char * buffer, long lLength) {
	long iCount = 0;
	char * pCurrentPos = buffer;
	char * pTestPos = buffer;
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
		lIndex = gl_mapTotalStockToIndex.at(m_strCurrentStockDownLoading);
		// ASSERT(!gl_vTotalStock[lIndex]->m_fActive); ��һ����ƱIPO����δ����ʱ����Ʊ������ڵ�û���������ݡ�ȡ���˶����жϡ�
    // ��Щ��Ʊ�����к���ֱ��չ�����������������������ݲ��ٸ��¡���������������ô˹�ƱΪ��Ч����
    if (gl_vTotalStock.at(lIndex)->GetDayLineEndDay() == 19900101) { // �����ʼ���߽������ڴ���û�б��������˹�Ʊ������δ��ʹ�ù�
      gl_vTotalStock.at(lIndex)->SetIPOStatus(__STOCK_NULL__);   // �˹�Ʊ������δʹ�á�
      //TRACE("��Ч��Ʊ���룺%s\n", static_cast<LPCWSTR>(m_strCurrentStockDownLoading));
    }
    else { // �Ѿ����еĹ�Ʊ
      if (gl_vTotalStock.at(lIndex)->GetDayLineEndDay() + 100 < gl_systemTime.GetDay()) {
        gl_vTotalStock.at(lIndex)->SetIPOStatus(__STOCK_DELISTED__);   // �˹�Ʊ�����Ѿ����С�
      }
      //TRACE("%S û�пɸ��µ���������\n", static_cast<LPCWSTR>(gl_strCurrentStockDownLoading));
    }
    gl_vTotalStock.at(lIndex)->SetDayLineNeedUpdate(false); // ������Ҫ������������
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
      long lIndexTemp = gl_mapTotalStockToIndex.at(pStock->GetStockCode());
      gl_vTotalStock.at(lIndexTemp)->SetStockCode(pStock->GetStockCode());
      gl_vTotalStock.at(lIndexTemp)->SetStockName(pStock->GetStockName());
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
  gl_vTotalStock.at(gl_mapTotalStockToIndex.at(pDayLine->GetStockCode()))->SetDayLineNeedUpdate(false); // ��������������ϣ�����Ҫ�����������ˡ�
  if ((vTempDayLine.at(0)->GetDay() + 100) < gl_systemTime.GetDay()) { // ��ȡ���Ĺ�Ʊ���������������ղ����ϸ��µ���������գ������˻��������������һ���µ�ʱ��۲졣
    gl_vTotalStock.at(gl_mapTotalStockToIndex.at(pDayLine->GetStockCode()))->SetIPOStatus(__STOCK_DELISTED__); // �����л���ͣ���ס�
  }
  else {
    gl_vTotalStock.at(gl_mapTotalStockToIndex.at(pDayLine->GetStockCode()))->SetIPOStatus(__STOCK_IPOED__); // �������׹�Ʊ
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
bool CMarket::ProcessOneItemDayLineData(CDayLinePtr pDayLine, char *& pCurrentPos, long & lLength) {
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
		pDayLine->SetMarket(1);
	}
	else if (str == _T("sz")) {
		pDayLine->SetMarket(2);
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
	if(*pCurrentPos++ != 0x0a) return false; // ���ݳ�����������
	iCount++;

	lLength = iCount;

	return true;
}

bool CMarket::ReadOneValue(char *& pCurrentPos, char * buffer, long & iReadNumber)
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
bool CMarket::ReadOneValueExceptperiod(char *& pCurrentPos, char * buffer, long & lCounter)
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

bool CMarket::SchedulingTask(void)
{
  static time_t s_time = 0;

  gl_systemTime.CalculateTime();

  //����ʱ�䣬���ȸ�������.ÿ�����һ��
  if (gl_systemTime.Gett_time() > s_time) {
    SchedulingTaskPerSecond();
    s_time = gl_systemTime.Gett_time();
  }

  if (!gl_fExiting && m_fGetRTStockData && (m_iCountDownRT <= 0)) {
    GetSinaStockRTData(); // ÿ400����(200X2)����һ��ʵʱ���ݡ����˵�ʵʱ�����������Ӧʱ�䲻����100���루30-70֮�䣩����û�г��ֹ����ݴ���
    if (m_fCountDownRT && MarketReady()) m_iCountDownRT = 1000; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ��� 
    else m_iCountDownRT = 1;  // ��������
  }
  m_iCountDownRT--;

  if (!gl_fExiting && m_fGetDayLineData && MarketReady()) {// �������ץȡ����������ϵͳ��ʼ̬�Ѿ�����
    GetNetEaseStockDayLineData();
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ʱ���Ⱥ�����ÿ��һ��
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SchedulingTaskPerSecond(void)
{
  static int i10SecondsCounter = 10; // ʮ��һ�εļ���
  static int i1MinuteCounter = 60;  // һ����һ�εļ���
  const long lTime = gl_systemTime.GetTime();

  if (((lTime > 91000) && (lTime < 113500)) || ((lTime > 125500) && (lTime < 150500))) {
    m_fCountDownRT = false;// ֻ���г�����ʱ����ٶ�ȡʵʱ���ݣ�����ʱ�������ٶ�ȡ
  }
  else m_fCountDownRT = true;

  if ((lTime < 91000) || (lTime > 150001)) { //���������г����׽�����
    m_fMarketOpened = false;
  }
  else if ((gl_systemTime.GetDayOfWeek() == 0) || (gl_systemTime.GetDayOfWeek() == 6)) {
    m_fMarketOpened = false;
  }
  else m_fMarketOpened = true;

  if ((lTime > 150100) && !IsTodayStockCompiled() && m_fMarketOpened) { // ��������һ�ֿ�ʼ������ʵʱ���ݡ�
    if (MarketReady()) {
      AfxBeginThread(ClientThreadCompileTodayStocks, nullptr);
      SetTodayStockCompiledFlag(true);
    }
  }

  if (i10SecondsCounter >= 0) {
    i10SecondsCounter = 10;
    // do something
  }
  else i10SecondsCounter--;

  if (i1MinuteCounter <= 0) {
    //gl_fResetSystem = true; // ����ϵͳ
    i1MinuteCounter = 60;
    if (IsTotalStockDayLineChecked() && !m_fUpdatedStockCodeDataBase) { // ������й�Ʊ�������Ҵ洢���߽����ݿ���߳��Ѿ����н���
      if (!gl_systemStatus.IsDataBaseInProcess()) { // ��������������ݿ��̲߳��ǻ�Ծ״̬����ֹͣ�������ݲ�ѯ��
        // �����������ݿ��̴߳��ڻ�Ծ��ʱ����������û�д洢������ֹͣ��ѯ���̣���ѯ�����ܹ���������̣߳�
        m_fUpdatedStockCodeDataBase = true;
        TRACE("������ʷ���ݸ������\n");
        CString str;
        str = gl_systemTime.GetTimeString();
        str += _T("������ʷ���ݸ������");
        gl_systemMessage.PushInformationMessage(str);
        UpdateStockCodeDataBase();  // ���¹�Ʊ�����ݿ�
        ASSERT(gl_setSavingDayLineOnly.IsOpen());
        gl_setSavingDayLineOnly.Close(); // �ر�������ʷ���ݴ洢��¼��
        m_fGetDayLineData = false; // ������Ҫ���µ��������϶����¹��ˣ�����ִ�и���������������
      }
    }
    else {
      if (!gl_systemStatus.IsDataBaseInProcess() && m_fGetDayLineData) {
        gl_systemStatus.SetDataBaseInProcess(true);
        AfxBeginThread(ClientThreadSaveDayLineProc, nullptr);
      }
    }

  }
  else i1MinuteCounter--;

  return true;

}


//////////////////////////////////////////////////////////////////////////////////////////////
//
// �õ���Ʊ�ļ��
//
//////////////////////////////////////////////////////////////////////////////////////////////
CString CMarket::GetStockName( CString strStockCode ) {
	long lIndex = m_mapActiveStockToIndex[strStockCode];
	return ( m_vActiveStock.at(lIndex)->GetStockName());
}

////////////////////////////////////////////////////////////////////////////////
//
//	ͨ����Ʊ����õ���Ʊ��������
//	���û�ҵ��Ļ�����ֵΪ�١�
//
//
////////////////////////////////////////////////////////////////////////////////
bool CMarket::GetStockIndex(CString strStockCode, long & lIndex) {

  if (m_mapActiveStockToIndex.find(strStockCode) == m_mapActiveStockToIndex.end()) {
    return false;
  }
  else lIndex = m_mapActiveStockToIndex[strStockCode];

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

	if ( m_mapActiveStockToIndex.find(strStockCode) != m_mapActiveStockToIndex.end() ) {
    lIndex = m_mapActiveStockToIndex[strStockCode];
		return (m_vActiveStock.at(lIndex));
	}
	else return nullptr;
}

CStockPtr CMarket::GetStockPtr( long lIndex ) { 
	ASSERT( (lIndex >= 0) && (lIndex < m_lTotalActiveStock) );
	return m_vActiveStock.at(lIndex); 
}

void CMarket::AddStockToMarket(CStockPtr pStock) {
  m_mapActiveStockToIndex[pStock->GetStockCode()] = m_lTotalActiveStock++;
  m_vActiveStock.push_back(pStock);
}


bool CMarket::GetStockIDPtr(CString strStockCode, StockIDPtr& pStockIDPtr)
{
  long lIndex = 0;
  if (gl_mapTotalStockToIndex.find(strStockCode) == gl_mapTotalStockToIndex.end()) { // δ�ҵ�
    return false;
  }
  lIndex = gl_mapTotalStockToIndex.at(strStockCode);
  pStockIDPtr = gl_vTotalStock.at(lIndex);
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
    AfxBeginThread(ClientthreadLoadDayLineProc, 0);
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
void CMarket::SetShowStock( CString strStockCode ) {
  m_pCurrentStock = GetStockPtr( strStockCode );
	ASSERT(m_pCurrentStock != NULL );
}



/////////////////////////////////////////////////////////////////////////////////////////
//
//	�����ߴ������ݿ⣮Ĭ�����ݿ�Ϊ�ա�
//  �˺����������̵߳��ã���Ҫע������ͬ�����⡣
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SaveDayLine(CSetDayLine * psetDayLine, CSetStockCode * psetStockCode, CStockPtr pStock, vector<CDayLinePtr> & vectorDayLine, bool fReversed) {

  long lIndex = 0;
  psetDayLine->m_pDatabase->BeginTrans();
  if (fReversed) {
    for (int i = vectorDayLine.size() - 1; i >= 0; i--) { // �����ǵ���洢�ģ���Ҫ��β����ʼ�洢
      auto pDayLine = vectorDayLine.at(i);
      lIndex = gl_mapTotalStockToIndex.at(pStock->GetStockCode());
      if (gl_vTotalStock.at(lIndex)->GetDayLineEndDay() >= pDayLine->GetDay()) continue; // �洢�����������ݲ��ô洢
      SaveOneRecord(psetDayLine, pDayLine);
    }
  }
  else {
    for (int i = 0; i < vectorDayLine.size(); i++) { // �����ǵ���洢�ģ���Ҫ��β����ʼ�洢
      auto pDayLine = vectorDayLine.at(i);
      lIndex = gl_mapTotalStockToIndex.at(pStock->GetStockCode());
      if (gl_vTotalStock.at(lIndex)->GetDayLineEndDay() >= pDayLine->GetDay()) continue; // �洢�����������ݲ��ô洢
      SaveOneRecord(psetDayLine, pDayLine);
    }
  }
  psetDayLine->m_pDatabase->CommitTrans();

  // ���������������ں���ʼ��������
  bool fSave = false;
  if (fReversed) {
    gl_vTotalStock.at(lIndex)->SetDayLineStartDay(vectorDayLine.at(vectorDayLine.size()-1)->GetDay());
    if (gl_vTotalStock.at(lIndex)->GetDayLineEndDay() < vectorDayLine.at(0)->GetDay()) {
      gl_vTotalStock.at(lIndex)->SetDayLineEndDay(vectorDayLine.at(vectorDayLine.size() - 1)->GetDay());
      gl_vTotalStock.at(lIndex)->SetNewestDayLineDay(vectorDayLine.at(vectorDayLine.size() - 1)->GetDay());
      fSave = true;
    }
  }
  else {
    gl_vTotalStock.at(lIndex)->SetDayLineStartDay(vectorDayLine.at(0)->GetDay());
    if (gl_vTotalStock.at(lIndex)->GetDayLineEndDay() < vectorDayLine.at(vectorDayLine.size() - 1)->GetDay()) {
      gl_vTotalStock.at(lIndex)->SetDayLineEndDay(vectorDayLine.at(vectorDayLine.size() - 1)->GetDay());
      gl_vTotalStock.at(lIndex)->SetNewestDayLineDay(vectorDayLine.at(vectorDayLine.size() - 1)->GetDay());
      fSave = true;
    }
  }

  if (fSave) {
    psetStockCode->m_pDatabase->BeginTrans();
    psetStockCode->AddNew();
    psetStockCode->m_Counter = gl_vTotalStock.at(lIndex)->GetIndex();
    psetStockCode->m_StockType = gl_vTotalStock.at(lIndex)->GetMarket();
    psetStockCode->m_StockCode = gl_vTotalStock.at(lIndex)->GetStockCode();
    psetStockCode->m_StockName = gl_vTotalStock.at(lIndex)->GetStockName();
    psetStockCode->m_DayLineStartDay = gl_vTotalStock.at(lIndex)->GetDayLineStartDay();
    psetStockCode->m_DayLineEndDay = gl_vTotalStock.at(lIndex)->GetDayLineEndDay();
    psetStockCode->m_NewestDayLineDay = gl_vTotalStock.at(lIndex)->GetNewestDayLineDay();
    psetStockCode->m_IPOed = gl_vTotalStock.at(lIndex)->GetIPOStatus();
    psetStockCode->Update();
    psetStockCode->m_pDatabase->CommitTrans();
  }
  
  return true;
}

bool CMarket::SaveOneRecord(CSetDayLine * psetDayLine, CDayLinePtr pDayLine) {
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

  psetDayLine->Update();

  return true;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//	���������ݴ������ݿ⣮
//  �˺����ɹ����߳�ClientThreadDayLineSaveProc���ã�������Ҫʹ��ȫ�ֱ�����
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMarket::SaveDayLineData(void) {
  CSetStockCode setStockCode;
  CSetDayLine setSavingDayLine;

  setStockCode.Open();
  long lIndex = 0;

	for (auto pStock : m_vActiveStock) {
		if (pStock->IsDayLineNeedSaving()) {
 			lIndex = gl_mapTotalStockToIndex.at(pStock->GetStockCode());
      if (pStock->m_vDayLine.size() > 0) { // �¹ɵ�һ������ʱ������ֻ�洢���ڽ�����������ݣ������������ǿյģ��ʶ���Ҫ�ж�һ��
        SaveDayLine(&gl_setSavingDayLineOnly, &setStockCode, pStock, pStock->m_vDayLine, false);
      }
      else {
        CString str1 = gl_vTotalStock.at(lIndex)->GetStockCode();
        str1 += _T(" �¹�����,û����������");
        gl_systemMessage.PushDayLineInfoMessage(str1);

      }
      pStock->m_vDayLine.clear();
      pStock->SetDayLineLoaded(false);
      CString str = gl_vTotalStock.at(lIndex)->GetStockCode();
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
bool CMarket::SaveRTData( void ) {
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
  for (auto pStockID : gl_vTotalStock) {
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
bool CMarket::CompileCurrentTradeDayStocks(long lCurrentTradeDay) {
  long lClose = 0, lLastClose = 0;
  char buffer[20];
  CString strDay;
  CSetDayLine setDayKLine;
  CSetDayLineInfo setDayLineInfo;
  long lIndex = 0;

  CString str = gl_systemTime.GetTimeString();
  str += _T("��ʼ�������½����յ�ʵʱ����");
  gl_systemMessage.PushDayLineInfoMessage(str);

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
  for (auto pStock : m_vActiveStock ) {
    if (pStock == nullptr) continue; // ����λ�á�Ӧ�ò����ڡ�
    if ((pStock->GetHigh() == 0) && (pStock->GetLow() == 0) && (pStock->GetAmount() == 0)
      && (pStock->GetVolume() == 0) && (pStock->GetNew() == 0)) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
      continue;
    }
    lIndex = gl_mapTotalStockToIndex.at(pStock->GetStockCode());
    gl_vTotalStock.at(lIndex)->SetNewestDayLineDay(lCurrentTradeDay); // �������½��յ������������ڡ�
    if (gl_vTotalStock.at(lIndex)->GetDayLineEndDay() == gl_systemTime.GetLastTradeDay()) { // �������������ʷ�������ھ�����һ�������գ�����´�����
      // ���ж���ȱ�ݣ�������ƽ������Ҳ�ǽڼ���ʱ���Ͳ�������ˡ��о�֮��
      gl_vTotalStock.at(lIndex)->SetDayLineEndDay(lCurrentTradeDay);
    }
    gl_vTotalStock.at(lIndex)->SetIPOStatus(__STOCK_IPOED__); // ������һ�Ρ���ֹ�¹ɹ�Ʊ��������û����ʷ���ݶ�������Ϊ�����ڡ�
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
    setDayLineInfo.m_Time = lCurrentTradeDay;
    setDayLineInfo.m_Market = pStock->GetMarket();
    setDayLineInfo.m_StockName = pStock->GetStockName();
    setDayLineInfo.m_StockCode = pStock->GetStockCode();

    setDayLineInfo.m_RelativeStrong = pStock->GetRelativeStrong();
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

  gl_systemMessage.PushDayLineInfoMessage(_T("���½�����ʵʱ���ݴ������"));
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
  long lYear = lDay / 10000;
  long lMonth = lDay / 100 - lYear * 100;
  long lDayOfMonth = lDay - lYear * 10000 - lMonth * 100;
  char buffer[100];
  gl_systemStatus.SetCalculateRSInProcess(true);

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
      long lIndex = gl_mapTotalStockToIndex.at(setDayKLine.m_StockCode);
      vStockID.push_back(gl_vTotalStock.at(lIndex));
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
    if (gl_fExiting) break; // ���ݿ���ʱ�����ܷ�ʱ�䣬��ϵͳ�˳�ʱ���ݿ����Ҫ�����жϡ�
    for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) { // ��������ȥ�������ݿ⡣
      setDayKLine.MoveNext();
    }
    setDayKLine.Edit();
    if ((((double)setDayKLine.m_Low / setDayKLine.m_LastClose) < 0.88) || (((double)setDayKLine.m_High / setDayKLine.m_LastClose) > 1.12)) { // ��Ȩ���¹����е�
      setDayKLine.m_RelativeStrong = 50; // �¹����л��߳�Ȩ��Ϣ��������˹�
    }
    else if ((fabs(setDayKLine.m_High - setDayKLine.m_Close) < 0.0001)
      && (((double)setDayKLine.m_Close / setDayKLine.m_LastClose) > 1.095)) { // ��ͣ��
      setDayKLine.m_RelativeStrong = 100;
    }
    else if ((fabs(setDayKLine.m_Close - setDayKLine.m_Low) < 0.0001)
      && (((double)setDayKLine.m_Close / setDayKLine.m_LastClose) < 0.905)) { // ��ͣ��
      setDayKLine.m_RelativeStrong = 0;
    }
    else {
      setDayKLine.m_RelativeStrong = ((double)iCount * 100) / iTotalAShare;
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
  strTemp = gl_systemTime.GetTimeString();
  strTemp += buffer;
  gl_systemMessage.PushDayLineInfoMessage(strTemp);    // ����ͬ�����Ʊ�����Ϣ

  gl_systemStatus.SetCalculateRSInProcess(false);
  return(true);
}

bool CMarket::UpdateStockCodeDataBase(void)
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
  for (auto pStockID : gl_vTotalStock) {
    setStockCode.AddNew();
    setStockCode.m_Counter = pStockID->GetIndex();
    setStockCode.m_StockType = pStockID->GetMarket();
    setStockCode.m_StockCode = pStockID->GetStockCode();
    setStockCode.m_StockName = pStockID->GetStockName();
    if (pStockID->GetIPOStatus() == __STOCK_IPOED__) { // ����˹�Ʊ�ǻ�Ծ��Ʊ
      if ((pStockID->GetDayLineEndDay() < (gl_systemTime.GetDay() - 100))
        && (pStockID->GetNewestDayLineDay() < (gl_systemTime.GetDay() - 100))) { // ����˹�Ʊ��������ʷ�����Ѿ�����һ�����ˣ������ô˹�Ʊ״̬Ϊ������
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
    setStockCode.m_NewestDayLineDay = pStockID->GetNewestDayLineDay();
    setStockCode.Update();
  }
  setStockCode.m_pDatabase->CommitTrans();
  setStockCode.Close();

  return true;
}

bool CMarket::UpdateOptionDataBase(void)
{
  CSetOption setOption;
  setOption.Open();
  setOption.m_pDatabase->BeginTrans();
  if (setOption.IsEOF()) {
    setOption.AddNew();
    setOption.m_RelativeStrongEndDay = gl_ChinaStockMarket.GetRelativeStrongEndDay();
    setOption.m_RalativeStrongStartDay = gl_ChinaStockMarket.GetRelativeStrongStartDay();
    setOption.Update();
  }
  else {
    setOption.Edit();
    setOption.m_RelativeStrongEndDay = gl_ChinaStockMarket.GetRelativeStrongEndDay();
    setOption.m_RalativeStrongStartDay = gl_ChinaStockMarket.GetRelativeStrongStartDay();
    setOption.Update();
  }
  setOption.m_pDatabase->CommitTrans();
  setOption.Close();
  return false;
}


