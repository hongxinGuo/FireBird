#pragma once

#include"VirtualWebSocket.h"

class CFinnhubWebSocket : public CVirtualWebSocket {
public:
	CFinnhubWebSocket();
	~CFinnhubWebSocket(void);

	virtual bool Connect(void);
	virtual bool Send(vector<CString> vSymbol) override;
	string CreateFinnhubWebSocketString(CString strSymbol);

	bool CreatingThreadConnectingWebSocketAndSendMessage(vector<CString> vSymbol);

	bool ParseFinnhubWebSocketData(shared_ptr<string> pData);
};
