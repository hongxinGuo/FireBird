#pragma once

#include"VirtualDataSource.h"

class CTengxunDayLineDataSource final : public CVirtualDataSource {
public:
	CTengxunDayLineDataSource();
	~CTengxunDayLineDataSource() override = default;
	bool Reset(void) override;
	bool UpdateStatus(void) override;

	bool Inquire(const long lCurrentTime) override;

	bool InquireDayLine(void);
	vector<CVirtualWebProductPtr> CreateWebProduct(CChinaStockPtr pStock);

	bool IsUpdateDayLine(void) const noexcept { return m_fUpdateDayLine; }
	void SetUpdateDayLine(bool fFlag) noexcept { m_fUpdateDayLine = fFlag; }

protected:
	bool m_fUpdateDayLine; // 每日更新公司日线数据
};

using CTengxunDayLineDataSourcePtr = shared_ptr<CTengxunDayLineDataSource>;
extern CTengxunDayLineDataSourcePtr gl_pTengxunDayLineDataSource;
