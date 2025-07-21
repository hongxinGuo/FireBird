#include"pch.h"

#include"ConvertToString.h"
#include"VirtualHistoryCandleBasic.h"

CVirtualHistoryCandleBasic::CVirtualHistoryCandleBasic() {
}

void CVirtualHistoryCandleBasic::CalculateRSLogarithm1(double dRS) {
	const double dLog50 = log10(50.0);
	const double dLog100 = log10(100.0);
	const double dLog = dLog100 - dLog50;
	if ((dRS - 50) > 0) {
		m_dRSLogarithm = 50 + (log10(dRS) - dLog50) * 50 / dLog;
	}
	else if ((dRS - 50) < 0) {
		m_dRSLogarithm = 50 - (log10(100 - dRS) - dLog50) * 50 / dLog;
	}
	else m_dRSLogarithm = 50;
}

bool CVirtualHistoryCandleBasic::IsActive() const {
	if (GetClose() != 0) {
		return true;
	}
	return false;
}

void CVirtualHistoryCandleBasic::SetAverage(const int length, const long lValue) noexcept {
	switch (length) {
	case 5:
		m_l5Average = lValue;
		break;
	case 10:
		m_l10Average = lValue;
		break;
	case 20:
		m_l20Average = lValue;
		break;
	case 30:
		m_l30Average = lValue;
		break;
	case 60:
		m_l60Average = lValue;
		break;
	case 120:
		m_l120Average = lValue;
		break;
	case 200:
		m_l200Average = lValue;
		break;
	case 240:
		m_l240Average = lValue;
		break;
	default: // ����״̬ʱ����MA��
		m_lAverage = lValue;
		break;
	}
}

long CVirtualHistoryCandleBasic::GetAverage(const int length) const noexcept {
	switch (length) {
	case 5:
		return m_l5Average;
	case 10:
		return m_l10Average;
	case 20:
		return m_l20Average;
	case 30:
		return m_l30Average;
	case 60:
		return m_l60Average;
	case 120:
		return m_l120Average;
	case 200:
		return m_l200Average;
	case 240:
		return m_l240Average;
	default: // ����״̬
		return m_lAverage;
	}
}

void CVirtualHistoryCandleBasic::SaveBasicData(CVirtualSetHistoryCandleBasic* pVirtualSetHistoryCandleBasic) const {
	ASSERT(pVirtualSetHistoryCandleBasic->IsOpen());
	auto ratio = GetRatio();

	pVirtualSetHistoryCandleBasic->m_Date = GetDate();
	pVirtualSetHistoryCandleBasic->m_Exchange = GetExchange().c_str();
	pVirtualSetHistoryCandleBasic->m_Symbol = GetStockSymbol().c_str();
	pVirtualSetHistoryCandleBasic->m_DisplaySymbol = GetDisplaySymbol().c_str();
	pVirtualSetHistoryCandleBasic->m_LastClose = ConvertValueToCString(GetLastClose(), ratio);
	pVirtualSetHistoryCandleBasic->m_High = ConvertValueToCString(GetHigh(), ratio);
	pVirtualSetHistoryCandleBasic->m_Low = ConvertValueToCString(GetLow(), ratio);
	pVirtualSetHistoryCandleBasic->m_Open = ConvertValueToCString(GetOpen(), ratio);
	pVirtualSetHistoryCandleBasic->m_Close = ConvertValueToCString(GetClose(), ratio);
	pVirtualSetHistoryCandleBasic->m_dividend = ConvertValueToCString(GetDividend());
	pVirtualSetHistoryCandleBasic->m_splitFactor = ConvertValueToCString(GetSplitFactor());
	pVirtualSetHistoryCandleBasic->m_Volume = ConvertValueToCString(GetVolume());
	pVirtualSetHistoryCandleBasic->m_Amount = ConvertValueToCString(GetAmount());
	pVirtualSetHistoryCandleBasic->m_UpAndDown = ConvertValueToCString(GetUpDown());
	pVirtualSetHistoryCandleBasic->m_UpDownRate = ConvertValueToCString(GetUpDownRate());
	pVirtualSetHistoryCandleBasic->m_ChangeHandRate = ConvertValueToCString(GetChangeHandRate());
	pVirtualSetHistoryCandleBasic->m_TotalValue = ConvertValueToCString(GetTotalValue());
	pVirtualSetHistoryCandleBasic->m_CurrentValue = ConvertValueToCString(GetCurrentValue());
	pVirtualSetHistoryCandleBasic->m_RS = ConvertValueToCString(GetRS());
	pVirtualSetHistoryCandleBasic->m_RSIndex = ConvertValueToCString(GetRSIndex());
	pVirtualSetHistoryCandleBasic->m_RSBackup = ConvertValueToCString(GetRSBackup());
}

void CVirtualHistoryCandleBasic::AppendBasicData(CVirtualSetHistoryCandleBasic* pVirtualSetHistoryCandleBasic) const {
	ASSERT(pVirtualSetHistoryCandleBasic->IsOpen());
	pVirtualSetHistoryCandleBasic->AddNew();
	SaveBasicData(pVirtualSetHistoryCandleBasic);
	pVirtualSetHistoryCandleBasic->Update();
}

void CVirtualHistoryCandleBasic::LoadBasicData(const CVirtualSetHistoryCandleBasic* pVirtualSetHistoryCandleBasic) {
	ASSERT(pVirtualSetHistoryCandleBasic->IsOpen());
	auto ratio = GetRatio();

	m_lDate = pVirtualSetHistoryCandleBasic->m_Date;
	m_strExchange = pVirtualSetHistoryCandleBasic->m_Exchange;
	m_strStockSymbol = pVirtualSetHistoryCandleBasic->m_Symbol;
	CString str;
	str = pVirtualSetHistoryCandleBasic->m_DisplaySymbol;
	m_strDisplaySymbol = str.GetString();
	m_lLastClose = atof(pVirtualSetHistoryCandleBasic->m_LastClose) * ratio;
	m_lOpen = atof(pVirtualSetHistoryCandleBasic->m_Open) * ratio;
	m_lHigh = atof(pVirtualSetHistoryCandleBasic->m_High) * ratio;
	m_lLow = atof(pVirtualSetHistoryCandleBasic->m_Low) * ratio;
	m_lClose = atof(pVirtualSetHistoryCandleBasic->m_Close) * ratio;
	m_dSplitFactor = atof(pVirtualSetHistoryCandleBasic->m_splitFactor);
	m_dDividend = atof(pVirtualSetHistoryCandleBasic->m_dividend);
	m_dUpDown = atof(pVirtualSetHistoryCandleBasic->m_UpAndDown);
	m_llVolume = atoll(pVirtualSetHistoryCandleBasic->m_Volume);
	m_llAmount = atoll(pVirtualSetHistoryCandleBasic->m_Amount);
	m_dUpDownRate = atof(pVirtualSetHistoryCandleBasic->m_UpDownRate);
	m_dChangeHandRate = atof(pVirtualSetHistoryCandleBasic->m_ChangeHandRate);
	m_llTotalValue = atoll(pVirtualSetHistoryCandleBasic->m_TotalValue);
	m_llCurrentValue = atoll(pVirtualSetHistoryCandleBasic->m_CurrentValue);
	m_dRS = atof(pVirtualSetHistoryCandleBasic->m_RS);
	m_dRSIndex = atof(pVirtualSetHistoryCandleBasic->m_RSIndex);
	m_dRSBackup = atof(pVirtualSetHistoryCandleBasic->m_RSBackup);

	CalculateRSLogarithm1(m_dRS);
}
