#pragma once

#include"pch.h"

#include"WebData.h"
#include"NeteaseDayLineWebInquiry.h"
#include"DayLine.h"

using namespace std;
#include<vector>
#include<memory>
//#include<queue>
//#include<deque>
//#include<map>
//#include<array>
//#include<mutex>

class CNeteaseDayLineWebData : public CObject {
public:
	CNeteaseDayLineWebData();
	~CNeteaseDayLineWebData();
	void Reset(void);

	// ��ȡ����������ʷ���ݸ�����
	vector<CDayLinePtr>& GetProcessedDayLine(void) { return m_vTempDayLine; }

	void SetStockCode(CNeteaseDayLineWebInquiry* pNeteaseWebDayLineData) noexcept { SetStockCode(pNeteaseWebDayLineData->GetDownLoadingStockCode()); }
	void SetStockCode(CString strSymbol) noexcept { m_strStockCode = strSymbol; }
	CString GetStockCode(void) { return m_strStockCode; }

	bool TransferNeteaseDayLineWebDataToBuffer(CNeteaseDayLineWebInquiry* pNeteaseWebDayLineData);
	bool TransferWebDataToBuffer(CWebDataPtr pWebData);
	bool ProcessNeteaseDayLineData();
	bool ProcessOneNeteaseDayLineData(void);
	INT64 GetBufferLength(void) noexcept { return m_lBufferLength; }
	INT64 GetCurrentPos(void) noexcept { return m_lCurrentPos; }
	void SetCurrentPos(INT64 lValue) noexcept { m_lCurrentPos = lValue; }
	CDayLinePtr GetCurrentProcessingDayLine(void) { return m_pCurrentProcessingDayLine; }
	bool SkipNeteaseDayLineInformationHeader(void);
	void ReportDayLineDownLoaded(void);

	// ���ڵ���
	void PushDayLine(CDayLinePtr pData) { m_vTempDayLine.push_back(pData); }

protected:

private:
	//�������߽��մ����������
	CString m_strStockCode;
	vector<char> m_vDataBuffer; // ���߶�ȡ������
	vector<CDayLinePtr> m_vTempDayLine; // �������ݻ�����
	INT64 m_lBufferLength; // ��������С�������������ӵ��Ǹ�������0x000����
	INT64 m_lCurrentPos;

	CDayLinePtr m_pCurrentProcessingDayLine;
};

typedef shared_ptr<CNeteaseDayLineWebData> CNeteaseDayLineWebDataPtr;
