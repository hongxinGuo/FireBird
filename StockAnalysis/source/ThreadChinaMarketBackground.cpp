/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 将ChinaMarket中的耗时计算任务移至此处。
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"

#include"ChinaMarket.h"
#include"WorldMarket.h"

#include"thread.h"
#include"WebInquirer.h"

#include"Parse.h"

UINT ThreadChinaMarketBackground(void) {
	gl_ThreadStatus.SetChinaMarketBackground(true);
	while (!gl_fExitingSystem) {
		// 此三个任务比较费时，尤其时网易实时数据解析时需要使用json解析器，故而放在此独立线程中。
		ParseWebRTDataGetFromSinaServer(); // 解析新浪实时数据
		ParseWebRTDataGetFromNeteaseServer(); // 解析网易实时数据
		ParseWebRTDataGetFromTengxunServer(); // 解析腾讯实时数据

		Sleep(100); // 最少间隔100ms
	}
	gl_ThreadStatus.SetChinaMarketBackground(false);
	return 201;
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
bool ParseWebRTDataGetFromSinaServer(void) {
	CWebDataPtr pWebDataReceived = nullptr;
	const size_t lTotalData = gl_WebInquirer.GetSinaRTDataSize();
	INT64 llTotal = 0;
	bool fSucceed = true;
	for (int i = 0; i < lTotalData; i++) {
		pWebDataReceived = gl_WebInquirer.PopSinaRTData();
		pWebDataReceived->ResetCurrentPos();
		while (!pWebDataReceived->IsProcessedAllTheData()) {
			CWebRTDataPtr pRTData = make_shared<CWebRTData>();
			if (pRTData->ReadSinaData(pWebDataReceived)) {
				llTotal++;
				gl_WebRTDataContainer.PushSinaData(pRTData); // 将此实时数据指针存入实时数据队列
			}
			else {
				fSucceed = false;
				break;  // 后面的数据出问题，抛掉不用。
			}
		}
	}
	gl_pChinaMarket->IncreaseRTDataReceived(llTotal);
	return fSucceed;
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
// 使用json解析，已经没有错误数据了。
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool ParseWebRTDataGetFromNeteaseServer(void) {
	CWebDataPtr pWebDataReceived = nullptr;
	const size_t lTotalData = gl_WebInquirer.GetNeteaseRTDataSize();
	string ss;
	ptree pt;
	bool fSucceed = true;
	INT64 llTotal = 0;

	for (int i = 0; i < lTotalData; i++) {
		pWebDataReceived = gl_WebInquirer.PopNeteaseRTData();
		if (pWebDataReceived->CreatePTree(pt, 21, 2)) { // 网易数据前21位为前缀，后两位为后缀
			for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
				CWebRTDataPtr pRTData = make_shared<CWebRTData>();
				pRTData->SetDataSource(__NETEASE_RT_WEB_DATA__);
				if (pRTData->ReadNeteaseData(it)) {
					llTotal++;
					gl_WebRTDataContainer.PushNeteaseData(pRTData); // 将此实时数据指针存入实时数据队列
				}
				else break;
			}
		}
	}
	gl_pChinaMarket->IncreaseRTDataReceived(llTotal);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 当所有被查询的股票皆为非上市股票时，腾讯实时股票服务器会返回一个21个字符长的字符串：v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IsTengxunRTDataInvalid(CWebData& WebDataReceived) {
	char buffer[50]{};
	char* pBuffer = buffer;
	CString strInvalidStock = _T("v_pv_none_match=\"1\";\n"); // 此为无效股票查询到的数据格式，共21个字符

	WebDataReceived.GetData(pBuffer, 21, WebDataReceived.GetCurrentPos());
	buffer[21] = 0x000;
	CString str1 = buffer;

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
// 0: 市场（上海为1，深圳为51）。
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
// 49 ：        之后的这些数字不清楚其含义
//
//
// 腾讯实时数据中，成交量的单位为手，无法达到计算所需的精度（股），故而只能作为数据补充之用。
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool ParseWebRTDataGetFromTengxunServer(void) {
	CWebDataPtr pWebDataReceived = nullptr;
	bool fSucceed = true;

	const size_t lTotalData = gl_WebInquirer.GetTengxunRTDataSize();
	for (int i = 0; i < lTotalData; i++) {
		pWebDataReceived = gl_WebInquirer.PopTengxunRTData();
		pWebDataReceived->ResetCurrentPos();
		if (!IsTengxunRTDataInvalid(*pWebDataReceived)) { // 处理这21个字符串的函数可以放在这里，也可以放在最前面。
			while (!pWebDataReceived->IsProcessedAllTheData()) {
				CWebRTDataPtr pRTData = make_shared<CWebRTData>();
				if (pRTData->ReadTengxunData(pWebDataReceived)) {
					gl_WebRTDataContainer.PushTengxunData(pRTData); // 将此实时数据指针存入实时数据队列
				}
				else {
					fSucceed = false;
					break;// 后面的数据出问题，抛掉不用。
				}
			}
		}
	}

	return fSucceed;
}