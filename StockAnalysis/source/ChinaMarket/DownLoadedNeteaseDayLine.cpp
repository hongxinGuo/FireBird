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
	// ����ȡ���������ݷ�����ع�Ʊ���������ݻ������У���������ر�ʶ��
	m_vDayLineBuffer.resize(pNeteaseWebDayLineData->GetByteReaded() + 1); // ��������Ҫ���һ���ַ����ȣ�����Ǹ�0x000����
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
// ������������߷������϶�ȡ�Ĺ�Ʊ�������ݡ�
// ������ʽΪ�� ����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CDownLoadedNeteaseDayLine::ProcessNeteaseDayLineData(void) {
	shared_ptr<CDayLine> pDayLine;

	if (m_lDayLineBufferLength == 0) { // û�����ݶ��룿����״̬�ǲ�ѯ�Ĺ�ƱΪ��Ч�������ڣ�����
		return false;
	}

	ASSERT(m_vDayLineBuffer.at(m_lDayLineBufferLength) == 0x000); // ����ַ�Ϊ���ӵ�0x000.
	if (!SkipNeteaseDayLineInformationHeader()) {
		return false;
	}

	if (m_lCurrentPos >= m_lDayLineBufferLength) {// ��Ч��Ʊ���룬����ֻ��ǰ׺˵����û��ʵ����Ϣ�����������ˣ������Ѿ������ˣ��������¹����еĵ�һ��
		return false;
	}

	CString strTemp;
	while (m_lCurrentPos < m_lDayLineBufferLength) {
		m_pCurrentProcessingDayLine = nullptr;
		if (!ProcessOneNeteaseDayLineData()) { // ����һ����������
			TRACE(_T("%s�������ݳ���\n"), m_strDownLoadStockSymbol.GetBuffer());
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
// �ַ�������ʽΪ��2019-07-10,600000,�ֶ�����,���̼�,��߼�,��ͼ�,���̼�,ǰ���̼�,�ǵ�ֵ,�ǵ�����,������,�ɽ�����,�ɽ����,����ֵ,��ͨ��ֵ\r\n
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
	while ((m_vDayLineBuffer.at(m_lCurrentPos) != 0x02c)) { // ��ȡ���ڣ�ֱ����������
		if ((m_vDayLineBuffer.at(m_lCurrentPos) == 0x0d) || (m_vDayLineBuffer.at(m_lCurrentPos) == 0x00a) || (m_vDayLineBuffer.at(m_lCurrentPos) == 0x000) || (i > 30)) { // ��������س������С��ַ������������߶�ȡ��20���ַ�
			return false; // ���ݳ�����������
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

	if (m_vDayLineBuffer.at(m_lCurrentPos) != 0x027) return(false); // ���ǵ�����(')�����ݳ�����������
	m_lCurrentPos++;

	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetStockSymbol(m_strDownLoadStockSymbol); // ����Ĺ�Ʊ����Ϊ600601��000001��������ʽ�����ٴ���֮��ʹ�ñ���Ʊ��Symbol:600601.SS��000001.SZֱ�Ӹ�ֵ��

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

	if (m_pCurrentProcessingDayLine->GetLastClose() == 0) { // �����ǵ�����
		m_pCurrentProcessingDayLine->SetUpDownRate(0.0); // ����������̼�Ϊ�㣨û���ף������ǵ���Ҳ��Ϊ�㡣
	}
	else {
		// ��Ҫ�Ŵ�1000 * 100�������̼۱�ʵ��ֵ��1000������¼���ǰٷֱȣ�ҲҪ����100����
		m_pCurrentProcessingDayLine->SetUpDownRate(((double)(m_pCurrentProcessingDayLine->GetUpDown() * 100000.0)) / m_pCurrentProcessingDayLine->GetLastClose());
	}

	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetChangeHandRate(buffer2);

	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetVolume(buffer2); // ������ǹ���

	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetAmount(buffer2);

	// ����ֵ��������������ʽ����Ҫ�����ж�
	if (!ReadOneValueOfNeteaseDayLine(m_vDayLineBuffer, buffer2, m_lCurrentPos)) return false;
	m_pCurrentProcessingDayLine->SetTotalValue(buffer2); // ����ֵ�ĵ�λΪ��Ԫ

	// ��ͨ��ֵ�����ö��Ž������ʶ�����ʹ��ReadOneValueFromNeteaseDayLine����
	// ��ͨ��ֵ��������ʽ�����֣��ʶ���Ҫ�����ж���
	i = 0;
	while (m_vDayLineBuffer.at(m_lCurrentPos) != 0x00d) {
		if ((m_vDayLineBuffer.at(m_lCurrentPos) == 0x00a) || (m_vDayLineBuffer.at(m_lCurrentPos) == 0x000) || (i > 30)) return false; // ���ݳ�����������
		buffer2[i++] = m_vDayLineBuffer.at(m_lCurrentPos++);
	}
	m_lCurrentPos++;
	buffer2[i] = 0x000;
	m_pCurrentProcessingDayLine->SetCurrentValue(buffer2); // ��ͨ��ֵ�ĵ�λΪ��Ԫ��
	// \r���������Ӧ����\n
	if (m_vDayLineBuffer.at(m_lCurrentPos++) != 0x0a) return false; // ���ݳ�����������

	return true;
}

bool CDownLoadedNeteaseDayLine::SkipNeteaseDayLineInformationHeader(void) {
	ASSERT(m_lCurrentPos == 0);
	while (m_vDayLineBuffer.at(m_lCurrentPos) != 0X0d) { // Ѱ��\r
		if (m_vDayLineBuffer.at(m_lCurrentPos) == 0x0a) {// ����\n
			m_lCurrentPos++; // �����\n
			return false;
		}
		else if (m_vDayLineBuffer.at(m_lCurrentPos) == 0x000) { // ����0x000
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
	//strTemp += _T("�����������.");
	//gl_systemMessage.PushDayLineInfoMessage(strTemp);
}