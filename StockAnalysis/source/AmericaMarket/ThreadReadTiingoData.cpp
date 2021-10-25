//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Finnhub行情接口读取线程。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"WebInquirer.h"

UINT ThreadReadTiingoData(not_null<CTiingoWebInquiry*> pTiingoWebData) {
	ASSERT(!gl_pWorldMarket->IsTiingoDataReceived());
	if (pTiingoWebData->ReadWebData()) {
		CWebDataPtr pWebDataReceived = pTiingoWebData->TransferWebDataToQueueData();
		if (pWebDataReceived != nullptr) {
			gl_WebInquirer.PushTiingoData(pWebDataReceived);
		}
	}
	else {
		while (gl_WebInquirer.GetFinnhubDataSize() > 0) gl_WebInquirer.PopFinnhubData();
		gl_pWorldMarket->SetTiingoInquiring(false); // 当工作线程出现故障时，需要清除Tiingo数据申请标志。
	}
	pTiingoWebData->SetReadingWebData(false);
	gl_pWorldMarket->SetTiingoDataReceived(true); // 接收完网络数据后，清除状态。

	return 8;
}