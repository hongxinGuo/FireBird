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
	// ����ȡ���������ݷ�����ع�Ʊ���������ݻ������У���������ر�ʶ��
	m_sDataBuffer = std::move(pWebData->m_sDataBuffer);
	m_strStockCode = pWebData->GetStockCode();
	m_lCurrentPos = 0;

	return true;
}

bool CDayLineWebData::ProcessNeteaseDayLineData() {
	shared_ptr<CDayLine> pDayLine;

	if (m_sDataBuffer.empty()) return false;	// û�����ݶ��룿����״̬�ǲ�ѯ�Ĺ�ƱΪ��Ч�������ڣ�����
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
	m_lCurrentPos += lEnd + 2; // ����ǰλ��������ǰ���ݽ�����֮��
	return svCurrentTotal.substr(0, lEnd + 2); // �������������ַ���\r\n
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
		pDayLine->SetClose(StrToDecimal(sv, 3));
		// ��߼�
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetHigh(StrToDecimal(sv, 3));
		// ��ͼ�
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetLow(StrToDecimal(sv, 3));
		// ���̼�
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetOpen(StrToDecimal(sv, 3));
		// ǰ���̼�
		sv = GetNextField(svData, lCurrentPos, ',');
		pDayLine->SetLastClose(StrToDecimal(sv, 3));
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
