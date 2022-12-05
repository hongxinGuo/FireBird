////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 回调函数集，用于需要谓词的函数中。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "CallableFunction.h"

#include"EPSSurprise.h"
#include"DayLine.h"
#include"Country.h"
#include"InsiderTransaction.h"
#include"InsiderSentiment.h"
#include"FinnhubCompanyNews.h"

#include"ChinaStock.h"
#include"WorldStock.h"

bool CompareEPSSurprise(CEPSSurprisePtr& p1, CEPSSurprisePtr& p2) { return (p1->m_lDate < p2->m_lDate); }
bool CompareDayLineDate(CDayLinePtr& p1, CDayLinePtr& p2) { return p1->GetMarketDate() < p2->GetMarketDate(); }
bool CompareCountryList(CCountryPtr& p1, CCountryPtr& p2) { return p1->m_strCountry < p2->m_strCountry; }
bool CompareInsiderTransaction(CInsiderTransactionPtr& p1, CInsiderTransactionPtr& p2) { return p1->m_lTransactionDate < p2->m_lTransactionDate; }
bool CompareInsiderSentiment(CInsiderSentimentPtr& p1, CInsiderSentimentPtr& p2) { return p1->m_lDate < p2->m_lDate; }
bool CompareFinnhubCompanyNews(CCompanyNewsPtr& p1, CCompanyNewsPtr& p2) { return (p1->m_llDateTime < p2->m_llDateTime); }

bool CompareChinaStock(CChinaStockPtr p1, CChinaStockPtr p2) { return (p1->GetSymbol().Compare(p2->GetSymbol()) < 0); }
bool CompareWorldStock(CWorldStockPtr p1, CWorldStockPtr p2) { return (p1->GetSymbol().Compare(p2->GetSymbol()) < 0); }