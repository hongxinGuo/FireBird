///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���е�ϵͳ���ݽ�λ�ڴ˴���
// ֻ��ͷ�ļ���
//
// ϵͳ�ڲ���ͨ�����ݣ����۴��������û�д�������˴��洢�����ݣ��������߳�ʹ�ã��ʶ�����ʹ���̰߳�ȫ���ơ�
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"readerwriterqueue.h"

using namespace moodycamel;

#include <gsl/pointers>
using gsl::not_null;

#include "ContainerChinaStock.h"
#include "containerChosenCrypto.h"
#include "ContainerChosenForex.h"
#include "ContainerChosenStock.h"
#include "ContainerFinnhubCountry.h"
#include "ContainerFinnhubCryptoExchange.h"
#include "containerFinnhubCryptoSymbol.h"
#include "ContainerFinnhubEconomicCalendar.h"
#include "ContainerFinnhubForexExchange.h"
#include "containerFinnhubForexSymbol.h"
#include "ContainerFinnhubStockExchange.h"
#include "ContainerStockSymbol.h"
#include "ContainerTiingoCryptoSymbol.h"
#include "ContainerTiingoStock.h"
#include "ContainerWorldStock.h"

#include"FinnhubWebSocket.h"
#include"TiingoIEXWebSocket.h"
#include"TiingoCryptoWebSocket.h"
#include"TiingoForexWebSocket.h"

#include "PriorityQueueWebRTData.h"
#include "DayLineWebData.h"

// ϵͳ״̬
extern bool gl_bChinaMarketResetting; // �й��г�������
extern bool gl_bWorldMarketResetting; // �����г�������

// �����ĸ�������
extern ReaderWriterQueue<CWebRTDataPtr> gl_qSinaRT; // �й��г�����ʵʱ���ݶ��С�
extern ReaderWriterQueue<CWebRTDataPtr> gl_qNeteaseRT; // �й��г�����ʵʱ���ݶ��С�
extern ReaderWriterQueue<CWebRTDataPtr> gl_qTengxunRT; // �й��г���Ѷʵʱ���ݶ��С�
extern CTemplateMutexAccessQueue<CDayLineWebData> gl_qDayLine; // ��������

// ChinaMarket���������
extern CContainerChinaStock gl_dataContainerChinaStock;
extern CContainerStockSymbol gl_dataContainerChinaStockSymbol;

// WorldMarket���������
extern CContainerFinnhubForexSymbol gl_dataFinnhubForexSymbol;
extern CContainerFinnhubCryptoSymbol gl_dataFinnhubCryptoSymbol;
extern CContainerFinnhubCountry gl_dataContainerFinnhubCountry;
extern CContainerFinnhubEconomicCalendar gl_dataContainerFinnhubEconomicCalendar;

extern CContainerFinnhubStockExchange gl_dataContainerFinnhubStockExchange;
extern CContainerFinnhubForexExchange gl_dataContainerFinnhubForexExchange;
extern CContainerFinnhubCryptoExchange gl_dataContainerFinnhubCryptoExchange;

extern CContainerWorldStock gl_dataContainerFinnhubStock;
extern CContainerTiingoStock gl_dataContainerTiingoStock;
extern CContainerTiingoCryptoSymbol gl_dataContainerTiingoCryptoSymbol;

extern CContainerChosenStock gl_dataContainerChosenWorldStock;
extern CContainerChosenForex gl_dataContainerChosenWorldForex;
extern CContainerChosenCrypto gl_dataContainerChosenWorldCrypto;

class CSystemData final {
public:
	CSystemData() = default;
	// ������ֵ��
	CSystemData(const CSystemData&) = delete;
	CSystemData& operator=(const CSystemData&) = delete;
	CSystemData(const CSystemData&&) noexcept = delete;
	CSystemData& operator=(const CSystemData&&) noexcept = delete;
	~CSystemData() = default;

	// Finnhub Socket��������
	size_t GetFinnhubSocketSize() const { return m_qFinnhubSocket.size(); }
	void PushFinnhubSocket(const CFinnhubSocketPtr& p) { m_qFinnhubSocket.push(p); }

	CFinnhubSocketPtr PopFinnhubSocket() {
		CFinnhubSocketPtr p = m_qFinnhubSocket.front();
		m_qFinnhubSocket.pop();
		return p;
	}

	// TiingoCrypto Socket��������
	size_t GetTiingoCryptoSocketSize() const { return m_qTiingoCryptoSocket.size(); }
	void PushTiingoCryptoSocket(const CTiingoCryptoSocketPtr& p) { m_qTiingoCryptoSocket.push(p); }

	CTiingoCryptoSocketPtr PopTiingoCryptoSocket() {
		CTiingoCryptoSocketPtr p = m_qTiingoCryptoSocket.front();
		m_qTiingoCryptoSocket.pop();
		return p;
	}

	// TiingoIEX Socket��������
	size_t GetTiingoIEXSocketSize() const { return m_qTiingoIEXSocket.size(); }
	void PushTiingoIEXSocket(const CTiingoIEXSocketPtr& p) { m_qTiingoIEXSocket.push(p); }

	CTiingoIEXSocketPtr PopTiingoIEXSocket() {
		CTiingoIEXSocketPtr p = m_qTiingoIEXSocket.front();
		m_qTiingoIEXSocket.pop();
		return p;
	}

	// TiingoForex Socket��������
	size_t GetTiingoForexSocketSize() const { return m_qTiingoForexSocket.size(); }
	void PushTiingoForexSocket(const CTiingoForexSocketPtr& p) { m_qTiingoForexSocket.push(p); }

	CTiingoForexSocketPtr PopTiingoForexSocket() {
		CTiingoForexSocketPtr p = m_qTiingoForexSocket.front();
		m_qTiingoForexSocket.pop();
		return p;
	}

	static void ClearRTDataQueue() {
		bool succeed = true;
		CWebRTDataPtr pRTData;
		while (succeed) succeed = gl_qSinaRT.try_dequeue(pRTData);
		while (succeed) succeed = gl_qNeteaseRT.try_dequeue(pRTData);
		while (succeed) succeed = gl_qTengxunRT.try_dequeue(pRTData);
		while (gl_qDayLine.Size() > 0) gl_qDayLine.PopData();
	}

private:
	queue<CFinnhubSocketPtr> m_qFinnhubSocket;
	queue<CTiingoCryptoSocketPtr> m_qTiingoCryptoSocket;
	queue<CTiingoForexSocketPtr> m_qTiingoForexSocket;
	queue<CTiingoIEXSocketPtr> m_qTiingoIEXSocket;
};

extern CSystemData gl_SystemData;
