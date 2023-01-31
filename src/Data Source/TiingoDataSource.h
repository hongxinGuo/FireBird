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
	void ConfigureSession(void) final; // ����m_pSession״̬��
	bool ParseData(CWebDataPtr pWebData) override; // ����ΪJSon��ʽ, ��Ҫ����

protected:
	CTiingoFactory m_TiingoFactory;

	bool m_fStockSymbolUpdated; // ÿ�ո��¹�˾�����
	bool m_fCryptoSymbolUpdated; // ÿ�ո���crypto�����
	bool m_fDayLineUpdated; // ÿ�ո��¹�˾��������
};

using CTiingoDataSourcePtr = shared_ptr<CTiingoDataSource>;

extern CTiingoDataSourcePtr gl_pTiingoDataSource;
