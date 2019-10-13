#include"globedef.h"

#include"DayLine.h"
#include"SetDayLineInfo.h"
#include"Market.h"

#include"Thread.h"

using namespace std;
#include<memory>

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
  gl_ChinaStockMarket.m_pCurrentStock->LoadDayLine();

  // ��������ǿ��
  gl_ChinaStockMarket.m_pCurrentStock->CalculateDayLineRS();

  gl_ChinaStockMarket.m_pCurrentStock->SetDayLineLoaded(true);

  return 7;
}
