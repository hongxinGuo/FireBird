//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Finnhub行情接口读取线程。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"WebInquirer.h"

UINT ThreadReadQuandlData(not_null<CQuandlWebInquiry*> pQuandlWebData) {
	ASSERT(!gl_pWorldMarket->IsQuandlDataReceived());
	if (pQuandlWebData->ReadWebData()) {
		CWebDataPtr pWebDataReceived = pQuandlWebData->TransferWebDataToQueueData();
		if (pWebDataReceived != nullptr) {
			gl_WebInquirer.PushQuandlData(pWebDataReceived);
		}
	}
	else {
		while (gl_WebInquirer.GetQuandlDataSize() > 0) gl_WebInquirer.PopQuandlData();
		gl_pWorldMarket->SetQuandlInquiring(false); // 当工作线程出现故障时，需要清除Quandl数据申请标志。
	}
	pQuandlWebData->SetReadingWebData(false);
	gl_pWorldMarket->SetQuandlDataReceived(true); // 接收完网络数据后，清除状态。

	return 9;
}