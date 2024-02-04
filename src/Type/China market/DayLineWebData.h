#pragma once

#include"DayLine.h"

class CDayLineWebData final {
public:
	CDayLineWebData();
	~CDayLineWebData() = default;
	void Reset();

	// ��ȡ����������ʷ���ݸ�����
	void ClearDayLine() noexcept { m_vTempDayLine.clear(); }
	std::vector<CDayLinePtr>& GetProcessedDayLine() { return m_vTempDayLine; }
	void AppendDayLine(const CDayLinePtr& pData) { m_vTempDayLine.push_back(pData); }
	//void SetStockCode(CNeteaseDayLineDataSourcePtr pNeteaseWebDayLineData) noexcept { SetStockCode(pNeteaseWebDayLineData->GetDownLoadingStockCode()); }

	void SetStockCode(const CString& strSymbol) noexcept { m_strStockCode = strSymbol; }
	CString GetStockCode() { return m_strStockCode; }

	bool TransferWebDataToBuffer(const CWebDataPtr& pWebData);
	string GetBuffer() { return m_sDataBuffer; }

	bool ProcessNeteaseDayLineData2();
	string_view GetCurrentNeteaseData();
	CDayLinePtr ProcessOneNeteaseDayLine(const string_view& svData) const;
	INT64 GetCurrentPos() const noexcept { return m_lCurrentPos; }
	void SetCurrentPos(const INT64 lValue) noexcept { m_lCurrentPos = lValue; }
	static bool SkipNeteaseDayLineInformationHeader(const string& sDataBuffer, INT64& lCurrentPos);

	void ReportDayLineDownLoaded();

	// ��Ȼ����������ȡ����ʵʱ���ݵ���ȫһ������Ϊ�˷�ֹ�Ժ���ܸı��Ե�ʣ����Ƿֱ�ʵ�֡�
	bool ReadOneValueOfNeteaseDayLine(const string& pBuffer, char* buffer, INT64& lCurrentPos);

private:
	CString m_strStockCode;
	string m_sDataBuffer; // ���߶�ȡ������
	vector<CDayLinePtr> m_vTempDayLine; // �������ݻ�����
	INT64 m_lCurrentPos;
};

using CDayLineWebDataPtr = shared_ptr<CDayLineWebData>;
