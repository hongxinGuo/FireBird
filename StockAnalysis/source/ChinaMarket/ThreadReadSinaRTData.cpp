/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ����ӿڶ�ȡ�̡߳�
//
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"WebInquirer.h"
#include "..\Thread.h"

UINT ThreadReadSinaRTData(not_null<CSinaRTWebInquiry*> pSinaRTWebData) {
	if (pSinaRTWebData->ReadWebData()) {
		CWebDataPtr pWebDataReceived = pSinaRTWebData->TransferWebDataToQueueData();
		if (pWebDataReceived != nullptr) {
			gl_WebInquirer.PushSinaRTData(pWebDataReceived);
		}
	}
	pSinaRTWebData->SetReadingWebData(false);

	return 10;
}