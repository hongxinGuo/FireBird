#pragma once

#include"VirtualDataSource.h"
#include"NeteaseDayLineWebData.h"

class CNeteaseDayLineDataSource final : public CVirtualDataSource {
public:
	CNeteaseDayLineDataSource();
	~CNeteaseDayLineDataSource() override = default;

	bool Reset(void) final;
	bool UpdateStatus(void) final;

	bool Inquire(const long lCurrentTime) override;

protected:
};

using CNeteaseDayLineDataSourcePtr = shared_ptr<CNeteaseDayLineDataSource>;
extern CNeteaseDayLineDataSourcePtr gl_pNeteaseDayLineDataSource;
