#pragma once

#include"VirtualDataSource.h"

class CTengxunRTDataSourceImp : public CVirtualDataSource {
public:
	CTengxunRTDataSourceImp();
	~CTengxunRTDataSourceImp() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(long lCurrentTime) override;

	void ConfigureSession() override; // 在读取网络数据前的准备工作，默认为设置Tengxun RT的m_pSession状态。
	void GenerateCurrentInquiryMessage() override;

	static bool IsInvalidTengxunRTData(const CWebData& WebDataReceived);
};

using CTengxunRTDataSourceImpPtr = shared_ptr<CTengxunRTDataSourceImp>;
