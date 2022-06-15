#pragma once

#include"ProductWebData.h"

class CProductDummy : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductDummy)
	CProductDummy();
	~CProductDummy() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CProductDummy> CProductDummyPtr;
