#pragma once

#include"VirtualDataSource.h"

class CSinaRTDataSource : public CVirtualDataSource {
public:
	CSinaRTDataSource();
	~CSinaRTDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(const long lCurrentTime) override;

	void ConfigureSession() override; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Sina RT��m_pSession״̬��
	void GenerateCurrentInquiryMessage() override;
	bool ParseData(CWebDataPtr pWebData) override { return false; } // ����Ϊ��JSon��ʽ��������

	bool InquireRTData(const long lCurrentTime);

protected:
};

using CSinaRTDataSourcePtr = shared_ptr<CSinaRTDataSource>;
extern CSinaRTDataSourcePtr gl_pSinaRTDataSource;
