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
	virtual CString GetNextInquiringMiddleStr(long lTotalNumer = 1, bool fSkipUnactiveStock = true) override;
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void ConfigerateSession(void) override final; // ����m_pSession״̬��
	virtual void ClearUpIfReadingWebDataFailed(void) override final; // ����ʧ�ܵĽ��չ���
	virtual void UpdateAfterReadingWebData(void) override final;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final;
};

typedef shared_ptr<CTiingoWebInquiry> CTiingoWebInquiryPtr;
