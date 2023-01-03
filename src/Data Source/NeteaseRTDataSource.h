#pragma once

#include"VirtualDataSource.h"

class CNeteaseRTDataSource final : public CVirtualDataSource {
public:
	CNeteaseRTDataSource();
	~CNeteaseRTDataSource() override = default;

	bool Reset(void) override;
	bool UpdateStatus(void) override;

	bool Inquire(const long lCurrentTime) override;

	bool InquireRTData(const long lCurrentTime);

protected:
};

using CNeteaseRTDataSourcePtr = shared_ptr<CNeteaseRTDataSource>;
extern CNeteaseRTDataSourcePtr gl_pNeteaseRTDataSource;
