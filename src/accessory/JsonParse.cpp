//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 数据解析的任务皆位于此文件中。
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "JsonParse.h"

#include "CharSetTransfer.h"
#include "ChinaMarket.h"
#include "DayLineWebData.h"

#include"WebRTData.h"

#include "ChinaStockCodeConverter.h"
#include "InfoReport.h"
#include "Thread.h"

#include"simdjsonGetValue.h"

#include"SystemMessage.h"

#include"NlohmannJsonDeclaration.h"
#include "ContainerChinaStock.h"
#include "SystemData.h"

#include"concurrencpp/concurrencpp.h"
using namespace concurrencpp;

#include<spanstream>

using std::istringstream;
using std::make_shared;
using std::min;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 将输入的字符串转变成放大了10^power倍的长整型。要确保精确地转换，不使用浮点数过渡。
// 这是coPilot提供的范本，执行时间要慢20%，但更清晰。
//
///////////////////////////////////////////////////////////////////////////////////////////////////

long long StrToDecimal(const std::string_view& svData, const size_t power) {
	std::string buffer;
	buffer.reserve(svData.size() + power + 2);

	try {
		auto iPointPosition = svData.find('.');
		if (iPointPosition == std::string_view::npos) {
			buffer.append(svData);
			buffer.append(power, '0');
			return std::stoll(buffer);
		}
		buffer.append(svData.substr(0, iPointPosition));
		auto fraction = svData.substr(iPointPosition + 1);
		if (fraction.size() > power) fraction = fraction.substr(0, power);
		buffer.append(fraction);
		buffer.append(power > fraction.size() ? power - fraction.size() : 0, '0');
		return std::stoll(buffer);
	} catch (std::out_of_range&) {
		return 0;
	} catch (std::invalid_argument&) {
		return 0;
	}
}

