#pragma once

#include"VirtualWebInquiry.h"
#include"WebRTData.h"

class CNeteaseRTWebInquiry : public CVirtualWebInquiry {
public:
	CNeteaseRTWebInquiry();
	virtual ~CNeteaseRTWebInquiry();

	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual bool PrepareNextInquiringString(void) override;
	virtual CString GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet = false) override final;
	virtual void ConfigureSession(void) override final; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����NeteaseRT��m_pSession״̬��

	virtual bool ParseData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CNeteaseRTWebInquiry> CNeteaseRTWebInquiryPtr;
extern CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry; // ����ʵʱ���ݲɼ�
