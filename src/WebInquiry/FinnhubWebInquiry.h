//////////////////////////////////////////////////////////////////////////////
//
// CFinnhubWebInquiry应该被定义为final。由于测试的需要, CMockFinnhubWebInquiry要继承该类，
// 故而不将此类定义为final，只是将需要的虚函数定义为final。
//
// 其他相似的类也同样处理。
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
	bool ParseData(CWebDataPtr pWebData) override; // 数据为JSon格式, 需要解析

protected:
};

using CFinnhubWebInquiryPtr = shared_ptr<CFinnhubWebInquiry>;
extern CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry; // Finnhub.io证券信息
