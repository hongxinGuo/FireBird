#include"pch.h"

#include"ctype.h"

#include"globedef.h"
#include"accessory.h"
#include"WorldMarket.h"

using namespace std;
#include<algorithm>
#include<string>
#include<map>
#include<vector>
#include<queue>
#include<atomic>
#include<memory>

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

#include"WebData.h"
#include"EconomicCalendar.h"

bool CompareDayLineDate(CDayLinePtr& p1, CDayLinePtr& p2);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// https://api.tiingo.com/documentation/websockets/iex
//
// 共四种格式：
// {"messageType":"I","data":{"subscriptionId":2563367},"response":{"code":200,"message":"Success"}}
// {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
// {"messageType":"A","service":"iex","data":["Q","2019-01-30T13:33:45.383129126-05:00",1548873225383129126,"vym",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}
// {"messageType":"A","service":"iex","data":["T","2019-01-30T13:33:45.594808294-05:00",1548873225594808294,"wes",null,null,null,null,null,50.285,200,null,0,0,0,0]}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::ParseTiingoIEXWebSocketData(shared_ptr<string> pData) {
	ptree::iterator it;
	ptree pt, pt2, pt3;
	string sType, sSymbol, sService;
	char chType;

	string sMessageType, sTicker, sExchange, sDatetime, sValue;
	int i = 0;
	CTiingoIEXSocketPtr pIEXData = nullptr;

	try {
		if (ConvertToJSON(pt, *pData)) {
			sType = pt.get<string>(_T("messageType"));
			switch (sType.at(0)) {
			case 'A': // 交易数据
				pIEXData = make_shared<CTiingoIEXSocket>();
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
					sTicker = pt3.get_value<string>();
					pIEXData->m_strSymbol = sTicker.c_str();
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dBidSize = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dBidPrice = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dMidPrice = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dAskPrice = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dAskSize = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dLastPrice = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dLastSize = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					pIEXData->m_iHalted = pt3.get_value<int>();
					it++;
					pt3 = it->second;
					pIEXData->m_iAfterHour = pt3.get_value<int>();
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_iISO = atoi(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_iOddlot = atoi(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_iNMSRule611 = atoi(sValue.c_str());
					}
					break;
				case 'T':// 'T' last trade message
					it++;
					pt3 = it->second;
					sDatetime = pt3.get_value<string>();
					it++;
					pt3 = it->second;
					pIEXData->m_iNanoseconds = pt3.get_value<INT64>();
					it++;
					pt3 = it->second;
					sTicker = pt3.get_value<string>();
					pIEXData->m_strSymbol = sTicker.c_str();
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dBidSize = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dBidPrice = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dMidPrice = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dAskPrice = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dAskSize = atof(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dLastPrice = pt3.get_value<double>();
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_dLastSize = atoi(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_iHalted = atoi(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_iISO = atoi(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_iOddlot = atoi(sValue.c_str());
					}
					it++;
					pt3 = it->second;
					sValue = pt3.get_value<string>();
					if (sValue.compare(_T("null")) != 0) {
						pIEXData->m_iNMSRule611 = atoi(sValue.c_str());
					}
					break;
				case 'B':// 'B'trade break messages
					i++;
					break;
				default: // 错误
					return false;
					break;
				}
				gl_SystemData.PushTiingoIEXSocket(pIEXData);
				break;
			case 'I':// authenization  {\"messageType\":\"I\",\"data\":{\"subscriptionId\":2563367},\"response\":{\"code\":200,\"message\":\"Success\"}}
				pt2 = pt.get_child(_T("data"));
				ASSERT(m_dataTiingoIEXWebSocket.GetSubscriptionId() == 0);
				m_dataTiingoIEXWebSocket.SetSubscriptionId(pt2.get<int>(_T("subscriptionId")));
				break;
			case 'H': // Heart beat {\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}
				// 无需处理
				TRACE("IEX heart beat\n");
				break;
			default:
				return false;
				break;
			}
		}
		else {
			// ERROR
			return false;
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo IEX WebSocket "), e);
		return false;
	}

	return true;
}

/// <summary>
///
/// https://api.tiingo.com/documentation/websockets/crypto
///
/// 共四种格式：
/// {"messageType":"I","response":{"code":200,"message":"Success"},"data":{"subscriptionId":2563396}}
/// {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
/// {"messageType":"A","service":"crypto_data","data":["Q","neojpy","2019-01-30T18:03:40.195515+00:00","bitfinex",38.11162867,787.82,787.83,42.4153887,787.84]}
/// {"messageType":"A","service":"crypto_data","data":["T","evxbtc","2019-01-30T18:03:40.056000+00:00","binance",405.0,9.631e-05]}
///
/// </summary>
/// <param name="pData"></param>
/// <returns></returns>
bool CWorldMarket::ParseTiingoCryptoWebSocketData(shared_ptr<string> pData) {
	ptree::iterator it;
	ptree pt, pt2, pt3;
	string sType, sSymbol;
	char chType;

	string sMessageType, sTickers, sExchange, sDatetime, sService;
	CTiingoCryptoSocketPtr pCryptoData = nullptr;

	try {
		if (ConvertToJSON(pt, *pData)) {
			sType = pt.get<string>(_T("messageType"));
			chType = sType.at(0);
			switch (chType) {
			case 'I': // 注册 {\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}
				pt2 = pt.get_child(_T("data"));
				ASSERT(m_dataTiingoCryptoWebSocket.GetSubscriptionId() == 0);
				m_dataTiingoCryptoWebSocket.SetSubscriptionId(pt2.get<int>(_T("subscriptionId")));
				break;
			case 'H': // heart beat {\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}
				// do nothing
				break;
			case 'A': // new data
				pCryptoData = make_shared<CTiingoCryptoSocket>();
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
				else if (sMessageType.at(0) == 'Q') { // 'Q' top-of-book update message.
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
				else { // 格式不对
					return false;
				}
				gl_SystemData.PushTiingoCryptoSocket(pCryptoData);
				break;
			default: // 错误
				return false;
				break;
			}
		}
		else {
			return false;
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Crypto WebSocket "), e);
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// https://api.tiingo.com/documentation/websockets/forex
//
// 共四种格式：
// {"messageType":"I","response":{"code":200,"message":"Success"},"data":{"subscriptionId":2563396}}
// {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
// {"messageType":"A","service":"fx","data":["Q","eurnok","2019-07-05T15:49:15.157000+00:00",5000000.0,9.6764,9.678135,5000000.0,9.67987]}
// {"messageType":"A","service":"fx","data":["Q","gbpaud","2019-07-05T15:49:15.236000+00:00",1000000.0,1.79457,1.79477,5000000.0,1.79497]}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldMarket::ParseTiingoForexWebSocketData(shared_ptr<string> pData) {
	ptree pt, pt2, pt3;
	ptree::iterator it;
	string sType, sSymbol, sService;
	char chType;

	string sMessageType, sTickers, sDatetime;
	CTiingoForexSocketPtr pForexData = nullptr;

	try {
		if (ConvertToJSON(pt, *pData)) {
			sType = pt.get<string>(_T("messageType"));
			chType = sType.at(0);
			switch (chType) {
			case 'I': // 注册 {\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}
				pt2 = pt.get_child(_T("data"));
				ASSERT(m_dataTiingoForexWebSocket.GetSubscriptionId() == 0);
				m_dataTiingoForexWebSocket.SetSubscriptionId(pt2.get<int>(_T("subscriptionId")));
				break;
			case 'H': // HeartBeat {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
				// do nothing
				break;
			case 'A': // new data
				sService = pt.get<string>(_T("service"));
				if (sService.compare(_T("fx")) != 0) return false; // 只有此项
				pForexData = make_shared<CTiingoForexSocket>();
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
				pForexData->m_dAskSize = pt3.get_value<double>(); // 卖价数量
				it++;
				pt3 = it->second;
				pForexData->m_dAskPrice = pt3.get_value<double>(); // 卖价
				gl_SystemData.PushTiingoForexSocket(pForexData);
				break;
			default:
				// error
				gl_systemMessage.PushErrorMessage(_T("Tiingo Forex WebSocket type Error"));
				return false;
				break;
			}
		}
		else {
			// ERROR
			return false;
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Forex WebSocket "), e);
		return false;
	}

	return true;
}