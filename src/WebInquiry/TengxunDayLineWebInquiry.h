#pragma once

#include "VirtualWebInquiry.h"

class CTengxunDayLineWebInquiry : public CVirtualWebInquiry {
public:
	CTengxunDayLineWebInquiry();
	~CTengxunDayLineWebInquiry() override = default;

	bool ParseData(CWebDataPtr pWebData) final;

	bool PrepareNextInquiringString(void) override;
	void CreateTotalInquiringString(CString strMiddle) override;
	void ConfigureSession(void) final; // 在读取网络数据前的准备工作，默认为设置Netease DayLine的m_pSession状态。

	void UpdateStatusAfterSucceed(CWebDataPtr pData) final; // 成功接收后更新系统状态, 此处更新其股票代码

	void SetDownLoadingStockCode(CString strStockCode);
	CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode(void) { m_strDownLoadingStockCode = _T(""); }

private:
	CString m_strDownLoadingStockCode;
};

using CTengxunDayLineWebInquiryPtr = shared_ptr<CTengxunDayLineWebInquiry>;
extern CTengxunDayLineWebInquiryPtr gl_pTengxunDayLineWebInquiry; // 网易日线历史数据
