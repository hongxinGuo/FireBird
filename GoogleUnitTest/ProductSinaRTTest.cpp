#include"pch.h"

#include"GeneralCheck.h"
#include"CharSetTransfer.h"

#include "ChinaMarket.h"

#include"ProductSinaRT.h"
#include "WebData.h"

using namespace testing;

namespace FireBirdTest {
	class CProductSinaRTTest : public ::testing::Test {
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
		CProductSinaRT sinaRT;
	};

	TEST_F(CProductSinaRTTest, TestInitialize) {
		EXPECT_EQ(sinaRT.GetInquiryFunction(), "https://hq.sinajs.cn/list=");
	}

	TEST_F(CProductSinaRTTest, TestCreateMessage) {
		const string strInquiry = sinaRT.CreateMessage();
		EXPECT_EQ(strInquiry.substr(0, 26), "https://hq.sinajs.cn/list=");
		EXPECT_GT(strInquiry.length(), 26) << "无需检查随后的股票代码制式";
	}

	TEST_F(CProductSinaRTTest, TestParseAndStoreWebData) {
		CWebRTDataPtr pRTData;
		const string strDataUTF8 = "var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\nvar hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n";
		string strGBK = Utf8ToGbk(strDataUTF8); // 新浪实时数据的编码为GBK
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->Test_SetBuffer_(strGBK);
		EXPECT_FALSE(gl_qChinaMarketRTData.try_dequeue(pRTData));

		sinaRT.ParseAndStoreWebData(pData);

		EXPECT_TRUE(gl_qChinaMarketRTData.try_dequeue(pRTData));
		EXPECT_EQ(pRTData->GetSymbol(), "600000.SS") << "第二个数据的代码";
		EXPECT_TRUE(pRTData->GetStockName() == "浦发银行") << "存储的股票名称是utf-8制式的";

		// 恢复原状
		while (gl_qChinaMarketRTData.try_dequeue(pRTData));
	}
}
