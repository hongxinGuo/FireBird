#pragma once

#include"VirtualDataSource.h"

class CTengxunRTDataSource : public CVirtualDataSource {
public:
	CTengxunRTDataSource();
	~CTengxunRTDataSource() override = default;

	bool Reset(void) override;
	bool UpdateStatus(void) override;

	bool Inquire(const long lCurrentTime) override;

	void ConfigureSession(void) override; // 在读取网络数据前的准备工作，默认为设置Tengxun RT的m_pSession状态。
	bool PrepareNextInquiringString(void) override;
	bool ParseData(CWebDataPtr pWebData) override { return true; } // 数据为非JSon格式，不解析

	bool InquireRTData(const long lCurrentTime);

	bool IsTengxunRTDataInvalid(CWebData& WebDataReceived);

protected:
};

using CTengxunRTDataSourcePtr = shared_ptr<CTengxunRTDataSource>;
extern CTengxunRTDataSourcePtr gl_pTengxunRTDataSource;
