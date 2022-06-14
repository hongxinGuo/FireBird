///////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ݲ�ѯ�������е������ѯ��λ�ڴ˴���
// ֻ��ͷ�ļ���
// WebData��ָ�������ϵõ���ԭʼ���ݡ�
//
// ���ڲ��ö����߳�ģʽ��ȡ�������ݣ��ʶ���Ҫ��֤�̰߳�ȫ��
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

extern CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry; // ��Ѷʵʱ���ݲɼ�
extern CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry; // ����ʵʱ���ݲɼ�
extern CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry; // ����ʵʱ���ݲɼ�
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry; // ����������ʷ����
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2; // ����������ʷ����
extern CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry; // Finnhub.io֤ȯ��Ϣ
extern CQuandlWebInquiryPtr gl_pQuandlWebInquiry;
extern CTiingoWebInquiryPtr gl_pTiingoWebInquiry;

class CWebInquirer : public CObject {
public:
	CWebInquirer(void);
	// ������ֵ��
	CWebInquirer(const CWebInquirer&) = delete;
	CWebInquirer& operator=(const CWebInquirer&) = delete;
	CWebInquirer(const CWebInquirer&&) noexcept = delete;
	CWebInquirer& operator=(const CWebInquirer&&) noexcept = delete;
	virtual ~CWebInquirer(void);

	void Initialize(void);
public:
	// �������ݲ�ѯ��
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
	// ����ʵʱ��Ʊ���ݡ�����վ�ϲ�ѯ����ԭʼ���ݣ�ÿ�����ݶ��Ǵ洢����վ�ṩ��ԭʼ���ݡ���ͬ
	size_t SinaRTDataSize(void) { return m_qSinaRTWebData.Size(); }
	void PushSinaRTData(CWebDataPtr pData) { m_qSinaRTWebData.PushData(pData); }
	CWebDataPtr PopSinaRTData(void) { return m_qSinaRTWebData.PopData(); }

	// ��Ѷʵʱ��Ʊ����
	size_t TengxunRTDataSize(void) { return m_qTengxunRTWebData.Size(); }
	void PushTengxunRTData(CWebDataPtr pData) { m_qTengxunRTWebData.PushData(pData); }
	CWebDataPtr PopTengxunRTData(void) { return m_qTengxunRTWebData.PopData(); }

	// ����ʵʱ��Ʊ����
	size_t NeteaseRTDataSize(void) { return m_qNeteaseRTWebData.Size(); }
	void PushNeteaseRTData(CWebDataPtr pData) { m_qNeteaseRTWebData.PushData(pData); }
	CWebDataPtr PopNeteaseRTData(void) { return m_qNeteaseRTWebData.PopData(); }

	// ������������
	size_t NeteaseDayLineDataSize(void) { return(m_qNeteaseDayLine.Size()); }
	void PushNeteaseDayLineData(CWebDataPtr pData) { m_qNeteaseDayLine.PushData(pData); }
	CWebDataPtr PopNeteaseDayLineData(void) { return m_qNeteaseDayLine.PopData(); }
	size_t GetParsedNeteaseDayLineDataSize(void) { return(m_qParsedNeteaseDayLine.Size()); }
	void PushParsedNeteaseDayLineData(CNeteaseDayLineWebDataPtr pData) { m_qParsedNeteaseDayLine.PushData(pData); }
	CNeteaseDayLineWebDataPtr PopParsedNeteaseDayLineData(void) { return m_qParsedNeteaseDayLine.PopData(); }

	// Finnhub��Ʊ��������
	size_t FinnhubWebSocketDataSize(void) { return m_qFinnhubWebSocketData.Size(); }
	void pushFinnhubWebSocketData(string data) { shared_ptr<string> pData = make_shared<string>(data); m_qFinnhubWebSocketData.PushData(pData); }
	void PushFinnhubWebSocketData(shared_ptr<string> pData) { m_qFinnhubWebSocketData.PushData(pData); }
	shared_ptr<string> PopFinnhubWebSocketData(void) { return m_qFinnhubWebSocketData.PopData(); }

	// Tiingo IEX��Ʊ��������
	size_t TiingoIEXWebSocketDataSize(void) { return m_qTiingoIEXWebSocketData.Size(); }
	void PushTiingoIEXWebSocketData(string data) { shared_ptr<string> pData = make_shared<string>(data); m_qTiingoIEXWebSocketData.PushData(pData); }
	void PushTiingoIEXWebSocketData(shared_ptr<string> pData) { m_qTiingoIEXWebSocketData.PushData(pData); }
	shared_ptr<string> PopTiingoIEXWebSocketData(void) { return m_qTiingoIEXWebSocketData.PopData(); }

	// Tiingo Crypto��������
	size_t TiingoCryptoWebSocketDataSize(void) { return m_qTiingoCryptoWebSocketData.Size(); }
	void PushTiingoCryptoWebSocketData(string data) { shared_ptr<string> pData = make_shared<string>(data); m_qTiingoCryptoWebSocketData.PushData(pData); }
	void PushTiingoCryptoWebSocketData(shared_ptr<string> pData) { m_qTiingoCryptoWebSocketData.PushData(pData); }
	shared_ptr<string> PopTiingoCryptoWebSocketData(void) { return m_qTiingoCryptoWebSocketData.PopData(); }

	// Tiingo forex��������
	size_t TiingoForexWebSocketDataSize(void) { return m_qTiingoForexWebSocketData.Size(); }
	void PushTiingoForexWebSocketData(string data) { shared_ptr<string> pData = make_shared<string>(data); m_qTiingoForexWebSocketData.PushData(pData); }
	void PushTiingoForexWebSocketData(shared_ptr<string> pData) { m_qTiingoForexWebSocketData.PushData(pData); }
	shared_ptr<string> PopTiingoForexWebSocketData(void) { return m_qTiingoForexWebSocketData.PopData(); }

protected:
	CTemplateMutexAccessQueue<CWebData> m_qSinaRTWebData; // ���˹�Ʊ���������ݴ����
	CTemplateMutexAccessQueue<CWebData> m_qTengxunRTWebData; // ��Ѷ���������ݴ����
	CTemplateMutexAccessQueue<CWebData> m_qNeteaseRTWebData; // �������������ݴ����
	CTemplateMutexAccessQueue<CWebData> m_qNeteaseWaitingParseRTWebData; // �������������ݴ���У��ȴ�Parsing��
	CTemplateMutexAccessQueue<CWebData> m_qFinnhubData; // Finnhub.io���������ݴ����
	CTemplateMutexAccessQueue<CWebData> m_qTiingoData; // Tiingo.com���������ݴ����

	CTemplateMutexAccessQueue<CWebData> m_qNeteaseDayLine; // �������������ݴ����

	CTemplateMutexAccessQueue<CNeteaseDayLineWebData> m_qParsedNeteaseDayLine;

	CTemplateMutexAccessQueue<string> m_qFinnhubWebSocketData; // finnhub��WebSocket����
	CTemplateMutexAccessQueue<string> m_qTiingoIEXWebSocketData; // tiingo��WebSocket����
	CTemplateMutexAccessQueue<string> m_qTiingoCryptoWebSocketData; // tiingo��WebSocket����
	CTemplateMutexAccessQueue<string> m_qTiingoForexWebSocketData; // tiingo��WebSocket����
};

extern CWebInquirer gl_WebInquirer; //
