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
  if (pMarket->Choice10RSStrong2StockSet()) {
    gl_systemMessage.PushInformationMessage(_T("10��RS2�������\n"));
    pMarket->SetUpdatedDayFor10DayRS2(pMarket->GetFormatedMarketDay());
    pMarket->SetUpdateOptionDB(true); // ����ѡ�����ݿ�
  }
  gl_ChoiceRSStrong.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 102;
}

UINT ThreadChoice10RSStrong1StockSet(CChinaMarket* pMarket) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_ChoiceRSStrong.Wait();
  gl_systemMessage.PushInformationMessage(_T("��ʼ����10��RS1\n"));

  // ���һ��ע��
  if (pMarket->Choice10RSStrong1StockSet()) {
    gl_systemMessage.PushInformationMessage(_T("10��RS1�������\n"));
    pMarket->SetUpdatedDayFor10DayRS1(pMarket->GetFormatedMarketDay());
    pMarket->SetUpdateOptionDB(true); // ����ѡ�����ݿ�
  }
  gl_ChoiceRSStrong.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 101;
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
  if (pMarket->Choice10RSStrongStockSet(pRef, iIndex)) {
    str = _T("10��RS ");
    str += buffer;
    str += _T("�������\n");
    gl_systemMessage.PushInformationMessage(str);
    //pMarket->SetUpdatedDayFor10DayRS1(pMarket->GetFormatedMarketDay());
    //pMarket->SetUpdateOptionDB(true); // ����ѡ�����ݿ�
  }
  gl_ChoiceRSStrong.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 103;
}

UINT ThreadCalculate10RSStrongStock(vector<CChinaStockPtr>* pv10RSStrongStock, CRSReference* pRef, CChinaStockPtr pStock) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_SemaphoreBackGroundTaskThreads.Wait();
  gl_ThreadStatus.IncreaseBackGroundWorkingthreads();     // ���ڹ������߳�����һ
  if (!gl_fExitingSystem) {
    if (gl_pChinaStockMarket->IsAStock(pStock) && pStock->IsActive()) {
      if (!pStock->IsDayLineLoaded()) {
        pStock->LoadDayLine();
        pStock->SetDayLineLoaded(true);
      }
      if (pStock->Calculate10RSStrongStockSet(pRef)) {
        pv10RSStrongStock->push_back(pStock);
      }
      if (!pStock->IsSameStock(gl_pChinaStockMarket->GetCurrentStock())) {
        pStock->UnloadDayLine();
      }
    }
  }
  gl_ThreadStatus.DecreaseBackGroundWorkingthreads(); // ���ڹ������߳�����һ
  gl_SemaphoreBackGroundTaskThreads.Signal();
  gl_ThreadStatus.DecreaseRunningThread();
  return 104;
}

UINT ThreadCalculate10RSStrong1Stock(vector<CChinaStockPtr>* pv10RSStrongStock, CChinaStockPtr pStock) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_SemaphoreBackGroundTaskThreads.Wait();
  gl_ThreadStatus.IncreaseBackGroundWorkingthreads();     // ���ڹ������߳�����һ
  if (!gl_fExitingSystem) {
    if (gl_pChinaStockMarket->IsAStock(pStock) && pStock->IsActive()) {
      if (!pStock->IsDayLineLoaded()) {
        pStock->LoadDayLine();
        pStock->SetDayLineLoaded(true);
      }
      if (pStock->Calculate10RSStrong1StockSet()) {
        pv10RSStrongStock->push_back(pStock);
      }
      if (!pStock->IsSameStock(gl_pChinaStockMarket->GetCurrentStock())) {
        pStock->UnloadDayLine();
      }
    }
  }
  gl_ThreadStatus.DecreaseBackGroundWorkingthreads(); // ���ڹ������߳�����һ
  gl_SemaphoreBackGroundTaskThreads.Signal();
  gl_ThreadStatus.DecreaseRunningThread();
  return 105;
}

UINT ThreadCalculate10RSStrong2Stock(vector<CChinaStockPtr>* pv10RSStrongStock, CChinaStockPtr pStock) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_SemaphoreBackGroundTaskThreads.Wait();
  gl_ThreadStatus.IncreaseBackGroundWorkingthreads();     // ���ڹ������߳�����һ
  if (!gl_fExitingSystem) {
    if (gl_pChinaStockMarket->IsAStock(pStock) && pStock->IsActive()) {
      if (!pStock->IsDayLineLoaded()) {
        pStock->LoadDayLine();
        pStock->SetDayLineLoaded(true);
      }
      if (pStock->Calculate10RSStrong2StockSet()) {
        pv10RSStrongStock->push_back(pStock);
      }
      if (!pStock->IsSameStock(gl_pChinaStockMarket->GetCurrentStock())) {
        pStock->UnloadDayLine();
      }
    }
  }
  gl_ThreadStatus.DecreaseBackGroundWorkingthreads(); // ���ڹ������߳�����һ
  gl_SemaphoreBackGroundTaskThreads.Signal();
  gl_ThreadStatus.DecreaseRunningThread();
  return 106;
}