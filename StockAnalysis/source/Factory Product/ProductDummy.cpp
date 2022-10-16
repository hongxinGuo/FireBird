#include "pch.h"

#include"worldMarket.h"

#include "ProductDummy.h"

IMPLEMENT_DYNCREATE(CProductDummy, CVirtualProductWebData)

CProductDummy::CProductDummy() {
	m_strClassName = _T("Dummy Product");
	m_strInquiringStr = _T("");
	m_lIndex = -1;
}

CString CProductDummy::CreatMessage(void) {
	return _T("");
}

bool CProductDummy::ParseAndStoreWebData(CWebDataPtr pWebData) {
	TRACE("dummy product\n");
	return true;
}