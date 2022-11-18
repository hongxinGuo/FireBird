#pragma once

#include"VirtualDataSource.h"

class CNeteaseDaylineDataSource : public CVirtualDataSource {
public:
	CNeteaseDaylineDataSource();
	~CNeteaseDaylineDataSource();

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;

	virtual bool Inquire(long lCurrentTime) override final;

	bool InquireRTData(long lCurrentTime);

protected:
};

typedef shared_ptr<CNeteaseDaylineDataSource> CNeteaseDaylineDataSourcePtr;
extern CNeteaseDaylineDataSourcePtr gl_pNeteaseDaylineDataSource;