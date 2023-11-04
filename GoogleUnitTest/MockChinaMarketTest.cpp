///////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʹ��Mock�����ChinaMarket�ĺ�����λ�ڴ��ļ��С�
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"Thread.h"
#include"TimeConvert.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"

#include"MockChinaMarket.h"

#include"GeneralCheck.h"

using namespace testing;
#include<memory>

namespace FireBirdTest {
	CMockChinaMarketPtr s_pMockChinaMarket;

	class CMockChinaMarketTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();

			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

			s_pMockChinaMarket = make_shared<CMockChinaMarket>();
			s_pMockChinaMarket->ResetMarket();
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
			EXPECT_LE(s_pMockChinaMarket->GetDayLineNeedUpdateNumber(), s_pMockChinaMarket->GetTotalStock());

			for (int i = 0; i < s_pMockChinaMarket->GetTotalStock(); i++) {
				const auto pStock = s_pMockChinaMarket->GetStock(i);
				pStock->SetDayLineNeedUpdate(true);
				if (pStock->GetDayLineEndDate() == 20210430) pStock->SetIPOStatus(_STOCK_IPOED_); // �޸Ļ�Ծ��Ʊ��IPO״̬

				if (IsEarlyThen(pStock->GetDayLineEndDate(), s_pMockChinaMarket->GetMarketDate(), 30)) {
					if (pStock->GetDayLineEndDate() == 20210430) {
						EXPECT_TRUE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol(); //"����Ʊ���߽�����������30��ʱ��װ���Ʊ�������ݿ�ʱҪ����´����";
						pStock->SetUpdateProfileDB(false);
					}
				}
			}
			EXPECT_EQ(s_pMockChinaMarket->GetDayLineNeedUpdateNumber(), s_pMockChinaMarket->GetTotalStock());
			while (!s_pMockChinaMarket->IsMarketTaskEmpty()) s_pMockChinaMarket->DiscardCurrentMarketTask();

			EXPECT_TRUE(s_pMockChinaMarket != nullptr) << "��Mock������EnvironmentSetUp.h������";
			EXPECT_FALSE(s_pMockChinaMarket->IsDayLineNeedSaving());
			EXPECT_EQ(s_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);

			ASSERT_THAT(gl_pNeteaseDayLineDataSource, NotNull());

