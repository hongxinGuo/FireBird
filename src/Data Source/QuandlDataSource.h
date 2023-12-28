#pragma once

#include"VirtualDataSource.h"
#include"QuandlFactory.h"

class CQuandlDataSource : public CVirtualDataSource {
public:
	CQuandlDataSource();
	CQuandlDataSource(const CQuandlDataSource& other) = delete;
	CQuandlDataSource(CQuandlDataSource&& other) noexcept = delete;
	CQuandlDataSource& operator=(const CQuandlDataSource& other) = delete;
	CQuandlDataSource& operator=(CQuandlDataSource&& other) noexcept = delete;
	~CQuandlDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(long lCurrentTime) override;

	virtual bool InquireQuandl();

	void ConfigureSession() override;
	bool ParseData(CWebDataPtr pWebData) override; // ����ΪJSon��ʽ, ��Ҫ����

protected:
	CQuandlFactory m_QuandlFactory;

	bool m_fQuandlDataInquiryFinished{false};
};

using CQuandlDataSourcePtr = shared_ptr<CQuandlDataSource>;

extern CQuandlDataSourcePtr gl_pQuandlDataSource;
