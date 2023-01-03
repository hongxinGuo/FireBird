#pragma once

#include "VirtualWebInquiry.h"

class CNeteaseDayLineWebInquiry : public CVirtualWebInquiry {
public:
	CNeteaseDayLineWebInquiry();
	~CNeteaseDayLineWebInquiry() override = default;

	virtual bool ParseData(CWebDataPtr pWebData) override final { return true; } // 数据为非JSon格式，不解析

	virtual bool PrepareNextInquiringString(void) override;
	virtual void ConfigureSession(void) override final; // 在读取网络数据前的准备工作，默认为设置Netease DayLine的m_pSession状态。

	virtual void UpdateStatusAfterSucceed(CWebDataPtr pData) override final; // 成功接收后更新系统状态, 此处更新其股票代码

	void SetDownLoadingStockCode(CString strStockCode);
	CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode(void) { m_strDownLoadingStockCode = _T(""); }

private:
	CString m_strDownLoadingStockCode;
};

typedef shared_ptr<CNeteaseDayLineWebInquiry> CNeteaseDayLineWebInquiryPtr;
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry; // 网易日线历史数据
