//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ÀàÉùÃ÷¡£
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include<memory>
using std::shared_ptr;

class CVirtualMarket;
typedef shared_ptr<CVirtualMarket> CVirtualMarketPtr;

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
class CInsiderSentiment;
typedef shared_ptr<CInsiderSentiment> CInsiderSentimentPtr;
class CEPSSurprise;
typedef shared_ptr<CEPSSurprise> CEPSSurprisePtr;
class CFinnhubCompanyNews;
typedef shared_ptr<CFinnhubCompanyNews> CCompanyNewsPtr;

class CSystemMessage;
class CThreadStatus;
class CWebRTDataContainer;

// Web Inquiry
class CVirtualWebInquiry;
class CNeteaseRTWebInquiry;
class CSinaRTWebInquiry;
class CTengxunRTWebInquiry;
class CNeteaseDayLineWebInquiry;

// Web data source
class CVirtualDataSource;
typedef shared_ptr<CVirtualDataSource> CVirtualDataSourcePtr;

// Product
class CVirtualWebProduct;
typedef shared_ptr<CVirtualWebProduct> CVirtualProductWebDataPtr;

// Web data
class CWebData;
typedef shared_ptr<CWebData> CWebDataPtr;

class CDayLineWebData;

// Web RT Data
class CWebRTData;
typedef shared_ptr<CWebRTData> CWebRTDataPtr;

class CFinnhubForexSymbol;
class CFinnhubCryptoSymbol;
