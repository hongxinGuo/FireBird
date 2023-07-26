#pragma once

#include"VirtualDataSource.h"
#include"TiingoFactory.h"

class CTiingoDataSource : public CVirtualDataSource {
public:
	CTiingoDataSource();
	// 只能有一个实例,不允许赋值、拷贝
	CTiingoDataSource(const CTiingoDataSource&) = delete;
	CTiingoDataSource& operator=(const CTiingoDataSource&) = delete;
	CTiingoDataSource(const CTiingoDataSource&&) noexcept = delete;
	CTiingoDataSource& operator=(const CTiingoDataSource&&) noexcept = delete;
	~CTiingoDataSource() override = default;

	bool Reset() override;
	void UpdateStatus() override;

	bool GenerateInquiryMessage(const long lCurrentTime) override;

	void ConfigureSession() override; // 设置m_pSession状态。
	bool ParseData(CWebDataPtr pWebData) override; // 数据为JSon格式, 需要解析

	bool InquireTiingo();
	virtual bool InquireCompanySymbol();
	virtual bool InquireCryptoSymbol();
	virtual bool InquireDayLine();

	bool IsUpdateStockSymbol() const noexcept { return m_fUpdateStockSymbol; }
	void SetUpdateStockSymbol(bool fFlag) noexcept { m_fUpdateStockSymbol = fFlag; }
	bool IsUpdateCryptoSymbol() const noexcept { return m_fUpdateCryptoSymbol; }
	void SetUpdateCryptoSymbol(bool fFlag) noexcept { m_fUpdateCryptoSymbol = fFlag; }
	bool IsUpdateDayLine() const noexcept { return m_fUpdateDayLine; }
	void SetUpdateDayLine(bool fFlag) noexcept { m_fUpdateDayLine = fFlag; }

protected:
	CTiingoFactory m_TiingoFactory;

	bool m_fUpdateStockSymbol; // 每日更新公司代码库
	bool m_fUpdateCryptoSymbol; // 每日更新crypto代码库
	bool m_fUpdateDayLine; // 每日更新公司日线数据
};

using CTiingoDataSourcePtr = shared_ptr<CTiingoDataSource>;

extern CTiingoDataSourcePtr gl_pTiingoDataSource;
