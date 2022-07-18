#include"pch.h"

#include"ChinaMarket.h"

#include"NeteaseDayLineWebInquiry.h"

#include"NeteaseDayLineWebData.h"

CNeteaseDayLineWebData::CNeteaseDayLineWebData() : CObject() {
	Reset();
}

CNeteaseDayLineWebData::~CNeteaseDayLineWebData() {
}

void CNeteaseDayLineWebData::Reset(void) {
	m_strStockCode = _T("");
	m_sDataBuffer.resize(0);
	m_vTempDayLine.resize(0);
	m_lBufferLength = 0;
	m_lCurrentPos = 0;

	m_pCurrentProcessingDayLine = nullptr;
}

bool CNeteaseDayLineWebData::TransferNeteaseDayLineWebDataToBuffer(CNeteaseDayLineWebInquiry* pNeteaseWebDayLineData) {
	// 将读取的日线数据放入相关股票的日线数据缓冲区中，并设置相关标识。
	m_sDataBuffer.resize(pNeteaseWebDayLineData->GetByteReaded());
	for (int i = 0; i < pNeteaseWebDayLineData->GetByteReaded(); i++) {
		m_sDataBuffer.at(i) = pNeteaseWebDayLineData->GetData(i);
	}
	m_lBufferLength = pNeteaseWebDayLineData->GetByteReaded();
	m_strStockCode = pNeteaseWebDayLineData->GetDownLoadingStockCode();
	m_lCurrentPos = 0;

	return true;
}

