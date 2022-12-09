#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"


#include<memory>

class CQuandlWebInquiry : public CVirtualWebInquiry {
public:
	CQuandlWebInquiry();
	virtual ~CQuandlWebInquiry();

	virtual bool PrepareNextInquiringString(void) override;
	virtual CString GetNextInquiringMiddleString(long, bool) override { return _T(""); }
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void ConfigerateSession(void) override final;
	virtual bool ParseData(CWebDataPtr pWebData); // 数据为JSon格式, 需要解析
};

typedef shared_ptr<CQuandlWebInquiry> CQuandlWebInquiryPtr;
extern CQuandlWebInquiryPtr gl_pQuandlWebInquiry;
