#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoDataSource.h"
#include"WorldMarket.h"

#include"ProductDummy.h"

using namespace testing;

namespace FireBirdTest {
	class CTiingoDataSourceTest : public Test {
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

			m_pTiingoDataSource = make_shared<CTiingoDataSource>();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CTiingoDataSourcePtr m_pTiingoDataSource;
	};

	TEST_F(CTiingoDataSourceTest, TestIsUpdateStockSymbol) {
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateStockSymbol());
		m_pTiingoDataSource->SetUpdateStockSymbol(false);
		EXPECT_FALSE(m_pTiingoDataSource->IsUpdateStockSymbol());
		m_pTiingoDataSource->SetUpdateStockSymbol(true);
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateStockSymbol());
	}

	TEST_F(CTiingoDataSourceTest, TestIsUpdateCryptoSymbol) {
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateCryptoSymbol());
		m_pTiingoDataSource->SetUpdateCryptoSymbol(false);
		EXPECT_FALSE(m_pTiingoDataSource->IsUpdateCryptoSymbol());
		m_pTiingoDataSource->SetUpdateCryptoSymbol(true);
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateCryptoSymbol());
	}

	TEST_F(CTiingoDataSourceTest, TestIsUpdateDayLine) {
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateDayLine());
		m_pTiingoDataSource->SetUpdateDayLine(false);
		EXPECT_FALSE(m_pTiingoDataSource->IsUpdateDayLine());
		m_pTiingoDataSource->SetUpdateDayLine(true);
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateDayLine());
	}
	TEST_F(CTiingoDataSourceTest, TestInquireTiingoCompanySymbol) {
		m_pTiingoDataSource->SetUpdateStockSymbol(false);
		EXPECT_FALSE(m_pTiingoDataSource->InquireCompanySymbol()) << "TiingoCompanySymbol Updated";

		m_pTiingoDataSource->SetUpdateStockSymbol(true);
		m_pTiingoDataSource->SetInquiring(true);
		EXPECT_FALSE(m_pTiingoDataSource->InquireCompanySymbol());

		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->InquireCompanySymbol());
		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring());
		const CVirtualProductWebDataPtr p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockSymbol"));
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateStockSymbol()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Tiingo stock symbol�Ѹ���"));
	}

	TEST_F(CTiingoDataSourceTest, TestInquireTiingoCryptoSymbol) {
		m_pTiingoDataSource->SetUpdateCryptoSymbol(false);
		EXPECT_FALSE(m_pTiingoDataSource->InquireCryptoSymbol()) << "TiingoCompanySymbol Updated";

		m_pTiingoDataSource->SetUpdateCryptoSymbol(true);
		m_pTiingoDataSource->SetInquiring(true);
		EXPECT_FALSE(m_pTiingoDataSource->InquireCryptoSymbol());

		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->InquireCryptoSymbol());
		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring());
		const CVirtualProductWebDataPtr p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoCryptoSymbol"));
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateStockSymbol()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Tiingo crypto symbol�Ѹ���"));
	}

	TEST_F(CTiingoDataSourceTest, TestInquireTiingoDayLine) {
		CWorldStockPtr pStock;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_dataContainerChosenWorldStock.GetStock(1)->SetDayLineNeedUpdate(true);
		gl_dataContainerChosenWorldStock.GetStock(3)->SetDayLineNeedUpdate(true);
		m_pTiingoDataSource->SetUpdateDayLine(false);
		EXPECT_FALSE(m_pTiingoDataSource->InquireDayLine()) << "DayLine Updated";

		m_pTiingoDataSource->SetUpdateDayLine(true);
		m_pTiingoDataSource->SetInquiring(true);
		EXPECT_FALSE(m_pTiingoDataSource->InquireDayLine());

		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->InquireDayLine());
		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring());
		auto lStockIndex = gl_dataContainerFinnhubStock.GetOffset(gl_dataContainerChosenWorldStock.GetStock(1)->GetSymbol());
		CVirtualProductWebDataPtr p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "��һ������ѯ��Ʊλ���ǵ�һ����Ʊ";
		EXPECT_TRUE(gl_dataContainerChosenWorldStock.GetStock(1)->IsDayLineNeedUpdate()) << "�����ݴ�������ô˱�ʶ";
		EXPECT_TRUE(gl_dataContainerChosenWorldStock.GetStock(3)->IsDayLineNeedUpdate());

		gl_dataContainerChosenWorldStock.GetStock(1)->SetDayLineNeedUpdate(false);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->InquireDayLine());
		lStockIndex = gl_dataContainerFinnhubStock.GetOffset(gl_dataContainerChosenWorldStock.GetStock(3)->GetSymbol());
		p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "�ڶ�������ѯ��Ʊλ���ǵ�������Ʊ";
		EXPECT_TRUE(gl_dataContainerChosenWorldStock.GetStock(3)->IsDayLineNeedUpdate()) << "�����ݴ�������ô˱�ʶ";

		gl_dataContainerChosenWorldStock.GetStock(3)->SetDayLineNeedUpdate(false);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_FALSE(m_pTiingoDataSource->InquireDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_pTiingoDataSource->IsUpdateDayLine()) << "��Ʊ����ѯ����";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("�����г���ѡ��Ʊ������ʷ���ݸ������"));

		// �ָ�ԭ״
		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(i);
			pStock->SetDayLineNeedUpdate(true);
		}
	}
}
