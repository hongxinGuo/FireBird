#pragma once

#include"NeteaseDayLineWebInquiry.h"
#include"DayLine.h"

#include<vector>
#include<memory>

class CDayLineWebData final {
public:
	CDayLineWebData();
	~CDayLineWebData() = default;
	void Reset(void);

	// ��ȡ����������ʷ���ݸ�����
	void ClearDayLine() noexcept { m_vTempDayLine.clear(); }
	vector<CDayLinePtr>& GetProcessedDayLine(void) { return m_vTempDayLine; }
	void AppendDayLine(const CDayLinePtr pData) { m_vTempDayLine.push_back(pData); }

	void SetStockCode(CNeteaseDayLineWebInquiry* pNeteaseWebDayLineData) noexcept { SetStockCode(pNeteaseWebDayLineData->GetDownLoadingStockCode()); }
	void SetStockCode(const CString strSymbol) noexcept { m_strStockCode = strSymbol; }
	CString GetStockCode(void) { return m_strStockCode; }

	bool TransferWebDataToBuffer(CWebDataPtr pWebData);
	string GetBuffer() { return m_sDataBuffer; }
	bool ProcessNeteaseDayLineData();
	CDayLinePtr ProcessOneNeteaseDayLineData(void);
	INT64 GetBufferLength(void) const noexcept { return m_lBufferLength; }
	INT64 GetCurrentPos(void) const noexcept { return m_lCurrentPos; }
	void SetCurrentPos(const INT64 lValue) noexcept { m_lCurrentPos = lValue; }
	bool SkipNeteaseDayLineInformationHeader(string& sDataBuffer, INT64& lCurrentPos);
	void ReportDayLineDownLoaded(void);

	// ��Ȼ����������ȡ����ʵʱ���ݵ���ȫһ������Ϊ�˷�ֹ�Ժ���ܸı��Ե�ʣ����Ƿֱ�ʵ�֡�
	bool ReadOneValueOfNeteaseDayLine(string& pBuffer, char* buffer, INT64& lCurrentPos);

protected:

private:
	//�������߽��մ����������
	CString m_strStockCode;
	string m_sDataBuffer; // ���߶�ȡ������
	vector<CDayLinePtr> m_vTempDayLine; // �������ݻ�����
	INT64 m_lBufferLength; // ��������С��
	INT64 m_lCurrentPos;
};

using CDayLineWebDataPtr = shared_ptr<CDayLineWebData>;
