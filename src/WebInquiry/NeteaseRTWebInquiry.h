#pragma once

#include"VirtualWebInquiry.h"

class CNeteaseRTWebInquiry : public CVirtualWebInquiry {
public:
	CNeteaseRTWebInquiry();
	~CNeteaseRTWebInquiry() override = default;

	bool ReportStatus(long lNumberOfData) const override;
	bool PrepareNextInquiringString(void) override;
	CString GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) final;
	void ConfigureSession(void) final; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����NeteaseRT��m_pSession״̬��

	bool ParseData(CWebDataPtr pWebData) final;
};

using CNeteaseRTWebInquiryPtr = shared_ptr<CNeteaseRTWebInquiry>;
extern CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry; // ����ʵʱ���ݲɼ�
