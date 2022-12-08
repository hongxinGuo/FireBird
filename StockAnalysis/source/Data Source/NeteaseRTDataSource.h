#pragma once

#include"VirtualDataSource.h"

class CNeteaseRTDataSource : public CVirtualDataSource {
public:
	CNeteaseRTDataSource();
	virtual ~CNeteaseRTDataSource();

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;

	virtual bool Inquire(long lCurrentTime) override final;

	bool InquireRTData(long lCurrentTime);

protected:
};

typedef shared_ptr<CNeteaseRTDataSource> CNeteaseRTDataSourcePtr;
extern CNeteaseRTDataSourcePtr gl_pNeteaseRTDataSource;