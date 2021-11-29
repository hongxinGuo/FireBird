///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网络数据查询器。所有的网络查询皆位于此处。
// 只有头文件。
// WebData意指从网络上得到的原始数据。
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"globedef.h"
#include"CrweberIndex.h"

extern int gl_iMaxSavingOneDayLineThreads;

#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"CrweberIndexWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"FinnhubWebInquiry.h"
#include"TiingoWebInquiry.h"
#include"QuandlWebInquiry.h"

#include"QueueString.h"
#include"QueueDownLoadedNeteaseDayLine.h"

extern CCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry; // crweber.com上的每日油运指数
extern CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry; // 腾讯实时数据采集
extern CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry; // 网易实时数据采集
extern CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry; // 新浪实时数据采集
extern CPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry; // Poten.com上的油运数据。
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry; // 网易日线历史数据
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2; // 网易日线历史数据
extern CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry; // Finnhub.io证券信息
extern CQuandlWebInquiryPtr gl_pQuandlWebInquiry;
extern CTiingoWebInquiryPtr gl_pTiingoWebInquiry;

class CWebInquirer : public CObject {
public:
	CWebInquirer(void);
	virtual ~CWebInquirer(void);

	void Initialize(void);
public:
	// 网络数据查询器
	bool GetQuandlData(void) { return gl_pQuandlWebInquiry->GetWebData(); }
	bool GetFinnhubData(void) { return gl_pFinnhubWebInquiry->GetWebData(); }
	bool GetSinaRTData(void) { return gl_pSinaRTWebInquiry->GetWebData(); }
	bool GetTengxunRTData(void) { return gl_pTengxunRTWebInquiry->GetWebData(); }
	bool GetNeteaseRTData(void) { return gl_pNeteaseRTWebInquiry->GetWebData(); }
	bool GetNeteaseDayLineData(void);
	bool GetCrweberIndexData(void) { return gl_pCrweberIndexWebInquiry->GetWebData(); }
	bool IsReadingCrweberIndex(void) noexcept { return gl_pCrweberIndexWebInquiry->IsReadingWebData(); }
	bool GetPotenDailyBriefingData(void) { return gl_pPotenDailyBriefingWebInquiry->GetWebData(); }
	bool IsReadingPotenDailyBriefing(void)  noexcept { return gl_pPotenDailyBriefingWebInquiry->IsReadingWebData(); }

public:
	// 新浪实时股票数据。从网站上查询到的原始数据，每个数据都是存储的网站提供的原始数据。下同
	size_t GetSinaRTDataSize(void) { return m_qSinaRTWebData.GetDataSize(); }
	void PushSinaRTData(CWebDataPtr pData) { m_qSinaRTWebData.PushData(pData); }
	CWebDataPtr PopSinaRTData(void) { return m_qSinaRTWebData.PopData(); }

	// 腾讯实时股票数据
	size_t GetTengxunRTDataSize(void) { return m_qTengxunRTWebData.GetDataSize(); }
	void PushTengxunRTData(CWebDataPtr pData) { m_qTengxunRTWebData.PushData(pData); }
	CWebDataPtr PopTengxunRTData(void) { return m_qTengxunRTWebData.PopData(); }

	// 网易实时股票数据
	size_t GetNeteaseRTDataSize(void) { return m_qNeteaseRTWebData.GetDataSize(); }
	void PushNeteaseRTData(CWebDataPtr pData) { m_qNeteaseRTWebData.PushData(pData); }
	CWebDataPtr PopNeteaseRTData(void) { return m_qNeteaseRTWebData.PopData(); }

	// Crweber.com航运数据（目前已失效）
	size_t GetCrweberDataSize(void) { return m_qCrweberWebData.GetDataSize(); }
	void PushCrweberData(CWebDataPtr pData) { m_qCrweberWebData.PushData(pData); }
	CWebDataPtr PopCrweberData(void) { return m_qCrweberWebData.PopData(); }

	// Poten Daily Briefing航运数据（目前已失效）
	size_t GetPotenDailyBriefingDataSize(void) { return m_qPotenDailyBriefingWebData.GetDataSize(); }
	void PushPotenDailyBriefingData(CWebDataPtr pData) { m_qPotenDailyBriefingWebData.PushData(pData); }
	CWebDataPtr PopPotenDailyBriefingData(void) { return m_qPotenDailyBriefingWebData.PopData(); }

	// Finnhub股票数据
	size_t GetFinnhubDataSize(void) { return m_qFinnhubData.GetDataSize(); }
	void PushFinnhubData(CWebDataPtr pData) { m_qFinnhubData.PushData(pData); }
	CWebDataPtr PopFinnhubData(void) { return m_qFinnhubData.PopData(); }

	// Quandl股票数据
	size_t GetQuandlDataSize(void) { return m_qQuandlData.GetDataSize(); }
	void PushQuandlData(CWebDataPtr pData) { m_qQuandlData.PushData(pData); }
	CWebDataPtr PopQuandlData(void) { return m_qQuandlData.PopData(); }

