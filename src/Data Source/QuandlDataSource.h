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
	bool ParseData(CWebDataPtr pWebData) override; // ����ΪJSon��ʽ, ��Ҫ����

protected:
	CQuandlFactory m_QuandlFactory;

	bool m_fQuandlDataInquiryFinished{false};
};

using CQuandlDataSourcePtr = shared_ptr<CQuandlDataSource>;

extern CQuandlDataSourcePtr gl_pQuandlDataSource;
