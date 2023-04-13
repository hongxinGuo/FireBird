#pragma once

#include"VirtualDataSource.h"

class CTengxunDayLineDataSource : public CVirtualDataSource {
public:
	CTengxunDayLineDataSource();
	~CTengxunDayLineDataSource() override = default;
	bool Reset() override;

	bool GenerateInquiryMessage(const long lCurrentTime) override;

	void ConfigureSession() override; // 在读取网络数据前的准备工作，默认为设置Netease DayLine的m_pSession状态。
	void GenerateCurrentInquiryMessage() override;
	bool ParseData(CWebDataPtr pWebData) override;
	void UpdateStatusAfterReading(CWebDataPtr pData) override; // 成功接收后更新系统状态, 此处更新其股票代码

	virtual bool InquireDayLine();
	vector<CVirtualWebProductPtr> CreateProduct(const CChinaStockPtr& pStock);

	bool IsUpdateDayLine() const noexcept { return m_fUpdateDayLine; }
	void SetUpdateDayLine(bool fFlag) noexcept { m_fUpdateDayLine = fFlag; }

	void SetDownLoadingStockCode(const CString& strStockCode);
	CString GetDownLoadingStockCode() { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode() { m_strDownLoadingStockCode = _T(""); }

protected:
	bool m_fUpdateDayLine; // 每日更新公司日线数据
	CString m_strDownLoadingStockCode;
};

using CTengxunDayLineDataSourcePtr = shared_ptr<CTengxunDayLineDataSource>;
extern CTengxunDayLineDataSourcePtr gl_pTengxunDayLineDataSource;
