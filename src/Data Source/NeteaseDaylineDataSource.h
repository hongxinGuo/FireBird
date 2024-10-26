#pragma once

#include"VirtualDataSource.h"

class CNeteaseDayLineDataSource : public CVirtualDataSource {
public:
	CNeteaseDayLineDataSource();
	// 只能有一个实例,不允许赋值、拷贝
	CNeteaseDayLineDataSource(const CNeteaseDayLineDataSource&) = delete;
	CNeteaseDayLineDataSource& operator=(const CNeteaseDayLineDataSource&) = delete;
	CNeteaseDayLineDataSource(const CNeteaseDayLineDataSource&&) noexcept = delete;
	CNeteaseDayLineDataSource& operator=(const CNeteaseDayLineDataSource&&) noexcept = delete;
	~CNeteaseDayLineDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(long lCurrentTime) override;
	void CreateProduct();

	void ConfigureInternetOption() override; // 在读取网络数据前的准备工作，默认为设置Netease DayLine的m_pSession状态。
	void GenerateCurrentInquiryMessage() override;
	void UpdateStatus(CWebDataPtr pData) override; // 成功接收后更新系统状态, 此处更新其股票代码

	void SetDownLoadingStockCode(const CString& strStockCode);
	CString GetDownLoadingStockCode() { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode() { m_strDownLoadingStockCode = _T(""); }

protected:
	CString m_strDownLoadingStockCode{ _T("") };
};

using CNeteaseDayLineDataSourcePtr = shared_ptr<CNeteaseDayLineDataSource>;
extern CNeteaseDayLineDataSourcePtr gl_pNeteaseDayLineDataSource;
