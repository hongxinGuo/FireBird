#pragma once

#include"VirtualDataSource.h"

class CTengxunRTDataSource final : public CVirtualDataSource {
public:
	CTengxunRTDataSource();
	~CTengxunRTDataSource() override = default;

	bool Reset(void) final;
	bool UpdateStatus(void) final;

	bool Inquire(const long lCurrentTime) final;

	bool InquireRTData(const long lCurrentTime);

protected:
};

using CTengxunRTDataSourcePtr = shared_ptr<CTengxunRTDataSource>;
extern CTengxunRTDataSourcePtr gl_pTengxunRTDataSource;
