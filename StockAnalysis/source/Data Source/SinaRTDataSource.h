#pragma once

#include"VirtualDataSource.h"

class CSinaRTDataSource final : public CVirtualDataSource {
public:
	CSinaRTDataSource();
	~CSinaRTDataSource() override = default;

	bool Reset(void) final;
	bool UpdateStatus(void) final;

	bool Inquire(const long lCurrentTime) override;

	bool InquireRTData(const long lCurrentTime);

protected:
};

using CSinaRTDataSourcePtr = shared_ptr<CSinaRTDataSource>;
extern CSinaRTDataSourcePtr gl_pSinaRTDataSource;
