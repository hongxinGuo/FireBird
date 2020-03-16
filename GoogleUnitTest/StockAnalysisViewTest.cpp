#include"stdafx.h"
#include"pch.h"

#include"globedef.h"
#include"ChinaMarket.h"

#include"MockStockAnalysisView.h"
using namespace Testing;
using namespace ::testing;

namespace StockAnalysisTest {
  CMockStockAnalysisView* s_pStockAnalysisView;
  class CStockAnalysisViewTest : public ::testing::Test {
  public:
    static void SetUpTestSuite(void) {
      EXPECT_FALSE(gl_fNormalMode);
      EXPECT_TRUE(gl_fTestMode);
      s_pStockAnalysisView = new CMockStockAnalysisView;
      EXPECT_EQ(s_pStockAnalysisView->GetClientSize().Height(), 0);
      EXPECT_EQ(s_pStockAnalysisView->GetClientSize().Width(), 0);
      EXPECT_FALSE(s_pStockAnalysisView->IsShowRS());
      EXPECT_FALSE(s_pStockAnalysisView->IsShow3DayRS());
      EXPECT_TRUE(s_pStockAnalysisView->IsShow5DayRS());
      EXPECT_TRUE(s_pStockAnalysisView->IsShow10DayRS());
      EXPECT_TRUE(s_pStockAnalysisView->IsShow30DayRS());
      EXPECT_TRUE(s_pStockAnalysisView->IsShow60DayRS());
      EXPECT_TRUE(s_pStockAnalysisView->IsShow120DayRS());
    }

    static void TearDownTestSuite(void) {
      delete s_pStockAnalysisView;
      EXPECT_FALSE(gl_fNormalMode);
      EXPECT_TRUE(gl_fTestMode);
    }
    virtual void SetUp(void) override {
    }
    virtual void TearDown(void) override {
    }
  public:
  };

  TEST_F(CStockAnalysisViewTest, TestOnSize) {
    EXPECT_CALL(*s_pStockAnalysisView, SysCallOnSize(1, 100, 200))
      .Times(1);
    s_pStockAnalysisView->OnSize(1, 100, 200);
    CRect rect = s_pStockAnalysisView->GetClientSize();
    EXPECT_EQ(rect.Height(), 200);
    EXPECT_EQ(rect.Width(), 100);
  }

  TEST_F(CStockAnalysisViewTest, TestOnUpdateShowRs3) {
    CCmdUI cmdUI;
    EXPECT_FALSE(s_pStockAnalysisView->IsShow3DayRS());
    EXPECT_CALL(*s_pStockAnalysisView, SysCallCmdUISetCheck(_, 0))
      .Times(1);
    s_pStockAnalysisView->OnUpdateShowRs3(&cmdUI);
    s_pStockAnalysisView->OnShowRs3();
    EXPECT_TRUE(s_pStockAnalysisView->IsShow3DayRS());
    EXPECT_CALL(*s_pStockAnalysisView, SysCallCmdUISetCheck(_, 1))
      .Times(1);
    s_pStockAnalysisView->OnUpdateShowRs3(&cmdUI);
    s_pStockAnalysisView->OnShowRs3();
    EXPECT_FALSE(s_pStockAnalysisView->IsShow3DayRS());
  }

  TEST_F(CStockAnalysisViewTest, TestOnUpdateShowRs5) {
    CCmdUI cmdUI;
    EXPECT_TRUE(s_pStockAnalysisView->IsShow5DayRS());
    EXPECT_CALL(*s_pStockAnalysisView, SysCallCmdUISetCheck(_, 1))
      .Times(1);
    s_pStockAnalysisView->OnUpdateShowRs5(&cmdUI);
    s_pStockAnalysisView->OnShowRs5();
    EXPECT_FALSE(s_pStockAnalysisView->IsShow5DayRS());
    EXPECT_CALL(*s_pStockAnalysisView, SysCallCmdUISetCheck(_, 0))
      .Times(1);
    s_pStockAnalysisView->OnUpdateShowRs5(&cmdUI);
    s_pStockAnalysisView->OnShowRs5();
    EXPECT_TRUE(s_pStockAnalysisView->IsShow5DayRS());
  }

