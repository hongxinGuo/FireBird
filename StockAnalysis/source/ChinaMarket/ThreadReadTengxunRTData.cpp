/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 腾讯实时行情接口读取线程。
//
// 此工作线程作为新浪实时行情线程的辅助和补充，故而需要在系统准备完毕后方可执行。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"globedef.h"
#include"WebInquirer.h"

UINT ThreadReadTengxunRTData(not_null<CTengxunRTWebInquiry*> pTengxunRTWebData) {
	if (pTengxunRTWebData->ReadWebData()) {
		CWebDataPtr pWebDataReceived = pTengxunRTWebData->TransferWebDataToQueueData();
		if (pWebDataReceived != nullptr) {
			gl_WebInquirer.PushTengxunRTData(pWebDataReceived);
		}
	}
	pTengxunRTWebData->SetReadingWebData(false);

	return 2;
}