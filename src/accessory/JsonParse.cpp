//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// json数据解析的任务皆位于此文件中。
// 解析是目前最费时的函数，故而DEBUG模式时也要进行全局优化（/GL），只有这样才能在规定时间内处理完数据。
// Netease实时数据是目前最需要关注的，每300毫秒接收900个数据时，系统基本上独占一个核心处理器了。如果再有耗时的任务，就需要
// 将其分配到其他核心处理器上，这样就会导致系统的响应时间增加，而且系统的负载也会增加。
//
// 在DEBUG模式下，boost PTree速度比Nlohmann json快，但Release模式下nlohmann json的速度比boost ptree快50%左右。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "JsonParse.h"

#include"WebData.h"
#include"WebRTData.h"

#include<string>
#include<memory>

#include "ChinaMarket.h"
#include "ChinaStockCodeConverter.h"
#include "JsonGetValue.h"

using namespace std;

wstring to_wide_string(const std::string& input) {
	const long lLength = input.size();
	const auto pBuffer = new char[lLength + 1];

	for (int i = 0; i < input.size(); i++) { pBuffer[i] = input.at(i); }
	pBuffer[lLength] = 0x000;
	const auto pBufferW = new WCHAR[lLength * 2];

	const long lReturnSize = MultiByteToWideChar(CP_UTF8, 0, pBuffer, lLength, pBufferW, lLength * 2);
	pBufferW[lReturnSize] = 0x000;
	wstring ws = pBufferW;

	delete[]pBuffer;
	delete[]pBufferW;

	return ws;
}

string to_byte_string(const wstring& input) {
	const long lLength = input.size();
	const auto pBufferW = new WCHAR[lLength + 1];

	for (int i = 0; i < lLength + 1; i++) pBufferW[i] = 0x000;
	for (int i = 0; i < input.size(); i++) { pBufferW[i] = input.at(i); }
	const auto pBuffer = new char[lLength * 2];

	const long lReturnSize = WideCharToMultiByte(CP_UTF8, 0, pBufferW, lLength, pBuffer, lLength * 2, nullptr, nullptr);
	pBuffer[lReturnSize] = 0x000;
	string s = pBuffer;

	delete[]pBuffer;
	delete[]pBufferW;

	return s;
}

void ReportJsonError(const json::parse_error& e, const std::string& s) {
	char buffer[180]{}, buffer2[100];
	int i;
	CString str = e.what();
	gl_systemMessage.PushErrorMessage(_T("Nlohmann JSon Reading Error ") + str);
	for (i = 0; i < 180; i++) buffer[i] = 0x000;
	for (i = 0; i < 180; i++) {
		if ((e.byte - 90 + i) < s.size()) { buffer[i] = s.at(e.byte - 90 + i); }
		else break;
	}
	sprintf_s(buffer2, _T("%d  "), static_cast<long>(s.size()));
	str = buffer2;
	sprintf_s(buffer2, _T("%d  "), static_cast<long>(e.byte));
	str += buffer2;
	sprintf_s(buffer2, _T("%d  "), i);
	str += buffer2;
	str += buffer;
	gl_systemMessage.PushErrorMessage(str);
}

