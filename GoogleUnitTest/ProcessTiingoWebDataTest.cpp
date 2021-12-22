#include"pch.h"
#include"globedef.h"
#include"GeneralCheck.h"

#include"WebInquirer.h"
#include"SystemData.h"

#include"WorldStock.h"
#include"WorldMarket.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	struct TiingoWebData {
		TiingoWebData(long lIndex, CString strSymbol, CString strData) {
			m_lIndex = lIndex;
			m_strSymbol = strSymbol;
			m_pData = make_shared<CWebData>();
			m_pData->__TEST_SetBuffer__(strData);
		}

		~TiingoWebData() {
		}

	public:
		long m_lIndex;
		CString m_strSymbol;
		CWebDataPtr m_pData;
	};

	struct TiingoWebSocketData {
		TiingoWebSocketData(long lIndex, CString strSymbol, CString strData) {
			m_lIndex = lIndex;
			m_strSymbol = strSymbol;
			m_pData = strData;
		}

		~TiingoWebSocketData() {
		}

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
	// jsong格式错误
	TiingoWebSocketData tiingoForexData9(9, _T(""), _T("\"messageType\":\"A\",\"service\":\"fx\",\"data\":[\"Q\",\"gbpaud\",\"2019-07-05T15:49:15.236000+00:00\",1000000.0,1.79457,1.79477,5000000.0,1.79497]}"));
	// heart beat
	TiingoWebSocketData tiingoForexData10(3, _T(""), _T("{\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}"));
	// authenizition
	TiingoWebSocketData tiingoForexData11(4, _T(""), _T("{\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}"));

	class ProcessOneTiingoForexWebSocketDataTest : public::testing::TestWithParam<TiingoWebSocketData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			TiingoWebSocketData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = nullptr;
			m_pWebData = make_shared<string>(pData->m_pData);
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		shared_ptr<string> m_pWebData;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessOneTiingoForexWebSocketData1, ProcessOneTiingoForexWebSocketDataTest,
		testing::Values(&tiingoForexData1, &tiingoForexData2, &tiingoForexData6, &tiingoForexData7, &tiingoForexData8, &tiingoForexData9,
			&tiingoForexData10, &tiingoForexData11));

	TEST_P(ProcessOneTiingoForexWebSocketDataTest, TestProcessOneTiingoForexWebSocketData0) {
		bool fSucceed = false;
		CTiingoForexSocketPtr pForex;
		fSucceed = gl_pWorldMarket->ParseTiingoForexWebSocketData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确
			EXPECT_TRUE(fSucceed);
			pForex = gl_SystemData.PopTiingoForexSocket();
			EXPECT_EQ(pForex->m_chMessageType, 'Q');
			EXPECT_STREQ(pForex->m_strSymbol, _T("eurnok"));
			EXPECT_DOUBLE_EQ(pForex->m_dBidSize, 5000000);
			EXPECT_DOUBLE_EQ(pForex->m_dBidPrice, 9.6764);
			EXPECT_DOUBLE_EQ(pForex->m_dMidPrice, 9.678135);
			EXPECT_DOUBLE_EQ(pForex->m_dAskSize, 5000000.0);
			EXPECT_DOUBLE_EQ(pForex->m_dAskPrice, 9.67987);
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
		case 9: // jsong格式错误
			EXPECT_FALSE(fSucceed);
			break;
		case 10: // heart beat
			EXPECT_TRUE(fSucceed);
			break;
		case 11: // authenizition
			EXPECT_TRUE(fSucceed);
			break;
		default:
			break;
		}
	}

	TiingoWebSocketData tiingoCryptoData1(1, _T(""), _T("{\"messageType\":\"A\",\"service\":\"crypto_data\",\"data\":[\"Q\",\"neojpy\",\"2019-01-30T18:03:40.195515+00:00\",\"bitfinex\",38.11162867,787.82,787.83,42.4153887,787.84]}"));
	TiingoWebSocketData tiingoCryptoData2(2, _T(""), _T("{\"messageType\":\"A\",\"service\":\"crypto_data\",\"data\":[\"T\",\"evxbtc\",\"2019-01-30T18:03:40.056000+00:00\",\"binance\",405.0,9.631e-05]}"));
	// heart beat
	TiingoWebSocketData tiingoCryptoData3(3, _T(""), _T("{\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}"));
	// authenizition
	TiingoWebSocketData tiingoCryptoData4(4, _T(""), _T("{\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}"));
	// messageType必须为'A' 'H' 或者'I'
	TiingoWebSocketData tiingoCryptoData5(5, _T(""), _T("{\"messageType\":\"B\",\"service\":\"crypto_data\",\"data\":[\"Q\",\"neojpy\",\"2019-01-30T18:03:40.195515+00:00\",\"bitfinex\",38.11162867,787.82,787.83,42.4153887,787.84]}"));
	// data错为'dta'
	TiingoWebSocketData tiingoCryptoData6(6, _T(""), _T("{\"messageType\":\"A\",\"service\":\"crypto_data\",\"dta\":[\"Q\",\"neojpy\",\"2019-01-30T18:03:40.195515+00:00\",\"bitfinex\",38.11162867,787.82,787.83,42.4153887,787.84]}"));
	// data第一项必须为'Q'或者'T'
	TiingoWebSocketData tiingoCryptoData7(7, _T(""), _T("{\"messageType\":\"A\",\"service\":\"crypto_data\",\"data\":[\"A\",\"evxbtc\",\"2019-01-30T18:03:40.056000+00:00\",\"binance\",405.0,9.631e-05]}"));
	// json格式错误
	TiingoWebSocketData tiingoCryptoData9(9, _T(""), _T("\"messageType\":\"A\",\"service\":\"crypto_data\",\"data\":[\"Q\",\"neojpy\",\"2019-01-30T18:03:40.195515+00:00\",\"bitfinex\",38.11162867,787.82,787.83,42.4153887,787.84]}"));

	class ProcessOneTiingoCryptoWebSocketDataTest : public::testing::TestWithParam<TiingoWebSocketData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			TiingoWebSocketData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = nullptr;
			m_pWebData = make_shared<string>(pData->m_pData);
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
			m_pWebData = nullptr;
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		shared_ptr<string> m_pWebData;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessOneTiingoCryptoWebSocketData1, ProcessOneTiingoCryptoWebSocketDataTest,
		testing::Values(&tiingoCryptoData1, &tiingoCryptoData2, &tiingoCryptoData3, &tiingoCryptoData4, &tiingoCryptoData5,
			&tiingoCryptoData6, &tiingoCryptoData7, &tiingoCryptoData9));

	TEST_P(ProcessOneTiingoCryptoWebSocketDataTest, TestProcessOneTiingoCryptoWebSocketData0) {
		bool fSucceed = false;
		CTiingoCryptoSocketPtr pCrypto;
		fSucceed = gl_pWorldMarket->ParseTiingoCryptoWebSocketData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确 Q
			EXPECT_TRUE(fSucceed);
			pCrypto = gl_SystemData.PopTiingoCryptoSocket();
			EXPECT_EQ(pCrypto->m_chMessageType, 'Q');
			EXPECT_STREQ(pCrypto->m_strExchange, _T("bitfinex"));
			EXPECT_STREQ(pCrypto->m_strSymbol, _T("neojpy"));
			EXPECT_DOUBLE_EQ(pCrypto->m_dBidSize, 38.11162867);
			EXPECT_DOUBLE_EQ(pCrypto->m_dBidPrice, 787.82);
			EXPECT_DOUBLE_EQ(pCrypto->m_dMidPrice, 787.83);
			EXPECT_DOUBLE_EQ(pCrypto->m_dAskPrice, 787.84);
			EXPECT_DOUBLE_EQ(pCrypto->m_dAskSize, 42.4153887);
			EXPECT_DOUBLE_EQ(pCrypto->m_dLastPrice, 0);
			EXPECT_DOUBLE_EQ(pCrypto->m_dLastSize, 0);
			break;
		case 2: // 正确 T
			EXPECT_TRUE(fSucceed);
			pCrypto = gl_SystemData.PopTiingoCryptoSocket();
			EXPECT_EQ(pCrypto->m_chMessageType, 'T');
			EXPECT_STREQ(pCrypto->m_strExchange, _T("binance"));
			EXPECT_STREQ(pCrypto->m_strSymbol, _T("evxbtc"));
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
			break;
		case 4: // 正确 authenizition
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
		default:
			break;
		}
	}

	// 正确数据
	TiingoWebSocketData tiingoIEXData1(1, _T(""), _T("{\"messageType\":\"A\",\"service\":\"iex\",\"data\":[\"Q\",\"2019-01-30T13:33:45.383129126-05:00\",1548873225383129126,\"vym\",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}"));
	TiingoWebSocketData tiingoIEXData2(2, _T(""), _T("{\"messageType\":\"A\",\"service\":\"iex\",\"data\":[\"T\",\"2019-01-30T13:33:45.594808294-05:00\",1548873225594808294,\"wes\",null,null,null,null,null,50.285,200,null,0,0,0,0]}"));
	// authenization
	TiingoWebSocketData tiingoIEXData3(3, _T(""), _T("{\"data\":{\"subscriptionId\":2563367},\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"}}"));
	// Heart beat
	TiingoWebSocketData tiingoIEXData4(4, _T(""), _T("{\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}"));
	// messageType只能为'A''I''H'
	TiingoWebSocketData tiingoIEXData5(5, _T(""), _T("{\"messageType\":\"B\",\"service\":\"iex\",\"data\":[\"Q\",\"2019-01-30T13:33:45.383129126-05:00\",1548873225383129126,\"vym\",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}"));
	// data的首项只能为‘Q’ ‘T’ 或者‘B’
	TiingoWebSocketData tiingoIEXData6(6, _T(""), _T("{\"messageType\":\"A\",\"service\":\"iex\",\"data\":[\"C\",\"2019-01-30T13:33:45.383129126-05:00\",1548873225383129126,\"vym\",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}"));
	// 错误。service的名称不为"iex",
	TiingoWebSocketData tiingoIEXData7(7, _T(""), _T("{\"messageType\":\"A\",\"service\":\"ex\",\"data\":[\"Q\",\"2019-01-30T13:33:45.383129126-05:00\",1548873225383129126,\"vym\",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}"));
	// service错为servi
	TiingoWebSocketData tiingoIEXData8(8, _T(""), _T("{\"messageType\":\"A\",\"servi\":\"iex\",\"data\":[\"T\",\"2019-01-30T13:33:45.594808294-05:00\",1548873225594808294,\"wes\",null,null,null,null,null,50.285,200,null,0,0,0,0]}"));
	// json格式错误
	TiingoWebSocketData tiingoIEXData9(9, _T(""), _T("\"messageType\":\"A\",\"service\":\"iex\",\"data\":[\"Q\",\"2019-01-30T13:33:45.383129126-05:00\",1548873225383129126,\"vym\",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}"));

	class ProcessOneTiingoIEXWebSocketDataTest : public::testing::TestWithParam<TiingoWebSocketData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			TiingoWebSocketData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = nullptr;
			m_pWebData = make_shared<string>(pData->m_pData);
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();

			GeneralCheck();
		}

	public:
		long m_lIndex;
		shared_ptr<string> m_pWebData;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessOneTiingoIEXWebSocketData1, ProcessOneTiingoIEXWebSocketDataTest,
		testing::Values(&tiingoIEXData1, &tiingoIEXData2, &tiingoIEXData3, &tiingoIEXData4, &tiingoIEXData5,
			&tiingoIEXData6, &tiingoIEXData7, &tiingoIEXData8, &tiingoIEXData9));

	TEST_P(ProcessOneTiingoIEXWebSocketDataTest, TestProcessOneTiingoIEXWebSocketData0) {
		CTiingoIEXSocketPtr pTiingoIEX;
		bool fSucceed = false;
		fSucceed = gl_pWorldMarket->ParseTiingoIEXWebSocketData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确 Q
			EXPECT_TRUE(fSucceed);
			pTiingoIEX = gl_SystemData.PopTiingoIEXSocket();
			EXPECT_EQ(pTiingoIEX->m_iNanoseconds, 1548873225383129126);
			EXPECT_STREQ(pTiingoIEX->m_strSymbol, _T("vym"));
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
			EXPECT_EQ(pTiingoIEX->m_iNMSRule611, 0); break;
		case 2: // 正确 T
			EXPECT_TRUE(fSucceed);
			pTiingoIEX = gl_SystemData.PopTiingoIEXSocket();
			EXPECT_EQ(pTiingoIEX->m_iNanoseconds, 1548873225594808294);
			EXPECT_STREQ(pTiingoIEX->m_strSymbol, _T("wes"));
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
		case 3: // authenization
			EXPECT_TRUE(fSucceed);
			break;
		case 4: // Heart beat
			EXPECT_TRUE(fSucceed);
			break;
		case 5: //
			EXPECT_FALSE(fSucceed);
			break;
		case 6: //
			EXPECT_FALSE(fSucceed);
			break;
		case 7: // service的名称必须为"iex"
			EXPECT_FALSE(fSucceed);
			break;
		case 8: // service错为serci
			EXPECT_FALSE(fSucceed);
			break;
		case 9: // jsong格式错误
			EXPECT_FALSE(fSucceed);
			break;
		default:
			break;
		}
	}
}