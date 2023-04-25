#include"pch.h"

#include"GeneralCheck.h"

#include"JsonParse.h"

#include "WebRTData.h"

using namespace std;

namespace FireBirdTest {
	class CNeteaseRTDataTest {
	public:
		CNeteaseRTDataTest(int index, const CString& strData) {
			m_iIndex = index;
			m_strData = strData;
		}

	public:
		int m_iIndex;
		CString m_strData;
	};

	//不知为何，此json字符串中无法使用中文字符。否则无法解析.
	CNeteaseRTDataTest TestNeteaseRTDataTest1(1, _T("_ntes_quote_callback({\"0601872\":{\"code\":\"0601872\",\"percent\":0.038251,\"high\":5.72,\"askvol3\":311970,\"askvol2\":257996,\"askvol5\":399200,\"askvol4\":201000,\"price\":5.7,\"open\":5.53,\"bid5\":5.65,\"bid4\":5.66,\"bid3\":5.67,\"bid2\":5.68,\"bid1\":5.69,\"low\":5.51,\"updown\":0.21,\"type\":\"SH\",\"symbol\":\"601872\",\"status\":0,\"ask4\":5.73,\"bidvol3\":234700,\"bidvol2\":166300,\"bidvol1\":641291,\"update\":\"2019/11/04 15:59:54\",\"bidvol5\":134500,\"bidvol4\":96600,\"yestclose\":5.49,\"askvol1\":396789,\"ask5\":5.74,\"volume\":78750304,\"ask1\":5.7,\"name\":\"don't use chinese character\", \"ask3\":5.72,\"ask2\":5.71,\"arrow\":\"2\",\"time\":\"2019/11/04 15:59:52\",\"turnover\":443978974}});"));

	class CNeteaseRTDataParserTest1 : public testing::TestWithParam<CNeteaseRTDataTest*> {
		void SetUp() override {
			GeneralCheck();

			const CNeteaseRTDataTest* pData = GetParam();
			m_iIndex = pData->m_iIndex;
			m_strData = pData->m_strData;
			m_pWebData = make_shared<CWebData>();
			m_pWebData->SetJSonContentType(true);
			m_pWebData->Test_SetBuffer_(m_strData);
		}

		void TearDown() override {
			GeneralCheck();
		}

	public:
		int m_iIndex;
		CString m_strData;
		CWebDataPtr m_pWebData;
	};

	INSTANTIATE_TEST_SUITE_P(TestParse123, CNeteaseRTDataParserTest1, testing::Values(&TestNeteaseRTDataTest1));

	TEST_P(CNeteaseRTDataParserTest1, ParseNeteaseRTDataWithNlohmannJSon123) {
		const auto pvWebRTData = ParseNeteaseRTDataWithNlohmannJSon(m_pWebData);

		switch (m_iIndex) {
		case 1:
			EXPECT_EQ(pvWebRTData->size(), 1);
			EXPECT_STREQ(pvWebRTData->at(0)->GetStockName(), _T("don't use chinese character"));
			break;
		default:
			break;
		}
	}
}
