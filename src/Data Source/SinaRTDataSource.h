#pragma once

#include"VirtualDataSource.h"

class CSinaRTDataSource : public CVirtualDataSource {
public:
	CSinaRTDataSource();
	~CSinaRTDataSource() override = default;

	bool Reset(void) override;
	bool UpdateStatus(void) override;

	bool Inquire(const long lCurrentTime) override;

	bool InquireRTData(const long lCurrentTime);

	bool ParseData(CWebDataPtr pWebData) override { return true; } // ����Ϊ��JSon��ʽ��������

	bool PrepareNextInquiringString(void) override;
	CString GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) override;
	void ConfigureSession(void) override; // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����Sina RT��m_pSession״̬��
	bool ReportStatus(long lNumberOfData) const override;

protected:
};

using CSinaRTDataSourcePtr = shared_ptr<CSinaRTDataSource>;
extern CSinaRTDataSourcePtr gl_pSinaRTDataSource;
