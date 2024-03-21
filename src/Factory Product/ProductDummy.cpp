#include "pch.h"

#include "ProductDummy.h"

CProductDummy::CProductDummy() {
	m_strInquiryFunction = _T("");
}

CString CProductDummy::CreateMessage() {
	return _T("");
}

void CProductDummy::ParseAndStoreWebData(CWebDataPtr pWebData) {
	TRACE("dummy product\n");
}
