#include"pch.h"
#include"globedef.h"

#include "FinnhubCryptoSymbol.h"

int CFinnhubCryptoSymbol::s_iRatio = 1000;

CFinnhubCryptoSymbol::CFinnhubCryptoSymbol() : CVirtualStock() {
	Reset();
}

void CFinnhubCryptoSymbol::Reset(void) {
	CVirtualStock::Reset();
	m_strDescription = _T(" ");
	m_strDisplaySymbol = _T(" ");
	m_strExchangeCode = _T(" ");
	m_strSymbol = _T(" ");
	m_lDayLineStartDate = 29900101;
	m_lDayLineEndDate = 19800101;
}

void CFinnhubCryptoSymbol::Load(CSetFinnhubCryptoSymbol& setCryptoSymbol) {
	m_strDescription = setCryptoSymbol.m_Description;
	m_strDisplaySymbol = setCryptoSymbol.m_DisplaySymbol;
	m_strExchangeCode = setCryptoSymbol.m_Exchange;
	m_strSymbol = setCryptoSymbol.m_Symbol;
	m_lDayLineStartDate = setCryptoSymbol.m_DayLineStartDate;
	m_lDayLineEndDate = setCryptoSymbol.m_DayLineEndDate;
	m_lIPOStatus = setCryptoSymbol.m_IPOStatus;
}

void CFinnhubCryptoSymbol::Append(CSetFinnhubCryptoSymbol& setCryptoSymbol) {
	setCryptoSymbol.AddNew();
	Save(setCryptoSymbol);
	setCryptoSymbol.Update();
}

void CFinnhubCryptoSymbol::Update(CSetFinnhubCryptoSymbol& setCryptoSymbol) {
	setCryptoSymbol.Edit();
	Save(setCryptoSymbol);
	setCryptoSymbol.Update();
}

void CFinnhubCryptoSymbol::Save(CSetFinnhubCryptoSymbol& setCryptoSymbol) {
	setCryptoSymbol.m_Description = m_strDescription;
	setCryptoSymbol.m_DisplaySymbol = m_strDisplaySymbol;
	setCryptoSymbol.m_Exchange = m_strExchangeCode;
	setCryptoSymbol.m_Symbol = m_strSymbol;
	setCryptoSymbol.m_DayLineStartDate = m_lDayLineStartDate;
	setCryptoSymbol.m_DayLineEndDate = m_lDayLineEndDate;
	setCryptoSymbol.m_IPOStatus = m_lIPOStatus;
}

bool CFinnhubCryptoSymbol::UpdateDayLineDB(void) {
	SaveDayLine();
	UpdateDayLineStartEndDate();
	SetUpdateProfileDB(true);

	return true;
}

bool CFinnhubCryptoSymbol::SaveDayLine() {
	CSetCryptoDayLine setCryptoDayLine;
	size_t lSize = 0;
	vector<CDayLinePtr> vDayLine;
	CDayLinePtr pDayLine = nullptr;
	long lCurrentPos = 0, lSizeOfOldDayLine = 0;
	bool fNeedUpdate = false;

	ASSERT(m_vDayLine.size() > 0);

	lSize = m_vDayLine.size();
	setCryptoDayLine.m_strFilter = _T("[Symbol] = '");
	setCryptoDayLine.m_strFilter += m_strSymbol + _T("'");
	setCryptoDayLine.m_strSort = _T("[Date]");

	setCryptoDayLine.Open();
	while (!setCryptoDayLine.IsEOF()) {
		pDayLine = make_shared<CDayLine>();
		pDayLine->LoadCryptoDayLine(&setCryptoDayLine);
		vDayLine.push_back(pDayLine);
		lSizeOfOldDayLine++;
		setCryptoDayLine.MoveNext();
	}
	setCryptoDayLine.Close();
	if (lSizeOfOldDayLine > 0) {
		if (vDayLine.at(0)->GetFormatedMarketDate() < m_lDayLineStartDate) {
			m_lDayLineStartDate = vDayLine.at(0)->GetFormatedMarketDate();
		}
	}

	lCurrentPos = 0;
	setCryptoDayLine.m_strFilter = _T("[ID] = 1");
	setCryptoDayLine.Open();
	setCryptoDayLine.m_pDatabase->BeginTrans();
	for (int i = 0; i < lSize; i++) { // ����������洢�ģ���Ҫ��ͷ����ʼ�洢
		pDayLine = m_vDayLine.at(i);
		while ((lCurrentPos < lSizeOfOldDayLine) && (vDayLine.at(lCurrentPos)->GetFormatedMarketDate() < pDayLine->GetFormatedMarketDate())) lCurrentPos++;
		if (lCurrentPos < lSizeOfOldDayLine) {
			if (vDayLine.at(lCurrentPos)->GetFormatedMarketDate() > pDayLine->GetFormatedMarketDate()) {
				pDayLine->AppendCryptoDayLine(&setCryptoDayLine);
				fNeedUpdate = true;
			}
		}
		else {
			pDayLine->AppendCryptoDayLine(&setCryptoDayLine);
			fNeedUpdate = true;
		}
	}
	setCryptoDayLine.m_pDatabase->CommitTrans();
	setCryptoDayLine.Close();

	return fNeedUpdate;
}

