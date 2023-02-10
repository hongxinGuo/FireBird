#pragma once

#include"VirtualDataSource.h"

class CNeteaseRTDataSource : public CVirtualDataSource {
public:
	CNeteaseRTDataSource();
	~CNeteaseRTDataSource() override = default;

	bool Reset(void) override;

	bool GenerateInquiryMessage(const long lCurrentTime) override;

	void ConfigureSession(void) override; // 在读取网络数据前的准备工作，默认为设置NeteaseRT的m_pSession状态。
	void CreateInquiryMessageFromCurrentProduct(void) override;
	bool ParseData(CWebDataPtr pWebData) override;

	bool InquireRTData(const long lCurrentTime);

protected:
};

using CNeteaseRTDataSourcePtr = shared_ptr<CNeteaseRTDataSource>;
extern CNeteaseRTDataSourcePtr gl_pNeteaseRTDataSource;
