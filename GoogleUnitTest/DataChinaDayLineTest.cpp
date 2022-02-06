#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"DataChinaDayLine.h"
using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CDataChinaDayLineTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) { // ��������ĳ�ʼ������
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
		}
	};

	TEST_F(CDataChinaDayLineTest, TestUpdateData1) {
		CDataChinaDayLine dataChinaDayLine;

		vector<CDayLinePtr> vDayLine;
		CDayLinePtr pDayLine = nullptr;

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200101); // ������
		pDayLine->SetHigh(10010);
		pDayLine->SetLow(9910);
		pDayLine->SetClose(10001);
		pDayLine->SetLastClose(10005);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200102); // ������
		pDayLine->SetHigh(10020);
		pDayLine->SetLow(9920);
		pDayLine->SetClose(10002);
		pDayLine->SetLastClose(10001);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200103); // ������
		pDayLine->SetHigh(10030);
		pDayLine->SetLow(9930);
		pDayLine->SetClose(10003);
		pDayLine->SetLastClose(10002);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		dataChinaDayLine.SetDataLoaded(false);

		dataChinaDayLine.UpdateData(vDayLine, false);

		EXPECT_THAT(dataChinaDayLine.IsDataLoaded(), IsTrue());
		EXPECT_THAT(dataChinaDayLine.GetDataSize(), 3);
		EXPECT_THAT(dataChinaDayLine.GetData(0)->GetClose(), 10001) << "����洢����һ�����ݵ����̼�";
		EXPECT_THAT(dataChinaDayLine.GetData(2)->GetClose(), 10003) << "����洢�����������ݵ����̼�";
	}

	TEST_F(CDataChinaDayLineTest, TestUpdateData2) {
		CDataChinaDayLine dataChinaDayLine;

		vector<CDayLinePtr> vDayLine;
		CDayLinePtr pDayLine = nullptr;

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200101); // ������
		pDayLine->SetHigh(10010);
		pDayLine->SetLow(9910);
		pDayLine->SetClose(10001);
		pDayLine->SetLastClose(10005);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200102); // ������
		pDayLine->SetHigh(10020);
		pDayLine->SetLow(9920);
		pDayLine->SetClose(10002);
		pDayLine->SetLastClose(10001);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200103); // ������
		pDayLine->SetHigh(10030);
		pDayLine->SetLow(9930);
		pDayLine->SetClose(10003);
		pDayLine->SetLastClose(10002);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		dataChinaDayLine.SetDataLoaded(false);

		dataChinaDayLine.UpdateData(vDayLine, true); // ����洢

		EXPECT_THAT(dataChinaDayLine.IsDataLoaded(), IsTrue());
		EXPECT_THAT(dataChinaDayLine.GetDataSize(), 3);
		EXPECT_THAT(dataChinaDayLine.GetData(0)->GetClose(), 10003) << "����洢�����������ݵ����̼�";
		EXPECT_THAT(dataChinaDayLine.GetData(2)->GetClose(), 10001) << "����洢����һ�����ݵ����̼�";
	}

	TEST_F(CDataChinaDayLineTest, TestCreateNewWeekLine) {
		CDataChinaDayLine dataChinaDayLine;

		vector<CDayLinePtr> vDayLine;
		CDayLinePtr pDayLine = nullptr;
		CWeekLinePtr pWeekLine = nullptr;
		long lCurrentDayLinePos = 0;

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200101); // ������
		pDayLine->SetHigh(10010);
		pDayLine->SetLow(9910);
		pDayLine->SetClose(10001);
		pDayLine->SetLastClose(10000); // ��һ������Ϊ0
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200102); // ������
		pDayLine->SetHigh(10020);
		pDayLine->SetLow(9920);
		pDayLine->SetClose(10002);
		pDayLine->SetLastClose(10001);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200103); // ������
		pDayLine->SetHigh(10030);
		pDayLine->SetLow(9930);
		pDayLine->SetClose(10003);
		pDayLine->SetLastClose(10002);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

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

		dataChinaDayLine.UpdateData(vDayLine, false);
		pWeekLine = dataChinaDayLine.CreateNewWeekLine(lCurrentDayLinePos);

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
		CDayLinePtr pDayLine = nullptr;

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200101); // ������
		pDayLine->SetHigh(10010);
		pDayLine->SetLow(9910);
		pDayLine->SetClose(10001);
		pDayLine->SetLastClose(10000);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200102); // ������
		pDayLine->SetHigh(10020);
		pDayLine->SetLow(9920);
		pDayLine->SetClose(10002);
		pDayLine->SetLastClose(10001);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200103); // ������
		pDayLine->SetHigh(10030);
		pDayLine->SetLow(9930);
		pDayLine->SetClose(10003);
		pDayLine->SetLastClose(10002);
		pDayLine->SetVolume(10000);
		vDayLine.push_back(pDayLine);

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

		dataChinaDayLine.UpdateData(vDayLine, false);
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
}