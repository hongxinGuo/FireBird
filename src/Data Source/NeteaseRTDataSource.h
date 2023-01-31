#pragma once

#include"VirtualDataSource.h"

class CNeteaseRTDataSource final : public CVirtualDataSource {
public:
	CNeteaseRTDataSource();
	~CNeteaseRTDataSource() override = default;

	bool Reset(void) override;
	bool UpdateStatus(void) override;

	bool Inquire(const long lCurrentTime) override;

	bool InquireRTData(const long lCurrentTime);

	bool ReportStatus(long lNumberOfData) const override;
	bool PrepareNextInquiringString(void) override;
	CString GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) final;
	void ConfigureSession(void) final; // 在读取网络数据前的准备工作，默认为设置NeteaseRT的m_pSession状态。

	bool ParseData(CWebDataPtr pWebData) final;

protected:
};

using CNeteaseRTDataSourcePtr = shared_ptr<CNeteaseRTDataSource>;
extern CNeteaseRTDataSourcePtr gl_pNeteaseRTDataSource;
