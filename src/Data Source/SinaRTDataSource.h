#pragma once

#include"VirtualDataSource.h"
#include <cpr/cprtypes.h>

class CSinaRTDataSource : public CVirtualDataSource {
public:
	CSinaRTDataSource();
	CSinaRTDataSource(const CSinaRTDataSource& other) = delete;
	CSinaRTDataSource(CSinaRTDataSource&& other) noexcept = delete;
	CSinaRTDataSource& operator=(const CSinaRTDataSource& other) = delete;
	CSinaRTDataSource& operator=(CSinaRTDataSource&& other) noexcept = delete;
	~CSinaRTDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(const local_seconds& currentTime) override;

	void CreateCurrentInquireString() override;

	void CheckWebData(const string& text) override;

	cpr::Header GetHeader() const noexcept { return cpr::Header{ { "Referer", "https://finance.sina.com.cn" } }; }
};

using CSinaRTDataSourcePtr = shared_ptr<CSinaRTDataSource>;
extern CSinaRTDataSourcePtr gl_pSinaRTDataSource; // 新浪实时数据引擎，唯一实例
