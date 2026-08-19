#pragma once

#include"VirtualDataSource.h"

class CAlpacaFactory;

using std::unique_ptr;

class CAlpacaDataSource : public CVirtualDataSource {
public:
	CAlpacaDataSource();
	CAlpacaDataSource(const CAlpacaDataSource& other) = delete;
	CAlpacaDataSource(CAlpacaDataSource&& other) noexcept = delete;
	CAlpacaDataSource& operator=(const CAlpacaDataSource& other) = delete;
	CAlpacaDataSource& operator=(CAlpacaDataSource&& other) noexcept = delete;
	~CAlpacaDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(const local_seconds& lCurrentTime) override;

	// 可供申请的项目，目前共2项。
	virtual bool GenerateTradingAsset();
	virtual bool GenerateStockDayLine();

	void ConfigureInternetOption() override;

	void SetInquireStockNumber(int number) noexcept { m_inquireStockNumber = number; }

	bool IsUpdateTradingAsset() const noexcept { return m_bUpdateTradingAsset; }
	void SetUpdateTradingAsset(const bool fFlag) noexcept { m_bUpdateTradingAsset = fFlag; }
	bool IsUpdateStockDayLine() const noexcept { return m_fUpdateStockDayLine; }
	void SetUpdateStockDayLine(const bool fFlag) noexcept { m_fUpdateStockDayLine = fFlag; }

protected:
	unique_ptr<CAlpacaFactory> m_pAlpacaFactory;

	bool m_bUpdateTradingAsset{ true };
	bool m_fUpdateStockDayLine{ false }; //每日更新公司日线数据，但在TiingoDataSource更新完stockProfile后方允许申请。
	int m_inquireStockNumber{ 50 }; // 申请日线的天数。默认申请10天的日线数据

private:
	bool m_fAlpacaDataInquiryFinished{ false };
};

using CAlpacaDataSourcePtr = shared_ptr<CAlpacaDataSource>;

extern CAlpacaDataSourcePtr gl_pAlpacaDataSource;
