////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ȡ����������ʷ���ݵ��̡߳�
//
// ��ȡ����ʱ���Լ��Σ���ֹ�����ӳٵ������ݽضϡ�Ŀǰ�ڶ������ݺ��ڲ������Σ���û�����ݺ�ŷ��ء�
//
// ��ʱfReading == false�Ķ��Ի�ʧ�ܣ���������ֻ���������ô˱������ʹ����ǲ�ͬ�߳�ͬʱ������ԭ����Ҫ��Ϊ
// ͬ���¼�ģʽ���Ѵ��߳�Ϊ�á��о�֮�����ڵ��ô��߳�ǰ�����ã��Ͳ�����ֹ����ˣ��ɼ�ʱ�����߳�ʱ�������ʱ���£���
//
// ���߳����յĹ��ܣ�Ҫ������ֻ����ȡ����Ҫ��һЩ��Ӧ�ü������ʹ洢���ܣ��о�֮�����ܶ���漰���������ͬ�����⣬��ҪȨ�⣩��
// �༴���̵߳Ĺ���Ϊ��
// 1.�ӹܵ�����ȡ��Ҫ��ȡ������ʷ���ݵĹ�Ʊ���룬���ô��봦����������߷�������Ҫ��ĸ�ʽ��Ȼ���͸����߷�������
// 2.�ȴ�һ��ʱ���100ms����ʼ�ӷ��������������ݡ�
// 3.�����յ������ݽ��룬�ɹ��Ļ�������Ӧ��Ʊ�����������У����������Ӧ�ı�ʶ��
//
// ���̵߳���������ֵΪ2
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include"Market.h"

UINT ThreadReadNeteaseDayLine(LPVOID pParam) {
  CNeteaseDayLineWebData* pNeteaseWebDayLineData = (CNeteaseDayLineWebData*)pParam;
  const clock_t tt = clock();

  if (pNeteaseWebDayLineData->ReadWebData(/*siDelayTime*/ 50, 30, 20)) {
    // ����ȡ���������ݷ�����ع�Ʊ���������ݻ������У���������ر�ʶ��
    CStockPtr pStock = gl_ChinaStockMarket.GetStockPtr(pNeteaseWebDayLineData->GetDownLoadingStockCode());
    pStock->TransferNeteaseDayLineWebDataToBuffer(pNeteaseWebDayLineData);
  }
  gl_ChinaStockMarket.SetReadingNeteaseDayLineDataTime(clock() - tt);

  return 4; // ���߳���������ֵΪ4
}