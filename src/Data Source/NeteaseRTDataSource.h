#pragma once

#include"VirtualDataSource.h"

class CNeteaseRTDataSource : public CVirtualDataSource {
public:
	CNeteaseRTDataSource();
	// ֻ����һ��ʵ��,������ֵ������
	CNeteaseRTDataSource(const CNeteaseRTDataSource&) = delete;
	CNeteaseRTDataSource& operator=(const CNeteaseRTDataSource&) = delete;
	CNeteaseRTDataSource(const CNeteaseRTDataSource&&) noexcept = delete;
	CNeteaseRTDataSource& operator=(const CNeteaseRTDataSource&&) noexcept = delete;
	~CNeteaseRTDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(const long lCurrentTime) override;

	void ConfigureSession() override; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����NeteaseRT��m_pSession״̬��
	void GenerateCurrentInquiryMessage() override;
	bool ParseData(CWebDataPtr pWebData) override;

	virtual void InquireRTData(long lCurrentTime);
};

using CNeteaseRTDataSourcePtr = shared_ptr<CNeteaseRTDataSource>;
extern CNeteaseRTDataSourcePtr gl_pNeteaseRTDataSource;
