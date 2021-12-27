#pragma once

#include"VirtualDataWebSocket.h"

class CDataTiingoIEXWebSocket : public CVirtualDataWebSocket {
public:
	CDataTiingoIEXWebSocket();
	~CDataTiingoIEXWebSocket(void);

	virtual bool Connecting(void) override;
	virtual bool Send(vector<CString> vSymbol) override;
};
