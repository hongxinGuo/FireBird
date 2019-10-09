///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ���ݵĹ����̡߳�
//
// ���߳�ֻ��������Ʊ��m_dequeRTData�������������Խ�����ͬ�������������С��
// ���߳���CMarket���SchedulingTaskPerSecond()�������ã�ÿ��һ�Ρ�
//
// ����ֵΪ3
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"Market.h"

UINT ThreadCalculatingRTDataProc(LPVOID) {
  ASSERT(gl_ChinaStockMarket.SystemReady()); // ���ñ������߳�ʱ�������ú��г���
  ASSERT(!gl_ThreadStatus.IsSavingTempData()); // �����������̻߳���

  gl_ThreadStatus.SetCalculatingRTData(true);
  if (gl_ThreadStatus.IsRTDataNeedCalculate()) { // ֻ���г���ʼ̬���úú󣬲�������ʵʱ���ݡ�
    gl_ChinaStockMarket.CalculateRTData();
    gl_ThreadStatus.SetRTDataNeedCalculate(false);
  }
  gl_ThreadStatus.SetCalculatingRTData(false);

  return 3;
}