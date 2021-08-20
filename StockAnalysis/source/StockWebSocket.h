////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 尝试将ixWebSocket变量封装于此类中。
// 牵涉太多，不容易封装。
// 此类不负责初始化ixWebSocket。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"pch.h"

using namespace std;
#include<string>

#include <ixwebsocket/IXWebSocket.h>

class CStockWebSocket : public CObject {
public:
	CStockWebSocket(bool fHaveSubscriptionId = true);
	virtual ~CStockWebSocket();

	bool Connecting(string url, const ix::OnMessageCallback& callback, int iPingPeriod = 30, bool fDeflate = true);
	bool Deconnecting(void);

	bool Send(string message);

	ix::ReadyState GetState(void) { return m_webSocket.getReadyState(); }

	void SetSubscriptionStatus(bool fFlag) noexcept { m_fHaveSubscriptionId = fFlag; }
	bool IsSubscriptable(void) noexcept { return m_fHaveSubscriptionId; }
	int GetSubscriptionId(void) noexcept { ASSERT(m_fHaveSubscriptionId); return m_iSubscriptionId; }
	void SetSubscriptionId(int iSubscriptionId) noexcept { ASSERT(m_fHaveSubscriptionId); m_iSubscriptionId = iSubscriptionId; }

	void SetMessagePreffix(string preffix) { m_Preffix = preffix; }
	string GetMessagePreffix(void) { return m_Preffix; }
	void SetMessageSuffix(string preffix) { m_Suffix = preffix; }
	string GetMessageSuffix(void) { return m_Suffix; }

protected:
	ix::WebSocket m_webSocket;
	bool m_fHaveSubscriptionId;
	int m_iSubscriptionId;
	int m_iPingPeriod;
	bool m_fDeflate;

	string m_url;
	string m_Preffix;
	string m_Suffix;
	string m_inputMessage;
};