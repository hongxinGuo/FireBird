#include"globedef.h"

#include"Market.h"
#include"Thread.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����װ�ع����߳�
//
// �����ݿ���װ����Ӧ��Ʊ���������ݣ�Ȼ���������ǿ��
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadLoadDayLine(LPVOID) {
  ASSERT(gl_ChinaStockMarket.m_pCurrentStock != nullptr);
  ASSERT(!gl_ChinaStockMarket.m_pCurrentStock->IsDayLineLoaded());

  // װ����������
  gl_ChinaStockMarket.m_pCurrentStock->LoadDayLineAndDayLineInfo();
  // ��������ǿ��
  gl_ChinaStockMarket.m_pCurrentStock->CalculateDayLineRS();

  gl_ChinaStockMarket.m_pCurrentStock->SetDayLineLoaded(true);

  return 7;
}