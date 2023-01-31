#pragma once

#include"VirtualDataSource.h"

class CTengxunRTDataSource : public CVirtualDataSource {
public:
	CTengxunRTDataSource();
	~CTengxunRTDataSource() override = default;

	bool Reset(void) override;
	bool UpdateStatus(void) override;

	bool Inquire(const long lCurrentTime) override;

	void ConfigureSession(void) override; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Tengxun RT��m_pSession״̬��
	bool PrepareNextInquiringString(void) override;
	bool ParseData(CWebDataPtr pWebData) override { return true; } // ����Ϊ��JSon��ʽ��������

	bool InquireRTData(const long lCurrentTime);

	bool IsTengxunRTDataInvalid(CWebData& WebDataReceived);

protected:
};

using CTengxunRTDataSourcePtr = shared_ptr<CTengxunRTDataSource>;
extern CTengxunRTDataSourcePtr gl_pTengxunRTDataSource;
