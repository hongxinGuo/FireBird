/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网络通用数据包结构，用于存储从网路上提取的各种原始数据。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ClassDeclaration.h"

using std::string;
using std::string_view;

#include"nlohmannJsonDeclaration.h"

constexpr int DefaultWebDataBufferSize_ = 1024 * 1024;

class CWebData final {
public:
	friend CVirtualDataSource;
	friend CDayLineWebData;

	CWebData();
	// 不允许赋值。
	CWebData(const CWebData&) = delete;
	CWebData& operator=(const CWebData&) = delete;
	CWebData(const CWebData&&) noexcept = delete;
	CWebData& operator=(const CWebData&&) noexcept = delete;
	~CWebData();

	void Resize(const long lSize) { m_sDataBuffer.resize(lSize); }

	void IncreaseCurrentPos(const long lNumberOfChars = 1) noexcept { m_lCurrentPos += lNumberOfChars; }
	void ResetCurrentPos() noexcept { m_lCurrentPos = 0; }

	bool OutOfRange() const noexcept {
		if (m_lCurrentPos >= m_sDataBuffer.size()) return true;
		return false;
	}
	bool CurrentParagraphOutOfRange() const noexcept {
		if ((m_lCurrentPos < m_lCurrentParagraphStartPos) || (m_lCurrentPos - m_lCurrentParagraphStartPos > m_svCurrentParagraph.size())) return true;
		return false;
	}

	bool IsLastDataParagraph() const noexcept { // 已读至最后一段数据
		if (m_lCurrentPos >= m_sDataBuffer.size() - 2) return true;
		return false;
	}

	time_t GetTime() const noexcept { return m_tTime; }
	void SetTime(const time_t tTime) noexcept { m_tTime = tTime; }
	CString GetStockCode() const noexcept { return m_strStockCode; }
	void SetStockCode(const CString& strStockCode) noexcept { m_strStockCode = strStockCode; }
	INT64 GetBufferLength() const noexcept { return m_sDataBuffer.size(); }

	string GetDataBuffer() noexcept { return m_sDataBuffer; }
	long GetCurrentPos() const noexcept { return m_lCurrentPos; }
	void SetCurrentPos(const long lValue) noexcept { m_lCurrentPos = lValue; }
	long GetCurrentParagraphStartPos() const noexcept { return m_lCurrentParagraphStartPos; }
	void SetCurrentParagraphStartPos(long lPos) noexcept { m_lCurrentParagraphStartPos = lPos; }
	string_view GetCurrentParagraph() const noexcept { return m_svCurrentParagraph; }
	void SetCurrentParagraph(string_view sv) noexcept { m_svCurrentParagraph = sv; }
	string_view GetStringViewData(const int iDatLength) const { return string_view(m_sDataBuffer.c_str() + m_lCurrentPos, iDatLength); }
	string_view GetAllOfNeedProcessStringViewData() const { return string_view(m_sDataBuffer.c_str() + m_lCurrentPos, m_sDataBuffer.size() - m_lCurrentPos); }
	bool GetData(char* buffer, long lDataLength) const; // 从m_lCurrentPos开始拷贝
	bool SetData(const char* buffer, long lDataLength); // 从m_lCurrentPos开始填充。

	char GetData(const long lIndex) const { return m_sDataBuffer.at(lIndex); }
	void SetData(const long lIndex, const char cValue) { m_sDataBuffer.at(lIndex) = cValue; }
	char GetCurrentPosData() const { return m_sDataBuffer.at(m_lCurrentPos); }
	void SetCurrentPosData(const char cValue) { m_sDataBuffer.at(m_lCurrentPos) = cValue; }

	void SetJSonContentType(const bool fFlag) noexcept { m_fJSonContentType = fFlag; }
	bool IsJSonContentType() const noexcept { return m_fJSonContentType; }
	void SetParsed(const bool fFlag) noexcept { m_fParsed = fFlag; }
	bool IsParsed() const noexcept { return m_fParsed; }

	bool IsVoidJson() const noexcept {
		if (IsJSonContentType() && (m_sDataBuffer == _T("{}"))) return true;
		return false;
	}

	bool CheckNoRightToAccess(const string& sCode = _T("error"), const string& sMessage = _T("You don't have access to this resource.")); // 默认的为finnhub禁止访问标识（目前只有此选项）
	bool IsNoRightToAccess() const noexcept { return m_fNoRightToAccess; }

	// 使用Nlohmann json将数据转换为json格式。
	bool CreateJson(long lBeginPos = 0, long lEndPos = 0);
	json* GetJSon() { return &m_js; }

	// 测试用函数
	void Test_SetBuffer_(CString strBuffer);

protected:
	json m_js; // 存储解析后的json
	time_t m_tTime; // 此数据的提取时间。UTC格式
	CString m_strStockCode; // 此数据的相关证券代码，可以空缺
	string m_sDataBuffer;
	long m_lCurrentPos;
	long m_lCurrentParagraphStartPos; // 当前段起始位置
	string_view m_svCurrentParagraph{}; // 当前段数据

	bool m_fJSonContentType;
	bool m_fParsed;
	bool m_fNoRightToAccess; // 该网络数据报告错误
	CString m_strErrorMessage; // 错误信息
};

using CWebDataPtr = shared_ptr<CWebData>;
