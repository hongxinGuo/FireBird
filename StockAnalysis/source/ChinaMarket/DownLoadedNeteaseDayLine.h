#pragma once

#include"pch.h"

#include"NeteaseDayLineWebInquiry.h"
#include"DayLine.h"

using namespace std;
#include<vector>
#include<memory>
#include<queue>
#include<deque>
#include<map>
#include<array>
#include<mutex>

class CDownLoadedNeteaseDayLine : public CObject {
public:
	CDownLoadedNeteaseDayLine();
	~CDownLoadedNeteaseDayLine();
	void Reset(void);

	// ��ȡ����������ʷ���ݸ�����
	vector<CDayLinePtr>& GetProcessedDayLine(void) { return m_vTempDayLine; }

	void SetDownLoadStockSymbol(CNeteaseDayLineWebInquiry* pNeteaseWebDayLineData) noexcept { SetDownLoadStockSymbol(pNeteaseWebDayLineData->GetDownLoadingStockCode()); }
	void SetDownLoadStockSymbol(CString strSymbol) noexcept { m_strDownLoadStockSymbol = strSymbol; }
	CString GetDownLoadedStockSymbol(void) { return m_strDownLoadStockSymbol; }

	bool TransferNeteaseDayLineWebDataToBuffer(CNeteaseDayLineWebInquiry* pNeteaseWebDayLineData);
	bool ProcessNeteaseDayLineData();
	bool ProcessOneNeteaseDayLineData(void);
	INT64 GetDayLineBufferLength(void) noexcept { return m_lDayLineBufferLength; }
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
	CString m_strDownLoadStockSymbol;
	vector<char> m_vDayLineBuffer; // ���߶�ȡ������
	vector<CDayLinePtr> m_vTempDayLine; // �������ݻ�����
	INT64 m_lDayLineBufferLength; // ��������С�������������ӵ��Ǹ�������0x000����
	INT64 m_lCurrentPos;

	CDayLinePtr m_pCurrentProcessingDayLine;
};

typedef shared_ptr<CDownLoadedNeteaseDayLine> CDownLoadedNeteaseDayLinePtr;
