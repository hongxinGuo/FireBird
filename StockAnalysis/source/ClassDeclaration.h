//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ÀàÉùÃ÷¡£
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

using namespace std;
#include<memory>

// chinaMarket
class CChinaStock;
typedef shared_ptr<CChinaStock> CChinaStockPtr;
class CChinaMarket;
typedef shared_ptr<CChinaMarket> CChinaMarketPtr;

// WorldMarket
class CWorldStock;
typedef shared_ptr<CWorldStock> CWorldStockPtr;
class CWorldMarket;
typedef shared_ptr<CWorldMarket> CWorldMarketPtr;

class CCountry;
typedef shared_ptr<CCountry> CCountryPtr;
class CDayLine;
typedef shared_ptr<CDayLine> CDayLinePtr;
class CInsiderTransaction;
typedef shared_ptr<CInsiderTransaction> CInsiderTransactionPtr;
class CEPSSurprise;
typedef shared_ptr<CEPSSurprise> CEPSSurprisePtr;

class CSystemMessage;
class CThreadStatus;
class CWebRTDataContainer;

// Web Inquiry
class CVirtualWebInquiry;
class CNeteaseRTWebInquiry;
class CSinaRTWebInquiry;
class CTengxunRTWebInquiry;
class CNeteaseDayLineWebInquiry;
class CNeteaseDayLineWebData;