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

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

#include"nlohmannJsonDeclaration.h"

constexpr int __DefaultWebDataBufferSize__ = 1024 * 1024;

class CWebData final : public CObject {
public:
	friend CVirtualWebInquiry;
	friend CNeteaseRTWebInquiry;
	friend CSinaRTWebInquiry;
	friend CTengxunRTWebInquiry;
	friend CNeteaseDayLineWebInquiry;
	friend CNeteaseDayLineWebData;

	CWebData();
	// 不允许赋值。
	CWebData(const CWebData&) = delete;
	CWebData& operator=(const CWebData&) = delete;
	CWebData(const CWebData&&) noexcept = delete;
	CWebData& operator=(const CWebData&&) noexcept = delete;
	~CWebData() override;

	bool IsProcessedAllTheData(void) const noexcept {
		if (m_lCurrentPos < m_lBufferLength) return false;
		else return true;
	}

	void IncreaseCurrentPos(long lNumberOfChars = 1) noexcept { m_lCurrentPos += lNumberOfChars; }
	void ResetCurrentPos(void) noexcept { m_lCurrentPos = 0; }

	bool OutOfRange(void) const noexcept {
		if (m_lCurrentPos >= m_lBufferLength) return true;
		else return false;
	}

	void Resize(long lSize) {
		m_sDataBuffer.resize(lSize);
		m_lBufferLength = lSize;
	}

	time_t GetTime(void) const noexcept { return m_tTime; }
	void SetTime(time_t tTime) noexcept { m_tTime = tTime; }
	CString GetStockCode(void) const noexcept { return m_strStockCode; }
	void SetStockCode(CString strStockCode) noexcept { m_strStockCode = strStockCode; }
	INT64 GetBufferLength(void) const noexcept { return m_lBufferLength; }
	void SetBufferLength(INT64 lValue) noexcept { m_lBufferLength = lValue; }
	INT64 GetCurrentPos(void) const noexcept { return m_lCurrentPos; }
	void SetCurrentPos(INT64 lValue) noexcept { m_lCurrentPos = lValue; }

	string GetDataBuffer(void) noexcept { return m_sDataBuffer; }
	bool GetData(char* buffer, INT64 lDataLength, INT64 lStartPosition);
	bool GetData(char* buffer, INT64 lDataLength); // 默认从m_lCurrentPos开始拷贝
	bool SetData(char* buffer, INT64 lDataLength, INT64 lStartPosition);
	bool SetData(char* buffer, INT64 lDataLength); // 默认从m_lCurrentPos开始填充。

	char GetData(INT64 lIndex) const { return m_sDataBuffer.at(lIndex); }
	void SetData(INT64 lIndex, char cValue) { m_sDataBuffer.at(lIndex) = cValue; }
	char GetCurrentPosData(void) const { return m_sDataBuffer.at(m_lCurrentPos); }
	void SetCurrentPosData(char cValue) { m_sDataBuffer.at(m_lCurrentPos) = cValue; }

	void SetJSonContentType(bool fFlag) noexcept { m_fJSonContentType = fFlag; }
	bool IsJSonContentType(void) const noexcept { return m_fJSonContentType; }
	void SetParsed(bool fFlag) noexcept { m_fParsed = fFlag; }
	bool IsParsed(void) const noexcept { return m_fParsed; }

	bool IsVoidJson(void) const noexcept {
		if (IsJSonContentType() && (m_sDataBuffer == _T("{}"))) return true;
		else return false;
	}

	bool CheckNoRightToAccess(string sCode = _T("error"),
	                          string sMessage = _T("You don't have access to this resource."));
	// 默认的为finnhub禁止访问标识（目前只有此选项）

	// 使用boost Property tree将数据转换为json格式。
	bool ParseUsingPropertyTree(long lBeginPos = 0, long lEndPos = 0);
	bool CreatePropertyTree(long lBeginPos = 0, long lEndPos = 0);
	shared_ptr<ptree> GetPTree(void) { return m_ppt; }

	// 使用Nlohmann json将数据转换为json格式。
	bool ParseUsingNlohmannJson(long lBeginPos = 0, long lEndPos = 0);
	bool CreateNlohmannJSon(long lBeginPos = 0, long lEndPos = 0);
	json* GetJSon(void) { return &m_js; }

	// 测试用函数
	void __Test_SetBuffer__(CString strBuffer);

protected:
	time_t m_tTime; // 此数据的提取时间。UTC格式
	CString m_strStockCode; // 此数据的相关证券代码，可以空缺
	string m_sDataBuffer;
	INT64 m_lBufferLength;
	INT64 m_lCurrentPos;

	bool m_fJSonContentType;
	bool m_fParsed;
	bool m_fNoRightToAccess; // 该网络数据报告错误
	CString m_strErrorMessage; // 错误信息
	shared_ptr<ptree> m_ppt;

	// 以下为nlohmann制式的json数据，目前暂不使用
	// 必须将json变量放在最后。如果将nlohmann json变量放在前面，则导致赋值错误，目前原因不明（VS系统bug或者Nholmann json库bug，两者必居其一）。
	// VS17.2.4的bug，17.2.5已修正。
	json m_js;
};

typedef shared_ptr<CWebData> CWebDataPtr;
