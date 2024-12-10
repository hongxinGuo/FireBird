#include "pch.h"

#include"VirtualDataSource.h"
#include "ProductTiingoForexDayLine.h"
import FireBird.Accessory.JsonParse;
#include"FinnhubStock.h"
#include"WorldMarket.h"
#include"NlohmannJsonHeader.h"
import FireBird.System.Message;
import FireBird.Accessory.TimeConvert;
import FireBird.Accessory.JsonGetValue;

CProductTiingoForexDayLine::CProductTiingoForexDayLine() {
	m_strInquiryFunction = _T("https://api.tiingo.com/tiingo/daily/");
}

///////////////////////////////////////////////////////////////////////////////////////////
///
/// ��ʹ������˻���tiingo����Ҳ�ܹ��ṩ30�����ϵ����ݣ��ʶ���������һ��ȫ�����ݣ���19800101��ʼ����
///	�˺�Ϊ�˼�����������������ֻ����δ���ص����ݡ�
///	Finnhub���������ֻ�ṩһ��ġ���ϵͳ�����ִ��ʱ��Ϊ2019�꣬��finnhubû��2018����ǰ�����ߡ�
///
///////////////////////////////////////////////////////////////////////////////////////////
CString CProductTiingoForexDayLine::CreateMessage() {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);
	/*
	const auto pForex = gl_dataContainerFinnhubForex.GetStock(GetIndex());
	CString strParam;
	if (pStock->GetDayLineStartDate() > 20180101) {
		strParam = pStock->GetTiingoDayLineInquiryParam(19800101, GetMarket()->GetMarketDate()); // �������δ��ȫ�����ʱ�������������ߡ�
	}
	else {
		strParam = pStock->GetTiingoDayLineInquiryParam(pStock->GetDayLineEndDate(), GetMarket()->GetMarketDate());
	}
	pStock->SetUpdateDayLine(false);
	m_strInquiry = m_strInquiryFunction + strParam;
		*/
	return m_strInquiry;
}

void CProductTiingoForexDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_lIndex >= 0);
	/*
	const auto pForex = gl_dataContainerFinnhubForex.GetForex(m_lIndex);
	const CDayLinesPtr pvDayLine = ParseTiingoForexDayLine(pWebData);
	pForex->SetUpdateDayLine(false);
	if (!pvDayLine->empty()) {
		for (const auto& pDayLine2 : *pvDayLine) {
			pDayLine2->SetExchange(pForex->GetExchangeCode());
			pDayLine2->SetForexSymbol(pForex->GetSymbol());
			pDayLine2->SetDisplaySymbol(pForex->GetTicker());
		}
		pForex->UpdateDayLine(*pvDayLine);
		pForex->SetUpdateDayLineDB(true);
		pForex->SetUpdateProfileDB(true);
		//TRACE("����Tiingo %s��������\n", pForex->GetSymbol().GetBuffer());
		return;
	}
	else {
		pForex->SetUpdateDayLineDB(false);
		pForex->SetUpdateProfileDB(false);
		//TRACE("����Tiingo %s��������\n", pForex->GetSymbol().GetBuffer());
	}
	*/
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
CDayLinesPtr CProductTiingoForexDayLine::ParseTiingoForexDayLine(const CWebDataPtr& pWebData) {
	auto pvDayLine = make_shared<vector<CDayLinePtr>>();
	string s;
	long year, month, day;
	nlohmann::ordered_json js;

	if (!pWebData->CreateJson(js)) return pvDayLine;
	if (!IsValidData(pWebData)) return pvDayLine;

	try {
		s = js.at(_T("detail")); // �Ƿ��б�����Ϣ
		CString strMessage = _T("Tiingo stock dayLine ");
		strMessage += s.c_str();
		gl_systemMessage.PushErrorMessage(strMessage); // ���������Ϣ
		return pvDayLine;
	} catch (nlohmann::ordered_json::exception&) {
		// ��ȷ�� do nothing������ִ��
	}
	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			auto pDayLine = make_shared<CDayLine>();
			s = jsonGetString(it, _T("date"));
			CString str = s.c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			long lTemp = XferYearMonthDayToYYYYMMDD(year, month, day);
			pDayLine->SetDate(lTemp);
			double dTemp = jsonGetDouble(it, _T("close"));
			pDayLine->SetClose(dTemp * 1000);
			dTemp = jsonGetDouble(it, _T("high"));
			pDayLine->SetHigh(dTemp * 1000);
			dTemp = jsonGetDouble(it, _T("low"));
			pDayLine->SetLow(dTemp * 1000);
			dTemp = jsonGetDouble(it, _T("open"));
			pDayLine->SetOpen(dTemp * 1000);
			lTemp = jsonGetLong(it, _T("volume"));
			pDayLine->SetVolume(lTemp);
			pvDayLine->push_back(pDayLine);
		}
	} catch (nlohmann::ordered_json::exception& e) {
		CString str3 = pWebData->GetDataBuffer().c_str();
		str3 = str3.Left(120);
		ReportJSonErrorToSystemMessage(_T("Tiingo Forex DayLine ") + str3, e.what());
		return pvDayLine; // ���ݽ�������Ļ����������
	}
	std::ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate); // ����������˳�����С�

	return pvDayLine;
}
