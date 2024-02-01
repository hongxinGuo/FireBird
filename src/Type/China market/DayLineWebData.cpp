#include"pch.h"

#include"ChinaMarket.h"
#include"DayLineWebData.h"

#include "JsonParse.h"
#include "TimeConvert.h"

using namespace std;

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
	// ����ȡ���������ݷ�����ع�Ʊ���������ݻ������У���������ر�ʶ��
	m_sDataBuffer = std::move(pWebData->m_sDataBuffer);
	m_strStockCode = pWebData->GetStockCode();
	m_lCurrentPos = 0;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// ������������߷������϶�ȡ�Ĺ�Ʊ�������ݡ�
// ������ʽΪ�� ����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n
//"����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"
//
// ��������������ģ��������ڵ���ǰ�档
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CDayLineWebData::ProcessNeteaseDayLineData() {
	shared_ptr<CDayLine> pDayLine;

	if (m_sDataBuffer.size() == 0) return false;	// û�����ݶ��룿����״̬�ǲ�ѯ�Ĺ�ƱΪ��Ч�������ڣ�����
	if (!SkipNeteaseDayLineInformationHeader(m_sDataBuffer, m_lCurrentPos)) return false;
	if (m_lCurrentPos >= m_sDataBuffer.size())return false;// ��Ч��Ʊ���룬����ֻ��ǰ׺˵����û��ʵ����Ϣ�����������ˣ������Ѿ������ˣ��������¹����еĵ�һ��

	CString strTemp;
	CDayLinePtr pCurrentDayLine = nullptr;
	while (m_lCurrentPos < m_sDataBuffer.size()) {	// ����һ����������
		pCurrentDayLine = ProcessOneNeteaseDayLineData();
		if (pCurrentDayLine == nullptr) {
			TRACE(_T("%s�������ݳ���\n"), m_strStockCode.GetBuffer());
			// ������ݴ����������
			m_vTempDayLine.clear();
			return false; // ���ݳ�����������
		}
		m_vTempDayLine.push_back(pCurrentDayLine); // �ݴ�����ʱvector�У������������ݵ�ʱ��˳���ǵߵ��ģ����µ�����ǰ��
	}
	// ��������
	ranges::sort(m_vTempDayLine, [](const CDayLinePtr& p1, const CDayLinePtr& p2) { return p1->GetMarketDate() < p2->GetMarketDate(); });
	ReportDayLineDownLoaded();

	return true;
}

bool CDayLineWebData::ProcessNeteaseDayLineData2() {
	shared_ptr<CDayLine> pDayLine;

	if (m_sDataBuffer.size() == 0) return false;	// û�����ݶ��룿����״̬�ǲ�ѯ�Ĺ�ƱΪ��Ч�������ڣ�����
	string_view svData = GetCurrentNeteaseData(); // ����ǰ׺
	if (m_lCurrentPos >= m_sDataBuffer.size())return false;// ��Ч��Ʊ���룬����ֻ��ǰ׺˵����û��ʵ����Ϣ�����������ˣ������Ѿ������ˣ��������¹����еĵ�һ��

	CString strTemp;
	CDayLinePtr pCurrentDayLine = nullptr;
	while (m_lCurrentPos < m_sDataBuffer.size()) {	// ����һ����������
		svData = GetCurrentNeteaseData();
		pCurrentDayLine = ProcessOneNeteaseDayLine(svData);
		if (pCurrentDayLine == nullptr) {
			TRACE(_T("%s�������ݳ���\n"), m_strStockCode.GetBuffer());
			// ������ݴ����������
			m_vTempDayLine.clear();
			return false; // ���ݳ�����������
		}
		m_vTempDayLine.push_back(pCurrentDayLine); // �ݴ�����ʱvector�У������������ݵ�ʱ��˳���ǵߵ��ģ����µ�����ǰ��
	}
	// ��������
	ranges::sort(m_vTempDayLine, [](const CDayLinePtr& p1, const CDayLinePtr& p2) { return p1->GetMarketDate() < p2->GetMarketDate(); });
	ReportDayLineDownLoaded();

	return true;
}

