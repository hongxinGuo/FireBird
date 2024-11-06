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

	TEST_F(CTiingoDataSourceTest, TestIsUpdateFinancialState) {
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateFinancialState());
		m_pTiingoDataSource->SetUpdateFinancialState(false);
		EXPECT_FALSE(m_pTiingoDataSource->IsUpdateFinancialState());
		m_pTiingoDataSource->SetUpdateFinancialState(true);
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateFinancialState());
	}

	TEST_F(CTiingoDataSourceTest, TestIsUpdateDayLine) {
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateDayLine());
		m_pTiingoDataSource->SetUpdateDayLine(false);
		EXPECT_FALSE(m_pTiingoDataSource->IsUpdateDayLine());
		m_pTiingoDataSource->SetUpdateDayLine(true);
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateDayLine());
	}

	TEST_F(CTiingoDataSourceTest, TestIsAErrorMessageData1) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(_T("abcde")); // �޹ؽ�Ҫ
		m_pTiingoDataSource->SetHTTPStatusCode(200); // ����
		auto pProduct = make_shared<CProductDummy>();
		m_pTiingoDataSource->SetCurrentInquiry(pProduct);

		EXPECT_EQ(m_pTiingoDataSource->IsAErrorMessageData(pWebData), ERROR_NO_ERROR__);
	}

	TEST_F(CTiingoDataSourceTest, TestIsAErrorMessageData2) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(_T("{\"detail\":\"You do not have permission to access the News API\"}")); // ��Ȩ����
		m_pTiingoDataSource->SetHTTPStatusCode(403); // error
		auto pProduct = make_shared<CProductDummy>();
		pProduct->SetReceivedDataStatus(GOOD_DATA__);
		m_pTiingoDataSource->SetCurrentInquiry(pProduct);

		EXPECT_EQ(m_pTiingoDataSource->IsAErrorMessageData(pWebData), ERROR_TIINGO_NO_RIGHT_TO_ACCESS__);
		EXPECT_EQ(pProduct->GetReceivedDataStatus(), NO_ACCESS_RIGHT_);
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);

		// �ָ�ԭ״
		gl_systemMessage.PopInnerSystemInformationMessage();
	}
	TEST_F(CTiingoDataSourceTest, TestIsAErrorMessageData3) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(_T("{\"detail\":\"Please supply a token\"}")); // ��Ȩ����
		m_pTiingoDataSource->SetHTTPStatusCode(403); // ����
		auto pProduct = make_shared<CProductDummy>();
		pProduct->SetReceivedDataStatus(GOOD_DATA__);
		m_pTiingoDataSource->SetCurrentInquiry(pProduct);

		EXPECT_EQ(m_pTiingoDataSource->IsAErrorMessageData(pWebData), ERROR_TIINGO_MISSING_API_KEY__);
		EXPECT_EQ(pProduct->GetReceivedDataStatus(), NO_ACCESS_RIGHT_);
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Tiingo missing API key"));

		// �ָ�ԭ״
		//gl_systemMessage.PopErrorMessage();
	}

	TEST_F(CTiingoDataSourceTest, TestIsAErrorMessageData4) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(_T("{\"detail\":\"Error: Free and Power plans are limited to the DOW 30. If you would like access to all supported tickers, then please E-mail support@tiingo.com to get the Fundamental Data API added as an add-on service.\"}")); // ��Ȩ����
		m_pTiingoDataSource->SetHTTPStatusCode(403); // ����
		auto pProduct = make_shared<CProductDummy>();
		pProduct->SetReceivedDataStatus(GOOD_DATA__);
		m_pTiingoDataSource->SetCurrentInquiry(pProduct);

		EXPECT_EQ(m_pTiingoDataSource->IsAErrorMessageData(pWebData), ERROR_TIINGO_ADD_ON_PERMISSION_NEEDED__);
		EXPECT_EQ(pProduct->GetReceivedDataStatus(), NO_ACCESS_RIGHT_);
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 0);
		EXPECT_FALSE(gl_systemConfiguration.IsTiingoAccountAddOnPaid());

		// �ָ�ԭ״
		gl_systemConfiguration.SetTiingoAccountAddOnPaid(true);
		//gl_systemMessage.PopInnerSystemInformationMessage();
	}

	TEST_F(CTiingoDataSourceTest, TestIsAErrorMessageData5) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(_T("{\"detail\":\"Not handled\"}")); // ��Ȩ����
		m_pTiingoDataSource->SetHTTPStatusCode(403); // ����
		auto pProduct = make_shared<CProductDummy>();
		pProduct->SetReceivedDataStatus(GOOD_DATA__);
		m_pTiingoDataSource->SetCurrentInquiry(pProduct);

		EXPECT_EQ(m_pTiingoDataSource->IsAErrorMessageData(pWebData), ERROR_TIINGO_NOT_HANDLED__);
		//EXPECT_EQ(pProduct->GetReceivedDataStatus(), NO_ACCESS_RIGHT_);
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);

		// �ָ�ԭ״
		gl_systemMessage.PopInnerSystemInformationMessage();
	}

	TEST_F(CTiingoDataSourceTest, TestGenerateTiingoCompanySymbol) {
		m_pTiingoDataSource->SetUpdateStockSymbol(false);
		EXPECT_FALSE(m_pTiingoDataSource->GenerateCompanySymbol()) << "TiingoCompanySymbol Updated";

		m_pTiingoDataSource->SetUpdateStockSymbol(true);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->GenerateCompanySymbol());
		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring());
		const CVirtualProductWebDataPtr p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockProfile"));
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateStockSymbol()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
	}

	TEST_F(CTiingoDataSourceTest, TestGenerateTiingoCryptoSymbol) {
		m_pTiingoDataSource->SetUpdateCryptoSymbol(false);
		EXPECT_FALSE(m_pTiingoDataSource->GenerateCryptoSymbol()) << "TiingoCompanySymbol Updated";

		m_pTiingoDataSource->SetUpdateCryptoSymbol(true);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->GenerateCryptoSymbol());
		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring());
		const CVirtualProductWebDataPtr p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoCryptoSymbol"));
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateStockSymbol()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
	}

	TEST_F(CTiingoDataSourceTest, TestGenerateTiingoFundamentalDefinition) {
		m_pTiingoDataSource->SetUpdateFundamentalDefinition(false);
		EXPECT_FALSE(m_pTiingoDataSource->GenerateFundamentalDefinition()) << "TiingoCompanySymbol Updated";

		m_pTiingoDataSource->SetUpdateFundamentalDefinition(true);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->GenerateFundamentalDefinition());
		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring());
		const CVirtualProductWebDataPtr p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoFundamentalDefinition"));
		EXPECT_TRUE(m_pTiingoDataSource->IsUpdateStockSymbol()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
	}

	TEST_F(CTiingoDataSourceTest, TestGenerateTiingoDayLine) {
		CTiingoStockPtr pStock;

		EXPECT_FALSE(gl_systemConfiguration.IsUpdateDB());

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_dataContainerTiingoStock.Size(); i++) {
			pStock = gl_dataContainerTiingoStock.GetStock(i);
			pStock->SetUpdateDayLine(false);
		}
		gl_dataContainerTiingoStock.GetStock(1)->SetUpdateDayLine(true);
		gl_dataContainerTiingoStock.GetStock(2)->SetUpdateDayLine(true);
		m_pTiingoDataSource->SetUpdateDayLine(false);
		EXPECT_FALSE(m_pTiingoDataSource->GenerateDayLine()) << "DayLine Updated";

		m_pTiingoDataSource->SetUpdateDayLine(true);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->GenerateDayLine());
		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring());
		//auto lStockIndex = gl_dataContainerTiingoStock.GetOffset(tiingoSymbol1);
		CVirtualProductWebDataPtr p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockDayLine"));
		//EXPECT_EQ(p->GetIndex(), lStockIndex) << "��һ������ѯ��Ʊλ���ǵ�һ����Ʊ";
		EXPECT_TRUE(gl_dataContainerTiingoStock.GetStock(1)->IsUpdateDayLine()) << "�����ݴ�������ô˱�ʶ";
		EXPECT_TRUE(gl_dataContainerTiingoStock.GetStock(2)->IsUpdateDayLine());

		gl_dataContainerTiingoStock.GetStock(1)->SetUpdateDayLine(false);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->GenerateDayLine());
		//lStockIndex = gl_dataContainerFinnhubStock.GetOffset(tiingoSymbol2);
		p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockDayLine"));
		//EXPECT_EQ(p->GetIndex(), lStockIndex) << "�ڶ�������ѯ��Ʊλ���ǵ�������Ʊ";
		EXPECT_TRUE(gl_dataContainerTiingoStock.GetStock(2)->IsUpdateDayLine()) << "�����ݴ�������ô˱�ʶ";

		gl_dataContainerTiingoStock.GetStock(2)->SetUpdateDayLine(false);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_FALSE(m_pTiingoDataSource->GenerateDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_pTiingoDataSource->IsUpdateDayLine()) << "��Ʊ����ѯ����";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Tiingo��Ʊ������ʷ���ݸ������"));

		// �ָ�ԭ״
		for (int i = 0; i < gl_dataContainerTiingoStock.Size(); i++) {
			pStock = gl_dataContainerTiingoStock.GetStock(i);
			pStock->SetUpdateDayLine(true);
		}

		for (int i = 0; i < gl_dataContainerFinnhubStock.Size(); i++) {
			auto pStock2 = gl_dataContainerFinnhubStock.GetStock(i);
			pStock2->SetUpdateDayLine(true);
		}

		gl_systemConfiguration.SetUpdateDB(false);
	}

	TEST_F(CTiingoDataSourceTest, TestGenerateTiingoFinancialStatement) {
		CTiingoStockPtr pStock;

		EXPECT_FALSE(gl_systemConfiguration.IsUpdateDB());

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_dataContainerTiingoStock.Size(); i++) {
			pStock = gl_dataContainerTiingoStock.GetStock(i);
			pStock->SetUpdateFinancialState(false);
		}
		gl_dataContainerTiingoStock.GetStock(1)->SetUpdateFinancialState(true);
		gl_dataContainerTiingoStock.GetStock(2)->SetUpdateFinancialState(true);
		m_pTiingoDataSource->SetUpdateFinancialState(false);
		EXPECT_FALSE(m_pTiingoDataSource->GenerateFinancialState()) << "FinancialState Updated";

		m_pTiingoDataSource->SetUpdateFinancialState(true);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->GenerateFinancialState());
		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring());
		//auto lStockIndex = gl_dataContainerTiingoStock.GetOffset(tiingoSymbol1);
		CVirtualProductWebDataPtr p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoFinancialState"));
		//EXPECT_EQ(p->GetIndex(), lStockIndex) << "��һ������ѯ��Ʊλ���ǵ�һ����Ʊ";
		EXPECT_TRUE(gl_dataContainerTiingoStock.GetStock(1)->IsUpdateFinancialState()) << "�����ݴ�������ô˱�ʶ";
		EXPECT_TRUE(gl_dataContainerTiingoStock.GetStock(2)->IsUpdateFinancialState());

		gl_dataContainerTiingoStock.GetStock(1)->SetUpdateFinancialState(false);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pTiingoDataSource->GenerateFinancialState());
		//lStockIndex = gl_dataContainerFinnhubStock.GetOffset(tiingoSymbol2);
		p = m_pTiingoDataSource->GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoFinancialState"));
		//EXPECT_EQ(p->GetIndex(), lStockIndex) << "�ڶ�������ѯ��Ʊλ���ǵ�������Ʊ";
		EXPECT_TRUE(gl_dataContainerTiingoStock.GetStock(2)->IsUpdateFinancialState()) << "�����ݴ�������ô˱�ʶ";

		gl_dataContainerTiingoStock.GetStock(2)->SetUpdateFinancialState(false);
		m_pTiingoDataSource->SetInquiring(false);
		EXPECT_FALSE(m_pTiingoDataSource->GenerateFinancialState()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_pTiingoDataSource->IsUpdateFinancialState()) << "��Ʊ����ѯ����";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Tiingo financial statements�������"));

		// �ָ�ԭ״
		for (int i = 0; i < gl_dataContainerTiingoStock.Size(); i++) {
			pStock = gl_dataContainerTiingoStock.GetStock(i);
			pStock->SetUpdateFinancialState(true);
		}

		gl_systemConfiguration.SetUpdateDB(false);
	}
}
