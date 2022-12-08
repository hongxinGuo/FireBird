#pragma once

#include"VirtualDataSource.h"
#include"QuandlFactory.h"

class CQuandlDataSource : public CVirtualDataSource
{
public:
	CQuandlDataSource(void);
	virtual ~CQuandlDataSource(void) override;

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;

	virtual bool Inquire(long lCurrentTime) override final;

protected:
	CQuandlFactory m_QuandlFactory;
};

typedef shared_ptr<CQuandlDataSource> CQuandlDataSourcePtr;

extern CQuandlDataSourcePtr gl_pQuandlDataSource;