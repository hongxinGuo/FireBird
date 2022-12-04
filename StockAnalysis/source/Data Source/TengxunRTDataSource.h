#pragma once

#include"VirtualDataSource.h"

class CTengxunRTDataSource : public CVirtualDataSource {
public:
	CTengxunRTDataSource();
	~CTengxunRTDataSource();

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;
	virtual void ParseAndStoreData(CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData) override final;

	virtual bool Inquire(long lCurrentTime) override final;

	bool InquireRTData(long lCurrentTime);

protected:
};

typedef shared_ptr<CTengxunRTDataSource> CTengxunRTDataSourcePtr;
extern CTengxunRTDataSourcePtr gl_pTengxunRTDataSource;