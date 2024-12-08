#include"pch.h"

#include"globedef.h"

import FireBird.System.Configuration;

#include "SystemMessage.h"
import FireBird.WindowInterface.OutputWnd;

map<long, string> gl_mapHTTPStatusCode{
	// Informational
	{ 100, "Continue" },
	{ 101, "Switching Protocol" },
	{ 102, "Processing(WebDAV)" },
	{ 103, "Early Hints" },

	// Success
	{ 200, "OK" },
	{ 201, "Created" },
	{ 202, "Accepted" },
	{ 203, "Non-Authoritative Information" },
	{ 204, "No Content" },
	{ 205, "Reset Content" },
	{ 206, "Partial Content" },
	{ 207, "Multi - Status(WebDAV)" },
	{ 208, "Already Reported(WebDAV)" },
	{ 226, "IM Used" },

	// Redirection
	{ 300, "Multiple Choices" },
	{ 301, "Moved Permanently" },
	{ 302, "Found" },
	{ 303, "See Other" },
	{ 304, "Not Modified" },
	{ 305, "Use Proxy(Deprecated)" },
	{ 306, "(Unused)" },
	{ 307, "Temporary Redirect" },
	{ 308, "Permanent Redirect(experimental)" },

	// Client Error
	{ 400, "Bad Request" },
	{ 401, "Unauthorized" },
	{ 402, "Payment Requested" },
	{ 403, "Forbidden" },
	{ 404, "Not Found" },
	{ 405, "Method Not Allowed" },
	{ 406, "Not Acceptable" },
	{ 407, "Proxy Authentication Required" },
	{ 408, "Request Timeout" },
	{ 409, "Conflict" },
	{ 410, "Gone" },
	{ 411, "Length Required" },
	{ 412, "Precondition Failed" },
	{ 413, "Request Entity Too Large" },
	{ 414, "Request-URI Too Long" },
	{ 415, "Unsupported Media Type" },
	{ 416, "Requested Range Not Satisfiable" },
	{ 417, "Expectation Failed" },
	{ 418, "I’m a teapot(RFC 2324)" },
	{ 420, "Enhance Your Calm(Twitter)" },
	{ 422, "Unprocessable Entity(WebDAV)" },
	{ 423, "Locked(WebDAV)" },
	{ 424, "Failed Dependency(WebDAV)" },
	{ 425, "Too Early(WebDAV)" },
	{ 426, "Upgrade Required" },
	{ 428, "Precondition Required" },
	{ 429, "Too Many Requests" },
	{ 431, "Request Header Fields Too Large" },
	{ 444, "No Response(Nginx)" },
	{ 449, "Retry With(Microsoft)" },
	{ 450, "Blocked by Windows Parental Controls(Microsoft)" },
	{ 451, "Unavailable For Legal Reasons" },
	{ 499, "Client Closed Request(Nginx)" },

	// Server Error
	{ 500, "Internal Server Error" },
	{ 501, "Not Implemented" },
	{ 502, "Bad Gateway" },
	{ 503, "Service Unavailable" },
	{ 504, "Gateway Timeout" },
	{ 505, "HTTP Version Not Supported(Experimental)" },
	{ 506, "Variant Also Negotiates(Experimental)" },
	{ 507, "Insufficient Storage(WebDAV)" },
	{ 508, "Loop Detected(WebDAV)" },
	{ 510, "Not Extended" },
	{ 511, "Network Authentication Required" },

};

