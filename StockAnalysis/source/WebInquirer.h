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
	// �������ݲ�ѯ��
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
	// ����ʵʱ��Ʊ���ݡ�����վ�ϲ�ѯ����ԭʼ���ݣ�ÿ�����ݶ��Ǵ洢����վ�ṩ��ԭʼ���ݡ���ͬ
	size_t GetSinaRTDataSize(void) { return m_qSinaRTWebData.GetDataSize(); }
	void PushSinaRTData(CWebDataPtr pData) { m_qSinaRTWebData.PushData(pData); }
	CWebDataPtr PopSinaRTData(void) { return m_qSinaRTWebData.PopData(); }

	// ��Ѷʵʱ��Ʊ����
	size_t GetTengxunRTDataSize(void) { return m_qTengxunRTWebData.GetDataSize(); }
	void PushTengxunRTData(CWebDataPtr pData) { m_qTengxunRTWebData.PushData(pData); }
	CWebDataPtr PopTengxunRTData(void) { return m_qTengxunRTWebData.PopData(); }

	// ����ʵʱ��Ʊ����
	size_t GetNeteaseRTDataSize(void) { return m_qNeteaseRTWebData.GetDataSize(); }
	void PushNeteaseRTData(CWebDataPtr pData) { m_qNeteaseRTWebData.PushData(pData); }
	CWebDataPtr PopNeteaseRTData(void) { return m_qNeteaseRTWebData.PopData(); }

	// Crweber.com�������ݣ�Ŀǰ��ʧЧ��
	size_t GetCrweberDataSize(void) { return m_qCrweberWebData.GetDataSize(); }
	void PushCrweberData(CWebDataPtr pData) { m_qCrweberWebData.PushData(pData); }
	CWebDataPtr PopCrweberData(void) { return m_qCrweberWebData.PopData(); }

	// Poten Daily Briefing�������ݣ�Ŀǰ��ʧЧ��
	size_t GetPotenDailyBriefingDataSize(void) { return m_qPotenDailyBriefingWebData.GetDataSize(); }
	void PushPotenDailyBriefingData(CWebDataPtr pData) { m_qPotenDailyBriefingWebData.PushData(pData); }
	CWebDataPtr PopPotenDailyBriefingData(void) { return m_qPotenDailyBriefingWebData.PopData(); }

	// Finnhub��Ʊ����
	size_t GetFinnhubDataSize(void) { return m_qFinnhubData.GetDataSize(); }
	void PushFinnhubData(CWebDataPtr pData) { m_qFinnhubData.PushData(pData); }
	CWebDataPtr PopFinnhubData(void) { return m_qFinnhubData.PopData(); }

	// Quandl��Ʊ����
	size_t GetQuandlDataSize(void) { return m_qQuandlData.GetDataSize(); }
	void PushQuandlData(CWebDataPtr pData) { m_qQuandlData.PushData(pData); }
	CWebDataPtr PopQuandlData(void) { return m_qQuandlData.PopData(); }

	// Tiingo��Ʊ����
	size_t GetTiingoDataSize(void) { return m_qTiingoData.GetDataSize(); }
	void PushTiingoData(CWebDataPtr pData) { m_qTiingoData.PushData(pData); }
	CWebDataPtr PopTiingoData(void) { return m_qTiingoData.PopData(); }

	// ������������
	size_t GetNeteaseDayLineDataSize(void) { return(m_qNeteaseDayLine.GetDataSize()); }
	void PushNeteaseDayLineData(CWebDataPtr pData) { m_qNeteaseDayLine.PushData(pData); }
	CWebDataPtr PopNeteaseDayLineData(void) { return m_qNeteaseDayLine.PopData(); }

	// Finnhub��Ʊ��������
	size_t GetFinnhubWebSocketDataSize(void) { return m_qFinnhubWebSocketData.GetDataSize(); }
	void pushFinnhubWebSocketData(string data) { m_qFinnhubWebSocketData.PushData(data); }
	void PushFinnhubWebSocketData(shared_ptr<string> pData) { m_qFinnhubWebSocketData.PushData(pData); }
	shared_ptr<string> PopFinnhubWebSocketData(void) { return m_qFinnhubWebSocketData.PopData(); }

	// Tiingo IEX��Ʊ��������
	size_t GetTiingoIEXWebSocketDataSize(void) { return m_qTiingoIEXWebSocketData.GetDataSize(); }
	void PushTiingoIEXWebSocketData(string data) { m_qTiingoIEXWebSocketData.PushData(data); }
	void PushTiingoIEXWebSocketData(shared_ptr<string> pData) { m_qTiingoIEXWebSocketData.PushData(pData); }
	shared_ptr<string> PopTiingoIEXWebSocketData(void) { return m_qTiingoIEXWebSocketData.PopData(); }

	// Tiingo Crypto��������
	size_t GetTiingoCryptoWebSocketDataSize(void) { return m_qTiingoCryptoWebSocketData.GetDataSize(); }
	void PushTiingoCryptoWebSocketData(string data) { m_qTiingoCryptoWebSocketData.PushData(data); }
	void PushTiingoCryptoWebSocketData(shared_ptr<string> pData) { m_qTiingoCryptoWebSocketData.PushData(pData); }
	shared_ptr<string> PopTiingoCryptoWebSocketData(void) { return m_qTiingoCryptoWebSocketData.PopData(); }

	// Tiingo forex��������
	size_t GetTiingoForexWebSocketDataSize(void) { return m_qTiingoForexWebSocketData.GetDataSize(); }
	void PushTiingoForexWebSocketData(string data) { m_qTiingoForexWebSocketData.PushData(data); }
	void PushTiingoForexWebSocketData(shared_ptr<string> pData) { m_qTiingoForexWebSocketData.PushData(pData); }
	shared_ptr<string> PopTiingoForexWebSocketData(void) { return m_qTiingoForexWebSocketData.PopData(); }

	// Finnhub WebSocket��������
	size_t GetFinnhubSocketSize(void) { return m_qFinnhubSocket.size(); }
	void PushFinnhubSocket(CFinnhubSocketPtr p) { m_qFinnhubSocket.push(p); }
	CFinnhubSocketPtr PopFinnhubSocket(void) { CFinnhubSocketPtr p = m_qFinnhubSocket.front(); m_qFinnhubSocket.pop(); return p; }

