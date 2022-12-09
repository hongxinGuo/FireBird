#include"pch.h"

#include"GeneralCheck.h"

#include"DayLine.h"
#include"DataWorldStockDayLine.h"

#include"SEtWorldStockDayLine.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CDataWorldStockDayLineTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearu
			GeneralCheck();
		}

	protected:
		CDataWorldStockDayLine m_dataWorldStockDayLine;
	};

	TEST_F(CDataWorldStockDayLineTest, TestSaveDB) {
		vector<CDayLinePtr> vDayLine;
		CDayLinePtr pDayLine = nullptr;

		pDayLine = make_shared<CDayLine>();
		pDayLine->SetDate(20220101); // ���Կ��е�������������Ϊ20210330��������λ�����
		pDayLine->SetStockSymbol(_T("A"));
		pDayLine->SetClose(100);
		vDayLine.push_back(pDayLine);
		m_dataWorldStockDayLine.UpdateData(vDayLine);

		m_dataWorldStockDayLine.SaveDB(_T("A"));

		m_dataWorldStockDayLine.LoadDB(_T("A"));
		EXPECT_EQ(m_dataWorldStockDayLine.GetData(m_dataWorldStockDayLine.Size() - 1)->GetMarketDate(),
		          20220101) << "�´洢����λ�����";

		// �ָ�ԭ״
		CSetWorldStockDayLine setWorldStockDayLineBasic;
		setWorldStockDayLineBasic.m_strFilter = _T("[Date] = 20220101");
		setWorldStockDayLineBasic.Open();
		setWorldStockDayLineBasic.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setWorldStockDayLineBasic.IsEOF()) << "�´洢���ݵ�����";
		EXPECT_STREQ(setWorldStockDayLineBasic.m_Symbol, _T("A"));
		while (!setWorldStockDayLineBasic.IsEOF()) {
			setWorldStockDayLineBasic.Delete();
			setWorldStockDayLineBasic.MoveNext();
		}
		setWorldStockDayLineBasic.m_pDatabase->CommitTrans();
		setWorldStockDayLineBasic.Close();
	}
}
