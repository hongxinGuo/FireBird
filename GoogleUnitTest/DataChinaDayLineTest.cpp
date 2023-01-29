#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"DataChinaDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CDataChinaDayLineTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) { // ��������ĳ�ʼ������
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			// clearUp
		}
	};

	TEST_F(CDataChinaDayLineTest, TestGetStartEndDate) {
		CDataChinaDayLine dataChinaDayLine;

		vector<CDayLinePtr> vDayLine;

		auto pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200101); // ������
		pDayLine->SetHigh(10010);
		pDayLine->SetLow(9910);
		pDayLine->SetClose(10001);
		pDayLine->SetLastClose(10005);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = nullptr;
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200102); // ������
		pDayLine->SetHigh(10020);
		pDayLine->SetLow(9920);
		pDayLine->SetClose(10002);
		pDayLine->SetLastClose(10001);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = nullptr;
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200103); // ������
		pDayLine->SetHigh(10030);
		pDayLine->SetLow(9930);
		pDayLine->SetClose(10003);
		pDayLine->SetLastClose(10002);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		dataChinaDayLine.SetDataLoaded(false);
		dataChinaDayLine.UpdateData(vDayLine);
		long lStartDate = 0, lEndDate = 0;
		dataChinaDayLine.GetStartEndDate(lStartDate, lEndDate);

		EXPECT_EQ(lStartDate, 20200101);
		EXPECT_EQ(lEndDate, 20200103);
	}

	TEST_F(CDataChinaDayLineTest, TestUpdateData1) {
		CDataChinaDayLine dataChinaDayLine;

		vector<CDayLinePtr> vDayLine;

		auto pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200101); // ������
		pDayLine->SetHigh(10010);
		pDayLine->SetLow(9910);
		pDayLine->SetClose(10001);
		pDayLine->SetLastClose(10005);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = nullptr;
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200102); // ������
		pDayLine->SetHigh(10020);
		pDayLine->SetLow(9920);
		pDayLine->SetClose(10002);
		pDayLine->SetLastClose(10001);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = nullptr;
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200103); // ������
		pDayLine->SetHigh(10030);
		pDayLine->SetLow(9930);
		pDayLine->SetClose(10003);
		pDayLine->SetLastClose(10002);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		dataChinaDayLine.SetDataLoaded(false);

		dataChinaDayLine.UpdateData(vDayLine);

		EXPECT_THAT(dataChinaDayLine.IsDataLoaded(), IsTrue());
		EXPECT_THAT(dataChinaDayLine.Size(), 3);
		EXPECT_THAT(dataChinaDayLine.GetData(0)->GetClose(), 10001) << "����洢����һ�����ݵ����̼�";
		EXPECT_THAT(dataChinaDayLine.GetData(2)->GetClose(), 10003) << "����洢�����������ݵ����̼�";
	}

	TEST_F(CDataChinaDayLineTest, TestCreateNewWeekLine) {
		CDataChinaDayLine dataChinaDayLine;

		vector<CDayLinePtr> vDayLine;
		long lCurrentDayLinePos = 0;

		auto pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200101); // ������
		pDayLine->SetHigh(10010);
		pDayLine->SetLow(9910);
		pDayLine->SetClose(10001);
		pDayLine->SetLastClose(10000); // ��һ������Ϊ0
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = nullptr;
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200102); // ������
		pDayLine->SetHigh(10020);
		pDayLine->SetLow(9920);
		pDayLine->SetClose(10002);
		pDayLine->SetLastClose(10001);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = nullptr;
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200103); // ������
		pDayLine->SetHigh(10030);
		pDayLine->SetLow(9930);
		pDayLine->SetClose(10003);
		pDayLine->SetLastClose(10002);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = nullptr;
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200106); // ����һ
		pDayLine->SetHigh(10040);
		pDayLine->SetLow(9940);
		pDayLine->SetClose(10004);
		pDayLine->SetLastClose(10003);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();

		pDayLine->SetDate(20200107); // ���ڶ�
		pDayLine->SetHigh(10050);
		pDayLine->SetLow(9950);
		pDayLine->SetClose(10005);
		pDayLine->SetLastClose(10004);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		dataChinaDayLine.UpdateData(vDayLine);
		CWeekLinePtr pWeekLine = dataChinaDayLine.CreateNewWeekLine(lCurrentDayLinePos);

		EXPECT_THAT(lCurrentDayLinePos, 3);
		EXPECT_THAT(pWeekLine->GetMarketDate(), 20191230) << "����һ";
		EXPECT_THAT(pWeekLine->GetClose(), 10003);
		EXPECT_THAT(pWeekLine->GetLastClose(), 10000);
		EXPECT_THAT(pWeekLine->GetHigh(), 10030);
		EXPECT_THAT(pWeekLine->GetLow(), 9910);

		pWeekLine = dataChinaDayLine.CreateNewWeekLine(lCurrentDayLinePos);

		EXPECT_THAT(lCurrentDayLinePos, 5);
		EXPECT_THAT(pWeekLine->GetMarketDate(), 20200106) << "����һ";
		EXPECT_THAT(pWeekLine->GetClose(), 10005);
		EXPECT_THAT(pWeekLine->GetLastClose(), 10003);
		EXPECT_THAT(pWeekLine->GetHigh(), 10050);
		EXPECT_THAT(pWeekLine->GetLow(), 9940);
	}

	TEST_F(CDataChinaDayLineTest, TestBuildWeekLine) {
		vector<CWeekLinePtr> vWeekLine;
		CDataChinaDayLine dataChinaDayLine;
		vector<CDayLinePtr> vDayLine;

		auto pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200101); // ������
		pDayLine->SetHigh(10010);
		pDayLine->SetLow(9910);
		pDayLine->SetClose(10001);
		pDayLine->SetLastClose(10000);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = nullptr;
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200102); // ������
		pDayLine->SetHigh(10020);
		pDayLine->SetLow(9920);
		pDayLine->SetClose(10002);
		pDayLine->SetLastClose(10001);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = nullptr;
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200103); // ������
		pDayLine->SetHigh(10030);
		pDayLine->SetLow(9930);
		pDayLine->SetClose(10003);
		pDayLine->SetLastClose(10002);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = nullptr;
		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200106); // ����һ
		pDayLine->SetHigh(10040);
		pDayLine->SetLow(9940);
		pDayLine->SetClose(10004);
		pDayLine->SetLastClose(10003);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);
		pDayLine = make_shared<CDayLine>();

		pDayLine->SetDate(20200107); // ���ڶ�
		pDayLine->SetHigh(10050);
		pDayLine->SetLow(9950);
		pDayLine->SetClose(10005);
		pDayLine->SetLastClose(10004);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		dataChinaDayLine.UpdateData(vDayLine);
		dataChinaDayLine.BuildWeekLine(vWeekLine);

		EXPECT_THAT(vWeekLine.size(), 2);
		EXPECT_THAT(vWeekLine.at(0)->GetMarketDate(), 20191230) << "����һ";
		EXPECT_THAT(vWeekLine.at(0)->GetClose(), 10003);
		EXPECT_THAT(vWeekLine.at(0)->GetLastClose(), 10000);
		EXPECT_THAT(vWeekLine.at(0)->GetHigh(), 10030);
		EXPECT_THAT(vWeekLine.at(0)->GetLow(), 9910);

		EXPECT_THAT(vWeekLine.at(1)->GetMarketDate(), 20200106) << "����һ";
		EXPECT_THAT(vWeekLine.at(1)->GetClose(), 10005);
		EXPECT_THAT(vWeekLine.at(1)->GetLastClose(), 10003);
		EXPECT_THAT(vWeekLine.at(1)->GetHigh(), 10050);
		EXPECT_THAT(vWeekLine.at(1)->GetLow(), 9940);
	}

	TEST_F(CDataChinaDayLineTest, TestSaveDB) {
		vector<CDayLinePtr> vDayLine;
		CDataChinaDayLine dataChinaDayLine;

		const auto pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(19910102); // �������ݿ������������Ϊ19910103���ʴ�����λ����ǰ��
		pDayLine->SetStockSymbol(_T("000001.SZ"));
		pDayLine->SetClose(100);
		vDayLine.push_back(pDayLine);
		dataChinaDayLine.UpdateData(vDayLine);

		CSetDayLineBasicInfo setDayLineBasic;
		setDayLineBasic.m_strFilter = _T("[symbol] = '000001.SZ'");
		setDayLineBasic.m_strSort = _T("Date");
		setDayLineBasic.Open();
		setDayLineBasic.m_pDatabase->BeginTrans();
		setDayLineBasic.AddNew();
		setDayLineBasic.m_Date = 19910103; // ������ڴ��������ݼ���
		setDayLineBasic.m_Symbol = _T("000001.SZ");
		setDayLineBasic.m_Amount = _T("10000"); // �������Ϊɾ�������ݵı�־��
		setDayLineBasic.Update();
		setDayLineBasic.m_pDatabase->CommitTrans();
		setDayLineBasic.Close();

		dataChinaDayLine.SaveDB(_T("000001.SZ"));

		dataChinaDayLine.LoadDB(_T("000001.SZ"));
		EXPECT_EQ(dataChinaDayLine.GetData(0)->GetMarketDate(), 19910102) << "�´洢���ݵ�����";
		EXPECT_EQ(dataChinaDayLine.GetData(1)->GetMarketDate(), 19910103) << "�����ݵ���ʼ����";
		EXPECT_EQ(dataChinaDayLine.GetData(2)->GetMarketDate(), 19910104) << "�����ݵĵڶ������ڣ�֮ǰ�洢����һ������Ϊ19910103�������ѱ�ɾ��";

		// �ָ�ԭ״
		CSetDayLineBasicInfo setChinaStockDayLineBasic;
		setChinaStockDayLineBasic.m_strFilter = _T("[Symbol] = '000001.SZ'");
		setChinaStockDayLineBasic.m_strSort = _T("[Date]");
		setChinaStockDayLineBasic.Open();
		setChinaStockDayLineBasic.m_pDatabase->BeginTrans();
		EXPECT_EQ(setChinaStockDayLineBasic.m_Date, 19910102) << "�´洢���ݵ�����";
		setChinaStockDayLineBasic.Delete();
		setChinaStockDayLineBasic.m_pDatabase->CommitTrans();
		setChinaStockDayLineBasic.Close();
	}
}
