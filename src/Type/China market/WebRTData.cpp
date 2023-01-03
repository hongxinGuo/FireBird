﻿#include"pch.h"
#include "globedef.h"

#include"ConvertToString.h"
#include"TimeConvert.h"
#include"ChinaStockCodeConverter.h"
#include"InfoReport.h"

#include"jsonParse.h"
#include"WebRTData.h"
#include"ChinaMarket.h"

void CWebRTData::Reset(void) {
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
	m_lHighLimit = 0;
	m_lLowLimit = 0;
	for (int i = 0; i < 5; i++) {
		m_lPBuy.at(i) = 0;
		m_lVBuy.at(i) = 0;
		m_lPSell.at(i) = 0;
		m_lVSell.at(i) = 0;
	}
	m_fActive = false;
}

CWebRTData::CWebRTData(void) {
	Reset();
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
bool CWebRTData::ReadSinaData(const CWebDataPtr& pSinaWebRTData) {
	const CString strHeader = _T("var hq_str_s");
	char bufferTest[2000];
	bool fBadData = false;

	int i = 0;
	while ((!fBadData) && (pSinaWebRTData->GetData(i + pSinaWebRTData->GetCurrentPos()) != ';')) {
		bufferTest[i] = pSinaWebRTData->GetData(i + pSinaWebRTData->GetCurrentPos());
		i++;
		if ((i >= 1900) || ((i + pSinaWebRTData->GetCurrentPos()) >= (pSinaWebRTData->GetBufferLength() - 1))) {
			fBadData = true;
			break;
		}
	}
	bufferTest[i] = pSinaWebRTData->GetData(i + pSinaWebRTData->GetCurrentPos());
	i++;
	bufferTest[i] = 0x000;
	if (fBadData) {
		const CString strTest = bufferTest;
		gl_systemMessage.PushInnerSystemInformationMessage(_T("SinaRTData整体数据出问题，抛掉不用"));
		gl_systemMessage.PushInnerSystemInformationMessage(strTest);
		return false; // 整个数据出现错误，后面的皆抛掉
	}

	try {
		CString strSinaStockCode;
		WORD wMarket;
		double dTemp = 0;
		char buffer3[100];
		char buffer2[20];
		char buffer1[100];
		m_fActive = false; // 初始状态为无效数据
		pSinaWebRTData->GetData(buffer1, 12, pSinaWebRTData->GetCurrentPos()); // 读入“var hq_str_s"
		buffer1[12] = 0x000;
		const CString str1 = buffer1;
		if (strHeader.Compare(str1) != 0) {
			// 数据格式出错
			throw exception();
		}
		pSinaWebRTData->IncreaseCurrentPos(12);

		if (pSinaWebRTData->GetCurrentPosData() == 'h') {
			// 上海股票
			wMarket = _SHANGHAI_MARKET_; // 上海股票标识
		}
		else if (pSinaWebRTData->GetCurrentPosData() == 'z') {
			wMarket = _SHENZHEN_MARKET_; // 深圳股票标识
		}
		else { throw exception(); }
		pSinaWebRTData->IncreaseCurrentPos();

		pSinaWebRTData->GetData(buffer2, 6, pSinaWebRTData->GetCurrentPos());
		buffer2[6] = 0x000;
		const CString strStockSymbol = buffer2;
		switch (wMarket) {
		case _SHANGHAI_MARKET_:
			strSinaStockCode = _T("sh") + strStockSymbol; // 由于上海深圳股票代码有重叠，故而所有的股票代码都带上市场前缀。上海为sh
			break;
		case _SHENZHEN_MARKET_:
			strSinaStockCode = _T("sz") + strStockSymbol; // 由于上海深圳股票代码有重叠，故而所有的股票代码都带上市场前缀。深圳为sz
			break;
		default:
			throw exception();
		}
		m_strSymbol = XferSinaToStandard(strSinaStockCode);
		long lStockCode = static_cast<long>(atof(buffer2));
		pSinaWebRTData->IncreaseCurrentPos(6);

		pSinaWebRTData->GetData(buffer1, 2, pSinaWebRTData->GetCurrentPos()); // 读入'="'
		if ((buffer1[0] != '=') || (buffer1[1] != '"')) { throw exception(); }
		pSinaWebRTData->IncreaseCurrentPos(2);
		pSinaWebRTData->GetData(buffer1, 2, pSinaWebRTData->GetCurrentPos());
		if (buffer1[0] == '"') {
			// 没有数据?
			if (buffer1[1] != ';') { throw exception(); }
			pSinaWebRTData->IncreaseCurrentPos(2);
			if (pSinaWebRTData->GetCurrentPosData() != 0x00a) { return false; }
			pSinaWebRTData->IncreaseCurrentPos();
			m_fActive = false;
			SetDataSource(SINA_RT_WEB_DATA_);
			return true; // 非活跃股票没有实时数据，在此返回。
		}
		if ((buffer1[0] == 0x00a) || pSinaWebRTData->OutOfRange()) { throw exception(); }
		if ((buffer1[1] == 0x00a) || pSinaWebRTData->OutOfRange()) { throw exception(); }
		pSinaWebRTData->IncreaseCurrentPos(2);

		i = 2;
		while ((pSinaWebRTData->GetCurrentPosData() != ',') && (i < 10)) {
			// 读入剩下的中文名字（第一个字在buffer1中）
			if ((pSinaWebRTData->GetCurrentPosData() == 0x00a) || pSinaWebRTData->OutOfRange()) { throw exception(); }
			buffer1[i++] = pSinaWebRTData->GetCurrentPosData();
			pSinaWebRTData->IncreaseCurrentPos();
		}
		buffer1[i] = 0x000;
		m_strStockName = buffer1; // 设置股票名称

		pSinaWebRTData->IncreaseCurrentPos();

		// 读入开盘价。放大一千倍后存储为长整型。其他价格亦如此。
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) { throw exception(); }
		m_lOpen = static_cast<long>((dTemp + 0.000001) * 1000);
		// 读入前收盘价
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) { throw exception(); }
		m_lLastClose = static_cast<long>((dTemp + 0.000001) * 1000);
		// 读入当前价
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) { throw exception(); }
		m_lNew = static_cast<long>((dTemp + 0.000001) * 1000);
		// 读入最高价
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) { throw exception(); }
		m_lHigh = static_cast<long>((dTemp + 0.000001) * 1000);
		// 读入最低价
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) { throw exception(); }
		m_lLow = static_cast<long>((dTemp + 0.000001) * 1000);
		// 读入竞买价
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) { throw exception(); }
		m_lBuy = static_cast<long>((dTemp + 0.000001) * 1000);
		// 读入竞卖价
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) { throw exception(); }
		m_lSell = static_cast<long>((dTemp + 0.000001) * 1000);
		// 读入成交股数。成交股数存储实际值
		if (!ReadSinaOneValue(pSinaWebRTData, m_llVolume)) { throw exception(); }
		// 读入成交金额
		if (!ReadSinaOneValue(pSinaWebRTData, m_llAmount)) { throw exception(); }
		// 读入买一--买五的股数和价格
		for (int j = 0; j < 5; j++) {
			// 读入数量
			if (!ReadSinaOneValue(pSinaWebRTData, m_lVBuy.at(j))) { throw exception(); }
			// 读入价格
			if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) { throw exception(); }
			m_lPBuy.at(j) = static_cast<long>((dTemp + 0.000001) * 1000);
		}
		// 读入卖一--卖五的股数和价格
		for (int j = 0; j < 5; j++) {
			// 读入数量
			if (!ReadSinaOneValue(pSinaWebRTData, m_lVSell.at(j))) { throw exception(); }
			// 读入价格
			if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) { throw exception(); }
			m_lPSell.at(j) = static_cast<long>((dTemp + 0.000001) * 1000);
		}
		// 读入成交日期和时间。此时间为东八区（北京标准时间）。
		if (!ReadSinaOneValue(pSinaWebRTData, buffer1)) { throw exception(); }
		CString strTime = buffer1;
		strTime += ' '; //添加一个空格，以利于下面的转换
		if (!ReadSinaOneValue(pSinaWebRTData, buffer3)) { throw exception(); }
		strTime += buffer3;
		m_time = ConvertBufferToTime("%04d-%02d-%02d %02d:%02d:%02d", strTime.GetBuffer());
		// 转成UTC时间。新浪实时数据的时区与默认的东八区相同，故而无需添加时区偏离量

		// 后面的数据皆为无效数据，读至此数据的结尾处即可。
		// 新浪实时数据的结束符是'"',然后跟着分号';'，然后跟随一个换行符\n。但将该数据存入txt文件后在读取时，换行符消失了。
		// 故而要先判断'"'和';'，然后用换行符作为附加判断，有否都认可。
		while (pSinaWebRTData->GetCurrentPosData() != '"') {
			// 寻找本段数据的结束符（“"”）。
			pSinaWebRTData->IncreaseCurrentPos();
			if (pSinaWebRTData->OutOfRange()) { throw exception(); }
		}
		pSinaWebRTData->IncreaseCurrentPos(); // 读过字符'"'
		if (pSinaWebRTData->GetCurrentPosData() != ';') {
			// 字符'"'后面应该跟随字符';'
			throw exception();
		}
		pSinaWebRTData->IncreaseCurrentPos(); // 读过字符';'
		// 最后一个数据时，如果没有换行符，则已到达数据的末尾，就不用测试是否存在换行符了。
		if (!pSinaWebRTData->OutOfRange()) {
			if (pSinaWebRTData->GetCurrentPosData() == 0x00a) {
				pSinaWebRTData->IncreaseCurrentPos(); // 如果有\n存在则跨过去。
			}
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
		return false;
	}
}

