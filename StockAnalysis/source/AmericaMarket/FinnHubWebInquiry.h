#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

using namespace std;
#include<memory>

class CFinnhubWebInquiry : public CVirtualWebInquiry {
public:
	CFinnhubWebInquiry();
	virtual ~CFinnhubWebInquiry();

	virtual bool PrepareNextInquiringStr(void) override;
	virtual CString GetNextInquiringMiddleStr(long lTotalNumer = 1, bool fSkipUnactiveStock = true) override;
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void ConfigerateSession(void) override final;
	virtual bool ParseData(CWebDataPtr pWebData); // 数据为JSon格式, 需要解析
	virtual void ClearUpIfReadingWebDataFailed(void) override final; // 处理失败的接收过程
	virtual void UpdateAfterReadingWebData(void) override final;
	virtual void StoreWebData(CWebDataPtr pWebDataReceived) override final;

protected:
};

typedef shared_ptr<CFinnhubWebInquiry> CFinnhubWebInquiryPtr;
