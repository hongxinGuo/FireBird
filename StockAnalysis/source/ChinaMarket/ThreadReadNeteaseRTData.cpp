/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ����ӿڶ�ȡ�̡߳�
//
// �˹����߳���Ϊ����ʵʱ�����̵߳ĸ����Ͳ��䣬�ʶ���Ҫ��ϵͳ׼����Ϻ󷽿�ִ�С�
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
		TRACE("��ȡ����ʵʱ���ݳ���\n");
	}
	pNeteaseRTWebData->SetReadingWebData(false);

	return 3;
}