///////////////////////////////////////////////////////////////////////////////////
//
// �ӻ�Ծ��Ʊ������ѡǿ�ƹ�Ʊ��
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ThreadStatus.h"
#include"Thread.h"

#include"ChinaMarket.h"

binary_semaphore gl_ChoiceRSStrong{1}; //���ڸ���ѡ���Ʊ���Ĺ����̶߳�����������ʷ���ݣ��ʶ�ͬһʱ��ֻ����һ���߳�ִ�У������ܹ�����ͬ�����⡣

UINT ThreadChoice10RSStrong2StockSet(not_null<CChinaMarket*> pMarket) {
	gl_ChoiceRSStrong.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	gl_systemMessage.PushInformationMessage(_T("��ʼ����10��RS2\n"));

	// ����һ��ע��
	if (pMarket->Choice10RSStrong2StockSet()) {
		gl_systemMessage.PushInformationMessage(_T("10��RS2�������\n"));
		pMarket->SetUpdatedDateFor10DaysRS2(pMarket->GetMarketDate());
		pMarket->SetUpdateOptionDB(true); // ����ѡ�����ݿ�
	}
	gl_ThreadStatus.DecreaseSavingThread();
	gl_ChoiceRSStrong.release();

	return 102;
}

UINT ThreadChoice10RSStrong1StockSet(not_null<CChinaMarket*> pMarket) {
	gl_ChoiceRSStrong.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	gl_systemMessage.PushInformationMessage(_T("��ʼ����10��RS1\n"));

	// ����һ��ע��
	if (pMarket->Choice10RSStrong1StockSet()) {
		gl_systemMessage.PushInformationMessage(_T("10��RS1�������\n"));
		pMarket->SetUpdatedDateFor10DaysRS1(pMarket->GetMarketDate());
		pMarket->SetUpdateOptionDB(true); // ����ѡ�����ݿ�
	}
	gl_ThreadStatus.DecreaseSavingThread();
	gl_ChoiceRSStrong.release();

	return 101;
}

UINT ThreadChoice10RSStrongStockSet(not_null<CChinaMarket*> pMarket, CRSReference* pRef, int iIndex) {
	gl_ChoiceRSStrong.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	CString str = _T("��ʼ����10��RS ");
	char buffer[30];
	sprintf_s(buffer, _T("%d"), iIndex);
	str += buffer;
	str += _T("\n");
	gl_systemMessage.PushInformationMessage(str);

	// ����һ��ע��
	if (pMarket->Choice10RSStrongStockSet(pRef, iIndex)) {
		str = _T("10��RS ");
		str += buffer;
		str += _T("�������\n");
		gl_systemMessage.PushInformationMessage(str);
		//pMarket->SetUpdatedDateFor10DAyRS1(pMarket->GetMarketDate());
		//pMarket->SetUpdateOptionDB(true); // ����ѡ�����ݿ�
	}
	gl_ThreadStatus.DecreaseSavingThread();
	gl_ChoiceRSStrong.release();

	return 103;
}

UINT ThreadCalculate10RSStrongStock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, CRSReference* pRef, not_null<CChinaStockPtr> pStock) {
	gl_BackGroundTaskThread.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	gl_ThreadStatus.IncreaseBackGroundWorkingThread(); // ���ڹ������߳�����һ
	if (!gl_systemStatus.IsExitingSystem()) {
		if (gl_pChinaMarket->IsAStock(pStock) && pStock->IsActive()) {
			if (!pStock->IsDayLineLoaded()) {
				pStock->LoadDayLine(pStock->GetSymbol());
				pStock->SetDayLineLoaded(true);
			}
			if (pStock->Calculate10RSStrongStockSet(pRef)) {
				pv10RSStrongStock->push_back(pStock);
			}
			if (!pStock->IsSameStock(gl_pChinaMarket->GetCurrentStock())) {
				pStock->UnloadDayLine();
				pStock->SetDayLineLoaded(false);
			}
		}
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingThread(); // ���ڹ������߳�����һ
	gl_ThreadStatus.DecreaseSavingThread();
	gl_BackGroundTaskThread.release();
	return 104;
}

UINT ThreadCalculate10RSStrong1Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, not_null<CChinaStockPtr> pStock) {
	gl_BackGroundTaskThread.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	gl_ThreadStatus.IncreaseBackGroundWorkingThread(); // ���ڹ������߳�����һ
	if (!gl_systemStatus.IsExitingSystem()) {
		if (gl_pChinaMarket->IsAStock(pStock) && pStock->IsActive()) {
			if (!pStock->IsDayLineLoaded()) {
				pStock->LoadDayLine(pStock->GetSymbol());
				pStock->SetDayLineLoaded(true);
			}
			if (pStock->Calculate10RSStrong1StockSet()) {
				pv10RSStrongStock->push_back(pStock);
			}
			if (!pStock->IsSameStock(gl_pChinaMarket->GetCurrentStock())) {
				pStock->UnloadDayLine();
				pStock->SetDayLineLoaded(false);
			}
		}
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingThread(); // ���ڹ������߳�����һ
	gl_ThreadStatus.DecreaseSavingThread();
	gl_BackGroundTaskThread.release();
	return 105;
}

UINT ThreadCalculate10RSStrong2Stock(not_null<vector<CChinaStockPtr>*> pv10RSStrongStock, not_null<CChinaStockPtr> pStock) {
	gl_BackGroundTaskThread.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	gl_ThreadStatus.IncreaseBackGroundWorkingThread(); // ���ڹ������߳�����һ
	if (!gl_systemStatus.IsExitingSystem()) {
		if (gl_pChinaMarket->IsAStock(pStock) && pStock->IsActive()) {
			if (!pStock->IsDayLineLoaded()) {
				pStock->LoadDayLine(pStock->GetSymbol());
				pStock->SetDayLineLoaded(true);
			}
			if (pStock->Calculate10RSStrong2StockSet()) {
				pv10RSStrongStock->push_back(pStock);
			}
			if (!pStock->IsSameStock(gl_pChinaMarket->GetCurrentStock())) {
				pStock->UnloadDayLine();
				pStock->SetDayLineLoaded(false);
			}
		}
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingThread(); // ���ڹ������߳�����һ
	gl_ThreadStatus.DecreaseSavingThread();
	gl_BackGroundTaskThread.release();
	return 106;
}