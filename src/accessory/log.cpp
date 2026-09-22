#include"pch.h"

#include "log.h"
#include"systemMessage.h"

using namespace std;

map<long, string> gl_mapHTTPStatusCode{
	// 虚拟状态码 
	{ 0, "No Response" },
	// Informational
	{ 100, "Continue" },
	{ 101, "Switching Protocols" },
	{ 102, "Processing" },
	{ 103, "Early Hints" },

	// Success
	{ 200, "OK" },
	{ 201, "Created" },
	{ 202, "Accepted" },
	{ 203, "Non-Authoritative Information" },
	{ 204, "No Content" },
	{ 205, "Reset Content" },
	{ 206, "Partial Content" },
	{ 207, "Multi-Status" },
	{ 208, "Already Reported" },
	{ 226, "IM Used" },

	// Redirection
	{ 300, "Multiple Choices" },
	{ 301, "Moved Permanently" },
	{ 302, "Found" },
	{ 303, "See Other" },
	{ 304, "Not Modified" },
	{ 305, "Use Proxy" },
	{ 306, "(Unused)" },
	{ 307, "Temporary Redirect" },
	{ 308, "Permanent Redirect" },

	// Client Error
	{ 400, "Bad Request" },
	{ 401, "Unauthorized" },
	{ 402, "Payment Required" },
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
	{ 418, "I’m a teapot" },
	{ 420, "Enhance Your Calm" },
	{ 422, "Unprocessable Entity" },
	{ 423, "Locked" },
	{ 424, "Failed Dependency" },
	{ 425, "Too Early" },
	{ 426, "Upgrade Required" },
	{ 428, "Precondition Required" },
	{ 429, "Too Many Requests" },
	{ 431, "Request Header Fields Too Large" },
	{ 444, "Connection Closed Without Response" },
	{ 449, "Retry With" },
	{ 450, "Blocked by Windows Parental Controls" },
	{ 451, "Unavailable For Legal Reasons" },
	{ 499, "Client Closed Request" },

	// Server Error
	{ 500, "Internal Server Error" },
	{ 501, "Not Implemented" },
	{ 502, "Bad Gateway" },
	{ 503, "Service Unavailable" },
	{ 504, "Gateway Timeout" },
	{ 505, "HTTP Version Not Supported" },
	{ 506, "Variant Also Negotiates" },
	{ 507, "Insufficient Storage" },
	{ 508, "Loop Detected" },
	{ 510, "Not Extended" },
	{ 511, "Network Authentication Required" },

};

