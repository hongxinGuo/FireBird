#include"pch.h"

#include"ChinaMarket.h"
#include"DayLineWebData.h"

using namespace std;

CDayLineWebData::CDayLineWebData() {
	Reset();
}

void CDayLineWebData::Reset() {
	m_strStockCode = _T("");
	m_sDataBuffer.resize(0);
	m_vTempDayLine.resize(0);
	m_lBufferLength = 0;
	m_lCurrentPos = 0;
}

bool CDayLineWebData::TransferWebDataToBuffer(const CWebDataPtr& pWebData) {
	// ����ȡ���������ݷ�����ع�Ʊ���������ݻ������У���������ر�ʶ��
	m_sDataBuffer = std::move(pWebData->m_sDataBuffer);

	m_lBufferLength = pWebData->GetBufferLength();
	m_strStockCode = pWebData->GetStockCode();
	m_lCurrentPos = 0;
	ASSERT(m_sDataBuffer.size() == m_lBufferLength);

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

	if (m_lBufferLength == 0) {
		// û�����ݶ��룿����״̬�ǲ�ѯ�Ĺ�ƱΪ��Ч�������ڣ�����
		return false;
	}

	if (!SkipNeteaseDayLineInformationHeader(m_sDataBuffer, m_lCurrentPos)) {
		return false;
	}

	if (m_lCurrentPos >= m_lBufferLength) {
		// ��Ч��Ʊ���룬����ֻ��ǰ׺˵����û��ʵ����Ϣ�����������ˣ������Ѿ������ˣ��������¹����еĵ�һ��
		return false;
	}

	CString strTemp;
	CDayLinePtr pCurrentDayLine = nullptr;
	while (m_lCurrentPos < m_lBufferLength) {
		// ����һ����������
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ���������ݡ���������������ʷ���ݸ�ʽ��
//
// ��ʵʱ���������ƣ����ּ۸�ԷŴ�һǧ�����Գ����ʹ洢���������ݿ�ʱ��DECIMAL(10,3)���ʹ洢��
// �ַ�������ʽΪ��2019-07-10,600000,�ֶ�����,���̼�,��߼�,��ͼ�,���̼�,ǰ���̼�,�ǵ�ֵ,������,�ɽ�����,�ɽ����,����ֵ,��ͨ��ֵ\r\n
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
	while ((m_sDataBuffer.at(m_lCurrentPos) != 0x02c)) {
		// ��ȡ���ڣ�ֱ����������
		if ((m_sDataBuffer.at(m_lCurrentPos) == 0x0d) || (m_sDataBuffer.at(m_lCurrentPos) == 0x00a) || (m_lCurrentPos >= m_lBufferLength) || (i > 30)) {
			// ��������س������С��ַ������������߶�ȡ��20���ַ�
			return nullptr; // ���ݳ�����������
		}
		buffer3[i++] = m_sDataBuffer.at(m_lCurrentPos++);
	}
	m_lCurrentPos++;
	buffer3[i] = 0x00;
	sscanf_s(buffer3, _T("%04d-%02d-%02d"), &year, &month, &day);
	const long lMarketDate = year * 10000 + month * 100 + day;
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
		dTemp = atof(buffer2);
		pDayLine->SetClose(dTemp * 1000);
	}

	// ��߼�
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return nullptr;
	dTemp = atof(buffer2);
	pDayLine->SetHigh(dTemp * 1000);

	// ��ͼ�
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return nullptr;
	dTemp = atof(buffer2);
	pDayLine->SetLow(dTemp * 1000);

	// ���̼�
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return nullptr;
	dTemp = atof(buffer2);
	pDayLine->SetOpen(dTemp * 1000);

	// ǰ���̼�
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return nullptr;
	dTemp = atof(buffer2);
	pDayLine->SetLastClose(dTemp * 1000);

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
		if ((m_sDataBuffer.at(m_lCurrentPos) == 0x00a) || (m_lCurrentPos >= m_lBufferLength) || (i > 30)) return nullptr; // ���ݳ�����������
		buffer2[i++] = m_sDataBuffer.at(m_lCurrentPos++);
	}
	m_lCurrentPos++;
	buffer2[i] = 0x000;
	pDayLine->SetCurrentValue(buffer2); // ��ͨ��ֵ�ĵ�λΪ��Ԫ��
	// \r���������Ӧ����\n
	if (m_sDataBuffer.at(m_lCurrentPos++) != 0x0a) return nullptr; // ���ݳ�����������

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
	}
	while (sDataBuffer.at(lCurrentPos++) != 0X0d); // Ѱ��\r
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
