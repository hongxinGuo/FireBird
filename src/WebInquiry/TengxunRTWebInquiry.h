#pragma once

#include "VirtualWebInquiry.h"

class CTengxunRTWebInquiry : public CVirtualWebInquiry {
public:
	CTengxunRTWebInquiry();
	~CTengxunRTWebInquiry() override = default;

	virtual bool ParseData(CWebDataPtr pWebData) override final { return true; } // ����Ϊ��JSon��ʽ��������

	virtual bool PrepareNextInquiringString(void) override;
	virtual CString GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) override final;
	virtual void ConfigureSession(void) override final; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Tengxun RT��m_pSession״̬��
	virtual bool ReportStatus(long lNumberOfData) const override;

	bool IsTengxunRTDataInvalid(CWebData& WebDataReceived);
};

typedef shared_ptr<CTengxunRTWebInquiry> CTengxunRTWebInquiryPtr;
extern CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry; // ��Ѷʵʱ���ݲɼ�
