//////////////////////////////////////////////////////////////////////////////////////
//
// �洢��ʱʵʱ���ݹ����߳�
//
// ���߳������ʵʱ�����̻߳��⣬��Ҫע��֮��
//
// ����ֵΪ13
//
///////////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"
#include"ChinaMarket.h"

UINT ThreadSaveTempRTData(void) {
  ASSERT(gl_pChinaStockMarket->IsSystemReady()); // ���ñ������߳�ʱ�������ú��г���
  ASSERT(!gl_ThreadStatus.IsCalculatingRTData()); // �����������̻߳���

  gl_ThreadStatus.SetSavingTempData(true);

  gl_pChinaStockMarket->UpdateTodayTempDB();

  ASSERT(!gl_ThreadStatus.IsCalculatingRTData()); // �ٴ�ȷ��һ��
  gl_ThreadStatus.SetSavingTempData(false);

  return 13;
}