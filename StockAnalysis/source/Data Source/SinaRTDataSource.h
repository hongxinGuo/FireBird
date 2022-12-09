#pragma once

#include"VirtualDataSource.h"

class CSinaRTDataSource final : public CVirtualDataSource {
public:
	CSinaRTDataSource();
	~CSinaRTDataSource() override = default;

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;

	virtual bool Inquire(long lCurrentTime) override final;

	bool InquireRTData(long lCurrentTime);

protected:
};

typedef shared_ptr<CSinaRTDataSource> CSinaRTDataSourcePtr;
extern CSinaRTDataSourcePtr gl_pSinaRTDataSource;
