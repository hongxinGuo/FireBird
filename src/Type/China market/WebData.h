/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网络通用数据包结构，用于存储从网路上提取的各种原始数据。
//
// 目前使用boost的propertytree来存储和处理网络数据中的json格式，其速度与nlohmann的json库相比差距较大（是其60%），故而考虑转移之。--20220516
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ClassDeclaration.h"

#include<memory>
#include<string>

using std::string;

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

	bool IsProcessedAllTheData(void) const noexcept {
		if (m_lCurrentPos < m_lBufferLength) return false;
		return true;
	}

	void IncreaseCurrentPos(const long lNumberOfChars = 1) noexcept { m_lCurrentPos += lNumberOfChars; }
	void ResetCurrentPos(void) noexcept { m_lCurrentPos = 0; }

	bool OutOfRange(void) const noexcept {
		if (m_lCurrentPos >= m_lBufferLength) return true;
		return false;
	}

	void Resize(const long lSize) {
		m_sDataBuffer.resize(lSize);
		m_lBufferLength = lSize;
	}

	time_t GetTime(void) const noexcept { return m_tTime; }
	void SetTime(const time_t tTime) noexcept { m_tTime = tTime; }
	CString GetStockCode(void) const noexcept { return m_strStockCode; }
	void SetStockCode(CString strStockCode) noexcept { m_strStockCode = strStockCode; }
	INT64 GetBufferLength(void) const noexcept { return m_lBufferLength; }
	void SetBufferLength(const INT64 lValue) noexcept { m_lBufferLength = lValue; }
	INT64 GetCurrentPos(void) const noexcept { return m_lCurrentPos; }
	void SetCurrentPos(const INT64 lValue) noexcept { m_lCurrentPos = lValue; }

	string GetDataBuffer(void) noexcept { return m_sDataBuffer; }
	bool GetData(char* buffer, INT64 lDataLength, INT64 lStartPosition);
	bool GetData(char* buffer, INT64 lDataLength); // 默认从m_lCurrentPos开始拷贝
	bool SetData(char* buffer, INT64 lDataLength, INT64 lStartPosition);
	bool SetData(char* buffer, INT64 lDataLength); // 默认从m_lCurrentPos开始填充。

	char GetData(const INT64 lIndex) const { return m_sDataBuffer.at(lIndex); }
	void SetData(const INT64 lIndex, const char cValue) { m_sDataBuffer.at(lIndex) = cValue; }
	char GetCurrentPosData(void) const { return m_sDataBuffer.at(m_lCurrentPos); }
	void SetCurrentPosData(const char cValue) { m_sDataBuffer.at(m_lCurrentPos) = cValue; }

	void SetJSonContentType(const bool fFlag) noexcept { m_fJSonContentType = fFlag; }
	bool IsJSonContentType(void) const noexcept { return m_fJSonContentType; }
	void SetParsed(const bool fFlag) noexcept { m_fParsed = fFlag; }
	bool IsParsed(void) const noexcept { return m_fParsed; }

	bool IsVoidJson(void) const noexcept {
		if (IsJSonContentType() && (m_sDataBuffer == _T("{}"))) return true;
		return false;
	}

	bool CheckNoRightToAccess(string sCode = _T("error"), string sMessage = _T("You don't have access to this resource.")); // 默认的为finnhub禁止访问标识（目前只有此选项）
	bool IsNoRightToAccess() const noexcept { return m_fNoRightToAccess; }

	// 使用Nlohmann json将数据转换为json格式。
	bool CreateJson(long lBeginPos = 0, long lEndPos = 0);
	json* GetJSon(void) { return &m_js; }

	// 测试用函数
	void Test_SetBuffer_(CString strBuffer);

protected:
	json m_js; // 存储解析后的json
	time_t m_tTime; // 此数据的提取时间。UTC格式
	CString m_strStockCode; // 此数据的相关证券代码，可以空缺
	string m_sDataBuffer;
	INT64 m_lBufferLength;
	INT64 m_lCurrentPos;

	bool m_fJSonContentType;
	bool m_fParsed;
	bool m_fNoRightToAccess; // 该网络数据报告错误
	CString m_strErrorMessage; // 错误信息
};

using CWebDataPtr = shared_ptr<CWebData>;
