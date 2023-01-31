#pragma once

#include"VirtualDataSource.h"
#include"DayLineWebData.h"

class CNeteaseDayLineDataSource : public CVirtualDataSource {
public:
	CNeteaseDayLineDataSource();
	~CNeteaseDayLineDataSource() override = default;

	bool Reset(void) override;
	bool UpdateStatus(void) override;

	bool Inquire(const long lCurrentTime) override;

	bool ParseData(CWebDataPtr pWebData) final { return true; } // ����Ϊ��JSon��ʽ��������

	bool PrepareNextInquiringString(void) override;
	void CreateTotalInquiringString(CString strMiddle) override;
	void ConfigureSession(void) final; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Netease DayLine��m_pSession״̬��

	void UpdateStatusAfterSucceed(CWebDataPtr pData) final; // �ɹ����պ����ϵͳ״̬, �˴��������Ʊ����

	void SetDownLoadingStockCode(CString strStockCode);
	CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode(void) { m_strDownLoadingStockCode = _T(""); }

protected:
	CString m_strDownLoadingStockCode;
};

using CNeteaseDayLineDataSourcePtr = shared_ptr<CNeteaseDayLineDataSource>;
extern CNeteaseDayLineDataSourcePtr gl_pNeteaseDayLineDataSource;
