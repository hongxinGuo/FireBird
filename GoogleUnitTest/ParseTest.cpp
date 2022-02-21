#include"pch.h"

#include"globedef.h"
#include"ChinaMarket.h"
#include"WebInquirer.h"

#include"GeneralCheck.h"

#include"accessory.h"

#include"Parse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CParseTest : public ::testing::Test {
		virtual void SetUp(void) override {
			GeneralCheck();
		}
		virtual void TearDown(void) override {
			gl_fNormalMode = false;
			gl_fTestMode = true;

			GeneralCheck();
		}
	};

	TEST_F(CParseTest, TestParseWebRTDataGetFromSinaServer) {
		INT64 llTotal = gl_pChinaMarket->GetRTDataReceived();
		CString strRTData = _T("var hq_str_sh600000=\"浦发银行,10.120,9.940,10.090,10.180,9.960,10.090,10.100,42609358,428744766.000,453445,10.090,312600,10.080,173519,10.070,242800,10.060,136700,10.050,3600,10.100,3200,10.120,5100,10.130,129900,10.150,476200,10.160,2020-03-20,15:00:00,00,\";\nvar hq_str_sh600004=\"白云机场,13.010,12.900,13.300,13.350,12.990,13.300,13.320,9689808,127814773.000,4810,13.300,11100,13.290,5800,13.280,5200,13.270,10800,13.260,1600,13.320,61100,13.330,46100,13.340,114467,13.350,35000,13.360,2020-03-20,15:00:09,00,\";\nvar hq_str_sh600008=\"首创股份,3.050,3.040,3.050,3.080,3.030,3.050,3.060,22087562,67408587.000,760200,3.050,625400,3.040,570900,3.030,329000,3.020,202000,3.010,9200,3.060,112100,3.070,491897,3.080,449600,3.090,664140,3.100,2020-03-20,15:00:09,00,\";\n");
		CWebDataPtr pData = make_shared<CWebData>();
		pData->__TEST_SetBuffer__(strRTData);
		gl_WebInquirer.PushSinaRTData(pData);
		EXPECT_TRUE(ParseWebRTDataGetFromSinaServer());
		EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 3);
		EXPECT_EQ(gl_WebInquirer.GetSinaRTDataSize(), 0);
		EXPECT_EQ(gl_pChinaMarket->GetRTDataReceived(), llTotal + 3) << "新浪实时数据用于更新接收总数, 此处接收了三个数据";
		for (int i = 0; i < 3; i++) {
			CWebRTDataPtr pRTData = gl_WebRTDataContainer.PopSinaData();
		}
	}

	TEST_F(CParseTest, TestParseWebRTDataGetFromNeteaseServer) {
		INT64 llTotal = gl_pChinaMarket->GetRTDataReceived();
		CString strRTData = "_ntes_quote_callback({\"0600270\": {\"time\": \"2020/04/23 08:30:01\", \"code\": \"0600270\",\"name\":\"don't use chinese character\",\"update\": \"2020/04/23 08:30:01\"} });";
		CWebDataPtr pData = make_shared<CWebData>();
		pData->__TEST_SetBuffer__(strRTData);
		gl_WebInquirer.PushNeteaseRTData(pData);
		EXPECT_TRUE(ParseWebRTDataGetFromNeteaseServer());
		EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 1);
		EXPECT_EQ(gl_WebInquirer.GetNeteaseRTDataSize(), 0);
		EXPECT_EQ(gl_pChinaMarket->GetRTDataReceived(), llTotal + 1) << "网易实时数据用于更新接收总数";
		for (int i = 0; i < 1; i++) {
			CWebRTDataPtr pRTData = gl_WebRTDataContainer.PopNeteaseData();
		}
	}

	TEST_F(CParseTest, TestParseWebRTDataGetFromTengxunServer) {
		INT64 llTotal = gl_pChinaMarket->GetRTDataReceived();
		CString strRTData = _T("v_sz000001=\"51~平安银行~000001~15.59~15.90~15.75~1046363~518391~527971~15.58~2365~15.57~802~15.56~1855~15.55~2316~15.54~320~15.59~661~15.60~15381~15.61~3266~15.62~450~15.63~520~~20190930154003~-0.31~-1.95~15.89~15.57~15.59/1046363/1645828527~1046363~164583~0.54~11.27~~15.89~15.57~2.01~3025.36~3025.38~1.15~17.49~14.31~0.73~-12617~15.73~9.82~12.19~~~1.24~164582.85~0.00~0~~GP-A~68.91~~0.82\";\n");
		CWebDataPtr pData = make_shared<CWebData>();
		pData->__TEST_SetBuffer__(strRTData);
		gl_WebInquirer.PushTengxunRTData(pData);
		EXPECT_TRUE(ParseWebRTDataGetFromTengxunServer());
		EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 1);
		EXPECT_EQ(gl_WebInquirer.GetTengxunRTDataSize(), 0);
		EXPECT_EQ(gl_pChinaMarket->GetRTDataReceived(), llTotal) << "腾讯实时数据不更新";
		for (int i = 0; i < 1; i++) {
			CWebRTDataPtr pRTData = gl_WebRTDataContainer.PopTengxunData();
		}
	}

	TEST_F(CParseTest, TestIsTengxunRTDataInValid) {
		CWebDataPtr pWebDataReceived;
		pWebDataReceived = make_shared<CWebData>();
		CString str = _T("v_pv_none_match=\"1\";\n");
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
		pWebDataReceived->SetBufferLength(str.GetLength());
		pWebDataReceived->ResetCurrentPos();

		EXPECT_TRUE(IsTengxunRTDataInvalid(*pWebDataReceived));
		EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);

		str = _T("v_pv_none_mtch=\"1\";\n");
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
		pWebDataReceived->SetBufferLength(str.GetLength());
		pWebDataReceived->ResetCurrentPos();

		EXPECT_FALSE(IsTengxunRTDataInvalid(*pWebDataReceived));
		EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);
	}
}