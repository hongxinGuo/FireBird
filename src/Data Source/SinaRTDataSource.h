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

	void ConfigureSession() override; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Sina RT��m_pSession״̬��
	void GenerateCurrentInquiryMessage() override;
	bool ParseData(CWebDataPtr pWebData) override { return false; } // ����Ϊ��JSon��ʽ��������
};

using CSinaRTDataSourcePtr = shared_ptr<CSinaRTDataSource>;
extern CSinaRTDataSourcePtr gl_pSinaRTDataSource; // ����ʵʱ�������棬Ψһʵ��
