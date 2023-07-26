#pragma once

#include"VirtualDataSource.h"
#include"TengxunRTDataSourceImp.h"

using std::array;

class CTengxunRTDataSource : public CVirtualDataSource {
public:
	CTengxunRTDataSource();
	// 只能有一个实例,不允许赋值、拷贝
	CTengxunRTDataSource(const CTengxunRTDataSource&) = delete;
	CTengxunRTDataSource& operator=(const CTengxunRTDataSource&) = delete;
	CTengxunRTDataSource(const CTengxunRTDataSource&&) noexcept = delete;
	CTengxunRTDataSource& operator=(const CTengxunRTDataSource&&) noexcept = delete;
	~CTengxunRTDataSource() override = default;

	bool GenerateInquiryMessage(const long lCurrentTime) override;
	time_t GetCurrentInquiryTime() const noexcept override { return m_DataSourceContainer.at(0)->GetCurrentInquiryTime(); } // 使用第一个DataSource的处理时间

	static void DisableDataSourceImp(const CTengxunRTDataSourceImpPtr& pData) {
		pData->SetInquiring(true);
		pData->SetWorkingThreadRunning(true);
	}
	void DisableDataSourceImp(const long lIndex) const { DisableDataSourceImp(m_DataSourceContainer.at(lIndex)); }
	void DisableDataSourceImp() const { for (auto pData : m_DataSourceContainer) DisableDataSourceImp(pData); }

protected:
	array<CTengxunRTDataSourceImpPtr, 8> m_DataSourceContainer;
};

using CTengxunRTDataSourcePtr = shared_ptr<CTengxunRTDataSource>;
extern CTengxunRTDataSourcePtr gl_pTengxunRTDataSource;
