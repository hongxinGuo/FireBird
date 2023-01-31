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

	bool ParseData(CWebDataPtr pWebData) final { return true; } // 数据为非JSon格式，不解析

	bool PrepareNextInquiringString(void) override;
	void CreateTotalInquiringString(CString strMiddle) override;
	void ConfigureSession(void) final; // 在读取网络数据前的准备工作，默认为设置Netease DayLine的m_pSession状态。

	void UpdateStatusAfterSucceed(CWebDataPtr pData) final; // 成功接收后更新系统状态, 此处更新其股票代码

	void SetDownLoadingStockCode(CString strStockCode);
	CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode(void) { m_strDownLoadingStockCode = _T(""); }

protected:
	CString m_strDownLoadingStockCode;
};

using CNeteaseDayLineDataSourcePtr = shared_ptr<CNeteaseDayLineDataSource>;
extern CNeteaseDayLineDataSourcePtr gl_pNeteaseDayLineDataSource;
