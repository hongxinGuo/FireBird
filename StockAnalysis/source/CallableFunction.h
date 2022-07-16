////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 回调函数集，用于需要谓词函数的函数中。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ClassDeclaration.h"

bool CompareEPSSurprise(CEPSSurprisePtr& p1, CEPSSurprisePtr& p2);
bool CompareDayLineDate(CDayLinePtr& p1, CDayLinePtr& p2);
bool CompareCountryList(CCountryPtr& p1, CCountryPtr& p2);
bool CompareInsiderTransaction(CInsiderTransactionPtr& p1, CInsiderTransactionPtr& p2);
bool CompareInsiderSentiment(CInsiderSentimentPtr& p1, CInsiderSentimentPtr& p2);
bool CompareChinaStock(CChinaStockPtr p1, CChinaStockPtr p2);
bool CompareWorldStock(CWorldStockPtr p1, CWorldStockPtr p2);
