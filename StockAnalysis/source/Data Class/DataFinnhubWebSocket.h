#pragma once

#include"VirtualDataWebSocket.h"

class CDataFinnhubWebSocket : public CVirtualDataWebSocket {
public:
	CDataFinnhubWebSocket();
	~CDataFinnhubWebSocket(void);

	virtual bool Connecting(void);
	virtual bool Send(vector<CString> vSymbol) override;
	string CreateFinnhubWebSocketString(CString strSymbol);
};
