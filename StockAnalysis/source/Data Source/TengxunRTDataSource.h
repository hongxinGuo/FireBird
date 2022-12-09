#pragma once

#include"VirtualDataSource.h"

class CTengxunRTDataSource final : public CVirtualDataSource {
public:
	CTengxunRTDataSource();
	~CTengxunRTDataSource() override = default;

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;

	virtual bool Inquire(long lCurrentTime) override final;

	bool InquireRTData(long lCurrentTime);

protected:
};

typedef shared_ptr<CTengxunRTDataSource> CTengxunRTDataSourcePtr;
extern CTengxunRTDataSourcePtr gl_pTengxunRTDataSource;
