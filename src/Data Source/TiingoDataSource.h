#pragma once

#include"VirtualDataSource.h"
#include"TiingoFactory.h"

class CTiingoDataSource : public CVirtualDataSource {
public:
	CTiingoDataSource();
	// ֻ����һ��ʵ��,������ֵ������
	CTiingoDataSource(const CTiingoDataSource&) = delete;
	CTiingoDataSource& operator=(const CTiingoDataSource&) = delete;
	CTiingoDataSource(const CTiingoDataSource&&) noexcept = delete;
	CTiingoDataSource& operator=(const CTiingoDataSource&&) noexcept = delete;
	~CTiingoDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(long lCurrentTime) override;

	void ConfigureInternetOption() override; // ����internet������
	enum_ErrorMessageData IsAErrorMessageData(const CWebDataPtr& pWebData) override;

	void Inquire(long lCurrentTime);
	virtual bool InquireCompanyNews();
	virtual bool InquireCompanySymbol();
	virtual bool InquireCryptoSymbol();
	virtual bool InquireDayLine();

	bool IsUpdateMarketNews() const noexcept { return m_fUpdateMarketNews; }
	void SetUpdateMarketNews(bool fFlag) noexcept { m_fUpdateMarketNews = fFlag; }
	bool IsUpdateStockSymbol() const noexcept { return m_fUpdateStockSymbol; }
	void SetUpdateStockSymbol(bool fFlag) noexcept { m_fUpdateStockSymbol = fFlag; }
	bool IsUpdateCryptoSymbol() const noexcept { return m_fUpdateCryptoSymbol; }
	void SetUpdateCryptoSymbol(bool fFlag) noexcept { m_fUpdateCryptoSymbol = fFlag; }
	bool IsUpdateDayLine() const noexcept { return m_fUpdateDayLine; }
	void SetUpdateDayLine(bool fFlag) noexcept { m_fUpdateDayLine = fFlag; }

protected:
	CTiingoFactory m_TiingoFactory;

	bool m_fUpdateMarketNews{ true }; // ÿ�ո����г�����
	bool m_fUpdateStockSymbol; // ÿ�ո��¹�˾�����
	bool m_fUpdateCryptoSymbol; // ÿ�ո���crypto�����
	bool m_fUpdateDayLine; // ÿ�ո��¹�˾��������

	bool m_fTiingoDataInquiryFinished{ false };
};

using CTiingoDataSourcePtr = shared_ptr<CTiingoDataSource>;

extern CTiingoDataSourcePtr gl_pTiingoDataSource;
