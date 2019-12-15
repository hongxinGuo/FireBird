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
  CNeteaseWebDayLineData* pNeteaseWebDayLineData = (CNeteaseWebDayLineData*)pParam;
  static int siDelayTime = 600;
  static bool fStarted = false;

  const clock_t tt = clock();

  if (pNeteaseWebDayLineData->ReadWebData(siDelayTime, 30, 30)) {
    // ����ȡ���������ݷ�����ع�Ʊ���������ݻ������У���������ر�ʶ��
    char* p = pNeteaseWebDayLineData->GetBufferAddr();
    CStockPtr pStock = gl_ChinaStockMarket.GetStockPtr(pNeteaseWebDayLineData->GetDownLoadingStockCode());
    if (pStock->m_pDayLineBuffer != nullptr) delete pStock->m_pDayLineBuffer;
    pStock->m_pDayLineBuffer = new char[pNeteaseWebDayLineData->GetByteReaded() + 1]; // ��������Ҫ���һ���ַ����ȣ�����Ǹ�0x000����
    char* pbuffer = pStock->m_pDayLineBuffer;
    for (int i = 0; i < pNeteaseWebDayLineData->GetByteReaded() + 1; i++) {
      *pbuffer++ = *p++;
    }
    pStock->m_lDayLineBufferLength = pNeteaseWebDayLineData->GetByteReaded();
    pStock->SetDayLineNeedProcess(true);
  }

  if (!fStarted) {
    fStarted = true;
    siDelayTime = 50;
  }

  gl_ChinaStockMarket.SetReadingNeteaseDayLineDataTime(clock() - tt);

  return 2; // ���߳���������ֵΪ2
}