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
// 1.�ӹܵ�����ȡ��Ҫ��ȡ������ʷ���ݵĹ�Ʊ���룬���ô��봦�����������߷�������Ҫ��ĸ�ʽ��Ȼ���͸����߷�������
// 2.�ȴ�һ��ʱ���200ms����ʼ�ӷ��������������ݡ�
// 3.�����յ������ݽ��룬�ɹ��Ļ�������Ӧ��Ʊ�����������У����������Ӧ�ı�ʶ��
//
// ���̵߳���������ֵΪ4
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"
#include"ChinaMarket.h"

UINT ThreadReadNeteaseDayLine(not_null<CNeteaseDayLineWebInquiry*> pNeteaseDayLineWebData) {
  CChinaStockPtr pStake = nullptr;

  ASSERT(pNeteaseDayLineWebData->IsReadingWebData());
  gl_ThreadStatus.IncreaseRunningThread();
  // ���׵��������ݷ������ƺ��޷�ʹ��һ�ζ���Ĺ��ܣ�ֻ�������ֵȴ���ʱ��ʽ��ԭ��δ֪��
  //if (pNeteaseDayLineWebData->ReadWebData()) { // �����ǲ���һ�ζ���ķ�ʽ���������������ݷ��������ܲ�֧�֡�
  if (pNeteaseDayLineWebData->ReadWebData3(/*siDelayTime*/ 200, 30, 30)) { // �����ǲ��õȴ�ʱ��ķ�ʽ��Ŀǰ���С�
    // ����ȡ���������ݷ�����ع�Ʊ���������ݻ������У���������ر�ʶ��
    pStake = gl_pChinaStakeMarket->GetStock(pNeteaseDayLineWebData->GetDownLoadingStockCode());
    pStake->TransferNeteaseDayLineWebDataToBuffer(pNeteaseDayLineWebData);
    pStake->SetDayLineNeedProcess(true);
  }
  gl_ThreadStatus.DecreaseRunningThread();

  return 4; // ���߳���������ֵΪ4
}