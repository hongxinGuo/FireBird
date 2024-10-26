#pragma once

#include"VirtualDataSource.h"

class CNeteaseDayLineDataSource : public CVirtualDataSource {
public:
	CNeteaseDayLineDataSource();
	// ֻ����һ��ʵ��,������ֵ������
	CNeteaseDayLineDataSource(const CNeteaseDayLineDataSource&) = delete;
	CNeteaseDayLineDataSource& operator=(const CNeteaseDayLineDataSource&) = delete;
	CNeteaseDayLineDataSource(const CNeteaseDayLineDataSource&&) noexcept = delete;
	CNeteaseDayLineDataSource& operator=(const CNeteaseDayLineDataSource&&) noexcept = delete;
	~CNeteaseDayLineDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(long lCurrentTime) override;
	void CreateProduct();

	void ConfigureInternetOption() override; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Netease DayLine��m_pSession״̬��
	void GenerateCurrentInquiryMessage() override;
	void UpdateStatus(CWebDataPtr pData) override; // �ɹ����պ����ϵͳ״̬, �˴��������Ʊ����

	void SetDownLoadingStockCode(const CString& strStockCode);
	CString GetDownLoadingStockCode() { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode() { m_strDownLoadingStockCode = _T(""); }

protected:
	CString m_strDownLoadingStockCode{ _T("") };
};

using CNeteaseDayLineDataSourcePtr = shared_ptr<CNeteaseDayLineDataSource>;
extern CNeteaseDayLineDataSourcePtr gl_pNeteaseDayLineDataSource;