void CFinnhubCryptoSymbol::SetCheckingDayLineStatus(void) {
	ASSERT(IsDayLineNeedUpdate()); // Ĭ��״̬Ϊ����������Ҫ����
	if (m_lIPOStatus == __STOCK_NULL__) {
		SetDayLineNeedUpdate(false);
	}
	// ���ٸ����������ݱ��ϸ�������Ҫ�µĹ�Ʊ���������еĹ�Ʊ����ѯһ�飬�Է�ֹ�����¹�Ʊ�����ϵĹ�Ʊ���»�Ծ������
	else if (gl_pWorldMarket->GetLastTradeDate() <= GetDayLineEndDate()) { // ������������Ϊ���ջ�����һ�������յ����ݡ�
		SetDayLineNeedUpdate(false); // �������ݲ���Ҫ����
	}
}

CString CFinnhubCryptoSymbol::GetFinnhubDayLineInquiryString(time_t tCurrentTime) {
	CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
	CString strTemp;
	char buffer[50];
	time_t tStartTime = 0;

	strMiddle += m_strSymbol;
	strMiddle += _T("&resolution=D");
	strMiddle += _T("&from=");
	tStartTime = FormatToTTime(m_lDayLineEndDate);
	if (tStartTime < (tCurrentTime - (time_t)(365) * 24 * 3600)) {// ����˻�ֻ�ܶ�ȡһ�����ڵ��������ݡ�
		tStartTime = (tCurrentTime - (time_t)(365) * 24 * 3600);
	}
	sprintf_s(buffer, _T("%I64i"), (INT64)tStartTime);
	strTemp = buffer;
	strMiddle += strTemp;
	strMiddle += _T("&to=");
	sprintf_s(buffer, _T("%I64i"), tCurrentTime);
	strTemp = buffer;
	strMiddle += strTemp;

	return strMiddle;
}

void CFinnhubCryptoSymbol::UpdateDayLine(vector<CDayLinePtr>& vDayLine) {
	m_vDayLine.resize(0);
	for (auto& pDayLine : vDayLine) {
		m_vDayLine.push_back(pDayLine);
	}
}

void CFinnhubCryptoSymbol::UpdateDayLineStartEndDate(void) {
	if (m_vDayLine.size() == 0) {
		SetDayLineStartDate(29900101);
		SetDayLineEndDate(19800101);
	}
	else {
		if (m_vDayLine.at(0)->GetFormatedMarketDate() < GetDayLineStartDate()) {
			SetDayLineStartDate(m_vDayLine.at(0)->GetFormatedMarketDate());
			m_fUpdateProfileDB = true;
		}
		if (m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDate() > m_lDayLineEndDate) {
			SetDayLineEndDate(m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDate());
			m_fUpdateProfileDB = true;
		}
	}
}

bool CFinnhubCryptoSymbol::HaveNewDayLineData(void) {
	if (m_vDayLine.size() <= 0) return false;
	if (m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDate() > m_lDayLineEndDate) return true;
	else return false;
}