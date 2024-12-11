#include"pch.h"

#include"ConcurrentQueueHeader.h"
#include"GeneralCheck.h"

import FireBird.Market.World;
import FireBird.Market.China;

import FireBird.WebProduct.TengxunRT;

using namespace testing;

namespace FireBirdTest {
	class CProductTengxunRTTest : public ::testing::Test {
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
			tengxunRT.SetMarket(gl_pChinaMarket);
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CProductTengxunRT tengxunRT;
	};

	TEST_F(CProductTengxunRTTest, TestInitialize) {
		EXPECT_STREQ(tengxunRT.GetInquiryFunction(), _T("http://qt.gtimg.cn/q="));
	}

	TEST_F(CProductTengxunRTTest, TestCreateMessage) {
		const CString strInquiry = tengxunRT.CreateMessage();
		EXPECT_STREQ(strInquiry.Left(21), _T("http://qt.gtimg.cn/q="));
		EXPECT_GT(strInquiry.GetLength(), 21) << "无需检查随后的股票代码制式";
	}

	TEST_F(CProductTengxunRTTest, TestParseTengxunRTData1) {
		// 正确的两个数据
		CWebRTDataPtr pRTData;
		const CString strData = _T("v_sh600000=\"1~浦发银行~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\nv_sh600001=\"1~浦发银行~600001~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\n");
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->Test_SetBuffer_(strData);
		EXPECT_FALSE(gl_qChinaMarketRTData.try_dequeue(pRTData));

		tengxunRT.ParseAndStoreWebData(pData);

		EXPECT_TRUE(gl_qChinaMarketRTData.try_dequeue(pRTData));

		//恢复原状
		while (gl_qChinaMarketRTData.try_dequeue(pRTData));
	}
}
