//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Finnhub����ӿڶ�ȡ�̡߳�
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
		gl_pWorldMarket->SetQuandlInquiring(false); // �������̳߳��ֹ���ʱ����Ҫ���Quandl���������־��
	}
	pQuandlWebData->SetReadingWebData(false);
	gl_pWorldMarket->SetQuandlDataReceived(true); // �������������ݺ����״̬��

	return 9;
}