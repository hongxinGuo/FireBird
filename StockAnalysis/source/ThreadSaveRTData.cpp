//////////////////////////////////////////////////////////////////////////////////////
//
// �洢��ǰ��ѡ��Ʊ��ʵʱ���ݵĹ����߳�
//
//
// ����ֵΪ19
//
///////////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"

#include"ChinaMarket.h"

UINT ThreadSaveRTData(CChinaMarket* pMarket) {
  ASSERT(pMarket->IsSystemReady()); // ���ñ������߳�ʱ�������ú��г���
  gl_ThreadStatus.IncreaseNumberOfRunningThread();
  pMarket->SaveRTData();
  gl_ThreadStatus.DecreaseNumberOfRunningThread();
  return 19;
}