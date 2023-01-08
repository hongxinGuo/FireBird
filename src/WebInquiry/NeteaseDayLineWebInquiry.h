#pragma once

#include "VirtualWebInquiry.h"

class CNeteaseDayLineWebInquiry : public CVirtualWebInquiry {
public:
	CNeteaseDayLineWebInquiry();
	~CNeteaseDayLineWebInquiry() override = default;

	bool ParseData(CWebDataPtr pWebData) final { return true; } // ����Ϊ��JSon��ʽ��������

	bool PrepareNextInquiringString(void) override;
	void CreateTotalInquiringString(CString strMiddle) override;
	void ConfigureSession(void) final; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Netease DayLine��m_pSession״̬��

	void UpdateStatusAfterSucceed(CWebDataPtr pData) final; // �ɹ����պ����ϵͳ״̬, �˴��������Ʊ����

	void SetDownLoadingStockCode(CString strStockCode);
	CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode(void) { m_strDownLoadingStockCode = _T(""); }

private:
	CString m_strDownLoadingStockCode;
};

using CNeteaseDayLineWebInquiryPtr = shared_ptr<CNeteaseDayLineWebInquiry>;
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry; // ����������ʷ����
