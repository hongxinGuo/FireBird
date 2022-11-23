#pragma once

#include"VirtualDataSource.h"

class CSinaRTDataSource : public CVirtualDataSource {
public:
	CSinaRTDataSource();
	~CSinaRTDataSource();

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;
	virtual void ParseAndStoreData(CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData) override final; // ����ʵʱ�����������ѯ�´�����֮��Ŵ��������ݣ��ʶ���Ҫ���¶���

	virtual bool Inquire(long lCurrentTime) override final;

	bool InquireRTData(long lCurrentTime);

protected:
};

typedef shared_ptr<CSinaRTDataSource> CSinaRTDataSourcePtr;
extern CSinaRTDataSourcePtr gl_pSinaRTDataSource;