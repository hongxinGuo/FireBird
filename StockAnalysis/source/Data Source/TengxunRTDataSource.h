#pragma once

#include"VirtualDataSource.h"

class CTengxunRTDataSource final : public CVirtualDataSource {
public:
	CTengxunRTDataSource();
	~CTengxunRTDataSource() override = default;

	bool Reset(void) override;
	bool UpdateStatus(void) override;

	bool Inquire(const long lCurrentTime) override;

	bool InquireRTData(const long lCurrentTime);

protected:
};

using CTengxunRTDataSourcePtr = shared_ptr<CTengxunRTDataSource>;
extern CTengxunRTDataSourcePtr gl_pTengxunRTDataSource;
