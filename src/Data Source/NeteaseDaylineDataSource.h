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

	void ConfigureSession(void) override; // 在读取网络数据前的准备工作，默认为设置Netease DayLine的m_pSession状态。
	void GenerateCurrentInquiryMessage(void) override;
	void UpdateStatusAfterReading(CWebDataPtr pData) override; // 成功接收后更新系统状态, 此处更新其股票代码
	bool ParseData(CWebDataPtr pWebData) override { return false; } // 数据为非JSon格式，不解析

	void SetDownLoadingStockCode(CString strStockCode);
	CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode(void) { m_strDownLoadingStockCode = _T(""); }

protected:
	CString m_strDownLoadingStockCode;
};

using CNeteaseDayLineDataSourcePtr = shared_ptr<CNeteaseDayLineDataSource>;
extern CNeteaseDayLineDataSourcePtr gl_pNeteaseDayLineDataSource;
