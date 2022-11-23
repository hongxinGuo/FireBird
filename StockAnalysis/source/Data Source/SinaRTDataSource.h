#pragma once

#include"VirtualDataSource.h"

class CSinaRTDataSource : public CVirtualDataSource {
public:
	CSinaRTDataSource();
	~CSinaRTDataSource();

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;
	virtual void ParseAndStoreData(CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData) override final; // 新浪实时数据在允许查询下次数据之后才处理本次数据，故而需要重新定义

	virtual bool Inquire(long lCurrentTime) override final;

	bool InquireRTData(long lCurrentTime);

protected:
};

typedef shared_ptr<CSinaRTDataSource> CSinaRTDataSourcePtr;
extern CSinaRTDataSourcePtr gl_pSinaRTDataSource;