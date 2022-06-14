///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网络数据查询器。所有的网络查询皆位于此处。
// 只有头文件。
// WebData意指从网络上得到的原始数据。
//
// 由于采用独立线程模式读取网络数据，故而需要保证线程安全。
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

extern int gl_iMaxSavingOneDayLineThreads;

#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"FinnhubWebInquiry.h"
#include"TiingoWebInquiry.h"
#include"QuandlWebInquiry.h"

#include"TemplateMutexAccessQueue.h"

#include"NeteaseDayLineWebData.h"

extern CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry; // 腾讯实时数据采集
extern CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry; // 网易实时数据采集
extern CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry; // 新浪实时数据采集
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry; // 网易日线历史数据
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2; // 网易日线历史数据
extern CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry; // Finnhub.io证券信息
extern CQuandlWebInquiryPtr gl_pQuandlWebInquiry;
extern CTiingoWebInquiryPtr gl_pTiingoWebInquiry;

class CWebInquirer : public CObject {
public:
	CWebInquirer(void);
	// 不允许赋值。
	CWebInquirer(const CWebInquirer&) = delete;
	CWebInquirer& operator=(const CWebInquirer&) = delete;
	CWebInquirer(const CWebInquirer&&) noexcept = delete;
	CWebInquirer& operator=(const CWebInquirer&&) noexcept = delete;
	virtual ~CWebInquirer(void);

	void Initialize(void);
public:
	// 网络数据查询器
	bool IsReadingSinaRTData(void) { return gl_pSinaRTWebInquiry->IsReadingWebData(); }
	bool IsReadingNeteaseRTData(void) { return gl_pNeteaseRTWebInquiry->IsReadingWebData(); }
	bool IsReadingTengxunRTData(void) { return gl_pTengxunRTWebInquiry->IsReadingWebData(); }

	bool GetQuandlData(void) { return gl_pQuandlWebInquiry->GetWebData(); }
	bool GetFinnhubData(void) { return gl_pFinnhubWebInquiry->GetWebData(); }
	bool GetSinaRTData(void) { return gl_pSinaRTWebInquiry->GetWebData(); }
	bool GetTengxunRTData(void) { return gl_pTengxunRTWebInquiry->GetWebData(); }
	bool GetNeteaseRTData(void) { return gl_pNeteaseRTWebInquiry->GetWebData(); }
	bool GetNeteaseDayLineData(void);

public:
	// 新浪实时股票数据。从网站上查询到的原始数据，每个数据都是存储的网站提供的原始数据。下同
	size_t SinaRTDataSize(void) { return m_qSinaRTWebData.Size(); }
	void PushSinaRTData(CWebDataPtr pData) { m_qSinaRTWebData.PushData(pData); }
	CWebDataPtr PopSinaRTData(void) { return m_qSinaRTWebData.PopData(); }

	// 腾讯实时股票数据
	size_t TengxunRTDataSize(void) { return m_qTengxunRTWebData.Size(); }
	void PushTengxunRTData(CWebDataPtr pData) { m_qTengxunRTWebData.PushData(pData); }
	CWebDataPtr PopTengxunRTData(void) { return m_qTengxunRTWebData.PopData(); }

	// 网易实时股票数据
	size_t NeteaseRTDataSize(void) { return m_qNeteaseRTWebData.Size(); }
	void PushNeteaseRTData(CWebDataPtr pData) { m_qNeteaseRTWebData.PushData(pData); }
	CWebDataPtr PopNeteaseRTData(void) { return m_qNeteaseRTWebData.PopData(); }

	// 网易日线数据
	size_t NeteaseDayLineDataSize(void) { return(m_qNeteaseDayLine.Size()); }
	void PushNeteaseDayLineData(CWebDataPtr pData) { m_qNeteaseDayLine.PushData(pData); }
	CWebDataPtr PopNeteaseDayLineData(void) { return m_qNeteaseDayLine.PopData(); }
	size_t GetParsedNeteaseDayLineDataSize(void) { return(m_qParsedNeteaseDayLine.Size()); }
	void PushParsedNeteaseDayLineData(CNeteaseDayLineWebDataPtr pData) { m_qParsedNeteaseDayLine.PushData(pData); }
	CNeteaseDayLineWebDataPtr PopParsedNeteaseDayLineData(void) { return m_qParsedNeteaseDayLine.PopData(); }

