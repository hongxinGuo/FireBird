#pragma once

#include"VirtualDataSource.h"
#include"FinnhubFactory.h"

class CFinnhubDataSource final : public CVirtualDataSource {
public:
	CFinnhubDataSource(void);
	~CFinnhubDataSource(void) override = default;

	bool Reset(void) final;
	bool UpdateStatus(void) final;

	bool Inquire(long lCurrentTime) override;

	bool InquireFinnhub(long lCurrentTime);

	bool InquireCountryList(void);
	bool InquireCompanySymbol(void);
	bool InquireCompanyProfileConcise(void);
	bool InquireCompanyNews(void);
	bool InquireCompanyBasicFinancial();
	bool InquireStockDayLine(void);
	bool InquireRTQuote(void);
	bool InquirePeer(void);
	bool InquireInsiderTransaction(void);
	bool InquireInsiderSentiment(void);
	bool InquireEconomicCalendar(void);
	bool InquireEPSSurprise(void);
	bool InquireForexExchange(void);
	bool InquireForexSymbol(void);
	bool InquireForexDayLine(void);
	bool InquireCryptoExchange(void);
	bool InquireCryptoSymbol(void);
	bool InquireCryptoDayLine(void);

	bool IsCountryListUpdated(void) const noexcept { return m_fCountryListUpdated; }
	void SetCountryListUpdated(const bool fFlag) noexcept { m_fCountryListUpdated = fFlag; }
	bool IsSymbolUpdated(void) const noexcept { return m_fSymbolUpdated; }
	void SetSymbolUpdated(const bool fFlag) noexcept { m_fSymbolUpdated = fFlag; }
	bool IsStockProfileUpdated(void) const noexcept { return m_fStockProfileUpdated; }
	void SetStockProfileUpdated(const bool fFlag) noexcept { m_fStockProfileUpdated = fFlag; }
	bool IsCompanyNewsUpdated(void) const noexcept { return m_fCompanyNewsUpdated; }
	void SetCompanyNewsUpdated(const bool fFlag) noexcept { m_fCompanyNewsUpdated = fFlag; }
	bool IsCompanyPriceMetricsUpdated(void) const noexcept { return m_fCompanyPriceMetricsUpdated; }
	void SetCompanyPriceMetricsUpdated(const bool fFlag) noexcept { m_fCompanyPriceMetricsUpdated = fFlag; }
	bool IsStockBasicFinancialUpdated(void) const noexcept { return m_fStockBasicFinancialUpdated; }
	void SetStockBasicFinancialUpdated(const bool fFlag) noexcept { m_fStockBasicFinancialUpdated = fFlag; }
	bool IsStockDayLineUpdated(void) const noexcept { return m_fDayLineUpdated; }
	void SetDayLineUpdated(const bool fFlag) noexcept { m_fDayLineUpdated = fFlag; }
	bool IsForexExchangeUpdated(void) const noexcept { return m_fForexExchangeUpdated; }
	void SetForexExchangeUpdated(const bool fFlag) noexcept { m_fForexExchangeUpdated = fFlag; }
	bool IsForexSymbolUpdated(void) const noexcept { return m_fForexSymbolUpdated; }
	void SetForexSymbolUpdated(const bool fFlag) noexcept { m_fForexSymbolUpdated = fFlag; }
	bool IsForexDayLineUpdated(void) const noexcept { return m_fForexDayLineUpdated; }
	void SetForexDayLineUpdated(const bool fFlag) noexcept { m_fForexDayLineUpdated = fFlag; }
	bool IsCryptoExchangeUpdated(void) const noexcept { return m_fCryptoExchangeUpdated; }
	void SetCryptoExchangeUpdated(const bool fFlag) noexcept { m_fCryptoExchangeUpdated = fFlag; }
	bool IsCryptoSymbolUpdated(void) const noexcept { return m_fCryptoSymbolUpdated; }
	void SetCryptoSymbolUpdated(const bool fFlag) noexcept { m_fCryptoSymbolUpdated = fFlag; }
	bool IsCryptoDayLineUpdated(void) const noexcept { return m_fCryptoDayLineUpdated; }
	void SetCryptoDayLineUpdated(const bool fFlag) noexcept { m_fCryptoDayLineUpdated = fFlag; }
	bool IsPeerUpdated(void) const noexcept { return m_fPeerUpdated; }
	void SetPeerUpdated(const bool fFlag) noexcept { m_fPeerUpdated = fFlag; }
	bool IsInsiderTransactionUpdated(void) const noexcept { return m_fInsiderTransactionUpdated; }
	void SetInsiderTransactionUpdated(const bool fFlag) noexcept { m_fInsiderTransactionUpdated = fFlag; }
	bool IsInsiderSentimentUpdated(void) const noexcept { return m_fInsiderSentimentUpdated; }
	void SetInsiderSentimentUpdated(const bool fFlag) noexcept { m_fInsiderSentimentUpdated = fFlag; }
	bool IsEconomicCalendarUpdated(void) const noexcept { return m_fEconomicCalendarUpdated; }
	void SetEconomicCalendarUpdated(const bool fFlag) noexcept { m_fEconomicCalendarUpdated = fFlag; }
	bool IsEPSSurpriseUpdated(void) const noexcept { return m_fEPSSurpriseUpdated; }
	void SetEPSSurpriseUpdated(const bool fFlag) noexcept { m_fEPSSurpriseUpdated = fFlag; }

protected:
	long m_lCurrentUpdateDayLinePos; // 由于更新一次日线数据超过24小时，故而将此计数器声明为类变量，且无需每日重置。
	long m_lCurrentUpdateEPSSurprisePos; // 此变量无需每日更新

