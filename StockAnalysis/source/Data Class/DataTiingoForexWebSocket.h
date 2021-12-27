#pragma once

#include"VirtualDataWebSocket.h"

class CDataTiingoForexWebSocket : public CVirtualDataWebSocket {
public:
	CDataTiingoForexWebSocket();
	~CDataTiingoForexWebSocket(void);

	virtual bool Connecting(void);
	virtual bool Send(vector<CString> vSymbol) override;
};
