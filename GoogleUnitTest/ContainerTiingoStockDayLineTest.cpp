#include"pch.h"

#include"GeneralCheck.h"

import FireBird.HistoryCandle.DayLine;
import FireBird.Container.Tiingo.StockDayLine;
#include "ProductTiingoStockDayLine.h"

#include"SEtTiingoStockDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CDataTiingoStockDayLineTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CContainerTiingoStockDayLine m_dataTiingoStockDayLine;
	};

	TEST_F(CDataTiingoStockDayLineTest, TestSaveDB) {
		vector<CDayLinePtr> vDayLine;
		CTiingoDayLinesPtr dayLinesPtr = make_shared<vector<CTiingoDayLinePtr>>();

		CTiingoDayLinePtr pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetDate(20241103); // ���Կ���û�У�����
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetClose(100);
		dayLinesPtr->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetDate(20241107); // ���Կ�����
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetClose(100);
		dayLinesPtr->push_back(pDayLine);
		pDayLine = make_shared<CTiingoDayLine>();
		pDayLine->SetDate(20241111); // ���Կ��е�������������Ϊ20241108��������λ�����
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetClose(100);
		dayLinesPtr->push_back(pDayLine);
		m_dataTiingoStockDayLine.UpdateData(dayLinesPtr);

		m_dataTiingoStockDayLine.SaveDB(_T("A"));

		m_dataTiingoStockDayLine.LoadDB(_T("A"));
		EXPECT_EQ(m_dataTiingoStockDayLine.GetData(m_dataTiingoStockDayLine.Size() - 1)->GetMarketDate(),
		          20241111) << "�´洢����λ�����";

		// �ָ�ԭ״
		CSetTiingoStockDayLine setTiingoStockDayLineBasic;
		setTiingoStockDayLineBasic.m_strFilter = _T("[Date] = 20241111");
		setTiingoStockDayLineBasic.Open();
		setTiingoStockDayLineBasic.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setTiingoStockDayLineBasic.IsEOF()) << "�´洢���ݵ�����";
		EXPECT_STREQ(setTiingoStockDayLineBasic.m_Symbol, _T("A"));
		while (!setTiingoStockDayLineBasic.IsEOF()) {
			setTiingoStockDayLineBasic.Delete();
			setTiingoStockDayLineBasic.MoveNext();
		}
		setTiingoStockDayLineBasic.m_pDatabase->CommitTrans();
		setTiingoStockDayLineBasic.Close();

		setTiingoStockDayLineBasic.m_strFilter = _T("[Date] = 20241103");
		setTiingoStockDayLineBasic.Open();
		setTiingoStockDayLineBasic.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setTiingoStockDayLineBasic.IsEOF()) << "�������ݵ�����";
		EXPECT_STREQ(setTiingoStockDayLineBasic.m_Symbol, _T("A"));
		while (!setTiingoStockDayLineBasic.IsEOF()) {
			setTiingoStockDayLineBasic.Delete();
			setTiingoStockDayLineBasic.MoveNext();
		}
		setTiingoStockDayLineBasic.m_pDatabase->CommitTrans();
		setTiingoStockDayLineBasic.Close();
	}
}