bool CWebRTData::CheckSinaRTDataActive(void) {
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
bool CWebRTData::ReadSinaOneValue(const CWebDataPtr& pSinaWebRTData, INT64& llReturnValue) {
	char buffer3[200];

	if (!ReadSinaOneValue(pSinaWebRTData, buffer3)) { return false; }
	const auto llTemp = static_cast<INT64>(atof(buffer3));
	if (llTemp < 0) return false;
	if (llTemp > 0) llReturnValue = llTemp;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个值，遇到逗号结束。返回值在lReturnValue中
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadSinaOneValue(const CWebDataPtr& pSinaWebRTData, long& lReturnValue) {
	char buffer3[200];

	if (!ReadSinaOneValue(pSinaWebRTData, buffer3)) { return false; }
	const long lTemp = static_cast<long>(atof(buffer3));
	if (lTemp < 0) return false;
	if (lTemp > 0) lReturnValue = lTemp;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个值，遇到逗号结束。返回值在lReturnValue中
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadSinaOneValue(const CWebDataPtr& pSinaWebRTData, double& dReturnValue) {
	char buffer3[200];

	if (!ReadSinaOneValue(pSinaWebRTData, buffer3)) { return false; }
	dReturnValue = atof(buffer3);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个值，遇到逗号结束。
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadSinaOneValue(const CWebDataPtr& pSinaWebRTData, char* buffer) {
	try {
		int i = 0;
		while ((pSinaWebRTData->GetCurrentPosData() != ',')) {
			if ((pSinaWebRTData->GetCurrentPosData() == 0x00a) || pSinaWebRTData->OutOfRange()) throw exception();
			if (i > 150) throw exception();
			buffer[i++] = pSinaWebRTData->GetCurrentPosData();
			pSinaWebRTData->IncreaseCurrentPos();
		}
		buffer[i] = 0x000;
		// 跨过','号。
		pSinaWebRTData->IncreaseCurrentPos();

		return true;
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ReadSinaDataOneValue异常 "), e);
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
//             0.73~-12617~15.73~9.82~12.19~~~1.24~164582.85~0.00~0~~GP-A~68.91~~0.82";\r
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
bool CWebRTData::ReadTengxunData(const CWebDataPtr& pTengxunWebRTData) {
	const CString strHeader = _T("v_s");
	long lTemp = 0;
	float fTemp = 0.0;

	try {
		CString strTengxunStockCode;
		WORD wMarket;
		long lStockCode = 0;
		double dTemp = 0.0;
		INT64 llTemp = 0;
		char buffer3[200];
		char buffer2[7];
		char buffer1[200];
		m_fActive = false; // 初始状态为无效数据
		pTengxunWebRTData->GetData(buffer1, 3, pTengxunWebRTData->GetCurrentPos()); // 读入“v_s"
		buffer1[3] = 0x000;
		const CString str1 = buffer1;
		if (strHeader.Compare(str1) != 0) {
			// 数据格式出错
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData格式出错"));
			return false;
		}
		pTengxunWebRTData->IncreaseCurrentPos(3);
		if (pTengxunWebRTData->GetCurrentPosData() == 'h') {
			// 上海股票
			wMarket = _SHANGHAI_MARKET_; // 上海股票标识
		}
		else if (pTengxunWebRTData->GetCurrentPosData() == 'z') {
			wMarket = _SHENZHEN_MARKET_; // 深圳股票标识
		}
		else {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：无效市场代码"));
			return false;
		}
		pTengxunWebRTData->IncreaseCurrentPos();

		// 六位股票代码
		pTengxunWebRTData->GetData(buffer2, 6, pTengxunWebRTData->GetCurrentPos());
		buffer2[6] = 0x000;
		const CString strStockSymbol = buffer2;
		switch (wMarket) {
		case _SHANGHAI_MARKET_:
			strTengxunStockCode = _T("sh") + strStockSymbol; // 由于上海深圳股票代码有重叠，故而所有的股票代码都带上市场前缀。上海为sh
			break;
		case _SHENZHEN_MARKET_:
			strTengxunStockCode = _T("sz") + strStockSymbol; // 由于上海深圳股票代码有重叠，故而所有的股票代码都带上市场前缀。深圳为sz
			break;
		default:
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：无效市场代码"));
			return false;
		}
		m_strSymbol = XferTengxunToStandard(strTengxunStockCode);
		lStockCode = atoi(buffer2);
		pTengxunWebRTData->IncreaseCurrentPos(6);

		pTengxunWebRTData->GetData(buffer1, 2, pTengxunWebRTData->GetCurrentPos()); // 读入'="'
		if (buffer1[0] != '=') {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：需要'='"));
			return false;
		}
		if (buffer1[1] != '"') {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：需要'\"'"));
			return false;
		}
		pTengxunWebRTData->IncreaseCurrentPos(2);

		// 市场标识代码（51为深市，1为沪市）
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：市场标识代码"));
			return false;
		}
#ifdef DEBUG
		if (lTemp == 1)
			ASSERT(wMarket == _SHANGHAI_MARKET_);
		else if (lTemp == 51)
			ASSERT(wMarket == _SHENZHEN_MARKET_);
		else
			ASSERT(0); // 报错
#endif
		if (!ReadTengxunOneValue(pTengxunWebRTData, buffer1)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：股票名称"));
			return false;
		}
		m_strStockName = buffer1; // 设置股票名称
		// 六位股票代码
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：股票代码"));
			return false;
		}
		if (lTemp != lStockCode) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：股票代码不符"));
			return false;
		}

		// 现在成交价。放大一千倍后存储为长整型。其他价格亦如此。
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：成交价"));
			return false;
		}
		m_lNew = static_cast<long>((dTemp + 0.000001) * 1000);
		// 前收盘价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：前收盘"));
			return false;
		}
		m_lLastClose = static_cast<long>((dTemp + 0.000001) * 1000);
		// 开盘价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：开盘价"));
			return false;
		}
		m_lOpen = static_cast<long>((dTemp + 0.000001) * 1000);
		// 成交手数。成交股数存储实际值
		// 不使用此处的成交量，而是使用第三十五项处的成交量。
		if (!ReadTengxunOneValue(pTengxunWebRTData, llTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：成交手数"));
			return false;
		}
		// 外盘
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：外盘"));
			return false;
		}
		// 内盘
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：内盘"));
			return false;
		}
		// 读入买一至买五的价格和手数
		for (int j = 0; j < 5; j++) {
			// 买盘价格
			if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
				gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：买盘价格"));
				return false;
			}
			m_lPBuy.at(j) = static_cast<long>((dTemp + 0.000001) * 1000);

			// 买盘数量（手）
			if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
				gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：买盘数量"));
				return false;
			}
			m_lVBuy.at(j) = lTemp * 100;
		}
		// 读入卖一至卖五的价格和手数
		for (int j = 0; j < 5; j++) {
			//读入卖盘价格
			if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
				gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：买盘价格"));
				return false;
			}
			m_lPSell.at(j) = static_cast<long>((dTemp + 0.000001) * 1000);
			// 卖盘数量（手）
			if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
				gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：买盘数量"));
				return false;
			}
			m_lVSell.at(j) = lTemp * 100;
		}
		// 最近逐笔成交
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：最近逐笔成交"));
			return false;
		}
		// 成交日期和时间.格式为：yyyymmddhhmmss. 此时间采用的时区为东八区（北京标准时间）
		if (!ReadTengxunOneValue(pTengxunWebRTData, buffer3)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：成交日期和时间"));
			return false;
		}
		m_time = ConvertBufferToTime("%04d%02d%02d%02d%02d%02d", buffer3); // 转成UTC时间。腾讯实时数据的时区与默认的东八区相同，故而无需添加时区偏离量
		// 涨跌
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：涨跌"));
			return false;
		}
		// 涨跌率
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：涨跌率"));
			return false;
		}
		// 最高价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：最高价"));
			return false;
		}
		m_lHigh = static_cast<long>((dTemp + 0.000001) * 1000);
		// 最低价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：最低价"));
			return false;
		}
		m_lLow = static_cast<long>((dTemp + 0.000001) * 1000);
		// 第三十五项，成交价/成交量（手）/成交金额（元）
		// 成交量和成交金额使用此处的数据，这样就可以使用腾讯实时数据了
		if (!ReadTengxunOneValue(pTengxunWebRTData, buffer3)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：成交价、成交手数、成交额"));
			return false;
		}
		sscanf_s(buffer3, _T("%f/%d/%I64d"), &fTemp, &lTemp, &m_llAmount);
		m_llVolume = lTemp * 100; // 腾讯成交量数据单位为手（100股）。
		// 成交手数
		// 不使用此处的成交量。这里的成交量会大于第三十五处的成交量。
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：成交量"));
			return false;
		}
		// 成交金额（万元）
		if (!ReadTengxunOneValue(pTengxunWebRTData, llTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：成交金额"));
			return false;
		}
		// 换手率
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：换手率"));
			return false;
		}
		// 市盈率
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：市盈率"));
			return false;
		}
		// 无名
		if (!ReadTengxunOneValue(pTengxunWebRTData, buffer3)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：无名"));
			return false;
		}
		// 最高价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：最高价"));
			return false;
		}
		// 最低价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：最低价"));
			return false;
		}
		// 振幅
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：振幅"));
			return false;
		}
		// 流通市值（单位为：亿元）
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：流通市值"));
			return false;
		}
		m_llCurrentValue = static_cast<INT64>(dTemp * 100000000);
		// 总市值（单位为：亿元）
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：总市值"));
			return false;
		}
		m_llTotalValue = static_cast<INT64>(dTemp * 100000000);
		// 市净率
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：市净率"));
			return false;
		}
		// 涨停价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：涨停价"));
			return false;
		}
		if (dTemp > 0.01) m_lHighLimit = static_cast<long>((dTemp + 0.000001) * 1000);
		// 跌停价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData错误：跌停价"));
			return false;
		}
		if (dTemp > 0.01) m_lLowLimit = static_cast<long>((dTemp + 0.000001) * 1000);

		// 后面的数据具体内容不清楚，暂时放弃解码。
		// 腾讯实时数据的结束符是分号（；），然后跟随一个换行符\n。但将该数据存入txt文件后在读取时，换行符消失了。
		// 故而要先判断分号，然后用回车符作为附加判断，有否都认可。
		// 最后一个数据时，如果没有换行符，则已到达数据的末尾，就不用测试是否存在换行符了。
		while (pTengxunWebRTData->GetCurrentPosData() != '"') {
			// 寻找本段数据的结束符（“"”）。
			pTengxunWebRTData->IncreaseCurrentPos();
			if (pTengxunWebRTData->OutOfRange()) { return false; }
		}
		pTengxunWebRTData->IncreaseCurrentPos();
		if (pTengxunWebRTData->GetCurrentPosData() != ';') {
			// 字符'"'后面应该跟随字符';'
			return false;
		}
		pTengxunWebRTData->IncreaseCurrentPos();
		// 最后一个数据时，如果没有换行符，则已到达数据的末尾，就不用测试是否存在换行符了。
		if (!pTengxunWebRTData->OutOfRange()) {
			if (pTengxunWebRTData->GetCurrentPosData() == 0x00a) {
				pTengxunWebRTData->IncreaseCurrentPos(); // 如果有\n存在则跨过去。
			}
		}
		CheckTengxunRTDataActive();
		SetDataSource(TENGXUN_RT_WEB_DATA_);
		return true;
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ReadTengxunData异常 "), e);
		return false;
	}
}

