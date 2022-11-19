#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

class CNeteaseDayLineWebInquiry : public CVirtualWebInquiry {
public:
	CNeteaseDayLineWebInquiry();
	virtual ~CNeteaseDayLineWebInquiry();

	virtual bool ParseData(CWebDataPtr pWebData) override final { return true; } // ����Ϊ��JSon��ʽ��������

	virtual bool PrepareNextInquiringString(void) override;
	virtual void ConfigerateSession(void) override final; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Netease Dayline��m_pSession״̬��

	virtual void UpdateStatusWhenSecceed(CWebDataPtr pData) override final; // �ɹ����պ����ϵͳ״̬, �˴��������Ʊ����
	virtual void ClearUpIfReadingWebDataFailed(void) override final; // ����ʧ�ܵĽ��չ���
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final; // ���轫�����϶�ȡ�������ݴ��뱾��

	void SetDownLoadingStockCode(CString strStockCode);
	CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode(void) { m_strDownLoadingStockCode = _T(""); }

private:
	CString m_strDownLoadingStockCode;
};

typedef shared_ptr<CNeteaseDayLineWebInquiry> CNeteaseDayLineWebInquiryPtr;
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry; // ����������ʷ����
