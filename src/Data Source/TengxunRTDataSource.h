#pragma once

#include"VirtualDataSource.h"

class CTengxunRTDataSource : public CVirtualDataSource {
public:
	CTengxunRTDataSource();
	~CTengxunRTDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(const long lCurrentTime) override;

	void ConfigureSession() override; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Tengxun RT��m_pSession״̬��
	void GenerateCurrentInquiryMessage() override;
	bool ParseData(CWebDataPtr pWebData) override { return false; } // ����Ϊ��JSon��ʽ��������

	bool InquireRTData(const long lCurrentTime);

	static bool IsInvalidTengxunRTData(CWebData& WebDataReceived);

protected:
};

using CTengxunRTDataSourcePtr = shared_ptr<CTengxunRTDataSource>;
extern CTengxunRTDataSourcePtr gl_pTengxunRTDataSource;
