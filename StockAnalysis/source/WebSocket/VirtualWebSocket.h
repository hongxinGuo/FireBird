////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���Խ�ixWebSocket������װ�ڴ����С�
// ǣ��̫�࣬�����׷�װ��
// ���ฺ��ixWebSocket�ĳ�ʼ����
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


#include<string>
#include<map>

#include <ixwebsocket/IXWebSocket.h>

#include"TemplateMutexAccessQueue.h"

class CVirtualWebSocket : public CObject {
public:
	CVirtualWebSocket(bool fHaveSubscriptionId = true);
	~CVirtualWebSocket() override;
	void Reset(void);

	virtual bool Connect(void) = 0;
	virtual bool Send(vector<CString> vSymbol) = 0;
	bool ConnectWebSocketAndSendMessage(vector<CString> vSymbol);

	CString CreateTiingoWebSocketSymbolString(vector<CString> vSymbol);

	bool IsSymbol(CString strSymbol);
	void AppendSymbol(vector<CString> vSymbol);
	bool AddSymbol(CString strSymbol);
	bool DeleteSymbol(CString strSymbol);
	void ClearSymbol(void);
	size_t GetSymbolSize(void) noexcept { return m_vSymbol.size(); }

	// ״̬
	ix::ReadyState GetState(void) const { return m_webSocket.getReadyState(); }
	bool IsClosed(void) const { return m_webSocket.getReadyState() == ix::ReadyState::Closed; }
	bool IsOpen(void) const { return m_webSocket.getReadyState() == ix::ReadyState::Open; }
	bool IsClosing(void) const { return m_webSocket.getReadyState() == ix::ReadyState::Closing; }
	bool IsConnecting(void) const { return m_webSocket.getReadyState() == ix::ReadyState::Connecting; }

	string GetURL(void) noexcept { return m_url; }
	void SetURL(string const url) noexcept { m_url = url; }

	void SetSubscriptionStatus(bool fFlag) noexcept { m_fHaveSubscriptionId = fFlag; }
	bool IsSubscriptable(void) const noexcept { return m_fHaveSubscriptionId; }
	int GetSubscriptionId(void) noexcept { ASSERT(m_fHaveSubscriptionId); return m_iSubscriptionId; }
	void SetSubscriptionId(int iSubscriptionId) noexcept { ASSERT(m_fHaveSubscriptionId); m_iSubscriptionId = iSubscriptionId; }

	bool IsReceivingData(void) const noexcept { return m_fReveivingData; }
	void SetReceivingData(bool fFlag) noexcept { m_fReveivingData = fFlag; }

	// ʵ��
	bool Connecting(string url, const ix::OnMessageCallback& callback, int iPingPeriod = 60, bool fDeflate = true);
	bool Disconnect(void);
	bool CreateThreadDisconnectWebSocket(void);
	// ����ϵͳ�˳�ʱ��
	bool DisconnectWithoutWaitingSucceed(void); // ���ڳ���������;ʱ�ж��������ӣ���ʱ����ȴ���
	bool SendMessage(const string& message);

	vector<CString> m_vCurrentSymbol;

	size_t DataSize(void) { return m_qWebSocketData.Size(); }
	void PushData(string data) { shared_ptr<string> pData = make_shared<string>(data); m_qWebSocketData.PushData(pData); }
	void PushData(shared_ptr<string> pData) { m_qWebSocketData.PushData(pData); }
	shared_ptr<string> PopData(void) { return m_qWebSocketData.PopData(); }

protected:
	ix::WebSocket m_webSocket;
	string m_url;

	bool m_fHaveSubscriptionId;
	int m_iSubscriptionId;
	vector<CString> m_vSymbol;
	map<CString, long> m_mapSymbol;

	string m_inputMessage;

	bool m_fReveivingData; // ���ڽ�������
	CTemplateMutexAccessQueue<string> m_qWebSocketData; // ���յ���WebSocket����
};