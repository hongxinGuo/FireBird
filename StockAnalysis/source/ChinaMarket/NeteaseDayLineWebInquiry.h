#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

class CNeteaseDayLineWebInquiry : public CVirtualWebInquiry {
public:
	CNeteaseDayLineWebInquiry();
	virtual ~CNeteaseDayLineWebInquiry();

	virtual bool ParseData(CWebDataPtr pWebData) override final { return true; } // 数据为非JSon格式，不解析
	virtual bool ProcessData(CWebDataPtr pWebData) override final; // 处理接收到的数据。

	virtual bool PrepareNextInquiringStr(void) override;

	virtual void UpdateStatusWhenSecceed(CWebDataPtr pData) override final; // 成功接收后更新系统状态, 此处更新其股票代码
	virtual void ClearUpIfReadingWebDataFailed(void) override final; // 处理失败的接收过程
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final; // 无需将网络上读取到的数据存入本地

	void SetDownLoadingStockCode(CString strStockCode);
	CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode(void) { m_strDownLoadingStockCode = _T(""); }

private:
	CString m_strDownLoadingStockCode;
};

typedef shared_ptr<CNeteaseDayLineWebInquiry> CNeteaseDayLineWebInquiryPtr;
