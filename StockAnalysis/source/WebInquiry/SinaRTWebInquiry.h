#pragma once

#include "VirtualWebInquiry.h"

class CSinaRTWebInquiry : public CVirtualWebInquiry {
public:
	CSinaRTWebInquiry();
	~CSinaRTWebInquiry() override = default;

	virtual bool ParseData(CWebDataPtr pWebData) override final { return true; } // ����Ϊ��JSon��ʽ��������

	virtual bool PrepareNextInquiringString(void) override final;
	virtual CString GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) override final;
	virtual void ConfigureSession(void) override final; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Sina RT��m_pSession״̬��
	virtual bool ReportStatus(long lNumberOfData) const override final;
};

typedef shared_ptr<CSinaRTWebInquiry> CSinaRTWebInquiryPtr;
extern CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry; // ����ʵʱ���ݲɼ�
