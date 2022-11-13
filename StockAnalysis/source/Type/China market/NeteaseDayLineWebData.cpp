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
	// ����ȡ���������ݷ�����ع�Ʊ���������ݻ������У���������ر�ʶ��
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
//
// ��������������ģ��������ڵ���ǰ�档
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CNeteaseDayLineWebData::ProcessNeteaseDayLineData(void) {
	shared_ptr<CDayLine> pDayLine;

	if (m_lBufferLength == 0) { // û�����ݶ��룿����״̬�ǲ�ѯ�Ĺ�ƱΪ��Ч�������ڣ�����
		return false;
	}

	if (!SkipNeteaseDayLineInformationHeader()) {
		return false;
	}

	if (m_lCurrentPos >= m_lBufferLength) {// ��Ч��Ʊ���룬����ֻ��ǰ׺˵����û��ʵ����Ϣ�����������ˣ������Ѿ������ˣ��������¹����еĵ�һ��
		return false;
	}

	CString strTemp;
	while (m_lCurrentPos < m_lBufferLength) {
		m_pCurrentProcessingDayLine = nullptr;
		if (!ProcessOneNeteaseDayLineData()) { // ����һ����������
			TRACE(_T("%s�������ݳ���\n"), m_strStockCode.GetBuffer());
			// ������ݴ����������
			m_vTempDayLine.clear();
			return false; // ���ݳ�����������
		}
		m_vTempDayLine.push_back(m_pCurrentProcessingDayLine); // �ݴ�����ʱvector�У���Ϊ�����������ݵ�ʱ��˳���ǵߵ��ģ����µ�����ǰ��
	}
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
	// ����
	while ((m_sDataBuffer.at(m_lCurrentPos) != 0x02c)) { // ��ȡ���ڣ�ֱ����������
		if ((m_sDataBuffer.at(m_lCurrentPos) == 0x0d) || (m_sDataBuffer.at(m_lCurrentPos) == 0x00a) || (m_lCurrentPos >= m_lBufferLength) || (i > 30)) { // ��������س������С��ַ������������߶�ȡ��20���ַ�
			return false; // ���ݳ�����������
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

	if (m_sDataBuffer.at(m_lCurrentPos) != 0x027) return(false); // ���ǵ�����(')�����ݳ�����������
	m_lCurrentPos++;

	// ��Ʊ����
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetStockSymbol(m_strStockCode); // ����Ĺ�Ʊ����Ϊ600601��000001��������ʽ�����ٴ���֮��ʹ�ñ���Ʊ��Symbol:600601.SS��000001.SZֱ�Ӹ�ֵ��

	// ��Ʊ����
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	str = buffer2;
	m_pCurrentProcessingDayLine->SetDisplaySymbol(str);

	// ���̼�
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	if (buffer2[0] != 0x000) {
		dTemp = atof(buffer2);
		m_pCurrentProcessingDayLine->SetClose(dTemp * 1000);
	}

	// ��߼�
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	dTemp = atof(buffer2);
	m_pCurrentProcessingDayLine->SetHigh(dTemp * 1000);

	// ��ͼ�
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	dTemp = atof(buffer2);
	m_pCurrentProcessingDayLine->SetLow(dTemp * 1000);

	// ���̼�
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	dTemp = atof(buffer2);
	m_pCurrentProcessingDayLine->SetOpen(dTemp * 1000);

	// ǰ���̼�
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	dTemp = atof(buffer2);
	m_pCurrentProcessingDayLine->SetLastClose(dTemp * 1000);

	// �ǵ�ֵ
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	if (m_pCurrentProcessingDayLine->GetOpen() == 0) {
		//ASSERT(strcmp(buffer2, _T("None") == 0);
		m_pCurrentProcessingDayLine->SetUpDown(0.0);
	}
	else m_pCurrentProcessingDayLine->SetUpDown(buffer2);
	if (m_pCurrentProcessingDayLine->GetLastClose() == 0) { // �����ǵ�����
		m_pCurrentProcessingDayLine->SetUpDownRate(0.0); // ����������̼�Ϊ�㣨û���ף������ǵ���Ҳ��Ϊ�㡣
	}
	else {
		// ��Ҫ�Ŵ�1000 * 100�������̼۱�ʵ��ֵ��1000������¼���ǰٷֱȣ�ҲҪ����100����
		m_pCurrentProcessingDayLine->SetUpDownRate(((double)(m_pCurrentProcessingDayLine->GetUpDown() * 100000.0)) / m_pCurrentProcessingDayLine->GetLastClose());
	}

	// ������
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetChangeHandRate(buffer2);

	// �ɽ���
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetVolume(buffer2); // ������ǹ���

// �ɽ����
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetAmount(buffer2);

	// ����ֵ��������������ʽ����Ҫ�����ж�
	if (!ReadOneValueOfNeteaseDayLine(m_sDataBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetTotalValue(buffer2); // ����ֵ�ĵ�λΪ��Ԫ

// ��ͨ��ֵ�����ö��Ž������ʶ�����ʹ��ReadOneValueFromNeteaseDayLine����
// ��ͨ��ֵ��������ʽ�����֣��ʶ���Ҫ�����ж���
	i = 0;
	while (m_sDataBuffer.at(m_lCurrentPos) != 0x00d) {
		if ((m_sDataBuffer.at(m_lCurrentPos) == 0x00a) || (m_lCurrentPos >= m_lBufferLength) || (i > 30)) return false; // ���ݳ�����������
		buffer2[i++] = m_sDataBuffer.at(m_lCurrentPos++);
	}
	m_lCurrentPos++;
	buffer2[i] = 0x000;
	m_pCurrentProcessingDayLine->SetCurrentValue(buffer2); // ��ͨ��ֵ�ĵ�λΪ��Ԫ��
// \r���������Ӧ����\n
	if (m_sDataBuffer.at(m_lCurrentPos++) != 0x0a) return false; // ���ݳ�����������

	return true;
}

bool CNeteaseDayLineWebData::SkipNeteaseDayLineInformationHeader(void) {
	ASSERT(m_lCurrentPos == 0);
	do {
		if (m_lCurrentPos >= m_lBufferLength) {
			return false;
		}
		if (m_sDataBuffer.at(m_lCurrentPos) == 0x0a) {// ����\n
			m_lCurrentPos++; // �����\n
			return false;
		}
	} while (m_sDataBuffer.at(m_lCurrentPos++) != 0X0d);// Ѱ��\r
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
	//strTemp += _T("�����������.");
	//gl_systemMessage.PushDayLineInfoMessage(strTemp);
}

bool CNeteaseDayLineWebData::ReadOneValueOfNeteaseDayLine(string& pBuffer, char* buffer, INT64& lCurrentPos) {
	int i = 0;

	while (pBuffer.at(lCurrentPos) != ',') { // ����һ������ǰ���ַ����뻺����. 0x2c���Ƕ��š�
		if ((pBuffer.at(lCurrentPos) == 0x0d) || (pBuffer.at(lCurrentPos) == 0x00a) || ((lCurrentPos + 1) >= pBuffer.size()) || (i > 100)) { // �����س������л��߳�����100���ַ�
			return false; // ���ݳ�����������
		}
		buffer[i++] = pBuffer.at(lCurrentPos++);
	}
	buffer[i] = 0x000;
	lCurrentPos++;
	return true;
}