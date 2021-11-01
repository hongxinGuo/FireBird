#pragma once

#include "VirtualWebInquiry.h"

class CNeteaseDayLineWebInquiry : public CVirtualWebInquiry {
public:
	CNeteaseDayLineWebInquiry();
	virtual ~CNeteaseDayLineWebInquiry();

	virtual bool PrepareNextInquiringStr(void) override;
	virtual void StartReadingThread(void) override;

	virtual void PrepareBeforeReadingWebData(void) override final; // 在读取网络数据前的准备工作，默认无动作。
	virtual void UpdateStatusWhenSecceed(CWebDataPtr pData) override final; // 成功接收后更新系统状态。 默认无动作
	virtual void ProcessFailedReading(void) override final; // 处理失败的接收过程
	virtual void StoreWebData(CWebDataPtr pWebData) override final; // 将网络上读取到的数据存入本地

	void SetDownLoadingStockCode(CString strStockCode);
	CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode(void) { m_strDownLoadingStockCode = _T(""); }

private:
	CString m_strDownLoadingStockCode;
};

typedef shared_ptr<CNeteaseDayLineWebInquiry> CNeteaseDayLineWebInquiryPtr;
