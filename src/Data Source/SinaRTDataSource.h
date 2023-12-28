#pragma once

#include"VirtualDataSource.h"

class CSinaRTDataSource : public CVirtualDataSource {
public:
	CSinaRTDataSource();
	CSinaRTDataSource(const CSinaRTDataSource& other) = delete;
	CSinaRTDataSource(CSinaRTDataSource&& other) noexcept = delete;
	CSinaRTDataSource& operator=(const CSinaRTDataSource& other) = delete;
	CSinaRTDataSource& operator=(CSinaRTDataSource&& other) noexcept = delete;
	~CSinaRTDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(long lCurrentTime) override;

	void ConfigureSession() override; // 在读取网络数据前的准备工作，默认为设置Sina RT的m_pSession状态。
	void GenerateCurrentInquiryMessage() override;
	bool ParseData(CWebDataPtr pWebData) override { return false; } // 数据为非JSon格式，不解析
};

using CSinaRTDataSourcePtr = shared_ptr<CSinaRTDataSource>;
extern CSinaRTDataSourcePtr gl_pSinaRTDataSource; // 新浪实时数据引擎，唯一实例
