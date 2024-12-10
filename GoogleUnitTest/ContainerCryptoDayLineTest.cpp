#include"pch.h"

#include"GeneralCheck.h"

import FireBird.HistoryCandle.DayLine;
import FireBird.Container.CryptoDayLine;
#include"SetCryptoDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CDataCryptoDayLineTest : public ::testing::Test {
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
		CContainerCryptoDayLine m_dataCryptoDayLine;
	};

	TEST_F(CDataCryptoDayLineTest, TestSaveDB) {
		vector<CDayLinePtr> vDayLine;

		const CDayLinePtr pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20200101); // �������ݿ������������Ϊ20200817���ʴ�����λ����ǰ��
		pDayLine->SetStockSymbol(_T("BINANCE:USDTUAH"));
		pDayLine->SetClose(100);
		vDayLine.push_back(pDayLine);
		m_dataCryptoDayLine.UpdateData(vDayLine);

		m_dataCryptoDayLine.SaveDB(_T("BINANCE:USDTUAH"));

		m_dataCryptoDayLine.LoadDB(_T("BINANCE:USDTUAH"));
		EXPECT_EQ(m_dataCryptoDayLine.GetData(0)->GetMarketDate(), 20200101) << "�´洢���ݵ�����";

		// �ָ�ԭ״
		CSetCryptoDayLine setCryptoDayLineBasic;
		setCryptoDayLineBasic.m_strFilter = _T("[Symbol] = 'BINANCE:USDTUAH'");
		setCryptoDayLineBasic.m_strSort = _T("[Date]");
		setCryptoDayLineBasic.Open();
		setCryptoDayLineBasic.m_pDatabase->BeginTrans();
		EXPECT_EQ(setCryptoDayLineBasic.m_Date, 20200101) << "�´洢���ݵ�����";
		setCryptoDayLineBasic.Delete();
		setCryptoDayLineBasic.m_pDatabase->CommitTrans();
		setCryptoDayLineBasic.Close();
	}
}
