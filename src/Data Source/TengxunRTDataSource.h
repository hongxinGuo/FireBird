#pragma once

#include"VirtualDataSource.h"

class CTengxunRTDataSource : public CVirtualDataSource {
public:
	CTengxunRTDataSource();
	CTengxunRTDataSource(const CTengxunRTDataSource&) = delete;
	CTengxunRTDataSource& operator=(const CTengxunRTDataSource&) = delete;
	CTengxunRTDataSource(const CTengxunRTDataSource&&) noexcept = delete;
	CTengxunRTDataSource& operator=(const CTengxunRTDataSource&&) noexcept = delete;
	~CTengxunRTDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(long lCurrentTime) override;

	void ConfigureInternetOption() override; // 在读取网络数据前的准备工作，默认为设置Tengxun RT的m_pSession状态。
	void CreateCurrentInquireString() override;

	static bool IsInvalidTengxunRTData(const CWebData& WebDataReceived);
};

using CTengxunRTDataSourcePtr = shared_ptr<CTengxunRTDataSource>;

extern CTengxunRTDataSourcePtr gl_pTengxunRTDataSource;
