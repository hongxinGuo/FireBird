//////////////////////////////////////////////////////////////////////////////////////////
//
// 系统队列数据中的各个函数皆采用的同步机制以防止线程之间出现数据不同步问题，但此下的测试函数没有测试这种同步机制，
// 只是简单的测试了内部逻辑。
//
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"
#include"SystemMessage.h"

#include"GeneralCheck.h"

#include"ChinaMarket.h"

#include"WebRTDataContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	TEST(RTDataContainerTest, TestInitialize) {
		ASSERT_FALSE(gl_fNormalMode);
		size_t l = gl_systemMessage.GetInformationDequeSize();
		for (int i = 0; i < l; i++) {
			CString str = gl_systemMessage.PopInformationMessage(); // 清除信息队列
		}
		EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
	}

	// 测试有优先级的队列存储临时实时数据。
	TEST(RTDataContainerTest, TestGetSinaRTDataDuqueSize) {
		ASSERT_FALSE(gl_fNormalMode);
		EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 0);
		CWebRTDataPtr pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(100100100);
		gl_WebRTDataContainer.PushSinaData(pRTData);
		CWebRTDataPtr pRTData2 = make_shared<CWebRTData>();
		pRTData2->SetTransactionTime(200200200);
		pRTData2->SetBuy(1);
		gl_WebRTDataContainer.PushSinaData(pRTData2);
		CWebRTDataPtr pRTData3 = make_shared<CWebRTData>();
		pRTData3->SetTransactionTime(200200);
		gl_WebRTDataContainer.PushSinaData(pRTData3);
		CWebRTDataPtr pRTData4 = make_shared<CWebRTData>();
		pRTData4->SetTransactionTime(200);
		gl_WebRTDataContainer.PushSinaData(pRTData4);
		CWebRTDataPtr pRTData5 = make_shared<CWebRTData>();
		pRTData5->SetTransactionTime(200200200);
		pRTData5->SetBuy(2);
		gl_WebRTDataContainer.PushSinaData(pRTData5);  // 这个与pRTData2的时间相同，应该位于pRTData2之后
		EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 5);
		CWebRTDataPtr p2 = gl_WebRTDataContainer.PopSinaData();
		EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 4);
		EXPECT_EQ(p2->GetTransactionTime(), 200);
		p2 = gl_WebRTDataContainer.PopSinaData();
		EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 3);
		EXPECT_EQ(p2->GetTransactionTime(), 200200);
		p2 = gl_WebRTDataContainer.PopSinaData();
		EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 2);
		EXPECT_EQ(p2->GetTransactionTime(), 100100100);
		p2 = gl_WebRTDataContainer.PopSinaData();
		EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 1);
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
		EXPECT_EQ(p2->GetBuy(), 1);
		p2 = gl_WebRTDataContainer.PopSinaData();
		EXPECT_EQ(gl_WebRTDataContainer.GetSinaDataSize(), 0);
		EXPECT_EQ(p2->GetBuy(), 2); // 后放入的相同时间的数据应该位于后面
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
	}

	TEST(RTDataContainerTest, TestGetTengxunRTDataDuqueSize) {
		ASSERT_FALSE(gl_fNormalMode);
		EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 0);
		CWebRTDataPtr pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(100100100);
		gl_WebRTDataContainer.PushTengxunData(pRTData);
		CWebRTDataPtr pRTData2 = make_shared<CWebRTData>();
		pRTData2->SetTransactionTime(200200200);
		pRTData2->SetBuy(1);
		gl_WebRTDataContainer.PushTengxunData(pRTData2);
		CWebRTDataPtr pRTData3 = make_shared<CWebRTData>();
		pRTData3->SetTransactionTime(200200);
		gl_WebRTDataContainer.PushTengxunData(pRTData3);
		CWebRTDataPtr pRTData4 = make_shared<CWebRTData>();
		pRTData4->SetTransactionTime(200);
		gl_WebRTDataContainer.PushTengxunData(pRTData4);
		CWebRTDataPtr pRTData5 = make_shared<CWebRTData>();
		pRTData5->SetTransactionTime(200200200);
		pRTData5->SetBuy(2);
		gl_WebRTDataContainer.PushTengxunData(pRTData5);  // 这个与pRTData2的时间相同，应该位于pRTData2之后
		EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 5);
		CWebRTDataPtr p2 = gl_WebRTDataContainer.PopTengxunData();
		EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 4);
		EXPECT_EQ(p2->GetTransactionTime(), 200);
		p2 = gl_WebRTDataContainer.PopTengxunData();
		EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 3);
		EXPECT_EQ(p2->GetTransactionTime(), 200200);
		p2 = gl_WebRTDataContainer.PopTengxunData();
		EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 2);
		EXPECT_EQ(p2->GetTransactionTime(), 100100100);
		p2 = gl_WebRTDataContainer.PopTengxunData();
		EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 1);
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
		EXPECT_EQ(p2->GetBuy(), 1);
		p2 = gl_WebRTDataContainer.PopTengxunData();
		EXPECT_EQ(gl_WebRTDataContainer.GetTengxunDataSize(), 0);
		EXPECT_EQ(p2->GetBuy(), 2); // 后放入的相同时间的数据应该位于后面
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
	}

	TEST(RTDataContainerTest, TestGetNeteaseRTDataDuqueSize) {
		ASSERT_FALSE(gl_fNormalMode);
		EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 0);
		CWebRTDataPtr pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(100100100);
		gl_WebRTDataContainer.PushNeteaseData(pRTData);
		CWebRTDataPtr pRTData2 = make_shared<CWebRTData>();
		pRTData2->SetTransactionTime(200200200);
		pRTData2->SetBuy(1);
		gl_WebRTDataContainer.PushNeteaseData(pRTData2);
		CWebRTDataPtr pRTData3 = make_shared<CWebRTData>();
		pRTData3->SetTransactionTime(200200);
		gl_WebRTDataContainer.PushNeteaseData(pRTData3);
		CWebRTDataPtr pRTData4 = make_shared<CWebRTData>();
		pRTData4->SetTransactionTime(200);
		gl_WebRTDataContainer.PushNeteaseData(pRTData4);
		CWebRTDataPtr pRTData5 = make_shared<CWebRTData>();
		pRTData5->SetTransactionTime(200200200);
		pRTData5->SetBuy(2);
		gl_WebRTDataContainer.PushNeteaseData(pRTData5);  // 这个与pRTData2的时间相同，应该位于pRTData2之后
		EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 5);
		CWebRTDataPtr p2 = gl_WebRTDataContainer.PopNeteaseData();
		EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 4);
		EXPECT_EQ(p2->GetTransactionTime(), 200);
		p2 = gl_WebRTDataContainer.PopNeteaseData();
		EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 3);
		EXPECT_EQ(p2->GetTransactionTime(), 200200);
		p2 = gl_WebRTDataContainer.PopNeteaseData();
		EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 2);
		EXPECT_EQ(p2->GetTransactionTime(), 100100100);
		p2 = gl_WebRTDataContainer.PopNeteaseData();
		EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 1);
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
		EXPECT_EQ(p2->GetBuy(), 1);
		p2 = gl_WebRTDataContainer.PopNeteaseData();
		EXPECT_EQ(gl_WebRTDataContainer.GetNeteaseDataSize(), 0);
		EXPECT_EQ(p2->GetBuy(), 2); // 后放入的相同时间的数据应该位于后面
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
	}
}