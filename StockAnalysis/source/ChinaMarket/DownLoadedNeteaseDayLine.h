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

	// 提取网易日线历史数据各函数
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

	// 用于调试
	void PushDayLine(CDayLinePtr pData) { m_vTempDayLine.push_back(pData); }

protected:

private:
	//网易日线接收处理相关数据
	CString m_strDownLoadStockSymbol;
	vector<char> m_vDayLineBuffer; // 日线读取缓冲区
	vector<CDayLinePtr> m_vTempDayLine; // 日线数据缓冲区
	INT64 m_lDayLineBufferLength; // 缓冲区大小（不包括最后添加的那个结束符0x000）。
	INT64 m_lCurrentPos;

	CDayLinePtr m_pCurrentProcessingDayLine;
};

typedef shared_ptr<CDownLoadedNeteaseDayLine> CDownLoadedNeteaseDayLinePtr;
