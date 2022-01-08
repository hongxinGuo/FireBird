#pragma once

#include"VirtualWebSocket.h"

class CTiingoForexWebSocket : public CVirtualWebSocket {
public:
	CTiingoForexWebSocket();
	~CTiingoForexWebSocket(void);

	virtual bool Connect(void);
	virtual bool Send(vector<CString> vSymbol) override;

	bool CreatingThreadConnectingWebSocketAndSendMessage(vector<CString> vSymbol);

	bool ParseTiingoForexWebSocketData(shared_ptr<string> pData);
};
