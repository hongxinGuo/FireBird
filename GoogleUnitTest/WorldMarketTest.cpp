#include"pch.h"

#include"GeneralCheck.h"

#include"WorldStock.h"

#include"ProductFinnhubCompanyInsiderTransaction.h"
#include"ProductFinnhubCompanyInsiderSentiment.h"
#include"ProductFinnhubCryptoSymbol.h"

#include"ProductTiingoStockSymbol.h"

#include"SetFinnhubForexExchange.h"
#include"SetFinnhubCryptoExchange.h"
#include"SetFinnhubCryptoSymbol.h"
#include"SetFinnhubForexSymbol.h"

#include"FinnhubDataSource.h"
#include"FinnhubCryptoSymbol.h"

using namespace testing;

namespace FireBirdTest {
	class CWorldMarketTest : public Test {
	protected:
		static void SetUpTestSuite() {
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			EXPECT_THAT(gl_systemMessage.InformationSize(), 0);
			GeneralCheck();
		}

		void SetUp() override {
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			gl_pFinnhubDataSource->SetInquiring(false);

			GeneralCheck();
		}

	protected:
	};

	TEST_F(CWorldMarketTest, TestIsResetSystemTime) {
		EXPECT_FALSE(gl_pWorldMarket->IsTimeToResetSystem(165759));
		EXPECT_TRUE(gl_pWorldMarket->IsTimeToResetSystem(165800));
		EXPECT_TRUE(gl_pWorldMarket->IsTimeToResetSystem(165801));
		EXPECT_TRUE(gl_pWorldMarket->IsTimeToResetSystem(170459));
		EXPECT_TRUE(gl_pWorldMarket->IsTimeToResetSystem(170500));
		EXPECT_FALSE(gl_pWorldMarket->IsTimeToResetSystem(170501));
	}

	TEST_F(CWorldMarketTest, TestTransferMarketTime) {
		tm tm2_;
		time_t tt;

		gl_pWorldMarket->CalculateTime();
		time(&tt);
		gmtime_s(&tm2_, &tt);
		const tm tm_ = gl_pWorldMarket->TransferToMarketTime();
		EXPECT_TRUE((tm_.tm_hour == (tm2_.tm_hour - 4) || (tm_.tm_hour == tm2_.tm_hour + 20))) << "WorldMarketĬ��Ϊ������(������׼ʱ��)";
	}

	TEST_F(CWorldMarketTest, TestGetTotalStock) {
		EXPECT_EQ(gl_pWorldMarket->GetStockSize(), 4847) << "Ĭ��״̬�����ݿ�����Ϊ4847(ȫ���Ϻ���Ʊ��С����������Ʊ)";
	}

	TEST_F(CWorldMarketTest, TestIsStock) {
		EXPECT_FALSE(gl_pWorldMarket->IsStock(_T("000000.SS")));
		EXPECT_TRUE(gl_pWorldMarket->IsStock(_T("000001.SS")));
		EXPECT_TRUE(gl_pWorldMarket->IsStock(_T("600601.SS")));
		EXPECT_TRUE(gl_pWorldMarket->IsStock(_T("A")));
		EXPECT_FALSE(gl_pWorldMarket->IsStock(_T("000001.SZ"))) << "Ŀǰ�������ݿ���ֻ���Ϻ���������Ʊ��";

		const auto pStock = make_shared<CWorldStock>();
		pStock->SetSymbol(_T("000000.SS"));
		EXPECT_FALSE(gl_pWorldMarket->IsStock(pStock));
		pStock->SetSymbol(_T("000001.SS"));
		EXPECT_TRUE(gl_pWorldMarket->IsStock(pStock));
		pStock->SetSymbol(_T("600601.SS"));
		EXPECT_TRUE(gl_pWorldMarket->IsStock(pStock));
		pStock->SetSymbol(_T("A"));
		EXPECT_TRUE(gl_pWorldMarket->IsStock(pStock));
		pStock->SetSymbol(_T("000001.SZ"));
		EXPECT_FALSE(gl_pWorldMarket->IsStock(pStock));
	}

	TEST_F(CWorldMarketTest, TestIsTiingoStock) {
		EXPECT_FALSE(gl_pWorldMarket->IsTiingoStock(_T("000000.SS")));
		EXPECT_TRUE(gl_pWorldMarket->IsTiingoStock(_T("AA")));
		EXPECT_FALSE(gl_pWorldMarket->IsTiingoStock(_T("600601.SS")));
		EXPECT_TRUE(gl_pWorldMarket->IsTiingoStock(_T("A")));
		EXPECT_FALSE(gl_pWorldMarket->IsTiingoStock(_T("000001.SZ"))) << "Ŀǰ�������ݿ���ֻ���Ϻ���������Ʊ��";

		const auto pStock = make_shared<CWorldStock>();
		pStock->SetSymbol(_T("000000.SS"));
		EXPECT_FALSE(gl_pWorldMarket->IsTiingoStock(pStock));
		pStock->SetSymbol(_T("AA"));
		EXPECT_TRUE(gl_pWorldMarket->IsTiingoStock(pStock));
		pStock->SetSymbol(_T("600601.SS"));
		EXPECT_FALSE(gl_pWorldMarket->IsTiingoStock(pStock));
		pStock->SetSymbol(_T("A"));
		EXPECT_TRUE(gl_pWorldMarket->IsTiingoStock(pStock));
		pStock->SetSymbol(_T("000001.SZ"));
		EXPECT_FALSE(gl_pWorldMarket->IsTiingoStock(pStock));

		const auto pTiingoStock = make_shared<CTiingoStock>();
		pTiingoStock->m_strTicker = _T("000000.SS");
		EXPECT_FALSE(gl_pWorldMarket->IsTiingoStock(pTiingoStock));
		pTiingoStock->m_strTicker = _T("AA");
		EXPECT_TRUE(gl_pWorldMarket->IsTiingoStock(pTiingoStock));
		pTiingoStock->m_strTicker = _T("600601.SS");
		EXPECT_FALSE(gl_pWorldMarket->IsTiingoStock(pTiingoStock));
		pTiingoStock->m_strTicker = _T("A");
		EXPECT_TRUE(gl_pWorldMarket->IsTiingoStock(pTiingoStock));
		pTiingoStock->m_strTicker = _T("000001.SZ");
		EXPECT_FALSE(gl_pWorldMarket->IsTiingoStock(pTiingoStock));
	}

