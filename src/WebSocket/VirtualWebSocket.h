////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ʹ��https://github.com/machinezone/IXWebSocket��
// ���Խ�ixWebSocket������װ�ڴ����С����ฺ��ixWebSocket�ĳ�ʼ����
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <ixwebsocket/IXWebSocket.h>

#include"TemplateMutexAccessQueue.h"

using std::string;
using std::map;
using std::vector;

using vectorString = vector<string>;

class CVirtualWebSocket : public std::enable_shared_from_this<CVirtualWebSocket> {
public:
	CVirtualWebSocket(bool fHaveSubscriptionId = true);
	virtual ~CVirtualWebSocket();

	std::shared_ptr<CVirtualWebSocket> GetShared() { return shared_from_this(); }

	bool ConnectWebSocketAndSendMessage(const vectorString& vSymbol);
	void Reset();

	virtual void Connect() { ASSERT(false); }
	void Disconnect();
	virtual void Send(const vectorString&) { ASSERT(FALSE); }
	auto SendString(const string& strMessage) { return m_webSocket.send(strMessage); }

	virtual void CreateThreadConnectWebSocketAndSendMessage(vectorString vSymbol) { ASSERT(false); } // �̳������ʵ�ָ��ԵĹ���

	bool IsSymbol(const string& sSymbol) const;
	void AppendSymbol(const vectorString& vSymbol);
	bool AddSymbol(const string& sSymbol);
	bool DeleteSymbol(const string& sSymbol);
	void ClearSymbol();
	size_t GetSymbolSize() const noexcept { return m_vSymbol.size(); }

	// ״̬
	virtual ix::ReadyState GetState() const { return m_webSocket.getReadyState(); }
	bool IsClosed() const { return GetState() == ix::ReadyState::Closed; }
	bool IsOpen() const { return GetState() == ix::ReadyState::Open; }
	bool IsClosing() const { return GetState() == ix::ReadyState::Closing; }
	bool IsConnecting() const { return GetState() == ix::ReadyState::Connecting; }

	virtual void StartWebSocket() { m_webSocket.start(); }
	virtual void StopWebSocket() { m_webSocket.stop(); }

	int GetStatusCode() const noexcept { return m_iStatusCode; }
	void SetStatusCode(int iCode) noexcept { m_iStatusCode = iCode; }
	void SetError(bool fFlag) { m_fError = fFlag; }
	bool IsError() const { return m_fError; }
	string GetStatusMessage() const noexcept { return m_statusMessage; }
	void SetStatusMessage(const string& sMessage) noexcept { m_statusMessage = sMessage; }

	string GetURL() noexcept { return m_url; }
	void SetURL(const string& url) noexcept { m_url = url; }

	void SetSubscriptionStatus(const bool fFlag) noexcept { m_fHaveSubscriptionId = fFlag; }
	bool IsSubscriptable() const noexcept { return m_fHaveSubscriptionId; }

	int GetSubscriptionId() const noexcept {
		ASSERT(m_fHaveSubscriptionId);
		return m_iSubscriptionId;
	}

	void SetSubscriptionId(const int iSubscriptionId) noexcept {
		ASSERT(m_fHaveSubscriptionId);
		m_iSubscriptionId = iSubscriptionId;
	}

	bool IsReceivingData() const noexcept { return m_fReceivingData; }
	void SetReceivingData(const bool fFlag) noexcept { m_fReceivingData = fFlag; }

	time_t GetHeartbeatTime() const noexcept { return m_HeartbeatTime; }
	void SetHeartbeatTime(const time_t tt) noexcept { m_HeartbeatTime = tt; }
	bool IsIdle(time_t tPeriod = 300) const; // Ĭ�������

	// ʵ��
	void Connecting(const string& url, const ix::OnMessageCallback& callback, int iPingPeriod = 60, bool fDeflate = true);
	bool CreateThreadDisconnectWebSocket();	// ����ϵͳ�˳�ʱ��
	bool DisconnectWithoutWaitingSucceed(); // ���ڳ���������;ʱ�ж��������ӣ���ʱ����ȴ���

	vectorString m_vCurrentSymbol;

	size_t DataSize() { return m_qWebSocketData.Size(); }

	void PushData(string data) {
		const auto pData = make_shared<string>(data);
		m_qWebSocketData.PushData(pData);
	}

	void PushData(const shared_ptr<string>& pData) { m_qWebSocketData.PushData(pData); }
	shared_ptr<string> PopData() { return m_qWebSocketData.PopData(); }

protected:
	ix::WebSocket m_webSocket;
	string m_url;

	int m_iStatusCode; // WebSocket���ص�״̬�롣��ȷ��200�� ����400�ȡ�
	bool m_fError{false};
	string m_statusMessage; // ��ȷʱΪ״̬��Ϣ������ʱΪ������Ϣ��
	bool m_fHaveSubscriptionId;
	int m_iSubscriptionId;
	vectorString m_vSymbol;
	map<string, size_t> m_mapSymbol;

	string m_inputMessage;

	bool m_fReceivingData; // ���ڽ�������
	time_t m_HeartbeatTime; // ��������ʱ�䣬 UTC��ʽ

	CTemplateMutexAccessQueue<string> m_qWebSocketData; // ���յ���WebSocket����
};

using CVirtualWebSocketPtr = shared_ptr<CVirtualWebSocket>;
