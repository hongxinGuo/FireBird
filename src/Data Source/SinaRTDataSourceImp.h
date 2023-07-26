#pragma once

#include"VirtualDataSource.h"

class CSinaRTDataSourceImp : public CVirtualDataSource {
public:
	CSinaRTDataSourceImp();
	~CSinaRTDataSourceImp() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(const long lCurrentTime) override;

	void ConfigureSession() override; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Sina RT��m_pSession״̬��
	void GenerateCurrentInquiryMessage() override;
	bool ParseData(CWebDataPtr pWebData) override { return false; } // ����Ϊ��JSon��ʽ��������

	bool InquireRTData(long lCurrentTime);
};

using CSinaRTDataSourceImpPtr = shared_ptr<CSinaRTDataSourceImp>;
