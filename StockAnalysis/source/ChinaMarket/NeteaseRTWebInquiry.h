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
	virtual void ConfigerateSession(void) override final; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����NeteaseRT��m_pSession״̬��
	virtual void ClearUpIfReadingWebDataFailed(void) override final { TRACE("��ȡ����ʵʱ���ݳ���\n"); }
	virtual bool ParseData(CWebDataPtr pWebData) override final;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final;
};

typedef shared_ptr<CNeteaseRTWebInquiry> CNeteaseRTWebInquiryPtr;
