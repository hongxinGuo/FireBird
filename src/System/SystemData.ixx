///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 所有的系统数据皆位于此处。
// 只有头文件。
//
// 系统内部的通用数据，无论处理过或者没有处理过。此处存储的数据，皆由主线程使用，故而无需使用线程安全机制。
//
///////////////////////////////////////////////////////////////////////////////////////////////////
module;

#include <queue>
#include<concurrentqueue/moodycamel/concurrentqueue.h>

export module FireBirdLib.SystemData;

import FireBirdLib.WebData.DayLine;
import FireBirdLib.WebRTData;

using namespace moodycamel;
import std;
using std::queue;
using std::shared_ptr;


import FireBirdLib.WebSocket.Finnhub;
import FireBirdLib.WebSocket.TiingoIEX;
import FireBirdLib.WebSocket.TiingoCrypto;
import FireBirdLib.WebSocket.TiingoForex;

export {
	// 处理后的各种数据
	extern ConcurrentQueue<CWebRTDataPtr> gl_qChinaMarketRTData; // 中国市场新浪实时数据队列。
	extern ConcurrentQueue<CDayLineWebDataPtr> gl_qDayLine; // 日线数据

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
		void PushFinnhubSocket(const shared_ptr<CFinnhubSocket>& p) { m_qFinnhubSocket.push(p); }

		shared_ptr<CFinnhubSocket> PopFinnhubSocket() {
			shared_ptr<CFinnhubSocket> p = m_qFinnhubSocket.front();
			m_qFinnhubSocket.pop();
			return p;
		}

		// TiingoCrypto Socket缓存数据
		size_t GetTiingoCryptoSocketSize() const { return m_qTiingoCryptoSocket.size(); }
		void PushTiingoCryptoSocket(const shared_ptr<CTiingoCryptoSocket>& p) { m_qTiingoCryptoSocket.push(p); }

		shared_ptr<CTiingoCryptoSocket> PopTiingoCryptoSocket() {
			shared_ptr<CTiingoCryptoSocket> p = m_qTiingoCryptoSocket.front();
			m_qTiingoCryptoSocket.pop();
			return p;
		}

		// TiingoIEX Socket缓存数据
		size_t GetTiingoIEXSocketSize() const { return m_qTiingoIEXSocket.size(); }
		void PushTiingoIEXSocket(const shared_ptr<CTiingoIEXSocket>& p) { m_qTiingoIEXSocket.push(p); }

		shared_ptr<CTiingoIEXSocket> PopTiingoIEXSocket() {
			shared_ptr<CTiingoIEXSocket> p = m_qTiingoIEXSocket.front();
			m_qTiingoIEXSocket.pop();
			return p;
		}

		// TiingoForex Socket缓存数据
		size_t GetTiingoForexSocketSize() const { return m_qTiingoForexSocket.size(); }
		void PushTiingoForexSocket(const shared_ptr<CTiingoForexSocket>& p) { m_qTiingoForexSocket.push(p); }

		shared_ptr<CTiingoForexSocket> PopTiingoForexSocket() {
			shared_ptr<CTiingoForexSocket> p = m_qTiingoForexSocket.front();
			m_qTiingoForexSocket.pop();
			return p;
		}

		void ClearChinaMarketRTDataQueue() {
			bool succeed = true;
			CWebRTDataPtr pRTData;
			while (succeed) succeed = gl_qChinaMarketRTData.try_dequeue(pRTData);
		}

		void ClearDataQueue() {
			ClearChinaMarketRTDataQueue();
			bool succeed = true;
			CDayLineWebDataPtr pData;
			while (succeed) succeed = gl_qDayLine.try_dequeue(pData);
		}

		size_t GetChinaMarketRTDataQueueSize() const { return gl_qChinaMarketRTData.size_approx(); }
		size_t GetDayLineQueueSize() const { return gl_qDayLine.size_approx(); }

	private:
		queue<shared_ptr<CFinnhubSocket>> m_qFinnhubSocket;
		queue<shared_ptr<CTiingoCryptoSocket>> m_qTiingoCryptoSocket;
		queue<shared_ptr<CTiingoForexSocket>> m_qTiingoForexSocket;
		queue<shared_ptr<CTiingoIEXSocket>> m_qTiingoIEXSocket;
	};

	extern CSystemData gl_SystemData;
}