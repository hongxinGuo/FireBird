/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Crweber.com行情接口读取线程。
//
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "Thread.h"
#include"VirtualWebInquiry.h"
#include"WebInquirer.h"

UINT ThreadReadCrweberIndex(not_null<CCrweberIndexWebInquiry*> pCrweberIndexWebData) {
	if (pCrweberIndexWebData->ReadWebData()) {
		CWebDataPtr pWebDataReceived = pCrweberIndexWebData->TransferWebDataToQueueData();
		if (pWebDataReceived != nullptr) {
			gl_WebInquirer.PushCrweberData(pWebDataReceived);
		}
	}
	pCrweberIndexWebData->SetReadingWebData(false);

	return 5;
}