#include "pch.h"

module FireBird.Product.Dummy;

CProductDummy::CProductDummy() {
	m_strInquiryFunction = _T("");
}

CString CProductDummy::CreateMessage() {
	return _T("");
}