			EXPECT_FALSE(s_pMockChinaMarket->IsSystemReady());
			s_pMockChinaMarket->SetSystemReady(true);// ����ϵͳĬ��Ϊ׼������
		}

		static void TearDownTestSuite() {
			EXPECT_EQ(s_pMockChinaMarket->GetDayLineNeedUpdateNumber(), s_pMockChinaMarket->GetTotalStock());
			EXPECT_FALSE(s_pMockChinaMarket->IsDayLineNeedSaving());
			EXPECT_EQ(s_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);

			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

			// ��������ָ�룬�Բ����Ƿ����û����Ե�Mock��
			s_pMockChinaMarket = nullptr;

			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			//EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_EQ(s_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
			EXPECT_EQ(s_pMockChinaMarket->GetDayLineNeedUpdateNumber(), s_pMockChinaMarket->GetTotalStock());
			EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty()) << s_pMockChinaMarket->GetMarketTask()->GetTime();

			s_pMockChinaMarket->SetRSEndDate(19900101);
		}

		void TearDown() override {
			// clearUp
			EXPECT_EQ(s_pMockChinaMarket->GetDayLineNeedUpdateNumber(), s_pMockChinaMarket->GetTotalStock());
			EXPECT_EQ(s_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
			if (s_pMockChinaMarket->GetDayLineNeedSaveNumber() > 0) {
				for (int i = 0; i < s_pMockChinaMarket->GetTotalStock(); i++) {
					const CChinaStockPtr pStock = s_pMockChinaMarket->GetStock(i);
					if (pStock->IsDayLineNeedSaving()) { EXPECT_STREQ(pStock->GetSymbol(), _T("")); }
				}
			}
			s_pMockChinaMarket->SetRSEndDate(19900101);
			s_pMockChinaMarket->SetUpdateOptionDB(false);

			EXPECT_TRUE(s_pMockChinaMarket->IsSystemReady());
			EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty()) << s_pMockChinaMarket->GetMarketTask()->GetTime();
			EXPECT_FALSE(s_pMockChinaMarket->IsUpdateChosenStockDB());

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask1) {
		s_pMockChinaMarket->AddTask(CREATE_TASK__, 10000);

		EXPECT_CALL(*s_pMockChinaMarket, TaskCreateTask(10000)).Times(1);

		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask(10000));

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty()) << s_pMockChinaMarket->GetMarketTask()->GetTime();
	}

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask2) {
		s_pMockChinaMarket->AddTask(CHINA_MARKET_RESET__, 91300);

		EXPECT_CALL(*s_pMockChinaMarket, TaskResetMarket(91300)).Times(1);

		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask(91300));

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask3) {
		s_pMockChinaMarket->AddTask(CHINA_MARKET_RESET__, 92600);

		EXPECT_CALL(*s_pMockChinaMarket, TaskResetMarket(92700)).Times(1);

		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask(92700));

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask4) {
		s_pMockChinaMarket->AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, 91500);

		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadDistributeAndCalculateRTData()).Times(1);

		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask(93000));

		EXPECT_FALSE(s_pMockChinaMarket->IsMarketTaskEmpty());
		const auto pTask = s_pMockChinaMarket->GetMarketTask();
		s_pMockChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 93001) << "ÿ��һ��";

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask6) {
		s_pMockChinaMarket->AddTask(CHINA_MARKET_SAVE_TEMP_RT_DATA__, 93500);

		EXPECT_TRUE(s_pMockChinaMarket->IsSystemReady());
		s_pMockChinaMarket->SetMarketOpened(true);

		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadUpdateTempRTData()).Times(1);

		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask(93500));

		EXPECT_FALSE(s_pMockChinaMarket->IsMarketTaskEmpty());
		const auto pTask = s_pMockChinaMarket->GetMarketTask();
		s_pMockChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetTime(), 94000);
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_SAVE_TEMP_RT_DATA__);

		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 1);
		gl_systemMessage.PopDayLineInfoMessage();
	}

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask7) {
		s_pMockChinaMarket->AddTask(CHINA_MARKET_BUILD_TODAY_DATABASE__, 150600);

		EXPECT_TRUE(s_pMockChinaMarket->IsSystemReady());

		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadProcessTodayStock()).Times(1);

		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask(150600));

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask8) {
		s_pMockChinaMarket->AddTask(CHINA_MARKET_UPDATE_OPTION_DB__, 150600);

		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadUpdateOptionDB()).Times(1);

		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask(150600));
		EXPECT_FALSE(s_pMockChinaMarket->IsMarketTaskEmpty()) << "������һ������";
		const auto pTask = s_pMockChinaMarket->GetMarketTask();
		s_pMockChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_UPDATE_OPTION_DB__);
		EXPECT_EQ(pTask->GetTime(), 151100) << "ÿ�����һ��";

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask9) {
		s_pMockChinaMarket->AddTask(RELOAD_SYSTEM__, 210000);

		EXPECT_CALL(*s_pMockChinaMarket, TaskReloadSystem(210000)).Times(1);

		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask(210000));

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty()) << s_pMockChinaMarket->GetMarketTask()->GetTime();
	}

	TEST_F(CMockChinaMarketTest, TestTaskSaveTempData1) {
		EXPECT_TRUE(s_pMockChinaMarket->IsSystemReady());
		s_pMockChinaMarket->SetMarketOpened(true);

		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadUpdateTempRTData()).Times(1);

		s_pMockChinaMarket->TaskSaveTempData(120000);

		EXPECT_FALSE(s_pMockChinaMarket->IsMarketTaskEmpty());
		const auto pTask = s_pMockChinaMarket->GetMarketTask();
		s_pMockChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetTime(), 125730) << "��������ʱ���洢��ʱʵʱ���ݣ���13ʱ����ʱ�Ŵ洢";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_SAVE_TEMP_RT_DATA__);
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 1);
		gl_systemMessage.PopDayLineInfoMessage();

		// �ָ�ԭ״
		s_pMockChinaMarket->SetMarketOpened(false);
	}

	TEST_F(CMockChinaMarketTest, TestTaskLoadCurrentStockDayLine1) {
		s_pMockChinaMarket->ResetCurrentStock();
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadLoadDayLine(_))
		.Times(0);

		s_pMockChinaMarket->TaskLoadCurrentStockHistoryData();
	}

	TEST_F(CMockChinaMarketTest, TestTaskLoadCurrentStockDayLine2) {
		s_pMockChinaMarket->ResetCurrentStock();
		s_pMockChinaMarket->SetCurrentStock(_T("600000.SS"));
		s_pMockChinaMarket->GetCurrentStock()->SetDayLineLoaded(true);
		s_pMockChinaMarket->GetCurrentStock()->SetWeekLineLoaded(true);
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadLoadDayLine(_))
		.Times(0);
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadLoadWeekLine(_))
		.Times(0);

		s_pMockChinaMarket->TaskLoadCurrentStockHistoryData();

		EXPECT_TRUE(s_pMockChinaMarket->GetCurrentStock()->IsDayLineLoaded());
		EXPECT_TRUE(s_pMockChinaMarket->GetCurrentStock()->IsWeekLineLoaded());

		//�ָ�ԭ״
		while (!s_pMockChinaMarket->IsMarketTaskEmpty()) s_pMockChinaMarket->DiscardCurrentMarketTask();
	}

	TEST_F(CMockChinaMarketTest, TestTaskLoadCurrentStockDayLine3) {
		s_pMockChinaMarket->ResetCurrentStock();
		s_pMockChinaMarket->SetCurrentStock(_T("600000.SS"));
		s_pMockChinaMarket->GetCurrentStock()->SetDayLineLoaded(false);
		s_pMockChinaMarket->GetCurrentStock()->SetWeekLineLoaded(false);
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadLoadDayLine(_))
		.Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadLoadWeekLine(_))
		.Times(1);

		s_pMockChinaMarket->TaskLoadCurrentStockHistoryData();

		EXPECT_TRUE(s_pMockChinaMarket->GetCurrentStock()->IsDayLineLoaded());
		EXPECT_TRUE(s_pMockChinaMarket->GetCurrentStock()->IsWeekLineLoaded());

		//�ָ�ԭ״
		while (!s_pMockChinaMarket->IsMarketTaskEmpty()) s_pMockChinaMarket->DiscardCurrentMarketTask();
	}

	TEST_F(CMockChinaMarketTest, TestTaskAccessoryTask) {
		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());

		s_pMockChinaMarket->TaskAccessoryTask(10000);

		EXPECT_FALSE(s_pMockChinaMarket->IsMarketTaskEmpty());
		const auto pTask = s_pMockChinaMarket->GetMarketTask();
		s_pMockChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_ACCESSORY_TASK__);
		EXPECT_EQ(pTask->GetTime(), 10100) << "ÿһ����һ��";

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CMockChinaMarketTest, TestTaskDistributeAndCalculateRTData) {
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadDistributeAndCalculateRTData()).Times(1);

		s_pMockChinaMarket->TaskDistributeAndCalculateRTData(100000);

		const auto pTask = s_pMockChinaMarket->GetMarketTask();
		s_pMockChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 100001) << "ÿ��һ��";

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CMockChinaMarketTest, TestTaskProcessAndSaveDayLine) {
		EXPECT_FALSE(s_pMockChinaMarket->IsDayLineNeedProcess());
		EXPECT_FALSE(s_pMockChinaMarket->IsDayLineNeedSaving());
		EXPECT_FALSE(s_pMockChinaMarket->IsTaskOfSavingDayLineDBFinished());
		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());

		s_pMockChinaMarket->TaskProcessAndSaveDayLine(100000);

		EXPECT_FALSE(s_pMockChinaMarket->IsRTDataNeedCalculate());
		EXPECT_FALSE(s_pMockChinaMarket->IsMarketTaskEmpty());
		const auto pTask = s_pMockChinaMarket->GetMarketTask();
		s_pMockChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PROCESS_AND_SAVE_DAY_LINE__);
		EXPECT_EQ(pTask->GetTime(), 100010) << "ÿʮ��һ��";

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CMockChinaMarketTest, TestTaskUpdateStockProfileDB) {
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadUpdateStockProfileDB())
		.Times(0);
		EXPECT_FALSE(s_pMockChinaMarket->TaskUpdateStockProfileDB(0));

		s_pMockChinaMarket->GetStock(1)->SetUpdateProfileDB(true);
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadUpdateStockProfileDB())
		.Times(1);
		EXPECT_TRUE(s_pMockChinaMarket->TaskUpdateStockProfileDB(0));

		// �ָ�ԭ״
		s_pMockChinaMarket->GetStock(1)->SetUpdateProfileDB(false);
		while (!s_pMockChinaMarket->IsMarketTaskEmpty()) s_pMockChinaMarket->DiscardCurrentMarketTask();
	}

	TEST_F(CMockChinaMarketTest, TestTaskChoice10RSStrong2StockSet) {
		tm tm_{};
		tm_.tm_wday = 1; // ����һ
		s_pMockChinaMarket->TEST_SetMarketTM(tm_);
		s_pMockChinaMarket->SetSystemReady(false);
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151200));
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151201));
		s_pMockChinaMarket->SetSystemReady(true);
		s_pMockChinaMarket->SetChosen10RSStrong2StockSet(true);
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151200));
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151201));
		s_pMockChinaMarket->SetSystemReady(true);
		s_pMockChinaMarket->SetChosen10RSStrong2StockSet(false);
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadChoice10RSStrong2StockSet)
		.Times(1);
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151200));
		EXPECT_TRUE(s_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151201));
		EXPECT_TRUE(s_pMockChinaMarket->IsChosen10RSStrong2StockSet());

		tm_.tm_wday = 0; // ������
		s_pMockChinaMarket->TEST_SetMarketTM(tm_);
		s_pMockChinaMarket->SetChosen10RSStrong2StockSet(false);
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151201));
		EXPECT_FALSE(s_pMockChinaMarket->IsChosen10RSStrong2StockSet()) << _T("��Ϣ�ղ�����");
	}

	TEST_F(CMockChinaMarketTest, TestTaskChoice10RSStrong1StockSet) {
		tm tm_{};
		tm_.tm_wday = 1; // ����һ
		s_pMockChinaMarket->TEST_SetMarketTM(tm_);
		s_pMockChinaMarket->SetSystemReady(false);
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151100));
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151101));
		s_pMockChinaMarket->SetSystemReady(true);
		s_pMockChinaMarket->SetChosen10RSStrong1StockSet(true);
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151100));
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151101));
		s_pMockChinaMarket->SetSystemReady(true);
		s_pMockChinaMarket->SetChosen10RSStrong1StockSet(false);
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadChoice10RSStrong1StockSet)
		.Times(1);
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151100));
		EXPECT_TRUE(s_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151101));
		EXPECT_TRUE(s_pMockChinaMarket->IsChosen10RSStrong1StockSet());

		tm_.tm_wday = 0; // ������
		s_pMockChinaMarket->TEST_SetMarketTM(tm_);
		s_pMockChinaMarket->SetChosen10RSStrong1StockSet(false);
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151101));
		EXPECT_FALSE(s_pMockChinaMarket->IsChosen10RSStrong1StockSet()) << _T("��Ϣ�ղ�����");
	}

	TEST_F(CMockChinaMarketTest, TestTaskChoice10RSStrongStockSet) {
		tm tm_{};
		tm_.tm_wday = 1; // ����һ
		s_pMockChinaMarket->TEST_SetMarketTM(tm_);
		s_pMockChinaMarket->SetSystemReady(false);
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrongStockSet(151000));
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
		s_pMockChinaMarket->SetSystemReady(true);
		s_pMockChinaMarket->SetChosen10RSStrongStockSet(true);
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrongStockSet(151000));
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
		s_pMockChinaMarket->SetSystemReady(true);
		s_pMockChinaMarket->SetChosen10RSStrongStockSet(false);
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadChoice10RSStrongStockSet)
		.Times(1);
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrongStockSet(151000));
		EXPECT_TRUE(s_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
		EXPECT_TRUE(s_pMockChinaMarket->IsChosen10RSStrongStockSet());

		tm_.tm_wday = 0; // ������
		s_pMockChinaMarket->TEST_SetMarketTM(tm_);
		s_pMockChinaMarket->SetChosen10RSStrongStockSet(false);
		EXPECT_FALSE(s_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
		EXPECT_FALSE(s_pMockChinaMarket->IsChosen10RSStrongStockSet()) << _T("��Ϣ�ղ�����");
	}

	TEST_F(CMockChinaMarketTest, TestThreadCalculateDayLineRS) {
		s_pMockChinaMarket->CalculateTime();
		time_t tStart = GetUTCTime() - 3600 * 24 * 6; // ��һ��ǰ��ʼ����
		tm _tm;
		GetMarketTimeStruct(&_tm, tStart, s_pMockChinaMarket->GetMarketTimeZone());
		long lStartDate = ConvertToDate(&_tm);
		gl_systemConfiguration.SetExitingCalculatingRS(true); // �м䱻���
		s_pMockChinaMarket->SetCalculatingDayLineRS(true);
		s_pMockChinaMarket->SetRSEndDate(0);
		s_pMockChinaMarket->SetUpdateOptionDB(false);
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadBuildDayLineRSOfDate(_)).Times(5);
		EXPECT_EQ(ThreadBuildDayLineRS(s_pMockChinaMarket.get(), lStartDate), static_cast<UINT>(11));
		EXPECT_FALSE(s_pMockChinaMarket->IsUpdateOptionDB()) << _T("����Ϻ����ô˱�ʶ");
		EXPECT_EQ(s_pMockChinaMarket->GetRSEndDate(), 0);
		EXPECT_FALSE(gl_systemConfiguration.IsExitingCalculatingRS());
		EXPECT_FALSE(s_pMockChinaMarket->IsCalculatingDayLineRS());

		tStart = GetUTCTime() - 3600 * 24 * 6; // ��һ��ǰ��ʼ����
		GetMarketTimeStruct(&_tm, tStart, s_pMockChinaMarket->GetMarketTimeZone());
		lStartDate = ConvertToDate(&_tm);
		gl_systemConfiguration.SetExitingCalculatingRS(false);
		s_pMockChinaMarket->SetCalculatingDayLineRS(true);
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadBuildDayLineRSOfDate(_))
		.Times(5);
		EXPECT_EQ(ThreadBuildDayLineRS(s_pMockChinaMarket.get(), lStartDate), static_cast<UINT>(11));
		EXPECT_TRUE(s_pMockChinaMarket->IsUpdateOptionDB());
		EXPECT_EQ(s_pMockChinaMarket->GetRSEndDate(), s_pMockChinaMarket->GetMarketDate());
		EXPECT_FALSE(s_pMockChinaMarket->IsCalculatingDayLineRS());

		EXPECT_THAT(gl_systemMessage.InformationSize(), 4); // �����ε��ã����ĸ���Ϣ
		gl_systemMessage.PopInformationMessage();
		gl_systemMessage.PopInformationMessage();
		gl_systemMessage.PopInformationMessage();
		gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CMockChinaMarketTest, TestThreadUpdateOptionDB) {
		EXPECT_CALL(*s_pMockChinaMarket, UpdateOptionDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateOptionDB(s_pMockChinaMarket.get()), static_cast<UINT>(20));
	}

	TEST_F(CMockChinaMarketTest, TestThreadUpdateStockProfileDB) {
		ASSERT_THAT(s_pMockChinaMarket->IsUpdateStockProfileDB(), IsFalse()) << "�˲��Կ�ʼʱ�����뱣֤û�����ø��´����ı�ʶ����������������˲��Դ����";

		EXPECT_CALL(*s_pMockChinaMarket, UpdateStockProfileDB)
		.Times(1);
		s_pMockChinaMarket->SetSystemReady(true);
		EXPECT_EQ(ThreadUpdateChinaStockProfileDB(s_pMockChinaMarket.get()), static_cast<UINT>(18));
	}

	TEST_F(CMockChinaMarketTest, TestThreadUpdateChosenStockDB) {
		EXPECT_CALL(*s_pMockChinaMarket, AppendChosenStockDB)
		.Times(1);
		s_pMockChinaMarket->SetSystemReady(true);
		EXPECT_EQ(ThreadAppendChosenStockDB(s_pMockChinaMarket.get()), static_cast<UINT>(22));
	}

	TEST_F(CMockChinaMarketTest, TestThreadSaveTempRTData) {
		EXPECT_CALL(*s_pMockChinaMarket, SaveTempRTData)
		.Times(1);
		s_pMockChinaMarket->SetSystemReady(true);
		EXPECT_EQ(ThreadSaveTempRTData(s_pMockChinaMarket.get()), static_cast<UINT>(13));
	}

	TEST_F(CMockChinaMarketTest, TestThreadLoadTempRTData) {
		EXPECT_CALL(*s_pMockChinaMarket, LoadTempRTData(20200101)).Times(1);

		EXPECT_EQ(ThreadLoadTempRTData(s_pMockChinaMarket.get(), 20200101), static_cast<UINT>(19));
	}

	TEST_F(CMockChinaMarketTest, TestThreadMaintainDayLineDatabase) {
		EXPECT_EQ(ThreadMaintainDayLineDataBase(), static_cast<UINT>(17)) << "�˹����߳�Ŀǰû��ʵ�ʶ���";
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLineOfCurrentWeek) {
		EXPECT_CALL(*s_pMockChinaMarket, BuildWeekLineOfCurrentWeek)
		.Times(1);
		EXPECT_EQ(ThreadBuildWeekLineOfCurrentWeek(s_pMockChinaMarket.get()), static_cast<UINT>(32));
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildCurrentWeekWeekLineTable) {
		EXPECT_CALL(*s_pMockChinaMarket, DeleteCurrentWeekWeekLine).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, BuildCurrentWeekWeekLineTable).Times(1);
		EXPECT_EQ(ThreadBuildCurrentWeekWeekLineTable(s_pMockChinaMarket.get()), static_cast<UINT>(33));
	}

	TEST_F(CMockChinaMarketTest, TestThreadSaveStockSection) {
		EXPECT_CALL(*s_pMockChinaMarket, SaveStockSection)
		.Times(1);
		EXPECT_EQ(ThreadSaveStockSection(s_pMockChinaMarket.get()), static_cast<UINT>(35));
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLineRSOfDate) {
		const long lPrevMonday = GetPrevMonday(20200101);
		gl_systemConfiguration.SetExitingSystem(true);
		EXPECT_CALL(*s_pMockChinaMarket, BuildWeekLineRS(_)).Times(0);
		EXPECT_EQ(ThreadBuildWeekLineRSOfDate(s_pMockChinaMarket.get(), lPrevMonday), static_cast<UINT>(31));

		gl_systemConfiguration.SetExitingSystem(false);
		gl_systemConfiguration.SetExitingCalculatingRS(true);
		EXPECT_CALL(*s_pMockChinaMarket, BuildWeekLineRS(_)).Times(0);
		EXPECT_EQ(ThreadBuildWeekLineRSOfDate(s_pMockChinaMarket.get(), lPrevMonday), static_cast<UINT>(31));

		gl_systemConfiguration.SetExitingCalculatingRS(false);
		EXPECT_CALL(*s_pMockChinaMarket, BuildWeekLineRS(lPrevMonday)).Times(1);
		EXPECT_EQ(ThreadBuildWeekLineRSOfDate(s_pMockChinaMarket.get(), lPrevMonday), static_cast<UINT>(31));
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLineRS) {
		s_pMockChinaMarket->CalculateTime();
		const long lPrevMonday4 = GetPrevMonday(s_pMockChinaMarket->GetMarketDate());
		const long lPrevMonday3 = GetPrevMonday(lPrevMonday4);
		const long lPrevMonday2 = GetPrevMonday(lPrevMonday3);
		const long lPrevMonday1 = GetPrevMonday(lPrevMonday2);
		const long lPrevMonday = GetPrevMonday(lPrevMonday1);
		InSequence seq;
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadBuildWeekLineRSOfDate(lPrevMonday)).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadBuildWeekLineRSOfDate(lPrevMonday1)).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadBuildWeekLineRSOfDate(lPrevMonday2)).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadBuildWeekLineRSOfDate(lPrevMonday3)).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadBuildWeekLineRSOfDate(lPrevMonday4)).Times(1);
		if (s_pMockChinaMarket->GetDayOfWeek() == 1) {
			// ����������һ
			EXPECT_CALL(*s_pMockChinaMarket, CreateThreadBuildWeekLineRSOfDate(s_pMockChinaMarket->GetMarketDate())).Times(1); // ��ǰ��Ϊ����һʱ��Ҫ���㵱ǰ��
		}
		EXPECT_EQ(ThreadBuildWeekLineRS(s_pMockChinaMarket.get(), lPrevMonday1), static_cast<UINT>(30));

		EXPECT_THAT(gl_systemMessage.InformationSize(), 1);
		gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLine1) {
		EXPECT_CALL(*s_pMockChinaMarket, DeleteWeekLine()).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, BuildWeekLine(19900101)).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, DeleteCurrentWeekWeekLine()).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, BuildCurrentWeekWeekLineTable()).Times(1);

		EXPECT_EQ(ThreadBuildWeekLine(s_pMockChinaMarket.get(), 19900101), static_cast<UINT>(25));
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLine2) {
		s_pMockChinaMarket->CalculateTime();
		const long lCurrentMonday = GetCurrentMonday(s_pMockChinaMarket->GetMarketDate());
		EXPECT_CALL(*s_pMockChinaMarket, DeleteWeekLine(lCurrentMonday)).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, BuildWeekLine(lCurrentMonday)).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, DeleteCurrentWeekWeekLine()).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, BuildCurrentWeekWeekLineTable()).Times(1);
		EXPECT_EQ(ThreadBuildWeekLine(s_pMockChinaMarket.get(), lCurrentMonday), static_cast<UINT>(25));
	}

	TEST_F(CMockChinaMarketTest, TestTaskLoadTempRTData) {
		EXPECT_FALSE(s_pMockChinaMarket->IsTodayTempRTDataLoaded());
		EXPECT_TRUE(s_pMockChinaMarket->IsSystemReady());
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadLoadTempRTData(20190517)).Times(1)
		.WillOnce(Invoke([] { s_pMockChinaMarket->SetTodayTempRTDataLoaded(true); })); // LoadTempRTDataִ��ʱ���ô˱�ʶ

		EXPECT_TRUE(s_pMockChinaMarket->TaskLoadTempRTData(20190517, 10000));

		EXPECT_TRUE(s_pMockChinaMarket->IsTodayTempRTDataLoaded());
		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty()) << "��������װ����ʱ���ݵ�����";

		// �ָ�ԭ״
		s_pMockChinaMarket->SetTodayTempRTDataLoaded(false);
	}

	TEST_F(CMockChinaMarketTest, TestProcessTodayStock1) {
		s_pMockChinaMarket->SetNewestTransactionTime(GetUTCTime());
		const long lDate = s_pMockChinaMarket->GetMarketDate();

		EXPECT_TRUE(s_pMockChinaMarket->IsSystemReady());
		s_pMockChinaMarket->TEST_SetFormattedMarketTime(60000);
		s_pMockChinaMarket->TEST_SetFormattedMarketDate(19800101);

		EXPECT_CALL(*s_pMockChinaMarket, BuildDayLine(_)).Times(0);
		EXPECT_CALL(*s_pMockChinaMarket, BuildDayLineRS(_)).Times(0);
		EXPECT_CALL(*s_pMockChinaMarket, BuildWeekLineOfCurrentWeek()).Times(0);
		EXPECT_CALL(*s_pMockChinaMarket, BuildWeekLineRS(_)).Times(0);
		EXPECT_CALL(*s_pMockChinaMarket, UpdateStockProfileDB()).Times(0);

		s_pMockChinaMarket->ProcessTodayStock();

		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);

		// �ָ�ԭ״
		gl_systemMessage.PopInformationMessage();
		s_pMockChinaMarket->TEST_SetFormattedMarketDate(lDate);
	}

	TEST_F(CMockChinaMarketTest, TestProcessTodayStock2) {
		s_pMockChinaMarket->SetNewestTransactionTime(GetUTCTime());
		const tm tm1 = s_pMockChinaMarket->GetMarketTime(GetUTCTime());
		const long lDate = ConvertToDate(&tm1);
		const long lWeekBeginDate = GetCurrentMonday(lDate);

		EXPECT_TRUE(s_pMockChinaMarket->IsSystemReady());
		s_pMockChinaMarket->TEST_SetFormattedMarketTime(60000);
		s_pMockChinaMarket->SetRSEndDate(19800101);
		s_pMockChinaMarket->SetUpdateOptionDB(false);

		EXPECT_CALL(*s_pMockChinaMarket, BuildDayLine(lDate)).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, BuildDayLineRS(lDate)).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, BuildWeekLineOfCurrentWeek()).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, BuildWeekLineRS(lWeekBeginDate)).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, UpdateStockProfileDB()).Times(1);

		s_pMockChinaMarket->ProcessTodayStock();

		EXPECT_FALSE(s_pMockChinaMarket->IsUpdateOptionDB());
		EXPECT_EQ(s_pMockChinaMarket->GetRSEndDate(), 19800101);
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CMockChinaMarketTest, TestProcessTodayStock3) {
		s_pMockChinaMarket->SetNewestTransactionTime(GetUTCTime());
		const tm tm1 = s_pMockChinaMarket->GetMarketTime(GetUTCTime());
		const long lDate = ConvertToDate(&tm1);
		const long lWeekBeginDate = GetCurrentMonday(lDate);

		EXPECT_TRUE(s_pMockChinaMarket->IsSystemReady());
		s_pMockChinaMarket->TEST_SetFormattedMarketTime(160000);
		s_pMockChinaMarket->SetRSEndDate(19800101);
		s_pMockChinaMarket->SetUpdateOptionDB(false);

		EXPECT_CALL(*s_pMockChinaMarket, BuildDayLine(lDate)).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, BuildDayLineRS(lDate)).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, BuildWeekLineOfCurrentWeek()).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, BuildWeekLineRS(lWeekBeginDate)).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, UpdateStockProfileDB()).Times(1);

		s_pMockChinaMarket->ProcessTodayStock();

		EXPECT_EQ(s_pMockChinaMarket->GetRSEndDate(), lDate);
		EXPECT_TRUE(s_pMockChinaMarket->IsUpdateOptionDB());
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);

		// �ָ�ԭ״
		s_pMockChinaMarket->SetUpdateOptionDB(false);
		gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CMockChinaMarketTest, TestTaskUpdateStockSection) {
		EXPECT_FALSE(s_pMockChinaMarket->IsUpdateStockSection());
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadSaveStockSection).Times(0);

		EXPECT_FALSE(s_pMockChinaMarket->TaskUpdateStockSection());

		s_pMockChinaMarket->SetUpdateStockSection(true);
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadSaveStockSection).Times(1);

		EXPECT_TRUE(s_pMockChinaMarket->TaskUpdateStockSection());

		EXPECT_FALSE(s_pMockChinaMarket->IsUpdateStockSection());
	}

	TEST_F(CMockChinaMarketTest, TestTaskUpdateChosenStockDB) {
		EXPECT_FALSE(s_pMockChinaMarket->IsUpdateChosenStockDB());
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadUpdateChoseStockDB).Times(0);

		EXPECT_FALSE(s_pMockChinaMarket->TaskUpdateChosenStockDB());

		s_pMockChinaMarket->SetUpdateChosenStockDB(true);
		EXPECT_CALL(*s_pMockChinaMarket, CreateThreadUpdateChoseStockDB).Times(1);

		EXPECT_TRUE(s_pMockChinaMarket->TaskUpdateChosenStockDB());

		EXPECT_TRUE(s_pMockChinaMarket->IsUpdateChosenStockDB());

		// �ָ�ԭ״
		s_pMockChinaMarket->SetUpdateChosenStockDB(false);
	}
}
