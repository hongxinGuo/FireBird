#pragma once

#include"VirtualWebSocket.h"

class CTiingoIEXWebSocket : public CVirtualWebSocket {
public:
	CTiingoIEXWebSocket();
	~CTiingoIEXWebSocket(void);

	virtual bool Connect(void) override;
	virtual bool Send(vector<CString> vSymbol) override;

	bool CreatingThreadConnectingWebSocketAndSendMessage(vector<CString> vSymbol);

	bool ParseTiingoIEXWebSocketData(shared_ptr<string> pData);
};
