#include"globedef.h"

#include"DayLine.h"
#include"Market.h"

#include"Thread.h"

using namespace std;
#include<memory>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���߳���ϵͳ�����к���15:05�Զ����ѣ�ÿ��ִֻ��һ��
//
//
//���ڴ������������գ��ʶ���ǰ�����ղ���һ�����ǵ�ǰ���ڣ�������ʱ����
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadProcessCurrentTradeDayStock(LPVOID) {
  ASSERT(gl_ChinaStockMarket.SystemReady()); // ���ñ������߳�ʱ�������ú��г���

  int i;
  long lDay = FormatToDay(gl_ChinaStockMarket.GetNewestTransactionTime());
  i = gl_ChinaStockMarket.ProcessCurrentTradeDayStock(lDay);
  char buffer[30];
  sprintf_s(buffer, "%d", i);
  CString str;
  str = _T("���մ�����");
  str += buffer;
  str += _T("����Ʊ");
  gl_systemMessage.PushInformationMessage(str);
  gl_ChinaStockMarket.CalculateOneDayRelativeStrong(lDay);
  if (gl_systemTime.GetTime() > 150300) {   // ����й����б�����
    gl_ChinaStockMarket.UpdateStockCodeDB();  // ���´��롣
    gl_ChinaStockMarket.UpdateOptionDB();   // ����״̬
    gl_ChinaStockMarket.SetTodayStockProcessedFlag(true);  // ���ý����Ѵ����ʶ
  }

  return 14;
}