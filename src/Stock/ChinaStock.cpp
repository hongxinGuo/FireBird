#include"pch.h"

#include"TimeConvert.h"
#include"ChinaStockCodeConverter.h"

#include "ChinaStock.h"
#include"ChinaMarket.h"

#include<gsl/gsl>
using namespace gsl;

////////////////////////////////////////////////////////////////////////////////////////////
//
// �ж�strStockCode�Ƿ�Ϊ����A������Ĺ�Ʊ���롣
// ����A�ɴ�����600��601��ͷ������A�ɴ�����000��001��ͷ��
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool IsShareA(const CString& strStockCode) {
	const CString strSymbol = GetStockSymbol(strStockCode);
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
	return false;
}

CChinaStock::CChinaStock() {
	for (int i = 0; i < 5; i++) {
		m_lPBuy.at(i) = m_lPSell.at(i) = 0;
		m_lVBuy.at(i) = m_lVSell.at(i) = 0;
	}

	m_vVolume.resize(240);
	m_vOrdinaryBuy.resize(240);
	m_vOrdinarySell.resize(240);
	m_vAttackBuy.resize(240);
	m_vAttackSell.resize(240);
	m_vStrongBuy.resize(240);
	m_vStrongSell.resize(240);
	m_vCancelBuy.resize(240);
	m_vCancelSell.resize(240);

	m_nHand = 100;

	m_lHighLimitFromTengxun = m_lLowLimitFromTengxun = 0;
	m_lHighLimit = m_lLowLimit = 0;
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

	for (int i = 0; i < 240; i++) {
		m_vVolume.at(i) = 0;
		m_vOrdinaryBuy.at(i) = 0;
		m_vOrdinarySell.at(i) = 0;
		m_vAttackBuy.at(i) = 0;
		m_vAttackSell.at(i) = 0;
		m_vStrongBuy.at(i) = 0;
		m_vStrongSell.at(i) = 0;
		m_vCancelBuy.at(i) = 0;
		m_vCancelSell.at(i) = 0;
	}

	m_fChosen = false;
	m_fUpdateChosenStockDB = false;

	m_fDayLineDBUpdated = false;

	m_fHaveFirstRTData = false; // ʵʱ���ݿ�ʼ�����ʶ����һ��ʵʱ����ֻ��������ʼ��ϵͳ���������ڼ��㡣�ӵڶ������ݿ�ʼ�������Ч��
	m_fNeedProcessRTData = true;
	m_fRTDataCalculated = false;
	m_pLastRTData = nullptr;

	ClearRTDataDeque();
}

void CChinaStock::ClearRTDataDeque() {
	const auto lTotalNumber = GetRTDataQueueSize();
	for (size_t i = 0; i < lTotalNumber; i++) {
		CWebRTDataPtr pRTData = PopRTData();
	}
}

bool CChinaStock::HaveNewDayLineData() {
	if (m_dataDayLine.Size() <= 0) return false;
	if (m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetMarketDate() > GetDayLineEndDate()) return true;
	return false;
}

void CChinaStock::UpdateStatusByDownloadedDayLine() {
	if (m_dataDayLine.Empty()) return;
	if (IsEarlyThen(m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetMarketDate(), gl_pChinaMarket->GetMarketDate(), 30)) {
		// ��ȡ���Ĺ�Ʊ���������������������ϸ��µ���������գ������˻������������һ���µ�ʱ��۲죩��
		SetIPOStatus(_STOCK_DELISTED_); // �����л���ͣ���ס�
	}
	else {
		SetIPOStatus(_STOCK_IPOED_); // �������׹�Ʊ
	}
}

void CChinaStock::ReportDayLineDownLoaded() {
	//CString strTemp = GetSymbol();
	//strTemp += _T("�����������.");
	//gl_systemMessage.PushDayLineInfoMessage(strTemp);
}

void CChinaStock::AppendTodayBasicInfo(CSetDayLineBasicInfo* pSetDayLineBasicInfo) const {
	const auto pDayLine = make_shared<CDayLine>();

	ASSERT(pSetDayLineBasicInfo->IsOpen());

	UpdateCurrentHistoryCandle(pDayLine);
	pDayLine->AppendBasicData(pSetDayLineBasicInfo);
}

void CChinaStock::SaveTempInfo(CSetDayLineTodaySaved* pSetDayLineTemp) const {
	const auto pDayLine = make_shared<CDayLine>();

	ASSERT(pSetDayLineTemp->IsOpen());

	UpdateCurrentHistoryCandle(pDayLine);
	pDayLine->Save(pSetDayLineTemp);
}