bool CNeteaseDayLineWebData::TransferWebDataToBuffer(CWebDataPtr pWebData) {
	// 将读取的日线数据放入相关股票的日线数据缓冲区中，并设置相关标识。
	m_sDataBuffer = std::move(pWebData->m_sDataBuffer);

	m_lBufferLength = pWebData->GetBufferLength();
	m_strStockCode = pWebData->GetStockCode();
	m_lCurrentPos = 0;
	ASSERT(m_sDataBuffer.size() == m_lBufferLength);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理从网易日线服务器上读取的股票日线数据。
// 数据制式为： 日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n
//
// 日线数据是逆序的，最新日期的在前面。
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CNeteaseDayLineWebData::ProcessNeteaseDayLineData(void) {
	shared_ptr<CDayLine> pDayLine;

	if (m_lBufferLength == 0) { // 没有数据读入？此种状态是查询的股票为无效（不存在）号码
		return false;
	}

	if (!SkipNeteaseDayLineInformationHeader()) {
		return false;
	}

	if (m_lCurrentPos >= m_lBufferLength) {// 无效股票号码，数据只有前缀说明，没有实际信息，或者退市了；或者已经更新了；或者是新股上市的第一天
		return false;
	}

	CString strTemp;
	while (m_lCurrentPos < m_lBufferLength) {
		m_pCurrentProcessingDayLine = nullptr;
		if (!ProcessOneNeteaseDayLineData()) { // 处理一条日线数据
			TRACE(_T("%s日线数据出错\n"), m_strStockCode.GetBuffer());
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
// 字符串的制式为：2019-07-10,600000,浦东银行,收盘价,最高价,最低价,开盘价,前收盘价,涨跌值,换手率,成交股数,成交金额,总市值,流通市值\r\n
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNeteaseDayLineWebData::ProcessOneNeteaseDayLineData(void) {
	char buffer2[200], buffer3[100];
	long i = 0;
	long lMarketDate = 0;
	int year = 0, month = 0, day = 0;
	CString str;
	double dTemp = 0;

	ASSERT(m_pCurrentProcessingDayLine == nullptr);
	m_pCurrentProcessingDayLine = make_shared<CDayLine>();

	i = 0;
	// 日期
	while ((m_sDataBuffer.at(m_lCurrentPos) != 0x02c)) { // 读取日期，直到遇到逗号
		if ((m_sDataBuffer.at(m_lCurrentPos) == 0x0d) || (m_sDataBuffer.at(m_lCurrentPos) == 0x00a) || (m_lCurrentPos >= m_lBufferLength) || (i > 30)) { // 如果遇到回车、换行、字符串结束符或者读取了20个字符
			return false; // 数据出错，放弃载入
		}
		buffer3[i++] = m_sDataBuffer.at(m_lCurrentPos++);
	}
	m_lCurrentPos++;
	buffer3[i] = 0x00;
	sscanf_s(buffer3, _T("%04d-%02d-%02d"), &year, &month, &day);
	lMarketDate = year * 10000 + month * 100 + day;
	m_pCurrentProcessingDayLine->SetTime(gl_pChinaMarket->TransferToUTCTime(lMarketDate));
	m_pCurrentProcessingDayLine->SetDate(lMarketDate);
	//TRACE("%d %d %d\n", year, month, day);

	if (m_sDataBuffer.at(m_lCurrentPos) != 0x027) return(false); // 不是单引号(')，数据出错，放弃载入
	m_lCurrentPos++;

	// 股票代码
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetStockSymbol(m_strStockCode); // 读入的股票代码为600601、000001这样的制式，不再处理之，使用本股票的Symbol:600601.SS、000001.SZ直接赋值。

	// 股票名称
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	str = buffer2;
	m_pCurrentProcessingDayLine->SetDisplaySymbol(str);

	// 收盘价
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	if (buffer2[0] != 0x000) {
		dTemp = atof(buffer2);
		m_pCurrentProcessingDayLine->SetClose(dTemp * 1000);
	}

	// 最高价
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	dTemp = atof(buffer2);
	m_pCurrentProcessingDayLine->SetHigh(dTemp * 1000);

	// 最低价
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	dTemp = atof(buffer2);
	m_pCurrentProcessingDayLine->SetLow(dTemp * 1000);

	// 开盘价
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	dTemp = atof(buffer2);
	m_pCurrentProcessingDayLine->SetOpen(dTemp * 1000);

	// 前收盘价
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	dTemp = atof(buffer2);
	m_pCurrentProcessingDayLine->SetLastClose(dTemp * 1000);

	// 涨跌值
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
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

	// 换手率
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetChangeHandRate(buffer2);

	// 成交量
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetVolume(buffer2); // 读入的是股数

// 成交金额
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetAmount(buffer2);

	// 总市值的数据有两种形式，需要程序判定
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetTotalValue(buffer2); // 总市值的单位为：元

// 流通市值不是用逗号结束，故而不能使用ReadOneValueFromNeteaseDayLine函数
// 流通市值的数据形式有两种，故而需要程序判定。
	i = 0;
	while (m_sDataBuffer.at(m_lCurrentPos) != 0x00d) {
		if ((m_sDataBuffer.at(m_lCurrentPos) == 0x00a) || (m_lCurrentPos >= m_lBufferLength) || (i > 30)) return false; // 数据出错，放弃载入
		buffer2[i++] = m_sDataBuffer.at(m_lCurrentPos++);
	}
	m_lCurrentPos++;
	buffer2[i] = 0x000;
	m_pCurrentProcessingDayLine->SetCurrentValue(buffer2); // 流通市值的单位为：元。
// \r后面紧跟着应该是\n
	if (m_sDataBuffer.at(m_lCurrentPos++) != 0x0a) return false; // 数据出错，放弃载入

	return true;
}

bool CNeteaseDayLineWebData::SkipNeteaseDayLineInformationHeader(void) {
	ASSERT(m_lCurrentPos == 0);
	do {
		if (m_lCurrentPos >= m_lBufferLength) {
			return false;
		}
		if (m_sDataBuffer.at(m_lCurrentPos) == 0x0a) {// 遇到\n
			m_lCurrentPos++; // 跨过此\n
			return false;
		}
	} while (m_sDataBuffer.at(m_lCurrentPos++) != 0X0d);// 寻找\r
	if (m_lCurrentPos >= m_lBufferLength) {
		return false;
	}
	if (m_sDataBuffer.at(m_lCurrentPos) != 0x0a) {
		return false;
	}
	m_lCurrentPos++;
	return true;
}

void CNeteaseDayLineWebData::ReportDayLineDownLoaded(void) {
	//CString strTemp = GetSymbol();
	//strTemp += _T("日线下载完成.");
	//gl_systemMessage.PushDayLineInfoMessage(strTemp);
}

bool CNeteaseDayLineWebData::ReadOneValueOfNeteaseDayLine(string& pBuffer, char* buffer, INT64& lCurrentPos) {
	int i = 0;

	while (pBuffer.at(lCurrentPos) != ',') { // 将下一个逗号前的字符存入缓冲区. 0x2c就是逗号。
		if ((pBuffer.at(lCurrentPos) == 0x0d) || (pBuffer.at(lCurrentPos) == 0x00a) || ((lCurrentPos + 1) >= pBuffer.size()) || (i > 100)) { // 遇到回车、换行或者超过了100个字符
			return false; // 数据出错，放弃载入
		}
		buffer[i++] = pBuffer.at(lCurrentPos++);
	}
	buffer[i] = 0x000;
	lCurrentPos++;
	return true;
}