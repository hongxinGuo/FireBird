#pragma once

#include <ixwebsocket/IXWebSocket.h>

void FunctionProcessFinnhubWebSocket(const ix::WebSocketMessagePtr& msg);
void FunctionProcessTiingoIEXWebSocket(const ix::WebSocketMessagePtr& msg);
void FunctionProcessTiingoCryptoWebSocket(const ix::WebSocketMessagePtr& msg);
void FunctionProcessTiingoForexWebSocket(const ix::WebSocketMessagePtr& msg);
