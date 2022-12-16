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

#include<string>

#include "JsonParse.h"

#include"WebRTData.h"
#include"SaveAndLoad.h"

#include<boost/property_tree/json_parser.hpp>

bool ConvertToWJSON(wptree& pt, string& s) {
	wstring ws = to_wide_string(s);
	wstringstream ss(ws);
	try {
		read_json(ss, pt);
	}
	catch (ptree_error& e) {
		//ReportJSonErrorToSystemMessage(_T("PTree JSon Reading Error "), e);
		return false;
	}
	return true;
}

wstring to_wide_string(const std::string& input) {
	long const lLength = input.size();
	auto const pBuffer = new char[lLength + 1];

	for (int i = 0; i < input.size(); i++) {
		pBuffer[i] = input.at(i);
	}
	pBuffer[lLength] = 0x000;
	auto const pBufferW = new WCHAR[lLength * 2];

	const long lReturnSize = MultiByteToWideChar(CP_UTF8, 0, pBuffer, lLength, pBufferW, lLength * 2);
	pBufferW[lReturnSize] = 0x000;
	wstring ws = pBufferW;

	delete[]pBuffer;
	delete[]pBufferW;

	return ws;
}

string to_byte_string(const wstring& input) {
	long const lLength = input.size();
	auto const pBufferW = new WCHAR[lLength + 1];

	for (int i = 0; i < lLength + 1; i++) pBufferW[i] = 0x000;
	for (int i = 0; i < input.size(); i++) {
		pBufferW[i] = input.at(i);
	}
	auto const pBuffer = new char[lLength * 2];

	long const lReturnSize = WideCharToMultiByte(CP_UTF8, 0, pBufferW, lLength, pBuffer, lLength * 2, NULL, NULL);
	pBuffer[lReturnSize] = 0x000;
	string s = pBuffer;

	delete[]pBuffer;
	delete[]pBufferW;

	return s;
}

void ReportJsonError(json::parse_error& e, std::string& s) {
	char buffer[180]{}, buffer2[100];
	int i = 0;
	CString str = e.what();
	gl_systemMessage.PushErrorMessage(_T("Nlohmann JSon Reading Error ") + str);
	for (i = 0; i < 180; i++) buffer[i] = 0x000;
	for (i = 0; i < 180; i++) {
		if ((e.byte - 90 + i) < s.size()) {
			buffer[i] = s.at(e.byte - 90 + i);
		}
		else break;
	}
	sprintf_s(buffer2, _T("%d  "), (long)(s.size()));
	str = buffer2;
	sprintf_s(buffer2, _T("%d  "), (long)(e.byte));
	str += buffer2;
	sprintf_s(buffer2, _T("%d  "), i);
	str += buffer2;
	str += buffer;
	gl_systemMessage.PushErrorMessage(str);
}

string ptreeGetString(ptree& pt, const char* szKey, const char* szDefault) {
	string s;
	try {
		s = pt.get<string>(szKey);
	}
	catch (ptree_error&) {
		s = szDefault;
	}
	if (s == _T("null")) return szDefault;
	return s;
}

string ptreeGetString(ptree* ppt, const char* szKey, const char* szDefault) {
	string s;
	try {
		s = ppt->get<string>(szKey);
	}
	catch (ptree_error&) {
		s = szDefault;
	}
	if (s != _T("null")) return szDefault;
	return s;
}

string ptreeGetString(shared_ptr<ptree> ppt, const char* szKey, const char* szDefault) {
	string s;
	try {
		s = ppt->get<string>(szKey);
	}
	catch (ptree_error&) {
		s = szDefault;
	}
	if (s == _T("null")) return szDefault;
	return s;
}

double ptreeGetDouble(ptree& pt, const char* szKey, double dDefault) {
	string s;
	try {
		s = pt.get<string>(szKey);
	}
	catch (ptree_error&) {
		return dDefault;
	}
	if (s == _T("null")) return dDefault;
	else return stod(s);
}

int ptreeGetInt(ptree& pt, const char* szKey, int iDefault) {
	string s;
	try {
		s = pt.get<string>(szKey);
	}
	catch (ptree_error&) {
		return iDefault;
	}
	if (s == _T("null")) return iDefault;
	else return stoi(s);
}

long long ptreeGetLongLong(ptree& pt, const char* szKey, long long llDefault) {
	string s;
	try {
		s = pt.get<string>(szKey);
	}
	catch (ptree_error&) {
		return llDefault;
	}
	if (s == _T("null")) return llDefault;
	else return stoll(s);
}

long ptreeGetLong(ptree& pt, const char* szKey, long lDefault) {
	string s;
	try {
		s = pt.get<string>(szKey);
	}
	catch (ptree_error&) {
		return lDefault;
	}
	if (s == _T("null")) return lDefault;
	else return stol(s);
}

bool ptreeGetChild(ptree& inputPt, const char* szKey, ptree* outputPpt) {
	try {
		*outputPpt = inputPt.get_child(szKey);
	}
	catch (ptree_error&) {
		return false;
	}
	return true;
}

void ReportJSonErrorToSystemMessage(CString strPrefix, ptree_error& e) {
	CString strError = strPrefix;
	strError += e.what();
	gl_systemMessage.PushErrorMessage(strError);
}

