#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubCryptoDayLine.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubCryptoDayLineTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearu
			GeneralCheck();
		}

	protected:
		CFinnhubCryptoDayLine cryptoDayLine;
	};

	TEST_F(CFinnhubCryptoDayLineTest, TestInitialize) {
		EXPECT_EQ(cryptoDayLine.GetIndex(), -1);
		EXPECT_STREQ(cryptoDayLine.GetInquiringStr(), _T("https://finnhub.io/api/v1/crypto/candle?symbol="));
	}

	TEST_F(CFinnhubCryptoDayLineTest, TestCreatMessage) {
		cryptoDayLine.SetMarket(gl_pWorldMarket.get());
		cryptoDayLine.SetIndex(1);
		EXPECT_STREQ(cryptoDayLine.CreatMessage(),
			cryptoDayLine.GetInquiringStr() + gl_pWorldMarket->GetCryptoSymbol(1)->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
	}

	TEST_F(CFinnhubCryptoDayLineTest, TestProcessWebData) {
		// ��MockWorldMarketTest�������
	}

	// ��ʽ����(ȱ��ʼ�ġ�{�������޷�˳��Parser
	FinnhubWebData finnhubWebData221(1, _T("OANDA:EUR_ZAR"), _T("\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// û��s��
	FinnhubWebData finnhubWebData222_1(11, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"a\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// s����ok
	FinnhubWebData finnhubWebData222(2, _T("OANDA:EUR_ZAR"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"not ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// s���no data
	FinnhubWebData finnhubWebData223(3, _T("OANDA:EUR_ZAR"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"no_data\",\"a\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// ����ȱ��t��
	FinnhubWebData finnhubWebData224(4, _T("OANDA:EUR_ZAR"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"a\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// ȱ��c���Ӱ����
	FinnhubWebData finnhubWebData225(5, _T("OANDA:EUR_ZAR"), _T("{\"a\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// ȱ��h��
	FinnhubWebData finnhubWebData226(6, _T("OANDA:EUR_ZAR"), _T("{\"c\":[1.10159,1.10784],\"a\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// ȱ��l��
	FinnhubWebData finnhubWebData227(7, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"a\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// ȱ��o��
	FinnhubWebData finnhubWebData228(8, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"a\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// ȱ��v��
	FinnhubWebData finnhubWebData229(9, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"a\":[36521,47505]}"));
	// ��ȷ������
	FinnhubWebData finnhubWebData230(10, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));

	class ParseFinnhubCryptoCandleTest : public::testing::TestWithParam<FinnhubWebData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			EXPECT_TRUE(gl_pWorldMarket->IsCryptoSymbol(pData->m_strSymbol));
			m_pWebData = pData->m_pData;
			m_pvDayLine = nullptr;
		}

		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CDayLineVectorPtr m_pvDayLine;
		CWebDataPtr m_pWebData;
		CFinnhubCryptoDayLine m_finnhubSryptoDayLine;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubCryptoCandle1,
		ParseFinnhubCryptoCandleTest,
		testing::Values(&finnhubWebData221, &finnhubWebData222_1, &finnhubWebData222, &finnhubWebData223, &finnhubWebData224, &finnhubWebData225,
			&finnhubWebData226, &finnhubWebData227, &finnhubWebData228, &finnhubWebData229, &finnhubWebData230));

	TEST_P(ParseFinnhubCryptoCandleTest, TestParseFinnhubCryptoCandle0) {
		CString strMessage;

		m_pvDayLine = m_finnhubSryptoDayLine.ParseFinnhubCryptoCandle(m_pWebData);
		switch (m_lIndex) {
		case 1: // ��ʽ����
			EXPECT_EQ(m_pvDayLine->size(), 0);
			strMessage += _T("����Ϊ��ЧJSon����");
			EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), strMessage);
			break;
		case 2: // s���not ok
			EXPECT_EQ(m_pvDayLine->size(), 0);
			strMessage = _T("���߷���ֵ��Ϊok");
			EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), strMessage);
			break;
		case 3: // s��� no data
			EXPECT_EQ(m_pvDayLine->size(), 0);
			break;
		case 4: //����ȱ��t��
			EXPECT_EQ(m_pvDayLine->size(), 0);
			break;
		case 5:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 6:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 7:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 8:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 9:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 10:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 11: // û��s��
			EXPECT_EQ(m_pvDayLine->size(), 0);
			break;
		default:
			break;
		}
	}
}