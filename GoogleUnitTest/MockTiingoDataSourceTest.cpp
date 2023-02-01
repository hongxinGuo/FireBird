#include"pch.h"

#include"GeneralCheck.h"
#include"FinnhubInquiryType.h"

#include"TiingoDataSource.h"
#include"WorldMarket.h"

#include "ProductTiingoStockSymbol.h"
#include"ProductTiingoStockDayLine.h"
#include"ProductDummy.h"

#include"MockTiingoDataSource.h"

using namespace testing;

namespace FireBirdTest {
	class CMockTiingoDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp

			GeneralCheck();
		}

	protected:
		CMockTiingoDataSource TiingoDataSource;
	};

	TEST_F(CMockTiingoDataSourceTest, TestUpdateStatus) {
		CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		TiingoDataSource.SetCurrentInquiry(p);

		p->SetProductType(STOCK_SYMBOLS_);
		TiingoDataSource.UpdateStatus();
		EXPECT_TRUE(TiingoDataSource.IsStockSymbolUpdated());
		TiingoDataSource.SetStockSymbolUpdated(false);

		p->SetProductType(CRYPTO_SYMBOLS_);
		TiingoDataSource.UpdateStatus();
		EXPECT_TRUE(TiingoDataSource.IsCryptoSymbolUpdated());
		TiingoDataSource.SetCryptoSymbolUpdated(false);
	}

	TEST_F(CMockTiingoDataSourceTest, TestInquireTiingoCompanySymbol) {
		TiingoDataSource.SetStockSymbolUpdated(true);
		EXPECT_FALSE(TiingoDataSource.InquireCompanySymbol()) << "TiingoCompanySymbol Updated";

		TiingoDataSource.SetStockSymbolUpdated(false);
		TiingoDataSource.SetInquiring(true);
		EXPECT_FALSE(TiingoDataSource.InquireCompanySymbol());

		TiingoDataSource.SetInquiring(false);
		EXPECT_TRUE(TiingoDataSource.InquireCompanySymbol());
		EXPECT_TRUE(TiingoDataSource.IsInquiring());
		CVirtualProductWebDataPtr p = TiingoDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockSymbol"));
		EXPECT_FALSE(TiingoDataSource.IsStockSymbolUpdated()) << "�˱�ʶ��Ҫ�ȴ��������ݺ�����";
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("Tiingo stock symbol�Ѹ���"));
	}

	TEST_F(CMockTiingoDataSourceTest, TestInquireTiingoDayLine) {
		CWorldStockPtr pStock;

		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(false);
		}
		gl_pWorldMarket->GetChosenStock(1)->SetDayLineNeedUpdate(true);
		gl_pWorldMarket->GetChosenStock(3)->SetDayLineNeedUpdate(true);
		TiingoDataSource.SetDayLineUpdated(true);
		EXPECT_FALSE(TiingoDataSource.InquireDayLine()) << "DayLine Updated";

		TiingoDataSource.SetDayLineUpdated(false);
		TiingoDataSource.SetInquiring(true);
		EXPECT_FALSE(TiingoDataSource.InquireDayLine());

		TiingoDataSource.SetInquiring(false);
		EXPECT_TRUE(TiingoDataSource.InquireDayLine());
		EXPECT_TRUE(TiingoDataSource.IsInquiring());
		auto lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChosenStock(1)->GetSymbol());
		CVirtualProductWebDataPtr p = TiingoDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "��һ������ѯ��Ʊλ���ǵ�һ����Ʊ";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(1)->IsDayLineNeedUpdate()) << "�����ݴ�������ô˱�ʶ";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(3)->IsDayLineNeedUpdate());

		gl_pWorldMarket->GetChosenStock(1)->SetDayLineNeedUpdate(false);
		TiingoDataSource.SetInquiring(false);
		EXPECT_TRUE(TiingoDataSource.InquireDayLine());
		lStockIndex = gl_pWorldMarket->GetStockIndex(gl_pWorldMarket->GetChosenStock(3)->GetSymbol());
		p = TiingoDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*p).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_EQ(p->GetIndex(), lStockIndex) << "�ڶ�������ѯ��Ʊλ���ǵ�������Ʊ";
		EXPECT_TRUE(gl_pWorldMarket->GetChosenStock(3)->IsDayLineNeedUpdate()) << "�����ݴ�������ô˱�ʶ";

		gl_pWorldMarket->GetChosenStock(3)->SetDayLineNeedUpdate(false);
		TiingoDataSource.SetInquiring(false);
		EXPECT_FALSE(TiingoDataSource.InquireDayLine()) << "�����β�ѯʱû���ҵ�����ѯ�Ĺ�Ʊ";
		EXPECT_TRUE(TiingoDataSource.IsDayLineUpdated()) << "��Ʊ����ѯ����";
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("�����г���ѡ��Ʊ������ʷ���ݸ������"));

		// �ָ�ԭ״
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CMockTiingoDataSourceTest, TestProcessTiingoInquiringMessage01) {
		while (TiingoDataSource.GetInquiryQueueSize() > 0) TiingoDataSource.GetInquiry();
		EXPECT_FALSE(TiingoDataSource.ProcessInquiringMessage());
	}

	TEST_F(CMockTiingoDataSourceTest, TestProcessTiingoInquiringMessage02) {
		const CVirtualProductWebDataPtr p = make_shared<CProductTiingoStockSymbol>();
		TiingoDataSource.StoreInquiry(p);
		EXPECT_EQ(TiingoDataSource.GetInquiryQueueSize(), 1);
		TiingoDataSource.SetInquiringWebData(true);
		TiingoDataSource.SetInquiring(true);
		EXPECT_FALSE(TiingoDataSource.ProcessInquiringMessage()) << "Tiingo web data��δ���ܵ�";
		EXPECT_TRUE(TiingoDataSource.IsInquiring()) << "û�д����ʴ˱�ʶû������";

		// �ָ�ԭ״
		TiingoDataSource.GetInquiry();
		TiingoDataSource.SetInquiring(false);
	}

	TEST_F(CMockTiingoDataSourceTest, TestParseTiingoInquiringMessage_STOCK_SYMBOLS_) {
		const CVirtualProductWebDataPtr p = make_shared<CProductTiingoStockSymbol>();
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		TiingoDataSource.StoreInquiry(p);
		EXPECT_EQ(TiingoDataSource.GetInquiryQueueSize(), 1);
		TiingoDataSource.SetInquiringWebData(false);
		TiingoDataSource.SetInquiring(true);
		TiingoDataSource.SetInquiringWebData(false);

		EXPECT_CALL(TiingoDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(TiingoDataSource.ProcessInquiringMessage());

		// ˳�����һ��
		EXPECT_STREQ(typeid(*TiingoDataSource.GetCurrentInquiry()).name(), _T("class CProductTiingoStockSymbol"));
		EXPECT_TRUE(TiingoDataSource.IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		TiingoDataSource.SetInquiring(false);
	}

	TEST_F(CMockTiingoDataSourceTest, TestParseTiingoInquiringMessage_STOCK_CANDLES_) {
		const CVirtualProductWebDataPtr p = make_shared<CProductTiingoStockDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetDayLineStartDate(20180101); // ������20180101��ʹ��19800101
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		TiingoDataSource.StoreInquiry(p);
		EXPECT_EQ(TiingoDataSource.GetInquiryQueueSize(), 1);
		TiingoDataSource.SetInquiringWebData(false);
		TiingoDataSource.SetInquiring(true);

		EXPECT_CALL(TiingoDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(TiingoDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(TiingoDataSource.GetInquiryFunction(),
			p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetTiingoDayLineInquiryParam(19800101, gl_pWorldMarket->GetMarketDate()));
		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate());
		// ˳�����һ��
		EXPECT_STREQ(typeid(*TiingoDataSource.GetCurrentInquiry()).name(), _T("class CProductTiingoStockDayLine"));
		EXPECT_TRUE(TiingoDataSource.IsInquiringWebData()) << "����ʹ����Mock��ʽ������˱�ʶû�����á���Ҫ��TearDown���ֹ�����֮";

		// �ָ�ԭ״
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		TiingoDataSource.SetInquiring(false);
	}

	TEST_F(CMockTiingoDataSourceTest, TestProcessTiingoWebDataReceived01) {
		TiingoDataSource.SetInquiringWebData(true);
		TiingoDataSource.SetCurrentInquiry(nullptr);

		EXPECT_FALSE(TiingoDataSource.ProcessWebDataReceived()) << "CurrentInquiryΪnullptr";
	}

	TEST_F(CMockTiingoDataSourceTest, TestProcessTiingoWebDataReceived02) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();

		TiingoDataSource.SetInquiringWebData(true);
		TiingoDataSource.SetCurrentInquiry(p);

		EXPECT_FALSE(TiingoDataSource.ProcessWebDataReceived()) << "DataReceived��ʶΪ��";

		// �ָ�ԭ״
		TiingoDataSource.SetCurrentInquiry(nullptr);
	}

	TEST_F(CMockTiingoDataSourceTest, TestProcessTiingoWebDataReceived03) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();

		TiingoDataSource.SetInquiringWebData(true);
		TiingoDataSource.SetCurrentInquiry(p);
		while (TiingoDataSource.GetReceivedDataSize() > 0) TiingoDataSource.GetReceivedData();

		EXPECT_FALSE(TiingoDataSource.ProcessWebDataReceived()) << "Received����Ϊ��";

		// �ָ�ԭ״
		TiingoDataSource.SetCurrentInquiry(nullptr);
	}

	TEST_F(CMockTiingoDataSourceTest, TestProcessTiingoWebDataReceived04) {
		const CVirtualProductWebDataPtr p = make_shared<CProductDummy>();
		const auto pData = make_shared<CWebData>();
		pData->SetStockCode(_T("{}"));
		pData->CreateNlohmannJson();
		pData->SetParsed(true);

		TiingoDataSource.StoreReceivedData(pData);
		TiingoDataSource.SetCurrentInquiry(p);
		TiingoDataSource.SetInquiringWebData(false);
		TiingoDataSource.SetInquiring(true);

		EXPECT_TRUE(TiingoDataSource.ProcessWebDataReceived());
		// �ָ�ԭ״
		TiingoDataSource.SetCurrentInquiry(nullptr);
	}
}
