#include"pch.h"
#include "globedef.h"
#include"Accessory.h"
#include"WebRTData.h"
#include"ChinaMarket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CWebRTData::Reset(void) {
	m_lDataSource = __INVALID_RT_WEB_DATA__;
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

	m_mapNeteaseSymbolToIndex[_T("time")] = 1;
	m_mapNeteaseSymbolToIndex[_T("code")] = 2;
	m_mapNeteaseSymbolToIndex[_T("name")] = 3;
	m_mapNeteaseSymbolToIndex[_T("type")] = 4;
	m_mapNeteaseSymbolToIndex[_T("symbol")] = 5;
	m_mapNeteaseSymbolToIndex[_T("status")] = 6;
	m_mapNeteaseSymbolToIndex[_T("update")] = 7;
	m_mapNeteaseSymbolToIndex[_T("open")] = 10;
	m_mapNeteaseSymbolToIndex[_T("yestclose")] = 11;
	m_mapNeteaseSymbolToIndex[_T("high")] = 12;
	m_mapNeteaseSymbolToIndex[_T("low")] = 13;
	m_mapNeteaseSymbolToIndex[_T("price")] = 14;
	m_mapNeteaseSymbolToIndex[_T("volume")] = 15;
	m_mapNeteaseSymbolToIndex[_T("precloseioev")] = 16;
	m_mapNeteaseSymbolToIndex[_T("bid1")] = 20;
	m_mapNeteaseSymbolToIndex[_T("bid2")] = 21;
	m_mapNeteaseSymbolToIndex[_T("bid3")] = 22;
	m_mapNeteaseSymbolToIndex[_T("bid4")] = 23;
	m_mapNeteaseSymbolToIndex[_T("bid5")] = 24;
	m_mapNeteaseSymbolToIndex[_T("bidvol1")] = 30;
	m_mapNeteaseSymbolToIndex[_T("bidvol2")] = 31;
	m_mapNeteaseSymbolToIndex[_T("bidvol3")] = 32;
	m_mapNeteaseSymbolToIndex[_T("bidvol4")] = 33;
	m_mapNeteaseSymbolToIndex[_T("bidvol5")] = 34;
	m_mapNeteaseSymbolToIndex[_T("ask1")] = 40;
	m_mapNeteaseSymbolToIndex[_T("ask2")] = 41;
	m_mapNeteaseSymbolToIndex[_T("ask3")] = 42;
	m_mapNeteaseSymbolToIndex[_T("ask4")] = 43;
	m_mapNeteaseSymbolToIndex[_T("ask5")] = 44;
	m_mapNeteaseSymbolToIndex[_T("askvol1")] = 50;
	m_mapNeteaseSymbolToIndex[_T("askvol2")] = 51;
	m_mapNeteaseSymbolToIndex[_T("askvol3")] = 52;
	m_mapNeteaseSymbolToIndex[_T("askvol4")] = 53;
	m_mapNeteaseSymbolToIndex[_T("askvol5")] = 54;
	m_mapNeteaseSymbolToIndex[_T("percent")] = 60;
	m_mapNeteaseSymbolToIndex[_T("updown")] = 61;
	m_mapNeteaseSymbolToIndex[_T("arrow")] = 62;
	m_mapNeteaseSymbolToIndex[_T("turnover")] = 63;
}

CWebRTData::CWebRTData(void) : CObject() {
	Reset();
}

/////////////////////////////////////////////////////////////////////////////
// CWebRTData diagnostics

#ifdef _DEBUG
void CWebRTData::AssertValid() const {
	CObject::AssertValid();
}

