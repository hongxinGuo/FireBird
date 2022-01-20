#include"pch.h"

#include"globedef.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"

#include"SetOption.h"
#include"SetCurrentWeekLine.h"
#include"SetChinaChoicedStock.h"

#include"WebInquirer.h"

#include"MockSinaRTWebInquiry.h"
#include"MockTengxunRTWebInquiry.h"
#include"MockNeteaseRTWebInquiry.h"
#include"MockNeteaseDayLineWebInquiry.h"

#include"GeneralCheck.h"

using namespace testing;

using namespace std;
#include<memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	static CMockSinaRTWebInquiryPtr s_pMockSinaRTWebInquiry;
	static CMockTengxunRTWebInquiryPtr s_pMockTengxunRTWebInquiry;
	static CMockNeteaseRTWebInquiryPtr s_pMockNeteaseRTWebInquiry;

	class CChinaMarketTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();

			ASSERT_THAT(gl_pSinaRTWebInquiry, NotNull());
			s_pMockSinaRTWebInquiry = static_pointer_cast<CMockSinaRTWebInquiry>(gl_pSinaRTWebInquiry);
			ASSERT_THAT(gl_pTengxunRTWebInquiry, NotNull());
			s_pMockTengxunRTWebInquiry = static_pointer_cast<CMockTengxunRTWebInquiry>(gl_pTengxunRTWebInquiry);
			ASSERT_THAT(gl_pNeteaseRTWebInquiry, NotNull());
			s_pMockNeteaseRTWebInquiry = static_pointer_cast<CMockNeteaseRTWebInquiry>(gl_pNeteaseRTWebInquiry);

			gl_pChinaMarket->Load10DaysRSStrongStockDB(); // 装入各十日强度股票集
			EXPECT_TRUE(gl_pChinaMarket->GetChoicedStockSize(10) > 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChoicedStockSize(11) > 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChoicedStockSize(12) > 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChoicedStockSize(13) == 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChoicedStockSize(14) == 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChoicedStockSize(15) == 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChoicedStockSize(16) == 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChoicedStockSize(17) == 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChoicedStockSize(18) > 0);
			EXPECT_TRUE(gl_pChinaMarket->GetChoicedStockSize(19) > 0);
			EXPECT_THAT(gl_pChinaMarket->IsUpdateStockCodeDB(), IsFalse());

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				CChinaStockPtr pStock = gl_pChinaMarket->GetStock(i);
				EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << pStock->GetSymbol();
			}
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedProcessNumber(), 0);
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

			EXPECT_EQ(gl_pChinaMarket->GetTotalStock(), 5040) << "测试数据库中的股票代码总数为5040";
			EXPECT_EQ(gl_pChinaMarket->GetTotalLoadedStock(), 5040) << "测试代码库中的股票代码总数为5040";
		}

		static void TearDownTestSuite(void) {
			s_pMockSinaRTWebInquiry = nullptr;
			s_pMockTengxunRTWebInquiry = nullptr;
			s_pMockNeteaseRTWebInquiry = nullptr;

			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedProcessNumber(), 0);
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
			gl_pChinaMarket->SetCurrentStockChanged(false);
			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				CChinaStockPtr pStock = gl_pChinaMarket->GetStock(i);
				EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << pStock->GetSymbol();
			}
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_THAT(gl_pChinaMarket->IsUpdateStockCodeDB(), IsFalse());

			EXPECT_EQ(gl_pChinaMarket->GetTotalStock(), 5040) << "测试数据库中的股票代码总数为5040";

			GeneralCheck();
		}

		virtual void SetUp(void) override {
			ASSERT_FALSE(gl_fNormalMode);
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedProcessNumber(), 0);
			gl_pChinaMarket->SetCurrentStockChanged(false);
			gl_pChinaMarket->CalculateTime();
			gl_pChinaMarket->SetNeteaseRTDataInquiringIndex(0);
			gl_pChinaMarket->SetNeteaseDayLineDataInquiringIndex(0);
			gl_pChinaMarket->SetSinaStockRTDataInquiringIndex(0);
			gl_pChinaMarket->SetTengxunRTDataInquiringIndex(0);
			gl_pChinaMarket->SetSystemReady(true); // 测试市场时，默认系统已经准备好
			gl_pChinaMarket->SetPermitResetMarket(true);
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
			EXPECT_THAT(gl_pChinaMarket->IsUpdateStockCodeDB(), IsFalse());
			EXPECT_EQ(gl_pChinaMarket->GetTotalStock(), 5040) << "测试数据库中的股票代码总数为5040";

			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
			EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
			gl_ThreadStatus.SetSavingTempData(false);
			gl_pChinaMarket->SetRTDataSetCleared(false);
			gl_pChinaMarket->SetUpdateOptionDB(false);
			gl_pChinaMarket->ClearChoicedRTDataQueue();
			gl_pChinaMarket->SetResetMarket(true);
			gl_pChinaMarket->SetNeteaseRTDataInquiringIndex(0);
			gl_pChinaMarket->SetNeteaseDayLineDataInquiringIndex(0);
			gl_pChinaMarket->SetSinaStockRTDataInquiringIndex(0);
			gl_pChinaMarket->SetTengxunRTDataInquiringIndex(0);
			while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
			gl_pChinaMarket->SetPermitResetMarket(true);
			gl_pChinaMarket->SetUpdateOptionDB(false);
			gl_pChinaMarket->SetSystemReady(true); // 离开此测试时，默认系统已准备好。
			EXPECT_TRUE(gl_pChinaMarket->IsCheckActiveStock());
			EXPECT_THAT(gl_pChinaMarket->IsUpdateStockCodeDB(), IsFalse());

			gl_pChinaMarket->ResetCurrentStock();
			gl_pChinaMarket->SetCurrentStockChanged(false);
			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				CChinaStockPtr pStock = gl_pChinaMarket->GetStock(i);
				if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
				if (pStock->IsDayLineNeedSaving()) pStock->SetDayLineNeedSaving(false);
			}
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedProcessNumber(), 0);
			EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
			EXPECT_THAT(gl_pChinaMarket->GetTotalStock(), Eq(5040));
			gl_pChinaMarket->SetSinaStockRTDataInquiringIndexFromTotalStockSet(0);

			EXPECT_EQ(gl_pChinaMarket->GetTotalStock(), 5040) << "测试数据库中的股票代码总数为5040";

			gl_pChinaMarket->SetCurrentSelectedStockSet(-1);

			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();

			GeneralCheck();
		}
	};

	TEST_F(CChinaMarketTest, TestInitialize) {
		CChinaStockPtr pStock = nullptr;
		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			pStock = gl_pChinaMarket->GetStock(i);
			EXPECT_EQ(pStock->GetOffset(), i);
			EXPECT_FALSE(pStock->IsDayLineNeedSaving());
			if (IsShanghaiExchange(pStock->GetSymbol())) {
				if ((pStock->GetSymbol().Left(6) >= _T("000000")) && (pStock->GetSymbol().Left(6) <= _T("000999"))) {
					EXPECT_FALSE(pStock->IsNeedProcessRTData());
				}
			}
			else if ((pStock->GetSymbol().Left(6) >= _T("399000")) && (pStock->GetSymbol().Left(6) <= _T("399999"))) {
				EXPECT_FALSE(pStock->IsNeedProcessRTData());
			}
			else {
				EXPECT_TRUE(pStock->IsNeedProcessRTData());
			}
		}
		EXPECT_FALSE(gl_pChinaMarket->IsLoadSelectedStock());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
		EXPECT_EQ(gl_pChinaMarket->GetTotalAttackBuyAmount(), 0);
		EXPECT_EQ(gl_pChinaMarket->GetTotalAttackSellAmount(), 0);
		EXPECT_TRUE(gl_pChinaMarket->IsGetRTData());
		EXPECT_FALSE(gl_pChinaMarket->IsTodayTempRTDataLoaded());
		EXPECT_EQ(gl_pChinaMarket->GetCountDownSlowReadingRTData(), 3);
		EXPECT_GT(gl_pChinaMarket->GetTotalStock(), 0);   // 在全局变量gl_ChinaStockMarket初始化时就生成了全部股票代码池
		EXPECT_EQ(gl_pChinaMarket->GetTotalStockMapSize(), gl_pChinaMarket->GetTotalStock());
		EXPECT_EQ(gl_pChinaMarket->GetTotalStock(), 5040) << "测试数据库中的股票代码总数为5040";
		pStock = gl_pChinaMarket->GetStock(0);
		EXPECT_STREQ(pStock->GetSymbol(), _T("000001.SS"));
		EXPECT_EQ(gl_pChinaMarket->GetStockOffset(_T("000001.SS")), 0);

		EXPECT_TRUE(gl_pChinaMarket->IsPermitResetMarket());
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedProcessNumber(), 0) << gl_pChinaMarket->GetDayLineNeedProcessNumber();
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedSaveNumber(), 0);
		EXPECT_TRUE(gl_pChinaMarket->IsUsingSinaRTDataReceiver());
		EXPECT_TRUE(gl_pChinaMarket->IsUsingTengxunRTDataReceiver());
		EXPECT_TRUE(gl_pChinaMarket->IsUsingNeteaseRTDataReceiver());

		EXPECT_EQ(gl_pChinaMarket->GetNeteaseDayLineDataInquiringIndex(), 0);
		EXPECT_EQ(gl_pChinaMarket->GetNeteaseRTDataInquiringIndex(), 0);
		EXPECT_EQ(gl_pChinaMarket->GetSinaStockRTDataInquiringIndex(), 0);
		EXPECT_EQ(gl_pChinaMarket->GetTengxunRTDataInquiringIndex(), 0);
	}

	TEST_F(CChinaMarketTest, TestClearUpdateStockCodeDBFlag) {
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineDBUpdated());
		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			CChinaStockPtr pStock = gl_pChinaMarket->GetStock(i);
			pStock->SetDayLineDBUpdated(true);
		}
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineDBUpdated());
		gl_pChinaMarket->ClearDayLineDBUpdatedFlag();
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineDBUpdated());
	}

	TEST_F(CChinaMarketTest, TestGetTengxunInquiringStockStr) {
		CChinaStockPtr pStock = nullptr;
		gl_pChinaMarket->SetSystemReady(true);
		CString str = gl_pChinaMarket->GetTengxunInquiringStockStr(900, gl_pChinaMarket->GetTotalStock());
		str = gl_pChinaMarket->GetTengxunInquiringStockStr(900, gl_pChinaMarket->GetTotalStock());
		CString strCompare, str2;
		pStock = gl_pChinaMarket->GetStock(899);
		strCompare = pStock->GetSymbol();
		str2 = XferTengxunToStandred(str.Left(8));
		EXPECT_EQ(str2.Compare(strCompare), 0);
		str = gl_pChinaMarket->GetTengxunInquiringStockStr(900, gl_pChinaMarket->GetTotalStock());
		pStock = gl_pChinaMarket->GetStock(1798);
		strCompare = pStock->GetSymbol();
		str2 = XferTengxunToStandred(str.Left(8));
		EXPECT_EQ(str2.Compare(strCompare), 0);
		str = gl_pChinaMarket->GetTengxunInquiringStockStr(900, gl_pChinaMarket->GetTotalStock());
		pStock = gl_pChinaMarket->GetStock(2697);
		strCompare = pStock->GetSymbol();
		str2 = XferTengxunToStandred(str.Left(8));
		EXPECT_EQ(str2.Compare(strCompare), 0);
		str = gl_pChinaMarket->GetTengxunInquiringStockStr(900, gl_pChinaMarket->GetTotalStock());
		pStock = gl_pChinaMarket->GetStock(3596);
		strCompare = pStock->GetSymbol();
		str2 = XferTengxunToStandred(str.Left(8));
		EXPECT_EQ(str2.Compare(strCompare), 0);
		str = gl_pChinaMarket->GetTengxunInquiringStockStr(900, gl_pChinaMarket->GetTotalStock());
		pStock = gl_pChinaMarket->GetStock(4495);
		strCompare = pStock->GetSymbol();
		str2 = XferTengxunToStandred(str.Left(8));
		EXPECT_EQ(str2.Compare(strCompare), 0);
		str = gl_pChinaMarket->GetTengxunInquiringStockStr(900, gl_pChinaMarket->GetTotalStock());
		pStock = gl_pChinaMarket->GetStock(0);
		strCompare = pStock->GetSymbol();
		str2 = XferTengxunToStandred(str.Left(8));
		EXPECT_EQ(str2.Compare(strCompare), 1);
	}

	TEST_F(CChinaMarketTest, TestGetInquiringStockStr) {
		CString str2;

		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->SetSinaStockRTDataInquiringIndex(0);
		CString str = gl_pChinaMarket->GetSinaStockInquiringStr(900, false);
		str2 = str.Left(2);
		EXPECT_STREQ(str2, _T("sh"));
		str = gl_pChinaMarket->GetSinaStockInquiringStr(900, false);
		str = gl_pChinaMarket->GetSinaStockInquiringStr(900, false);
		str = gl_pChinaMarket->GetSinaStockInquiringStr(900, false);
		str = gl_pChinaMarket->GetSinaStockInquiringStr(900, false);
		str = gl_pChinaMarket->GetSinaStockInquiringStr(900, false);
		EXPECT_TRUE(gl_pChinaMarket->GetSinaStockRTDataInquiringIndex() < 900);// 目前不到五千个活跃股票，故而六次即可遍历一次
	}

	TEST_F(CChinaMarketTest, TestGetNeteaseInquiringStockStr) {
		CChinaStockPtr pStock = nullptr;
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->SetNeteaseRTDataInquiringIndex(0);
		CString str = gl_pChinaMarket->GetNextNeteaseStockInquiringStr(900, gl_pChinaMarket->GetTotalStock());
		EXPECT_EQ(gl_pChinaMarket->GetNeteaseRTDataInquiringIndex(), 899);
		str = gl_pChinaMarket->GetNextNeteaseStockInquiringStr(900, gl_pChinaMarket->GetTotalStock());
		CString strCompare, str2;
		pStock = gl_pChinaMarket->GetStock(899);
		strCompare = pStock->GetSymbol();
		str2 = XferNeteaseToStandred(str.Left(7));
		EXPECT_STREQ(str2, strCompare);
		str = gl_pChinaMarket->GetNextNeteaseStockInquiringStr(900, gl_pChinaMarket->GetTotalStock());
		pStock = gl_pChinaMarket->GetStock(1798);
		strCompare = pStock->GetSymbol();
		str2 = XferNeteaseToStandred(str.Left(7));
		EXPECT_STREQ(str2, strCompare);
		str = gl_pChinaMarket->GetNextNeteaseStockInquiringStr(900, gl_pChinaMarket->GetTotalStock());
		pStock = gl_pChinaMarket->GetStock(2697);
		strCompare = pStock->GetSymbol();
		str2 = XferNeteaseToStandred(str.Left(7));
		EXPECT_STREQ(str2, strCompare);
		str = gl_pChinaMarket->GetNextNeteaseStockInquiringStr(900, gl_pChinaMarket->GetTotalStock());
		pStock = gl_pChinaMarket->GetStock(3596);
		strCompare = pStock->GetSymbol();
		str2 = XferNeteaseToStandred(str.Left(7));
		EXPECT_STREQ(str2, strCompare);
		str = gl_pChinaMarket->GetNextNeteaseStockInquiringStr(900, gl_pChinaMarket->GetTotalStock());
		pStock = gl_pChinaMarket->GetStock(4495);
		strCompare = pStock->GetSymbol();
		str2 = XferNeteaseToStandred(str.Left(7));
		EXPECT_STREQ(str2, strCompare);
		str = gl_pChinaMarket->GetNextNeteaseStockInquiringStr(900, gl_pChinaMarket->GetTotalStock());
		pStock = gl_pChinaMarket->GetStock(0);
		strCompare = pStock->GetSymbol();
		str2 = XferNeteaseToStandred(str.Left(7));
		EXPECT_STREQ(str2, strCompare);
		gl_pChinaMarket->SetNeteaseRTDataInquiringIndex(0);
		gl_pChinaMarket->SetSinaStockRTDataInquiringIndex(0);
	}

	TEST_F(CChinaMarketTest, TestGetNeteaseDayLineInquiringStr) {
		CString str;
		bool fStatus = false;
		CChinaStockPtr pStock = gl_pChinaMarket->GetStock(0);
		EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << _T("测试时使用teststock数据库，此数据库比较旧，最后更新时间不是昨日，故而活跃股票也需要更新日线");
		long lDate;

		EXPECT_EQ(gl_pChinaMarket->GetNeteaseDayLineDataInquiringIndex(), 0);
		pStock->SetDayLineNeedUpdate(false);
		pStock = gl_pChinaMarket->GetStock(1);
		EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
		EXPECT_LT(pStock->GetDayLineEndDate(), gl_pChinaMarket->GetLastTradeDate()) << pStock->GetDayLineEndDate();
		pStock = gl_pChinaMarket->GetStock(2);
		EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
		EXPECT_LT(pStock->GetDayLineEndDate(), gl_pChinaMarket->GetLastTradeDate());
		lDate = pStock->GetDayLineEndDate();
		pStock->SetDayLineEndDate(gl_pChinaMarket->GetMarketDate());
		fStatus = gl_pChinaMarket->CreateNeteaseDayLineInquiringStr(str, gl_pChinaMarket->GetTotalStock());
		EXPECT_TRUE(fStatus);
		EXPECT_STREQ(str, _T("1000001"));
		pStock = gl_pChinaMarket->GetStock(1);
		EXPECT_FALSE(pStock->IsDayLineNeedUpdate());
		fStatus = gl_pChinaMarket->CreateNeteaseDayLineInquiringStr(str, gl_pChinaMarket->GetTotalStock());
		EXPECT_TRUE(fStatus);
		EXPECT_STREQ(str, _T("1000002"));

		gl_pChinaMarket->GetStock(2)->SetDayLineEndDate(lDate); // 恢复原状。
	}

	TEST_F(CChinaMarketTest, TestGetMinLineOffset) {
		tm tmMarketTime;
		tmMarketTime.tm_year = 2020 - 1900;
		tmMarketTime.tm_mon = 1;
		tmMarketTime.tm_mday = 1;
		tmMarketTime.tm_hour = 9;
		tmMarketTime.tm_min = 0;
		tmMarketTime.tm_sec = 0;
		time_t tUTC = gl_pChinaMarket->TransferToUTCTime(&tmMarketTime);
		long lOffset = gl_pChinaMarket->GetMinLineOffset(tUTC);
		EXPECT_EQ(lOffset, 0);
		tmMarketTime.tm_hour = 10;
		tmMarketTime.tm_min = 30;
		tmMarketTime.tm_sec = 59;
		tUTC = gl_pChinaMarket->TransferToUTCTime(&tmMarketTime);
		lOffset = gl_pChinaMarket->GetMinLineOffset(tUTC);
		EXPECT_EQ(lOffset, 60);
		tmMarketTime.tm_hour = 12;
		tmMarketTime.tm_min = 30;
		tmMarketTime.tm_sec = 59;
		tUTC = gl_pChinaMarket->TransferToUTCTime(&tmMarketTime);
		lOffset = gl_pChinaMarket->GetMinLineOffset(tUTC);
		EXPECT_EQ(lOffset, 119);
		tmMarketTime.tm_hour = 14;
		tmMarketTime.tm_min = 30;
		tmMarketTime.tm_sec = 59;
		tUTC = gl_pChinaMarket->TransferToUTCTime(&tmMarketTime);
		lOffset = gl_pChinaMarket->GetMinLineOffset(tUTC);
		EXPECT_EQ(lOffset, 210);
		tmMarketTime.tm_hour = 15;
		tmMarketTime.tm_min = 30;
		tmMarketTime.tm_sec = 59;
		tUTC = gl_pChinaMarket->TransferToUTCTime(&tmMarketTime);
		lOffset = gl_pChinaMarket->GetMinLineOffset(tUTC);
		EXPECT_EQ(lOffset, 239);
	}

	TEST_F(CChinaMarketTest, TestIsAStock) {
		CChinaStockPtr pstock = make_shared<CChinaStock>();
		pstock->SetSymbol(_T("600000.SS"));
		EXPECT_TRUE(gl_pChinaMarket->IsAStock(pstock));
		pstock->SetSymbol(_T("600000.SA"));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(pstock));
		pstock->SetSymbol(_T("000001.SZ"));
		EXPECT_TRUE(gl_pChinaMarket->IsAStock(pstock));
		pstock->SetSymbol(_T("10001.SZ"));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(pstock));
	}

	TEST_F(CChinaMarketTest, TestIsAStock2) {
		EXPECT_TRUE(gl_pChinaMarket->IsAStock(_T("600000.SS")));
		EXPECT_TRUE(gl_pChinaMarket->IsAStock(_T("601198.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("602102.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("603604.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("604604.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("605604.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("606604.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("607604.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("608604.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("609604.SS")));

		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("600000.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("000001.SA")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("000001.AZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("200001.SZ")));
		EXPECT_TRUE(gl_pChinaMarket->IsAStock(_T("000001.SZ")));
		EXPECT_TRUE(gl_pChinaMarket->IsAStock(_T("002389.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("003389.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("004389.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("005389.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("006389.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("007389.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("008389.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("009389.SZ")));
		EXPECT_FALSE(gl_pChinaMarket->IsAStock(_T("001389.SZ")));
	}

	TEST_F(CChinaMarketTest, TestIsStock) {
		EXPECT_GT(gl_pChinaMarket->GetTotalStock(), 1);
		EXPECT_TRUE(gl_pChinaMarket->IsStock(_T("600000.SS")));
		EXPECT_FALSE(gl_pChinaMarket->IsStock(_T("60000.SS")));
	}

	TEST_F(CChinaMarketTest, TestGetStockName) {
		//未实现.由于stockName存储时使用的是UniCode制式，而本系统默认是Ansi制式，导致无法进行字符串对比。暂时不进行测试了。
		//EXPECT_STREQ(gl_pChinaMarket->GetStockName(_T("600000.SS")), _T("浦发银行"));
		EXPECT_STREQ(gl_pChinaMarket->GetStockName(_T("60000.SS")), _T("")); // 没找到返回空字符串
	}

	TEST_F(CChinaMarketTest, TestGetStockCode) {
		EXPECT_EQ(gl_pChinaMarket->GetStock(_T("66000.SZ")), nullptr);
		EXPECT_FALSE(gl_pChinaMarket->GetStock(_T("600001.SS")) == nullptr);

		EXPECT_EQ(gl_pChinaMarket->GetStock(-1), nullptr);
		EXPECT_EQ(gl_pChinaMarket->GetStock(gl_pChinaMarket->GetTotalStock()), nullptr);
		EXPECT_FALSE(gl_pChinaMarket->GetStock(0) == nullptr);
		EXPECT_FALSE(gl_pChinaMarket->GetStock(gl_pChinaMarket->GetTotalStock() - 1) == nullptr);
	}

	TEST_F(CChinaMarketTest, TestGetCurrentStock) {
		CChinaStockPtr pStock = gl_pChinaMarket->GetStock(7);
		CChinaStockPtr pStock2 = gl_pChinaMarket->GetStock(4);

		gl_pChinaMarket->SetCurrentStock(pStock);
		EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), pStock);
		EXPECT_TRUE(pStock->IsRecordRTData());
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentStockChanged());
		gl_pChinaMarket->SetCurrentStockChanged(false);
		gl_pChinaMarket->SetCurrentStock(pStock);
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
		EXPECT_FALSE(pStock2->IsRecordRTData());
		gl_pChinaMarket->SetCurrentStock(pStock2);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentStockChanged());
		EXPECT_TRUE(pStock2->IsRecordRTData());
		gl_pChinaMarket->SetCurrentStock(_T("600000.SS"));
		pStock = gl_pChinaMarket->GetCurrentStock();
		EXPECT_TRUE(pStock->IsRecordRTData());
		EXPECT_STREQ(pStock->GetSymbol(), _T("600000.SS"));
		gl_pChinaMarket->ResetCurrentStock();
		EXPECT_FALSE(pStock->IsRecordRTData());
		EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr);

		gl_pChinaMarket->SetCurrentStockChanged(false);
	}

	TEST_F(CChinaMarketTest, TestUnloadDayLine) {
		EXPECT_TRUE(gl_pChinaMarket->UnloadDayLine());
	}

	TEST_F(CChinaMarketTest, TestMarketReady) {
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady()) << "中国股票市场默认为假, 但测试系统默认为真";
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetSystemReady(false);
		EXPECT_FALSE(gl_pChinaMarket->IsSystemReady());

		gl_pChinaMarket->SetSystemReady(true); // 恢复原态
	}

	TEST_F(CChinaMarketTest, TestIsTodayStockProcessed) {
		gl_pChinaMarket->SetTodayStockProcessed(true);
		EXPECT_TRUE(gl_pChinaMarket->IsTodayStockProcessed());
		EXPECT_FALSE(gl_pChinaMarket->IsProcessingTodayStock());
		EXPECT_FALSE(gl_pChinaMarket->IsTodayStockNotProcessed());
		gl_pChinaMarket->SetTodayStockProcessed(false);
		EXPECT_TRUE(gl_pChinaMarket->IsTodayStockNotProcessed());
		EXPECT_FALSE(gl_pChinaMarket->IsTodayStockProcessed());
		EXPECT_FALSE(gl_pChinaMarket->IsProcessingTodayStock());
		gl_pChinaMarket->SetProcessingTodayStock();
		EXPECT_TRUE(gl_pChinaMarket->IsProcessingTodayStock());
	}

	TEST_F(CChinaMarketTest, TestGetCurrentSelectedPosition) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedPosition(), 0);
		gl_pChinaMarket->SetCurrentSelectedPosition(10101010);
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedPosition(), 10101010);

		gl_pChinaMarket->SetCurrentSelectedPosition(0);
	}

	TEST_F(CChinaMarketTest, TestGetCurrentSelectedStockSet) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
		gl_pChinaMarket->SetCurrentSelectedStockSet(10);
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 10);

		gl_pChinaMarket->SetCurrentSelectedStockSet(-1);
	}

	TEST_F(CChinaMarketTest, TestIsChoiced10RSStrongStockSet) {
		gl_pChinaMarket->SetChoiced10RSStrongStockSet(true);
		EXPECT_TRUE(gl_pChinaMarket->IsChoiced10RSStrongStockSet());
		gl_pChinaMarket->SetChoiced10RSStrongStockSet(false);
		EXPECT_FALSE(gl_pChinaMarket->IsChoiced10RSStrongStockSet());
	}

	TEST_F(CChinaMarketTest, TestIsChoiced10RSStrong1StockSet) {
		gl_pChinaMarket->SetChoiced10RSStrong1StockSet(true);
		EXPECT_TRUE(gl_pChinaMarket->IsChoiced10RSStrong1StockSet());
		gl_pChinaMarket->SetChoiced10RSStrong1StockSet(false);
		EXPECT_FALSE(gl_pChinaMarket->IsChoiced10RSStrong1StockSet());
	}
	TEST_F(CChinaMarketTest, TestIsChoiced10RSStrong2StockSet) {
		gl_pChinaMarket->SetChoiced10RSStrong2StockSet(true);
		EXPECT_TRUE(gl_pChinaMarket->IsChoiced10RSStrong2StockSet());
		gl_pChinaMarket->SetChoiced10RSStrong2StockSet(false);
		EXPECT_FALSE(gl_pChinaMarket->IsChoiced10RSStrong2StockSet());
	}

	TEST_F(CChinaMarketTest, TestIsDayLineNeedSaving) {
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineNeedSaving());
		CChinaStockPtr pStock = gl_pChinaMarket->GetStock(0);
		pStock->SetDayLineNeedSaving(true);
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineNeedSaving());
		pStock->SetDayLineNeedSaving(false);
	}

	TEST_F(CChinaMarketTest, TestIsDayLineNeedUpdate) {
		CChinaStockPtr pStock = nullptr;
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineNeedUpdate());
		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			pStock = gl_pChinaMarket->GetStock(i);
			if (pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(false);
		}
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineNeedUpdate());
		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			pStock = gl_pChinaMarket->GetStock(i);
			if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CChinaMarketTest, TestIsDayLineNeedProcess) {
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineNeedProcess()) << "默认状态下无需处理";
		CWebDataPtr pData = make_shared<CWebData>();
		gl_WebInquirer.PushNeteaseDayLineData(pData);

		EXPECT_TRUE(gl_pChinaMarket->IsDayLineNeedProcess());

		gl_WebInquirer.PopNeteaseDayLineData();
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineNeedProcess());
	}

	TEST_F(CChinaMarketTest, TestTaskProcessDayLineGetFromNeeteaseServer) {
		CWebDataPtr pData = make_shared<CWebData>();
		CChinaStockPtr pStock = gl_pChinaMarket->GetStock(_T("600666.SS"));
		CString strTest = _T("");

		pData->SetStockCode(_T("600666.SS"));
		pData->__TEST_SetBuffer__(strTest);
		gl_WebInquirer.PushNeteaseDayLineData(pData);

		EXPECT_TRUE(gl_pChinaMarket->TaskProcessDayLineGetFromNeeteaseServer());
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedProcessNumber(), 0);
		EXPECT_EQ(gl_WebInquirer.GetNeteaseDayLineDataSize(), 0);
	}

	TEST_F(CChinaMarketTest, TestIsLoadSelectedStock) {
		EXPECT_FALSE(gl_pChinaMarket->IsLoadSelectedStock());
		gl_pChinaMarket->SetLoadSelectedStock(true);
		EXPECT_TRUE(gl_pChinaMarket->IsLoadSelectedStock());
		gl_pChinaMarket->SetLoadSelectedStock(false);
		EXPECT_FALSE(gl_pChinaMarket->IsLoadSelectedStock());
	}

	TEST_F(CChinaMarketTest, TestIsRecordingRTData) {
		EXPECT_FALSE(gl_pChinaMarket->IsRecordingRTData());
		gl_pChinaMarket->SetRecordRTData(true);
		EXPECT_TRUE(gl_pChinaMarket->IsRecordingRTData());
		gl_pChinaMarket->SetRecordRTData(false);
		EXPECT_FALSE(gl_pChinaMarket->IsRecordingRTData());
	}

	TEST_F(CChinaMarketTest, TestSetUpdateOptionDB) {
		EXPECT_FALSE(gl_pChinaMarket->IsUpdateOptionDB());
		gl_pChinaMarket->SetUpdateOptionDB(true);
		EXPECT_TRUE(gl_pChinaMarket->IsUpdateOptionDB());
		gl_pChinaMarket->SetUpdateOptionDB(false);
		EXPECT_FALSE(gl_pChinaMarket->IsUpdateOptionDB());
	}

	TEST_F(CChinaMarketTest, TestSetUpdateChoicedStockDB) {
		EXPECT_FALSE(gl_pChinaMarket->IsUpdateChoicedStockDB());
		gl_pChinaMarket->SetUpdateChoicedStockDB(true);
		EXPECT_TRUE(gl_pChinaMarket->IsUpdateChoicedStockDB());
		gl_pChinaMarket->SetUpdateChoicedStockDB(false);
		EXPECT_FALSE(gl_pChinaMarket->IsUpdateChoicedStockDB());
	}

	TEST_F(CChinaMarketTest, TestIsSaveDayLine) {
		EXPECT_FALSE(gl_pChinaMarket->IsSaveDayLine());
		gl_pChinaMarket->SetSaveDayLine(true);
		EXPECT_TRUE(gl_pChinaMarket->IsSaveDayLine());
		gl_pChinaMarket->SetSaveDayLine(false);
		EXPECT_FALSE(gl_pChinaMarket->IsSaveDayLine());
	}

	TEST_F(CChinaMarketTest, TestIsUpdateStockSection) {
		EXPECT_FALSE(gl_pChinaMarket->IsUpdateStockSection());
		gl_pChinaMarket->SetUpdateStockSection(true);
		EXPECT_TRUE(gl_pChinaMarket->IsUpdateStockSection());
		gl_pChinaMarket->SetUpdateStockSection(false);
		EXPECT_FALSE(gl_pChinaMarket->IsUpdateStockSection());
	}

	TEST_F(CChinaMarketTest, TestIsStockSectionActive) {
		EXPECT_TRUE(gl_pChinaMarket->IsStockSectionActive(0));
		gl_pChinaMarket->SetStockSectionActiveFlag(0, true);
		EXPECT_TRUE(gl_pChinaMarket->IsStockSectionActive(0));
		gl_pChinaMarket->SetStockSectionActiveFlag(0, false);
		EXPECT_FALSE(gl_pChinaMarket->IsStockSectionActive(0));
	}

	TEST_F(CChinaMarketTest, TestIsRTDataSetCleared) {
		EXPECT_FALSE(gl_pChinaMarket->IsRTDataSetCleared());
		gl_pChinaMarket->SetRTDataSetCleared(true);
		EXPECT_TRUE(gl_pChinaMarket->IsRTDataSetCleared());
		gl_pChinaMarket->SetRTDataSetCleared(false);
		EXPECT_FALSE(gl_pChinaMarket->IsRTDataSetCleared());
	}

	TEST_F(CChinaMarketTest, TestIsSavingTempData) {
		EXPECT_TRUE(gl_pChinaMarket->IsSavingTempData());
		gl_pChinaMarket->SetSavingTempData(false);
		EXPECT_FALSE(gl_pChinaMarket->IsSavingTempData());
		gl_pChinaMarket->SetSavingTempData(true);
		EXPECT_TRUE(gl_pChinaMarket->IsSavingTempData());
	}

	TEST_F(CChinaMarketTest, TestIsUpdateStockCodeDB) {
		EXPECT_THAT(gl_pChinaMarket->IsUpdateStockCodeDB(), IsFalse());
		gl_pChinaMarket->GetStock(1)->SetUpdateProfileDB(true);
		EXPECT_THAT(gl_pChinaMarket->IsUpdateStockCodeDB(), IsTrue());

		gl_pChinaMarket->GetStock(1)->SetUpdateProfileDB(false);
	}

	TEST_F(CChinaMarketTest, TestTaskResetMarket1) {
		tm tm_;
		tm_.tm_wday = 1; // 星期一
		gl_pChinaMarket->__TEST_SetMarketTM(tm_);
		EXPECT_TRUE(gl_pChinaMarket->IsPermitResetMarket());
		EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->SetResetMarket(false);
		gl_pChinaMarket->TaskResetMarket(91259);
		EXPECT_FALSE(gl_pChinaMarket->IsResetMarket());
		gl_pChinaMarket->TaskResetMarket(91400);
		EXPECT_FALSE(gl_pChinaMarket->IsResetMarket()) << _T("第一次重启市场，其结束时间必须在9:14之前，这样才能保证只运行了一次（此函数必须每分钟调度一次");
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->TaskResetMarket(91300);
		if (gl_pChinaMarket->TooManyStockDayLineNeedUpdate()) {
			EXPECT_FALSE(gl_pChinaMarket->IsResetMarket());
			EXPECT_TRUE(gl_pChinaMarket->IsPermitResetMarket());
			EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		}
		else {
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			EXPECT_TRUE(gl_pChinaMarket->IsPermitResetMarket());
			EXPECT_FALSE(gl_pChinaMarket->IsSystemReady());
		}
	}

	TEST_F(CChinaMarketTest, TestTaskResetMarket2) {
		tm tm_;
		tm_.tm_wday = 0; // 星期日, 休息日
		gl_pChinaMarket->__TEST_SetMarketTM(tm_);
		EXPECT_TRUE(gl_pChinaMarket->IsPermitResetMarket());
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->SetResetMarket(false);
		gl_pChinaMarket->TaskResetMarket(91259);
		EXPECT_FALSE(gl_pChinaMarket->IsResetMarket());
		gl_pChinaMarket->TaskResetMarket(91401);
		EXPECT_FALSE(gl_pChinaMarket->IsResetMarket());
		gl_pChinaMarket->TaskResetMarket(91300);
		EXPECT_FALSE(gl_pChinaMarket->IsResetMarket());
		EXPECT_TRUE(gl_pChinaMarket->IsPermitResetMarket());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
	}

	TEST_F(CChinaMarketTest, TestTaskResetMarket3) {
		tm tm_;
		tm_.tm_wday = 1; // 星期一
		gl_pChinaMarket->__TEST_SetMarketTM(tm_);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(gl_pChinaMarket->TooManyStockDayLineNeedUpdate());
		EXPECT_TRUE(gl_pChinaMarket->IsPermitResetMarket());
		EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
		gl_pChinaMarket->SetResetMarket(false);
		gl_pChinaMarket->TaskResetMarket(91259);
		EXPECT_FALSE(gl_pChinaMarket->IsResetMarket());
		gl_pChinaMarket->TaskResetMarket(91400);
		EXPECT_FALSE(gl_pChinaMarket->IsResetMarket()) << _T("第一次重启市场，其结束时间必须在9:14之前，这样才能保证只运行了一次（此函数必须每分钟调度一次");
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->TaskResetMarket(91300);
		EXPECT_FALSE(gl_pChinaMarket->IsResetMarket());
		EXPECT_TRUE(gl_pChinaMarket->IsPermitResetMarket());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());

		// 恢复系统原态
		gl_pChinaMarket->SetStockDayLineNeedUpdate(gl_pChinaMarket->GetTotalStock());
	}

	TEST_F(CChinaMarketTest, TestTaskResetMarket4) {
		tm tm_;
		tm_.tm_wday = 1; // 星期一
		gl_pChinaMarket->__TEST_SetMarketTM(tm_);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(gl_pChinaMarket->TooManyStockDayLineNeedUpdate());
		EXPECT_TRUE(gl_pChinaMarket->IsPermitResetMarket());
		EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());

		gl_pChinaMarket->SetStockDayLineNeedUpdate(999);
		gl_pChinaMarket->SetResetMarket(false);
		gl_pChinaMarket->TaskResetMarket(91259);
		EXPECT_FALSE(gl_pChinaMarket->IsResetMarket());
		gl_pChinaMarket->TaskResetMarket(91400);
		EXPECT_FALSE(gl_pChinaMarket->IsResetMarket()) << _T("第一次重启市场，其结束时间必须在9:14之前，这样才能保证只运行了一次（此函数必须每分钟调度一次");
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->TaskResetMarket(91300);
		EXPECT_TRUE(gl_pChinaMarket->IsPermitResetMarket());
		EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
		EXPECT_FALSE(gl_pChinaMarket->IsSystemReady());

		// 恢复系统原态
		gl_pChinaMarket->SetStockDayLineNeedUpdate(4800);
		gl_pChinaMarket->SetStockDayLineNeedUpdate(gl_pChinaMarket->GetTotalStock());
	}

	TEST_F(CChinaMarketTest, TestTaskResetMarketAgain) {
		tm tm_;
		tm_.tm_wday = 1;
		gl_pChinaMarket->SetSystemReady(false);
		gl_pChinaMarket->__TEST_SetMarketTM(tm_);
		EXPECT_TRUE(gl_pChinaMarket->IsPermitResetMarket());
		EXPECT_FALSE(gl_pChinaMarket->IsSystemReady());
		EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
		EXPECT_TRUE(gl_pChinaMarket->TaskResetMarketAgain(92459));
		EXPECT_FALSE(gl_pChinaMarket->IsSystemReady());
		EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
		EXPECT_TRUE(gl_pChinaMarket->IsPermitResetMarket());
		EXPECT_TRUE(gl_pChinaMarket->TaskResetMarketAgain(92701));
		EXPECT_FALSE(gl_pChinaMarket->IsSystemReady());
		EXPECT_FALSE(gl_pChinaMarket->IsPermitResetMarket());
		EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
		gl_pChinaMarket->SetResetMarket(false);
		gl_pChinaMarket->SetSystemReady(false);
		gl_pChinaMarket->SetPermitResetMarket(true);
		EXPECT_TRUE(gl_pChinaMarket->TaskResetMarketAgain(92500));
		EXPECT_FALSE(gl_pChinaMarket->IsSystemReady());
		EXPECT_FALSE(gl_pChinaMarket->IsPermitResetMarket());
		EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
	}

	TEST_F(CChinaMarketTest, TestTaskClearChoicedRTDataSet) {
		EXPECT_FALSE(gl_pChinaMarket->IsRTDataSetCleared());
		gl_pChinaMarket->TaskClearChoicedRTDataSet(92900);
		EXPECT_FALSE(gl_pChinaMarket->IsRTDataSetCleared());
		gl_pChinaMarket->TaskClearChoicedRTDataSet(92901);
		EXPECT_TRUE(gl_pChinaMarket->IsRTDataSetCleared());
	}

	TEST_F(CChinaMarketTest, TestIsCurrentEditStockChanged) {
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentEditStockChanged());
		gl_pChinaMarket->SetCurrentEditStockChanged(true);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentEditStockChanged());
	}

	TEST_F(CChinaMarketTest, TestAddChoicedStock) {
		auto pStock = gl_pChinaMarket->GetStock(1);
		gl_pChinaMarket->AddChoicedStock(pStock);
		EXPECT_EQ(gl_pChinaMarket->GetChoicedStockSize(), 1);
		pStock = gl_pChinaMarket->GetStock(2);
		EXPECT_TRUE(gl_pChinaMarket->AddChoicedStock(pStock));
		EXPECT_EQ(gl_pChinaMarket->GetChoicedStockSize(), 2);
		pStock = gl_pChinaMarket->GetStock(1);
		EXPECT_FALSE(gl_pChinaMarket->AddChoicedStock(pStock));
		EXPECT_EQ(gl_pChinaMarket->GetChoicedStockSize(), 2);

		EXPECT_TRUE(gl_pChinaMarket->DeleteChoicedStock(pStock));
		pStock = gl_pChinaMarket->GetStock(5);
		EXPECT_EQ(gl_pChinaMarket->GetChoicedStockSize(), 1);
		EXPECT_FALSE(gl_pChinaMarket->DeleteChoicedStock(pStock));
		gl_pChinaMarket->ClearChoiceStockContainer();
	}

	TEST_F(CChinaMarketTest, TestAddChoicedStock2) {
		EXPECT_EQ(gl_pChinaMarket->GetChoicedStockSize(), 0);
		CChinaStockPtr pStock = gl_pChinaMarket->GetStock(0);
		gl_pChinaMarket->AddChoicedStock(pStock);
		EXPECT_EQ(gl_pChinaMarket->GetChoicedStockSize(), 1);
		gl_pChinaMarket->ClearChoiceStockContainer();
		EXPECT_EQ(gl_pChinaMarket->GetChoicedStockSize(), 0);
	}

	TEST_F(CChinaMarketTest, TestCheckMarketOpen) {
		tm tm_;
		tm_.tm_wday = 1;
		gl_pChinaMarket->__TEST_SetMarketTM(tm_);
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckMarketOpen(92800));
		EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
		EXPECT_TRUE(gl_pChinaMarket->TaskCheckMarketOpen(92801));
		EXPECT_TRUE(gl_pChinaMarket->IsMarketOpened());
		EXPECT_TRUE(gl_pChinaMarket->TaskCheckMarketOpen(150559));
		EXPECT_TRUE(gl_pChinaMarket->IsMarketOpened());
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckMarketOpen(150600));
		tm_.tm_wday = 0;
		gl_pChinaMarket->__TEST_SetMarketTM(tm_);
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckMarketOpen(92859));
		EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckMarketOpen(92900));
		EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckMarketOpen(150559));
		EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckMarketOpen(150600));
		EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
	}

	TEST_F(CChinaMarketTest, TestCheckFastReceivingRTData) {
		tm tm_;
		tm_.tm_wday = 1;
		gl_pChinaMarket->__TEST_SetMarketTM(tm_);
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckStartReceivingData(91159));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_TRUE(gl_pChinaMarket->TaskCheckStartReceivingData(91200));
		EXPECT_TRUE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_TRUE(gl_pChinaMarket->TaskCheckStartReceivingData(114500));
		EXPECT_TRUE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckStartReceivingData(114501));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckStartReceivingData(124459));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_TRUE(gl_pChinaMarket->TaskCheckStartReceivingData(124500));
		EXPECT_TRUE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_TRUE(gl_pChinaMarket->TaskCheckStartReceivingData(150630));
		EXPECT_TRUE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckStartReceivingData(150631));
		tm_.tm_wday = 0;
		gl_pChinaMarket->__TEST_SetMarketTM(tm_);
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckStartReceivingData(91459));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckStartReceivingData(91500));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckStartReceivingData(113459));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckStartReceivingData(113500));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckStartReceivingData(125459));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckStartReceivingData(125500));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckStartReceivingData(150630));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckStartReceivingData(150631));
		EXPECT_FALSE(gl_pChinaMarket->IsFastReceivingRTData());
	}

	TEST_F(CChinaMarketTest, TestIncreaseNeteaseDayLineInquiringIndex) {
		long k = 0;
		int i = gl_pChinaMarket->IncreaseStockInquiringIndex(k, gl_pChinaMarket->GetTotalStock());
		EXPECT_EQ(i, 1);
		EXPECT_EQ(k, 1);
		i = gl_pChinaMarket->IncreaseStockInquiringIndex(k, gl_pChinaMarket->GetTotalStock());
		EXPECT_EQ(i, 2);
		EXPECT_EQ(k, 2);
		k = 11999;
		i = gl_pChinaMarket->IncreaseStockInquiringIndex(k, gl_pChinaMarket->GetTotalStock());
		EXPECT_EQ(k, 0);
		EXPECT_EQ(i, 0);
	}

	TEST_F(CChinaMarketTest, TestTaskProcessRTData) {
		gl_ThreadStatus.SetRTDataNeedCalculate(true);
		EXPECT_TRUE(gl_pChinaMarket->TaskProcessRTData());
		gl_ThreadStatus.SetRTDataNeedCalculate(false);
	}

	TEST_F(CChinaMarketTest, TestSetCheckActiveStockFlag1) {
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady()) << "单元测试时，系统状态永远为准备好了";
		gl_pChinaMarket->SetSystemReady(false);
		gl_pChinaMarket->TaskSetCheckActiveStockFlag(93500);
		EXPECT_TRUE(gl_pChinaMarket->IsCheckActiveStock()) << "当系统尚未准备好时，检测活跃股票的标识永远为真";
		gl_pChinaMarket->SetSystemReady(true);
	}

	TEST_F(CChinaMarketTest, TestSetCheckActiveStockFlag2) {
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		EXPECT_TRUE(gl_pChinaMarket->IsCheckActiveStock());
		gl_pChinaMarket->TaskSetCheckActiveStockFlag(91459);
		EXPECT_FALSE(gl_pChinaMarket->IsCheckActiveStock());
		gl_pChinaMarket->TaskSetCheckActiveStockFlag(91500);
		EXPECT_TRUE(gl_pChinaMarket->IsCheckActiveStock());
		gl_pChinaMarket->TaskSetCheckActiveStockFlag(92659);
		EXPECT_TRUE(gl_pChinaMarket->IsCheckActiveStock());
		gl_pChinaMarket->TaskSetCheckActiveStockFlag(92700);
		EXPECT_FALSE(gl_pChinaMarket->IsCheckActiveStock());
		gl_pChinaMarket->TaskSetCheckActiveStockFlag(113059);
		EXPECT_FALSE(gl_pChinaMarket->IsCheckActiveStock());
		gl_pChinaMarket->TaskSetCheckActiveStockFlag(113300);
		EXPECT_TRUE(gl_pChinaMarket->IsCheckActiveStock());
		gl_pChinaMarket->TaskSetCheckActiveStockFlag(125859);
		EXPECT_TRUE(gl_pChinaMarket->IsCheckActiveStock());
		gl_pChinaMarket->TaskSetCheckActiveStockFlag(125900);
		EXPECT_FALSE(gl_pChinaMarket->IsCheckActiveStock());
		gl_pChinaMarket->TaskSetCheckActiveStockFlag(150000);
		EXPECT_FALSE(gl_pChinaMarket->IsCheckActiveStock());

		gl_pChinaMarket->SetCheckActiveStock(true); // 恢复原态
	}

	TEST_F(CChinaMarketTest, TestLoadStockCodeDB) {
		// 股票代码数据库在全局环境设置时即已装入测试系统，故而直接测试即可。
		CChinaStockPtr pStock = nullptr;
		pStock = gl_pChinaMarket->GetStock(0);
		EXPECT_THAT(pStock->IsIPOed(), IsTrue());
		EXPECT_STREQ(pStock->GetSymbol(), _T("000001.SS"));
		EXPECT_EQ(pStock->GetDayLineStartDate(), 19901220);
		EXPECT_FALSE(pStock->IsActive()) << "装载股票代码时永远设置为假";
		pStock = gl_pChinaMarket->GetStock(_T("600002.SS"));
		EXPECT_TRUE(pStock->IsDelisted());
		EXPECT_EQ(pStock->GetDayLineStartDate(), 19980409);
		EXPECT_EQ(pStock->GetDayLineEndDate(), 20060406);
		EXPECT_FALSE(pStock->IsActive());
	}

	TEST_F(CChinaMarketTest, TestGetLastLoginDate) {
		gl_pChinaMarket->SetLastLoginDate(19900102);
		EXPECT_EQ(gl_pChinaMarket->GetLastLoginDate(), 19900102);
	}

	TEST_F(CChinaMarketTest, TestGetRSStartDate) {
		gl_pChinaMarket->SetRSStartDate(19900202);
		EXPECT_EQ(gl_pChinaMarket->GetRSStartDate(), 19900202);
	}

	TEST_F(CChinaMarketTest, TestGetRSEndDate) {
		gl_pChinaMarket->SetRSEndDate(19900302);
		EXPECT_EQ(gl_pChinaMarket->GetRSEndDate(), 19900302);
	}

	TEST_F(CChinaMarketTest, TestGetNewestTransactionTime) {
		gl_pChinaMarket->SetNewestTransactionTime(10101010);
		EXPECT_EQ(gl_pChinaMarket->GetNewestTransactionTime(), 10101010);
	}

	TEST_F(CChinaMarketTest, TestGetReadingTengxunRTDataTime) {
		time_t tt = 1010101010;
		gl_pChinaMarket->SetReadingTengxunRTDataTime(tt);
		EXPECT_EQ(gl_pChinaMarket->GetReadingTengxunRTDataTime(), tt);
	}

	TEST_F(CChinaMarketTest, TesstIsTodayTempRTDataLoaded) {
		EXPECT_FALSE(gl_pChinaMarket->IsTodayTempRTDataLoaded());
		gl_pChinaMarket->SetTodayTempRTDataLoaded(true);
		EXPECT_TRUE(gl_pChinaMarket->IsTodayTempRTDataLoaded());
		gl_pChinaMarket->SetTodayTempRTDataLoaded(false);
		EXPECT_FALSE(gl_pChinaMarket->IsTodayTempRTDataLoaded());
	}

	TEST_F(CChinaMarketTest, TesstIsCheckActiveStock) {
		EXPECT_TRUE(gl_pChinaMarket->IsCheckActiveStock());
		gl_pChinaMarket->SetCheckActiveStock(false);
		EXPECT_FALSE(gl_pChinaMarket->IsCheckActiveStock());
		gl_pChinaMarket->SetCheckActiveStock(true);
		EXPECT_TRUE(gl_pChinaMarket->IsCheckActiveStock());
	}

	TEST_F(CChinaMarketTest, TestGetStockPtr) {
		CChinaStockPtr pStock = nullptr;
		pStock = gl_pChinaMarket->GetStock(_T("600000.SS"));
		EXPECT_STREQ(pStock->GetSymbol(), _T("600000.SS"));
	}

	TEST_F(CChinaMarketTest, TestIsInvalidNeteaseRTData) {
		CWebDataPtr pWebDataReceived;
		pWebDataReceived = make_shared<CWebData>();
		CString str = _T("_ntes_quote_callback({ });");
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
		pWebDataReceived->SetBufferLength(str.GetLength());
		pWebDataReceived->ResetCurrentPos();
		EXPECT_TRUE(gl_pChinaMarket->IsInvalidNeteaseRTData(*pWebDataReceived));
		str = _T("_ntes_quote_callback({});");
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
		pWebDataReceived->SetBufferLength(str.GetLength());
		pWebDataReceived->ResetCurrentPos();
		EXPECT_FALSE(gl_pChinaMarket->IsInvalidNeteaseRTData(*pWebDataReceived));
	}

	TEST_F(CChinaMarketTest, TestIsValidNeteaseRTDataPrefix) {
		CWebDataPtr pWebDataReceived;
		pWebDataReceived = make_shared<CWebData>();
		CString str = _T("_ntes_quote_callback({\"");
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
		pWebDataReceived->SetBufferLength(str.GetLength());
		pWebDataReceived->ResetCurrentPos();
		EXPECT_TRUE(gl_pChinaMarket->IsValidNeteaseRTDataPrefix(*pWebDataReceived));
		str = _T("_ntes_quo_callback({\"");
		pWebDataReceived->SetData((LPSTR)str.GetBuffer(), str.GetLength(), 0);
		pWebDataReceived->SetBufferLength(str.GetLength());
		pWebDataReceived->ResetCurrentPos();
		EXPECT_FALSE(gl_pChinaMarket->IsValidNeteaseRTDataPrefix(*pWebDataReceived));
	}

	TEST_F(CChinaMarketTest, TestValidateNeteaseRTData) {
		CWebRTData webRTData;

		webRTData.SetDataSource(__NETEASE_RT_WEB_DATA__);
		webRTData.SetActive(false);
		webRTData.SetSymbol(_T("000001.SZ"));

		EXPECT_FALSE(gl_pChinaMarket->CheckNeteaseRTDataValidation(webRTData));

		gl_pChinaMarket->GetStock(webRTData.GetSymbol())->SetActive(true);
		webRTData.SetActive(true);
		EXPECT_TRUE(gl_pChinaMarket->CheckNeteaseRTDataValidation(webRTData));

		gl_pChinaMarket->GetStock(webRTData.GetSymbol())->SetActive(false);
		EXPECT_FALSE(gl_pChinaMarket->CheckNeteaseRTDataValidation(webRTData));
		EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("000001.SZ 网易实时检测到不处于活跃状态"));

		gl_pChinaMarket->GetStock(webRTData.GetSymbol())->SetActive(true);

		webRTData.SetSymbol(_T("60601.SZ")); // 没有这个股票
		EXPECT_FALSE(gl_pChinaMarket->CheckNeteaseRTDataValidation(webRTData));
		EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("60601.SZ 无效股票代码（网易实时数据）"));

		gl_pChinaMarket->GetStock(_T("000001.SZ"))->SetActive(true);
	}

	TEST_F(CChinaMarketTest, TestIsValidTengxunRTData) {
		CWebDataPtr pWebDataReceived;
		pWebDataReceived = make_shared<CWebData>();
		CString str = _T("v_pv_none_match=\"1\";\n");
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
		pWebDataReceived->SetBufferLength(str.GetLength());
		pWebDataReceived->ResetCurrentPos();

		EXPECT_TRUE(gl_pChinaMarket->IsInvalidTengxunRTData(*pWebDataReceived));
		EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);

		str = _T("v_pv_none_mtch=\"1\";\n");
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
		pWebDataReceived->SetBufferLength(str.GetLength());
		pWebDataReceived->ResetCurrentPos();

		EXPECT_FALSE(gl_pChinaMarket->IsInvalidTengxunRTData(*pWebDataReceived));
		EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);
	}

	TEST_F(CChinaMarketTest, TestCheckTengxunRTData) {
		CWebRTData webRTData;

		webRTData.SetDataSource(__TENGXUN_RT_WEB_DATA__);
		webRTData.SetActive(false);
		webRTData.SetSymbol(_T("000001.SZ"));

		EXPECT_FALSE(gl_pChinaMarket->CheckTengxunRTDataValidation(webRTData));

		gl_pChinaMarket->GetStock(webRTData.GetSymbol())->SetActive(true);
		webRTData.SetActive(true);
		EXPECT_TRUE(gl_pChinaMarket->CheckTengxunRTDataValidation(webRTData));

		gl_pChinaMarket->GetStock(webRTData.GetSymbol())->SetActive(false);
		EXPECT_FALSE(gl_pChinaMarket->CheckTengxunRTDataValidation(webRTData));
		//EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("000001.SZ腾讯实时检测到不处于活跃状态"));

		gl_pChinaMarket->GetStock(webRTData.GetSymbol())->SetActive(true);

		webRTData.SetSymbol(_T("60601.SZ")); // 没有这个股票
		EXPECT_FALSE(gl_pChinaMarket->CheckTengxunRTDataValidation(webRTData));
		EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("60601.SZ无效股票代码（腾讯实时数据）"));

		gl_pChinaMarket->GetStock(_T("000001.SZ"))->SetActive(true);
	}

	TEST_F(CChinaMarketTest, TestTaskProcessWebRTDataGetFromSinaServer) {
		CString strRTData = _T("var hq_str_sh600000=\"浦发银行,10.120,9.940,10.090,10.180,9.960,10.090,10.100,42609358,428744766.000,453445,10.090,312600,10.080,173519,10.070,242800,10.060,136700,10.050,3600,10.100,3200,10.120,5100,10.130,129900,10.150,476200,10.160,2020-03-20,15:00:00,00,\";\nvar hq_str_sh600004=\"白云机场,13.010,12.900,13.300,13.350,12.990,13.300,13.320,9689808,127814773.000,4810,13.300,11100,13.290,5800,13.280,5200,13.270,10800,13.260,1600,13.320,61100,13.330,46100,13.340,114467,13.350,35000,13.360,2020-03-20,15:00:09,00,\";\nvar hq_str_sh600008=\"首创股份,3.050,3.040,3.050,3.080,3.030,3.050,3.060,22087562,67408587.000,760200,3.050,625400,3.040,570900,3.030,329000,3.020,202000,3.010,9200,3.060,112100,3.070,491897,3.080,449600,3.090,664140,3.100,2020-03-20,15:00:09,00,\";\n");
		CWebDataPtr pData = make_shared<CWebData>();
		pData->__TEST_SetBuffer__(strRTData);
		gl_WebInquirer.PushSinaRTData(pData);
		gl_pChinaMarket->TaskProcessWebRTDataGetFromSinaServer();
		EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 3);
		EXPECT_EQ(gl_WebInquirer.GetSinaRTDataSize(), 0);
		for (int i = 0; i < 3; i++) {
			CWebRTDataPtr pRTData = gl_WebRTDataContainer.PopSinaData();
		}
	}

	TEST_F(CChinaMarketTest, TestTaskProcessWebRTDataGetFromNeteaseServer) {
		CString strRTData = _T("_ntes_quote_callback({\"0601872\":{\"code\": \"0601872\",\"percent\": 0.038251, \"high\": 5.72, \"askvol3\": 311970, \"askvol2\": 257996,\"askvol5\": 399200, \"askvol4\": 201000, \"price\": 5.7, \"open\": 5.53, \"bid5\": 5.65, \"bid4\": 5.66, \"bid3\": 5.67,\"bid2\": 5.68, \"bid1\": 5.69, \"low\": 5.51, \"updown\": 0.21, \"type\": \"SH\", \"symbol\": \"601872\", \"status\": 0,\"ask4\": 5.73, \"bidvol3\": 234700, \"bidvol2\": 166300, \"bidvol1\": 641291, \"update\": \"2019/11/04 15:59:54\",\"bidvol5\": 134500, \"bidvol4\": 96600, \"yestclose\": 5.49, \"askvol1\": 396789, \"ask5\": 5.74, \"volume\": 78750304,\"ask1\": 5.7, \"name\": \"\u62db\u5546\u8f6e\u8239\", \"ask3\": 5.72, \"ask2\": 5.71, \"arrow\": \"\u2191\",\"time\": \"2019/11/04 15:59:52\", \"turnover\": 443978974} });");
		CWebDataPtr pData = make_shared<CWebData>();
		pData->__TEST_SetBuffer__(strRTData);
		gl_WebInquirer.PushNeteaseRTData(pData);
		gl_pChinaMarket->TaskProcessWebRTDataGetFromNeteaseServer();
		EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 1);
		EXPECT_EQ(gl_WebInquirer.GetNeteaseRTDataSize(), 0);
		for (int i = 0; i < 1; i++) {
			CWebRTDataPtr pRTData = gl_WebRTDataContainer.PopNeteaseData();
		}
	}

	TEST_F(CChinaMarketTest, TestTaskProcessWebRTDataGetFromTengxunServer) {
		CString strRTData = _T("v_sz000001=\"51~平安银行~000001~15.59~15.90~15.75~1046363~518391~527971~15.58~2365~15.57~802~15.56~1855~15.55~2316~15.54~320~15.59~661~15.60~15381~15.61~3266~15.62~450~15.63~520~~20190930154003~-0.31~-1.95~15.89~15.57~15.59/1046363/1645828527~1046363~164583~0.54~11.27~~15.89~15.57~2.01~3025.36~3025.38~1.15~17.49~14.31~0.73~-12617~15.73~9.82~12.19~~~1.24~164582.85~0.00~0~~GP-A~68.91~~0.82\";\n");
		CWebDataPtr pData = make_shared<CWebData>();
		pData->__TEST_SetBuffer__(strRTData);
		gl_WebInquirer.PushTengxunRTData(pData);
		gl_pChinaMarket->TaskProcessWebRTDataGetFromTengxunServer();
		EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 1);
		EXPECT_EQ(gl_WebInquirer.GetTengxunRTDataSize(), 0);
		for (int i = 0; i < 1; i++) {
			CWebRTDataPtr pRTData = gl_WebRTDataContainer.PopTengxunData();
		}
	}

	TEST_F(CChinaMarketTest, TestTaskDiscardNeteaseRTData) {
		CWebRTDataPtr prtData1, prtData2;
		prtData1 = make_shared<CWebRTData>();
		prtData2 = make_shared<CWebRTData>();
		EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 0);
		gl_WebRTDataContainer.PushNeteaseData(prtData1);
		gl_WebRTDataContainer.PushNeteaseData(prtData2);
		EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 2);
		gl_pChinaMarket->TaskDiscardNeteaseRTData();
		EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 0);
	}

	TEST_F(CChinaMarketTest, TestTaskDiscardSinaRTData) {
		CWebRTDataPtr prtData1, prtData2;
		prtData1 = make_shared<CWebRTData>();
		prtData2 = make_shared<CWebRTData>();
		EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 0);
		gl_WebRTDataContainer.PushSinaData(prtData1);
		gl_WebRTDataContainer.PushSinaData(prtData2);
		EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 2);
		gl_pChinaMarket->TaskDiscardSinaRTData();
		EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 0);
	}

	TEST_F(CChinaMarketTest, TestTaskDiscardTengxunRTData) {
		CWebRTDataPtr prtData1, prtData2;
		prtData1 = make_shared<CWebRTData>();
		prtData2 = make_shared<CWebRTData>();
		EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 0);
		gl_WebRTDataContainer.PushTengxunData(prtData1);
		gl_WebRTDataContainer.PushTengxunData(prtData2);
		EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 2);
		gl_pChinaMarket->TaskDiscardTengxunRTData();
		EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 0);
	}

	TEST_F(CChinaMarketTest, TestStoreChoicedRTData) {
		EXPECT_EQ(gl_pChinaMarket->GetChoicedRTDataSize(), 0);
		CWebRTDataPtr pRTData = make_shared<CWebRTData>();
		gl_pChinaMarket->StoreChoiceRTData(pRTData);
		EXPECT_EQ(gl_pChinaMarket->GetChoicedRTDataSize(), 1);
		gl_pChinaMarket->ClearChoicedRTDataQueue();
		EXPECT_EQ(gl_pChinaMarket->GetChoicedRTDataSize(), 0);
	}

	TEST_F(CChinaMarketTest, TestClearDayLineNeedUpdaeStatus) {
		gl_pChinaMarket->ClearDayLineNeedUpdateStatus();

		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			EXPECT_FALSE(gl_pChinaMarket->GetStock(i)->IsDayLineNeedUpdate());
		}

		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CChinaMarketTest, TestTaskGetRTDataFromWeb1) { // 这三个从网络读取数据的测试有内存泄露问题，待查。
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		EXPECT_TRUE(gl_pChinaMarket->IsUsingSinaRTDataServer());
		gl_pChinaMarket->SetSystemReady(false);
		gl_pChinaMarket->SetCountDownTengxunNumber(5);
		EXPECT_TRUE(gl_pChinaMarket->IsUsingTengxunRTDataReceiver());
		EXPECT_TRUE(gl_pChinaMarket->IsUsingNeteaseRTDataReceiver());
		gl_pChinaMarket->SetUsingNeteaseRTDataReceiver(true);
		EXPECT_CALL(*s_pMockSinaRTWebInquiry, StartReadingThread).Times(1);
		EXPECT_CALL(*s_pMockNeteaseRTWebInquiry, StartReadingThread).Times(0);
		EXPECT_CALL(*s_pMockTengxunRTWebInquiry, StartReadingThread).Times(0);
		EXPECT_FALSE(s_pMockSinaRTWebInquiry->IsReadingWebData());
		gl_pChinaMarket->TaskGetRTDataFromWeb();
		EXPECT_TRUE(s_pMockSinaRTWebInquiry->IsReadingWebData());

		s_pMockSinaRTWebInquiry->SetReadingWebData(false);

		gl_pChinaMarket->SetUsingNeteaseRTDataReceiver(true);
		s_pMockSinaRTWebInquiry->SetReadingWebData(false);
		s_pMockTengxunRTWebInquiry->SetReadingWebData(false);
		s_pMockNeteaseRTWebInquiry->SetReadingWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->SetSinaStockRTDataInquiringIndexFromTotalStockSet(0);
	}

	TEST_F(CChinaMarketTest, TestTaskGetRTDataFromWeb2) {
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		EXPECT_TRUE(gl_pChinaMarket->IsUsingSinaRTDataServer());
		gl_pChinaMarket->SetUsingNeteaseRTDataServer();
		gl_pChinaMarket->SetCountDownTengxunNumber(5);
		EXPECT_TRUE(gl_pChinaMarket->IsUsingTengxunRTDataReceiver());
		EXPECT_TRUE(gl_pChinaMarket->IsUsingNeteaseRTDataReceiver());
		gl_pChinaMarket->SetUsingNeteaseRTDataReceiver(true);
		EXPECT_CALL(*s_pMockSinaRTWebInquiry, StartReadingThread).Times(0);
		EXPECT_CALL(*s_pMockNeteaseRTWebInquiry, StartReadingThread).Times(1);
		EXPECT_CALL(*s_pMockTengxunRTWebInquiry, StartReadingThread).Times(0); // _T("腾讯实时数据采集需要计数器倒数至零");
		EXPECT_FALSE(s_pMockNeteaseRTWebInquiry->IsReadingWebData());
		gl_pChinaMarket->TaskGetRTDataFromWeb();
		EXPECT_TRUE(s_pMockNeteaseRTWebInquiry->IsReadingWebData());
		EXPECT_EQ(gl_pChinaMarket->GetCountDownTengxunNumber(), 4) << _T("调用TaskGetRTDataFromWeb后计数器已减一");
		s_pMockNeteaseRTWebInquiry->SetReadingWebData(false);
		gl_pChinaMarket->SetUsingSinaRTDataServer();

		gl_pChinaMarket->SetCountDownTengxunNumber(5);
		gl_pChinaMarket->SetUsingNeteaseRTDataReceiver(true);
		s_pMockSinaRTWebInquiry->SetReadingWebData(false);
		s_pMockTengxunRTWebInquiry->SetReadingWebData(false);
		s_pMockNeteaseRTWebInquiry->SetReadingWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
	}

	TEST_F(CChinaMarketTest, TestTaskGetRTDataFromWeb3) {
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->SetCountDownTengxunNumber(0);
		EXPECT_CALL(*s_pMockSinaRTWebInquiry, StartReadingThread).Times(1);
		EXPECT_CALL(*s_pMockTengxunRTWebInquiry, StartReadingThread).Times(1); // _T("腾讯实时数据采集计数器已倒数至零");
		EXPECT_FALSE(s_pMockSinaRTWebInquiry->IsReadingWebData());
		EXPECT_FALSE(s_pMockTengxunRTWebInquiry->IsReadingWebData());
		gl_pChinaMarket->TaskGetRTDataFromWeb();
		EXPECT_TRUE(s_pMockSinaRTWebInquiry->IsReadingWebData());
		EXPECT_TRUE(s_pMockTengxunRTWebInquiry->IsReadingWebData());

		gl_pChinaMarket->SetUsingNeteaseRTDataReceiver(true);
		s_pMockSinaRTWebInquiry->SetReadingWebData(false);
		s_pMockTengxunRTWebInquiry->SetReadingWebData(false);
		s_pMockNeteaseRTWebInquiry->SetReadingWebData(false);
		gl_pChinaMarket->SetSinaStockRTDataInquiringIndexFromTotalStockSet(0);
	}

	TEST_F(CChinaMarketTest, TestGetRTDataReceived) {
		gl_pChinaMarket->SetRTDataReceived(101010);
		EXPECT_EQ(gl_pChinaMarket->GetRTDataReceived(), 101010);
		gl_pChinaMarket->SetRTDataReceived(1010101010);
		EXPECT_EQ(gl_pChinaMarket->GetRTDataReceived(), 1010101010);
	}

	TEST_F(CChinaMarketTest, TestIsTimeToResetSystem) {
		EXPECT_FALSE(gl_pChinaMarket->IsTimeToResetSystem(91259));
		EXPECT_TRUE(gl_pChinaMarket->IsTimeToResetSystem(91300));
		EXPECT_TRUE(gl_pChinaMarket->IsTimeToResetSystem(91400));
		EXPECT_FALSE(gl_pChinaMarket->IsTimeToResetSystem(91401));
		EXPECT_FALSE(gl_pChinaMarket->IsTimeToResetSystem(92459));
		EXPECT_TRUE(gl_pChinaMarket->IsTimeToResetSystem(92500));
		EXPECT_TRUE(gl_pChinaMarket->IsTimeToResetSystem(92700));
		EXPECT_FALSE(gl_pChinaMarket->IsTimeToResetSystem(92701));
		EXPECT_FALSE(gl_pChinaMarket->IsTimeToResetSystem(93000));
	}

	TEST_F(CChinaMarketTest, TestCheckMarketReady) {
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->SetRTDataReceived(0);
		EXPECT_TRUE(gl_pChinaMarket->CheckMarketReady());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetSystemReady(false);
		gl_pChinaMarket->SetRTDataReceived(0);
		gl_pChinaMarket->SetSystemReady(false);
		EXPECT_FALSE(gl_pChinaMarket->CheckMarketReady());
		EXPECT_FALSE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetSystemReady(false);
		gl_pChinaMarket->SetRTDataReceived(0);
		gl_pChinaMarket->SetSystemReady(false);
		EXPECT_FALSE(gl_pChinaMarket->CheckMarketReady());
		EXPECT_FALSE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetRTDataReceived(24000);
		EXPECT_FALSE(gl_pChinaMarket->CheckMarketReady());
		EXPECT_FALSE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetRTDataReceived(24000 + 1);
		EXPECT_TRUE(gl_pChinaMarket->CheckMarketReady());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetSystemReady(false);
	}

	TEST_F(CChinaMarketTest, TestChangeCurrentStockToNextStock1) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
		gl_pChinaMarket->SetCurrentStock(gl_pChinaMarket->GetStock(0));
		gl_pChinaMarket->ChangeToNextStock();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentStock()->GetOffset(), 1);
		gl_pChinaMarket->SetCurrentStockChanged(false);

		gl_pChinaMarket->SetCurrentSelectedPosition(0);
		gl_pChinaMarket->SetCurrentSelectedStockSet(-1);
	}

	TEST_F(CChinaMarketTest, TestChangeCurrentStockToNextStock2) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
		gl_pChinaMarket->SetCurrentSelectedStockSet(10); // 10、11、12股票集中有股票
		EXPECT_TRUE(gl_pChinaMarket->GetCurrentSelectedStock() != nullptr);
		gl_pChinaMarket->SetCurrentStock(gl_pChinaMarket->GetCurrentSelectedStock());

		gl_pChinaMarket->ChangeToPrevStock();
		gl_pChinaMarket->ChangeToNextStock();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedPosition(), 0);
		gl_pChinaMarket->ChangeToNextStock();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedPosition(), 1);
		gl_pChinaMarket->ChangeToPrevStock();
		gl_pChinaMarket->ChangeToPrevStock();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentStockSetSize(), gl_pChinaMarket->GetCurrentSelectedPosition() + 1);
		gl_pChinaMarket->SetCurrentStockChanged(false);

		gl_pChinaMarket->SetCurrentSelectedPosition(0);
		gl_pChinaMarket->SetCurrentSelectedStockSet(-1);
	}

	TEST_F(CChinaMarketTest, TestChangeCurrentStockToPrevStock1) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
		gl_pChinaMarket->SetCurrentStock(gl_pChinaMarket->GetStock(1)); // 选取A股指数
		gl_pChinaMarket->ChangeToPrevStock();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentStock()->GetOffset(), 0) << _T("上一个是上证指数");
		gl_pChinaMarket->ChangeToPrevStock();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentStock()->GetOffset(), gl_pChinaMarket->GetTotalStock() - 1) << _T("上证指数前的为空，然后就转到最后面的中证煤炭了");
		gl_pChinaMarket->SetCurrentStockChanged(false);
		gl_pChinaMarket->SetCurrentSelectedPosition(0);
	}

	TEST_F(CChinaMarketTest, TestChangeCurrentStockToPrevStock2) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
		gl_pChinaMarket->SetCurrentSelectedStockSet(10); // 10、11、12股票集中有股票
		EXPECT_TRUE(gl_pChinaMarket->GetCurrentSelectedStock() != nullptr);
		gl_pChinaMarket->SetCurrentStock(gl_pChinaMarket->GetCurrentSelectedStock());

		gl_pChinaMarket->ChangeToNextStock();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedPosition(), 1);
		gl_pChinaMarket->ChangeToPrevStock();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedPosition(), 0);
		gl_pChinaMarket->ChangeToPrevStock();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedPosition() + 1, gl_pChinaMarket->GetCurrentStockSetSize());
		gl_pChinaMarket->SetCurrentStockChanged(false);

		gl_pChinaMarket->SetCurrentSelectedPosition(0);
		gl_pChinaMarket->SetCurrentSelectedStockSet(-1);
	}

	TEST_F(CChinaMarketTest, TestChangeToPrevStockSet) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
		gl_pChinaMarket->ChangeToPrevStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 19);
		gl_pChinaMarket->ChangeToPrevStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 18);
		gl_pChinaMarket->ChangeToPrevStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 12);
		gl_pChinaMarket->ChangeToPrevStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 11);
		gl_pChinaMarket->ChangeToPrevStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 10);

		gl_pChinaMarket->SetCurrentSelectedStockSet(-1);
	}

	TEST_F(CChinaMarketTest, TestChangeToNextStockSet) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
		gl_pChinaMarket->ChangeToNextStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 10);
		gl_pChinaMarket->ChangeToNextStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 11);
		gl_pChinaMarket->ChangeToNextStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 12);
		gl_pChinaMarket->ChangeToNextStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 18);
		gl_pChinaMarket->ChangeToNextStockSet();
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), 19);

		gl_pChinaMarket->SetCurrentSelectedStockSet(-1);
	}

	TEST_F(CChinaMarketTest, TestIsTotalStockSetSelected) {
		EXPECT_EQ(gl_pChinaMarket->GetCurrentSelectedStockSet(), -1);
		EXPECT_TRUE(gl_pChinaMarket->IsTotalStockSetSelected());
		gl_pChinaMarket->SetCurrentSelectedStockSet(0);
		EXPECT_FALSE(gl_pChinaMarket->IsTotalStockSetSelected());

		gl_pChinaMarket->SetCurrentSelectedStockSet(-1);
	}

	TEST_F(CChinaMarketTest, TestSetUsingSinaRTDataServer) {
		EXPECT_TRUE(gl_pChinaMarket->IsUsingSinaRTDataServer());
		EXPECT_FALSE(gl_pChinaMarket->IsUsingNeteaseRTDataServer());
		gl_pChinaMarket->SetUsingNeteaseRTDataServer();
		EXPECT_FALSE(gl_pChinaMarket->IsUsingSinaRTDataServer());
		EXPECT_TRUE(gl_pChinaMarket->IsUsingNeteaseRTDataServer());
		gl_pChinaMarket->SetUsingSinaRTDataServer();
		EXPECT_TRUE(gl_pChinaMarket->IsUsingSinaRTDataServer());
		EXPECT_FALSE(gl_pChinaMarket->IsUsingNeteaseRTDataServer());
	}

	TEST_F(CChinaMarketTest, TestIsUsingNeteaseRTDataReceiver) {
		EXPECT_TRUE(gl_pChinaMarket->IsUsingNeteaseRTDataReceiver());
		gl_pChinaMarket->SetUsingNeteaseRTDataReceiver(false);
		EXPECT_FALSE(gl_pChinaMarket->IsUsingNeteaseRTDataReceiver());
		gl_pChinaMarket->SetUsingNeteaseRTDataReceiver(true);
		EXPECT_TRUE(gl_pChinaMarket->IsUsingNeteaseRTDataReceiver());
	}

	TEST_F(CChinaMarketTest, TestIsUsingTengxunRTDataReceiver) {
		EXPECT_TRUE(gl_pChinaMarket->IsUsingTengxunRTDataReceiver());
		gl_pChinaMarket->SetUsingTengxunRTDataReceiver(false);
		EXPECT_FALSE(gl_pChinaMarket->IsUsingTengxunRTDataReceiver());
		gl_pChinaMarket->SetUsingTengxunRTDataReceiver(true);
		EXPECT_TRUE(gl_pChinaMarket->IsUsingTengxunRTDataReceiver());
	}

	TEST_F(CChinaMarketTest, TestGetCountDownTengxunNumber) {
		EXPECT_EQ(gl_pChinaMarket->GetCountDownTengxunNumber(), 5);
		gl_pChinaMarket->SetCountDownTengxunNumber(100);
		EXPECT_EQ(gl_pChinaMarket->GetCountDownTengxunNumber(), 100);
		gl_pChinaMarket->SetCountDownTengxunNumber(5);
	}

	TEST_F(CChinaMarketTest, TestGetUpDownRate) {
		EXPECT_DOUBLE_EQ(gl_pChinaMarket->GetUpDownRate(_T("10.0"), _T("0.0009")), 0.0) << _T("LastClose小于0.001时返回0");
		EXPECT_DOUBLE_EQ(gl_pChinaMarket->GetUpDownRate(_T("11.0"), _T("10.0")), 0.1);
		EXPECT_DOUBLE_EQ(gl_pChinaMarket->GetUpDownRate(_T("10.5"), _T("10.0")), 0.05);
		EXPECT_DOUBLE_EQ(gl_pChinaMarket->GetUpDownRate(_T("9.0"), _T("10.0")), -0.1);
		EXPECT_DOUBLE_EQ(gl_pChinaMarket->GetUpDownRate(_T("11.11"), _T("10.0")), 0.0) << _T("大于0.11时返回0");
		EXPECT_DOUBLE_EQ(gl_pChinaMarket->GetUpDownRate(_T("8.89"), _T("10.0")), 0.0) << _T("小于-0.11时返回0");
	}

	TEST_F(CChinaMarketTest, TestLoadOptionDB) {
		CSetOption setOption;

		setOption.Open();
		setOption.m_pDatabase->BeginTrans();
		while (!setOption.IsEOF()) {
			setOption.Delete();
			setOption.MoveNext();
		}
		setOption.m_pDatabase->CommitTrans();
		setOption.Close();

		gl_pChinaMarket->SetRSStartDate(20200101);
		gl_pChinaMarket->SetRSEndDate(20200202);
		gl_pChinaMarket->SetLastLoginDate(gl_pChinaMarket->GetMarketDate());
		gl_pChinaMarket->SetUpdatedDateFor10DaysRS1(19990101);
		gl_pChinaMarket->SetUpdatedDateFor10DaysRS2(19990202);

		gl_pChinaMarket->UpdateOptionDB();

		gl_pChinaMarket->SetRSStartDate(1);
		gl_pChinaMarket->SetRSEndDate(1);
		gl_pChinaMarket->SetLastLoginDate(1);
		gl_pChinaMarket->SetUpdatedDateFor10DaysRS1(1);
		gl_pChinaMarket->SetUpdatedDateFor10DaysRS2(1);

		gl_pChinaMarket->LoadOptionDB();

		EXPECT_EQ(gl_pChinaMarket->GetRSStartDate(), 20200101);
		EXPECT_EQ(gl_pChinaMarket->GetRSEndDate(), 20200202);
		EXPECT_EQ(gl_pChinaMarket->GetLastLoginDate(), gl_pChinaMarket->GetMarketDate());
		EXPECT_EQ(gl_pChinaMarket->GetUpdatedDateFor10DaysRS1(), 19990101);
		EXPECT_FALSE(gl_pChinaMarket->IsChoiced10RSStrong1StockSet());
		EXPECT_EQ(gl_pChinaMarket->GetUpdatedDateFor10DaysRS2(), 19990202);
		EXPECT_FALSE(gl_pChinaMarket->IsChoiced10RSStrong2StockSet());

		gl_pChinaMarket->SetRSStartDate(20100101);
		gl_pChinaMarket->SetRSEndDate(20100202);
		gl_pChinaMarket->SetLastLoginDate(20200303);
		gl_pChinaMarket->SetUpdatedDateFor10DaysRS1(19980101);
		gl_pChinaMarket->SetUpdatedDateFor10DaysRS2(19980202);

		gl_pChinaMarket->UpdateOptionDB();

		gl_pChinaMarket->SetRSStartDate(1);
		gl_pChinaMarket->SetRSEndDate(1);
		gl_pChinaMarket->SetLastLoginDate(1);
		gl_pChinaMarket->SetUpdatedDateFor10DaysRS1(1);
		gl_pChinaMarket->SetUpdatedDateFor10DaysRS2(1);

		gl_pChinaMarket->LoadOptionDB();

		EXPECT_EQ(gl_pChinaMarket->GetRSStartDate(), 20100101);
		EXPECT_EQ(gl_pChinaMarket->GetRSEndDate(), 20100202);
		EXPECT_EQ(gl_pChinaMarket->GetLastLoginDate(), gl_pChinaMarket->GetMarketDate()) << _T("永远是当前日期\n");
		EXPECT_EQ(gl_pChinaMarket->GetUpdatedDateFor10DaysRS1(), 19980101);
		EXPECT_FALSE(gl_pChinaMarket->IsChoiced10RSStrong1StockSet());
		EXPECT_EQ(gl_pChinaMarket->GetUpdatedDateFor10DaysRS2(), 19980202);
		EXPECT_FALSE(gl_pChinaMarket->IsChoiced10RSStrong2StockSet());

		setOption.Open();
		setOption.m_pDatabase->BeginTrans();
		while (!setOption.IsEOF()) {
			setOption.Delete();
			setOption.MoveNext();
		}
		setOption.m_pDatabase->CommitTrans();
		setOption.Close();
		gl_pChinaMarket->LoadOptionDB();

		EXPECT_EQ(gl_pChinaMarket->GetRSStartDate(), __CHINA_MARKET_BEGIN_DATE__);
		EXPECT_EQ(gl_pChinaMarket->GetRSEndDate(), __CHINA_MARKET_BEGIN_DATE__);
		EXPECT_EQ(gl_pChinaMarket->GetLastLoginDate(), __CHINA_MARKET_BEGIN_DATE__);
		EXPECT_EQ(gl_pChinaMarket->GetUpdatedDateFor10DaysRS1(), __CHINA_MARKET_BEGIN_DATE__);
		EXPECT_FALSE(gl_pChinaMarket->IsChoiced10RSStrong1StockSet());
		EXPECT_EQ(gl_pChinaMarket->GetUpdatedDateFor10DaysRS2(), __CHINA_MARKET_BEGIN_DATE__);
		EXPECT_FALSE(gl_pChinaMarket->IsChoiced10RSStrong2StockSet());
	}

	TEST_F(CChinaMarketTest, TestSetStockDayLineNeedUpdate) {
		EXPECT_TRUE(gl_pChinaMarket->TooManyStockDayLineNeedUpdate());
		gl_pChinaMarket->SetStockDayLineNeedUpdate(1000);
		EXPECT_FALSE(gl_pChinaMarket->TooManyStockDayLineNeedUpdate());
		gl_pChinaMarket->SetStockDayLineNeedUpdate(1001);
		EXPECT_TRUE(gl_pChinaMarket->TooManyStockDayLineNeedUpdate());
		gl_pChinaMarket->SetStockDayLineNeedUpdate(0);
		EXPECT_FALSE(gl_pChinaMarket->TooManyStockDayLineNeedUpdate());

		// 恢复原状
		gl_pChinaMarket->SetStockDayLineNeedUpdate(gl_pChinaMarket->GetTotalStock());
	}

	TEST_F(CChinaMarketTest, TestDeleteDayLineBasicInfo) {
		char buffer[20];
		CString strDate;

		CSetDayLineBasicInfo setDayLine, setDayLine2;
		CDayLinePtr pDayLine = make_shared<CDayLine>();

		pDayLine->SetStockSymbol(_T("600000.SS"));
		pDayLine->SetDate(19900101);

		_ltoa_s(19900101, buffer, 10);
		strDate = buffer;
		setDayLine.m_strFilter = _T("[Date] =");
		setDayLine.m_strFilter += strDate;
		setDayLine.Open();
		setDayLine.m_pDatabase->BeginTrans();
		pDayLine->AppendHistoryCandleBasic(&setDayLine);
		setDayLine.m_pDatabase->CommitTrans();
		setDayLine.Close();

		setDayLine.m_strFilter = _T("[Date] =");
		setDayLine.m_strFilter += strDate;
		setDayLine.Open();
		EXPECT_FALSE(setDayLine.IsEOF());
		setDayLine.Close();

		gl_pChinaMarket->DeleteDayLineBasicInfo(19900101);

		setDayLine2.m_strFilter = _T("[Date] =");
		setDayLine2.m_strFilter += strDate;
		setDayLine2.Open();
		EXPECT_TRUE(setDayLine2.IsEOF());
		setDayLine2.Close();
	}

	TEST_F(CChinaMarketTest, TestDeleteDayLineExtendInfo) {
		char buffer[20];
		CString strDate;

		CSetDayLineExtendInfo setDayLine, setDayLine2;
		CDayLinePtr pDayLine = make_shared<CDayLine>();

		pDayLine->SetStockSymbol(_T("600000.SS"));
		pDayLine->SetDate(19900101);

		_ltoa_s(19900101, buffer, 10);
		strDate = buffer;
		setDayLine.m_strFilter = _T("[ID] = 1");
		setDayLine.Open();
		setDayLine.m_pDatabase->BeginTrans();
		setDayLine.AddNew();
		setDayLine.m_Symbol = _T("600000.SS");
		setDayLine.m_Date = 19900101;
		setDayLine.Update();
		setDayLine.m_pDatabase->CommitTrans();
		setDayLine.Close();

		setDayLine.m_strFilter = _T("[Date] =");
		setDayLine.m_strFilter += strDate;
		setDayLine.Open();
		EXPECT_FALSE(setDayLine.IsEOF());
		setDayLine.Close();

		gl_pChinaMarket->DeleteDayLineExtendInfo(19900101);

		setDayLine2.m_strFilter = _T("[Date] =");
		setDayLine2.m_strFilter += strDate;
		setDayLine2.Open();
		EXPECT_TRUE(setDayLine2.IsEOF());
		setDayLine2.Close();
	}

	TEST_F(CChinaMarketTest, TestDeleteCurrentWeekLine) {
		CSetCurrentWeekLine setCurrentWeekLine, setCurrentWeekLine2;
		CWeekLinePtr pWeekLine = make_shared<CWeekLine>();

		pWeekLine->SetStockSymbol(_T("600000.SS"));
		pWeekLine->SetDate(GetCurrentMonday(20200101));
		setCurrentWeekLine.m_strFilter = _T("[ID] = 1");
		setCurrentWeekLine.Open();
		setCurrentWeekLine.m_pDatabase->BeginTrans();
		pWeekLine->AppendHistoryCandle(&setCurrentWeekLine);
		setCurrentWeekLine.m_pDatabase->CommitTrans();
		setCurrentWeekLine.Close();

		setCurrentWeekLine.m_strFilter = _T("");
		setCurrentWeekLine.Open();
		EXPECT_FALSE(setCurrentWeekLine.IsEOF());
		setCurrentWeekLine.Close();

		gl_pChinaMarket->DeleteCurrentWeekWeekLine();

		setCurrentWeekLine2.Open();
		EXPECT_TRUE(setCurrentWeekLine2.IsEOF());
		setCurrentWeekLine2.Close();
	}

	TEST_F(CChinaMarketTest, TestLoadDayLine) {
		CDayLineContainer dayLineContainer;
		long lDate = GetCurrentMonday(20200101);

		gl_pChinaMarket->LoadDayLine(dayLineContainer, lDate);

		CString strSQL;
		CString strDate;
		char  pch[30];
		CTime ctTime;
		CSetDayLineBasicInfo setDayLineBasicInfo;
		CSetDayLineExtendInfo setDayLineExtendInfo;
		CDayLinePtr pDayLine;
		long i = 0;

		sprintf_s(pch, _T("%08d"), lDate);
		strDate = pch;
		setDayLineBasicInfo.m_strSort = _T("[Symbol]");
		setDayLineBasicInfo.m_strFilter = _T("[Date] =");
		setDayLineBasicInfo.m_strFilter += strDate;
		setDayLineBasicInfo.Open();
		while (!setDayLineBasicInfo.IsEOF()) {
			pDayLine = dynamic_pointer_cast<CDayLine>(dayLineContainer.GetData(i++));
			EXPECT_STREQ(setDayLineBasicInfo.m_Symbol, pDayLine->GetStockSymbol());
			setDayLineBasicInfo.MoveNext();
		}
		EXPECT_EQ(i, dayLineContainer.GetDataSize());
		setDayLineBasicInfo.Close();
	}

	TEST_F(CChinaMarketTest, TestCreateStockCodeSet) {
		vector<CVirtualHistoryCandleExtendPtr> vData;
		CVirtualHistoryCandleExtendPtr pData = make_shared<CVirtualHistoryCandleExtend>();
		pData->SetStockSymbol(_T("600000.SS"));
		vData.resize(2);
		vData[0] = pData;
		pData = make_shared<CVirtualHistoryCandleExtend>();
		pData->SetStockSymbol(_T("600004.SS"));
		vData[1] = pData;

		set<CString> setStockCode;
		gl_pChinaMarket->CreateStockCodeSet(setStockCode, &vData);

		EXPECT_TRUE(setStockCode.find(_T("600000.SS")) != setStockCode.end());
		EXPECT_TRUE(setStockCode.find(_T("600004.SS")) != setStockCode.end());
		EXPECT_FALSE(setStockCode.find(_T("600001.SS")) != setStockCode.end());
	}

	TEST_F(CChinaMarketTest, TestUpdateStockCodeDB) {
		ASSERT_THAT(gl_pChinaMarket->IsUpdateStockCodeDB(), IsFalse()) << "此测试开始时，必须保证没有设置更新代码库的标识，否则会真正更新了测试代码库";

		CChinaStockPtr pStock = make_shared<CChinaStock>();
		pStock->SetSymbol(_T("SS.SS.SS"));
		pStock->SetTodayNewStock(true);
		pStock->SetUpdateProfileDB(true);
		EXPECT_FALSE(gl_pChinaMarket->IsStock(pStock->GetSymbol())); // 确保是一个新股票代码
		gl_pChinaMarket->AddStock(pStock);
		pStock = gl_pChinaMarket->GetStock(_T("000001.SS"));
		EXPECT_EQ(pStock->GetIPOStatus(), __STOCK_IPOED__);
		pStock->SetUpdateProfileDB(true);
		pStock->SetIPOStatus(__STOCK_DELISTED__);
		gl_pChinaMarket->UpdateStockCodeDB();

		CSetChinaStockSymbol setChinaStock;
		setChinaStock.m_strFilter = _T("[Symbol] = '000001.SS'");
		setChinaStock.Open();
		EXPECT_EQ(setChinaStock.m_IPOStatus, __STOCK_DELISTED__);
		setChinaStock.m_pDatabase->BeginTrans();
		setChinaStock.Edit();
		setChinaStock.m_IPOStatus = __STOCK_IPOED__;
		setChinaStock.Update();
		setChinaStock.m_pDatabase->CommitTrans();
		setChinaStock.Close();

		setChinaStock.m_strFilter = _T("[Symbol] = 'SS.SS.SS'");
		setChinaStock.Open();
		EXPECT_FALSE(setChinaStock.IsEOF());
		setChinaStock.m_pDatabase->BeginTrans();
		while (!setChinaStock.IsEOF()) {
			setChinaStock.Delete();
			setChinaStock.MoveNext();
		}
		setChinaStock.m_pDatabase->CommitTrans();
		setChinaStock.Close();

		pStock = gl_pChinaMarket->GetStock(_T("000001.SS"));
		pStock->SetIPOStatus(__STOCK_IPOED__); // 恢复原状
		pStock = gl_pChinaMarket->GetStock(_T("SS.SS.SS"));
		EXPECT_TRUE(pStock != nullptr);
		gl_pChinaMarket->DeleteStock(pStock); // 恢复原状

		EXPECT_THAT(gl_pChinaMarket->IsUpdateStockCodeDB(), IsFalse());

		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			pStock = gl_pChinaMarket->GetStock(i);
			pStock->SetUpdateProfileDB(false);
		}
	}

	TEST_F(CChinaMarketTest, TestAddStock) {
		CChinaStockPtr pStock = nullptr;
		EXPECT_FALSE(gl_pChinaMarket->AddStock(pStock));

		pStock = gl_pChinaMarket->GetStock(1);
		EXPECT_FALSE(gl_pChinaMarket->AddStock(pStock));

		pStock = make_shared<CChinaStock>();
		pStock->SetSymbol(_T("SS.SS.SS"));
		long lTotal = gl_pChinaMarket->GetTotalStock();
		EXPECT_FALSE(gl_pChinaMarket->IsStock(pStock->GetSymbol()));
		EXPECT_TRUE(gl_pChinaMarket->AddStock(pStock));
		EXPECT_EQ(gl_pChinaMarket->GetTotalStock(), lTotal + 1);

		gl_pChinaMarket->DeleteStock(pStock);
	}

	TEST_F(CChinaMarketTest, TestDeleteStock) {
		CChinaStockPtr pStock = nullptr;
		EXPECT_FALSE(gl_pChinaMarket->DeleteStock(pStock));

		pStock = make_shared<CChinaStock>();
		pStock->SetSymbol(_T("SS.SS.SS"));
		EXPECT_FALSE(gl_pChinaMarket->DeleteStock(pStock));

		gl_pChinaMarket->AddStock(pStock);
		EXPECT_TRUE(gl_pChinaMarket->DeleteStock(pStock));
	}

	TEST_F(CChinaMarketTest, TestTaskProcessTengxunRTData) {
		CWebRTDataPtr pRTData = make_shared<CWebRTData>();

		EXPECT_THAT(gl_WebRTDataContainer.GetTengxunDataSize(), Eq(0));

		pRTData->SetActive(true);
		pRTData->SetSymbol(_T("600000.SS"));
		pRTData->SetTotalValue(101010101010);
		pRTData->SetCurrentValue(2020202020202020);
		pRTData->SetHighLimit(10101010);
		pRTData->SetLowLimit(1010);

		gl_WebRTDataContainer.PushTengxunData(pRTData);

		pRTData = make_shared<CWebRTData>();

		pRTData->SetActive(false);
		pRTData->SetSymbol(_T("600001.SS"));
		pRTData->SetTotalValue(101010101010);
		pRTData->SetCurrentValue(2020202020202020);
		pRTData->SetHighLimit(10101010);
		pRTData->SetLowLimit(1010);

		gl_WebRTDataContainer.PushTengxunData(pRTData);

		CChinaStockPtr pStock = gl_pChinaMarket->GetStock(_T("600000.SS"));
		EXPECT_THAT(pStock->GetTotalValue(), 0);
		EXPECT_THAT(pStock->GetCurrentValue(), 0);
		EXPECT_THAT(pStock->GetHighLimit(), 0);
		EXPECT_THAT(pStock->GetLowLimit(), 0);

		pStock = gl_pChinaMarket->GetStock(_T("600001.SS"));
		EXPECT_THAT(pStock->GetTotalValue(), 0);
		EXPECT_THAT(pStock->GetCurrentValue(), 0);
		EXPECT_THAT(pStock->GetHighLimit(), 0);
		EXPECT_THAT(pStock->GetLowLimit(), 0);

		EXPECT_TRUE(gl_pChinaMarket->TaskProcessTengxunRTData());

		pStock = gl_pChinaMarket->GetStock(_T("600000.SS"));
		EXPECT_THAT(pStock->GetTotalValue(), 101010101010);
		EXPECT_THAT(pStock->GetCurrentValue(), 2020202020202020);
		EXPECT_THAT(pStock->GetHighLimit(), 10101010);
		EXPECT_THAT(pStock->GetLowLimit(), 1010);

		pStock = gl_pChinaMarket->GetStock(_T("600001.SS"));
		EXPECT_THAT(pStock->GetTotalValue(), 0);
		EXPECT_THAT(pStock->GetCurrentValue(), 0);
		EXPECT_THAT(pStock->GetHighLimit(), 0);
		EXPECT_THAT(pStock->GetLowLimit(), 0);

		EXPECT_THAT(gl_WebRTDataContainer.GetTengxunDataSize(), Eq(0));
	}

	TEST_F(CChinaMarketTest, TestTaskCheckDayLineDB) {
		CChinaStockPtr pStock;

		EXPECT_FALSE(gl_pChinaMarket->IsSaveDayLine());
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineNeedUpdate());
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineNeedProcess());
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineNeedSaving());
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineDBUpdated());

		EXPECT_FALSE(gl_pChinaMarket->TaskCheckDayLineDB()) << "IsSaveDayLine为假";

		gl_pChinaMarket->SetSaveDayLine(true);
		pStock = gl_pChinaMarket->GetStock(0);
		pStock->SetDayLineNeedSaving(true);
		pStock->SetDayLineDBUpdated(true);
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckDayLineDB()) << "IsDayLineNeedUpdate等皆为真";
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineDBUpdated());

		EXPECT_FALSE(gl_pChinaMarket->TaskCheckDayLineDB()) << "IsDayLineNeedUpdate和IsDayLineNeedSaving为真";
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineDBUpdated());

		pStock->SetDayLineNeedSaving(false);
		EXPECT_FALSE(gl_pChinaMarket->TaskCheckDayLineDB()) << "IsDayLineNeedUpdate为真";
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineDBUpdated());

		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			auto pstock = gl_pChinaMarket->GetStock(i);
			pstock->SetDayLineNeedUpdate(false);
		}
		EXPECT_TRUE(gl_pChinaMarket->TaskCheckDayLineDB()) << "条件满足了";
		EXPECT_FALSE(gl_pChinaMarket->IsSaveDayLine()) << "此标识被重置";
		EXPECT_FALSE(pStock->IsDayLineDBUpdated());
		EXPECT_FALSE(gl_pChinaMarket->IsDayLineDBUpdated());
		EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("中国市场日线历史数据更新完毕"));

		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			auto pstock = gl_pChinaMarket->GetStock(i);
			pstock->SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CChinaMarketTest, TestUpdateStockSection1) {
		EXPECT_FALSE(gl_pChinaMarket->IsStockSectionActive(1));
		EXPECT_TRUE(gl_pChinaMarket->UpdateStockSection(1));
		EXPECT_TRUE(gl_pChinaMarket->IsStockSectionActive(1));
		EXPECT_FALSE(gl_pChinaMarket->UpdateStockSection(1));

		gl_pChinaMarket->SetStockSectionActiveFlag(1, false);
	}

	TEST_F(CChinaMarketTest, TestUpdateStockSection2) {
		CString strShanghaiStock = _T("600601.SS");
		CString strShenzhenStock = _T("000001.SZ");
		long lIndex = 600;
		long lIndex2 = 1000;

		EXPECT_FALSE(gl_pChinaMarket->UpdateStockSection(_T("60061.SS"))); // 无效股票代码
		EXPECT_FALSE(gl_pChinaMarket->UpdateStockSection(_T("00001.SZ"))); // 无效股票代码

		EXPECT_TRUE(gl_pChinaMarket->IsStockSectionActive(lIndex)) << "装载预设数据库后如此";
		gl_pChinaMarket->SetStockSectionActiveFlag(lIndex, false);
		EXPECT_TRUE(gl_pChinaMarket->UpdateStockSection(strShanghaiStock));
		EXPECT_TRUE(gl_pChinaMarket->IsStockSectionActive(lIndex));
		EXPECT_FALSE(gl_pChinaMarket->UpdateStockSection(strShanghaiStock));

		EXPECT_TRUE(gl_pChinaMarket->IsStockSectionActive(lIndex2)) << "装载预设数据库后如此";
		gl_pChinaMarket->SetStockSectionActiveFlag(lIndex2, false);
		EXPECT_TRUE(gl_pChinaMarket->UpdateStockSection(strShenzhenStock));
		EXPECT_TRUE(gl_pChinaMarket->IsStockSectionActive(lIndex2));
		EXPECT_FALSE(gl_pChinaMarket->UpdateStockSection(strShenzhenStock));
	}

	TEST_F(CChinaMarketTest, TestAppendChoicedStock) { // 也测试UpdateChoicedStock和LoadChoicedStock
		CChinaStockPtr pStock = nullptr;

		pStock = gl_pChinaMarket->GetStock(_T("600601.SS"));
		pStock->SetChoiced(true);
		gl_pChinaMarket->AddChoicedStock(pStock);
		pStock = gl_pChinaMarket->GetStock(_T("000001.SZ"));
		pStock->SetChoiced(true);
		gl_pChinaMarket->AddChoicedStock(pStock);

		gl_pChinaMarket->AppendChoicedStockDB();

		pStock = gl_pChinaMarket->GetStock(_T("600601.SS"));
		gl_pChinaMarket->DeleteChoicedStock(pStock);
		pStock = gl_pChinaMarket->GetStock(_T("000001.SZ"));
		gl_pChinaMarket->DeleteChoicedStock(pStock);
		EXPECT_FALSE(gl_pChinaMarket->GetStock(_T("600601.SS"))->IsChoiced());
		EXPECT_FALSE(gl_pChinaMarket->GetStock(_T("000001.SZ"))->IsChoiced());

		gl_pChinaMarket->LoadChoicedStockDB();
		EXPECT_TRUE(gl_pChinaMarket->GetStock(_T("600601.SS"))->IsChoiced());
		EXPECT_TRUE(gl_pChinaMarket->GetStock(_T("000001.SZ"))->IsChoiced());

		pStock = gl_pChinaMarket->GetStock(_T("600601.SS"));
		gl_pChinaMarket->DeleteChoicedStock(pStock);
		pStock = gl_pChinaMarket->GetStock(_T("000001.SZ"));
		gl_pChinaMarket->DeleteChoicedStock(pStock);

		pStock = gl_pChinaMarket->GetStock(_T("600000.SS"));
		pStock->SetChoiced(true);
		gl_pChinaMarket->AddChoicedStock(pStock);
		pStock = gl_pChinaMarket->GetStock(_T("000002.SZ"));
		pStock->SetChoiced(true);
		gl_pChinaMarket->AddChoicedStock(pStock);

		gl_pChinaMarket->UpdateChoicedStockDB();

		// 恢复原状
		pStock = gl_pChinaMarket->GetStock(_T("600000.SS"));
		gl_pChinaMarket->DeleteChoicedStock(pStock);
		pStock = gl_pChinaMarket->GetStock(_T("000002.SZ"));
		gl_pChinaMarket->DeleteChoicedStock(pStock);

		CSetChinaChoicedStock setChinaChoicedStock;
		setChinaChoicedStock.Open();
		setChinaChoicedStock.m_pDatabase->BeginTrans();
		setChinaChoicedStock.MoveLast();
		EXPECT_STREQ(setChinaChoicedStock.m_Symbol, _T("000002.SZ"));
		setChinaChoicedStock.Delete();
		setChinaChoicedStock.MovePrev();
		EXPECT_STREQ(setChinaChoicedStock.m_Symbol, _T("600000.SS"));
		setChinaChoicedStock.Delete();
		setChinaChoicedStock.m_pDatabase->CommitTrans();
		setChinaChoicedStock.Close();
	}

	TEST_F(CChinaMarketTest, TestLoadTodayTempDB) {
		CSetDayLineTodaySaved setDayLineTemp;
		CChinaStockPtr pStock;

		pStock = gl_pChinaMarket->GetStock(_T("000001.SZ"));
		pStock->SetUnknownVolume(0);
		pStock->SetTransactionNumber(0);

		gl_pChinaMarket->LoadTodayTempDB(20210715); // 测试库today表中的日期为20210715。

		EXPECT_EQ(pStock->GetUnknownVolume(), 10517770);
		EXPECT_EQ(pStock->GetTransactionNumber(), 2183);
	}
}