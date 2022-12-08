#pragma once

#include"VirtualDataSource.h"
#include"NeteaseDayLineWebData.h"

class CNeteaseDayLineDataSource : public CVirtualDataSource {
public:
	CNeteaseDayLineDataSource();
	virtual ~CNeteaseDayLineDataSource();

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;

	virtual bool Inquire(long lCurrentTime) override final;

protected:
};

typedef shared_ptr<CNeteaseDayLineDataSource> CNeteaseDayLineDataSourcePtr;
extern CNeteaseDayLineDataSourcePtr gl_pNeteaseDayLineDataSource;