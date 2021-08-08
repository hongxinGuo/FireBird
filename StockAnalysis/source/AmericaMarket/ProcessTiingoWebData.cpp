#include"pch.h"

#include"ctype.h"

#include"globedef.h"
#include"accessory.h"
#include"WorldMarket.h"

using namespace std;
#include <algorithm>
#include<string>
#include<map>
#include<vector>
#include<queue>
#include<atomic>

#include"WebData.h"
#include"EconomicCalendar.h"

using namespace std;
#include<algorithm>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

bool CompareDayLineDate(CDayLinePtr& p1, CDayLinePtr& p2);

bool CWorldMarket::ProcessTiingoStockSymbol(CWebDataPtr pWebData, vector<CTiingoStockPtr>& vTiingoStock) {
	string strNotAvailable{ _T("Field not available for free/evaluation") }; // Tiingo����˻��ж������ݿ�ȱ���᷵�ش���Ϣ��
	CString strNULL = _T(" ");
	CTiingoStockPtr pStock = make_shared<CTiingoStock>();
	ptree pt, pt2;
	string s;
	int iCount = 0;
	CString str, strNumber;
	char buffer[30];
	long year, month, day;
	bool fSucceed = true;

	if (!ConvertToJSON(pt, pWebData)) return false;
	for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
		pStock = make_shared<CTiingoStock>();
		try {
			pt2 = it->second;
			s = pt2.get<string>(_T("permaTicker"));
			if (s.size() > 0) pStock->m_strTiingoPermaTicker = s.c_str();
			s = pt2.get<string>(_T("ticker"));
			transform(s.begin(), s.end(), s.begin(), _toupper); // ��֪Ϊʲô�������ɿ�ʱ��ʹ��toupper������ʹ��_toupper����������ͨ����
			pStock->m_strTicker = s.c_str();
			s = pt2.get<string>(_T("name"));
			if (s.size() > 0) pStock->m_strName = s.c_str();
			pStock->m_fIsActive = pt2.get<bool>(_T("isActive"));
			pStock->m_fIsADR = pt2.get<bool>(_T("isADR"));
			s = pt2.get<string>(_T("industry"));
			if (s.compare(strNotAvailable) != 0) {
				if (s.size() > 0) pStock->m_strTiingoIndustry = s.c_str();
			}
			else pStock->m_strTiingoIndustry = strNULL;
			s = pt2.get<string>(_T("sector"));
			if (s.compare(strNotAvailable) != 0) {
				if (s.size() > 0) pStock->m_strTiingoSector = s.c_str();
			}
			else pStock->m_strTiingoSector = strNULL;
			s = pt2.get<string>(_T("sicCode"));
			if (s.compare(strNotAvailable) != 0) {
				pStock->m_iSICCode = atoi(s.c_str());
			}
			else pStock->m_iSICCode = 0;
			s = pt2.get<string>(_T("sicIndustry"));
			if (s.compare(strNotAvailable) != 0) {
				if (s.size() > 0) pStock->m_strSICIndustry = s.c_str();
			}
			else pStock->m_strSICIndustry = strNULL;
			s = pt2.get<string>(_T("sicSector"));
			if (s.compare(strNotAvailable) != 0) {
				if (s.size() > 0) pStock->m_strSICSector = s.c_str();
			}
			else pStock->m_strSICSector = strNULL;
			s = pt2.get<string>(_T("reportingCurrency"));
			if (s.compare(strNotAvailable) != 0) { // ����Ӧ����Զ����
				if ((s.size() > 0)) pStock->m_strReportingCurrency = s.c_str();
			}
			else pStock->m_strReportingCurrency = strNULL;
			s = pt2.get<string>(_T("location"));
			if (s.compare(strNotAvailable) != 0) {
				if ((s.size() > 0)) pStock->m_strLocation = s.c_str();
			}
			else pStock->m_strLocation = _T(" ");
			s = pt2.get<string>(_T("companyWebsite"));
			if (s.compare(strNotAvailable) != 0) {
				if (s.size() > 0) pStock->m_strCompanyWebSite = s.c_str();
			}
			else pStock->m_strCompanyWebSite = strNULL;
			s = pt2.get<string>(_T("secFilingWebsite"));
			if (s.compare(strNotAvailable) != 0) {
				if (s.size() > 0) pStock->m_strSECFilingWebSite = s.c_str();
			}
			else pStock->m_strSECFilingWebSite = strNULL;
			s = pt2.get<string>(_T("statementLastUpdated"));
			if (s.size() > 0) str = s.c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			pStock->m_lStatementUpdateDate = year * 10000 + month * 100 + day;
			s = pt2.get<string>(_T("dailyLastUpdated"));
			if (s.size() > 0) str = s.c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			pStock->m_lDailyDataUpdateDate = year * 10000 + month * 100 + day;
		}
		catch (ptree_error&) {
			fSucceed = false;
			break;
		}
		vTiingoStock.push_back(pStock);
		iCount++;
	}

	TRACE("����Tiingo Symbol����Ϊ%d\n", iCount);
	sprintf_s(buffer, _T("%6d"), iCount);
	strNumber = buffer;
	str = _T("����Tiingo Symbol����Ϊ") + strNumber;
	gl_systemMessage.PushInnerSystemInformationMessage(str);

	return fSucceed;
}

