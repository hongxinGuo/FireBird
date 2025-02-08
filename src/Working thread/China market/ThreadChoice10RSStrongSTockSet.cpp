///////////////////////////////////////////////////////////////////////////////////
//
// 从活跃股票集中挑选强势股票。
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"Thread.h"

#include"ChinaMarket.h"

UINT ThreadChoice10RSStrong2StockSet(const CChinaMarketPtr& pMarket) {
	TRACE("choice10RS2\n");
	gl_UpdateChinaMarketDB.acquire();
	gl_systemMessage.PushInformationMessage(_T("开始计算10日RS2\n"));// 添加一个注释
	if (gl_dataContainerChinaStock.Choice10RSStrong2StockSet()) {
		gl_systemMessage.PushInformationMessage(_T("10日RS2计算完毕\n"));
		pMarket->SetUpdatedDateFor10DaysRS2(pMarket->GetMarketDate());
		pMarket->SetUpdateOptionDB(true); // 更新选项数据库
	}
	gl_UpdateChinaMarketDB.release();

	return 102;
}

UINT ThreadChoice10RSStrong1StockSet(const CChinaMarketPtr& pMarket) {
	TRACE("choice10RS1\n");
	gl_UpdateChinaMarketDB.acquire();
	gl_systemMessage.PushInformationMessage(_T("开始计算10日RS1\n"));// 添加一个注释
	if (gl_dataContainerChinaStock.Choice10RSStrong1StockSet()) {
		gl_systemMessage.PushInformationMessage(_T("10日RS1计算完毕\n"));
		pMarket->SetUpdatedDateFor10DaysRS1(pMarket->GetMarketDate());
		pMarket->SetUpdateOptionDB(true); // 更新选项数据库
	}
	gl_UpdateChinaMarketDB.release();

	return 101;
}

UINT ThreadChoice10RSStrongStockSet(CRSReference* pRef, int iIndex) {
	TRACE("choice10RS\n");
	gl_UpdateChinaMarketDB.acquire();
	string s = fmt::format("开始计算10日RS {:d}\n", iIndex);
	gl_systemMessage.PushInformationMessage(s.c_str());

	// 报告一下
	if (gl_dataContainerChinaStock.Choice10RSStrongStockSet(pRef, iIndex)) {
		s = fmt::format("10日RS  {:d}计算完毕\n", iIndex);
		gl_systemMessage.PushInformationMessage(s.c_str());
	}
	gl_UpdateChinaMarketDB.release();

	return 103;
}

UINT ThreadCalculate10RSStrongStock(vector<CChinaStockPtr>* pv10RSStrongStock, const CRSReference* pRef, const CChinaStockPtr& pStock) {
	TRACE("calculate10RS\n");
	gl_UpdateChinaMarketDB.acquire();
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
	gl_UpdateChinaMarketDB.release();
	return 104;
}

UINT ThreadCalculate10RSStrong1Stock(vector<CChinaStockPtr>* pv10RSStrongStock, const CChinaStockPtr& pStock) {
	TRACE("calculate10RS1\n");
	gl_UpdateChinaMarketDB.acquire();
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
	gl_UpdateChinaMarketDB.release();
	return 105;
}

UINT ThreadCalculate10RSStrong2Stock(vector<CChinaStockPtr>* pv10RSStrongStock, const CChinaStockPtr& pStock) {
	TRACE("calculate10RS2\n");
	gl_UpdateChinaMarketDB.acquire();
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
	gl_UpdateChinaMarketDB.release();
	return 106;
}
