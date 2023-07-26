#pragma once

#include"VirtualDataSource.h"

class CSinaRTDataSourceImp : public CVirtualDataSource {
public:
	CSinaRTDataSourceImp();
	~CSinaRTDataSourceImp() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(const long lCurrentTime) override;

	void ConfigureSession() override; // 在读取网络数据前的准备工作，默认为设置Sina RT的m_pSession状态。
	void GenerateCurrentInquiryMessage() override;
	bool ParseData(CWebDataPtr pWebData) override { return false; } // 数据为非JSon格式，不解析

	bool InquireRTData(long lCurrentTime);
};

using CSinaRTDataSourceImpPtr = shared_ptr<CSinaRTDataSourceImp>;
