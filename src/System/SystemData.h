///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���е�ϵͳ���ݽ�λ�ڴ˴���
// ֻ��ͷ�ļ���
//
// ϵͳ�ڲ���ͨ�����ݣ����۴��������û�д�������˴��洢�����ݣ��������߳�ʹ�ã��ʶ�����ʹ���̰߳�ȫ���ơ�
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <gsl/pointers>

#include "ContainerFinnhubCountry.h"
#include "containerFinnhubCryptoSymbol.h"
#include "ContainerFinnhubEconomicCalendar.h"
#include "containerFinnhubForexSymbol.h"
using gsl::not_null;

#include"FinnhubWebSocket.h"
#include"TiingoIEXWebSocket.h"
#include"TiingoCryptoWebSocket.h"
#include"TiingoForexWebSocket.h"

#include "PriorityQueueWebRTData.h"
#include "DayLineWebData.h"

// �����ĸ�������
extern CPriorityQueueWebRTData gl_qSinaRT; // �й��г�����ʵʱ���ݶ��С�
extern CPriorityQueueWebRTData gl_qNeteaseRT; // �й��г�����ʵʱ���ݶ��С�
extern CPriorityQueueWebRTData gl_qTengxunRT; // �й��г���Ѷʵʱ���ݶ��С�
extern CTemplateMutexAccessQueue<CDayLineWebData> gl_qDayLine; // ��������

// WorldMarket���������
extern CContainerFinnhubForexSymbol gl_dataFinnhubForexSymbol;
extern CContainerFinnhubCryptoSymbol gl_dataFinnhubCryptoSymbol;
extern CContainerFinnhubCountry gl_dataFinnhubCountry;
extern CContainerFinnhubEconomicCalendar gl_dataFinnhubEconomicCalendar;

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
