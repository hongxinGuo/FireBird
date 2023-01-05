//////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"GeneralCheck.h"

#include<memory>
using std::make_shared;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace FireBirdTest {
	class CSystemDataTest : public ::testing::Test {
	protected:
		void SetUp(void) override { GeneralCheck(); }

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}
	};

	TEST_F(CSystemDataTest, TestFinnhubSocket) {
		EXPECT_EQ(gl_SystemData.GetFinnhubSocketSize(), 0);
		auto pData = make_shared<CFinnhubSocket>();
		pData->m_sSymbol = _T("Test1");
		gl_SystemData.PushFinnhubSocket(pData);
		pData = nullptr;
		EXPECT_EQ(gl_SystemData.GetFinnhubSocketSize(), 1);
		pData = gl_SystemData.PopFinnhubSocket();
		EXPECT_TRUE(pData->m_sSymbol == _T("Test1"));
	}

	TEST_F(CSystemDataTest, TestTiingoCryptoSocket) {
		EXPECT_EQ(gl_SystemData.GetTiingoCryptoSocketSize(), 0);
		auto pData = make_shared<CTiingoCryptoSocket>();
		pData->m_sSymbol = _T("Test1");
		gl_SystemData.PushTiingoCryptoSocket(pData);
		pData = nullptr;
		EXPECT_EQ(gl_SystemData.GetTiingoCryptoSocketSize(), 1);
		pData = gl_SystemData.PopTiingoCryptoSocket();
		EXPECT_TRUE(pData->m_sSymbol == _T("Test1"));
	}

	TEST_F(CSystemDataTest, TestTiingoIEXSocket) {
		EXPECT_EQ(gl_SystemData.GetTiingoIEXSocketSize(), 0);
		auto pData = make_shared<CTiingoIEXSocket>();
		pData->m_sSymbol = _T("Test1");
		gl_SystemData.PushTiingoIEXSocket(pData);
		pData = nullptr;
		EXPECT_EQ(gl_SystemData.GetTiingoIEXSocketSize(), 1);
		pData = gl_SystemData.PopTiingoIEXSocket();
		EXPECT_TRUE(pData->m_sSymbol == _T("Test1"));
	}

	TEST_F(CSystemDataTest, TestTiingoForexSocket) {
		EXPECT_EQ(gl_SystemData.GetTiingoForexSocketSize(), 0);
		auto pData = make_shared<CTiingoForexSocket>();
		pData->m_sSymbol = _T("Test1");
		gl_SystemData.PushTiingoForexSocket(pData);
		pData = nullptr;
		EXPECT_EQ(gl_SystemData.GetTiingoForexSocketSize(), 1);
		pData = gl_SystemData.PopTiingoForexSocket();
		EXPECT_TRUE(pData->m_sSymbol == _T("Test1"));
	}
}
