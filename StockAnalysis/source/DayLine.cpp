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
// ����һ���������ݡ���������������ʷ���ݸ�ʽ��
//
// ��ʵʱ���������ƣ����ּ۸�ԷŴ�һǧ�����Գ����ʹ洢���������ݿ�ʱ��DECIMAL(10,3)���ʹ洢��
// �ַ�������ʽΪ��2019-07-10,600000,�ֶ�����,���̼�,��߼�,��ͼ�,���̼�,ǰ���̼�,�ǵ�ֵ,�ǵ�����,������,�ɽ�����,�ɽ����,����ֵ,��ͨ��ֵ\r\n
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
	while ((pBuffer.at(lCurrentPos) != 0x02c)) { // ��ȡ���ڣ�ֱ����������
		if ((pBuffer.at(lCurrentPos) == 0x0d) || (pBuffer.at(lCurrentPos) == 0x00a) || (pBuffer.at(lCurrentPos) == 0x000) || (i > 30)) { // ��������س������С��ַ������������߶�ȡ��20���ַ�
			return false; // ���ݳ�����������
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

	if (pBuffer.at(lCurrentPos) != 0x027) return(false); // ���ǵ�����(')�����ݳ�����������
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

	if (GetLastClose() == 0) { // �����ǵ�����
		SetUpDownRate(0.0); // ����������̼�Ϊ�㣨û���ף������ǵ���Ҳ��Ϊ�㡣
	}
	else {
		// ��Ҫ�Ŵ�1000 * 100�������̼۱�ʵ��ֵ��1000������¼���ǰٷֱȣ�ҲҪ����100����
		SetUpDownRate(((double)(GetUpDown() * 100000.0)) / GetLastClose());
	}

	if (!ReadOneValueOfNeteaseDayLine(pBuffer, buffer2, lCurrentPos)) return false;
	SetChangeHandRate(buffer2);

	if (!ReadOneValueOfNeteaseDayLine(pBuffer, buffer2, lCurrentPos)) return false;
	SetVolume(buffer2); // ������ǹ���

	if (!ReadOneValueOfNeteaseDayLine(pBuffer, buffer2, lCurrentPos)) return false;
	SetAmount(buffer2);

	// ����ֵ��������������ʽ����Ҫ�����ж�
	if (!ReadOneValueOfNeteaseDayLine(pBuffer, buffer2, lCurrentPos)) return false;
	SetTotalValue(buffer2); // ����ֵ�ĵ�λΪ��Ԫ

	// ��ͨ��ֵ�����ö��Ž������ʶ�����ʹ��ReadOneValueFromNeteaseDayLine����
	// ��ͨ��ֵ��������ʽ�����֣��ʶ���Ҫ�����ж���
	i = 0;
	while (pBuffer.at(lCurrentPos) != 0x00d) {
		if ((pBuffer.at(lCurrentPos) == 0x00a) || (pBuffer.at(lCurrentPos) == 0x000) || (i > 30)) return false; // ���ݳ�����������
		buffer2[i++] = pBuffer.at(lCurrentPos++);
	}
	lCurrentPos++;
	buffer2[i] = 0x000;
	SetCurrentValue(buffer2); // ��ͨ��ֵ�ĵ�λΪ��Ԫ��
	// \r���������Ӧ����\n
	if (pBuffer.at(lCurrentPos++) != 0x0a) return false; // ���ݳ�����������

	return true;
}

bool CDayLine::IsActive(void) const {
	if ((GetClose() != 0) && (GetLastClose() != 0)) {
		return true;
	}
	else return false;
}