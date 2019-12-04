///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ���ݵĹ����̡߳�
//
// ���߳�ֻ��������Ʊ��m_queueRTData�������������Խ�����ͬ�������������С��
// ���߳���CMarket���SchedulingTaskPerSecond()�������ã�ÿ��һ�Ρ�
//
// ���߳���洢ʵʱ��ʱ���ݵ��̻߳��⡣
//
// ����ֵΪ3
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"Market.h"

UINT ThreadCalculateRTData(LPVOID) {
  ASSERT(gl_ChinaStockMarket.SystemReady()); // ���ñ������߳�ʱ�������ú��г���
  ASSERT(!gl_ThreadStatus.IsSavingTempData()); // �����������̻߳���

  gl_ThreadStatus.SetCalculatingRTData(true);
  if (gl_ThreadStatus.IsRTDataNeedCalculate()) { // ֻ���г���ʼ̬���úú󣬲�������ʵʱ���ݡ�
    gl_ChinaStockMarket.ProcessRTData();
    gl_ThreadStatus.SetRTDataNeedCalculate(false);
  }
  gl_ThreadStatus.SetCalculatingRTData(false);

  return 3;
}