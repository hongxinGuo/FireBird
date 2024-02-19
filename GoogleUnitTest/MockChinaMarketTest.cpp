///////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʹ��Mock�����ChinaMarket�ĺ�����λ�ڴ��ļ��С�
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"Thread.h"
#include"TimeConvert.h"

#include"ChinaStock.h"

#include"MockChinaMarket.h"

#include"GeneralCheck.h"
#include "NeteaseDayLineDataSource.h"

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
			EXPECT_LE(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());

			for (int i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
				const auto pStock = gl_dataContainerChinaStock.GetStock(i);
				pStock->SetDayLineNeedUpdate(true);
				if (pStock->GetDayLineEndDate() == 20210430) pStock->SetIPOStatus(_STOCK_IPOED_); // �޸Ļ�Ծ��Ʊ��IPO״̬

				if (IsEarlyThen(pStock->GetDayLineEndDate(), s_pMockChinaMarket->GetMarketDate(), 30)) {
					if (pStock->GetDayLineEndDate() == 20210430) {
						EXPECT_TRUE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol(); //"����Ʊ���߽�����������30��ʱ��װ���Ʊ�������ݿ�ʱҪ����´����";
						pStock->SetUpdateProfileDB(false);
					}
				}
			}
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			while (!s_pMockChinaMarket->IsMarketTaskEmpty()) s_pMockChinaMarket->DiscardCurrentMarketTask();

			EXPECT_TRUE(s_pMockChinaMarket != nullptr) << "��Mock������EnvironmentSetUp.h������";
			EXPECT_FALSE(gl_dataContainerChinaStock.IsDayLineNeedSaving());
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedSaveNumber(), 0);

			ASSERT_THAT(gl_pNeteaseDayLineDataSource, NotNull());

			EXPECT_FALSE(s_pMockChinaMarket->IsSystemReady());
			s_pMockChinaMarket->SetSystemReady(true);// ����ϵͳĬ��Ϊ׼������
		}

		static void TearDownTestSuite() {
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_FALSE(gl_dataContainerChinaStock.IsDayLineNeedSaving());
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedSaveNumber(), 0);

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

			//EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedSaveNumber(), 0);
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty()) << s_pMockChinaMarket->GetMarketTask()->GetTime();

			s_pMockChinaMarket->SetRSEndDate(19900101);
		}

		void TearDown() override {
			// clearUp
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedSaveNumber(), 0);
			if (gl_dataContainerChinaStock.GetDayLineNeedSaveNumber() > 0) {
				for (int i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
					const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(i);
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

		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask(93000));

		EXPECT_FALSE(s_pMockChinaMarket->IsMarketTaskEmpty());
		const auto pTask = s_pMockChinaMarket->GetMarketTask();
		s_pMockChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 93001) << "ÿ��һ��";

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CMockChinaMarketTest, TestProcessEveryDayTask9) {
		s_pMockChinaMarket->AddTask(RELOAD_SYSTEM__, 210000);

		EXPECT_CALL(*s_pMockChinaMarket, TaskExitSystem(210000)).Times(1);

		EXPECT_TRUE(s_pMockChinaMarket->ProcessTask(210000));

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty()) << s_pMockChinaMarket->GetMarketTask()->GetTime();
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
		s_pMockChinaMarket->TaskDistributeAndCalculateRTData(100000);

		const auto pTask = s_pMockChinaMarket->GetMarketTask();
		s_pMockChinaMarket->DiscardCurrentMarketTask();
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(pTask->GetTime(), 100001) << "ÿ��һ��";

		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());
	}

	TEST_F(CMockChinaMarketTest, TestTaskProcessAndSaveDayLine) {
		EXPECT_FALSE(s_pMockChinaMarket->IsDayLineNeedProcess());
		EXPECT_FALSE(gl_dataContainerChinaStock.IsDayLineNeedSaving());
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

	TEST_F(CMockChinaMarketTest, TestTaskProcessAndSaveDayLine2) {
		EXPECT_FALSE(gl_systemConfiguration.IsExitingSystem());
		gl_systemConfiguration.SetExitingSystem(true);
		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty());

		s_pMockChinaMarket->TaskProcessAndSaveDayLine(100000);

		EXPECT_FALSE(s_pMockChinaMarket->IsRTDataNeedCalculate());
		EXPECT_TRUE(s_pMockChinaMarket->IsMarketTaskEmpty()) << "���˳�ϵͳ��ʶ����ʱ���������ɴ�����������";

		// �ָ�ԭ״
		gl_systemConfiguration.SetExitingSystem(false);
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
		EXPECT_EQ(ThreadBuildDayLineRS(s_pMockChinaMarket, lStartDate), static_cast<UINT>(11));
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
		EXPECT_EQ(ThreadBuildDayLineRS(s_pMockChinaMarket, lStartDate), static_cast<UINT>(11));
		EXPECT_TRUE(s_pMockChinaMarket->IsUpdateOptionDB());
		EXPECT_EQ(s_pMockChinaMarket->GetRSEndDate(), s_pMockChinaMarket->GetMarketDate());
		EXPECT_FALSE(s_pMockChinaMarket->IsCalculatingDayLineRS());

		EXPECT_THAT(gl_systemMessage.InformationSize(), 4); // �����ε��ã����ĸ���Ϣ
		gl_systemMessage.PopInformationMessage();
		gl_systemMessage.PopInformationMessage();
		gl_systemMessage.PopInformationMessage();
		gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLineOfCurrentWeek) {
		EXPECT_CALL(*s_pMockChinaMarket, BuildWeekLineOfCurrentWeek)
		.Times(1);
		EXPECT_EQ(ThreadBuildWeekLineOfCurrentWeek(s_pMockChinaMarket), static_cast<UINT>(32));
	}

	TEST_F(CMockChinaMarketTest, TestThreadBuildCurrentWeekWeekLineTable) {
		EXPECT_CALL(*s_pMockChinaMarket, DeleteCurrentWeekWeekLine).Times(1);
		EXPECT_CALL(*s_pMockChinaMarket, BuildCurrentWeekWeekLineTable).Times(1);
		EXPECT_EQ(ThreadBuildCurrentWeekWeekLineTable(s_pMockChinaMarket), static_cast<UINT>(33));
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
		EXPECT_EQ(ThreadBuildWeekLineRS(s_pMockChinaMarket, lPrevMonday1), static_cast<UINT>(30));

		EXPECT_THAT(gl_systemMessage.InformationSize(), 1);
		gl_systemMessage.PopInformationMessage();
	}
}
