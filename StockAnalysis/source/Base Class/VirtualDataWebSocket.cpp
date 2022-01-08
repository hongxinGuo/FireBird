#include "pch.h"

#include "VirtualDataWebSocket.h"

bool CVirtualDataWebSocket::ConnectingWebSocketAndSendMessage(vector<CString> vSymbol) {
	ASSERT(IsClosed());

	Connecting();
	while (!IsOpen()) Sleep(1);
	Send(vSymbol);

	return true;
}

CString CVirtualDataWebSocket::CreateTiingoWebSocketSymbolString(vector<CString> vSymbol)
{
	CString strSymbols;
	CString strSymbol;

	for (long l = 0; l < vSymbol.size(); l++) {
		strSymbol = _T("\"") + vSymbol.at(l) + _T("\"") + _T(",");
		strSymbols += strSymbol;
	}
	strSymbols = strSymbols.Left(strSymbols.GetLength() - 1); // 去除最后多余的字符','

	return strSymbols;
}

CString CVirtualDataWebSocket::CreateTiingoWebSocketSymbolString(CString strSymbol)
{
	CString strSymbols = _T("\"") + strSymbol + _T("\"");

	return strSymbols;
}