void ReportJSonErrorToSystemMessage(const CString& strPrefix, const CString& strWhat) {
	gl_systemMessage.PushErrorMessage(strPrefix + strWhat);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 从网络文件file中读取新浪制式实时数据，返回值是所读数据是否出现格式错误。
//
//  新浪实时行情站点：https://hq.sinajs.cn/list=sh601006
// OpenURL时，需要设置	m_strHeaders = _T("User-Agent:FireBird\r\nReferer:https://finance.sina.com.cn\r\n");
//
// var hq_str_sh601006="大秦铁路,27.55,27.25,26.91,27.55,26.20,26.91,26.92,
//                     22114263,589824680,4695,26.91,57590,26.90,14700,26.89,14300,
//                     26.88,15100,26.87,3100,26.92,8900,26.93,14230,26.94,25150,26.95,15220,26.96,2008-01-11,15:05:32,00";
//
// 无效数据格式为：var hq_str_sh688801="";
//
// 这个字符串由许多数据拼接在一起，不同含义的数据用逗号隔开了，按照程序员的思路，顺序号从0开始。
// 0：”大秦铁路”，股票名字；
// 1：”27.55″，今日开盘价；
// 2：”27.25″，昨日收盘价；
// 3：”26.91″，当前价格；
// 4：”27.55″，今日最高价；
// 5：”26.20″，今日最低价；
// 6：”26.91″，竞买价，即“买一”报价；
// 7：”26.92″，竞卖价，即“卖一”报价；
// 8：”22114263″，成交的股票数，由于股票交易以一百股为基本单位，所以在使用时，通常把该值除以一百；
// 9：”589824680″，成交金额，单位为“元”，为了一目了然，通常以“万元”为成交金额的单位，所以通常把该值除以一万；
// 10：”4695″，“买一”申请4695股，即47手；
// 11：”26.91″，“买一”报价；
// 12：”57590″，“买二”
// 13：”26.90″，“买二”
// 14：”14700″，“买三”
// 15：”26.89″，“买三”
// 16：”14300″，“买四”
// 17：”26.88″，“买四”
// 18：”15100″，“买五”
// 19：”26.87″，“买五”
// 20：”3100″，“卖一”申报3100股，即31手；
// 21：”26.92″，“卖一”报价
// (22, 23), (24, 25), (26, 27), (28, 29)分别为“卖二”至“卖四的情况”
// 30：”2008 - 01 - 11″，日期；（此日期为当地市场的日期）
// 31：”15:05:32″，时间；（此时间为当地市场的时间，此处为东八区北京标准时间）
// 32：”00”，  不明数据
//////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<CWebRTDataPtr>> ParseSinaRTData(CWebDataPtr pWebData) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();

	pWebData->ResetCurrentPos();
	while (!pWebData->IsProcessedAllTheData()) {
		auto pRTData = make_shared<CWebRTData>();
		if (pRTData->ReadSinaData(pWebData)) {
			pvWebRTData->push_back(pRTData);
		}
		else {
			gl_systemMessage.PushErrorMessage(_T("新浪实时数据解析返回失败信息"));
			break; // 后面的数据出问题，抛掉不用。
		}
	}
	return pvWebRTData;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 当所有被查询的股票皆为非上市股票时，腾讯实时股票服务器会返回一个21个字符长的字符串：v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IsTengxunRTDataInvalid(CWebData& WebDataReceived) {
	char buffer[50]{};
	char* pBuffer = buffer;
	const CString strInvalidStock = _T("v_pv_none_match=\"1\";\n"); // 此为无效股票查询到的数据格式，共21个字符

	WebDataReceived.GetData(pBuffer, 21, WebDataReceived.GetCurrentPos());
	buffer[21] = 0x000;
	const CString str1 = buffer;

	if (str1.Compare(strInvalidStock) == 0) {
		ASSERT(WebDataReceived.GetBufferLength() == 21);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 腾讯实时数据，如果遇到被查询股票代码为非上市时，只是简单略过，不返回数据。故而查询900个股票，返回的数据量要小于900.
// 只有当所有的查询股票皆为非上市时，才返回一个21个字符串：v_pv_none_match=\"1\";\n
//
//
// 从网络文件file中读取腾讯制式实时数据，返回值是所读数据是否出现格式错误。
//
// 要获取最新行情，访问数据接口：http://qt.gtimg.cn/q=sz002818
//
// v_sz000001="51~平安银行~000001~15.59~15.90~15.75~1046363~518391~527971~
//             15.58~2365~15.57~802~15.56~1855~15.55~2316~15.54~320~15.59~661~15.60~15381~15.61~3266~15.62~450~15.63~520~~
//             20190930154003~-0.31~-1.95~15.89~15.57~15.59/1046363/1645828527~1046363~164583~0.54~11.27~~
//             15.89~15.57~2.01~3025.36~3025.38~1.15~17.49~14.31~
//             0.73~-12617~15.73~9.82~12.19~~~1.24~164582.85~0.00~0~~GP-A~68.91~~0.82";\n
//
// 0: 市场（上海：1，深圳：51, 港股：100， 美股：200）。
// 1 : 名字
// 2 : 代码
// 3 : 现价
// 4 : 昨收
// 5 : 今开
// 6 : 成交量（手）
// 7 : 外盘
// 8 : 内盘 （第一行结束）
// 9 : 买一
// 10 : 买一量（手）
// 11 - 18 : 买二 - 买五
// 19 : 卖一
// 20 : 卖一量
// 21 - 28 : 卖二 - 卖五
// 29 : 最近逐笔成交 ( 第二行结束）
// 30 : 时间（此时间为当地市场时间，此处为东八区北京标准时间）
// 31 : 涨跌
// 32 : 涨跌 %
// 33 : 最高
// 34 : 最低
// 35 : 价格 / 成交量（手） / 成交额（元）。 可以使用此处的数据代替36、37处的数据，这样就可以使用腾讯实时数据了。
// 36 : 成交量（手）
// 37 : 成交额（万）
// 38 : 换手率
// 39 : 市盈率
// 40 : /       (第三行结束）
// 41 : 最高
// 42 : 最低
// 43 : 振幅
// 44 : 流通市值(单位为：亿）
// 45 : 总市值（单位为：亿）
// 46 : 市净率
// 47 : 涨停价
// 48 : 跌停价   （第四行结束）
// 49 ：量比
// 50 :
// 51 : 均价
// 52 ：动态市盈率
// 53 ：静态市盈率
// 54 ：
// 55 ：
// 56 ：
// 57 ：成交额
// 58 ：
// 59 ：
// 60 ：
// 61 ：GP-A
// 62 ：
// 63 ：
// 64 ：
// 65 ：
// 66 ：
//
//
// 腾讯实时数据中，成交量的单位为手，无法达到计算所需的精度（股），故而只能作为数据补充之用。
//
//////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<CWebRTDataPtr>> ParseTengxunRTData(CWebDataPtr pWebData) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();

	pWebData->ResetCurrentPos();
	if (!IsTengxunRTDataInvalid(*pWebData)) { // 处理这21个字符串的函数可以放在这里，也可以放在最前面。
		while (!pWebData->IsProcessedAllTheData()) {
			auto pRTData = make_shared<CWebRTData>();
			if (pRTData->ReadTengxunData(pWebData)) {
				pvWebRTData->push_back(pRTData);
			}
			else {
				break; // 后面的数据出问题，抛掉不用。
			}
		}
	}
	return pvWebRTData;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理从网易日线服务器上读取的股票日线数据。
// 数据制式为： 日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n
//
// 日线数据是逆序的，最新日期的在前面。
//
/////////////////////////////////////////////////////////////////////////////////////////////////
CDayLineWebDataPtr ParseNeteaseDayLine(CWebDataPtr pWebData) {
	auto pData = make_shared<CDayLineWebData>();

	pData->TransferWebDataToBuffer(pWebData);
	pData->ProcessNeteaseDayLineData(); //pData的日线数据是逆序的，最新日期的在前面。

	return pData;
}

//
// {
// "code":0,
// "msg":"",
// "data":
//   {
//   "sh600601":
//      { "day":
//				[ ["2023-01-19","2.550","2.600","2.610","2.550","86162.000"],
//					["2023-01-20","2.600","2.620","2.620","2.590","100735.000"]],
//				"qt":{},
//				"mx_price":{"mx":[],"price":[]},
//				"prec":"2.560",
//				"version":"16"
//		  }
//	 }
// }
//
shared_ptr<vector<CDayLinePtr>> ParseTengxunDayLine(json* pjs, CString strStockCode) {
	auto pvDayLine = make_shared<vector<CDayLinePtr>>();
	string sTemp;
	try {
		json js2 = pjs->at(_T("data"));
		json js3 = jsonGetChild(&js2, strStockCode);
		double dLastClose = 0;
		//json js6 = js2.();
		json js4 = js3.at("day");
		for (auto it = js4.begin(); it != js4.end(); ++it) {
			auto pDayLine = make_shared<CDayLine>();
			long year, month, day;
			json js5 = it.value();
			pDayLine->SetLastClose(dLastClose * 1000);
			sTemp = js5.at(0);
			sscanf_s(sTemp.c_str(), _T("%4d-%02d-%02d"), &year, &month, &day);
			pDayLine->SetDate(year * 10000 + month * 100 + day);
			sTemp = js5.at(1);
			pDayLine->SetOpen(atof(sTemp.c_str()) * 1000);
			sTemp = js5.at(2);
			double dClose = atof(sTemp.c_str());
			pDayLine->SetClose(dClose * 1000);
			dLastClose = dClose;
			sTemp = js5.at(3);
			pDayLine->SetHigh(atof(sTemp.c_str()) * 1000);
			sTemp = js5.at(4);
			pDayLine->SetLow(atof(sTemp.c_str()) * 1000);
			sTemp = js5.at(5);
			pDayLine->SetVolume(atof(sTemp.c_str()) * 100);
			pvDayLine->push_back(pDayLine);
		}
	}
	catch (json::exception&) {
		return pvDayLine;
	}
	return pvDayLine;
}

//
// {
// "code":0,
// "msg":"",
// "data":
//   {
//   "sh600601":
//      { "day":
//				[ ["2023-01-19","2.550","2.600","2.610","2.550","86162.000"],
//					["2023-01-20","2.600","2.620","2.620","2.590","100735.000"]],
//				"qt":{},
//				"mx_price":{"mx":[],"price":[]},
//				"prec":"2.560",
//				"version":"16"
//		  }
//	 }
// }
//
CDayLineWebDataPtr ParseTengxunDayLine(CWebDataPtr pData) {
	auto pDayLineData = make_shared<CDayLineWebData>();
	const CString strSymbol = pData->GetStockCode();
	const CString strDisplaySymbol = gl_pChinaMarket->GetStock(strSymbol)->GetDisplaySymbol();
	bool fProcess = true;
	if (!pData->IsParsed()) {
		if (!pData->CreateJson()) {
			const CString strMessage = pData->GetStockCode() + _T(": Tengxun DayLine data json parse error");
			gl_systemMessage.PushErrorMessage(strMessage);
			fProcess = false;
		}
	}
	if (fProcess && pData->IsParsed()) {
		json* pjs = pData->GetJSon();
		const shared_ptr<vector<CDayLinePtr>> pvDayLine = ParseTengxunDayLine(pjs, XferStandardToTengxun(pData->GetStockCode()));
		ranges::sort(*pvDayLine, [](CDayLinePtr pData1, CDayLinePtr pData2) { return pData1->GetMarketDate() < pData2->GetMarketDate(); });
		for (const auto& pDayLine : *pvDayLine) {
			pDayLine->SetStockSymbol(strSymbol);
			pDayLine->SetDisplaySymbol(strDisplaySymbol);
			pDayLineData->AppendDayLine(pDayLine);
		}
	}
	return pDayLineData;
}

// 将utf-8字符串转化为CString
CString XferToCString(const std::string& s) {
	CString strName3;
	wstring wsName;
	CStringW strWName;

	wsName = to_wide_string(s); // 将中文utf8转成宽字节字符串
	strWName = wsName.c_str(); // 将标准库的宽字节字符串转换成CStringW制式，
	strName3 = strWName; // 将CStringW制式转换成CString
	return strName3;
}
