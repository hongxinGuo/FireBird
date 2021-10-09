///////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ݲ�ѯ�������е������ѯ��λ�ڴ˴���
// ֻ��ͷ�ļ���
// WebData��ָ�������ϵõ���ԭʼ���ݡ�
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

extern CCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry; // crweber.com�ϵ�ÿ������ָ��
extern CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry; // ��Ѷʵʱ���ݲɼ�
extern CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry; // ����ʵʱ���ݲɼ�
extern CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry; // ����ʵʱ���ݲɼ�
extern CPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry; // Poten.com�ϵ��������ݡ�
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry; // ����������ʷ����
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2; // ����������ʷ����
extern CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry; // Finnhub.io֤ȯ��Ϣ
extern CQuandlWebInquiryPtr gl_pQuandlWebInquiry;
extern CTiingoWebInquiryPtr gl_pTiingoWebInquiry;

class CWebInquirer : public CObject {
public:
	CWebInquirer(void);
	virtual ~CWebInquirer(void);

	void Initialize(void);
public:
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
	size_t GetSinaRTDataSize(void) { return m_qSinaRTWebData.GetDataSize(); }
	void PushSinaRTData(CWebDataPtr pData) { m_qSinaRTWebData.PushData(pData); }
	CWebDataPtr PopSinaRTData(void) { return m_qSinaRTWebData.PopData(); }
	size_t GetTengxunRTDataSize(void) { return m_qTengxunRTWebData.GetDataSize(); }
	void PushTengxunRTData(CWebDataPtr pData) { m_qTengxunRTWebData.PushData(pData); }
	CWebDataPtr PopTengxunRTData(void) { return m_qTengxunRTWebData.PopData(); }
	size_t GetNeteaseRTDataSize(void) { return m_qNeteaseRTWebData.GetDataSize(); }
	void PushNeteaseRTData(CWebDataPtr pData) { m_qNeteaseRTWebData.PushData(pData); }
	CWebDataPtr PopNeteaseRTData(void) { return m_qNeteaseRTWebData.PopData(); }
	size_t GetCrweberDataSize(void) { return m_qCrweberWebData.GetDataSize(); }
	void PushCrweberData(CWebDataPtr pData) { m_qCrweberWebData.PushData(pData); }
	CWebDataPtr PopCrweberData(void) { return m_qCrweberWebData.PopData(); }

	size_t GetPotenDailyBriefingDataSize(void) { return m_qPotenDailyBriefingWebData.GetDataSize(); }
	void PushPotenDailyBriefingData(CWebDataPtr pData) { m_qPotenDailyBriefingWebData.PushData(pData); }
	CWebDataPtr PopPotenDailyBriefingData(void) { return m_qPotenDailyBriefingWebData.PopData(); }

	size_t GetFinnhubDataSize(void) { return m_qFinnhubData.GetDataSize(); }
	void PushFinnhubData(CWebDataPtr pData) { m_qFinnhubData.PushData(pData); }
	CWebDataPtr PopFinnhubData(void) { return m_qFinnhubData.PopData(); }

	size_t GetQuandlDataSize(void) { return m_qQuandlData.GetDataSize(); }
	void PushQuandlData(CWebDataPtr pData) { m_qQuandlData.PushData(pData); }
	CWebDataPtr PopQuandlData(void) { return m_qQuandlData.PopData(); }

	size_t GetTiingoDataSize(void) { return m_qTiingoData.GetDataSize(); }
	void PushTiingoData(CWebDataPtr pData) { m_qTiingoData.PushData(pData); }
	CWebDataPtr PopTiingoData(void) { return m_qTiingoData.PopData(); }

	size_t GetDownLoadedNeteaseDayLineDataSize(void) { return(m_qDownLoadedNeteaseDayLine.GetDataSize()); }
	void PushDownLoadedNeteaseDayLineData(CDownLoadedNeteaseDayLinePtr pData) { m_qDownLoadedNeteaseDayLine.PushData(pData); }
	CDownLoadedNeteaseDayLinePtr PopDownLoadedNeteaseDayLineData(void) { return m_qDownLoadedNeteaseDayLine.PopData(); }

	size_t GetFinnhubWebSocketDataSize(void) { return m_qFinnhubWebSocketData.GetDataSize(); }
	void pushFinnhubWebSocketData(string data) { m_qFinnhubWebSocketData.PushData(data); }
	void PushFinnhubWebSocketData(shared_ptr<string> pData) { m_qFinnhubWebSocketData.PushData(pData); }
	shared_ptr<string> PopFinnhubWebSocketData(void) { return m_qFinnhubWebSocketData.PopData(); }

	size_t GetTiingoIEXWebSocketDataSize(void) { return m_qTiingoIEXWebSocketData.GetDataSize(); }
	void PushTiingoIEXWebSocketData(string data) { m_qTiingoIEXWebSocketData.PushData(data); }
	void PushTiingoIEXWebSocketData(shared_ptr<string> pData) { m_qTiingoIEXWebSocketData.PushData(pData); }
	shared_ptr<string> PopTiingoIEXWebSocketData(void) { return m_qTiingoIEXWebSocketData.PopData(); }

	size_t GetTiingoCryptoWebSocketDataSize(void) { return m_qTiingoCryptoWebSocketData.GetDataSize(); }
	void PushTiingoCryptoWebSocketData(string data) { m_qTiingoCryptoWebSocketData.PushData(data); }
	void PushTiingoCryptoWebSocketData(shared_ptr<string> pData) { m_qTiingoCryptoWebSocketData.PushData(pData); }
	shared_ptr<string> PopTiingoCryptoWebSocketData(void) { return m_qTiingoCryptoWebSocketData.PopData(); }

	size_t GetTiingoForexWebSocketDataSize(void) { return m_qTiingoForexWebSocketData.GetDataSize(); }
	void PushTiingoForexWebSocketData(string data) { m_qTiingoForexWebSocketData.PushData(data); }
	void PushTiingoForexWebSocketData(shared_ptr<string> pData) { m_qTiingoForexWebSocketData.PushData(pData); }
	shared_ptr<string> PopTiingoForexWebSocketData(void) { return m_qTiingoForexWebSocketData.PopData(); }

protected:
	CQueueWebData m_qSinaRTWebData; // ���˹�Ʊ���������ݴ����
	CQueueWebData m_qTengxunRTWebData; // ��Ѷ���������ݴ����
	CQueueWebData m_qNeteaseRTWebData; // �������������ݴ����
	CQueueWebData m_qCrweberWebData; // crweber.com���������ݴ����
	CQueueWebData m_qPotenDailyBriefingWebData; // Poten.com���������ݴ����
	CQueueWebData m_qFinnhubData; // Finnhub.io���������ݴ����
	CQueueWebData m_qQuandlData; // Quandl.com���������ݴ����
	CQueueWebData m_qTiingoData; // Tiingo.com���������ݴ����

	CQueueDownLoadedNeteaseDayLine m_qDownLoadedNeteaseDayLine; // �������������ݴ����

	CQueueString m_qFinnhubWebSocketData; // finnhub��WebSocket����
	CQueueString m_qTiingoIEXWebSocketData; // tiingo��WebSocket����
	CQueueString m_qTiingoCryptoWebSocketData; // tiingo��WebSocket����
	CQueueString m_qTiingoForexWebSocketData; // tiingo��WebSocket����
};

extern CWebInquirer gl_WebInquirer; //
