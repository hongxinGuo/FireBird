#pragma once

#include"VirtualDataSource.h"
#include"FinnhubFactory.h"

class CFinnhubDataSource : public CVirtualDataSource {
public:
	CFinnhubDataSource(void);
	~CFinnhubDataSource(void) override = default;

	bool Reset(void) override;
	void UpdateStatus(void) override;
	bool GenerateInquiryMessage(long lCurrentTime) override;

	void ConfigureSession(void) override;
	bool ParseData(CWebDataPtr pWebData) override; // 数据为JSon格式, 需要解析

	bool InquireFinnhub(long lCurrentTime);

	virtual bool InquireCountryList(void);
	virtual bool InquireCompanySymbol(void);
	virtual bool InquireCompanyProfileConcise(void);
	virtual bool InquireCompanyNews(void);
	virtual bool InquireCompanyBasicFinancial();
	virtual bool InquireStockDayLine(void);
	virtual bool InquireRTQuote(void);
	virtual bool InquirePeer(void);
	virtual bool InquireInsiderTransaction(void);
	virtual bool InquireInsiderSentiment(void);
	virtual bool InquireEconomicCalendar(void);
	virtual bool InquireEPSSurprise(void);
	virtual bool InquireForexExchange(void);
	virtual bool InquireForexSymbol(void);
	virtual bool InquireForexDayLine(void);
	virtual bool InquireCryptoExchange(void);
	virtual bool InquireCryptoSymbol(void);
	virtual bool InquireCryptoDayLine(void);

	bool IsUpdateCountryList(void) const noexcept { return m_fUpdateCountryList; }
	void SetUpdateCountryList(const bool fFlag) noexcept { m_fUpdateCountryList = fFlag; }
	bool IsUpdateSymbol(void) const noexcept { return m_fUpdateSymbol; }
	void SetUpdateSymbol(const bool fFlag) noexcept { m_fUpdateSymbol = fFlag; }
	bool IsUpdateStockProfile(void) const noexcept { return m_fUpdateStockProfile; }
	void SetUpdateStockProfile(const bool fFlag) noexcept { m_fUpdateStockProfile = fFlag; }
	bool IsUpdateCompanyNews(void) const noexcept { return m_fUpdateCompanyNews; }
	void SetUpdateCompanyNews(const bool fFlag) noexcept { m_fUpdateCompanyNews = fFlag; }
	bool IsUpdateCompanyPriceMetrics(void) const noexcept { return m_fUpdateCompanyPriceMetrics; }
	void SetUpdateCompanyPriceMetrics(const bool fFlag) noexcept { m_fUpdateCompanyPriceMetrics = fFlag; }
	bool IsUpdateStockBasicFinancial(void) const noexcept { return m_fUpdateStockBasicFinancial; }
	void SetUpdateStockBasicFinancial(const bool fFlag) noexcept { m_fUpdateStockBasicFinancial = fFlag; }
	bool IsUpdateStockDayLine(void) const noexcept { return m_fUpdateStockDayLine; }
	void SetUpdateStockDayLine(const bool fFlag) noexcept { m_fUpdateStockDayLine = fFlag; }
	bool IsUpdateForexExchange(void) const noexcept { return m_fUpdateForexExchange; }
	void SetUpdateForexExchange(const bool fFlag) noexcept { m_fUpdateForexExchange = fFlag; }
	bool IsUpdateForexSymbol(void) const noexcept { return m_fUpdateForexSymbol; }
	void SetUpdateForexSymbol(const bool fFlag) noexcept { m_fUpdateForexSymbol = fFlag; }
	bool IsUpdateForexDayLine(void) const noexcept { return m_fUpdateForexDayLine; }
	void SetUpdateForexDayLine(const bool fFlag) noexcept { m_fUpdateForexDayLine = fFlag; }
	bool IsUpdateCryptoExchange(void) const noexcept { return m_fUpdateCryptoExchange; }
	void SetUpdateCryptoExchange(const bool fFlag) noexcept { m_fUpdateCryptoExchange = fFlag; }
	bool IsUpdateCryptoSymbol(void) const noexcept { return m_fUpdateCryptoSymbol; }
	void SetUpdateCryptoSymbol(const bool fFlag) noexcept { m_fUpdateCryptoSymbol = fFlag; }
	bool IsUpdateCryptoDayLine(void) const noexcept { return m_fUpdateCryptoDayLine; }
	void SetUpdateCryptoDayLine(const bool fFlag) noexcept { m_fUpdateCryptoDayLine = fFlag; }
	bool IsUpdatePeer(void) const noexcept { return m_fUpdatePeer; }
	void SetUpdatePeer(const bool fFlag) noexcept { m_fUpdatePeer = fFlag; }
	bool IsUpdateInsiderTransaction(void) const noexcept { return m_fUpdateInsiderTransaction; }
	void SetUpdateInsiderTransaction(const bool fFlag) noexcept { m_fUpdateInsiderTransaction = fFlag; }
	bool IsUpdateInsiderSentiment(void) const noexcept { return m_fUpdateInsiderSentiment; }
	void SetUpdateInsiderSentiment(const bool fFlag) noexcept { m_fUpdateInsiderSentiment = fFlag; }
	bool IsUpdateEconomicCalendar(void) const noexcept { return m_fUpdateEconomicCalendar; }
	void SetUpdateEconomicCalendar(const bool fFlag) noexcept { m_fUpdateEconomicCalendar = fFlag; }
	bool IsUpdateEPSSurprise(void) const noexcept { return m_fUpdateEPSSurprise; }
	void SetUpdateEPSSurprise(const bool fFlag) noexcept { m_fUpdateEPSSurprise = fFlag; }

protected:
	long m_lCurrentUpdateDayLinePos; // 由于更新一次日线数据超过24小时，故而将此计数器声明为类变量，且无需每日重置。
	long m_lCurrentUpdateEPSSurprisePos; // 此变量无需每日更新

	bool m_fUpdateCountryList;
	bool m_fUpdateSymbol; // 每日更新公司代码库
	bool m_fUpdateStockProfile; // 每日更新公司简介
	bool m_fUpdateCompanyNews; // 每日更新公司新闻
	bool m_fUpdateCompanyPriceMetrics;
	bool m_fUpdateStockBasicFinancial; // 每日更新公司日线
	bool m_fUpdateStockDayLine; // 每日更新公司日线数据
	bool m_fUpdateForexExchange; // 每日更新Forex交易所
	bool m_fUpdateForexSymbol; // 每日更新Forex交易所代码
	bool m_fUpdateForexDayLine; // 每日更新Forex日线数据
	bool m_fUpdateCryptoExchange; // 每日更新Crypto交易所
	bool m_fUpdateCryptoSymbol; // 每日更新Crypto交易所代码
	bool m_fUpdateCryptoDayLine; // 每日更新Crypto日线数据
	bool m_fUpdatePeer; // 每90天更新Peers数据
	bool m_fUpdateInsiderTransaction; // 每30天更新InsiderTransaction数据
	bool m_fUpdateInsiderSentiment; // 每30天更新InsiderSentiment数据
	bool m_fUpdateEconomicCalendar; // 每日更新经济日历数据
	bool m_fUpdateEPSSurprise;

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
