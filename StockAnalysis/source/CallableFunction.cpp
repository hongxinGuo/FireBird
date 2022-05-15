#include"pch.h"

#include"globedef.h"
#include "CallableFunction.h"

#include"EPSSurprise.h"
#include"DayLine.h"
#include"Country.h"
#include"InsiderTransaction.h"
#include"ChinaStock.h"
#include"WorldStock.h"

#include"WebInquirer.h"

bool CompareEPSSurprise(CEPSSurprisePtr& p1, CEPSSurprisePtr& p2) { return (p1->m_lDate < p2->m_lDate); }
bool CompareDayLineDate(CDayLinePtr& p1, CDayLinePtr& p2) { return p1->GetMarketDate() < p2->GetMarketDate(); }
bool CompareCountryList(CCountryPtr& p1, CCountryPtr& p2) { return p1->m_strCountry < p2->m_strCountry; }
bool CompareInsiderTransaction(CInsiderTransactionPtr& p1, CInsiderTransactionPtr& p2) { return p1->m_lTransactionDate < p2->m_lTransactionDate; }

bool CompareChinaStock(CChinaStockPtr p1, CChinaStockPtr p2) { return (p1->GetSymbol().Compare(p2->GetSymbol()) < 0); }
bool CompareWorldStock(CWorldStockPtr p1, CWorldStockPtr p2) { return (p1->GetSymbol().Compare(p2->GetSymbol()) < 0); }