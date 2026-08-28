#include "pch.h"

#include "ProductDummy.h"

using std::make_shared;

CProductDummy::CProductDummy() {
	m_strInquiryFunction = "";
}

void CProductDummy::InquireData(const std::stop_token& st) {
}

void CProductDummy::WebStatusCheck(cpr::Response& r) {
}

void CProductDummy::UpdateSystemStatus() {
}

shared_ptr<vector<string>> CProductDummy::CreateMessage() {
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back("");
	return pInquiry;
}
