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

	// 可供申请的项目，目前共1项。
	virtual bool GenerateStockDayLine();

	void ConfigureInternetOption() override;

	bool IsUpdateStockDayLine() const noexcept { return m_fUpdateStockDayLine; }
	void SetUpdateStockDayLine(const bool fFlag) noexcept { m_fUpdateStockDayLine = fFlag; }

protected:
	unique_ptr<CAlpacaFactory> m_pAlpacaFactory;

	bool m_fUpdateStockDayLine{ true }; //每日更新公司日线数据

private:
	bool m_fAlpacaDataInquiryFinished{ false };
};

using CAlpacaDataSourcePtr = shared_ptr<CAlpacaDataSource>;

extern CAlpacaDataSourcePtr gl_pAlpacaDataSource;
