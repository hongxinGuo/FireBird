#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"FinnhubStock.h"
#include"WorldMarket.h"

#include "ProductTiingoStockDayLine.h"

#include "TimeConvert.h"

CString GetTiingoDayLineInquiryParam(const CString& strSymbol, long lStartDate, long lCurrentDate) {
	CString strParam = _T("");
	char buffer[50];
	const long year = lCurrentDate / 10000;
	const long month = lCurrentDate / 100 - year * 100;
	const long date = lCurrentDate - year * 10000 - month * 100;

	const long year2 = lStartDate / 10000;
	const long month2 = lStartDate / 100 - year2 * 100;
	const long date2 = lStartDate - year2 * 10000 - month2 * 100;

	strParam += strSymbol;
	strParam += _T("/prices?&startDate=");
	sprintf_s(buffer, _T("%4d-%d-%d"), year2, month2, date2);
	CString strTemp = buffer;
	strParam += strTemp;
	strParam += _T("&endDate=");
	sprintf_s(buffer, _T("%4d-%d-%d"), year, month, date);
	strTemp = buffer;
	strParam += strTemp;

	return strParam;
}

CProductTiingoStockDayLine::CProductTiingoStockDayLine() {
	m_strInquiryFunction = _T("https://api.tiingo.com/tiingo/daily/");
}

///////////////////////////////////////////////////////////////////////////////////////////
///
/// ��ʹ������˻���tiingo����Ҳ�ܹ��ṩ30�����ϵ����ݣ��ʶ�����ȫ�����ݡ�
///
///////////////////////////////////////////////////////////////////////////////////////////
CString CProductTiingoStockDayLine::CreateMessage() {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = gl_dataContainerTiingoStock.GetStock(GetIndex());
	long lStartDate = 19800101;
	if (pStock->GetDayLineEndDate() > 19800101) lStartDate = pStock->GetDayLineEndDate();
	CString strParam = GetTiingoDayLineInquiryParam(pStock->GetSymbol(), lStartDate, GetMarket()->GetMarketDate()); // ������ߴ�δ�����ʱ�������������ߡ�
	m_strInquiringSymbol = pStock->GetSymbol();

	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

void CProductTiingoStockDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(m_lIndex);

	const CDayLinesPtr pvDayLine = ParseTiingoStockDayLine(pWebData);
	if (!pvDayLine->empty()) {
		long lastClose = 0;
		for (const auto& pDayLine2 : *pvDayLine) {
			pDayLine2->SetExchange(pTiingoStock->GetExchangeCode());
			pDayLine2->SetStockSymbol(pTiingoStock->GetSymbol());
			pDayLine2->SetDisplaySymbol(pTiingoStock->GetDisplaySymbol());
			if ((lastClose != 0) && (pDayLine2->GetLastClose() == 0)) pDayLine2->SetLastClose(lastClose);
			lastClose = pDayLine2->GetClose();
		}
		pTiingoStock->UpdateDayLine(*pvDayLine);
		pTiingoStock->SetUpdateDayLineDB(true);
	}
	// ���tiingo stock�����߸��±�ʶ
	pTiingoStock->SetUpdateDayLine(false);
	pTiingoStock->SetDayLineUpdateDate(GetMarket()->GetMarketDate());
	pTiingoStock->SetUpdateProfileDB(true);
	gl_systemConfiguration.DecreaseTiingoBandWidth(pWebData->GetBufferLength());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo���߸�ʽ��
// [{
//	"date":"2019-01-02T00:00:00.000Z",
//		"close" : 157.92,
//		"high" : 158.85,
//		"low" : 154.23,
//		"open" : 154.89,
//		"volume" : 37039737,
//		"adjClose" : 157.92,
//		"adjHigh" : 158.85,
//		"adjLow" : 154.23,
//		"adjOpen" : 154.89,
//		"adjVolume" : 37039737,
//		"divCash" : 0.0,
//		"splitFactor" : 1.0
// },
//	{
//		"date":"2019-01-03T00:00:00.000Z",
//		"close" : 142.19,
//		"high" : 145.72,
//		"low" : 142.0,
//		"open" : 143.98,
//		"volume" : 91312195,
//		"adjClose" : 142.19,
//		"adjHigh" : 145.72,
//		"adjLow" : 142.0,
//		"adjOpen" : 143.98,
//		"adjVolume" : 91312195,
//		"divCash" : 0.0,
//		"splitFactor" : 1.0
//	}
// ]
//
// ���û�й�Ʊ600600.SS�������ݣ��򷵻أ�{"detail":"Error:Ticker '600600.SS' not found"}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDayLinesPtr CProductTiingoStockDayLine::ParseTiingoStockDayLine(const CWebDataPtr& pWebData) {
	auto pvDayLine = make_shared<vector<CDayLinePtr>>();
	string s;
	long year, month, day;
	json js;

	if (!pWebData->CreateJson(js)) return pvDayLine;
	if (!IsValidData(pWebData)) return pvDayLine;

	try {
		s = js.at(_T("detail")); // �Ƿ��б�����Ϣ
		CString strMessage = _T("Tiingo stock dayLine ");
		strMessage += s.c_str();
		gl_systemMessage.PushErrorMessage(strMessage); // ���������Ϣ
		return pvDayLine;
	} catch (json::exception&) {
		// ��ȷ�� do nothing������ִ��
	}
	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			auto pDayLine = make_shared<CDayLine>();
			//pDayLine->SetExchange(_T("US")); // ���е�Tiingo֤ȯ��Ϊ�����г���
			s = jsonGetString(it, _T("date"));
			CString str = s.c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			long lTemp = XferYearMonthDayToYYYYMMDD(year, month, day);
			pDayLine->SetDate(lTemp);
			double dTemp = jsonGetDouble(it,_T("close"));
			pDayLine->SetClose(dTemp * 1000);
			dTemp = jsonGetDouble(it,_T("high"));
			pDayLine->SetHigh(dTemp * 1000);
			dTemp = jsonGetDouble(it, _T("low"));
			pDayLine->SetLow(dTemp * 1000);
			dTemp = jsonGetDouble(it, _T("open"));
			pDayLine->SetOpen(dTemp * 1000);
			lTemp = jsonGetLong(it, _T("volume"));
			pDayLine->SetVolume(lTemp);
			pvDayLine->push_back(pDayLine);
		}
	} catch (json::exception& e) {
		CString str3 = pWebData->GetDataBuffer().c_str();
		str3 = str3.Left(120);
		ReportJSonErrorToSystemMessage(_T("Tiingo Stock DayLine ") + str3, e.what());
		return pvDayLine; // ���ݽ�������Ļ����������
	}
	std::ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate); // ����������˳�����С�

	return pvDayLine;
}
