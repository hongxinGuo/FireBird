#pragma once

#include"VirtualDataSource.h"

class CAccessoryFactory;

using std::unique_ptr;

class CAccessoryDataSource : public CVirtualDataSource {
public:
	CAccessoryDataSource();
	CAccessoryDataSource(const CAccessoryDataSource& other) = delete;
	CAccessoryDataSource(CAccessoryDataSource&& other) noexcept = delete;
	CAccessoryDataSource& operator=(const CAccessoryDataSource& other) = delete;
	CAccessoryDataSource& operator=(CAccessoryDataSource&& other) noexcept = delete;
	~CAccessoryDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(const local_seconds& lCurrentTime) override;

	// Accessory factory 更新项目
	virtual bool GenerateIndexNasdaq100Stocks();

	void ConfigureInternetOption() override;

	bool IsUpdateIndexNasdaq100Stocks() const noexcept { return m_fUpdateIndexNasdaq100Stocks; }
	virtual void SetUpdateIndexNasdaq100Stocks(bool fFlag) noexcept { m_fUpdateIndexNasdaq100Stocks = fFlag; }

protected:
	unique_ptr<CAccessoryFactory> m_pAccessoryFactory;

	// 每日更新项目
	bool m_fUpdateIndexNasdaq100Stocks{ true };

	bool m_fAccessoryDataInquiryFinished{ false };
};

using CAccessoryDataSourcePtr = shared_ptr<CAccessoryDataSource>;

extern CAccessoryDataSourcePtr gl_pAccessoryDataSource;
