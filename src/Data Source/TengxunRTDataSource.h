#pragma once

#include"VirtualDataSource.h"

class CTengxunRTDataSource final : public CVirtualDataSource {
public:
	CTengxunRTDataSource();
	~CTengxunRTDataSource() override = default;

	bool Reset(void) override;
	bool UpdateStatus(void) override;

	bool Inquire(const long lCurrentTime) override;

	bool InquireRTData(const long lCurrentTime);

	virtual bool ParseData(CWebDataPtr pWebData) override final { return true; } // 数据为非JSon格式，不解析

	bool PrepareNextInquiringString(void) override;
	CString GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) override final;
	void ConfigureSession(void) override final; // 在读取网络数据前的准备工作，默认为设置Tengxun RT的m_pSession状态。
	bool ReportStatus(long lNumberOfData) const override;

	bool IsTengxunRTDataInvalid(CWebData& WebDataReceived);

protected:
};

using CTengxunRTDataSourcePtr = shared_ptr<CTengxunRTDataSource>;
extern CTengxunRTDataSourcePtr gl_pTengxunRTDataSource;
