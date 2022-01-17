#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

using namespace std;
#include<memory>

class CQuandlWebInquiry : public CVirtualWebInquiry {
public:
	CQuandlWebInquiry();
	virtual ~CQuandlWebInquiry();

	virtual bool PrepareNextInquiringStr(void) override;
	virtual CString GetNextInquiringMiddleStr(long lTotalNumer = 1, bool fSkipUnactiveStock = true) override;
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void PrepareReadingWebData(void) override final;
	virtual void ClearUpIfReadingWebDataFailed(void) override final; // 处理失败的接收过程
	virtual void UpdateAfterReadingWebData(void) override final;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final;
};

typedef shared_ptr<CQuandlWebInquiry> CQuandlWebInquiryPtr;
