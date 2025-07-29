////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 使用https://github.com/machinezone/IXWebSocket。
// 尝试将ixWebSocket变量封装于此类中。此类负责ixWebSocket的初始化。
//
// ix::initNetSystem();// Note 在Windows环境下，IXWebSocket库需要初始化一次，且只能初始化一次，此操作由MainFrame的构造函数完成
// ix::uninitNetSystem();// Note 退出系统时，析构IXWebSocket库，且只能析构一次。此操作由MainFrame的析构函数完成
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <ixwebsocket/IXWebSocket.h>

#include"concurrentqueue/concurrentqueue.h"
using namespace moodycamel;

using std::string;
using std::map;
using std::vector;

using vectorString = vector<string>;
class CVirtualWebSocket;
using CVirtualWebSocketPtr = shared_ptr<CVirtualWebSocket>;

class CVirtualWebSocket : public std::enable_shared_from_this<CVirtualWebSocket> {
public:
	CVirtualWebSocket();
	virtual ~CVirtualWebSocket();

	std::shared_ptr<CVirtualWebSocket> GetShared() { return shared_from_this(); }
	void Reset();

	virtual void TaskConnectAndSendMessage(vectorString vSymbol);
	virtual void TaskDisconnect();
	bool ConnectAndSendMessage(const vectorString& vSymbol);
	void Disconnect();

	virtual bool ParseWebSocketData(shared_ptr<string> pData) { return true; }

protected:
	virtual void Connect() { ASSERT(false); }
	virtual void Send(const vectorString&) { ASSERT(FALSE); }
	void Connecting(const string& url, const ix::OnMessageCallback& callback, int iPingPeriod = 60, bool fDeflate = true);
	virtual void StartWebSocket() { m_webSocket.start(); } // start()为异步的。为了测试，将此函数声明为虚函数
	virtual void StopWebSocket() { m_webSocket.stop(); } // stop()是同步的。为了测试，将此函数声明为虚函数

public:
	void MonitorWebSocket(bool fDataSourceError, bool fWebSocketOpened, const vectorString& vSymbol);

public:
	bool IsSymbol(const string& sSymbol) const { return m_mapSymbol.contains(sSymbol); }
	void AppendSymbol(const vectorString& vSymbol);
	bool AddSymbol(const string& sSymbol);
	bool DeleteSymbol(const string& sSymbol);
	void ClearSymbol();
	size_t GetSymbolSize() const noexcept { return m_vSymbol.size(); }

	// 状态
	virtual ix::ReadyState GetState() const { return m_webSocket.getReadyState(); } // 为了测试，将此函数声明为虚函数
	bool IsClosed() const { return GetState() == ix::ReadyState::Closed; }
	bool IsOpen() const { return GetState() == ix::ReadyState::Open; }
	bool IsClosing() const { return GetState() == ix::ReadyState::Closing; }
	bool IsConnecting() const { return GetState() == ix::ReadyState::Connecting; }

	int GetStatusCode() const noexcept { return m_iStatusCode; }
	void SetStatusCode(int iCode) noexcept { m_iStatusCode = iCode; }
	void SetError(bool fFlag) { m_fError = fFlag; }
	bool IsError() const { return m_fError; }
	string GetStatusMessage() const noexcept { return m_statusMessage; }
	void SetStatusMessage(const string& sMessage) noexcept { m_statusMessage = sMessage; }

	string GetURL() noexcept { return m_url; }
	void SetURL(const string& url) noexcept { m_url = url; }

	int GetSubscriptionId() const noexcept { return m_iSubscriptionId; }
	void SetSubscriptionId(const int iSubscriptionId) noexcept { m_iSubscriptionId = iSubscriptionId; }

	time_t GetHeartbeatTime() const noexcept { return m_HeartbeatTime; }
	void SetHeartbeatTime(const time_t tt) noexcept { m_HeartbeatTime = tt; }
	bool IsIdle(time_t tPeriod = 300) const; // 默认五分钟

	void PushData(const string& data) {
		const auto pData = make_shared<string>(data);
		m_qWebSocketData.enqueue(pData);
	}
	void PushData(const shared_ptr<string>& pData) { m_qWebSocketData.enqueue(pData); }
	shared_ptr<string> PopData() {
		shared_ptr<string> pString;
		m_qWebSocketData.try_dequeue(pString);
		return pString;
	}
	size_t DataSize() const { return m_qWebSocketData.size_approx(); }

public:
	vectorString m_vCurrentInquireSymbol;

protected:
	ix::WebSocket m_webSocket;
	string m_url{ "" };

	int m_iStatusCode{ 0 }; // WebSocket返回的状态码。正确：200， 错误：400等。
	bool m_fError{ false };
	string m_statusMessage{ "" }; // 正确时为状态信息，错误时为错误信息。
	int m_iSubscriptionId{ 0 };

	vectorString m_vSymbol;
	map<string, size_t> m_mapSymbol;

	time_t m_HeartbeatTime{ 0 }; // 最新心跳时间， UTC制式

	ConcurrentQueue<shared_ptr<string>> m_qWebSocketData; // 接收到的WebSocket数据
};

using CVirtualWebSocketPtr = shared_ptr<CVirtualWebSocket>;