map<long, string> gl_mapNetError{
	{ ERROR_INTERNET_OUT_OF_HANDLES, _T("No more handles could be generated at this time.") },
	{ ERROR_INTERNET_TIMEOUT, _T("The request has timed out.") },
	{ ERROR_INTERNET_EXTENDED_ERROR, _T("An extended error was returned from the server.This is typically a string or buffer containing a verbose error message.Call InternetGetLastResponseInfo to retrieve the error text.") },
	{ ERROR_INTERNET_INTERNAL_ERROR, _T("An internal error has occurred.") },
	{ ERROR_INTERNET_INVALID_URL, _T("The URL is invalid.") },
	{ ERROR_INTERNET_UNRECOGNIZED_SCHEME, _T("The URL scheme could not be recognized or is not supported.") },
	{ ERROR_INTERNET_NAME_NOT_RESOLVED, _T("The server name could not be resolved.") },
	{ ERROR_INTERNET_PROTOCOL_NOT_FOUND, _T("The requested protocol could not be located.") },
	{ ERROR_INTERNET_INVALID_OPTION, _T("A request to InternetQueryOption or InternetSetOption specified an invalid option value.") },
	{ ERROR_INTERNET_BAD_OPTION_LENGTH, _T("The length of an option supplied to InternetQueryOption or InternetSetOption is incorrect for the type of option specified.") },
	{ ERROR_INTERNET_OPTION_NOT_SETTABLE, _T("The request option cannot be set, only queried.") },
	{ ERROR_INTERNET_SHUTDOWN, _T("The Win32 Internet function support is being shut down or	unloaded.") },
	{ ERROR_INTERNET_INCORRECT_USER_NAME, _T("The request to connect and log on to an FTP server could	not be completed because the supplied user name is incorrect.") },
	{ ERROR_INTERNET_INCORRECT_PASSWORD, _T("The request to connect and log on to an FTP server could not be completed because the supplied password is incorrect.") },
	{ ERROR_INTERNET_LOGIN_FAILURE, _T("The request to connect to and log on to an FTP server	failed.") },
	{ ERROR_INTERNET_INVALID_OPERATION, _T("The requested operation is invalid.") },
	{ ERROR_INTERNET_OPERATION_CANCELLED, _T("The operation was canceled, usually because the handle on which the request was operating was closed before the operation completed.") },
	{ ERROR_INTERNET_INCORRECT_HANDLE_TYPE, _T("The type of handle supplied is incorrect for this operation.") },
	{ ERROR_INTERNET_INCORRECT_HANDLE_STATE, _T("The requested operation cannot be carried out because the	handle supplied is not in the correct state.") },
	{ ERROR_INTERNET_NOT_PROXY_REQUEST, _T("The request cannot be made via a proxy.") },
	{ ERROR_INTERNET_REGISTRY_VALUE_NOT_FOUND, _T("A required registry value could not be located.") },
	{ ERROR_INTERNET_BAD_REGISTRY_PARAMETER, _T("A required registry value was located but is an incorrect	type or has an invalid value.") },
	{ ERROR_INTERNET_NO_DIRECT_ACCESS, _T("Direct network access cannot be made at this time.") },
	{ ERROR_INTERNET_NO_CONTEXT, _T("An asynchronous request could not be made because a zero context value was supplied.") },
	{ ERROR_INTERNET_NO_CALLBACK, _T("An asynchronous request could not be made because a callback function has not been set.") },
	{ ERROR_INTERNET_REQUEST_PENDING, _T("The required operation could not be completed because one	or more requests are pending.") },
	{ ERROR_INTERNET_INCORRECT_FORMAT, _T("The format of the request is invalid.") },
	{ ERROR_INTERNET_ITEM_NOT_FOUND, _T("The requested item could not be located.") },
	{ ERROR_INTERNET_CANNOT_CONNECT, _T("The attempt to connect to the server failed.") },
	{ ERROR_INTERNET_CONNECTION_ABORTED, _T("The connection with the server has been terminated.") },
	{ ERROR_INTERNET_CONNECTION_RESET, _T("The connection with the server has been reset.") },
	{ ERROR_INTERNET_FORCE_RETRY, _T("Calls for the Win32 Internet function to redo the request.") },
	{ ERROR_INTERNET_INVALID_PROXY_REQUEST, _T("The request to the proxy was invalid.") },
	{ ERROR_INTERNET_HANDLE_EXISTS, _T("The request failed because the handle already exists.") },
	{ ERROR_INTERNET_SEC_CERT_DATE_INVALID, _T("SSL certificate date that was received from the server is	bad.The certificate is expired.") },
	{ ERROR_INTERNET_SEC_CERT_CN_INVALID, _T("SSL certificate common name(host name field) is incorrect. For example, if you entered www.server.com and the common name on the certificate says www.different.com.") },
	{ ERROR_INTERNET_HTTP_TO_HTTPS_ON_REDIR, _T("The application is moving from a non - SSL to an SSL	connection because of a redirect.") },
	{ ERROR_INTERNET_HTTPS_TO_HTTP_ON_REDIR, _T("The application is moving from an SSL to an non - SSL connection because of a redirect.") },
	{ ERROR_INTERNET_MIXED_SECURITY, _T("Indicates that the content is not entirely secure.Some of the content being viewed may have come from unsecured servers.") },
	{ ERROR_INTERNET_CHG_POST_IS_NON_SECURE, _T("The application is posting and attempting to change	multiple lines of text on a server that is not secure.") },
	{ ERROR_INTERNET_POST_IS_NON_SECURE, _T("The application is posting data to a server that is not secure.") },
	{ ERROR_FTP_TRANSFER_IN_PROGRESS, _T("The requested operation cannot be made on the FTP session handle because an operation is already in progress.") },
	{ ERROR_FTP_DROPPED, _T("The FTP operation was not completed because the session was aborted.") },
	{ ERROR_GOPHER_PROTOCOL_ERROR, _T("An error was detected while parsing data returned from the gopher server.") },
	{ ERROR_GOPHER_NOT_FILE, _T("The request must be made for a file locator.") },
	{ ERROR_GOPHER_DATA_ERROR, _T("An error was detected while receiving data from the gopher	server.") },
	{ ERROR_GOPHER_END_OF_DATA, _T("The end of the data has been reached.") },
	{ ERROR_GOPHER_INVALID_LOCATOR, _T("The supplied locator is not valid.") },
	{ ERROR_GOPHER_INCORRECT_LOCATOR_TYPE, _T("The type of the locator is not correct for this operation.") },
	{ ERROR_GOPHER_NOT_GOPHER_PLUS, _T("The requested operation can only be made against a Gopher +	server or with a locator that specifies a Gopher + operation.") },
	{ ERROR_GOPHER_ATTRIBUTE_NOT_FOUND, _T("The requested attribute could not be located.") },
	{ ERROR_GOPHER_UNKNOWN_LOCATOR, _T("The locator type is unknown.") },
	{ ERROR_HTTP_HEADER_NOT_FOUND, _T("The requested header could not be located.") },
	{ ERROR_HTTP_DOWNLEVEL_SERVER, _T("The server did not return any headers.") },
	{ ERROR_HTTP_INVALID_SERVER_RESPONSE, _T("The server response could not be parsed.") },
	{ ERROR_HTTP_INVALID_HEADER, _T("The supplied header is invalid.") },
	{ ERROR_HTTP_INVALID_QUERY_REQUEST, _T("The request made to HttpQueryInfo is invalid.") },
	{ ERROR_HTTP_HEADER_ALREADY_EXISTS, _T("The header could not be added because it already exists.") },
	{ ERROR_HTTP_REDIRECT_FAILED, _T("The redirection failed because either the scheme changed(for example, HTTP to FTP) or all attempts made to redirect	failed(default is five attempts).") },
};

