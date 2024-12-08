#pragma once

#include"VirtualDataSource.h"
import FireBird.Factory.Finnhub;

class CFinnhubDataSource : public CVirtualDataSource {
public:
	CFinnhubDataSource();
	// ֻ����һ��ʵ��,������ֵ������
	CFinnhubDataSource(const CFinnhubDataSource&) = delete;
	CFinnhubDataSource& operator=(const CFinnhubDataSource&) = delete;
	CFinnhubDataSource(const CFinnhubDataSource&&) noexcept = delete;
	CFinnhubDataSource& operator=(const CFinnhubDataSource&&) noexcept = delete;
	~CFinnhubDataSource() override = default;

	bool Reset() override;
	bool GenerateInquiryMessage(long lCurrentTime) override;

	void ConfigureInternetOption() override;
	enum_ErrorMessageData IsAErrorMessageData(const CWebDataPtr& pWebData) override;

	// �ɹ��������Ŀ��Ŀǰ��21�
	virtual bool GenerateCountryList();
	virtual bool GenerateCompanySymbol();
	virtual bool GenerateMarketStatus();
	virtual bool GenerateMarketHoliday();
	virtual bool GenerateCompanyProfileConcise();
	virtual bool GenerateCompanyNews();
	virtual bool GenerateCompanyBasicFinancial();
	virtual bool GenerateStockDayLine();
	virtual bool GenerateRTQuote();
	virtual bool GeneratePeer();
	virtual bool GenerateInsiderTransaction();
	virtual bool GenerateInsiderSentiment();
	virtual bool GenerateEconomicCalendar();
	virtual bool GenerateEPSSurprise();
	virtual bool GenerateSECFilings();
	virtual bool GenerateForexExchange();
	virtual bool GenerateForexSymbol();
	virtual bool GenerateForexDayLine();
	virtual bool GenerateCryptoExchange();
	virtual bool GenerateCryptoSymbol();
	virtual bool GenerateCryptoDayLine();

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
	bool IsUpdateSECFilings() const noexcept { return m_fUpdateSECFilings; }
	void SetUpdateSECFilings(const bool fFlag) noexcept { m_fUpdateSECFilings = fFlag; }

protected:
	// ÿ�ո�����Ŀ��Ŀǰ��21�
	bool m_fUpdateSymbol{ true }; // ÿ�ո��¹�˾�����
	bool m_fUpdateMarketStatus{ true }; // ÿ�ղ�ѯ�г�״̬
	bool m_fUpdateMarketHoliday{ true }; // ÿ�ղ�ѯ�г�����
	bool m_fUpdateForexExchange{ true }; // ÿ�ո���Forex������
	bool m_fUpdateForexSymbol{ true }; // ÿ�ո���Forex����������
	bool m_fUpdateCryptoExchange{ true }; // ÿ�ո���Crypto������
	bool m_fUpdateCryptoSymbol{ true }; // ÿ�ո���Crypto����������
	bool m_fUpdateCountryList{ true };
	bool m_fUpdateStockProfile{ true }; // ÿ�ո��¹�˾���
	bool m_fUpdateCompanyNews{ true }; // ÿ�ո��¹�˾����
	bool m_fUpdateCompanyPriceMetrics{ true };
	bool m_fUpdateStockBasicFinancial{ true }; // ÿ�ո��¹�˾����
	bool m_fUpdateStockDayLine{ true }; // ÿ�ո��¹�˾��������
	bool m_fUpdateForexDayLine{ true }; // ÿ�ո���Forex��������
	bool m_fUpdateCryptoDayLine{ true }; // ÿ�ո���Crypto��������
	bool m_fUpdatePeer{ true }; // ÿ90�����Peers����
	bool m_fUpdateInsiderTransaction{ true }; // ÿ30�����InsiderTransaction����
	bool m_fUpdateInsiderSentiment{ true }; // ÿ30�����InsiderSentiment����
	bool m_fUpdateEconomicCalendar{ true }; // ÿ�ո��¾�����������
	bool m_fUpdateEPSSurprise{ true };
	bool m_fUpdateSECFilings{ true };

	CFinnhubFactory m_FinnhubFactory;

private:
	bool m_fFinnhubDataInquiryFinished{ false };
	size_t m_lCurrentUpdateDayLinePos{ 0 }; // ���ڸ���һ���������ݳ���24Сʱ���ʶ����˼���������Ϊ�������������ÿ�����á�
	size_t m_lCurrentUpdateEPSSurprisePos{ 0 }; // �˱�������ÿ�ո���
	size_t m_lCurrentUpdateSECFilingsPos{ 0 };

	size_t m_lCurrentRTDataQuotePos{ 0 };
	size_t m_lCurrentForexExchangePos{ 0 };
	size_t m_lCurrentCryptoExchangePos{ 0 };
};

using CFinnhubDataSourcePtr = shared_ptr<CFinnhubDataSource>;
extern CFinnhubDataSourcePtr gl_pFinnhubDataSource;
