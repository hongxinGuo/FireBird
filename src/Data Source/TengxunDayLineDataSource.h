#pragma once

#include"VirtualDataSource.h"

class CTengxunDayLineDataSource final : public CVirtualDataSource {
public:
	CTengxunDayLineDataSource();
	~CTengxunDayLineDataSource() override = default;
	bool Reset(void) override;
	bool UpdateStatus(void) override;

	bool Inquire(const long lCurrentTime) override;

	bool InquireDayLine(void);
	vector<CVirtualWebProductPtr> CreateProduct(CChinaStockPtr pStock);

	bool IsUpdateDayLine(void) const noexcept { return m_fUpdateDayLine; }
	void SetUpdateDayLine(bool fFlag) noexcept { m_fUpdateDayLine = fFlag; }

	bool ParseData(CWebDataPtr pWebData) final;

	bool PrepareNextInquiringString(void) override;
	void CreateTotalInquiringString(CString strMiddle) override;
	void ConfigureSession(void) final; // 在读取网络数据前的准备工作，默认为设置Netease DayLine的m_pSession状态。

	void UpdateStatusAfterSucceed(CWebDataPtr pData) final; // 成功接收后更新系统状态, 此处更新其股票代码

	void SetDownLoadingStockCode(CString strStockCode);
	CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode(void) { m_strDownLoadingStockCode = _T(""); }

protected:
	bool m_fUpdateDayLine; // 每日更新公司日线数据
	CString m_strDownLoadingStockCode;
};

using CTengxunDayLineDataSourcePtr = shared_ptr<CTengxunDayLineDataSource>;
extern CTengxunDayLineDataSourcePtr gl_pTengxunDayLineDataSource;
