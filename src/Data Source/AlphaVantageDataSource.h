#pragma once

#include"VirtualDataSource.h"
#include"AlphaVantageFactory.h"

class CAlphaVantageDataSource : public CVirtualDataSource {
public:
	CAlphaVantageDataSource();
	CAlphaVantageDataSource(const CAlphaVantageDataSource& other) = delete;
	CAlphaVantageDataSource(CAlphaVantageDataSource&& other) noexcept = delete;
	CAlphaVantageDataSource& operator=(const CAlphaVantageDataSource& other) = delete;
	CAlphaVantageDataSource& operator=(CAlphaVantageDataSource&& other) noexcept = delete;
	~CAlphaVantageDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(const chrono::local_seconds& lCurrentTime) override;

	// 可供申请的项目，目前共1项。
	virtual bool GenerateStockSplit();
	virtual bool GenerateStockDayLine();

	void ConfigureInternetOption() override;

	bool IsUpdateStockSplit() const noexcept { return m_fUpdateStockSplit; }
	void SetUpdateStockSplit(const bool fFlag) noexcept { m_fUpdateStockSplit = fFlag; }
	bool IsUpdateStockDayLine() const noexcept { return m_fUpdateStockDayLine; }
	void SetUpdateStockDayLine(const bool fFlag) noexcept { m_fUpdateStockDayLine = fFlag; }

protected:
	CAlphaVantageFactory m_AlphaVantageFactory;

	bool m_fUpdateStockSplit{ true }; //每日更新公司股票分割数据 
	bool m_fUpdateStockDayLine{ true }; //每日更新公司日线数据

private:
	bool m_fAlphaVantageDataInquiryFinished{ false };
};

using CAlphaVantageDataSourcePtr = shared_ptr<CAlphaVantageDataSource>;

extern CAlphaVantageDataSourcePtr gl_pAlphaVantageDataSource;
