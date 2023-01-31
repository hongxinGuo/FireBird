#pragma once

#include"VirtualDataSource.h"
#include"TiingoFactory.h"

class CTiingoDataSource final : public CVirtualDataSource {
public:
	CTiingoDataSource(void);
	~CTiingoDataSource(void) override = default;

	bool Reset(void) override;
	bool UpdateStatus(void) override;

	bool Inquire(const long lCurrentTime) override;

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

	bool PrepareNextInquiringString(void) override;
	void CreateTotalInquiringString(CString strMiddle) override;
	CString GetNextInquiringMiddleString(long, bool) final { return _T(""); }
	bool ReportStatus(long lNumberOfData) const override;
	void ConfigureSession(void) final; // 设置m_pSession状态。
	bool ParseData(CWebDataPtr pWebData) override; // 数据为JSon格式, 需要解析

protected:
	CTiingoFactory m_TiingoFactory;

	bool m_fStockSymbolUpdated; // 每日更新公司代码库
	bool m_fCryptoSymbolUpdated; // 每日更新crypto代码库
	bool m_fDayLineUpdated; // 每日更新公司日线数据
};

using CTiingoDataSourcePtr = shared_ptr<CTiingoDataSource>;

extern CTiingoDataSourcePtr gl_pTiingoDataSource;
