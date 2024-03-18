#pragma once

#include"VirtualDataSource.h"

class CTengxunRTDataSource : public CVirtualDataSource {
public:
	CTengxunRTDataSource();
	~CTengxunRTDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(long lCurrentTime) override;

	void ConfigureInternetOption() override; // 在读取网络数据前的准备工作，默认为设置Tengxun RT的m_pSession状态。
	void GenerateCurrentInquiryMessage() override;

	static bool IsInvalidTengxunRTData(const CWebData& WebDataReceived);
};

using CTengxunRTDataSourcePtr = shared_ptr<CTengxunRTDataSource>;

extern CTengxunRTDataSourcePtr gl_pTengxunRTDataSource;
