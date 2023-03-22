#include "pch.h"

#include"worldMarket.h"

#include "ProductDummy.h"

CProductDummy::CProductDummy() {
	m_strClassName = _T("Dummy Product");
	m_strInquiryFunction = _T("");
	m_lIndex = -1;
}

CString CProductDummy::CreateMessage() {
	return _T("");
}

bool CProductDummy::ParseAndStoreWebData(CWebDataPtr pWebData) {
	TRACE("dummy product\n");
	return true;
}
