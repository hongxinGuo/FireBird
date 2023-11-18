#pragma once

#include"VirtualDataSource.h"
#include"QuandlFactory.h"

class CQuandlDataSource : public CVirtualDataSource {
public:
	CQuandlDataSource();
	~CQuandlDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(const long lCurrentTime) override;

	virtual bool InquireQuandl();

	void ConfigureSession() override;
	bool ParseData(CWebDataPtr pWebData) override; // 数据为JSon格式, 需要解析

protected:
	CQuandlFactory m_QuandlFactory;
};

using CQuandlDataSourcePtr = shared_ptr<CQuandlDataSource>;

extern CQuandlDataSourcePtr gl_pQuandlDataSource;