	// Finnhub股票推送数据
	size_t FinnhubWebSocketDataSize(void) { return m_qFinnhubWebSocketData.Size(); }
	void pushFinnhubWebSocketData(string data) { shared_ptr<string> pData = make_shared<string>(data); m_qFinnhubWebSocketData.PushData(pData); }
	void PushFinnhubWebSocketData(shared_ptr<string> pData) { m_qFinnhubWebSocketData.PushData(pData); }
	shared_ptr<string> PopFinnhubWebSocketData(void) { return m_qFinnhubWebSocketData.PopData(); }

	// Tiingo IEX股票推送数据
	size_t TiingoIEXWebSocketDataSize(void) { return m_qTiingoIEXWebSocketData.Size(); }
	void PushTiingoIEXWebSocketData(string data) { shared_ptr<string> pData = make_shared<string>(data); m_qTiingoIEXWebSocketData.PushData(pData); }
	void PushTiingoIEXWebSocketData(shared_ptr<string> pData) { m_qTiingoIEXWebSocketData.PushData(pData); }
	shared_ptr<string> PopTiingoIEXWebSocketData(void) { return m_qTiingoIEXWebSocketData.PopData(); }

	// Tiingo Crypto推送数据
	size_t TiingoCryptoWebSocketDataSize(void) { return m_qTiingoCryptoWebSocketData.Size(); }
	void PushTiingoCryptoWebSocketData(string data) { shared_ptr<string> pData = make_shared<string>(data); m_qTiingoCryptoWebSocketData.PushData(pData); }
	void PushTiingoCryptoWebSocketData(shared_ptr<string> pData) { m_qTiingoCryptoWebSocketData.PushData(pData); }
	shared_ptr<string> PopTiingoCryptoWebSocketData(void) { return m_qTiingoCryptoWebSocketData.PopData(); }

	// Tiingo forex推送数据
	size_t TiingoForexWebSocketDataSize(void) { return m_qTiingoForexWebSocketData.Size(); }
	void PushTiingoForexWebSocketData(string data) { shared_ptr<string> pData = make_shared<string>(data); m_qTiingoForexWebSocketData.PushData(pData); }
	void PushTiingoForexWebSocketData(shared_ptr<string> pData) { m_qTiingoForexWebSocketData.PushData(pData); }
	shared_ptr<string> PopTiingoForexWebSocketData(void) { return m_qTiingoForexWebSocketData.PopData(); }

protected:
	CTemplateMutexAccessQueue<CWebData> m_qSinaRTWebData; // 新浪股票网络数据暂存队列
	CTemplateMutexAccessQueue<CWebData> m_qTengxunRTWebData; // 腾讯网络数据暂存队列
	CTemplateMutexAccessQueue<CWebData> m_qNeteaseRTWebData; // 网易网络数据暂存队列
	CTemplateMutexAccessQueue<CWebData> m_qNeteaseWaitingParseRTWebData; // 网易网络数据暂存队列（等待Parsing）
	CTemplateMutexAccessQueue<CWebData> m_qFinnhubData; // Finnhub.io网络数据暂存队列
	CTemplateMutexAccessQueue<CWebData> m_qTiingoData; // Tiingo.com网络数据暂存队列

	CTemplateMutexAccessQueue<CWebData> m_qNeteaseDayLine; // 网易日线数据暂存队列

	CTemplateMutexAccessQueue<CNeteaseDayLineWebData> m_qParsedNeteaseDayLine;

	CTemplateMutexAccessQueue<string> m_qFinnhubWebSocketData; // finnhub的WebSocket数据
	CTemplateMutexAccessQueue<string> m_qTiingoIEXWebSocketData; // tiingo的WebSocket数据
	CTemplateMutexAccessQueue<string> m_qTiingoCryptoWebSocketData; // tiingo的WebSocket数据
	CTemplateMutexAccessQueue<string> m_qTiingoForexWebSocketData; // tiingo的WebSocket数据
};

extern CWebInquirer gl_WebInquirer; //
