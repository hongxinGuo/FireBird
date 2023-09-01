#pragma once

#include"VirtualDataSource.h"

class CNeteaseRTDataSource : public CVirtualDataSource {
public:
	CNeteaseRTDataSource();
	// 只能有一个实例,不允许赋值、拷贝
	CNeteaseRTDataSource(const CNeteaseRTDataSource&) = delete;
	CNeteaseRTDataSource& operator=(const CNeteaseRTDataSource&) = delete;
	CNeteaseRTDataSource(const CNeteaseRTDataSource&&) noexcept = delete;
	CNeteaseRTDataSource& operator=(const CNeteaseRTDataSource&&) noexcept = delete;
	~CNeteaseRTDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(const long lCurrentTime) override;

	void ConfigureSession() override; // 在读取网络数据前的准备工作，默认为设置NeteaseRT的m_pSession状态。
	void GenerateCurrentInquiryMessage() override;
	bool ParseData(CWebDataPtr pWebData) override;

	virtual void InquireRTData(long lCurrentTime);
};

using CNeteaseRTDataSourcePtr = shared_ptr<CNeteaseRTDataSource>;
extern CNeteaseRTDataSourcePtr gl_pNeteaseRTDataSource;
