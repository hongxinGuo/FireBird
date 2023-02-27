#pragma once

#include"VirtualDataSource.h"

class CSinaRTDataSource : public CVirtualDataSource {
public:
	CSinaRTDataSource();
	~CSinaRTDataSource() override = default;

	bool Reset(void) override;

	bool GenerateInquiryMessage(const long lCurrentTime) override;

	void ConfigureSession(void) override; // 在读取网络数据前的准备工作，默认为设置Sina RT的m_pSession状态。
	void GenerateCurrentInquiryMessage(void) override;
	bool ParseData(CWebDataPtr pWebData) override { return false; } // 数据为非JSon格式，不解析

	bool InquireRTData(const long lCurrentTime);

protected:
};

using CSinaRTDataSourcePtr = shared_ptr<CSinaRTDataSource>;
extern CSinaRTDataSourcePtr gl_pSinaRTDataSource;
