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

class CWebSocket : public CObject {
public:
	CWebSocket(bool fHaveSubscriptionId = true);
	virtual ~CWebSocket();

	bool Connecting(string url, const ix::OnMessageCallback& callback, int iPingPeriod = 60, bool fDeflate = true);
	bool Deconnecting(void);

	bool Send(string message);

	ix::ReadyState GetState(void) { return m_webSocket.getReadyState(); }

	void SetSubscriptionStatus(bool fFlag) noexcept { m_fHaveSubscriptionId = fFlag; }
	bool IsSubscriptable(void) noexcept { return m_fHaveSubscriptionId; }
	int GetSubscriptionId(void) noexcept { ASSERT(m_fHaveSubscriptionId); return m_iSubscriptionId; }
	void SetSubscriptionId(int iSubscriptionId) noexcept { ASSERT(m_fHaveSubscriptionId); m_iSubscriptionId = iSubscriptionId; }

	void SetMessagePreffix(string preffix) noexcept { m_Preffix = preffix; }
	string GetMessagePreffix(void) noexcept { return m_Preffix; }
	void SetMessageSuffix(string preffix) noexcept { m_Suffix = preffix; }
	string GetMessageSuffix(void) noexcept { return m_Suffix; }

protected:
	static int sm_iInitIxWebSocket;

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