#include"pch.h"

#include <gtest/gtest.h>

#include"GeneralCheck.h"
#include "log.h"

namespace FireBirdTest {
	class logTest : public ::testing::Test {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(logTest, TestMapHTTPStatusCode) {
		string str;
		int counter = 0;
		for (const auto& pair : gl_mapHTTPStatusCode) {
			counter++;
			str = pair.second;
			EXPECT_TRUE(!str.empty());
		}
		EXPECT_EQ(counter, 68) << "目前总项数为68个";

		for (const auto& pair : gl_mapHTTPStatusCode) {
			switch (pair.first) {
			case 0:
				EXPECT_EQ(pair.second, "No Response");
				break;
			case 100:
				EXPECT_EQ(pair.second, "Continue");
				break;
			case 101:
				EXPECT_EQ(pair.second, "Switching Protocols");
				break;
			case 102:
				EXPECT_EQ(pair.second, "Processing");
				break;
			case 103:
				EXPECT_EQ(pair.second, "Early Hints");
				break;
			case 200:
				EXPECT_EQ(pair.second, "OK");
				break;
			case 201:
				EXPECT_EQ(pair.second, "Created");
				break;
			case 202:
				EXPECT_EQ(pair.second, "Accepted");
				break;
			case 203:
				EXPECT_EQ(pair.second, "Non-Authoritative Information");
				break;
			case 204:
				EXPECT_EQ(pair.second, "No Content");
				break;
			case 205:
				EXPECT_EQ(pair.second, "Reset Content");
				break;
			case 206:
				EXPECT_EQ(pair.second, "Partial Content");
				break;
			case 207:
				EXPECT_EQ(pair.second, "Multi-Status");
				break;
			case 208:
				EXPECT_EQ(pair.second, "Already Reported");
				break;
			case 226:
				EXPECT_EQ(pair.second, "IM Used");
				break;
			case 300:
				EXPECT_EQ(pair.second, "Multiple Choices");
				break;
			case 301:
				EXPECT_EQ(pair.second, "Moved Permanently");
				break;
			case 302:
				EXPECT_EQ(pair.second, "Found");
				break;
			case 303:
				EXPECT_EQ(pair.second, "See Other");
				break;
			case 304:
				EXPECT_EQ(pair.second, "Not Modified");
				break;
			case 305:
				EXPECT_EQ(pair.second, "Use Proxy");
				break;
			case 306:
				EXPECT_EQ(pair.second, "(Unused)");
				break;
			case 307:
				EXPECT_EQ(pair.second, "Temporary Redirect");
				break;
			case 308:
				EXPECT_EQ(pair.second, "Permanent Redirect");
				break;
			case 400:
				EXPECT_EQ(pair.second, "Bad Request");
				break;
			case 401:
				EXPECT_EQ(pair.second, "Unauthorized");
				break;
			case 402:
				EXPECT_EQ(pair.second, "Payment Required");
				break;
			case 403:
				EXPECT_EQ(pair.second, "Forbidden");
				break;
			case 404:
				EXPECT_EQ(pair.second, "Not Found");
				break;
			case 405:
				EXPECT_EQ(pair.second, "Method Not Allowed");
				break;
			case 406:
				EXPECT_EQ(pair.second, "Not Acceptable");
				break;
			case 407:
				EXPECT_EQ(pair.second, "Proxy Authentication Required");
				break;
			case 408:
				EXPECT_EQ(pair.second, "Request Timeout");
				break;
			case 409:
				EXPECT_EQ(pair.second, "Conflict");
				break;
			case 410:
				EXPECT_EQ(pair.second, "Gone");
				break;
			case 411:
				EXPECT_EQ(pair.second, "Length Required");
				break;
			case 412:
				EXPECT_EQ(pair.second, "Precondition Failed");
				break;
			case 413:
				EXPECT_EQ(pair.second, "Request Entity Too Large");
				break;
			case 414:
				EXPECT_EQ(pair.second, "Request-URI Too Long");
				break;
			case 415:
				EXPECT_EQ(pair.second, "Unsupported Media Type");
				break;
			case 416:
				EXPECT_EQ(pair.second, "Requested Range Not Satisfiable");
				break;
			case 417:
				EXPECT_EQ(pair.second, "Expectation Failed");
				break;
			case 418:
				EXPECT_EQ(pair.second, "I’m a teapot");
				break;
			case 420:
				EXPECT_EQ(pair.second, "Enhance Your Calm");
				break;
			case 422:
				EXPECT_EQ(pair.second, "Unprocessable Entity");
				break;
			case 423:
				EXPECT_EQ(pair.second, "Locked");
				break;
			case 424:
				EXPECT_EQ(pair.second, "Failed Dependency");
				break;
			case 425:
				EXPECT_EQ(pair.second, "Too Early");
				break;
			case 426:
				EXPECT_EQ(pair.second, "Upgrade Required");
				break;
			case 428:
				EXPECT_EQ(pair.second, "Precondition Required");
				break;
			case 429:
				EXPECT_EQ(pair.second, "Too Many Requests");
				break;
			case 431:
				EXPECT_EQ(pair.second, "Request Header Fields Too Large");
				break;
			case 444:
				EXPECT_EQ(pair.second, "Connection Closed Without Response");
				break;
			case 449:
				EXPECT_EQ(pair.second, "Retry With");
				break;
			case 450:
				EXPECT_EQ(pair.second, "Blocked by Windows Parental Controls");
				break;
			case 451:
				EXPECT_EQ(pair.second, "Unavailable For Legal Reasons");
				break;
			case 499:
				EXPECT_EQ(pair.second, "Client Closed Request");
				break;
			case 500:
				EXPECT_EQ(pair.second, "Internal Server Error");
				break;
			case 501:
				EXPECT_EQ(pair.second, "Not Implemented");
				break;
			case 502:
				EXPECT_EQ(pair.second, "Bad Gateway");
				break;
			case 503:
				EXPECT_EQ(pair.second, "Service Unavailable");
				break;
			case 504:
				EXPECT_EQ(pair.second, "Gateway Timeout");
				break;
			case 505:
				EXPECT_EQ(pair.second, "HTTP Version Not Supported");
				break;
			case 506:
				EXPECT_EQ(pair.second, "Variant Also Negotiates");
				break;
			case 507:
				EXPECT_EQ(pair.second, "Insufficient Storage");
				break;
			case 508:
				EXPECT_EQ(pair.second, "Loop Detected");
				break;
			case 510:
				EXPECT_EQ(pair.second, "Not Extended");
				break;
			case 511:
				EXPECT_EQ(pair.second, "Network Authentication Required");
				break;

			default:
				FAIL() << "Unexpected HTTP status code: " << pair.first;
			}
		}
	}

