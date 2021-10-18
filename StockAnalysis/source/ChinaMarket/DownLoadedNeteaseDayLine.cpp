#include"pch.h"

#include"globedef.h"

#include"NeteaseDayLineWebInquiry.h"

#include"DownLoadedNeteaseDayLine.h"

CDownLoadedNeteaseDayLine::CDownLoadedNeteaseDayLine() {
	m_lDayLineBufferLength = 0;
	m_lCurrentPos = 0;
}

CDownLoadedNeteaseDayLine::~CDownLoadedNeteaseDayLine() {
}

bool CDownLoadedNeteaseDayLine::TransferNeteaseDayLineWebDataToBuffer(CNeteaseDayLineWebInquiry* pNeteaseWebDayLineData) {
	// 将读取的日线数据放入相关股票的日线数据缓冲区中，并设置相关标识。
	m_vDayLineBuffer.resize(pNeteaseWebDayLineData->GetByteReaded() + 1); // 缓冲区需要多加一个字符长度（最后那个0x000）。
	for (int i = 0; i < pNeteaseWebDayLineData->GetByteReaded() + 1; i++) {
		m_vDayLineBuffer.at(i) = pNeteaseWebDayLineData->GetData(i);
	}
	m_lDayLineBufferLength = pNeteaseWebDayLineData->GetByteReaded();
	m_strDownLoadStockSymbol = pNeteaseWebDayLineData->GetDownLoadingStockCode();
	m_lCurrentPos = 0;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理从网易日线服务器上读取的股票日线数据。
// 数据制式为： 日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CDownLoadedNeteaseDayLine::ProcessNeteaseDayLineData(void) {
	shared_ptr<CDayLine> pDayLine;

	if (m_lDayLineBufferLength == 0) { // 没有数据读入？此种状态是查询的股票为无效（不存在）号码
		return false;
	}

	ASSERT(m_vDayLineBuffer.at(m_lDayLineBufferLength) == 0x000); // 最后字符为增加的0x000.
	if (!SkipNeteaseDayLineInformationHeader()) {
		return false;
	}

	if (m_lCurrentPos >= m_lDayLineBufferLength) {// 无效股票号码，数据只有前缀说明，没有实际信息，或者退市了；或者已经更新了；或者是新股上市的第一天
		return false;
	}

	CString strTemp;
	while (m_lCurrentPos < m_lDayLineBufferLength) {
		m_pCurrentProcessingDayLine = nullptr;
		if (!ProcessOneNeteaseDayLineData()) { // 处理一条日线数据
			TRACE(_T("%s日线数据出错\n"), m_strDownLoadStockSymbol.GetBuffer());
			// 清除已暂存的日线数据
			m_vTempDayLine.clear();
			return false; // 数据出错，放弃载入
		}
		m_vTempDayLine.push_back(m_pCurrentProcessingDayLine); // 暂存于临时vector中，因为网易日线数据的时间顺序是颠倒的，最新的在最前面
	}
	ReportDayLineDownLoaded();

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理一条日线数据。采用网易日线历史数据格式。
//
// 与实时数据相类似，各种价格皆放大一千倍后以长整型存储。存入数据库时以DECIMAL(10,3)类型存储。
// 字符串的制式为：2019-07-10,600000,浦东银行,收盘价,最高价,最低价,开盘价,前收盘价,涨跌值,涨跌比率,换手率,成交股数,成交金额,总市值,流通市值\r\n
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDownLoadedNeteaseDayLine::ProcessOneNeteaseDayLineData(void) {
	static char buffer2[200], buffer3[100];
	long i = 0;
	long lMarketDate = 0;
	int year = 0, month = 0, day = 0;
	CString str;
	double dTemp = 0;

	ASSERT(m_pCurrentProcessingDayLine == nullptr);
	m_pCurrentProcessingDayLine = make_shared<CDayLine>();

	i = 0;
	while ((m_vDayLineBuffer.at(m_lCurrentPos) != 0x02c)) { // 读取日期，直到遇到逗号
		if ((m_vDayLineBuffer.at(m_lCurrentPos) == 0x0d) || (m_vDayLineBuffer.at(m_lCurrentPos) == 0x00a) || (m_vDayLineBuffer.at(m_lCurrentPos) == 0x000) || (i > 30)) { // 如果遇到回车、换行、字符串结束符或者读取了20个字符
			return false; // 数据出错，放弃载入
		}
		buffer3[i++] = m_vDayLineBuffer.at(m_lCurrentPos++);
	}
	m_lCurrentPos++;
	buffer3[i] = 0x00;
	sscanf_s(buffer3, _T("%04d-%02d-%02d"), &year, &month, &day);
	lMarketDate = year * 10000 + month * 100 + day;
	m_pCurrentProcessingDayLine->SetTime(gl_pChinaMarket->TransferToUTCTime(lMarketDate));
	m_pCurrentProcessingDayLine->SetDate(lMarketDate);
	//TRACE("%d %d %d\n", year, month, day);

	if (m_vDayLineBuffer.at(m_lCurrentPos) != 0x027) return(false); // 不是单引号(')，数据出错，放弃载入
	m_lCurrentPos++;

	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetStockSymbol(m_strDownLoadStockSymbol); // 读入的股票代码为600601、000001这样的制式，不再处理之，使用本股票的Symbol:600601.SS、000001.SZ直接赋值。

	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	str = buffer2;
	m_pCurrentProcessingDayLine->SetDisplaySymbol(str);

	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	dTemp = atof(buffer2);
	m_pCurrentProcessingDayLine->SetClose(dTemp * 1000);

	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	dTemp = atof(buffer2);
	m_pCurrentProcessingDayLine->SetHigh(dTemp * 1000);

	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	dTemp = atof(buffer2);
	m_pCurrentProcessingDayLine->SetLow(dTemp * 1000);

	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	dTemp = atof(buffer2);
	m_pCurrentProcessingDayLine->SetOpen(dTemp * 1000);

	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	dTemp = atof(buffer2);
	m_pCurrentProcessingDayLine->SetLastClose(dTemp * 1000);

	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	if (m_pCurrentProcessingDayLine->GetOpen() == 0) {
		//ASSERT(strcmp(buffer2, _T("None") == 0);
		m_pCurrentProcessingDayLine->SetUpDown(0.0);
	}
	else m_pCurrentProcessingDayLine->SetUpDown(buffer2);

	if (m_pCurrentProcessingDayLine->GetLastClose() == 0) { // 设置涨跌幅。
		m_pCurrentProcessingDayLine->SetUpDownRate(0.0); // 如果昨日收盘价为零（没交易），则涨跌幅也设为零。
	}
	else {
		// 需要放大1000 * 100倍。收盘价比实际值大1000倍，记录的是百分比，也要增大100倍。
		m_pCurrentProcessingDayLine->SetUpDownRate(((double)(m_pCurrentProcessingDayLine->GetUpDown() * 100000.0)) / m_pCurrentProcessingDayLine->GetLastClose());
	}

	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetChangeHandRate(buffer2);

	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetVolume(buffer2); // 读入的是股数

	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetAmount(buffer2);

	// 总市值的数据有两种形式，需要程序判定
	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetTotalValue(buffer2); // 总市值的单位为：元

	// 流通市值不是用逗号结束，故而不能使用ReadOneValueFromNeteaseDayLine函数
	// 流通市值的数据形式有两种，故而需要程序判定。
	i = 0;
	while (m_vDayLineBuffer.at(m_lCurrentPos) != 0x00d) {
		if ((m_vDayLineBuffer.at(m_lCurrentPos) == 0x00a) || (m_vDayLineBuffer.at(m_lCurrentPos) == 0x000) || (i > 30)) return false; // 数据出错，放弃载入
		buffer2[i++] = m_vDayLineBuffer.at(m_lCurrentPos++);
	}
	m_lCurrentPos++;
	buffer2[i] = 0x000;
	m_pCurrentProcessingDayLine->SetCurrentValue(buffer2); // 流通市值的单位为：元。
	// \r后面紧跟着应该是\n
	if (m_vDayLineBuffer.at(m_lCurrentPos++) != 0x0a) return false; // 数据出错，放弃载入

	return true;
}

bool CDownLoadedNeteaseDayLine::SkipNeteaseDayLineInformationHeader(void) {
	ASSERT(m_lCurrentPos == 0);
	while (m_vDayLineBuffer.at(m_lCurrentPos) != 0X0d) { // 寻找\r
		if (m_vDayLineBuffer.at(m_lCurrentPos) == 0x0a) {// 遇到\n
			m_lCurrentPos++; // 跨过此\n
			return false;
		}
		else if (m_vDayLineBuffer.at(m_lCurrentPos) == 0x000) { // 遇到0x000
			return false;
		}
		m_lCurrentPos++;
	}
	m_lCurrentPos++;
	if (m_vDayLineBuffer.at(m_lCurrentPos) != 0x0a) {
		return false;
	}
	m_lCurrentPos++;
	return true;
}

void CDownLoadedNeteaseDayLine::ReportDayLineDownLoaded(void) {
	//CString strTemp = GetSymbol();
	//strTemp += _T("日线下载完成.");
	//gl_systemMessage.PushDayLineInfoMessage(strTemp);
}