	TEST_F(CWorldMarketTest, TestGetTiingoStock) {
		CTiingoStockPtr pStock = gl_pWorldMarket->GetTiingoStock(0); // A
		EXPECT_STREQ(pStock->m_strTicker, _T("A")) << "��һ����Ʊ����ΪA";
		pStock = gl_pWorldMarket->GetTiingoStock(_T("A"));
		EXPECT_FALSE(pStock == nullptr);
		EXPECT_STREQ(pStock->m_strName, _T("Agilent Technologies Inc"));
	}

	TEST_F(CWorldMarketTest, TestIsUpdateProfileDB) {
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			const CWorldStockPtr pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateProfileDB(false);
		}
		EXPECT_FALSE(gl_pWorldMarket->IsUpdateStockProfileDB());

		gl_pWorldMarket->GetStock(0)->SetUpdateProfileDB(true);
		EXPECT_TRUE(gl_pWorldMarket->IsUpdateStockProfileDB());

		gl_pWorldMarket->GetStock(0)->SetUpdateProfileDB(false);
		EXPECT_FALSE(gl_pWorldMarket->IsUpdateStockProfileDB());
	}

	TEST_F(CWorldMarketTest, TestAddStock) {
		const auto pStock = make_shared<CWorldStock>();
		pStock->SetSymbol(_T("000001.SZ"));

		EXPECT_FALSE(gl_pWorldMarket->IsStock(pStock));
		gl_pWorldMarket->AddStock(pStock);
		EXPECT_TRUE(gl_pWorldMarket->IsStock(pStock));
		EXPECT_EQ(gl_pWorldMarket->GetStockSize(), 4848);
		gl_pWorldMarket->DeleteStock(pStock);
		EXPECT_FALSE(gl_pWorldMarket->IsStock(pStock));
		EXPECT_EQ(gl_pWorldMarket->GetStockSize(), 4847);
	}

	TEST_F(CWorldMarketTest, TestDeleteStock) {
		// do nothing. �Ѿ���TestAddStock�в�����DeleteStock����
		CWorldStockPtr pStock = nullptr;

		EXPECT_FALSE(gl_pWorldMarket->DeleteStock(pStock)) << "��ָ��";

		pStock = make_shared<CWorldStock>();
		pStock->SetSymbol(_T("000001.SZ"));
		EXPECT_FALSE(gl_pWorldMarket->DeleteStock(pStock)) << "�˹�Ʊ���벻�����ڴ��뼯��";
	}

	TEST_F(CWorldMarketTest, TestGetStock) {
		CWorldStockPtr pStock = gl_pWorldMarket->GetStock(0); // 000001.SS
		EXPECT_STREQ(pStock->GetSymbol(), _T("000001.SS")) << "��һ����Ʊ����Ϊ000001.SS";
		pStock = gl_pWorldMarket->GetStock(_T("000001.SS"));
		EXPECT_FALSE(pStock == nullptr);
		EXPECT_STREQ(pStock->GetDescription(), _T("SSE Composite Index"));
	}

	TEST_F(CWorldMarketTest, TestAddTiingoStock) {
		const auto pStock = make_shared<CTiingoStock>();
		const auto lTotalStock = gl_pWorldMarket->GetTotalTiingoStock();
		pStock->m_strTicker = _T("ABCDEF");

		EXPECT_FALSE(gl_pWorldMarket->IsTiingoStock(pStock));
		gl_pWorldMarket->AddTiingoStock(pStock);
		EXPECT_TRUE(gl_pWorldMarket->IsTiingoStock(pStock));
		EXPECT_EQ(gl_pWorldMarket->GetTotalTiingoStock(), lTotalStock + 1);
		gl_pWorldMarket->DeleteTiingoStock(pStock);
		EXPECT_FALSE(gl_pWorldMarket->IsTiingoStock(pStock));
		EXPECT_EQ(gl_pWorldMarket->GetTotalTiingoStock(), lTotalStock);
	}

	TEST_F(CWorldMarketTest, TestDeleteTiingoStock) {
		// do nothing. �Ѿ���TestAddStock�в�����DeleteStock����
		CTiingoStockPtr pStock = nullptr;

		EXPECT_FALSE(gl_pWorldMarket->DeleteTiingoStock(pStock)) << "��ָ��";

		pStock = make_shared<CTiingoStock>();
		pStock->m_strTicker = _T("ABCDEF");
		EXPECT_FALSE(gl_pWorldMarket->DeleteTiingoStock(pStock)) << "�˹�Ʊ���벻�����ڴ��뼯��";
	}

	TEST_F(CWorldMarketTest, TestIsForexExchange) {
		EXPECT_FALSE(gl_pWorldMarket->IsForexExchange(_T("ABC")));
		EXPECT_TRUE(gl_pWorldMarket->IsForexExchange(_T("forex.com")));
		EXPECT_TRUE(gl_pWorldMarket->IsForexExchange(_T("ic markets")));
	}

	TEST_F(CWorldMarketTest, TestAddForexExchange) {
		const auto lTotalForexExchange = gl_pWorldMarket->GetForexExchangeSize();
		const CString strForexExchange = _T("000001.SZ");

		EXPECT_FALSE(gl_pWorldMarket->IsForexExchange(strForexExchange));
		gl_pWorldMarket->AddForexExchange(strForexExchange);
		EXPECT_TRUE(gl_pWorldMarket->IsForexExchange(strForexExchange));
		EXPECT_EQ(gl_pWorldMarket->GetForexExchangeSize(), lTotalForexExchange + 1);
		gl_pWorldMarket->DeleteForexExchange(strForexExchange);
		EXPECT_FALSE(gl_pWorldMarket->IsForexExchange(strForexExchange));
		EXPECT_EQ(gl_pWorldMarket->GetForexExchangeSize(), lTotalForexExchange);
	}

	TEST_F(CWorldMarketTest, TestDeleteForexExchange) {
		// do nothing. �Ѿ���TestAddForexExchange�в�����DeleteForexExchange����
		EXPECT_FALSE(gl_pWorldMarket->DeleteForexExchange(_T("US.US.US"))) << "�˷����ڷ��ż��в�����";
	}

	TEST_F(CWorldMarketTest, TestIsForexSymbol) {
		EXPECT_FALSE(gl_pWorldMarket->IsForexSymbol(_T("ABC")));
		EXPECT_TRUE(gl_pWorldMarket->IsForexSymbol(_T("OANDA:XAU_SGD")));
		EXPECT_TRUE(gl_pWorldMarket->IsForexSymbol(_T("FXCM:EUR/CHF")));

		const auto pForexSymbol = make_shared<CFinnhubForexSymbol>();
		pForexSymbol->SetSymbol(_T("ABC"));
		EXPECT_FALSE(gl_pWorldMarket->IsForexSymbol(pForexSymbol));
		pForexSymbol->SetSymbol(_T("OANDA:XAU_SGD"));
		EXPECT_TRUE(gl_pWorldMarket->IsForexSymbol(pForexSymbol));
		pForexSymbol->SetSymbol(_T("FXCM:EUR/CHF"));
		EXPECT_TRUE(gl_pWorldMarket->IsForexSymbol(pForexSymbol));
	}

	TEST_F(CWorldMarketTest, TestAddForexSymbol) {
		const auto pForexSymbol = make_shared<CFinnhubForexSymbol>();
		const auto lTotalForexSymbol = gl_pWorldMarket->GetForexSymbolSize();
		pForexSymbol->SetSymbol(_T("000001.SZ"));

		EXPECT_FALSE(gl_pWorldMarket->IsForexSymbol(pForexSymbol));
		gl_pWorldMarket->AddForexSymbol(pForexSymbol);
		EXPECT_TRUE(gl_pWorldMarket->IsForexSymbol(pForexSymbol));
		EXPECT_EQ(gl_pWorldMarket->GetForexSymbolSize(), lTotalForexSymbol + 1);
		EXPECT_TRUE(gl_pWorldMarket->DeleteForexSymbol(pForexSymbol));
		EXPECT_FALSE(gl_pWorldMarket->IsForexSymbol(pForexSymbol));
		EXPECT_EQ(gl_pWorldMarket->GetForexSymbolSize(), lTotalForexSymbol);
	}

	TEST_F(CWorldMarketTest, TestDeleteForexSymbol) {
		// do nothing. �Ѿ���TestAddForexSymbol�в�����DeleteForexSymbol����
		CForexSymbolPtr pForexSymbol = nullptr;

		EXPECT_FALSE(gl_pWorldMarket->DeleteForexSymbol(pForexSymbol)) << "��ָ��";

		pForexSymbol = make_shared<CFinnhubForexSymbol>();
		pForexSymbol->SetSymbol(_T("000001.SZ"));
		EXPECT_FALSE(gl_pWorldMarket->DeleteForexSymbol(pForexSymbol)) << "�˷����ڷ��ż��в�����";
	}

	TEST_F(CWorldMarketTest, TestIsCryptoExchange) {
		EXPECT_FALSE(gl_pWorldMarket->IsCryptoExchange(_T("ABC")));
		EXPECT_TRUE(gl_pWorldMarket->IsCryptoExchange(_T("BITFINEX")));
		EXPECT_TRUE(gl_pWorldMarket->IsCryptoExchange(_T("HUOBI")));
	}

	TEST_F(CWorldMarketTest, TestAddCryptoExchange) {
		const auto lTotalCryptoExchange = gl_pWorldMarket->GetCryptoExchangeSize();
		const CString strCryptoExchange = _T("000001.SZ");

		EXPECT_FALSE(gl_pWorldMarket->IsCryptoExchange(strCryptoExchange));
		gl_pWorldMarket->AddCryptoExchange(strCryptoExchange);
		EXPECT_TRUE(gl_pWorldMarket->IsCryptoExchange(strCryptoExchange));
		EXPECT_EQ(gl_pWorldMarket->GetCryptoExchangeSize(), lTotalCryptoExchange + 1);
		gl_pWorldMarket->DeleteCryptoExchange(strCryptoExchange);
		EXPECT_FALSE(gl_pWorldMarket->IsCryptoExchange(strCryptoExchange));
		EXPECT_EQ(gl_pWorldMarket->GetCryptoExchangeSize(), lTotalCryptoExchange);
	}

	TEST_F(CWorldMarketTest, TestDeleteCryptoExchange) {
		// do nothing. �Ѿ���TestAddCryptoExchange�в�����DeleteCryptoExchange����
		EXPECT_FALSE(gl_pWorldMarket->DeleteCryptoExchange(_T("US.US.US"))) << "�˷����ڷ��ż��в�����";
	}

	TEST_F(CWorldMarketTest, TestIsCryptoSymbol) {
		EXPECT_FALSE(gl_pWorldMarket->IsFinnhubCryptoSymbol(_T("ABC")));
		EXPECT_TRUE(gl_pWorldMarket->IsFinnhubCryptoSymbol(_T("POLONIEX:BTC_DOT")));
		EXPECT_TRUE(gl_pWorldMarket->IsFinnhubCryptoSymbol(_T("BINANCE:USDTUAH")));
		EXPECT_TRUE(gl_pWorldMarket->IsFinnhubCryptoSymbol(_T("COINBASE:TRIBE-USD")));

		const auto pCryptoSymbol = make_shared<CFinnhubCryptoSymbol>();
		pCryptoSymbol->SetSymbol(_T("ABC"));
		EXPECT_FALSE(gl_pWorldMarket->IsFinnhubCryptoSymbol(pCryptoSymbol));
		pCryptoSymbol->SetSymbol(_T("BINANCE:USDTUAH"));
		EXPECT_TRUE(gl_pWorldMarket->IsFinnhubCryptoSymbol(pCryptoSymbol));
		pCryptoSymbol->SetSymbol(_T("COINBASE:TRIBE-USD"));
		EXPECT_TRUE(gl_pWorldMarket->IsFinnhubCryptoSymbol(pCryptoSymbol));
	}

	TEST_F(CWorldMarketTest, TestAddCryptoSymbol) {
		const auto pCryptoSymbol = make_shared<CFinnhubCryptoSymbol>();
		const auto lTotalCryptoSymbol = gl_pWorldMarket->GetFinnhubCryptoSymbolSize();
		pCryptoSymbol->SetSymbol(_T("000001.SZ"));

		EXPECT_FALSE(gl_pWorldMarket->IsFinnhubCryptoSymbol(pCryptoSymbol));
		gl_pWorldMarket->AddFinnhubCryptoSymbol(pCryptoSymbol);
		EXPECT_TRUE(gl_pWorldMarket->IsFinnhubCryptoSymbol(pCryptoSymbol));
		EXPECT_EQ(gl_pWorldMarket->GetFinnhubCryptoSymbolSize(), lTotalCryptoSymbol + 1);

		EXPECT_TRUE(gl_pWorldMarket->DeleteFinnhubCryptoSymbol(pCryptoSymbol));
		EXPECT_FALSE(gl_pWorldMarket->IsFinnhubCryptoSymbol(pCryptoSymbol));
		EXPECT_EQ(gl_pWorldMarket->GetFinnhubCryptoSymbolSize(), lTotalCryptoSymbol);
	}

	TEST_F(CWorldMarketTest, TestDeleteCryptoSymbol) {
		// do nothing. �Ѿ���TestAddCryptoSymbol�в�����DeleteCryptoSymbol����
		CFinnhubCryptoSymbolPtr pCryptoSymbol = nullptr;

		EXPECT_FALSE(gl_pWorldMarket->DeleteFinnhubCryptoSymbol(pCryptoSymbol)) << "��ָ��";

		pCryptoSymbol = make_shared<CFinnhubCryptoSymbol>();
		pCryptoSymbol->SetSymbol(_T("000001.SZ"));
		EXPECT_FALSE(gl_pWorldMarket->DeleteFinnhubCryptoSymbol(pCryptoSymbol)) << "�˷����ڷ��ż��в�����";
	}

	TEST_F(CWorldMarketTest, TestIsCountry) {
		const auto pCountry = make_shared<CCountry>();

		EXPECT_FALSE(gl_pWorldMarket->IsCountry(_T("ABC")));
		EXPECT_TRUE(gl_pWorldMarket->IsCountry(_T("American Samoa")));

		pCountry->m_strCountry = _T("ABC");
		EXPECT_FALSE(gl_pWorldMarket->IsCountry(pCountry));
		pCountry->m_strCountry = _T("American Samoa");
		EXPECT_TRUE(gl_pWorldMarket->IsCountry(pCountry));
	}

	TEST_F(CWorldMarketTest, TestAddCountry) {
		const auto pCountry = make_shared<CCountry>();
		const auto lTotalCountry = gl_pWorldMarket->GetTotalCountry();
		pCountry->m_strCountry = _T("SZ");

		EXPECT_FALSE(gl_pWorldMarket->IsCountry(pCountry));
		gl_pWorldMarket->AddCountry(pCountry);
		EXPECT_TRUE(gl_pWorldMarket->IsCountry(pCountry));
		EXPECT_EQ(gl_pWorldMarket->GetTotalCountry(), lTotalCountry + 1);
		gl_pWorldMarket->DeleteCountry(pCountry);
		EXPECT_FALSE(gl_pWorldMarket->IsCountry(pCountry));
		EXPECT_EQ(gl_pWorldMarket->GetTotalCountry(), lTotalCountry);
	}

	TEST_F(CWorldMarketTest, TestDeleteCountry) {
		// do nothing. �Ѿ���TestAddCountry�в�����DeleteCountry����
		CCountryPtr pCountry = nullptr;

		EXPECT_FALSE(gl_pWorldMarket->DeleteCountry(pCountry)) << "��ָ��";

		pCountry = make_shared<CCountry>();
		pCountry->m_strCountry = _T("SZ");
		EXPECT_FALSE(gl_pWorldMarket->DeleteCountry(pCountry)) << "�˹�Ʊ���벻�����ڴ��뼯��";
	}

	TEST_F(CWorldMarketTest, TestLoadExchangeCode) {
		// todo �ݻ�
	}

	TEST_F(CWorldMarketTest, TestUpdateCountryDB) {
		const size_t lTotal = gl_pWorldMarket->GetTotalCountry();

		const auto pCountry = make_shared<CCountry>();
		pCountry->m_strCode2 = _T("AB");
		pCountry->m_strCountry = _T("NoName");
		EXPECT_FALSE(gl_pWorldMarket->IsCountry(pCountry));
		gl_pWorldMarket->AddCountry(pCountry);
		EXPECT_EQ(gl_pWorldMarket->GetTotalCountry(), lTotal + 1);
		gl_pWorldMarket->UpdateCountryListDB(); // �˲��Ժ���ִ�����������һ��Countryû��ɾ�������ݿ��е�ɾ���ˣ���

		CSetCountry setCountry;
		setCountry.m_strFilter = _T("[Country] = 'NoName'");
		setCountry.Open();
		EXPECT_FALSE(setCountry.IsEOF());
		setCountry.m_pDatabase->BeginTrans();
		while (!setCountry.IsEOF()) {
			setCountry.Delete();
			setCountry.MoveNext();
		}
		setCountry.m_pDatabase->CommitTrans();
		setCountry.Close();
	}

	TEST_F(CWorldMarketTest, TestUpdateStockProfileDB) {
		//bug �˲��������⣺����unknown c++ exception thrown�������ж�ԭ���ں���UpdateStockProfileDB()�С�
		auto pStock = make_shared<CWorldStock>();
		pStock->SetSymbol(_T("SS.SS.US"));
		EXPECT_FALSE(gl_pWorldMarket->IsStock(pStock)); // ȷ����һ���¹�Ʊ����
		pStock->SetTodayNewStock(true);
		pStock->SetUpdateProfileDB(true);
		EXPECT_TRUE(gl_pWorldMarket->AddStock(pStock));
		pStock = gl_pWorldMarket->GetStock(_T("000001.SS"));
		EXPECT_TRUE(pStock != nullptr);
		EXPECT_STREQ(pStock->GetCurrency(), _T(""));
		pStock->SetUpdateProfileDB(true);
		pStock->SetCurrency(_T("No Currency")); // ���������Ŀ

		try {
			gl_pWorldMarket->UpdateStockProfileDB();
		}
		catch (std::exception& e) {
			EXPECT_TRUE(false) << e.what(); // todo ��������ʲô����exception����
		}

		CSetWorldStock setWorldStock;
		setWorldStock.m_strFilter = _T("[Symbol] = '000001.SS'");
		setWorldStock.Open();
		EXPECT_FALSE(setWorldStock.IsEOF());
		EXPECT_STREQ(setWorldStock.m_Currency, _T("No Currency")) << "����Ŀ�Ѹ���";
		setWorldStock.m_pDatabase->BeginTrans();
		setWorldStock.Edit();
		setWorldStock.m_Currency = _T("");
		setWorldStock.Update();
		setWorldStock.m_pDatabase->CommitTrans();
		setWorldStock.Close();

		CSetWorldStock setWorldStock2;
		setWorldStock2.m_strFilter = _T("[Symbol] = 'SS.SS.US'");
		setWorldStock2.Open();
		EXPECT_FALSE(setWorldStock2.IsEOF());
		setWorldStock2.m_pDatabase->BeginTrans();
		while (!setWorldStock2.IsEOF()) {
			setWorldStock2.Delete();
			setWorldStock2.MoveNext();
		}
		setWorldStock2.m_pDatabase->CommitTrans();
		setWorldStock2.Close();

		// �ָ�ԭ״
		pStock = gl_pWorldMarket->GetStock(_T("SS.SS.US"));
		EXPECT_TRUE(pStock != nullptr);
		EXPECT_TRUE(gl_pWorldMarket->DeleteStock(pStock));
		pStock = gl_pWorldMarket->GetStock(_T("000001.SS"));
		EXPECT_TRUE(pStock != nullptr);
		pStock->SetCurrency(_T(""));
		while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
		EXPECT_EQ(gl_pWorldMarket->GetStockSize(), 4847);
	}

	TEST_F(CWorldMarketTest, TestUpdateDayLineDB) {
		EXPECT_TRUE(gl_pWorldMarket->UpdateStockDayLineDB());

		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedSaving()) << "�˱�ʶ������";
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedSaving(true);

		EXPECT_TRUE(gl_pWorldMarket->UpdateStockDayLineDB());
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			EXPECT_FALSE(gl_pWorldMarket->GetStock(i)->IsDayLineNeedSaving()) << "�˱�ʶ������";
		}
	}

	TEST_F(CWorldMarketTest, TestUpdateForexSymbolDB) {
		auto pForexSymbol = make_shared<CFinnhubForexSymbol>();
		pForexSymbol->SetSymbol(_T("SS.SS.US")); // �·���
		EXPECT_FALSE(gl_pWorldMarket->IsForexSymbol(pForexSymbol));
		gl_pWorldMarket->AddForexSymbol(pForexSymbol);
		pForexSymbol = gl_pWorldMarket->GetForexSymbol(_T("OANDA:GBP_ZAR")); // �ڶ����ִ�ķ���
		EXPECT_EQ(pForexSymbol->GetIPOStatus(), _STOCK_IPOED_);
		pForexSymbol->SetUpdateProfileDB(true);
		pForexSymbol->SetIPOStatus(_STOCK_DELISTED_);
		gl_pWorldMarket->UpdateForexSymbolDB();

		CSetFinnhubForexSymbol setCryptoSymbol;
		setCryptoSymbol.m_strFilter = _T("[Symbol] = 'OANDA:GBP_ZAR'");
		setCryptoSymbol.Open();
		EXPECT_EQ(setCryptoSymbol.m_IPOStatus, _STOCK_DELISTED_);
		setCryptoSymbol.m_pDatabase->BeginTrans();
		setCryptoSymbol.Edit();
		setCryptoSymbol.m_IPOStatus = _STOCK_IPOED_;
		setCryptoSymbol.Update();
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();

		setCryptoSymbol.m_strFilter = _T("[Symbol] = 'SS.SS.US'");
		setCryptoSymbol.Open();
		EXPECT_FALSE(setCryptoSymbol.IsEOF()) << "�������·���";
		setCryptoSymbol.m_pDatabase->BeginTrans();
		while (!setCryptoSymbol.IsEOF()) {
			setCryptoSymbol.Delete();
			setCryptoSymbol.MoveNext();
		}
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();

		pForexSymbol = gl_pWorldMarket->GetForexSymbol(_T("SS.SS.US"));
		EXPECT_TRUE(pForexSymbol != nullptr);
		gl_pWorldMarket->DeleteForexSymbol(pForexSymbol); // �ָ�ԭ״
	}

	TEST_F(CWorldMarketTest, TestUpdateFinnhubCryptoSymbolDB) {
		auto pCryptoSymbol = make_shared<CFinnhubCryptoSymbol>();
		pCryptoSymbol->SetSymbol(_T("SS.SS.US")); // �·���
		EXPECT_FALSE(gl_pWorldMarket->IsFinnhubCryptoSymbol(pCryptoSymbol));
		gl_pWorldMarket->AddFinnhubCryptoSymbol(pCryptoSymbol);
		pCryptoSymbol = gl_pWorldMarket->GetFinnhubCryptoSymbol(_T("BINANCE:USDTUAH")); // �ڶ����ִ�ķ���
		EXPECT_EQ(pCryptoSymbol->GetIPOStatus(), _STOCK_IPOED_);
		pCryptoSymbol->SetUpdateProfileDB(true);
		pCryptoSymbol->SetIPOStatus(_STOCK_DELISTED_);
		gl_pWorldMarket->UpdateFinnhubCryptoSymbolDB();

		CSetFinnhubCryptoSymbol setCryptoSymbol;
		setCryptoSymbol.m_strFilter = _T("[Symbol] = 'BINANCE:USDTUAH'");
		setCryptoSymbol.Open();
		EXPECT_EQ(setCryptoSymbol.m_IPOStatus, _STOCK_DELISTED_) << "״̬�ѱ��޸�Ϊժ��";
		setCryptoSymbol.m_pDatabase->BeginTrans();
		setCryptoSymbol.Edit();
		setCryptoSymbol.m_IPOStatus = _STOCK_IPOED_;
		setCryptoSymbol.Update();
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();

		setCryptoSymbol.m_strFilter = _T("[Symbol] = 'SS.SS.US'");
		setCryptoSymbol.Open();
		EXPECT_FALSE(setCryptoSymbol.IsEOF()) << "�������·���";
		setCryptoSymbol.m_pDatabase->BeginTrans();
		while (!setCryptoSymbol.IsEOF()) {
			setCryptoSymbol.Delete(); // ɾ�����´���
			setCryptoSymbol.MoveNext();
		}
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();

		pCryptoSymbol = gl_pWorldMarket->GetFinnhubCryptoSymbol(_T("SS.SS.US"));
		EXPECT_TRUE(pCryptoSymbol != nullptr);
		gl_pWorldMarket->DeleteFinnhubCryptoSymbol(pCryptoSymbol); // �ָ�ԭ״
	}

	TEST_F(CWorldMarketTest, TestUpdateTiingoStockDB) {
		CSetTiingoStock setTiingoStock;

		const auto pTiingoStock = make_shared<CTiingoStock>();
		pTiingoStock->m_fIsActive = true;
		pTiingoStock->m_fIsADR = false;
		pTiingoStock->m_iSICCode = 1002;
		pTiingoStock->m_lDailyDataUpdateDate = 20200101;
		pTiingoStock->m_lStatementUpdateDate = 20210101;
		pTiingoStock->m_strCompanyWebSite = _T("www.abc.com");
		pTiingoStock->m_strLocation = _T("Irvine CA USA");
		pTiingoStock->m_strName = _T("ABCDEF"); // �´���
		pTiingoStock->m_strReportingCurrency = _T("US Doller");
		pTiingoStock->m_strSECFilingWebSite = _T("abc");
		pTiingoStock->m_strSICIndustry = _T("Computer Science");
		pTiingoStock->m_strSICSector = _T("Communication");
		pTiingoStock->m_strTicker = _T("ABCDEF"); // �´���
		pTiingoStock->m_strTiingoIndustry = _T("Computer");
		pTiingoStock->m_strTiingoPermaTicker = _T("abcdefg");
		pTiingoStock->m_strTiingoSector = _T("gfedcba");

		gl_pWorldMarket->AddTiingoStock(pTiingoStock);

		gl_pWorldMarket->UpdateTiingoStockDB(); // ���´��뼯

		// �ָ�ԭ״
		setTiingoStock.m_strFilter = _T("[Ticker] = 'ABCDEF'");
		setTiingoStock.Open();
		EXPECT_FALSE(setTiingoStock.IsEOF()) << "������ABCDEF��Ʊ����";
		setTiingoStock.m_pDatabase->BeginTrans();
		while (!setTiingoStock.IsEOF()) {
			setTiingoStock.Delete();
			setTiingoStock.MoveNext();
		}
		setTiingoStock.m_pDatabase->CommitTrans();
		setTiingoStock.Close();

		gl_pWorldMarket->DeleteTiingoStock(pTiingoStock);
	}

	TEST_F(CWorldMarketTest, TestUpdateForexExchangeDB) {
		const CString strSymbol = _T("US.US.US");

		EXPECT_FALSE(gl_pWorldMarket->UpdateForexExchangeDB()) << "û����Forex Exchange";

		EXPECT_FALSE(gl_pWorldMarket->IsForexExchange(strSymbol)); // ȷ����һ���¹�Ʊ����
		gl_pWorldMarket->AddForexExchange(strSymbol);
		EXPECT_TRUE(gl_pWorldMarket->UpdateForexExchangeDB());

		CSetFinnhubForexExchange setForexExchange;
		setForexExchange.m_strFilter = _T("[Code] = 'US.US.US'");
		setForexExchange.Open();
		EXPECT_FALSE(setForexExchange.IsEOF());
		setForexExchange.m_pDatabase->BeginTrans();
		while (!setForexExchange.IsEOF()) {
			setForexExchange.Delete();
			setForexExchange.MoveNext();
		}
		setForexExchange.m_pDatabase->CommitTrans();
		setForexExchange.Close();

		gl_pWorldMarket->DeleteForexExchange(strSymbol); // �ָ�ԭ״
	}

	TEST_F(CWorldMarketTest, TestUpdateCryptoExchangeDB) {
		const CString strSymbol = _T("US.US.US");

		EXPECT_FALSE(gl_pWorldMarket->UpdateCryptoExchangeDB()) << "û����Crypto Exchange";

		EXPECT_FALSE(gl_pWorldMarket->IsCryptoExchange(strSymbol)); // ȷ����һ����Crypto����
		gl_pWorldMarket->AddCryptoExchange(strSymbol);
		EXPECT_TRUE(gl_pWorldMarket->UpdateCryptoExchangeDB());

		CSetFinnhubCryptoExchange setCryptoExchange;
		setCryptoExchange.m_strFilter = _T("[Code] = 'US.US.US'");
		setCryptoExchange.Open();
		EXPECT_FALSE(setCryptoExchange.IsEOF());
		setCryptoExchange.m_pDatabase->BeginTrans();
		while (!setCryptoExchange.IsEOF()) {
			setCryptoExchange.Delete(); // ɾ������ӵ��������
			setCryptoExchange.MoveNext();
		}
		setCryptoExchange.m_pDatabase->CommitTrans();
		setCryptoExchange.Close();

		gl_pWorldMarket->DeleteCryptoExchange(strSymbol); // �ָ�ԭ״
	}

	TEST_F(CWorldMarketTest, TaskUpdateInsiderTransactionDB) {
		EXPECT_FALSE(gl_pWorldMarket->GetStock(_T("A"))->HaveInsiderTransaction());
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 0);

		vector<CInsiderTransactionPtr> vInsiderTransaction;
		CSetInsiderTransaction setInsiderTransaction;

		CInsiderTransactionPtr pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = _T("B");
		pInsiderTransaction->m_strPersonName = _T("a b c");
		pInsiderTransaction->m_lTransactionDate = 20200101; // �����Ʊ���벻������Ҫ��ӽ����ݿ�
		vInsiderTransaction.push_back(pInsiderTransaction);
		pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = _T("A");
		pInsiderTransaction->m_strPersonName = _T("a b c d");
		pInsiderTransaction->m_lTransactionDate = 20210101; // ����ڲ�������Ա���Ʋ�������Ҫ��ӽ����ݿ�
		vInsiderTransaction.push_back(pInsiderTransaction);
		pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = _T("A");
		pInsiderTransaction->m_strPersonName = _T("a b c");
		pInsiderTransaction->m_lTransactionDate = 20210107;
		pInsiderTransaction->m_strTransactionCode = _T("M"); // ������ݿ����У��������
		vInsiderTransaction.push_back(pInsiderTransaction);
		pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = _T("A");
		pInsiderTransaction->m_strPersonName = _T("a b c");
		pInsiderTransaction->m_lTransactionDate = 20210124; // ������ڲ�������Ҫ��ӽ����ݿ�
		vInsiderTransaction.push_back(pInsiderTransaction);
		pInsiderTransaction->m_strSymbol = _T("A");
		pInsiderTransaction->m_strPersonName = _T("a b c");
		pInsiderTransaction->m_strTransactionCode = _T("S"); // ����������Ͳ�������Ҫ��ӽ����ݿ�
		vInsiderTransaction.push_back(pInsiderTransaction);

		const CWorldStockPtr pStock = gl_pWorldMarket->GetStock(_T("A"));
		EXPECT_FALSE(pStock->HaveInsiderTransaction()) << "��ʱ��δ��������";

		pStock->SetSaveInsiderTransaction(true);
		pStock->SetInsiderTransactionUpdateDate(20210123);
		pStock->UpdateInsiderTransaction(vInsiderTransaction);

		EXPECT_TRUE(gl_pWorldMarket->UpdateInsiderTransactionDB());

		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 1);
		const CString str = gl_systemMessage.PopDayLineInfoMessage();
		EXPECT_STREQ(str, _T("A�ڲ��������ϸ������"));
		EXPECT_FALSE(gl_pWorldMarket->GetStock(_T("A"))->IsSaveInsiderTransaction());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(_T("A"))->HaveInsiderTransaction()) << "�洢��û��ɾ������";

		// ��֤���ָ�ԭ״
		setInsiderTransaction.m_strFilter = _T("[Symbol] = 'B'");
		setInsiderTransaction.Open();
		setInsiderTransaction.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderTransaction.IsEOF());
		setInsiderTransaction.Delete();
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();

		setInsiderTransaction.m_strFilter = _T("[PersonName] = 'a b c d'");
		setInsiderTransaction.Open();
		setInsiderTransaction.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderTransaction.IsEOF());
		setInsiderTransaction.Delete();
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();

		setInsiderTransaction.m_strFilter = _T("[TransactionDate] = '20210124'");
		setInsiderTransaction.Open();
		setInsiderTransaction.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderTransaction.IsEOF());
		setInsiderTransaction.Delete();
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();

		setInsiderTransaction.m_strFilter = _T("[TransactionCode] = 'S'");
		setInsiderTransaction.Open();
		setInsiderTransaction.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderTransaction.IsEOF());
		setInsiderTransaction.Delete();
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();
	}

	TEST_F(CWorldMarketTest, TaskUpdateInsiderSentimentDB) {
		EXPECT_FALSE(gl_pWorldMarket->GetStock(_T("A"))->HaveInsiderSentiment());
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 0);

		vector<CInsiderSentimentPtr> vInsiderSentiment;
		CSetInsiderSentiment setInsiderSentiment;

		CInsiderSentimentPtr pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment->m_strSymbol = _T("B");// �����Ʊ���벻����bu��Ҫ��ӽ����ݿ�
		pInsiderSentiment->m_lDate = 20200101;
		vInsiderSentiment.push_back(pInsiderSentiment);
		pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment->m_strSymbol = _T("A");
		pInsiderSentiment->m_lDate = 20200101;// ������ݿ����У��������
		vInsiderSentiment.push_back(pInsiderSentiment);
		pInsiderSentiment = make_shared<CInsiderSentiment>();
		pInsiderSentiment->m_strSymbol = _T("A");
		pInsiderSentiment->m_lDate = 20210101; // ������ڲ�������Ҫ��ӽ����ݿ�
		vInsiderSentiment.push_back(pInsiderSentiment);

		const CWorldStockPtr pStock = gl_pWorldMarket->GetStock(_T("A"));
		EXPECT_FALSE(pStock->HaveInsiderSentiment()) << "��ʱ��δ��������";

		pStock->SetSaveInsiderSentiment(true);
		pStock->SetInsiderSentimentUpdateDate(20210101);
		pStock->UpdateInsiderSentiment(vInsiderSentiment);

		EXPECT_TRUE(gl_pWorldMarket->UpdateInsiderSentimentDB());

		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 1);
		const CString str = gl_systemMessage.PopDayLineInfoMessage();
		EXPECT_STREQ(str, _T("A�ڲ������������ϸ������"));
		EXPECT_FALSE(gl_pWorldMarket->GetStock(_T("A"))->IsSaveInsiderSentiment());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(_T("A"))->HaveInsiderSentiment()) << "�洢��û��ɾ������";

		// ��֤���ָ�ԭ״
		setInsiderSentiment.m_strFilter = _T("[Symbol] = 'B'");
		setInsiderSentiment.Open();
		EXPECT_TRUE(setInsiderSentiment.IsEOF());
		setInsiderSentiment.Close();

		setInsiderSentiment.m_strFilter = _T("[Date] = '20210101'");
		setInsiderSentiment.Open();
		setInsiderSentiment.m_pDatabase->BeginTrans();
		EXPECT_FALSE(setInsiderSentiment.IsEOF());
		setInsiderSentiment.Delete();
		setInsiderSentiment.m_pDatabase->CommitTrans();
		setInsiderSentiment.Close();
	}

	TEST_F(CWorldMarketTest, TestUpdateEconomicCalendarDB) {
		CSetEconomicCalendar setEconomicCalendar;
		const auto pEconomicCalendar = make_shared<CEconomicCalendar>();
		vector<CEconomicCalendarPtr> vEconomicCalendar;

		pEconomicCalendar->m_strCountry = _T("USA");
		pEconomicCalendar->m_strTime = _T("20200101");
		pEconomicCalendar->m_strEvent = _T("abc");
		pEconomicCalendar->m_dActual = 1.0;
		pEconomicCalendar->m_dEstimate = 2.0;
		pEconomicCalendar->m_dPrev = 3.0;
		pEconomicCalendar->m_strImpact = _T("s");
		pEconomicCalendar->m_strUnit = _T("USD");

		vEconomicCalendar.push_back(pEconomicCalendar);

		EXPECT_TRUE(gl_pWorldMarket->UpdateEconomicCalendar(vEconomicCalendar));
		EXPECT_TRUE(gl_pWorldMarket->UpdateEconomicCalendarDB());

		// ���Բ��ָ�ԭ״
		setEconomicCalendar.Open();
		EXPECT_FALSE(setEconomicCalendar.IsEOF());
		EXPECT_STREQ(setEconomicCalendar.m_Country, _T("USA"));
		EXPECT_STREQ(setEconomicCalendar.m_Time, _T("20200101"));
		EXPECT_STREQ(setEconomicCalendar.m_Event, _T("abc"));
		EXPECT_DOUBLE_EQ(atof(setEconomicCalendar.m_Actual), 1.0);
		EXPECT_DOUBLE_EQ(atof(setEconomicCalendar.m_Estimate), 2.0);
		EXPECT_DOUBLE_EQ(atof(setEconomicCalendar.m_Prev), 3.0);
		EXPECT_STREQ(setEconomicCalendar.m_Impact, _T("s"));
		EXPECT_STREQ(setEconomicCalendar.m_Unit, _T("USD"));
		setEconomicCalendar.m_pDatabase->BeginTrans();
		while (!setEconomicCalendar.IsEOF()) {
			setEconomicCalendar.Delete();
			setEconomicCalendar.MoveNext();
		}
		setEconomicCalendar.m_pDatabase->CommitTrans();
		setEconomicCalendar.Close();
	}

	TEST_F(CWorldMarketTest, TestRebuildEPSSurprise) {
		CWorldStockPtr pStock;
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetLastEPSSurpriseUpdateDate(20200101);
			pStock->m_fEPSSurpriseUpdated = true;
		}
		gl_pFinnhubDataSource->SetUpdateEPSSurprise(false);

		EXPECT_TRUE(gl_pWorldMarket->RebuildEPSSurprise());

		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			EXPECT_EQ(pStock->GetLastEPSSurpriseUpdateDate(), 19800101);
			EXPECT_FALSE(pStock->m_fEPSSurpriseUpdated);
		}
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateEPSSurprise());
	}

	TEST_F(CWorldMarketTest, TestRebuildPeer) {
		CWorldStockPtr pStock;
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetPeerUpdateDate(20200101);
			pStock->SetUpdatePeer(false);
			pStock->SetUpdateProfileDB(false);
		}
		gl_pFinnhubDataSource->SetUpdatePeer(false);

		EXPECT_TRUE(gl_pWorldMarket->RebuildPeer());

		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			EXPECT_EQ(pStock->GetPeerUpdateDate(), 19800101);
			EXPECT_TRUE(pStock->IsUpdatePeer());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
		}
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdatePeer());

		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdatePeer(true);
			pStock->SetUpdateProfileDB(false);
		}
	}

	TEST_F(CWorldMarketTest, TestRebuildStockDayLine) {
		CWorldStockPtr pStock;
		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetIPOStatus(_STOCK_IPOED_);
			pStock->SetDayLineStartDate(20200101);
			pStock->SetDayLineEndDate(20200101);
			pStock->SetDayLineNeedUpdate(false);
			pStock->SetUpdateProfileDB(false);
		}
		gl_pFinnhubDataSource->SetUpdateStockProfile(false);

		EXPECT_TRUE(gl_pWorldMarket->RebuildStockDayLineDB());

		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			EXPECT_EQ(pStock->GetDayLineStartDate(), 29900101);
			EXPECT_EQ(pStock->GetDayLineEndDate(), 19800101);
			EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
		}
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateStockProfile());

		for (int i = 0; i < gl_pWorldMarket->GetStockSize(); i++) {
			pStock = gl_pWorldMarket->GetStock(i);
			pStock->SetUpdateProfileDB(false);
		}
	}

	TEST_F(CWorldMarketTest, TestUpdateTiingoIndustry) {
		EXPECT_THAT(gl_pWorldMarket->UpdateTiingoIndustry(), IsFalse());
	}

	TEST_F(CWorldMarketTest, TestUpdateSICIndustry) {
		EXPECT_THAT(gl_pWorldMarket->UpdateSICIndustry(), IsFalse());
	}

	TEST_F(CWorldMarketTest, TestUpdateNaicsIndustry) {
		EXPECT_THAT(gl_pWorldMarket->UpdateNaicsIndustry(), IsFalse());
	}
}
