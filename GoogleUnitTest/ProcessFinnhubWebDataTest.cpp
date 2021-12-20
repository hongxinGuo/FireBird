#include"pch.h"
#include"globedef.h"
#include"GeneralCheck.h"

#include"WebInquirer.h"

#include"WorldStock.h"
#include"WorldMarket.h"

using namespace std;
#include<string>

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	struct FinnhubWebData {
		FinnhubWebData(long lIndex, CString strSymbol, CString strData) {
			m_lIndex = lIndex;
			m_strSymbol = strSymbol;
			m_pData = make_shared<CWebData>();
			m_pData->__TEST_SetBuffer__(strData);
		}

		~FinnhubWebData() {
		}

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

		~FinnhubWebSocketData() {
		}

	public:
		long m_lIndex;
		string m_pData;
	};

	// ��ʽ����(ȱ��ʼ�ġ�{�������޷�˳��Parser
	FinnhubWebData finnhubWebData61(1, _T("OANDA:EUR_ZAR"), _T("\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// û��s��
	FinnhubWebData finnhubWebData62_1(11, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"a\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// s����ok
	FinnhubWebData finnhubWebData62(2, _T("OANDA:EUR_ZAR"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"not ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// s���no data
	FinnhubWebData finnhubWebData63(3, _T("OANDA:EUR_ZAR"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"no_data\",\"a\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// ����ȱ��t��
	FinnhubWebData finnhubWebData64(4, _T("OANDA:EUR_ZAR"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"a\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// ȱ��c���Ӱ����
	FinnhubWebData finnhubWebData65(5, _T("OANDA:EUR_ZAR"), _T("{\"a\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// ȱ��h��
	FinnhubWebData finnhubWebData66(6, _T("OANDA:EUR_ZAR"), _T("{\"c\":[1.10159,1.10784],\"a\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// ȱ��l��
	FinnhubWebData finnhubWebData67(7, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"a\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// ȱ��o��
	FinnhubWebData finnhubWebData68(8, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"a\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// ȱ��v��
	FinnhubWebData finnhubWebData69(9, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"a\":[36521,47505]}"));
	// ��ȷ������
	FinnhubWebData finnhubWebData70(10, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));

	class ParseFinnhubForexCandleTest : public::testing::TestWithParam<FinnhubWebData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pvDayLine = nullptr;
			EXPECT_TRUE(gl_pWorldMarket->IsForexSymbol(pData->m_strSymbol));
			m_pWebData = pData->m_pData;
		}

		virtual void TearDown(void) override {
			// clearup

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CDayLineVectorPtr m_pvDayLine;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubForexCandle1,
		ParseFinnhubForexCandleTest,
		testing::Values(&finnhubWebData61, &finnhubWebData62_1, &finnhubWebData62, &finnhubWebData63, &finnhubWebData64, &finnhubWebData65,
			&finnhubWebData66, &finnhubWebData67, &finnhubWebData68, &finnhubWebData69, &finnhubWebData70));

	TEST_P(ParseFinnhubForexCandleTest, TestParseFinnhubForexCandle0) {
		CString strMessage;

		m_pvDayLine = gl_pWorldMarket->ParseFinnhubForexCandle(m_pWebData);
		switch (m_lIndex) {
		case 1: // ��ʽ����
			EXPECT_EQ(m_pvDayLine->size(), 0);
			strMessage = _T("����Ϊ��ЧJSon����\n");
			EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), strMessage);
			break;
		case 2: // s���not ok
			EXPECT_EQ(m_pvDayLine->size(), 0);
			strMessage = _T("���߷���ֵ��Ϊok\n");
			EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), strMessage);
			break;
		case 3: // s��� no data
			EXPECT_EQ(m_pvDayLine->size(), 0);
			break;
		case 4:
			EXPECT_EQ(m_pvDayLine->size(), 0);
			EXPECT_THAT(gl_systemMessage.GetErrorMessageDequeSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 5:
			EXPECT_GT(m_pvDayLine->size(), 0);
			EXPECT_THAT(gl_systemMessage.GetErrorMessageDequeSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 6:
			EXPECT_GT(m_pvDayLine->size(), 0);
			EXPECT_THAT(gl_systemMessage.GetErrorMessageDequeSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 7:
			EXPECT_GT(m_pvDayLine->size(), 0);
			EXPECT_THAT(gl_systemMessage.GetErrorMessageDequeSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 8:
			EXPECT_GT(m_pvDayLine->size(), 0);
			EXPECT_THAT(gl_systemMessage.GetErrorMessageDequeSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 9:
			EXPECT_GT(m_pvDayLine->size(), 0);
			EXPECT_THAT(gl_systemMessage.GetErrorMessageDequeSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 10:
			EXPECT_GT(m_pvDayLine->size(), 0);
			break;
		case 11: // û��s��
			EXPECT_EQ(m_pvDayLine->size(), 0);
			EXPECT_THAT(gl_systemMessage.GetErrorMessageDequeSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		default:
			break;
		}
	}

	// ��ʽ����(ȱ��ʼ�ġ�[�������޷�˳��Parser
	FinnhubWebData finnhubWebData82(2, _T(""), _T("[\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// ����ȱ��description
	FinnhubWebData finnhubWebData83(3, _T(""), _T("[{\"a\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// ����ȱ��displaySymbol
	FinnhubWebData finnhubWebData84(4, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"a\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// ����ȱ��symbol
	FinnhubWebData finnhubWebData85(5, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"a\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// ��ȷ������
	FinnhubWebData finnhubWebData90(10, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));

	class ParseFinnhubForexSymbolTest : public::testing::TestWithParam<FinnhubWebData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pvForexSymbol = nullptr;
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CForexSymbolVectorPtr m_pvForexSymbol;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubForexSymbol1, ParseFinnhubForexSymbolTest,
		testing::Values(&finnhubWebData82, &finnhubWebData83, &finnhubWebData84,
			&finnhubWebData85, &finnhubWebData90));

	TEST_P(ParseFinnhubForexSymbolTest, TestParseFinnhubForexSymbol0) {
		m_pvForexSymbol = gl_pWorldMarket->ParseFinnhubForexSymbol(m_pWebData);
		switch (m_lIndex) {
		case 2: // ��ʽ����
			EXPECT_EQ(m_pvForexSymbol->size(), 0);
			break;
		case 3: // ȱ���ַ���
			EXPECT_EQ(m_pvForexSymbol->size(), 0);
			break;
		case 4: // ȱ���ַ���
			EXPECT_EQ(m_pvForexSymbol->size(), 0);
			break;
		case 5: // ȱ���ַ���
			EXPECT_EQ(m_pvForexSymbol->size(), 0);
			break;
		case 10:
			EXPECT_STREQ(m_pvForexSymbol->at(0)->GetSymbol(), _T("OANDA:SG30_SGD"));
			EXPECT_STREQ(m_pvForexSymbol->at(1)->GetSymbol(), _T("OANDA:DE10YB_EUR"));
			EXPECT_EQ(m_pvForexSymbol->size(), 2);
			break;
		default:
			break;
		}
	}

	// ��ʽ����(ȱ��ʼ�ġ�[�������޷�˳��Parser
	FinnhubWebData finnhubWebData92(2, _T(""), _T("{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"Nauru\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));
	// ��һ������ȱ��CodeNo
	FinnhubWebData finnhubWebData93(3, _T(""), _T("[{\"code2\":\"NR\",\"code3\":\"NRU\",\"Missing\":\"520\",\"country\":\"Nauru\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));
	// �ڶ�������ȱ��Code2
	FinnhubWebData finnhubWebData94(4, _T(""), _T("[{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"Nauru\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"Missing\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));
	// ����ȱ��symbol
	FinnhubWebData finnhubWebData95(5, _T(""), _T("[{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"Nauru\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"Missing\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));
	// ��ȷ������
	FinnhubWebData finnhubWebData100(10, _T(""), _T("[{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"Nauru\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));

	class ParseFinnhubCountryListTest : public::testing::TestWithParam<FinnhubWebData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pvCountry = nullptr;
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CCountryVectorPtr m_pvCountry;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubCountryList1, ParseFinnhubCountryListTest,
		testing::Values(&finnhubWebData92, &finnhubWebData93, &finnhubWebData94,
			&finnhubWebData95, &finnhubWebData100));

	TEST_P(ParseFinnhubCountryListTest, TestParseFinnhubCountryList0) {
		m_pvCountry = gl_pWorldMarket->ParseFinnhubCountryList(m_pWebData);
		switch (m_lIndex) {
		case 2: // ��ʽ����
			EXPECT_EQ(m_pvCountry->size(), 0);
			break;
		case 3: // ȱ��CodeNo
			EXPECT_EQ(m_pvCountry->size(), 0);
			break;
		case 4: // �ڶ�������ȱCode2
			EXPECT_EQ(m_pvCountry->size(), 1);
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCode2, _T("NR"));
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCurrencyCode, _T("AUD"));
			break;
		case 5: // �ڶ�������ȱCodeNo
			EXPECT_EQ(m_pvCountry->size(), 1);
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCode2, _T("NR"));
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCurrencyCode, _T("AUD"));
			break;
		case 10:
			EXPECT_EQ(m_pvCountry->size(), 2);
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCode2, _T("NR"));
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCurrencyCode, _T("AUD"));
			EXPECT_STREQ(m_pvCountry->at(1)->m_strCode2, _T("MF"));
			EXPECT_STREQ(m_pvCountry->at(1)->m_strCurrencyCode, _T("ANG"));
			break;
		default:
			break;
		}
	}

	// ���������ַ�
	FinnhubWebData finnhubWebData102(2, _T("AAPL"), _T("[]"));
	// ��ʽ����(ȱ��ʼ�ġ�[�������޷�˳��Parser
	FinnhubWebData finnhubWebData103(3, _T("AAPL"), _T("\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
	// ��ʽ����
	FinnhubWebData finnhubWebData104(4, _T("AAPL"), _T("[\"AAPL,\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
	// ��ȷ������,������200���ַ�
	FinnhubWebData finnhubWebData105(5, _T("AAPL"), _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\",\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\"]"));
	// ��ȷ������
	FinnhubWebData finnhubWebData110(10, _T("AAPL"), _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));

	class ParseFinnhubStockPeerTest : public::testing::TestWithParam<FinnhubWebData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_strPeer = _T("");
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CString m_strPeer;
		CWebDataPtr m_pWebData;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockPeer1, ParseFinnhubStockPeerTest,
		testing::Values(&finnhubWebData102, &finnhubWebData103, &finnhubWebData104, &finnhubWebData105,
			&finnhubWebData110));

	TEST_P(ParseFinnhubStockPeerTest, TestParseFinnhubStockPeer0) {
		m_strPeer = gl_pWorldMarket->ParseFinnhubStockPeer(m_pWebData);
		switch (m_lIndex) {
		case 2: // ���������ַ�
			EXPECT_STREQ(m_strPeer, _T(""));
			break;
		case 3: // ��ʽ����
			EXPECT_STREQ(m_strPeer, _T("")) << "û�иı�";
			break;
		case 4: // �ڶ�������ȱCode2
			EXPECT_STREQ(m_strPeer, _T("")) << "û�иı�";
			break;
		case 5: // ��ȷ�����ݣ�������200���ַ�
			EXPECT_EQ(m_strPeer.GetLength(), 200) << "����200���ַ�ʱ�ض�";
			break;
		case 10:
			EXPECT_STREQ(m_strPeer, _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
			break;
		default:
			break;
		}
	}

	// ��ʽ����(ȱ��ʼ�ġ�{�������޷�˳��Parser
	FinnhubWebData finnhubWebData112(2, _T(""), _T("\"economicCalendar\":[{\"actual\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"actual\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));
	// ȱ��economicCalendar
	FinnhubWebData finnhubWebData113(3, _T(""), _T("{\"Missing\":[{\"actual\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"actual\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));
	// ��һ������ȱ��actual
	FinnhubWebData finnhubWebData114(4, _T(""), _T("{\"economicCalendar\":[{\"Missing\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"actual\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));
	// �ڶ�������ȱ��actual
	FinnhubWebData finnhubWebData115(5, _T(""), _T("{\"economicCalendar\":[{\"actual\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"Missing\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));
	// ��ȷ������
	FinnhubWebData finnhubWebData120(10, _T(""), _T("{\"economicCalendar\":[{\"actual\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"actual\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));

	class ParseFinnhubEconomicCalendarTest : public::testing::TestWithParam<FinnhubWebData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pvEconomicCalendar = nullptr;
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CEconomicCalendarVectorPtr m_pvEconomicCalendar;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubEconomicCalendar1, ParseFinnhubEconomicCalendarTest,
		testing::Values(&finnhubWebData112, &finnhubWebData113, &finnhubWebData114,
			&finnhubWebData115, &finnhubWebData120));

	TEST_P(ParseFinnhubEconomicCalendarTest, TestParseFinnhubEconomicCalendar10) {
		m_pvEconomicCalendar = gl_pWorldMarket->ParseFinnhubEconomicCalendar(m_pWebData);
		switch (m_lIndex) {
		case 2: // ��ʽ����
			EXPECT_EQ(m_pvEconomicCalendar->size(), 0);
			break;
		case 3: // ȱ��economicCalendar
			EXPECT_EQ(m_pvEconomicCalendar->size(), 0);
			break;
		case 4: // ��һ������ȱactual
			EXPECT_EQ(m_pvEconomicCalendar->size(), 0);
			break;
		case 5: // �ڶ�������ȱactual
			EXPECT_EQ(m_pvEconomicCalendar->size(), 1);
			EXPECT_DOUBLE_EQ(m_pvEconomicCalendar->at(0)->m_dActual, 0.6);
			EXPECT_STREQ(m_pvEconomicCalendar->at(0)->m_strUnit, _T("%"));
			break;
		case 10:
			EXPECT_EQ(m_pvEconomicCalendar->size(), 2);
			EXPECT_DOUBLE_EQ(m_pvEconomicCalendar->at(0)->m_dActual, 0.6);
			EXPECT_STREQ(m_pvEconomicCalendar->at(0)->m_strUnit, _T("%"));
			EXPECT_DOUBLE_EQ(m_pvEconomicCalendar->at(1)->m_dActual, -0.2);
			EXPECT_STREQ(m_pvEconomicCalendar->at(1)->m_strUnit, _T("%"));
			EXPECT_DOUBLE_EQ(m_pvEconomicCalendar->at(0)->m_dEstimate, 0.6);
			EXPECT_DOUBLE_EQ(m_pvEconomicCalendar->at(0)->m_dPrev, 1);
			EXPECT_STREQ(m_pvEconomicCalendar->at(0)->m_strCountry, _T("CN"));
			EXPECT_STREQ(m_pvEconomicCalendar->at(0)->m_strEvent, _T("CPI MM"));
			EXPECT_STREQ(m_pvEconomicCalendar->at(0)->m_strTime, _T("2021-03-10 01:30:00"));
			break;
		default:
			break;
		}
	}

	// ��ʽ����(ȱ��ʼ�ġ�[�������޷�˳��Parser
	FinnhubWebData finnhubWebData122(2, _T("AAPL"), _T("{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
	// ��һ������ȱ��actual
	FinnhubWebData finnhubWebData123(3, _T("AAPL"), _T("[{\"Missing\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
	// �ڶ�������ȱ��actual
	FinnhubWebData finnhubWebData124(4, _T("AAPL"), _T("[{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"Missing\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
	// ����������ȱ��actual
	FinnhubWebData finnhubWebData125(5, _T("AAPL"), _T("[{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"Missing\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
	// ��ȷ������
	FinnhubWebData finnhubWebData130(10, _T("AAPL"), _T("[{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));

	class ParseFinnhubEPSSurpriseTest : public::testing::TestWithParam<FinnhubWebData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pWebData = pData->m_pData;
			m_pvEPSSurprise = nullptr;
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetProfileUpdated(false);
			m_pStock->SetUpdateProfileDB(false);
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CEPSSurpriseVectorPtr m_pvEPSSurprise;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubEPSSurprise1, ParseFinnhubEPSSurpriseTest,
		testing::Values(&finnhubWebData122, &finnhubWebData123, &finnhubWebData124,
			&finnhubWebData125, &finnhubWebData130));

	TEST_P(ParseFinnhubEPSSurpriseTest, TestParseFinnhubEPSSurprise0) {
		m_pvEPSSurprise = gl_pWorldMarket->ParseFinnhubEPSSurprise(m_pWebData);
		switch (m_lIndex) {
		case 2: // ��ʽ����
			EXPECT_EQ(m_pvEPSSurprise->size(), 0);
			break;
		case 3: //
			EXPECT_EQ(m_pvEPSSurprise->size(), 0);
			break;
		case 4: // �ڶ�������ȱȱactual
			EXPECT_EQ(m_pvEPSSurprise->size(), 1);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0)->m_dActual, 1.68);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0)->m_dEstimate, 1.555857);
			EXPECT_EQ(m_pvEPSSurprise->at(0)->m_lDate, 20201231);
			EXPECT_STREQ(m_pvEPSSurprise->at(0)->m_strSymbol, _T("AAPL"));
			break;
		case 5: // ����������ȱCodeNo
			EXPECT_EQ(m_pvEPSSurprise->size(), 2);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0)->m_dActual, 1.68);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0)->m_dEstimate, 1.555857);
			EXPECT_EQ(m_pvEPSSurprise->at(0)->m_lDate, 20201231);
			EXPECT_STREQ(m_pvEPSSurprise->at(0)->m_strSymbol, _T("AAPL"));
			break;
		case 10:
			EXPECT_EQ(m_pvEPSSurprise->size(), 4);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0)->m_dActual, 0.6375);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0)->m_dEstimate, 0.5765856);
			EXPECT_EQ(m_pvEPSSurprise->at(0)->m_lDate, 20200331);
			EXPECT_STREQ(m_pvEPSSurprise->at(0)->m_strSymbol, _T("AAPL"));
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(3)->m_dActual, 1.68) << "�ɹ�������Զ����������У������䱻���������";
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(3)->m_dEstimate, 1.555857);
			EXPECT_EQ(m_pvEPSSurprise->at(3)->m_lDate, 20201231);
			EXPECT_STREQ(m_pvEPSSurprise->at(3)->m_strSymbol, _T("AAPL"));
			break;
		default:
			break;
		}
	}

	FinnhubWebData finnhubWebData132(2, _T("AAPL"), _T("{\"data\":[{\"name\":\"Long Brady K\",\"share\":269036,\"change\":-14236,\"filingDate\":\"2021-03-03\",\"transactionDate\":\"2021-03-02\",\"transactionCode\":\"F\",\"transactionPrice\":3.68},{\"name\":\"Adamson Keelan\",\"share\":221083,\"change\":-11347,\"filingDate\" : \"2021-03-03\",\"transactionDate\" : \"2021-03-02\",\"transactionCode\" : \"F\",\"transactionPrice\" : 3.68 }] , \"symbol\" : \"RIG\"}"));

	class ParseFinnhubInsiderTransactionTest : public::testing::TestWithParam<FinnhubWebData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pWebData = pData->m_pData;
			m_pvInsiderTransaction = nullptr;
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();

			GeneralCheck();
			m_pStock->SetUpdateProfileDB(false);
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CInsiderTransactionVectorPtr m_pvInsiderTransaction;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubInsiderTransaction1, ParseFinnhubInsiderTransactionTest,
		testing::Values(&finnhubWebData132));

	TEST_P(ParseFinnhubInsiderTransactionTest, TestParseFinnhubInsiderTransaction0) {
		m_pvInsiderTransaction = gl_pWorldMarket->ParseFinnhubStockInsiderTransaction(m_pWebData);
		switch (m_lIndex) {
		case 2: // ��ʽ����
			EXPECT_EQ(m_pvInsiderTransaction->size(), 2);
			EXPECT_STREQ(m_pvInsiderTransaction->at(0)->m_strPersonName, _T("Long Brady K"));
			EXPECT_STREQ(m_pvInsiderTransaction->at(0)->m_strSymbol, _T("RIG"));
			EXPECT_EQ(m_pvInsiderTransaction->at(0)->m_lShare, 269036);
			EXPECT_EQ(m_pvInsiderTransaction->at(0)->m_lChange, -14236);
			EXPECT_EQ(m_pvInsiderTransaction->at(0)->m_lFilingDate, 20210303);
			EXPECT_EQ(m_pvInsiderTransaction->at(0)->m_lTransactionDate, 20210302);
			EXPECT_DOUBLE_EQ(m_pvInsiderTransaction->at(0)->m_dTransactionPrice, 3.68);
			EXPECT_TRUE(m_pvInsiderTransaction->at(0)->m_lTransactionDate <= m_pvInsiderTransaction->at(1)->m_lTransactionDate) << "�����а���������˳������";
			break;
		default:
			break;
		}
	}

	// ��ȷ����
	FinnhubWebSocketData finnhubWebSocketData141(1, _T("{\"data\":[{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.76,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":43},{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.75,\"s\":\"A\",\"t\":1628238530220,\"v\":1}],\"type\":\"trade\"}"));
	// ��ȷ��ping���ݸ�ʽ
	FinnhubWebSocketData finnhubWebSocketData142(2, _T("{\"type\":\"ping\"}"));
	// json��ʽ���� ���ش���
	FinnhubWebSocketData finnhubWebSocketData143(3, _T("\"data\":[{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.76,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":43},{\"c\":[\"1\",\"24\",\"12\",\"p\":146.75,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":1},\"type\":\"trade\"}"));
	// typeֻ����"trade","ping"����"error"
	FinnhubWebSocketData finnhubWebSocketData144(4, _T("{\"data\":[{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.76,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":43},{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.75,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":1}],\"type\":\"message\"}"));
	// ��ȷ��error���ݸ�ʽ
	FinnhubWebSocketData finnhubWebSocketData145(5, _T("{\"msg\":\"Subscribing to too many symbols\",\"type\":\"error\"}"));
	// "dta"�Ƿ�����
	FinnhubWebSocketData finnhubWebSocketData149(9, _T("{\"dta\":[{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.76,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":43},{\"c\":[\"1\",\"24\",\"12\"],\"p\":146.75,\"s\":\"AAPL\",\"t\":1628238530221,\"v\":1}],\"type\":\"trade\"}"));

	class ProcessOneFinnhubWebSocketDataTest : public::testing::TestWithParam<FinnhubWebSocketData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebSocketData* pData = GetParam();
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

	INSTANTIATE_TEST_SUITE_P(TestProcessOneFinnhubWebSocketData1, ProcessOneFinnhubWebSocketDataTest,
		testing::Values(&finnhubWebSocketData141, &finnhubWebSocketData142, &finnhubWebSocketData143, &finnhubWebSocketData144,
			&finnhubWebSocketData145, &finnhubWebSocketData149));

	TEST_P(ProcessOneFinnhubWebSocketDataTest, TestProcessOneFinnhubWebSocketData0) {
		bool fSucceed = false;
		CFinnhubSocketPtr pFinnhubWebSocket;
		fSucceed = gl_pWorldMarket->ProcessOneFinnhubWebSocketData(m_pWebData);
		switch (m_lIndex) {
		case 1: // ��ȷ
			EXPECT_TRUE(fSucceed);
			pFinnhubWebSocket = gl_SystemData.PopFinnhubSocket();
			EXPECT_STREQ(pFinnhubWebSocket->m_strSymbol, _T("AAPL"));
			//EXPECT_STREQ(pFinnhubWebSocket->m_strCode, _T("")); // CodeĿǰ������
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastPrice, 146.76);
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastVolume, 43);
			EXPECT_EQ(pFinnhubWebSocket->m_iSeconds, 1628238530221);
			pFinnhubWebSocket = gl_SystemData.PopFinnhubSocket();
			EXPECT_STREQ(pFinnhubWebSocket->m_strSymbol, _T("A"));
			//EXPECT_STREQ(pFinnhubWebSocket->m_strCode, _T("")); // CodeĿǰ������
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastPrice, 146.75);
			EXPECT_DOUBLE_EQ(pFinnhubWebSocket->m_dLastVolume, 1);
			EXPECT_EQ(pFinnhubWebSocket->m_iSeconds, 1628238530220);
			break;
		case 2: // ping
			EXPECT_TRUE(fSucceed);
			break;
		case 3: // json��ʽ����
			EXPECT_FALSE(fSucceed);
			break;
		case 4: // type���Ͳ�����
			EXPECT_FALSE(fSucceed);
			break;
		case 5: // error message
			EXPECT_FALSE(fSucceed);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Finnhub WebSocket error message: Subscribing to too many symbols"));
			break;
		case 9: // data����Ϊ"data"
			EXPECT_FALSE(fSucceed);
			break;
		default:
			break;
		}
	}

	// ��ʽ����(ȱ��ʼ�ġ�[�������޷�˳��Parser
	FinnhubWebData finnhubWebData202(2, _T(""), _T("\"oanda\",\"fxcm\",\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]"));
	// ��ʽ����
	FinnhubWebData finnhubWebData203(3, _T(""), _T("[\"oanda\",fxcm,\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]"));
	// ��ȷ������
	FinnhubWebData finnhubWebData210(10, _T(""), _T("[\"oanda\",\"fxcm\",\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]"));

	class ParseFinnhubCryptoExchangeTest : public::testing::TestWithParam<FinnhubWebData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pvExchange = nullptr;
		}
		virtual void TearDown(void) override {
			// clearup
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		shared_ptr<vector<CString>> m_pvExchange;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubCryptoExchange1, ParseFinnhubCryptoExchangeTest,
		testing::Values(&finnhubWebData202, &finnhubWebData203, &finnhubWebData210));

	TEST_P(ParseFinnhubCryptoExchangeTest, TestParseFinnhubCryptoExchange0) {
		m_pvExchange = gl_pWorldMarket->ParseFinnhubCryptoExchange(m_pWebData);
		switch (m_lIndex) {
		case 2: // ��ʽ����
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 3: // ȱ���ַ���
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 10:
			EXPECT_STREQ(m_pvExchange->at(0), _T("oanda"));
			EXPECT_STREQ(m_pvExchange->at(1), _T("fxcm"));
			EXPECT_EQ(m_pvExchange->size(), 9);
			break;
		default:
			break;
		}
	}

	// ��ʽ����(ȱ��ʼ�ġ�[�������޷�˳��Parser
	FinnhubWebData finnhubWebData212(2, _T(""), _T("[\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// ����ȱ��description
	FinnhubWebData finnhubWebData213(3, _T(""), _T("[{\"a\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// ����ȱ��displaySymbol
	FinnhubWebData finnhubWebData214(4, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"a\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// ����ȱ��symbol
	FinnhubWebData finnhubWebData215(5, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"a\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// ��ȷ������
	FinnhubWebData finnhubWebData220(10, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));

	class ParseFinnhubCryptoSymbolTest : public::testing::TestWithParam<FinnhubWebData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pvCryptoSymbol = nullptr;
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CCryptoSymbolVectorPtr m_pvCryptoSymbol;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubCryptoSymbol1, ParseFinnhubCryptoSymbolTest,
		testing::Values(&finnhubWebData82, &finnhubWebData83, &finnhubWebData84,
			&finnhubWebData85, &finnhubWebData90));

	TEST_P(ParseFinnhubCryptoSymbolTest, TestParseFinnhubCryptoSymbol0) {
		m_pvCryptoSymbol = gl_pWorldMarket->ParseFinnhubCryptoSymbol(m_pWebData);
		switch (m_lIndex) {
		case 2: // ��ʽ����
			EXPECT_EQ(m_pvCryptoSymbol->size(), 0);
			break;
		case 3: // ȱ���ַ���
			EXPECT_EQ(m_pvCryptoSymbol->size(), 0);
			break;
		case 4: // ȱ���ַ���
			EXPECT_EQ(m_pvCryptoSymbol->size(), 0);
			break;
		case 5: // ȱ���ַ���
			EXPECT_EQ(m_pvCryptoSymbol->size(), 0);
			break;
		case 10:
			EXPECT_STREQ(m_pvCryptoSymbol->at(0)->GetSymbol(), _T("OANDA:SG30_SGD"));
			EXPECT_STREQ(m_pvCryptoSymbol->at(1)->GetSymbol(), _T("OANDA:DE10YB_EUR"));
			EXPECT_EQ(m_pvCryptoSymbol->size(), 2);
			break;
		default:
			break;
		}
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
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubCryptoCandle1,
		ParseFinnhubCryptoCandleTest,
		testing::Values(&finnhubWebData221, &finnhubWebData222_1, &finnhubWebData222, &finnhubWebData223, &finnhubWebData224, &finnhubWebData225,
			&finnhubWebData226, &finnhubWebData227, &finnhubWebData228, &finnhubWebData229, &finnhubWebData230));

	TEST_P(ParseFinnhubCryptoCandleTest, TestParseFinnhubCryptoCandle0) {
		CString strMessage;

		m_pvDayLine = gl_pWorldMarket->ParseFinnhubCryptoCandle(m_pWebData);
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