binary_semaphore s_RTDataStockCode{ 1 };
binary_semaphore s_DayLineDataStockCode{ 1 };
binary_semaphore s_CurrentFinnhubWebSocketStake{ 1 };
binary_semaphore s_CurrentTiingoWebSocketIEX{ 1 };
binary_semaphore s_CurrentTiingoWebSocketCrypto{ 1 };
binary_semaphore s_CurrentTiingoWebSocketForex{ 1 };
binary_semaphore s_sReadCurrentFinnhubFunction{ 1 };
binary_semaphore s_sReadCurrentTiingoFunction{ 1 };

CSystemDeque::CSystemDeque() {}

CSystemDeque::~CSystemDeque() {}

CSystemMessage::~CSystemMessage() = default;

void CSystemMessage::SetStockCodeForInquiringRTData(const CString& strStockCode) {
	s_RTDataStockCode.acquire();
	m_strStockCodeForInquiringRTData = strStockCode;
	s_RTDataStockCode.release();
}

CString CSystemMessage::GetStockCodeForInquiringRTData() const {
	s_RTDataStockCode.acquire();
	CString s = m_strStockCodeForInquiringRTData;
	s_RTDataStockCode.release();
	return s;
}

void CSystemMessage::SetStockCodeForInquiryDayLine(const CString& strStockCode) {
	s_DayLineDataStockCode.acquire();
	m_strStockCodeForInquiryDayLine = strStockCode;
	s_DayLineDataStockCode.release();
}

CString CSystemMessage::GetStockCodeForInquiryDayLine() const {
	s_DayLineDataStockCode.acquire();
	CString s = m_strStockCodeForInquiryDayLine;
	s_DayLineDataStockCode.release();
	return s;
}

void CSystemMessage::SetCurrentFinnhubWebSocketStake(const CString& s) {
	s_CurrentFinnhubWebSocketStake.acquire();
	m_strCurrentFinnhubWebSocketStake = s;
	s_CurrentFinnhubWebSocketStake.release();
}

