#include"Market.h"

UINT ThreadSaveDayLineOfOneStock(LPVOID pParam) {
  // ���ݹ�����ΪЯ������ָ��Ľṹ������ָ���޷�����pParamֱ�Ӵ��ݹ���
  // ���ߴ��ݹ������Ǵ�����ָ����ȡ����ԭʼָ�룬ע�ⲻ������֮��������ָ�루�����Զ�������
  CStock* pStock;
  CString str;

  CSingleLock singleLock(&gl_SaveOneStockDayLine);
  singleLock.Lock();

  gl_ThreadStatus.IncreaseNunberOfSavingDayLineThreads();
  pStock = (CStock*)pParam;
  pStock->SaveDayLine();
  pStock->SetDayLineLoaded(false);
  pStock->m_vDayLine.clear();
  gl_ThreadStatus.DecreaseNumberOfSavingDayLineThreads();
  ASSERT(pStock != nullptr);
  singleLock.Unlock();

  return 6;
}