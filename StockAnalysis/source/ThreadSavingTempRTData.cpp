//////////////////////////////////////////////////////////////////////////////////////
//
// �洢��ʱʵʱ���ݹ����߳�
//
// ���߳������ʵʱ�����̻߳��⣬��Ҫע��֮��
//
// ����ֵΪ4
//
///////////////////////////////////////////////////////////////////////////////////////
#include"Market.h"

UINT ThreadSavingTempRTDataProc(LPVOID)
{
  ASSERT(gl_ChinaStockMarket.SystemReady()); // ���ñ������߳�ʱ�������ú��г���
  ASSERT(!gl_ThreadStatus.IsCalculatingRTData()); // �����������̻߳���

  gl_ThreadStatus.SetSavingTempData(true);

  gl_ChinaStockMarket.SaveTodayTempData();

  ASSERT(!gl_ThreadStatus.IsCalculatingRTData()); // �ٴ�ȷ��һ��
  gl_ThreadStatus.SetSavingTempData(false);

  return 4;
}