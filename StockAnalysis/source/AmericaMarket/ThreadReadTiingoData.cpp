//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Finnhub����ӿڶ�ȡ�̡߳�
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
		gl_pWorldMarket->SetTiingoInquiring(false); // �������̳߳��ֹ���ʱ����Ҫ���Tiingo���������־��
	}
	pTiingoWebData->SetReadingWebData(false);
	gl_pWorldMarket->SetTiingoDataReceived(true); // �������������ݺ����״̬��

	return 8;
}