void CWebRTData::Dump(CDumpContext& dc) const {
	CObject::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 从网络文件file中读取新浪制式实时数据，返回值是所读数据是否出现格式错误。
//
//  新浪实时行情站点：http://hq.sinajs.cn/list=sh601006
// var hq_str_sh601006=”大秦铁路,27.55,27.25,26.91,27.55,26.20,26.91,26.92,
//                     22114263,589824680,4695,26.91,57590,26.90,14700,26.89,14300,
//                     26.88,15100,26.87,3100,26.92,8900,26.93,14230,26.94,25150,26.95,15220,26.96,2008-01-11,15:05:32,00”;
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
// 30：”2008 - 01 - 11″，日期；
// 31：”15:05:32″，时间；
// 32：”00”，  不明数据
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadSinaData(CWebDataPtr pSinaWebRTData) {
	static char buffer1[100];
	char buffer2[7];
	static char buffer3[100];
	static CString strHeader = _T("var hq_str_s");
	long lStockCode = 0;
	double dTemp = 0;
	char bufferTest[2000];
	CString strTest;
	CString strStockSymbol = _T("");
	WORD wMarket;
	CString strSinaStockCode;

	int i = 0;
	while ((pSinaWebRTData->GetData(i + pSinaWebRTData->GetCurrentPos()) != ';') && (i < 1900)) {
		bufferTest[i] = pSinaWebRTData->GetData(i + pSinaWebRTData->GetCurrentPos());
		i++;
	}
	bufferTest[i] = pSinaWebRTData->GetData(i + pSinaWebRTData->GetCurrentPos());
	i++;
	bufferTest[i] = 0x000;
	strTest = bufferTest;
	if (i >= 1900) {
		TRACE(_T("%s\n"), strTest.GetBuffer());
		gl_systemMessage.PushInnerSystemInformationMessage(_T("整体数据出问题，抛掉不用"));
		gl_systemMessage.PushInnerSystemInformationMessage(strTest);
		return false; // 整个数据出现错误，后面的皆抛掉
	}

	try {
		m_fActive = false;    // 初始状态为无效数据
		pSinaWebRTData->GetData(buffer1, 12, pSinaWebRTData->GetCurrentPos()); // 读入“var hq_str_s"
		buffer1[12] = 0x000;
		CString str1;
		str1 = buffer1;
		if (strHeader.Compare(str1) != 0) { // 数据格式出错
			throw exception();
		}
		pSinaWebRTData->IncreaseCurrentPos(12);

		if (pSinaWebRTData->GetCurrentPosData() == 'h') { // 上海股票
			wMarket = __SHANGHAI_MARKET__; // 上海股票标识
		}
		else if (pSinaWebRTData->GetCurrentPosData() == 'z') {
			wMarket = __SHENZHEN_MARKET__; // 深圳股票标识
		}
		else {
			throw exception();
		}
		pSinaWebRTData->IncreaseCurrentPos();

		pSinaWebRTData->GetData(buffer2, 6, pSinaWebRTData->GetCurrentPos());
		buffer2[6] = 0x000;
		strStockSymbol = buffer2;
		switch (wMarket) {
		case __SHANGHAI_MARKET__:
			strSinaStockCode = _T("sh") + strStockSymbol; // 由于上海深圳股票代码有重叠，故而所有的股票代码都带上市场前缀。上海为sh
			break;
		case __SHENZHEN_MARKET__:
			strSinaStockCode = _T("sz") + strStockSymbol;// 由于上海深圳股票代码有重叠，故而所有的股票代码都带上市场前缀。深圳为sz
			break;
		default:
			throw exception();
		}
		m_strSymbol = XferSinaToStandred(strSinaStockCode);
		lStockCode = static_cast<long>(atof(buffer2));
		pSinaWebRTData->IncreaseCurrentPos(6);

		pSinaWebRTData->GetData(buffer1, 2, pSinaWebRTData->GetCurrentPos()); // 读入'="'
		if ((buffer1[0] != '=') || (buffer1[1] != '"')) {
			throw exception();
		}
		pSinaWebRTData->IncreaseCurrentPos(2);
		pSinaWebRTData->GetData(buffer1, 2, pSinaWebRTData->GetCurrentPos());
		if (buffer1[0] == '"') { // 没有数据?
			if (buffer1[1] != ';') {
				throw exception();
			}
			pSinaWebRTData->IncreaseCurrentPos(2);
			if (pSinaWebRTData->GetCurrentPosData() != 0x00a) {
				return false;
			}
			pSinaWebRTData->IncreaseCurrentPos();
			m_fActive = false;
			SetDataSource(__SINA_RT_WEB_DATA__);
			return true;  // 非活跃股票没有实时数据，在此返回。
		}
		if ((buffer1[0] == 0x00a) || (buffer1[0] == 0x000)) {
			throw exception();
		}
		if ((buffer1[1] == 0x00a) || (buffer1[1] == 0x000)) {
			throw exception();
		}
		pSinaWebRTData->IncreaseCurrentPos(2);

		i = 2;
		while ((pSinaWebRTData->GetCurrentPosData() != ',') && (i < 10)) { // 读入剩下的中文名字（第一个字在buffer1中）
			if ((pSinaWebRTData->GetCurrentPosData() == 0x00a) || (pSinaWebRTData->GetCurrentPosData() == 0x000)) {
				throw exception();
			}
			buffer1[i++] = pSinaWebRTData->GetCurrentPosData();
			pSinaWebRTData->IncreaseCurrentPos();
		}
		buffer1[i] = 0x000;
		m_strStockName = buffer1; // 设置股票名称

		pSinaWebRTData->IncreaseCurrentPos();

		// 读入开盘价。放大一千倍后存储为长整型。其他价格亦如此。
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
			throw exception();
		}
		m_lOpen = static_cast<long>((dTemp + 0.000001) * 1000);
		// 读入前收盘价
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
			throw exception();
		}
		m_lLastClose = static_cast<long>((dTemp + 0.000001) * 1000);
		// 读入当前价
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
			throw exception();
		}
		m_lNew = static_cast<long>((dTemp + 0.000001) * 1000);
		// 读入最高价
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
			throw exception();
		}
		m_lHigh = static_cast<long>((dTemp + 0.000001) * 1000);
		// 读入最低价
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
			throw exception();
		}
		m_lLow = static_cast<long>((dTemp + 0.000001) * 1000);
		// 读入竞买价
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
			throw exception();
		}
		m_lBuy = static_cast<long>((dTemp + 0.000001) * 1000);
		// 读入竞卖价
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
			throw exception();
		}
		m_lSell = static_cast<long>((dTemp + 0.000001) * 1000);
		// 读入成交股数。成交股数存储实际值
		if (!ReadSinaOneValue(pSinaWebRTData, m_llVolume)) {
			throw exception();
		}
		// 读入成交金额
		if (!ReadSinaOneValue(pSinaWebRTData, m_llAmount)) {
			throw exception();
		}
		// 读入买一--买五的股数和价格
		for (int j = 0; j < 5; j++) {
			// 读入数量
			if (!ReadSinaOneValue(pSinaWebRTData, m_lVBuy.at(j))) {
				throw exception();
			}
			// 读入价格
			if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
				throw exception();
			}
			m_lPBuy.at(j) = static_cast<long>((dTemp + 0.000001) * 1000);
		}
		// 读入卖一--卖五的股数和价格
		for (int j = 0; j < 5; j++) {
			// 读入数量
			if (!ReadSinaOneValue(pSinaWebRTData, m_lVSell.at(j))) {
				throw exception();
			}
			// 读入价格
			if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
				throw exception();
			}
			m_lPSell.at(j) = static_cast<long>((dTemp + 0.000001) * 1000);
		}
		// 读入成交日期和时间。此时间为东八区（北京标准时间）。
		if (!ReadSinaOneValue(pSinaWebRTData, buffer1)) {
			throw exception();
		}
		CString strTime;
		strTime = buffer1;
		strTime += ' '; //添加一个空格，以利于下面的转换
		if (!ReadSinaOneValue(pSinaWebRTData, buffer3)) {
			throw exception();
		}
		strTime += buffer3;
		m_time = ConvertBufferToTime("%04d-%02d-%02d %02d:%02d:%02d", strTime.GetBuffer());

		// 后面的数据皆为无效数据，读至此数据的结尾处即可。
		while (pSinaWebRTData->GetCurrentPosData() != 0x00a) { // 寻找字符'\n'（回车符）
			pSinaWebRTData->IncreaseCurrentPos();
			if (pSinaWebRTData->GetCurrentPosData() == 0x000) {
				throw exception();
			}
		}
		pSinaWebRTData->IncreaseCurrentPos(); // 读过字符'\n'
		// 判断此实时数据是否有效，可以在此判断，结果就是今日有效股票数会减少（退市的股票有数据，但其值皆为零，而生成今日活动股票池时需要实时数据是有效的）。
		// 0.03版本和其之前的都没有做判断，0.04版本还是使用不判断的这种吧。
		// 在系统准备完毕前就判断新浪活跃股票数，只使用成交时间一项，故而依然存在非活跃股票在其中。
		// 0.07版后，采用十四天内的实时数据为活跃股票数据（最长的春节放假七天，加上前后的休息日，共十天，宽限四天）
		CheckSinaRTDataActive();
		SetDataSource(__SINA_RT_WEB_DATA__);
		return true;
	}
	catch (exception&) {
		TRACE(_T("ReadSinaData异常\n"));
		CString str = m_strSymbol;
		str += _T(" ReadSinaData异常\n");
		gl_systemMessage.PushInnerSystemInformationMessage(str);
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
bool CWebRTData::ReadSinaOneValue(CWebDataPtr pSinaWebRTData, INT64& llReturnValue) {
	INT64 llTemp;
	static char buffer3[200];

	if (!ReadSinaOneValue(pSinaWebRTData, buffer3)) {
		return false;
	}
	llTemp = static_cast<INT64>(atof(buffer3));
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
bool CWebRTData::ReadSinaOneValue(CWebDataPtr pSinaWebRTData, long& lReturnValue) {
	long lTemp;
	static char buffer3[200];

	if (!ReadSinaOneValue(pSinaWebRTData, buffer3)) {
		return false;
	}
	lTemp = static_cast<long>(atof(buffer3));
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
bool CWebRTData::ReadSinaOneValue(CWebDataPtr pSinaWebRTData, double& dReturnValue) {
	static char buffer3[200];

	if (!ReadSinaOneValue(pSinaWebRTData, buffer3)) {
		return false;
	}
	dReturnValue = atof(buffer3);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个值，遇到逗号结束。
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadSinaOneValue(CWebDataPtr pSinaWebRTData, char* buffer) {
	int i = 0;
	try {
		while ((pSinaWebRTData->GetCurrentPosData() != ',')) {
			if ((pSinaWebRTData->GetCurrentPosData() == 0x00a) || (pSinaWebRTData->GetCurrentPosData() == 0x000)) throw exception();
			if (i > 150) throw exception();
			buffer[i++] = pSinaWebRTData->GetCurrentPosData();
			pSinaWebRTData->IncreaseCurrentPos();
		}
		buffer[i] = 0x000;
		// 跨过','号。
		pSinaWebRTData->IncreaseCurrentPos();

		return true;
	}
	catch (exception&) {
		TRACE(_T("ReadSinaOneValue异常\n"));
		CString str = _T("ReadSinaOneValue异常\n");
		gl_systemMessage.PushInnerSystemInformationMessage(str);
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
// 30 : 时间
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
bool CWebRTData::ReadTengxunData(CWebDataPtr pTengxunWebRTData) {
	static char buffer1[200];
	char buffer2[7];
	static char buffer3[200];
	static CString strHeader = _T("v_s");
	long lTemp = 0;
	INT64 llTemp = 0;
	double dTemp = 0.0;
	float fTemp = 0.0;
	long lStockCode = 0;
	WORD wMarket;
	CString strStockSymbol, strTengxunStockCode;

	try {
		m_fActive = false;    // 初始状态为无效数据
		pTengxunWebRTData->GetData(buffer1, 3, pTengxunWebRTData->GetCurrentPos()); // 读入“v_s"
		buffer1[3] = 0x000;
		CString str1;
		str1 = buffer1;
		if (strHeader.Compare(str1) != 0) { // 数据格式出错
			return false;
		}
		pTengxunWebRTData->IncreaseCurrentPos(3);
		if (pTengxunWebRTData->GetCurrentPosData() == 'h') { // 上海股票
			wMarket = __SHANGHAI_MARKET__; // 上海股票标识
		}
		else if (pTengxunWebRTData->GetCurrentPosData() == 'z') {
			wMarket = __SHENZHEN_MARKET__; // 深圳股票标识
		}
		else {
			return false;
		}
		pTengxunWebRTData->IncreaseCurrentPos();

		// 六位股票代码
		pTengxunWebRTData->GetData(buffer2, 6, pTengxunWebRTData->GetCurrentPos());
		buffer2[6] = 0x000;
		strStockSymbol = buffer2;
		switch (wMarket) {
		case __SHANGHAI_MARKET__:
			strTengxunStockCode = _T("sh") + strStockSymbol; // 由于上海深圳股票代码有重叠，故而所有的股票代码都带上市场前缀。上海为sh
			break;
		case __SHENZHEN_MARKET__:
			strTengxunStockCode = _T("sz") + strStockSymbol;// 由于上海深圳股票代码有重叠，故而所有的股票代码都带上市场前缀。深圳为sz
			break;
		default:
			return false;
		}
		m_strSymbol = XferTengxunToStandred(strTengxunStockCode);
		lStockCode = atoi(buffer2);
		pTengxunWebRTData->IncreaseCurrentPos(6);

		pTengxunWebRTData->GetData(buffer1, 2, pTengxunWebRTData->GetCurrentPos()); // 读入'="'
		if (buffer1[0] != '=') {
			return false;
		}
		if (buffer1[1] != '"') {
			return false;
		}
		pTengxunWebRTData->IncreaseCurrentPos(2);

		// 市场标识代码（51为深市，1为沪市）
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			return false;
		}
#ifdef DEBUG
		if (lTemp == 1) ASSERT(wMarket == __SHANGHAI_MARKET__);
		else if (lTemp == 51) ASSERT(wMarket == __SHENZHEN_MARKET__);
		else ASSERT(0); // 报错
#endif
		if (!ReadTengxunOneValue(pTengxunWebRTData, buffer1)) {
			return false;
		}
		m_strStockName = buffer1; // 设置股票名称
		// 六位股票代码
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			return false;
		}
		if (lTemp != lStockCode) return false;

		// 现在成交价。放大一千倍后存储为长整型。其他价格亦如此。
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		m_lNew = static_cast<long>((dTemp + 0.000001) * 1000);
		// 前收盘价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		m_lLastClose = static_cast<long>((dTemp + 0.000001) * 1000);
		// 开盘价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		m_lOpen = static_cast<long>((dTemp + 0.000001) * 1000);
		// 成交手数。成交股数存储实际值
		// 不使用此处的成交量，而是使用第三十五项处的成交量。
		if (!ReadTengxunOneValue(pTengxunWebRTData, llTemp)) {
			return false;
		}
		// 外盘
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			return false;
		}
		// 内盘
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			return false;
		}
		// 读入买一至买五的价格和手数
		for (int j = 0; j < 5; j++) {
			// 买盘价格
			if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
				return false;
			}
			m_lPBuy.at(j) = static_cast<long>((dTemp + 0.000001) * 1000);

			// 买盘数量（手）
			if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
				return false;
			}
			m_lVBuy.at(j) = lTemp * 100;
		}
		// 读入卖一至卖五的价格和手数
		for (int j = 0; j < 5; j++) {
			//读入卖盘价格
			if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
				return false;
			}
			m_lPSell.at(j) = static_cast<long>((dTemp + 0.000001) * 1000);
			// 卖盘数量（手）
			if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
				return false;
			}
			m_lVSell.at(j) = lTemp * 100;
		}
		// 最近逐笔成交
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			return false;
		}
		// 成交日期和时间.格式为：yyyymmddhhmmss. 此时间采用的时东八区（北京标准时间）
		if (!ReadTengxunOneValue(pTengxunWebRTData, buffer3)) {
			return false;
		}
		m_time = ConvertBufferToTime("%04d%02d%02d%02d%02d%02d", buffer3);
		// 涨跌
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		// 涨跌率
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		// 最高价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		m_lHigh = static_cast<long>((dTemp + 0.000001) * 1000);
		// 最低价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		m_lLow = static_cast<long>((dTemp + 0.000001) * 1000);
		// 第三十五项，成交价/成交量（手）/成交金额（元）
		// 成交量和成交金额使用此处的数据，这样就可以使用腾讯实时数据了
		if (!ReadTengxunOneValue(pTengxunWebRTData, buffer3)) {
			return false;
		}
		sscanf_s(buffer3, _T("%f/%d/%I64d"), &fTemp, &lTemp, &m_llAmount);
		m_llVolume = lTemp * 100; // 腾讯成交量数据单位为手（100股）。
		// 成交手数
		// 不使用此处的成交量。这里的成交量会大于第三十五处的成交量。
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			return false;
		}
		// 成交金额（万元）
		if (!ReadTengxunOneValue(pTengxunWebRTData, llTemp)) {
			return false;
		}
		// 换手率
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		// 市盈率
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		// 无名
		if (!ReadTengxunOneValue(pTengxunWebRTData, buffer3)) {
			return false;
		}
		// 最高价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		// 最低价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		// 振幅
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		// 流通市值（单位为：亿元）
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		m_llCurrentValue = static_cast<INT64>(dTemp * 100000000);
		// 总市值（单位为：亿元）
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		m_llTotalValue = static_cast<INT64>(dTemp * 100000000);
		// 市净率
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		// 涨停价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		if (dTemp > 0.01) m_lHighLimit = static_cast<long>((dTemp + 0.000001) * 1000);
		// 跌停价
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			return false;
		}
		if (dTemp > 0.01) m_lLowLimit = static_cast<long>((dTemp + 0.000001) * 1000);

		// 后面的数据具体内容不清楚，暂时放弃解码。
		while (pTengxunWebRTData->GetCurrentPosData() != 0x00a) {
			pTengxunWebRTData->IncreaseCurrentPos();
			if (pTengxunWebRTData->GetCurrentPosData() == 0x000) {
				return false;
			}
		}
		pTengxunWebRTData->IncreaseCurrentPos();
		CheckTengxunRTDataActive();
		SetDataSource(__TENGXUN_RT_WEB_DATA__);
		return true;
	}
	catch (exception&) {
		TRACE(_T("ReadTengxunData异常\n"));
		return false;
	}
}

