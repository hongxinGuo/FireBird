#include"pch.h"
#include "globedef.h"

#include"TimeConvert.h"
#include"ChinaStockCodeConverter.h"
#include"InfoReport.h"

#include"jsonParse.h"
#include"WebRTData.h"

void CWebRTData::Reset() {
	m_lDataSource = INVALID_RT_WEB_DATA_;
	m_time = 0;
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

bool CWebRTData::ReadVoidSinaData(const CWebDataPtr& pSinaWebData) {
	const string_view svCurrentParagraph = pSinaWebData->GetCurrentParagraph();
	string strSinaStockCode{};
	try {
		switch (svCurrentParagraph.at(12)) {
		case 'h':
			strSinaStockCode = _T("sh");// 由于上海深圳股票代码有重叠，故而所有的股票代码都带上市场前缀。上海为sh
			break;
		case 'z':
			strSinaStockCode = _T("sz"); // 由于上海深圳股票代码有重叠，故而所有的股票代码都带上市场前缀。上海为sh
			break;
		default:
			throw exception(_T("void data error"));
		}
		const string_view svStockSymbol = string_view(svCurrentParagraph.data() + 13, 6);
		strSinaStockCode.append(svStockSymbol.data(), svStockSymbol.size());
		m_strSymbol = XferSinaToStandard(strSinaStockCode);

		pSinaWebData->IncreaseCurrentPos(23);
		if (!pSinaWebData->IsLastDataParagraph()) {
			if (pSinaWebData->GetCurrentPosData() != 0x00a) { return false; }
			pSinaWebData->IncreaseCurrentPos();
		}
		m_fActive = false;
		SetDataSource(SINA_RT_WEB_DATA_);
		return true;
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ReadSinaData异常 "), e);
		if (pSinaWebData->OutOfRange()) return false;
		pSinaWebData->IncreaseCurrentPos(23);
		// 最后一个数据时，如果没有换行符，则已到达数据的末尾，就不用测试是否存在换行符了。
		if (!pSinaWebData->IsLastDataParagraph()) { if (pSinaWebData->GetCurrentPosData() == 0x00a) { pSinaWebData->IncreaseCurrentPos(); } }
		return false;
	}
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
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadSinaData(const CWebDataPtr& pSinaWebData) {
	try {
		string strSinaStockCode{};
		double dTemp = 0;
		string_view sv3;
		m_fActive = false; // 初始状态为无效数据
		pSinaWebData->SetCurrentParagraphStartPos(pSinaWebData->GetCurrentPos()); // 本段数据起始位置
		sv3 = pSinaWebData->GetStringView(pSinaWebData->GetCurrentPos(), pSinaWebData->GetBufferLength() - pSinaWebData->GetCurrentPos());
		const long lParagraphLength = sv3.find_first_of(_T(";")); // 每段数据以字符';'作为结束符
		if (lParagraphLength < 0) {
			pSinaWebData->SetCurrentPos(pSinaWebData->GetBufferLength());
			return false;
		}
		pSinaWebData->SetCurrentParagraph(sv3.substr(0, lParagraphLength + 1));
		string_view sv1 = pSinaWebData->GetStringView(0, 12); //默认状态下起始字符串为var hq_str_ 读入“var hq_str_s"
		if (pSinaWebData->GetCurrentParagraph().size() == 23) { // 无效数据
			return ReadVoidSinaData(pSinaWebData);
		}
		if (sv1.compare(_T("var hq_str_s")) != 0) throw exception(_T("Bad header: need var hq_str_s"));	// 数据格式出错
		pSinaWebData->IncreaseCurrentPos(12);

		switch (pSinaWebData->GetCurrentPosData()) {
		case 'h':
			strSinaStockCode = _T("sh");// 由于上海深圳股票代码有重叠，故而所有的股票代码都带上市场前缀。上海为sh
			break;
		case 'z':
			strSinaStockCode = _T("sz"); // 由于上海深圳股票代码有重叠，故而所有的股票代码都带上市场前缀。上海为sh
			break;
		default:
			throw exception(_T("Bad header: need sh or sz"));
		}
		pSinaWebData->IncreaseCurrentPos();
		const string_view svStockSymbol = pSinaWebData->GetStringView(pSinaWebData->GetCurrentPos(), 6);
		strSinaStockCode.append(svStockSymbol.data(), svStockSymbol.size());
		m_strSymbol = XferSinaToStandard(strSinaStockCode);
		pSinaWebData->IncreaseCurrentPos(6);

		sv1 = pSinaWebData->GetStringView(pSinaWebData->GetCurrentPos(), 2); // 读入'="'
		if ((sv1.at(0) != '=') || (sv1.at(1) != '"')) { throw exception(_T("need '=\"'")); }
		pSinaWebData->IncreaseCurrentPos(2);

		ReadSinaOneValue(pSinaWebData, sv1); // 股票名称
		m_strStockName.Append(sv1.data(), sv1.size()); // 设置股票名称

		// 读入开盘价。放大一千倍后存储为长整型。其他价格亦如此。
		if (!ReadSinaOneValue(pSinaWebData, dTemp)) { throw exception(_T("open")); }
		m_lOpen = dTemp * 1000;
		// 读入前收盘价
		if (!ReadSinaOneValue(pSinaWebData, dTemp)) { throw exception(_T("LastClose")); }
		m_lLastClose = dTemp * 1000;
		// 读入当前价
		if (!ReadSinaOneValue(pSinaWebData, dTemp)) { throw exception(_T("New")); }
		m_lNew = dTemp * 1000;
		// 读入最高价
		if (!ReadSinaOneValue(pSinaWebData, dTemp)) { throw exception(_T("High")); }
		m_lHigh = dTemp * 1000;
		// 读入最低价
		if (!ReadSinaOneValue(pSinaWebData, dTemp)) { throw exception(_T("Low")); }
		m_lLow = dTemp * 1000;
		// 读入竞买价
		if (!ReadSinaOneValue(pSinaWebData, dTemp)) { throw exception(_T("Buy")); }
		m_lBuy = dTemp * 1000;
		// 读入竞卖价
		if (!ReadSinaOneValue(pSinaWebData, dTemp)) { throw exception(_T("Sell")); }
		m_lSell = dTemp * 1000;
		// 读入成交股数。成交股数存储实际值
		if (!ReadSinaOneValue(pSinaWebData, m_llVolume)) { throw exception(_T("Volume")); }
		// 读入成交金额
		if (!ReadSinaOneValue(pSinaWebData, m_llAmount)) { throw exception(_T("Amount")); }
		// 读入买一--买五的股数和价格
		for (int j = 0; j < 5; j++) {
			// 读入数量
			if (!ReadSinaOneValue(pSinaWebData, m_lVBuy.at(j))) { throw exception(_T("VBuy")); }
			// 读入价格
			if (!ReadSinaOneValue(pSinaWebData, dTemp)) { throw exception(_T("PBuy")); }
			m_lPBuy.at(j) = dTemp * 1000;
		}
		// 读入卖一--卖五的股数和价格
		for (int j = 0; j < 5; j++) {
			// 读入数量
			if (!ReadSinaOneValue(pSinaWebData, m_lVSell.at(j))) { throw exception(_T("VSell")); }
			// 读入价格
			if (!ReadSinaOneValue(pSinaWebData, dTemp)) { throw exception(_T("PSell")); }
			m_lPSell.at(j) = dTemp * 1000;
		}
		// 读入成交日期和时间。此时间为东八区（北京标准时间）。
		if (!ReadSinaOneValue(pSinaWebData, sv1)) { throw exception(_T("Time1")); }
		string sTime(sv1.data(), sv1.size());
		sTime += ' '; //添加一个空格，以利于下面的转换
		if (!ReadSinaOneValue(pSinaWebData, sv3)) { throw exception(_T("Time2")); }
		sTime.append(sv3.data(), sv3.size());
		m_time = ConvertBufferToTime("%04d-%02d-%02d %02d:%02d:%02d", sTime.c_str());	//转成UTC时间。新浪实时数据的时区与默认的东八区相同，故而无需添加时区偏离量
		// 后面的数据皆为无效数据，读至此数据的结尾处即可。
		if (pSinaWebData->CurrentParagraphOutOfRange()) throw exception(_T("bad data")); // 如果已读过本段数据，则该段数据有误
		pSinaWebData->SetCurrentPos(pSinaWebData->GetCurrentParagraphStartPos() + pSinaWebData->GetCurrentParagraph().size()); // 本段数据以字符';'结束，读至其后的\n字符处
		if (!pSinaWebData->IsLastDataParagraph()) {
			if (pSinaWebData->GetCurrentPosData() != 0x00a) throw exception(_T("need \n"));
			pSinaWebData->IncreaseCurrentPos(); // 读过换行符\n
		}
		else {// 最后一个数据时，如果没有换行符，则已到达数据的末尾，就不用测试是否存在换行符了。
			if (pSinaWebData->GetCurrentPosData() == 0x00a) pSinaWebData->IncreaseCurrentPos(); // 如果有\n存在则跨过去。
		}
		// 判断此实时数据是否有效，可以在此判断，结果就是今日有效股票数会减少（退市的股票有数据，但其值皆为零，而生成今日活动股票池时需要实时数据是有效的）。
		// 0.03版本和其之前的都没有做判断，0.04版本还是使用不判断的这种吧。
		// 在系统准备完毕前就判断新浪活跃股票数，只使用成交时间一项，故而依然存在非活跃股票在其中。
		// 0.07版后，采用十四天内的实时数据为活跃股票数据（最长的春节放假七天，加上前后的休息日，共十天，宽限四天）
		CheckSinaRTDataActive();
		SetDataSource(SINA_RT_WEB_DATA_);
		return true;
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ReadSinaData异常 "), e);
		if (pSinaWebData->OutOfRange()) return false;
		// 最后一个数据时，如果没有换行符，则已到达数据的末尾，就不用测试是否存在换行符了。
		pSinaWebData->SetCurrentPos(pSinaWebData->GetCurrentParagraphStartPos() + pSinaWebData->GetCurrentParagraph().size()); // 本段数据以字符';'结束，读至其后的\n字符处
		if (pSinaWebData->GetCurrentPosData() == 0x00a) pSinaWebData->IncreaseCurrentPos(); // 读过换行符\n
		return false;
	}
}

bool CWebRTData::CheckSinaRTDataActive() {
	if (IsValidTime(14)) m_fActive = true;
	else m_fActive = false;

	return m_fActive;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个INT64值，遇到逗号结束。返回值在llReturnValue中
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadSinaOneValue(const CWebDataPtr& pSinaWebData, INT64& llReturnValue) const {
	string_view sv;

	if (!ReadSinaOneValue(pSinaWebData, sv)) {
		return false;
	}
	llReturnValue = atoll(sv.data());
	if (llReturnValue < 0) return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个值，遇到逗号结束。返回值在lReturnValue中
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadSinaOneValue(const CWebDataPtr& pSinaWebData, long& lReturnValue) const {
	string_view sv;

	if (!ReadSinaOneValue(pSinaWebData, sv)) {
		return false;
	}
	lReturnValue = atol(sv.data());
	if (lReturnValue < 0) return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个值，遇到逗号结束。返回值在lReturnValue中
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadSinaOneValue(const CWebDataPtr& pSinaWebData, double& dReturnValue) const {
	string_view sv;

	if (!ReadSinaOneValue(pSinaWebData, sv)) {
		return false;
	}
	dReturnValue = atof(sv.data());
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个值，遇到逗号结束。
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadSinaOneValue(const CWebDataPtr& pSinaWebData, string_view& svSub) {
	try {
		const string_view sv = pSinaWebData->GetStringView(pSinaWebData->GetCurrentPos(), pSinaWebData->GetBufferLength() - pSinaWebData->GetCurrentPos());
		const auto iPosition = sv.find_first_of(',');
		if (iPosition > sv.size()) throw exception(_T("out of range"));
		svSub = sv.substr(0, iPosition);
		pSinaWebData->IncreaseCurrentPos(iPosition + 1);// 跨过本数据和随后的','号
		return true;
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ReadSinaDataOneValue异常 "), e);
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// var hq_str_sh601006="大秦铁路,27.55,27.25,26.91,27.55,26.20,26.91,26.92,
//                     22114263,589824680,4695,26.91,57590,26.90,14700,26.89,14300,
//                     26.88,15100,26.87,3100,26.92,8900,26.93,14230,26.94,25150,26.95,15220,26.96,2008-01-11,15:05:32,00";
//
// 无效数据格式为：var hq_str_sh688801="";
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadSinaData(const string_view svData) {
	try {
		string_view sv2;
		long lCurrentPos = 11; // 跨过字符串：var hq_str_
		const string_view svStockSymbol(svData.data() + lCurrentPos, 8);
		if (svStockSymbol.at(0) != 's') throw exception(_T("Bad header: need sh or sz"));
		if (!(svStockSymbol.at(1) == 'h' || svStockSymbol.at(1) == 'z')) throw exception(_T("Bad header: need sh or sz"));
		m_strSymbol = XferSinaToStandard(svStockSymbol);
		if (svData.length() == 23) { // 空数据: var hq_str_sh688801="";,包括最后的';'分号
			m_fActive = false;
			SetDataSource(SINA_RT_WEB_DATA_);
			return true;
		}
		lCurrentPos += 10; // 跨过字符串： sh601006="
		// 读入证券名称
		string_view sv = GetNextField(svData, lCurrentPos, ',');
		m_strStockName.Append(sv.data(), sv.length());
		// 读入开盘价。放大一千倍后存储为长整型。其他价格亦如此。
		sv = GetNextField(svData, lCurrentPos, ',');
		m_lOpen = atof(sv.data()) * 1000;
		// 读入前收盘价
		sv = GetNextField(svData, lCurrentPos, ',');
		m_lLastClose = atof(sv.data()) * 1000;
		// 读入当前价
		sv = GetNextField(svData, lCurrentPos, ',');
		m_lNew = atof(sv.data()) * 1000;
		// 读入最高价
		sv = GetNextField(svData, lCurrentPos, ',');
		m_lHigh = atof(sv.data()) * 1000;
		// 读入最低价
		sv = GetNextField(svData, lCurrentPos, ',');
		m_lLow = atof(sv.data()) * 1000;
		// 读入竞买价
		sv = GetNextField(svData, lCurrentPos, ',');
		m_lBuy = atof(sv.data()) * 1000;
		// 读入竞卖价
		sv = GetNextField(svData, lCurrentPos, ',');
		m_lSell = atof(sv.data()) * 1000;
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
			m_lPBuy.at(j) = atof(sv.data()) * 1000;
		}
		// 读入卖一--卖五的股数和价格
		for (int j = 0; j < 5; j++) {
			// 读入数量
			sv = GetNextField(svData, lCurrentPos, ',');
			m_lVSell.at(j) = atol(sv.data());
			// 读入价格
			sv = GetNextField(svData, lCurrentPos, ',');
			m_lPSell.at(j) = atof(sv.data()) * 1000;
		}
		// 读入成交日期和时间。此时间为东八区（北京标准时间）。
		sv = GetNextField(svData, lCurrentPos, ',');
		string sTime(sv.data(), sv.size());
		sTime += ' '; //添加一个空格，以利于下面的转换
		sv2 = GetNextField(svData, lCurrentPos, ',');
		sTime.append(sv2.data(), sv2.size());
		m_time = ConvertBufferToTime("%04d-%02d-%02d %02d:%02d:%02d", sTime.c_str());	//转成UTC时间。新浪实时数据的时区与默认的东八区相同，故而无需添加时区偏离量
		// 后面的数据皆为无效数据，不再处理
		// 判断此实时数据是否有效，可以在此判断，结果就是今日有效股票数会减少（退市的股票有数据，但其值皆为零，而生成今日活动股票池时需要实时数据是有效的）。
		// 在系统准备完毕前就判断新浪活跃股票数，只使用成交时间一项，故而依然存在非活跃股票在其中。
		// 0.07版后，采用十四天内的实时数据为活跃股票数据（最长的春节放假七天，加上前后的休息日，共十天，宽限四天）
		CheckSinaRTDataActive();
		SetDataSource(SINA_RT_WEB_DATA_);
		return true;
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ReadSinaData异常 "), e);
		return false;
	}
}

string_view CWebRTData::GetNextField(string_view svData, long& lCurrentPos, char chDelimiter) {
	const string_view sv(svData.data() + lCurrentPos, svData.length() - lCurrentPos);
	const long lEnd = sv.find_first_of(chDelimiter);
	if (lEnd > sv.length()) throw exception("GetNextField() out of range"); // 没找到的话抛出异常
	lCurrentPos += lEnd + 1; // 将当前位置移至本数据之后
	return string_view(sv.data(), lEnd);
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
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadTengxunData(string_view svData) {
	long lTemp = 0;
	float fTemp = 0.0;
	CString strTengxunStockCode;

	try {
		long lCurrentPos = 12;
		double dTemp = 0.0;

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
			ASSERT(0);
			break;
		}
		// 现在成交价。放大一千倍后存储为长整型。其他价格亦如此。
		sv = GetNextField(svData, lCurrentPos, '~'); //
		m_lNew = atof(sv.data()) * 1000;
		// 前收盘价
		sv = GetNextField(svData, lCurrentPos, '~'); //
		m_lLastClose = atof(sv.data()) * 1000;
		// 开盘价
		sv = GetNextField(svData, lCurrentPos, '~'); //
		m_lOpen = atof(sv.data()) * 1000;
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
			dTemp = atof(sv.data());
			m_lPBuy.at(j) = static_cast<long>(dTemp * 1000);
			// 买盘数量（手）
			sv = GetNextField(svData, lCurrentPos, '~'); //
			lTemp = atol(sv.data());
			m_lVBuy.at(j) = lTemp * 100;
		}
		// 读入卖一至卖五的价格和手数
		for (int j = 0; j < 5; j++) {
			//读入卖盘价格
			sv = GetNextField(svData, lCurrentPos, '~'); //
			m_lPSell.at(j) = atof(sv.data()) * 1000;
			// 卖盘数量（手）
			sv = GetNextField(svData, lCurrentPos, '~'); //
			lTemp = atol(sv.data());
			m_lVSell.at(j) = lTemp * 100;
		}
		// 最近逐笔成交
		sv = GetNextField(svData, lCurrentPos, '~'); //
		lTemp = atol(sv.data());
		// 成交日期和时间.格式为：yyyymmddhhmmss. 此时间采用的时区为东八区（北京标准时间）
		sv = GetNextField(svData, lCurrentPos, '~'); //
		//if (!ReadTengxunOneValue(pTengxunWebRTData, buffer1)) {
		//throw exception(_T("成交日期和时间"));
		//}
		const string sTime(sv.data(), sv.size());
		m_time = ConvertBufferToTime("%04d%02d%02d%02d%02d%02d", sTime.c_str()); // 转成UTC时间。腾讯实时数据的时区与默认的东八区相同，故而无需添加时区偏离量
		// 涨跌
		sv = GetNextField(svData, lCurrentPos, '~'); //
		// 涨跌率
		sv = GetNextField(svData, lCurrentPos, '~'); //
		// 最高价
		sv = GetNextField(svData, lCurrentPos, '~'); //
		m_lHigh = atof(sv.data()) * 1000;
		// 最低价
		sv = GetNextField(svData, lCurrentPos, '~'); //
		m_lLow = atof(sv.data()) * 1000;
		// 第三十五项，成交价/成交量（手）/成交金额（元）
		// 成交量和成交金额使用此处的数据，这样就可以使用腾讯实时数据了
		//if (!ReadTengxunOneValue(pTengxunWebRTData, buffer1)) {
		//throw exception(_T("成交价、成交手数、成交额"));
		//}
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
		// 无名
		sv = GetNextField(svData, lCurrentPos, '~'); //
		// 最高价
		sv = GetNextField(svData, lCurrentPos, '~'); //
		// 最低价
		sv = GetNextField(svData, lCurrentPos, '~'); //
		// 振幅
		sv = GetNextField(svData, lCurrentPos, '~'); //
		// 流通市值（单位为：亿元）
		sv = GetNextField(svData, lCurrentPos, '~'); //
		m_llCurrentValue = atof(sv.data()) * 100000000;
		// 总市值（单位为：亿元）
		sv = GetNextField(svData, lCurrentPos, '~'); //
		m_llTotalValue = atof(sv.data()) * 100000000;
		// 市净率
		sv = GetNextField(svData, lCurrentPos, '~'); //
		// 涨停价
		sv = GetNextField(svData, lCurrentPos, '~'); //
		m_lHighLimitFromTengxun = atof(sv.data()) * 1000;
		// 跌停价
		sv = GetNextField(svData, lCurrentPos, '~'); //
		m_lLowLimitFromTengxun = atof(sv.data()) * 1000;

		// 后面的数据具体内容不清楚，暂时放弃解码。
		// 腾讯实时数据的结束符是分号（；），然后跟随一个换行符\n。但将该数据存入txt文件后再读取时，换行符消失了。
		// 故而要先判断分号，然后用回车符作为附加判断，有否都认可。
		// 最后一个数据时，如果没有换行符，则已到达数据的末尾，就不用测试是否存在换行符了。
		CheckTengxunRTDataActive();
		SetDataSource(TENGXUN_RT_WEB_DATA_);
		return true;
	}
	catch (exception& e) {
		const CString strMessage = _T("ReadTengxunData异常, 股票代码为：") + strTengxunStockCode;
		ReportErrorToSystemMessage(strMessage, e);
		return false;
	}
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
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadTengxunData(const CWebDataPtr& pTengxunWebRTData) {
	const long lRecordStringStartPos = pTengxunWebRTData->GetCurrentPos() + 12; // 跨过"v_sh000001=\""
	//const long lStartPos = lRecordStringStartPos;
	long lTemp = 0;
	float fTemp = 0.0;
	CString strTengxunStockCode;
	pTengxunWebRTData->SetCurrentParagraphStartPos(pTengxunWebRTData->GetCurrentPos()); // 本段数据起始位置
	const string_view sv3 = pTengxunWebRTData->GetStringView(pTengxunWebRTData->GetCurrentPos(), pTengxunWebRTData->GetBufferLength() - pTengxunWebRTData->GetCurrentPos());
	const long lParagraphLength = sv3.find_first_of(_T(";")); // 每段数据以字符';'作为结束符
	if (lParagraphLength < 0) {
		pTengxunWebRTData->SetCurrentPos(pTengxunWebRTData->GetBufferLength());
		return false;
	}
	pTengxunWebRTData->SetCurrentParagraph(sv3.substr(0, lParagraphLength + 1));

	long lCurrentPos = pTengxunWebRTData->GetCurrentPos();
	try {
		WORD wMarket;
		double dTemp = 0.0;
		INT64 llTemp = 0;

		m_fActive = false; // 初始状态为无效数据
		string_view sv1 = pTengxunWebRTData->GetStringView(pTengxunWebRTData->GetCurrentPos(), 3); // 读入“v_s"
		if (sv1.compare(_T("v_s")) != 0) {// 数据格式出错
			throw exception(_T("bad head, \"v_s\" needed "));
		}
		pTengxunWebRTData->IncreaseCurrentPos(3);
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		if (pTengxunWebRTData->GetCurrentPosData() == 'h') {
			// 上海股票
			wMarket = _SHANGHAI_MARKET_; // 上海股票标识
		}
		else if (pTengxunWebRTData->GetCurrentPosData() == 'z') {
			wMarket = _SHENZHEN_MARKET_; // 深圳股票标识
		}
		else { throw exception(_T("bad market head")); }
		pTengxunWebRTData->IncreaseCurrentPos();
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();

		// 六位股票代码
		sv1 = pTengxunWebRTData->GetStringView(pTengxunWebRTData->GetCurrentPos(), 6);
		const CString strStockSymbol(sv1.data(), sv1.size());
		switch (wMarket) {
		case _SHANGHAI_MARKET_:
			strTengxunStockCode = _T("sh") + strStockSymbol; // 由于上海深圳股票代码有重叠，故而所有的股票代码都带上市场前缀。上海为sh
			break;
		case _SHENZHEN_MARKET_:
			strTengxunStockCode = _T("sz") + strStockSymbol; // 由于上海深圳股票代码有重叠，故而所有的股票代码都带上市场前缀。深圳为sz
			break;
		default:
			throw exception(_T("无效市场代码"));
		}
		m_strSymbol = XferTengxunToStandard(strTengxunStockCode);
		const long lStockCode = atoi(sv1.data());
		pTengxunWebRTData->IncreaseCurrentPos(6);
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();

		sv1 = pTengxunWebRTData->GetStringView(pTengxunWebRTData->GetCurrentPos(), 2); // 读入'="'
		if (sv1.compare(_T("=\"")) != 0) { throw exception(_T("需要'=\"'")); }
		pTengxunWebRTData->IncreaseCurrentPos(2);
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();

		// 市场标识代码（51为深市，1为沪市）
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) { throw exception(_T("市场标识代码错误")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		if (lTemp == 1)
			ASSERT(wMarket == _SHANGHAI_MARKET_);
		else if (lTemp == 51)
			ASSERT(wMarket == _SHENZHEN_MARKET_);
		else
			ASSERT(0); // 报错
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();

		if (!ReadTengxunOneValue(pTengxunWebRTData, sv1)) { throw exception(_T("股票名称错误")); }
		m_strStockName.Append(sv1.data(), sv1.size()); // 设置股票名称
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();

		// 六位股票代码
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) { throw exception(_T("股票代码错误")); }
		if (lTemp != lStockCode) { throw exception(_T("股票代码不符")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();

		// 现在成交价。放大一千倍后存储为长整型。其他价格亦如此。
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("成交价")); }
		m_lNew = static_cast<long>(dTemp * 1000);
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 前收盘价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("前收盘")); }
		m_lLastClose = static_cast<long>(dTemp * 1000);
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 开盘价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("开盘价")); }
		m_lOpen = static_cast<long>(dTemp * 1000);
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 成交手数。成交股数存储实际值
		// 不使用此处的成交量，而是使用第三十五项处的成交量。
		if (!ReadTengxunOneValue(pTengxunWebRTData, llTemp)) { throw exception(_T("成交手数")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 外盘
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) { throw exception(_T("外盘")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 内盘
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) { throw exception(_T("内盘")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 读入买一至买五的价格和手数
		for (int j = 0; j < 5; j++) {
			// 买盘价格
			if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("买盘价格")); }
			m_lPBuy.at(j) = static_cast<long>(dTemp * 1000);
			lCurrentPos = pTengxunWebRTData->GetCurrentPos();
			// 买盘数量（手）
			if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) { throw exception(_T("买盘数量")); }
			m_lVBuy.at(j) = lTemp * 100;
			lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		}
		// 读入卖一至卖五的价格和手数
		for (int j = 0; j < 5; j++) {
			//读入卖盘价格
			if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("买盘价格")); }
			m_lPSell.at(j) = static_cast<long>(dTemp * 1000);
			lCurrentPos = pTengxunWebRTData->GetCurrentPos();
			// 卖盘数量（手）
			if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) { throw exception(_T("买盘数量")); }
			m_lVSell.at(j) = lTemp * 100;
			lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		}
		// 最近逐笔成交
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) { throw exception(_T("最近逐笔成交")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 成交日期和时间.格式为：yyyymmddhhmmss. 此时间采用的时区为东八区（北京标准时间）
		if (!ReadTengxunOneValue(pTengxunWebRTData, sv1)) { throw exception(_T("成交日期和时间")); }
		//if (!ReadTengxunOneValue(pTengxunWebRTData, buffer1)) {
		//throw exception(_T("成交日期和时间"));
		//}
		const string sTime(sv1.data(), sv1.size());
		m_time = ConvertBufferToTime("%04d%02d%02d%02d%02d%02d", sTime.c_str()); // 转成UTC时间。腾讯实时数据的时区与默认的东八区相同，故而无需添加时区偏离量
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 涨跌
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("涨跌")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 涨跌率
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("涨跌率")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 最高价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("最高价")); }
		m_lHigh = static_cast<long>(dTemp * 1000);
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 最低价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("最低价")); }
		m_lLow = static_cast<long>(dTemp * 1000);
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 第三十五项，成交价/成交量（手）/成交金额（元）
		// 成交量和成交金额使用此处的数据，这样就可以使用腾讯实时数据了
		//if (!ReadTengxunOneValue(pTengxunWebRTData, buffer1)) {
		//throw exception(_T("成交价、成交手数、成交额"));
		//}
		if (!ReadTengxunOneValue(pTengxunWebRTData, sv1)) { throw exception(_T("成交价、成交手数、成交额")); }
		CString str(sv1.data(), sv1.size());
		sscanf_s(str.GetBuffer(), _T("%f/%d/%I64d"), &fTemp, &lTemp, &m_llAmount);
		m_llVolume = lTemp * 100; // 腾讯成交量数据单位为手（100股）。
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 成交手数
		// 不使用此处的成交量。这里的成交量会大于第三十五处的成交量。
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) { throw exception(_T("成交量")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 成交金额（万元）
		if (!ReadTengxunOneValue(pTengxunWebRTData, llTemp)) { throw exception(_T("成交金额")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 换手率
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("换手率")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 市盈率
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("市盈率")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 无名
		if (!ReadTengxunOneValue(pTengxunWebRTData, sv1)) { throw exception(_T("无名")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 最高价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("最高价")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 最低价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("最低价")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 振幅
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("振幅")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 流通市值（单位为：亿元）
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("流通市值")); }
		m_llCurrentValue = static_cast<INT64>(dTemp * 100000000);
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 总市值（单位为：亿元）
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("总市值")); }
		m_llTotalValue = static_cast<INT64>(dTemp * 100000000);
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 市净率
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("市净率")); }
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 涨停价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("涨停价")); }
		if (dTemp > 0.01) m_lHighLimitFromTengxun = static_cast<long>(dTemp * 1000);
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();
		// 跌停价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) { throw exception(_T("跌停价")); }
		if (dTemp > 0.01) m_lLowLimitFromTengxun = static_cast<long>(dTemp * 1000);
		lCurrentPos = pTengxunWebRTData->GetCurrentPos();

		// 后面的数据具体内容不清楚，暂时放弃解码。
		// 腾讯实时数据的结束符是分号（；），然后跟随一个换行符\n。但将该数据存入txt文件后再读取时，换行符消失了。
		// 故而要先判断分号，然后用回车符作为附加判断，有否都认可。
		// 最后一个数据时，如果没有换行符，则已到达数据的末尾，就不用测试是否存在换行符了。
		if (pTengxunWebRTData->CurrentParagraphOutOfRange()) throw exception(_T("bad data")); // 如果已读过本段数据，则该段数据有误
		pTengxunWebRTData->SetCurrentPos(pTengxunWebRTData->GetCurrentParagraphStartPos() + pTengxunWebRTData->GetCurrentParagraph().size()); // 本段数据以字符';'结束，读至其后的\n字符处
		// 最后一个数据时，如果没有换行符，则已到达数据的末尾，就不用测试是否存在换行符了。
		if (!pTengxunWebRTData->IsLastDataParagraph()) {
			if (pTengxunWebRTData->GetCurrentPosData() != 0x00a) { throw exception(_T("expect \n")); }
			pTengxunWebRTData->IncreaseCurrentPos(); // 读过换行符\a
		}
		else {
			if (pTengxunWebRTData->GetCurrentPosData() == 0x00a) pTengxunWebRTData->IncreaseCurrentPos(); // 如果有\n存在则跨过去。
		}
		CheckTengxunRTDataActive();
		SetDataSource(TENGXUN_RT_WEB_DATA_);
		return true;
	}
	catch (exception& e) {
		const CString strMessage = _T("ReadTengxunData异常, 股票代码为：") + strTengxunStockCode;
		ReportErrorToSystemMessage(strMessage, e);
		pTengxunWebRTData->SetCurrentPos(pTengxunWebRTData->GetCurrentParagraphStartPos() + pTengxunWebRTData->GetCurrentParagraph().size()); // 本段数据以字符';'结束，读至其后的\n字符处
		// 前进至下一个数据开始处
		if (pTengxunWebRTData->GetCurrentPosData() == 0x00a) pTengxunWebRTData->IncreaseCurrentPos(); // 读过换行符\a
		return false;
	}
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

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个INT64值，遇到~号结束。返回值在llReturnValue中
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadTengxunOneValue(const CWebDataPtr& pTengxunWebRTData, INT64& llReturnValue) {
	string_view sv;

	if (!ReadTengxunOneValue(pTengxunWebRTData, sv)) { return false; }
	llReturnValue = atoll(sv.data());
	if (llReturnValue < 0) return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个浮点型值，遇到~号结束。返回值在dReturnValue中
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadTengxunOneValue(const CWebDataPtr& pTengxunWebRTData, double& dReturnValue) {
	string_view sv;

	if (!ReadTengxunOneValue(pTengxunWebRTData, sv)) { return false; }
	dReturnValue = atof(sv.data());
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个长整型值，遇到~号结束。返回值在lReturnValue中
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadTengxunOneValue(const CWebDataPtr& pTengxunWebRTData, long& lReturnValue) {
	string_view sv;

	if (!ReadTengxunOneValue(pTengxunWebRTData, sv)) { return false; }
	lReturnValue = atol(sv.data());
	if (lReturnValue < 0) return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个字符串，遇到~号结束，结果在string_view中。
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadTengxunOneValue(const CWebDataPtr& pTengxunWebRTData, string_view& svSub) {
	string_view sv;
	try {
		sv = pTengxunWebRTData->GetStringView(pTengxunWebRTData->GetCurrentPos(), pTengxunWebRTData->GetBufferLength() - pTengxunWebRTData->GetCurrentPos());
		const auto iPosition = sv.find_first_of('~');
		if (iPosition > sv.size()) throw exception(_T("out of range"));
		svSub = sv.substr(0, iPosition);
		pTengxunWebRTData->IncreaseCurrentPos(iPosition + 1);	 // 跨过本数据和其后的'~'号。
		return true;
	}
	catch (exception& e) {
		if (strcmp(e.what(), _T("out of range")) == 0) {
			svSub = sv;
			throw exception(_T("out of range")); // 越界的话则继续抛出exception。
		}
		CString errorMessage = pTengxunWebRTData->GetStockCode() + _T(" ReadTengxunOnValue异常:");
		errorMessage += svSub.data();
		errorMessage += _T("  ");
		ReportErrorToSystemMessage(errorMessage, e);
		return false;
	}
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
	if (m_time < (GetUTCTime() - lDays * 24 * 3600)) {// 确保实时数据不早于当前时间的14天前（春节放假最长为7天，加上前后的休息日，共十一天）
		return false;
	}
	if (m_time > GetUTCTime()) { return false; }
	return true;
}