	bool m_fCountryListUpdated;
	bool m_fSymbolUpdated; // 每日更新公司代码库
	bool m_fStockProfileUpdated; // 每日更新公司简介
	bool m_fCompanyNewsUpdated; // 每日更新公司新闻
	bool m_fCompanyPriceMetricsUpdated;
	bool m_fStockBasicFinancialUpdated; // 每日更新公司日线
	bool m_fDayLineUpdated; // 每日更新公司日线数据
	bool m_fForexExchangeUpdated; // 每日更新Forex交易所
	bool m_fForexSymbolUpdated; // 每日更新Forex交易所代码
	bool m_fForexDayLineUpdated; // 每日更新Forex日线数据
	bool m_fCryptoExchangeUpdated; // 每日更新Crypto交易所
	bool m_fCryptoSymbolUpdated; // 每日更新Crypto交易所代码
	bool m_fCryptoDayLineUpdated; // 每日更新Crypto日线数据
	bool m_fPeerUpdated; // 每90天更新Peers数据
	bool m_fInsiderTransactionUpdated; // 每30天更新InsiderTransaction数据
	bool m_fInsiderSentimentUpdated; // 每30天更新InsiderSentiment数据
	bool m_fEconomicCalendarUpdated; // 每日更新经济日历数据
	bool m_fEPSSurpriseUpdated;

	CFinnhubFactory m_FinnhubFactory;

private:
	bool m_fInquiringFinnhubStockSymbol = {false};
	bool m_fInquiringFinnhubStockProfile = {false};
	bool m_fInquiringFinnhubCompanyNews = {false};
	bool m_fInquiringFinnhubCompanyBasicFinancial = {false};
	bool m_fInquiringFinnhubStockDayLine = {false};
	bool m_fInquiringFinnhubStockInsiderTransaction = {false};
	bool m_fInquiringFinnhubStockInsiderSentiment = {false};
	bool m_fInquiringFinnhubStockPeer = {false};
	bool m_fInquiringFinnhubStockEPSSurprise = {false};
	bool m_fInquiringFinnhubForexDayLine = {false};
	bool m_fInquiringFinnhubCryptoDayLine = {false};
};

using CFinnhubDataSourcePtr = shared_ptr<CFinnhubDataSource>;
extern CFinnhubDataSourcePtr gl_pFinnhubDataSource;