bool CWebRTData::CheckTengxunRTDataActive() {
	if (!IsValidTime(14)) { // 如果交易时间在14天前
		m_fActive = false;
	}
	else if ((m_lOpen == 0) && (m_llVolume == 0) && (m_lHigh == 0) && (m_lLow == 0)) { // 腾讯非活跃股票的m_lNew不为零，故而不能使用其作为判断依据
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
bool CWebRTData::ReadTengxunOneValue(CWebDataPtr pTengxunWebRTData, INT64& llReturnValue) {
	INT64 llTemp;
	static char buffer3[200];

	if (!ReadTengxunOneValue(pTengxunWebRTData, buffer3)) {
		return false;
	}
	llTemp = atoll(buffer3);
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
bool CWebRTData::ReadTengxunOneValue(CWebDataPtr pTengxunWebRTData, double& dReturnValue) {
	double dTemp;
	static char buffer3[200];

	if (!ReadTengxunOneValue(pTengxunWebRTData, buffer3)) {
		return false;
	}
	dTemp = atof(buffer3);
	dReturnValue = dTemp;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入一个长整型值，遇到~号结束。返回值在lReturnValue中
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadTengxunOneValue(CWebDataPtr pWebDataReceived, long& lReturnValue) {
	long lTemp;
	static char buffer3[200];

	if (!ReadTengxunOneValue(pWebDataReceived, buffer3)) {
		return false;
	}
	lTemp = atol(buffer3);
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
bool CWebRTData::ReadTengxunOneValue(CWebDataPtr pWebDataReceived, char* buffer) {
	int i = 0;
	try {
		while (pWebDataReceived->GetCurrentPosData() != '~') {
			if ((pWebDataReceived->GetCurrentPosData() == 0x00a) || (pWebDataReceived->GetCurrentPosData() == 0x000)) return false;
			buffer[i++] = pWebDataReceived->GetCurrentPosData();
			pWebDataReceived->IncreaseCurrentPos();
		}
		buffer[i] = 0x000;
		pWebDataReceived->IncreaseCurrentPos();
		return true;
	}
	catch (exception&) {
		TRACE("ReadTengxunOneValue函数异常\n");
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 从网络文件file中读取网易制式实时数据，返回值是所读数据是否出现格式错误。
// 开始处为第一个{，结束处为倒数第二个}。如果尚有数据需处理，则被处理的字符为','；如果没有数据了，则被处理的字符为' '。
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
// 网易实时数据缺少关键性的成交金额一项，故而无法作为基本数据，只能作为补充用。
// （turnover即为成交金额，可以使用之。05/12/2020）
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadNeteaseData(CWebDataPtr pNeteaseWebRTData) {
	long lIndex = 0;
	CString strValue = _T("");
	char bufferTest[2000];
	long lSectionBegin = pNeteaseWebRTData->GetCurrentPos();
	CString strStockCode = _T(" "), strHeader;
	long lSectionLength = 0;
	CString strTest;
	CString strStockSymbol, strNeteaseStockCode;

	int i = 0;
	while ((pNeteaseWebRTData->GetData(pNeteaseWebRTData->GetCurrentPos() + i) != '}') && (i < 1900)) {
		bufferTest[i] = pNeteaseWebRTData->GetData(pNeteaseWebRTData->GetCurrentPos() + i);
		i++;
	}
	bufferTest[i] = pNeteaseWebRTData->GetData(pNeteaseWebRTData->GetCurrentPos() + i);
	i++;
	lSectionLength = i;
	bufferTest[i] = 0x000;
	strTest = bufferTest;
	if (i >= 1900) {
		gl_systemMessage.PushInnerSystemInformationMessage(_T("整体数据出问题，抛掉不用"));
		gl_systemMessage.PushInnerSystemInformationMessage(strTest);
		return false; // 整个数据出现错误，后面的皆抛掉
	}

	char ch = pNeteaseWebRTData->GetData(pNeteaseWebRTData->GetCurrentPos() + lSectionLength - 1);
	ASSERT(ch == '}');
	ASSERT(bufferTest[i - 1] == '}');

	try {
		m_fActive = false;    // 初始状态为无效数据
		// 跨过前缀字符（"0601872")，直接使用其后的数据
		if (!ReadNeteaseStockCodePrefix(pNeteaseWebRTData)) {
			throw exception();
		}
		do {
			if (GetNeteaseIndexAndValue(pNeteaseWebRTData, lIndex, strValue)) {
				if (!SetNeteaseRTValue(lIndex, strValue)) throw exception();
			}
			else {
				throw exception();
			}
		} while ((lIndex != 63) && (pNeteaseWebRTData->GetCurrentPosData() != '}'));  // 读至turnover(63)或者遇到字符'}'
		// 读过'}'就结束了
		while (pNeteaseWebRTData->GetCurrentPosData() != '}') {
			pNeteaseWebRTData->IncreaseCurrentPos();
		}
		ASSERT(pNeteaseWebRTData->GetCurrentPosData() == '}');
		pNeteaseWebRTData->IncreaseCurrentPos();

		CheckNeteaseRTDataActive();
		SetDataSource(__NETEASE_RT_WEB_DATA__);
		return true;
	}
	catch (exception&) {
		TRACE(_T("%s's ReadNeteaseData异常\n"), strStockCode.GetBuffer());
		CString str = _T("ReadNeteaseData异常");
		gl_systemMessage.PushInnerSystemInformationMessage(str);
		TRACE(_T("%s\n"), strTest.GetBuffer());
#ifdef DEBUG
		gl_systemMessage.PushInnerSystemInformationMessage(strTest);
#endif // DEBUG
		m_fActive = false;
		// 跨过此错误数据，寻找下一个数据的起始处。
		pNeteaseWebRTData->SetCurrentPos(lSectionBegin + lSectionLength);
		ASSERT(pNeteaseWebRTData->GetData(pNeteaseWebRTData->GetCurrentPos() - 1) == '}');
		SetDataSource(__NETEASE_RT_WEB_DATA__);
		return true; // 返回真，则跨过此错误数据，继续处理。
	}
}

bool CWebRTData::CheckNeteaseRTDataActive(void) {
	if (!IsValidTime(14)) { // 非活跃股票的update时间为0，转换为time_t时为-1.
		m_fActive = false;
	}
	else {
		if ((m_lOpen == 0) && (m_llVolume == 0) && (m_lHigh == 0) && (m_lLow == 0) && (m_lLastClose == 0)) {
			m_fActive = false; // 网易非活跃股票的实时数据也具有所有的字段，故而在此确认其为非活跃
		}
		else m_fActive = true;
	}
	return m_fActive;
}

bool CWebRTData::ReadNeteaseStockCodePrefix(CWebDataPtr pWebDataReceived) {
	CString strValue = _T("");
	char bufferStockCode[50];
	char bufferTest[30];
	bool fFind = false;
	CString strStockCode, strHeader;
	CString strTest;

	int i = 0;
	while ((pWebDataReceived->GetData(pWebDataReceived->GetCurrentPos() + i) != '{') && (i < 20)) {
		bufferTest[i] = pWebDataReceived->GetData(pWebDataReceived->GetCurrentPos() + i);
		i++;
	}
	bufferTest[i] = 0x000;
	strTest = bufferTest;

	i = 0;  // 跨过前缀字符（"0601872")，直接使用其后的数据
	if (!((pWebDataReceived->GetCurrentPosData() == '{') || (pWebDataReceived->GetCurrentPosData() == ','))) {
		return false;
	}
	else pWebDataReceived->IncreaseCurrentPos();
	if (pWebDataReceived->GetCurrentPosData() != '\"') {
		return false;
	}
	else pWebDataReceived->IncreaseCurrentPos();
	if (pWebDataReceived->GetCurrentPosData() == '0') strHeader = _T("sh");
	else if (pWebDataReceived->GetCurrentPosData() == '1') strHeader = _T("sz");
	else {
		return false;
	}
	pWebDataReceived->IncreaseCurrentPos();
	i = 0;
	while (!fFind && (i < 13)) {
		if (pWebDataReceived->GetCurrentPosData() == '"') {
			fFind = true;
			bufferStockCode[i] = 0x000;
		}
		else {
			bufferStockCode[i++] = pWebDataReceived->GetCurrentPosData();
		}
		pWebDataReceived->IncreaseCurrentPos();
	}
	if (!fFind) return false;
	i = 1;
	while ((pWebDataReceived->GetCurrentPosData() != '{') && (pWebDataReceived->GetCurrentPosData() != '"' && (i < 5))) {
		i++;
		pWebDataReceived->IncreaseCurrentPos();
	}
	if (i >= 5) return false;
	pWebDataReceived->IncreaseCurrentPos();
	strStockCode = strHeader;
	strStockCode += bufferStockCode;
	strStockCode = XferSinaToStandred(strStockCode);
	if (!gl_pChinaMarket->IsStock(strStockCode)) {
		return false;
	}
	return true;
}

long CWebRTData::GetNeteaseSymbolIndex(CString strSymbol) {
	long lIndex = 0;
	try {
		lIndex = m_mapNeteaseSymbolToIndex.at(strSymbol);
	}
	catch (exception&) {
		TRACE(_T("GetNeteaseSymbolIndex异常: %s\n"), strSymbol.GetBuffer());
		lIndex = 0;
	}
	return lIndex;
}

bool CWebRTData::GetNeteaseIndexAndValue(CWebDataPtr pNeteaseWebRTData, long& lIndex, CString& strValue) {
	char buffer[100];
	int i = 0;
	CString strIndex;
	bool fFind = false;
	char bufferTest[100];

	try {
		while (pNeteaseWebRTData->GetCurrentPosData() != '"') {
			pNeteaseWebRTData->IncreaseCurrentPos();
		}
		pNeteaseWebRTData->IncreaseCurrentPos();

		while ((pNeteaseWebRTData->GetData(pNeteaseWebRTData->GetCurrentPos() + i) != '}') && (pNeteaseWebRTData->GetData(pNeteaseWebRTData->GetCurrentPos() + i) != ',') && (i < 99)) {
			bufferTest[i] = pNeteaseWebRTData->GetData(pNeteaseWebRTData->GetCurrentPos() + i);
			i++;
		}
		bufferTest[i] = 0x000;

		i = 0;
		while ((pNeteaseWebRTData->GetCurrentPosData() != '"') && (pNeteaseWebRTData->GetCurrentPosData() != ':') && (pNeteaseWebRTData->GetCurrentPosData() != ',')) {
			buffer[i++] = pNeteaseWebRTData->GetCurrentPosData();
			pNeteaseWebRTData->IncreaseCurrentPos();
		}
		if (pNeteaseWebRTData->GetCurrentPosData() != '"') {
			TRACE(_T("未遇到正确字符'\"'\n"));
			return false;
		}
		buffer[i] = 0x000;
		strIndex = buffer;
		if ((lIndex = GetNeteaseSymbolIndex(strIndex)) == 0) throw exception();
		// 跨过"\""字符
		pNeteaseWebRTData->IncreaseCurrentPos();
		if (pNeteaseWebRTData->GetCurrentPosData() != ':') {
			TRACE(_T("未遇到正确字符':'\n"));
			return false;
		}
		pNeteaseWebRTData->IncreaseCurrentPos();
		if (pNeteaseWebRTData->GetCurrentPosData() != ' ') {
			TRACE(_T("未遇到正确字符' '\n"));
			return false;
		}
		pNeteaseWebRTData->IncreaseCurrentPos();

		if (pNeteaseWebRTData->GetCurrentPosData() == '"') {
			fFind = true;
			pNeteaseWebRTData->IncreaseCurrentPos();
		}
		else fFind = false;

		i = 0;
		if (fFind) {
			while ((pNeteaseWebRTData->GetCurrentPosData() != '"') && (pNeteaseWebRTData->GetCurrentPosData() != ',')) {
				buffer[i++] = pNeteaseWebRTData->GetCurrentPosData();
				pNeteaseWebRTData->IncreaseCurrentPos();
			}
			if (pNeteaseWebRTData->GetCurrentPosData() != '"') {
				TRACE(_T("未遇到正确字符'\"'\n"));
				return false;
			}
			buffer[i] = 0x000;
			strValue = buffer;
			pNeteaseWebRTData->IncreaseCurrentPos();
		}
		else {
			while ((pNeteaseWebRTData->GetCurrentPosData() != ',') && (pNeteaseWebRTData->GetCurrentPosData() != '}')) {
				buffer[i++] = pNeteaseWebRTData->GetCurrentPosData();
				pNeteaseWebRTData->IncreaseCurrentPos();
			}
			buffer[i] = 0x000;
			strValue = buffer;
		}
		return true;
	}
	catch (exception&) {
		TRACE(_T("GetNeteaseIndexAndValue Exception\n"));
		lIndex = 0;
		strValue = _T("");
		return false;
	}
}

bool CWebRTData::SetNeteaseRTValue(long lIndex, CString strValue) {
	CString str1, str;
	time_t ttTemp = 0;
	WORD wMarket;

	switch (lIndex) {
	case 1: // time. 这个时间可能是实际的成交时间。需要与Update时间比较，采用较早的时间。此时间为东八区（北京标准时间）
		if (m_time > 0) { // 设置过？
			ttTemp = ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strValue);
			if (m_time > ttTemp) m_time = ttTemp;
		}
		else {
			m_time = ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strValue);
		}
		break;
	case 2: // code
		ASSERT(strValue.GetLength() == 7);
		str = strValue.Left(1);
		if (str.Compare(_T("0")) == 0) {
			str1 = _T("SS");
		}
		else str1 = _T("SZ");
		m_strSymbol = CreateStockCode(str1, strValue.Right(6));
		break;
	case 3: // name。网易的股票名称，采用的格式目前尚不清楚，暂时不用。
		//m_strStockName = buffer;
		break;
	case 4: // type
		if (strValue.Compare(_T("SH")) == 0) wMarket = __SHANGHAI_MARKET__;
		else wMarket = __SHENZHEN_MARKET__;
		break;
	case 5: // symbol
		break;
	case 6: // status
		break;
	case 7: // update。 这个时间估计是交易所发布此交易的时间，比实际交易时间可能要晚。采用较早的时间。此时间为东八区（北京标准时间）
		if (m_time > 0) { // 设置过？
			ttTemp = ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strValue);
			if (m_time > ttTemp) m_time = ttTemp;
		}
		else {
			m_time = ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strValue);
		}
		break;
	case 10: // open
		m_lOpen = static_cast<long>(atof(strValue) * 1000);
		break;
	case 11: // yestclose
		m_lLastClose = static_cast<long>(atof(strValue) * 1000);
		break;
	case 12: // high
		m_lHigh = static_cast<long>(atof(strValue) * 1000);
		break;
	case 13: // low
		m_lLow = static_cast<long>(atof(strValue) * 1000);
		break;
	case 14: // price
		m_lNew = static_cast<long>(atof(strValue) * 1000);
		break;
	case 15: // volume
		m_llVolume = atoll(strValue);
		break;
	case 20: // bid1
		m_lPBuy[0] = static_cast<long>(atof(strValue) * 1000);
		break;
	case 21: // bid2
		m_lPBuy[1] = static_cast<long>(atof(strValue) * 1000);
		break;
	case 22: // bid3
		m_lPBuy[2] = static_cast<long>(atof(strValue) * 1000);
		break;
	case 23: // bid4
		m_lPBuy[3] = static_cast<long>(atof(strValue) * 1000);
		break;
	case 24: // bid5
		m_lPBuy[4] = static_cast<long>(atof(strValue) * 1000);
		break;
	case 30: // bidvol1
		m_lVBuy[0] = atol(strValue);
		break;
	case 31: // bidvol2
		m_lVBuy[1] = atol(strValue);
		break;
	case 32: // bidvol3
		m_lVBuy[2] = atol(strValue);
		break;
	case 33: // bidvol4
		m_lVBuy[3] = atol(strValue);
		break;
	case 34: // bidvol5
		m_lVBuy[4] = atol(strValue);
		break;
	case 40: // ask1
		m_lPSell[0] = static_cast<long>(atof(strValue) * 1000);
		break;
	case 41: // ask2
		m_lPSell[1] = static_cast<long>(atof(strValue) * 1000);
		break;
	case 42: // ask3
		m_lPSell[2] = static_cast<long>(atof(strValue) * 1000);
		break;
	case 43: // ask4
		m_lPSell[3] = static_cast<long>(atof(strValue) * 1000);
		break;
	case 44: // ask5
		m_lPSell[4] = static_cast<long>(atof(strValue) * 1000);
		break;
	case 50: // askvol1
		m_lVSell[0] = atol(strValue);
		break;
	case 51: // askvol2
		m_lVSell[1] = atol(strValue);
		break;
	case 52: // askvol3
		m_lVSell[2] = atol(strValue);
		break;
	case 53: // askvol4
		m_lVSell[3] = atol(strValue);
		break;
	case 54: // askvol5
		m_lVSell[4] = atol(strValue);
		break;
	case 60: // percent

	case 61: // updown
	case 62: // arrow
	case 63: // turnover
		m_llAmount = atoll(strValue);
		break;
	default:
		// 出错了
		TRACE(_T("SetNeteaseRTValue异常， Index = %d strValue = %s\n"), lIndex, strValue.GetBuffer());
		return false;
		break;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 实时数据的有效时间范围为最近两周内。当股市放假时，其最新数据是放假前的最后一天数据。春节放假时间最长，有十一天时间，加上三天富裕，
// 故而十四天内的数据都被认为是有效时间数据，这样能够保证生成当日活动股票集。
//
//////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::IsValidTime(long lDays) const {
	if (m_time < (gl_pChinaMarket->GetMarketTime() - lDays * 24 * 3600)) { // 确保实时数据不早于当前时间的14天前（春节放假最长为7天，加上前后的休息日，共十一天）
		return false;
	}
	else if (m_time > gl_pChinaMarket->GetMarketTime()) {
		return false;
	}
	else  return true;
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