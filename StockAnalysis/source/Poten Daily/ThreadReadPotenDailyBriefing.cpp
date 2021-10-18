/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Poten.com����ӿڶ�ȡ�̡߳�
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "Thread.h"
#include"VirtualWebInquiry.h"
#include"WebInquirer.h"

UINT ThreadReadPotenDailyBriefing(not_null<CPotenDailyBriefingWebInquiry*> pPotenDailyBriefingWebInquiry) {
	if (pPotenDailyBriefingWebInquiry->ReadWebData()) {
		CWebDataPtr pWebDataReceived = pPotenDailyBriefingWebInquiry->TransferWebDataToQueueData();
		if (pWebDataReceived != nullptr) {
			pWebDataReceived->SetTime(gl_pPotenDailyBriefingMarket->TransferToUTCTime(pPotenDailyBriefingWebInquiry->GetInquiringDate()));
			gl_WebInquirer.PushPotenDailyBriefingData(pWebDataReceived);
		}
	}
	pPotenDailyBriefingWebInquiry->SetReadingWebData(false);

	return 6;
}