#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

class CSinaRTWebInquiry : public CVirtualWebInquiry {
public:
	CSinaRTWebInquiry();
	virtual ~CSinaRTWebInquiry();

	virtual bool PrepareNextInquiringStr(void) override;
	virtual CString GetNextInquiringMiddleStr(long lTotalNumer = 900, bool fCheckActiveStock = false) override;
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final; // �������϶�ȡ�������ݴ��뱾��
};

typedef shared_ptr<CSinaRTWebInquiry> CSinaRTWebInquiryPtr;
