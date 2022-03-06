#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

class CTengxunRTWebInquiry : public CVirtualWebInquiry {
public:
	CTengxunRTWebInquiry();
	virtual ~CTengxunRTWebInquiry();

	virtual bool TransferData(CWebDataPtr pWebData) override final; // �����յ���������pWebData�С�
	virtual bool ParseData(CWebDataPtr pWebData) override final { return true; } // ����Ϊ��JSon��ʽ��������
	virtual bool ProcessData(CWebDataPtr pWebData) override final; // ������յ������ݡ�

	virtual bool PrepareNextInquiringStr(void) override;
	virtual CString GetNextInquiringMiddleStr(long lTotalNubmer, bool fUsingTotalStockSet = false) override;
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) override final {} // ���轫�����϶�ȡ�������ݴ��뱾��

	bool IsTengxunRTDataInvalid(CWebData& WebDataReceived);
};

typedef shared_ptr<CTengxunRTWebInquiry> CTengxunRTWebInquiryPtr;
