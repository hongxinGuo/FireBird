#pragma once

#include"VirtualDataSource.h"
#include"NeteaseDaylineWebData.h"

class CNeteaseDaylineDataSource : public CVirtualDataSource {
public:
	CNeteaseDaylineDataSource();
	virtual ~CNeteaseDaylineDataSource();

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;

	virtual bool Inquire(long lCurrentTime) override final;

protected:
};

typedef shared_ptr<CNeteaseDaylineDataSource> CNeteaseDaylineDataSourcePtr;
extern CNeteaseDaylineDataSourcePtr gl_pNeteaseDaylineDataSource;