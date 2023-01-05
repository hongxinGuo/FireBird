#include"pch.h"

#include"afxinet.h"

#include"GeneralCheck.h"

#include"VirtualWebInquiry.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace FireBirdTest {
	class CVirtualWebInquiryTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) { GeneralCheck(); }

		static void TearDownTestSuite(void) { GeneralCheck(); }

		void SetUp(void) override {
			GeneralCheck();
			EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();

			m_VirtualWebInquiry.SetInquiringString(_T(""));
			m_VirtualWebInquiry.SetReadingWebData(false);
		}

	public:
		CVirtualWebInquiry m_VirtualWebInquiry;
	};

	TEST_F(CVirtualWebInquiryTest, TestInitialize) {
		EXPECT_STREQ(m_VirtualWebInquiry.GetHeaders(), _T(""));
		EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringString(), _T(""));
		EXPECT_STREQ(m_VirtualWebInquiry.GetInquiryFunction(), _T(""));
		EXPECT_STREQ(m_VirtualWebInquiry.GetInquiryToken(), _T(""));
		EXPECT_EQ(m_VirtualWebInquiry.GetByteRead(), 0);
		EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
		EXPECT_FALSE(m_VirtualWebInquiry.IsReportStatus());
	}

	TEST_F(CVirtualWebInquiryTest, TestGetHeaders) {
		EXPECT_STREQ(m_VirtualWebInquiry.GetHeaders(), _T(""));
		m_VirtualWebInquiry.SetHeaders(_T("abcdefg"));
		EXPECT_STREQ(m_VirtualWebInquiry.GetHeaders(), _T("abcdefg"));
	}

	TEST_F(CVirtualWebInquiryTest, TestReset) {
		m_VirtualWebInquiry.SetByteRead(1000);
		m_VirtualWebInquiry.Reset();
		EXPECT_EQ(m_VirtualWebInquiry.GetByteRead(), 0);
		EXPECT_FALSE(m_VirtualWebInquiry.IsWebError());
	}

	TEST_F(CVirtualWebInquiryTest, TestGetInquiringStringPrefix) {
		m_VirtualWebInquiry.SetInquiryFunction(_T("abcdefghigh"));
		EXPECT_STREQ(m_VirtualWebInquiry.GetInquiryFunction(), _T("abcdefghigh"));
		m_VirtualWebInquiry.SetInquiryFunction(_T(""));
	}

	TEST_F(CVirtualWebInquiryTest, TestGetInquiringStringSuffix) {
		m_VirtualWebInquiry.SetInquiryToken(_T("cdefghigh"));
		EXPECT_STREQ(m_VirtualWebInquiry.GetInquiryToken(), _T("cdefghigh"));
		m_VirtualWebInquiry.SetInquiryToken(_T(""));
	}

	TEST_F(CVirtualWebInquiryTest, TestReportStatus) { EXPECT_TRUE(m_VirtualWebInquiry.ReportStatus(0)); }

	TEST_F(CVirtualWebInquiryTest, TestCreateTotalInquiringString) {
		CString str = _T("abcdef");
		m_VirtualWebInquiry.CreateTotalInquiringString(str);
		EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringString(), str);
	}

	TEST_F(CVirtualWebInquiryTest, TestIsReadingWebData) {
		EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
		m_VirtualWebInquiry.SetReadingWebData(true);
		EXPECT_TRUE(m_VirtualWebInquiry.IsReadingWebData());
		m_VirtualWebInquiry.SetReadingWebData(false);
		EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
	}

	TEST_F(CVirtualWebInquiryTest, TestIsWebError) {
		EXPECT_FALSE(m_VirtualWebInquiry.IsWebError());
		m_VirtualWebInquiry.SetErrorCode(102);
		EXPECT_TRUE(m_VirtualWebInquiry.IsWebError());
		EXPECT_EQ(m_VirtualWebInquiry.GetErrorCode(), static_cast<DWORD>(102));
		m_VirtualWebInquiry.SetErrorCode(0);
		EXPECT_FALSE(m_VirtualWebInquiry.IsWebError());
		EXPECT_EQ(m_VirtualWebInquiry.GetErrorCode(), 0);
	}

	TEST_F(CVirtualWebInquiryTest, TestIsTimeout) {
		EXPECT_FALSE(m_VirtualWebInquiry.IsTimeout());
		m_VirtualWebInquiry.SetErrorCode(12002);
		EXPECT_TRUE(m_VirtualWebInquiry.IsTimeout());
		m_VirtualWebInquiry.SetErrorCode(12000);
		EXPECT_FALSE(m_VirtualWebInquiry.IsTimeout());
	}

	TEST_F(CVirtualWebInquiryTest, TestGetByteRead) {
		EXPECT_EQ(m_VirtualWebInquiry.GetByteRead(), 0);
		m_VirtualWebInquiry.SetByteRead(10000);
		EXPECT_EQ(m_VirtualWebInquiry.GetByteRead(), 10000);
		m_VirtualWebInquiry.AddByteRead(10000);
		EXPECT_EQ(m_VirtualWebInquiry.GetByteRead(), 20000);
	}

	TEST_F(CVirtualWebInquiryTest, TestGetInquiringString) {
		EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringString(), _T(""));
		m_VirtualWebInquiry.SetInquiringString(_T("abcdefg"));
		EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringString(), _T("abcdefg"));
		m_VirtualWebInquiry.AppendInquiringString(_T("hijk"));
		EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringString(), _T("abcdefghijk"));
		m_VirtualWebInquiry.CreateTotalInquiringString(_T("dcba"));
		EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringString(), _T("dcba"));
	}

	TEST_F(CVirtualWebInquiryTest, TestGetInquiringNumber) {
		EXPECT_EQ(m_VirtualWebInquiry.GetInquiringNumber(), 500) << _T("Ä¬ÈÏÖµÎª500");
		m_VirtualWebInquiry.SetInquiringNumber(800);
		EXPECT_EQ(m_VirtualWebInquiry.GetInquiringNumber(), 800);
	}

	TEST_F(CVirtualWebInquiryTest, TestGetTotalByteRead) {
		m_VirtualWebInquiry.ClearTotalByteRead();
		EXPECT_EQ(m_VirtualWebInquiry.GetTotalByteRead(), 0);
		m_VirtualWebInquiry.SetTotalByteRead(1000);
		EXPECT_EQ(m_VirtualWebInquiry.GetTotalByteRead(), 1000);
		m_VirtualWebInquiry.ClearTotalByteRead();
		EXPECT_EQ(m_VirtualWebInquiry.GetTotalByteRead(), 0);
	}
}
