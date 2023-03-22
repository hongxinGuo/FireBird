#pragma once

#include"VirtualDataSource.h"

class CNeteaseRTDataSource : public CVirtualDataSource {
public:
	CNeteaseRTDataSource();
	~CNeteaseRTDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(const long lCurrentTime) override;

	void ConfigureSession() override; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����NeteaseRT��m_pSession״̬��
	void GenerateCurrentInquiryMessage() override;
	bool ParseData(CWebDataPtr pWebData) override;

	virtual bool InquireRTData(const long lCurrentTime);

protected:
};

using CNeteaseRTDataSourcePtr = shared_ptr<CNeteaseRTDataSource>;
extern CNeteaseRTDataSourcePtr gl_pNeteaseRTDataSource;
