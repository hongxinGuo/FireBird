#pragma once

#include"VirtualDataSource.h"
#include"FinnhubFactory.h"

class CFinnhubDataSource : public CVirtualDataSource {
public:
	CFinnhubDataSource(void);
	~CFinnhubDataSource(void);

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;

	virtual bool Inquiry(long lCurrentTime) override final;

	bool InquiryFinnhub(long lCurrentTime);

	bool InquiryCountryList(void);
	bool InquiryCompanySymbol(void);
	bool InquiryCompanyProfileConcise(void);
	bool InquiryCompanyNews(void);
	bool InquiryCompanyBasicFinancial();
	bool InquiryStockDayLine(void);
	bool InquiryRTQuote(void);
	bool InquiryPeer(void);
	bool InquiryInsiderTransaction(void);
	bool InquiryInsiderSentiment(void);
	bool InquiryEconomicCalendar(void);
	bool InquiryEPSSurprise(void);
	bool InquiryForexExchange(void);
	bool InquiryForexSymbol(void);
	bool InquiryForexDayLine(void);
	bool InquiryCryptoExchange(void);
	bool InquiryCryptoSymbol(void);
	bool InquiryCryptoDayLine(void);

	bool IsCountryListUpdated(void) noexcept { return m_fCountryListUpdated; }
	void SetCountryListUpdated(bool fFlag) noexcept { m_fCountryListUpdated = fFlag; }
	bool IsSymbolUpdated(void) noexcept { return m_fSymbolUpdated; }
	void SetSymbolUpdated(bool fFlag) noexcept { m_fSymbolUpdated = fFlag; }
	bool IsStockProfileUpdated(void) noexcept { return m_fStockProfileUpdated; }
	void SetStockProfileUpdated(bool fFlag) noexcept { m_fStockProfileUpdated = fFlag; }
	bool IsCompanyNewsUpdated(void) noexcept { return m_fCompanyNewsUpdated; }
	void SetCompanyNewsUpdated(bool fFlag) noexcept { m_fCompanyNewsUpdated = fFlag; }
	bool IsCompanyPriceMetricsUpdated(void) noexcept { return m_fCompanyPriceMetricsUpdated; }
	void SetCompanyPriceMetricsUpdated(bool fFlag) noexcept { m_fCompanyPriceMetricsUpdated = fFlag; }
	bool IsStockBasicFinancialUpdated(void) noexcept { return m_fStockBasicFinancialUpdated; }
	void SetStockBasicFinancialUpdated(bool fFlag) noexcept { m_fStockBasicFinancialUpdated = fFlag; }
	bool IsStockDayLineUpdated(void) noexcept { return m_fDayLineUpdated; }
	void SetDayLineUpdated(bool fFlag) noexcept { m_fDayLineUpdated = fFlag; }
	bool IsForexExchangeUpdated(void) noexcept { return m_fForexExchangeUpdated; }
	void SetForexExchangeUpdated(bool fFlag) noexcept { m_fForexExchangeUpdated = fFlag; }
	bool IsForexSymbolUpdated(void) noexcept { return m_fForexSymbolUpdated; }
	void SetForexSymbolUpdated(bool fFlag) noexcept { m_fForexSymbolUpdated = fFlag; }
	bool IsForexDayLineUpdated(void) noexcept { return m_fForexDayLineUpdated; }
	void SetForexDayLineUpdated(bool fFlag) noexcept { m_fForexDayLineUpdated = fFlag; }
	bool IsCryptoExchangeUpdated(void) noexcept { return m_fCryptoExchangeUpdated; }
	void SetCryptoExchangeUpdated(bool fFlag) noexcept { m_fCryptoExchangeUpdated = fFlag; }
	bool IsCryptoSymbolUpdated(void) noexcept { return m_fCryptoSymbolUpdated; }
	void SetCryptoSymbolUpdated(bool fFlag) noexcept { m_fCryptoSymbolUpdated = fFlag; }
	bool IsCryptoDayLineUpdated(void) noexcept { return m_fCryptoDayLineUpdated; }
	void SetCryptoDayLineUpdated(bool fFlag) noexcept { m_fCryptoDayLineUpdated = fFlag; }
	bool IsPeerUpdated(void) noexcept { return m_fPeerUpdated; }
	void SetPeerUpdated(bool fFlag) noexcept { m_fPeerUpdated = fFlag; }
	bool IsInsiderTransactionUpdated(void) noexcept { return m_fInsiderTransactionUpdated; }
	void SetInsiderTransactionUpdated(bool fFlag) noexcept { m_fInsiderTransactionUpdated = fFlag; }
	bool IsInsiderSentimentUpdated(void) noexcept { return m_fInsiderSentimentUpdated; }
	void SetInsiderSentimentUpdated(bool fFlag) noexcept { m_fInsiderSentimentUpdated = fFlag; }
	bool IsEconomicCalendarUpdated(void) noexcept { return m_fEconomicCalendarUpdated; }
	void SetEconomicCalendarUpdated(bool fFlag) noexcept { m_fEconomicCalendarUpdated = fFlag; }
	bool IsEPSSurpriseUpdated(void) noexcept { return m_fEPSSurpriseUpdated; }
	void SetEPSSurpriseUpdated(bool fFlag) noexcept { m_fEPSSurpriseUpdated = fFlag; }

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
};

typedef shared_ptr<CFinnhubDataSource> CFinnhubDataSourcePtr;
extern CFinnhubDataSourcePtr gl_pDataSourceFinnhub;
