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
	bool ParseData(CWebDataPtr pWebData) override; // ����ΪJSon��ʽ, ��Ҫ����

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
	long m_lCurrentUpdateDayLinePos; // ���ڸ���һ���������ݳ���24Сʱ���ʶ����˼���������Ϊ�������������ÿ�����á�
	long m_lCurrentUpdateEPSSurprisePos; // �˱�������ÿ�ո���

	bool m_fUpdateCountryList;
	bool m_fUpdateSymbol; // ÿ�ո��¹�˾�����
	bool m_fUpdateStockProfile; // ÿ�ո��¹�˾���
	bool m_fUpdateCompanyNews; // ÿ�ո��¹�˾����
	bool m_fUpdateCompanyPriceMetrics;
	bool m_fUpdateStockBasicFinancial; // ÿ�ո��¹�˾����
	bool m_fUpdateStockDayLine; // ÿ�ո��¹�˾��������
	bool m_fUpdateForexExchange; // ÿ�ո���Forex������
	bool m_fUpdateForexSymbol; // ÿ�ո���Forex����������
	bool m_fUpdateForexDayLine; // ÿ�ո���Forex��������
	bool m_fUpdateCryptoExchange; // ÿ�ո���Crypto������
	bool m_fUpdateCryptoSymbol; // ÿ�ո���Crypto����������
	bool m_fUpdateCryptoDayLine; // ÿ�ո���Crypto��������
	bool m_fUpdatePeer; // ÿ90�����Peers����
	bool m_fUpdateInsiderTransaction; // ÿ30�����InsiderTransaction����
	bool m_fUpdateInsiderSentiment; // ÿ30�����InsiderSentiment����
	bool m_fUpdateEconomicCalendar; // ÿ�ո��¾�����������
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
