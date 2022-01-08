#pragma once

#include"VirtualDataWebSocket.h"

class CDataTiingoCryptoWebSocket : public CVirtualDataWebSocket {
public:
	CDataTiingoCryptoWebSocket();
	~CDataTiingoCryptoWebSocket(void);

	virtual bool Connecting(void) override;
	virtual bool Send(vector<CString> vSymbol) override;

	bool CreatingThreadConnectingWebSocketAndSendMessage(vector<CString> vSymbol);
};
