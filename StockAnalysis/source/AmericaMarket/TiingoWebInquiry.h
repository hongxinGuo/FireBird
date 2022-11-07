#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

using namespace std;
#include<memory>

class CTiingoWebInquiry : public CVirtualWebInquiry {
public:
	CTiingoWebInquiry();
	virtual ~CTiingoWebInquiry();

	virtual bool PrepareNextInquiringStr(void) override;
	virtual CString GetNextInquiringMiddleStr(long lTotalNumer = 1, bool fSkipUnactiveStock = true) override final;
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void ConfigerateSession(void) override final; // 设置m_pSession状态。
	virtual bool ParseData(CWebDataPtr pWebData); // 数据为JSon格式, 需要解析
	virtual void ClearUpIfReadingWebDataFailed(void) override final; // 处理失败的接收过程
	virtual void UpdateStatusAfterReadingWebData(void) override final;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final;
};

typedef shared_ptr<CTiingoWebInquiry> CTiingoWebInquiryPtr;
