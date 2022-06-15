#include "pch.h"

#include"globedef.h"
#include"accessory.h"
#include"worldMarket.h"

#include "DummyProduct.h"

IMPLEMENT_DYNCREATE(CProductDummy, CProductWebSourceData)

CProductDummy::CProductDummy() {
	m_strClassName = _T("Dummy Product");
	m_strInquiringStr = _T("");
	m_lIndex = -1;
}

CString CProductDummy::CreatMessage(void) {
	ASSERT(0);
	return _T("");
}

bool CProductDummy::ParseAndStoreWebData(CWebDataPtr pWebData) {
	TRACE("dummy product\n");
	ASSERT(0);
	return true;
}