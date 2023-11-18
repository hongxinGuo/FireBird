#pragma once

#include"VirtualDataSource.h"
#include"FinnhubFactory.h"

#include"ProductFinnhubEconomicCountryList.h"
#include"ProductFinnhubEconomicCalendar.h"
#include"ProductFinnhubForexExchange.h"
#include"ProductFinnhubCryptoExchange.h"

class CFinnhubDataSource : public CVirtualDataSource {
	friend CProductFinnhubEconomicCountryList;
	friend CProductFinnhubEconomicCalendar;
	friend CProductFinnhubCryptoExchange;
	friend CProductFinnhubForexExchange;

public:
	CFinnhubDataSource();
	// 只能有一个实例,不允许赋值、拷贝
	CFinnhubDataSource(const CFinnhubDataSource&) = delete;
	CFinnhubDataSource& operator=(const CFinnhubDataSource&) = delete;
	CFinnhubDataSource(const CFinnhubDataSource&&) noexcept = delete;
	CFinnhubDataSource& operator=(const CFinnhubDataSource&&) noexcept = delete;
	~CFinnhubDataSource() override = default;

	bool Reset() override;
	bool GenerateInquiryMessage(const long lCurrentTime) override;

	void ConfigureSession() override;
	bool ParseData(CWebDataPtr pWebData) override; // 数据为JSon格式, 需要解析

	void InquireFinnhub(long lCurrentTime);

	virtual bool InquireCountryList();
	virtual bool InquireCompanySymbol();
	virtual bool InquireMarketStatus();
	virtual bool InquireMarketHoliday();
	virtual bool InquireCompanyProfileConcise();
	virtual bool InquireCompanyNews();
	virtual bool InquireCompanyBasicFinancial();
	virtual bool InquireStockDayLine();
	virtual bool InquireRTQuote();
	virtual bool InquirePeer();
	virtual bool InquireInsiderTransaction();
	virtual bool InquireInsiderSentiment();
	virtual bool InquireEconomicCalendar();
	virtual bool InquireEPSSurprise();
	virtual bool InquireForexExchange();
	virtual bool InquireForexSymbol();
	virtual bool InquireForexDayLine();
	virtual bool InquireCryptoExchange();
	virtual bool InquireCryptoSymbol();
	virtual bool InquireCryptoDayLine();

