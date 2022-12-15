//////////////////////////////////////////////////////////////////////////////
//
// CFinnhuaWebInquiry应该被定义为final。由于测试的需要, CMockFinnhubWebInquiry要继承该类，
// 故而不将此类定义为final，只是将需要的虚函数定义为final。
//
// 其他相似的类也同样处理。
//
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

#include<memory>

class CFinnhubWebInquiry : public CVirtualWebInquiry {
public:
	CFinnhubWebInquiry();
	virtual ~CFinnhubWebInquiry();

	virtual bool PrepareNextInquiringString(void) override;
	virtual CString GetNextInquiringMiddleString(long, bool) override final { return _T(""); }
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void ConfigureSession(void) override final;
	virtual bool ParseData(CWebDataPtr pWebData); // 数据为JSon格式, 需要解析

protected:
};

typedef shared_ptr<CFinnhubWebInquiry> CFinnhubWebInquiryPtr;
extern CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry; // Finnhub.io证券信息
