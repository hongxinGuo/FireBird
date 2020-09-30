#include"globedef.h"

#include"ChinaMarket.h"
#include"Thread.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����װ�ع����߳�
//
// �����ݿ���װ����Ӧ��Ʊ���������ݣ�Ȼ���������ǿ��
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadLoadDL(CChinaStockPtr pStock) {
  ASSERT(pStock != nullptr);

  gl_ThreadStatus.IncreaseRunningThread();
  pStock->UnloadDL();
  // װ����������
  pStock->LoadDL(pStock->GetStockCode());
  // ��������ǿ�ȣ���ָ�����ǿ��ΪĬ��ֵ��
  pStock->CalculateDLRSIndex();
  pStock->SetDLLoaded(true);
  gl_ThreadStatus.DecreaseRunningThread();

  return 16;
}