long long StrToDecimal2(const std::string_view& svData, const size_t power) {
	try {
		auto iPointPosition = svData.find('.');
		if (iPointPosition == std::string_view::npos) {
			std::string result(svData);
			result.append(power, '0');
			return std::stoll(result);
		}
		std::string result(svData.substr(0, iPointPosition));
		auto fraction = svData.substr(iPointPosition + 1);
		if (fraction.size() > power) {
			fraction = fraction.substr(0, power);
		}
		result.append(fraction);
		if (power > fraction.size()) {
			result.append(power - fraction.size(), '0');
		}
		return std::stoll(result);
	} catch (std::out_of_range&) {
		return 0;
	} catch (std::invalid_argument&) {
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 将输入的字符串转变成放大了10^power倍的长整型。要确保精确地转换，不使用浮点数过渡。
// 这是coPilot提供的范本，执行时间要慢20%，但更清晰。
//
///////////////////////////////////////////////////////////////////////////////////////////////////
long Str2Long(const std::string_view& svData, const size_t power) {
	std::string buffer;
	buffer.reserve(svData.size() + power + 2);

	try {
		auto iPointPosition = svData.find('.');
		if (iPointPosition == std::string_view::npos) {
			buffer.append(svData);
			buffer.append(power, '0');
			return std::stoll(buffer);
		}
		buffer.append(svData.substr(0, iPointPosition));
		auto fraction = svData.substr(iPointPosition + 1);
		if (fraction.size() > power) fraction = fraction.substr(0, power);
		buffer.append(fraction);
		buffer.append(power > fraction.size() ? power - fraction.size() : 0, '0');
		return std::stol(buffer);
	} catch (std::out_of_range&) {
		return 0;
	} catch (std::invalid_argument&) {
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 出现exception时，上级调用函数负责处理
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
string_view GetNextField(const string_view& svData, size_t& lCurrentPos, const char delimiter) {
	const string_view sv = svData.substr(lCurrentPos);
	const auto lEnd = sv.find_first_of(delimiter);
	if (lEnd > sv.length()) throw exception("GetNextField() out of range"); // 没找到的话抛出异常
	lCurrentPos += lEnd + 1; // 将当前位置移至本数据之后
	return string_view{ sv.data(), lEnd };
}

void ReportJSonErrorToSystemMessage(const string& strPrefix, const string& strWhat) {
	string s = strPrefix;
	s += strWhat;
	gl_systemMessage.PushErrorMessage(s);
}

void ReportJSonErrorToSystemMessage(const string& strPrefix, const string& strWhat, const char* jsonData) {
	string s(jsonData);
	s = s.substr(0, 40);
	string str = strWhat;
	str += " ";
	str += s;

	ReportJSonErrorToSystemMessage(strPrefix, str);
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
// Note: 新浪实时数据的字符集为GBK18030，需要转换为UTF-8。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWebRTDataPtr ParseSinaOneData(const string_view& svData) {
	CWebRTDataPtr pData = make_shared<CWebRTData>();
	ABSL_DCHECK(svData.length() >= 23);
	size_t lCurrentPos = 11; // 跨过字符串："var hq_str_"
	const string_view svStockSymbol(svData.data() + lCurrentPos, 8);
	pData->SetSymbol(XferSinaToStandard(svStockSymbol));
	if (svData.length() == 23) { // 空数据: var hq_str_sh688801="";,包括最后的';'
		pData->SetActive(false);
		pData->SetDataSource(SINA_RT_WEB_DATA_);
		return pData;
	}
	lCurrentPos += 10; // 跨过字符串： sh601006="
	// 读入证券名称
	auto sv = GetNextField(svData, lCurrentPos, ',');
	string s(sv.data(), sv.length());
	pData->SetStockName(Gbk2Utf8(s)); //Note 新浪实时数据的字符集为GBK18030，需要转换为UTF-8。
	// 读入开盘价。放大一千倍后存储为长整型。其他价格亦如此。
	sv = GetNextField(svData, lCurrentPos, ',');
	pData->SetOpen(Str2Long(sv, 3));
	// 读入前收盘价
	sv = GetNextField(svData, lCurrentPos, ',');
	pData->SetLastClose(Str2Long(sv, 3));
	// 读入当前价
	sv = GetNextField(svData, lCurrentPos, ',');
	pData->SetNew(Str2Long(sv, 3));
	// 读入最高价
	sv = GetNextField(svData, lCurrentPos, ',');
	pData->SetHigh(Str2Long(sv, 3));
	// 读入最低价
	sv = GetNextField(svData, lCurrentPos, ',');
	pData->SetLow(Str2Long(sv, 3));
	// 读入竞买价
	sv = GetNextField(svData, lCurrentPos, ',');
	pData->SetBuy(Str2Long(sv, 3));
	// 读入竞卖价
	sv = GetNextField(svData, lCurrentPos, ',');
	pData->SetSell(Str2Long(sv, 3));
	// 读入成交股数。成交股数存储实际值
	sv = GetNextField(svData, lCurrentPos, ',');
	pData->SetVolume(atoll(sv.data()));
	// 读入成交金额
	sv = GetNextField(svData, lCurrentPos, ',');
	pData->SetAmount(atoll(sv.data()));
	// 读入买一--买五的股数和价格
	for (int j = 0; j < 5; j++) {
		// 读入数量
		sv = GetNextField(svData, lCurrentPos, ',');
		pData->SetVBuy(j, atol(sv.data()));
		// 读入价格
		sv = GetNextField(svData, lCurrentPos, ',');
		pData->SetPBuy(j, Str2Long(sv, 3));
	}
	// 读入卖一--卖五的股数和价格
	for (int j = 0; j < 5; j++) {
		// 读入数量
		sv = GetNextField(svData, lCurrentPos, ',');
		pData->SetVSell(j, atol(sv.data()));
		// 读入价格
		sv = GetNextField(svData, lCurrentPos, ',');
		pData->SetPSell(j, Str2Long(sv, 3));
	}
	// 读入成交日期和时间。此时间为东八区（北京标准时间）。

	sv = GetNextField(svData, lCurrentPos, ',');
	string_view sv2 = GetNextField(svData, lCurrentPos, ',');
	std::stringstream oss;
	oss << sv << ' ' << sv2;
	std::chrono::local_seconds lt;
	oss >> std::chrono::parse("%Y-%m-%d %H:%M:%S", lt);
	pData->SetTime(gl_pChinaMarket->ToUTCTime(lt));
	/*
	string sTime(sv.data(), sv.size());
	sTime += ' '; //添加一个空格，以利于下面的转换
	const string_view svTime = GetNextField(svData, lCurrentPos, ',');
	sTime.append(svTime.data(), svTime.size());
	// Note 此处不能调用chrono::from_stream(), 否则当使用并行处理以加速时，堵塞在此函数调用上。估计是此函数调用不可重入
	auto time = ConvertBufferToTime("%04d-%02d-%02d %02d:%02d:%02d", sTime.c_str(), -gl_pChinaMarket->GetTimeZoneOffset().count());	//转成UTC时间。
	m_tpTime = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::from_time_t(time));
	*/
	// 后面的数据为字符串"00",无效数据，不再处理
	// 判断此实时数据是否有效，可以在此判断，结果就是今日有效股票数会减少（退市的股票有数据，但其值皆为零，而生成今日活动股票池时需要实时数据是有效的）。
	// 在系统准备完毕前就判断新浪活跃股票数，只使用成交时间一项，故而依然存在非活跃股票在其中。
	// 0.07版后，采用十四天内的实时数据为活跃股票数据（最长的春节放假七天，加上前后的休息日，共十天，宽限四天）
	pData->CheckSinaRTDataActive();
	pData->SetDataSource(SINA_RT_WEB_DATA_);

	return pData;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 解析新浪实时数据
//
// 使用thread pool + coroutine并行解析，每个工作线程解析1/gl_concurrency_level数的数据，将解析后的数据存入缓存队列。
// 由于数据中不会包含相同股票的实时数据，故而不会出现同时操作同一个股票的问题，所以可以并行解析
// 只有工作线程都执行完后，本函数方可退出。
//
// 使用这种多线程模式与单线程模式相比，4个线程时运行时间减至1/3，更多的线程会减少运行时间，但效率降低。
//
// Note 调用此函数得线程不能使用thread_pool_executor或者background_executor生成，只能使用thread_executor生成，原因待查。
// Note 测试时，发现8个线程时效率反而下降，原因待查（CPU有8个物理核，16个逻辑核）。
//
//////////////////////////////////////////////////////////////////////////////////////////////////
result<bool> ParseSinaRTDataUsingCoroutine(shared_ptr<vector<string_view>> pvStringView) {
	vector<result<bool>> results;
	long DataSize = pvStringView->size();
	const auto chunk_size = 1 + std::div(DataSize, gl_concurrency_level).quot;
	//const auto chunk_size = 1 + DataSize / gl_concurrency_level;
	//for (long i = 0; i < gl_concurrency_level; i++) {
	for (auto i : std::views::iota(0, gl_concurrency_level)) {
		auto chunk_begin = i * chunk_size;
		auto chunk_end = chunk_begin + chunk_size;
		chunk_end = min(chunk_end, DataSize);
		auto result = gl_runtime.thread_pool_executor()->submit([pvStringView, chunk_begin, chunk_end] {
			for (auto j = chunk_begin; j < chunk_end; j++) {
				gl_qChinaMarketRTData.enqueue(ParseSinaOneData(pvStringView->at(j))); // Note: 多个协程并行往里存时，无法通过size_approx()函数得到队列数量。
			}
			return true;
		});
		results.emplace_back(std::move(result));
	}

	bool succeed = true;
	for (auto& r : results) {
		succeed &= co_await r; // r.get(); wait for each task to complete
	}
	//bool succeed = ranges::all_of(results, [](auto& r) { return  r.get(); });
	co_return succeed;
}

static string_view GetCurrentSinaRTData(const string& text, size_t& currentPos) {
	const string_view svCurrentTotal = string_view(text).substr(currentPos);
	const auto lStart = svCurrentTotal.find_first_of('v');
	const auto lEnd = svCurrentTotal.find_first_of(';');
	ABSL_DCHECK(lStart <= svCurrentTotal.length());
	ABSL_DCHECK(lEnd <= svCurrentTotal.length());
	ABSL_DCHECK(lStart <= lEnd);
	currentPos += lEnd + 1; // 将当前位置移至当前数据结束处之后
	return svCurrentTotal.substr(lStart, lEnd - lStart + 1); // 包括最后的字符';'
}

void ParseSinaRTData(const string& text) {
	size_t currentPos = 0;
	const shared_ptr<vector<string_view>> pvStringView = make_shared<vector<string_view>>();
	while (currentPos < text.size() - 2) {
		pvStringView->emplace_back(GetCurrentSinaRTData(text, currentPos));
	}
	if (pvStringView->empty()) return;
	ParseSinaRTDataUsingCoroutine(pvStringView).get();// 在这里堵塞
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
CWebRTDataPtr ParseOneTengxunData(const string_view& svData) {
	CWebRTDataPtr pData = make_shared<CWebRTData>();
	long lTemp;
	float fTemp = 0.0;
	size_t lCurrentPos = 12;
	string stockName;
	string symbol;
	pData->SetActive(false); // 初始状态为无效数据
	// 市场标识代码（51为深市，1为沪市）
	string_view sv = GetNextField(svData, lCurrentPos, '~'); //
	const long lMarket = atol(sv.data());

	sv = GetNextField(svData, lCurrentPos, '~'); //
	stockName.append(sv.data(), sv.size()); // 设置股票名称
	pData->SetStockName(stockName);

	// 六位股票代码
	sv = GetNextField(svData, lCurrentPos, '~'); //
	symbol.append(sv.data(), sv.size());
	switch (lMarket) {
	case 1: // 上海市场
		symbol += ".SS";
		break;
	case 51: // 深圳市场
		symbol += ".SZ";
		break;
	default:
		string s = "bad market: ";
		s.append(svData.data(), svData.length());
		throw exception(s.c_str());
	}
	pData->SetSymbol(symbol);
	// 现在成交价。放大一千倍后存储为长整型。其他价格亦如此。
	sv = GetNextField(svData, lCurrentPos, '~'); //
	pData->SetNew(Str2Long(sv, 3));
	// 前收盘价
	sv = GetNextField(svData, lCurrentPos, '~'); //
	pData->SetLastClose(Str2Long(sv, 3));
	// 开盘价
	sv = GetNextField(svData, lCurrentPos, '~'); //
	pData->SetOpen(Str2Long(sv, 3));
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
		pData->SetPBuy(j, Str2Long(sv, 3));
		// 买盘数量（手）
		sv = GetNextField(svData, lCurrentPos, '~'); //
		lTemp = atol(sv.data());
		pData->SetVBuy(j, lTemp * 100);
	}
	// 读入卖一至卖五的价格和手数
	for (int j = 0; j < 5; j++) {
		//读入卖盘价格
		sv = GetNextField(svData, lCurrentPos, '~'); //
		pData->SetPSell(j, Str2Long(sv, 3));
		// 卖盘数量（手）
		sv = GetNextField(svData, lCurrentPos, '~'); //
		lTemp = atol(sv.data());
		pData->SetVSell(j, lTemp * 100);
	}
	// 最近逐笔成交
	sv = GetNextField(svData, lCurrentPos, '~'); //
	lTemp = atol(sv.data());
	// 30 成交日期和时间.格式为：yyyymmddhhmmss. 此时间采用的时区为东八区（北京标准时间）
	// Note 此处不能调用chrono::from_stream(), 否则当使用并行处理以加速时，堵塞在此函数调用上。估计是此函数调用不可重入
	sv = GetNextField(svData, lCurrentPos, '~'); //
	std::ispanstream ss(sv);
	std::chrono::local_seconds lt;
	ss >> std::chrono::parse("%Y%m%d%H%M%S", lt);
	pData->SetTime(gl_pChinaMarket->ToUTCTime(lt));

	// 涨跌
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 涨跌率
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 最高价
	sv = GetNextField(svData, lCurrentPos, '~'); //
	pData->SetHigh(Str2Long(sv, 3));
	// 最低价
	sv = GetNextField(svData, lCurrentPos, '~'); //
	pData->SetLow(Str2Long(sv, 3));
	// 35 成交价/成交量（手）/成交金额（元）
	// 成交量和成交金额使用此处的数据，这样就可以使用腾讯实时数据了
	sv = GetNextField(svData, lCurrentPos, '~'); //
	string str(sv.data(), sv.size());
	int64_t amount;
	sscanf_s(str.c_str(), "%f/%d/%I64d", &fTemp, &lTemp, &amount);
	pData->SetVolume(static_cast<int64_t>(lTemp) * 100); // 腾讯成交量数据单位为手（100股）。
	pData->SetAmount(amount);
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
	int64_t lTemp2 = StrToDecimal(sv, 3);
	pData->SetCurrentValue(lTemp2 * 100000); // 这里需要两次乘以100000
	// 总市值（单位为：亿元）
	sv = GetNextField(svData, lCurrentPos, '~'); //
	lTemp2 = StrToDecimal(sv, 3);
	pData->SetTotalValue(lTemp2 * 100000); // 这里需要两次乘以100000
	// 市净率
	sv = GetNextField(svData, lCurrentPos, '~'); //
	// 涨停价
	sv = GetNextField(svData, lCurrentPos, '~'); //
	pData->SetHighLimitFromTengxun(Str2Long(sv, 3));
	// 48 跌停价
	sv = GetNextField(svData, lCurrentPos, '~'); //
	pData->SetLowLimitFromTengxun(Str2Long(sv, 3));

	// 后面的数据具体内容不清楚，暂时放弃解码。
	pData->CheckTengxunRTDataActive();
	pData->SetDataSource(TENGXUN_RT_WEB_DATA_);
	return pData;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 解析腾讯实时数据
//
// 使用thread pool + coroutine并行解析，每个工作线程解析1/gl_concurrency_level数的数据，将解析后的数据存入缓存队列。
// 由于数据中不会包含相同股票的实时数据，故而不会出现同时操作同一个股票的问题，所以可以并行解析
//
// 使用这种多线程模式与单线程模式相比，4个线程时运行时间减至1/3，更多的线程会减少运行时间，但效率降低。
//
// Note 调用此函数得线程不能使用thread_pool_executor或者background_executor生成，只能使用thread_executor生成，原因待查。
//
//////////////////////////////////////////////////////////////////////////////////////////////////
concurrencpp::result<bool> ParseTengxunRTDataUsingCoroutine(shared_ptr<concurrencpp::thread_pool_executor> tpe, shared_ptr<vector<string_view>> pvStringView) {
	bool succeed = true;
	vector<concurrencpp::result<bool>> results;
	const long DataSize = pvStringView->size();
	const auto chunk_size = 1 + std::div(DataSize, gl_concurrency_level).quot;
	//const auto chunk_size = 1 + DataSize / gl_concurrency_level;
	//for (long i = 0; i < gl_concurrency_level; i++) {
	for (auto i : std::views::iota(0, gl_concurrency_level)) {
		auto chunk_begin = i * chunk_size;
		auto chunk_end = chunk_begin + chunk_size;
		chunk_end = min(chunk_end, DataSize);
		auto result = tpe->submit([pvStringView, chunk_begin, chunk_end] {
			try {
				for (auto j = chunk_begin; j < chunk_end; j++) {
					const string_view sv = pvStringView->at(j);
					auto pRTData = ParseOneTengxunData(sv);
					gl_qChinaMarketRTData.enqueue(pRTData); // Note 多个协程并行往里存时，无法通过size_approx()函数得到队列数量。
				}
			} catch (exception& e) {
				ReportErrorToSystemMessage("ParseSinaData异常 ", e);
			}
			return true;
		});
		results.emplace_back(std::move(result));
	}
	for (auto& r : results) {
		succeed = succeed & co_await r;// r.get();
	}
	co_return succeed;
}

static string_view GetCurrentTengxunRTData(const string& text, size_t& currentPos) {
	//const string_view svCurrentTotal = string_view(m_sDataBuffer.c_str() + m_lCurrentPos, m_sDataBuffer.size() - m_lCurrentPos);
	const string_view svCurrentTotal = string_view(text).substr(currentPos);
	const auto lStart = svCurrentTotal.find_first_of('v');
	const auto lEnd = svCurrentTotal.find_first_of(';');
	ABSL_DCHECK(lStart <= svCurrentTotal.length());
	ABSL_DCHECK(lEnd <= svCurrentTotal.length());
	ABSL_DCHECK(lStart <= lEnd);
	currentPos += lEnd + 1; // 将当前位置移至当前数据结束处之后
	return svCurrentTotal.substr(lStart, lEnd - lStart + 1);
}

void ParseTengxunRTData(const string& text) {
	size_t currentPos = 0;
	shared_ptr<vector<string_view>> pvStringView = make_shared<vector<string_view>>();
	while (currentPos < text.size() - 2) {
		pvStringView->emplace_back(GetCurrentTengxunRTData(text, currentPos));
	}
	if (pvStringView->empty()) return;
	ParseTengxunRTDataUsingCoroutine(gl_runtime.thread_pool_executor(), pvStringView).get(); // 等待线程执行完后方继续。
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 腾讯日线数据结构：
// {
// "code":0,
// "msg":"",
// "data": { "sh600601": { "day":	[
//																	["2023-01-19","2.550","2.600","2.610","2.550","86162.000"],
//																	["2023-01-20","2.600","2.620","2.620","2.590","100735.000"]
//																],
//													"qt":{},
//													"mx_price":{"mx":[],"price":[]},
//													"prec":"2.560",
//													"version":"16"
//												}
//					}
// }
//
// 使用simdjson解析速度release模式下比Nholmann json快50%，但debug模式下慢一倍。
//
////////////////////////////////////////////////////////////////////////////////////////////////
CDayLinesPtr ParseTengxunDayLineImp(const string& text, const string& strStockCode) {
	auto pvDayLine = make_shared<vector<CDayLine>>();
	pvDayLine->reserve(2000);

	const string strStockSymbol = XferTengxunToStandard(strStockCode);
	try {
		string_view sv;
		long lLastClose = 0;
		ondemand::parser parser;
		ondemand::document doc;

		//const padded_string_view jsonPaddedView(svData, svData.length()); // Note 此时的svData带有长度为SIMDJSON_PADDING长度的后缀
		//doc = parser.iterate(jsonPaddedView).value();
		const padded_string jsonPadded(text);
		doc = parser.iterate(jsonPadded).value();
		auto a1 = doc["data"];
		auto a2 = a1[strStockCode];
		auto a3 = a2["day"];
		auto a4 = a3.get_array();
		auto dayArray = a4.value();
		//ondemand::array dayArray = doc["data"][strStockCode]["day"].get_array().value(); // 使用索引strStockCode找到日线数组
		// 以下为不使用索引strStockCode找到日线数组的方法
		//ondemand::value data = doc["data"];
		//ondemand::field field = *data.get_object().begin();
		//ondemand::value stock = field.value();
		//ondemand::array dayArray = stock["day"].get_array();
		for (auto dayLine : dayArray) {
			CDayLine dayLine2;
			dayLine2.SetStockSymbol(strStockSymbol);
			dayLine2.SetLastClose(lLastClose);
			ondemand::array_iterator it = dayLine.get_array().begin();
			ondemand::value item = (*it).value();
			sv = simdjsonGetStringView(item);
			string str1(sv.data(), sv.length()); // 这里需要转换一下，直接使用string_view会导致内存溢出
			istringstream ss(str1);
			local_days ld;
			ss >> parse("%F", ld);
			dayLine2.SetDate(ld);
			item = (*++it).value();
			sv = simdjsonGetStringView(item);
			dayLine2.SetOpen(Str2Long(sv, 3));
			item = (*++it).value();
			sv = simdjsonGetStringView(item);
			const long lClose = Str2Long(sv, 3);
			dayLine2.SetClose(lClose);
			lLastClose = lClose;
			item = (*++it).value();
			sv = simdjsonGetStringView(item);
			dayLine2.SetHigh(Str2Long(sv, 3));
			item = (*++it).value();
			sv = simdjsonGetStringView(item);
			dayLine2.SetLow(Str2Long(sv, 3));
			item = (*++it).value();
			sv = simdjsonGetStringView(item);
			dayLine2.SetVolume(atof(sv.data()) * 100);

			pvDayLine->push_back(dayLine2);
		}
	} catch (nlohmannJson::exception&) {
		return pvDayLine;
	}
	return pvDayLine;
}

//
// 腾讯日线数据结构：
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
CDayLineWebDataPtr ParseTengxunDayLine(const string& text, const string& stockSymbol) {
	auto pDayLineData = make_shared<CDayLineWebData>();
	ABSL_DCHECK(gl_dataContainerChinaStock.IsSymbol(stockSymbol));

	const shared_ptr<vector<CDayLine>> pvDayLine = ParseTengxunDayLineImp(text, XferStandardToTengxun(stockSymbol));
	std::ranges::sort(*pvDayLine, [](const CDayLine& pData1, const CDayLine& pData2) { return pData1.GetDate() < pData2.GetDate(); });
	for (auto& dayLine : *pvDayLine) {
		dayLine.SetStockSymbol(stockSymbol);
		pDayLineData->AppendDayLine(dayLine);
	}
	pDayLineData->SetStockCode(stockSymbol);
	return pDayLineData;
}

bool CreateJsonWithNlohmann(nlohmannJson& js, const std::string& s, const size_t lBeginPos, const size_t lEndPos) {
	try {
		js = nlohmannJson::parse(s.begin() + lBeginPos, s.end() - lEndPos);
	} catch (nlohmannJson::parse_error&) {
		js.clear();
		return false;
	}
	return true;
}

bool CreateJsonWithNlohmann(nlohmannJson& js, const std::string_view s, const size_t lBeginPos, const size_t lEndPos) {
	try {
		js = nlohmannJson::parse(s.begin() + lBeginPos, s.end() - lEndPos);
	} catch (nlohmannJson::parse_error&) {
		js.clear();
		return false;
	}
	return true;
}