  TEST_F(CStockAnalysisViewTest, TestOnUpdateShowRs10) {
    CCmdUI cmdUI;
    EXPECT_TRUE(s_pStockAnalysisView->IsShow10DayRS());
    EXPECT_CALL(*s_pStockAnalysisView, SysCallCmdUISetCheck(_, 1))
      .Times(1);
    s_pStockAnalysisView->OnUpdateShowRs10(&cmdUI);
    s_pStockAnalysisView->OnShowRs10();
    EXPECT_FALSE(s_pStockAnalysisView->IsShow10DayRS());
    EXPECT_CALL(*s_pStockAnalysisView, SysCallCmdUISetCheck(_, 0))
      .Times(1);
    s_pStockAnalysisView->OnUpdateShowRs10(&cmdUI);
    s_pStockAnalysisView->OnShowRs10();
    EXPECT_TRUE(s_pStockAnalysisView->IsShow10DayRS());
  }

  TEST_F(CStockAnalysisViewTest, TestOnUpdateShowRs30) {
    CCmdUI cmdUI;
    EXPECT_TRUE(s_pStockAnalysisView->IsShow30DayRS());
    EXPECT_CALL(*s_pStockAnalysisView, SysCallCmdUISetCheck(_, 1))
      .Times(1);
    s_pStockAnalysisView->OnUpdateShowRs30(&cmdUI);
    s_pStockAnalysisView->OnShowRs30();
    EXPECT_FALSE(s_pStockAnalysisView->IsShow30DayRS());
    EXPECT_CALL(*s_pStockAnalysisView, SysCallCmdUISetCheck(_, 0))
      .Times(1);
    s_pStockAnalysisView->OnUpdateShowRs30(&cmdUI);
    s_pStockAnalysisView->OnShowRs30();
    EXPECT_TRUE(s_pStockAnalysisView->IsShow30DayRS());
  }

  TEST_F(CStockAnalysisViewTest, TestOnUpdateShowRs60) {
    CCmdUI cmdUI;
    EXPECT_TRUE(s_pStockAnalysisView->IsShow60DayRS());
    EXPECT_CALL(*s_pStockAnalysisView, SysCallCmdUISetCheck(_, 1))
      .Times(1);
    s_pStockAnalysisView->OnUpdateShowRs60(&cmdUI);
    s_pStockAnalysisView->OnShowRs60();
    EXPECT_FALSE(s_pStockAnalysisView->IsShow60DayRS());
    EXPECT_CALL(*s_pStockAnalysisView, SysCallCmdUISetCheck(_, 0))
      .Times(1);
    s_pStockAnalysisView->OnUpdateShowRs60(&cmdUI);
    s_pStockAnalysisView->OnShowRs60();
    EXPECT_TRUE(s_pStockAnalysisView->IsShow60DayRS());
  }

  TEST_F(CStockAnalysisViewTest, TestOnUpdateShowRs120) {
    CCmdUI cmdUI;
    EXPECT_TRUE(s_pStockAnalysisView->IsShow120DayRS());
    EXPECT_CALL(*s_pStockAnalysisView, SysCallCmdUISetCheck(_, 1))
      .Times(1);
    s_pStockAnalysisView->OnUpdateShowRs120(&cmdUI);
    s_pStockAnalysisView->OnShowRs120();
    EXPECT_FALSE(s_pStockAnalysisView->IsShow120DayRS());
    EXPECT_CALL(*s_pStockAnalysisView, SysCallCmdUISetCheck(_, 0))
      .Times(1);
    s_pStockAnalysisView->OnUpdateShowRs120(&cmdUI);
    s_pStockAnalysisView->OnShowRs120();
    EXPECT_TRUE(s_pStockAnalysisView->IsShow120DayRS());
  }
}