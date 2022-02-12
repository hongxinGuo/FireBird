#include"pch.h"

#include"globedef.h"
#include"SystemMessage.h"
#include"ChinaStock.h"
#include"CrweberIndexMarket.h"
#include"WebInquirer.h"

#include"MockCrweberIndexWebInquiry.h"

#include"GeneralCheck.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CVirtualMarket无法生成实例，故而其函数的测试放在这里。
namespace StockAnalysisTest {
	static CMockCrweberIndexWebInquiryPtr s_pMockCrweberIndexWebInquiryPtr;

	class CCrweberIndexMarketTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			ASSERT_THAT(gl_pCrweberIndexWebInquiry, NotNull());
			//在测试中，gl_pCrweberIndexWebInquiry的实际类型是CMockCrweberIndexWebInquiryPtr，在此使用其实际类型进行测试。
			s_pMockCrweberIndexWebInquiryPtr = static_pointer_cast<CMockCrweberIndexWebInquiry>(gl_pCrweberIndexWebInquiry);
		}

		static void TearDownTestSuite(void) {
			s_pMockCrweberIndexWebInquiryPtr = nullptr;
		}

		virtual void SetUp(void) override {
			gl_pCrweberIndexMarket->SetDatabaseLoaded(false);
			gl_pCrweberIndexMarket->SetPermitResetMarket(true);
			gl_pCrweberIndexMarket->SetReadyToRun(true);
			gl_pCrweberIndexMarket->SetResetMarket(true);
			gl_pCrweberIndexMarket->SetNewestUpdateDate(0);

			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
			gl_pCrweberIndexMarket->SetDatabaseLoaded(false);
			gl_pCrweberIndexMarket->SetPermitResetMarket(true);
			gl_pCrweberIndexMarket->SetReadyToRun(true);
			gl_pCrweberIndexMarket->SetResetMarket(true);
			gl_pCrweberIndexMarket->SetNewestUpdateDate(0);

			GeneralCheck();
		}
	};

	TEST_F(CCrweberIndexMarketTest, TestInitialize) {
		EXPECT_FALSE(gl_pCrweberIndexMarket->IsDatabaseLoaded());
		EXPECT_EQ(gl_pCrweberIndexMarket->GetNewestUpdateDate(), 0);
		EXPECT_TRUE(gl_pCrweberIndexMarket->IsPermitResetMarket());
		EXPECT_EQ(gl_pCrweberIndexMarket->GetMarketTimeZone(), 0);
	}

	TEST_F(CCrweberIndexMarketTest, TestResetMarket) {
		size_t l = gl_systemMessage.GetInformationDequeSize();
		CString strCmp = _T("重置Crweber.com于格林威治标准时间：");
		gl_pCrweberIndexMarket->ResetMarket();
		EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), l + 1);
		CString str = gl_systemMessage.PopInformationMessage();
		//EXPECT_THAT(str, StartsWith(_T("重置Crweber.com于格林威治标准时间：")));
		CString strLeft = str.Left(35);
		EXPECT_STREQ(strLeft, strCmp);
	}

	TEST_F(CCrweberIndexMarketTest, TestTaskResetMarket) {
		EXPECT_TRUE(gl_pCrweberIndexMarket->IsPermitResetMarket());
		EXPECT_TRUE(gl_pCrweberIndexMarket->IsResetMarket());
		gl_pCrweberIndexMarket->SetResetMarket(false);
		gl_pCrweberIndexMarket->TaskResetMarket(69999);
		EXPECT_TRUE(gl_pCrweberIndexMarket->IsPermitResetMarket());
		EXPECT_FALSE(gl_pCrweberIndexMarket->IsResetMarket());
		gl_pCrweberIndexMarket->TaskResetMarket(71000);
		EXPECT_TRUE(gl_pCrweberIndexMarket->IsPermitResetMarket());
		EXPECT_FALSE(gl_pCrweberIndexMarket->IsResetMarket());
		gl_pCrweberIndexMarket->TaskResetMarket(70000);
		EXPECT_FALSE(gl_pCrweberIndexMarket->IsPermitResetMarket());
		EXPECT_TRUE(gl_pCrweberIndexMarket->IsResetMarket());
		gl_pCrweberIndexMarket->SetPermitResetMarket(true);
	}

	TEST_F(CCrweberIndexMarketTest, TestSetNewestDatabaseDate) {
		gl_pCrweberIndexMarket->Reset();
		gl_pCrweberIndexMarket->GetNewestDatabaseDateFromDB();

		CSetCrweberIndex setCrweberIndex;
		long lDate = 0;

		setCrweberIndex.m_strSort = _T("[Date]");
		setCrweberIndex.Open();
		if (!setCrweberIndex.IsEOF()) {
			setCrweberIndex.MoveLast();
			lDate = setCrweberIndex.m_Date;
		}
		setCrweberIndex.Close();

		EXPECT_EQ(gl_pCrweberIndexMarket->GetNewestDatabaseDate(), lDate);
	}

	TEST_F(CCrweberIndexMarketTest, TestSetNewestUpdateDate) {
		EXPECT_EQ(gl_pCrweberIndexMarket->GetNewestUpdateDate(), 0);
		gl_pCrweberIndexMarket->SetNewestUpdateDate(20201220);
		EXPECT_EQ(gl_pCrweberIndexMarket->GetNewestUpdateDate(), 20201220);
	}

	TEST_F(CCrweberIndexMarketTest, TestSetDatabaseLoaded) {
		EXPECT_FALSE(gl_pCrweberIndexMarket->IsDatabaseLoaded());
		gl_pCrweberIndexMarket->SetDatabaseLoaded(true);
		EXPECT_TRUE(gl_pCrweberIndexMarket->IsDatabaseLoaded());
		gl_pCrweberIndexMarket->SetDatabaseLoaded(false);
		EXPECT_FALSE(gl_pCrweberIndexMarket->IsDatabaseLoaded());
	}

	TEST_F(CCrweberIndexMarketTest, TestSchedulingTaskPer1Hour) {
		gl_pCrweberIndexMarket->SetDatabaseLoaded(true);
		EXPECT_CALL(*s_pMockCrweberIndexWebInquiryPtr, StartReadingThread).Times(1); // 当数据库装载后此工作线程会执行一次
		EXPECT_TRUE(gl_pCrweberIndexMarket->SchedulingTaskPer1Hour(90000));
		EXPECT_TRUE(gl_pCrweberIndexMarket->IsDatabaseLoaded());
		//EXPECT_TRUE(gl_pCrweberIndexWebInquiry->IsReadingWebData()) << _T("调用Mock类时没有重置此标识\n");
		s_pMockCrweberIndexWebInquiryPtr->SetReadingWebData(false);
	}

	TEST_F(CCrweberIndexMarketTest, TestTaskProcessWebRTDataGetFromCrweberdotcom) {
		EXPECT_TRUE(gl_pCrweberIndexMarket->TaskProcessWebRTDataGetFromCrweberdotcom()) << "目前没有数据，只是简单运行一下";
	}
}