#include"pch.h"
#include"globedef.h"
#include"GeneralCheck.h"

#include"ChinaMarket.h"

#include"WebRTData.h"
#include"NeteaseRTWebInquiry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	TEST(CWebRTDataTest, TestNeteaseInitialize) {
		ASSERT_FALSE(gl_fNormalMode);
		CWebRTData RTData;
		EXPECT_EQ(RTData.GetTransactionTime(), 0);
		EXPECT_STREQ(RTData.GetSymbol(), _T(""));
		EXPECT_STREQ(RTData.GetStockName(), _T(""));
		EXPECT_EQ(RTData.GetOpen(), 0);
		EXPECT_EQ(RTData.GetLastClose(), 0);
		EXPECT_EQ(RTData.GetNew(), 0);
		EXPECT_EQ(RTData.GetHigh(), 0);
		EXPECT_EQ(RTData.GetLow(), 0);
		EXPECT_EQ(RTData.GetBuy(), 0);
		EXPECT_EQ(RTData.GetSell(), 0);
		EXPECT_EQ(RTData.GetVolume(), 0);
		EXPECT_EQ(RTData.GetAmount(), 0);
		for (int i = 0; i < 5; i++) {
			EXPECT_EQ(RTData.GetVBuy(i), 0);
			EXPECT_EQ(RTData.GetPBuy(i), 0);
			EXPECT_EQ(RTData.GetVSell(i), 0);
			EXPECT_EQ(RTData.GetPSell(i), 0);
		}
		EXPECT_FALSE(RTData.IsActive());
	}

	TEST(CWebRTDataTest, TestNeteaseRTDataActive) {
		CWebRTData id;
		EXPECT_FALSE(id.CheckNeteaseRTDataActive());
		tm tm_;
		tm_.tm_year = 2019 - 1900;
		tm_.tm_mon = 10;
		tm_.tm_mday = 7; // 2019年11月7日是星期四。
		tm_.tm_hour = 12;
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		time_t tt = gl_pChinaMarket->TransferToUTCTime(&tm_);
		gl_pChinaMarket->__TEST_SetUTCTime(tt);
		id.SetTransactionTime(tt);
		EXPECT_TRUE(id.IsValidTime(14));
		EXPECT_FALSE(id.CheckNeteaseRTDataActive());
		id.SetOpen(10);
		EXPECT_TRUE(id.CheckNeteaseRTDataActive());
		id.SetOpen(0);
		id.SetVolume(10);
		EXPECT_TRUE(id.CheckNeteaseRTDataActive());
		id.SetVolume(0);
		id.SetHigh(10);
		EXPECT_TRUE(id.CheckNeteaseRTDataActive());
		id.SetHigh(0);
		id.SetLow(10);
		EXPECT_TRUE(id.CheckNeteaseRTDataActive());
	}

	struct NeteaseRTData {
		NeteaseRTData(int count, CString Data) {
			m_iCount = count;
			m_strData = Data;
		}
	public:
		int m_iCount;
		CString m_strData;
	};

	// 无错误数据
	NeteaseRTData Data101(0, _T("{\"0600000\":{\"code\": \"0600000\", \"percent\": -0.022275, \"high\": 12.48, \"askvol3\": 162290, \"askvol2\": 106387, \"askvol5\": 609700, \"askvol4\": 237059, \"price\": 12.29, \"open\": 12.48, \"bid5\": 12.25, \"bid4\": 12.26, \"bid3\": 12.27, \"bid2\": 12.28, \"bid1\": 12.29, \"low\": 12.29, \"updown\": -0.28, \"type\": \"SH\", \"symbol\": \"600000\", \"status\": 0, \"ask4\": 12.33, \"bidvol3\": 118700, \"bidvol2\": 184600, \"bidvol1\": 178647, \"update\": \"2019/11/11 15:59:50\", \"bidvol5\": 640700, \"bidvol4\": 175500, \"yestclose\": 12.57, \"askvol1\": 51100, \"ask5\": 12.34, \"volume\": 38594267, \"ask1\": 12.3, \"name\": \"浦发银行\", \"ask3\": 12.32, \"ask2\": 12.31, \"arrow\": \"\u2193\", \"time\": \"2019/11/11 15:59:55\", \"turnover\": 477989511} });"));
	// 两个数据，无错误
	NeteaseRTData Data102(1, _T("{\"0600601\":{\"code\": \"0600601\", \"percent\": -0.003077, \"high\": 3.3, \"askvol3\": 269300, \"askvol2\": 133985, \"askvol5\": 283900, \"askvol4\": 89800, \"price\": 3.24, \"open\": 3.25, \"bid5\": 3.2, \"bid4\": 3.21, \"bid3\": 3.22, \"bid2\": 3.23, \"bid1\": 3.24, \"low\": 3.22, \"updown\": -0.01, \"type\": \"SH\", \"symbol\": \"600601\", \"status\": 0, \"ask4\": 3.28, \"bidvol3\": 113300, \"bidvol2\": 472100, \"bidvol1\": 24100, \"update\": \"2019/11/11 15:59:55\", \"bidvol5\": 181600, \"bidvol4\": 94800, \"yestclose\": 3.25, \"askvol1\": 143800, \"ask5\": 3.29, \"volume\": 9349540, \"ask1\": 3.25, \"name\": \"\u65b9\u6b63\u79d1\u6280\", \"ask3\": 3.27, \"ask2\": 3.26, \"arrow\": \"\u2193\", \"time\": \"2019/11/11 15:59:55\", \"turnover\": 30503027},\"0600000\":{\"code\": \"0600000\", \"percent\": -0.022275, \"high\": 12.48, \"askvol3\": 162290, \"askvol2\": 106387, \"askvol5\": 609700, \"askvol4\": 237059, \"price\": 12.29, \"open\": 12.48, \"bid5\": 12.25, \"bid4\": 12.26, \"bid3\": 12.27, \"bid2\": 12.28, \"bid1\": 12.29, \"low\": 12.29, \"updown\": -0.28, \"type\": \"SH\", \"symbol\": \"600000\", \"status\": 0, \"ask4\": 12.33, \"bidvol3\": 118700, \"bidvol2\": 184600, \"bidvol1\": 178647, \"update\": \"2019/11/11 15:59:55\", \"bidvol5\": 640700, \"bidvol4\": 175500, \"yestclose\": 12.57, \"askvol1\": 51100, \"ask5\": 12.34, \"volume\": 38594267, \"ask1\": 12.3, \"name\": \"\u6d66\u53d1\u94f6\u884c\", \"ask3\": 12.32, \"ask2\": 12.31, \"arrow\": \"\u2193\", \"time\": \"2019/11/11 15:59:53\", \"turnover\": 477989511} });"));
	// 所有的数量皆为零
	NeteaseRTData Data103(2, _T("{\"0600000\":{\"cod\": \"0600000\", \"percent\": -0.022275, \"high\": 12.48, \"askvol3\": 162290, \"askvol2\": 106387, \"askvol5\": 609700, \"askvol4\": 237059, \"price\": 12.29, \"open\": 12.48, \"bid5\": 12.25, \"bid4\": 12.26, \"bid3\": 12.27, \"bid2\": 12.28, \"bid1\": 12.29, \"low\": 12.29, \"updown\": -0.28, \"type\": \"SH\", \"symbol\": \"600000\", \"status\": 0, \"ask4\": 12.33, \"bidvol3\": 118700, \"bidvol2\": 184600, \"bidvol1\": 178647, \"update\": \"2019/11/11 15:59:50\", \"bidvol5\": 640700, \"bidvol4\": 175500, \"yestclose\": 12.57, \"askvol1\": 51100, \"ask5\": 12.34, \"volume\": 38594267, \"ask1\": 12.3, \"name\": \"浦发银行\", \"ask3\": 12.32, \"ask2\": 12.31, \"arrow\": \"\u2193\", \"time\": \"2019/11/11 15:59:55\", \"turnover\": 477989511} });"));
	// 两个数据，第一个错误，第二个正确
	NeteaseRTData Data104(3, _T("{\"0600601\":{\"code\": \"0600601\", \"percent\": -0.003077, \"hig\": 3.3, \"askvol3\": 269300, \"askvol2\": 133985, \"askvol5\": 283900, \"askvol4\": 89800, \"price\": 3.24, \"open\": 3.25, \"bid5\": 3.2, \"bid4\": 3.21, \"bid3\": 3.22, \"bid2\": 3.23, \"bid1\": 3.24, \"low\": 3.22, \"updown\": -0.01, \"type\": \"SH\", \"symbol\": \"600601\", \"status\": 0, \"ask4\": 3.28, \"bidvol3\": 113300, \"bidvol2\": 472100, \"bidvol1\": 24100, \"update\": \"2019/11/11 15:59:55\", \"bidvol5\": 181600, \"bidvol4\": 94800, \"yestclose\": 3.25, \"askvol1\": 143800, \"ask5\": 3.29, \"volume\": 9349540, \"ask1\": 3.25, \"name\": \"\u65b9\u6b63\u79d1\u6280\", \"ask3\": 3.27, \"ask2\": 3.26, \"arrow\": \"\u2193\", \"time\": \"2019/11/11 15:59:55\", \"turnover\": 30503027},\"0600000\":{\"code\": \"0600000\", \"percent\": -0.022275, \"high\": 12.48, \"askvol3\": 162290, \"askvol2\": 106387, \"askvol5\": 609700, \"askvol4\": 237059, \"price\": 12.29, \"open\": 12.48, \"bid5\": 12.25, \"bid4\": 12.26, \"bid3\": 12.27, \"bid2\": 12.28, \"bid1\": 12.29, \"low\": 12.29, \"updown\": -0.28, \"type\": \"SH\", \"symbol\": \"600000\", \"status\": 0, \"ask4\": 12.33, \"bidvol3\": 118700, \"bidvol2\": 184600, \"bidvol1\": 178647, \"update\": \"2019/11/11 15:59:55\", \"bidvol5\": 640700, \"bidvol4\": 175500, \"yestclose\": 12.57, \"askvol1\": 51100, \"ask5\": 12.34, \"volume\": 38594267, \"ask1\": 12.3, \"name\": \"浦发银行\", \"ask3\": 12.32, \"ask2\": 12.31, \"arrow\": \"\u2193\", \"time\": \"2019/11/11 15:59:53\", \"turnover\": 477989511} });"));

	class CalculateNeteaseWebRTDataTest : public::testing::TestWithParam<NeteaseRTData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();

			NeteaseRTData* pData = GetParam();
			m_pNeteaseWebRTData = make_shared<CWebData>();
			m_iCount = pData->m_iCount;
			m_lStringLength = pData->m_strData.GetLength();
			for (int i = 0; i < m_lStringLength; i++) {
				m_pNeteaseWebRTData->SetData(i, pData->m_strData[i]);
			}
			m_pNeteaseWebRTData->SetData(m_lStringLength, 0x000);
			m_pNeteaseWebRTData->SetBufferLength(m_lStringLength);
			m_pNeteaseWebRTData->ResetCurrentPos();
			for (int i = 0; i < 5; i++) {
				m_RTData.SetPBuy(i, -1);
				m_RTData.SetPSell(i, -1);
				m_RTData.SetVBuy(i, -1);
				m_RTData.SetVSell(i, -1);
			}
			m_RTData.SetAmount(-1);
			m_RTData.SetVolume(-1);
			m_RTData.SetOpen(-1);
			m_RTData.SetNew(-1);
			m_RTData.SetLastClose(-1);
			m_RTData.SetHigh(-1);
			m_RTData.SetLow(-1);
			m_RTData.SetSell(-1);
			m_RTData.SetBuy(-1);
		}

		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();

			GeneralCheck();
		}

	public:
		int m_iCount;
		char* m_pData;
		long m_lStringLength;
		CWebDataPtr m_pNeteaseWebRTData;
		CWebRTData m_RTData;
	};

	INSTANTIATE_TEST_SUITE_P(TestNeteaseRTData, CalculateNeteaseWebRTDataTest, testing::Values(&Data101, &Data102, &Data103, &Data104
	));

	TEST_P(CalculateNeteaseWebRTDataTest, TestNeteaseRTData) {
		bool fSucceed = m_RTData.ReadNeteaseData(m_pNeteaseWebRTData);
		time_t ttime, ttime2, ttime3;
		tm tm_;
		tm_.tm_year = 2019 - 1900;
		tm_.tm_mon = 11 - 1;
		tm_.tm_mday = 11;
		tm_.tm_hour = 15;
		tm_.tm_min = 59;
		tm_.tm_sec = 50;
		ttime = gl_pChinaMarket->TransferToUTCTime(&tm_);
		tm_.tm_sec = 53;
		ttime2 = gl_pChinaMarket->TransferToUTCTime(&tm_);
		tm_.tm_sec = 55;
		ttime3 = gl_pChinaMarket->TransferToUTCTime(&tm_);
		switch (m_iCount) {
		case 0:
			EXPECT_TRUE(fSucceed); // 没有错误
			EXPECT_EQ(m_lStringLength, m_pNeteaseWebRTData->GetCurrentPos() + 4); // 最后剩下四个字符" });"没有读
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T(""));
			EXPECT_EQ(m_RTData.GetOpen(), 12480);
			EXPECT_EQ(m_RTData.GetLastClose(), 12570);
			EXPECT_EQ(m_RTData.GetNew(), 12290);
			EXPECT_EQ(m_RTData.GetVolume(), 38594267);
			EXPECT_EQ(m_RTData.GetVBuy(0), 178647);
			EXPECT_EQ(m_RTData.GetPBuy(0), 12290);
			EXPECT_EQ(m_RTData.GetVBuy(1), 184600);
			EXPECT_EQ(m_RTData.GetPBuy(1), 12280);
			EXPECT_EQ(m_RTData.GetVBuy(2), 118700);
			EXPECT_EQ(m_RTData.GetPBuy(2), 12270);
			EXPECT_EQ(m_RTData.GetVBuy(3), 175500);
			EXPECT_EQ(m_RTData.GetPBuy(3), 12260);
			EXPECT_EQ(m_RTData.GetVBuy(4), 640700);
			EXPECT_EQ(m_RTData.GetPBuy(4), 12250);
			EXPECT_EQ(m_RTData.GetVSell(0), 51100);
			EXPECT_EQ(m_RTData.GetPSell(0), 12300);
			EXPECT_EQ(m_RTData.GetVSell(1), 106387);
			EXPECT_EQ(m_RTData.GetPSell(1), 12310);
			EXPECT_EQ(m_RTData.GetVSell(2), 162290);
			EXPECT_EQ(m_RTData.GetPSell(2), 12320);
			EXPECT_EQ(m_RTData.GetVSell(3), 237059);
			EXPECT_EQ(m_RTData.GetPSell(3), 12330);
			EXPECT_EQ(m_RTData.GetVSell(4), 609700);
			EXPECT_EQ(m_RTData.GetPSell(4), 12340);
			EXPECT_EQ(m_RTData.GetTransactionTime(), ttime);
			break;
		case 1:
			EXPECT_TRUE(fSucceed); // 第一个数据没有错误
			EXPECT_EQ(m_RTData.GetTransactionTime(), ttime3);
			fSucceed = m_RTData.ReadNeteaseData(m_pNeteaseWebRTData);
			EXPECT_TRUE(fSucceed); // 第二个数据没有错误
			EXPECT_EQ(m_lStringLength, m_pNeteaseWebRTData->GetCurrentPos() + 4); // 最后剩下四个字符" });"没有读
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T(""));
			EXPECT_EQ(m_RTData.GetOpen(), 12480);
			EXPECT_EQ(m_RTData.GetLastClose(), 12570);
			EXPECT_EQ(m_RTData.GetNew(), 12290);
			EXPECT_EQ(m_RTData.GetVolume(), 38594267);
			EXPECT_EQ(m_RTData.GetVBuy(0), 178647);
			EXPECT_EQ(m_RTData.GetPBuy(0), 12290);
			EXPECT_EQ(m_RTData.GetVBuy(1), 184600);
			EXPECT_EQ(m_RTData.GetPBuy(1), 12280);
			EXPECT_EQ(m_RTData.GetVBuy(2), 118700);
			EXPECT_EQ(m_RTData.GetPBuy(2), 12270);
			EXPECT_EQ(m_RTData.GetVBuy(3), 175500);
			EXPECT_EQ(m_RTData.GetPBuy(3), 12260);
			EXPECT_EQ(m_RTData.GetVBuy(4), 640700);
			EXPECT_EQ(m_RTData.GetPBuy(4), 12250);
			EXPECT_EQ(m_RTData.GetVSell(0), 51100);
			EXPECT_EQ(m_RTData.GetPSell(0), 12300);
			EXPECT_EQ(m_RTData.GetVSell(1), 106387);
			EXPECT_EQ(m_RTData.GetPSell(1), 12310);
			EXPECT_EQ(m_RTData.GetVSell(2), 162290);
			EXPECT_EQ(m_RTData.GetPSell(2), 12320);
			EXPECT_EQ(m_RTData.GetVSell(3), 237059);
			EXPECT_EQ(m_RTData.GetPSell(3), 12330);
			EXPECT_EQ(m_RTData.GetVSell(4), 609700);
			EXPECT_EQ(m_RTData.GetPSell(4), 12340);
			EXPECT_EQ(m_RTData.GetTransactionTime(), ttime2) << "每个数据中有两个时间，以较早的时间为准";
			break;
		case 2:
			EXPECT_TRUE(fSucceed);
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("")); // 没有设置，仍是初始值
			EXPECT_EQ(m_RTData.GetHigh(), -1); // 后续部分皆未设置。
			EXPECT_EQ(gl_systemMessage.GetErrorMessageDequeSize(), 3);
			break;
		case 3:
			EXPECT_TRUE(fSucceed) << "数据错误，跨过错误数据后继续，故而返回正确"; // 第一个数据错误
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600601.SS")); // 股票代码已设置
			EXPECT_EQ(m_RTData.GetHigh(), -1); // 此位置出错，没有设置，为测试开始时设置的值（-1）。
			fSucceed = m_RTData.ReadNeteaseData(m_pNeteaseWebRTData);
			EXPECT_TRUE(fSucceed); // 第二个数据没有错误
			EXPECT_EQ(m_lStringLength, m_pNeteaseWebRTData->GetCurrentPos() + 4); // 最后剩下四个字符" });"没有读
			EXPECT_STREQ(m_RTData.GetSymbol(), _T("600000.SS"));
			EXPECT_STREQ(m_RTData.GetStockName(), _T("")) << "目前不解析网易的股票名称";
			EXPECT_EQ(m_RTData.GetOpen(), 12480);
			EXPECT_EQ(m_RTData.GetLastClose(), 12570);
			EXPECT_EQ(m_RTData.GetNew(), 12290);
			EXPECT_EQ(m_RTData.GetVolume(), 38594267);
			EXPECT_EQ(m_RTData.GetVBuy(0), 178647);
			EXPECT_EQ(m_RTData.GetPBuy(0), 12290);
			EXPECT_EQ(m_RTData.GetVBuy(1), 184600);
			EXPECT_EQ(m_RTData.GetPBuy(1), 12280);
			EXPECT_EQ(m_RTData.GetVBuy(2), 118700);
			EXPECT_EQ(m_RTData.GetPBuy(2), 12270);
			EXPECT_EQ(m_RTData.GetVBuy(3), 175500);
			EXPECT_EQ(m_RTData.GetPBuy(3), 12260);
			EXPECT_EQ(m_RTData.GetVBuy(4), 640700);
			EXPECT_EQ(m_RTData.GetPBuy(4), 12250);
			EXPECT_EQ(m_RTData.GetVSell(0), 51100);
			EXPECT_EQ(m_RTData.GetPSell(0), 12300);
			EXPECT_EQ(m_RTData.GetVSell(1), 106387);
			EXPECT_EQ(m_RTData.GetPSell(1), 12310);
			EXPECT_EQ(m_RTData.GetVSell(2), 162290);
			EXPECT_EQ(m_RTData.GetPSell(2), 12320);
			EXPECT_EQ(m_RTData.GetVSell(3), 237059);
			EXPECT_EQ(m_RTData.GetPSell(3), 12330);
			EXPECT_EQ(m_RTData.GetVSell(4), 609700);
			EXPECT_EQ(m_RTData.GetPSell(4), 12340);
			EXPECT_EQ(m_RTData.GetTransactionTime(), ttime2) << "由于第一个数据有错误，故而没有更新时间。所以使用的是第二个数据的时间";
			EXPECT_EQ(gl_systemMessage.GetErrorMessageDequeSize(), 3);
			break;

		default:
			break;
		}
	}

	struct NeteaseRTDataStockCodePrefix {
		NeteaseRTDataStockCodePrefix(int count, CString Data) {
			m_iCount = count;
			m_strData = Data;
		}
	public:
		int m_iCount;
		CString m_strData;
	};

	// 无错误数据，上海市场股票
	NeteaseRTDataStockCodePrefix StockCodePrefixData101(0, _T("{\"0600000\":{ "));
	// 无错误，起始为','
	NeteaseRTDataStockCodePrefix StockCodePrefixData102(1, _T(",\"0600000\":{ "));
	//无错误，深圳市场股票
	NeteaseRTDataStockCodePrefix StockCodePrefixData103(2, _T("{\"1000001\":{ "));
	// 起始非'{'','
	NeteaseRTDataStockCodePrefix StockCodePrefixData104(3, _T("'\"0600601\":{ "));
	// 第二个字符非'\"'
	NeteaseRTDataStockCodePrefix StockCodePrefixData105(4, _T("{'0600601\":{ "));
	// 代码第一个字符非0或者1
	NeteaseRTDataStockCodePrefix StockCodePrefixData106(5, _T("{\"2600601\":{ "));
	// 新股票代码， 无错误
	NeteaseRTDataStockCodePrefix StockCodePrefixData107(6, _T("{\"1600601\":{ "));
	// 新股票代码，无错误
	NeteaseRTDataStockCodePrefix StockCodePrefixData108(7, _T("{\"0500000\":{ "));
	// 无错误，':'后多一个空格
	NeteaseRTDataStockCodePrefix StockCodePrefixData109(8, _T("{\"0600601\": { "));
	// 缺少后'"'
	NeteaseRTDataStockCodePrefix StockCodePrefixData110(9, _T("{\"0600601':{ "));
	// 缺少后'{'
	NeteaseRTDataStockCodePrefix StockCodePrefixData111(10, _T("{\"0600601\":}   "));
	// 缺少后'{'
	NeteaseRTDataStockCodePrefix StockCodePrefixData112(11, _T("{\"0600601\":}}   "));
	// 后'}'超过了五个字符
	NeteaseRTDataStockCodePrefix StockCodePrefixData113(12, _T("{\"0600601\":   { "));
	NeteaseRTDataStockCodePrefix StockCodePrefixData114(13, _T("{\"0600601\":{ "));
	NeteaseRTDataStockCodePrefix StockCodePrefixData115(14, _T("{\"0600601\":{ "));
	NeteaseRTDataStockCodePrefix StockCodePrefixData116(15, _T("{\"0600601\":{ "));
	// 所有的数量皆为零

	class StockCodePrefixTest : public::testing::TestWithParam<NeteaseRTDataStockCodePrefix*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();

			NeteaseRTDataStockCodePrefix* pData = GetParam();
			m_pNeteaseWebRTData = make_shared<CWebData>();
			m_iCount = pData->m_iCount;
			m_lStringLength = pData->m_strData.GetLength();
			for (int i = 0; i < m_lStringLength; i++) {
				m_pNeteaseWebRTData->SetData(i, pData->m_strData[i]);
			}
			m_pNeteaseWebRTData->SetData(m_lStringLength, 0x000);
			m_pNeteaseWebRTData->SetBufferLength(m_lStringLength);
			m_pNeteaseWebRTData->ResetCurrentPos();
			for (int i = 0; i < 5; i++) {
				m_RTData.SetPBuy(i, -1);
				m_RTData.SetPSell(i, -1);
				m_RTData.SetVBuy(i, -1);
				m_RTData.SetVSell(i, -1);
			}
			m_RTData.SetAmount(-1);
			m_RTData.SetVolume(-1);
			m_RTData.SetOpen(-1);
			m_RTData.SetNew(-1);
			m_RTData.SetLastClose(-1);
			m_RTData.SetHigh(-1);
			m_RTData.SetLow(-1);
			m_RTData.SetSell(-1);
			m_RTData.SetBuy(-1);
		}

		virtual void TearDown(void) override {
			// clearup
		}

	public:
		int m_iCount;
		char* m_pData;
		long m_lStringLength;
		CWebDataPtr m_pNeteaseWebRTData;
		CWebRTData m_RTData;
	};

	INSTANTIATE_TEST_SUITE_P(TestNeteaseRTData, StockCodePrefixTest,
		testing::Values(&StockCodePrefixData101, &StockCodePrefixData102
			, &StockCodePrefixData103, &StockCodePrefixData104, &StockCodePrefixData105,
			&StockCodePrefixData106, &StockCodePrefixData107, &StockCodePrefixData108,
			&StockCodePrefixData109, &StockCodePrefixData110, &StockCodePrefixData111,
			&StockCodePrefixData112, &StockCodePrefixData113/*, &StockCodePrefixData114,
			&StockCodePrefixData115, &StockCodePrefixData116*/
		));

	TEST_P(StockCodePrefixTest, TestStockCodePrefix) {
		bool fSucceed = m_RTData.ReadNeteaseStockCodePrefix(m_pNeteaseWebRTData);

		switch (m_iCount) {
		case 0:
			EXPECT_TRUE(fSucceed);
			break;
		case 1:
			EXPECT_TRUE(fSucceed);
			break;
		case 2:
			EXPECT_TRUE(fSucceed);
			break;
		case 3:
			EXPECT_FALSE(fSucceed);
			break;
		case 4:
			EXPECT_FALSE(fSucceed);
			break;
		case 5:
			EXPECT_FALSE(fSucceed);
			break;
		case 6:
			EXPECT_TRUE(fSucceed);
			break;
		case 7:
			EXPECT_TRUE(fSucceed);
			break;
		case 8: // 无错误
			EXPECT_TRUE(fSucceed);
			break;
		case 9:
			EXPECT_FALSE(fSucceed);
			break;
		case 10:
			EXPECT_FALSE(fSucceed);
			break;
		case 11:
			EXPECT_FALSE(fSucceed);
			break;
		case 12:
			EXPECT_FALSE(fSucceed);
			break;
		case 13:
			EXPECT_FALSE(fSucceed);
			break;
		case 14:
			EXPECT_FALSE(fSucceed);
			break;
		case 15:
			EXPECT_FALSE(fSucceed);
			break;
		default:
			break;
		}
	}

	struct ReadNeteaseOneValueData {
		ReadNeteaseOneValueData(int count, CString Data) {
			m_iCount = count;
			m_strData = Data;
		}
	public:
		int m_iCount;
		CString m_strData;
	};

	ReadNeteaseOneValueData neteasedata1(1, _T("\"name\": \"浦发银行\","));
	ReadNeteaseOneValueData neteasedata2(2, _T("\"turnover\": 12345}"));
	ReadNeteaseOneValueData neteasedata3(3, _T("\"high\": 4.04,"));
	ReadNeteaseOneValueData neteasedata4(4, _T("\"turnover\","));
	ReadNeteaseOneValueData neteasedata5(5, _T("\"turnover\"}"));
	ReadNeteaseOneValueData neteasedata6(6, _T("\"update\": \"2019/08/03 08:00:03\"}"));
	ReadNeteaseOneValueData neteasedata7(7, _T("\"time\": \"2019/08/03 08:00:03\","));

	class ReadNeteaseOneValueTest : public::testing::TestWithParam<ReadNeteaseOneValueData*> {
	protected:
		virtual void SetUp(void) override {
			ReadNeteaseOneValueData* pData = GetParam();
			m_pNeteaseWebRTData = make_shared<CWebData>();
			m_iCount = pData->m_iCount;
			long lLength = pData->m_strData.GetLength();
			for (int i = 0; i < lLength; i++) {
				m_pNeteaseWebRTData->SetData(i, pData->m_strData[i]);
			}
			m_pNeteaseWebRTData->SetData(lLength, 0x000);
			m_pNeteaseWebRTData->ResetCurrentPos();
		}

		virtual void TearDown(void) override {
			// clearup
			GeneralCheck();
		}

	public:
		int m_iCount;
		char* m_pData;
		CWebDataPtr m_pNeteaseWebRTData;
		CWebRTData m_RTData;
	};

	INSTANTIATE_TEST_SUITE_P(TestReadNeteaseOneValue, ReadNeteaseOneValueTest,
		testing::Values(&neteasedata1, &neteasedata2, &neteasedata3, &neteasedata4, &neteasedata5, &neteasedata6,
			&neteasedata7
		));

	TEST_P(ReadNeteaseOneValueTest, TestReadNeteaseOneCValue) {
		CString strValue;
		long lIndex = 0;
		bool fSucceed = m_RTData.GetNeteaseIndexAndValue(m_pNeteaseWebRTData, lIndex, strValue);
		switch (m_iCount) {
		case 1:
			EXPECT_TRUE(fSucceed);
			EXPECT_STREQ("浦发银行", strValue);
			break;
		case 2:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(lIndex, 63);
			EXPECT_STREQ(strValue, "12345");
			break;
		case 3:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(lIndex, 12);
			EXPECT_STREQ(strValue, _T("4.04"));
			break;
		case 4:
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(lIndex, 63);
			EXPECT_STREQ(strValue, _T(""));
			break;
		case 5:
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(lIndex, 63);
			EXPECT_STREQ(strValue, _T(""));
			break;
		case 6:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(lIndex, 7);
			EXPECT_STREQ(strValue, _T("2019/08/03 08:00:03"));
			break;
		case 7:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(lIndex, 1);
			EXPECT_STREQ(strValue, _T("2019/08/03 08:00:03"));
			break;
		default:
			break;
		}
	}

	struct NeteaseRTDataIndexValue {
		NeteaseRTDataIndexValue(int count, CString Data, CString strIndex, long lValue) {
			m_iCount = count;
			m_strData = Data;
			m_strIndex = strIndex;
			m_lValue = lValue;
		}
		int m_iCount;
		CString m_strData;
		CString m_strIndex;
		long m_lValue;
	};

	NeteaseRTDataIndexValue NeteaseData101(1, _T("\"code\": \"0600000\","), _T("code"), 0600000);

	class GetNeteaseIndexValueRTDataTest : public::testing::TestWithParam<NeteaseRTDataIndexValue*> {
	protected:
		virtual void SetUp(void) override {
			ASSERT_FALSE(gl_fNormalMode);
			NeteaseRTDataIndexValue* pData = GetParam();
			m_iCount = pData->m_iCount;
			m_lStringLength = pData->m_strData.GetLength();
			for (int i = 0; i < m_lStringLength; i++) {
				m_NeteaseWebRTData.SetData(i, pData->m_strData[i]);
			}
			m_lIndex = m_RTData.GetNeteaseSymbolIndex(pData->m_strIndex);
			for (int i = 0; i < 5; i++) {
				m_RTData.SetPBuy(i, -1);
				m_RTData.SetPSell(i, -1);
				m_RTData.SetVBuy(i, -1);
				m_RTData.SetVSell(i, -1);
			}
			m_RTData.SetAmount(-1);
			m_RTData.SetVolume(-1);
			m_RTData.SetOpen(-1);
			m_RTData.SetNew(-1);
			m_RTData.SetLastClose(-1);
			m_RTData.SetHigh(-1);
			m_RTData.SetLow(-1);
			m_RTData.SetSell(-1);
			m_RTData.SetBuy(-1);
		}

		virtual void TearDown(void) override {
			// clearup
		}

	public:
		int m_iCount;
		char* m_pData;
		long m_lStringLength;
		long m_lIndex;
		long m_lValue;
		CString m_strValue;
		CNeteaseRTWebInquiry m_NeteaseWebRTData;
		CWebRTData m_RTData;
	};

	INSTANTIATE_TEST_SUITE_P(TestNeteaseRTData, GetNeteaseIndexValueRTDataTest, testing::Values(&NeteaseData101
		/*, &Data2, &Data3,
		&Data4, &Data5, &Data6, &Data7, &Data8, &Data9, &Data10,
		&Data11, &Data12, &Data13, &Data14, &Data15, &Data16, &Data17, &Data18, &Data19, &Data20,
		&Data21, &Data22, &Data23, &Data24, &Data25, &Data26, &Data27, &Data28, &Data29, &Data30,
		&Data31, &Data32, &Data33, &Data34, &Data35, &Data36, &Data37, &Data38*/
	));

	TEST_P(GetNeteaseIndexValueRTDataTest, TestReadNeteaseOneValue) {
		CString strValue;
		switch (m_iCount) {
		case 1:
			break;

		default:
			break;
		}
	}
}