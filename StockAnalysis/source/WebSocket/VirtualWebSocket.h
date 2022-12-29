////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 尝试将ixWebSocket变量封装于此类中。
// 牵涉太多，不容易封装。
// 此类负责ixWebSocket的初始化。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include<string>
#include<map>
#include<vector>
using std::string;
using std::map;
using std::vector;

#include <ixwebsocket/IXWebSocket.h>

#include"TemplateMutexAccessQueue.h"

using vectorString = vector<string>;

class CVirtualWebSocket : public CObject {
public:
	CVirtualWebSocket(bool fHaveSubscriptionId = true);
	~CVirtualWebSocket() override;
	void Reset(void);

	virtual bool Connect(void) = 0;
	virtual bool Send(vector<string> vSymbol) = 0;
	bool ConnectWebSocketAndSendMessage(vectorString vSymbol);

	string CreateTiingoWebSocketSymbolString(vectorString vSymbol);

	bool IsSymbol(string sSymbol);
	void AppendSymbol(vectorString vSymbol);
	bool AddSymbol(const string& sSymbol);
	bool DeleteSymbol(const string& sSymbol);
	void ClearSymbol(void);
	size_t GetSymbolSize(void) const noexcept { return m_vSymbol.size(); }

	// 状态
	ix::ReadyState GetState(void) const { return m_webSocket.getReadyState(); }
	bool IsClosed(void) const { return m_webSocket.getReadyState() == ix::ReadyState::Closed; }
	bool IsOpen(void) const { return m_webSocket.getReadyState() == ix::ReadyState::Open; }
	bool IsClosing(void) const { return m_webSocket.getReadyState() == ix::ReadyState::Closing; }
	bool IsConnecting(void) const { return m_webSocket.getReadyState() == ix::ReadyState::Connecting; }

	string GetURL(void) noexcept { return m_url; }
	void SetURL(const string& url) noexcept { m_url = url; }

	void SetSubscriptionStatus(const bool fFlag) noexcept { m_fHaveSubscriptionId = fFlag; }
	bool IsSubscriptable(void) const noexcept { return m_fHaveSubscriptionId; }

	int GetSubscriptionId(void) const noexcept {
		ASSERT(m_fHaveSubscriptionId);
		return m_iSubscriptionId;
	}

	void SetSubscriptionId(const int iSubscriptionId) noexcept {
		ASSERT(m_fHaveSubscriptionId);
		m_iSubscriptionId = iSubscriptionId;
	}

	bool IsReceivingData(void) const noexcept { return m_fReceivingData; }
	void SetReceivingData(const bool fFlag) noexcept { m_fReceivingData = fFlag; }

	// 实现
	bool Connecting(string url, const ix::OnMessageCallback& callback, int iPingPeriod = 60, bool fDeflate = true);
	bool Disconnect(void);
	bool CreateThreadDisconnectWebSocket(void);
	// 用于系统退出时。
	bool DisconnectWithoutWaitingSucceed(void); // 用于程序运行中途时切断网络链接，此时无需等待。
	bool SendMessage(const string& message);

	vectorString m_vCurrentSymbol;

	size_t DataSize(void) { return m_qWebSocketData.Size(); }

	void PushData(string data) {
		const shared_ptr<string> pData = make_shared<string>(data);
		m_qWebSocketData.PushData(pData);
	}

	void PushData(const shared_ptr<string> pData) { m_qWebSocketData.PushData(pData); }
	shared_ptr<string> PopData(void) { return m_qWebSocketData.PopData(); }

protected:
	ix::WebSocket m_webSocket;
	string m_url;

	bool m_fHaveSubscriptionId;
	int m_iSubscriptionId;
	vectorString m_vSymbol;
	map<string, size_t> m_mapSymbol;

	string m_inputMessage;

	bool m_fReceivingData; // 正在接收数据
	CTemplateMutexAccessQueue<string> m_qWebSocketData; // 接收到的WebSocket数据
};
