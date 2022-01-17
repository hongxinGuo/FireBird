#pragma once

#include"VirtualWebInquiry.h"
#include"WebData.h"

class CCrweberIndexWebInquiry : public CVirtualWebInquiry
{
public:
	CCrweberIndexWebInquiry();
	virtual ~CCrweberIndexWebInquiry();

	virtual bool PrepareNextInquiringStr(void) override;
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored); // 将网络上读取到的数据存入本地
};

typedef shared_ptr<CCrweberIndexWebInquiry> CCrweberIndexWebInquiryPtr;
