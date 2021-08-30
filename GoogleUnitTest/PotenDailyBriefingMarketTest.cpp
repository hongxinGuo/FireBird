#include"pch.h"

#include"globedef.h"
#include"ChinaStock.h"
#include"PotenDailyBriefingMarket.h"
#include"WebInquirer.h"
#include"MockPotenDailyBriefingWebInquiry.h"

#include"GeneralCheck.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CVirtualMarket无法生成实例，故而其函数的测试放在这里。
namespace StockAnalysisTest {
	static CMockPotenDailyBriefingWebInquiryPtr s_pMockPotenDailyBriefingWebInquiry;

	class CPotenDailyBriefingMarketTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();

			ASSERT_THAT(gl_pPotenDailyBriefingWebInquiry, NotNull());
			s_pMockPotenDailyBriefingWebInquiry = static_pointer_cast<CMockPotenDailyBriefingWebInquiry>(gl_pPotenDailyBriefingWebInquiry);

			EXPECT_FALSE(s_pMockPotenDailyBriefingWebInquiry->IsReadingWebData());
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();

			while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
			EXPECT_FALSE(s_pMockPotenDailyBriefingWebInquiry->IsReadingWebData());

			s_pMockPotenDailyBriefingWebInquiry = nullptr;
		}

		virtual void SetUp(void) override {
			EXPECT_FALSE(s_pMockPotenDailyBriefingWebInquiry->IsReadingWebData());
			EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsDatabaseLoaded());
			EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetCurrentInquiringDate(), 20180411);
			EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsPermitResetMarket());
			EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsReadyToRun()) << _T("Poten目前停止更新，故而暂缓提取其数据\n");
			EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
			gl_pPotenDailyBriefingMarket->ClearDatabase();
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
			EXPECT_FALSE(s_pMockPotenDailyBriefingWebInquiry->IsReadingWebData());
			gl_pPotenDailyBriefingMarket->SetDatabaseLoaded(false);
			gl_pPotenDailyBriefingMarket->SetPermitResetMarket(true);
			gl_pPotenDailyBriefingMarket->SetReadyToRun(true);
			gl_pPotenDailyBriefingMarket->SetResetMarket(true);
			gl_pPotenDailyBriefingMarket->SetCurrentInquiringDate(20180411);
			gl_pPotenDailyBriefingMarket->ClearDatabase();
			gl_pPotenDailyBriefingMarket->Reset();
			gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(false);
			GeneralCheck();
		}
	};

	TEST_F(CPotenDailyBriefingMarketTest, TestInitialize) {
		EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsDatabaseLoaded());
		EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetCurrentInquiringDate(), 20180411);
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsPermitResetMarket());
	}

	TEST_F(CPotenDailyBriefingMarketTest, TestTaskResetMarket) {
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsPermitResetMarket());
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
		gl_pPotenDailyBriefingMarket->TaskResetMarket(5959);
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
		gl_pPotenDailyBriefingMarket->TaskResetMarket(13001);
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsPermitResetMarket());
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
		gl_pPotenDailyBriefingMarket->TaskResetMarket(10000);
		EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsPermitResetMarket());
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
		gl_pPotenDailyBriefingMarket->SetPermitResetMarket(true); // 重置此标识
		gl_pPotenDailyBriefingMarket->TaskResetMarket(13000);
		EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsPermitResetMarket());
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());

		gl_pPotenDailyBriefingMarket->SetPermitResetMarket(true); // 重置此标识
	}

	TEST_F(CPotenDailyBriefingMarketTest, TestIsResetMarket) {
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
		gl_pPotenDailyBriefingMarket->SetResetMarket(false);
		EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsResetMarket());
		gl_pPotenDailyBriefingMarket->SetResetMarket(true);
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
	}

	TEST_F(CPotenDailyBriefingMarketTest, TestCheckTodayDataUpdated) {
		EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());
		gl_pPotenDailyBriefingMarket->SetCurrentInquiringDate(gl_pPotenDailyBriefingMarket->GetNextDay(gl_pPotenDailyBriefingMarket->GetFormatedMarketDate()));
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->TaskCheckTodayDataUpdated());
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());
		gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(false);
		gl_pPotenDailyBriefingMarket->SetCurrentInquiringDate(gl_pPotenDailyBriefingMarket->GetFormatedMarketDate());
		if (gl_pPotenDailyBriefingMarket->IsWorkingDay()) {
			EXPECT_TRUE(gl_pPotenDailyBriefingMarket->TaskCheckTodayDataUpdated());
			EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());
		}
		else {
			EXPECT_TRUE(gl_pPotenDailyBriefingMarket->TaskCheckTodayDataUpdated()) << _T("周末无数据");
			EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated()) << _T("周末无数据");
		}
		gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(false);
		gl_pPotenDailyBriefingMarket->SetCurrentInquiringDate(gl_pPotenDailyBriefingMarket->GetPrevDay(gl_pPotenDailyBriefingMarket->GetFormatedMarketDate()));
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->TaskCheckTodayDataUpdated());
		EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());

		while (gl_systemMessage.GetInformationDequeSize() > 0)	gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CPotenDailyBriefingMarketTest, TestTaskLoadDatabase) {
		EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsDatabaseLoaded());
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->TaskLoadDataBase());
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsDatabaseLoaded());
		EXPECT_FALSE(gl_pPotenDailyBriefingMarket->TaskLoadDataBase());
	}

	TEST_F(CPotenDailyBriefingMarketTest, TestTaskInquiringData) {
		gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(true);
		EXPECT_FALSE(gl_WebInquirer.IsReadingPotenDailyBriefing()) << _T("此标识无法设置，故而永远为假");
		gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(false);
		gl_pPotenDailyBriefingMarket->SetDatabaseLoaded(false);
		EXPECT_CALL(*s_pMockPotenDailyBriefingWebInquiry, StartReadingThread).Times(0);
		EXPECT_FALSE(gl_pPotenDailyBriefingMarket->TaskInquiringData());
		gl_pPotenDailyBriefingMarket->SetDatabaseLoaded(true);
		EXPECT_FALSE(gl_WebInquirer.IsReadingPotenDailyBriefing()) << _T("此标识无法设置，故而永远为假");
		EXPECT_CALL(*s_pMockPotenDailyBriefingWebInquiry, StartReadingThread).Times(1);
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->TaskInquiringData());
		EXPECT_TRUE(s_pMockPotenDailyBriefingWebInquiry->IsReadingWebData());

		s_pMockPotenDailyBriefingWebInquiry->SetReadingWebData(false);
	}

	TEST_F(CPotenDailyBriefingMarketTest, TestSetCurrentInquiringDate) {
		EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetCurrentInquiringDate(), 20180411);
		gl_pPotenDailyBriefingMarket->SetCurrentInquiringDate(20190101);
		EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetCurrentInquiringDate(), 20190101);
	}

	TEST_F(CPotenDailyBriefingMarketTest, TestResetMarket) {
		gl_pPotenDailyBriefingMarket->ResetMarket();
		CString str = gl_systemMessage.PopInformationMessage();
		CString strLeft = str.Left(29);
		EXPECT_STREQ(strLeft, _T("重置poten.com于美东标准时间："));
	}

	TEST_F(CPotenDailyBriefingMarketTest, TestLoadDatabase) {
		CSetPotenDailyBriefing setPoten;

		EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetDatabaseSize(), 0);
		gl_pPotenDailyBriefingMarket->LoadDatabase();
		EXPECT_GT(gl_pPotenDailyBriefingMarket->GetDatabaseSize(), 0);
		setPoten.Open();
		setPoten.MoveLast();
		EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetCurrentInquiringDate(), setPoten.m_Date);
		setPoten.Close();
	}

	TEST_F(CPotenDailyBriefingMarketTest, TestSchedulingTaskPerSecond) {
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->SchedulingTaskPerSecond(1, 10000));
	}

	TEST_F(CPotenDailyBriefingMarketTest, TestSchedulingTaskPerMinute) {
		gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(false);
		EXPECT_FALSE(s_pMockPotenDailyBriefingWebInquiry->IsReadingWebData());
		EXPECT_CALL(*s_pMockPotenDailyBriefingWebInquiry, StartReadingThread).Times(1);
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->SchedulingTaskPerMinute(60, 10000));
		EXPECT_TRUE(s_pMockPotenDailyBriefingWebInquiry->IsReadingWebData()) << _T("预先设置的此标识，由于Mock类没有重置之，故而还保持着设置状态\n");
		s_pMockPotenDailyBriefingWebInquiry->SetReadingWebData(false);
		EXPECT_CALL(*s_pMockPotenDailyBriefingWebInquiry, StartReadingThread).Times(1);
		EXPECT_FALSE(gl_pPotenDailyBriefingMarket->SchedulingTaskPerMinute(59, 11000));
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->SchedulingTaskPerMinute(1, 11000));
		EXPECT_TRUE(s_pMockPotenDailyBriefingWebInquiry->IsReadingWebData()) << _T("预先设置的此标识，由于Mock类没有重置之，故而还保持着设置状态\n");
		s_pMockPotenDailyBriefingWebInquiry->SetReadingWebData(false);
	}

	TEST_F(CPotenDailyBriefingMarketTest, TestSchedulingTaskPerHour) {
		gl_pPotenDailyBriefingMarket->SetDatabaseLoaded(true);
		gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(false);
		EXPECT_FALSE(s_pMockPotenDailyBriefingWebInquiry->IsReadingWebData());
		gl_pPotenDailyBriefingMarket->SetCurrentInquiringDate(1);
		EXPECT_CALL(*s_pMockPotenDailyBriefingWebInquiry, StartReadingThread).Times(0);
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->SchedulingTaskPerHour(3600, 10000));
		EXPECT_FALSE(s_pMockPotenDailyBriefingWebInquiry->IsReadingWebData()) << _T("预先设置的此标识，由于Mock类没有重置之，故而还保持着设置状态\n");
		gl_pPotenDailyBriefingMarket->SetCurrentInquiringDate(gl_pPotenDailyBriefingMarket->GetFormatedMarketDate());
		EXPECT_CALL(*s_pMockPotenDailyBriefingWebInquiry, StartReadingThread).Times(1);
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->SchedulingTaskPerHour(3600, 10000));
		EXPECT_TRUE(s_pMockPotenDailyBriefingWebInquiry->IsReadingWebData()) << _T("预先设置的此标识，由于Mock类没有重置之，故而还保持着设置状态\n");
		s_pMockPotenDailyBriefingWebInquiry->SetReadingWebData(false);
		EXPECT_CALL(*s_pMockPotenDailyBriefingWebInquiry, StartReadingThread).Times(1);
		EXPECT_FALSE(gl_pPotenDailyBriefingMarket->SchedulingTaskPerHour(3599, 11000));
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->SchedulingTaskPerHour(1, 11000));
		EXPECT_TRUE(s_pMockPotenDailyBriefingWebInquiry->IsReadingWebData()) << _T("预先设置的此标识，由于Mock类没有重置之，故而还保持着设置状态\n");
		s_pMockPotenDailyBriefingWebInquiry->SetReadingWebData(false);
	}

	TEST_F(CPotenDailyBriefingMarketTest, TestChoiceNextInquiringDate) {
		gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDate(20190101, true);
		gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDate(20190102, false);
		gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDate(20190103, true);
		gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDate(20190104, false);
		gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDate(20190105, true); // 星期六，总为真
		gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDate(20190106, true); // 星期日，总为真
		gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDate(20190107, true);
		gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDate(20190108, false);
		gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDate(20190109, true);
		gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDate(20190110, true);
		gl_pPotenDailyBriefingMarket->SetCurrentInquiringDate(20190101);
		gl_pPotenDailyBriefingMarket->ChoiceNextInquiringDate();
		EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetCurrentInquiringDate(), 20190102);
		gl_pPotenDailyBriefingMarket->ChoiceNextInquiringDate();
		EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetCurrentInquiringDate(), 20190104);
		gl_pPotenDailyBriefingMarket->ChoiceNextInquiringDate();
		EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetCurrentInquiringDate(), 20190108);
	}

	TEST_F(CPotenDailyBriefingMarketTest, TestIsTodayDataUpdated) {
		EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());
		gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(true);
		EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());
		gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(false);
		EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());
	}
}