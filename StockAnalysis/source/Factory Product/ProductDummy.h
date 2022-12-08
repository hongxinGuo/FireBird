#pragma once

#include"VirtualProductWebData.h"

class CProductDummy : public CVirtualProductWebData {
public:
	DECLARE_DYNCREATE(CProductDummy)
		CProductDummy();
	~CProductDummy() = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CProductDummy> CProductDummyPtr;
