////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ȡ����������ʷ���ݵ��̡߳�
//
// ��ȡ����ʱ���Լ��Σ���ֹ�����ӳٵ������ݽضϡ�Ŀǰ�ڶ������ݺ��ٲ������Σ���û�����ݺ�ŷ��ء�
//
// ��ʱfReading == false�Ķ��Ի�ʧ�ܣ���������ֻ���������ô˱������ʹ����ǲ�ͬ�߳�ͬʱ������ԭ����Ҫ��Ϊ
// ͬ���¼�ģʽ���Ѵ��߳�Ϊ�á��о�֮�����ڵ��ô��߳�ǰ�����ã��Ͳ�����ֹ����ˣ��ɼ�ʱ�����߳�ʱ�������ʱ���£���
//
// ���߳����յĹ��ܣ�Ҫ������ֻ����ȡ����Ҫ��һЩ��Ӧ�ü������ʹ洢���ܣ��о�֮�����ܶ���漰���������ͬ�����⣬��ҪȨ�⣩��
// �༴���̵߳Ĺ���Ϊ��
// 1.�ӹܵ�����ȡ��Ҫ��ȡ������ʷ���ݵĹ�Ʊ���룬���ô��봦����������߷�������Ҫ��ĸ�ʽ��Ȼ���͸����߷�������
// 2.�ȴ�һ��ʱ���200ms����ʼ�ӷ��������������ݡ�
// 3.�����յ������ݽ��룬�ɹ��Ļ�������Ӧ��Ʊ�����������У����������Ӧ�ı�ʶ��
//
// ���̵߳���������ֵΪ4
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"globedef.h"
#include"WebInquirer.h"
#include"ChinaMarket.h"

UINT ThreadReadNeteaseDayLine(not_null<CNeteaseDayLineWebInquiry*> pNeteaseDayLineWebInquiry) {
	CWebDataPtr pWebDataReceived = nullptr;

	pNeteaseDayLineWebInquiry->PrepareBeforeReadingWebData();
	// ���׵��������ݷ������ƺ��޷�ʹ��һ�ζ���Ĺ��ܣ�ֻ�������ֵȴ���ʱ��ʽ��ԭ��δ֪��
	if (pNeteaseDayLineWebInquiry->ReadWebData()) { // �����ǲ���һ�ζ���ķ�ʽ���������������ݷ��������ܲ�֧�֡�
	//if (pNeteaseDayLineWebInquiry->ReadWebDataTimeLimit(/*siDelayTime*/ 200, 30, 30)) { // �����ǲ��õȴ�ʱ��ķ�ʽ��Ŀǰ���С�
		// ����ȡ���������ݴ������������ݴ���
		pWebDataReceived = pNeteaseDayLineWebInquiry->TransferWebDataToQueueData();
		pNeteaseDayLineWebInquiry->SetTime(pWebDataReceived);
		pNeteaseDayLineWebInquiry->UpdateStatusWhenSecceed(pWebDataReceived);
		pNeteaseDayLineWebInquiry->StoreWebData(pWebDataReceived);
	}
	else {
		pNeteaseDayLineWebInquiry->ProcessFailedReading();
	}
	pNeteaseDayLineWebInquiry->SetReadingWebData(false);

	return 4; // ���߳���������ֵΪ4
}