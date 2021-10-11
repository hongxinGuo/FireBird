///////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʹ��Mock�����ChinaMarket�ĺ�����λ�ڴ��ļ��С�
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"

#include"Thread.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"

#include"WebInquirer.h"

#include"MockNeteaseDayLineWebInquiry.h"
#include"MockChinaMarket.h"

#include"GeneralCheck.h"

using namespace std;
using namespace testing;
#include<memory>

namespace StockAnalysisTest {
	extern CMockChinaMarketPtr gl_pMockChinaMarket;

	static CMockNeteaseDayLineWebInquiryPtr s_pMockNeteaseDayLineWebInquiry;
	static CMockNeteaseDayLineWebInquiryPtr s_pMockNeteaseDayLineWebInquiry2;
	class CMockChinaMarketTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

			EXPECT_TRUE(gl_pMockChinaMarket != nullptr) << "��Mock������EnvironmentSetUp.h������";
			gl_pMockChinaMarket->SetTodayStockProcessed(false);
			EXPECT_FALSE(gl_pMockChinaMarket->IsDayLineNeedSaving());
			EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
			EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateStockCodeDB());

			ASSERT_THAT(gl_pNeteaseDayLineWebInquiry, NotNull());
			s_pMockNeteaseDayLineWebInquiry = static_pointer_cast<CMockNeteaseDayLineWebInquiry>(gl_pNeteaseDayLineWebInquiry);
			ASSERT_THAT(gl_pNeteaseDayLineWebInquiry2, NotNull());
			s_pMockNeteaseDayLineWebInquiry2 = static_pointer_cast<CMockNeteaseDayLineWebInquiry>(gl_pNeteaseDayLineWebInquiry2);

			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedUpdateNumber(), gl_pMockChinaMarket->GetTotalStock());
			EXPECT_FALSE(gl_pMockChinaMarket->IsDayLineNeedSaving());
			EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);

			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
			EXPECT_THAT(gl_pChinaMarket->IsUpdateStockCodeDB(), IsFalse());

			s_pMockNeteaseDayLineWebInquiry = nullptr;
			s_pMockNeteaseDayLineWebInquiry2 = nullptr;

			GeneralCheck();
		}

		virtual void SetUp(void) override {
			//EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateStockCodeDB());
			EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
			EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedUpdateNumber(), gl_pMockChinaMarket->GetTotalStock());

			gl_pMockChinaMarket->SetTodayStockProcessed(false);
			gl_pMockChinaMarket->SetRSEndDate(19900101);

			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
			EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedUpdateNumber(), gl_pMockChinaMarket->GetTotalStock());
			EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateStockCodeDB());
			EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
			CChinaStockPtr pStock;
			if (gl_pMockChinaMarket->GetDayLineNeedSaveNumber() > 0) {
				for (int i = 0; i < gl_pMockChinaMarket->GetTotalStock(); i++) {
					pStock = gl_pMockChinaMarket->GetStock(i);
					if (pStock->IsDayLineNeedSaving()) {
						EXPECT_STREQ(pStock->GetSymbol(), _T(""));
					}
				}
			}
			gl_pMockChinaMarket->SetTodayStockProcessed(false);
			gl_pMockChinaMarket->SetRSEndDate(19900101);
			gl_pMockChinaMarket->SetUpdateOptionDB(false);
			gl_ThreadStatus.SetSavingTempData(false);

			GeneralCheck();
		}
	};

	TEST_F(CMockChinaMarketTest, TestTaskSaveDayLineData1) {
		CChinaStockPtr pStock = gl_pMockChinaMarket->GetStock(_T("600666.SS"));
		EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
		EXPECT_FALSE(pStock->IsDayLineNeedSaving());
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadSaveDayLineBasicInfoOfStock(_))
			.Times(0);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskSaveDayLineData());
	}

	TEST_F(CMockChinaMarketTest, TestTaskSaveDayLineData2) {
		CChinaStockPtr pStock = gl_pMockChinaMarket->GetStock(_T("600668.SS"));

		EXPECT_FALSE(pStock->IsDayLineNeedSaving());
		EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
		pStock->SetDayLineNeedSaving(true);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadSaveDayLineBasicInfoOfStock(_))
			.Times(0);
		EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 1);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskSaveDayLineData());
		EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
		pStock = gl_pMockChinaMarket->GetStock(_T("600668.SS"));
		EXPECT_FALSE(pStock->IsDayLineNeedSaving()) << "����ִ����������˱�ʶ";

		EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 1);
		gl_systemMessage.PopDayLineInfoMessage();
	}

	TEST_F(CMockChinaMarketTest, TestTaskSaveDayLineData3) {
		CChinaStockPtr pStock = gl_pMockChinaMarket->GetStock(_T("600282.SS"));
		long lDate = pStock->GetDayLineEndDate();

		EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
		EXPECT_FALSE(pStock->IsDayLineNeedSaving());
		pStock->SetDayLineNeedSaving(true);
		CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(19900101);
		pStock->SetDayLineEndDate(20000101);
		pStock->StoreDayLine(pDayLine);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadSaveDayLineBasicInfoOfStock(_))
			.Times(0);
		EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 1);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskSaveDayLineData());
		EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);

		pStock = gl_pMockChinaMarket->GetStock(_T("600282.SS"));
		EXPECT_FALSE(pStock->IsDayLineNeedSaving()) << "����ִ����������˱�ʶ";

		pStock->SetDayLineEndDate(lDate);
		pStock->UnloadDayLine();
	}

	TEST_F(CMockChinaMarketTest, TestTaskSaveDayLineData4) {
		CChinaStockPtr pStock = gl_pMockChinaMarket->GetStock(_T("600608.SS"));
		long lDate = pStock->GetDayLineEndDate();

		EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
		EXPECT_FALSE(gl_pMockChinaMarket->IsDayLineNeedSaving());
		EXPECT_FALSE(pStock->IsDayLineNeedSaving());
		pStock->SetDayLineNeedSaving(true);
		CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(19900101);
		pStock->SetDayLineEndDate(20000101);
		pStock->StoreDayLine(pDayLine);
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20210101);
		pStock->StoreDayLine(pDayLine);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadSaveDayLineBasicInfoOfStock(_))
			.Times(1);
		EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 1);
		EXPECT_TRUE(gl_pMockChinaMarket->TaskSaveDayLineData());
		EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);

		pStock = gl_pMockChinaMarket->GetStock(_T("600608.SS"));
		EXPECT_FALSE(pStock->IsDayLineNeedSaving()) << "����ִ����������˱�ʶ";

		pStock->SetDayLineEndDate(lDate);
		pStock->UnloadDayLine();
	}

	TEST_F(CMockChinaMarketTest, TestUpdateTempRTData) {
		gl_ThreadStatus.SetSavingTempData(true);
		EXPECT_FALSE(gl_pMockChinaMarket->UpdateTempRTData());
		gl_ThreadStatus.SetSavingTempData(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadSaveTempRTData())
			.Times(1);
		EXPECT_TRUE(gl_pMockChinaMarket->UpdateTempRTData());
		EXPECT_TRUE(gl_ThreadStatus.IsSavingTempData());
		gl_ThreadStatus.SetSavingTempData(true);
	}

	TEST_F(CMockChinaMarketTest, TestTaskLoadCurrentStockDayLine1) {
		gl_pMockChinaMarket->ResetCurrentStock();
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadLoadDayLine(_))
			.Times(0);
		EXPECT_TRUE(gl_pMockChinaMarket->TaskLoadCurrentStockHistoryData());
	}

	TEST_F(CMockChinaMarketTest, TestTaskLoadCurrentStockDayLine2) {
		gl_pMockChinaMarket->ResetCurrentStock();
		gl_pMockChinaMarket->SetCurrentStock(_T("600000.SS"));
		gl_pMockChinaMarket->GetCurrentStock()->SetDayLineLoaded(true);
		gl_pMockChinaMarket->GetCurrentStock()->SetWeekLineLoaded(true);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadLoadDayLine(_))
			.Times(0);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadLoadWeekLine(_))
			.Times(0);
		EXPECT_TRUE(gl_pMockChinaMarket->TaskLoadCurrentStockHistoryData());
		EXPECT_TRUE(gl_pMockChinaMarket->GetCurrentStock()->IsDayLineLoaded());
		EXPECT_TRUE(gl_pMockChinaMarket->GetCurrentStock()->IsWeekLineLoaded());
	}

	TEST_F(CMockChinaMarketTest, TestTaskLoadCurrentStockDayLine3) {
		gl_pMockChinaMarket->ResetCurrentStock();
		gl_pMockChinaMarket->SetCurrentStock(_T("600000.SS"));
		gl_pMockChinaMarket->GetCurrentStock()->SetDayLineLoaded(false);
		gl_pMockChinaMarket->GetCurrentStock()->SetWeekLineLoaded(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadLoadDayLine(_))
			.Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadLoadWeekLine(_))
			.Times(1);
		EXPECT_TRUE(gl_pMockChinaMarket->TaskLoadCurrentStockHistoryData());
		EXPECT_TRUE(gl_pMockChinaMarket->GetCurrentStock()->IsDayLineLoaded());
		EXPECT_TRUE(gl_pMockChinaMarket->GetCurrentStock()->IsWeekLineLoaded());
	}

	TEST_F(CMockChinaMarketTest, TestTaskUpdateStockCodeDB) {
		EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateStockCodeDB());
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadUpdateStockCodeDB())
			.Times(0);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskUpdateStockCodeDB());

		gl_pMockChinaMarket->GetStock(1)->SetUpdateProfileDB(true);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadUpdateStockCodeDB())
			.Times(1);
		EXPECT_TRUE(gl_pMockChinaMarket->TaskUpdateStockCodeDB());

		gl_pMockChinaMarket->GetStock(1)->SetUpdateProfileDB(false);
	}

	TEST_F(CMockChinaMarketTest, TestTaskUpdateOptionDB) {
		gl_pMockChinaMarket->SetUpdateOptionDB(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadUpdateOptionDB())
			.Times(0);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskUpdateOptionDB());
		gl_pMockChinaMarket->SetUpdateOptionDB(true);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadUpdateOptionDB())
			.Times(1);
		EXPECT_TRUE(gl_pMockChinaMarket->TaskUpdateOptionDB());
		EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateOptionDB());
	}

	TEST_F(CMockChinaMarketTest, TestTaskUpdateChoicedStockDB) {
		gl_pMockChinaMarket->SetUpdateChoicedStockDB(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadAppendChoicedStockDB())
			.Times(0);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskUpdateChoicedStockDB());
		gl_pMockChinaMarket->SetUpdateChoicedStockDB(true);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadAppendChoicedStockDB())
			.Times(1);
		EXPECT_TRUE(gl_pMockChinaMarket->TaskUpdateChoicedStockDB());
		EXPECT_TRUE(gl_pMockChinaMarket->IsUpdateChoicedStockDB()) << _T("��ѡ��Ʊ���ı�ʶ���ú󣬾���Ҳ�����ã������ܹ���֤ϵͳ�˳�ʱ�ٴθ������ݿ�");
	}

	TEST_F(CMockChinaMarketTest, TestTaskSaveChoicedRTData) {
		gl_pMockChinaMarket->SetSystemReady(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadSaveChoicedRTData())
			.Times(0);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskSaveChoicedRTData());
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetRecordRTData(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadSaveChoicedRTData())
			.Times(0);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskSaveChoicedRTData());
		gl_pMockChinaMarket->SetRecordRTData(true);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadSaveChoicedRTData())
			.Times(1);
		EXPECT_TRUE(gl_pMockChinaMarket->TaskSaveChoicedRTData());
		gl_pMockChinaMarket->SetRecordRTData(false);
		gl_pMockChinaMarket->SetSystemReady(false);
	}

	TEST_F(CMockChinaMarketTest, TestTaskSaveTempDataIntoDB) {
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetMarketOpened(true);
		gl_ThreadStatus.SetCalculatingRTData(false);
		gl_ThreadStatus.SetSavingTempData(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadSaveTempRTData())
			.Times(1);
		gl_pMockChinaMarket->TaskSaveTempDataIntoDB(93001);
		gl_ThreadStatus.SetSavingTempData(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadSaveTempRTData())
			.Times(1);
		gl_pMockChinaMarket->TaskSaveTempDataIntoDB(113559);
		gl_ThreadStatus.SetSavingTempData(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadSaveTempRTData())
			.Times(1);
		gl_pMockChinaMarket->TaskSaveTempDataIntoDB(130001);
		gl_ThreadStatus.SetSavingTempData(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadSaveTempRTData())
			.Times(1);
		gl_pMockChinaMarket->TaskSaveTempDataIntoDB(150559);

		EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 4);
		for (int i = 0; i < 4; i++) {
			gl_systemMessage.PopDayLineInfoMessage();
		}
	}

	TEST_F(CMockChinaMarketTest, TestTaskChoice10RSStrong2StockSet) {
		tm tm_;
		tm_.tm_wday = 1; // ����һ
		gl_pMockChinaMarket->__TEST_SetMarketTM(tm_);
		gl_pMockChinaMarket->SetSystemReady(false);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151200));
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151201));
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetChoiced10RSStrong2StockSet(true);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151200));
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151201));
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetChoiced10RSStrong2StockSet(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadChoice10RSStrong2StockSet)
			.Times(1);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151200));
		EXPECT_TRUE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151201));
		EXPECT_TRUE(gl_pMockChinaMarket->IsChoiced10RSStrong2StockSet());

		tm_.tm_wday = 0; // ������
		gl_pMockChinaMarket->__TEST_SetMarketTM(tm_);
		gl_pMockChinaMarket->SetTodayStockProcessed(false);
		gl_pMockChinaMarket->SetChoiced10RSStrong2StockSet(false);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151201));
		EXPECT_FALSE(gl_pMockChinaMarket->IsChoiced10RSStrong2StockSet()) << _T("��Ϣ�ղ�����");
	}

	TEST_F(CMockChinaMarketTest, TestTaskChoice10RSStrong1StockSet) {
		tm tm_;
		tm_.tm_wday = 1; // ����һ
		gl_pMockChinaMarket->__TEST_SetMarketTM(tm_);
		gl_pMockChinaMarket->SetSystemReady(false);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151100));
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151101));
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetChoiced10RSStrong1StockSet(true);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151100));
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151101));
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetChoiced10RSStrong1StockSet(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadChoice10RSStrong1StockSet)
			.Times(1);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151100));
		EXPECT_TRUE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151101));
		EXPECT_TRUE(gl_pMockChinaMarket->IsChoiced10RSStrong1StockSet());

		tm_.tm_wday = 0; // ������
		gl_pMockChinaMarket->__TEST_SetMarketTM(tm_);
		gl_pMockChinaMarket->SetTodayStockProcessed(false);
		gl_pMockChinaMarket->SetChoiced10RSStrong1StockSet(false);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151101));
		EXPECT_FALSE(gl_pMockChinaMarket->IsChoiced10RSStrong1StockSet()) << _T("��Ϣ�ղ�����");
	}

	TEST_F(CMockChinaMarketTest, TestTaskChoice10RSStrongStockSet) {
		tm tm_;
		tm_.tm_wday = 1; // ����һ
		gl_pMockChinaMarket->__TEST_SetMarketTM(tm_);
		gl_pMockChinaMarket->SetSystemReady(false);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151000));
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetChoiced10RSStrongStockSet(true);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151000));
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetChoiced10RSStrongStockSet(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadChoice10RSStrongStockSet)
			.Times(1);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151000));
		EXPECT_TRUE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
		EXPECT_TRUE(gl_pMockChinaMarket->IsChoiced10RSStrongStockSet());

		tm_.tm_wday = 0; // ������
		gl_pMockChinaMarket->__TEST_SetMarketTM(tm_);
		gl_pMockChinaMarket->SetTodayStockProcessed(false);
		gl_pMockChinaMarket->SetChoiced10RSStrongStockSet(false);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
		EXPECT_FALSE(gl_pMockChinaMarket->IsChoiced10RSStrongStockSet()) << _T("��Ϣ�ղ�����");
	}

	TEST_F(CMockChinaMarketTest, TestTaskGetNeteaseDayLineFromWeb) {
		EXPECT_TRUE(gl_pMockChinaMarket->IsDayLineNeedUpdate());
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->__TEST_SetFormatedMarketTime(92559);
		EXPECT_CALL(*s_pMockNeteaseDayLineWebInquiry, StartReadingThread()).Times(0);
		EXPECT_CALL(*s_pMockNeteaseDayLineWebInquiry2, StartReadingThread()).Times(0);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskGetNeteaseDayLineFromWeb());

		gl_pMockChinaMarket->__TEST_SetFormatedMarketTime(92600);
		EXPECT_CALL(*s_pMockNeteaseDayLineWebInquiry, PrepareNextInquiringStr())
			.Times(1)
			.WillOnce(Return(true))
			.RetiresOnSaturation();
		EXPECT_CALL(*s_pMockNeteaseDayLineWebInquiry2, PrepareNextInquiringStr())
			.Times(1)
			.WillOnce(Return(true))
			.RetiresOnSaturation();
		EXPECT_CALL(*s_pMockNeteaseDayLineWebInquiry, StartReadingThread()).Times(1);
		EXPECT_CALL(*s_pMockNeteaseDayLineWebInquiry2, StartReadingThread()).Times(1);
		EXPECT_TRUE(gl_pMockChinaMarket->TaskGetNeteaseDayLineFromWeb());
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
		}
		s_pMockNeteaseDayLineWebInquiry2->SetReadingWebData(false);
		s_pMockNeteaseDayLineWebInquiry->SetReadingWebData(false);
	}

	TEST_F(CMockChinaMarketTest, TestProcessTodayStock) {
		tm tm_;
		tm_.tm_wday = 1; // ����һ
		gl_pMockChinaMarket->__TEST_SetMarketTM(tm_);
		gl_pMockChinaMarket->SetSystemReady(false);
		gl_pMockChinaMarket->SetTodayStockProcessed(false);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150359));
		EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150400));
		EXPECT_FALSE(gl_pMockChinaMarket->IsTodayStockProcessed());
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetTodayStockProcessed(true);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150359));
		EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150400));
		EXPECT_TRUE(gl_pMockChinaMarket->IsTodayStockProcessed());
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_pMockChinaMarket->SetTodayStockProcessed(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadProcessTodayStock)
			.Times(1);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150359));
		EXPECT_TRUE(gl_pMockChinaMarket->TaskProcessTodayStock(150400));
		EXPECT_FALSE(gl_pMockChinaMarket->IsTodayStockProcessed()) << _T("�˱�־�ɹ����̸߳������ã��ʶ���ʱ��δ����");

		tm_.tm_wday = 0; // ������
		gl_pMockChinaMarket->__TEST_SetMarketTM(tm_);
		gl_pMockChinaMarket->SetTodayStockProcessed(false);
		EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150400));
		EXPECT_FALSE(gl_pMockChinaMarket->IsTodayStockProcessed()) << _T("��Ϣ�ղ�����");
	}

	TEST_F(CMockChinaMarketTest, TestThreadProcessTodayStock) {
		gl_pMockChinaMarket->SetTodayStockProcessed(false);
		gl_pMockChinaMarket->CalculateTime();
		gl_pMockChinaMarket->SetNewestTransactionTime(gl_pMockChinaMarket->GetUTCTime());
		long lDate = FormatToDate(gl_pMockChinaMarket->GetNewestTransactionTime());
		gl_pMockChinaMarket->__TEST_SetFormatedMarketTime(130000); // �����г�ʱ��ΪС��150400��
		EXPECT_CALL(*gl_pMockChinaMarket, BuildDayLine(lDate))
			.Times(1)
			.WillOnce(Return(4000));
		EXPECT_CALL(*gl_pMockChinaMarket, BuildDayLineRS(lDate))
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineOfCurrentWeek)
			.Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineRS(GetCurrentMonday(lDate)))
			.Times(1)
			.WillOnce(Return(true));
		gl_pMockChinaMarket->SetSystemReady(true);
		EXPECT_EQ(ThreadProcessTodayStock(gl_pMockChinaMarket.get()), (UINT)14);
		// �г�ʱ��С��150400ʱ
		EXPECT_EQ(gl_pMockChinaMarket->GetRSEndDate(), 19900101) << "û��ִ���޸��������ǿ���յĶ���";
		EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateStockCodeDB());
		EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateOptionDB());
		EXPECT_FALSE(gl_pMockChinaMarket->IsTodayStockProcessed());

		gl_pMockChinaMarket->__TEST_SetFormatedMarketTime(150500); // �����г�ʱ��Ϊ����150400��
		EXPECT_CALL(*gl_pMockChinaMarket, BuildDayLine(lDate))
			.Times(1)
			.WillOnce(Return(4000));
		EXPECT_CALL(*gl_pMockChinaMarket, BuildDayLineRS(lDate))
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineOfCurrentWeek)
			.Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineRS(GetCurrentMonday(lDate)))
			.Times(1)
			.WillOnce(Return(true));
		gl_pMockChinaMarket->SetSystemReady(true);
		EXPECT_EQ(ThreadProcessTodayStock(gl_pMockChinaMarket.get()), (UINT)14);
		// �г�ʱ�����150400ʱ
		EXPECT_EQ(gl_pMockChinaMarket->GetRSEndDate(), lDate);
		EXPECT_TRUE(gl_pMockChinaMarket->IsUpdateOptionDB());
		EXPECT_TRUE(gl_pMockChinaMarket->IsTodayStockProcessed());
	}

	TEST_F(CMockChinaMarketTest, TestThreadCalculateDayLineRS) {
		gl_pMockChinaMarket->CalculateTime();
		time_t tStart = gl_pMockChinaMarket->GetUTCTime() - 3600 * 24 * 6; // ��һ��ǰ��ʼ����
		tm _tm;
		gmtime_s(&_tm, &tStart);
		long lStartDate = (_tm.tm_year + 1900) * 10000 + (_tm.tm_mon + 1) * 100 + _tm.tm_mday;
		gl_fExitingCalculatingRS = true; // �м䱻���
		gl_ThreadStatus.SetCalculatingDayLineRS(true);
		gl_pMockChinaMarket->SetRSEndDate(0);
		gl_pMockChinaMarket->SetUpdateOptionDB(false);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadBuildDayLineRSOfDate(_))
			.Times(5)
			.WillRepeatedly(Return(true));
		EXPECT_EQ(ThreadBuildDayLineRS(gl_pMockChinaMarket.get(), lStartDate), (UINT)11);
		EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateOptionDB()) << _T("����Ϻ����ô˱�ʶ");
		EXPECT_EQ(gl_pMockChinaMarket->GetRSEndDate(), 0);
		EXPECT_FALSE(gl_fExitingCalculatingRS);
		EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());

		tStart = gl_pMockChinaMarket->GetUTCTime() - 3600 * 24 * 6; // ��һ��ǰ��ʼ����
		gmtime_s(&_tm, &tStart);
		lStartDate = (_tm.tm_year + 1900) * 10000 + (_tm.tm_mon + 1) * 100 + _tm.tm_mday;
		gl_fExitingCalculatingRS = false;
		gl_ThreadStatus.SetCalculatingDayLineRS(true);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadBuildDayLineRSOfDate(_))
			.Times(5);
		EXPECT_EQ(ThreadBuildDayLineRS(gl_pMockChinaMarket.get(), lStartDate), (UINT)11);
		EXPECT_TRUE(gl_pMockChinaMarket->IsUpdateOptionDB());
		EXPECT_EQ(gl_pMockChinaMarket->GetRSEndDate(), gl_pMockChinaMarket->GetFormatedMarketDate());
		EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());

		EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 2);
		gl_systemMessage.PopInformationMessage();
		gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CMockChinaMarketTest, TestThreadUpdateOptionDB) {
		EXPECT_CALL(*gl_pMockChinaMarket, UpdateOptionDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateOptionDB(gl_pMockChinaMarket.get()), (UINT)20);
	}

	TEST_F(CMockChinaMarketTest, TestThreadUpdateStockCodeDB) {
		ASSERT_THAT(gl_pMockChinaMarket->IsUpdateStockCodeDB(), IsFalse()) << "�˲��Կ�ʼʱ�����뱣֤û�����ø��´����ı�ʶ����������������˲��Դ����";

		EXPECT_CALL(*gl_pMockChinaMarket, UpdateStockCodeDB)
			.Times(1);
		gl_pMockChinaMarket->SetSystemReady(true);
		gl_ThreadStatus.SetCalculatingRTData(false);
		EXPECT_EQ(ThreadUpdateStockCodeDB(gl_pMockChinaMarket.get()), (UINT)18);
	}

	TEST_F(CMockChinaMarketTest, TestThreadUpdateChoicedStockDB) {
		EXPECT_CALL(*gl_pMockChinaMarket, AppendChoicedStockDB)
			.Times(1);
		gl_pMockChinaMarket->SetSystemReady(true);
		EXPECT_EQ(ThreadAppendChoicedStockDB(gl_pMockChinaMarket.get()), (UINT)22);
	}

	TEST_F(CMockChinaMarketTest, TestThreadSaveTempRTData) {
		EXPECT_CALL(*gl_pMockChinaMarket, UpdateTodayTempDB)
			.Times(1);
		gl_pMockChinaMarket->SetSystemReady(true);
		EXPECT_EQ(ThreadSaveTempRTData(gl_pMockChinaMarket.get()), (UINT)13);
	}

	TEST_F(CMockChinaMarketTest, TestThreadSaveRTData) {
		EXPECT_CALL(*gl_pMockChinaMarket, SaveRTData)
			.Times(1);
		gl_pMockChinaMarket->SetSystemReady(true);
		EXPECT_EQ(ThreadSaveRTData(gl_pMockChinaMarket.get()), (UINT)19);
	}

	TEST_F(CMockChinaMarketTest, TestThreadMaintainDayLineDatabase) {
		EXPECT_EQ(ThreadMaintainDayLineDataBase(), (UINT)17) << "�˹����߳�Ŀǰû��ʵ�ʶ���";
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLineOfCurrentWeek) {
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineOfCurrentWeek)
			.Times(1);
		EXPECT_EQ(ThreadBuildWeekLineOfCurrentWeek(gl_pMockChinaMarket.get()), (UINT)32);
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildCurrentWeekWeekLineTable) {
		EXPECT_CALL(*gl_pMockChinaMarket, DeleteCurrentWeekWeekLine).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, BuildCurrentWeekWeekLineTable).Times(1);
		EXPECT_EQ(ThreadBuildCurrentWeekWeekLineTable(gl_pMockChinaMarket.get()), (UINT)33);
	}

	TEST_F(CMockChinaMarketTest, TestThreadSaveStockSection) {
		EXPECT_CALL(*gl_pMockChinaMarket, SaveStockSection)
			.Times(1);
		EXPECT_EQ(ThreadSaveStockSection(gl_pMockChinaMarket.get()), (UINT)35);
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLineRSOfDate) {
		long lPrevMonday = GetPrevMonday(20200101);
		gl_fExitingSystem = true;
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineRS(_)).Times(0);
		EXPECT_EQ(ThreadBuildWeekLineRSOfDate(gl_pMockChinaMarket.get(), lPrevMonday), (UINT)31);

		gl_fExitingSystem = false;
		gl_fExitingCalculatingRS = true;
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineRS(_)).Times(0);
		EXPECT_EQ(ThreadBuildWeekLineRSOfDate(gl_pMockChinaMarket.get(), lPrevMonday), (UINT)31);

		gl_fExitingCalculatingRS = false;
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineRS(lPrevMonday)).Times(1);
		EXPECT_EQ(ThreadBuildWeekLineRSOfDate(gl_pMockChinaMarket.get(), lPrevMonday), (UINT)31);
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLineRS) {
		long lPrevMonday, lPrevMonday1, lPrevMonday2, lPrevMonday3, lPrevMonday4 = 0;

		gl_pMockChinaMarket->CalculateTime();
		lPrevMonday4 = GetPrevMonday(gl_pMockChinaMarket->GetFormatedMarketDate());
		lPrevMonday3 = GetPrevMonday(lPrevMonday4);
		lPrevMonday2 = GetPrevMonday(lPrevMonday3);
		lPrevMonday1 = GetPrevMonday(lPrevMonday2);
		lPrevMonday = GetPrevMonday(lPrevMonday1);
		InSequence seq;
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadBuildWeekLineRSOfDate(lPrevMonday)).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadBuildWeekLineRSOfDate(lPrevMonday1)).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadBuildWeekLineRSOfDate(lPrevMonday2)).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadBuildWeekLineRSOfDate(lPrevMonday3)).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadBuildWeekLineRSOfDate(lPrevMonday4)).Times(1);
		if (gl_pMockChinaMarket->GetDayOfWeek() == 1) { // ����������һ
			EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadBuildWeekLineRSOfDate(gl_pMockChinaMarket->GetFormatedMarketDate())).Times(1); // ��ǰ��Ϊ����һʱ��Ҫ���㵱ǰ��
		}
		EXPECT_EQ(ThreadBuildWeekLineRS(gl_pMockChinaMarket.get(), lPrevMonday1), (UINT)30);

		EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 1);
		gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLine1) {
		gl_ThreadStatus.SetCreatingWeekLine(true);
		EXPECT_CALL(*gl_pMockChinaMarket, DeleteWeekLine()).Times(1).RetiresOnSaturation();
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLine(19900101)).Times(1).RetiresOnSaturation();
		EXPECT_CALL(*gl_pMockChinaMarket, DeleteCurrentWeekWeekLine()).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, BuildCurrentWeekWeekLineTable()).Times(1).RetiresOnSaturation();
		EXPECT_EQ(ThreadBuildWeekLine(gl_pMockChinaMarket.get(), 19900101), (UINT)25);
		EXPECT_FALSE(gl_ThreadStatus.IsCreatingWeekLine());
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLine2) {
		gl_ThreadStatus.SetCreatingWeekLine(true);
		gl_pMockChinaMarket->CalculateTime();
		long lCurrentMonday = GetCurrentMonday(gl_pMockChinaMarket->GetFormatedMarketDate());
		EXPECT_CALL(*gl_pMockChinaMarket, DeleteWeekLine(lCurrentMonday)).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLine(lCurrentMonday)).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, DeleteCurrentWeekWeekLine()).Times(1);
		EXPECT_CALL(*gl_pMockChinaMarket, BuildCurrentWeekWeekLineTable()).Times(1);
		EXPECT_EQ(ThreadBuildWeekLine(gl_pMockChinaMarket.get(), lCurrentMonday), (UINT)25);
		EXPECT_FALSE(gl_ThreadStatus.IsCreatingWeekLine());
	}

	TEST_F(CMockChinaMarketTest, TestTaskSaveStockSection) {
		EXPECT_CALL(*gl_pMockChinaMarket, CreatingThreadSaveStockSection).Times(1);

		EXPECT_TRUE(gl_pMockChinaMarket->TaskSaveStockSection());
	}
}