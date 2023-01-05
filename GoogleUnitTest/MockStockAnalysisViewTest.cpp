#include"pch.h"

#include"GeneralCheck.h"

#include"ChinaMarket.h"

#include"MockFireBirdView.h"

#include"GeneralCheck.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace FireBirdTest {
	class CMockFireBirdViewTest : public ::testing::Test {
	public:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();

			s_pFireBirdView = new CMockFireBirdView;
			EXPECT_EQ(s_pFireBirdView->GetClientSize().Height(), 0);
			EXPECT_EQ(s_pFireBirdView->GetClientSize().Width(), 0);
			EXPECT_FALSE(s_pFireBirdView->IsShowRS());
			EXPECT_FALSE(s_pFireBirdView->IsShow3DaysRS());
			EXPECT_FALSE(s_pFireBirdView->IsShow5DaysRS());
			EXPECT_TRUE(s_pFireBirdView->IsShow10DaysRS());
			EXPECT_TRUE(s_pFireBirdView->IsShow30DaysRS());
			EXPECT_FALSE(s_pFireBirdView->IsShow60DaysRS());
			EXPECT_FALSE(s_pFireBirdView->IsShow120DaysRS());
			EXPECT_TRUE(s_pFireBirdView->IsShowRSInIndex());
			EXPECT_FALSE(s_pFireBirdView->IsShowRSInLinear());
		}

		void TearDown(void) override {
			gl_pChinaMarket->ResetCurrentStock();
			gl_pChinaMarket->SetCurrentStockChanged(false);
			delete s_pFireBirdView;

			GeneralCheck();
		}

	public:
		CMockFireBirdView* s_pFireBirdView;
	};

	TEST_F(CMockFireBirdViewTest, TestGetCurrentShowType) {
		EXPECT_EQ(s_pFireBirdView->GetCurrentShowType(), 1) << _T("初始状态为显示日线（1）");
		s_pFireBirdView->SetCurrentShowType(2);
		EXPECT_EQ(s_pFireBirdView->GetCurrentShowType(), 2);
		s_pFireBirdView->SetCurrentShowType(1);
	}

	TEST_F(CMockFireBirdViewTest, TestShowOrdinaryBuySell) {
		CDC DC;
		CChinaStockPtr pStock = gl_pChinaMarket->GetStock(1);
		CRect rectArea(0, 0, 300, 300);

		pStock->SetVolume(10000);
		EXPECT_CALL(*s_pFireBirdView, SysCallFillSolidRect(&DC, _, _))
			.Times(14);

		s_pFireBirdView->ShowOrdinaryBuySell(&DC, pStock, rectArea);
	}

	TEST_F(CMockFireBirdViewTest, TestShowCanceledBuySell) {
		CDC DC;
		CChinaStockPtr pStock = gl_pChinaMarket->GetStock(1);
		CRect rectArea(0, 0, 300, 300);

		pStock->SetCanceledBuyVolume(10000);
		EXPECT_CALL(*s_pFireBirdView, SysCallFillSolidRect(&DC, _, _))
			.Times(14);

		s_pFireBirdView->ShowCanceledBuySell(&DC, pStock, rectArea);
	}

	TEST_F(CMockFireBirdViewTest, TestShowCurrentRS) {
		CDC DC;
		vector<double> vRS{5, 10, 20, 30};
		CRect rect(0, 0, 100, 100);
		s_pFireBirdView->SetClientSize(rect);
		EXPECT_CALL(*s_pFireBirdView, SysCallMoveTo(&DC, 99, 85))
			.Times(1);
		EXPECT_CALL(*s_pFireBirdView, SysCallLineTo(&DC, _, _))
			.Times(2);
		s_pFireBirdView->ShowCurrentRS(&DC, vRS);
	}

	TEST_F(CMockFireBirdViewTest, TestRSLineTo) {
		CDC DC;
		CRect rect(0, 0, 100, 100);
		s_pFireBirdView->SetClientSize(rect);
		EXPECT_CALL(*s_pFireBirdView, SysCallLineTo(&DC, 69, 75))
			.Times(1)
			.WillOnce(Return(TRUE));
		EXPECT_TRUE(s_pFireBirdView->RSLineTo(&DC, 10, 50.0, 100));

		EXPECT_CALL(*s_pFireBirdView, SysCallLineTo(&DC, -21, 75))
			.Times(1)
			.WillOnce(Return(TRUE));
		EXPECT_FALSE(s_pFireBirdView->RSLineTo(&DC, 40, 50.0, 100));

		EXPECT_CALL(*s_pFireBirdView, SysCallLineTo(&DC, 39, 75))
			.Times(1)
			.WillOnce(Return(TRUE));
		EXPECT_FALSE(s_pFireBirdView->RSLineTo(&DC, 20, 50.0, 50));
	}

	TEST_F(CMockFireBirdViewTest, TestOnSize) {
		EXPECT_CALL(*s_pFireBirdView, SysCallOnSize(1, 100, 200))
			.Times(1);
		s_pFireBirdView->OnSize(1, 100, 200);
		CRect rect = s_pFireBirdView->GetClientSize();
		EXPECT_EQ(rect.Height(), 200);
		EXPECT_EQ(rect.Width(), 100);
	}

	TEST_F(CMockFireBirdViewTest, TestOnUpdateShowRs3) {
		CCmdUI cmdUI;
		EXPECT_FALSE(s_pFireBirdView->IsShow3DaysRS());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 0))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRs3(&cmdUI);
		s_pFireBirdView->OnShowRs3();
		EXPECT_TRUE(s_pFireBirdView->IsShow3DaysRS());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 1))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRs3(&cmdUI);
		s_pFireBirdView->OnShowRs3();
		EXPECT_FALSE(s_pFireBirdView->IsShow3DaysRS());
	}

	TEST_F(CMockFireBirdViewTest, TestOnUpdateShowRs5) {
		CCmdUI cmdUI;
		EXPECT_FALSE(s_pFireBirdView->IsShow5DaysRS());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 0))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRs5(&cmdUI);
		s_pFireBirdView->OnShowRs5();
		EXPECT_TRUE(s_pFireBirdView->IsShow5DaysRS());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 1))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRs5(&cmdUI);
		s_pFireBirdView->OnShowRs5();
		EXPECT_FALSE(s_pFireBirdView->IsShow5DaysRS());
	}

	TEST_F(CMockFireBirdViewTest, TestOnUpdateShowRs10) {
		CCmdUI cmdUI;
		EXPECT_TRUE(s_pFireBirdView->IsShow10DaysRS());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 1))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRs10(&cmdUI);
		s_pFireBirdView->OnShowRs10();
		EXPECT_FALSE(s_pFireBirdView->IsShow10DaysRS());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 0))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRs10(&cmdUI);
		s_pFireBirdView->OnShowRs10();
		EXPECT_TRUE(s_pFireBirdView->IsShow10DaysRS());
	}

	TEST_F(CMockFireBirdViewTest, TestOnUpdateShowRs30) {
		CCmdUI cmdUI;
		EXPECT_TRUE(s_pFireBirdView->IsShow30DaysRS());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 1))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRs30(&cmdUI);
		s_pFireBirdView->OnShowRs30();
		EXPECT_FALSE(s_pFireBirdView->IsShow30DaysRS());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 0))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRs30(&cmdUI);
		s_pFireBirdView->OnShowRs30();
		EXPECT_TRUE(s_pFireBirdView->IsShow30DaysRS());
	}

	TEST_F(CMockFireBirdViewTest, TestOnUpdateShowRs60) {
		CCmdUI cmdUI;
		EXPECT_FALSE(s_pFireBirdView->IsShow60DaysRS());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 0))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRs60(&cmdUI);
		s_pFireBirdView->OnShowRs60();
		EXPECT_TRUE(s_pFireBirdView->IsShow60DaysRS());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 1))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRs60(&cmdUI);
		s_pFireBirdView->OnShowRs60();
		EXPECT_FALSE(s_pFireBirdView->IsShow60DaysRS());
	}

	TEST_F(CMockFireBirdViewTest, TestOnUpdateShowRs120) {
		CCmdUI cmdUI;
		EXPECT_FALSE(s_pFireBirdView->IsShow120DaysRS());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 0))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRs120(&cmdUI);
		s_pFireBirdView->OnShowRs120();
		EXPECT_TRUE(s_pFireBirdView->IsShow120DaysRS());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 1))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRs120(&cmdUI);
		s_pFireBirdView->OnShowRs120();
		EXPECT_FALSE(s_pFireBirdView->IsShow120DaysRS());
	}

	TEST_F(CMockFireBirdViewTest, TestOnUpdateShowRSInLogarithm) {
		CCmdUI cmdUI;
		gl_pChinaMarket->ResetCurrentStock();
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUIEnable(_, false))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRsInLogarithm(&cmdUI);

		gl_pChinaMarket->SetCurrentStock(_T("600000.SS"));
		EXPECT_TRUE(gl_pChinaMarket->GetCurrentStock() != nullptr);
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUIEnable(_, true))
			.Times(1);
		EXPECT_FALSE(s_pFireBirdView->IsShowRSInLogarithm());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 0))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRsInLogarithm(&cmdUI);

		s_pFireBirdView->OnShowRsInLogarithm();
		EXPECT_TRUE(s_pFireBirdView->IsShowRSInLogarithm());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUIEnable(_, true))
			.Times(1);
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 1))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRsInLogarithm(&cmdUI);
		s_pFireBirdView->OnShowRsInLinear();

		EXPECT_FALSE(s_pFireBirdView->IsShowRSInLogarithm());
		EXPECT_FALSE(s_pFireBirdView->IsShowRSInIndex());
		EXPECT_TRUE(s_pFireBirdView->IsShowRSInLinear());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUIEnable(_, true))
			.Times(1);
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 1))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRsInLinear(&cmdUI);
		s_pFireBirdView->OnShowRsIndex();
	}

	TEST_F(CMockFireBirdViewTest, TestOnUpdateShowRSInLinear) {
		CCmdUI cmdUI;
		gl_pChinaMarket->ResetCurrentStock();
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUIEnable(_, false))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRsInLinear(&cmdUI);
		gl_pChinaMarket->SetCurrentStock(_T("600000.SS"));
		EXPECT_TRUE(gl_pChinaMarket->GetCurrentStock() != nullptr);
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUIEnable(_, true))
			.Times(1);
		EXPECT_FALSE(s_pFireBirdView->IsShowRSInLinear());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 0))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRsInLinear(&cmdUI);
		s_pFireBirdView->OnShowRsInLinear();
		EXPECT_TRUE(s_pFireBirdView->IsShowRSInLinear());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUIEnable(_, true))
			.Times(1);
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 1))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRsInLinear(&cmdUI);
		s_pFireBirdView->OnShowRsIndex();
		EXPECT_FALSE(s_pFireBirdView->IsShowRSInLinear());
	}

	TEST_F(CMockFireBirdViewTest, TestOnUpdateShowRSIndex) {
		CCmdUI cmdUI;
		gl_pChinaMarket->ResetCurrentStock();
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUIEnable(_, false))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRsIndex(&cmdUI);
		gl_pChinaMarket->SetCurrentStock(_T("600000.SS"));
		s_pFireBirdView->SetShowRSOption(2);
		EXPECT_TRUE(gl_pChinaMarket->GetCurrentStock() != nullptr);
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUIEnable(_, true))
			.Times(1);
		EXPECT_FALSE(s_pFireBirdView->IsShowRSInIndex());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 0))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRsIndex(&cmdUI);
		s_pFireBirdView->OnShowRsIndex();
		EXPECT_TRUE(s_pFireBirdView->IsShowRSInIndex());
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUIEnable(_, true))
			.Times(1);
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 1))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRsIndex(&cmdUI);
		s_pFireBirdView->OnShowRsInLinear();
		EXPECT_FALSE(s_pFireBirdView->IsShowRSInIndex());
	}

	TEST_F(CMockFireBirdViewTest, TestOnShowDayLine) {
		s_pFireBirdView->SetCurrentShowType(2);
		s_pFireBirdView->OnShowDayLine();
		EXPECT_EQ(s_pFireBirdView->GetCurrentShowType(), 1);
	}

	TEST_F(CMockFireBirdViewTest, TestOnShowRealTime) {
		EXPECT_EQ(s_pFireBirdView->GetCurrentShowType(), 1);
		s_pFireBirdView->OnShowRealTime();
		EXPECT_EQ(s_pFireBirdView->GetCurrentShowType(), 2);

		s_pFireBirdView->SetCurrentShowType(1);
	}

	TEST_F(CMockFireBirdViewTest, TestOnUpdateShowDayLine) {
		CCmdUI cmdUI;
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 1))
			.Times(1);
		s_pFireBirdView->OnUpdateShowDayLine(&cmdUI);
		s_pFireBirdView->SetCurrentShowType(2);
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 0))
			.Times(1);
		s_pFireBirdView->OnUpdateShowDayLine(&cmdUI);

		s_pFireBirdView->SetCurrentShowType(1);
	}

	TEST_F(CMockFireBirdViewTest, TestOnUpdateShowRealTime) {
		CCmdUI cmdUI;
		s_pFireBirdView->SetCurrentShowType(_SHOW_DAY_LINE_DATA_);
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 0))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRealTime(&cmdUI);
		s_pFireBirdView->SetCurrentShowType(_SHOW_REAL_TIME_DATA_);
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 1))
			.Times(1);
		s_pFireBirdView->OnUpdateShowRealTime(&cmdUI);
	}

	TEST_F(CMockFireBirdViewTest, TestOnUpdateShowWeekLine) {
		CCmdUI cmdUI;
		s_pFireBirdView->SetCurrentShowType(_SHOW_DAY_LINE_DATA_);
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 0))
			.Times(1);
		s_pFireBirdView->OnUpdateShowWeekLine(&cmdUI);
		s_pFireBirdView->SetCurrentShowType(_SHOW_WEEK_LINE_DATA_);
		EXPECT_CALL(*s_pFireBirdView, SysCallCmdUISetCheck(_, 1))
			.Times(1);
		s_pFireBirdView->OnUpdateShowWeekLine(&cmdUI);
	}

	TEST_F(CMockFireBirdViewTest, TestOnShow) {
		CDC dc;
		gl_pChinaMarket->SetCurrentStock(_T("600000.SS"));
		gl_pChinaMarket->GetCurrentStock()->SetDayLineLoaded(true);
		s_pFireBirdView->SetCurrentShowType(1);
		EXPECT_CALL(*s_pFireBirdView, SysCallGetClientRect(_))
			.Times(1);
		EXPECT_CALL(*s_pFireBirdView, ShowStockHistoryDataLine(_))
			.Times(1);
		EXPECT_CALL(*s_pFireBirdView, SysCallBitBlt(&dc, 0, 0, _, _, _, 0, 0, SRCCOPY))
			.Times(1)
			.WillOnce(Return(TRUE));
		s_pFireBirdView->Show(&dc);
		EXPECT_CALL(*s_pFireBirdView, SysCallGetClientRect(_))
			.Times(1);
		EXPECT_CALL(*s_pFireBirdView, ShowRealtimeData(_))
			.Times(1);
		EXPECT_CALL(*s_pFireBirdView, SysCallBitBlt(&dc, 0, 0, _, _, _, 0, 0, SRCCOPY))
			.Times(1)
			.WillOnce(Return(TRUE));
		s_pFireBirdView->SetCurrentShowType(2);
		s_pFireBirdView->Show(&dc);

		gl_pChinaMarket->GetCurrentStock()->SetDayLineLoaded(false);
	}

	TEST_F(CMockFireBirdViewTest, TestEnlarge) {
		vector<double> vData{0, 100, 50, 49, 51};

		s_pFireBirdView->ZoomIn(vData, 50, 10);
		EXPECT_EQ(vData.at(0), 0);
		EXPECT_EQ(vData.at(1), 100);
		EXPECT_EQ(vData.at(2), 50);
		EXPECT_EQ(vData.at(3), 40);
		EXPECT_EQ(vData.at(4), 60);
	}
}
