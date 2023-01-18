#pragma once

#include"NeteaseDayLineWebInquiry.h"
#include"DayLine.h"

#include<vector>
#include<memory>

class CNeteaseDayLineWebData final {
public:
	CNeteaseDayLineWebData();
	~CNeteaseDayLineWebData() = default;
	void Reset(void);

	// ��ȡ����������ʷ���ݸ�����
	vector<CDayLinePtr>& GetProcessedDayLine(void) { return m_vTempDayLine; }

	void SetStockCode(CNeteaseDayLineWebInquiry* pNeteaseWebDayLineData) noexcept { SetStockCode(pNeteaseWebDayLineData->GetDownLoadingStockCode()); }
	void SetStockCode(const CString strSymbol) noexcept { m_strStockCode = strSymbol; }
	CString GetStockCode(void) { return m_strStockCode; }

	bool TransferNeteaseDayLineWebDataToBuffer(CNeteaseDayLineWebInquiry* pNeteaseWebDayLineData);
	bool TransferWebDataToBuffer(CWebDataPtr pWebData);
	bool ProcessNeteaseDayLineData();
	bool ProcessOneNeteaseDayLineData(void);
	INT64 GetBufferLength(void) const noexcept { return m_lBufferLength; }
	INT64 GetCurrentPos(void) const noexcept { return m_lCurrentPos; }
	void SetCurrentPos(const INT64 lValue) noexcept { m_lCurrentPos = lValue; }
	CDayLinePtr GetCurrentProcessingDayLine(void) { return m_pCurrentProcessingDayLine; }
	bool SkipNeteaseDayLineInformationHeader(void);
	void ReportDayLineDownLoaded(void);

	// ��Ȼ����������ȡ����ʵʱ���ݵ���ȫһ������Ϊ�˷�ֹ�Ժ���ܸı��Ե�ʣ����Ƿֱ�ʵ�֡�
	bool ReadOneValueOfNeteaseDayLine(string& pBuffer, char* buffer, INT64& lCurrentPos);

	// ���ڵ���
	void PushDayLine(const CDayLinePtr pData) { m_vTempDayLine.push_back(pData); }

protected:

private:
	//�������߽��մ����������
	CString m_strStockCode;
	string m_sDataBuffer; // ���߶�ȡ������
	vector<CDayLinePtr> m_vTempDayLine; // �������ݻ�����
	INT64 m_lBufferLength; // ��������С��
	INT64 m_lCurrentPos;

	CDayLinePtr m_pCurrentProcessingDayLine;
};

using CNeteaseDayLineWebDataPtr = shared_ptr<CNeteaseDayLineWebData>;
