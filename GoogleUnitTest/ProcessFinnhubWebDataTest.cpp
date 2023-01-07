#include"pch.h"

#include"GeneralCheck.h"

#include"WebData.h"

#include"FinnhubWebSocket.h"

#include<memory>
using std::make_shared;

using namespace testing;

namespace FireBirdTest {
	struct FinnhubWebData {
		FinnhubWebData(long lIndex, CString strSymbol, CString strData) {
			m_lIndex = lIndex;
			m_strSymbol = strSymbol;
			m_pData = make_shared<CWebData>();
			m_pData->Test_SetBuffer_(strData);
		}

		~FinnhubWebData() { }

	public:
		long m_lIndex;
		CString m_strSymbol;
		CWebDataPtr m_pData;
	};

	struct FinnhubWebSocketData {
		FinnhubWebSocketData(long lIndex, CString strData) {
			m_lIndex = lIndex;
			m_pData = strData;
		}

		~FinnhubWebSocketData() { }

	public:
		long m_lIndex;
		string m_pData;
	};

	// 正确数据
	FinnhubWebSocketData finnhubWebSocketData141(
		1, _T(
			"{\"data\":[{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.76,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":43},{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.75,\"s\":\"A\",\"t\":1628238530220,\"v\":1}],\"type\":\"trade\"}"));
	// 正确的ping数据格式
	FinnhubWebSocketData finnhubWebSocketData142(2, _T("{\"type\":\"ping\"}"));
	// json格式错误， 返回错误
	FinnhubWebSocketData finnhubWebSocketData143(
		3, _T(
			"\"data\":[{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.76,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":43},{\"c\":[\"1\",\"24\",\"12\",\"p\":146.75,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":1},\"type\":\"trade\"}"));
	// type只能是"trade","ping"或者"error"
	FinnhubWebSocketData finnhubWebSocketData144(
		4, _T(
			"{\"data\":[{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.76,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":43},{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.75,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":1}],\"type\":\"message\"}"));
	// 正确的error数据格式
	FinnhubWebSocketData finnhubWebSocketData145(
		5, _T("{\"msg\":\"Subscribing to too many symbols\",\"type\":\"error\"}"));
	// "dta"非法数据
	FinnhubWebSocketData finnhubWebSocketData149(
		9, _T(
			"{\"dta\":[{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.76,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":43},{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.75,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":1}],\"type\":\"trade\"}"));

	class ProcessOneFinnhubWebSocketDataTest : public::testing::TestWithParam<FinnhubWebSocketData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();
			EXPECT_FALSE(m_finnhubWebSocket.IsReceivingData());

			FinnhubWebSocketData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = nullptr;
			m_pWebData = make_shared<string>(pData->m_pData);
		}

		void TearDown(void) override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		shared_ptr<string> m_pWebData;
		CFinnhubWebSocket m_finnhubWebSocket;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessOneFinnhubWebSocketData1, ProcessOneFinnhubWebSocketDataTest,
	                         testing::Values(&finnhubWebSocketData141, &finnhubWebSocketData142, &finnhubWebSocketData143,
		                         &finnhubWebSocketData144,
		                         &finnhubWebSocketData145, &finnhubWebSocketData149));

	TEST_P(ProcessOneFinnhubWebSocketDataTest, TestProcessOneFinnhubWebSocketData0) {
		bool fSucceed = false;
		CFinnhubSocketPtr pFinnhubWebSocket;
		fSucceed = m_finnhubWebSocket.ParseFinnhubWebSocketData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(m_finnhubWebSocket.IsReceivingData());
			pFinnhubWebSocket = gl_SystemData.PopFinnhubSocket();
			EXPECT_TRUE(pFinnhubWebSocket->m_sSymbol == _T("AAPL"));
		//EXPECT_STREQ(pFinnhubWebSocket->m_strCode, _T("")); // Code目前不考虑
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastPrice, 146.76);
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastVolume, 43);
			EXPECT_EQ(pFinnhubWebSocket->m_iSeconds, 1628238530221);
			pFinnhubWebSocket = gl_SystemData.PopFinnhubSocket();
			EXPECT_TRUE(pFinnhubWebSocket->m_sSymbol == _T("A"));
		//EXPECT_STREQ(pFinnhubWebSocket->m_strCode, _T("")); // Code目前不考虑
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastPrice, 146.75);
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastVolume, 1);
			EXPECT_EQ(pFinnhubWebSocket->m_iSeconds, 1628238530220);
			break;
		case 2: // ping
			EXPECT_TRUE(fSucceed);
			break;
		case 3: // json格式错误
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Finnhub Web Socket json error"));
			break;
		case 4: // type类型不存在
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Finnhub Web Socket type error: message"));
			break;
		case 5: // error message
			EXPECT_FALSE(fSucceed);
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(),
			             _T("Finnhub WebSocket error message: Subscribing to too many symbols"));
			break;
		case 9: // data名不为"data"
			EXPECT_FALSE(fSucceed);
			break;
		default:
			break;
		}
	}
}
