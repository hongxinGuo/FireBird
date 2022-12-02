#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

class CTengxunRTWebInquiry : public CVirtualWebInquiry {
public:
	CTengxunRTWebInquiry();
	virtual ~CTengxunRTWebInquiry();

	virtual bool ParseData(CWebDataPtr pWebData) override final { return true; } // ����Ϊ��JSon��ʽ��������

	virtual bool PrepareNextInquiringString(void) override;
	virtual CString GetNextInquiringMiddleString(long lTotalNubmer, bool fUsingTotalStockSet = false) override final;
	virtual void ConfigerateSession(void) override final; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Tengxun RT��m_pSession״̬��
	virtual bool ReportStatus(long lNumberOfData) const override;

	bool IsTengxunRTDataInvalid(CWebData& WebDataReceived);
};

typedef shared_ptr<CTengxunRTWebInquiry> CTengxunRTWebInquiryPtr;
extern CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry; // ��Ѷʵʱ���ݲɼ�
