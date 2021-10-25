//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Finnhub����ӿڶ�ȡ�̡߳�
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
		gl_pWorldMarket->SetFinnhubInquiring(false); // �������̳߳��ֹ���ʱ����Ҫ���Finnhub���������־��
	}
	pFinnhubWebData->SetReadingWebData(false);
	gl_pWorldMarket->SetFinnhubDataReceived(true); // �������������ݺ����״̬��

	return 7;
}