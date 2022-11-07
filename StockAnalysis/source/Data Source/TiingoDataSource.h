#pragma once

#include"VirtualDataSource.h"
#include"TiingoFactory.h"

class CTiingoDataSource : public CVirtualDataSource
{
public:
	CTiingoDataSource(void);
	~CTiingoDataSource(void);

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;

	virtual bool Inquire(long lCurrentTime) override final;

	bool InquireTiingo(void);
	bool InquireCompanySymbol(void);
	bool InquireCryptoSymbol(void);
	bool InquireDayLine(void);

	bool IsStockSymbolUpdated(void) noexcept { return m_fStockSymbolUpdated; }
	void SetStockSymbolUpdated(bool fFlag) noexcept { m_fStockSymbolUpdated = fFlag; }
	bool IsCryptoSymbolUpdated(void) noexcept { return m_fCryptoSymbolUpdated; }
	void SetCryptoSymbolUpdated(bool fFlag) noexcept { m_fCryptoSymbolUpdated = fFlag; }
	bool IsDayLineUpdated(void) noexcept { return m_fDayLineUpdated; }
	void SetDayLineUpdated(bool fFlag) noexcept { m_fDayLineUpdated = fFlag; }

protected:
	CTiingoFactory m_TiingoFactory;

	bool m_fStockSymbolUpdated; // 每日更新公司代码库
	bool m_fCryptoSymbolUpdated; // 每日更新crypto代码库
	bool m_fDayLineUpdated; // 每日更新公司日线数据
};

typedef shared_ptr<CTiingoDataSource> CTiingoDataSourcePtr;

extern CTiingoDataSourcePtr gl_pDataSourceTiingo;