void ReportJSonErrorToSystemMessage(CString strPrefix, std::string data, ptree_error& e) {
	CString const strData = data.c_str();

	ReportJSonErrorToSystemMessage(strPrefix + strData.Left(80) + _T(" "), e);
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
	int iTotal = 0;
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();

	// 截取实时数据时用。为了测试解析速度
	if (static int i = 0; i < pWebData->GetBufferLength()) {
		string s = pWebData->GetDataBuffer();
		//SaveToFile(_T("C:\\StockAnalysis\\SinaRTData.dat"), s.c_str());
		i = pWebData->GetBufferLength();
	}
	pWebData->ResetCurrentPos();
	while (!pWebData->IsProcessedAllTheData()) {
		CWebRTDataPtr pRTData = make_shared<CWebRTData>();
		if (pRTData->ReadSinaData(pWebData)) {
			iTotal++;
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
	else return false;
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
	static int i = 0;
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();

	// 截取实时数据时用。为了测试解析速度
	if (i <= pWebData->GetBufferLength()) {
		string s = pWebData->GetDataBuffer();
		//SaveToFile(_T("C:\\StockAnalysis\\TengxunRTData.dat"), s.c_str());
		i = pWebData->GetBufferLength();
	}

	pWebData->ResetCurrentPos();
	if (!IsTengxunRTDataInvalid(*pWebData)) {
		// 处理这21个字符串的函数可以放在这里，也可以放在最前面。
		while (!pWebData->IsProcessedAllTheData()) {
			CWebRTDataPtr pRTData = make_shared<CWebRTData>();
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
CNeteaseDayLineWebDataPtr ParseNeteaseDayLine(CWebDataPtr pWebData) {
	auto pData = make_shared<CNeteaseDayLineWebData>();

	pData->TransferWebDataToBuffer(pWebData);
	pData->ProcessNeteaseDayLineData(); //pData的日线数据是逆序的，最新日期的在前面。

	return pData;
}

shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();

	for (auto it = pjs->begin(); it != pjs->end(); ++it) {
		if (gl_systemStatus.IsExitingSystem()) return 0;
		auto pRTData = make_shared<CWebRTData>();
		pRTData->SetDataSource(NETEASE_RT_WEB_DATA_);
		if (ParseOneNeteaseRTDataWithNlohmannJSon(it, pRTData)) {
			pRTData->CheckNeteaseRTDataActive();
			pvWebRTData->push_back(pRTData);
		}
	}
	return pvWebRTData;
}

shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(ptree* ppt) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();

	for (auto it = ppt->begin(); it != ppt->end(); ++it) {
		auto pRTData = make_shared<CWebRTData>();
		pRTData->SetDataSource(NETEASE_RT_WEB_DATA_);
		if (pRTData->ParseNeteaseDataWithPTree(it)) {
			pvWebRTData->push_back(pRTData);
		}
	}
	return pvWebRTData;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// 要获取最新行情，访问数据接口：http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
// 使用json解析，已经没有错误数据了。(偶尔还会有，大致每分钟出现一次）。
//
//////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithPTree(CWebDataPtr pData) {
	string ss;
	shared_ptr<ptree> ppt = nullptr;
	int iTotal = 0;
	bool fProcess = true;
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();

	fProcess = true;
	if (!pData->IsParsed()) {
		if (!pData->CreatePropertyTree(21, 2)) {
			// 网易数据前21位为前缀，后两位为后缀
			fProcess = false;
		}
	}
	else {
		ASSERT(pData->GetJSon() == nullptr);
	}
	if (fProcess && pData->IsParsed()) {
		ppt = pData->GetPTree();
		ptree* ppt2 = ppt.get();
		pvWebRTData = ParseNeteaseRTData(ppt2);
	}
	return pvWebRTData;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// 要获取最新行情，访问数据接口：http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
// 使用json解析，已经没有错误数据了。(偶尔还会有，大致每分钟出现一次）。
//
//////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithNlohmannJSon(CWebDataPtr pData) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();

	static int i = 0;
	// 截取实时数据时用。为了测试解析速度
	if (i < pData->GetBufferLength()) {
		//SaveToFile(_T("C:\\StockAnalysis\\NeteaseRTData.json"), pData->GetDataBuffer());
		i = pData->GetBufferLength();
	}
	bool fProcess = true;
	if (!pData->IsParsed()) {
		if (!pData->CreateNlohmannJSon(21, 2)) {
			// 网易数据前21位为前缀，后两位为后缀
			fProcess = false;
		}
	}
	if (fProcess && pData->IsParsed()) {
		json* pjs = pData->GetJSon();
		pvWebRTData = ParseNeteaseRTData(pjs);
	}
	return pvWebRTData;
}

// 将PTree中提取的utf-8字符串转化为CString
CString XferToCString(string s) {
	CString strName3;
	wstring wsName;
	CStringW strWName;

	wsName = to_wide_string(s); // 将中文utf8转成宽字节字符串
	strWName = wsName.c_str(); // 将标准库的宽字节字符串转换成CStringW制式，
	strName3 = strWName; // 将CStringW制式转换成CString
	return strName3;
}
