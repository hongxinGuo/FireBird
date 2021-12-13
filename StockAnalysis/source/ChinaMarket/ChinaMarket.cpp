//////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"globedef.h"
#include"accessory.h"
#include"Thread.h"

#include"WebInquirer.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

#include"SetDayLineExtendInfo.h"
#include"SetDayLineTodaySaved.h"
#include"SetOption.h"
#include"SetChinaMarketOption.h"
#include"SetCrweberIndex.h"
#include"SetChinaChoicedStock.h"
#include"SetRSStrong2Stock.h"
#include"SetRSStrong1Stock.h"
#include"SetRSStrongStock.h"
#include"SetRSOption.h"
#include"SetStockSection.h"

#include"SetCurrentWeekLine.h"

#include"CallableFunction.h"

using namespace std;
#include<thread>
#include<algorithm>
#include<set>

#include<gsl/gsl>
using namespace gsl;

// �ź�����������Ϊȫ�ֱ�����Ϊ�˳�ʼ����
Semaphore gl_SaveOneStockDayLine(4);  // ���ź�����������������ʷ���ݿ�
Semaphore gl_ProcessSinaRTDataQueue(1);   // ����ʵʱ���ݴ���ͬʱֻ����һ���̴߳���
Semaphore gl_ProcessTengxunRTDataQueue(1);
Semaphore gl_ProcessNeteaseRTDataQueue(1);
Semaphore gl_SemaphoreBackGroundTaskThreads(cMaxBackGroundTaskThreads); // ��̨�����߳��������Ϊ8

CWebRTDataContainer gl_WebRTDataContainer;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CChinaMarket, CVirtualMarket)