	// Tiingo股票数据
	size_t GetTiingoDataSize(void) { return m_qTiingoData.GetDataSize(); }
	void PushTiingoData(CWebDataPtr pData) { m_qTiingoData.PushData(pData); }
	CWebDataPtr PopTiingoData(void) { return m_qTiingoData.PopData(); }

	// 网易日线数据
	size_t GetNeteaseDayLineDataSize(void) { return(m_qNeteaseDayLine.GetDataSize()); }
	void PushNeteaseDayLineData(CWebDataPtr pData) { m_qNeteaseDayLine.PushData(pData); }
	CWebDataPtr PopNeteaseDayLineData(void) { return m_qNeteaseDayLine.PopData(); }

	// Finnhub股票推送数据
	size_t GetFinnhubWebSocketDataSize(void) { return m_qFinnhubWebSocketData.GetDataSize(); }
	void pushFinnhubWebSocketData(string data) { m_qFinnhubWebSocketData.PushData(data); }
	void PushFinnhubWebSocketData(shared_ptr<string> pData) { m_qFinnhubWebSocketData.PushData(pData); }
	shared_ptr<string> PopFinnhubWebSocketData(void) { return m_qFinnhubWebSocketData.PopData(); }

	// Tiingo IEX股票推送数据
	size_t GetTiingoIEXWebSocketDataSize(void) { return m_qTiingoIEXWebSocketData.GetDataSize(); }
	void PushTiingoIEXWebSocketData(string data) { m_qTiingoIEXWebSocketData.PushData(data); }
	void PushTiingoIEXWebSocketData(shared_ptr<string> pData) { m_qTiingoIEXWebSocketData.PushData(pData); }
	shared_ptr<string> PopTiingoIEXWebSocketData(void) { return m_qTiingoIEXWebSocketData.PopData(); }

	// Tiingo Crypto推送数据
	size_t GetTiingoCryptoWebSocketDataSize(void) { return m_qTiingoCryptoWebSocketData.GetDataSize(); }
	void PushTiingoCryptoWebSocketData(string data) { m_qTiingoCryptoWebSocketData.PushData(data); }
	void PushTiingoCryptoWebSocketData(shared_ptr<string> pData) { m_qTiingoCryptoWebSocketData.PushData(pData); }
	shared_ptr<string> PopTiingoCryptoWebSocketData(void) { return m_qTiingoCryptoWebSocketData.PopData(); }

	// Tiingo forex推送数据
	size_t GetTiingoForexWebSocketDataSize(void) { return m_qTiingoForexWebSocketData.GetDataSize(); }
	void PushTiingoForexWebSocketData(string data) { m_qTiingoForexWebSocketData.PushData(data); }
	void PushTiingoForexWebSocketData(shared_ptr<string> pData) { m_qTiingoForexWebSocketData.PushData(pData); }
	shared_ptr<string> PopTiingoForexWebSocketData(void) { return m_qTiingoForexWebSocketData.PopData(); }

	// Finnhub WebSocket缓存数据
	size_t GetFinnhubSocketSize(void) { return m_qFinnhubSocket.size(); }
	void PushFinnhubSocket(CFinnhubSocketPtr p) { m_qFinnhubSocket.push(p); }
	CFinnhubSocketPtr PopFinnhubSocket(void) { CFinnhubSocketPtr p = m_qFinnhubSocket.front(); m_qFinnhubSocket.pop(); return p; }

protected:
	CQueueWebData m_qSinaRTWebData; // 新浪股票网络数据暂存队列
	CQueueWebData m_qTengxunRTWebData; // 腾讯网络数据暂存队列
	CQueueWebData m_qNeteaseRTWebData; // 网易网络数据暂存队列
	CQueueWebData m_qCrweberWebData; // crweber.com网络数据暂存队列
	CQueueWebData m_qPotenDailyBriefingWebData; // Poten.com网络数据暂存队列
	CQueueWebData m_qFinnhubData; // Finnhub.io网络数据暂存队列
	CQueueWebData m_qQuandlData; // Quandl.com网络数据暂存队列
	CQueueWebData m_qTiingoData; // Tiingo.com网络数据暂存队列

	CQueueWebData m_qNeteaseDayLine; // 网易日线数据暂存队列

	CQueueString m_qFinnhubWebSocketData; // finnhub的WebSocket数据
	CQueueString m_qTiingoIEXWebSocketData; // tiingo的WebSocket数据
	CQueueString m_qTiingoCryptoWebSocketData; // tiingo的WebSocket数据
	CQueueString m_qTiingoForexWebSocketData; // tiingo的WebSocket数据

	queue<CFinnhubSocketPtr> m_qFinnhubSocket;
	queue<CTiingoCryptoSocketPtr> m_qTiingoCryptoSocket;
	queue<CTiingoForexSocketPtr> m_qTiingoForexSocket;
	queue<CTiingoIEXSocketPtr> m_qTiingoIEXSocket;

};

extern CWebInquirer gl_WebInquirer; //
