#include "pch.h"

#include "CharSetTransfer.h"

using namespace testing;

TEST(CharSetTransferTest, Utf8ToWstring_Empty) {
	std::string empty;
	std::wstring out = Utf8ToWstring(empty);
	EXPECT_TRUE(out.empty());
}

TEST(CharSetTransferTest, Utf8ToWstring_Roundtrip_ASCIIAndUnicode) {
	std::string utf8 = "Hello 世界";
	std::u8string utf = u8"(Hello 世界)";
	// Convert UTF-8 -> wstring -> UTF-8 (via W2Utf8(wstring))
	std::wstring wide = Utf8ToWstring(utf8);
	EXPECT_FALSE(wide.empty());
	std::string round = Wstring2Utf8(wide);
	EXPECT_EQ(round, utf8);
	EXPECT_EQ(round.size(), utf8.size());
}

TEST(CharSetTransferTest, ToUTF8_From_CString) {
	// Construct CString from wide literal; T2Utf8 should produce UTF-8 bytes
	const std::string utf8 = "Test ASCII and 中文";
	CString cs = _T("Test ASCII and 中文");
	std::string out = T2Utf8(cs);
	EXPECT_EQ(out, utf8);
	EXPECT_EQ(out.size(), utf8.size());
}

TEST(CharSetTransferTest, WtoUTF8_From_wstring) {
	const std::wstring w = L"Sample 文本";
	const std::string expected = "Sample 文本";
	std::string out = Wstring2Utf8(w);
	EXPECT_EQ(out, expected);
	EXPECT_EQ(out.size(), expected.size());
}

TEST(CharSetTransferTest, UTF8toW_ASCII) {
	const std::string ascii = "plain ascii";
	const std::wstring wstr = L"plain ascii";
	std::wstring w = Utf8ToWstring(ascii);
	EXPECT_EQ(w, wstr);
	EXPECT_EQ(w.size(), wstr.size());
}

TEST(CharSetTransferTest, UTF8ToGBK_and_GBKtoUTF8_ASCII_Roundtrip) {
	const std::string ascii = "roundtrip ascii";
	std::string gbk = Utf8ToGbk(ascii);
	// ASCII bytes should survive CP conversions
	EXPECT_FALSE(gbk.empty());
	std::string back = Gbk2Utf8(gbk);
	EXPECT_EQ(back, ascii);
	EXPECT_EQ(back.size(), ascii.size());

	const std::string str = "roundtrip 中文";
	std::string gbk2 = Utf8ToGbk(str);
	EXPECT_FALSE(gbk.empty());
	std::string back2 = Gbk2Utf8(gbk2);
	EXPECT_EQ(back2, str);
	EXPECT_EQ(back2.size(), str.size());
}

TEST(CharSetTransferTest, GBKToWstring_Empty) {
	std::string empty;
	std::wstring out = Gbk2Wstring(empty);
	EXPECT_TRUE(out.empty());
}

TEST(CharSetTransferTest, GBK_Roundtrip_With_Unicode) {
	// This verifies the intended behavior: UTF-8 -> GBK -> UTF-8 should restore original text.
	// On a correct implementation this should succeed for characters representable in GBK.
	const std::string original = "浦发银行"; // Chinese text used elsewhere in tests
	std::string gbk = Utf8ToGbk(original);
	EXPECT_FALSE(gbk.empty());
	std::string restored = Gbk2Utf8(gbk);
	// Expect restoration for GBK-representable characters; if implementation has errors this will fail and reveal the bug.
	EXPECT_EQ(restored, original);
	EXPECT_EQ(restored.size(), original.size());
}
