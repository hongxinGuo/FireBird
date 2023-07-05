#pragma once

#include"VirtualDataSource.h"
#include"TengxunRTDataSourceImp.h"

using std::array;

class CTengxunRTDataSource : public CVirtualDataSource {
public:
	CTengxunRTDataSource();
	~CTengxunRTDataSource() override = default;

	bool GenerateInquiryMessage(long lCurrentTime) override;
	time_t GetCurrentInquiryTime() const noexcept override { return m_DataSourceContainer.at(0)->GetCurrentInquiryTime(); } // 使用第一个DataSource的处理时间

	static void DisableDataSourceImp(const CTengxunRTDataSourceImpPtr& pData) {
		pData->SetInquiring(true);
		pData->SetWorkingThreadRunning(true);
	}

	void DisableDataSourceImp(const long lIndex) const {
		const auto pData = m_DataSourceContainer.at(lIndex);
		DisableDataSourceImp(pData);
	}
	void DisableDataSourceImp() const {
		for (auto pData : m_DataSourceContainer) DisableDataSourceImp(pData);
	}

protected:
	array<CTengxunRTDataSourceImpPtr, 8> m_DataSourceContainer;
};

using CTengxunRTDataSourcePtr = shared_ptr<CTengxunRTDataSource>;
extern CTengxunRTDataSourcePtr gl_pTengxunRTDataSource;
