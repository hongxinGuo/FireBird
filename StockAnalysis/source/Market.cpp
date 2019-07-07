//////////////////////////////////////////////////////////////////////////////////////////////////


//#include"stdafx.h"

#include"globedef.h"
#include"accessory.h"
#include"ClientThread.h"

#include"Market.h"

#include"SetDayLineInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMarket::CMarket ( void ) : CObject() {
  static int siInstance = 0;
  m_fLoadedSelectedStock = false;
  if (++siInstance > 1) {
    TRACE("�г�����ֻ�������һ��ʵ��\n");
    ASSERT(0);
  }
	m_fMarketReady = false;    // �г���ʼ״̬Ϊδ���úá�
  m_fCurrentStockChanged = false;
  m_fCurrentEditStockChanged = false;

	m_lTotalMarketBuy = m_lTotalMarketSell = 0;

  m_lTotalActiveStock = 0; // ��ʼʱ��Ʊ���еĹ�Ʊ����Ϊ��
}

CMarket::~CMarket( ) {

   
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

/////////////////////////////////////////////////////////////////////////
//
//	�õ���ʱ��ƫ������09:30Ϊ0��15:00Ϊ240,����Ϊ1����
//
//
////////////////////////////////////////////////////////////////////////
long CMarket::GetMinLineOffset( CStockID sID, time_t Time ) {
	ASSERT( Time >= 0 );
	tm tmTemp;
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

  if ((pStock->m_strStockCode[1] == 'h') && (pStock->m_strStockCode[2] == '6')) {
    return true;
  }
  else {
    if ((pStock->m_strStockCode[1] == 'z') && (pStock->m_strStockCode[2] == '0') && (pStock->m_strStockCode[3] == '0')) {
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

  if ((strStockCode[1] == 'h') && (strStockCode[2] == '6')) {
    return true;
  }
  else {
    if ((strStockCode[1] == 'z') && (strStockCode[2] == '0') && (strStockCode[3] == '0')) {
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
	if ( (pStock = gl_sMarket.GetStockPtr( strStockCode )) != NULL ) {
		return( true );
	}
	else if ( (pStock = gl_sMarket.GetStockPtr( strStockCode )) != NULL ) {
		return ( true );
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
long long CMarket::GetTotalAttackBuyAmount( void ) {
	long long lAmount = 0;
	for ( auto pStock : m_vActiveStock ) {
		if ( IsAStock( pStock ) ) {
			lAmount += pStock->m_lAttackBuyAmount;
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
long long CMarket::GetTotalAttackSellAmount( void ) {
	long long lAmount = 0;
	for ( auto pStock : m_vActiveStock ) {
		if ( IsAStock( pStock ) ) {
			lAmount += pStock->m_lAttackSellAmount;
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
    if (pRTData->m_fActive) { // ��ʵʱ������Ч��
      long lIndex = 0;
      if (m_mapActiveStockToIndex.find(pRTData->m_strStockCode) == m_mapActiveStockToIndex.end()) { // �µĹ�Ʊ���룿
        m_mapActiveStockToIndex[pRTData->m_strStockCode] = m_lTotalActiveStock++; // �����µ�ӳ��
        pStock = make_shared<CStock>();
				pStock->m_fActive = true;
				pStock->m_wMarket = pRTData->m_wMarket;
				pStock->m_strStockCode = pRTData->m_strStockCode;
				pStock->m_strStockName = pRTData->m_strStockName;
				pStock->m_iStockCode = pRTData->m_iStockCode;
				pStock->m_lLastClose = pRTData->m_lLastClose;
				pStock->m_lOpen = pRTData->m_lOpen;
				pStock->UpdataCurrentStatus(pRTData);

        m_vActiveStock.push_back(pStock); // ��Ӵ˹�������������������Ŀǰ��m_lTotalActiveStaock��ֵ��
        ASSERT(m_vActiveStock.size() == m_lTotalActiveStock);
				pStock->PushRTStockData(pRTData);
        pStock->UpdataCurrentStatus(pRTData);
				lIndex = gl_mapTotalStockToIndex[pStock->m_strStockCode];
				gl_vTotalStock.at(lIndex)->m_strStockName = pStock->m_strStockName;
				gl_vTotalStock.at(lIndex)->m_fActive = true; // ���ս��յ������ݣ�
        gl_vTotalStock.at(lIndex)->m_fDayLineNeedUpdated = true;
        // ����˹�Ʊ������δʹ�ù���������Ϊ��ʹ�á�
        // ͣ�ƺ�Ĺ�Ʊ��Ҳ�ܽ��յ�ʵʱ���ݣ�ֻ��������ֻ�����̼ۣ�������Ϊ�㡣�������������Ч���ݡ�
        gl_vTotalStock.at(lIndex)->m_lIPOed = __STOCK_IPOED__;
				ASSERT(gl_vTotalStock.at(lIndex)->m_strStockCode.Compare(pStock->m_strStockCode) == 0);
      }
      else {
        lIndex = m_mapActiveStockToIndex.at(pRTData->m_strStockCode);
        ASSERT(lIndex <= m_lTotalActiveStock);
        if (pRTData->m_time > m_vActiveStock.at(lIndex)->m_Time) { // �µ����ݣ�
          m_vActiveStock.at(lIndex)->UpdataCurrentStatus(pRTData);
          m_vActiveStock.at(lIndex)->PushRTStockData(pRTData); // �洢�µ����������ݳ�
        }
      }
    }
  }
  
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
  static bool sfFirstTime = true;
  int iCount = 1;

  if (sfFirstTime) {
    m_itStock = m_vActiveStock.begin();
    sfFirstTime = false;
  }

  while ((m_itStock != m_vActiveStock.end()) && (iCount < 900)) { // ÿ������ѯ��Ϊ900����Ʊ
    iCount++;
    str += ',';
    str += (*m_itStock++)->m_strStockCode;
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
    if( pStock != nullptr) pStock->CalculateRTData();
    else {
      TRACE(_T("���棺���ջ�Ծ��Ʊ���з���nulltr, %S"), pStock->m_strStockCode);
    }
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
		lIndex = gl_mapTotalStockToIndex.at(gl_strCurrentStockDownLoading);
		// ASSERT(!gl_vTotalStock[lIndex]->m_fActive); ��һ����ƱIPO����δ����ʱ����Ʊ������ڵ�û���������ݡ�ȡ���˶����жϡ�
    // ��Щ��Ʊ�����к���ֱ��չ�����������������������ݲ��ٸ��¡���������������ô˹�ƱΪ��Ч����
    if (gl_vTotalStock.at(lIndex)->m_lDayLineEndDay == 19900101) { // �����ʼ���߽������ڴ���û�б��������˹�Ʊ������δ��ʹ�ù�
      gl_vTotalStock.at(lIndex)->m_lIPOed = __STOCK_NULL__;   // �˹�Ʊ������δʹ�á�
      //TRACE("��Ч��Ʊ���룺%S\n", static_cast<LPCWSTR>(gl_strCurrentStockDownLoading));
    }
    else { // �Ѿ����еĹ�Ʊ
      if (gl_vTotalStock.at(lIndex)->m_lDayLineEndDay + 100 < gl_lToday) {
        gl_vTotalStock.at(lIndex)->m_lIPOed = __STOCK_DELISTED__;   // �˹�Ʊ�����Ѿ����С�
      }
      //TRACE("%S û�пɸ��µ���������\n", static_cast<LPCWSTR>(gl_strCurrentStockDownLoading));
    }
    gl_vTotalStock.at(lIndex)->m_fDayLineNeedUpdated = false; // ������Ҫ������������
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
			TRACE("%S �������ݳ���\n", pDayLine->GetStockCode());
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
			m_mapActiveStockToIndex[pDayLine->GetStockCode()] = m_lTotalActiveStock++;
			auto pStock = make_shared<CStock>();
			pStock->m_fActive = true;
			pStock->m_fDayLineLoaded = false;
			pStock->m_wMarket = pDayLine->GetMarket();
			pStock->m_strStockCode = pDayLine->GetStockCode();
			pStock->m_strStockName = pDayLine->GetStockName();
			strTemp = pStock->m_strStockCode.Right(6); // ��ȡ��Ʊ�����ұߵ���������
			pStock->m_iStockCode = atoi(strTemp.GetBuffer());
			lIndex = m_lTotalActiveStock - 1;
			m_vActiveStock.push_back(pStock);
      ASSERT(m_vActiveStock.size() == m_lTotalActiveStock);
      // ����ȫ�ֹ�Ʊ����Ϣ����ʱRTData��ȫ���޷��������еĹ�Ʊ��Ϣ��
      long lIndexTemp = gl_mapTotalStockToIndex.at(pStock->m_strStockCode);
      gl_vTotalStock.at(lIndexTemp)->m_strStockCode = pStock->m_strStockCode;
      gl_vTotalStock.at(lIndexTemp)->m_strStockName = pStock->m_strStockName;
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
  gl_systemMessage.PushOutputMessage(strTemp);
  gl_vTotalStock.at(gl_mapTotalStockToIndex.at(pDayLine->GetStockCode()))->m_fDayLineNeedUpdated = false; // ��������������ϣ�����Ҫ�����������ˡ�
  if ((vTempDayLine.at(0)->GetDay() + 100) < gl_lToday) { // ��ȡ���Ĺ�Ʊ���������������ղ����ϸ��µ���������գ������˻��������������һ���µ�ʱ��۲졣
    gl_vTotalStock.at(gl_mapTotalStockToIndex.at(pDayLine->GetStockCode()))->m_lIPOed = __STOCK_DELISTED__; // �����л���ͣ���ס�
  }
  else {
    gl_vTotalStock.at(gl_mapTotalStockToIndex.at(pDayLine->GetStockCode()))->m_lIPOed = __STOCK_IPOED__; // �������׹�Ʊ
  }
	ASSERT(lIndex >= 0);
	m_vActiveStock.at(lIndex)->m_fDayLineLoaded = true;
  m_vActiveStock.at(lIndex)->SetDayLineNeedSavingFlag(true); // ���ô洢���߱�ʶ 
  m_vActiveStock.at(lIndex)->m_vDayLine.clear(); // �����������������ݣ�����еĻ���
  // ������������ʱ��Ϊ�������
  for (int i = vTempDayLine.size() - 1; i >= 0; i--) {
    pDayLine = vTempDayLine.at(i);
    if (pDayLine->m_lDay < gl_lToday) { // ��Ҫ�洢�����������ݣ���������������ʵʱ�������ɣ�.
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
	char * pTestPos = pCurrentPos;
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
	pDayLine->SetStockCode(gl_strCurrentStockDownLoading);
	str = gl_strCurrentStockDownLoading.Left(2);
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
	pDayLine->SetVolume(atoi(buffer2)); // ������ǹ���

	if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
	pDayLine->SetAmount(atof(buffer2));

	if (!ReadOneValue(pCurrentPos, buffer2, iCount)) return false;
	pDayLine->SetTotalValue(atof(buffer2)); // ����ֵ�ĵ�λΪ��Ԫ

	i = 0;
	while (*pCurrentPos != 0x0d) {
		if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) return false; // ���ݳ�����������
		buffer2[i++] = *pCurrentPos++;
		iCount++;
	}
	pCurrentPos++;
	iCount++;
	buffer2[i] = 0x000;
	pDayLine->SetCurrentValue(atof(buffer2)); // ��ͨ��ֵ�ĵ�λΪ��Ԫ��
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

//////////////////////////////////////////////////////////////////////////////////////////////
//
// �õ���Ʊ�ļ��
//
//////////////////////////////////////////////////////////////////////////////////////////////
CString CMarket::GetStockName( CString strStockCode ) {
	long lIndex = m_mapActiveStockToIndex[strStockCode];
	return ( m_vActiveStock.at(lIndex)->m_strStockName );
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
    m_pCurrentStock->m_fDayLineLoaded = false;
    AfxBeginThread(ClientthreadLoadDayLineProc, 0);
  }
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
      lIndex = gl_mapTotalStockToIndex.at(pStock->m_strStockCode);
      if (gl_vTotalStock.at(lIndex)->m_lDayLineEndDay >= pDayLine->GetDay()) continue; // �洢�����������ݲ��ô洢
      SaveOneRecord(psetDayLine, pDayLine);
    }
  }
  else {
    for (int i = 0; i < vectorDayLine.size(); i++) { // �����ǵ���洢�ģ���Ҫ��β����ʼ�洢
      auto pDayLine = vectorDayLine.at(i);
      lIndex = gl_mapTotalStockToIndex.at(pStock->m_strStockCode);
      if (gl_vTotalStock.at(lIndex)->m_lDayLineEndDay >= pDayLine->GetDay()) continue; // �洢�����������ݲ��ô洢
      SaveOneRecord(psetDayLine, pDayLine);
    }
  }
  psetDayLine->m_pDatabase->CommitTrans();

  // ���������������ں���ʼ��������
  bool fSave = false;
  if (fReversed) {
    gl_vTotalStock.at(lIndex)->m_lDayLineStartDay = vectorDayLine.at(vectorDayLine.size()-1)->GetDay();
    if (gl_vTotalStock.at(lIndex)->m_lDayLineEndDay < vectorDayLine.at(0)->GetDay()) {
      gl_vTotalStock.at(lIndex)->m_lDayLineEndDay = vectorDayLine.at(vectorDayLine.size() - 1)->GetDay();
      gl_vTotalStock.at(lIndex)->m_lNewestDayLineDay = vectorDayLine.at(vectorDayLine.size() - 1)->GetDay();
      fSave = true;
    }
  }
  else {
    gl_vTotalStock.at(lIndex)->m_lDayLineStartDay = vectorDayLine.at(0)->GetDay();
    if (gl_vTotalStock.at(lIndex)->m_lDayLineEndDay < vectorDayLine.at(vectorDayLine.size() - 1)->GetDay()) {
      gl_vTotalStock.at(lIndex)->m_lDayLineEndDay = vectorDayLine.at(vectorDayLine.size() - 1)->GetDay();
      gl_vTotalStock.at(lIndex)->m_lNewestDayLineDay = vectorDayLine.at(vectorDayLine.size() - 1)->GetDay();
      fSave = true;
    }
  }

  if (fSave) {
    psetStockCode->m_pDatabase->BeginTrans();
    psetStockCode->AddNew();
    psetStockCode->m_Counter = gl_vTotalStock.at(lIndex)->m_nIndex;
    psetStockCode->m_StockType = gl_vTotalStock.at(lIndex)->m_wMarket;
    psetStockCode->m_StockCode = gl_vTotalStock.at(lIndex)->m_strStockCode;
    psetStockCode->m_StockName = gl_vTotalStock.at(lIndex)->m_strStockName;
    psetStockCode->m_DayLineStartDay = gl_vTotalStock.at(lIndex)->m_lDayLineStartDay;
    psetStockCode->m_DayLineEndDay = gl_vTotalStock.at(lIndex)->m_lDayLineEndDay;
    psetStockCode->m_NewestDayLineDay = gl_vTotalStock.at(lIndex)->m_lNewestDayLineDay;
    psetStockCode->m_IPOed = gl_vTotalStock.at(lIndex)->m_lIPOed;
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

  setStockCode.Open();
  long lIndex = 0;
  if (!gl_setSavingDayLineOnly.IsOpen()) gl_setSavingDayLineOnly.Open();

	for (auto pStock : m_vActiveStock) {
		if (pStock->IsDayLineNeedSaving()) {
 			lIndex = gl_mapTotalStockToIndex.at(pStock->m_strStockCode);
      if (pStock->m_vDayLine.size() > 0) { // �¹ɵ�һ������ʱ������ֻ�洢���ڽ�����������ݣ������������ǿյģ��ʶ���Ҫ�ж�һ��
        SaveDayLine(&gl_setSavingDayLineOnly, &setStockCode, pStock, pStock->m_vDayLine, false);
      }
      else {
        CString str1 = gl_vTotalStock.at(lIndex)->m_strStockCode;
        str1 += _T(" �¹�����,û����������");
        gl_systemMessage.PushOutputMessage(str1);

      }
      pStock->m_vDayLine.clear();
      pStock->m_fDayLineLoaded = false;
      CString str = gl_vTotalStock.at(lIndex)->m_strStockCode;
      str += _T("�������ϴ洢���");
      gl_systemMessage.PushOutputMessage(str);
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
bool CMarket::SaveRTData(CSetRealTimeData * psetRT) {
  ASSERT(!psetRT->IsOpen());
  psetRT->Open();
  psetRT->m_pDatabase->BeginTrans();
  for (auto pStock : m_vActiveStock) {
    if (pStock != nullptr) {
      if (pStock->m_fActive) {
        pStock->SaveRealTimeData(psetRT);
      }
    }
  }
  psetRT->m_pDatabase->CommitTrans();
  psetRT->Close();
  return(true);
}

bool CMarket::IsTotalStockDayLineChecked(void) {
  for (auto pStockID : gl_vTotalStock) {
    if (pStockID->m_fDayLineNeedUpdated) return false;
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
  gl_systemMessage.PushFindMessage(_T("��ʼ�������½����յ�ʵʱ����"));

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
    if ((pStock->m_lHigh == 0) && (pStock->m_lLow == 0) && (pStock->m_lAmount == 0)
      && (pStock->m_lVolume == 0) && (pStock->m_lNew == 0)) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
      continue;
    }
    lIndex = gl_mapTotalStockToIndex.at(pStock->m_strStockCode);
    gl_vTotalStock.at(lIndex)->m_lNewestDayLineDay = lCurrentTradeDay; // �������½��յ������������ڡ�
    if (gl_vTotalStock.at(lIndex)->m_lDayLineEndDay == gl_lLastTradeDay) { // �������������ʷ�������ھ�����һ�������գ�����´�����
      // ���ж���ȱ�ݣ�������ƽ������Ҳ�ǽڼ���ʱ���Ͳ�������ˡ��о�֮��
      gl_vTotalStock.at(lIndex)->m_lDayLineEndDay = lCurrentTradeDay;
    }
    gl_vTotalStock.at(lIndex)->m_lIPOed = __STOCK_IPOED__; // ������һ�Ρ���ֹ�¹ɹ�Ʊ��������û����ʷ���ݶ�������Ϊ�����ڡ�
    setDayKLine.AddNew();
    setDayKLine.m_Time = lCurrentTradeDay;
    setDayKLine.m_Market = pStock->m_wMarket;
    setDayKLine.m_StockName = pStock->m_strStockName;
    setDayKLine.m_StockCode = pStock->m_strStockCode;
    lLastClose = pStock->m_lLastClose;
    setDayKLine.m_LastClose = (double)pStock->m_lLastClose / 1000;
    setDayKLine.m_Open = (double)pStock->m_lOpen / 1000;
    setDayKLine.m_High = (double)pStock->m_lHigh / 1000;
    setDayKLine.m_Low = (double)pStock->m_lLow / 1000;
    lClose = pStock->m_lNew;
    setDayKLine.m_Close = (double)pStock->m_lNew / 1000;
    setDayKLine.m_UpAndDown = ((double)(lClose - lLastClose)) / 1000;
    if (lLastClose == 0) { // �����е�һ��Ĺ�Ʊ
      setDayKLine.m_UpDownRate = 0;
    }
    else {
      setDayKLine.m_UpDownRate = (((double)(lClose - lLastClose)) * 100.0) / lLastClose;
    }
    
    setDayKLine.m_Volume = pStock->m_lVolume;
    setDayKLine.m_Amount = pStock->m_lAmount;
    setDayKLine.m_TotalValue = 0;
    setDayKLine.m_CurrentValue = 0;
    setDayKLine.m_RelativeStrong = pStock->m_dRelativeStrong;
    setDayKLine.m_TransactionNumber = pStock->m_lTransactionNumber;
    setDayKLine.m_TransactionNumberBelow5000 = pStock->m_lTransactionNumberBelow5000;
    setDayKLine.m_TransactionNumberBelow50000 = pStock->m_lTransactionNumberBelow50000;
    setDayKLine.m_TransactionNumberBelow200000 = pStock->m_lTransactionNumberBelow200000;
    setDayKLine.m_TransactionNumberAbove200000 = pStock->m_lTransactionNumberAbove200000;
    setDayKLine.m_CancelBuyVolume = pStock->m_lCancelBuyVolume;
    setDayKLine.m_CancelSellVolume = pStock->m_lCancelSellVolume;
    setDayKLine.m_AttackBuyVolume = pStock->m_lAttackBuyVolume;
    setDayKLine.m_AttackSellVolume = pStock->m_lAttackSellVolume;
    setDayKLine.m_StrongBuyVolume = pStock->m_lStrongBuyVolume;
    setDayKLine.m_StrongSellVolume = pStock->m_lStrongSellVolume;
    setDayKLine.m_UnknownVolume = pStock->m_lUnknownVolume;
    setDayKLine.m_OrdinaryBuyVolume = pStock->m_lOrdinaryBuyVolume;
    setDayKLine.m_OrdinarySellVolume = pStock->m_lOrdinarySellVolume;
    setDayKLine.m_AttackBuyBelow50000 = pStock->m_lAttackBuyBelow50000;
    setDayKLine.m_AttackBuyBelow200000 = pStock->m_lAttackBuyBelow200000;
    setDayKLine.m_AttackBuyAbove200000 = pStock->m_lAttackBuyAbove200000;
    setDayKLine.m_AttackSellBelow50000 = pStock->m_lAttackSellBelow50000;
    setDayKLine.m_AttackSellBelow200000 = pStock->m_lAttackSellBelow200000;
    setDayKLine.m_AttackSellAbove200000 = pStock->m_lAttackSellAbove200000;
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
    if ((pStock->m_lHigh == 0) && (pStock->m_lLow == 0) && (pStock->m_lAmount == 0)
      && (pStock->m_lVolume == 0) && (pStock->m_lNew == 0)) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
      continue;
    }
    setDayLineInfo.AddNew();
    setDayLineInfo.m_Time = lCurrentTradeDay;
    setDayLineInfo.m_Market = pStock->m_wMarket;
    setDayLineInfo.m_StockName = pStock->m_strStockName;
    setDayLineInfo.m_StockCode = pStock->m_strStockCode;

    setDayLineInfo.m_RelativeStrong = pStock->m_dRelativeStrong;
    setDayLineInfo.m_TransactionNumber = pStock->m_lTransactionNumber;
    setDayLineInfo.m_TransactionNumberBelow5000 = pStock->m_lTransactionNumberBelow5000;
    setDayLineInfo.m_TransactionNumberBelow50000 = pStock->m_lTransactionNumberBelow50000;
    setDayLineInfo.m_TransactionNumberBelow200000 = pStock->m_lTransactionNumberBelow200000;
    setDayLineInfo.m_TransactionNumberAbove200000 = pStock->m_lTransactionNumberAbove200000;
    setDayLineInfo.m_CancelBuyVolume = pStock->m_lCancelBuyVolume;
    setDayLineInfo.m_CancelSellVolume = pStock->m_lCancelSellVolume;
    setDayLineInfo.m_AttackBuyVolume = pStock->m_lAttackBuyVolume;
    setDayLineInfo.m_AttackSellVolume = pStock->m_lAttackSellVolume;
    setDayLineInfo.m_StrongBuyVolume = pStock->m_lStrongBuyVolume;
    setDayLineInfo.m_StrongSellVolume = pStock->m_lStrongSellVolume;
    setDayLineInfo.m_UnknownVolume = pStock->m_lUnknownVolume;
    setDayLineInfo.m_OrdinaryBuyVolume = pStock->m_lOrdinaryBuyVolume;
    setDayLineInfo.m_OrdinarySellVolume = pStock->m_lOrdinarySellVolume;
    setDayLineInfo.m_AttackBuyBelow50000 = pStock->m_lAttackBuyBelow50000;
    setDayLineInfo.m_AttackBuyBelow200000 = pStock->m_lAttackBuyBelow200000;
    setDayLineInfo.m_AttackBuyAbove200000 = pStock->m_lAttackBuyAbove200000;
    setDayLineInfo.m_AttackSellBelow50000 = pStock->m_lAttackSellBelow50000;
    setDayLineInfo.m_AttackSellBelow200000 = pStock->m_lAttackSellBelow200000;
    setDayLineInfo.m_AttackSellAbove200000 = pStock->m_lAttackSellAbove200000;
    setDayLineInfo.Update();
  }
  setDayLineInfo.m_pDatabase->CommitTrans();
  setDayLineInfo.Close();

  gl_systemMessage.PushFindMessage(_T("���½�����ʵʱ���ݴ������"));
  return true;
}
