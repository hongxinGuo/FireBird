////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 尝试将ixWebSocket变量封装于此类中。
// 牵涉太多，不容易封装。
// 此类负责ixWebSocket的初始化。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"pch.h"

using namespace std;
#include<string>

#include <ixwebsocket/IXWebSocket.h>

class CVirtualWebSocket : public CObject {
public:
	CVirtualWebSocket(bool fHaveSubscriptionId = true);
	virtual ~CVirtualWebSocket();

	virtual bool Connect(void) = 0;
	virtual bool Send(vector<CString> vSymbol) = 0;
	bool ConnectingWebSocketAndSendMessage(vector<CString> vSymbol);

	CString CreateTiingoWebSocketSymbolString(vector<CString> vSymbol);

	bool Connecting(string url, const ix::OnMessageCallback& callback, int iPingPeriod = 60, bool fDeflate = true);
	bool Deconnecting(void);

	bool Sending(string message);

	ix::ReadyState GetState(void) { return m_webSocket.getReadyState(); }
	bool IsClosed(void) { return m_webSocket.getReadyState() == ix::ReadyState::Closed; }
	bool IsOpen(void) { return m_webSocket.getReadyState() == ix::ReadyState::Open; }
	bool IsClosing(void) { return m_webSocket.getReadyState() == ix::ReadyState::Closing; }
	bool IsConnecting(void) { return m_webSocket.getReadyState() == ix::ReadyState::Connecting; }

	void SetSubscriptionStatus(bool fFlag) noexcept { m_fHaveSubscriptionId = fFlag; }
	bool IsSubscriptable(void) noexcept { return m_fHaveSubscriptionId; }
	int GetSubscriptionId(void) noexcept { ASSERT(m_fHaveSubscriptionId); return m_iSubscriptionId; }
	void SetSubscriptionId(int iSubscriptionId) noexcept { ASSERT(m_fHaveSubscriptionId); m_iSubscriptionId = iSubscriptionId; }

protected:
	ix::WebSocket m_webSocket;

	bool m_fHaveSubscriptionId;
	int m_iSubscriptionId;
	int m_iPingPeriod;
	bool m_fDeflate;

	string m_url;
	string m_inputMessage;
};