bool CWebRTData::CheckTengxunRTDataActive() {
	if (!IsValidTime(14)) {
		// 如果交易时间在14天前
		m_fActive = false;
	}
	else if ((m_lOpen == 0) && (m_llVolume == 0) && (m_lHigh == 0) && (m_lLow == 0)) {
		// 腾讯非活跃股票的m_lNew不为零，故而不能使用其作为判断依据
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
	char buffer3[200];

	if (!ReadTengxunOneValue(pTengxunWebRTData, buffer3)) { return false; }
	const INT64 llTemp = atoll(buffer3);
	if (llTemp < 0) return false;
	if (llTemp > 0) llReturnValue = llTemp;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个浮点型值，遇到~号结束。返回值在dReturnValue中
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadTengxunOneValue(const CWebDataPtr& pTengxunWebRTData, double& dReturnValue) {
	char buffer3[200];

	if (!ReadTengxunOneValue(pTengxunWebRTData, buffer3)) { return false; }
	const double dTemp = atof(buffer3);
	dReturnValue = dTemp;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个长整型值，遇到~号结束。返回值在lReturnValue中
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadTengxunOneValue(const CWebDataPtr& pTengxunWebRTData, long& lReturnValue) {
	char buffer3[200];

	if (!ReadTengxunOneValue(pTengxunWebRTData, buffer3)) { return false; }
	const long lTemp = atol(buffer3);
	if (lTemp < 0) return false;
	if (lTemp > 0) lReturnValue = lTemp;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个字符串，遇到~号结束，结果在buffer中。
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadTengxunOneValue(const CWebDataPtr& pTengxunWebRTData, char* buffer) {
	int i = 0;
	try {
		while (pTengxunWebRTData->GetCurrentPosData() != '~') {
			if ((pTengxunWebRTData->GetCurrentPosData() == 0x00a) || pTengxunWebRTData->OutOfRange()) return false;
			buffer[i++] = pTengxunWebRTData->GetCurrentPosData();
			pTengxunWebRTData->IncreaseCurrentPos();
		}
		buffer[i] = 0x000;
		pTengxunWebRTData->IncreaseCurrentPos();
		return true;
	}
	catch (exception& e) {
		buffer[i] = 0x000;
		CString errorMessage = pTengxunWebRTData->GetStockCode() + _T(" ReadTengxunOnValue异常:");
		errorMessage += buffer;
		ReportErrorToSystemMessage(errorMessage, e);
		return false;
	}
}

bool CWebRTData::CheckNeteaseRTDataActive(void) {
	m_fActive = false;
	if (!IsValidTime(14)) {
		// 非活跃股票的update时间为0，转换为time_t时为-1.
		return m_fActive;
	}
	if ((m_lOpen == 0) || (m_lNew == 0)) {
		// 网易非活跃股票的实时数据也具有所有的字段，故而在此确认其为非活跃
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
	if (m_time < (gl_pChinaMarket->GetUTCTime() - lDays * 24 * 3600)) {
		// 确保实时数据不早于当前时间的14天前（春节放假最长为7天，加上前后的休息日，共十一天）
		return false;
	}
	else if (m_time > gl_pChinaMarket->GetUTCTime()) { return false; }
	else return true;
}

void CWebRTData::SaveData(CSetRealTimeData& setRTData) {
	ASSERT(setRTData.IsOpen());

	setRTData.m_Time = ConvertValueToString(m_time);
	setRTData.m_Symbol = GetSymbol();
	setRTData.m_StockName = GetStockName();
	setRTData.m_New = ConvertValueToString(GetNew(), 1000);
	setRTData.m_High = ConvertValueToString(GetHigh(), 1000);
	setRTData.m_Low = ConvertValueToString(GetLow(), 1000);
	setRTData.m_LastClose = ConvertValueToString(GetLastClose(), 1000);
	setRTData.m_Open = ConvertValueToString(GetOpen(), 1000);
	setRTData.m_Volume = ConvertValueToString(GetVolume());
	setRTData.m_Amount = ConvertValueToString(GetAmount());
	setRTData.m_Stroke = _T("0");
	setRTData.m_PBuy1 = ConvertValueToString(GetPBuy(0), 1000);
	setRTData.m_VBuy1 = ConvertValueToString(GetVBuy(0));
	setRTData.m_PSell1 = ConvertValueToString(GetPSell(0), 1000);
	setRTData.m_VSell1 = ConvertValueToString(GetVSell(0));

	setRTData.m_PBuy2 = ConvertValueToString(GetPBuy(1), 1000);
	setRTData.m_VBuy2 = ConvertValueToString(GetVBuy(1));
	setRTData.m_PSell2 = ConvertValueToString(GetPSell(1), 1000);
	setRTData.m_VSell2 = ConvertValueToString(GetVSell(1));

	setRTData.m_PBuy3 = ConvertValueToString(GetPBuy(2), 1000);
	setRTData.m_VBuy3 = ConvertValueToString(GetVBuy(2));
	setRTData.m_PSell3 = ConvertValueToString(GetPSell(2), 1000);
	setRTData.m_VSell3 = ConvertValueToString(GetVSell(2));

	setRTData.m_PBuy4 = ConvertValueToString(GetPBuy(3), 1000);
	setRTData.m_VBuy4 = ConvertValueToString(GetVBuy(3));
	setRTData.m_PSell4 = ConvertValueToString(GetPSell(3), 1000);
	setRTData.m_VSell4 = ConvertValueToString(GetVSell(3));

	setRTData.m_PBuy5 = ConvertValueToString(GetPBuy(4), 1000);
	setRTData.m_VBuy5 = ConvertValueToString(GetVBuy(4));
	setRTData.m_PSell5 = ConvertValueToString(GetPSell(4), 1000);
	setRTData.m_VSell5 = ConvertValueToString(GetVSell(4));
}

void CWebRTData::AppendData(CSetRealTimeData& setRTData) {
	ASSERT(setRTData.IsOpen());
	setRTData.AddNew();
	SaveData(setRTData);
	setRTData.Update();
}

void CWebRTData::LoadData(CSetRealTimeData& setRTData) {
	ASSERT(setRTData.IsOpen());

	m_time = atoll(setRTData.m_Time);
	m_strSymbol = setRTData.m_Symbol;
	m_strStockName = setRTData.m_StockName;
	m_lLastClose = static_cast<long>(atof(setRTData.m_LastClose) * 1000);
	m_lOpen = static_cast<long>(atof(setRTData.m_Open) * 1000);
	m_lNew = static_cast<long>(atof(setRTData.m_New) * 1000);
	m_lHigh = static_cast<long>(atof(setRTData.m_High) * 1000);
	m_lLow = static_cast<long>(atof(setRTData.m_Low) * 1000);
	m_llVolume = atoll(setRTData.m_Volume);
	m_llAmount = atoll(setRTData.m_Amount);
	m_lPBuy.at(0) = static_cast<long>(atof(setRTData.m_PBuy1) * 1000);
	m_lVBuy.at(0) = atol(setRTData.m_VBuy1);
	m_lPBuy.at(1) = static_cast<long>(atof(setRTData.m_PBuy2) * 1000);
	m_lVBuy.at(1) = atol(setRTData.m_VBuy2);
	m_lPBuy.at(2) = static_cast<long>(atof(setRTData.m_PBuy3) * 1000);
	m_lVBuy.at(2) = atol(setRTData.m_VBuy3);
	m_lPBuy.at(3) = static_cast<long>(atof(setRTData.m_PBuy4) * 1000);
	m_lVBuy.at(3) = atol(setRTData.m_VBuy4);
	m_lPBuy.at(4) = static_cast<long>(atof(setRTData.m_PBuy5) * 1000);
	m_lVBuy.at(4) = atol(setRTData.m_VBuy5);
	m_lPSell.at(0) = static_cast<long>(atof(setRTData.m_PSell1) * 1000);
	m_lVSell.at(0) = atol(setRTData.m_VSell1);
	m_lPSell.at(1) = static_cast<long>(atof(setRTData.m_PSell2) * 1000);
	m_lVSell.at(1) = atol(setRTData.m_VSell2);
	m_lPSell.at(2) = static_cast<long>(atof(setRTData.m_PSell3) * 1000);
	m_lVSell.at(2) = atol(setRTData.m_VSell3);
	m_lPSell.at(3) = static_cast<long>(atof(setRTData.m_PSell4) * 1000);
	m_lVSell.at(3) = atol(setRTData.m_VSell4);
	m_lPSell.at(4) = static_cast<long>(atof(setRTData.m_PSell5) * 1000);
	m_lVSell.at(4) = atol(setRTData.m_VSell5);
}
