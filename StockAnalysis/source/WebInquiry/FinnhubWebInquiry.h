//////////////////////////////////////////////////////////////////////////////
//
// CFinnhuaWebInquiryӦ�ñ�����Ϊfinal�����ڲ��Ե���Ҫ, CMockFinnhubWebInquiryҪ�̳и��࣬
// �ʶ��������ඨ��Ϊfinal��ֻ�ǽ���Ҫ���麯������Ϊfinal��
//
// �������Ƶ���Ҳͬ������
//
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

using namespace std;
#include<memory>

class CFinnhubWebInquiry : public CVirtualWebInquiry {
public:
	CFinnhubWebInquiry();
	virtual ~CFinnhubWebInquiry();

	virtual bool PrepareNextInquiringString(void) override;
	virtual CString GetNextInquiringMiddleString(long, bool) override final { return _T(""); }
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void ConfigerateSession(void) override final;
	virtual bool ParseData(CWebDataPtr pWebData); // ����ΪJSon��ʽ, ��Ҫ����
	virtual void ClearUpIfReadingWebDataFailed(void) override final; // ����ʧ�ܵĽ��չ���
	virtual void UpdateStatusAfterReadingWebData(void) override final;
	virtual void StoreWebData(CWebDataPtr pWebDataReceived) override final;

protected:
};

typedef shared_ptr<CFinnhubWebInquiry> CFinnhubWebInquiryPtr;
