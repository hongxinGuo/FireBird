#include"pch.h"
#include"globedef.h"

#include"ChinaStockCodeConverter.h"

#include"jsonParse.h"
#include"WebRTData.h"

#include "ChinaMarket.h"
#include "TimeConvert.h"

void CWebRTData::Reset() {
	m_lDataSource = INVALID_RT_WEB_DATA_;
	m_tpTime = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::from_time_t(0));
	m_strSymbol = _T("");
	m_strStockName = _T("");
	m_lLastClose = 0;
	m_lOpen = 0;
	m_lHigh = 0;
	m_lLow = 0;
	m_lNew = 0;
	m_llVolume = 0;
	m_llAmount = 0;
	m_llCurrentValue = m_llTotalValue = 0;
	m_lBuy = 0;
	m_lSell = 0;
	m_lHighLimitFromTengxun = 0;
	m_lLowLimitFromTengxun = 0;
	for (int i = 0; i < 5; i++) {
		m_lPBuy.at(i) = 0;
		m_lVBuy.at(i) = 0;
		m_lPSell.at(i) = 0;
		m_lVSell.at(i) = 0;
	}
	m_fActive = false;
}

CWebRTData::CWebRTData() {
	Reset();
}

bool CWebRTData::CheckSinaRTDataActive() {
	if (IsValidTime(14)) m_fActive = true;
	else m_fActive = false;

	return m_fActive;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 出现exception时，由上级调用函数处理
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
// 30："2008-01-11″，日期；（此日期为当地市场的日期）
// 31："15:05:32″，时间；（此时间为当地市场的时间，此处为东八区北京标准时间）
// 32：”00”，  不明数据
//
// 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebRTData::ParseSinaData(const string_view& svData) {
	ASSERT(svData.length() >= 23);
	size_t lCurrentPos = 11; // 跨过字符串："var hq_str_"
	const string_view svStockSymbol(svData.data() + lCurrentPos, 8);
	m_strSymbol = XferSinaToStandard(svStockSymbol).c_str();
	if (svData.length() == 23) { // 空数据: var hq_str_sh688801="";,包括最后的';'分号
		m_fActive = false;
		SetDataSource(SINA_RT_WEB_DATA_);
		return;
	}
	lCurrentPos += 10; // 跨过字符串： sh601006="
	// 读入证券名称
	string_view sv = GetNextField(svData, lCurrentPos, ',');
	m_strStockName.Append(sv.data(), sv.length());
	// 读入开盘价。放大一千倍后存储为长整型。其他价格亦如此。
	sv = GetNextField(svData, lCurrentPos, ',');
	m_lOpen = StrToDecimal(sv, 3);
	// 读入前收盘价
	sv = GetNextField(svData, lCurrentPos, ',');
	m_lLastClose = StrToDecimal(sv, 3);
	// 读入当前价
	sv = GetNextField(svData, lCurrentPos, ',');
	m_lNew = StrToDecimal(sv, 3);
	// 读入最高价
	sv = GetNextField(svData, lCurrentPos, ',');
	m_lHigh = StrToDecimal(sv, 3);
	// 读入最低价
	sv = GetNextField(svData, lCurrentPos, ',');
	m_lLow = StrToDecimal(sv, 3);
	// 读入竞买价
	sv = GetNextField(svData, lCurrentPos, ',');
	m_lBuy = StrToDecimal(sv, 3);
	// 读入竞卖价
	sv = GetNextField(svData, lCurrentPos, ',');
	m_lSell = StrToDecimal(sv, 3);
	// 读入成交股数。成交股数存储实际值
	sv = GetNextField(svData, lCurrentPos, ',');
	m_llVolume = atoll(sv.data());
	// 读入成交金额
	sv = GetNextField(svData, lCurrentPos, ',');
	m_llAmount = atoll(sv.data());
	// 读入买一--买五的股数和价格
	for (int j = 0; j < 5; j++) {
		// 读入数量
		sv = GetNextField(svData, lCurrentPos, ',');
		m_lVBuy.at(j) = atol(sv.data());
		// 读入价格
		sv = GetNextField(svData, lCurrentPos, ',');
		m_lPBuy.at(j) = StrToDecimal(sv, 3);
	}
	// 读入卖一--卖五的股数和价格
	for (int j = 0; j < 5; j++) {
		// 读入数量
		sv = GetNextField(svData, lCurrentPos, ',');
		m_lVSell.at(j) = atol(sv.data());
		// 读入价格
		sv = GetNextField(svData, lCurrentPos, ',');
		m_lPSell.at(j) = StrToDecimal(sv, 3);
	}
	// 读入成交日期和时间。此时间为东八区（北京标准时间）。

	sv = GetNextField(svData, lCurrentPos, ',');
	string sTime(sv.data(), sv.size());
	sTime += ' '; //添加一个空格，以利于下面的转换
	const string_view svTime = GetNextField(svData, lCurrentPos, ',');
	sTime.append(svTime.data(), svTime.size());
	// Note 此处不能调用chrono::from_stream(), 否则当使用并行处理以加速时，堵塞在此函数调用上。估计是此函数调用不可重入
	auto time = ConvertBufferToTime("%04d-%02d-%02d %02d:%02d:%02d", sTime.c_str(), -gl_pChinaMarket->GetTimeZoneOffset().count());	//转成UTC时间。
	m_tpTime = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::from_time_t(time));
	// 后面的数据为字符串"00",无效数据，不再处理
	// 判断此实时数据是否有效，可以在此判断，结果就是今日有效股票数会减少（退市的股票有数据，但其值皆为零，而生成今日活动股票池时需要实时数据是有效的）。
	// 在系统准备完毕前就判断新浪活跃股票数，只使用成交时间一项，故而依然存在非活跃股票在其中。
	// 0.07版后，采用十四天内的实时数据为活跃股票数据（最长的春节放假七天，加上前后的休息日，共十天，宽限四天）
	CheckSinaRTDataActive();
	SetDataSource(SINA_RT_WEB_DATA_);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
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
// 20230627日结构：
// v_sh600026="1~中远海能~600026~12.25~12.40~12.34~366190~150067~216123~
//						 12.24~36~12.23~803~12.22~501~12.21~905~12.20~1899~12.25~1026~12.26~1158~12.27~645~12.28~615~12.29~139~~
//						 20230627155915~-0.15~-1.21~12.44~11.95~12.25/366190/445074382~366190~44507~1.05~23.12~~
//						 12.44~11.95~3.95~425.66~584.42~1.79~13.64~11.16~1.01~561~12.15~13.33~40.10~~~1.53~44507.4382~0.0000~0~~GP-A~1.66~-8.85~0.00~
//						 7.73~4.16~21.04~8.58~1.16~7.64~-11.81~3474776395~4770776395~7.26~- 34.49~3474776395~~~17.22~0.16~~CNY~0~___D__F__N";\n
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
// 腾讯实时数据中，成交量的单位为手，无法达到计算所需的精度（股），故而只能作为数据补充之用。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CWebRTData::ParseTengxunData(const string_view& svData) {
	long lTemp;
	float fTemp = 0.0;
	CString strTengxunStockCode;
	size_t lCurrentPos = 12;

	m_fActive = false; // 初始状态为无效数据
	// 市场标识代码（51为深市，1为沪市）
	string_view sv = GetNextField(svData, lCurrentPos, '~'); //
	const long lMarket = atol(sv.data());

	sv = GetNextField(svData, lCurrentPos, '~'); //
	m_strStockName.Append(sv.data(), sv.size()); // 设置股票名称

	// 六位股票代码
	sv = GetNextField(svData, lCurrentPos, '~'); //
	m_strSymbol.Append(sv.data(), sv.size());
	switch (lMarket) {
	case 1: // 上海市场
		m_strSymbol += _T(".SS");
		break;
	case 51: // 深圳市场
		m_strSymbol += _T(".SZ");
		break;
	default:
		string s = _T("bad market: ");
		s.append(svData.data(), svData.length());
		throw exception(s.c_str());
	}
	// 现在成交价。放大一千倍后存储为长整型。其他价格亦如此。
	sv = GetNextField(svData, lCurrentPos, '~'); //
	m_lNew = StrToDecimal(sv, 3);
	// 前收盘价
	sv = GetNextField(svData, lCurrentPos, '~'); //
	m_lLastClose = StrToDecimal(sv, 3);
	// 开盘价
	sv = GetNextField(svData, lCurrentPos, '~'); //
	m_lOpen = StrToDecimal(sv, 3);
	// 成交手数。成交股数存储实际值
	// 不使用此处的成交量，而是使用第三十五项处的成交量。
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 外盘
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 内盘
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 读入买一至买五的价格和手数
	for (int j = 0; j < 5; j++) {
		// 买盘价格
		sv = GetNextField(svData, lCurrentPos, '~'); //
		m_lPBuy.at(j) = StrToDecimal(sv, 3);
		// 买盘数量（手）
		sv = GetNextField(svData, lCurrentPos, '~'); //
		lTemp = atol(sv.data());
		m_lVBuy.at(j) = lTemp * 100;
	}
	// 读入卖一至卖五的价格和手数
	for (int j = 0; j < 5; j++) {
		//读入卖盘价格
		sv = GetNextField(svData, lCurrentPos, '~'); //
		m_lPSell.at(j) = StrToDecimal(sv, 3);
		// 卖盘数量（手）
		sv = GetNextField(svData, lCurrentPos, '~'); //
		lTemp = atol(sv.data());
		m_lVSell.at(j) = lTemp * 100;
	}
	// 最近逐笔成交
	sv = GetNextField(svData, lCurrentPos, '~'); //
	lTemp = atol(sv.data());
	// 30 成交日期和时间.格式为：yyyymmddhhmmss. 此时间采用的时区为东八区（北京标准时间）
	// Note 此处不能调用chrono::from_stream(), 否则当使用并行处理以加速时，堵塞在此函数调用上。估计是此函数调用不可重入
	sv = GetNextField(svData, lCurrentPos, '~'); //
	const string sTime(sv.data(), sv.size());
	auto time = ConvertBufferToTime("%04d%02d%02d%02d%02d%02d", sTime.c_str(), -gl_pChinaMarket->GetTimeZoneOffset().count()); // 转成UTC时间。腾讯实时数据的时区为东八区
	m_tpTime = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::from_time_t(time));

	// 涨跌
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 涨跌率
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 最高价
	sv = GetNextField(svData, lCurrentPos, '~'); //
	m_lHigh = StrToDecimal(sv, 3);
	// 最低价
	sv = GetNextField(svData, lCurrentPos, '~'); //
	m_lLow = StrToDecimal(sv, 3);
	// 35 成交价/成交量（手）/成交金额（元）
	// 成交量和成交金额使用此处的数据，这样就可以使用腾讯实时数据了
	sv = GetNextField(svData, lCurrentPos, '~'); //
	CString str(sv.data(), sv.size());
	sscanf_s(str.GetBuffer(), _T("%f/%d/%I64d"), &fTemp, &lTemp, &m_llAmount);
	m_llVolume = lTemp * 100; // 腾讯成交量数据单位为手（100股）。
	// 成交手数
	// 不使用此处的成交量。这里的成交量会大于第三十五处的成交量。
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 成交金额（万元）
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 换手率
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 市盈率
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 40 无名
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 最高价
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 最低价
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 振幅
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 流通市值（单位为：亿元）
	sv = GetNextField(svData, lCurrentPos, '~'); //
	INT64 lTemp2 = StrToDecimal(sv, 3);
	m_llCurrentValue = lTemp2 * 100000; // 这里需要两次乘以100000
	// 总市值（单位为：亿元）
	sv = GetNextField(svData, lCurrentPos, '~'); //
	lTemp2 = StrToDecimal(sv, 3);
	m_llTotalValue = lTemp2 * 100000; // 这里需要两次乘以100000
	// 市净率
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 涨停价
	sv = GetNextField(svData, lCurrentPos, '~'); //
	m_lHighLimitFromTengxun = StrToDecimal(sv, 3);
	// 48 跌停价
	sv = GetNextField(svData, lCurrentPos, '~'); //
	m_lLowLimitFromTengxun = StrToDecimal(sv, 3);

	// 后面的数据具体内容不清楚，暂时放弃解码。
	CheckTengxunRTDataActive();
	SetDataSource(TENGXUN_RT_WEB_DATA_);
}

