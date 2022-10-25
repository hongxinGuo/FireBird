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
	long m_lCurrentUpdateDayLinePos; // ���ڸ���һ���������ݳ���24Сʱ���ʶ����˼���������Ϊ�������������ÿ�����á�
	long m_lCurrentUpdateEPSSurprisePos; // �˱�������ÿ�ո���

	bool m_fCountryListUpdated;
	bool m_fSymbolUpdated; // ÿ�ո��¹�˾�����
	bool m_fStockProfileUpdated; // ÿ�ո��¹�˾���
	bool m_fCompanyNewsUpdated; // ÿ�ո��¹�˾����
	bool m_fCompanyPriceMetricsUpdated;
	bool m_fStockBasicFinancialUpdated; // ÿ�ո��¹�˾����
	bool m_fDayLineUpdated; // ÿ�ո��¹�˾��������
	bool m_fForexExchangeUpdated; // ÿ�ո���Forex������
	bool m_fForexSymbolUpdated; // ÿ�ո���Forex����������
	bool m_fForexDayLineUpdated; // ÿ�ո���Forex��������
	bool m_fCryptoExchangeUpdated; // ÿ�ո���Crypto������
	bool m_fCryptoSymbolUpdated; // ÿ�ո���Crypto����������
	bool m_fCryptoDayLineUpdated; // ÿ�ո���Crypto��������
	bool m_fPeerUpdated; // ÿ90�����Peers����
	bool m_fInsiderTransactionUpdated; // ÿ30�����InsiderTransaction����
	bool m_fInsiderSentimentUpdated; // ÿ30�����InsiderSentiment����
	bool m_fEconomicCalendarUpdated; // ÿ�ո��¾�����������
	bool m_fEPSSurpriseUpdated;

	CFinnhubFactory m_FinnhubFactory;
};

typedef shared_ptr<CFinnhubDataSource> CFinnhubDataSourcePtr;
extern CFinnhubDataSourcePtr gl_pDataSourceFinnhub;
