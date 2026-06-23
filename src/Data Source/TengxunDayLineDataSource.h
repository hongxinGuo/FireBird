#pragma once

#include"VirtualDataSource.h"
#include"VirtualWebProduct.h"

class CTengxunDayLineDataSource : public CVirtualDataSource {
public:
	CTengxunDayLineDataSource();
	// 只能有一个实例,不允许赋值、拷贝
	CTengxunDayLineDataSource(const CTengxunDayLineDataSource&) = delete;
	CTengxunDayLineDataSource& operator=(const CTengxunDayLineDataSource&) = delete;
	CTengxunDayLineDataSource(const CTengxunDayLineDataSource&&) noexcept = delete;
	CTengxunDayLineDataSource& operator=(const CTengxunDayLineDataSource&&) noexcept = delete;
	~CTengxunDayLineDataSource() override = default;
	bool Reset() override;

	bool GenerateInquiryMessage(const chrono::local_seconds& currentTime) override;

	void ConfigureInternetOption() override; // 在读取网络数据前的准备工作，默认为设置Tengxun DayLine的m_pSession状态。
	void CheckWebData(const CWebDataPtr& pWebData) override;

	void CreateCurrentInquireString() override;
	void UpdateStatus(const CWebDataPtr& pData) override; // 成功接收后更新系统状态, 此处更新其股票代码

	virtual bool Inquire();
	vector<CVirtualWebProductPtr> CreateProduct(const CChinaStockPtr& pStock) const;

	bool IsUpdateDayLine() const noexcept { return m_fUpdateDayLine; }
	void SetUpdateDayLine(bool fFlag) noexcept { m_fUpdateDayLine = fFlag; }

	void SetDownLoadingStockCode(const string& strStockCode);
	string GetDownLoadingStockCode() { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode() { m_strDownLoadingStockCode = ""; }

protected:
	bool m_fUpdateDayLine{ true }; // 每日更新公司日线数据
	string m_strDownLoadingStockCode{};
};

using CTengxunDayLineDataSourcePtr = shared_ptr<CTengxunDayLineDataSource>;
extern CTengxunDayLineDataSourcePtr gl_pTengxunDayLineDataSource;
