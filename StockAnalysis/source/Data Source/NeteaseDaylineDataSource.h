#pragma once

#include"VirtualDataSource.h"
#include"NeteaseDaylineWebData.h"

class CNeteaseDaylineDataSource : public CVirtualDataSource {
public:
	CNeteaseDaylineDataSource();
	~CNeteaseDaylineDataSource();

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;
	virtual void ParseAndStoreData(CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData) override final; //

	virtual bool Inquire(long lCurrentTime) override final;

	size_t DataSize(void) { return(m_qNeteaseDayLine.Size()); }
	void PushData(CNeteaseDayLineWebDataPtr pData) { m_qNeteaseDayLine.PushData(pData); }
	CNeteaseDayLineWebDataPtr PopData(void) { return m_qNeteaseDayLine.PopData(); }

protected:
	CTemplateMutexAccessQueue<CNeteaseDayLineWebData> m_qNeteaseDayLine; // 网易日线数据
};

typedef shared_ptr<CNeteaseDaylineDataSource> CNeteaseDaylineDataSourcePtr;
extern CNeteaseDaylineDataSourcePtr gl_pNeteaseDaylineDataSource;