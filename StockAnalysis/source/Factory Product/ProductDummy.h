#pragma once

#include"VirtualProductWebData.h"

class CProductDummy : public CVirtualProductWebData {
public:
	DECLARE_DYNCREATE(CProductDummy)
		CProductDummy();
	~CProductDummy() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData, CVirtualDataSource* pDataSource = nullptr) override final;
};

typedef shared_ptr<CProductDummy> CProductDummyPtr;