map<long, string> gl_mapNetError{
	{ NET_ERROR_INTERNET_OUT_OF_HANDLES, "No more handles could be generated at this time." },
	{ NET_ERROR_INTERNET_TIMEOUT, "The request has timed out." },
	{ NET_ERROR_INTERNET_EXTENDED_ERROR, "An extended error was returned from the server.This is typically a string or buffer containing a verbose error message.Call InternetGetLastResponseInfo to retrieve the error text." },
	{ NET_ERROR_INTERNET_INTERNAL_ERROR, "An internal error has occurred." },
	{ NET_ERROR_INTERNET_INVALID_URL, "The URL is invalid." },
	{ NET_ERROR_INTERNET_UNRECOGNIZED_SCHEME, "The URL scheme could not be recognized or is not supported." },
	{ NET_ERROR_INTERNET_NAME_NOT_RESOLVED, "The server name could not be resolved." },
	{ NET_ERROR_INTERNET_PROTOCOL_NOT_FOUND, "The requested protocol could not be located." },
	{ NET_ERROR_INTERNET_INVALID_OPTION, "A request to InternetQueryOption or InternetSetOption specified an invalid option value." },
	{ NET_ERROR_INTERNET_BAD_OPTION_LENGTH, "The length of an option supplied to InternetQueryOption or InternetSetOption is incorrect for the type of option specified." },
	{ NET_ERROR_INTERNET_OPTION_NOT_SETTABLE, "The request option cannot be set, only queried." },
	{ NET_ERROR_INTERNET_SHUTDOWN, "The Win32 Internet function support is being shut down or	unloaded." },
	{ NET_ERROR_INTERNET_INCORRECT_USER_NAME, "The request to connect and log on to an FTP server could	not be completed because the supplied user name is incorrect." },
	{ NET_ERROR_INTERNET_INCORRECT_PASSWORD, "The request to connect and log on to an FTP server could not be completed because the supplied password is incorrect." },
	{ NET_ERROR_INTERNET_LOGIN_FAILURE, "The request to connect to and log on to an FTP server	failed." },
	{ NET_ERROR_INTERNET_INVALID_OPERATION, "The requested operation is invalid." },
	{ NET_ERROR_INTERNET_OPERATION_CANCELLED, "The operation was canceled, usually because the handle on which the request was operating was closed before the operation completed." },
	{ NET_ERROR_INTERNET_INCORRECT_HANDLE_TYPE, "The type of handle supplied is incorrect for this operation." },
	{ NET_ERROR_INTERNET_INCORRECT_HANDLE_STATE, "The requested operation cannot be carried out because the	handle supplied is not in the correct state." },
	{ NET_ERROR_INTERNET_NOT_PROXY_REQUEST, "The request cannot be made via a proxy." },
	{ NET_ERROR_INTERNET_REGISTRY_VALUE_NOT_FOUND, "A required registry value could not be located." },
	{ NET_ERROR_INTERNET_BAD_REGISTRY_PARAMETER, "A required registry value was located but is an incorrect	type or has an invalid value." },
	{ NET_ERROR_INTERNET_NO_DIRECT_ACCESS, "Direct network access cannot be made at this time." },
	{ NET_ERROR_INTERNET_NO_CONTEXT, "An asynchronous request could not be made because a zero context value was supplied." },
	{ NET_ERROR_INTERNET_NO_CALLBACK, "An asynchronous request could not be made because a callback function has not been set." },
	{ NET_ERROR_INTERNET_REQUEST_PENDING, "The required operation could not be completed because one	or more requests are pending." },
	{ NET_ERROR_INTERNET_INCORRECT_FORMAT, "The format of the request is invalid." },
	{ NET_ERROR_INTERNET_ITEM_NOT_FOUND, "The requested item could not be located." },
	{ NET_ERROR_INTERNET_CANNOT_CONNECT, "The attempt to connect to the server failed." },
	{ NET_ERROR_INTERNET_CONNECTION_ABORTED, "The connection with the server has been terminated." },
	{ NET_ERROR_INTERNET_CONNECTION_RESET, "The connection with the server has been reset." },
	{ NET_ERROR_INTERNET_FORCE_RETRY, "Calls for the Win32 Internet function to redo the request." },
	{ NET_ERROR_INTERNET_INVALID_PROXY_REQUEST, "The request to the proxy was invalid." },
	{ NET_ERROR_INTERNET_HANDLE_EXISTS, "The request failed because the handle already exists." },
	{ NET_ERROR_INTERNET_SEC_CERT_DATE_INVALID, "SSL certificate date that was received from the server is	bad.The certificate is expired." },
	{ NET_ERROR_INTERNET_SEC_CERT_CN_INVALID, "SSL certificate common name(host name field) is incorrect. For example, if you entered www.server.com and the common name on the certificate says www.different.com." },
	{ NET_ERROR_INTERNET_HTTP_TO_HTTPS_ON_REDIR, "The application is moving from a non - SSL to an SSL	connection because of a redirect." },
	{ NET_ERROR_INTERNET_HTTPS_TO_HTTP_ON_REDIR, "The application is moving from an SSL to an non - SSL connection because of a redirect." },
	{ NET_ERROR_INTERNET_MIXED_SECURITY, "Indicates that the content is not entirely secure.Some of the content being viewed may have come from unsecured servers." },
	{ NET_ERROR_INTERNET_CHG_POST_IS_NON_SECURE, "The application is posting and attempting to change	multiple lines of text on a server that is not secure." },
	{ NET_ERROR_INTERNET_POST_IS_NON_SECURE, "The application is posting data to a server that is not secure." },
	{ NET_ERROR_FTP_TRANSFER_IN_PROGRESS, "The requested operation cannot be made on the FTP session handle because an operation is already in progress." },
	{ NET_ERROR_FTP_DROPPED, "The FTP operation was not completed because the session was aborted." },
	{ NET_ERROR_GOPHER_PROTOCOL_ERROR, "An error was detected while parsing data returned from the gopher server." },
	{ NET_ERROR_GOPHER_NOT_FILE, "The request must be made for a file locator." },
	{ NET_ERROR_GOPHER_DATA_ERROR, "An error was detected while receiving data from the gopher	server." },
	{ NET_ERROR_GOPHER_END_OF_DATA, "The end of the data has been reached." },
	{ NET_ERROR_GOPHER_INVALID_LOCATOR, "The supplied locator is not valid." },
	{ NET_ERROR_GOPHER_INCORRECT_LOCATOR_TYPE, "The type of the locator is not correct for this operation." },
	{ NET_ERROR_GOPHER_NOT_GOPHER_PLUS, "The requested operation can only be made against a Gopher +	server or with a locator that specifies a Gopher + operation." },
	{ NET_ERROR_GOPHER_ATTRIBUTE_NOT_FOUND, "The requested attribute could not be located." },
	{ NET_ERROR_GOPHER_UNKNOWN_LOCATOR, "The locator type is unknown." },
	{ NET_ERROR_HTTP_HEADER_NOT_FOUND, "The requested header could not be located." },
	{ NET_ERROR_HTTP_DOWNLEVEL_SERVER, "The server did not return any headers." },
	{ NET_ERROR_HTTP_INVALID_SERVER_RESPONSE, "The server response could not be parsed." },
	{ NET_ERROR_HTTP_INVALID_HEADER, "The supplied header is invalid." },
	{ NET_ERROR_HTTP_INVALID_QUERY_REQUEST, "The request made to HttpQueryInfo is invalid." },
	{ NET_ERROR_HTTP_HEADER_ALREADY_EXISTS, "The header could not be added because it already exists." },
	{ NET_ERROR_HTTP_REDIRECT_FAILED, "The redirection failed because either the scheme changed(for example, HTTP to FTP) or all attempts made to redirect	failed(default is five attempts)." },
};

void logInfoDatabaseException(const string& typeName, const string& msg, const sqlpp::mysql::exception& e) {
	string s = std::format("{}'s {} exception: {}", typeName, msg, e.what());
	gl_dailyLogger->info("{}", s);
	gl_systemMessage.PushInnerSystemInformationMessage(s);
}

void logWarnDatabaseException(const string& typeName, const string& msg, const sqlpp::mysql::exception& e) {
	string s = std::format("{}'s {} exception: {}", typeName, msg, e.what());
	gl_dailyLogger->warn("{}", s);
	gl_systemMessage.PushInnerSystemInformationMessage(s);
}

void logErrorDatabaseException(const string& typeName, const string& msg, const sqlpp::mysql::exception& e) {
	string s = std::format("{}'s {} exception: {}", typeName, msg, e.what());
	gl_dailyLogger->error("{}", s);
	gl_systemMessage.PushInnerSystemInformationMessage(s);
}
