#include"pch.h"

#include "ChinaMarket.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductSinaRT.h"

using namespace testing;

namespace FireBirdTest {
	class CProductSinaRTTest : public ::testing::Test {
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
		CProductSinaRT sinaRT;
	};

	TEST_F(CProductSinaRTTest, TestInitialize) {
		EXPECT_STREQ(sinaRT.GetInquiryFunction(), _T("https://hq.sinajs.cn/list="));
	}

	TEST_F(CProductSinaRTTest, TestCreateMessage) {
		const CString strInquiry = sinaRT.CreateMessage();
		EXPECT_STREQ(strInquiry.Left(26), _T("https://hq.sinajs.cn/list="));
		EXPECT_GT(strInquiry.GetLength(), 26) << "无需检查随后的股票代码制式";
	}

	TEST_F(CProductSinaRTTest, TestParseAndStoreWebData) {
		const CString strData = _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\nvar hq_str_sh600001=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n");
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->Test_SetBuffer_(strData);
		EXPECT_EQ(gl_pChinaMarket->SinaRTSize(), 0);

		sinaRT.ParseAndStoreWebData(pData);

		EXPECT_EQ(gl_pChinaMarket->SinaRTSize(), 2);

		// 恢复原状
		while (gl_pChinaMarket->SinaRTSize() > 0) gl_pChinaMarket->PopSinaRT();
	}

	TEST_F(CProductSinaRTTest, TestParseSinaRT) {
		vector<CWebRTDataPtr> vWebRTData;
		const auto pWebData = make_shared<CWebData>();
		EXPECT_FALSE(sinaRT.ParseSinaRT(vWebRTData, pWebData)) << "新浪实时数据非json制式，无需解析";
	}
}