void CChinaStock::UpdateCurrentHistoryCandle(const CVirtualHistoryCandleExtendPtr& pBeUpdated) const {
	pBeUpdated->SetDate(ConvertToDate(m_TransactionTime, gl_pChinaMarket->GetMarketTimeZone()));
	pBeUpdated->SetExchange(m_strExchangeCode);
	pBeUpdated->SetStockSymbol(m_strSymbol);
	pBeUpdated->SetDisplaySymbol(m_strDisplaySymbol);
	pBeUpdated->SetLastClose(m_lLastClose);
	pBeUpdated->SetOpen(m_lOpen);
	pBeUpdated->SetHigh(m_lHigh);
	pBeUpdated->SetLow(m_lLow);
	pBeUpdated->SetClose(m_lNew);
	pBeUpdated->SetVolume(m_llVolume);
	pBeUpdated->SetAmount(m_llAmount);
	pBeUpdated->SetUpDown(static_cast<double>(m_lUpDown) / GetRatio());
	pBeUpdated->SetUpDownRate(m_dUpDownRate);
	if (m_llTotalValue != 0) {
		pBeUpdated->SetChangeHandRate(static_cast<double>(100) * m_llAmount / m_llTotalValue);
	}
	else {
		pBeUpdated->SetChangeHandRate(0.0);
	}
	pBeUpdated->SetTotalValue(m_llTotalValue);
	pBeUpdated->SetCurrentValue(m_llCurrentValue);

	pBeUpdated->SetTransactionNumber(m_lTransactionNumber);
	pBeUpdated->SetTransactionNumberBelow5000(m_lTransactionNumberBelow5000);
	pBeUpdated->SetTransactionNumberBelow50000(m_lTransactionNumberBelow50000);
	pBeUpdated->SetTransactionNumberBelow200000(m_lTransactionNumberBelow200000);
	pBeUpdated->SetTransactionNumberAbove200000(m_lTransactionNumberAbove200000);

	pBeUpdated->SetCanceledBuyVolume(m_lCanceledBuyVolume);
	pBeUpdated->SetCanceledSellVolume(m_lCanceledSellVolume);
	pBeUpdated->SetAttackBuyVolume(m_lAttackBuyVolume);
	pBeUpdated->SetAttackSellVolume(m_lAttackSellVolume);
	pBeUpdated->SetStrongBuyVolume(m_lStrongBuyVolume);
	pBeUpdated->SetStrongSellVolume(m_lStrongSellVolume);
	pBeUpdated->SetUnknownVolume(m_lUnknownVolume);
	pBeUpdated->SetOrdinaryBuyVolume(m_lOrdinaryBuyVolume);
	pBeUpdated->SetOrdinarySellVolume(m_lOrdinarySellVolume);
	pBeUpdated->SetAttackBuyBelow50000(m_lAttackBuyBelow50000);
	pBeUpdated->SetAttackBuyBelow200000(m_lAttackBuyBelow200000);
	pBeUpdated->SetAttackBuyAbove200000(m_lAttackBuyAbove200000);
	pBeUpdated->SetAttackSellBelow50000(m_lAttackSellBelow50000);
	pBeUpdated->SetAttackSellBelow200000(m_lAttackSellBelow200000);
	pBeUpdated->SetAttackSellAbove200000(m_lAttackSellAbove200000);

	pBeUpdated->SetOrdinaryBuyVolumeBelow5000(m_lOrdinaryBuyVolumeBelow5000);
	pBeUpdated->SetOrdinaryBuyVolumeBelow10000(m_lOrdinaryBuyVolumeBelow10000);
	pBeUpdated->SetOrdinaryBuyVolumeBelow20000(m_lOrdinaryBuyVolumeBelow20000);
	pBeUpdated->SetOrdinaryBuyVolumeBelow50000(m_lOrdinaryBuyVolumeBelow50000);
	pBeUpdated->SetOrdinaryBuyVolumeBelow100000(m_lOrdinaryBuyVolumeBelow100000);
	pBeUpdated->SetOrdinaryBuyVolumeBelow200000(m_lOrdinaryBuyVolumeBelow200000);
	pBeUpdated->SetOrdinaryBuyVolumeAbove200000(m_lOrdinaryBuyVolumeAbove200000);
	pBeUpdated->SetOrdinarySellVolumeBelow5000(m_lOrdinarySellVolumeBelow5000);
	pBeUpdated->SetOrdinarySellVolumeBelow10000(m_lOrdinarySellVolumeBelow10000);
	pBeUpdated->SetOrdinarySellVolumeBelow20000(m_lOrdinarySellVolumeBelow20000);
	pBeUpdated->SetOrdinarySellVolumeBelow50000(m_lOrdinarySellVolumeBelow50000);
	pBeUpdated->SetOrdinarySellVolumeBelow100000(m_lOrdinarySellVolumeBelow100000);
	pBeUpdated->SetOrdinarySellVolumeBelow200000(m_lOrdinarySellVolumeBelow200000);
	pBeUpdated->SetOrdinarySellVolumeAbove200000(m_lOrdinarySellVolumeAbove200000);
	pBeUpdated->SetOrdinaryBuyNumberBelow5000(m_lOrdinaryBuyNumberBelow5000);
	pBeUpdated->SetOrdinaryBuyNumberBelow10000(m_lOrdinaryBuyNumberBelow10000);
	pBeUpdated->SetOrdinaryBuyNumberBelow20000(m_lOrdinaryBuyNumberBelow20000);
	pBeUpdated->SetOrdinaryBuyNumberBelow50000(m_lOrdinaryBuyNumberBelow50000);
	pBeUpdated->SetOrdinaryBuyNumberBelow100000(m_lOrdinaryBuyNumberBelow100000);
	pBeUpdated->SetOrdinaryBuyNumberBelow200000(m_lOrdinaryBuyNumberBelow200000);
	pBeUpdated->SetOrdinaryBuyNumberAbove200000(m_lOrdinaryBuyNumberAbove200000);
	pBeUpdated->SetOrdinarySellNumberBelow5000(m_lOrdinarySellNumberBelow5000);
	pBeUpdated->SetOrdinarySellNumberBelow10000(m_lOrdinarySellNumberBelow10000);
	pBeUpdated->SetOrdinarySellNumberBelow20000(m_lOrdinarySellNumberBelow20000);
	pBeUpdated->SetOrdinarySellNumberBelow50000(m_lOrdinarySellNumberBelow50000);
	pBeUpdated->SetOrdinarySellNumberBelow100000(m_lOrdinarySellNumberBelow100000);
	pBeUpdated->SetOrdinarySellNumberBelow200000(m_lOrdinarySellNumberBelow200000);
	pBeUpdated->SetOrdinarySellNumberAbove200000(m_lOrdinarySellNumberAbove200000);

	pBeUpdated->SetCanceledBuyVolumeBelow5000(m_lCanceledBuyVolumeBelow5000);
	pBeUpdated->SetCanceledBuyVolumeBelow10000(m_lCanceledBuyVolumeBelow10000);
	pBeUpdated->SetCanceledBuyVolumeBelow20000(m_lCanceledBuyVolumeBelow20000);
	pBeUpdated->SetCanceledBuyVolumeBelow50000(m_lCanceledBuyVolumeBelow50000);
	pBeUpdated->SetCanceledBuyVolumeBelow100000(m_lCanceledBuyVolumeBelow100000);
	pBeUpdated->SetCanceledBuyVolumeBelow200000(m_lCanceledBuyVolumeBelow200000);
	pBeUpdated->SetCanceledBuyVolumeAbove200000(m_lCanceledBuyVolumeAbove200000);
	pBeUpdated->SetCanceledSellVolumeBelow5000(m_lCanceledSellVolumeBelow5000);
	pBeUpdated->SetCanceledSellVolumeBelow10000(m_lCanceledSellVolumeBelow10000);
	pBeUpdated->SetCanceledSellVolumeBelow20000(m_lCanceledSellVolumeBelow20000);
	pBeUpdated->SetCanceledSellVolumeBelow50000(m_lCanceledSellVolumeBelow50000);
	pBeUpdated->SetCanceledSellVolumeBelow100000(m_lCanceledSellVolumeBelow100000);
	pBeUpdated->SetCanceledSellVolumeBelow200000(m_lCanceledSellVolumeBelow200000);
	pBeUpdated->SetCanceledSellVolumeAbove200000(m_lCanceledSellVolumeAbove200000);
}

void CChinaStock::UpdateRTData(const CWebRTDataPtr& pRTData) {
	SetTransactionTime(pRTData->GetTransactionTime());
	SetLastClose(pRTData->GetLastClose());
	SetNew(pRTData->GetNew());
	SetHigh(pRTData->GetHigh());
	SetLow(pRTData->GetLow());
	SetOpen(pRTData->GetOpen());
	SetVolume(pRTData->GetVolume());
	SetAmount(pRTData->GetAmount());
	SetUpDown(m_lNew - m_lLastClose);
	if (m_lLastClose > 0) SetUpDownRate(static_cast<double>(m_lUpDown) * 100 / m_lLastClose);
	else SetUpDownRate(0);
	// ����ʵʱ������û������ֵ����ͨ��ֵ������ģ��ʶ���Ҫ�ж�һ���Ƿ�Ϊ��
	if (pRTData->GetTotalValue() > 0) SetTotalValue(pRTData->GetTotalValue());
	if (pRTData->GetCurrentValue() > 0) SetCurrentValue(pRTData->GetCurrentValue());
	if (m_llTotalValue != 0) m_dChangeHandRate = static_cast<double>(m_llAmount) * 100 / m_llTotalValue;
	else m_dChangeHandRate = 0;
	for (int i = 0; i < 5; i++) {
		SetPBuy(i, pRTData->GetPBuy(i));
		SetVBuy(i, pRTData->GetVBuy(i));
		SetPSell(i, pRTData->GetPSell(i));
		SetVSell(i, pRTData->GetVSell(i));
	}
}

void CChinaStock::UpdateStatus(const CWebRTDataPtr& pRTData) {
	SetActive(true);
	SetSymbol(pRTData->GetSymbol()); // ����ȫ�ֹ�Ʊ����Ϣ����ʱRTData��ȫ���޷��������еĹ�Ʊ��Ϣ��
	if (pRTData->GetStockName() != _T("")) SetDisplaySymbol(pRTData->GetStockName()); // ����ȫ�ֹ�Ʊ����Ϣ����ʱRTData��ȫ���޷��������еĹ�Ʊ��Ϣ��
	SetIPOStatus(_STOCK_IPOED_);
}

