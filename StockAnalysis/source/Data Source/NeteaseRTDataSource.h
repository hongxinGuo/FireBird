#pragma once

#include"VirtualDataSource.h"

class CNeteaseRTDataSource final : public CVirtualDataSource {
public:
	CNeteaseRTDataSource();
	~CNeteaseRTDataSource() override = default;

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;

	virtual bool Inquire(long lCurrentTime) override final;

	bool InquireRTData(long lCurrentTime);

protected:
};

typedef shared_ptr<CNeteaseRTDataSource> CNeteaseRTDataSourcePtr;
extern CNeteaseRTDataSourcePtr gl_pNeteaseRTDataSource;
