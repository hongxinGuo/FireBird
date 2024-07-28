#include"pch.h"

#include"GeneralCheck.h"

#include"WebData.h"

#include"WorldStock.h"

using namespace testing;

namespace FireBirdTest {
	struct Test_TiingoWebData {
		Test_TiingoWebData(const long lIndex, const CString& strSymbol, const CString& strData) {
			m_lIndex = lIndex;
			m_strSymbol = strSymbol;
			m_pData = make_shared<CWebData>();
			m_pData->Test_SetBuffer_(strData);
		}

		~Test_TiingoWebData() = default;

	public:
		long m_lIndex;
		CString m_strSymbol;
		CWebDataPtr m_pData;
	};

	struct TiingoWebSocketData {
		TiingoWebSocketData(const long lIndex, const CString& strSymbol, const CString& strData) {
			m_lIndex = lIndex;
			m_strSymbol = strSymbol;
			m_pData = strData;
		}

		~TiingoWebSocketData() = default;

	public:
		long m_lIndex;
		CString m_strSymbol;
		string m_pData;
	};

	TiingoWebSocketData tiingoForexData1(1, _T(""), _T("{\"messageType\":\"A\",\"service\":\"fx\",\"data\":[\"Q\",\"eurnok\",\"2019-07-05T15:49:15.157000+00:00\",5000000.0,9.6764,9.678135,5000000.0,9.67987]}"));
	TiingoWebSocketData tiingoForexData2(2, _T(""), _T("{\"messageType\":\"A\",\"service\":\"fx\",\"data\":[\"Q\",\"gbpaud\",\"2019-07-05T15:49:15.236000+00:00\",1000000.0,1.79457,1.79477,5000000.0,1.79497]}"));
	TiingoWebSocketData tiingoForexData4(4, _T(""), _T("{\"messageType\":\"A\",\"service\":\"fx\",\"data\":[\"Q\",\"gbpaud\",\"2019-07-05T15:49:15.236000+00:00\",1000000.0,1.79457,1.79477,5000000.0,1.79497]}"));
	TiingoWebSocketData tiingoForexData5(5, _T(""), _T("{\"messageType\":\"A\",\"service\":\"fx\",\"data\":[\"Q\",\"gbpaud\",\"2019-07-05T15:49:15.236000+00:00\",1000000.0,1.79457,1.79477,5000000.0,1.79497]}"));
	// data名称错为'dta'
	TiingoWebSocketData tiingoForexData6(6, _T(""), _T("{\"messageType\":\"A\",\"service\":\"fx\",\"dta\":[\"Q\",\"gbpaud\",\"2019-07-05T15:49:15.236000+00:00\",1000000.0,1.79457,1.79477,5000000.0,1.79497]}"));
	// data首项必须为‘fx'
	TiingoWebSocketData tiingoForexData7(7, _T(""), _T("{\"messageType\":\"A\",\"service\":\"ifx\",\"data\":[\"Q\",\"gbpaud\",\"2019-07-05T15:49:15.236000+00:00\",1000000.0,1.79457,1.79477,5000000.0,1.79497]}"));
	// messageType错误
	TiingoWebSocketData tiingoForexData8(8, _T(""), _T("{\"messageType\":\"B\",\"service\":\"fx\",\"data\":[\"Q\",\"gbpaud\",\"2019-07-05T15:49:15.236000+00:00\",1000000.0,1.79457,1.79477,5000000.0,1.79497]}"));
	// json格式错误
	TiingoWebSocketData tiingoForexData9(9, _T(""), _T("\"messageType\":\"A\",\"service\":\"fx\",\"data\":[\"Q\",\"gbpaud\",\"2019-07-05T15:49:15.236000+00:00\",1000000.0,1.79457,1.79477,5000000.0,1.79497]}"));
	// heart beat
	TiingoWebSocketData tiingoForexData10(
		10, _T(""), _T("{\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}"));
	// authentication
	TiingoWebSocketData tiingoForexData11(11, _T(""), _T("{\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}"));
	// subscribe
	TiingoWebSocketData tiingoForexData12(12, _T(""), _T("{\"data\":{\"tickers\":[\"*\",\"FXCM:EUR/USD\",\"IC MARKETS:2\",\"OANDA:USD_JPY\"],\"thresholdLevel\":\"0\"},\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"}}"));
	// error message
	TiingoWebSocketData tiingoForexData13(13, _T(""), _T("{\"messageType\":\"E\",\"response\":{\"code\":400,\"message\":\"thresholdLevel not valid\"}}"));

