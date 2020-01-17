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
UINT ThreadLoadDayLine(LPVOID) {
  CStockPtr pCurrentStock = gl_ChinaStockMarket.GetCurrentStockPtr();

  ASSERT(pCurrentStock != nullptr);
  ASSERT(!pCurrentStock->IsDayLineLoaded());

  // װ����������
  pCurrentStock->LoadDayLineAndDayLineInfo();
  // ��������ǿ��
  pCurrentStock->CalculateDayLineRS();
  pCurrentStock->SetDayLineLoaded(true);

  return 16;
}