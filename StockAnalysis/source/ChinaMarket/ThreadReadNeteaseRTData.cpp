/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网易实时行情接口读取线程。
//
// 此工作线程作为新浪实时行情线程的辅助和补充，故而需要在系统准备完毕后方可执行。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"globedef.h"
#include"WebInquirer.h"

UINT ThreadReadNeteaseRTData(not_null<CNeteaseRTWebInquiry*> pNeteaseRTWebData) {
	if (pNeteaseRTWebData->ReadWebData()) {
		CWebDataPtr pWebDataReceived = pNeteaseRTWebData->TransferWebDataToQueueData();
		if (pWebDataReceived != nullptr) {
			gl_WebInquirer.PushNeteaseRTData(pWebDataReceived);
		}
	}
	else {
		TRACE("读取网易实时数据出错\n");
	}
	pNeteaseRTWebData->SetReadingWebData(false);

	return 3;
}