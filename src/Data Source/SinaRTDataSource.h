#pragma once

#include"VirtualDataSource.h"
#include"SinaRTDataSourceImp.h"

using std::array;

class CSinaRTDataSource : public CVirtualDataSource {
public:
	CSinaRTDataSource();
	// ֻ����һ��ʵ��,������ֵ������
	CSinaRTDataSource(const CSinaRTDataSource&) = delete;
	CSinaRTDataSource& operator=(const CSinaRTDataSource&) = delete;
	CSinaRTDataSource(const CSinaRTDataSource&&) noexcept = delete;
	CSinaRTDataSource& operator=(const CSinaRTDataSource&&) noexcept = delete;
	~CSinaRTDataSource() override = default;

	bool GenerateInquiryMessage(const long lCurrentTime) override;
	time_t GetCurrentInquiryTime() const noexcept override { return m_DataSourceContainer.at(0)->GetCurrentInquiryTime(); } // ʹ�õ�һ��DataSource�Ĵ���ʱ��

	static void DisableDataSourceImp(const CSinaRTDataSourceImpPtr& pData) {
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
	array<CSinaRTDataSourceImpPtr, 8> m_DataSourceContainer;
};

using CSinaRTDataSourcePtr = shared_ptr<CSinaRTDataSource>;
extern CSinaRTDataSourcePtr gl_pSinaRTDataSource; // ����ʵʱ�������棬Ψһʵ��
