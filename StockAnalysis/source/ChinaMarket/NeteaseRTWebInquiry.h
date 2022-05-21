#pragma once

#include"VirtualWebInquiry.h"
#include"WebRTData.h"

class CNeteaseRTWebInquiry : public CVirtualWebInquiry {
public:
	CNeteaseRTWebInquiry();
	virtual ~CNeteaseRTWebInquiry();


	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual bool PrepareNextInquiringStr(void) override;
	virtual CString GetNextInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet = false) override;
	virtual void ClearUpIfReadingWebDataFailed(void) override final { TRACE("��ȡ����ʵʱ���ݳ���\n"); }
	virtual bool ParseAndStoreData(CWebDataPtr pWebData);
	virtual bool ParseData(CWebDataPtr pWebData) override final; // �ѷ������ϲ���ParseAndStoreData�У�
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final; // ����ʵʱ�����Ѵ������轫���������С��ѷ������ϲ���ParseAndStoreData�У�
};

UINT ThreadParseAndStoreNeteaseRTData(CNeteaseRTWebInquiry* pInquiry, CWebDataPtr pWebData);

typedef shared_ptr<CNeteaseRTWebInquiry> CNeteaseRTWebInquiryPtr;
