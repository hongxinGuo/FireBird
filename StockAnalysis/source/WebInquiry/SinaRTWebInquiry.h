#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

class CSinaRTWebInquiry : public CVirtualWebInquiry {
public:
	CSinaRTWebInquiry();
	virtual ~CSinaRTWebInquiry();

	virtual bool ParseData(CWebDataPtr pWebData) override final { return true; } // ����Ϊ��JSon��ʽ��������

	virtual bool PrepareNextInquiringString(void) override final;
	virtual CString GetNextInquiringMiddleString(long lTotalNumer, bool fUsingTotalStockSet = false) override final;
	virtual void ConfigerateSession(void) override final; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Sina RT��m_pSession״̬��
	virtual bool ReportStatus(long lNumberOfData) const override final;
	virtual void ClearUpIfReadingWebDataFailed(void) override final; // ����ʧ�ܵĽ��չ���
	virtual void UpdateStatusAfterReadingWebData(void) override final;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final; // ���轫�����϶�ȡ�������ݴ��뱾��
};

typedef shared_ptr<CSinaRTWebInquiry> CSinaRTWebInquiryPtr;