	class ProcessOneTiingoForexWebSocketDataTest : public TestWithParam<TiingoWebSocketData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			TiingoWebSocketData* pData = GetParam();
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
		CWorldStockPtr m_pStock;
		shared_ptr<string> m_pWebData;

		CTiingoForexWebSocket tiingoForexWebSocket;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessOneTiingoForexWebSocketData1, ProcessOneTiingoForexWebSocketDataTest,
	                         testing::Values(&tiingoForexData1, &tiingoForexData2, &tiingoForexData6, &tiingoForexData7, &
		                         tiingoForexData8, &tiingoForexData9,
		                         &tiingoForexData10, &tiingoForexData11, &tiingoForexData12, &tiingoForexData13));

	TEST_P(ProcessOneTiingoForexWebSocketDataTest, TestProcessOneTiingoForexWebSocketData0) {
		bool fSucceed = false;
		CTiingoForexSocketPtr pForex;
		fSucceed = tiingoForexWebSocket.ParseTiingoForexWebSocketData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确
			EXPECT_TRUE(fSucceed);
			pForex = gl_SystemData.PopTiingoForexSocket();
			EXPECT_EQ(pForex->m_chMessageType, 'Q');
			EXPECT_TRUE(pForex->m_sSymbol == _T("eurnok"));
			EXPECT_DOUBLE_EQ(pForex->m_dBidSize, 5000000);
			EXPECT_DOUBLE_EQ(pForex->m_dBidPrice, 9.6764);
			EXPECT_DOUBLE_EQ(pForex->m_dMidPrice, 9.678135);
			EXPECT_DOUBLE_EQ(pForex->m_dAskSize, 5000000.0);
			EXPECT_DOUBLE_EQ(pForex->m_dAskPrice, 9.67987);
			EXPECT_EQ(tiingoForexWebSocket.GetHeartbeatTime(), GetUTCTime()) << "只有有效数据才设置心跳时间";
			break;
		case 2: // 正确
			EXPECT_TRUE(fSucceed);
			pForex = gl_SystemData.PopTiingoForexSocket();
			break;
		case 6: //
			EXPECT_FALSE(fSucceed);
			break;
		case 7: //
			EXPECT_FALSE(fSucceed);
			break;
		case 8: // messageType错误
			EXPECT_FALSE(fSucceed);
			break;
		case 9: // json格式错误
			EXPECT_FALSE(fSucceed);
			break;
		case 10: // heart beat
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(tiingoForexWebSocket.GetHeartbeatTime(), 0);
			break;
		case 11: // authentication
			EXPECT_TRUE(fSucceed);
			break;
		case 12: // subscribe
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(tiingoForexWebSocket.m_vCurrentInquireSymbol.size(), 4);
			EXPECT_TRUE(tiingoForexWebSocket.m_vCurrentInquireSymbol.at(0) == _T("*"));
			EXPECT_TRUE(tiingoForexWebSocket.m_vCurrentInquireSymbol.at(1) == _T("FXCM:EUR/USD"));
			EXPECT_TRUE(tiingoForexWebSocket.m_vCurrentInquireSymbol.at(2) == _T("IC MARKETS:2"));
			EXPECT_TRUE(tiingoForexWebSocket.m_vCurrentInquireSymbol.at(3) == _T("OANDA:USD_JPY"));
			break;
		case 13: // error message
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(tiingoForexWebSocket.GetStatusCode(), 400);
			EXPECT_TRUE(tiingoForexWebSocket.GetStatusMessage() == _T("thresholdLevel not valid"));
			break;
		default:
			break;
		}
	}

	TiingoWebSocketData tiingoCryptoData1(1, _T(""), _T(
		                                      "{\"messageType\":\"A\",\"service\":\"crypto_data\",\"data\":[\"Q\",\"neojpy\",\"2019-01-30T18:03:40.195515+00:00\",\"bitfinex\",38.11162867,787.82,787.83,42.4153887,787.84]}"));
	TiingoWebSocketData tiingoCryptoData2(2, _T(""), _T(
		                                      "{\"messageType\":\"A\",\"service\":\"crypto_data\",\"data\":[\"T\",\"evxbtc\",\"2019-01-30T18:03:40.056000+00:00\",\"binance\",405.0,9.631e-05]}"));
	// heart beat
	TiingoWebSocketData tiingoCryptoData3(
		3, _T(""), _T("{\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}"));
	// authentication
	TiingoWebSocketData tiingoCryptoData4(4, _T(""), _T("{\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}"));
	// messageType必须为'A' 'H' 或者'I'
	TiingoWebSocketData tiingoCryptoData5(5, _T(""), _T("{\"messageType\":\"B\",\"service\":\"crypto_data\",\"data\":[\"Q\",\"neojpy\",\"2019-01-30T18:03:40.195515+00:00\",\"bitfinex\",38.11162867,787.82,787.83,42.4153887,787.84]}"));
	// data错为'dta'
	TiingoWebSocketData tiingoCryptoData6(6, _T(""), _T("{\"messageType\":\"A\",\"service\":\"crypto_data\",\"dta\":[\"Q\",\"neojpy\",\"2019-01-30T18:03:40.195515+00:00\",\"bitfinex\",38.11162867,787.82,787.83,42.4153887,787.84]}"));
	// data第一项必须为'Q'或者'T'
	TiingoWebSocketData tiingoCryptoData7(7, _T(""), _T("{\"messageType\":\"A\",\"service\":\"crypto_data\",\"data\":[\"A\",\"evxbtc\",\"2019-01-30T18:03:40.056000+00:00\",\"binance\",405.0,9.631e-05]}"));
	// json格式错误
	TiingoWebSocketData tiingoCryptoData9(9, _T(""), _T("\"messageType\":\"A\",\"service\":\"crypto_data\",\"data\":[\"Q\",\"neojpy\",\"2019-01-30T18:03:40.195515+00:00\",\"bitfinex\",38.11162867,787.82,787.83,42.4153887,787.84]}"));
	// subscribe
	TiingoWebSocketData tiingoCryptoData10(10, _T(""), _T("{\"data\":{\"tickers\":[\"*\",\"BINANCE:IDEXBUSD\",\"BITTREX:USDT-ADA\",\"HITBTC:XRPEOS\"],\"thresholdLevel\":\"0\"},\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"}}"));
	// error message
	TiingoWebSocketData tiingoCryptoData11(11, _T(""), _T("{\"messageType\":\"E\",\"response\":{\"code\":400,\"message\":\"thresholdLevel not valid\"}}"));

	class ProcessOneTiingoCryptoWebSocketDataTest : public TestWithParam<TiingoWebSocketData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			TiingoWebSocketData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = nullptr;
			m_pWebData = make_shared<string>(pData->m_pData);
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
			m_pWebData = nullptr;
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		shared_ptr<string> m_pWebData;

		CTiingoCryptoWebSocket tiingoCryptoWebSocket;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessOneTiingoCryptoWebSocketData1, ProcessOneTiingoCryptoWebSocketDataTest,
	                         testing::Values(&tiingoCryptoData1, &tiingoCryptoData2, &tiingoCryptoData3, &tiingoCryptoData4, &tiingoCryptoData5,
		                         &tiingoCryptoData6, &tiingoCryptoData7, &tiingoCryptoData9, &tiingoCryptoData10, &tiingoCryptoData11));

	TEST_P(ProcessOneTiingoCryptoWebSocketDataTest, TestProcessOneTiingoCryptoWebSocketData0) {
		bool fSucceed = false;
		CTiingoCryptoSocketPtr pCrypto;
		fSucceed = tiingoCryptoWebSocket.ParseTiingoCryptoWebSocketData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确 Q
			EXPECT_TRUE(fSucceed);
			pCrypto = gl_SystemData.PopTiingoCryptoSocket();
			EXPECT_EQ(pCrypto->m_chMessageType, 'Q');
			EXPECT_TRUE(pCrypto->m_strExchange == _T("bitfinex"));
			EXPECT_TRUE(pCrypto->m_sSymbol == _T("neojpy"));
			EXPECT_DOUBLE_EQ(pCrypto->m_dBidSize, 38.11162867);
			EXPECT_DOUBLE_EQ(pCrypto->m_dBidPrice, 787.82);
			EXPECT_DOUBLE_EQ(pCrypto->m_dMidPrice, 787.83);
			EXPECT_DOUBLE_EQ(pCrypto->m_dAskPrice, 787.84);
			EXPECT_DOUBLE_EQ(pCrypto->m_dAskSize, 42.4153887);
			EXPECT_DOUBLE_EQ(pCrypto->m_dLastPrice, 0);
			EXPECT_DOUBLE_EQ(pCrypto->m_dLastSize, 0);
			EXPECT_EQ(tiingoCryptoWebSocket.GetHeartbeatTime(), GetUTCTime()) << "只有有效数据才设置心跳时间";
			break;
		case 2: // 正确 T
			EXPECT_TRUE(fSucceed);
			pCrypto = gl_SystemData.PopTiingoCryptoSocket();
			EXPECT_EQ(pCrypto->m_chMessageType, 'T');
			EXPECT_TRUE(pCrypto->m_strExchange == _T("binance"));
			EXPECT_TRUE(pCrypto->m_sSymbol == _T("evxbtc"));
			EXPECT_DOUBLE_EQ(pCrypto->m_dBidSize, 0);
			EXPECT_DOUBLE_EQ(pCrypto->m_dBidPrice, 0);
			EXPECT_DOUBLE_EQ(pCrypto->m_dMidPrice, 0);
			EXPECT_DOUBLE_EQ(pCrypto->m_dAskPrice, 0);
			EXPECT_DOUBLE_EQ(pCrypto->m_dAskSize, 0);
			EXPECT_DOUBLE_EQ(pCrypto->m_dLastSize, 405.0);
			EXPECT_DOUBLE_EQ(pCrypto->m_dLastPrice, 9.631e-05);
			break;
		case 3: // 正确 heart beat
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(tiingoCryptoWebSocket.GetHeartbeatTime(), 0);
			break;
		case 4: // 正确 authentication
			EXPECT_TRUE(fSucceed);
			break;
		case 5: // messageType错误
			EXPECT_FALSE(fSucceed);
			break;
		case 6:
			EXPECT_FALSE(fSucceed);
			break;
		case 7:
			EXPECT_FALSE(fSucceed);
			break;
		case 9: // json格式错误
			EXPECT_FALSE(fSucceed);
			break;
		case 10: // subscribe
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(tiingoCryptoWebSocket.m_vCurrentInquireSymbol.size(), 4);
			EXPECT_TRUE(tiingoCryptoWebSocket.m_vCurrentInquireSymbol.at(0) == _T("*"));
			EXPECT_TRUE(tiingoCryptoWebSocket.m_vCurrentInquireSymbol.at(1) == _T("BINANCE:IDEXBUSD"));
			EXPECT_TRUE(tiingoCryptoWebSocket.m_vCurrentInquireSymbol.at(2) == _T("BITTREX:USDT-ADA"));
			EXPECT_TRUE(tiingoCryptoWebSocket.m_vCurrentInquireSymbol.at(3) == _T("HITBTC:XRPEOS"));
			break;
		case 11: // error message
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(tiingoCryptoWebSocket.GetStatusCode(), 400);
			EXPECT_TRUE(tiingoCryptoWebSocket.GetStatusMessage() == _T("thresholdLevel not valid"));
			break;
		default:
			break;
		}
	}

	// 正确数据
	TiingoWebSocketData tiingoIEXData1(1, _T(""), _T(
		                                   "{\"messageType\":\"A\",\"service\":\"iex\",\"data\":[\"Q\",\"2019-01-30T13:33:45.383129126-05:00\",1548873225383129126,\"vym\",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}"));
	TiingoWebSocketData tiingoIEXData2(2, _T(""), _T(
		                                   "{\"messageType\":\"A\",\"service\":\"iex\",\"data\":[\"T\",\"2019-01-30T13:33:45.594808294-05:00\",1548873225594808294,\"wes\",null,null,null,null,null,50.285,200,null,0,0,0,0]}"));
	// authentication
	TiingoWebSocketData tiingoIEXData3(3, _T(""), _T(
		                                   "{\"data\":{\"subscriptionId\":2563367},\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"}}"));
	// Heart beat
	TiingoWebSocketData tiingoIEXData4(
		4, _T(""), _T("{\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}"));
	// messageType只能为'A''I''H'
	TiingoWebSocketData tiingoIEXData5(5, _T(""), _T(
		                                   "{\"messageType\":\"B\",\"service\":\"iex\",\"data\":[\"Q\",\"2019-01-30T13:33:45.383129126-05:00\",1548873225383129126,\"vym\",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}"));
	// data的首项只能为‘Q’ ‘T’ 或者‘B’
	TiingoWebSocketData tiingoIEXData6(6, _T(""), _T(
		                                   "{\"messageType\":\"A\",\"service\":\"iex\",\"data\":[\"C\",\"2019-01-30T13:33:45.383129126-05:00\",1548873225383129126,\"vym\",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}"));
	// 错误。service的名称不为"iex",
	TiingoWebSocketData tiingoIEXData7(7, _T(""), _T(
		                                   "{\"messageType\":\"A\",\"service\":\"ex\",\"data\":[\"Q\",\"2019-01-30T13:33:45.383129126-05:00\",1548873225383129126,\"vym\",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}"));
	// service错为servi
	TiingoWebSocketData tiingoIEXData8(8, _T(""), _T(
		                                   "{\"messageType\":\"A\",\"servi\":\"iex\",\"data\":[\"T\",\"2019-01-30T13:33:45.594808294-05:00\",1548873225594808294,\"wes\",null,null,null,null,null,50.285,200,null,0,0,0,0]}"));
	// json格式错误
	TiingoWebSocketData tiingoIEXData9(9, _T(""), _T(
		                                   "\"messageType\":\"A\",\"service\":\"iex\",\"data\":[\"Q\",\"2019-01-30T13:33:45.383129126-05:00\",1548873225383129126,\"vym\",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}"));
	// subscribe
	TiingoWebSocketData tiingoIEXData10(10, _T(""), _T(
		                                    "{\"data\":{\"tickers\":[\"*\",\"uso\",\"msft\",\"tnk\"],\"thresholdLevel\":\"0\"},\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"}}"));
	// error message
	TiingoWebSocketData tiingoIEXData11(11, _T(""), _T("{\"messageType\":\"E\",\"response\":{\"code\":400,\"message\":\"thresholdLevel not valid\"}}"));

	class ProcessOneTiingoIEXWebSocketDataTest : public TestWithParam<TiingoWebSocketData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			TiingoWebSocketData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = nullptr;
			m_pWebData = make_shared<string>(pData->m_pData);
			EXPECT_EQ(tiingoIEXWebSocket.m_vCurrentInquireSymbol.size(), 0);
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

		CTiingoIEXWebSocket tiingoIEXWebSocket;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessOneTiingoIEXWebSocketData1, ProcessOneTiingoIEXWebSocketDataTest,
	                         testing::Values(&tiingoIEXData1, &tiingoIEXData2, &tiingoIEXData3, &tiingoIEXData4, &
		                         tiingoIEXData5,
		                         &tiingoIEXData6, &tiingoIEXData7, &tiingoIEXData8, &tiingoIEXData9, &tiingoIEXData10,&tiingoCryptoData11));

	TEST_P(ProcessOneTiingoIEXWebSocketDataTest, TestProcessOneTiingoIEXWebSocketData0) {
		CTiingoIEXSocketPtr pTiingoIEX;
		bool fSucceed = false;
		fSucceed = tiingoIEXWebSocket.ParseTiingoIEXWebSocketData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确 Q
			EXPECT_TRUE(fSucceed);
			pTiingoIEX = gl_SystemData.PopTiingoIEXSocket();
			EXPECT_EQ(pTiingoIEX->m_iNanoseconds, 1548873225383129126);
			EXPECT_TRUE(pTiingoIEX->m_sSymbol == _T("vym"));
			EXPECT_EQ(pTiingoIEX->m_dBidSize, 100);
			EXPECT_EQ(pTiingoIEX->m_dBidPrice, 81.58);
			EXPECT_EQ(pTiingoIEX->m_dMidPrice, 81.585);
			EXPECT_EQ(pTiingoIEX->m_dAskPrice, 81.59);
			EXPECT_EQ(pTiingoIEX->m_dAskSize, 100);
			EXPECT_EQ(pTiingoIEX->m_dLastPrice, 0);
			EXPECT_EQ(pTiingoIEX->m_dLastSize, 0);
			EXPECT_EQ(pTiingoIEX->m_iHalted, 0);
			EXPECT_EQ(pTiingoIEX->m_iAfterHour, 0);
			EXPECT_EQ(pTiingoIEX->m_iISO, 0);
			EXPECT_EQ(pTiingoIEX->m_iOddlot, 0);
			EXPECT_EQ(pTiingoIEX->m_iNMSRule611, 0);
			EXPECT_EQ(tiingoIEXWebSocket.GetHeartbeatTime(), GetUTCTime()) << "只有有效数据才设置心跳时间";
			break;
		case 2: // 正确 T
			EXPECT_TRUE(fSucceed);
			pTiingoIEX = gl_SystemData.PopTiingoIEXSocket();
			EXPECT_EQ(pTiingoIEX->m_iNanoseconds, 1548873225594808294);
			EXPECT_TRUE(pTiingoIEX->m_sSymbol == _T("wes"));
			EXPECT_EQ(pTiingoIEX->m_dBidSize, 0);
			EXPECT_EQ(pTiingoIEX->m_dBidPrice, 0);
			EXPECT_EQ(pTiingoIEX->m_dMidPrice, 0);
			EXPECT_EQ(pTiingoIEX->m_dAskPrice, 0);
			EXPECT_EQ(pTiingoIEX->m_dAskSize, 0);
			EXPECT_EQ(pTiingoIEX->m_dLastPrice, 50.285);
			EXPECT_EQ(pTiingoIEX->m_dLastSize, 200);
			EXPECT_EQ(pTiingoIEX->m_iHalted, 0);
			EXPECT_EQ(pTiingoIEX->m_iAfterHour, 0);
			EXPECT_EQ(pTiingoIEX->m_iISO, 0);
			EXPECT_EQ(pTiingoIEX->m_iOddlot, 0);
			EXPECT_EQ(pTiingoIEX->m_iNMSRule611, 0);
			break;
		case 3: // authentication
			EXPECT_TRUE(fSucceed);
			break;
		case 4: // Heart beat
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(tiingoIEXWebSocket.GetHeartbeatTime(), 0) << "收到ping时不设置";
			break;
		case 5: //
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		case 6: //
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		case 7: // service的名称必须为"iex"
			EXPECT_FALSE(fSucceed);
			break;
		case 8: // service错为serci
			EXPECT_FALSE(fSucceed);
			break;
		case 9: // json格式错误
			EXPECT_FALSE(fSucceed);
			break;
		case 10: // subscribe
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(tiingoIEXWebSocket.m_vCurrentInquireSymbol.size(), 4);
			EXPECT_TRUE(tiingoIEXWebSocket.m_vCurrentInquireSymbol.at(0) == _T("*"));
			EXPECT_TRUE(tiingoIEXWebSocket.m_vCurrentInquireSymbol.at(1) == _T("uso"));
			EXPECT_TRUE(tiingoIEXWebSocket.m_vCurrentInquireSymbol.at(2) == _T("msft"));
			EXPECT_TRUE(tiingoIEXWebSocket.m_vCurrentInquireSymbol.at(3) == _T("tnk"));
			break;
		case 11: // error message
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(tiingoIEXWebSocket.GetStatusCode(), 400);
			EXPECT_TRUE(tiingoIEXWebSocket.GetStatusMessage() == _T("thresholdLevel not valid"));
			break;
		default:
			break;
		}
	}
}
