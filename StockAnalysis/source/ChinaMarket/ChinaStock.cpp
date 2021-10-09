#include"pch.h"

#include "globedef.h"
//#include "timer.h"

#include"Accessory.h"

#include "ChinaStock.h"
#include"ChinaMarket.h"

#include<gsl/gsl>
using namespace gsl;

using namespace std;
#include<string>
#include<algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChinaStock::CChinaStock() {
	Reset();
}

CChinaStock::~CChinaStock(void) {
}

void CChinaStock::Reset(void) {
	CVirtualStock::Reset();
	m_lOffsetInContainer = -1;
	m_lDayLineStartDate = __CHINA_MARKET_BEGIN_DATE__; //
	m_lDayLineEndDate = __CHINA_MARKET_BEGIN_DATE__; //
	m_nHand = 100;

	m_lHighLimit = m_lLowLimit = 0;
	m_lHighLimit2 = m_lLowLimit2 = 0;
	for (int i = 0; i < 5; i++) {
		m_lPBuy.at(i) = m_lPSell.at(i) = 0;
		m_lVBuy.at(i) = m_lVSell.at(i) = 0;
	}
	m_dRealtimeRS = 0;
	m_dRealtimeRSIndex = 0;

	m_lAttackBuyAmount = 0;
	m_lAttackSellAmount = 0;
	m_lAttackBuyVolume = 0;
	m_lCurrentAttackBuy = 0;
	m_lAttackSellVolume = 0;
	m_lCurrentAttackSell = 0;
	m_lStrongBuyVolume = 0;
	m_lCurrentStrongBuy = 0;
	m_lStrongSellVolume = 0;
	m_lCurrentStrongSell = 0;
	m_lUnknownVolume = 0;
	m_lCurrentUnknown = 0;
	m_lCanceledBuyVolume = 0;
	m_lCanceledSellVolume = 0;
	m_lTransactionNumber = 0;
	m_lTransactionNumberBelow5000 = 0;
	m_lTransactionNumberBelow50000 = 0;
	m_lTransactionNumberBelow200000 = 0;
	m_lTransactionNumberAbove200000 = 0;

	m_lOrdinaryBuyNumberBelow5000 = 0;
	m_lOrdinaryBuyNumberBelow10000 = 0;
	m_lOrdinaryBuyNumberBelow20000 = 0;
	m_lOrdinaryBuyNumberBelow50000 = 0;
	m_lOrdinaryBuyNumberBelow100000 = 0;
	m_lOrdinaryBuyNumberBelow200000 = 0;
	m_lOrdinaryBuyNumberAbove200000 = 0;
	m_lOrdinarySellNumberBelow5000 = 0;
	m_lOrdinarySellNumberBelow10000 = 0;
	m_lOrdinarySellNumberBelow20000 = 0;
	m_lOrdinarySellNumberBelow50000 = 0;
	m_lOrdinarySellNumberBelow100000 = 0;
	m_lOrdinarySellNumberBelow200000 = 0;
	m_lOrdinarySellNumberAbove200000 = 0;
	m_lOrdinaryBuyVolumeBelow5000 = 0;
	m_lOrdinaryBuyVolumeBelow10000 = 0;
	m_lOrdinaryBuyVolumeBelow20000 = 0;
	m_lOrdinaryBuyVolumeBelow50000 = 0;
	m_lOrdinaryBuyVolumeBelow100000 = 0;
	m_lOrdinaryBuyVolumeBelow200000 = 0;
	m_lOrdinaryBuyVolumeAbove200000 = 0;
	m_lOrdinarySellVolumeBelow5000 = 0;
	m_lOrdinarySellVolumeBelow10000 = 0;
	m_lOrdinarySellVolumeBelow20000 = 0;
	m_lOrdinarySellVolumeBelow50000 = 0;
	m_lOrdinarySellVolumeBelow100000 = 0;
	m_lOrdinarySellVolumeBelow200000 = 0;
	m_lOrdinarySellVolumeAbove200000 = 0;

	m_lCanceledBuyVolumeBelow5000 = 0;
	m_lCanceledBuyVolumeBelow10000 = 0;
	m_lCanceledBuyVolumeBelow20000 = 0;
	m_lCanceledBuyVolumeBelow50000 = 0;
	m_lCanceledBuyVolumeBelow100000 = 0;
	m_lCanceledBuyVolumeBelow200000 = 0;
	m_lCanceledBuyVolumeAbove200000 = 0;
	m_lCanceledSellVolumeBelow5000 = 0;
	m_lCanceledSellVolumeBelow10000 = 0;
	m_lCanceledSellVolumeBelow20000 = 0;
	m_lCanceledSellVolumeBelow50000 = 0;
	m_lCanceledSellVolumeBelow100000 = 0;
	m_lCanceledSellVolumeBelow200000 = 0;
	m_lCanceledSellVolumeAbove200000 = 0;

	m_lOrdinaryBuyVolume = m_lAttackBuyBelow50000 = m_lAttackBuyBelow200000 = m_lAttackBuyAbove200000 = 0;
	m_lOrdinarySellVolume = m_lAttackSellBelow50000 = m_lAttackSellBelow200000 = m_lAttackSellAbove200000 = 0;

	m_llLastSavedVolume = 0;

	m_lCurrentCanceledSellVolume = m_lCurrentCanceledBuyVolume = m_lCurrentGuadanTransactionVolume = 0;

	m_dCurrentGuadanTransactionPrice = 0;
	m_nCurrentTransactionType = 0;

	m_fChoiced = false;
	m_fSaveToChoicedStockDB = false;

	m_fDayLineDBUpdated = false;

	m_fHaveFirstRTData = false;  // ʵʱ���ݿ�ʼ�����ʶ����һ��ʵʱ����ֻ��������ʼ��ϵͳ���������ڼ��㡣�ӵڶ������ݿ�ʼ�������Ч��
	m_fNeedProcessRTData = true;
	m_fRTDataCalculated = false;
	m_fRecordRTData = false;
	m_pLastRTData = nullptr;

	ClearRTDataDeque();
}

void CChinaStock::ClearRTDataDeque(void) {
	long lTotalNumber = GetRTDataQueueSize();
	for (int i = 0; i < lTotalNumber; i++) {
		CWebRTDataPtr pRTData = PopRTData();
	}
}

bool CChinaStock::HaveNewDayLineData(void) {
	if (m_DayLine.GetDataSize() <= 0) return false;
	if (m_DayLine.GetData(m_DayLine.GetDataSize() - 1)->GetFormatedMarketDate() > GetDayLineEndDate()) return true;
	else return false;
}

void CChinaStock::UpdateStatusByDownloadedDayLine(void) {
	if (m_DayLine.GetDataSize() == 0) return;
	if (gl_pChinaMarket->IsEarlyThen(m_DayLine.GetData(m_DayLine.GetDataSize() - 1)->GetFormatedMarketDate(), gl_pChinaMarket->GetFormatedMarketDate(), 30)) { // ��ȡ���Ĺ�Ʊ���������������������ϸ��µ���������գ������˻������������һ���µ�ʱ��۲죩��
		SetIPOStatus(__STOCK_DELISTED__); // �����л���ͣ���ס�
	}
	else {
		SetIPOStatus(__STOCK_IPOED__); // �������׹�Ʊ
	}
}

void CChinaStock::SetTodayActive(CString strStockCode, CString strStockName) {
	SetActive(true);
	SetDayLineLoaded(false);
	SetSymbol(strStockCode); // ����ȫ�ֹ�Ʊ����Ϣ����ʱRTData��ȫ���޷��������еĹ�Ʊ��Ϣ��
	if (strStockName != _T("")) SetDisplaySymbol(strStockName);// ����ȫ�ֹ�Ʊ����Ϣ����ʱRTData��ȫ���޷��������еĹ�Ʊ��Ϣ��
}

/////////////////////////////////////////////////////////////////////////////////////
//
// ��������������
//
/////////////////////////////////////////////////////////////////////////////////////
void CChinaStock::UpdateDayLine(vector<CDayLinePtr>& vTempDayLine, bool fRevertSave) {
	m_DayLine.UpdateData(vTempDayLine, fRevertSave);
}

void CChinaStock::ReportDayLineDownLoaded(void) {
	//CString strTemp = GetSymbol();
	//strTemp += _T("�����������.");
	//gl_systemMessage.PushDayLineInfoMessage(strTemp);
}

void CChinaStock::SaveTodayBasicInfo(CSetDayLineBasicInfo* psetDayLineBasicInfo) {
	CDayLine dayLine;

	ASSERT(psetDayLineBasicInfo->IsOpen());

	UpdateCurrentHistoryCandle(&dayLine);
	dayLine.SaveHistoryCandleBasic(psetDayLineBasicInfo);
}

void CChinaStock::SaveTempInfo(CSetDayLineTemp* psetDayLineTemp) {
	CDayLine dayLine;

	ASSERT(psetDayLineTemp->IsOpen());

	UpdateCurrentHistoryCandle(&dayLine);
	dayLine.SaveHistoryCandle(psetDayLineTemp);
}

void CChinaStock::UpdateCurrentHistoryCandle(CVirtualHistoryCandleExtend* pVirtualHistoryCandleExtend) {
	pVirtualHistoryCandleExtend->SetDate(FormatToDate(m_TransactionTime));
	pVirtualHistoryCandleExtend->SetExchange(m_strExchangeCode);
	pVirtualHistoryCandleExtend->SetStockSymbol(m_strSymbol);
	pVirtualHistoryCandleExtend->SetDisplaySymbol(m_strDisplaySymbol);
	pVirtualHistoryCandleExtend->SetLastClose(m_lLastClose);
	pVirtualHistoryCandleExtend->SetOpen(m_lOpen);
	pVirtualHistoryCandleExtend->SetHigh(m_lHigh);
	pVirtualHistoryCandleExtend->SetLow(m_lLow);
	pVirtualHistoryCandleExtend->SetClose(m_lNew);
	pVirtualHistoryCandleExtend->SetVolume(m_llVolume);
	pVirtualHistoryCandleExtend->SetAmount(m_llAmount);
	pVirtualHistoryCandleExtend->SetUpDown((double)m_lUpDown / GetRatio());
	pVirtualHistoryCandleExtend->SetUpDownRate(m_dUpDownRate);
	if (m_llTotalValue != 0) {
		pVirtualHistoryCandleExtend->SetChangeHandRate(static_cast<double>(100) * m_llAmount / m_llTotalValue);
	}
	else {
		pVirtualHistoryCandleExtend->SetChangeHandRate(0.0);
	}
	pVirtualHistoryCandleExtend->SetTotalValue(m_llTotalValue);
	pVirtualHistoryCandleExtend->SetCurrentValue(m_llCurrentValue);

	pVirtualHistoryCandleExtend->SetTransactionNumber(m_lTransactionNumber);
	pVirtualHistoryCandleExtend->SetTransactionNumberBelow5000(m_lTransactionNumberBelow5000);
	pVirtualHistoryCandleExtend->SetTransactionNumberBelow50000(m_lTransactionNumberBelow50000);
	pVirtualHistoryCandleExtend->SetTransactionNumberBelow200000(m_lTransactionNumberBelow200000);
	pVirtualHistoryCandleExtend->SetTransactionNumberAbove200000(m_lTransactionNumberAbove200000);

	pVirtualHistoryCandleExtend->SetCanceledBuyVolume(m_lCanceledBuyVolume);
	pVirtualHistoryCandleExtend->SetCanceledSellVolume(m_lCanceledSellVolume);
	pVirtualHistoryCandleExtend->SetAttackBuyVolume(m_lAttackBuyVolume);
	pVirtualHistoryCandleExtend->SetAttackSellVolume(m_lAttackSellVolume);
	pVirtualHistoryCandleExtend->SetStrongBuyVolume(m_lStrongBuyVolume);
	pVirtualHistoryCandleExtend->SetStrongSellVolume(m_lStrongSellVolume);
	pVirtualHistoryCandleExtend->SetUnknownVolume(m_lUnknownVolume);
	pVirtualHistoryCandleExtend->SetOrdinaryBuyVolume(m_lOrdinaryBuyVolume);
	pVirtualHistoryCandleExtend->SetOrdinarySellVolume(m_lOrdinarySellVolume);
	pVirtualHistoryCandleExtend->SetAttackBuyBelow50000(m_lAttackBuyBelow50000);
	pVirtualHistoryCandleExtend->SetAttackBuyBelow200000(m_lAttackBuyBelow200000);
	pVirtualHistoryCandleExtend->SetAttackBuyAbove200000(m_lAttackBuyAbove200000);
	pVirtualHistoryCandleExtend->SetAttackSellBelow50000(m_lAttackSellBelow50000);
	pVirtualHistoryCandleExtend->SetAttackSellBelow200000(m_lAttackSellBelow200000);
	pVirtualHistoryCandleExtend->SetAttackSellAbove200000(m_lAttackSellAbove200000);

	pVirtualHistoryCandleExtend->SetOrdinaryBuyVolumeBelow5000(m_lOrdinaryBuyVolumeBelow5000);
	pVirtualHistoryCandleExtend->SetOrdinaryBuyVolumeBelow10000(m_lOrdinaryBuyVolumeBelow10000);
	pVirtualHistoryCandleExtend->SetOrdinaryBuyVolumeBelow20000(m_lOrdinaryBuyVolumeBelow20000);
	pVirtualHistoryCandleExtend->SetOrdinaryBuyVolumeBelow50000(m_lOrdinaryBuyVolumeBelow50000);
	pVirtualHistoryCandleExtend->SetOrdinaryBuyVolumeBelow100000(m_lOrdinaryBuyVolumeBelow100000);
	pVirtualHistoryCandleExtend->SetOrdinaryBuyVolumeBelow200000(m_lOrdinaryBuyVolumeBelow200000);
	pVirtualHistoryCandleExtend->SetOrdinaryBuyVolumeAbove200000(m_lOrdinaryBuyVolumeAbove200000);
	pVirtualHistoryCandleExtend->SetOrdinarySellVolumeBelow5000(m_lOrdinarySellVolumeBelow5000);
	pVirtualHistoryCandleExtend->SetOrdinarySellVolumeBelow10000(m_lOrdinarySellVolumeBelow10000);
	pVirtualHistoryCandleExtend->SetOrdinarySellVolumeBelow20000(m_lOrdinarySellVolumeBelow20000);
	pVirtualHistoryCandleExtend->SetOrdinarySellVolumeBelow50000(m_lOrdinarySellVolumeBelow50000);
	pVirtualHistoryCandleExtend->SetOrdinarySellVolumeBelow100000(m_lOrdinarySellVolumeBelow100000);
	pVirtualHistoryCandleExtend->SetOrdinarySellVolumeBelow200000(m_lOrdinarySellVolumeBelow200000);
	pVirtualHistoryCandleExtend->SetOrdinarySellVolumeAbove200000(m_lOrdinarySellVolumeAbove200000);
	pVirtualHistoryCandleExtend->SetOrdinaryBuyNumberBelow5000(m_lOrdinaryBuyNumberBelow5000);
	pVirtualHistoryCandleExtend->SetOrdinaryBuyNumberBelow10000(m_lOrdinaryBuyNumberBelow10000);
	pVirtualHistoryCandleExtend->SetOrdinaryBuyNumberBelow20000(m_lOrdinaryBuyNumberBelow20000);
	pVirtualHistoryCandleExtend->SetOrdinaryBuyNumberBelow50000(m_lOrdinaryBuyNumberBelow50000);
	pVirtualHistoryCandleExtend->SetOrdinaryBuyNumberBelow100000(m_lOrdinaryBuyNumberBelow100000);
	pVirtualHistoryCandleExtend->SetOrdinaryBuyNumberBelow200000(m_lOrdinaryBuyNumberBelow200000);
	pVirtualHistoryCandleExtend->SetOrdinaryBuyNumberAbove200000(m_lOrdinaryBuyNumberAbove200000);
	pVirtualHistoryCandleExtend->SetOrdinarySellNumberBelow5000(m_lOrdinarySellNumberBelow5000);
	pVirtualHistoryCandleExtend->SetOrdinarySellNumberBelow10000(m_lOrdinarySellNumberBelow10000);
	pVirtualHistoryCandleExtend->SetOrdinarySellNumberBelow20000(m_lOrdinarySellNumberBelow20000);
	pVirtualHistoryCandleExtend->SetOrdinarySellNumberBelow50000(m_lOrdinarySellNumberBelow50000);
	pVirtualHistoryCandleExtend->SetOrdinarySellNumberBelow100000(m_lOrdinarySellNumberBelow100000);
	pVirtualHistoryCandleExtend->SetOrdinarySellNumberBelow200000(m_lOrdinarySellNumberBelow200000);
	pVirtualHistoryCandleExtend->SetOrdinarySellNumberAbove200000(m_lOrdinarySellNumberAbove200000);

	pVirtualHistoryCandleExtend->SetCanceledBuyVolumeBelow5000(m_lCanceledBuyVolumeBelow5000);
	pVirtualHistoryCandleExtend->SetCanceledBuyVolumeBelow10000(m_lCanceledBuyVolumeBelow10000);
	pVirtualHistoryCandleExtend->SetCanceledBuyVolumeBelow20000(m_lCanceledBuyVolumeBelow20000);
	pVirtualHistoryCandleExtend->SetCanceledBuyVolumeBelow50000(m_lCanceledBuyVolumeBelow50000);
	pVirtualHistoryCandleExtend->SetCanceledBuyVolumeBelow100000(m_lCanceledBuyVolumeBelow100000);
	pVirtualHistoryCandleExtend->SetCanceledBuyVolumeBelow200000(m_lCanceledBuyVolumeBelow200000);
	pVirtualHistoryCandleExtend->SetCanceledBuyVolumeAbove200000(m_lCanceledBuyVolumeAbove200000);
	pVirtualHistoryCandleExtend->SetCanceledSellVolumeBelow5000(m_lCanceledSellVolumeBelow5000);
	pVirtualHistoryCandleExtend->SetCanceledSellVolumeBelow10000(m_lCanceledSellVolumeBelow10000);
	pVirtualHistoryCandleExtend->SetCanceledSellVolumeBelow20000(m_lCanceledSellVolumeBelow20000);
	pVirtualHistoryCandleExtend->SetCanceledSellVolumeBelow50000(m_lCanceledSellVolumeBelow50000);
	pVirtualHistoryCandleExtend->SetCanceledSellVolumeBelow100000(m_lCanceledSellVolumeBelow100000);
	pVirtualHistoryCandleExtend->SetCanceledSellVolumeBelow200000(m_lCanceledSellVolumeBelow200000);
	pVirtualHistoryCandleExtend->SetCanceledSellVolumeAbove200000(m_lCanceledSellVolumeAbove200000);
}

