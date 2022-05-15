////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 回调函数集，用于需要谓词函数的函数中。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

//#include"ChinaStock.h"
//#include"WorldStock.h"
//#include"Country.h"

class CChinaStock;
typedef shared_ptr<CChinaStock> CChinaStockPtr;
class CWorldStock;
typedef shared_ptr<CWorldStock> CWorldStockPtr;
class CCountry;
typedef shared_ptr<CCountry> CCountryPtr;
class CDayLine;
typedef shared_ptr<CDayLine> CDayLinePtr;
class CInsiderTransaction;
typedef shared_ptr<CInsiderTransaction> CInsiderTransactionPtr;
class CEPSSurprise;
typedef shared_ptr<CEPSSurprise> CEPSSurprisePtr;

bool CompareEPSSurprise(CEPSSurprisePtr& p1, CEPSSurprisePtr& p2);
bool CompareDayLineDate(CDayLinePtr& p1, CDayLinePtr& p2);
bool CompareCountryList(CCountryPtr& p1, CCountryPtr& p2);
bool CompareInsiderTransaction(CInsiderTransactionPtr& p1, CInsiderTransactionPtr& p2);
bool CompareChinaStock(CChinaStockPtr p1, CChinaStockPtr p2);
bool CompareWorldStock(CWorldStockPtr p1, CWorldStockPtr p2);