	TEST_F(logTest, TestMapNetError) {
		string str;
		int counter = 0;
		for (int i = 12001; i < 12300; i++) {
			switch (i) {
			case NET_ERROR_INTERNET_OUT_OF_HANDLES:
			case NET_ERROR_INTERNET_TIMEOUT:
			case NET_ERROR_INTERNET_EXTENDED_ERROR:
			case NET_ERROR_INTERNET_INTERNAL_ERROR:
			case NET_ERROR_INTERNET_INVALID_URL:
			case NET_ERROR_INTERNET_UNRECOGNIZED_SCHEME:
			case NET_ERROR_INTERNET_NAME_NOT_RESOLVED:
			case NET_ERROR_INTERNET_PROTOCOL_NOT_FOUND:
			case NET_ERROR_INTERNET_INVALID_OPTION:
			case NET_ERROR_INTERNET_BAD_OPTION_LENGTH:
			case NET_ERROR_INTERNET_OPTION_NOT_SETTABLE:
			case NET_ERROR_INTERNET_SHUTDOWN:
			case NET_ERROR_INTERNET_INCORRECT_USER_NAME:
			case NET_ERROR_INTERNET_INCORRECT_PASSWORD:
			case NET_ERROR_INTERNET_LOGIN_FAILURE:
			case NET_ERROR_INTERNET_INVALID_OPERATION:
			case NET_ERROR_INTERNET_OPERATION_CANCELLED:
			case NET_ERROR_INTERNET_INCORRECT_HANDLE_TYPE:
			case NET_ERROR_INTERNET_INCORRECT_HANDLE_STATE:
			case NET_ERROR_INTERNET_NOT_PROXY_REQUEST:
			case NET_ERROR_INTERNET_REGISTRY_VALUE_NOT_FOUND:
			case NET_ERROR_INTERNET_BAD_REGISTRY_PARAMETER:
			case NET_ERROR_INTERNET_NO_DIRECT_ACCESS:
			case NET_ERROR_INTERNET_NO_CONTEXT:
			case NET_ERROR_INTERNET_NO_CALLBACK:
			case NET_ERROR_INTERNET_REQUEST_PENDING:
			case NET_ERROR_INTERNET_INCORRECT_FORMAT:
			case NET_ERROR_INTERNET_ITEM_NOT_FOUND:
			case NET_ERROR_INTERNET_CANNOT_CONNECT:
			case NET_ERROR_INTERNET_CONNECTION_ABORTED:
			case NET_ERROR_INTERNET_CONNECTION_RESET:
			case NET_ERROR_INTERNET_FORCE_RETRY:
			case NET_ERROR_INTERNET_INVALID_PROXY_REQUEST:

			case NET_ERROR_INTERNET_HANDLE_EXISTS:
			case NET_ERROR_INTERNET_SEC_CERT_DATE_INVALID:
			case NET_ERROR_INTERNET_SEC_CERT_CN_INVALID:
			case NET_ERROR_INTERNET_HTTP_TO_HTTPS_ON_REDIR:
			case NET_ERROR_INTERNET_HTTPS_TO_HTTP_ON_REDIR:
			case NET_ERROR_INTERNET_MIXED_SECURITY:
			case NET_ERROR_INTERNET_CHG_POST_IS_NON_SECURE:
			case NET_ERROR_INTERNET_POST_IS_NON_SECURE:

			case NET_ERROR_FTP_TRANSFER_IN_PROGRESS:
			case NET_ERROR_FTP_DROPPED:

			case NET_ERROR_GOPHER_PROTOCOL_ERROR:
			case NET_ERROR_GOPHER_NOT_FILE:
			case NET_ERROR_GOPHER_DATA_ERROR:
			case NET_ERROR_GOPHER_END_OF_DATA:
			case NET_ERROR_GOPHER_INVALID_LOCATOR:
			case NET_ERROR_GOPHER_INCORRECT_LOCATOR_TYPE:
			case NET_ERROR_GOPHER_NOT_GOPHER_PLUS:
			case NET_ERROR_GOPHER_ATTRIBUTE_NOT_FOUND:
			case NET_ERROR_GOPHER_UNKNOWN_LOCATOR:

			case NET_ERROR_HTTP_HEADER_NOT_FOUND:
			case NET_ERROR_HTTP_DOWNLEVEL_SERVER:
			case NET_ERROR_HTTP_INVALID_SERVER_RESPONSE:
			case NET_ERROR_HTTP_INVALID_HEADER:
			case NET_ERROR_HTTP_INVALID_QUERY_REQUEST:
			case NET_ERROR_HTTP_HEADER_ALREADY_EXISTS:
			case NET_ERROR_HTTP_REDIRECT_FAILED:
				counter++;
				str = gl_mapNetError.at(i);
				EXPECT_TRUE(!str.empty());
				break;
			default:
				;
			}
		}
		EXPECT_EQ(counter, 59) << "目前总项数";
	}
}
