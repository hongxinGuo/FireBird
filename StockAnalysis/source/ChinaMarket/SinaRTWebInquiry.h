#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

class CSinaRTWebInquiry : public CVirtualWebInquiry {
public:
	CSinaRTWebInquiry();
	virtual ~CSinaRTWebInquiry();

	virtual bool TransferData(CWebDataPtr pWebData) override final; // �����յ���������pWebData�У�Ĭ�ϲ��ƶ���
	virtual bool ParseData(CWebDataPtr pWebData) override final { return true; } // ����Ϊ��JSon��ʽ��������
	virtual bool ProcessData(CWebDataPtr pWebData) override final { return true; } // ������յ������ݡ� Ĭ�ϲ�����

	virtual bool PrepareNextInquiringStr(void) override;
	virtual CString GetNextInquiringMiddleStr(long lTotalNumer, bool fUsingTotalStockSet = false) override;
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final; // �������϶�ȡ�������ݴ��뱾��
};

typedef shared_ptr<CSinaRTWebInquiry> CSinaRTWebInquiryPtr;