void CChinaStock::UpdateDayLineStartEndDate() {
	bool fUpdated = false;

	if (m_dataDayLine.Size() > 0) {
		if ((GetDayLineStartDate() == 19900101) || (m_dataDayLine.GetData(0)->GetMarketDate() < GetDayLineStartDate())) {
			SetDayLineStartDate(m_dataDayLine.GetData(0)->GetMarketDate());
			SetDayLineDBUpdated(true);
			fUpdated = true;
		}
		if (m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetMarketDate() > GetDayLineEndDate()) {
			SetDayLineEndDate(m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetMarketDate());
			SetDayLineDBUpdated(true);
			fUpdated = true;
		}
	}
	if (fUpdated) {
		SetUpdateProfileDB(true);
	}
}

void CChinaStock::AppendTodayExtendInfo(CSetDayLineExtendInfo* pSetDayLineExtendInfo) const {
	const auto pDayLine = make_shared<CDayLine>();

	ASSERT(pSetDayLineExtendInfo->IsOpen());
	UpdateCurrentHistoryCandle(pDayLine);
	pDayLine->AppendExtendData(pSetDayLineExtendInfo);
}

////////////////////////////////////////////////////////////////////////////
//
// ֻ��һ�����ñ�������������ϵͳ��ʼ��ʱװ��֮ǰ���������Ϣ����������˵Ļ���.
// ��Ҫͬʱ�����ܳɽ����������ݴ�˹��������ڼ���δ������ɽ����� �ܳɽ��������µ�ʵʱ��������ʱ��ͬ�����£��ʶ��޷����ڼ��㣩
//
////////////////////////////////////////////////////////////////////////////
void CChinaStock::LoadTodaySavedInfo(const CSetDayLineTodaySaved* pSetDayLineTemp) {
	m_lUnknownVolume = atoll(pSetDayLineTemp->m_UnknownVolume);

	m_lTransactionNumber = atol(pSetDayLineTemp->m_TransactionNumber);
	m_lTransactionNumberBelow5000 = atol(pSetDayLineTemp->m_TransactionNumberBelow5000);
	m_lTransactionNumberBelow50000 = atol(pSetDayLineTemp->m_TransactionNumberBelow50000);
	m_lTransactionNumberBelow200000 = atol(pSetDayLineTemp->m_TransactionNumberBelow200000);
	m_lTransactionNumberAbove200000 = atol(pSetDayLineTemp->m_TransactionNumberAbove200000);
	m_lCanceledBuyVolume = atoll(pSetDayLineTemp->m_CanceledBuyVolume);
	m_lCanceledSellVolume = atoll(pSetDayLineTemp->m_CanceledSellVolume);
	m_lAttackBuyVolume = atoll(pSetDayLineTemp->m_AttackBuyVolume);
	m_lAttackSellVolume = atoll(pSetDayLineTemp->m_AttackSellVolume);
	m_lStrongBuyVolume = atoll(pSetDayLineTemp->m_StrongBuyVolume);
	m_lStrongSellVolume = atoll(pSetDayLineTemp->m_StrongSellVolume);
	m_lOrdinaryBuyVolume = atoll(pSetDayLineTemp->m_OrdinaryBuyVolume);
	m_lOrdinarySellVolume = atoll(pSetDayLineTemp->m_OrdinarySellVolume);
	m_lAttackBuyBelow50000 = atoll(pSetDayLineTemp->m_AttackBuyBelow50000);
	m_lAttackBuyBelow200000 = atoll(pSetDayLineTemp->m_AttackBuyBelow200000);
	m_lAttackBuyAbove200000 = atoll(pSetDayLineTemp->m_AttackBuyAbove200000);
	m_lAttackSellBelow50000 = atoll(pSetDayLineTemp->m_AttackSellBelow50000);
	m_lAttackSellBelow200000 = atoll(pSetDayLineTemp->m_AttackSellBelow200000);
	m_lAttackSellAbove200000 = atoll(pSetDayLineTemp->m_AttackSellAbove200000);
	m_llVolume = atoll(pSetDayLineTemp->m_Volume);
	m_llLastSavedVolume = atoll(pSetDayLineTemp->m_Volume);

	m_lOrdinaryBuyVolumeBelow5000 = atoll(pSetDayLineTemp->m_OrdinaryBuyVolumeBelow5000);
	m_lOrdinaryBuyVolumeBelow10000 = atoll(pSetDayLineTemp->m_OrdinaryBuyVolumeBelow10000);
	m_lOrdinaryBuyVolumeBelow20000 = atoll(pSetDayLineTemp->m_OrdinaryBuyVolumeBelow20000);
	m_lOrdinaryBuyVolumeBelow50000 = atoll(pSetDayLineTemp->m_OrdinaryBuyVolumeBelow50000);
	m_lOrdinaryBuyVolumeBelow100000 = atoll(pSetDayLineTemp->m_OrdinaryBuyVolumeBelow100000);
	m_lOrdinaryBuyVolumeBelow200000 = atoll(pSetDayLineTemp->m_OrdinaryBuyVolumeBelow200000);
	m_lOrdinaryBuyVolumeAbove200000 = atoll(pSetDayLineTemp->m_OrdinaryBuyVolumeAbove200000);
	m_lOrdinarySellVolumeBelow5000 = atoll(pSetDayLineTemp->m_OrdinarySellVolumeBelow5000);
	m_lOrdinarySellVolumeBelow10000 = atoll(pSetDayLineTemp->m_OrdinarySellVolumeBelow10000);
	m_lOrdinarySellVolumeBelow20000 = atoll(pSetDayLineTemp->m_OrdinarySellVolumeBelow20000);
	m_lOrdinarySellVolumeBelow50000 = atoll(pSetDayLineTemp->m_OrdinarySellVolumeBelow50000);
	m_lOrdinarySellVolumeBelow100000 = atoll(pSetDayLineTemp->m_OrdinarySellVolumeBelow100000);
	m_lOrdinarySellVolumeBelow200000 = atoll(pSetDayLineTemp->m_OrdinarySellVolumeBelow200000);
	m_lOrdinarySellVolumeAbove200000 = atoll(pSetDayLineTemp->m_OrdinarySellVolumeAbove200000);
	m_lOrdinaryBuyNumberBelow5000 = atoll(pSetDayLineTemp->m_OrdinaryBuyNumberBelow5000);
	m_lOrdinaryBuyNumberBelow10000 = atoll(pSetDayLineTemp->m_OrdinaryBuyNumberBelow10000);
	m_lOrdinaryBuyNumberBelow20000 = atoll(pSetDayLineTemp->m_OrdinaryBuyNumberBelow20000);
	m_lOrdinaryBuyNumberBelow50000 = atoll(pSetDayLineTemp->m_OrdinaryBuyNumberBelow50000);
	m_lOrdinaryBuyNumberBelow100000 = atoll(pSetDayLineTemp->m_OrdinaryBuyNumberBelow100000);
	m_lOrdinaryBuyNumberBelow200000 = atoll(pSetDayLineTemp->m_OrdinaryBuyNumberBelow200000);
	m_lOrdinaryBuyNumberAbove200000 = atoll(pSetDayLineTemp->m_OrdinaryBuyNumberAbove200000);
	m_lOrdinarySellNumberBelow5000 = atoll(pSetDayLineTemp->m_OrdinarySellNumberBelow5000);
	m_lOrdinarySellNumberBelow10000 = atoll(pSetDayLineTemp->m_OrdinarySellNumberBelow10000);
	m_lOrdinarySellNumberBelow20000 = atoll(pSetDayLineTemp->m_OrdinarySellNumberBelow20000);
	m_lOrdinarySellNumberBelow50000 = atoll(pSetDayLineTemp->m_OrdinarySellNumberBelow50000);
	m_lOrdinarySellNumberBelow100000 = atoll(pSetDayLineTemp->m_OrdinarySellNumberBelow100000);
	m_lOrdinarySellNumberBelow200000 = atoll(pSetDayLineTemp->m_OrdinarySellNumberBelow200000);
	m_lOrdinarySellNumberAbove200000 = atoll(pSetDayLineTemp->m_OrdinarySellNumberAbove200000);

	m_lCanceledBuyVolumeBelow5000 = atoll(pSetDayLineTemp->m_CanceledBuyVolumeBelow5000);
	m_lCanceledBuyVolumeBelow10000 = atoll(pSetDayLineTemp->m_CanceledBuyVolumeBelow10000);
	m_lCanceledBuyVolumeBelow20000 = atoll(pSetDayLineTemp->m_CanceledBuyVolumeBelow20000);
	m_lCanceledBuyVolumeBelow50000 = atoll(pSetDayLineTemp->m_CanceledBuyVolumeBelow50000);
	m_lCanceledBuyVolumeBelow100000 = atoll(pSetDayLineTemp->m_CanceledBuyVolumeBelow100000);
	m_lCanceledBuyVolumeBelow200000 = atoll(pSetDayLineTemp->m_CanceledBuyVolumeBelow200000);
	m_lCanceledBuyVolumeAbove200000 = atoll(pSetDayLineTemp->m_CanceledBuyVolumeAbove200000);
	m_lCanceledSellVolumeBelow5000 = atoll(pSetDayLineTemp->m_CanceledSellVolumeBelow5000);
	m_lCanceledSellVolumeBelow10000 = atoll(pSetDayLineTemp->m_CanceledSellVolumeBelow10000);
	m_lCanceledSellVolumeBelow20000 = atoll(pSetDayLineTemp->m_CanceledSellVolumeBelow20000);
	m_lCanceledSellVolumeBelow50000 = atoll(pSetDayLineTemp->m_CanceledSellVolumeBelow50000);
	m_lCanceledSellVolumeBelow100000 = atoll(pSetDayLineTemp->m_CanceledSellVolumeBelow100000);
	m_lCanceledSellVolumeBelow200000 = atoll(pSetDayLineTemp->m_CanceledSellVolumeBelow200000);
	m_lCanceledSellVolumeAbove200000 = atoll(pSetDayLineTemp->m_CanceledSellVolumeAbove200000);
}

