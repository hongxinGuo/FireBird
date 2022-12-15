#pragma once

#include "VirtualWebInquiry.h"

#include<memory>

class CQuandlWebInquiry : public CVirtualWebInquiry {
public:
	CQuandlWebInquiry();
	~CQuandlWebInquiry() override = default;

	virtual bool PrepareNextInquiringString(void) override;
	virtual CString GetNextInquiringMiddleString(long, bool) override { return _T(""); }
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void ConfigureSession(void) override final;
	virtual bool ParseData(CWebDataPtr pWebData) override; // ����ΪJSon��ʽ, ��Ҫ����
};

typedef shared_ptr<CQuandlWebInquiry> CQuandlWebInquiryPtr;
extern CQuandlWebInquiryPtr gl_pQuandlWebInquiry;
