#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include "ProductFinnhubMarketHoliday.h"

CProductFinnhubMarketHoliday::CProductFinnhubMarketHoliday() {
	m_strClassName = _T("Finnhub company symbols");
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/stock/symbol?exchange=");
	m_lIndex = -1;
}

CString CProductFinnhubMarketHoliday::CreateMessage() {
	ASSERT(std::strcmp(typeid(*m_pMarket).name(), _T("class CWorldMarket")) == 0);

	const auto strParam = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStockExchangeCode(m_lIndex);

	m_strInquiringExchange = strParam;
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

bool CProductFinnhubMarketHoliday::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*m_pMarket).name(), _T("class CWorldMarket")) == 0);

	const auto strExchangeCode = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStockExchangeCode(m_lIndex);
	const auto pvStock = ParseFinnhubMarketHoliday(pWebData);
	const auto pExchange = gl_pWorldMarket->GetStockExchange(m_lIndex);
	pExchange->SetMarketHolidayUpdated(true);
	// ���Ͻ��������롣
	for (const auto& pStock3 : *pvStock) {
		pStock3->SetExchangeCode(strExchangeCode);
	}
	//���Ϸ��ԣ�ֻ��������Ʊ����������Ͻ�������׺��
	if (!pvStock->empty()) {
		const auto pStock = pvStock->at(0);
		if (IsNeedAddExchangeCode(pStock->GetSymbol(), strExchangeCode) && (strExchangeCode.CompareNoCase(_T("US")) == 0)) {
			gl_systemMessage.PushErrorMessage(_T("��Ʊ�����ʽ������") + pStock->GetSymbol() + _T("  ") + strExchangeCode);
		}
	}
	for (const auto& pStock2 : *pvStock) {
		if (!dynamic_cast<CWorldMarket*>(m_pMarket)->IsStock(pStock2)) {
			pStock2->SetTodayNewStock(true);
			pStock2->SetUpdateProfileDB(true);
			dynamic_cast<CWorldMarket*>(m_pMarket)->AddStock(pStock2);
			const auto str = _T("Finnhub�����´���:") + pStock2->GetSymbol();
			gl_systemMessage.PushInnerSystemInformationMessage(str);
		}
	}
	char buffer[30];
	sprintf_s(buffer, _T("%lld"), pvStock->size());
	CString str = _T("���������г���Ʊ����Ϊ��");
	str += buffer;
	//gl_systemMessage.PushInnerSystemInformationMessage(str);

	return true;
}

bool CProductFinnhubMarketHoliday::IsNeedAddExchangeCode(const CString& strMarketHoliday, const CString& strExchangeCode) {
	const int iLength = strExchangeCode.GetLength();
	const int iSymbolLength = strMarketHoliday.GetLength();
	const CString strRight = strMarketHoliday.Right(iLength);
	if ((strRight.CompareNoCase(strExchangeCode) == 0) && (strMarketHoliday.GetAt(iSymbolLength - iLength - 1) == '.')) {
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ַ��https://finnhub.io/docs/api/stock-symbols
//
// {
// "currency": "EUR",
// "description": "ASN Groenprojectenfonds",
// "displaySymbol" : "NL0012314660.AS",
// "figi" : "",
// "isin" : null,
// "mic" : "XAMS",
// "shareClassFIGI" : "",
// "symbol" : "NL0012314660.AS",
// "symbol2" : "",
// "type" : ""
// }
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
CWorldStockVectorPtr CProductFinnhubMarketHoliday::ParseFinnhubMarketHoliday(const CWebDataPtr& pWebData) {
	auto pvStock = make_shared<vector<CWorldStockPtr>>();
	CWorldStockPtr pStock = nullptr;
	string s, sError;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvStock;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = VOID_DATA_;
		return pvStock;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return pvStock;
	}
	auto pjs = pWebData->GetJSon();
	try {
		for (auto it = pjs->begin(); it != pjs->end(); ++it) {
			pStock = make_shared<CWorldStock>();
			s = jsonGetString(it, _T("currency"));
			if (!s.empty()) pStock->SetCurrency(s.c_str());
			s = jsonGetString(it, _T("description"));
			if (!s.empty()) pStock->SetDescription(s.c_str());
			s = jsonGetString(it, _T("displaySymbol"));
			pStock->SetDisplaySymbol(s.c_str());
			s = jsonGetString(it, _T("figi"));
			if (!s.empty()) pStock->SetFigi(s.c_str());
			s = jsonGetString(it, _T("isin"));
			if (!s.empty()) pStock->SetIsin(s.c_str());
			s = jsonGetString(it, _T("mic"));
			if (!s.empty()) pStock->SetMic(s.c_str());
			s = jsonGetString(it, _T("shareClassFIGI"));
			if (!s.empty()) pStock->SetShareClassFIGI(s.c_str());
			s = jsonGetString(it, _T("symbol"));
			pStock->SetSymbol(s.c_str());
			s = jsonGetString(it, _T("symbol2"));
			pStock->SetSymbol2(s.c_str());
			s = jsonGetString(it, _T("type"));
			if (!s.empty()) pStock->SetType(s.c_str());
			pvStock->push_back(pStock);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub market holiday "), e.what());
		return pvStock;
	}
	return pvStock;
}