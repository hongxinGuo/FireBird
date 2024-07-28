#include"pch.h"

#include"ChinaMarket.h"
#include"DayLineWebData.h"

#include"WebData.h"

#include "JsonParse.h"
#include "TimeConvert.h"

using std::make_shared;

CDayLineWebData::CDayLineWebData() {
	Reset();
}

void CDayLineWebData::Reset() {
	m_strStockCode = _T("");
	m_sDataBuffer.resize(0);
	m_vTempDayLine.resize(0);
	m_lCurrentPos = 0;
}

bool CDayLineWebData::TransferWebDataToBuffer(const CWebDataPtr& pWebData) {
	// 将读取的日线数据放入相关股票的日线数据缓冲区中，并设置相关标识。
	m_sDataBuffer = std::move(pWebData->m_sDataBuffer);
	m_strStockCode = pWebData->GetStockCode();
	m_lCurrentPos = 0;

	return true;
}

bool CDayLineWebData::ProcessNeteaseDayLineData() {
	shared_ptr<CDayLine> pDayLine;

	if (m_sDataBuffer.empty()) return false;	// 没有数据读入？此种状态是查询的股票为无效（不存在）号码
	string_view svData = GetCurrentNeteaseData(); // 读过前缀
	if (m_lCurrentPos >= m_sDataBuffer.size())return false;// 无效股票号码，数据只有前缀说明，没有实际信息，或者退市了；或者已经更新了；或者是新股上市的第一天

	CString strTemp;
	CDayLinePtr pCurrentDayLine = nullptr;
	while (m_lCurrentPos < m_sDataBuffer.size()) {	// 处理一条日线数据
		svData = GetCurrentNeteaseData();
		pCurrentDayLine = ProcessOneNeteaseDayLine(svData);
		if (pCurrentDayLine == nullptr) {
			TRACE(_T("%s日线数据出错\n"), m_strStockCode.GetBuffer());
			// 清除已暂存的日线数据
			m_vTempDayLine.clear();
			return false; // 数据出错，放弃载入
		}
		m_vTempDayLine.push_back(pCurrentDayLine); // 暂存于临时vector中，网易日线数据的时间顺序是颠倒的，最新的在最前面
	}
	// 正序排列
	std::ranges::sort(m_vTempDayLine, [](const CDayLinePtr& p1, const CDayLinePtr& p2) { return p1->GetMarketDate() < p2->GetMarketDate(); });
	ReportDayLineDownLoaded();

	return true;
}

