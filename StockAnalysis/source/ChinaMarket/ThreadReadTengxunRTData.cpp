/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��Ѷʵʱ����ӿڶ�ȡ�̡߳�
//
// �˹����߳���Ϊ����ʵʱ�����̵߳ĸ����Ͳ��䣬�ʶ���Ҫ��ϵͳ׼����Ϻ󷽿�ִ�С�
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