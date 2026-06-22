#pragma once

#include"VirtualDataSource.h"
#include"VirtualWebProduct.h"

class CEastmoneyDayLineDataSource : public CVirtualDataSource {
public:
	CEastmoneyDayLineDataSource();
	// 只能有一个实例,不允许赋值、拷贝
	CEastmoneyDayLineDataSource(const CEastmoneyDayLineDataSource&) = delete;
	CEastmoneyDayLineDataSource& operator=(const CEastmoneyDayLineDataSource&) = delete;
	CEastmoneyDayLineDataSource(const CEastmoneyDayLineDataSource&&) noexcept = delete;
	CEastmoneyDayLineDataSource& operator=(const CEastmoneyDayLineDataSource&&) noexcept = delete;
	~CEastmoneyDayLineDataSource() override = default;
	bool Reset() override;

	bool GenerateInquiryMessage(const chrono::local_seconds& currentTime) override;

	void ConfigureInternetOption() override; // 在读取网络数据前的准备工作，默认为设置Netease DayLine的m_pSession状态。
	void CheckWebData(const CWebDataPtr& pWebData) override;

	void CreateCurrentInquireString() override;
	void UpdateStatus(const CWebDataPtr& pData) override; // 成功接收后更新系统状态, 此处更新其股票代码

	virtual bool Inquire();
	CVirtualWebProductPtr CreateProduct(const CChinaStockPtr& pStock) const;

	bool IsUpdateDayLine() const noexcept { return m_fUpdateDayLine; }
	void SetUpdateDayLine(bool fFlag) noexcept { m_fUpdateDayLine = fFlag; }

	void SetDownLoadingStockCode(const string& strStockCode);
	string GetDownLoadingStockCode() { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode() { m_strDownLoadingStockCode = ""; }

protected:
	bool m_fUpdateDayLine{ true }; // 每日更新公司日线数据
	string m_strDownLoadingStockCode{};

	chrono::time_point<chrono::steady_clock> m_PrevInquireTimePoint{};
};

using CEastmoneyDayLineDataSourcePtr = shared_ptr<CEastmoneyDayLineDataSource>;
extern CEastmoneyDayLineDataSourcePtr gl_pEastmoneyDayLineDataSource;
