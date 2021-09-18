#include"pch.h"

#include"globedef.h"
#include "DayLine.h"

CDayLine::CDayLine() : CVirtualHistoryCandleExtend() {
	Reset();
}

void CDayLine::Reset(void) {
	CVirtualHistoryCandleExtend::Reset();
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
bool CDayLine::ProcessNeteaseData(CString strStockCode, vector<char>& pBuffer, INT64& lCurrentPos) {
	static char buffer2[200], buffer3[100];
	long i = 0;
	tm tm_;
	int year = 0, month = 0, day = 0;
	long lDate = 0;
	CString str;
	double dTemp = 0;

	i = 0;
	while ((pBuffer.at(lCurrentPos) != 0x02c)) { // 读取日期，直到遇到逗号
		if ((pBuffer.at(lCurrentPos) == 0x0d) || (pBuffer.at(lCurrentPos) == 0x00a) || (pBuffer.at(lCurrentPos) == 0x000) || (i > 30)) { // 如果遇到回车、换行、字符串结束符或者读取了20个字符
			return false; // 数据出错，放弃载入
		}
		buffer3[i++] = pBuffer.at(lCurrentPos++);
	}
	lCurrentPos++;
	buffer3[i] = 0x00;
	sscanf_s(buffer3, _T("%04d-%02d-%02d"), &year, &month, &day);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = 15;
	tm_.tm_min = 0;
	tm_.tm_sec = 0;
	tm_.tm_isdst = 0;
	SetTime(mktime(&tm_));
	lDate = year * 10000 + month * 100 + day;
	SetDate(lDate);
	//TRACE("%d %d %d\n", year, month, day);

	if (pBuffer.at(lCurrentPos) != 0x027) return(false); // 不是单引号(')，数据出错，放弃载入
	lCurrentPos++;

	if (!ReadOneValueOfNeteaseDayLine(pBuffer, buffer2, lCurrentPos)) return false;
	str = buffer2;
	SetStockSymbol(strStockCode);
	str = strStockCode.Left(2);
	if (!ReadOneValueOfNeteaseDayLine(pBuffer, buffer2, lCurrentPos)) return false;
	str = buffer2;
	SetDisplaySymbol(str);

	if (!ReadOneValueOfNeteaseDayLine(pBuffer, buffer2, lCurrentPos)) return false;
	dTemp = atof(buffer2);
	SetClose(dTemp * 1000);

	if (!ReadOneValueOfNeteaseDayLine(pBuffer, buffer2, lCurrentPos)) return false;
	dTemp = atof(buffer2);
	SetHigh(dTemp * 1000);

	if (!ReadOneValueOfNeteaseDayLine(pBuffer, buffer2, lCurrentPos)) return false;
	dTemp = atof(buffer2);
	SetLow(dTemp * 1000);

	if (!ReadOneValueOfNeteaseDayLine(pBuffer, buffer2, lCurrentPos)) return false;
	dTemp = atof(buffer2);
	SetOpen(dTemp * 1000);

	if (!ReadOneValueOfNeteaseDayLine(pBuffer, buffer2, lCurrentPos)) return false;
	dTemp = atof(buffer2);
	SetLastClose(dTemp * 1000);

	if (!ReadOneValueOfNeteaseDayLine(pBuffer, buffer2, lCurrentPos)) return false;
	if (GetOpen() == 0) {
		//ASSERT(strcmp(buffer2, _T("None") == 0);
		SetUpDown(0.0);
	}
	else SetUpDown(buffer2);

	if (GetLastClose() == 0) { // 设置涨跌幅。
		SetUpDownRate(0.0); // 如果昨日收盘价为零（没交易），则涨跌幅也设为零。
	}
	else {
		// 需要放大1000 * 100倍。收盘价比实际值大1000倍，记录的是百分比，也要增大100倍。
		SetUpDownRate(((double)(GetUpDown() * 100000.0)) / GetLastClose());
	}

	if (!ReadOneValueOfNeteaseDayLine(pBuffer, buffer2, lCurrentPos)) return false;
	SetChangeHandRate(buffer2);

	if (!ReadOneValueOfNeteaseDayLine(pBuffer, buffer2, lCurrentPos)) return false;
	SetVolume(buffer2); // 读入的是股数

	if (!ReadOneValueOfNeteaseDayLine(pBuffer, buffer2, lCurrentPos)) return false;
	SetAmount(buffer2);

	// 总市值的数据有两种形式，需要程序判定
	if (!ReadOneValueOfNeteaseDayLine(pBuffer, buffer2, lCurrentPos)) return false;
	SetTotalValue(buffer2); // 总市值的单位为：元

	// 流通市值不是用逗号结束，故而不能使用ReadOneValueFromNeteaseDayLine函数
	// 流通市值的数据形式有两种，故而需要程序判定。
	i = 0;
	while (pBuffer.at(lCurrentPos) != 0x00d) {
		if ((pBuffer.at(lCurrentPos) == 0x00a) || (pBuffer.at(lCurrentPos) == 0x000) || (i > 30)) return false; // 数据出错，放弃载入
		buffer2[i++] = pBuffer.at(lCurrentPos++);
	}
	lCurrentPos++;
	buffer2[i] = 0x000;
	SetCurrentValue(buffer2); // 流通市值的单位为：元。
	// \r后面紧跟着应该是\n
	if (pBuffer.at(lCurrentPos++) != 0x0a) return false; // 数据出错，放弃载入

	return true;
}

bool CDayLine::IsActive(void) const {
	if ((GetClose() != 0) && (GetLastClose() != 0)) {
		return true;
	}
	else return false;
}