#pragma once

#include "VirtualWebInquiry.h"

class CNeteaseDayLineWebInquiry : public CVirtualWebInquiry {
public:
	CNeteaseDayLineWebInquiry();
	virtual ~CNeteaseDayLineWebInquiry();

	virtual bool PrepareNextInquiringStr(void) override;

	virtual void UpdateStatusWhenSecceed(CWebDataPtr pData) override final; // �ɹ����պ����ϵͳ״̬�� Ĭ���޶���
	virtual void ClearUpIfReadingWebDataFailed(void) override final; // ����ʧ�ܵĽ��չ���
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final; // �������϶�ȡ�������ݴ��뱾��

	void SetDownLoadingStockCode(CString strStockCode);
	CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode(void) { m_strDownLoadingStockCode = _T(""); }

private:
	CString m_strDownLoadingStockCode;
};

typedef shared_ptr<CNeteaseDayLineWebInquiry> CNeteaseDayLineWebInquiryPtr;