bool CChinaStock::Calculate10RSStrong2StockSet() {
	CSetDayLineBasicInfo setDayLineBasicInfo;

	ASSERT(m_dataDayLine.IsDataLoaded());
	const size_t iDayLineSize = GetDayLineSize();
	if (iDayLineSize > 155) {
		int iCountFirst = 0;
		vector<double> m_v10DaysRS;
		m_v10DaysRS.resize(iDayLineSize);
		CalculateDayLineRSIndex();
		Get10DaysRS(m_v10DaysRS);
		size_t i;
		for (i = iDayLineSize - 1; i > iDayLineSize - 5; i--) {
			if (m_v10DaysRS.at(i) > 55) iCountFirst++;
			if (iCountFirst >= 3) break;
		}
		if (iCountFirst >= 3) {
			int iCountSecond = 0;
			while (m_v10DaysRS.at(i) > 53) i--;
			for (auto j = i; j > i - 50; j--) { if (m_v10DaysRS.at(j) > 55) iCountSecond++; }
			if (iCountSecond >= 3) { return true; }
		}
	}

	return false;
}

bool CChinaStock::Calculate10RSStrong1StockSet() {
	CSetDayLineBasicInfo setDayLineBasicInfo;
	vector<double> m_v10DaysRS;
	int iCountFirst = 0, iCountSecond = 0, iCountThird = 0;

	ASSERT(m_dataDayLine.IsDataLoaded());
	const size_t iDayLineSize = GetDayLineSize();

	if (iDayLineSize < 350) return false;
	m_v10DaysRS.resize(iDayLineSize);
	CalculateDayLineRSIndex();
	Get10DaysRS(m_v10DaysRS);
	size_t i, j;

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
	for (auto k = j; k > j - 130; k--) {
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
	double dStrong1, dStrong2, dStrong3, dStrong4;
	bool fFindHigh1, fFindHigh2, fFindHigh3, fFindHigh4;

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

	ASSERT(m_dataDayLine.IsDataLoaded());
	const size_t iDayLineSize = GetDayLineSize();
	if ((iDayLineSize < (pRef->m_lDayLength[0] + pRef->m_lDayLength[1] + pRef->m_lDayLength[2] + 10))
		|| (iDayLineSize < pRef->m_lDayLength[3] + 10))
		return false;

	m_v10DaysRS.resize(iDayLineSize);
	CalculateDayLineRSIndex();
	Get10DaysRS(m_v10DaysRS);
	size_t i = 0;
	size_t j = 0;

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
		for (auto k = j; k > (j - pRef->m_lDayLength[2]); k--) {
			if (m_v10DaysRS.at(k) > dStrong3) iCountThird++;
			if (iCountThird >= pRef->m_lStrongDayLength[2]) {
				fFind3 = true;
				break;
			}
		}
	}
	else {
		for (auto k = j; k > (j - pRef->m_lDayLength[2]); k--) {
			if (m_v10DaysRS.at(k) < dStrong3) iCountThird++;
			if (iCountThird >= pRef->m_lStrongDayLength[2]) {
				fFind3 = true;
				break;
			}
		}
	}
	if (!fFind3) return false;

	if (pRef->m_lDayLength[3] == 0) return true; // DayLength4Ϊ��Ļ�������ͨ��ѡ��
	if (fFindHigh4) {
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
void CChinaStock::ProcessRTData() {
	const INT64 lTotalNumber = GetRTDataQueueSize(); //  ������еĳ��ȡ�����ͬ�����ƻ�ȡ����ֵ.
	// ����Ϊ����ҵ��仯����Ʊ��Ծ�ȡ����������
	for (INT64 i = 0; i < lTotalNumber; i++) {
		const CWebRTDataPtr pRTData = PopRTData(); // ����ͬ�����ƻ�ȡ����
		if (pRTData->IsActive()) {// ������Ч
			UpdateRTData(pRTData); // ���¹�Ʊ��ʱ״̬��
			if (gl_pChinaMarket->IsMarketOpened() && IsNeedProcessRTData()) {// ����ʱ���ڼ�����������ָ�����Ʊ������㽻������͹ҵ��仯
				ProcessOneRTData(pRTData);
				CheckCurrentRTData();
				m_fRTDataCalculated = true;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ���ҵ���
// ����ǵ�һ�μ��㣬��ֻ���ó�ʼ״̬��
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::ProcessOneRTData(const CWebRTDataPtr& pRTData) {
	CalculateHighLowLimit(pRTData);
	if (HaveFirstRTData()) {// �����ʼ���㣨�ڶ����յ�ʵʱ���ݼ��Ժ�
		CalculateOneRTData(pRTData);
		UpdateVolumeVector();
	}
	else {// ��һ���յ�ʵʱ���ݣ���ֻ��ʼ��ϵͳ��������
		InitializeCalculatingRTDataEnvironment(pRTData);
	}
	return true;
}

void CChinaStock::UpdateVolumeVector() {
	int index = gl_pChinaMarket->XferMarketTimeToIndex();
	ASSERT(index >= 0 && index < 240);
	m_vVolume.at(index) = m_llVolume;
	m_vOrdinaryBuy.at(index) = m_lOrdinaryBuyVolume;
	m_vOrdinarySell.at(index) = m_lOrdinarySellVolume;
	m_vAttackBuy.at(index) = m_lAttackBuyVolume;
	m_vAttackSell.at(index) = m_lAttackSellVolume;
	m_vStrongBuy.at(index) = m_lStrongBuyVolume;
	m_vStrongSell.at(index) = m_lStrongSellVolume;
	m_vCancelBuy.at(index) = m_lCanceledBuyVolume;
	m_vCancelSell.at(index) = m_lCanceledSellVolume;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// �й����е��ǵ�ͣ��۸���㷽��Ŀǰ��֪��ֻ�ܿ�������
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CChinaStock::CalculateHighLowLimit(const CWebRTDataPtr& pRTData) {
	double d1, d2;
	int iAdjust = 0;
	int iCompare;
	int i2;

	if (pRTData->GetPSell(0) == 0) {// ��һ�۸�Ϊ�㡣��ʱΪ��ͣ��
		if (pRTData->GetPBuy(0) > 0) {
			m_lHighLimit = pRTData->GetPBuy(0);
			if (pRTData->GetLastClose() < 1200) {	// �ͼ۹ɣ�
				m_lLowLimit = pRTData->GetLastClose() - (m_lHighLimit - pRTData->GetLastClose());
			}
			else {
				i2 = pRTData->GetPBuy(0) - pRTData->GetLastClose();
				iCompare = (static_cast<double>(i2) * 100 + pRTData->GetLastClose() * 0.65) / pRTData->GetLastClose(); // ϵ��0.70��ʵ������ģ�Ŀǰ��ͨ�á�
				if (iCompare <= 21) {
					if ((iCompare % 5) != 0) {// ȷ���ǵ���Ϊ5%�ı���
						TRACE("%s iCompare = %i, ����5�ı���\n", m_strSymbol.GetBuffer(), iCompare);
					}
					d1 = static_cast<double>(i2) * 100 / pRTData->GetLastClose();
					if (d1 > iCompare) {
						d2 = static_cast<double>(i2 - 10) * 100 / pRTData->GetLastClose();
						if ((iCompare - d2) <= (d1 - iCompare)) {// �������ͣ��ʱ���������һ��ʱ��9.7777��10.22222�������ý�С�ģ�9.7777����
							iAdjust = 10;
						}
					}
					m_lLowLimit = pRTData->GetLastClose() - i2 + iAdjust;
				}
				else { m_lLowLimit = m_lLowLimitFromTengxun; }
			}
			if (m_lHighLimitFromTengxun == 0) m_lHighLimitFromTengxun = m_lHighLimit;
			if (m_lLowLimitFromTengxun == 0) m_lLowLimitFromTengxun = m_lLowLimit;
		}
		else {// ��һ��һͬʱΪ�㡣��ʱΪû�н���
			m_lHighLimit = m_lHighLimitFromTengxun;
			m_lLowLimit = m_lLowLimitFromTengxun;
		}
	}
	else if (pRTData->GetPBuy(0) == 0) {// ��һ�۸�Ϊ�㣬��ʱΪ��ͣ��
		if (pRTData->GetPSell(0) > 0) {
			m_lLowLimit = pRTData->GetPSell(0);
			if (pRTData->GetLastClose() < 1200) {	// �ͼ۹ɣ�
				m_lHighLimit = pRTData->GetLastClose() + (pRTData->GetLastClose() - m_lLowLimit);
			}
			else {
				i2 = pRTData->GetLastClose() - pRTData->GetPSell(0);
				iCompare = (static_cast<double>(i2) * 100 + pRTData->GetLastClose() * 0.65) / pRTData->GetLastClose(); // ϵ��0.70��ʵ������ģ�Ŀǰ��ͨ�á�
				if (iCompare <= 21) {
					if ((iCompare % 5) != 0) {// ȷ���ǵ���Ϊ5%�ı���
						TRACE("%s iCompare = %i, ����5�ı���\n", m_strSymbol.GetBuffer(), iCompare);
					}
					d1 = static_cast<double>(i2) * 100 / pRTData->GetLastClose();
					if (d1 < iCompare) {
						d2 = static_cast<double>(i2 + 10) * 100 / pRTData->GetLastClose();
						if ((d2 - iCompare) <= (iCompare - d1)) {	// ��������ͣ��ʱ���������һ��ʱ��9.7777��10.22222�������ýϴ�ģ�10.2222����
							iAdjust = 10;
						}
					}
					m_lHighLimit = pRTData->GetLastClose() + i2 + iAdjust;
				}
				else { m_lHighLimit = m_lHighLimitFromTengxun; }
			}
			if (m_lHighLimitFromTengxun == 0) m_lHighLimitFromTengxun = m_lHighLimit;
			if (m_lLowLimitFromTengxun == 0) m_lLowLimitFromTengxun = m_lLowLimit;
		}
		else {// ��һ��һͬʱΪ�㡣��ʱΪû�н���
			ASSERT(0); // �����ܳ��ִ������
			m_lHighLimit = m_lHighLimitFromTengxun;
			m_lLowLimit = m_lLowLimitFromTengxun;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//
// ��һ���յ�ʵʱ����ʱ��ֻ��ʼ��ϵͳ�������㣨��Ϊû�г�ʼ���ݣ�
//
////////////////////////////////////////////////////////////////////////////////////////
void CChinaStock::InitializeCalculatingRTDataEnvironment(const CWebRTDataPtr& pRTData) {
	SetLastRTData(pRTData);
	SetHavingFirstRTData(true);
	// ��һ�ιҵ����޷��ж�����״̬���ʶ�������Ϊ�޷��жϡ����֮ǰ�Ѿ����й�ϵͳ���˴��ǿ�����;��¼�ģ���ϵͳ�洢����ʱ���������ݿ��У�
	// ��ϵͳ����ʱ�Ѿ�������ʱ״̬�����ˣ��ʶ�m_lUnknownVolume��Ϊ�㣬��Ҫ���¼���m_lUnknownVolume.
	// m_lUnknownVolume = ��ǰ�ĳɽ��� - ֮ǰ��ʱ�洢�ĳɽ��� + ֮ǰ�洢��m_lUnknownVolume.
	SetVolume(pRTData->GetVolume());
	SetUnknownVolume(GetUnknownVolume() + m_pLastRTData->GetVolume() - m_llLastSavedVolume);

	// ���õ�һ�εĹҵ�ӳ�䡣
	for (int j = 0; j < 5; j++) {
		SetGuadan(pRTData->GetPBuy(j), pRTData->GetVBuy(j));
		SetGuadan(pRTData->GetPSell(j), pRTData->GetVSell(j));
	}
}

void CChinaStock::CalculateOneRTData(const CWebRTDataPtr& pRTData) {
	long lCurrentGuadanTransactionPrice;

	ResetCalculatingData();
	m_lCurrentGuadanTransactionVolume = pRTData->GetVolume() - m_pLastRTData->GetVolume();
	if (m_lCurrentGuadanTransactionVolume == 0) {// �޳ɽ���
		// ���ҵ����
		lCurrentGuadanTransactionPrice = 0;
		m_nCurrentTransactionType = NO_TRANSACTION_;
	}
	else {
		lCurrentGuadanTransactionPrice = (pRTData->GetAmount() - m_pLastRTData->GetAmount()) * 1000 / m_lCurrentGuadanTransactionVolume; // ���ɱȽ��õļ۸񣨷Ŵ�һǧ������ó����ͣ�
		if ((lCurrentGuadanTransactionPrice == m_lHighLimitFromTengxun) || (lCurrentGuadanTransactionPrice == m_lLowLimitFromTengxun)) {// �ǵ�ͣ��ʱ���ɽ�������δ֪�ɽ�����
			IncreaseTransactionNumber();
			m_nCurrentTransactionType = UNKNOWN_BUYSELL_;
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

void CChinaStock::CalculateOneDeal(const CWebRTDataPtr& pRTData, INT64 lCurrentGuadanTransactionPrice) {
	IncreaseTransactionNumber();
	ASSERT(lCurrentGuadanTransactionPrice == ((pRTData->GetAmount() - m_pLastRTData->GetAmount()) * 1000 / m_lCurrentGuadanTransactionVolume)); // ���ɱȽ��õļ۸񣨷Ŵ�һǧ������ó����ͣ�
	m_dCurrentGuadanTransactionPrice = static_cast<double>(lCurrentGuadanTransactionPrice) / 1000; // �任��ʵ�ʼ۸�
	if (lCurrentGuadanTransactionPrice >= m_pLastRTData->GetPBuy(0)) {// ����ǰ����1
		if (lCurrentGuadanTransactionPrice <= m_pLastRTData->GetPSell(0)) {	// ����ǰ����1
			CalculateOrdinaryBuySell(lCurrentGuadanTransactionPrice);
		}
		else if (lCurrentGuadanTransactionPrice < m_pLastRTData->GetPSell(1)) {	// ��������һ���������̶������������롣AttackBuy
			CalculateAttackBuy();
		}
		else {// �������̶���ǿ�����롣StrongBuy��
			CalculateStrongBuy();
		}
	} // ����ǰ��һ
	else if (lCurrentGuadanTransactionPrice > m_pLastRTData->GetPBuy(1)) {// ��������һ���������̶���������������AttackSell
		CalculateAttackSell();
	}
	else {// �������̶���ǿ��������StrongSell
		CalculateStrongSell();
	}
	ASSERT(GetTransactionTime() >= pRTData->GetTransactionTime());
	const INT64 I = pRTData->GetVolume();
	const INT64 j = GetOrdinaryBuyVolume() + GetOrdinarySellVolume()
	+ GetAttackBuyVolume() + GetAttackSellVolume()
	+ GetStrongBuyVolume() + GetStrongSellVolume() + GetUnknownVolume();
	ASSERT(I == j);
}

void CChinaStock::IncreaseTransactionNumber() {
	m_lTransactionNumber++; // �ɽ�����һ��
	if (m_lCurrentGuadanTransactionVolume < 5000) { m_lTransactionNumberBelow5000++; }
	else if (m_lCurrentGuadanTransactionVolume < 50000) { m_lTransactionNumberBelow50000++; }
	else if (m_lCurrentGuadanTransactionVolume < 200000) { m_lTransactionNumberBelow200000++; }
	else { m_lTransactionNumberAbove200000++; }
	ASSERT(GetTransactionNumber() == (GetTransactionNumberAbove200000()
		+ GetTransactionNumberBelow200000()
		+ GetTransactionNumberBelow50000() + GetTransactionNumberBelow5000()));
}

void CChinaStock::CalculateOrdinaryBuySell(INT64 lCurrentGuadanTransactionPrice) {
	if ((m_pLastRTData->GetPSell(0) - lCurrentGuadanTransactionPrice) <= 2) {//һ��������
		m_lOrdinaryBuyVolume += m_lCurrentGuadanTransactionVolume;
		m_nCurrentTransactionType = ORDINARY_BUY_;
		CalculateOrdinaryBuyVolume();
	}
	else if ((lCurrentGuadanTransactionPrice - m_pLastRTData->GetPBuy(0)) <= 2) {// һ��������
		m_nCurrentTransactionType = ORDINARY_SELL_;
		m_lOrdinarySellVolume += m_lCurrentGuadanTransactionVolume;
		CalculateOrdinarySellVolume();
	}
	else {// �������ӣ���������
		m_nCurrentTransactionType = UNKNOWN_BUYSELL_;
		m_lUnknownVolume += m_lCurrentGuadanTransactionVolume;
	}
}

void CChinaStock::CalculateOrdinaryBuyVolume() {
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

void CChinaStock::CalculateOrdinarySellVolume() {
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

void CChinaStock::CalculateAttackBuy() {
	m_nCurrentTransactionType = ATTACK_BUY_;
	m_lAttackBuyVolume += m_lCurrentGuadanTransactionVolume;
	CalculateAttackBuyVolume();
}

void CChinaStock::CalculateStrongBuy() {
	m_nCurrentTransactionType = STRONG_BUY_;
	m_lStrongBuyVolume += m_lCurrentGuadanTransactionVolume;
	CalculateAttackBuyVolume();
}

void CChinaStock::CalculateAttackBuyVolume() {
	if (m_lCurrentGuadanTransactionVolume < 50000) { m_lAttackBuyBelow50000 += m_lCurrentGuadanTransactionVolume; }
	else if (m_lCurrentGuadanTransactionVolume < 200000) { m_lAttackBuyBelow200000 += m_lCurrentGuadanTransactionVolume; }
	else { m_lAttackBuyAbove200000 += m_lCurrentGuadanTransactionVolume; }
}

void CChinaStock::CalculateAttackSell() {
	m_nCurrentTransactionType = ATTACK_SELL_;
	m_lAttackSellVolume += m_lCurrentGuadanTransactionVolume;
	CalculateAttackSellVolume();
}

void CChinaStock::CalculateStrongSell() {
	m_nCurrentTransactionType = STRONG_SELL_;
	m_lStrongSellVolume += m_lCurrentGuadanTransactionVolume;
	CalculateAttackSellVolume();
}

void CChinaStock::CalculateAttackSellVolume() {
	if (m_lCurrentGuadanTransactionVolume < 50000) { m_lAttackSellBelow50000 += m_lCurrentGuadanTransactionVolume; }
	else if (m_lCurrentGuadanTransactionVolume < 200000) { m_lAttackSellBelow200000 += m_lCurrentGuadanTransactionVolume; }
	else { m_lAttackSellAbove200000 += m_lCurrentGuadanTransactionVolume; }
}

void CChinaStock::ResetCalculatingData() {
	m_lCurrentCanceledBuyVolume = 0;
	m_lCurrentCanceledSellVolume = 0;
	m_lCurrentGuadanTransactionVolume = 0;
	m_lCurrentStrongBuy = 0;
	m_lCurrentStrongSell = 0;
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
bool CChinaStock::AnalysisGuadan(const CWebRTDataPtr& pCurrentRTData, INT64 lCurrentTransactionPrice) {
	// ��Ҫ���Ĺҵ�λ�á�˳��Ϊ������4, ����3, ... ����0, ����0, .... ��3, ��4
	// ������˭��ǰ������ν��������ʱ��Ҫ��ס��˳��
	array<bool, 10> fNeedCheck{ true, true, true, true, true, true, true, true, true, true };

	for (int i = 0; i < 10; i++) fNeedCheck.at(i) = true; // Ԥ��Ϊ��Ҫ���㡣
	SelectGuadanThatNeedToCalculate(pCurrentRTData, lCurrentTransactionPrice, fNeedCheck);
	SetCurrentGuadan(pCurrentRTData);
	// �����ʮ���ҵ���λ�Ϲ����ı仯���.��Ŀǰ�ҵ�״̬��֮ǰ��ʮ����λ��m_pLastRTData�����ģ���ȣ��鿴�仯
	CheckGuadan(pCurrentRTData, fNeedCheck);

	return (true);
}

void CChinaStock::SelectGuadanThatNeedToCalculate(const CWebRTDataPtr& pCurrentRTData, INT64 lCurrentTransactionPrice, array<bool, 10>& fNeedCheck) const {
	// ȷ����Ҫ������Щ�ҵ���һ����ʮ����û���ܵ�����Ӱ��Ķ�Ҫ���㡣
	switch (m_nCurrentTransactionType) {
	case NO_TRANSACTION_: // û�гɽ�������ٵ���������Ӧ��λ�ϵĳ�����
		ASSERT(lCurrentTransactionPrice == 0);
		break;
	case ATTACK_BUY_: // ����һ�Ѿ���ʧ����������Ӱ�졣���������߸��ҵ���
		fNeedCheck.at(3) = false;
		fNeedCheck.at(4) = fNeedCheck.at(5) = false;
		break;
	case STRONG_BUY_: // ����һ����������ʧ������������δ������Ҫ���㡣
		if (lCurrentTransactionPrice < m_pLastRTData->GetPSell(2)) {// ����4������5�д�
			fNeedCheck.at(2) = false;
		}
		else if (lCurrentTransactionPrice < m_pLastRTData->GetPSell(3)) {	// ����5�д�
			fNeedCheck.at(1) = false;
			fNeedCheck.at(2) = false;
		}
		else {// ���е���������Ӱ��
			fNeedCheck.at(0) = false;
			fNeedCheck.at(2) = false;
			fNeedCheck.at(1) = false;
		}
		fNeedCheck.at(3) = false;
		fNeedCheck.at(4) = false;
		fNeedCheck.at(5) = false;
		break;
	case ORDINARY_BUY_: // ����һ��Ӱ�졣���������˸��ҵ���
	case UNKNOWN_BUYSELL_: // ����һ����һ��Ӱ�졣���������˸��ҵ���
	case ORDINARY_SELL_: // ��һ��Ӱ�졣���������˸��ҵ���
		fNeedCheck.at(4) = fNeedCheck.at(5) = false;
		break;
	case ATTACK_SELL_: // ��һ��ʧ���򵥶���Ӱ�졣���������߸��ҵ���
		fNeedCheck.at(4) = fNeedCheck.at(5) = fNeedCheck.at(6) = false;
		break;
	case STRONG_SELL_: // ��һ���򵥶���ʧ����������Ҫ���㡣
		if (m_pLastRTData->GetPBuy(3) > lCurrentTransactionPrice) {// �����򵥶���Ӱ��
			fNeedCheck.at(7) = false;
			fNeedCheck.at(8) = false;
			fNeedCheck.at(9) = false;
		}
		else if (m_pLastRTData->GetPBuy(2) > lCurrentTransactionPrice) {// ��5�д�
			fNeedCheck.at(7) = false;
			fNeedCheck.at(8) = false;
		}
		else {// ��4����5û�仯
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

void CChinaStock::SetCurrentGuadan(const CWebRTDataPtr& pCurrentRTData) {
	// ��λ�������ǳɽ��ˣ�Ҳ�����ǳ����ˣ�Ŀǰ��������Щϸ�ڣ�ͳһ��Ϊ�ǳɽ��ˣ������㳷�������Ժ��ٷ���֮��
	// ����յ�ǰ�ҵ�֮��Ĺҵ�������Ȼ�����ϵ�ǰ��������пյ��Ļ�������Ȼ����ˡ�
	for (int i = pCurrentRTData->GetPBuy(4); i <= pCurrentRTData->GetPSell(4); i += 10) {
		m_mapGuadan[i] = 0; // �˴���Ҫ����ֵ���п���֮ǰΪ�գ��ʶ�����ʹ��at��������������[]�±������
	}
	for (int i = 0; i < 5; i++) {// ��Ŀǰ��ʮ���ҵ�״̬����ӳ���У��ҵ�״̬����Ϊ����̬
		SetGuadan(pCurrentRTData->GetPSell(i), pCurrentRTData->GetVSell(i));
		SetGuadan(pCurrentRTData->GetPBuy(i), pCurrentRTData->GetVBuy(i));
	}
}

void CChinaStock::CheckGuadan(CWebRTDataPtr pCurrentRTData, const array<bool, 10>& fNeedCheck) {
	for (int i = 0; i < 5; i++) {
		CheckSellGuadan(fNeedCheck, i);
		CheckBuyGuadan(fNeedCheck, i);
	}
}

void CChinaStock::CheckSellGuadan(const array<bool, 10>& fNeedCheck, int i) {
	ASSERT((i < 5) && (i >= 0));
	if (fNeedCheck.at(4 - i)) {
		if (GetGuadan(m_pLastRTData->GetPSell(i)) < m_pLastRTData->GetVSell(i)) {	// �����˵Ļ�
			const INT64 iTemp = m_pLastRTData->GetVSell(i) - GetGuadan(m_pLastRTData->GetPSell(i));
			m_lCurrentCanceledSellVolume += iTemp;
			CalculateCanceledSellVolume(iTemp);
			m_lCanceledSellVolume += iTemp;
		}
	}
	ASSERT(m_lCanceledSellVolume == (m_lCanceledSellVolumeAbove200000 + m_lCanceledSellVolumeBelow10000 + m_lCanceledSellVolumeBelow100000
		+ m_lCanceledSellVolumeBelow20000 + m_lCanceledSellVolumeBelow200000 + m_lCanceledSellVolumeBelow5000
		+ m_lCanceledSellVolumeBelow50000));
}

void CChinaStock::CalculateCanceledSellVolume(INT64 lCurrentCanceledSellVolume) {
	if (lCurrentCanceledSellVolume < 5000) { m_lCanceledSellVolumeBelow5000 += lCurrentCanceledSellVolume; }
	else if (lCurrentCanceledSellVolume < 10000) { m_lCanceledSellVolumeBelow10000 += lCurrentCanceledSellVolume; }
	else if (lCurrentCanceledSellVolume < 20000) { m_lCanceledSellVolumeBelow20000 += lCurrentCanceledSellVolume; }
	else if (lCurrentCanceledSellVolume < 50000) { m_lCanceledSellVolumeBelow50000 += lCurrentCanceledSellVolume; }
	else if (lCurrentCanceledSellVolume < 100000) { m_lCanceledSellVolumeBelow100000 += lCurrentCanceledSellVolume; }
	else if (lCurrentCanceledSellVolume < 200000) { m_lCanceledSellVolumeBelow200000 += lCurrentCanceledSellVolume; }
	else { m_lCanceledSellVolumeAbove200000 += lCurrentCanceledSellVolume; }
}

void CChinaStock::CheckBuyGuadan(const array<bool, 10>& fNeedCheck, int i) {
	ASSERT((i < 5) && (i >= 0));
	if (fNeedCheck.at(5 + i)) {
		if (GetGuadan(m_pLastRTData->GetPBuy(i)) < m_pLastRTData->GetVBuy(i)) {	// �����˵Ļ�
			const INT64 iTemp = m_pLastRTData->GetVBuy(i) - GetGuadan(m_pLastRTData->GetPBuy(i));
			m_lCurrentCanceledBuyVolume += iTemp;
			CalculateCanceledBuyVolume(iTemp);
			m_lCanceledBuyVolume += iTemp;
		}
	}
	ASSERT(m_lCanceledBuyVolume == (m_lCanceledBuyVolumeAbove200000 + m_lCanceledBuyVolumeBelow10000 + m_lCanceledBuyVolumeBelow100000
		+ m_lCanceledBuyVolumeBelow20000 + m_lCanceledBuyVolumeBelow200000 + m_lCanceledBuyVolumeBelow5000
		+ m_lCanceledBuyVolumeBelow50000));
}

void CChinaStock::CalculateCanceledBuyVolume(INT64 lCurrentCanceledBuyVolume) {
	if (lCurrentCanceledBuyVolume < 5000) { m_lCanceledBuyVolumeBelow5000 += lCurrentCanceledBuyVolume; }
	else if (lCurrentCanceledBuyVolume < 10000) { m_lCanceledBuyVolumeBelow10000 += lCurrentCanceledBuyVolume; }
	else if (lCurrentCanceledBuyVolume < 20000) { m_lCanceledBuyVolumeBelow20000 += lCurrentCanceledBuyVolume; }
	else if (lCurrentCanceledBuyVolume < 50000) { m_lCanceledBuyVolumeBelow50000 += lCurrentCanceledBuyVolume; }
	else if (lCurrentCanceledBuyVolume < 100000) { m_lCanceledBuyVolumeBelow100000 += lCurrentCanceledBuyVolume; }
	else if (lCurrentCanceledBuyVolume < 200000) { m_lCanceledBuyVolumeBelow200000 += lCurrentCanceledBuyVolume; }
	else { m_lCanceledBuyVolumeAbove200000 += lCurrentCanceledBuyVolume; }
}

bool CChinaStock::HaveGuadan(INT64 lPrice) const {
	if (!m_mapGuadan.contains(lPrice)) return false;
	if (m_mapGuadan.at(lPrice) == 0) return false;
	return true;
}

void CChinaStock::CheckCurrentRTData() const {
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
		TRACE(_T("%06d %s Error in volume. Error  code = %d\n"), gl_pChinaMarket->GetMarketTime(), GetSymbol().GetBuffer(), j);
	}
}

bool CChinaStock::LoadStockCodeDB(CSetChinaStockSymbol& setChinaStockSymbol) {
	LoadSymbol(setChinaStockSymbol);

	return true;
}

void CChinaStock::CheckNeedProcessRTData() {
	SetNeedProcessRTData(true);
	if (IsShanghaiExchange(GetSymbol())) {
		if (GetSymbol().Left(6) <= _T("000999")) {//����ָ����
			SetNeedProcessRTData(false);
		}
	}
	else if ((GetSymbol().Left(6) >= _T("399000"))) {// ����ָ��
		SetNeedProcessRTData(false);
	}
}

void CChinaStock::CheckIPOStatus() {
	if (!IsDelisted()) {
		if (IsEarlyThen(GetDayLineEndDate(), gl_pChinaMarket->GetMarketDate(), 30)) {
			SetIPOStatus(_STOCK_DELISTED_);
			SetUpdateProfileDB(true);
		}
	}
}

bool CChinaStock::CheckDayLineStatus() {
	// ���ٸ����������ݱ��ϸ�������Ҫ�µĹ�Ʊ���������еĹ�Ʊ����ѯһ�飬�Է�ֹ�����¹�Ʊ�����ϵĹ�Ʊ���»�Ծ������
	if (gl_pChinaMarket->GetLastTradeDate() <= GetDayLineEndDate()) {// ������������Ϊ���ջ�����һ�������յ����ݡ�
		SetUpdateDayLine(false); // �������ݲ���Ҫ����
	}
	else if (IsDelisted()) {// ���й�Ʊ��������ع��������ݣ���ÿ����һ������������
		if ((gl_pChinaMarket->GetDayOfWeek() != 1) && (GetDayLineEndDate() != _CHINA_MARKET_BEGIN_DATE_)) {
			SetUpdateDayLine(false);
		}
	}
	return m_fUpdateDayLine;
}

bool CChinaStock::BuildWeekLine(long lStartDate) {
	if (IsNullStock()) return true;
	if (!IsDayLineLoaded()) { LoadDayLine(GetSymbol()); }
	if (GetDayLineSize() <= 0) return true;

	if (CalculatingWeekLine(lStartDate)) { SaveWeekLine(); }

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

bool CChinaStock::IsSameStock(const CChinaStockPtr& pStock) const {
	if (pStock == nullptr) return false;
	if (m_strSymbol.Compare(pStock->GetSymbol()) == 0) {
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
//
// �жϹ�Ʊ�����Ƿ���Ч��������߼ۡ���ͼۡ��ɽ����ͳɽ������жϣ������Ϊ�㣬����Ϊ�˹ɽ���û����Ч����
// �����ÿ��̼ۣ����̼��п��ܲ�Ϊ��ʱ���������ݽ�Ϊ�㣨ͣ��ʱ����״̬����
//
////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::IsTodayDataActive() const {
	if (!m_fActive) return false;
	return IsTodayDataChanged();
}

////////////////////////////////////////////////////////////////////////////////
//
// �жϹ�Ʊ�����Ƿ���Ч��������߼ۡ���ͼۡ��ɽ����ͳɽ������жϣ������Ϊ�㣬����Ϊ�˹ɽ���û����Ч����
// �����ÿ��̼ۣ����̼��п��ܲ�Ϊ��ʱ���������ݽ�Ϊ�㣨ͣ��ʱ����״̬����
//
////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::IsTodayDataChanged() const {
	if ((GetHigh() != 0) || (GetLow() != 0) || (GetVolume() != 0) || (GetAmount() != 0)) {
		return true;
	}
	return false;
}

bool CChinaStock::IsVolumeConsistence() noexcept {
	if ((m_lHighLimit > 0) && (m_lLowLimit > 0)) {
		if ((m_lHighLimitFromTengxun != m_lHighLimit) || (m_lLowLimitFromTengxun != m_lLowLimit)) {
			TRACE(_T("%s�ǵ�ͣ��۸񲻷���%d %d    %d  %d\n"), GetSymbol().GetBuffer(), m_lHighLimitFromTengxun, m_lHighLimit, m_lLowLimitFromTengxun, m_lLowLimit);
		}
		if ((m_lPBuy[0] > 0) && (m_lPSell[0] > 0)) {// ���ǵ�ͣ���ʱ
			m_lHighLimit = m_lLowLimit = 0; // ���ô�������
		}
	}
	if (GetVolume() != GetOrdinaryBuyVolume() + GetOrdinarySellVolume() + GetAttackBuyVolume()
		+ GetAttackSellVolume() + GetStrongBuyVolume() + GetStrongSellVolume() + GetUnknownVolume()) {
		TRACE(_T("%14Id %s����%d\n"), ConvertToDateTime(m_TransactionTime, gl_pChinaMarket->GetMarketTimeZone()), GetSymbol().GetBuffer(), GetVolume());
		TRACE(_T("%d %d %d %d %d %d %d\n"), GetOrdinaryBuyVolume(), GetOrdinarySellVolume(), GetAttackBuyVolume(),
		      GetAttackSellVolume(), GetStrongBuyVolume(), GetStrongSellVolume(), GetUnknownVolume());
		return false;
	}
	return true;
}

bool CChinaStock::CalculatingWeekLine(long lStartDate) {
	ASSERT(IsDayLineLoaded());
	ASSERT(m_dataDayLine.Size() > 0);
	long i = 0;
	CWeekLinePtr pWeekLine = nullptr;

	m_dataWeekLine.Unload();
	while ((i < m_dataDayLine.Size()) && (m_dataDayLine.GetData(i)->GetMarketDate() < lStartDate)) {
		i++;
	}
	if (i < m_dataDayLine.Size()) {
		do {
			pWeekLine = m_dataDayLine.CreateNewWeekLine(i);
			m_dataWeekLine.Add(pWeekLine);
		} while (i < m_dataDayLine.Size());
		m_dataWeekLine.SetDataLoaded(true);
		return true;
	}
	return false;
}
