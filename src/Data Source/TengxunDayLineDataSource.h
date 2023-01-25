#pragma once

#include"VirtualDataSource.h"

class CTengxunDayLineDataSource final : public CVirtualDataSource {
public:
	CTengxunDayLineDataSource();
	~CTengxunDayLineDataSource() override = default;
	bool Reset(void) override;
	bool UpdateStatus(void) override;

	bool Inquire(const long lCurrentTime) override;

protected:
};

using CTengxunDayLineDataSourcePtr = shared_ptr<CTengxunDayLineDataSource>;
extern CTengxunDayLineDataSourcePtr gl_pTengxunDayLineDataSource;