CChinaMarket::CChinaMarket(void) : CVirtualMarket() {
	static int siInstance = 0;

	if (++siInstance > 1) {
		TRACE(_T("ChinaMarket�г�����ֻ�������һ��ʵ��\n"));
	}
	m_strMarketId = _T("�й���Ʊ�г�");
	m_lMarketTimeZone = -8 * 3600; // ������׼ʱ��λ�ڶ���������ǰGMT8Сʱ
	m_fSaveRTData = false; // �˴洢ʵʱ���ݱ�ʶ�����ڴ洢�����Ժ����õ�ʵʱ���ݡ�Ŀǰ�����Ѿ���ɡ�

	m_avChoicedStock.resize(30);
	m_aRSStrongOption.resize(10);
	m_vStockSection.resize(2000); // �����1000���Ρ�
	CStockSectionPtr pStockSection;
	for (int i = 0; i < 2000; i++) {
		pStockSection = make_shared<CStockSection>();
		pStockSection->SetIndexNumber(i);
		if (i < 1000) pStockSection->SetMarket(__SHANGHAI_MARKET__);
		else pStockSection->SetMarket(__SHENZHEN_MARKET__);
		m_vStockSection.at(i) = pStockSection;
	}

	Reset();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ȫ�ֱ����Ľ���λ�ڳ����˳������Ҫ����CMainFrame�Ľ������ʶ����Ҫ�뽫ϵͳ�˳��Ĺ��̷��������Ҫ�о���
// Ŀǰ�������������������κι��ܡ�
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
CChinaMarket::~CChinaMarket() {
	m_vChinaMarketStock.resize(0);
}

void CChinaMarket::ResetMarket(void) {
	TRACE(_T("�����й�����\n"));
	CString str = _T("�����й������ڱ�����׼ʱ�䣺");
	str += GetStringOfMarketTime();
	gl_systemMessage.PushInformationMessage(str);
	while (gl_ThreadStatus.IsBackGroundthreadsWorking() || gl_ThreadStatus.IsCalculatingRTData() || gl_ThreadStatus.IsSavingTempData()
		|| gl_ThreadStatus.IsSavingThreadRunning()) {
		Sleep(1);
	}
	Reset();

	ASSERT(m_mapChinaMarketStock.size() == GetTotalStock()); // �������ݿ�ǰ��Ҫ��֤�Ѿ�װ����Ԥ�����õĹ�Ʊ����
	LoadStockSection(); // װ�����֤ȯ����ռ��Ƿ��ѱ�ʹ�õı�ʶ����λ���룬��1000Ϊ��λ���ӣ��������1000000�����ô��룩
	LoadStockCodeDB();
	LoadOptionDB();
	LoadOptionChinaStockMarketDB();
	LoadChoicedStockDB();
	Load10DaysRSStrong1StockSet();
	Load10DaysRSStrong2StockSet();
	LoadCalculatingRSOption();
	Load10DaysRSStrongStockDB();
}

void CChinaMarket::Reset(void) {
	CalculateTime(); // ��ʼ���г�ʱ��
	m_lLoadedStock = 0;

	m_llRTDataReceived = 0;
	m_lStockDayLineNeedUpdate = 0;

	m_fLoadedSelectedStock = false;
	SetSystemReady(false); // �г���ʼ״̬Ϊδ���úá�
	m_fCurrentStockChanged = false;
	m_fChoiced10RSStrong1StockSet = false;
	m_fChoiced10RSStrong2StockSet = false;
	m_fChoiced10RSStrongStockSet = false;
	m_fCurrentEditStockChanged = false;
	m_fCalculateChoiced10RS = false;

	m_lTotalMarketBuy = m_lTotalMarketSell = 0;

	m_ttNewestTransactionTime = 0;

	if (GetMarketTime() >= 150400) { // �й���Ʊ�г��Ѿ�����
		SetTodayStockProcessed(true); // ���к��ִ�б�ϵͳ������Ϊ�Ѿ���������չ�Ʊ�����ˡ�
	}
	else SetTodayStockProcessed(false);

	m_lRSEndDate = m_lRSStartDate = m_lLastLoginDate = __CHINA_MARKET_BEGIN_DATE__;
	m_lLastLoginTime = 0;
	m_lUpdatedDateFor10DaysRS2 = m_lUpdatedDateFor10DaysRS1 = m_lUpdatedDateFor10DaysRS = __CHINA_MARKET_BEGIN_DATE__;

	m_fSaveDayLine = false;
	m_fMarketOpened = false;
	m_fSaveTempData = true;

	m_fTodayTempDataLoaded = false;

	m_fUpdateStockSection = false;

	m_lCurrentRSStrongIndex = 0;
	m_lCurrentSelectedStockSet = -1; // ѡ��ʹ��ȫ���Ʊ����
	m_lCurrentSelectedPosition = 0;

	m_fRTDataSetCleared = false;

	m_fCheckActiveStock = true;  //��鵱�ջ�Ծ��Ʊ������Ϊ�档

	m_fGetRTData = true;
	m_iCountDownSlowReadingRTData = 3; // 400����ÿ��

	m_iRTDataServer = 0; // ʹ������ʵʱ���ݷ�����
	m_iRTDataInquiryTickNumber = 3;// Ĭ��400�����ѯһ��

	m_fUsingSinaRTDataReceiver = true; // ʹ������ʵʱ������ȡ��
	m_fUsingTengxunRTDataReceiver = true; // Ĭ��״̬�¶�ȡ��Ѷʵʱ����
	m_fUsingNeteaseRTDataReceiver = true; // ʹ������ʵʱ������ȡ��
	m_iCountDownTengxunNumber = 5;

	m_fUpdateChoicedStockDB = false;

	m_lSinaStockRTDataInquiringIndexFromTotalStockSet = 0;
	m_lSinaStockRTDataInquiringIndex = 0;
	m_lTengxunRTDataInquiringIndex = 0;
	m_lNeteaseRTDataInquiringIndex = 0;
	m_lNeteaseDayLineDataInquiringIndex = 0;

	m_pCurrentStock = nullptr;

	m_vChinaMarketStock.clear();
	m_mapChinaMarketStock.clear();

	m_vTotalStockSet.resize(0);
	m_mapTotalStockSet.clear();

	m_vCurrentSectionStockCode.clear();
	// Ԥ���Ʊ���뼯����
	m_vCurrentSectionStockCode.push_back(_T("000000.SS")); // �Ϻ�ָ��
	m_vCurrentSectionStockCode.push_back(_T("600000.SS")); // �Ϻ�����
	m_vCurrentSectionStockCode.push_back(_T("601000.SS")); // �Ϻ�����
	m_vCurrentSectionStockCode.push_back(_T("603000.SS")); // �Ϻ�����
	m_vCurrentSectionStockCode.push_back(_T("688000.SS")); // �Ϻ��ƴ���
	m_vCurrentSectionStockCode.push_back(_T("900000.SS")); // �Ϻ�B��
	m_vCurrentSectionStockCode.push_back(_T("000000.SZ")); // ��������
	m_vCurrentSectionStockCode.push_back(_T("001000.SZ")); // ��������
	m_vCurrentSectionStockCode.push_back(_T("002000.SZ")); // ������С��
	m_vCurrentSectionStockCode.push_back(_T("200000.SZ")); // ����B��
	m_vCurrentSectionStockCode.push_back(_T("300000.SZ")); // ���ڴ�ҵ��
	m_vCurrentSectionStockCode.push_back(_T("399000.SZ")); // ����ָ��
	// �ӹ�Ʊ���뼯���ݿ��ж���������Ʊ��

	//����StockSection
	for (int i = 0; i < m_vStockSection.size(); i++) {
		m_vStockSection.at(i)->SetBuildStockPtr(false);
	}
	// ���ɹ�Ʊ�����
	CreateTotalStockContainer();
}

#ifdef _DEBUG
void CChinaMarket::AssertValid() const {
	CVirtualMarket::AssertValid();
}

void CChinaMarket::Dump(CDumpContext& dc) const {
	CVirtualMarket::Dump(dc);
}
#endif //_DEBUG

bool CChinaMarket::CheckMarketReady(void) {
	if (!IsSystemReady()) {
		long lMax = GetTotalStock() > 12000 ? GetTotalStock() * 2 : 24000;
		if (m_llRTDataReceived > lMax) {
			SetSystemReady(true);
			gl_systemMessage.PushInformationMessage(_T("�й���Ʊ�г���ʼ�����"));
		}
	}
	return IsSystemReady();
}

bool CChinaMarket::ChangeToNextStock(void) {
	ASSERT(m_pCurrentStock != nullptr);
	const long lIndex = m_pCurrentStock->GetOffset();
	CChinaStockPtr pStock = m_pCurrentStock;

	if (IsTotalStockSetSelected()) {
		bool fFound = false;
		int i = 1;
		while (!fFound) {
			if ((lIndex + i) < GetTotalStock()) {
				pStock = GetStock(lIndex + i);
			}
			else {
				pStock = GetStock(lIndex + i - GetTotalStock());
			}
			if (!pStock->IsNullStock()) fFound = true;
			i++;
		}
	}
	else {
		ASSERT(m_avChoicedStock.at(m_lCurrentSelectedStockSet).size() > 0); //
		if (m_lCurrentSelectedPosition >= (m_avChoicedStock.at(m_lCurrentSelectedStockSet).size() - 1)) {
			m_lCurrentSelectedPosition = 0;
			pStock = m_avChoicedStock.at(m_lCurrentSelectedStockSet).at(m_lCurrentSelectedPosition);
		}
		else {
			m_lCurrentSelectedPosition++;
			pStock = m_avChoicedStock.at(m_lCurrentSelectedStockSet).at(m_lCurrentSelectedPosition);
		}
	}

	SetCurrentStock(pStock);
	return true;
}

bool CChinaMarket::ChangeToPrevStock(void) {
	ASSERT(m_pCurrentStock != nullptr);
	const long lIndex = m_pCurrentStock->GetOffset();
	CChinaStockPtr pStock = m_pCurrentStock;

	if (IsTotalStockSetSelected()) {
		bool fFound = false;
		int i = 1;
		while (!fFound) {
			if ((lIndex - i) >= 0) {
				pStock = GetStock(lIndex - i);
			}
			else {
				pStock = GetStock(lIndex + GetTotalStock() - i);
			}
			if (!pStock->IsNullStock()) fFound = true;
			i++;
		}
	}
	else {
		ASSERT(m_avChoicedStock.at(m_lCurrentSelectedStockSet).size() > 0); //
		if (m_lCurrentSelectedPosition == 0) {
			m_lCurrentSelectedPosition = m_avChoicedStock.at(m_lCurrentSelectedStockSet).size() - 1;
			pStock = m_avChoicedStock.at(m_lCurrentSelectedStockSet).at(m_lCurrentSelectedPosition);
		}
		else {
			m_lCurrentSelectedPosition--;
			pStock = m_avChoicedStock.at(m_lCurrentSelectedStockSet).at(m_lCurrentSelectedPosition);
		}
	}
	SetCurrentStock(pStock);
	return true;
}

bool CChinaMarket::ChangeToPrevStockSet(void) {
	do {
		if (m_lCurrentSelectedStockSet > -1) m_lCurrentSelectedStockSet--;
		else {
			m_lCurrentSelectedStockSet = c_10DaysRSStockSetStartPosition + 9;
		}
		ASSERT(m_lCurrentSelectedStockSet < 20);
	} while ((m_lCurrentSelectedStockSet != -1) && (m_avChoicedStock.at(m_lCurrentSelectedStockSet).size() == 0));

	return true;
}

bool CChinaMarket::ChangeToNextStockSet(void) {
	do {
		if (m_lCurrentSelectedStockSet == (c_10DaysRSStockSetStartPosition + 9)) m_lCurrentSelectedStockSet = -1;
		else {
			m_lCurrentSelectedStockSet++;
		}
		ASSERT(m_lCurrentSelectedStockSet < 20);
	} while ((m_lCurrentSelectedStockSet != -1) && (m_avChoicedStock.at(m_lCurrentSelectedStockSet).size() == 0));

	return true;
}

size_t CChinaMarket::GetCurrentStockSetSize(void) {
	if (IsTotalStockSetSelected()) return GetTotalStock();
	else return m_avChoicedStock.at(m_lCurrentSelectedStockSet).size();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ʼ�����п��ܵĹ�Ʊ����أ�ֻ��CChinaMarket�ĳ�ʼ��������һ�Ρ�
// ���������Ҫ����ȫ�ֱ�����ʼ����֧�֣��ʶ�gl_ChinaStockMarketʵ����Ҫ��������ȫ�ֱ��������
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::CreateTotalStockContainer(void) {
	CChinaStockPtr pStock = nullptr;

	// ���֮ǰ�����ݣ�����еĻ�����Resetʱ�������������оʹ������ݣ���
	ASSERT(GetTotalStock() == 0);

	for (int i = 0; i < m_vCurrentSectionStockCode.size(); i++) {
		CreateStockSection(m_vCurrentSectionStockCode.at(i));
	}
	ASSERT(m_mapChinaMarketStock.size() == m_vChinaMarketStock.size());
	return true;
}

void CChinaMarket::CreateStockSection(CString strFirstStockCode) {
	CString strCode = GetStockSymbol(strFirstStockCode);
	CString strStockCode, strStockSymbol, strExchange;
	CString str = _T("");
	int iCode = atoi(strCode.GetBuffer());
	int iMarket = 0;
	char buffer[10];
	CChinaStockPtr pStock = nullptr;

	if (IsShanghaiExchange(strFirstStockCode)) { // �Ϻ��г�
		iMarket = 0;
	}
	else if (IsShenzhenExchange(strFirstStockCode)) { // �����г�
		iMarket = 1000;
	}
	if (m_vStockSection.at((iCode / 1000) + iMarket)->IsBuildStockPtr()) return; // �Ѿ���֤ȯ���н�����
	// �����Ϻ���Ʊ����
	for (int i = iCode; i < (iCode + 1000); i++) {
		strExchange = GetStockExchange(strFirstStockCode);
		sprintf_s(buffer, _T("%06d"), i);
		strStockSymbol = buffer;
		strStockCode = CreateStockCode(strExchange, strStockSymbol);
		m_vTotalStockSet.push_back(strStockCode); //
		m_mapTotalStockSet[strStockCode] = m_vTotalStockSet.size();
	}
	if (UpdateStockSection(iCode / 1000 + iMarket)) {
		SetUpdateStockSection(true);
	}
	m_vStockSection.at(iCode / 1000 + iMarket)->SetBuildStockPtr(true); // �Ѿ���֤ȯ���н�����
}

bool CChinaMarket::CreateStock(CString strStockCode, CString strStockName, bool fProcessRTData) {
	CChinaStockPtr pStock;
	CString str;

	ASSERT(m_vChinaMarketStock.size() == m_mapChinaMarketStock.size());
	pStock = make_shared<CChinaStock>();
	pStock->SetActive(false);
	pStock->SetTodayNewStock(true);
	pStock->SetSymbol(strStockCode);
	pStock->SetDisplaySymbol(strStockName);
	pStock->SetIPOStatus(__STOCK_NOT_CHECKED__);
	pStock->SetOffset(GetTotalStock());
	pStock->SetDayLineEndDate(19900101);
	pStock->SetDayLineStartDate(19900101);
	pStock->SetUpdateProfileDB(true);
	pStock->SetNeedProcessRTData(fProcessRTData);
	AddStock(pStock);
	ASSERT(pStock->IsDayLineNeedUpdate());
	str = _T("china Market�����´���") + pStock->GetSymbol();
	gl_systemMessage.PushInnerSystemInformationMessage(str);
	ASSERT(m_vChinaMarketStock.size() == m_mapChinaMarketStock.size());
	return true;
}

bool CChinaMarket::AddStock(CChinaStockPtr pStock) {
	if (pStock == nullptr) return false;
	if (IsStock(pStock->GetSymbol())) return false;

	m_mapChinaMarketStock[pStock->GetSymbol()] = m_vChinaMarketStock.size(); // ʹ���±������µ�ӳ��
	m_vChinaMarketStock.push_back(pStock);

	return true;
}

bool CChinaMarket::DeleteStock(CChinaStockPtr pStock) {
	if (pStock == nullptr) return false;
	if (!IsStock(pStock->GetSymbol())) return false;

	m_mapChinaMarketStock.erase(pStock->GetSymbol());

	auto it = find(m_vChinaMarketStock.begin(), m_vChinaMarketStock.end(), pStock);
	m_vChinaMarketStock.erase(it);

	return true;
}

bool CChinaMarket::UpdateStockSection(CString strStockCode) {
	if (!IsStock(strStockCode)) return false;

	CString strCode = GetStockSymbol(strStockCode);
	int iCode = atoi(strCode.GetBuffer());
	int iMarket = 0;

	if (IsShanghaiExchange(strStockCode)) { // �Ϻ��г�
		iMarket = 0;
	}
	else if (IsShenzhenExchange(strStockCode)) { // �����г�
		iMarket = 1000;
	}
	return UpdateStockSection(iCode / 1000 + iMarket);
}

bool CChinaMarket::UpdateStockSection(long lIndex) {
	if (!m_vStockSection.at(lIndex)->IsActive()) {
		m_vStockSection.at(lIndex)->SetActive(true);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����������߹�Ʊ������ַ��������ڲ�ѯ�˹�Ʊ�ڵ�ǰ�г��Ƿ��ڻ�Ծ״̬�������Ƿ���ڴ˹�Ʊ���룩
//
//  �˺����Ǽ��m_vChinaMarketStock��Ʊ��
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::CreateNeteaseDayLineInquiringStr(CString& strReturn, long lEndPosition) {
	bool fFound = false;
	int iCount = 0;
	CString strTemp;
	strReturn = _T("");

	while (!fFound && (iCount++ < GetTotalStock())) {
		CChinaStockPtr pStock = m_vChinaMarketStock.at(m_lNeteaseDayLineDataInquiringIndex);
		if (!pStock->IsDayLineNeedUpdate()) { // �������ݲ���Ҫ���¡���ϵͳ��ʼʱ�����ô�m_fDayLineNeedUpdate��ʶ
			// TRACE("%S ���������������\n", static_cast<LPCWSTR>(pStock->m_strSymbol));
			IncreaseStockInquiringIndex(m_lNeteaseDayLineDataInquiringIndex, lEndPosition);
		}
		else if (pStock->GetDayLineEndDate() >= GetLastTradeDate()) { //��һ�����յ����������Ѿ��洢����ʱ�Ѿ������һ�����������ˣ������ٴδ���
			pStock->SetDayLineNeedUpdate(false); // �˹�Ʊ�������ϲ���Ҫ�����ˡ�
			// TRACE("%S �������ݱ����Ѹ���\n", static_cast<LPCWSTR>(pStock->m_strSymbol));
			IncreaseStockInquiringIndex(m_lNeteaseDayLineDataInquiringIndex, lEndPosition);
		}
		else {
			fFound = true;
		}
	}

	if (iCount >= GetTotalStock()) { //  û���ҵ���Ҫ�������ߵ�֤ȯ
		TRACE("δ�ҵ������������ʷ���ݵĹ�Ʊ\n");
		return false;
	}

	// �ҵ���������������ʷ���ݵĹ�Ʊ��siCounterΪ������
	CChinaStockPtr pStock = m_vChinaMarketStock.at(m_lNeteaseDayLineDataInquiringIndex);
	ASSERT(!pStock->IsDayLineNeedSaving());
	ASSERT(pStock->IsDayLineNeedUpdate());
	pStock->SetDayLineNeedUpdate(false);
	strReturn += XferStandredToNetease(pStock->GetSymbol());
	IncreaseStockInquiringIndex(m_lNeteaseDayLineDataInquiringIndex, lEndPosition);
	return true;
}

long CChinaMarket::IncreaseStockInquiringIndex(long& lIndex, long lEndPosition) {
	if (++lIndex >= lEndPosition) {
		lIndex = 0;
	}
	return lIndex;
}

/////////////////////////////////////////////////////////////////////////
//
//	�õ���ʱ��ƫ������09:30Ϊ0��15:00Ϊ240,����Ϊ1����
//
//
////////////////////////////////////////////////////////////////////////
long CChinaMarket::GetMinLineOffset(time_t tUTC) {
	ASSERT(tUTC >= 0);
	tm tmMarketTime{};
	time_t tUTC2 = 0;
	long lIndex = 0;

	tmMarketTime = TransferToMarketTime(tUTC);
	tmMarketTime.tm_hour = 9;
	tmMarketTime.tm_min = 30;
	tmMarketTime.tm_sec = 0;
	tUTC2 = TransferToUTCTime(&tmMarketTime);
	lIndex = (tUTC - tUTC2) / 60;
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
bool CChinaMarket::IsAStock(not_null<CChinaStockPtr> pStock) const {
	return(IsAStock(pStock->GetSymbol()));
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//		�ж�strStockCode�Ƿ�Ϊ����A������Ĺ�Ʊ���롣
//		����A�ɴ�����600��601��ͷ������A�ɴ�����000��001��ͷ��
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::IsAStock(CString strStockCode) const {
	CString strSymbol = GetStockSymbol(strStockCode);
	if (IsShanghaiExchange(strStockCode)) {
		if ((strSymbol[0] == '6') && (strSymbol[1] == '0')) {
			if ((strSymbol[2] == '0') || (strSymbol[2] == '1')) {
				return true;
			}
		}
	}
	else if (IsShenzhenExchange(strStockCode)) {
		if ((strSymbol[0] == '0') && (strSymbol[1] == '0')) {
			if ((strSymbol[2] == '0') || (strSymbol[2] == '2')) {
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
bool CChinaMarket::IsStock(CString strStockCode) const {
	if (m_mapChinaMarketStock.find(strStockCode) == m_mapChinaMarketStock.end()) {
		return(false);
	}
	else {
		return(true);
	}
}

bool CChinaMarket::IsDayLineDBUpdated(void) noexcept {
	for (auto& pStock : m_vChinaMarketStock) {
		if (pStock->IsDayLineDBUpdated()) return true;
	}
	return false;
}

void CChinaMarket::ClearDayLineDBUpdatedFlag(void) noexcept {
	for (auto& pStock : m_vChinaMarketStock) {
		pStock->SetDayLineDBUpdated(false);
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////
INT64 CChinaMarket::GetTotalAttackBuyAmount(void) {
	INT64 lAmount = 0;
	for (auto& pStock : m_vChinaMarketStock) {
		if (pStock->IsActive() && IsAStock(pStock)) {
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
INT64 CChinaMarket::GetTotalAttackSellAmount(void) {
	INT64 lAmount = 0;
	for (auto& pStock : m_vChinaMarketStock) {
		if (pStock->IsActive() && IsAStock(pStock)) {
			lAmount += pStock->GetAttackSellAmount();
		}
	}
	return(lAmount);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ץȡ������ʷ��������
// ���ڿ��ܻ�ץȡȫ��5000�������������ݣ�����ʱ�䳬��10���ӣ��ʶ�9:15:00��һ������ϵͳʱ��ȥ���£�����9:25:00�ڶ�������ϵͳ��ſ�ʼ��
// Ϊ�˷�ֹ������ϵͳ������ͻ��ʵ��ִ��ʱ���Ӻ���9:26:00��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskGetNeteaseDayLineFromWeb(void) {
	ASSERT(IsSystemReady());
	if ((GetMarketTime() >= 92600) && IsDayLineNeedUpdate()) {
		// ץȡ��������.��ʼ��09:26:00
		// ���ʹ���ĸ����棬�������ױ����׷������ܾ�����һ�㻹��������Ϊ�á�
		return(gl_WebInquirer.GetNeteaseDayLineData());
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ���ݵȣ���SchedulingTaskPerSecond��������,ÿ����ִ��һ�Ρ�
// ��ʵʱ�����ݴ�����е����ݷֱ��ŵ����Թ�Ʊ��ʵʱ�����С�
// �ַ�����ʱ��ֻ�ַ��µģ�����ʱ������֮ǰ���ݵģ�ʵʱ���ݡ�
//
// �˺����õ�������ȫ�ֱ��������Ƿ������߳�Ϊ�á������߳�Ŀǰ����ֻ��������ɵĹҵ������
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskDistributeSinaRTDataToProperStock(void) {
	gl_ProcessSinaRTDataQueue.Wait();
	const size_t lTotalNumber = gl_WebRTDataContainer.GetSinaDataSize();
	CString strVolume;
	bool fFoundNewStock = false;
	CString strStandredStockCode;
	CWebRTDataPtr pRTData = nullptr;
	CChinaStockPtr pStock = nullptr;

	for (int iCount = 0; iCount < lTotalNumber; iCount++) {
		pRTData = gl_WebRTDataContainer.PopSinaData();
		if (pRTData->GetDataSource() == __INVALID_RT_WEB_DATA__) {
			gl_systemMessage.PushInnerSystemInformationMessage(_T("����ʵʱ����Դ��������"));
			continue;
		}
		if (pRTData->IsActive()) { // ��ʵʱ������Ч��
			if (m_ttNewestTransactionTime < pRTData->GetTransactionTime()) {
				m_ttNewestTransactionTime = pRTData->GetTransactionTime();
			}
			if (IsCheckActiveStock()) {
				if (!IsStock(pRTData->GetSymbol())) {
					CreateStock(pRTData->GetSymbol(), pRTData->GetStockName(), true);
					fFoundNewStock = true;
				}
			}
			ASSERT(IsStock(pRTData->GetSymbol()));
			pStock = GetStock(pRTData->GetSymbol());
			if (!pStock->IsActive()) { // �����ڷ��а�����ʱ���ִ���ԭ����顣
				if (pRTData->IsValidTime(14)) {
					pStock->SetTodayActive(pRTData->GetSymbol(), pRTData->GetStockName());
					pStock->SetIPOStatus(__STOCK_IPOED__);
				}
			}
			if (pRTData->GetTransactionTime() > pStock->GetTransactionTime()) { // �µ����ݣ�
				pStock->PushRTData(pRTData); // �洢�µ����������ݳ�
				if (pStock->IsRecordRTData()) {
					StoreChoiceRTData(pRTData);
				}
				pStock->SetTransactionTime(pRTData->GetTransactionTime());   // �������½��ܵ�ʵʱ���ݵ�ʱ��
			}
		}
	}
	gl_ThreadStatus.SetRTDataNeedCalculate(true); // ���ý��յ�ʵʱ���ݱ�ʶ
	ASSERT(gl_WebRTDataContainer.GetSinaDataSize() == 0); // ����һ�δ���ȫ�����ݡ�
	gl_ProcessSinaRTDataQueue.Signal();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ���ݵȣ���SchedulingTaskPerSecond��������,ÿ����ִ��һ�Ρ�
// ��ʵʱ�����ݴ�����е����ݷֱ��ŵ����Թ�Ʊ��ʵʱ�����С�
// �ַ�����ʱ��ֻ�ַ��µģ�����ʱ������֮ǰ���ݵģ�ʵʱ���ݡ�
//
// �˺����õ�������ȫ�ֱ��������Ƿ������߳�Ϊ�á������߳�Ŀǰ����ֻ��������ɵĹҵ������
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskDistributeNeteaseRTDataToProperStock(void) {
	gl_ProcessNeteaseRTDataQueue.Wait();
	CChinaStockPtr pStock;
	const size_t lTotalNumber = gl_WebRTDataContainer.GetNeteaseDataSize();
	CString strVolume;
	bool fFoundNewStock = false;

	for (int iCount = 0; iCount < lTotalNumber; iCount++) {
		CWebRTDataPtr pRTData = gl_WebRTDataContainer.PopNeteaseData();
		if (pRTData->GetDataSource() == __INVALID_RT_WEB_DATA__) {
			gl_systemMessage.PushErrorMessage(_T("����ʵʱ����Դ��������"));
			continue;
		}
		if (pRTData->IsActive()) { // ��ʵʱ������Ч��
			if (m_ttNewestTransactionTime < pRTData->GetTransactionTime()) {
				m_ttNewestTransactionTime = pRTData->GetTransactionTime();
			}
			if (IsCheckActiveStock()) {
				if (!IsStock(pRTData->GetSymbol())) {
					CreateStock(pRTData->GetSymbol(), pRTData->GetStockName(), true);
					fFoundNewStock = true;
				}
			}
			ASSERT(IsStock(pRTData->GetSymbol()));
			pStock = GetStock(pRTData->GetSymbol());
			if (!pStock->IsActive()) {
				if (pRTData->IsValidTime(14)) {
					pStock->SetTodayActive(pRTData->GetSymbol(), pRTData->GetStockName());
					pStock->SetIPOStatus(__STOCK_IPOED__);
				}
			}
			if (pRTData->GetTransactionTime() > pStock->GetTransactionTime()) { // �µ����ݣ�
				pStock->PushRTData(pRTData); // �洢�µ����������ݳ�
				if (pStock->IsRecordRTData()) {
					StoreChoiceRTData(pRTData);
				}
				pStock->SetTransactionTime(pRTData->GetTransactionTime());   // �������½��ܵ�ʵʱ���ݵ�ʱ��
			}
		}
	}
	gl_ThreadStatus.SetRTDataNeedCalculate(true); // ���ý��յ�ʵʱ���ݱ�ʶ
	ASSERT(gl_WebRTDataContainer.GetNeteaseDataSize() == 0); // ����һ�δ���ȫ�����ݡ�
	gl_ProcessNeteaseRTDataQueue.Signal();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����ÿ�β�ѯ����ʵʱ��Ʊ���ݵ��ַ���
//
//////////////////////////////////////////////////////////////////////////////////////////
CString CChinaMarket::GetSinaStockInquiringStr(long lTotalNumber, bool fCheckActiveStock) {
	if (fCheckActiveStock) {
		return GetNextSinaStockInquiringMiddleStrFromTotalStockSet(m_lSinaStockRTDataInquiringIndexFromTotalStockSet, _T(","), lTotalNumber);
	}
	else {
		return GetNextStockInquiringMiddleStr(m_lSinaStockRTDataInquiringIndex, _T(","), lTotalNumber, GetTotalStock());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ����ÿ�β�ѯ��Ѷʵʱ��Ʊ���ݵ��ַ���
//
//////////////////////////////////////////////////////////////////////////////////////////
CString CChinaMarket::GetTengxunInquiringStockStr(long lTotalNumber, long lEndPosition) {
	ASSERT(IsSystemReady());
	return GetNextStockInquiringMiddleStr(m_lTengxunRTDataInquiringIndex, _T(","), lTotalNumber, lEndPosition);
}

CString CChinaMarket::GetNeteaseStockInquiringMiddleStr(long lTotalNumber, long lEndPosition, bool fCheckActiveStock) {
	if (fCheckActiveStock) {
		return GetNextNeteaseStockInquiringMiddleStrFromTotalStockSet(_T(","), lTotalNumber);
	}
	else {
		return GetNextNeteaseStockInquiringStr(lTotalNumber, lEndPosition);
	}
}

CString CChinaMarket::GetNextNeteaseStockInquiringStr(long lTotalNumber, long lEndPosition) {
	CString strStockCode, strRight6, strLeft2, strPrefix;

	m_strNeteaseRTDataInquiringStr = _T("");
	m_strNeteaseRTDataInquiringStr = XferStandredToNetease(m_vChinaMarketStock.at(m_lNeteaseRTDataInquiringIndex)->GetSymbol());
	IncreaseStockInquiringIndex(m_lNeteaseRTDataInquiringIndex, lEndPosition);
	int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
	while ((m_lNeteaseRTDataInquiringIndex < lEndPosition) && (iCount < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
		iCount++;
		m_strNeteaseRTDataInquiringStr += _T(",");
		m_strNeteaseRTDataInquiringStr += XferStandredToNetease(m_vChinaMarketStock.at(m_lNeteaseRTDataInquiringIndex)->GetSymbol());
		if (m_lNeteaseRTDataInquiringIndex == 0) break;
		IncreaseStockInquiringIndex(m_lNeteaseRTDataInquiringIndex, lEndPosition);
	}
	if (m_lNeteaseRTDataInquiringIndex > 0) m_lNeteaseRTDataInquiringIndex--;// �˺�һ������ֹ���һ����Ʊ��ѯ������ʵ����Ҫ�ˣ�

	return m_strNeteaseRTDataInquiringStr;
}

bool CChinaMarket::CheckValidOfNeteaseDayLineInquiringStr(CString str) {
	CString strStockCode, strNetease;

	strNetease = str.Left(7);
	strStockCode = XferNeteaseToStandred(strNetease);
	if (!IsStock(strStockCode)) {
		CString strReport = _T("�������߲�ѯ��Ʊ�������");
		TRACE(_T("�������߲�ѯ��Ʊ�������%s\n"), strStockCode.GetBuffer());
		strReport += strStockCode;
		gl_systemMessage.PushInnerSystemInformationMessage(strReport);
		return false;
	}

	return true;
}

CString CChinaMarket::GetNextStockInquiringMiddleStr(long& iStockIndex, CString strPostfix, long lTotalNumber, long lEndPosition) {
	CString strReturn = _T("");
	CString strStockCode, strStockExchange, strStockSymbol;

	if (0 == lEndPosition) return _T("sh600000"); // ��û��֤ȯ�ɲ�ѯʱ������һ����Ч�ַ���
	strReturn = XferStandredToSina(m_vChinaMarketStock.at(iStockIndex)->GetSymbol());  // �õ���һ����Ʊ����
	IncreaseStockInquiringIndex(iStockIndex, lEndPosition);
	int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
	while ((iStockIndex < lEndPosition) && (iCount < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
		iCount++;
		strReturn += strPostfix;
		strReturn += XferStandredToSina(m_vChinaMarketStock.at(iStockIndex)->GetSymbol());  // �õ���һ����Ʊ����
			// ÿ�β鵽���ʱ��ͣһ�¡�Ŀǰ��ʹ��֮���Ѽӿ��ѯ�ٶ�
		// if (iStockIndex == lStartPosition) break;
		IncreaseStockInquiringIndex(iStockIndex, lEndPosition);
	}
	if (iStockIndex > 0) iStockIndex--; // �˺�һ������ֹ���һ����Ʊ��ѯ������ʵ����Ҫ�ˣ�

	return strReturn;
}

CString CChinaMarket::GetNextSinaStockInquiringMiddleStrFromTotalStockSet(long& lStockIndex, CString strPostfix, long lTotalNumber) {
	CString strReturn = _T("");
	long lEndPosition = m_vTotalStockSet.size();

	if (0 == lEndPosition) return _T("sh600000"); // ��û��֤ȯ�ɲ�ѯʱ������һ����Ч�ַ���
	strReturn = XferStandredToSina(m_vTotalStockSet.at(lStockIndex));  // �õ���һ����Ʊ����
	IncreaseStockInquiringIndex(lStockIndex, lEndPosition);
	int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
	while ((lStockIndex < lEndPosition) && (iCount < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
		iCount++;
		strReturn += strPostfix;
		strReturn += XferStandredToSina(m_vTotalStockSet.at(lStockIndex));
		IncreaseStockInquiringIndex(lStockIndex, lEndPosition);
	}
	if (lStockIndex > 0) lStockIndex--; // �˺�һ������ֹ���һ����Ʊ��ѯ������ʵ����Ҫ�ˣ�

	return strReturn;
}

CString CChinaMarket::GetNextNeteaseStockInquiringMiddleStrFromTotalStockSet(CString strPostfix, long lTotalNumber) {
	CString strReturn;
	long lEndPosition = m_vTotalStockSet.size();
	static long s_lIndex = 0;

	if (0 == lEndPosition) return _T("0600000"); // ��û��֤ȯ�ɲ�ѯʱ������һ����Ч�ַ���
	strReturn = XferStandredToNetease(m_vTotalStockSet.at(s_lIndex));  // �õ���һ����Ʊ����
	IncreaseStockInquiringIndex(s_lIndex, lEndPosition);
	int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
	while ((s_lIndex < lEndPosition) && (iCount < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
		iCount++;
		strReturn += strPostfix;
		strReturn += XferStandredToNetease(m_vTotalStockSet.at(s_lIndex));  // �õ���һ����Ʊ����
		IncreaseStockInquiringIndex(s_lIndex, lEndPosition);
	}
	if (s_lIndex > 0) s_lIndex--; // �˺�һ������ֹ���һ����Ʊ��ѯ������ʵ����Ҫ�ˣ�

	return strReturn;
}

////////////////////////////////////////////////////////////////////////////////////////
//
// �ɹ����߳�ThreadCalculatingRTDataProc���ã�ע��ȫ�ֱ�����ʹ��
//
// Ŀǰ���õķ�ʽ��ֱ�Ӽ������߳��У������ù����߳�ģʽ���á�
//
/////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskProcessRTData(void) {
	ASSERT(gl_ThreadStatus.IsRTDataNeedCalculate() && !gl_ThreadStatus.IsSavingTempData());// �������̻߳���
	for (auto& pStock : m_vChinaMarketStock) {
		if (pStock->IsActive()) {
			pStock->ProcessRTData();
		}
	}
	return true;
}

bool CChinaMarket::TaskProcessWebRTDataGetFromSinaServer(void) {
	CWebDataPtr pWebDataReceived = nullptr;
	const size_t lTotalData = gl_WebInquirer.GetSinaRTDataSize();
	for (int i = 0; i < lTotalData; i++) {
		pWebDataReceived = gl_WebInquirer.PopSinaRTData();
		pWebDataReceived->ResetCurrentPos();
		while (!pWebDataReceived->IsProcessedAllTheData()) {
			CWebRTDataPtr pRTData = make_shared<CWebRTData>();
			if (pRTData->ReadSinaData(pWebDataReceived)) {
				m_llRTDataReceived++;
				gl_WebRTDataContainer.PushSinaData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
			}
			else return false;  // ��������ݳ����⣬�׵����á�
		}
	}
	return true;
}

void CChinaMarket::StoreChoiceRTData(CWebRTDataPtr pRTData) {
	m_qRTData.push(pRTData);
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
// ����ʵʱ����ȱ�ٹؼ��Եĳɽ����һ��ʶ��޷���Ϊ�������ݣ�ֻ����Ϊ������.��turnover��Ϊ�ɽ�������ʹ��֮��05/12/2020��
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskProcessWebRTDataGetFromNeteaseServer(void) {
	CWebDataPtr pWebDataReceived = nullptr;
	int iCount = 0;

	const size_t lTotalData = gl_WebInquirer.GetNeteaseRTDataSize();
	for (int i = 0; i < lTotalData; i++) {
		pWebDataReceived = gl_WebInquirer.PopNeteaseRTData();
		pWebDataReceived->ResetCurrentPos();
		if (!IsInvalidNeteaseRTData(*pWebDataReceived)) {
			if (!IsValidNeteaseRTDataPrefix(*pWebDataReceived)) return false;
			iCount = 0;
			while (!((pWebDataReceived->GetCurrentPosData() == ' ') || (pWebDataReceived->GetCurrentPos() >= (pWebDataReceived->GetBufferLength() - 4)))) {
				CWebRTDataPtr pRTData = make_shared<CWebRTData>();
				if (pRTData->ReadNeteaseData(pWebDataReceived)) {// ���һ��
					CheckNeteaseRTDataValidation(*pRTData);
					iCount++;
					m_llRTDataReceived++;
					gl_WebRTDataContainer.PushNeteaseData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
				}
				else return false;  // ��������ݳ����⣬�׵����á�
			}
			//TRACE(_T("ReadNetease��������,��������%d������\n"), iCount);
		}
	}

	return true;
}

bool CChinaMarket::IsInvalidNeteaseRTData(CWebData& WebDataReceived) {
	char buffer[50];
	char* pBuffer = buffer;
	CString strInvalidStock = _T("_ntes_quote_callback({ });"); // ��Ϊ��Ч��Ʊ��ѯ�������ݸ�ʽ����26���ַ�
	WebDataReceived.GetData(pBuffer, 26, WebDataReceived.GetCurrentPos());
	buffer[26] = 0x000;
	CString str1 = buffer;

	if (str1.Compare(strInvalidStock) == 0) {
		ASSERT(WebDataReceived.GetBufferLength() == 26);
		return true;
	}
	else return false;
}

bool CChinaMarket::IsValidNeteaseRTDataPrefix(CWebData& WebDataReceived) {
	char buffer[50];
	CString strInvalidStock = _T("_ntes_quote_callback("); // ��Ϊ��Ч��Ʊ��ѯ�������ݸ�ʽ����22���ַ�

	WebDataReceived.GetData(buffer, 21, WebDataReceived.GetCurrentPos()); // ����"_ntes_quote_callback("
	buffer[21] = 0x000;
	CString str1;
	str1 = buffer;
	if (strInvalidStock.Compare(str1) != 0) { // ���ݸ�ʽ����
		return false;
	}
	else {
		WebDataReceived.IncreaseCurrentPos(21);
		return true;
	}
}

bool CChinaMarket::CheckNeteaseRTDataValidation(CWebRTData& RTData) {
	// ���һ��
	CString str;

	ASSERT(RTData.GetDataSource() == __NETEASE_RT_WEB_DATA__);
	if (RTData.IsActive()) {
		CChinaStockPtr pStock = nullptr;
		if (IsStock(RTData.GetSymbol())) {
			pStock = GetStock(RTData.GetSymbol());
			if (!pStock->IsActive()) {
				str = pStock->GetSymbol();
				str += _T(" ����ʵʱ��⵽�����ڻ�Ծ״̬");
				gl_systemMessage.PushInnerSystemInformationMessage(str);
				return false;
			}
		}
		else {
			str = RTData.GetSymbol();
			str += _T(" ��Ч��Ʊ���루����ʵʱ���ݣ�");
			TRACE("\n��Ч��Ʊ����%s\n", RTData.GetSymbol().GetBuffer());
			TRACE("����Ĺ�Ʊ��Ϊ�� %s\n\n", m_strNeteaseRTDataInquiringStr.GetBuffer());
			gl_systemMessage.PushInnerSystemInformationMessage(str);
			return false;
		}
		return true;
	}
	return false;
}

bool CChinaMarket::TaskDiscardNeteaseRTData(void) {
	CWebRTDataPtr pRTData = nullptr;
	const size_t lTotalData = gl_WebRTDataContainer.GetNeteaseDataSize();

	for (int i = 0; i < lTotalData; i++) {
		// Ŀǰ��ʹ������ʵʱ���ݣ�����ֻ�Ǽ򵥵�ȡ�����ӵ���
		pRTData = gl_WebRTDataContainer.PopNeteaseData();
		pRTData = nullptr;
	}

	return true;
}

bool CChinaMarket::TaskDiscardSinaRTData(void) {
	CWebRTDataPtr pRTData = nullptr;
	const size_t lTotalData = gl_WebRTDataContainer.GetSinaDataSize();

	for (int i = 0; i < lTotalData; i++) {
		// Ŀǰ��ʹ������ʵʱ���ݣ�����ֻ�Ǽ򵥵�ȡ�����ӵ���
		pRTData = gl_WebRTDataContainer.PopSinaData();
		pRTData = nullptr;
	}

	return true;
}

bool CChinaMarket::TaskDiscardTengxunRTData(void) {
	CWebRTDataPtr pRTData = nullptr;
	const size_t lTotalData = gl_WebRTDataContainer.GetTengxunDataSize();

	for (int i = 0; i < lTotalData; i++) {
		// Ŀǰ��ʹ������ʵʱ���ݣ�����ֻ�Ǽ򵥵�ȡ�����ӵ���
		pRTData = gl_WebRTDataContainer.PopTengxunData();
		pRTData = nullptr;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ��Ѷʵʱ���ݣ������������ѯ��Ʊ����Ϊ������ʱ��ֻ�Ǽ��Թ������������ݡ��ʶ���ѯ900����Ʊ�����ص�������ҪС��900.
// ֻ�е����еĲ�ѯ��Ʊ��Ϊ������ʱ���ŷ���һ��21���ַ�����v_pv_none_match=\"1\";\n
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskProcessWebRTDataGetFromTengxunServer(void) {
	CWebDataPtr pWebDataReceived = nullptr;
	int j = 0;

	const size_t lTotalData = gl_WebInquirer.GetTengxunRTDataSize();
	for (int i = 0; i < lTotalData; i++) {
		pWebDataReceived = gl_WebInquirer.PopTengxunRTData();
		pWebDataReceived->ResetCurrentPos();
		if (!IsInvalidTengxunRTData(*pWebDataReceived)) { // ������21���ַ����ĺ������Է������Ҳ���Է�����ǰ�档
			j = 0;
			while (!pWebDataReceived->IsProcessedAllTheData()) {
				CWebRTDataPtr pRTData = make_shared<CWebRTData>();
				if (pRTData->ReadTengxunData(pWebDataReceived)) {
					CheckTengxunRTDataValidation(*pRTData); // ���һ��
					j++;
					gl_WebRTDataContainer.PushTengxunData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
				}
				else return false;  // ��������ݳ����⣬�׵����á�
			}
			//TRACE(_T("���յ�%d����Ѷʵʱ����\n"), j);
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����б���ѯ�Ĺ�Ʊ��Ϊ�����й�Ʊʱ����Ѷʵʱ��Ʊ�������᷵��һ��21���ַ������ַ�����v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::IsInvalidTengxunRTData(CWebData& WebDataReceived) {
	char buffer[50];
	char* pBuffer = buffer;
	CString strInvalidStock = _T("v_pv_none_match=\"1\";\n"); // ��Ϊ��Ч��Ʊ��ѯ�������ݸ�ʽ����21���ַ�

	WebDataReceived.GetData(pBuffer, 21, WebDataReceived.GetCurrentPos());
	buffer[21] = 0x000;
	CString str1 = buffer;

	if (str1.Compare(strInvalidStock) == 0) {
		ASSERT(WebDataReceived.GetBufferLength() == 21);
		return true;
	}
	else return false;
}

bool CChinaMarket::CheckTengxunRTDataValidation(CWebRTData& RTData) {
	CString str;
	ASSERT(RTData.GetDataSource() == __TENGXUN_RT_WEB_DATA__);
	if (RTData.IsActive()) {
		CChinaStockPtr pStock = nullptr;
		if (IsStock(RTData.GetSymbol())) {
			pStock = GetStock(RTData.GetSymbol());
			if (!pStock->IsActive()) {
				str = pStock->GetSymbol();
				str += _T("��Ѷʵʱ��⵽�����ڻ�Ծ״̬");
				//gl_systemMessage.PushInnerSystemInformationMessage(str);
				return false;
			}
		}
		else {
			str = RTData.GetSymbol();
			str += _T("��Ч��Ʊ���루��Ѷʵʱ���ݣ�");
			gl_systemMessage.PushInnerSystemInformationMessage(str);
			return false;
		}
		return true;
	}
	return false;
}

bool CChinaMarket::TaskProcessTengxunRTData(void) {
	CWebRTDataPtr pRTData = nullptr;
	const size_t lTotalData = gl_WebRTDataContainer.GetTengxunDataSize();

	for (int i = 0; i < lTotalData; i++) {
		pRTData = gl_WebRTDataContainer.PopTengxunData();
		if (pRTData->IsActive()) {
			ASSERT(IsStock(pRTData->GetSymbol()));
			auto pStock = GetStock(pRTData->GetSymbol());
			pStock->SetTotalValue(pRTData->GetTotalValue());
			pStock->SetCurrentValue(pRTData->GetCurrentValue());
			pStock->SetHighLimit(pRTData->GetHighLimit());
			pStock->SetLowLimit(pRTData->GetLowLimit());
		}
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//
// ��Լÿ100�������һ��
//
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::SchedulingTask(void) {
	CVirtualMarket::SchedulingTask();

	static time_t s_timeLast = 0;
	const long lCurrentTime = GetMarketTime();

	// ץȡʵʱ����(���ˡ���Ѷ�����ף���ÿ400��������һ�Σ����ɱ�֤��3�����ڱ���һ��ȫ���Ծ��Ʊ��
	if (m_fGetRTData && (m_iCountDownSlowReadingRTData <= 0)) {
		TaskGetRTDataFromWeb();
		TaskProcessWebRTDataGetFromSinaServer();
		TaskProcessWebRTDataGetFromNeteaseServer();
		// ���Ҫ�����ٶ�ȡʵʱ���ݣ������ö�ȡ����Ϊÿ����һ��
		if (!m_fFastReceivingRTData && IsSystemReady()) m_iCountDownSlowReadingRTData = 100 * m_iRTDataInquiryTickNumber; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
		else m_iCountDownSlowReadingRTData = m_iRTDataInquiryTickNumber;  // Ĭ�ϼ���4��,��ÿ400��������һ��ʵʱ����
	}
	m_iCountDownSlowReadingRTData--;

	//����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
	if (GetUTCTime() > s_timeLast) {
		SchedulingTaskPerSecond(GetUTCTime() - s_timeLast, lCurrentTime);
		s_timeLast = GetUTCTime();
	}

	// ϵͳ׼������֮����Ҫ��ɵĸ����
	if (IsSystemReady()) {
		if (!m_fTodayTempDataLoaded) { // �˹���������һ�Ρ�
			LoadTodayTempDB(GetMarketDate());
			m_fTodayTempDataLoaded = true;
		}
		TaskProcessWebRTDataGetFromTengxunServer();
		TaskGetNeteaseDayLineFromWeb();
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////
//
// �����ˡ����׻�����Ѷʵʱ�������ݷ�������ȡʵʱ���ݡ�ʹ������֮һ���ɡ�
//
/////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskGetRTDataFromWeb(void) {
	switch (m_iRTDataServer) {
	case 0: // ʹ������ʵʱ���ݷ�����
		if (IsUsingSinaRTDataReceiver()) {
			gl_WebInquirer.GetSinaRTData(); // ÿ400����(100X4)����һ��ʵʱ���ݡ����˵�ʵʱ�����������Ӧʱ�䲻����100���루30-70֮�䣩����û�г��ֹ����ݴ���
		}
		break;
	case 1: // ʹ������ʵʱ���ݷ�����
		// ����ʵʱ�����д�����ȱʧ�ֶΣ���ǰ׺��׺Ҳ��ʱȱʧ��
		// ����ʵʱ������ʱ������û��Ҫ����Ĺ�Ʊ����֪Ϊ�Ρ�
		if (IsUsingNeteaseRTDataReceiver()) {
			// ��ȡ����ʵʱ�������ݡ���������ʵʱ���������˵�����Դ��ͬ���ʶ����߿ɻ�����ʹ����һ���ɡ�
			gl_WebInquirer.GetNeteaseRTData();
		}
		break;
	default: // ����
		break;
	}

	if (IsSystemReady()) {
		// ��ȡ��Ѷʵʱ�������ݡ� ������Ѷʵʱ����Ĺ�������Ϊ�֣�û�������Ϣ�������޷�������ʵʱ�������ݶԽӣ����˾���Ϊ�ɣ����ʶ���ʱ����
		if (IsUsingTengxunRTDataReceiver()) {
			if (m_iCountDownTengxunNumber <= 0) {
				gl_WebInquirer.GetTengxunRTData();// ֻ�е�ϵͳ׼����Ϻ󣬷���ִ�ж�ȡ��Ѷʵʱ�������ݵĹ�����Ŀǰ��ʹ�ô˹���
				m_iCountDownTengxunNumber = 5;
			}
			else m_iCountDownTengxunNumber--; // ����ʵʱ���ݶ�ȡ��Σ���Ѷ�Ŷ�ȡһ�Ρ���Ϊ��Ѷ�Ĺҵ��������õ���ÿ�ֱ�׼�����Ȳ���
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
bool CChinaMarket::SchedulingTaskPerSecond(long lSecond, long lCurrentTime) {
	static int s_iCountDownProcessWebRTData = 0;
	static int s_iCount1Hour = 3599;
	static int s_iCount5Minute = 299;
	static int s_iCount1Minute = 59;
	static int s_iCount10Second = 9;

	s_iCount10Second -= lSecond;
	s_iCount1Minute -= lSecond;
	s_iCount5Minute -= lSecond;
	s_iCount1Hour -= lSecond;
	if (s_iCount1Hour < 0) {
		s_iCount1Hour = 3599;
		SchedulingTaskPerHour(lCurrentTime);
	}
	if (s_iCount5Minute < 0) {
		s_iCount5Minute = 299;
		SchedulingTaskPer5Minutes(lCurrentTime);
	}
	if (s_iCount1Minute < 0) {
		s_iCount1Minute = 59;
		SchedulingTaskPerMinute(lCurrentTime);
	}
	if (s_iCount10Second < 0) {
		s_iCount10Second = 9;
		SchedulingTaskPer10Seconds(lCurrentTime);
	}

	CheckMarketReady(); // ����г��Ƿ���ɳ�ʼ��

	if ((GetDayLineNeedUpdateNumber() <= 0) && (GetDayLineNeedSaveNumber() <= 0) && m_fCalculateChoiced10RS) {
		TaskChoice10RSStrongStockSet(lCurrentTime);
		TaskChoice10RSStrong1StockSet(lCurrentTime);
		TaskChoice10RSStrong2StockSet(lCurrentTime);
	}

	// �ж��Ƿ�ʼ�����ռ�����
	TaskCheckStartReceivingData(lCurrentTime);
	// �ж��й���Ʊ�г�����״̬
	TaskCheckMarketOpen(lCurrentTime);

	if (s_iCountDownProcessWebRTData <= 0) {
		// �����յ���ʵʱ���ݷַ�������ع�Ʊ��ʵʱ���ݶ����С�
		// �����ж������Դ���ʶ���Ҫ�ȴ�������Դ��ִ��һ�κ󣬷����Էַ�����ع�Ʊ�����ʶ���Ҫÿ����ִ��һ�Σ��Ա�֤������Դ���ٶ����ṩһ�����ݡ�
		TaskDistributeSinaRTDataToProperStock();
		// �ַ�����ʵʱ����������ع�Ʊ�С�
		TaskDistributeNeteaseRTDataToProperStock();

		TaskProcessTengxunRTData();

		TaskDiscardNeteaseRTData();
		TaskDiscardSinaRTData();
		TaskDiscardTengxunRTData();
		s_iCountDownProcessWebRTData = 0;
	}
	else s_iCountDownProcessWebRTData--;

	// ����ʵʱ���ݣ�ÿ����һ�Ρ�Ŀǰ����ʵʱ����Ϊÿ3����һ�θ��£��ʶ������ٿ��ˡ�
	// �˼�������Ҫ��DistributeRTDataReceivedFromWebToProperStock֮��ִ�У��Է�ֹ����ͬ�����⡣
	// ��ϵͳ�洢��ʱ����ʱ����ͬʱ����ʵʱ���ݣ��������׳���ͬ�����⡣���ϵͳ���ڴ洢��ʱʵʱ���ݣ���ȴ�һ������һ����ѯʱ�ټ���ʵʱ����
	if (IsSystemReady() && !gl_ThreadStatus.IsSavingTempData() && IsTodayTempRTDataLoaded()) {
		if (gl_ThreadStatus.IsRTDataNeedCalculate()) {
			gl_ThreadStatus.SetCalculatingRTData(true);
			TaskProcessRTData();
			gl_ThreadStatus.SetRTDataNeedCalculate(false);
			gl_ThreadStatus.SetCalculatingRTData(false);
		}
	}

	TaskShowCurrentTransaction();

	// װ�ص�ǰ��Ʊ��������
	TaskLoadCurrentStockHistoryData();

	return true;
}

bool CChinaMarket::SchedulingTaskPerHour(long lCurrentTime) {
	// ����ÿһСʱһ�ε�����
	return true;
}

bool CChinaMarket::SchedulingTaskPer5Minutes(long lCurrentTime) {
	// ����ÿ�����һ�ε�����

	TaskUpdateOptionDB();
	TaskUpdateStockCodeDB();

	if (IsSavingTempData()) {
		TaskSaveTempDataIntoDB(lCurrentTime);
	}

	return true;
}

void CChinaMarket::TaskSaveTempDataIntoDB(long lCurrentTime) {
	// ����ʱÿ����Ӵ洢һ�ε�ǰ״̬������һ�����ô�ʩ����ֹ�˳�ϵͳ��Ͷ��������е����ݣ�����̫Ƶ����
	if (IsSystemReady() && m_fMarketOpened && !gl_ThreadStatus.IsCalculatingRTData()) {
		if (((lCurrentTime > 93000) && (lCurrentTime < 113600)) || ((lCurrentTime > 130000) && (lCurrentTime < 150600))) { // �洢��ʱ�����ϸ��ս���ʱ����ȷ��(�м������ڼ�ͱ��к��Ҫ�洢һ�Σ��ʶ���11:36��15:06����ֹ��
			CString str;
			str = _T("�洢��ʱ����");
			gl_systemMessage.PushDayLineInfoMessage(str);
			UpdateTempRTData();
		}
	}
}

bool CChinaMarket::AddChoicedStock(CChinaStockPtr pStock) {
	auto it = find(m_avChoicedStock.at(0).cbegin(), m_avChoicedStock.at(0).cend(), pStock);
	if (it == m_avChoicedStock.at(0).end()) {
		m_avChoicedStock.at(0).push_back(pStock);
		ASSERT(!pStock->IsSaveToChoicedStockDB());
		return true;
	}
	return false;
}

bool CChinaMarket::DeleteChoicedStock(CChinaStockPtr pStock) {
	auto it = find(m_avChoicedStock.at(0).cbegin(), m_avChoicedStock.at(0).cend(), pStock);
	if (it == m_avChoicedStock.at(0).end()) {
		return false;
	}
	else {
		(*it)->SetChoiced(false);
		(*it)->SetSaveToChoicedStockDB(false);
		m_avChoicedStock.at(0).erase(it);
		return true;
	}
}

void CChinaMarket::ClearDayLineNeedUpdateStatus(void) {
	for (auto& pStock : m_vChinaMarketStock) {
		pStock->SetDayLineNeedUpdate(false);
	}
}

bool CChinaMarket::IsUpdateStockCodeDB(void) {
	for (auto& pStock : m_vChinaMarketStock) {
		if (pStock->IsUpdateProfileDB()) return true;
	}
	return false;
}

bool CChinaMarket::SchedulingTaskPerMinute(long lCurrentTime) {
	// ����ÿ����һ�ε��������еĶ�ʱ����Ҫ����ʱ�����ӳ��������У�����ִ��ÿ����һ�ε�������ִ��ÿ����һ�ε����������ܹ���֤���������������ִ�С�
	TaskResetMarket(lCurrentTime);
	TaskResetMarketAgain(lCurrentTime);

	// �ڿ���ǰ��������ͣʱ��ѯ���й�Ʊ�أ��ҵ������Ծ��Ʊ��
	TaskSetCheckActiveStockFlag(lCurrentTime);

	// �����������ֿ�ʼ������ʵʱ���ݡ�
	TaskProcessTodayStock(lCurrentTime);

	TaskSaveChoicedRTData();

	TaskClearChoicedRTDataSet(lCurrentTime);

	TaskUpdateChoicedStockDB();

	TaskCheckDayLineDB();

	if (m_fUpdateStockSection) {
		TaskSaveStockSection();
		m_fUpdateStockSection = false;
	}

	return true;
}

bool CChinaMarket::TaskSetCheckActiveStockFlag(long lCurrentTime) {
	if (!IsSystemReady()) {
		m_fCheckActiveStock = true;
		return true;
	}
	if (((lCurrentTime >= 91500) && (lCurrentTime < 92700))
		|| ((lCurrentTime >= 113300) && (lCurrentTime < 125900))
		|| (lCurrentTime > 150300)) {
		m_fCheckActiveStock = true;
		return true;
	}
	else {
		m_fCheckActiveStock = false;
		return false;
	}
}

bool CChinaMarket::TaskChoice10RSStrong1StockSet(long lCurrentTime) {
	if (IsSystemReady() && !m_fChoiced10RSStrong1StockSet && (lCurrentTime > 151100) && IsWorkingDay()) {
		CreatingThreadChoice10RSStrong1StockSet();
		m_fChoiced10RSStrong1StockSet = true;
		return true;
	}
	return false;
}

bool CChinaMarket::TaskChoice10RSStrong2StockSet(long lCurrentTime) {
	if (IsSystemReady() && !m_fChoiced10RSStrong2StockSet && (lCurrentTime > 151200) && IsWorkingDay()) {
		CreatingThreadChoice10RSStrong2StockSet();
		m_fChoiced10RSStrong2StockSet = true;
		return true;
	}
	return false;
}

bool CChinaMarket::TaskChoice10RSStrongStockSet(long lCurrentTime) {
	if (IsSystemReady() && !m_fChoiced10RSStrongStockSet && (lCurrentTime > 151000) && IsWorkingDay()) {
		CreatingThreadChoice10RSStrongStockSet();
		m_fChoiced10RSStrongStockSet = true;
		return true;
	}
	return false;
}

bool CChinaMarket::TaskProcessTodayStock(long lCurrentTime) {
	if (IsSystemReady() && (lCurrentTime >= 150400) && IsTodayStockNotProcessed() && IsWorkingDay()) {
		CreatingThreadProcessTodayStock();
		return true;
	}
	return false;
}

bool CChinaMarket::TaskCheckDayLineDB(void) {
	if (m_fSaveDayLine && (!IsDayLineNeedSaving()) && (!IsDayLineNeedUpdate()) && (!IsDayLineNeedProcess())) {
		m_fSaveDayLine = false;
		TRACE("������ʷ���ݸ������\n");
		CString str;
		str = _T("�й��г�������ʷ���ݸ������");
		gl_systemMessage.PushInformationMessage(str);
		if (IsDayLineDBUpdated()) {
			// ���¹�Ʊ�����ݿ�
			ClearDayLineDBUpdatedFlag();
		}
		return true;
	}
	return false;
}

bool CChinaMarket::TaskCheckStartReceivingData(long lCurrentTime) {
	if (!IsWorkingDay()) { //�����������ձ��С��ṹtm��0--6��ʾ��������������
		m_fFastReceivingRTData = false;
		return(m_fFastReceivingRTData);
	}
	else if ((lCurrentTime < 91200) || (lCurrentTime > 150630) || ((lCurrentTime > 114500) && (lCurrentTime < 124500))) { //��������������ʮ���г����׽�����Ϊ�˱�֤���һ����ʱ���ݵĴ洢��
		m_fFastReceivingRTData = false;

		return(m_fFastReceivingRTData);
	}
	else m_fFastReceivingRTData = true;
	return m_fFastReceivingRTData;
}

bool CChinaMarket::TaskCheckMarketOpen(long lCurrentTime) {
	if (!IsWorkingDay()) { //�����������ձ��С��ṹtm��0--6��ʾ��������������
		m_fMarketOpened = false;
	}
	else if ((lCurrentTime > 92800) && (lCurrentTime < 150600)) { // �г������������ݵ�ʱ�䣬�Զ�Ϊ150600����ֹͣ�洢��ʱ���ݵ�ʱ��һ����
		m_fMarketOpened = true;
	}
	else m_fMarketOpened = false;

	return m_fMarketOpened;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���������ÿ���ӵ���һ�Σ�����ʵ�ֻ��Ʋ�����
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskResetMarket(long lCurrentTime) {
	// �ŵ�ʮ��������ϵͳ
// �����ڴ�ʱ������������������Ļ����׳������ݲ�ȫ�����⡣
	if (IsPermitResetMarket()) { // �����������ϵͳ
		if ((lCurrentTime >= 91300) && (lCurrentTime < 91400) && IsWorkingDay()) { // �����վŵ�ʮ�������ϵͳ
			if (!TooManyStockDayLineNeedUpdate()) { // ���й�������Ϊ�ݼ��պ����е��������ݶ���Ҫ���һ�飬��ʱ����0915ʱ����ϵͳ�Ա���������ߺ�������ִ�С�
				SetResetMarket(true);// ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
				SetSystemReady(false);
			}
		}
	}
	return true;
}

bool CChinaMarket::TaskResetMarketAgain(long lCurrentTime) {
	// �ŵ��ʮ����ٴ�����ϵͳ
	if (IsPermitResetMarket()) { // �����������ϵͳ
		if ((lCurrentTime >= 92500)) {
			if ((lCurrentTime <= 93000) && IsWorkingDay()) { // �����վŵ��ʮ����ٴ�����ϵͳ
				SetResetMarket(true);// ֻ������������ʶ��ʵ������������CMainFrame��OnTimer������ɡ�
				SetSystemReady(false);
			}
			SetPermitResetMarket(false); // ���첻����������ϵͳ��
		}
	}
	return true;
}

bool CChinaMarket::TaskUpdateStockCodeDB(void) {
	if (IsUpdateStockCodeDB()) {
		CreatingThreadUpdateStockCodeDB();
		return true;
	}
	return false;
}

bool CChinaMarket::TaskUpdateOptionDB(void) {
	CreatingThreadUpdateOptionDB();
	return true;
}

bool CChinaMarket::TaskUpdateChoicedStockDB(void) {
	if (IsUpdateChoicedStockDB()) {
		CreatingThreadAppendChoicedStockDB();
		return true;
	}
	return false;
}

bool CChinaMarket::TaskShowCurrentTransaction(void) {
	// ��ʾ��ǰ�������
	CChinaStockPtr pCurrentStock = GetCurrentStock();

	if (pCurrentStock != nullptr) {
		if (pCurrentStock->IsRTDataCalculated()) {
			if (pCurrentStock->GetCurrentTransationVolume() > 0) pCurrentStock->ReportGuadanTransaction();
			pCurrentStock->ReportGuadan();
			pCurrentStock->SetRTDataCalculated(false);
		}
	}

	return true;
}

bool CChinaMarket::TaskSaveChoicedRTData(void) {
	if (IsSystemReady() && m_fSaveRTData) {
		CreatingThreadSaveChoicedRTData();
		return true;
	}
	else return false;
}

/////////////////////////////////////////////////////////////////////
//
// �ŵ��ʮ������ŵ���ʮ����������յ�ʵʱ���ݡ�
//
/////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskClearChoicedRTDataSet(long lCurrentTime) {
	if (lCurrentTime > 93100) {
		m_fRTDataSetCleared = true;
	}

	if (!m_fRTDataSetCleared) {
		if ((lCurrentTime > 92900) && (lCurrentTime < 93100)) {
			CSetRealTimeData setRTData;
			setRTData.Open();
			setRTData.m_pDatabase->BeginTrans();
			while (!setRTData.IsEOF()) {
				setRTData.Delete();
				setRTData.MoveNext();
			}
			setRTData.m_pDatabase->CommitTrans();
			setRTData.Close();

			m_fRTDataSetCleared = true;
		}
	}
	return true;
}

bool CChinaMarket::TaskSaveStockSection(void) {
	CreatingThreadSaveStockSection();
	return true;
}

bool CChinaMarket::SaveStockSection(void) {
	CSetStockSection setStockSection;

	setStockSection.Open();
	setStockSection.m_pDatabase->BeginTrans();
	while (!setStockSection.IsEOF()) {
		setStockSection.Delete();
		setStockSection.MoveNext();
	}
	setStockSection.m_pDatabase->CommitTrans();
	setStockSection.Close();

	CStockSectionPtr pStockSection = nullptr;

	setStockSection.Open();
	setStockSection.m_pDatabase->BeginTrans();
	for (int i = 0; i < 2000; i++) {
		pStockSection = m_vStockSection.at(i);
		setStockSection.AddNew();
		setStockSection.m_ID = i;
		setStockSection.m_Active = pStockSection->IsActive();
		setStockSection.m_Market = pStockSection->GetMarket();
		setStockSection.m_IndexNumber = pStockSection->GetIndexNumber();
		setStockSection.m_Comment = pStockSection->GetComment();
		setStockSection.Update();
	}
	setStockSection.m_pDatabase->CommitTrans();
	setStockSection.Close();

	m_fUpdateStockSection = false;
	return true;
}

bool CChinaMarket::ChangeDayLineStockCodeToStandred(void) {
	CSetDayLineExtendInfo setDayLineExtendInfo;

	setDayLineExtendInfo.Open(AFX_DB_USE_DEFAULT_TYPE, NULL, CRecordset::executeDirect);
	setDayLineExtendInfo.m_pDatabase->BeginTrans();
	while (!setDayLineExtendInfo.IsEOF()) {
		setDayLineExtendInfo.Edit();
		setDayLineExtendInfo.m_Symbol = XferSinaToStandred(setDayLineExtendInfo.m_Symbol);
		setDayLineExtendInfo.Update();
		setDayLineExtendInfo.MoveNext();
	}
	setDayLineExtendInfo.m_pDatabase->CommitTrans();
	setDayLineExtendInfo.Close();
	gl_systemMessage.PushInformationMessage("Updated dayline extend info");
	return false;
}

bool CChinaMarket::SchedulingTaskPer10Seconds(long lCurrentTime) {
	// ����ÿʮ����һ�ε�����
		// ������������ʷ���ݵĺ�����Ϊ��ʱ��ѯ����ȡ�ʹ洢���ù������̡�
	if (IsDayLineNeedProcess()) {
		TaskProcessDayLineGetFromNeeteaseServer();
	}

	// �ж��Ƿ�洢���߿�͹�Ʊ�����
	if (IsDayLineNeedSaving()) {
		m_fSaveDayLine = true;
		TaskSaveDayLineData();
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// �õ���Ʊ�ļ��
//
//////////////////////////////////////////////////////////////////////////////////////////////
CString CChinaMarket::GetStockName(CString strStockCode) {
	try {
		return (m_vChinaMarketStock.at(m_mapChinaMarketStock.at(strStockCode))->GetDisplaySymbol());
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("GetStockName ") + strStockCode + _T(" "), e);
		return _T("");
	}
}

////////////////////////////////////////////////////////////////////////////////
//
//	ͨ����Ʊ����õ���Ʊ��ָ�롣
//	���û�ҵ��Ļ����ؿ�ָ��
//
//
////////////////////////////////////////////////////////////////////////////////
CChinaStockPtr CChinaMarket::GetStock(CString strStockCode) {
	CChinaStockPtr pStock;
	if (!IsStock(strStockCode)) {
		return nullptr;
	}
	else {
		return (m_vChinaMarketStock.at(m_mapChinaMarketStock.at(strStockCode)));
	}
}

CChinaStockPtr CChinaMarket::GetStock(long lIndex) {
	try {
		return m_vChinaMarketStock.at(lIndex);
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("GetStock "), e);
		return nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
//	ͨ����Ʊ������г��������õ�ǰѡ���Ʊ
//
//////////////////////////////////////////////////////////////////////////////////////
void CChinaMarket::SetCurrentStock(CString strStockCode) {
	ASSERT(IsStock(strStockCode));
	CChinaStockPtr pStock = GetStock(strStockCode);
	SetCurrentStock(pStock);
	ASSERT(m_pCurrentStock != nullptr);
}

//////////////////////////////////////////////////////////////////////////
//
// ���õ�ǰ�����Ĺ�Ʊ
//
// ������Ӧ�Ĺ�Ʊָ�룬װ�����������ݡ�
//
/////////////////////////////////////////////////////////////////////////
void CChinaMarket::SetCurrentStock(CChinaStockPtr pStock) {
	bool fSet = false;
	if (m_pCurrentStock != nullptr) {
		if (!m_pCurrentStock->IsSameStock(pStock)) {
			m_pCurrentStock->SetRecordRTData(false);
			fSet = true;
		}
	}
	else {
		fSet = true;
	}
	if (fSet) {
		pStock->SetRecordRTData(true);
		m_pCurrentStock = pStock;
		SetCurrentStockChanged(true);
		m_pCurrentStock->SetDayLineLoaded(false); // ����ֻ�����ñ�ʶ��ʵ��װ�������ɵ��ȳ���ִ�С�
	}
}

void CChinaMarket::ResetCurrentStock(void) {
	if (m_pCurrentStock != nullptr) {
		m_pCurrentStock->SetRecordRTData(false);
		m_pCurrentStock = nullptr;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	���������ݴ������ݿ⣮
//  �˺����ɹ����߳�ThreadDayLineSaveProc���ã�������Ҫʹ��ȫ�ֱ�����(Ŀǰʹ�����̵߳���֮��������ͬ������ĳ��֣�
//
// �����Ƿ�ִ���˴洢����������Ҫ�����ص�������ʷ����ɾ���������ܹ���ʡ�ڴ��ռ�á�����ʵ�ʴ洢����ʹ���߳�ģʽʵ�֣�
// �ʶ���ִ��ʱ������������̣߳��������߳�ɾ����������ʱ����ͬ�����⡣����ķ������ù����̶߳���ɾ���洢����������ݣ�
// ���̵߳�ɾ������ֻ�ڲ����ù����̣߳�����洢�������ݣ�������·���ִ�С�
//////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::TaskSaveDayLineData(void) {
	CString str;
	bool fSave = false;

	for (auto& pStock : m_vChinaMarketStock) {
		if (pStock->IsDayLineNeedSavingAndClearFlag()) { // �����ʶ��Ҫ�����ʶ����ͬһԭ�ӹ����У���ֹͬ���������
			if (pStock->GetDayLineSize() > 0) {
				if (pStock->HaveNewDayLineData()) {
					CreatingThreadSaveDayLineBasicInfoOfStock(pStock.get());
					fSave = true;
				}
				else pStock->UnloadDayLine(); // ������ִ�д洢����ʱ�����ﻹҪ����ж���������ݡ���洢���������߳��Ժ��ִ�У��ʶ������ڴ�ͳһִ��ɾ��������
			}
			else { // �������Ϊ�й�Ʊ���룬���˴�����δ����
				CString str1 = pStock->GetSymbol();
				str1 += _T(" Ϊδ���й�Ʊ����");
				gl_systemMessage.PushDayLineInfoMessage(str1);
			}
		}
		if (gl_fExitingSystem) {
			break; // ������������˳�����ֹͣ�洢��
		}
	}

	return fSave;
}

bool CChinaMarket::UnloadDayLine(void) noexcept {
	for (auto& pStock : m_vChinaMarketStock) {
		pStock->UnloadDayLine();
	}

	return true;
}

bool CChinaMarket::BuildWeekLine(long lStartDate) {
	gl_systemMessage.PushInformationMessage(_T("��������������ʷ����"));
	for (auto& pStock : m_vChinaMarketStock) {
		CreatingThreadBuildWeekLineOfStock(pStock.get(), lStartDate);
	}
	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) {
		Sleep(1000);
	}
	gl_systemMessage.PushInformationMessage(_T("������ʷ�����������"));

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ʹ�õ�ǰ���ڵ������������ɱ��ܵ��������ݡ�
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::BuildWeekLineOfCurrentWeek(void) {
	CDayLineContainer dayLineContainer;
	CWeekLineContainer weekLineContainer;
	set<CString> setDayLineStockCode;
	set<CString> setWeekLineStockCode;
	const long lCurrentMonday = GetCurrentMonday(GetMarketDate());

	if (!LoadDayLine(dayLineContainer, GetMarketDate())) {
		return true; // ���ر�����������ʧ�ܣ������������账��
	}
	auto pDayLineData = dayLineContainer.GetContainer();

	gl_systemMessage.PushInformationMessage(_T("��ʼ���ɽ�������"));

	CreateStockCodeSet(setDayLineStockCode, dayLineContainer.GetContainer());

	DeleteCurrentWeekWeekLineBeforeTheDate(lCurrentMonday); // �ӵ�ǰ�����߱��������������һ֮ǰ������
	weekLineContainer.LoadCurrentWeekLine();
	CreateStockCodeSet(setWeekLineStockCode, weekLineContainer.GetContainer());

	CWeekLinePtr pWeekLine;
	for (auto& pData : *pDayLineData) {
		if (setWeekLineStockCode.find(pData->GetStockSymbol()) == setWeekLineStockCode.end()) { //���������������޴���������
			 // �洢������������������������
			pWeekLine = make_shared<CWeekLine>();
			pWeekLine->UpdateWeekLine(pData);
			weekLineContainer.StoreData(pWeekLine);
		}
		else {
			// ����������������
			weekLineContainer.UpdateData(pData);
		}
	}

	// ���֮ǰ����������
	DeleteWeekLine(lCurrentMonday);
	// �洢��������ֵ�������ݱ�
	weekLineContainer.SaveData();
	// �����ǰ�ܵ�����
	DeleteCurrentWeekWeekLine();
	// �洢��ǰ�������ڵ�ǰ�����ݱ�
	weekLineContainer.SaveCurrentWeekLine();

	gl_systemMessage.PushInformationMessage(_T("���ɽ��������������"));

	return true;
}

bool CChinaMarket::CreateStockCodeSet(set<CString>& setStockCode, not_null<vector<CVirtualHistoryCandleExtendPtr>*> pvData) {
	CString strStockSymbol;
	vector<CString> vectorStockCode;

	for (auto& pData : *pvData) {
		strStockSymbol = pData->GetStockSymbol();
		vectorStockCode.push_back(strStockSymbol);
	}
	setStockCode.insert(vectorStockCode.begin(), vectorStockCode.end());

	return true;
}

bool CChinaMarket::BuildCurrentWeekWeekLineTable(void) {
	const long lCurrentMonday = GetCurrentMonday(GetMarketDate());
	CSetWeekLineBasicInfo setWeekLineBasicInfo;
	CSetWeekLineExtendInfo setWeekLineExtendInfo;
	CString strDate;
	char buffer[10];
	CWeekLinePtr pWeekLine = nullptr;
	CWeekLineContainer weekLineContainer;

	DeleteCurrentWeekWeekLine();

	sprintf_s(buffer, _T("%08d"), lCurrentMonday);
	strDate = buffer;
	setWeekLineBasicInfo.m_strFilter = _T("[Date] = ");
	setWeekLineBasicInfo.m_strFilter += strDate;
	setWeekLineBasicInfo.m_strSort = _T("[Symbol]");
	setWeekLineBasicInfo.Open();

	setWeekLineExtendInfo.m_strFilter = _T("[Date] = ");
	setWeekLineExtendInfo.m_strFilter += strDate;
	setWeekLineExtendInfo.m_strSort = _T("[Symbol]");
	setWeekLineExtendInfo.Open();

	while (!setWeekLineBasicInfo.IsEOF()) {
		pWeekLine = make_shared<CWeekLine>();
		pWeekLine->LoadHistoryCandleBasic(&setWeekLineBasicInfo);
		while (!setWeekLineExtendInfo.IsEOF() && (setWeekLineBasicInfo.m_Symbol > setWeekLineExtendInfo.m_Symbol)) {
			setWeekLineExtendInfo.MoveNext();
		}
		if (setWeekLineExtendInfo.IsEOF()) {
			setWeekLineExtendInfo.MoveFirst();
		}
		else if (setWeekLineBasicInfo.m_Symbol == setWeekLineExtendInfo.m_Symbol) { // ���ڴ����º����ݵ�Ե�ʣ���������Ĺ�Ʊ���ܲ���һһ��Ӧ
			pWeekLine->LoadHistoryCandleExtend(&setWeekLineExtendInfo);
			weekLineContainer.StoreData(pWeekLine);
			setWeekLineExtendInfo.MoveNext();
		}
		else {
			setWeekLineExtendInfo.MoveFirst();
		}
		setWeekLineBasicInfo.MoveNext();
	}

	weekLineContainer.SaveCurrentWeekLine();

	return true;
}

bool CChinaMarket::SortStockVector(void) {
	sort(m_vChinaMarketStock.begin(), m_vChinaMarketStock.end(), CompareChinaStock);
	m_mapChinaMarketStock.clear();
	int j = 0;
	for (auto& pStock : m_vChinaMarketStock) {
		pStock->SetOffset(j);
		ASSERT(!IsStock(pStock->GetSymbol()));
		m_mapChinaMarketStock[pStock->GetSymbol()] = j++;
	}
	ASSERT(m_vChinaMarketStock.size() == m_mapChinaMarketStock.size());
	return true;
}

bool CChinaMarket::LoadDayLine(CDayLineContainer& dayLineContainer, long lDate) {
	CString strSQL;
	CString strDate;
	char  pch[30];
	//CTime ctTime;
	CSetDayLineBasicInfo setDayLineBasicInfo;
	CSetDayLineExtendInfo setDayLineExtendInfo;

	sprintf_s(pch, _T("%08d"), lDate);
	strDate = pch;
	setDayLineBasicInfo.m_strSort = _T("[Symbol]");
	setDayLineBasicInfo.m_strFilter = _T("[Date] =");
	setDayLineBasicInfo.m_strFilter += strDate;
	setDayLineBasicInfo.Open();
	if (setDayLineBasicInfo.IsEOF()) { // ���ݼ�Ϊ�գ���������û�н���
		setDayLineBasicInfo.Close();
		CString str = strDate;
		str += _T("�����ݼ�Ϊ�գ����账����������");
		gl_systemMessage.PushDayLineInfoMessage(str);    // ����ͬ�����Ʊ�����Ϣ
		return false;
	}
	setDayLineExtendInfo.m_strSort = _T("[Symbol]");
	setDayLineExtendInfo.m_strFilter = _T("[Date] =");
	setDayLineExtendInfo.m_strFilter += strDate;
	setDayLineExtendInfo.Open();
	setDayLineExtendInfo.m_pDatabase->BeginTrans();
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	while (!setDayLineBasicInfo.IsEOF()) {
		CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->LoadHistoryCandleBasic(&setDayLineBasicInfo);
		while (!setDayLineExtendInfo.IsEOF() && (strcmp(setDayLineExtendInfo.m_Symbol, setDayLineBasicInfo.m_Symbol) < 0)) {
			setDayLineExtendInfo.MoveNext();
		}
		if (!setDayLineExtendInfo.IsEOF() && (strcmp(setDayLineExtendInfo.m_Symbol, setDayLineBasicInfo.m_Symbol) == 0)) {
			pDayLine->LoadHistoryCandleExtend(&setDayLineExtendInfo);
		}
		dayLineContainer.StoreData(pDayLine);
		setDayLineBasicInfo.MoveNext();
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineExtendInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();
	setDayLineExtendInfo.Close();

	return true;
}

bool CChinaMarket::DeleteWeekLine(long lMonday) {
	DeleteWeekLineBasicInfo(lMonday);
	DeleteWeekLineExtendInfo(lMonday);

	return true;
}

bool CChinaMarket::DeleteWeekLine(void) {
	if (gl_fTestMode) {
		ASSERT(0); // ���ڴ���ʵ�����ݿ⣬�ʲ�������Դ˺���
		exit(1);
	}
	DeleteWeekLineBasicInfo();
	DeleteWeekLineExtendInfo();
	return true;
}

bool CChinaMarket::DeleteWeekLineBasicInfo(void) {
	CDatabase database;

	if (gl_fTestMode) {
		ASSERT(0); // ���ڴ���ʵ�����ݿ⣬�ʲ�������Դ˺���
		exit(1);
	}

	database.Open(_T("ChinaMarket"), FALSE, FALSE, _T("ODBC;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	database.BeginTrans();
	database.ExecuteSQL(_T("TRUNCATE `chinamarket`.`weekline`;"));
	database.CommitTrans();
	database.Close();

	return true;
}

bool CChinaMarket::DeleteWeekLineExtendInfo(void) {
	CDatabase database;

	if (gl_fTestMode) {
		ASSERT(0); // ���ڴ���ʵ�����ݿ⣬�ʲ�������Դ˺���
		exit(1);
	}

	database.Open(_T("ChinaMarket"), FALSE, FALSE, _T("ODBC;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	database.BeginTrans();
	database.ExecuteSQL(_T("TRUNCATE `chinamarket`.`weeklineinfo`;"));
	database.CommitTrans();
	database.Close();

	return true;
}

bool CChinaMarket::DeleteWeekLineBasicInfo(long lMonday) {
	CString strSQL;
	CString strDate;
	char  pch[30];
	//CTime ctTime;
	CSetWeekLineBasicInfo setWeekLineBasicInfo;

	sprintf_s(pch, _T("%08d"), lMonday);
	strDate = pch;
	setWeekLineBasicInfo.m_strFilter = _T("[Date] =");
	setWeekLineBasicInfo.m_strFilter += strDate;
	setWeekLineBasicInfo.Open();
	setWeekLineBasicInfo.m_pDatabase->BeginTrans();
	while (!setWeekLineBasicInfo.IsEOF()) {
		setWeekLineBasicInfo.Delete();
		setWeekLineBasicInfo.MoveNext();
	}
	setWeekLineBasicInfo.m_pDatabase->CommitTrans();
	setWeekLineBasicInfo.Close();

	return true;
}

bool CChinaMarket::DeleteWeekLineExtendInfo(long lMonday) {
	CString strSQL;
	CString strDate;
	char  pch[30];
	CSetWeekLineExtendInfo setWeekLineExtendInfo;

	sprintf_s(pch, _T("%08d"), lMonday);
	strDate = pch;
	setWeekLineExtendInfo.m_strFilter = _T("[Date] =");
	setWeekLineExtendInfo.m_strFilter += strDate;
	setWeekLineExtendInfo.Open();
	setWeekLineExtendInfo.m_pDatabase->BeginTrans();
	while (!setWeekLineExtendInfo.IsEOF()) {
		setWeekLineExtendInfo.Delete();
		setWeekLineExtendInfo.MoveNext();
	}
	setWeekLineExtendInfo.m_pDatabase->CommitTrans();
	setWeekLineExtendInfo.Close();

	return true;
}

bool CChinaMarket::DeleteCurrentWeekWeekLine(void) {
	CSetCurrentWeekLine setCurrentWeekLineInfo;

	setCurrentWeekLineInfo.Open();
	setCurrentWeekLineInfo.m_pDatabase->BeginTrans();
	while (!setCurrentWeekLineInfo.IsEOF()) {
		setCurrentWeekLineInfo.Delete();
		setCurrentWeekLineInfo.MoveNext();
	}
	setCurrentWeekLineInfo.m_pDatabase->CommitTrans();
	setCurrentWeekLineInfo.Close();

	return true;
}

bool CChinaMarket::DeleteCurrentWeekWeekLineBeforeTheDate(long lCutOffDate) {
	CSetCurrentWeekLine setCurrentWeekLineInfo;

	setCurrentWeekLineInfo.Open();
	setCurrentWeekLineInfo.m_pDatabase->BeginTrans();
	while (!setCurrentWeekLineInfo.IsEOF()) {
		if (setCurrentWeekLineInfo.m_Date < lCutOffDate) {
			setCurrentWeekLineInfo.Delete();
		}
		setCurrentWeekLineInfo.MoveNext();
	}
	setCurrentWeekLineInfo.m_pDatabase->CommitTrans();
	setCurrentWeekLineInfo.Close();

	return true;
}

CChinaStockPtr CChinaMarket::GetCurrentSelectedStock(void) {
	if (m_lCurrentSelectedStockSet >= 0) {
		return m_avChoicedStock.at(m_lCurrentSelectedStockSet).at(0);
	}
	else {
		return GetStock(0);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	��ʵʱ���ݴ������ݿ⣮Ĭ�����ݿ�Ϊ�ա�
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::SaveRTData(void) {
	CSetRealTimeData setRTData;
	CWebRTDataPtr pRTData = nullptr;
	const size_t lTotal = m_qRTData.size();

	if (lTotal > 0) {
		setRTData.m_strFilter = _T("[ID] = 1");
		setRTData.Open();
		setRTData.m_pDatabase->BeginTrans();
		for (size_t i = 0; i < lTotal; i++) {
			pRTData = m_qRTData.front();
			m_qRTData.pop(); // �׵���ǰ���������
			pRTData->AppendData(setRTData);
		}
		setRTData.m_pDatabase->CommitTrans();
		setRTData.Close();
	}
	return(true);
}

bool CChinaMarket::Choice10RSStrong2StockSet(void) {
	vector<CChinaStockPtr> v10RSStrongStock;

	for (auto& pStock : m_vChinaMarketStock) {
		CreatingThreadCalculate10RSStrong2Stock(&v10RSStrongStock, pStock);
	}
	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) {
		if (gl_fExitingSystem) return false;
		Sleep(100); // �ȴ������߳������������
	}

	CSetRSStrong2Stock setRSStrong2;

	setRSStrong2.Open();
	setRSStrong2.m_pDatabase->BeginTrans();
	while (!setRSStrong2.IsEOF()) {
		setRSStrong2.Delete();
		setRSStrong2.MoveNext();
	}
	setRSStrong2.m_pDatabase->CommitTrans();
	setRSStrong2.m_pDatabase->BeginTrans();
	for (auto& pStock : v10RSStrongStock) {
		setRSStrong2.AddNew();
		setRSStrong2.m_Symbol = pStock->GetSymbol();
		setRSStrong2.Update();
	}
	setRSStrong2.m_pDatabase->CommitTrans();
	setRSStrong2.Close();

	return true;
}

bool CChinaMarket::Choice10RSStrong1StockSet(void) {
	vector<CChinaStockPtr> v10RSStrongStock;

	for (auto& pStock : m_vChinaMarketStock) {
		CreatingThreadCalculate10RSStrong1Stock(&v10RSStrongStock, pStock);
	}
	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) {
		if (gl_fExitingSystem) return false;
		Sleep(100); // �ȴ������߳������������
	}

	CSetRSStrong1Stock setRSStrong1;

	setRSStrong1.Open();
	setRSStrong1.m_pDatabase->BeginTrans();
	while (!setRSStrong1.IsEOF()) {
		setRSStrong1.Delete();
		setRSStrong1.MoveNext();
	}
	setRSStrong1.m_pDatabase->CommitTrans();
	setRSStrong1.m_pDatabase->BeginTrans();
	for (auto& pStock : v10RSStrongStock) {
		setRSStrong1.AddNew();
		setRSStrong1.m_Symbol = pStock->GetSymbol();
		setRSStrong1.Update();
	}
	setRSStrong1.m_pDatabase->CommitTrans();
	setRSStrong1.Close();

	return true;
}

bool CChinaMarket::Choice10RSStrongStockSet(CRSReference* pRef, int iIndex) {
	vector<CChinaStockPtr> v10RSStrongStock;

	for (auto& pStock : m_vChinaMarketStock) {
		CreatingThreadCalculate10RSStrongStock(&v10RSStrongStock, pRef, pStock);
	}

	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) {
		if (gl_fExitingSystem) return false;
		Sleep(100); // �ȴ������߳������������
	}

	m_lCurrentRSStrongIndex = iIndex; // CSetRSStrongStock��Ҫ��m_lCurrentRSStrongIndex��ѡ����ȷ�����ݱ�
	CSetRSStrongStock setRSStrong(iIndex);

	setRSStrong.Open();
	setRSStrong.m_pDatabase->BeginTrans();
	while (!setRSStrong.IsEOF()) {
		setRSStrong.Delete();
		setRSStrong.MoveNext();
	}
	setRSStrong.m_pDatabase->CommitTrans();
	setRSStrong.m_pDatabase->BeginTrans();
	for (auto& pStock : v10RSStrongStock) {
		setRSStrong.AddNew();
		setRSStrong.m_Symbol = pStock->GetSymbol();
		setRSStrong.Update();
	}
	setRSStrong.m_pDatabase->CommitTrans();
	setRSStrong.Close();

	return true;
}

bool CChinaMarket::IsDayLineNeedUpdate(void) const noexcept {
	for (auto& pStock : m_vChinaMarketStock) {
		if (pStock->IsDayLineNeedUpdate()) return true;
	}
	return false;
}

bool CChinaMarket::IsDayLineNeedProcess(void) const noexcept {
	if (gl_WebInquirer.GetNeteaseDayLineDataSize() > 0) return true;
	else return false;
}

bool CChinaMarket::IsDayLineNeedSaving(void) const {
	for (auto& pStock : m_vChinaMarketStock) {
		if (pStock->IsDayLineNeedSaving()) {
			return true;
		}
	}
	return false;
}

long CChinaMarket::GetDayLineNeedUpdateNumber(void) {
	long l = 0;
	for (auto& pStock : m_vChinaMarketStock) {
		if (pStock->IsDayLineNeedUpdate()) l++;
	}
	return l;
}

long CChinaMarket::GetDayLineNeedProcessNumber(void) {
	return gl_WebInquirer.GetNeteaseDayLineDataSize();
}
long CChinaMarket::GetDayLineNeedSaveNumber(void) {
	long l = 0;
	for (auto& pStock : m_vChinaMarketStock) {
		if (pStock->IsDayLineNeedSaving()) l++;
	}
	return l;
}

bool CChinaMarket::TaskProcessDayLineGetFromNeeteaseServer(void) {
	CNeteaseDayLineWebDataPtr pData;
	CWebDataPtr pWebData = nullptr;
	CChinaStockPtr pStock = nullptr;

	while (gl_WebInquirer.GetNeteaseDayLineDataSize() > 0) {
		pWebData = gl_WebInquirer.PopNeteaseDayLineData();
		pData = make_shared<CNeteaseDayLineWebData>();
		pData->TransferWebDataToBuffer(pWebData);
		pData->ProcessNeteaseDayLineData();
		ASSERT(gl_pChinaMarket->IsStock(pData->GetStockCode()));
		pStock = gl_pChinaMarket->GetStock(pData->GetStockCode());
		pStock->UpdateDayLine(pData->GetProcessedDayLine(), true); // pData����������������ģ��������ڵ���ǰ�档
		pStock->UpdateStatusByDownloadedDayLine();

		pStock->SetDayLineLoaded(true);
		pStock->SetDayLineNeedSaving(true); // ���ô洢���߱�ʶ

		pData = nullptr;
	}
	return true;
}

bool CChinaMarket::TaskLoadCurrentStockHistoryData(void) {
	if (m_pCurrentStock != nullptr) {
		if (!m_pCurrentStock->IsDayLineLoaded()) {
			CreatingThreadLoadDayLine(m_pCurrentStock.get());
			m_pCurrentStock->SetDayLineLoaded(true);
		}
		if (!m_pCurrentStock->IsWeekLineLoaded()) {
			CreatingThreadLoadWeekLine(m_pCurrentStock.get());
			m_pCurrentStock->SetWeekLineLoaded(true);
		}
	}
	return true;
}

bool CChinaMarket::CreatingThreadSaveChoicedRTData(void) {
	thread thread1(ThreadSaveRTData, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CChinaMarket::CreatingThreadProcessTodayStock(void) {
	thread thread1(ThreadProcessTodayStock, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CChinaMarket::CreatingThreadBuildDayLineRS(long lStartCalculatingDay) {
	thread thread1(ThreadBuildDayLineRS, this, lStartCalculatingDay);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CChinaMarket::CreatingThreadBuildDayLineRSOfDate(long lThisDay) {
	thread thread1(ThreadBuildDayLineRSOfDate, this, lThisDay);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CChinaMarket::CreatingThreadBuildWeekLineRSOfDate(long lThisDay) {
	thread thread1(ThreadBuildWeekLineRSOfDate, this, lThisDay);
	thread1.detach(); // �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CChinaMarket::CreatingThreadSaveTempRTData(void) {
	thread thread1(ThreadSaveTempRTData, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CChinaMarket::CreatingThreadSaveDayLineBasicInfoOfStock(CChinaStock* pStock) {
	thread thread1(ThreadSaveDayLineBasicInfoOfStock, pStock);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CChinaMarket::CreatingThreadLoadDayLine(CChinaStock* pCurrentStock) {
	thread thread1(ThreadLoadDayLine, pCurrentStock);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CChinaMarket::CreatingThreadLoadWeekLine(CChinaStock* pCurrentStock) {
	thread thread1(ThreadLoadWeekLine, pCurrentStock);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CChinaMarket::CreatingThreadUpdateStockCodeDB(void) {
	thread thread1(ThreadUpdateStockCodeDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CChinaMarket::CreatingThreadUpdateOptionDB(void) {
	thread thread1(ThreadUpdateOptionDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CChinaMarket::CreatingThreadAppendChoicedStockDB(void) {
	thread thread1(ThreadAppendChoicedStockDB, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CChinaMarket::CreatingThreadChoice10RSStrong2StockSet(void) {
	thread thread1(ThreadChoice10RSStrong2StockSet, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CChinaMarket::CreatingThreadChoice10RSStrong1StockSet(void) {
	thread thread1(ThreadChoice10RSStrong1StockSet, this);
	thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
	return true;
}

bool CChinaMarket::CreatingThreadChoice10RSStrongStockSet(void) {
	for (int i = 0; i < 10; i++) {
		if (m_aRSStrongOption.at(i).m_fActive) {
			thread thread1(ThreadChoice10RSStrongStockSet, this, &(m_aRSStrongOption.at(i)), i);
			thread1.detach();// �������֮����ʵ�ֲ��в���������֤��ϵͳ������Դ��
		}
	}
	SetUpdatedDateFor10DaysRS(GetMarketDate());
	SetUpdateOptionDB(true); // ����ѡ�����ݿ�.��ʱ���㹤���߳�ֻ�Ǹո���������Ҫʱ��ȥ��ɡ�

	return true;
}

bool CChinaMarket::CreatingThreadCalculate10RSStrong1Stock(vector<CChinaStockPtr>* pv10RSStrongStock, CChinaStockPtr pStock) {
	thread thread1(ThreadCalculate10RSStrong1Stock, pv10RSStrongStock, pStock);
	thread1.detach();

	return true;
}

bool CChinaMarket::CreatingThreadCalculate10RSStrong2Stock(vector<CChinaStockPtr>* pv10RSStrongStock, CChinaStockPtr pStock) {
	thread thread1(ThreadCalculate10RSStrong2Stock, pv10RSStrongStock, pStock);
	thread1.detach();

	return true;
}

bool CChinaMarket::CreatingThreadBuildWeekLine(long lStartDate) {
	thread thread1(ThreadBuildWeekLine, this, lStartDate);
	thread1.detach();
	return true;
}

bool CChinaMarket::CreatingThreadBuildWeekLineOfStock(CChinaStock* pStock, long lStartDate) {
	thread thread1(ThreadBuildWeekLineOfStock, pStock, lStartDate);
	thread1.detach();

	return true;
}

bool CChinaMarket::CreatingThreadBuildWeekLineRS(void) {
	thread thread1(ThreadBuildWeekLineRS, this, __CHINA_MARKET_BEGIN_DATE__);
	thread1.detach();

	return true;
}

bool CChinaMarket::CreatingThreadCalculate10RSStrongStock(vector<CChinaStockPtr>* pv10RSStrongStock, CRSReference* pRef, CChinaStockPtr pStock) {
	thread thread1(ThreadCalculate10RSStrongStock, pv10RSStrongStock, pRef, pStock);
	thread1.detach();

	return true;
}

bool CChinaMarket::CreatingThreadBuildWeekLineOfCurrentWeek(void) {
	thread thread1(ThreadBuildWeekLineOfCurrentWeek, this);
	thread1.detach();

	return true;
}

bool CChinaMarket::CreatingThreadBuildCurrentWeekWeekLineTable(void) {
	thread thread1(ThreadBuildCurrentWeekWeekLineTable, this);
	thread1.detach();

	return true;
}

bool CChinaMarket::CreatingThreadSaveStockSection(void) {
	thread thread1(ThreadSaveStockSection, this);
	thread1.detach();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
//
// ����ǰ�����յ�ʵʱ���ݣ��������߸��������ݣ����ǿ�ȡ������������̣���
//
// ֻ��������������ʷ���ݺ󣬷���ִ�д���ʵʱ���ݣ�����������й�Ʊ����������
//
// long lCurrentTradeDay ��ǰ�����ա����ڴ������������գ��ʶ������ڲ���һ�����ǵ�ǰ���ڣ�������ʱ����
//
//////////////////////////////////////////////////////////////////////////////////
long CChinaMarket::BuildDayLine(long lCurrentTradeDay) {
	char buffer[20]{ 0 };
	CString strDate;
	CSetDayLineBasicInfo setDayLineBasicInfo;
	CSetDayLineExtendInfo setDayLineExtendInfo;
	long iCount = 0;

	CString str;
	str = _T("��ʼ����");
	str += GetStringOfDate(lCurrentTradeDay);
	str += _T("��ʵʱ����");
	gl_systemMessage.PushInformationMessage(str);

	DeleteDayLineBasicInfo(lCurrentTradeDay);
	DeleteDayLineExtendInfo(lCurrentTradeDay);

	// �洢��ǰ�����յ�����
	_ltoa_s(lCurrentTradeDay, buffer, 10);
	strDate = buffer;
	setDayLineBasicInfo.m_strFilter = _T("[ID] = 1");
	setDayLineBasicInfo.Open();
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	for (auto& pStock : m_vChinaMarketStock) {
		if (!pStock->IsTodayDataActive()) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
			continue;
		}
		iCount++;
		pStock->SetDayLineEndDate(lCurrentTradeDay);
		pStock->SetIPOStatus(__STOCK_IPOED__); // ������һ�Ρ���ֹ�¹ɹ�Ʊ��������û����ʷ���ݶ�������Ϊ�����ڡ�
		pStock->SetUpdateProfileDB(true);
		setDayLineBasicInfo.AddNew();
		pStock->SaveTodayBasicInfo(&setDayLineBasicInfo);
		setDayLineBasicInfo.Update();
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();

	// �洢�������ɵ�������DayLineExtendInfo���С�
	setDayLineExtendInfo.m_strFilter = _T("[ID] = 1");
	setDayLineExtendInfo.Open();
	setDayLineExtendInfo.m_pDatabase->BeginTrans();
	for (auto& pStock : m_vChinaMarketStock) {
		if (!pStock->IsTodayDataActive()) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
			continue;
		}
		setDayLineExtendInfo.AddNew();
		pStock->SaveTodayExtendInfo(&setDayLineExtendInfo);
		setDayLineExtendInfo.Update();
	}
	setDayLineExtendInfo.m_pDatabase->CommitTrans();
	setDayLineExtendInfo.Close();

	str = GetStringOfDate(lCurrentTradeDay);
	str += _T("��ʵʱ���ݴ������");
	gl_systemMessage.PushInformationMessage(str);

	sprintf_s(buffer, _T("%d"), iCount);
	str = _T("���մ�����");
	str += buffer;
	str += _T("����Ʊ");
	gl_systemMessage.PushInformationMessage(str);

	return iCount;
}

bool CChinaMarket::DeleteDayLine(long lDate) {
	DeleteDayLineBasicInfo(lDate);
	DeleteDayLineExtendInfo(lDate);

	return true;
}

bool CChinaMarket::DeleteDayLineBasicInfo(long lDate) {
	char buffer[20]{ 0x000 };
	CString strDate;
	CSetDayLineBasicInfo setDayLineBasicInfo;

	_ltoa_s(lDate, buffer, 10);
	strDate = buffer;
	setDayLineBasicInfo.m_strFilter = _T("[Date] =");
	setDayLineBasicInfo.m_strFilter += strDate;
	setDayLineBasicInfo.Open();
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	while (!setDayLineBasicInfo.IsEOF()) {
		setDayLineBasicInfo.Delete();
		setDayLineBasicInfo.MoveNext();
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();

	return true;
}

bool CChinaMarket::DeleteDayLineExtendInfo(long lDate) {
	char buffer[20]{ 0x000 };
	CString strDate;
	CSetDayLineExtendInfo setDayLineExtendInfo;

	_ltoa_s(lDate, buffer, 10);
	strDate = buffer;
	setDayLineExtendInfo.m_strFilter = _T("[Date] =");
	setDayLineExtendInfo.m_strFilter += strDate;
	setDayLineExtendInfo.Open();
	setDayLineExtendInfo.m_pDatabase->BeginTrans();
	while (!setDayLineExtendInfo.IsEOF()) {
		setDayLineExtendInfo.Delete();
		setDayLineExtendInfo.MoveNext();
	}
	setDayLineExtendInfo.m_pDatabase->CommitTrans();
	setDayLineExtendInfo.Close();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
//
// �����մ���õ����ݴ��������ݿ��У��Ա���һϵͳ����ʱ����װ�롣
// �ƺ�Ӧ����һ����ʱ�����̵߳���ʽ���ڡ�
//
// �о�֮��
//
//////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::UpdateTodayTempDB(void) {
	CSetDayLineTodaySaved setDayLineTemp;
	CString str;

	DeleteTodayTempDB();

	// �洢�������ɵ�������DayLineToday���С�
	setDayLineTemp.m_strFilter = _T("[ID] = 1");
	setDayLineTemp.Open();
	setDayLineTemp.m_pDatabase->BeginTrans();
	for (auto& pStock : m_vChinaMarketStock) {
		if (!pStock->IsTodayDataActive()) {  // �˹�Ʊ����ͣ��,���е����ݽ�Ϊ��,����Ҫ�洢.
			continue;
		}
		if (pStock->IsNeedProcessRTData() && (!pStock->IsVolumeConsistence())) {
			str = pStock->GetSymbol();
			str += _T(" ��������ȷ");
			gl_systemMessage.PushInnerSystemInformationMessage(str);
		}
		setDayLineTemp.AddNew();
		pStock->SaveTempInfo(&setDayLineTemp);
		setDayLineTemp.Update();
	}
	setDayLineTemp.m_pDatabase->CommitTrans();
	setDayLineTemp.Close();

	return true;
}

bool CChinaMarket::DeleteTodayTempDB(void) {
	CDatabase database;

	if (gl_fTestMode) {
		ASSERT(0); // ���ڴ���ʵ�����ݿ⣬�ʲ�������Դ˺���
		exit(1); //�˳�ϵͳ
	}

	database.Open(_T("ChinaMarket"), FALSE, FALSE, _T("ODBC;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	database.BeginTrans();
	database.ExecuteSQL(_T("TRUNCATE `chinamarket`.`today`;"));
	database.CommitTrans();
	database.Close();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// �����ݴ�ĵ�ǰ���ݣ����÷����ĳ�ʼ̬���������ڿ���ʱϵͳ�˳�ʱ����������ʧ�������ѷ���������
//
// ������m_lUnknownVolumeΪ��¼���е�m_UnknownVolume - m_Volume��������Ϊ��һ�μ���ʱֻ�ǳ�ʼ��ϵͳ��
// ��Ҫ����m_lUnknownVolume = pRTData->m_lVolume - setDayLineTemp.m_Volume + setDayLineTemp.m_UnknownVolume
// ����һ��ִ�м���ʵʱ����ʱ��ֻ�ǳ�ʼ��ϵͳ��������������m_lUnknownVolume += pRTData->GetVolume
// �ʶ��˴��������㡣
/////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::LoadTodayTempDB(long lTheDay) {
	CChinaStockPtr pStock = nullptr;
	CSetDayLineTodaySaved setDayLineTemp;
	CWebRTDataPtr pRTData;

	ASSERT(!m_fTodayTempDataLoaded);
	ASSERT(!gl_ThreadStatus.IsCalculatingRTData());    // ִ�д˳�ʼ������ʱ������ʵʱ���ݵĹ����̱߳���û�����С�
	// ��ȡ�������ɵ�������DayLineToday���С�
	setDayLineTemp.Open();
	if (!setDayLineTemp.IsEOF()) {
		if (setDayLineTemp.m_Date == lTheDay) { // ����ǵ�������飬�����룬���������Ĭ�����е��������ڽ�Ϊͬһ��ʱ�䣩
			while (!setDayLineTemp.IsEOF()) {
				if (IsStock(setDayLineTemp.m_Symbol)) {
					pStock = GetStock(setDayLineTemp.m_Symbol);
					ASSERT(!pStock->HaveFirstRTData()); // ȷ��û�п�ʼ����ʵʱ����
					pStock->LoadTodaySavedInfo(&setDayLineTemp);
				}
				setDayLineTemp.MoveNext();
			}
		}
	}
	setDayLineTemp.Close();

	return true;
}

bool CChinaMarket::Load10DaysRSStrong1StockSet(void) {
	CSetRSStrong1Stock setRSStrong1;
	CChinaStockPtr pStock = nullptr;

	m_v10RSStrong1Stock.clear();
	setRSStrong1.Open();
	while (!setRSStrong1.IsEOF()) {
		if (IsStock(setRSStrong1.m_Symbol)) {
			pStock = gl_pChinaMarket->GetStock(setRSStrong1.m_Symbol);
			m_v10RSStrong1Stock.push_back(pStock);
		}
		setRSStrong1.MoveNext();
	}
	setRSStrong1.Close();

	return true;
}

bool CChinaMarket::Load10DaysRSStrong2StockSet(void) {
	CSetRSStrong2Stock setRSStrong2;
	CChinaStockPtr pStock = nullptr;

	m_v10RSStrong2Stock.clear();
	setRSStrong2.Open();
	while (!setRSStrong2.IsEOF()) {
		if (IsStock(setRSStrong2.m_Symbol)) {
			pStock = gl_pChinaMarket->GetStock(setRSStrong2.m_Symbol);
			m_v10RSStrong2Stock.push_back(pStock);
		}
		setRSStrong2.MoveNext();
	}
	setRSStrong2.Close();

	return true;
}

bool CChinaMarket::LoadCalculatingRSOption(void) {
	CSetRSOption setRSOption;

	setRSOption.Open();
	while (!setRSOption.IsEOF()) {
		m_aRSStrongOption.at(setRSOption.m_Index).m_fActive = setRSOption.m_Active;
		m_aRSStrongOption.at(setRSOption.m_Index).m_lDayLength[0] = setRSOption.m_DayLengthFirst;
		m_aRSStrongOption.at(setRSOption.m_Index).m_lDayLength[1] = setRSOption.m_DayLengthSecond;
		m_aRSStrongOption.at(setRSOption.m_Index).m_lDayLength[2] = setRSOption.m_DayLengthThird;
		m_aRSStrongOption.at(setRSOption.m_Index).m_lDayLength[3] = setRSOption.m_DayLengthFourth;
		m_aRSStrongOption.at(setRSOption.m_Index).m_lStrongDayLength[0] = setRSOption.m_StrongDayLengthFirst;
		m_aRSStrongOption.at(setRSOption.m_Index).m_lStrongDayLength[1] = setRSOption.m_StrongDayLengthSecond;
		m_aRSStrongOption.at(setRSOption.m_Index).m_lStrongDayLength[2] = setRSOption.m_StrongDayLengthThird;
		m_aRSStrongOption.at(setRSOption.m_Index).m_lStrongDayLength[3] = setRSOption.m_StrongDayLengthFourth;
		m_aRSStrongOption.at(setRSOption.m_Index).m_dRSStrong[0] = atof(setRSOption.m_RSStrongFirst);
		m_aRSStrongOption.at(setRSOption.m_Index).m_dRSStrong[1] = atof(setRSOption.m_RSStrongSecond);
		m_aRSStrongOption.at(setRSOption.m_Index).m_dRSStrong[2] = atof(setRSOption.m_RSStrongThird);
		m_aRSStrongOption.at(setRSOption.m_Index).m_dRSStrong[3] = atof(setRSOption.m_RSStrongFourth);
		setRSOption.MoveNext();
	}
	setRSOption.Close();
	return true;
}

void CChinaMarket::SaveCalculatingRSOption(void) {
	CSetRSOption setRSOption;

	setRSOption.Open();
	setRSOption.m_pDatabase->BeginTrans();
	while (!setRSOption.IsEOF()) {
		setRSOption.Delete();
		setRSOption.MoveNext();
	}
	setRSOption.m_pDatabase->CommitTrans();
	setRSOption.Close();

	setRSOption.m_strFilter = _T("[ID] = 1");
	setRSOption.Open();
	setRSOption.m_pDatabase->BeginTrans();
	for (int i = 0; i < 10; i++) {
		setRSOption.AddNew();
		setRSOption.m_Index = i;
		setRSOption.m_Active = m_aRSStrongOption.at(i).m_fActive;
		setRSOption.m_DayLengthFirst = m_aRSStrongOption.at(i).m_lDayLength[0];
		setRSOption.m_DayLengthSecond = m_aRSStrongOption.at(i).m_lDayLength[1];
		setRSOption.m_DayLengthThird = m_aRSStrongOption.at(i).m_lDayLength[2];
		setRSOption.m_DayLengthFourth = m_aRSStrongOption.at(i).m_lDayLength[3];
		setRSOption.m_StrongDayLengthFirst = m_aRSStrongOption.at(i).m_lStrongDayLength[0];
		setRSOption.m_StrongDayLengthSecond = m_aRSStrongOption.at(i).m_lStrongDayLength[1];
		setRSOption.m_StrongDayLengthThird = m_aRSStrongOption.at(i).m_lStrongDayLength[2];
		setRSOption.m_StrongDayLengthFourth = m_aRSStrongOption.at(i).m_lStrongDayLength[3];
		setRSOption.m_RSStrongFirst = ConvertValueToString(m_aRSStrongOption.at(i).m_dRSStrong[0]);
		setRSOption.m_RSStrongSecond = ConvertValueToString(m_aRSStrongOption.at(i).m_dRSStrong[1]);
		setRSOption.m_RSStrongThird = ConvertValueToString(m_aRSStrongOption.at(i).m_dRSStrong[2]);
		setRSOption.m_RSStrongFourth = ConvertValueToString(m_aRSStrongOption.at(i).m_dRSStrong[3]);
		setRSOption.Update();
	}
	setRSOption.m_pDatabase->CommitTrans();
	setRSOption.Close();
}

bool CChinaMarket::Load10DaysRSStrongStockDB(void) {
	for (int i = 0; i < 10; i++) {
		LoadOne10DaysRSStrongStockDB(i);
	}
	return true;
}

bool CChinaMarket::LoadOne10DaysRSStrongStockDB(long lIndex) {
	m_lCurrentRSStrongIndex = lIndex;
	CSetRSStrongStock setRSStrongStock(lIndex);

	setRSStrongStock.Open();
	while (!setRSStrongStock.IsEOF()) {
		if (IsStock(setRSStrongStock.m_Symbol)) {
			CChinaStockPtr pStock = gl_pChinaMarket->GetStock(setRSStrongStock.m_Symbol);
			m_avChoicedStock.at(m_lCurrentRSStrongIndex + c_10DaysRSStockSetStartPosition).push_back(pStock); // 10��RS��Ʊ����ʼλ��Ϊ��10����
		}
		setRSStrongStock.MoveNext();
	}
	setRSStrongStock.Close();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////
//
// ����lDate���������ǿ��, lDate�ĸ�ʽΪ��YYYYMMDD,�� 19990605.
// �����߰��ǵ����к�,�����ǿ�������ڶ����е�λ��.
// m_dRSIndex���Ǽ������ָ�����ǵ�ǿ�ȡ�
//
//////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::BuildDayLineRS(long lDate) {
	vector<CChinaStockPtr> vStock;
	vector<int> vIndex;
	vector<double> vRS;
	int iTotalAShare = 0;
	CString strSQL;
	CString strDate;
	char  pch[30];
	int iStockNumber = 0;
	CSetDayLineBasicInfo setDayLineBasicInfo;
	double dShanghaiIndexUpDownRate = 0;
	double dShenzhenIndexUpDownRate = 0;
	double dIndexUpDownRate{ 0.0 };
	double dRSIndex{ 0.0 };

	sprintf_s(pch, _T("%08d"), lDate);
	strDate = pch;
	setDayLineBasicInfo.m_strSort = _T("[UpDownRate]");
	setDayLineBasicInfo.m_strFilter = _T("[Date] =");
	setDayLineBasicInfo.m_strFilter += strDate;
	setDayLineBasicInfo.Open();
	if (setDayLineBasicInfo.IsEOF()) { // ���ݼ�Ϊ�գ���������û�н���
		setDayLineBasicInfo.Close();
		CString str = strDate;
		str += _T("�����ݼ�Ϊ�գ�û�м������ǿ��");
		gl_systemMessage.PushInformationMessage(str);    // ����ͬ�����Ʊ�����Ϣ
		return false;
	}
	setDayLineBasicInfo.m_pDatabase->BeginTrans();
	iStockNumber = 0;
	while (!setDayLineBasicInfo.IsEOF()) {
		if (strcmp(setDayLineBasicInfo.m_Symbol, _T("sh000001")) == 0) { // �Ϻ���ָ
			dShanghaiIndexUpDownRate = GetUpDownRate(setDayLineBasicInfo.m_Close, setDayLineBasicInfo.m_LastClose);
		}
		else if (strcmp(setDayLineBasicInfo.m_Symbol, _T("sz399001")) == 0) { // ���ڳ�ָ
			dShenzhenIndexUpDownRate = GetUpDownRate(setDayLineBasicInfo.m_Close, setDayLineBasicInfo.m_LastClose);
		}
		if (IsAStock(setDayLineBasicInfo.m_Symbol)) {
			const long lIndex = m_mapChinaMarketStock.at(setDayLineBasicInfo.m_Symbol);
			vStock.push_back(m_vChinaMarketStock.at(lIndex));
			vIndex.push_back(iStockNumber); // ��A�ɵ�������¼�������С�
			iTotalAShare++;
		}
		iStockNumber++;
		setDayLineBasicInfo.MoveNext();
	}
	dIndexUpDownRate = (dShanghaiIndexUpDownRate + dShenzhenIndexUpDownRate) / 2;

	setDayLineBasicInfo.MoveFirst();
	int iCount = 0;
	int iBefore = 0;
	while (iCount < vIndex.size()) { // ֻ�����Ծ��Ʊ�����ǿ��
		for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) { // ��������ȥ�������ݿ�,�������A�ɵĹ�Ʊ
			setDayLineBasicInfo.MoveNext();
		}
		setDayLineBasicInfo.Edit();
		const double dLastClose = atof(setDayLineBasicInfo.m_LastClose);
		const double dLow = atof(setDayLineBasicInfo.m_Low);
		const double dHigh = atof(setDayLineBasicInfo.m_High);
		const double dClose = atof(setDayLineBasicInfo.m_Close);
		double dUpDownRate = 0;
		// ����ָ�����ǿ��
		if (dLastClose < 0.001) { // �¹����еȣ��������̼۸�Ϊ��
			dRSIndex = 50;
		}
		else {
			dUpDownRate = (dClose - dLastClose) / dLastClose;
			if ((dUpDownRate > 0.11) || (dUpDownRate < -0.11)) { // ��Ȩ�ȵ��¼۸�ͻ��
				dRSIndex = 50;
			}
			else {
				dRSIndex = (dUpDownRate - dIndexUpDownRate) * 500 + 50; // �Դ����ǵ�Ϊ��׼��50����
			}
		}
		setDayLineBasicInfo.m_RSIndex = ConvertValueToString(dRSIndex);

		// �����ǵ��������ǿ��
		if (dLastClose < 0.001) {
			setDayLineBasicInfo.m_RS = ConvertValueToString(50); // �¹����л��߳�Ȩ��Ϣ��������˹�
		}
		else if (((dLow / dLastClose) < 0.88) || ((dHigh / dLastClose) > 1.12)) { // ��Ȩ���¹����е�
			setDayLineBasicInfo.m_RS = ConvertValueToString(50); // �¹����л��߳�Ȩ��Ϣ��������˹�
		}
		else if ((fabs(dHigh - dClose) < 0.0001) && (((dClose / dLastClose)) > 1.095)) { // ��ͣ��
			setDayLineBasicInfo.m_RS = ConvertValueToString(100);
		}
		else if ((fabs(dClose - dLow) < 0.0001) && ((dClose / dLastClose) < 0.905)) { // ��ͣ��
			setDayLineBasicInfo.m_RS = ConvertValueToString(0);
		}
		else {
			setDayLineBasicInfo.m_RS = ConvertValueToString((static_cast<double>(iCount) * 100) / iTotalAShare);
		}
		setDayLineBasicInfo.Update();
		iBefore = vIndex.at(iCount++);
		setDayLineBasicInfo.MoveNext(); // �Ƶ���һ�����ݡ�
		iBefore++; // ������Ҳͬʱ��һ��
	}
	setDayLineBasicInfo.m_pDatabase->CommitTrans();
	setDayLineBasicInfo.Close();

	vStock.clear();
	vIndex.clear();
	vRS.clear();

	CString strDate2 = GetStringOfDate(lDate);
	CString strTemp;
	strTemp = strDate2 + _T("�Ĺ�Ʊ�������ǿ�ȼ������");
	gl_systemMessage.PushInformationMessage(strTemp);    // ����ͬ�����Ʊ�����Ϣ

	return(true);
}

///////////////////////////////////////////////////////////////////////////////////
//
// ����lDate���������ǿ��, lDate�ĸ�ʽΪ��YYYYMMDD,�� 19990605.
// �����߰��ǵ����к�,�����ǿ�������ڶ����е�λ��.
// m_dRSIndex���Ǽ������ָ�����ǵ�ǿ�ȡ�
//
//////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::BuildWeekLineRS(long lDate) {
	vector<CChinaStockPtr> vStock;
	vector<int> vIndex;
	vector<double> vRS;
	int iTotalAShare = 0;
	CString strSQL;
	CString strDate;
	char  pch[30];
	int iStockNumber = 0;
	//CTime ctTime;
	CSetWeekLineBasicInfo setWeekLineBasicInfo;
	double dShanghaiIndexUpDownRate = 0;
	double dShenzhenIndexUpDownRate = 0;
	double dIndexUpDownRate{ 0 };
	double dRSIndex{ 0 };

	ASSERT(GetCurrentMonday(lDate) == lDate); // ȷ��������Ϊ����һ

	sprintf_s(pch, _T("%08d"), lDate);
	strDate = pch;
	setWeekLineBasicInfo.m_strSort = _T("[UpDownRate]");
	setWeekLineBasicInfo.m_strFilter = _T("[Date] =");
	setWeekLineBasicInfo.m_strFilter += strDate;
	setWeekLineBasicInfo.Open();
	if (setWeekLineBasicInfo.IsEOF()) { // ���ݼ�Ϊ�գ���������û�н���
		setWeekLineBasicInfo.Close();
		CString str = strDate;
		str += _T("�����ݼ�Ϊ�գ�û�м������ǿ��");
		gl_systemMessage.PushDayLineInfoMessage(str);    // ����ͬ�����Ʊ�����Ϣ
		return false;
	}
	setWeekLineBasicInfo.m_pDatabase->BeginTrans();
	iStockNumber = 0;
	while (!setWeekLineBasicInfo.IsEOF()) {
		if (strcmp(setWeekLineBasicInfo.m_Symbol, _T("sh000001")) == 0) { // �Ϻ���ָ
			dShanghaiIndexUpDownRate = GetUpDownRate(setWeekLineBasicInfo.m_Close, setWeekLineBasicInfo.m_LastClose);
		}
		else if (strcmp(setWeekLineBasicInfo.m_Symbol, _T("sz399001")) == 0) { // ���ڳ�ָ
			dShenzhenIndexUpDownRate = GetUpDownRate(setWeekLineBasicInfo.m_Close, setWeekLineBasicInfo.m_LastClose);
		}
		if (IsAStock(setWeekLineBasicInfo.m_Symbol)) {
			const long lIndex = m_mapChinaMarketStock.at(setWeekLineBasicInfo.m_Symbol);
			vStock.push_back(m_vChinaMarketStock.at(lIndex));
			vIndex.push_back(iStockNumber); // ��A�ɵ�������¼�������С�
			iTotalAShare++;
		}
		iStockNumber++;
		setWeekLineBasicInfo.MoveNext();
	}
	dIndexUpDownRate = (dShanghaiIndexUpDownRate + dShenzhenIndexUpDownRate) / 2;

	setWeekLineBasicInfo.MoveFirst();
	int iCount = 0;
	int iBefore = 0;
	while (iCount < vIndex.size()) { // ֻ�����Ծ��Ʊ�����ǿ��
		for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) { // ��������ȥ�������ݿ�,�������A�ɵĹ�Ʊ
			setWeekLineBasicInfo.MoveNext();
		}
		setWeekLineBasicInfo.Edit();
		const double dLastClose = atof(setWeekLineBasicInfo.m_LastClose);
		const double dClose = atof(setWeekLineBasicInfo.m_Close);
		double dUpDownRate = 0;
		// ����ָ�����ǿ��
		if (dLastClose < 0.001) { // �¹����еȣ��������̼۸�Ϊ��
			dRSIndex = 50;
		}
		else {
			dUpDownRate = (dClose - dLastClose) / dLastClose;
			dRSIndex = (dUpDownRate - dIndexUpDownRate) * 500 + 50; // �Դ����ǵ�Ϊ��׼��50����
		}
		setWeekLineBasicInfo.m_RSIndex = ConvertValueToString(dRSIndex);

		// �����ǵ��������ǿ��
		if (dLastClose < 0.001) {
			setWeekLineBasicInfo.m_RS = ConvertValueToString(50); // �¹����л��߳�Ȩ��Ϣ��������˹�
		}
		else {
			setWeekLineBasicInfo.m_RS = ConvertValueToString((static_cast<double>(iCount) * 100) / iTotalAShare);
		}
		setWeekLineBasicInfo.Update();
		iBefore = vIndex.at(iCount++);
		setWeekLineBasicInfo.MoveNext(); // �Ƶ���һ�����ݡ�
		iBefore++; // ������Ҳͬʱ��һ��
	}
	setWeekLineBasicInfo.m_pDatabase->CommitTrans();
	setWeekLineBasicInfo.Close();

	vStock.clear();
	vIndex.clear();
	vRS.clear();

	CString strDate2 = GetStringOfDate(lDate);
	CString strTemp;
	strTemp = strDate2 + _T("�Ĺ�Ʊ�������ǿ�ȼ������");
	gl_systemMessage.PushInformationMessage(strTemp);    // ����ͬ�����Ʊ�����Ϣ

	return(true);
}

double CChinaMarket::GetUpDownRate(CString strClose, CString strLastClose) noexcept {
	const double lastClose = atof(strLastClose);
	if (lastClose < 0.001) return 0;
	double result = (atof(strClose) - lastClose) / lastClose;
	if ((result > 0.11) || (result < -0.11)) result = 0;
	return result;
}

bool CChinaMarket::UpdateStockCodeDB(void) {
	CChinaStockPtr pStock = nullptr;
	CSetChinaStockSymbol setChinaStockSymbol;
	int iUpdatedStock = 0;
	int iCount = 0;

	//����ԭ�еĴ��뼯״̬
	if (IsUpdateStockCodeDB()) {
		for (auto& pStock2 : m_vChinaMarketStock) {
			if (pStock2->IsUpdateProfileDB()) iUpdatedStock++;
		}
		setChinaStockSymbol.m_strSort = _T("[Symbol]");
		setChinaStockSymbol.Open();
		setChinaStockSymbol.m_pDatabase->BeginTrans();
		while (iCount < iUpdatedStock) {
			if (setChinaStockSymbol.IsEOF()) break;
			pStock = m_vChinaMarketStock.at(m_mapChinaMarketStock.at(setChinaStockSymbol.m_Symbol));
			if (pStock->IsUpdateProfileDBAndClearFlag()) {
				//ASSERT(!pStock3->IsTodayNewStock());
				iCount++;
				pStock->UpdateSymbol(setChinaStockSymbol);
			}
			setChinaStockSymbol.MoveNext();
		}
		if (iCount < iUpdatedStock) {
			for (auto& pStock3 : m_vChinaMarketStock) {
				if (pStock3->IsUpdateProfileDBAndClearFlag()) {
					ASSERT(pStock3->IsTodayNewStock());
					iCount++;
					pStock3->AppendSymbol(setChinaStockSymbol);
					pStock3->SetTodayNewStock(false);
				}
				if (iCount >= iUpdatedStock) break;
			}
		}
		setChinaStockSymbol.m_pDatabase->CommitTrans();
		setChinaStockSymbol.Close();
		m_lLoadedStock = m_vChinaMarketStock.size();
	}
	ASSERT(iCount == iUpdatedStock);
	return true;
}

void CChinaMarket::LoadStockSection(void) {
	CSetStockSection setStockSection;

	setStockSection.Open();
	while (!setStockSection.IsEOF()) {
		if (!m_vStockSection.at(setStockSection.m_IndexNumber)->IsActive()) {
			m_vStockSection.at(setStockSection.m_IndexNumber)->SetActive(setStockSection.m_Active);
			m_vStockSection.at(setStockSection.m_IndexNumber)->SetMarket(setStockSection.m_Market);
			m_vStockSection.at(setStockSection.m_IndexNumber)->SetIndexNumber(setStockSection.m_IndexNumber);
			m_vStockSection.at(setStockSection.m_IndexNumber)->SetComment(setStockSection.m_Comment);
		}
		setStockSection.MoveNext();
	}
	setStockSection.Close();
}

void CChinaMarket::LoadStockCodeDB(void) {
	CSetChinaStockSymbol setChinaStockSymbol;
	char buffer[30]{ 0, 0, 0 };
	CString str;

	setChinaStockSymbol.m_strSort = _T("[Symbol]");
	setChinaStockSymbol.Open();
	// װ���Ʊ�������ݿ�
	while (!setChinaStockSymbol.IsEOF()) {
		CChinaStockPtr pStock = make_shared<CChinaStock>();
		if (!IsStock(setChinaStockSymbol.m_Symbol)) {
			pStock->LoadStockCodeDB(setChinaStockSymbol);
			m_vChinaMarketStock.push_back(pStock);
		}
		else {
			str = _T("�����ظ����룺");
			str += setChinaStockSymbol.m_Symbol;
			gl_systemMessage.PushInnerSystemInformationMessage(str);
			setChinaStockSymbol.Delete(); // ɾ�����ظ�����
		}
		setChinaStockSymbol.MoveNext();
	}
	if (IsDayLineNeedUpdate()) {
		const int i = GetDayLineNeedUpdateNumber();
		m_lStockDayLineNeedUpdate = i;
		if (GetDayOfWeek() == 1) gl_systemMessage.PushInformationMessage(_T("ÿ����һ�������й�Ʊ����"));
		TRACE("����%d����Ʊ��Ҫ�����������\n", i);
		_itoa_s(i, buffer, 10);
		str = buffer;
		str += _T("����Ʊ��Ҫ�����������");
		gl_systemMessage.PushInformationMessage(str);
	}
	setChinaStockSymbol.Close();
	m_lLoadedStock = m_vChinaMarketStock.size();
	SortStockVector();
}

///////////////////////////////////////////////////////////////////////////////////
//
// ����ѡ�����ݿ�
//
//
//
//////////////////////////////////////////////////////////////////////////////////
bool CChinaMarket::UpdateOptionDB(void) {
	CSetOption setOption;

	setOption.Open();
	setOption.m_pDatabase->BeginTrans();
	if (setOption.IsEOF()) {
		setOption.AddNew();
		setOption.m_RSEndDate = GetRSEndDate();
		setOption.m_RSStartDate = GetRSStartDate();
		setOption.m_LastLoginDate = GetMarketDate();
		setOption.m_LastLoginTime = GetMarketTime();
		setOption.m_UpdatedDateFor10DaysRS1 = GetUpdatedDateFor10DaysRS1();
		setOption.m_UpdatedDateFor10DaysRS2 = GetUpdatedDateFor10DaysRS2();
		setOption.m_UpdatedDateFor10DaysRS = GetUpdatedDateFor10DaysRS();
		setOption.Update();
	}
	else {
		setOption.Edit();
		setOption.m_RSEndDate = GetRSEndDate();
		setOption.m_RSStartDate = GetRSStartDate();
		setOption.m_LastLoginDate = GetMarketDate();
		setOption.m_LastLoginTime = GetMarketTime();
		setOption.m_UpdatedDateFor10DaysRS1 = GetUpdatedDateFor10DaysRS1();
		setOption.m_UpdatedDateFor10DaysRS2 = GetUpdatedDateFor10DaysRS2();
		setOption.m_UpdatedDateFor10DaysRS = GetUpdatedDateFor10DaysRS();
		setOption.Update();
	}
	setOption.m_pDatabase->CommitTrans();
	setOption.Close();
	return true;
}

void CChinaMarket::LoadOptionDB(void) {
	CSetOption setOption;
	setOption.Open();
	if (setOption.IsEOF()) {
		SetRSStartDate(__CHINA_MARKET_BEGIN_DATE__);
		SetRSEndDate(__CHINA_MARKET_BEGIN_DATE__);
		SetLastLoginDate(__CHINA_MARKET_BEGIN_DATE__);
		SetUpdatedDateFor10DaysRS1(__CHINA_MARKET_BEGIN_DATE__);
		SetUpdatedDateFor10DaysRS2(__CHINA_MARKET_BEGIN_DATE__);
	}
	else {
		if (setOption.m_RSEndDate == 0) {
			SetRSEndDate(__CHINA_MARKET_BEGIN_DATE__);
		}
		else {
			SetRSEndDate(setOption.m_RSEndDate);
			if (GetRSEndDate() > __CHINA_MARKET_BEGIN_DATE__) {
				// ��������ʷ���ݿ��д��ھ�����ʱ�����õ��߳�ģʽ�洢�����ݡ�ʹ�ö��߳�ģʽʱ��MySQL����ֻ�����Exception�����������ݿ�����ʱ����ͬ�����⣩��
				// �ʶ��޲�����ʱͬʱֻ����һ���洢�̣߳���������������״̬���������ⲻ�����������������������ʱ���ʶ��½���������ʱ����ʹ�ö��̣߳�ĿǰΪ4������
				// ʹ��8.0.27���ԣ����ֿ��Բ���4���߳��ˣ�20211103��
				gl_SaveOneStockDayLine.SetMaxCount(1);
			}
		}
		if (setOption.m_RSStartDate == 0) {
			SetRSStartDate(__CHINA_MARKET_BEGIN_DATE__);
		}
		else {
			SetRSStartDate(setOption.m_RSStartDate);
		}
		if (setOption.m_LastLoginDate == 0) {
			SetLastLoginDate(__CHINA_MARKET_BEGIN_DATE__);
		}
		else {
			SetLastLoginDate(setOption.m_LastLoginDate);
		}
		SetLastLoginTime(setOption.m_LastLoginTime);
		SetUpdatedDateFor10DaysRS1(setOption.m_UpdatedDateFor10DaysRS1);
		SetUpdatedDateFor10DaysRS2(setOption.m_UpdatedDateFor10DaysRS2);
		SetUpdatedDateFor10DaysRS(setOption.m_UpdatedDateFor10DaysRS);
		if (setOption.m_UpdatedDateFor10DaysRS1 < GetMarketDate())  m_fChoiced10RSStrong1StockSet = false;
		else m_fChoiced10RSStrong1StockSet = true;
		if (setOption.m_UpdatedDateFor10DaysRS2 < GetMarketDate())  m_fChoiced10RSStrong2StockSet = false;
		else m_fChoiced10RSStrong2StockSet = true;
		if (setOption.m_UpdatedDateFor10DaysRS < GetMarketDate())  m_fChoiced10RSStrongStockSet = false;
		else m_fChoiced10RSStrongStockSet = true;
	}

	setOption.Close();
}

void CChinaMarket::LoadOptionChinaStockMarketDB(void) {
	CSetChinaMarketOption setOptionChinaStockMarket;

	setOptionChinaStockMarket.Open();
	if (!setOptionChinaStockMarket.IsEOF()) {
		m_iRTDataServer = setOptionChinaStockMarket.m_RTDataServerIndex;
		m_iRTDataInquiryTickNumber = setOptionChinaStockMarket.m_RTDataInquiryTime / 100 - 1;
	}
	else {
		m_iRTDataServer = 0; // Ĭ��ʹ������ʵʱ���ݷ�����
		m_iRTDataInquiryTickNumber = 3; // Ĭ��400�����ѯһ��
	}
}

bool CChinaMarket::UpdateOptionChinaMarketDB(void) {
	CSetChinaMarketOption setOptionChinaStockMarket;

	setOptionChinaStockMarket.Open();
	setOptionChinaStockMarket.m_pDatabase->BeginTrans();
	if (setOptionChinaStockMarket.IsEOF()) {
		setOptionChinaStockMarket.AddNew();
		setOptionChinaStockMarket.m_RTDataServerIndex = m_iRTDataServer;
		setOptionChinaStockMarket.m_RTDataInquiryTime = (m_iRTDataInquiryTickNumber + 1) * 100;
		setOptionChinaStockMarket.Update();
	}
	else {
		setOptionChinaStockMarket.Edit();
		setOptionChinaStockMarket.m_RTDataServerIndex = m_iRTDataServer;
		setOptionChinaStockMarket.m_RTDataInquiryTime = (m_iRTDataInquiryTickNumber + 1) * 100;
		setOptionChinaStockMarket.Update();
	}
	setOptionChinaStockMarket.m_pDatabase->CommitTrans();
	setOptionChinaStockMarket.Close();

	return true;
}

bool CChinaMarket::UpdateChoicedStockDB(void) {
	CSetChinaChoicedStock setChinaChoicedStock;

	setChinaChoicedStock.Open();
	setChinaChoicedStock.m_pDatabase->BeginTrans();
	while (!setChinaChoicedStock.IsEOF()) {
		setChinaChoicedStock.Delete();
		setChinaChoicedStock.MoveNext();
	}
	setChinaChoicedStock.m_pDatabase->CommitTrans();
	setChinaChoicedStock.m_pDatabase->BeginTrans();
	for (auto& pStock : m_avChoicedStock.at(0)) {
		ASSERT(pStock->IsChoiced());
		setChinaChoicedStock.AddNew();
		setChinaChoicedStock.m_Symbol = pStock->GetSymbol();
		setChinaChoicedStock.Update();
		pStock->SetSaveToChoicedStockDB(true);
	}
	setChinaChoicedStock.m_pDatabase->CommitTrans();
	setChinaChoicedStock.Close();

	return true;
}

bool CChinaMarket::AppendChoicedStockDB(void) {
	CSetChinaChoicedStock setChinaChoicedStock;

	setChinaChoicedStock.Open();
	setChinaChoicedStock.m_pDatabase->BeginTrans();
	for (auto& pStock : m_avChoicedStock.at(0)) {
		ASSERT(pStock->IsChoiced());
		if (!pStock->IsSaveToChoicedStockDB()) {
			setChinaChoicedStock.AddNew();
			setChinaChoicedStock.m_Symbol = pStock->GetSymbol();
			setChinaChoicedStock.Update();
			pStock->SetSaveToChoicedStockDB(true);
		}
	}
	setChinaChoicedStock.m_pDatabase->CommitTrans();
	setChinaChoicedStock.Close();

	return true;
}

void CChinaMarket::LoadChoicedStockDB(void) {
	CSetChinaChoicedStock setChinaChoicedStock;

	setChinaChoicedStock.Open();
	// װ���Ʊ�������ݿ�
	while (!setChinaChoicedStock.IsEOF()) {
		CChinaStockPtr pStock = nullptr;
		if (IsStock(setChinaChoicedStock.m_Symbol)) {
			pStock = GetStock(setChinaChoicedStock.m_Symbol);
			auto it = find(m_avChoicedStock.at(0).cbegin(), m_avChoicedStock.at(0).cend(), pStock);
			if (it == m_avChoicedStock.at(0).end()) {
				m_avChoicedStock.at(0).push_back(pStock);
			}
			pStock->SetChoiced(true);
			pStock->SetSaveToChoicedStockDB(true);
		}
		setChinaChoicedStock.MoveNext();
	}
	setChinaChoicedStock.Close();
}

bool CChinaMarket::UpdateTempRTData(void) {
	if (!gl_ThreadStatus.IsSavingTempData()) {
		gl_ThreadStatus.SetSavingTempData(true);
		CreatingThreadSaveTempRTData();
		return true;
	}
	return false;
}