	bool IsUpdateCountryList() const noexcept { return m_fUpdateCountryList; }
	void SetUpdateCountryList(const bool fFlag) noexcept { m_fUpdateCountryList = fFlag; }
	bool IsUpdateSymbol() const noexcept { return m_fUpdateSymbol; }
	void SetUpdateSymbol(const bool fFlag) noexcept { m_fUpdateSymbol = fFlag; }
	bool IsUpdateMarketStatus() const noexcept { return m_fUpdateMarketStatus; }
	void SetUpdateMarketStatus(const bool fFlag) noexcept { m_fUpdateMarketStatus = fFlag; }
	bool IsUpdateMarketHoliday() const noexcept { return m_fUpdateMarketHoliday; }
	void SetUpdateMarketHoliday(const bool fFlag) noexcept { m_fUpdateMarketHoliday = fFlag; }
	bool IsUpdateStockProfile() const noexcept { return m_fUpdateStockProfile; }
	void SetUpdateStockProfile(const bool fFlag) noexcept { m_fUpdateStockProfile = fFlag; }
	bool IsUpdateCompanyNews() const noexcept { return m_fUpdateCompanyNews; }
	void SetUpdateCompanyNews(const bool fFlag) noexcept { m_fUpdateCompanyNews = fFlag; }
	bool IsUpdateCompanyPriceMetrics() const noexcept { return m_fUpdateCompanyPriceMetrics; }
	void SetUpdateCompanyPriceMetrics(const bool fFlag) noexcept { m_fUpdateCompanyPriceMetrics = fFlag; }
	bool IsUpdateStockBasicFinancial() const noexcept { return m_fUpdateStockBasicFinancial; }
	void SetUpdateStockBasicFinancial(const bool fFlag) noexcept { m_fUpdateStockBasicFinancial = fFlag; }
	bool IsUpdateStockDayLine() const noexcept { return m_fUpdateStockDayLine; }
	void SetUpdateStockDayLine(const bool fFlag) noexcept { m_fUpdateStockDayLine = fFlag; }
	bool IsUpdateForexExchange() const noexcept { return m_fUpdateForexExchange; }
	void SetUpdateForexExchange(const bool fFlag) noexcept { m_fUpdateForexExchange = fFlag; }
	bool IsUpdateForexSymbol() const noexcept { return m_fUpdateForexSymbol; }
	void SetUpdateForexSymbol(const bool fFlag) noexcept { m_fUpdateForexSymbol = fFlag; }
	bool IsUpdateForexDayLine() const noexcept { return m_fUpdateForexDayLine; }
	void SetUpdateForexDayLine(const bool fFlag) noexcept { m_fUpdateForexDayLine = fFlag; }
	bool IsUpdateCryptoExchange() const noexcept { return m_fUpdateCryptoExchange; }
	void SetUpdateCryptoExchange(const bool fFlag) noexcept { m_fUpdateCryptoExchange = fFlag; }
	bool IsUpdateCryptoSymbol() const noexcept { return m_fUpdateCryptoSymbol; }
	void SetUpdateCryptoSymbol(const bool fFlag) noexcept { m_fUpdateCryptoSymbol = fFlag; }
	bool IsUpdateCryptoDayLine() const noexcept { return m_fUpdateCryptoDayLine; }
	void SetUpdateCryptoDayLine(const bool fFlag) noexcept { m_fUpdateCryptoDayLine = fFlag; }
	bool IsUpdatePeer() const noexcept { return m_fUpdatePeer; }
	void SetUpdatePeer(const bool fFlag) noexcept { m_fUpdatePeer = fFlag; }
	bool IsUpdateInsiderTransaction() const noexcept { return m_fUpdateInsiderTransaction; }
	void SetUpdateInsiderTransaction(const bool fFlag) noexcept { m_fUpdateInsiderTransaction = fFlag; }
	bool IsUpdateInsiderSentiment() const noexcept { return m_fUpdateInsiderSentiment; }
	void SetUpdateInsiderSentiment(const bool fFlag) noexcept { m_fUpdateInsiderSentiment = fFlag; }
	bool IsUpdateEconomicCalendar() const noexcept { return m_fUpdateEconomicCalendar; }
	void SetUpdateEconomicCalendar(const bool fFlag) noexcept { m_fUpdateEconomicCalendar = fFlag; }
	bool IsUpdateEPSSurprise() const noexcept { return m_fUpdateEPSSurprise; }
	void SetUpdateEPSSurprise(const bool fFlag) noexcept { m_fUpdateEPSSurprise = fFlag; }

protected:
	long m_lCurrentUpdateDayLinePos; // 由于更新一次日线数据超过24小时，故而将此计数器声明为类变量，且无需每日重置。
	long m_lCurrentUpdateEPSSurprisePos; // 此变量无需每日更新

	bool m_fUpdateSymbol; // 每日更新公司代码库
	bool m_fUpdateMarketStatus; // 每日查询市场状态
	bool m_fUpdateMarketHoliday; // 每日查询市场假日
	bool m_fUpdateForexExchange; // 每日更新Forex交易所
	bool m_fUpdateForexSymbol; // 每日更新Forex交易所代码
	bool m_fUpdateCryptoExchange; // 每日更新Crypto交易所
	bool m_fUpdateCryptoSymbol; // 每日更新Crypto交易所代码
	bool m_fUpdateCountryList;
	bool m_fUpdateStockProfile; // 每日更新公司简介
	bool m_fUpdateCompanyNews; // 每日更新公司新闻
	bool m_fUpdateCompanyPriceMetrics;
	bool m_fUpdateStockBasicFinancial; // 每日更新公司日线
	bool m_fUpdateStockDayLine; // 每日更新公司日线数据
	bool m_fUpdateForexDayLine; // 每日更新Forex日线数据
	bool m_fUpdateCryptoDayLine; // 每日更新Crypto日线数据
	bool m_fUpdatePeer; // 每90天更新Peers数据
	bool m_fUpdateInsiderTransaction; // 每30天更新InsiderTransaction数据
	bool m_fUpdateInsiderSentiment; // 每30天更新InsiderSentiment数据
	bool m_fUpdateEconomicCalendar; // 每日更新经济日历数据
	bool m_fUpdateEPSSurprise;

	CFinnhubFactory m_FinnhubFactory;

private:
	bool m_fInquiringFinnhubStockSymbol = {false};
	bool m_fInquiringFinnhubMarketStatus = {false};
	bool m_fInquiringFinnhubMarketHoliday = {false};
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

	long m_lCurrentRTDataQuotePos;
	long m_lCurrentForexExchangePos;
	long m_lCurrentCryptoExchangePos;
};

using CFinnhubDataSourcePtr = shared_ptr<CFinnhubDataSource>;
extern CFinnhubDataSourcePtr gl_pFinnhubDataSource;