CString CSystemMessage::GetCurrentFinnhubWebSocketStake() const {
	s_CurrentFinnhubWebSocketStake.acquire();
	CString s = m_strCurrentFinnhubWebSocketStake;
	s_CurrentFinnhubWebSocketStake.release();
	return s;
}

void CSystemMessage::SetCurrentTiingoWebSocketIEX(const CString& s) {
	s_CurrentTiingoWebSocketIEX.acquire();
	m_strCurrentTiingoWebSocketIEX = s;
	s_CurrentTiingoWebSocketIEX.release();
}

CString CSystemMessage::GetCurrentTiingoWebSocketIEX() const {
	s_CurrentTiingoWebSocketIEX.acquire();
	CString s = m_strCurrentTiingoWebSocketIEX;
	s_CurrentTiingoWebSocketIEX.release();
	return s;
}

void CSystemMessage::SetCurrentTiingoWebSocketForex(const CString& s) {
	s_CurrentTiingoWebSocketForex.acquire();
	m_strCurrentTiingoWebSocketForex = s;
	s_CurrentTiingoWebSocketForex.release();
}

CString CSystemMessage::GetCurrentTiingoWebSocketForex() const {
	s_CurrentTiingoWebSocketForex.acquire();
	CString s = m_strCurrentTiingoWebSocketForex;
	s_CurrentTiingoWebSocketForex.release();
	return s;
}

void CSystemMessage::SetCurrentTiingoWebSocketCrypto(const CString& s) {
	s_CurrentTiingoWebSocketCrypto.acquire();
	m_strCurrentTiingoWebSocketCrypto = s;
	s_CurrentTiingoWebSocketCrypto.release();
}

CString CSystemMessage::GetCurrentTiingoWebSocketCrypto() const {
	s_CurrentTiingoWebSocketCrypto.acquire();
	CString s = m_strCurrentTiingoWebSocketCrypto;
	s_CurrentTiingoWebSocketCrypto.release();
	return s;
}

CString CSystemMessage::GetCurrentFinnhubFunction() const {
	s_sReadCurrentFinnhubFunction.acquire();
	CString str = m_strCurrentFinnhubFunction;
	s_sReadCurrentFinnhubFunction.release();
	return str;
}

void CSystemMessage::SetCurrentFinnhubFunction(const CString& str) {
	s_sReadCurrentFinnhubFunction.acquire();
	m_strCurrentFinnhubFunction = str;
	s_sReadCurrentFinnhubFunction.release();
}

CString CSystemMessage::GetCurrentTiingoFunction() const {
	s_sReadCurrentTiingoFunction.acquire();
	CString str = m_strCurrentTiingoFunction;
	s_sReadCurrentTiingoFunction.release();
	return str;
}

void CSystemMessage::SetCurrentTiingoFunction(const CString& str) {
	s_sReadCurrentTiingoFunction.acquire();
	m_strCurrentTiingoFunction = str;
	s_sReadCurrentTiingoFunction.release();
}

void CSystemDeque::Display(COutputList* pOutputList, const CString& strTime) {
	size_t lTotal = Size();
	if (lTotal > pOutputList->GetLineNumber()) lTotal = pOutputList->GetLineNumber();
	for (int i = 0; i < lTotal; i++) {
		CString str2 = strTime + _T(": ") + PopMessage();
		SysCallOutputListAddString(pOutputList, str2);
	}
}

void CSystemDeque::SysCallOutputListAddString(COutputList* pOutputList, const CString& str) {
	pOutputList->AddString(str);
}

void CSystemDeque::PushMessage(const CString& str) {
	m_queueMessage.enqueue(str.GetString());
}

CString CSystemDeque::PopMessage() {
	string str;
	if (m_queueMessage.try_dequeue(str)) {
		return str.c_str();
	}
	else return _T("");
}

size_t CSystemDeque::Size() const {
	return m_queueMessage.size_approx();
}

CSystemMessage::CSystemMessage() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	if (static int siCounter = 0; siCounter++ > 0) {
		TRACE("系统消息只允许一个实例\n");
		gl_systemMessage.PushErrorMessage(_T("错误：系统不允许生成多个CSystemMessage实例"));
	}

	m_iProcessedFinnhubWebSocket = 0;
	m_iProcessedTiingoIEXWebSocket = 0;
	m_iProcessedTiingoCryptoWebSocket = 0;
	m_iProcessedTiingoForexWebSocket = 0;
}
