#pragma once

#include"VirtualDataSource.h"
#include"TiingoFactory.h"

class CTiingoDataSource final : public CVirtualDataSource {
public:
	CTiingoDataSource(void);
	~CTiingoDataSource(void) override = default;;

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;

	virtual bool Inquire(long lCurrentTime) override final;

	bool InquireTiingo(void);
	bool InquireCompanySymbol(void);
	bool InquireCryptoSymbol(void);
	bool InquireDayLine(void);

	bool IsStockSymbolUpdated(void) const noexcept { return m_fStockSymbolUpdated; }
	void SetStockSymbolUpdated(bool fFlag) noexcept { m_fStockSymbolUpdated = fFlag; }
	bool IsCryptoSymbolUpdated(void) const noexcept { return m_fCryptoSymbolUpdated; }
	void SetCryptoSymbolUpdated(bool fFlag) noexcept { m_fCryptoSymbolUpdated = fFlag; }
	bool IsDayLineUpdated(void) const noexcept { return m_fDayLineUpdated; }
	void SetDayLineUpdated(bool fFlag) noexcept { m_fDayLineUpdated = fFlag; }

protected:
	CTiingoFactory m_TiingoFactory;

	bool m_fStockSymbolUpdated; // ÿ�ո��¹�˾�����
	bool m_fCryptoSymbolUpdated; // ÿ�ո���crypto�����
	bool m_fDayLineUpdated; // ÿ�ո��¹�˾��������
};

typedef shared_ptr<CTiingoDataSource> CTiingoDataSourcePtr;

extern CTiingoDataSourcePtr gl_pTiingoDataSource;