string_view CDayLineWebData::GetCurrentNeteaseData() {
	const string_view svCurrentTotal = string_view(m_sDataBuffer.c_str() + m_lCurrentPos, m_sDataBuffer.size() - m_lCurrentPos);
	const long lEnd = svCurrentTotal.find_first_of(0x0d);
	if (lEnd > svCurrentTotal.length()) {
		throw std::exception(_T("GetCurrentNeteaseDayLine() out of range"));
	}
	m_lCurrentPos += lEnd + 1; // ����ǰλ��������ǰ���ݽ�����֮��
	return svCurrentTotal.substr(0, lEnd + 1); // ���������ַ�';'
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ���������ݡ���������������ʷ���ݸ�ʽ��
//
// ��ʵʱ���������ƣ����ּ۸�ԷŴ�һǧ�����Գ����ʹ洢���������ݿ�ʱ��DECIMAL(10,3)���ʹ洢��
// ������ʽΪ�� ����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n
//"����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDayLinePtr CDayLineWebData::ProcessOneNeteaseDayLineData() {
	char buffer2[200], buffer3[100];
	int year = 0, month = 0, day = 0;
	double dTemp;

	auto pDayLine = make_shared<CDayLine>();

	long i = 0;
	// ����
	while ((m_sDataBuffer.at(m_lCurrentPos) != 0x02c)) {// ��ȡ���ڣ�ֱ����������
		if ((m_sDataBuffer.at(m_lCurrentPos) == 0x0d) || (m_sDataBuffer.at(m_lCurrentPos) == 0x00a) || (m_lCurrentPos >= m_sDataBuffer.size()) || (i > 30)) {// ��������س������С��ַ������������߶�ȡ��20���ַ�
			return nullptr; // ���ݳ�����������
		}
		buffer3[i++] = m_sDataBuffer.at(m_lCurrentPos++);
	}
	m_lCurrentPos++;
	buffer3[i] = 0x00;
	sscanf_s(buffer3, _T("%04d-%02d-%02d"), &year, &month, &day);
	const long lMarketDate = XferYearMonthDayToYYYYMMDD(year, month, day);
	pDayLine->SetTime(gl_pChinaMarket->TransferToUTCTime(lMarketDate));
	pDayLine->SetDate(lMarketDate);
	//TRACE("%d %d %d\n", year, month, day);

	if (m_sDataBuffer.at(m_lCurrentPos) != 0x027) return nullptr; // ���ǵ�����(')�����ݳ�����������
	m_lCurrentPos++;
	// ��Ʊ����
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return nullptr;
	pDayLine->SetStockSymbol(m_strStockCode); // ����Ĺ�Ʊ����Ϊ600601��000001��������ʽ�����ٴ���֮��ʹ�ñ���Ʊ��Symbol:600601.SS��000001.SZֱ�Ӹ�ֵ��
	// ��Ʊ����
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return nullptr;
	const CString str = buffer2;
	pDayLine->SetDisplaySymbol(str);
	// ���̼�
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return nullptr;
	if (buffer2[0] != 0x000) {
		pDayLine->SetClose(StrToDecimal(buffer2));
	}
	// ��߼�
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return nullptr;
	pDayLine->SetHigh(StrToDecimal(buffer2));
	// ��ͼ�
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return nullptr;
	pDayLine->SetLow(StrToDecimal(buffer2));
	// ���̼�
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return nullptr;
	pDayLine->SetOpen(StrToDecimal(buffer2));
	// ǰ���̼�
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return nullptr;
	pDayLine->SetLastClose(StrToDecimal(buffer2));
	// �ǵ�ֵ
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return nullptr;
	if (pDayLine->GetOpen() == 0) {
		pDayLine->SetUpDown(0.0);
	}
	else pDayLine->SetUpDown(buffer2);
	if (pDayLine->GetLastClose() == 0) {
		// �����ǵ�����
		pDayLine->SetUpDownRate(0.0); // ����������̼�Ϊ�㣨û���ף������ǵ���Ҳ��Ϊ�㡣
	}
	else {
		// ��Ҫ�Ŵ�1000 * 100�������̼۱�ʵ��ֵ��1000������¼���ǰٷֱȣ�ҲҪ����100����
		pDayLine->SetUpDownRate(pDayLine->GetUpDown() * 100000.0 / pDayLine->GetLastClose());
	}
	// ������
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return nullptr;
	pDayLine->SetChangeHandRate(buffer2);
	// �ɽ���
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return nullptr;
	pDayLine->SetVolume(buffer2); // ������ǹ���
	// �ɽ����
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return nullptr;
	pDayLine->SetAmount(buffer2);
	// ����ֵ��������������ʽ����Ҫ�����ж�
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return nullptr;
	pDayLine->SetTotalValue(buffer2); // ����ֵ�ĵ�λΪ��Ԫ
	// ��ͨ��ֵ�����ö��Ž������ʶ�����ʹ��ReadOneValueFromNeteaseDayLine����
	// ��ͨ��ֵ��������ʽ�����֣��ʶ���Ҫ�����ж���
	i = 0;
	while (m_sDataBuffer.at(m_lCurrentPos) != 0x00d) {
		if ((m_sDataBuffer.at(m_lCurrentPos) == 0x00a) || (m_lCurrentPos >= m_sDataBuffer.size()) || (i > 30)) return nullptr; // ���ݳ�����������
		buffer2[i++] = m_sDataBuffer.at(m_lCurrentPos++);
	}
	m_lCurrentPos++;
	buffer2[i] = 0x000;
	pDayLine->SetCurrentValue(buffer2); // ��ͨ��ֵ�ĵ�λΪ��Ԫ��
	// \r���������Ӧ����\n
	if (m_sDataBuffer.at(m_lCurrentPos++) != 0x0a) return nullptr; // ���ݳ�����������

	return pDayLine;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ���������ݡ���������������ʷ���ݸ�ʽ��
//
// ��ʵʱ���������ƣ����ּ۸�ԷŴ�һǧ�����Գ����ʹ洢���������ݿ�ʱ��DECIMAL(10,3)���ʹ洢��
// ������ʽΪ�� ����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n
//"����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n2019-07-23,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n2019-07-24,'600000,�ַ�����,11.49,11.56,11.43,11.43,11.48,0.01,0.0638,17927898,206511000.0,3.37255403762e+11,3.229122472e+11\r\n"
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDayLinePtr CDayLineWebData::ProcessOneNeteaseDayLine(const string_view& svData) const {
	int year = 0, month = 0, day = 0;
	string_view sv;

	auto pDayLine = make_shared<CDayLine>();

	try {
		long lCurrentPos = 0;
		// ����
		sv = GetNextField(svData, lCurrentPos, ',');
		sscanf_s(sv.data(), _T("%04d-%02d-%02d"), &year, &month, &day);
		const long lMarketDate = XferYearMonthDayToYYYYMMDD(year, month, day);
		pDayLine->SetTime(gl_pChinaMarket->TransferToUTCTime(lMarketDate));
		pDayLine->SetDate(lMarketDate);
		sv = GetNextField(svData, lCurrentPos, 0x027);
		// ��Ʊ����
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetStockSymbol(m_strStockCode); // ����Ĺ�Ʊ����Ϊ600601��000001��������ʽ�����ٴ���֮��ʹ�ñ���Ʊ��Symbol:600601.SS��000001.SZֱ�Ӹ�ֵ��
		// ��Ʊ����
		sv = GetNextField(svData, lCurrentPos, ',');
		const CString str(sv.data(), sv.length());
		pDayLine->SetDisplaySymbol(str);
		// ���̼�
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetClose(StrToDecimal(sv));
		// ��߼�
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetHigh(StrToDecimal(sv));
		// ��ͼ�
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetLow(StrToDecimal(sv));
		// ���̼�
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetOpen(StrToDecimal(sv));
		// ǰ���̼�
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetLastClose(StrToDecimal(sv));
		// �ǵ�ֵ
		sv = GetNextField(svData, lCurrentPos, ',');
		if (pDayLine->GetOpen() == 0) {
			pDayLine->SetUpDown(0.0);
		}
		else pDayLine->SetUpDown(sv.data());
		if (pDayLine->GetLastClose() == 0) {
			// �����ǵ�����
			pDayLine->SetUpDownRate(0.0); // ����������̼�Ϊ�㣨û���ף������ǵ���Ҳ��Ϊ�㡣
		}
		else {
			// ��Ҫ�Ŵ�1000 * 100�������̼۱�ʵ��ֵ��1000������¼���ǰٷֱȣ�Ҫ������100����
			pDayLine->SetUpDownRate(pDayLine->GetUpDown() * 100000.0 / pDayLine->GetLastClose());
		}
		// ������
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetChangeHandRate(sv.data());
		// �ɽ���
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetVolume(atoll(sv.data())); // ������ǹ���
		// �ɽ����
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetAmount(StrToDecimal(sv.data(), 0));
		// ����ֵ��������������ʽ����Ҫ�����ж���
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetTotalValue(atof(sv.data())); // ����ֵ�ĵ�λΪ��Ԫ
		// ��ͨ��ֵ��������ʽ�����֣��ʶ���Ҫ�����ж���
		sv = GetNextField(svData, lCurrentPos, 0x0d); // ��������û���ַ�','���ϣ�ֱ��ʹ������\r\n
		pDayLine->SetCurrentValue(sv.data()); // ��ͨ��ֵ�ĵ�λΪ��Ԫ��
	}
	catch (exception& e) {
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
			// ����\n
			lCurrentPos++; // �����\n
			return false;
		}
	} while (sDataBuffer.at(lCurrentPos++) != 0X0d); // Ѱ��\r
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
	//strTemp += _T("�����������.");
	//gl_systemMessage.PushDayLineInfoMessage(strTemp);
}

bool CDayLineWebData::ReadOneValueOfNeteaseDayLine(const string& pBuffer, char* buffer, INT64& lCurrentPos) {
	int i = 0;

	while (pBuffer.at(lCurrentPos) != ',') {
		// ����һ������ǰ���ַ����뻺����. 0x2c���Ƕ��š�
		if ((pBuffer.at(lCurrentPos) == 0x0d) || (pBuffer.at(lCurrentPos) == 0x00a) || ((lCurrentPos + 1) >= pBuffer.size()) || (i > 100)) {
			// �����س������л��߳�����100���ַ�
			return false; // ���ݳ�����������
		}
		buffer[i++] = pBuffer.at(lCurrentPos++);
	}
	buffer[i] = 0x000;
	lCurrentPos++;
	return true;
}
