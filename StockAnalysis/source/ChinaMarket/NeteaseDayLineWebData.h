#pragma once

#include"NeteaseDayLineWebInquiry.h"
#include"DayLine.h"

using namespace std;
#include<vector>
#include<memory>

class CNeteaseDayLineWebData : public CObject {
public:
	CNeteaseDayLineWebData();
	~CNeteaseDayLineWebData();
	void Reset(void);

	// 提取网易日线历史数据各函数
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

	// 虽然这个函数与读取新浪实时数据的完全一样，但为了防止以后可能改变的缘故，还是分别实现。
	bool ReadOneValueOfNeteaseDayLine(string& pBuffer, char* buffer, INT64& lCurrentPos);

	// 用于调试
	void PushDayLine(CDayLinePtr pData) { m_vTempDayLine.push_back(pData); }

protected:

private:
	//网易日线接收处理相关数据
	CString m_strStockCode;
	string m_sDataBuffer; // 日线读取缓冲区
	vector<CDayLinePtr> m_vTempDayLine; // 日线数据缓冲区
	INT64 m_lBufferLength; // 缓冲区大小。
	INT64 m_lCurrentPos;

	CDayLinePtr m_pCurrentProcessingDayLine;
};

typedef shared_ptr<CNeteaseDayLineWebData> CNeteaseDayLineWebDataPtr;