bool CWebRTData::CheckTengxunRTDataActive() {
	if (!IsValidTime(14)) {// 如果交易时间在14天前
		m_fActive = false;
	}
	else if ((m_lOpen == 0) && (m_llVolume == 0) && (m_lHigh == 0) && (m_lLow == 0)) {// 腾讯非活跃股票的m_lNew不为零，故而不能使用其作为判断依据
		m_fActive = false; // 腾讯非活跃股票的实时数据也具有所有的字段，故而在此确认其为非活跃
	}
	else m_fActive = true;

	return m_fActive;
}

bool CWebRTData::CheckNeteaseRTDataActive() {
	m_fActive = false;
	if (!IsValidTime(14)) {	// 非活跃股票的update时间为0，转换为time_t时为-1.
		return m_fActive;
	}
	if ((m_lOpen == 0) || (m_lNew == 0)) {// 网易非活跃股票的实时数据也具有所有的字段，故而在此确认其为非活跃
		return m_fActive;
	}
	m_fActive = true;

	return m_fActive;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 实时数据的有效时间范围为最近两周内。当股市放假时，其最新数据是放假前的最后一天数据。春节放假时间最长，有十一天时间，加上三天富裕，
// 故而十四天内的数据都被认为是有效时间数据，这样能够保证生成当日活动股票集。
//
//////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::IsValidTime(long lDays) const {
	if (GetTransactionTime() < (GetUTCTime() - lDays * 24 * 3600)) {// 确保实时数据不早于当前时间的14天前（春节放假最长为7天，加上前后的休息日，共十一天）
		return false;
	}
	if (m_tpTime > gl_tpNow) {
		return false;
	}
	return true;
}
