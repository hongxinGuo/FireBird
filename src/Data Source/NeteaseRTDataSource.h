#pragma once

#include"VirtualDataSource.h"

class CNeteaseRTDataSource : public CVirtualDataSource {
public:
	CNeteaseRTDataSource();
	~CNeteaseRTDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(long lCurrentTime) override;

	void ConfigureSession() override; // 在读取网络数据前的准备工作，默认为设置NeteaseRT的m_pSession状态。
	void GenerateCurrentInquiryMessage() override;
	bool ParseData(CWebDataPtr pWebData) override;

	virtual bool InquireRTData(long lCurrentTime);
};

using CNeteaseRTDataSourcePtr = shared_ptr<CNeteaseRTDataSource>;
extern CNeteaseRTDataSourcePtr gl_pNeteaseRTDataSource;
