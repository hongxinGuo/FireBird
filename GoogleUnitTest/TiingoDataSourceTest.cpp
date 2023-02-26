#include"pch.h"

#include"GeneralCheck.h"
#include"FinnhubInquiryType.h"

#include"TiingoDataSource.h"
#include"WorldMarket.h"

#include "ProductTiingoStockSymbol.h"
#include"ProductTiingoStockDayLine.h"
#include"ProductDummy.h"

using namespace testing;

namespace FireBirdTest {
	class CTiingoDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CTiingoDataSource m_TiingoDataSource;
	};

	TEST_F(CTiingoDataSourceTest, TestIsUpdateStockSymbol) {
		EXPECT_TRUE(m_TiingoDataSource.IsUpdateStockSymbol());
		m_TiingoDataSource.SetUpdateStockSymbol(false);
		EXPECT_FALSE(m_TiingoDataSource.IsUpdateStockSymbol());
		m_TiingoDataSource.SetUpdateStockSymbol(true);
		EXPECT_TRUE(m_TiingoDataSource.IsUpdateStockSymbol());
	}

	TEST_F(CTiingoDataSourceTest, TestIsUpdateCryptoSymbol) {
		EXPECT_TRUE(m_TiingoDataSource.IsUpdateCryptoSymbol());
		m_TiingoDataSource.SetUpdateCryptoSymbol(false);
		EXPECT_FALSE(m_TiingoDataSource.IsUpdateCryptoSymbol());
		m_TiingoDataSource.SetUpdateCryptoSymbol(true);
		EXPECT_TRUE(m_TiingoDataSource.IsUpdateCryptoSymbol());
	}

	TEST_F(CTiingoDataSourceTest, TestIsUpdateDayLine) {
		EXPECT_TRUE(m_TiingoDataSource.IsUpdateDayLine());
		m_TiingoDataSource.SetUpdateDayLine(false);
		EXPECT_FALSE(m_TiingoDataSource.IsUpdateDayLine());
		m_TiingoDataSource.SetUpdateDayLine(true);
		EXPECT_TRUE(m_TiingoDataSource.IsUpdateDayLine());
	}

	TEST_F(CTiingoDataSourceTest, TestUpdateStatus) {
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateStockSymbol());
		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		gl_pTiingoDataSource->SetCurrentInquiry(p);

		p->SetProductType(STOCK_SYMBOLS_);
		gl_pTiingoDataSource->UpdateStatus();
		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateStockSymbol());
		gl_pTiingoDataSource->SetUpdateStockSymbol(true);

		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateCryptoSymbol());
		p->SetProductType(CRYPTO_SYMBOLS_);
		gl_pTiingoDataSource->UpdateStatus();
		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateCryptoSymbol());
		gl_pTiingoDataSource->SetUpdateCryptoSymbol(true);
	}

	TEST_F(CTiingoDataSourceTest, TestInquireTiingoCompanySymbol) {
		m_TiingoDataSource.SetUpdateStockSymbol(false);
		EXPECT_FALSE(m_TiingoDataSource.InquireCompanySymbol()) << "TiingoCompanySymbol Updated";

		m_TiingoDataSource.SetUpdateStockSymbol(true);
		m_TiingoDataSource.SetInquiring(true);
		EXPECT_FALSE(m_TiingoDataSource.InquireCompanySymbol());

		m_TiingoDataSource.SetInquiring(false);
		EXPECT_TRUE(m_TiingoDataSource.InquireCompanySymbol());
		EXPECT_TRUE(m_TiingoDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_TiingoDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockSymbol"));
		EXPECT_TRUE(m_TiingoDataSource.IsUpdateStockSymbol()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Tiingo stock symbol�Ѹ���"));
	}

	TEST_F(CTiingoDataSourceTest, TestInquireTiingoCryptoSymbol) {
		m_TiingoDataSource.SetUpdateCryptoSymbol(false);
		EXPECT_FALSE(m_TiingoDataSource.InquireCryptoSymbol()) << "TiingoCompanySymbol Updated";

		m_TiingoDataSource.SetUpdateCryptoSymbol(true);
		m_TiingoDataSource.SetInquiring(true);
		EXPECT_FALSE(m_TiingoDataSource.InquireCryptoSymbol());

		m_TiingoDataSource.SetInquiring(false);
		EXPECT_TRUE(m_TiingoDataSource.InquireCryptoSymbol());
		EXPECT_TRUE(m_TiingoDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = m_TiingoDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoCryptoSymbol"));
		EXPECT_TRUE(m_TiingoDataSource.IsUpdateStockSymbol()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Tiingo crypto symbol�Ѹ���"));
	}

	TEST_F(CTiingoDataSourceTest, TestInquireTiingoDayLine) {
		CWorldStockPtr pStock;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetChosenStock(1)->SetDayLineNeedUpdate(true);
		gl_pWorldMarket->GetChosenStock(3)->SetDayLineNeedUpdate(true);
		m_TiingoDataSource.SetUpdateDayLine(false);
		EXPECT_FALSE(m_TiingoDataSource.InquireDayLine()) << "DayLine Updated";

		m_TiingoDataSource.SetUpdateDayLine(true);
		m_TiingoDataSource.SetInquiring(true);
		EXPECT_FALSE(m_TiingoDataSource.InquireDayLine());

		m_TiingoDataSource.SetInquiring(false);
		EXPECT_TRUE(m_TiingoDataSource.InquireDayLine());
		EXPECT_TRUE(m_TiingoDataSource.IsInquiring());
		auto lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChosenStock(1)->GetSymbol());
		CVirtualProductWebDataPtr p = m_TiingoDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "��һ������ѯ��Ʊλ���ǵ�һ����Ʊ";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(1)->IsDayLineNeedUpdate()) << "�����ݴ�������ô˱�ʶ";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(3)->IsDayLineNeedUpdate());

		gl_pWorldMarket->GetChosenStock(1)->SetDayLineNeedUpdate(false);
		m_TiingoDataSource.SetInquiring(false);
		EXPECT_TRUE(m_TiingoDataSource.InquireDayLine());
		lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChosenStock(3)->GetSymbol());
		p = m_TiingoDataSource.GetCurrentProduct();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "�ڶ�������ѯ��Ʊλ���ǵ�������Ʊ";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(3)->IsDayLineNeedUpdate()) << "�����ݴ�������ô˱�ʶ";

		gl_pWorldMarket->GetChosenStock(3)->SetDayLineNeedUpdate(false);
		m_TiingoDataSource.SetInquiring(false);
		EXPECT_FALSE(m_TiingoDataSource.InquireDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_FALSE(m_TiingoDataSource.IsUpdateDayLine()) << "��Ʊ����ѯ����";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("�����г���ѡ��Ʊ������ʷ���ݸ������"));

		// �ָ�ԭ״
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived01) {
		m_TiingoDataSource.SetCurrentInquiry(nullptr);
		m_TiingoDataSource.SetInquiring(true);

		EXPECT_FALSE(m_TiingoDataSource.ProcessWebDataReceived()) << "CurrentInquiryΪnullptr";

		// �ָ�ԭ״
		m_TiingoDataSource.SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived02) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();

		m_TiingoDataSource.SetCurrentInquiry(p);
		m_TiingoDataSource.SetInquiring(true);

		EXPECT_FALSE(m_TiingoDataSource.ProcessWebDataReceived()) << "DataReceived��ʶΪ��";

		// �ָ�ԭ״
		m_TiingoDataSource.SetCurrentInquiry(nullptr);
		m_TiingoDataSource.SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived03) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();

		m_TiingoDataSource.SetCurrentInquiry(p);
		m_TiingoDataSource.SetInquiring(true);
		while (m_TiingoDataSource.GetReceivedDataSize() > 0) m_TiingoDataSource.GetReceivedData();

		EXPECT_FALSE(m_TiingoDataSource.ProcessWebDataReceived()) << "Received����Ϊ��";

		// �ָ�ԭ״
		m_TiingoDataSource.SetCurrentInquiry(nullptr);
		m_TiingoDataSource.SetInquiring(false);
	}

	TEST_F(CTiingoDataSourceTest, TestProcessTiingoWebDataReceived04) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		const auto pData = make_shared<CWebData>();
		pData->SetStockCode(_T("{}"));
		pData->CreateJson();
		pData->SetParsed(true);

		m_TiingoDataSource.StoreReceivedData(pData);
		m_TiingoDataSource.SetCurrentInquiry(p);
		m_TiingoDataSource.SetInquiring(true);

		EXPECT_TRUE(m_TiingoDataSource.ProcessWebDataReceived());
		// �ָ�ԭ״
		m_TiingoDataSource.SetCurrentInquiry(nullptr);
	}
}
