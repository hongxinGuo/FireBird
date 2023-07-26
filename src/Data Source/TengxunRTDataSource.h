#pragma once

#include"VirtualDataSource.h"
#include"TengxunRTDataSourceImp.h"

using std::array;

class CTengxunRTDataSource : public CVirtualDataSource {
public:
	CTengxunRTDataSource();
	// ֻ����һ��ʵ��,������ֵ������
	CTengxunRTDataSource(const CTengxunRTDataSource&) = delete;
	CTengxunRTDataSource& operator=(const CTengxunRTDataSource&) = delete;
	CTengxunRTDataSource(const CTengxunRTDataSource&&) noexcept = delete;
	CTengxunRTDataSource& operator=(const CTengxunRTDataSource&&) noexcept = delete;
	~CTengxunRTDataSource() override = default;

	bool GenerateInquiryMessage(const long lCurrentTime) override;
	time_t GetCurrentInquiryTime() const noexcept override { return m_DataSourceContainer.at(0)->GetCurrentInquiryTime(); } // ʹ�õ�һ��DataSource�Ĵ���ʱ��

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
