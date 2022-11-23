#pragma once

#include"VirtualDataSource.h"

class CNeteaseRTDataSource : public CVirtualDataSource {
public:
	CNeteaseRTDataSource();
	~CNeteaseRTDataSource();

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;
	virtual void ParseAndStoreData(CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData) override final;// 网易实时数据在允许查询下次数据之后才处理本次数据，故而需要重新定义

	virtual bool Inquire(long lCurrentTime) override final;

	bool InquireRTData(long lCurrentTime);

protected:
};

typedef shared_ptr<CNeteaseRTDataSource> CNeteaseRTDataSourcePtr;
extern CNeteaseRTDataSourcePtr gl_pNeteaseRTDataSource;