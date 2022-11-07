#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

using namespace std;
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
	virtual void ClearUpIfReadingWebDataFailed(void) override final; // 处理失败的接收过程
	virtual void UpdateStatusAfterReadingWebData(void) override final;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final;
};

typedef shared_ptr<CQuandlWebInquiry> CQuandlWebInquiryPtr;
