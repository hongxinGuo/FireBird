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
UINT ThreadLoadDayLine(CChinaStockPtr pStock) {
  ASSERT(pStock != nullptr);
  ASSERT(!pStock->IsDayLineLoaded());

  pStock->ClearDayLineContainer();
  // װ����������
  pStock->LoadDayLineAndDayLineInfo();
  // ��������ǿ��
  pStock->CalculateDayLineRS();
  pStock->SetDayLineLoaded(true);

  return 16;
}