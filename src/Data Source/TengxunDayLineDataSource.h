#pragma once

#include"VirtualDataSource.h"

class CTengxunDayLineDataSource : public CVirtualDataSource {
public:
	CTengxunDayLineDataSource();
	~CTengxunDayLineDataSource() override = default;
	bool Reset(void) override;

	bool GenerateInquiryMessage(const long lCurrentTime) override;

	void ConfigureSession(void) override; // 在读取网络数据前的准备工作，默认为设置Netease DayLine的m_pSession状态。
	void GenerateCurrentInquiryMessage(void) override;
	bool ParseData(CWebDataPtr pWebData) override;
	void UpdateStatusAfterReading(CWebDataPtr pData) override; // 成功接收后更新系统状态, 此处更新其股票代码

	virtual bool InquireDayLine(void);
	vector<CVirtualWebProductPtr> CreateProduct(CChinaStockPtr pStock);

	bool IsUpdateDayLine(void) const noexcept { return m_fUpdateDayLine; }
	void SetUpdateDayLine(bool fFlag) noexcept { m_fUpdateDayLine = fFlag; }

	void SetDownLoadingStockCode(CString strStockCode);
	CString GetDownLoadingStockCode(void) { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode(void) { m_strDownLoadingStockCode = _T(""); }

protected:
	bool m_fUpdateDayLine; // 每日更新公司日线数据
	CString m_strDownLoadingStockCode;
};

using CTengxunDayLineDataSourcePtr = shared_ptr<CTengxunDayLineDataSource>;
extern CTengxunDayLineDataSourcePtr gl_pTengxunDayLineDataSource;