protected:
	CQueueWebData m_qSinaRTWebData; // ���˹�Ʊ���������ݴ����
	CQueueWebData m_qTengxunRTWebData; // ��Ѷ���������ݴ����
	CQueueWebData m_qNeteaseRTWebData; // �������������ݴ����
	CQueueWebData m_qCrweberWebData; // crweber.com���������ݴ����
	CQueueWebData m_qPotenDailyBriefingWebData; // Poten.com���������ݴ����
	CQueueWebData m_qFinnhubData; // Finnhub.io���������ݴ����
	CQueueWebData m_qQuandlData; // Quandl.com���������ݴ����
	CQueueWebData m_qTiingoData; // Tiingo.com���������ݴ����

	CQueueWebData m_qNeteaseDayLine; // �������������ݴ����

	CQueueString m_qFinnhubWebSocketData; // finnhub��WebSocket����
	CQueueString m_qTiingoIEXWebSocketData; // tiingo��WebSocket����
	CQueueString m_qTiingoCryptoWebSocketData; // tiingo��WebSocket����
	CQueueString m_qTiingoForexWebSocketData; // tiingo��WebSocket����

	queue<CFinnhubSocketPtr> m_qFinnhubSocket;
	queue<CTiingoCryptoSocketPtr> m_qTiingoCryptoSocket;
	queue<CTiingoForexSocketPtr> m_qTiingoForexSocket;
	queue<CTiingoIEXSocketPtr> m_qTiingoIEXSocket;

};

extern CWebInquirer gl_WebInquirer; //
