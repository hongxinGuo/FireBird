#pragma once

#include "VirtualWebInquiry.h"
#include"WebRTData.h"

using namespace std;
#include<memory>

class CTiingoWebInquiry : public CVirtualWebInquiry {
public:
	CTiingoWebInquiry();
	virtual ~CTiingoWebInquiry();

	virtual bool PrepareNextInquiringString(void) override;
	virtual CString GetNextInquiringMiddleString(long, bool) override final { return _T(""); }
	virtual bool ReportStatus(long lNumberOfData) const override;
	virtual void ConfigerateSession(void) override final; // ����m_pSession״̬��
	virtual bool ParseData(CWebDataPtr pWebData); // ����ΪJSon��ʽ, ��Ҫ����
};

typedef shared_ptr<CTiingoWebInquiry> CTiingoWebInquiryPtr;
extern CTiingoWebInquiryPtr gl_pTiingoWebInquiry;
