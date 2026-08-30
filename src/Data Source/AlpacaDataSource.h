#pragma once

#include <cpr/cprtypes.h>

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
	int GetInquireStockNumber() const noexcept { return m_inquireStockNumber; }

	bool IsUpdateTradingAsset() const noexcept { return m_bUpdateTradingAsset; }
	void SetUpdateTradingAsset(const bool fFlag) noexcept { m_bUpdateTradingAsset = fFlag; }
	bool IsUpdateStockDayLine() const noexcept { return m_fUpdateStockDayLine; }
	void SetUpdateStockDayLine(const bool fFlag) noexcept { m_fUpdateStockDayLine = fFlag; }

	void SetApiKey(const string& key) noexcept { m_api_key = key; }
	string GetApiKey() const noexcept { return m_api_key; }
	void SetSecretKey(const string& secretKey) noexcept { m_secret_key = secretKey; }
	string GetSecretKey() const noexcept { return m_secret_key; }

	std::pair<string, string> GetApiKeyPair() const noexcept { return std::pair{ "APCA-API-KEY-ID", m_api_key }; }
	std::pair<string, string> GetSecretKeyPair() const noexcept { return std::pair{ "APCA-API-SECRET-KEY", m_secret_key }; }
	cpr::Header GetHeader() const noexcept { return cpr::Header{ GetApiKeyPair(), GetSecretKeyPair(), { "accept", "application/json" } }; }

protected:
	unique_ptr<CAlpacaFactory> m_pAlpacaFactory;

	bool m_bUpdateTradingAsset{ true };
	bool m_fUpdateStockDayLine{ false }; //每日更新公司日线数据，但在TiingoDataSource更新完stockProfile后方允许申请。
	int m_inquireStockNumber{ 50 }; // 申请日线的天数。默认申请10天的日线数据

	// Alpaca token, 
	string m_api_key;
	string m_secret_key;

private:
	bool m_fAlpacaDataInquiryFinished{ false };
};

using CAlpacaDataSourcePtr = shared_ptr<CAlpacaDataSource>;

extern CAlpacaDataSourcePtr gl_pAlpacaDataSource;
