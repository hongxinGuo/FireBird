#pragma once

#include"VirtualDataSource.h"
#include"QuandlFactory.h"

class CQuandlDataSource final : public CVirtualDataSource {
public:
	CQuandlDataSource(void);
	~CQuandlDataSource(void) override = default;

	bool Reset(void) final;
	bool UpdateStatus(void) final;

	bool Inquire(const long lCurrentTime) override;

protected:
	CQuandlFactory m_QuandlFactory;
};

using CQuandlDataSourcePtr = shared_ptr<CQuandlDataSource>;

extern CQuandlDataSourcePtr gl_pQuandlDataSource;
