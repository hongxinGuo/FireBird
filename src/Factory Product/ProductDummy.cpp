#include "pch.h"

#include "ProductDummy.h"

using std::make_shared;

CProductDummy::CProductDummy() {
	m_strInquiryFunction = "";
}

shared_ptr<vector<string>> CProductDummy::CreateMessage() {
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back("");
	return pInquiry;
}
