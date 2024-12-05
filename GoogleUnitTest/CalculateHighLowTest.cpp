#include"pch.h"

//#include"gtest/gtest.h"

#include"GeneralCheck.h"

#include"ChinaStock.h"

namespace FireBirdTest {
	struct HighLowData {
		HighLowData(const CString& strStockCode, long lPBuy0, long lPSell0, long lLastClose, long lHighLimit, long lLowLimit, long lHighLimit2, long lLowLimit2) {
			m_strSymbol = strStockCode;
			m_lPBuy0 = lPBuy0;
			m_lPSell0 = lPSell0;
			m_lLastClose = lLastClose;
			m_lHighLimitFromTengxun = lHighLimit;
			m_lLowLimitFromTengxun = lLowLimit;
			m_lHighLimit = lHighLimit2;
			m_lLowLimit = lLowLimit2;
		}

	public:
		CString m_strSymbol;
		long m_lPBuy0;
		long m_lPSell0;
		long m_lLastClose;
		long m_lHighLimitFromTengxun;
		long m_lLowLimitFromTengxun;
		long m_lHighLimit;
		long m_lLowLimit;
	};

	HighLowData HighLowData1("", 0, 0, 95, 110, 80, 0, 0);
	HighLowData HighLowData2("", 1400, 0, 1330, 1400, 1260, 0, 0);
	HighLowData HighLowData3("", 11170, 0, 10150, 11170, 9140, 0, 0);
	HighLowData HighLowData4("", 0, 22640, 25150, 27670, 22640, 0, 0);
	HighLowData HighLowData5("", 0, 12770, 14190, 15610, 12770, 0, 0);
	HighLowData HighLowData6("", 5690, 0, 5420, 5690, 5150, 0, 0);
	HighLowData HighLowData7("", 0, 21110, 23450, 25800, 21110, 0, 0);
	HighLowData HighLowData8("", 3250, 0, 2950, 3250, 2660, 0, 0);
	HighLowData HighLowData9("", 0, 2030, 2250, 2480, 2030, 0, 0);
	HighLowData HighLowData10("", 94970, 0, 86340, 94970, 77710, 0, 0);
	HighLowData HighLowData11("", 93760, 0, 85240, 93760, 76720, 0, 0);
	HighLowData HighLowData12("600421.SS", 0, 13810, 14540, 15270, 13810, 0, 0);
	HighLowData HighLowData13("300216.SZ", 0, 1490, 1650, 1820, 1490, 0, 0);
	HighLowData HighLowData14("", 0, 18290, 20320, 22350, 18290, 0, 0);
	HighLowData HighLowData15("600568.SS", 1790, 0, 1700, 1790, 1620, 0, 0);
	HighLowData HighLowData16("002586.SZ", 2000, 0, 1900, 2000, 1810, 0, 0);
	HighLowData HighLowData17("600145.SS", 1580, 0, 1500, 1580, 1430, 0, 0);
	HighLowData HighLowData18("600978.SS", 0, 1240, 1300, 1370, 1240, 0, 0);
	HighLowData HighLowData19("000572.SZ", 0, 3330, 3500, 3680, 3330, 0, 0);
	HighLowData HighLowData20("600543.SS", 0, 10240, 11380, 12520, 10240, 0, 0);
	HighLowData HighLowData21("603595.SS", 0, 32530, 36140, 39750, 32530, 0, 0);
	HighLowData HighLowData22("600978.SS", 0, 1240, 1300, 1370, 1240, 0, 0);
	HighLowData HighLowData23("600978.SS", 0, 1240, 1300, 1370, 1240, 0, 0);
	HighLowData HighLowData24("600978.SS", 0, 1240, 1300, 1370, 1240, 0, 0);
	HighLowData HighLowData25("600978.SS", 0, 1240, 1300, 1370, 1240, 0, 0);
	HighLowData HighLowData26("600978.SS", 0, 1240, 1300, 1370, 1240, 0, 0);
	HighLowData HighLowData27("002089.SZ", 1040, 0, 990, 1040, 940, 0, 0);
	HighLowData HighLowData28("002089.SZ", 0, 940, 990, 1040, 940, 0, 0);

	class CChinaStockHighLowLimitTest1 : public::testing::TestWithParam<HighLowData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const HighLowData* pData = GetParam();
			pRTData = make_shared<CWebRTData>();

			pRTData->SetLastClose(pData->m_lLastClose);
			pRTData->SetPBuy(0, pData->m_lPBuy0);
			pRTData->SetPSell(0, pData->m_lPSell0);
			m_stock.SetHighLimitFromTengxun(pData->m_lHighLimitFromTengxun);
			m_stock.SetLowLimitFromTengxun(pData->m_lLowLimitFromTengxun);
			m_stock.SetSymbol(pData->m_strSymbol);
		}

		void TearDown() override {
			// clearUp
			pRTData = nullptr;

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		CChinaStock m_stock;
		CWebRTDataPtr pRTData;
	};

	INSTANTIATE_TEST_SUITE_P(TestGuadanDataNew12, CChinaStockHighLowLimitTest1, testing::Values(&HighLowData1, &HighLowData2, &HighLowData3,
		                         &HighLowData4, &HighLowData5, &HighLowData6,
		                         &HighLowData7, &HighLowData8, &HighLowData9,
		                         &HighLowData10, &HighLowData11, &HighLowData12,
		                         &HighLowData13, &HighLowData14, &HighLowData15,
		                         &HighLowData16, &HighLowData17, &HighLowData18,
		                         &HighLowData19, &HighLowData20, &HighLowData21,
		                         &HighLowData22, &HighLowData23, &HighLowData24,
		                         &HighLowData25, &HighLowData26, &HighLowData27,
		                         &HighLowData28));

	TEST_P(CChinaStockHighLowLimitTest1, ChinaStockHighLowTest11) {
		m_stock.CalculateHighLowLimit(pRTData);
		EXPECT_EQ(m_stock.GetHighLimit(), m_stock.GetHighLimitFromTengxun());
		EXPECT_EQ(m_stock.GetLowLimit(), m_stock.GetLowLimitFromTengxun());
	}
}
