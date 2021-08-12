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

//#include<boost/property_tree/ptree.hpp>
//#include<boost/property_tree/json_parser.hpp>
//using namespace boost::property_tree;

bool CompareDayLineDate(CDayLinePtr& p1, CDayLinePtr& p2);

bool CWorldMarket::ProcessTiingoStockSymbol(CWebDataPtr pWebData, vector<CTiingoStockPtr>& vTiingoStock) {
	string strNotAvailable{ _T("Field not available for free/evaluation") }; // Tiingo免费账户有多项内容空缺，会返回此信息。
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
			transform(s.begin(), s.end(), s.begin(), _toupper); // 不知为什么，当生成库时，使用toupper报错；而使用_toupper则正常编译通过。
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
			if (s.compare(strNotAvailable) != 0) { // 此项应该永远存在
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

	TRACE("今日Tiingo Symbol总数为%d\n", iCount);
	sprintf_s(buffer, _T("%6d"), iCount);
	strNumber = buffer;
	str = _T("今日Tiingo Symbol总数为") + strNumber;
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

	if (!ConvertToJSON(pt, pWebData)) { // 工作线程故障
		str = _T("Tiingo下载");
		str += pStock->GetSymbol();
		str += _T("日线故障\n");
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
			return false; // 数据解析出错的话，则放弃。
		}
	}
	sort(vDayLine.begin(), vDayLine.end(), CompareDayLineDate); // 以日期早晚顺序排列。
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
	ptree::iterator it;
	ptree pt, pt2, pt3;
	string sType, sSymbol, sService;
	char chType;
	double price = 0;
	double volume = 0;
	time_t time = 0;

	int iSubscriptionId = 0;
	string sMessageType, sTicker, sExchange, sDatetime;
	INT64 iSecond = 0;
	double dBidSize = 0;
	double dBidPrice = 0;
	double dMidPrice = 0;
	double dAskPrice = 0;
	double dAskSize = 0;
	double dLastSize = 0;
	double dLastPrice = 0;
	int iHalted = 0, iAfterHour = 0, iISO = 0, iOddlot = 0, iNMSRule611 = 0;
	int i = 0;
	CTiingoIEXWebSocketDataPtr pIEXData = nullptr;

	try {
		if (ConvertToJSON(pt, *pData)) {
			sType = pt.get<string>(_T("messageType"));
			switch (sType.at(0)) {
			case 'A': // 交易数据
				pIEXData = make_shared<CTiingoIEXWebSocketData>();
				sService = pt.get<string>(_T("service"));
				if (sService.compare(_T("iex")) != 0) return false; // 此项必须为"iex"
				pt2 = pt.get_child(_T("data"));
				it = pt2.begin();
				pt3 = it->second;
				sMessageType = pt3.get_value<string>(); // message type, 'Q'、'T'或者'B'
				chType = sMessageType.at(0);
				pIEXData->m_chMessageType = chType;
				switch (chType) {
				case 'Q':// top-of-book update message
					it++;
					pt3 = it->second;
					sDatetime = pt3.get_value<string>();
					it++;
					pt3 = it->second;
					pIEXData->m_iNanoseconds = pt3.get_value<INT64>();
					it++;
					pt3 = it->second;
					pIEXData->m_dBidSize = pt3.get_value<double>();
					it++;
					pt3 = it->second;
					pIEXData->m_dBidPrice = pt3.get_value<double>();
					it++;
					pt3 = it->second;
					pIEXData->m_dMidPrice = pt3.get_value<double>(); // MidPrice = (BidPrice + AskPrice) / 2
					it++;
					pt3 = it->second;
					pIEXData->m_dAskPrice = pt3.get_value<double>();
					it++;
					pt3 = it->second;
					pIEXData->m_dAskSize = pt3.get_value<double>();
					it++;
					pt3 = it->second;
					pIEXData->m_dLastPrice = pt3.get_value<double>();
					it++;
					pt3 = it->second;
					pIEXData->m_iHalted = pt3.get_value<int>();
					it++;
					pt3 = it->second;
					pIEXData->m_iAfterHour = pt3.get_value<int>();
					it++;
					pt3 = it->second;
					pIEXData->m_iISO = pt3.get_value<int>();
					it++;
					pt3 = it->second;
					pIEXData->m_iOddlot = pt3.get_value<int>();
					it++;
					pt3 = it->second;
					pIEXData->m_iNMSRule611 = pt3.get_value<int>();
					break;
				case 'T':// 'T' last trade message
					i++;
					break;
				case 'B':// 'B'trade break messages
					i++;
					break;
				default:
					i++;
					break;
				}
				m_qTiingoIEXWebSockerData.push(pIEXData);
				break;
			case 'I':// authenization  {\"data\":{\"subscriptionId\":2563367},\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"}}
				pt2 = pt.get_child(_T("data"));
				ASSERT(m_iTiingoIEXSubscriptionId == 0);
				m_iTiingoIEXSubscriptionId = pt2.get<int>(_T("subscriptionId"));
				break;
			case 'H': // Heart beat {\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}
				// 无需处理
				break;
			default:
				i++;
				break;
			}
		}
		else {
			// ERROR
			return false;
		}
	}
	catch (ptree_error&) {
		return false;
	}

	return true;
}

