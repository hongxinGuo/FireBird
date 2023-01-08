//////////////////////////////////////////////////////////////////////////////
//
// CFinnhubWebInquiryӦ�ñ�����Ϊfinal�����ڲ��Ե���Ҫ, CMockFinnhubWebInquiryҪ�̳и��࣬
// �ʶ��������ඨ��Ϊfinal��ֻ�ǽ���Ҫ���麯������Ϊfinal��
//
// �������Ƶ���Ҳͬ������
//
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "VirtualWebInquiry.h"

#include<memory>

class CFinnhubWebInquiry : public CVirtualWebInquiry {
public:
	CFinnhubWebInquiry();
	~CFinnhubWebInquiry() override;

	bool PrepareNextInquiringString(void) override;
	void CreateTotalInquiringString(CString strMiddle) override;
	CString GetNextInquiringMiddleString(long, bool) final { return _T(""); }
	bool ReportStatus(long lNumberOfData) const override;
	void ConfigureSession(void) final;
	bool ParseData(CWebDataPtr pWebData) override; // ����ΪJSon��ʽ, ��Ҫ����

protected:
};

using CFinnhubWebInquiryPtr = shared_ptr<CFinnhubWebInquiry>;
extern CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry; // Finnhub.io֤ȯ��Ϣ
