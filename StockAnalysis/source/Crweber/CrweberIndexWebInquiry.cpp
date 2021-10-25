#include"pch.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include"Accessory.h"

#include "CrweberIndexWebInquiry.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

CCrweberIndexWebInquiry::CCrweberIndexWebInquiry() : CVirtualWebInquiry() {
	m_strWebDataInquirePrefix = _T("http://www.crweber.com");
	m_strWebDataInquireSuffix = _T("");
	m_strConnectionName = _T("CrweberIndex");
}

CCrweberIndexWebInquiry::~CCrweberIndexWebInquiry() {
}

bool CCrweberIndexWebInquiry::PrepareNextInquiringStr(void) {
	CString strMiddle = _T("");

	CreateTotalInquiringString(strMiddle);
	return true;
}

void CCrweberIndexWebInquiry::StoreWebData(CWebDataPtr pWebData) {
	gl_WebInquirer.PushCrweberData(pWebData);
}