bool CWorldMarket::ProcessOneTiingoCryptoWebSocketData(shared_ptr<string> pData) {
	ptree::iterator it;
	ptree pt, pt2, pt3;
	string sType, sSymbol;
	char chType;
	double price = 0;
	double volume = 0;
	time_t time = 0;
	int i = 0;

	int iSubscriptionId = 0;
	string sMessageType, sTickers, sExchange, sDatetime, sService;
	double dBidSize;
	double dBidPrice;
	double dMidPrice;
	double dAskPrice;
	double dAskSize;
	double dLastSize;
	double dLastPrice;
	CTiingoCryptoWebSocketDataPtr pCryptoData = nullptr;

	try {
		if (ConvertToJSON(pt, *pData)) {
			sType = pt.get<string>(_T("messageType"));
			chType = sType.at(0);
			switch (chType) {
			case 'I': // 注册 {\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}
				pt2 = pt.get_child(_T("data"));
				ASSERT(m_iTiingoCryptoSubscriptionId == 0);
				m_iTiingoCryptoSubscriptionId = pt2.get<int>(_T("subscriptionId"));
				break;
			case 'H': // heart beat {\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}
				// do nothing
				break;
			case 'A': // new data
				pCryptoData = make_shared<CTiingoCryptoWebSocketData>();
				sService = pt.get<string>(_T("service"));
				if (sService.compare(_T("crypto_data")) != 0) return false; // 格式不符则退出
				pt2 = pt.get_child(_T("data"));
				it = pt2.begin();
				pt3 = it->second;
				sMessageType = pt3.get_value<string>(); // ‘Q’或者‘T’
				if (sMessageType.at(0) == 'T') { //last trade message {\"service\":\"crypto_data\",\"data\":[\"T\",\"jstusdt\",\"2021-08-10T23:56:55.237000+00:00\",\"huobi\",3952.5,0.062108],\"messageType\":\"A\"}
					pCryptoData->m_chMessageType = 'T';
					it++;
					pt3 = it->second;
					sTickers = pt3.get_value<string>(); // 证券名称
					pCryptoData->m_strSymbol = sTickers.c_str();
					it++;
					pt3 = it->second;
					sDatetime = pt3.get_value<string>(); // 时间串："2019-07-05T15:49:15.157000+00:00"
					it++;
					pt3 = it->second;
					sExchange = pt3.get_value<string>(); // 交易所
					pCryptoData->m_strExchange = sExchange.c_str();
					it++;
					pt3 = it->second;
					pCryptoData->m_dLastSize = pt3.get_value<double>(); // 最新数量
					it++;
					pt3 = it->second;
					pCryptoData->m_dLastPrice = pt3.get_value<double>(); // 最新价格
				}
				else { // 'Q' top-of-book update message.
					pCryptoData->m_chMessageType = 'Q';
					it++;
					pt3 = it->second;
					sTickers = pt3.get_value<string>(); // 证券名称
					pCryptoData->m_strSymbol = sTickers.c_str();
					it++;
					pt3 = it->second;
					sDatetime = pt3.get_value<string>(); // 时间串："2019-07-05T15:49:15.157000+00:00"
					it++;
					pt3 = it->second;
					sExchange = pt3.get_value<string>(); // 交易所
					pCryptoData->m_strExchange = sExchange.c_str();
					it++;
					pt3 = it->second;
					pCryptoData->m_dBidSize = pt3.get_value<double>(); // 买价数量
					it++;
					pt3 = it->second;
					pCryptoData->m_dBidPrice = pt3.get_value<double>(); // 买价
					it++;
					pt3 = it->second;
					pCryptoData->m_dMidPrice = pt3.get_value<double>(); // 中间价 （BidPrice + AskPrice)/2
					it++;
					pt3 = it->second;
					pCryptoData->m_dAskSize = pt3.get_value<double>(); // 卖价数量
					it++;
					pt3 = it->second;
					pCryptoData->m_dAskPrice = pt3.get_value<double>(); // 卖价
				}
				m_qTiingoCryptoWebSocketData.push(pCryptoData);
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

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// {
//	"messageType":"A",
//		"service" : "fx",
//		"data" : [
//			  "Q",
//				"eurnok",
//				"2019-07-05T15:49:15.157000+00:00",
//				5000000.0,
//				9.6764,
//				9.678135,
//				5000000.0,
//				9.67987
//		]
// }
// {
//	"messageType":"A",
//		"service" : "fx",
//		"data" : [
//			  "Q",
//				"gbpaud",
//				"2019-07-05T15:49:15.236000+00:00",
//				1000000.0,
//				1.79457,
//				1.79477,
//				5000000.0,
//				1.79497
//		]
// }
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::ProcessOneTiingoForexWebSocketData(shared_ptr<string> pData) {
	ptree pt, pt2, pt3;
	ptree::iterator it;
	string sType, sSymbol, sService;
	char chType;
	double price = 0;
	double volume = 0;
	time_t time = 0;

	string sMessageType, sTickers, sDatetime;
	double dBidSize;
	double dBidPrice;
	double dMidPrice;
	double dAskPrice;
	double dAskSize;
	CTiingoForexWebSocketDataPtr pForexData = nullptr;

	try {
		if (ConvertToJSON(pt, *pData)) {
			sService = pt.get<string>(_T("service"));
			if (sService.compare(_T("fx")) != 0) return false;
			sType = pt.get<string>(_T("messageType"));
			chType = sType.at(0);
			switch (chType) { // 交易数据
			case 'A': // forex目前只有此项
				pForexData = make_shared<CTiingoForexWebSocketData>();
				pt2 = pt.get_child(_T("data"));
				it = pt2.begin();
				pt3 = it->second;
				sMessageType = pt3.get_value<string>(); // 必须是‘Q’
				pForexData->m_chMessageType = sMessageType.at(0);
				it++;
				pt3 = it->second;
				sTickers = pt3.get_value<string>(); // 证券名称
				pForexData->m_strSymbol = sTickers.c_str();
				it++;
				pt3 = it->second;
				sDatetime = pt3.get_value<string>(); // 时间串："2019-07-05T15:49:15.157000+00:00"
				it++;
				pt3 = it->second;
				pForexData->m_dBidSize = pt3.get_value<double>(); // 买价数量
				it++;
				pt3 = it->second;
				pForexData->m_dBidPrice = pt3.get_value<double>(); // 买价
				it++;
				pt3 = it->second;
				pForexData->m_dMidPrice = pt3.get_value<double>(); // 中间价 （BidPrice + AskPrice)/2
				it++;
				pt3 = it->second;
				pForexData->m_dAskPrice = pt3.get_value<double>(); // 卖价数量
				it++;
				pt3 = it->second;
				pForexData->m_dAskSize = pt3.get_value<double>(); // 卖价
				m_qTiingoForexWebSocketData.push(pForexData);
				break;
			default:
				// error
				break;
			}
		}
		else {
			// ERROR
			return false;
		}
	}
	catch (ptree_error&) {
		return false;
	}

	return true;
}