#pragma once

#include"VirtualDataSource.h"
#include"NeteaseDayLineWebData.h"

class CNeteaseDayLineDataSource final : public CVirtualDataSource {
public:
	CNeteaseDayLineDataSource();
	~CNeteaseDayLineDataSource() override = default;

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;

	virtual bool Inquire(long lCurrentTime) override final;

protected:
};

typedef shared_ptr<CNeteaseDayLineDataSource> CNeteaseDayLineDataSourcePtr;
extern CNeteaseDayLineDataSourcePtr gl_pNeteaseDayLineDataSource;
