#include"pch.h"

#include"GeneralCheck.h"

#include"WebData.h"

#include"FinnhubWebSocket.h"
#include "TimeConvert.h"

using namespace testing;

namespace FireBirdTest {
	struct Test_FinnhubWebData {
		Test_FinnhubWebData(long lIndex, const CString& strSymbol, const CString& strData) {
			m_lIndex = lIndex;
			m_strSymbol = strSymbol;
			m_pData = make_shared<CWebData>();
			m_pData->Test_SetBuffer_(strData);
		}

		~Test_FinnhubWebData() {}

	public:
		long m_lIndex;
		CString m_strSymbol;
		CWebDataPtr m_pData;
	};

	struct FinnhubWebSocketData {
		FinnhubWebSocketData(long lIndex, const CString& strData) {
			m_lIndex = lIndex;
			m_pData = strData;
		}

		~FinnhubWebSocketData() {}

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
	// 正确数据,但condition项为空
	FinnhubWebSocketData finnhubWebSocketData146(
		6, _T(
			"{\"data\":[{\"c\":[],\"p\":146.76,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":43},{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.75,\"s\":\"A\",\"t\":1628238530220,\"v\":1}],\"type\":\"trade\"}"));
	// "dta"非法数据
	FinnhubWebSocketData finnhubWebSocketData149(
		9, _T(
			"{\"dta\":[{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.76,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":43},{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.75,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":1}],\"type\":\"trade\"}"));

	class ProcessOneFinnhubWebSocketDataTest : public::testing::TestWithParam<FinnhubWebSocketData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			EXPECT_FALSE(m_finnhubWebSocket.IsReceivingData());

			FinnhubWebSocketData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = nullptr;
			m_pWebData = make_shared<string>(pData->m_pData);
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		shared_ptr<string> m_pWebData;
		CFinnhubWebSocket m_finnhubWebSocket;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessOneFinnhubWebSocketData1, ProcessOneFinnhubWebSocketDataTest,
	                         testing::Values(&finnhubWebSocketData141, &finnhubWebSocketData142, &finnhubWebSocketData143,
		                         &finnhubWebSocketData144, &finnhubWebSocketData145,&finnhubWebSocketData146, &finnhubWebSocketData149));

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
			EXPECT_EQ(m_finnhubWebSocket.GetHeartbeatTime(), GetUTCTime()) << "只有有效数据才设置心跳时间";
			break;
		case 2: // ping
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(m_finnhubWebSocket.GetHeartbeatTime(), 0);
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
		case 6: // 正确,但condition项为空
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(m_finnhubWebSocket.IsReceivingData());
			pFinnhubWebSocket = gl_SystemData.PopFinnhubSocket();
			EXPECT_TRUE(pFinnhubWebSocket->m_sSymbol == _T("AAPL"));
		//EXPECT_STREQ(pFinnhubWebSocket->m_strCode, _T("")); // Code目前不考虑
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastPrice, 146.76);
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastVolume, 43);
			EXPECT_EQ(pFinnhubWebSocket->m_iSeconds, 1628238530221);
			EXPECT_EQ(pFinnhubWebSocket->m_vCode.size(), 0) << "c项为null";
			pFinnhubWebSocket = gl_SystemData.PopFinnhubSocket();
			EXPECT_TRUE(pFinnhubWebSocket->m_sSymbol == _T("A"));
		//EXPECT_STREQ(pFinnhubWebSocket->m_strCode, _T("")); // Code目前不考虑
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastPrice, 146.75);
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastVolume, 1);
			EXPECT_EQ(pFinnhubWebSocket->m_iSeconds, 1628238530220);
			EXPECT_EQ(pFinnhubWebSocket->m_vCode.size(), 3);
			EXPECT_EQ(m_finnhubWebSocket.GetHeartbeatTime(), GetUTCTime()) << "只有有效数据才设置心跳时间";
			break;
		case 9: // data名不为"data"
			EXPECT_FALSE(fSucceed);
			break;
		default:
			break;
		}
	}

	TEST_P(ProcessOneFinnhubWebSocketDataTest, TestProcessOneFinnhubWebSocketDataWithSidmjson) {
		bool fSucceed = false;
		CFinnhubSocketPtr pFinnhubWebSocket;
		fSucceed = m_finnhubWebSocket.ParseFinnhubWebSocketDataWithSidmjson(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(m_finnhubWebSocket.IsReceivingData());
			pFinnhubWebSocket = gl_SystemData.PopFinnhubSocket();
			EXPECT_TRUE(pFinnhubWebSocket->m_sSymbol == _T("AAPL"));
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastPrice, 146.76);
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastVolume, 43);
			EXPECT_EQ(pFinnhubWebSocket->m_iSeconds, 1628238530221);
			EXPECT_EQ(pFinnhubWebSocket->m_vCode.size(), 3);
			pFinnhubWebSocket = gl_SystemData.PopFinnhubSocket();
			EXPECT_TRUE(pFinnhubWebSocket->m_sSymbol == _T("A"));
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastPrice, 146.75);
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastVolume, 1);
			EXPECT_EQ(pFinnhubWebSocket->m_iSeconds, 1628238530220);
			EXPECT_EQ(pFinnhubWebSocket->m_vCode.size(), 3);
			EXPECT_EQ(m_finnhubWebSocket.GetHeartbeatTime(), GetUTCTime()) << "只有有效数据才设置心跳时间";
			break;
		case 2: // ping
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(m_finnhubWebSocket.GetHeartbeatTime(), 0);
			break;
		case 3: // json格式错误
			EXPECT_FALSE(fSucceed);
			break;
		case 4: // type类型不存在
			EXPECT_FALSE(fSucceed);
			break;
		case 5: // error message
			EXPECT_FALSE(fSucceed);
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(),
			             _T("Finnhub WebSocket error message: Subscribing to too many symbols"));
			break;
		case 6: // 正确,但condition项为空
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(m_finnhubWebSocket.IsReceivingData());
			pFinnhubWebSocket = gl_SystemData.PopFinnhubSocket();
			EXPECT_TRUE(pFinnhubWebSocket->m_sSymbol == _T("AAPL"));
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastPrice, 146.76);
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastVolume, 43);
			EXPECT_EQ(pFinnhubWebSocket->m_iSeconds, 1628238530221);
			EXPECT_EQ(pFinnhubWebSocket->m_vCode.size(), 0) << "c项为null";
			pFinnhubWebSocket = gl_SystemData.PopFinnhubSocket();
			EXPECT_TRUE(pFinnhubWebSocket->m_sSymbol == _T("A"));
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastPrice, 146.75);
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastVolume, 1);
			EXPECT_EQ(pFinnhubWebSocket->m_iSeconds, 1628238530220);
			EXPECT_EQ(pFinnhubWebSocket->m_vCode.size(), 3);
			EXPECT_EQ(m_finnhubWebSocket.GetHeartbeatTime(), GetUTCTime()) << "只有有效数据才设置心跳时间";
			break;
		case 9: // data名不为"data",函数出现exception而退出
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(gl_SystemData.GetFinnhubSocketSize(), 0) << "没有data项，数据个数为零";
			EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		default:
			break;
		}
	}
}
