#pragma once

#include"VirtualWebInquiry.h"

class CCrweberIndexWebInquiry : public CVirtualWebInquiry
{
public:
	CCrweberIndexWebInquiry();
	virtual ~CCrweberIndexWebInquiry();

	virtual bool PrepareNextInquiringStr(void) override;
	virtual void StoreWebData(CWebDataPtr pWebData); // �������϶�ȡ�������ݴ��뱾��
};

typedef shared_ptr<CCrweberIndexWebInquiry> CCrweberIndexWebInquiryPtr;
