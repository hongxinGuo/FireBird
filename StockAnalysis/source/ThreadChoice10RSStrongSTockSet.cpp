///////////////////////////////////////////////////////////////////////////////////
//
// �ӻ�Ծ��Ʊ������ѡǿ�ƹ�Ʊ��
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"
#include"Thread.h"

#include"ChinaMarket.h"

Semaphore gl_ChoiceRSStrong(1); //���ڸ���ѡ���Ʊ���Ĺ����̶߳�����������ʷ���ݣ��ʶ�ͬһʱ��ֻ����һ���߳�ִ�У������ܹ�����ͬ�����⡣

UINT ThreadChoice10RSStrong2StockSet(CChinaMarket* pMarket) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_ChoiceRSStrong.Wait();
  gl_systemMessage.PushInformationMessage(_T("��ʼ����10��RS2\n"));

  // ���һ��ע��
  pMarket->Choice10RSStrong2StockSet();

  gl_systemMessage.PushInformationMessage(_T("10��RS2�������\n"));
  pMarket->SetUpdatedDayFor10DayRS2(pMarket->GetFormatedMarketDay());
  pMarket->SetUpdateOptionDB(true); // ����ѡ�����ݿ�
  gl_ChoiceRSStrong.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 32;
}

UINT ThreadChoice10RSStrong1StockSet(CChinaMarket* pMarket) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_ChoiceRSStrong.Wait();
  gl_systemMessage.PushInformationMessage(_T("��ʼ����10��RS1\n"));

  // ���һ��ע��
  pMarket->Choice10RSStrong1StockSet();

  gl_systemMessage.PushInformationMessage(_T("10��RS1�������\n"));
  pMarket->SetUpdatedDayFor10DayRS1(pMarket->GetFormatedMarketDay());
  pMarket->SetUpdateOptionDB(true); // ����ѡ�����ݿ�
  gl_ChoiceRSStrong.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 31;
}

UINT ThreadChoice10RSStrongStockSet(CChinaMarket* pMarket, CRSReference* pRef, int iIndex) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_ChoiceRSStrong.Wait();
  CString str = _T("��ʼ����10��RS ");
  char buffer[30];
  sprintf_s(buffer, _T("%d"), iIndex);
  str += buffer;
  str += _T("\n");
  gl_systemMessage.PushInformationMessage(str);

  // ���һ��ע��
  pMarket->Choice10RSStrongStockSet(pRef, iIndex);

  str = _T("10��RS ");
  str += buffer;
  str += _T("�������\n");
  gl_systemMessage.PushInformationMessage(str);
  //pMarket->SetUpdatedDayFor10DayRS1(pMarket->GetFormatedMarketDay());
  //pMarket->SetUpdateOptionDB(true); // ����ѡ�����ݿ�
  gl_ChoiceRSStrong.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 33;
}