void CChinaStock::UpdateStatus(CWebRTDataPtr pRTData) {
	SetTransactionTime(pRTData->GetTransactionTime());
	SetLastClose(pRTData->GetLastClose());
	SetNew(pRTData->GetNew());
	SetHigh(pRTData->GetHigh());
	SetLow(pRTData->GetLow());
	SetOpen(pRTData->GetOpen());
	SetVolume(pRTData->GetVolume());
	SetAmount(pRTData->GetAmount());
	SetUpDown(m_lNew - m_lLastClose);
	if (m_lLastClose != 0) SetUpDownRate((double)m_lUpDown * 100 / m_lLastClose);
	else SetUpDownRate(0);
	// ����ʵʱ������û������ֵ����ͨ��ֵ������ģ��ʶ���Ҫ�ж�һ���Ƿ�Ϊ��
	if (pRTData->GetTotalValue() > 0) SetTotalValue(pRTData->GetTotalValue());
	if (pRTData->GetCurrentValue() > 0) SetCurrentValue(pRTData->GetCurrentValue());
	if (m_llTotalValue != 0) m_dChangeHandRate = (double)m_llAmount * 100 / m_llTotalValue;
	else m_dChangeHandRate = 0;
	for (int i = 0; i < 5; i++) {
		SetPBuy(i, pRTData->GetPBuy(i));
		SetVBuy(i, pRTData->GetVBuy(i));
		SetPSell(i, pRTData->GetPSell(i));
		SetVSell(i, pRTData->GetVSell(i));
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//	��������ʷ���ݴ������ݿ⣮Ĭ�����ݿ�Ϊ�ա�
//  �˺����������̵߳��ã���Ҫע������ͬ�����⡣
// �����ھ�������ʷ����ʱ��������ֻ�Ǹ��¡�
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::SaveDayLineBasicInfo(void) {
	CSetDayLineBasicInfo setDayLineBasic;

	return m_DayLine.SaveBasicData(&setDayLineBasic, GetSymbol());
}

void CChinaStock::UpdateDayLineStartEndDate(void) {
	bool fUpdated = false;

	if (m_DayLine.GetDataSize() > 0) {
		if ((GetDayLineStartDate() == 19900101) || (m_DayLine.GetData(0)->GetFormatedMarketDate() < GetDayLineStartDate())) {
			SetDayLineStartDate(m_DayLine.GetData(0)->GetFormatedMarketDate());
			SetDayLineDBUpdated(true);
			fUpdated = true;
		}
		if (m_DayLine.GetData(m_DayLine.GetDataSize() - 1)->GetFormatedMarketDate() > GetDayLineEndDate()) {
			SetDayLineEndDate(m_DayLine.GetData(m_DayLine.GetDataSize() - 1)->GetFormatedMarketDate());
			SetDayLineDBUpdated(true);
			fUpdated = true;
		}
	}
	if (fUpdated) {
		SetUpdateProfileDB(true);
	}
}

void CChinaStock::SaveTodayExtendInfo(CSetDayLineExtendInfo* psetDayLineExtendInfo) {
	CDayLine dayLine;

	ASSERT(psetDayLineExtendInfo->IsOpen());
	UpdateCurrentHistoryCandle(&dayLine);
	dayLine.SaveHistoryCandleExtend(psetDayLineExtendInfo);
}

////////////////////////////////////////////////////////////////////////////
//
// ֻ��һ�����ñ�������������ϵͳ��ʼ��ʱװ��֮ǰ���������Ϣ����������˵Ļ���.
// ��Ҫͬʱ�����ܳɽ����������ݴ�˹��������ڼ���δ������ɽ����� �ܳɽ��������µ�ʵʱ��������ʱ��ͬ�����£��ʶ��޷����ڼ��㣩
//
////////////////////////////////////////////////////////////////////////////
void CChinaStock::LoadTempInfo(const CSetDayLineTemp& setDayLineTemp) {
	m_lUnknownVolume = atoll(setDayLineTemp.m_UnknownVolume);

	m_lTransactionNumber = atol(setDayLineTemp.m_TransactionNumber);
	m_lTransactionNumberBelow5000 = atol(setDayLineTemp.m_TransactionNumberBelow5000);
	m_lTransactionNumberBelow50000 = atol(setDayLineTemp.m_TransactionNumberBelow50000);
	m_lTransactionNumberBelow200000 = atol(setDayLineTemp.m_TransactionNumberBelow200000);
	m_lTransactionNumberAbove200000 = atol(setDayLineTemp.m_TransactionNumberAbove200000);
	m_lCanceledBuyVolume = atoll(setDayLineTemp.m_CanceledBuyVolume);
	m_lCanceledSellVolume = atoll(setDayLineTemp.m_CanceledSellVolume);
	m_lAttackBuyVolume = atoll(setDayLineTemp.m_AttackBuyVolume);
	m_lAttackSellVolume = atoll(setDayLineTemp.m_AttackSellVolume);
	m_lStrongBuyVolume = atoll(setDayLineTemp.m_StrongBuyVolume);
	m_lStrongSellVolume = atoll(setDayLineTemp.m_StrongSellVolume);
	m_lOrdinaryBuyVolume = atoll(setDayLineTemp.m_OrdinaryBuyVolume);
	m_lOrdinarySellVolume = atoll(setDayLineTemp.m_OrdinarySellVolume);
	m_lAttackBuyBelow50000 = atoll(setDayLineTemp.m_AttackBuyBelow50000);
	m_lAttackBuyBelow200000 = atoll(setDayLineTemp.m_AttackBuyBelow200000);
	m_lAttackBuyAbove200000 = atoll(setDayLineTemp.m_AttackBuyAbove200000);
	m_lAttackSellBelow50000 = atoll(setDayLineTemp.m_AttackSellBelow50000);
	m_lAttackSellBelow200000 = atoll(setDayLineTemp.m_AttackSellBelow200000);
	m_lAttackSellAbove200000 = atoll(setDayLineTemp.m_AttackSellAbove200000);
	m_llVolume = atoll(setDayLineTemp.m_Volume);
	m_llLastSavedVolume = atoll(setDayLineTemp.m_Volume);

	m_lOrdinaryBuyVolumeBelow5000 = atoll(setDayLineTemp.m_OrdinaryBuyVolumeBelow5000);
	m_lOrdinaryBuyVolumeBelow10000 = atoll(setDayLineTemp.m_OrdinaryBuyVolumeBelow10000);
	m_lOrdinaryBuyVolumeBelow20000 = atoll(setDayLineTemp.m_OrdinaryBuyVolumeBelow20000);
	m_lOrdinaryBuyVolumeBelow50000 = atoll(setDayLineTemp.m_OrdinaryBuyVolumeBelow50000);
	m_lOrdinaryBuyVolumeBelow100000 = atoll(setDayLineTemp.m_OrdinaryBuyVolumeBelow100000);
	m_lOrdinaryBuyVolumeBelow200000 = atoll(setDayLineTemp.m_OrdinaryBuyVolumeBelow200000);
	m_lOrdinaryBuyVolumeAbove200000 = atoll(setDayLineTemp.m_OrdinaryBuyVolumeAbove200000);
	m_lOrdinarySellVolumeBelow5000 = atoll(setDayLineTemp.m_OrdinarySellVolumeBelow5000);
	m_lOrdinarySellVolumeBelow10000 = atoll(setDayLineTemp.m_OrdinarySellVolumeBelow10000);
	m_lOrdinarySellVolumeBelow20000 = atoll(setDayLineTemp.m_OrdinarySellVolumeBelow20000);
	m_lOrdinarySellVolumeBelow50000 = atoll(setDayLineTemp.m_OrdinarySellVolumeBelow50000);
	m_lOrdinarySellVolumeBelow100000 = atoll(setDayLineTemp.m_OrdinarySellVolumeBelow100000);
	m_lOrdinarySellVolumeBelow200000 = atoll(setDayLineTemp.m_OrdinarySellVolumeBelow200000);
	m_lOrdinarySellVolumeAbove200000 = atoll(setDayLineTemp.m_OrdinarySellVolumeAbove200000);
	m_lOrdinaryBuyNumberBelow5000 = atoll(setDayLineTemp.m_OrdinaryBuyNumberBelow5000);
	m_lOrdinaryBuyNumberBelow10000 = atoll(setDayLineTemp.m_OrdinaryBuyNumberBelow10000);
	m_lOrdinaryBuyNumberBelow20000 = atoll(setDayLineTemp.m_OrdinaryBuyNumberBelow20000);
	m_lOrdinaryBuyNumberBelow50000 = atoll(setDayLineTemp.m_OrdinaryBuyNumberBelow50000);
	m_lOrdinaryBuyNumberBelow100000 = atoll(setDayLineTemp.m_OrdinaryBuyNumberBelow100000);
	m_lOrdinaryBuyNumberBelow200000 = atoll(setDayLineTemp.m_OrdinaryBuyNumberBelow200000);
	m_lOrdinaryBuyNumberAbove200000 = atoll(setDayLineTemp.m_OrdinaryBuyNumberAbove200000);
	m_lOrdinarySellNumberBelow5000 = atoll(setDayLineTemp.m_OrdinarySellNumberBelow5000);
	m_lOrdinarySellNumberBelow10000 = atoll(setDayLineTemp.m_OrdinarySellNumberBelow10000);
	m_lOrdinarySellNumberBelow20000 = atoll(setDayLineTemp.m_OrdinarySellNumberBelow20000);
	m_lOrdinarySellNumberBelow50000 = atoll(setDayLineTemp.m_OrdinarySellNumberBelow50000);
	m_lOrdinarySellNumberBelow100000 = atoll(setDayLineTemp.m_OrdinarySellNumberBelow100000);
	m_lOrdinarySellNumberBelow200000 = atoll(setDayLineTemp.m_OrdinarySellNumberBelow200000);
	m_lOrdinarySellNumberAbove200000 = atoll(setDayLineTemp.m_OrdinarySellNumberAbove200000);

	m_lCanceledBuyVolumeBelow5000 = atoll(setDayLineTemp.m_CanceledBuyVolumeBelow5000);
	m_lCanceledBuyVolumeBelow10000 = atoll(setDayLineTemp.m_CanceledBuyVolumeBelow10000);
	m_lCanceledBuyVolumeBelow20000 = atoll(setDayLineTemp.m_CanceledBuyVolumeBelow20000);
	m_lCanceledBuyVolumeBelow50000 = atoll(setDayLineTemp.m_CanceledBuyVolumeBelow50000);
	m_lCanceledBuyVolumeBelow100000 = atoll(setDayLineTemp.m_CanceledBuyVolumeBelow100000);
	m_lCanceledBuyVolumeBelow200000 = atoll(setDayLineTemp.m_CanceledBuyVolumeBelow200000);
	m_lCanceledBuyVolumeAbove200000 = atoll(setDayLineTemp.m_CanceledBuyVolumeAbove200000);
	m_lCanceledSellVolumeBelow5000 = atoll(setDayLineTemp.m_CanceledSellVolumeBelow5000);
	m_lCanceledSellVolumeBelow10000 = atoll(setDayLineTemp.m_CanceledSellVolumeBelow10000);
	m_lCanceledSellVolumeBelow20000 = atoll(setDayLineTemp.m_CanceledSellVolumeBelow20000);
	m_lCanceledSellVolumeBelow50000 = atoll(setDayLineTemp.m_CanceledSellVolumeBelow50000);
	m_lCanceledSellVolumeBelow100000 = atoll(setDayLineTemp.m_CanceledSellVolumeBelow100000);
	m_lCanceledSellVolumeBelow200000 = atoll(setDayLineTemp.m_CanceledSellVolumeBelow200000);
	m_lCanceledSellVolumeAbove200000 = atoll(setDayLineTemp.m_CanceledSellVolumeAbove200000);
}

bool CChinaStock::LoadDayLine(CString strStockCode) {
	return m_DayLine.LoadData(strStockCode);
}

bool CChinaStock::LoadDayLineBasicInfo(CSetDayLineBasicInfo* psetDayLineBasicInfo) {
	return m_DayLine.LoadBasicData(psetDayLineBasicInfo);
}

bool CChinaStock::CalculateDayLineRS(void) {
	return m_DayLine.CalculateRS0();
}

bool CChinaStock::CalculateDayLineRSLogarithm(void) {
	return m_DayLine.CalculateRSLogarithm0();
}

bool CChinaStock::CalculateDayLineRSIndex(void) {
	return m_DayLine.CalculateRSIndex0();
}

bool CChinaStock::CalculateWeekLineRS(void) {
	return m_WeekLine.CalculateRS0();
}

bool CChinaStock::CalculateWeekLineRSLogarithm(void) {
	return m_WeekLine.CalculateRSLogarithm0();
}

bool CChinaStock::CalculateWeekLineRSIndex(void) {
	return m_WeekLine.CalculateRSIndex0();
}

bool CChinaStock::Calculate10RSStrong2StockSet(void) {
	CSetDayLineBasicInfo setDayLineBasicInfo;
	vector<double> m_v10DaysRS;
	int iCountFirst = 0, iCountSecond = 0;

	ASSERT(m_DayLine.IsDataLoaded());
	const size_t iDayLineSize = GetDayLineSize();
	if (iDayLineSize > 155) {
		m_v10DaysRS.resize(iDayLineSize);
		CalculateDayLineRSIndex();
		Get10DaysRS(m_v10DaysRS);
		int i = 0;
		for (i = iDayLineSize - 1; i > iDayLineSize - 5; i--) {
			if (m_v10DaysRS.at(i) > 55) iCountFirst++;
			if (iCountFirst >= 3) break;
		}
		if (iCountFirst >= 3) {
			while (m_v10DaysRS.at(i) > 53) i--;
			for (int j = i; j > i - 50; j--) {
				if (m_v10DaysRS.at(j) > 55) iCountSecond++;
			}
			if (iCountSecond >= 3) {
				return true;
			}
		}
	}

	return false;
}

bool CChinaStock::Calculate10RSStrong1StockSet(void) {
	CSetDayLineBasicInfo setDayLineBasicInfo;
	vector<double> m_v10DaysRS;
	int iCountFirst = 0, iCountSecond = 0, iCountThird = 0;

	ASSERT(m_DayLine.IsDataLoaded());
	const size_t iDayLineSize = GetDayLineSize();

	if (iDayLineSize < 350) return false;
	m_v10DaysRS.resize(iDayLineSize);
	CalculateDayLineRSIndex();
	Get10DaysRS(m_v10DaysRS);
	int i = 0, j = 0;

	for (i = iDayLineSize - 1; i > iDayLineSize - 40; i--) {
		if (m_v10DaysRS.at(i) > 55) iCountFirst++;
		if (iCountFirst >= 3) break;
	}
	if (iCountFirst < 3) return false;
	while (m_v10DaysRS.at(i) > 53) i--;
	for (j = i; j > i - 80; j--) {
		if (m_v10DaysRS.at(j) < 55) iCountSecond++;
		if (iCountSecond >= 78) break;
	}
	if (iCountSecond < 78) return false;
	while (m_v10DaysRS.at(j) > 53) j--;
	for (int k = j; k > j - 130; k--) {
		if (m_v10DaysRS.at(k) > 47) iCountThird++;
		if (iCountThird > 128) break;
	}
	if (iCountThird < 128) return false;

	return true;
}

bool CChinaStock::Calculate10RSStrongStockSet(const CRSReference* pRef) {
	CSetDayLineBasicInfo setDayLineBasicInfo;
	vector<double> m_v10DaysRS;
	int iCountFirst = 0, iCountSecond = 0, iCountThird = 0, iCountFourth = 0;
	bool fFind1 = false, fFind2 = false, fFind3 = false, fFind4 = false;
	double dStrong1{ 0.0 }, dStrong2{ 0.0 }, dStrong3{ 0.0 }, dStrong4{ 0.0 };
	bool fFindHigh1{ false }, fFindHigh2{ false }, fFindHigh3{ false }, fFindHigh4{ false };

	if (pRef->m_dRSStrong[0] < 0) {
		dStrong1 = -pRef->m_dRSStrong[0];
		fFindHigh1 = false;
	}
	else {
		dStrong1 = pRef->m_dRSStrong[0];
		fFindHigh1 = true;
	}
	if (pRef->m_dRSStrong[1] < 0) {
		dStrong2 = -pRef->m_dRSStrong[1];
		fFindHigh2 = false;
	}
	else {
		dStrong2 = pRef->m_dRSStrong[1];
		fFindHigh2 = true;
	}
	if (pRef->m_dRSStrong[2] < 0) {
		dStrong3 = -pRef->m_dRSStrong[2];
		fFindHigh3 = false;
	}
	else {
		dStrong3 = pRef->m_dRSStrong[2];
		fFindHigh3 = true;
	}
	if (pRef->m_dRSStrong[3] < 0) {
		dStrong4 = -pRef->m_dRSStrong[3];
		fFindHigh4 = false;
	}
	else {
		dStrong4 = pRef->m_dRSStrong[3];
		fFindHigh4 = true;
	}

	ASSERT(m_DayLine.IsDataLoaded());
	const size_t iDayLineSize = GetDayLineSize();
	if ((iDayLineSize < (pRef->m_lDayLength[0] + pRef->m_lDayLength[1] + pRef->m_lDayLength[2] + 10))
		|| (iDayLineSize < pRef->m_lDayLength[3] + 10)) return false;

	m_v10DaysRS.resize(iDayLineSize);
	CalculateDayLineRSIndex();
	Get10DaysRS(m_v10DaysRS);
	int i = 0, j = 0;

	if (pRef->m_lDayLength[0] == 0) fFind1 = true;
	else if (fFindHigh1) {
		for (i = iDayLineSize - 1; i > (iDayLineSize - pRef->m_lDayLength[0]); i--) {
			if (m_v10DaysRS.at(i) > dStrong1) iCountFirst++;
			if (iCountFirst >= pRef->m_lStrongDayLength[0]) {
				fFind1 = true;
				break;
			}
		}
	}
	else {
		for (i = iDayLineSize - 1; i > (iDayLineSize - pRef->m_lDayLength[0]); i--) {
			if (m_v10DaysRS.at(i) < dStrong1) iCountFirst++;
			if (iCountFirst >= pRef->m_lStrongDayLength[0]) {
				fFind1 = true;
				break;
			}
		}
	}
	if (!fFind1) return false;
	while (m_v10DaysRS.at(i) > (dStrong1 - 2)) i--;

	if (pRef->m_lDayLength[1] == 0) fFind2 = true;
	else if (fFindHigh2) {
		for (j = i; j > (i - pRef->m_lDayLength[1]); j--) {
			if (m_v10DaysRS.at(j) > dStrong2) iCountSecond++;
			if (iCountSecond >= pRef->m_lStrongDayLength[1]) {
				fFind2 = true;
				break;
			}
		}
	}
	else {
		for (j = i; j > (i - pRef->m_lDayLength[1]); j--) {
			if (m_v10DaysRS.at(j) < dStrong2) iCountSecond++;
			if (iCountSecond >= pRef->m_lStrongDayLength[1]) {
				fFind2 = true;
				break;
			}
		}
	}
	if (!fFind2) return false;
	while (m_v10DaysRS.at(j) > (dStrong2 - 2)) j--;

	if (pRef->m_lDayLength[2] == 0) fFind3 = true;
	else if (fFindHigh3) {
		for (int k = j; k > (j - pRef->m_lDayLength[2]); k--) {
			if (m_v10DaysRS.at(k) > dStrong3) iCountThird++;
			if (iCountThird >= pRef->m_lStrongDayLength[2]) {
				fFind3 = true;
				break;
			}
		}
	}
	else {
		for (int k = j; k > (j - pRef->m_lDayLength[2]); k--) {
			if (m_v10DaysRS.at(k) < dStrong3) iCountThird++;
			if (iCountThird >= pRef->m_lStrongDayLength[2]) {
				fFind3 = true;
				break;
			}
		}
	}
	if (!fFind3) return false;

	if (pRef->m_lDayLength[3] == 0) return true; // DayLength4Ϊ��Ļ�������ͨ��ѡ��
	else if (fFindHigh4) {
		for (i = iDayLineSize - 1; i > (iDayLineSize - pRef->m_lDayLength[3]); i--) {
			if (m_v10DaysRS.at(i) > dStrong4) iCountFourth++;
			if (iCountFourth >= pRef->m_lStrongDayLength[3]) {
				fFind4 = true;
				break;
			}
		}
	}
	else {
		for (i = iDayLineSize - 1; i > (iDayLineSize - pRef->m_lDayLength[3]); i--) {
			if (m_v10DaysRS.at(i) < dStrong4) iCountFourth++;
			if (iCountFourth >= pRef->m_lStrongDayLength[3]) {
				fFind4 = true;
				break;
			}
		}
	}
	if (!fFind4) return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// �˺����������߳�ThreadCalculatingRTDataProc���ã��������ٶ�ȫ�ֱ����Ĳ�����
//
// ��������͹ҵ�����ƺ��ֿ�����ȽϺã�˼·����������Ŀǰ�������������й�����Ҫ���Ǿɻ������ٶ��ˡ�
//
//
////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::ProcessRTData(void) {
	CWebRTDataPtr pRTData;

	const INT64 lTotalNumber = GetRTDataQueueSize(); //  ������еĳ��ȡ�����ͬ�����ƻ�ȡ����ֵ.
	if (lTotalNumber == 0) return false;
	// ����Ϊ����ҵ��仯����Ʊ��Ծ�ȡ����������
	for (INT64 i = 0; i < lTotalNumber; i++) {
		pRTData = PopRTData(); // ����ͬ�����ƻ�ȡ����
		if (pRTData->IsActive()) { // ������Ч
			UpdateStatus(pRTData);   // ���¹�Ʊ��ʱ״̬��
			if (gl_pChinaMarket->IsMarketOpened() && IsNeedProcessRTData()) {// ����ʱ���ڼ�����������ָ�����Ʊ������㽻������͹ҵ��仯
				ProcessOneRTData(pRTData);
				CheckCurrentRTData();
				m_fRTDataCalculated = true;
			}
		}
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ���ҵ���
// ����ǵ�һ�μ��㣬��ֻ���ó�ʼ״̬��
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::ProcessOneRTData(CWebRTDataPtr pRTData) {
	CalculateHighLowLimit(pRTData);
	if (HaveFirstRTData()) { // �����ʼ���㣨�ڶ����յ�ʵʱ���ݼ��Ժ�
		CalculateOneRTData(pRTData);
	}
	else { // ��һ���յ�ʵʱ���ݣ���ֻ��ʼ��ϵͳ��������
		InitializeCalculatingRTDataEnvionment(pRTData);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// �й����е��ǵ�ͣ��۸���㷽��Ŀǰ��֪��ֻ�ܿ�������

void CChinaStock::CalculateHighLowLimit(CWebRTDataPtr pRTData) {
	double d1, d2;
	int iAdjust = 0;
	int iCompare = 10;
	int i2 = 0;

	if (pRTData->GetPSell(0) == 0) { // ��һ�۸�Ϊ��
		if (pRTData->GetPBuy(0) > 0) {
			m_lHighLimit2 = pRTData->GetPBuy(0);
			if (pRTData->GetLastClose() < 1200) { // �ͼ۹ɣ�
				m_lLowLimit2 = pRTData->GetLastClose() - (m_lHighLimit2 - pRTData->GetLastClose());
			}
			else {
				i2 = pRTData->GetPBuy(0) - pRTData->GetLastClose();
				iCompare = (static_cast<double>(i2) * 100 + pRTData->GetLastClose() * 0.65) / pRTData->GetLastClose(); // ϵ��0.70��ʵ������ģ�Ŀǰ��ͨ�á�
				if (iCompare <= 21) {
					if ((iCompare % 5) != 0) { // ȷ���ǵ���Ϊ5%�ı���
						TRACE("%s iCompare = %i, ����5�ı���\n", m_strSymbol.GetBuffer(), iCompare);
					}
					d1 = static_cast<double>(i2) * 100 / pRTData->GetLastClose();
					if (d1 > iCompare) {
						d2 = static_cast<double>(i2 - 10) * 100 / pRTData->GetLastClose();
						if ((iCompare - d2) <= (d1 - iCompare)) { // �������ͣ��ʱ���������һ��ʱ��9.7777��10.22222�������ý�С�ģ�9.7777����
							iAdjust = 10;
						}
					}
					m_lLowLimit2 = pRTData->GetLastClose() - i2 + iAdjust;
				}
				else {
					m_lLowLimit2 = m_lLowLimit;
				}
			}
			if (m_lHighLimit == 0) m_lHighLimit = m_lHighLimit2;
			if (m_lLowLimit == 0) m_lLowLimit = m_lLowLimit2;
		}
		else { // ��һ��һͬʱΪ��
			m_lHighLimit2 = m_lHighLimit;
			m_lLowLimit2 = m_lLowLimit;
		}
	}
	else if (pRTData->GetPBuy(0) == 0) { // ��һ�۸�Ϊ��
		if (pRTData->GetPSell(0) > 0) {
			m_lLowLimit2 = pRTData->GetPSell(0);
			if (pRTData->GetLastClose() < 1200) { // �ͼ۹ɣ�
				m_lHighLimit2 = pRTData->GetLastClose() + (pRTData->GetLastClose() - m_lLowLimit2);
			}
			else {
				i2 = pRTData->GetLastClose() - pRTData->GetPSell(0);
				iCompare = (static_cast<double>(i2) * 100 + pRTData->GetLastClose() * 0.65) / pRTData->GetLastClose(); // ϵ��0.70��ʵ������ģ�Ŀǰ��ͨ�á�
				if (iCompare <= 21) {
					if ((iCompare % 5) != 0) { // ȷ���ǵ���Ϊ5%�ı���
						TRACE("%s iCompare = %i, ����5�ı���\n", m_strSymbol.GetBuffer(), iCompare);
					}
					d1 = static_cast<double>(i2) * 100 / pRTData->GetLastClose();
					if (d1 < iCompare) {
						d2 = static_cast<double>(i2 + 10) * 100 / pRTData->GetLastClose();
						if ((d2 - iCompare) <= (iCompare - d1)) { // ��������ͣ��ʱ���������һ��ʱ��9.7777��10.22222�������ýϴ�ģ�10.2222����
							iAdjust = 10;
						}
					}
					m_lHighLimit2 = pRTData->GetLastClose() + i2 + iAdjust;
				}
				else {
					m_lHighLimit2 = m_lHighLimit;
				}
			}
			if (m_lHighLimit == 0) m_lHighLimit = m_lHighLimit2;
			if (m_lLowLimit == 0) m_lLowLimit = m_lLowLimit2;
		}
		else { // ��һ��һͬʱΪ��
			m_lHighLimit2 = m_lHighLimit;
			m_lLowLimit2 = m_lLowLimit;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//
// ��һ���յ�ʵʱ����ʱ��ֻ��ʼ��ϵͳ�������㣨��Ϊû�г�ʼ���ݣ�
//
////////////////////////////////////////////////////////////////////////////////////////
void CChinaStock::InitializeCalculatingRTDataEnvionment(CWebRTDataPtr pRTData) {
	SetLastRTData(pRTData);
	SetHavingFirstRTData(true);
	// ��һ�ιҵ����޷��ж�����״̬���ʶ�������Ϊ�޷��жϡ����֮ǰ�Ѿ����й�ϵͳ���˴��ǿ�����;��¼�ģ���ϵͳ�洢����ʱ���������ݿ��У�
	// ��ϵͳ����ʱ�Ѿ�������ʱ״̬�����ˣ��ʶ�m_lUnknownVolume��Ϊ�㣬�ʶ���Ҫ���¼���m_lUnknownVolume.
	// m_lUnknownVolume = ��ǰ�ĳɽ��� - ֮ǰ��ʱ�洢�ĳɽ��� + ֮ǰ�洢��m_lUnkonwnVolume.
	SetVolume(pRTData->GetVolume());
	SetUnknownVolume(GetUnknownVolume() + m_pLastRTData->GetVolume() - m_llLastSavedVolume);

	// ���õ�һ�εĹҵ�ӳ�䡣
	for (int j = 0; j < 5; j++) {
		SetGuadan(pRTData->GetPBuy(j), pRTData->GetVBuy(j));
		SetGuadan(pRTData->GetPSell(j), pRTData->GetVSell(j));
	}
}

void CChinaStock::CalculateOneRTData(CWebRTDataPtr pRTData) {
	long lCurrentGuadanTransactionPrice = 0;

	ResetCalculatingData();
	m_lCurrentGuadanTransactionVolume = pRTData->GetVolume() - m_pLastRTData->GetVolume();
	if (m_lCurrentGuadanTransactionVolume == 0) { // �޳ɽ���
		// ���ҵ����
		lCurrentGuadanTransactionPrice = 0;
		m_nCurrentTransactionType = __NO_TRANSACTION__;
	}
	else {
		lCurrentGuadanTransactionPrice = (pRTData->GetAmount() - m_pLastRTData->GetAmount()) * 1000 / m_lCurrentGuadanTransactionVolume; // ���ɱȽ��õļ۸񣨷Ŵ�һǧ������ó����ͣ�
		if ((lCurrentGuadanTransactionPrice == m_lHighLimit) || (lCurrentGuadanTransactionPrice == m_lLowLimit)) { // �ǵ�ͣ��ʱ���ɽ�������δ֪�ɽ�����
			IncreaseTransactionNumber();
			m_nCurrentTransactionType = __UNKNOWN_BUYSELL__;
			m_lUnknownVolume += m_lCurrentGuadanTransactionVolume;
		}
		else {
			CalculateOneDeal(pRTData, lCurrentGuadanTransactionPrice);
		}
	}

	// ���濪ʼ�����ҵ����
	AnalysisGuadan(pRTData, lCurrentGuadanTransactionPrice);

	// ����ǰ����״̬
	SetLastRTData(pRTData);
}

void CChinaStock::CalculateOneDeal(CWebRTDataPtr pRTData, INT64 lCurrentGuadanTransactionPrice) {
	IncreaseTransactionNumber();
	lCurrentGuadanTransactionPrice = (pRTData->GetAmount() - m_pLastRTData->GetAmount()) * 1000 / m_lCurrentGuadanTransactionVolume; // ���ɱȽ��õļ۸񣨷Ŵ�һǧ������ó����ͣ�
	m_dCurrentGuadanTransactionPrice = static_cast<double>(lCurrentGuadanTransactionPrice) / 1000; // �任��ʵ�ʼ۸�
	if (lCurrentGuadanTransactionPrice >= m_pLastRTData->GetPBuy(0)) { // ����ǰ����1
		if (lCurrentGuadanTransactionPrice <= m_pLastRTData->GetPSell(0)) { // ����ǰ����1
			CalculateOrdinaryBuySell(lCurrentGuadanTransactionPrice);
		}
		else if (lCurrentGuadanTransactionPrice < m_pLastRTData->GetPSell(1)) { // ��������һ���������̶������������롣AttackBuy
			CalculateAttackBuy();
		}
		else { // �������̶���ǿ�����롣StrongBuy��
			CalculateStrongBuy();
		}
	} // ����ǰ��һ
	else if (lCurrentGuadanTransactionPrice > m_pLastRTData->GetPBuy(1)) { // ��������һ���������̶���������������AttackSell
		CalculateAttackSell();
	}
	else { // �������̶���ǿ��������StrongSell
		CalculateStrongSell();
	}
	ASSERT(GetTransactionTime() >= pRTData->GetTransactionTime());
	const INT64 I = pRTData->GetVolume();
	const INT64 j = GetOrdinaryBuyVolume() + GetOrdinarySellVolume()
		+ GetAttackBuyVolume() + GetAttackSellVolume()
		+ GetStrongBuyVolume() + GetStrongSellVolume() + GetUnknownVolume();
	ASSERT(pRTData->GetVolume() == GetOrdinaryBuyVolume() + GetOrdinarySellVolume()
		+ GetAttackBuyVolume() + GetAttackSellVolume()
		+ GetStrongBuyVolume() + GetStrongSellVolume() + GetUnknownVolume());
}

void CChinaStock::IncreaseTransactionNumber(void) {
	m_lTransactionNumber++; // �ɽ�����һ��
	if (m_lCurrentGuadanTransactionVolume < 5000) {
		m_lTransactionNumberBelow5000++;
	}
	else if (m_lCurrentGuadanTransactionVolume < 50000) {
		m_lTransactionNumberBelow50000++;
	}
	else if (m_lCurrentGuadanTransactionVolume < 200000) {
		m_lTransactionNumberBelow200000++;
	}
	else {
		m_lTransactionNumberAbove200000++;
	}
	ASSERT(GetTransactionNumber() == (GetTransactionNumberAbove200000()
		+ GetTransactionNumberBelow200000()
		+ GetTransactionNumberBelow50000() + GetTransactionNumberBelow5000()));
}

void CChinaStock::CalculateOrdinaryBuySell(INT64 lCurrentGuadanTransactionPrice) {
	if ((m_pLastRTData->GetPSell(0) - lCurrentGuadanTransactionPrice) <= 2) { //һ��������
		m_lOrdinaryBuyVolume += m_lCurrentGuadanTransactionVolume;
		m_nCurrentTransactionType = __ORDINARY_BUY__;
		CalculateOrdinaryBuyVolume();
	}
	else if ((lCurrentGuadanTransactionPrice - m_pLastRTData->GetPBuy(0)) <= 2) { // һ��������
		m_nCurrentTransactionType = __ORDINARY_SELL__;
		m_lOrdinarySellVolume += m_lCurrentGuadanTransactionVolume;
		CalculateOrdinarySellVolume();
	}
	else { // �������ӣ���������
		m_nCurrentTransactionType = __UNKNOWN_BUYSELL__;
		m_lUnknownVolume += m_lCurrentGuadanTransactionVolume;
	}
}

void CChinaStock::CalculateOrdinaryBuyVolume(void) {
	if (m_lCurrentGuadanTransactionVolume < 5000) {
		m_lOrdinaryBuyNumberBelow5000++;
		m_lOrdinaryBuyVolumeBelow5000 += m_lCurrentGuadanTransactionVolume;
	}
	else if (m_lCurrentGuadanTransactionVolume < 10000) {
		m_lOrdinaryBuyNumberBelow10000++;
		m_lOrdinaryBuyVolumeBelow10000 += m_lCurrentGuadanTransactionVolume;
	}
	else if (m_lCurrentGuadanTransactionVolume < 20000) {
		m_lOrdinaryBuyNumberBelow20000++;
		m_lOrdinaryBuyVolumeBelow20000 += m_lCurrentGuadanTransactionVolume;
	}
	else if (m_lCurrentGuadanTransactionVolume < 50000) {
		m_lOrdinaryBuyNumberBelow50000++;
		m_lOrdinaryBuyVolumeBelow50000 += m_lCurrentGuadanTransactionVolume;
	}
	else if (m_lCurrentGuadanTransactionVolume < 100000) {
		m_lOrdinaryBuyNumberBelow100000++;
		m_lOrdinaryBuyVolumeBelow100000 += m_lCurrentGuadanTransactionVolume;
	}
	else if (m_lCurrentGuadanTransactionVolume < 200000) {
		m_lOrdinaryBuyNumberBelow200000++;
		m_lOrdinaryBuyVolumeBelow200000 += m_lCurrentGuadanTransactionVolume;
	}
	else if (m_lCurrentGuadanTransactionVolume >= 200000) {
		m_lOrdinaryBuyNumberAbove200000++;
		m_lOrdinaryBuyVolumeAbove200000 += m_lCurrentGuadanTransactionVolume;
	}
}

void CChinaStock::CalculateOrdinarySellVolume(void) {
	if (m_lCurrentGuadanTransactionVolume < 5000) {
		m_lOrdinarySellNumberBelow5000++;
		m_lOrdinarySellVolumeBelow5000 += m_lCurrentGuadanTransactionVolume;
	}
	else if (m_lCurrentGuadanTransactionVolume < 10000) {
		m_lOrdinarySellNumberBelow10000++;
		m_lOrdinarySellVolumeBelow10000 += m_lCurrentGuadanTransactionVolume;
	}
	else if (m_lCurrentGuadanTransactionVolume < 20000) {
		m_lOrdinarySellNumberBelow20000++;
		m_lOrdinarySellVolumeBelow20000 += m_lCurrentGuadanTransactionVolume;
	}
	else if (m_lCurrentGuadanTransactionVolume < 50000) {
		m_lOrdinarySellNumberBelow50000++;
		m_lOrdinarySellVolumeBelow50000 += m_lCurrentGuadanTransactionVolume;
	}
	else if (m_lCurrentGuadanTransactionVolume < 100000) {
		m_lOrdinarySellNumberBelow100000++;
		m_lOrdinarySellVolumeBelow100000 += m_lCurrentGuadanTransactionVolume;
	}
	else if (m_lCurrentGuadanTransactionVolume < 200000) {
		m_lOrdinarySellNumberBelow200000++;
		m_lOrdinarySellVolumeBelow200000 += m_lCurrentGuadanTransactionVolume;
	}
	else if (m_lCurrentGuadanTransactionVolume >= 200000) {
		m_lOrdinarySellNumberAbove200000++;
		m_lOrdinarySellVolumeAbove200000 += m_lCurrentGuadanTransactionVolume;
	}
}

void CChinaStock::CalculateAttackBuy(void) {
	m_nCurrentTransactionType = __ATTACK_BUY__;
	m_lAttackBuyVolume += m_lCurrentGuadanTransactionVolume;
	CalculateAttackBuyVolume();
}

void CChinaStock::CalculateStrongBuy(void) {
	m_nCurrentTransactionType = __STRONG_BUY__;
	m_lStrongBuyVolume += m_lCurrentGuadanTransactionVolume;
	CalculateAttackBuyVolume();
}

void CChinaStock::CalculateAttackBuyVolume(void) {
	if (m_lCurrentGuadanTransactionVolume < 50000) {
		m_lAttackBuyBelow50000 += m_lCurrentGuadanTransactionVolume;
	}
	else if (m_lCurrentGuadanTransactionVolume < 200000) {
		m_lAttackBuyBelow200000 += m_lCurrentGuadanTransactionVolume;
	}
	else {
		m_lAttackBuyAbove200000 += m_lCurrentGuadanTransactionVolume;
	}
}

void CChinaStock::CalculateAttackSell(void) {
	m_nCurrentTransactionType = __ATTACK_SELL__;
	m_lAttackSellVolume += m_lCurrentGuadanTransactionVolume;
	CalculateAttackSellVolume();
}

void CChinaStock::CalculateStrongSell(void) {
	m_nCurrentTransactionType = __STRONG_SELL__;
	m_lStrongSellVolume += m_lCurrentGuadanTransactionVolume;
	CalculateAttackSellVolume();
}

void CChinaStock::CalculateAttackSellVolume(void) {
	if (m_lCurrentGuadanTransactionVolume < 50000) {
		m_lAttackSellBelow50000 += m_lCurrentGuadanTransactionVolume;
	}
	else if (m_lCurrentGuadanTransactionVolume < 200000) {
		m_lAttackSellBelow200000 += m_lCurrentGuadanTransactionVolume;
	}
	else {
		m_lAttackSellAbove200000 += m_lCurrentGuadanTransactionVolume;
	}
}

void CChinaStock::ResetCalculatingData(void) {
	m_lCurrentCanceledBuyVolume = 0;
	m_lCurrentCanceledSellVolume = 0;
	m_lCurrentGuadanTransactionVolume = 0;
	m_lCurrentStrongBuy = 0;
	m_lCurrentStrongSell = 0;
	m_lCurrentUnknown = 0;
	m_lCurrentAttackBuy = 0;
	m_lCurrentAttackSell = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ҵ����ı仯���ɹ����̵߳���
//
// ����map�ṹ�洢����������������⣬Ψһ�������Ѱַ����ʱ�䳤����Ҫ���Ժ󿴿��ٶ����(���Ժ��ٶ�û�κ����⣩��
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::AnalysisGuadan(CWebRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice) {
	// ��Ҫ���Ĺҵ�λ�á�˳��Ϊ������4, ����3, ... ����0, ����0, .... ��3, ��4
	// ������˭��ǰ������ν��������ʱ��Ҫ��ס��˳��
	array<bool, 10> fNeedCheck{ true,true,true,true,true,true,true,true,true,true };

	for (int i = 0; i < 10; i++) fNeedCheck.at(i) = true; // Ԥ��Ϊ��Ҫ���㡣
	SelectGuadanThatNeedToCalculate(pCurrentRTData, lCurrentTransactionPrice, fNeedCheck);
	SetCurrentGuadan(pCurrentRTData);
	// �����ʮ���ҵ���λ�Ϲ����ı仯���.��Ŀǰ�ҵ�״̬��֮ǰ��ʮ����λ��m_pLastRTData�����ģ���ȣ��鿴�仯
	CheckGuadan(pCurrentRTData, fNeedCheck);

	return(true);
}

void CChinaStock::SelectGuadanThatNeedToCalculate(CWebRTDataPtr pCurrentRTData, INT64 lCurrentTransactionPrice, array<bool, 10>& fNeedCheck) {
	// ȷ����Ҫ������Щ�ҵ���һ����ʮ����û���ܵ�����Ӱ��Ķ�Ҫ���㡣
	switch (m_nCurrentTransactionType) {
	case __NO_TRANSACTION__: // û�гɽ�������ٵ���������Ӧ��λ�ϵĳ�����
		ASSERT(lCurrentTransactionPrice == 0);
		break;
	case __ATTACK_BUY__: // ����һ�Ѿ���ʧ����������Ӱ�졣���������߸��ҵ���
		fNeedCheck.at(3) = false;
		fNeedCheck.at(4) = fNeedCheck.at(5) = false;
		break;
	case __STRONG_BUY__: // ����һ����������ʧ������������δ������Ҫ���㡣
		if (lCurrentTransactionPrice < m_pLastRTData->GetPSell(2)) { // ����4������5�д�
			fNeedCheck.at(2) = false;
		}
		else if (lCurrentTransactionPrice < m_pLastRTData->GetPSell(3)) { // ����5�д�
			fNeedCheck.at(1) = false;
			fNeedCheck.at(2) = false;
		}
		else { // ���е���������Ӱ��
			fNeedCheck.at(0) = false;
			fNeedCheck.at(2) = false;
			fNeedCheck.at(1) = false;
		}
		fNeedCheck.at(3) = false;
		fNeedCheck.at(4) = false;
		fNeedCheck.at(5) = false;
		break;
	case __ORDINARY_BUY__: // ����һ��Ӱ�졣���������˸��ҵ���
	case __UNKNOWN_BUYSELL__: // ����һ����һ��Ӱ�졣���������˸��ҵ���
	case __ORDINARY_SELL__: // ��һ��Ӱ�졣���������˸��ҵ���
		fNeedCheck.at(4) = fNeedCheck.at(5) = false;
		break;
	case __ATTACK_SELL__: // ��һ��ʧ���򵥶���Ӱ�졣���������߸��ҵ���
		fNeedCheck.at(4) = fNeedCheck.at(5) = fNeedCheck.at(6) = false;
		break;
	case __STRONG_SELL__: // ��һ���򵥶���ʧ����������Ҫ���㡣
		if (m_pLastRTData->GetPBuy(3) > lCurrentTransactionPrice) { // �����򵥶���Ӱ��
			fNeedCheck.at(7) = false;
			fNeedCheck.at(8) = false;
			fNeedCheck.at(9) = false;
		}
		else if (m_pLastRTData->GetPBuy(2) > lCurrentTransactionPrice) { // ��5�д�
			fNeedCheck.at(7) = false;
			fNeedCheck.at(8) = false;
		}
		else { // ��4����5û�仯
			fNeedCheck.at(7) = false;
		}
		fNeedCheck.at(6) = false;
		fNeedCheck.at(5) = false;
		fNeedCheck.at(4) = false;
		break;
	default:
		ASSERT(0); // ������ִ�е��˷�֧��
		break;
	}
}

void CChinaStock::SetCurrentGuadan(CWebRTDataPtr pCurrentRTData) {
	// ��λ�������ǳɽ��ˣ�Ҳ�����ǳ����ˣ�Ŀǰ��������Щϸ�ڣ�ͳһ��Ϊ�ǳɽ��ˣ������㳷�������Ժ��ٷ���֮��
	// ����յ�ǰ�ҵ�֮��Ĺҵ�������Ȼ�����ϵ�ǰ��������пյ��Ļ�������Ȼ����ˡ�
	for (int i = pCurrentRTData->GetPBuy(4); i <= pCurrentRTData->GetPSell(4); i += 10) {
		m_mapGuadan[i] = 0; // �˴���Ҫ����ֵ���п���֮ǰΪ�գ��ʶ�����ʹ��at��������������[]�±������
	}
	for (int i = 0; i < 5; i++) { // ��Ŀǰ��ʮ���ҵ�״̬����ӳ���У��ҵ�״̬����Ϊ����̬
		SetGuadan(pCurrentRTData->GetPSell(i), pCurrentRTData->GetVSell(i));
		SetGuadan(pCurrentRTData->GetPBuy(i), pCurrentRTData->GetVBuy(i));
	}
}

void CChinaStock::CheckGuadan(CWebRTDataPtr pCurrentRTData, array<bool, 10>& fNeedCheck) {
	for (int i = 0; i < 5; i++) {
		CheckSellGuadan(fNeedCheck, i);
		CheckBuyGuadan(fNeedCheck, i);
	}
}

void CChinaStock::CheckSellGuadan(array<bool, 10>& fNeedCheck, int i) {
	ASSERT((i < 5) && (i >= 0));
	INT64 iTemp = 0;
	if (fNeedCheck.at(4 - i)) {
		if (GetGuadan(m_pLastRTData->GetPSell(i)) < m_pLastRTData->GetVSell(i)) { // �����˵Ļ�
			iTemp = m_pLastRTData->GetVSell(i) - GetGuadan(m_pLastRTData->GetPSell(i));
			m_lCurrentCanceledSellVolume += iTemp;
			CalculateCanceledSellVolume(iTemp);
			m_lCanceledSellVolume += iTemp;
		}
	}
	ASSERT(m_lCanceledSellVolume == (m_lCanceledSellVolumeAbove200000 + m_lCanceledSellVolumeBelow10000 + m_lCanceledSellVolumeBelow100000
		+ m_lCanceledSellVolumeBelow20000 + m_lCanceledSellVolumeBelow200000 + m_lCanceledSellVolumeBelow5000
		+ m_lCanceledSellVolumeBelow50000));
}

void CChinaStock::CalculateCanceledSellVolume(INT64 lCurrentCancelSellVolume) {
	if (lCurrentCancelSellVolume < 5000) {
		m_lCanceledSellVolumeBelow5000 += lCurrentCancelSellVolume;
	}
	else if (lCurrentCancelSellVolume < 10000) {
		m_lCanceledSellVolumeBelow10000 += lCurrentCancelSellVolume;
	}
	else if (lCurrentCancelSellVolume < 20000) {
		m_lCanceledSellVolumeBelow20000 += lCurrentCancelSellVolume;
	}
	else if (lCurrentCancelSellVolume < 50000) {
		m_lCanceledSellVolumeBelow50000 += lCurrentCancelSellVolume;
	}
	else if (lCurrentCancelSellVolume < 100000) {
		m_lCanceledSellVolumeBelow100000 += lCurrentCancelSellVolume;
	}
	else if (lCurrentCancelSellVolume < 200000) {
		m_lCanceledSellVolumeBelow200000 += lCurrentCancelSellVolume;
	}
	else {
		m_lCanceledSellVolumeAbove200000 += lCurrentCancelSellVolume;
	}
}

void CChinaStock::CheckBuyGuadan(array<bool, 10>& fNeedCheck, int i) {
	ASSERT((i < 5) && (i >= 0));
	INT64 iTemp = 0;
	if (fNeedCheck.at(5 + i)) {
		if (GetGuadan(m_pLastRTData->GetPBuy(i)) < m_pLastRTData->GetVBuy(i)) { // �����˵Ļ�
			iTemp = m_pLastRTData->GetVBuy(i) - GetGuadan(m_pLastRTData->GetPBuy(i));
			m_lCurrentCanceledBuyVolume += iTemp;
			CalculateCanceledBuyVolume(iTemp);
			m_lCanceledBuyVolume += iTemp;
		}
	}
	ASSERT(m_lCanceledBuyVolume == (m_lCanceledBuyVolumeAbove200000 + m_lCanceledBuyVolumeBelow10000 + m_lCanceledBuyVolumeBelow100000
		+ m_lCanceledBuyVolumeBelow20000 + m_lCanceledBuyVolumeBelow200000 + m_lCanceledBuyVolumeBelow5000
		+ m_lCanceledBuyVolumeBelow50000));
}

void CChinaStock::CalculateCanceledBuyVolume(INT64 lCurrentCancelBuyVolume) {
	if (lCurrentCancelBuyVolume < 5000) {
		m_lCanceledBuyVolumeBelow5000 += lCurrentCancelBuyVolume;
	}
	else if (lCurrentCancelBuyVolume < 10000) {
		m_lCanceledBuyVolumeBelow10000 += lCurrentCancelBuyVolume;
	}
	else if (lCurrentCancelBuyVolume < 20000) {
		m_lCanceledBuyVolumeBelow20000 += lCurrentCancelBuyVolume;
	}
	else if (lCurrentCancelBuyVolume < 50000) {
		m_lCanceledBuyVolumeBelow50000 += lCurrentCancelBuyVolume;
	}
	else if (lCurrentCancelBuyVolume < 100000) {
		m_lCanceledBuyVolumeBelow100000 += lCurrentCancelBuyVolume;
	}
	else if (lCurrentCancelBuyVolume < 200000) {
		m_lCanceledBuyVolumeBelow200000 += lCurrentCancelBuyVolume;
	}
	else {
		m_lCanceledBuyVolumeAbove200000 += lCurrentCancelBuyVolume;
	}
}

bool CChinaStock::HaveGuadan(INT64 lPrice) {
	if (m_mapGuadan.find(lPrice) == m_mapGuadan.end()) return false;
	else if (m_mapGuadan.at(lPrice) == 0) return false;
	return true;
}

bool CChinaStock::CheckCurrentRTData() {
	if ((GetOrdinaryBuyVolume() < 0) || (GetOrdinarySellVolume() < 0)
		|| (GetAttackBuyVolume() < 0) || (GetAttackSellVolume() < 0)
		|| (GetStrongBuyVolume() < 0) || (GetStrongSellVolume() < 0)) {
		int j = 0;
		if (GetOrdinaryBuyVolume() < 0) j = 1;
		if (GetOrdinarySellVolume() < 0) j += 2;
		if (GetAttackBuyVolume() < 0) j += 4;
		if (GetAttackSellVolume() < 0) j += 8;
		if (GetStrongBuyVolume() < 0) j += 16;
		if (GetStrongSellVolume() < 0) j += 32;
		TRACE(_T("%06d %s Error in volume. Error  code = %d\n"), gl_pChinaMarket->GetFormatedMarketTime(), GetSymbol().GetBuffer(), j);
		return false;
	}
	return true;
}

void CChinaStock::ShowCurrentTransaction() {
	// ��ʾ��ǰ�������

	if (GetCurrentTransationVolume() > 0) {
		ReportGuadanTransaction();
	}
}

void CChinaStock::ShowCurrentInformationOfCancelingGuadan(void) {
	// ��ʾ��ǰȡ���ҵ������
	ReportGuadan();
}

void CChinaStock::ReportGuadanTransaction(void) {
	char buffer[100];
	CString str, str2, strTime;
	const CTime ctime(m_pLastRTData->GetTransactionTime());
	sprintf_s(buffer, _T("%02d:%02d:%02d"), ctime.GetHour(), ctime.GetMinute(), ctime.GetSecond());
	strTime = buffer;
	sprintf_s(buffer, _T(" %s %I64d�ɳɽ���%10.3f    "), GetSymbol().GetBuffer(),
		m_lCurrentGuadanTransactionVolume, m_dCurrentGuadanTransactionPrice);
	str = strTime;
	str += buffer;
	CString str1;
	switch (m_nCurrentTransactionType) {
	case __STRONG_BUY__:
		str1 = _T(" STRONG BUY");
		sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, GetStrongBuyVolume());
		break;
	case __STRONG_SELL__:
		str1 = _T(" STRONG SELL");
		sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, GetStrongSellVolume());
		break;
	case __ATTACK_BUY__:
		str1 = _T(" ATTACK BUY");
		sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, GetAttackBuyVolume());
		break;
	case __ATTACK_SELL__:
		str1 = _T(" ATTACK SELL");
		sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, GetAttackSellVolume());
		break;
	case __ORDINARY_BUY__:
		str1 = _T(" ORDINARY BUY");
		sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, GetOrdinaryBuyVolume());
		break;
	case __ORDINARY_SELL__:
		str1 = _T(" ORDINARY SELL");
		sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, GetOrdinarySellVolume());
		break;
	case __UNKNOWN_BUYSELL__:
		str1 = _T(" UNKNOWN BUYSELL");
		sprintf_s(buffer, _T(": %I64d��  %I64d"), m_lCurrentGuadanTransactionVolume, GetUnknownVolume());
		break;
	default:
		break;
	}
	str += str1;
	// ��ʾ��ǰ�ɽ����
	gl_systemMessage.PushTransactionMessage(str); // ����ͬ�����ƴ�����Ϣ

	str2 = strTime;
	str2 += str1;
	str2 += buffer;
	gl_systemMessage.PushTrace2Message(str2);
}

void CChinaStock::ReportGuadan(void) {
	CString str1;
	char buffer[30];
	if (m_lCurrentCanceledSellVolume > 0) {
		sprintf_s(buffer, _T("��ǰȡ����������%I64d"), m_lCurrentCanceledSellVolume);
		str1 = buffer;
		sprintf_s(buffer, _T("  ��ȡ����������%I64d"), GetCanceledSellVolume());
		str1 += buffer;
		gl_systemMessage.PushCancelSellMessage(str1);   // ����ͬ�����ƴ�����Ϣ
	}
	if (m_lCurrentCanceledBuyVolume > 0) {
		sprintf_s(buffer, _T("��ǰȡ��������%I64d"), m_lCurrentCanceledBuyVolume);
		str1 = buffer;
		sprintf_s(buffer, _T("  ��ȡ��������%I64d"), GetCanceledBuyVolume());
		str1 += buffer;
		gl_systemMessage.PushCancelSellMessage(str1); // ����ͬ�����ƴ�����Ϣ
	}
}

bool CChinaStock::LoadStockCodeDB(CSetChinaStockSymbol& setChinaStockSymbol) {
	time_t tt = 0;
	long lDayLineEndDate = m_lDayLineEndDate; // ����Ŀǰ�������������

	LoadSymbol(setChinaStockSymbol);

	SetNeedProcessRTData(true);
	if (IsShanghaiExchange(GetSymbol())) {
		if (GetSymbol().Left(6) <= _T("000999")) { //����ָ����
			SetNeedProcessRTData(false);
		}
	}
	else if ((GetSymbol().Left(6) >= _T("399000"))) { // ����ָ��
		SetNeedProcessRTData(false);
	}
	if (GetDayLineEndDate() < lDayLineEndDate) { // ��ʱһ����Ʊ���ж����¼������������Ϊ׼��
		SetDayLineEndDate(lDayLineEndDate);
	}
	if (!IsDelisted()) {
		time(&tt);
		if (IsEarlyThen(GetDayLineEndDate(), FormatToDate(tt), 30)) {
			SetIPOStatus(__STOCK_DELISTED__);
			SetUpdateProfileDB(true);
		}
	}
	CheckDayLineStatus();
	return true;
}

bool CChinaStock::CheckDayLineStatus(void) {
	ASSERT(IsDayLineNeedUpdate()); // Ĭ��״̬Ϊ����������Ҫ����
	// ���ٸ����������ݱ��ϸ�������Ҫ�µĹ�Ʊ���������еĹ�Ʊ����ѯһ�飬�Է�ֹ�����¹�Ʊ�����ϵĹ�Ʊ���»�Ծ������
	if (gl_pChinaMarket->GetLastTradeDate() <= GetDayLineEndDate()) { // ������������Ϊ���ջ�����һ�������յ����ݡ�
		SetDayLineNeedUpdate(false); // �������ݲ���Ҫ����
	}
	else if (IsDelisted()) { // ���й�Ʊ��������ع��������ݣ���ÿ����һ������������
		if ((gl_pChinaMarket->GetDayOfWeek() != 1) && (GetDayLineEndDate() != __CHINA_MARKET_BEGIN_DATE__)) {
			SetDayLineNeedUpdate(false);
		}
	}
	return m_fDayLineNeedUpdate;
}

bool CChinaStock::BuildWeekLine(long lStartDate) {
	if (IsNullStock()) return true;
	if (!IsDayLineLoaded()) {
		LoadDayLine(GetSymbol());
	}
	if (GetDayLineSize() <= 0) return true;

	if (CalculatingWeekLine(lStartDate)) {
		SaveWeekLine();
	}

	if (gl_pChinaMarket->GetCurrentStock() != nullptr) {
		if (!IsSameStock(gl_pChinaMarket->GetCurrentStock())) {
			UnloadDayLine();
			UnloadWeekLine();
		}
	}
	else {
		UnloadDayLine();
		UnloadWeekLine();
	}
	return true;
}

bool CChinaStock::SaveWeekLine() {
	return m_WeekLine.SaveData(GetSymbol());
}

bool CChinaStock::LoadWeekLine() {
	return m_WeekLine.LoadData(GetSymbol());
}

bool CChinaStock::LoadWeekLineBasicInfo(CSetWeekLineBasicInfo* psetWeekLineBasicInfo) {
	return m_WeekLine.LoadBasicData(psetWeekLineBasicInfo);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ�����ƴ洢ʵʱ���ݡ�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CChinaStock::PushRTData(CWebRTDataPtr pData) {
	m_qRTData.PushData(pData);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ��������ȡʵʱ���ݡ�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
CWebRTDataPtr CChinaStock::PopRTData(void) {
	return m_qRTData.PopData();
}

CWebRTDataPtr CChinaStock::GetRTDataAtHead(void) {
	return m_qRTData.GetHead();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ��������ȡջ�ڵ�ʵʱ����������
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
INT64 CChinaStock::GetRTDataQueueSize(void) {
	return m_qRTData.GetDataSize();
}

bool CChinaStock::IsSameStock(CChinaStockPtr pStock) {
	if (pStock == nullptr) return false;
	else if (m_strSymbol.Compare(pStock->GetSymbol()) == 0) {
		ASSERT(m_lOffsetInContainer == pStock->GetOffset());
		return true;
	}
	else return false;
}

////////////////////////////////////////////////////////////////////////////////
//
// �жϹ�Ʊ�����Ƿ���Ч��������߼ۡ���ͼۡ��ɽ����ͳɽ������жϣ������Ϊ�㣬����Ϊ�˹ɽ���û����Ч����
// �����ÿ��̼ۣ����̼��п��ܲ�Ϊ��ʱ���������ݽ�Ϊ�㣨ͣ��ʱ����״̬����
//
////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::IsTodayDataActive(void) const {
	if (!m_fActive) return false;
	else {
		return IsTodayDataChanged();
	}
}

////////////////////////////////////////////////////////////////////////////////
//
// �жϹ�Ʊ�����Ƿ���Ч��������߼ۡ���ͼۡ��ɽ����ͳɽ������жϣ������Ϊ�㣬����Ϊ�˹ɽ���û����Ч����
// �����ÿ��̼ۣ����̼��п��ܲ�Ϊ��ʱ���������ݽ�Ϊ�㣨ͣ��ʱ����״̬����
//
////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::IsTodayDataChanged(void) const {
	if ((GetHigh() != 0) || (GetLow() != 0) || (GetAmount() != 0) || (GetVolume() != 0)) {
		return true;
	}
	else {
		return false;
	}
}

void CChinaStock::ShowDayLine(CDC* pDC, CRect rectClient) {
	m_DayLine.ShowData(pDC, rectClient);
}

void CChinaStock::ShowWeekLine(CDC* pDC, CRect rectClient) {
	m_WeekLine.ShowData(pDC, rectClient);
}

void CChinaStock::Get1DaysRS(vector<double>& vRS) {
	m_DayLine.GetRS1(vRS);
}

void CChinaStock::GetRSIndex1Day(vector<double>& vRS) {
	m_DayLine.GetRSIndex1(vRS);
}

void CChinaStock::GetRSLogarithm1Day(vector<double>& vRS) {
	m_DayLine.GetRSLogarithm1(vRS);
}

void CChinaStock::Get3DaysRS(vector<double>& vRS) {
	m_DayLine.GetRS3(vRS);
}

void CChinaStock::Get5DaysRS(vector<double>& vRS) {
	m_DayLine.GetRS5(vRS);
}

void CChinaStock::Get10DaysRS(vector<double>& vRS) {
	m_DayLine.GetRS10(vRS);
}

void CChinaStock::Get30DaysRS(vector<double>& vRS) {
	m_DayLine.GetRS30(vRS);
}

void CChinaStock::Get60DaysRS(vector<double>& vRS) {
	m_DayLine.GetRS60(vRS);
}

void CChinaStock::Get120DaysRS(vector<double>& vRS) {
	m_DayLine.GetRS120(vRS);
}

bool CChinaStock::IsVolumeConsistence(void) noexcept {
	if ((m_lHighLimit2 > 0) && (m_lLowLimit2 > 0)) {
		if ((m_lHighLimit != m_lHighLimit2) || (m_lLowLimit != m_lLowLimit2)) {
			TRACE(_T("%s�ǵ�ͣ��۸񲻷���%d %d    %d  %d\n"), GetSymbol().GetBuffer(), m_lHighLimit, m_lHighLimit2, m_lLowLimit, m_lLowLimit2);
		}
		if ((m_lPBuy[0] > 0) && (m_lPSell[0] > 0)) { // ���ǵ�ͣ���ʱ
			m_lHighLimit2 = m_lLowLimit2 = 0; // ���ô�������
		}
	}
	if (GetVolume() != GetOrdinaryBuyVolume() + GetOrdinarySellVolume() + GetAttackBuyVolume()
		+ GetAttackSellVolume() + GetStrongBuyVolume() + GetStrongSellVolume() + GetUnknownVolume()) {
		TRACE(_T("%14Id %s����%d\n"), FormatToDateTime(m_TransactionTime), GetSymbol().GetBuffer(), GetVolume());
		TRACE(_T("%d %d %d %d %d %d %d\n"), GetOrdinaryBuyVolume(), GetOrdinarySellVolume(), GetAttackBuyVolume(),
			GetAttackSellVolume(), GetStrongBuyVolume(), GetStrongSellVolume(), GetUnknownVolume());
		return false;
	}
	else return true;
}

bool CChinaStock::CalculatingWeekLine(long lStartDate) {
	ASSERT(IsDayLineLoaded());
	ASSERT(m_DayLine.GetDataSize() > 0);
	long i = 0;
	CWeekLinePtr pWeekLine = nullptr;

	m_WeekLine.Unload();
	while ((i < m_DayLine.GetDataSize()) && (m_DayLine.GetData(i)->GetFormatedMarketDate() < lStartDate)) {
		i++;
	}
	if (i < m_DayLine.GetDataSize()) {
		do {
			pWeekLine = m_DayLine.CreateNewWeekLine(i);
			m_WeekLine.StoreData(pWeekLine);
		} while (i < m_DayLine.GetDataSize());
		m_WeekLine.SetDataLoaded(true);
		return true;
	}
	else return false;
}

#ifdef _DEBUG
void CChinaStock::AssertValid() const {
	CObject::AssertValid();
}

void CChinaStock::Dump(CDumpContext& dc) const {
	CObject::Dump(dc);
}

#endif //_DEBUG