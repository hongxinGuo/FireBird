#include"pch.h"

#include"afxinet.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"VirtualWebInquiry.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CVirtualWebInquiryTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
			EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
		}

		virtual void TearDown(void) override {
			// clearup
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
		EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringStringPrefix(), _T(""));
		EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringStringSuffix(), _T(""));
		EXPECT_EQ(m_VirtualWebInquiry.GetByteReaded(), 0);
		EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
		EXPECT_FALSE(m_VirtualWebInquiry.IsReportStatus());
	}

	TEST_F(CVirtualWebInquiryTest, TestGetHeaders) {
		EXPECT_STREQ(m_VirtualWebInquiry.GetHeaders(), _T(""));
		m_VirtualWebInquiry.SetHeaders(_T("abcdefg"));
		EXPECT_STREQ(m_VirtualWebInquiry.GetHeaders(), _T("abcdefg"));
	}

	TEST_F(CVirtualWebInquiryTest, TestReset) {
		m_VirtualWebInquiry.SetByteReaded(1000);
		m_VirtualWebInquiry.SetWebError(true);
		m_VirtualWebInquiry.Reset();
		EXPECT_EQ(m_VirtualWebInquiry.GetByteReaded(), 0);
		EXPECT_FALSE(m_VirtualWebInquiry.IsWebError());
	}

	TEST_F(CVirtualWebInquiryTest, TestGetInquiringStringPrefix) {
		m_VirtualWebInquiry.SetInquiryingStringPrefix(_T("abcdefghigh"));
		EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringStringPrefix(), _T("abcdefghigh"));
		m_VirtualWebInquiry.SetInquiryingStringPrefix(_T(""));
	}

	TEST_F(CVirtualWebInquiryTest, TestGetInquiringStringMiddle) {
		m_VirtualWebInquiry.SetInquiryingStringMiddle(_T("bcdefghigh"));
		EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringStringMiddle(), _T("bcdefghigh"));
		m_VirtualWebInquiry.SetInquiryingStringMiddle(_T(""));
	}

	TEST_F(CVirtualWebInquiryTest, TestGetInquiringStringSuffix) {
		m_VirtualWebInquiry.SetInquiryingStringSuffix(_T("cdefghigh"));
		EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringStringSuffix(), _T("cdefghigh"));
		m_VirtualWebInquiry.SetInquiryingStringSuffix(_T(""));
	}

	TEST_F(CVirtualWebInquiryTest, TestTransferReceivedDataToWebData) {
		CString str = _T("abcdefghijklmnop");
		m_VirtualWebInquiry.__TESTSetBuffer(str);
		CWebDataPtr p = m_VirtualWebInquiry.TransferReceivedDataToWebData();
		EXPECT_EQ(p->GetBufferLength(), str.GetLength());
		char buffer[30];
		int i = 0;
		while ((i < 30) && (p->GetData(i) != 0x000)) {
			buffer[i] = p->GetData(i);
			i++;
		}
		buffer[i] = 0x000;
		CString strCompare = buffer;
		EXPECT_STREQ(strCompare, str);

		str = _T("");
		m_VirtualWebInquiry.__TESTSetBuffer(str); // 清除掉之前的设置。
	}

	TEST_F(CVirtualWebInquiryTest, TestReportStatus) {
		EXPECT_TRUE(m_VirtualWebInquiry.ReportStatus(0));
	}

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
		m_VirtualWebInquiry.SetWebError(true);
		EXPECT_TRUE(m_VirtualWebInquiry.IsWebError());
		m_VirtualWebInquiry.SetWebError(false);
		EXPECT_FALSE(m_VirtualWebInquiry.IsWebError());
	}

	TEST_F(CVirtualWebInquiryTest, TestGetByteReaded) {
		EXPECT_EQ(m_VirtualWebInquiry.GetByteReaded(), 0);
		m_VirtualWebInquiry.SetByteReaded(10000);
		EXPECT_EQ(m_VirtualWebInquiry.GetByteReaded(), 10000);
		m_VirtualWebInquiry.AddByteReaded(10000);
		EXPECT_EQ(m_VirtualWebInquiry.GetByteReaded(), 20000);
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

	TEST_F(CVirtualWebInquiryTest, TestGetShortestInquiringInterval) {
		EXPECT_EQ(m_VirtualWebInquiry.GetShortestInquiringInterval(), 1000) << _T("默认值为1000毫秒");
		m_VirtualWebInquiry.SetShortestINquiringInterval(800);
		EXPECT_EQ(m_VirtualWebInquiry.GetShortestInquiringInterval(), 800);

		m_VirtualWebInquiry.SetShortestINquiringInterval(1000);
	}

	TEST_F(CVirtualWebInquiryTest, TestGetInquiringNumber) {
		EXPECT_EQ(m_VirtualWebInquiry.GetInquiringNumber(), 500) << _T("默认值为500");
		m_VirtualWebInquiry.SetInquiringNumber(800);
		EXPECT_EQ(m_VirtualWebInquiry.GetInquiringNumber(), 800);
	}

	TEST_F(CVirtualWebInquiryTest, TestGetTotalByteReaded) {
		m_VirtualWebInquiry.ClearTotalByteReaded();
		EXPECT_EQ(m_VirtualWebInquiry.GetTotalByteReaded(), 0);
		m_VirtualWebInquiry.SetTotalByteReaded(1000);
		EXPECT_EQ(m_VirtualWebInquiry.GetTotalByteReaded(), 1000);
		m_VirtualWebInquiry.ClearTotalByteReaded();
		EXPECT_EQ(m_VirtualWebInquiry.GetTotalByteReaded(), 0);
	}
}