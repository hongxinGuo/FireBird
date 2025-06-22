///////////////////////////////////////////////////////////////////////////////////
//
// �ӻ�Ծ��Ʊ������ѡǿ�ƹ�Ʊ��
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"Thread.h"

#include"ChinaMarket.h"

UINT ThreadChoice10RSStrong2StockSet(const CChinaMarketPtr& pMarket) {
	TRACE("choice10RS2\n");
	gl_UpdateChinaMarketDB.acquire();
	gl_systemMessage.PushInformationMessage(_T("��ʼ����10��RS2\n"));// ���һ��ע��
	if (gl_dataContainerChinaStock.Choice10RSStrong2StockSet()) {
		gl_systemMessage.PushInformationMessage(_T("10��RS2�������\n"));
		pMarket->SetUpdatedDateFor10DaysRS2(pMarket->GetMarketDate());
		pMarket->SetUpdateOptionDB(true); // ����ѡ�����ݿ�
	}
	gl_UpdateChinaMarketDB.release();

	return 102;
}

UINT ThreadChoice10RSStrong1StockSet(const CChinaMarketPtr& pMarket) {
	gl_UpdateChinaMarketDB.acquire();
	TRACE("choice10RS1\n");
	gl_systemMessage.PushInformationMessage(_T("��ʼ����10��RS1\n"));// ���һ��ע��
	if (gl_dataContainerChinaStock.Choice10RSStrong1StockSet()) {
		gl_systemMessage.PushInformationMessage(_T("10��RS1�������\n"));
		pMarket->SetUpdatedDateFor10DaysRS1(pMarket->GetMarketDate());
		pMarket->SetUpdateOptionDB(true); // ����ѡ�����ݿ�
	}
	TRACE("choice10RS1\n");
	gl_UpdateChinaMarketDB.release();

	return 101;
}

UINT ThreadChoice10RSStrongStockSet(CRSReference* pRef, int iIndex) {
	gl_UpdateChinaMarketDB.acquire();
	TRACE("choice10RS\n");
	string s = fmt::format("��ʼ����10��RS {:d}\n", iIndex);
	gl_systemMessage.PushInformationMessage(s);

	// ����һ��
	if (gl_dataContainerChinaStock.Choice10RSStrongStockSet(pRef, iIndex)) {
		s = fmt::format("10��RS  {:d}�������\n", iIndex);
		gl_systemMessage.PushInformationMessage(s);
	}
	TRACE("choice10RS\n");
	gl_UpdateChinaMarketDB.release();

	return 103;
}

UINT ThreadCalculate10RSStrongStock(vector<CChinaStockPtr>* pv10RSStrongStock, const CRSReference* pRef, const CChinaStockPtr& pStock) {
	gl_UpdateChinaMarketDB.acquire();
	TRACE("calculate10RS\n");
	if (!gl_systemConfiguration.IsExitingSystem()) {
		if (pStock->IsShareA() && pStock->IsActive()) {
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
	TRACE("calculated 10RS\n");
	gl_UpdateChinaMarketDB.release();
	return 104;
}

UINT ThreadCalculate10RSStrong1Stock(vector<CChinaStockPtr>* pv10RSStrongStock, const CChinaStockPtr& pStock) {
	gl_UpdateChinaMarketDB.acquire();
	TRACE("calculate10RS1\n");
	if (!gl_systemConfiguration.IsExitingSystem()) {
		if (pStock->IsShareA() && pStock->IsActive()) {
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
	TRACE("calculated 10RS1\n");
	gl_UpdateChinaMarketDB.release();
	return 105;
}

UINT ThreadCalculate10RSStrong2Stock(vector<CChinaStockPtr>* pv10RSStrongStock, const CChinaStockPtr& pStock) {
	gl_UpdateChinaMarketDB.acquire();
	TRACE("calculate10RS2\n");
	if (!gl_systemConfiguration.IsExitingSystem()) {
		if (pStock->IsShareA() && pStock->IsActive()) {
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
	TRACE("calculated 10RS2\n");
	gl_UpdateChinaMarketDB.release();
	return 106;
}
