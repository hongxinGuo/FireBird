#pragma once

#include"VirtualDataSource.h"
#include"QuandlFactory.h"

class CQuandlDataSource final : public CVirtualDataSource {
public:
	CQuandlDataSource(void);
	~CQuandlDataSource(void) override = default;

	bool Reset(void) override;
	bool UpdateStatus(void) override;

	bool Inquire(const long lCurrentTime) override;

	bool PrepareNextInquiringString(void) override;
	void CreateTotalInquiringString(CString strMiddle) override;
	CString GetNextInquiringMiddleString(long, bool) override { return _T(""); }
	bool ReportStatus(long lNumberOfData) const override;
	void ConfigureSession(void) final;
	bool ParseData(CWebDataPtr pWebData) override; // 数据为JSon格式, 需要解析

protected:
	CQuandlFactory m_QuandlFactory;
};

using CQuandlDataSourcePtr = shared_ptr<CQuandlDataSource>;

extern CQuandlDataSourcePtr gl_pQuandlDataSource;
