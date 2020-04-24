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
  pMarket->SetUpdatedDayFor10DayRS2(pMarket->GetDay());
  pMarket->SetUpdateOptionDB(true); // ����ѡ�����ݿ�
  gl_ChoiceRSStrong.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 23;
}

UINT ThreadChoice10RSStrong1StockSet(CChinaMarket* pMarket) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_ChoiceRSStrong.Wait();
  gl_systemMessage.PushInformationMessage(_T("��ʼ����10��RS1\n"));

  // ���һ��ע��
  pMarket->Choice10RSStrong1StockSet();

  gl_systemMessage.PushInformationMessage(_T("10��RS1�������\n"));
  pMarket->SetUpdatedDayFor10DayRS1(pMarket->GetDay());
  pMarket->SetUpdateOptionDB(true); // ����ѡ�����ݿ�
  gl_ChoiceRSStrong.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 24;
}