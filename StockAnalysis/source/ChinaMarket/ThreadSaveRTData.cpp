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

UINT ThreadSaveRTData(not_null<CChinaMarket*> pMarket) {
  ASSERT(pMarket->IsSystemReady()); // ���ñ������߳�ʱ�������ú��г���
  gl_ThreadStatus.IncreaseSavingThread();
  pMarket->SaveRTData();
  gl_ThreadStatus.DecreaseSavingThread();

  return 19;
}