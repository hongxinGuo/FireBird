#include"pch.h"

#include"GeneralCheck.h"

#include"ContainerTiingoFundamentalDefinition.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerTiingoFundamentalDefinitionTest : public ::testing::Test {
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
		CContainerTiingoFundamentalDefinition m_dataTiingoFundamentalDefinition;
	};

	TEST_F(CContainerTiingoFundamentalDefinitionTest, TestInitialize) {
		EXPECT_EQ(m_dataTiingoFundamentalDefinition.GetTotalDefinition(), 0);
		EXPECT_FALSE(m_dataTiingoFundamentalDefinition.IsUpdateDB());
	}

	TEST_F(CContainerTiingoFundamentalDefinitionTest, TestAddDelete) {
		m_dataTiingoFundamentalDefinition.LoadDB();
		EXPECT_EQ(m_dataTiingoFundamentalDefinition.GetTotalDefinition(), 85) << "���Կ�����85����������";
		EXPECT_FALSE(m_dataTiingoFundamentalDefinition.IsUpdateDB());

		CTiingoFundamentalDefinitionPtr pDefinition = make_shared<CTiingoFundamentalDefinition>();
		pDefinition->m_strDataCode = _T("AAA"); // �´���
		pDefinition->m_strName = _T("abc");

		m_dataTiingoFundamentalDefinition.Add(pDefinition);
		EXPECT_EQ(m_dataTiingoFundamentalDefinition.GetTotalDefinition(), 86);
		EXPECT_TRUE(m_dataTiingoFundamentalDefinition.IsUpdateDB());
		m_dataTiingoFundamentalDefinition.SetUpdateDB(false);

		CTiingoFundamentalDefinitionPtr pDefinition2 = make_shared<CTiingoFundamentalDefinition>();
		pDefinition2->m_strDataCode = _T("AAA"); // ���д���
		pDefinition2->m_strName = _T("abc");

		m_dataTiingoFundamentalDefinition.Add(pDefinition2);
		EXPECT_EQ(m_dataTiingoFundamentalDefinition.GetTotalDefinition(), 86) << "��������д���";
		EXPECT_FALSE(m_dataTiingoFundamentalDefinition.IsUpdateDB());

		m_dataTiingoFundamentalDefinition.Delete(pDefinition);
		EXPECT_EQ(m_dataTiingoFundamentalDefinition.GetTotalDefinition(), 85) << "ɾ���Ѵ��ڴ���";
		EXPECT_TRUE(m_dataTiingoFundamentalDefinition.IsUpdateDB());
		m_dataTiingoFundamentalDefinition.SetUpdateDB(false);

		pDefinition2->m_strDataCode = _T("AAA"); // ��ɾ������
		pDefinition2->m_strName = _T("abc");

		m_dataTiingoFundamentalDefinition.Delete(pDefinition);
		EXPECT_EQ(m_dataTiingoFundamentalDefinition.GetTotalDefinition(), 85) << "�����ڸô���";
		EXPECT_FALSE(m_dataTiingoFundamentalDefinition.IsUpdateDB());
	}

	TEST_F(CContainerTiingoFundamentalDefinitionTest, TestUpdateDB) {
		m_dataTiingoFundamentalDefinition.LoadDB();
		EXPECT_EQ(m_dataTiingoFundamentalDefinition.GetTotalDefinition(), 85) << "���Կ�����85����������";
		EXPECT_FALSE(m_dataTiingoFundamentalDefinition.IsUpdateDB());
		m_dataTiingoFundamentalDefinition.Delete(_T("rps"));
		m_dataTiingoFundamentalDefinition.Delete((_T("roa")));
		EXPECT_EQ(m_dataTiingoFundamentalDefinition.GetTotalDefinition(), 83) << "���Կ�����85����������";

		CTiingoFundamentalDefinitionPtr pDefinition = make_shared<CTiingoFundamentalDefinition>();
		pDefinition->m_strDataCode = _T("AAAA"); // �´���
		pDefinition->m_strName = _T("Test");

		m_dataTiingoFundamentalDefinition.Add(pDefinition);
		EXPECT_EQ(m_dataTiingoFundamentalDefinition.GetTotalDefinition(), 84);
		EXPECT_TRUE(m_dataTiingoFundamentalDefinition.IsUpdateDB());
		m_dataTiingoFundamentalDefinition.SetUpdateDB(false);

		pDefinition = make_shared<CTiingoFundamentalDefinition>();
		pDefinition->m_strDataCode = _T("AAAAA"); // �´���
		pDefinition->m_strName = _T("Test");

		m_dataTiingoFundamentalDefinition.Add(pDefinition);
		EXPECT_EQ(m_dataTiingoFundamentalDefinition.GetTotalDefinition(), 85);
		EXPECT_TRUE(m_dataTiingoFundamentalDefinition.IsUpdateDB());

		m_dataTiingoFundamentalDefinition.UpdateDB();

		CSetTiingoFundamentalDefinition setDefinition;
		setDefinition.m_strFilter = _T("[name] = 'Test'");
		setDefinition.Open();
		setDefinition.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setDefinition.IsEOF());
		setDefinition.Delete();
		setDefinition.MoveNext();
		setDefinition.Delete();
		setDefinition.MoveNext();
		EXPECT_TRUE(setDefinition.IsEOF());
		setDefinition.m_pDatabase->CommitTrans();
		setDefinition.Close();

		m_dataTiingoFundamentalDefinition.LoadDB();
		EXPECT_EQ(m_dataTiingoFundamentalDefinition.GetTotalDefinition(), 85);
	}
}
