///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 所有的系统数据皆位于此处。
// 只有头文件。
//
// 系统内部的通用数据，无论处理过或者没有处理过。此处存储的数据，皆由主线程使用，故而无需使用线程安全机制。
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <gsl/pointers>

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
using gsl::not_null;

#include"FinnhubWebSocket.h"
#include"TiingoIEXWebSocket.h"
#include"TiingoCryptoWebSocket.h"
#include"TiingoForexWebSocket.h"

#include "PriorityQueueWebRTData.h"
#include "DayLineWebData.h"

// 处理后的各种数据
extern CPriorityQueueWebRTData gl_qSinaRT; // 中国市场新浪实时数据队列。
extern CPriorityQueueWebRTData gl_qNeteaseRT; // 中国市场网易实时数据队列。
extern CPriorityQueueWebRTData gl_qTengxunRT; // 中国市场腾讯实时数据队列。
extern CTemplateMutexAccessQueue<CDayLineWebData> gl_qDayLine; // 日线数据

// ChinaMarket处理的数据
extern CContainerChinaStock gl_containerChinaStock;
extern CContainerStockSymbol gl_dataContainerChinaStockSymbol;

// WorldMarket处理的数据
extern CContainerFinnhubForexSymbol gl_dataFinnhubForexSymbol;
extern CContainerFinnhubCryptoSymbol gl_dataFinnhubCryptoSymbol;
extern CContainerFinnhubCountry gl_dataFinnhubCountry;
extern CContainerFinnhubEconomicCalendar gl_dataFinnhubEconomicCalendar;

extern CContainerFinnhubStockExchange gl_dataFinnhubStockExchange;
extern CContainerFinnhubForexExchange gl_dataFinnhubForexExchange;
extern CContainerFinnhubCryptoExchange gl_dataFinnhubCryptoExchange;

extern CContainerWorldStock gl_dataContainerFinnhubStock;
extern CContainerTiingoStock gl_dataTiingoStock;
extern CContainerTiingoCryptoSymbol gl_dataTiingoCryptoSymbol;

extern CContainerChosenStock gl_dataContainerChosenWorldStock;
extern CContainerChosenForex gl_dataContainerChosenWorldForex;
extern CContainerChosenCrypto gl_dataContainerChosenWorldCrypto;

class CSystemData final {
public:
	CSystemData() = default;
	// 不允许赋值。
	CSystemData(const CSystemData&) = delete;
	CSystemData& operator=(const CSystemData&) = delete;
	CSystemData(const CSystemData&&) noexcept = delete;
	CSystemData& operator=(const CSystemData&&) noexcept = delete;
	~CSystemData() = default;

	// Finnhub Socket缓存数据
	size_t GetFinnhubSocketSize() const { return m_qFinnhubSocket.size(); }
	void PushFinnhubSocket(const CFinnhubSocketPtr& p) { m_qFinnhubSocket.push(p); }

	CFinnhubSocketPtr PopFinnhubSocket() {
		CFinnhubSocketPtr p = m_qFinnhubSocket.front();
		m_qFinnhubSocket.pop();
		return p;
	}

	// TiingoCrypto Socket缓存数据
	size_t GetTiingoCryptoSocketSize() const { return m_qTiingoCryptoSocket.size(); }
	void PushTiingoCryptoSocket(const CTiingoCryptoSocketPtr& p) { m_qTiingoCryptoSocket.push(p); }

	CTiingoCryptoSocketPtr PopTiingoCryptoSocket() {
		CTiingoCryptoSocketPtr p = m_qTiingoCryptoSocket.front();
		m_qTiingoCryptoSocket.pop();
		return p;
	}

	// TiingoIEX Socket缓存数据
	size_t GetTiingoIEXSocketSize() const { return m_qTiingoIEXSocket.size(); }
	void PushTiingoIEXSocket(const CTiingoIEXSocketPtr& p) { m_qTiingoIEXSocket.push(p); }

	CTiingoIEXSocketPtr PopTiingoIEXSocket() {
		CTiingoIEXSocketPtr p = m_qTiingoIEXSocket.front();
		m_qTiingoIEXSocket.pop();
		return p;
	}

	// TiingoForex Socket缓存数据
	size_t GetTiingoForexSocketSize() const { return m_qTiingoForexSocket.size(); }
	void PushTiingoForexSocket(const CTiingoForexSocketPtr& p) { m_qTiingoForexSocket.push(p); }

	CTiingoForexSocketPtr PopTiingoForexSocket() {
		CTiingoForexSocketPtr p = m_qTiingoForexSocket.front();
		m_qTiingoForexSocket.pop();
		return p;
	}

	static void ClearRTDataQueue() {
		while (gl_qSinaRT.Size() > 0) gl_qSinaRT.PopData();
		while (gl_qNeteaseRT.Size() > 0) gl_qNeteaseRT.PopData();
		while (gl_qTengxunRT.Size() > 0) gl_qTengxunRT.PopData();
		while (gl_qDayLine.Size() > 0) gl_qDayLine.PopData();
	}

private:
	queue<CFinnhubSocketPtr> m_qFinnhubSocket;
	queue<CTiingoCryptoSocketPtr> m_qTiingoCryptoSocket;
	queue<CTiingoForexSocketPtr> m_qTiingoForexSocket;
	queue<CTiingoIEXSocketPtr> m_qTiingoIEXSocket;
};

extern CSystemData gl_SystemData;
