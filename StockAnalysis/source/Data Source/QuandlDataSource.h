#pragma once

#include"VirtualDataSource.h"
#include"QuandlFactory.h"

class CQuandlDataSource final : public CVirtualDataSource {
public:
	CQuandlDataSource(void);
	~CQuandlDataSource(void) override = default;

	virtual bool Reset(void) override final;
	virtual bool UpdateStatus(void) override final;

	virtual bool Inquire(long lCurrentTime) override final;

protected:
	CQuandlFactory m_QuandlFactory;
};

typedef shared_ptr<CQuandlDataSource> CQuandlDataSourcePtr;

extern CQuandlDataSourcePtr gl_pQuandlDataSource;
