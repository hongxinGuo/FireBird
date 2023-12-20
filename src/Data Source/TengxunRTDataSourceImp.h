#pragma once

#include"VirtualDataSource.h"

class CTengxunRTDataSourceImp : public CVirtualDataSource {
public:
	CTengxunRTDataSourceImp();
	~CTengxunRTDataSourceImp() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(long lCurrentTime) override;

	void ConfigureSession() override; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Tengxun RT��m_pSession״̬��
	void GenerateCurrentInquiryMessage() override;
	bool ParseData(CWebDataPtr pWebData) override { return false; } // ����Ϊ��JSon��ʽ��������

	static bool IsInvalidTengxunRTData(const CWebData& WebDataReceived);
};

using CTengxunRTDataSourceImpPtr = shared_ptr<CTengxunRTDataSourceImp>;
