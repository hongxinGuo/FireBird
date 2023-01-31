#pragma once

#include"VirtualDataSource.h"

class CSinaRTDataSource : public CVirtualDataSource {
public:
	CSinaRTDataSource();
	~CSinaRTDataSource() override = default;

	bool Reset(void) override;
	bool UpdateStatus(void) override;

	bool Inquire(const long lCurrentTime) override;

	bool InquireRTData(const long lCurrentTime);

	bool ParseData(CWebDataPtr pWebData) override { return true; } // 数据为非JSon格式，不解析

	bool PrepareNextInquiringString(void) override;
	CString GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) override;
	void ConfigureSession(void) override; // 在读取网络数据前的准备工作，默认为设置Sina RT的m_pSession状态。
	bool ReportStatus(long lNumberOfData) const override;

protected:
};

using CSinaRTDataSourcePtr = shared_ptr<CSinaRTDataSource>;
extern CSinaRTDataSourcePtr gl_pSinaRTDataSource;
