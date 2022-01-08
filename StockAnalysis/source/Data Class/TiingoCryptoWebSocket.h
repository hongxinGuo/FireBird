#pragma once

#include"VirtualWebSocket.h"

class CTiingoCryptoWebSocket : public CVirtualWebSocket {
public:
	CTiingoCryptoWebSocket();
	~CTiingoCryptoWebSocket(void);

	virtual bool Connect(void) override;
	virtual bool Send(vector<CString> vSymbol) override;

	bool CreatingThreadConnectingWebSocketAndSendMessage(vector<CString> vSymbol);

	bool ParseTiingoCryptoWebSocketData(shared_ptr<string> pData);
};
