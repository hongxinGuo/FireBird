//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Finnhub行情接口读取线程。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"WebInquirer.h"

UINT ThreadReadFinnhubData(not_null<CFinnhubWebInquiry*> pFinnhubWebData) {
	ASSERT(!gl_pWorldMarket->IsFinnhubDataReceived());
	if (pFinnhubWebData->ReadWebData()) {
		CWebDataPtr pWebDataReceived = pFinnhubWebData->TransferWebDataToQueueData();
		if (pWebDataReceived != nullptr) {
			gl_WebInquirer.PushFinnhubData(pWebDataReceived);
		}
	}
	else {
		while (gl_WebInquirer.GetFinnhubDataSize() > 0) gl_WebInquirer.PopFinnhubData();
		gl_pWorldMarket->SetFinnhubInquiring(false); // 当工作线程出现故障时，需要清除Finnhub数据申请标志。
	}
	pFinnhubWebData->SetReadingWebData(false);
	gl_pWorldMarket->SetFinnhubDataReceived(true); // 接收完网络数据后，清除状态。

	return 7;
}