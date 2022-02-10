#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"DataChinaWeekLine.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CDataChinaWeekLineTest : public ::testing::Test {
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
		CDataChinaWeekLine m_dataChinaStockWeekLine;
	};

	TEST_F(CDataChinaWeekLineTest, TestSaveDB) {
		vector<CWeekLinePtr> vWeekLine;
		CWeekLinePtr pWeekLine = nullptr;

		pWeekLine = make_shared<CWeekLine>();
		pWeekLine->SetDate(19901224); // �������ݿ���000003.SZ���������Ϊ19901231���ʴ�����λ����ǰ��
		pWeekLine->SetStockSymbol(_T("000003.SZ"));
		pWeekLine->SetClose(100);
		vWeekLine.push_back(pWeekLine);
		m_dataChinaStockWeekLine.UpdateData(vWeekLine);

		m_dataChinaStockWeekLine.SaveDB(_T("000003.SZ"));

		m_dataChinaStockWeekLine.LoadDB(_T("000003.SZ"));
		EXPECT_EQ(m_dataChinaStockWeekLine.GetData(0)->GetMarketDate(), 19901224) << "�´洢���ݵ�����";

		// �ָ�ԭ״
		CSetWeekLineBasicInfo setChinaStockWeekLineBasic;
		setChinaStockWeekLineBasic.m_strFilter = _T("[Symbol] = '000003.SZ'");
		setChinaStockWeekLineBasic.m_strSort = _T("[Date]");
		setChinaStockWeekLineBasic.Open();
		setChinaStockWeekLineBasic.m_pDatabase->BeginTrans();
		EXPECT_EQ(setChinaStockWeekLineBasic.m_Date, 19901224) << "�´洢���ݵ�����";
		setChinaStockWeekLineBasic.Delete();
		setChinaStockWeekLineBasic.m_pDatabase->CommitTrans();
		setChinaStockWeekLineBasic.Close();
	}
}