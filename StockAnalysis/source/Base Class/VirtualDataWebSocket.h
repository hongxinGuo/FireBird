#pragma once

#include"WebSocket.h"

class CVirtualDataWebSocket : public CObject {
public:
	CVirtualDataWebSocket() {}
	~CVirtualDataWebSocket(void) {}

	void SetSubscriptionStatus(bool fFlag) noexcept { m_webSocket.SetSubscriptionStatus(fFlag); }
	bool IsSubscriptable(void) noexcept { return m_webSocket.IsSubscriptable(); }
	int GetSubscriptionId(void) noexcept { ASSERT(m_webSocket.IsSubscriptable()); return m_webSocket.GetSubscriptionId(); }
	void SetSubscriptionId(int iSubscriptionId) noexcept { ASSERT(m_webSocket.IsSubscriptable()); m_webSocket.SetSubscriptionId(iSubscriptionId); }

	bool IsClosed(void) { return m_webSocket.IsClosed() ? true : false; }
	bool IsOpen(void) { return m_webSocket.IsOpen() ? true : false; }
	bool IsClosing(void) { return m_webSocket.IsClosing() ? true : false; }

	virtual bool Connecting(void) = 0;
	virtual bool Send(vector<CString> vSymbol) = 0;
	bool Deconnecting(void) { return 	m_webSocket.Deconnecting(); }

	bool ConnectingWebSocketAndSendMessage(vector<CString> vSymbol);

	CString CreateTiingoWebSocketSymbolString(vector<CString> vSymbol);
	CString CreateTiingoWebSocketSymbolString(CString strSymbol);

protected:
	CWebSocket m_webSocket;
};
