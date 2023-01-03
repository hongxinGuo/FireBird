#pragma once

#include "VirtualWebInquiry.h"

class CSinaRTWebInquiry : public CVirtualWebInquiry {
public:
	CSinaRTWebInquiry();
	~CSinaRTWebInquiry() override = default;

	virtual bool ParseData(CWebDataPtr pWebData) override final { return true; } // 数据为非JSon格式，不解析

	virtual bool PrepareNextInquiringString(void) override final;
	virtual CString GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) override final;
	virtual void ConfigureSession(void) override final; // 在读取网络数据前的准备工作，默认为设置Sina RT的m_pSession状态。
	virtual bool ReportStatus(long lNumberOfData) const override final;
};

typedef shared_ptr<CSinaRTWebInquiry> CSinaRTWebInquiryPtr;
extern CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry; // 新浪实时数据采集
