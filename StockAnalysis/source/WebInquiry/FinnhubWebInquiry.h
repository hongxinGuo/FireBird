//////////////////////////////////////////////////////////////////////////////
//
// CFinnhubWebInquiryӦ�ñ�����Ϊfinal�����ڲ��Ե���Ҫ, CMockFinnhubWebInquiryҪ�̳и��࣬
// �ʶ��������ඨ��Ϊfinal��ֻ�ǽ���Ҫ���麯������Ϊfinal��
//
// �������Ƶ���Ҳͬ��������
//
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "VirtualWebInquiry.h"

#include<memory>

class CFinnhubWebInquiry : public CVirtualWebInquiry {
public:
	CFinnhubWebInquiry();
	~CFinnhubWebInquiry() override;

	virtual bool PrepareNextInquiringString(void) override;
	virtual CString GetNextInquiringMiddleString(long, bool) override final { return _T(""); }
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void ConfigureSession(void) override final;
	virtual bool ParseData(CWebDataPtr pWebData) override; // ����ΪJSon��ʽ, ��Ҫ����

protected:
};

typedef shared_ptr<CFinnhubWebInquiry> CFinnhubWebInquiryPtr;
extern CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry; // Finnhub.io֤ȯ��Ϣ