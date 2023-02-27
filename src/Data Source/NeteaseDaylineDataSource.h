#pragma once

#include"VirtualDataSource.h"
#include"DayLineWebData.h"
#include"ProductNeteaseDayline.h"

class CNeteaseDayLineDataSource : public CVirtualDataSource {
public:
	CNeteaseDayLineDataSource();
	~CNeteaseDayLineDataSource() override = default;

	bool Reset(void) override;

	bool GenerateInquiryMessage(const long lCurrentTime) override;
	void CreateProduct();

	void ConfigureSession(void) override; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Netease DayLine��m_pSession״̬��
	void GenerateCurrentInquiryMessage(void) override;
	void UpdateStatusAfterReading(CWebDataPtr pData) override; // �ɹ����պ����ϵͳ״̬, �˴��������Ʊ����
	bool ParseData(CWebDataPtr pWebData) override { return false; } // ����Ϊ��JSon��ʽ��������

	void SetDownLoadingStockCode(CString strStockCode);
	CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode(void) { m_strDownLoadingStockCode = _T(""); }

protected:
	CString m_strDownLoadingStockCode;
};

using CNeteaseDayLineDataSourcePtr = shared_ptr<CNeteaseDayLineDataSource>;
extern CNeteaseDayLineDataSourcePtr gl_pNeteaseDayLineDataSource;
