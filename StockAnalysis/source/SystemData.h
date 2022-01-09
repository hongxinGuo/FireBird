///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���е�ϵͳ���ݽ�λ�ڴ˴���
// ֻ��ͷ�ļ���
//
// ϵͳ�ڲ���ͨ�����ݣ����۴��������û�д�������˴��洢�����ݣ��������߳�ʹ�ã��ʶ�����ʹ���̰߳�ȫ���ơ�
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"globedef.h"
#include"FinnhubWebSocket.h"
#include"TiingoIEXWebSocket.h"
#include"TiingoCryptoWebSocket.h"
#include"TiingoForexWebSocket.h"

class CSystemData : public CObject {
public:
	CSystemData(void) {}
	~CSystemData(void) {}

public:
	// Finnhub Socket��������
	size_t GetFinnhubSocketSize(void) { return m_qFinnhubSocket.size(); }
	void PushFinnhubSocket(CFinnhubSocketPtr p) { m_qFinnhubSocket.push(p); }
	CFinnhubSocketPtr PopFinnhubSocket(void) { CFinnhubSocketPtr p = m_qFinnhubSocket.front(); m_qFinnhubSocket.pop(); return p; }

	// TiingoCrypto Socket��������
	size_t GetTiingoCryptoSocketSize(void) { return m_qTiingoCryptoSocket.size(); }
	void PushTiingoCryptoSocket(CTiingoCryptoSocketPtr p) { m_qTiingoCryptoSocket.push(p); }
	CTiingoCryptoSocketPtr PopTiingoCryptoSocket(void) { CTiingoCryptoSocketPtr p = m_qTiingoCryptoSocket.front(); m_qTiingoCryptoSocket.pop(); return p; }

	// TiingoIEX Socket��������
	size_t GetTiingoIEXSocketSize(void) { return m_qTiingoIEXSocket.size(); }
	void PushTiingoIEXSocket(CTiingoIEXSocketPtr p) { m_qTiingoIEXSocket.push(p); }
	CTiingoIEXSocketPtr PopTiingoIEXSocket(void) { CTiingoIEXSocketPtr p = m_qTiingoIEXSocket.front(); m_qTiingoIEXSocket.pop(); return p; }

	// TiingoForex Socket��������
	size_t GetTiingoForexSocketSize(void) { return m_qTiingoForexSocket.size(); }
	void PushTiingoForexSocket(CTiingoForexSocketPtr p) { m_qTiingoForexSocket.push(p); }
	CTiingoForexSocketPtr PopTiingoForexSocket(void) { CTiingoForexSocketPtr p = m_qTiingoForexSocket.front(); m_qTiingoForexSocket.pop(); return p; }

private:
	queue<CFinnhubSocketPtr> m_qFinnhubSocket;
	queue<CTiingoCryptoSocketPtr> m_qTiingoCryptoSocket;
	queue<CTiingoForexSocketPtr> m_qTiingoForexSocket;
	queue<CTiingoIEXSocketPtr> m_qTiingoIEXSocket;
};

extern CSystemData gl_SystemData;