bool CWorldMarket::ProcessTiingoStockDayLine(CWebDataPtr pWebData, CWorldStockPtr& pStock) {
	vector<CDayLinePtr> vDayLine;
	ptree pt, pt2;
	string s;
	double dTemp = 0;
	long lTemp = 0;
	CDayLinePtr pDayLine = nullptr;
	CString str;
	long year, month, day;

	if (!ConvertToJSON(pt, pWebData)) { // �����̹߳���
		str = _T("Tiingo����");
		str += pStock->GetSymbol();
		str += _T("���߹���\n");
		TRACE("%s", str.GetBuffer());
		gl_systemMessage.PushInnerSystemInformationMessage(str);
		return false;
	}

	for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
		try {
			pDayLine = make_shared<CDayLine>();
			pt2 = it->second;
			s = pt2.get<string>(_T("date"));
			str = s.c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			lTemp = year * 10000 + month * 100 + day;
			pDayLine->SetDate(lTemp);
			dTemp = pt2.get<double>(_T("close"));
			pDayLine->SetClose(dTemp * 1000);
			dTemp = pt2.get<double>(_T("high"));
			pDayLine->SetHigh(dTemp * 1000);
			dTemp = pt2.get<double>(_T("low"));
			pDayLine->SetLow(dTemp * 1000);
			dTemp = pt2.get<double>(_T("open"));
			pDayLine->SetOpen(dTemp * 1000);
			lTemp = pt2.get<long>(_T("volume"));
			pDayLine->SetVolume(lTemp);
			vDayLine.push_back(pDayLine);
		}
		catch (ptree_error&) {
			return false; // ���ݽ�������Ļ����������
		}
	}
	sort(vDayLine.begin(), vDayLine.end(), CompareDayLineDate); // ����������˳�����С�
	for (auto& pDayLine2 : vDayLine) {
		pDayLine2->SetExchange(pStock->GetListedExchange());
		pDayLine2->SetStockSymbol(pStock->GetSymbol());
		pDayLine2->SetStockName(pStock->GetTicker());
	}
	pStock->UpdateDayLine(vDayLine);
	pStock->SetDayLineNeedUpdate(false);
	pStock->SetDayLineNeedSaving(true);
	pStock->SetUpdateProfileDB(true);
	return true;
}

bool CWorldMarket::ProcessOneTiingoIEXWebSocketData(shared_ptr<string> pData) {
	ptree pt, pt2, pt3;
	string sType, sSymbol;
	double price = 0;
	double volume = 0;
	time_t time = 0;
	try {
		if (ConvertToJSON(pt, *pData)) {
			sType = pt.get<string>(_T("type"));
			if (sType.compare(_T("trade")) == 0) { // ��������
				for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
					pt3 = it->second;
					sSymbol = pt3.get<string>(_T("s"));
					if (IsStock(sSymbol.c_str())) {
						price = pt3.get<double>(_T("p"));
						volume = pt3.get<double>(_T("v"));
						time = pt3.get<time_t>(_T("t"));
					}
				}
			}
			else {
				// ERROR
				int i = 0;
				i++;
			}
		}
	}
	catch (ptree_error&) {
		return false;
	}

	return true;
}

bool CWorldMarket::ProcessOneTiingoCryptoWebSocketData(shared_ptr<string> pData) {
	ptree pt, pt2, pt3;
	string sType, sSymbol;
	char chType;
	double price = 0;
	double volume = 0;
	time_t time = 0;
	int i = 0;

	try {
		if (ConvertToJSON(pt, *pData)) {
			sType = pt.get<string>(_T("messageType"));
			chType = sType.at(0);
			switch (chType) {
			case 'I': // ע��ɹ�
				break;
			case 'H': // heart beat
				break;
			case 'A': // new data
				break;
			default: //
				i++;
				break;
			}
		}
		else {
			return false;
		}
	}
	catch (ptree_error&) {
		return false;
	}

	return true;
}

bool CWorldMarket::ProcessOneTiingoForexWebSocketData(shared_ptr<string> pData) {
	ptree pt, pt2, pt3;
	string sType, sSymbol;
	char chType;
	double price = 0;
	double volume = 0;
	time_t time = 0;
	try {
		if (ConvertToJSON(pt, *pData)) {
			sType = pt.get<string>(_T("messageType"));
			chType = sType.at(0);
			switch (chType) { // ��������
			case 'A':
				break;
			case 'I':
				break;
			default:
				break;
			}
		}
		else {
			// ERROR
			int i = 0;
			i++;
		}
	}
	catch (ptree_error&) {
		return false;
	}

	return true;
}