string_view CDayLineWebData::GetCurrentNeteaseData() {
	const string_view svCurrentTotal = string_view(m_sDataBuffer.c_str() + m_lCurrentPos, m_sDataBuffer.size() - m_lCurrentPos);
	const long lEnd = svCurrentTotal.find_first_of(0x0d);
	if (lEnd > svCurrentTotal.length()) {
		throw std::exception(_T("GetCurrentNeteaseDayLine() out of range"));
	}
	m_lCurrentPos += lEnd + 2; // 将当前位置移至当前数据结束处之后
	return svCurrentTotal.substr(0, lEnd + 2); // 包括最后的两个字符：\r\n
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理一条日线数据。采用网易日线历史数据格式。
//
// 与实时数据相类似，各种价格皆放大一千倍后以长整型存储。存入数据库时以DECIMAL(10,3)类型存储。
// 数据制式为： 日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n
//"日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n2019-07-23,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,浦发银行,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDayLinePtr CDayLineWebData::ProcessOneNeteaseDayLine(const string_view& svData) const {
	int year = 0, month = 0, day = 0;
	string_view sv;
	auto pDayLine = make_shared<CDayLine>();

	try {
		long lCurrentPos = 0;
		// 日期
		sv = GetNextField(svData, lCurrentPos, ',');
		sscanf_s(sv.data(), _T("%04d-%02d-%02d"), &year, &month, &day);
		const long lMarketDate = XferYearMonthDayToYYYYMMDD(year, month, day);
		pDayLine->SetTime(gl_pChinaMarket->TransferToUTCTime(lMarketDate));
		pDayLine->SetDate(lMarketDate);
		sv = GetNextField(svData, lCurrentPos, 0x027);
		// 股票代码
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetStockSymbol(m_strStockCode); // 读入的股票代码为600601、000001这样的制式，不再处理之，使用本股票的Symbol:600601.SS、000001.SZ直接赋值。
		// 股票名称
		sv = GetNextField(svData, lCurrentPos, ',');
		const CString str(sv.data(), sv.length());
		pDayLine->SetDisplaySymbol(str);
		// 收盘价
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetClose(StrToDecimal(sv, 3));
		// 最高价
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetHigh(StrToDecimal(sv, 3));
		// 最低价
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetLow(StrToDecimal(sv, 3));
		// 开盘价
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetOpen(StrToDecimal(sv, 3));
		// 前收盘价
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetLastClose(StrToDecimal(sv, 3));
		// 涨跌值
		sv = GetNextField(svData, lCurrentPos, ',');
		if (pDayLine->GetOpen() == 0) {
			pDayLine->SetUpDown(0.0);
		}
		else pDayLine->SetUpDown(sv.data());
		if (pDayLine->GetLastClose() == 0) {
			// 设置涨跌幅。
			pDayLine->SetUpDownRate(0.0); // 如果昨日收盘价为零（没交易），则涨跌幅也设为零。
		}
		else {
			// 需要放大1000 * 100倍。收盘价比实际值大1000倍，记录的是百分比，要再增大100倍。
			pDayLine->SetUpDownRate(pDayLine->GetUpDown() * 100000.0 / pDayLine->GetLastClose());
		}
		// 换手率
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetChangeHandRate(sv.data());
		// 成交量
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetVolume(atoll(sv.data())); // 读入的是股数
		// 成交金额
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetAmount(StrToDecimal(sv.data(), 0));
		// 总市值的数据有两种形式，需要程序判定。
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetTotalValue(atof(sv.data())); // 总市值的单位为：元
		// 流通市值的数据形式有两种，故而需要程序判定。
		sv = GetNextField(svData, lCurrentPos, 0x0d); // 最后的数据没有字符','隔断，直接使用最后的\r\n
		pDayLine->SetCurrentValue(sv.data()); // 流通市值的单位为：元。
	}
	catch ([[maybe_unused]] std::exception& e) {
		return nullptr;
	}
	return pDayLine;
}

bool CDayLineWebData::SkipNeteaseDayLineInformationHeader(const string& sDataBuffer, INT64& lCurrentPos) {
	ASSERT(lCurrentPos == 0);
	do {
		if (lCurrentPos >= sDataBuffer.size()) {
			return false;
		}
		if (sDataBuffer.at(lCurrentPos) == 0x0a) {
			// 遇到\n
			lCurrentPos++; // 跨过此\n
			return false;
		}
	} while (sDataBuffer.at(lCurrentPos++) != 0X0d); // 寻找\r
	if (lCurrentPos >= sDataBuffer.size()) {
		return false;
	}
	if (sDataBuffer.at(lCurrentPos) != 0x0a) {
		return false;
	}
	lCurrentPos++;
	return true;
}

void CDayLineWebData::ReportDayLineDownLoaded() {
	//CString strTemp = GetSymbol();
	//strTemp += _T("日线下载完成.");
	//gl_systemMessage.PushDayLineInfoMessage(strTemp);
}

bool CDayLineWebData::ReadOneValueOfNeteaseDayLine(const string& pBuffer, char* buffer, INT64& lCurrentPos) {
	int i = 0;

	while (pBuffer.at(lCurrentPos) != ',') {
		// 将下一个逗号前的字符存入缓冲区. 0x2c就是逗号。
		if ((pBuffer.at(lCurrentPos) == 0x0d) || (pBuffer.at(lCurrentPos) == 0x00a) || ((lCurrentPos + 1) >= pBuffer.size()) || (i > 100)) {
			// 遇到回车、换行或者超过了100个字符
			return false; // 数据出错，放弃载入
		}
		buffer[i++] = pBuffer.at(lCurrentPos++);
	}
	buffer[i] = 0x000;
	lCurrentPos++;
	return true;
}
