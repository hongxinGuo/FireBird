/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网络通用数据包结构，用于存储从网路上提取的各种原始数据。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ClassDeclaration.h"
#include"nlohmannJsonDeclaration.h"

constexpr int DefaultWebDataBufferSize_ = 1024 * 1024;

class CWebData {
public:
	friend CVirtualDataSource;
	friend CInquireEngine;
	friend CDayLineWebData;

	CWebData();
	// 不允许赋值。
	CWebData(const CWebData&) = delete;
	CWebData& operator=(const CWebData&) = delete;
	CWebData(const CWebData&&) noexcept = delete;
	CWebData& operator=(const CWebData&&) noexcept = delete;
	virtual ~CWebData();

	void Resize(const size_t lSize) { m_sDataBuffer.resize(lSize); }

	void IncreaseCurrentPos(const size_t lNumberOfChars = 1) noexcept { m_lCurrentPos += lNumberOfChars; }
	void ResetCurrentPos() noexcept { m_lCurrentPos = 0; }

	bool OutOfRange() const noexcept { return m_lCurrentPos >= m_sDataBuffer.size(); }

	bool IsLastDataParagraph() const noexcept { return m_lCurrentPos >= m_sDataBuffer.size() - 2; }// 已读至最后一段数据

	time_t GetTime() const noexcept { return m_tpTime.time_since_epoch().count(); }
	void SetTime(chrono::sys_seconds time) noexcept { m_tpTime = time; }
	void SetTime(const time_t tTime) noexcept { m_tpTime = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::from_time_t(tTime)); }
	string GetStockCode() const noexcept { return m_strStockCode; }
	void SetStockCode(const string& strStockCode) noexcept { m_strStockCode = strStockCode; }
	size_t GetBufferLength() const noexcept { return m_sDataBuffer.size(); }

	string GetDataBuffer() noexcept { return m_sDataBuffer; }
	auto GetCurrentPos() const noexcept { return m_lCurrentPos; }
	void SetCurrentPos(const size_t lValue) noexcept { m_lCurrentPos = lValue; }
	//string_view GetStringView(const size_t iDataPos, const size_t iDataLength) const { return string_view(m_sDataBuffer.c_str() + iDataPos, iDataLength); }
	string_view GetStringView(const size_t iDataPos, const size_t iDataLength) const { return m_svDataBuffer.substr(iDataPos, iDataLength); }
	string_view GetStringView(const size_t iDataPos) const { return m_svDataBuffer.substr(iDataPos); }
	string_view GetStringView() const { return m_svDataBuffer; }
	bool GetData(char* buffer, size_t lDataLength) const; // 从m_lCurrentPos开始拷贝
	bool SetData(const char* buffer, size_t lDataLength); // 从m_lCurrentPos开始填充。
	char GetData(const size_t lIndex) const { return m_sDataBuffer.at(lIndex); }
	void SetData(const size_t lIndex, const char cValue) { m_sDataBuffer.at(lIndex) = cValue; }
	char GetCurrentPosData() const { return m_sDataBuffer.at(m_lCurrentPos); }
	void SetCurrentPosData(const char cValue) { m_sDataBuffer.at(m_lCurrentPos) = cValue; }

	bool IsVoidJson() const noexcept { return m_sDataBuffer == _T("{}") || m_sDataBuffer == _T("[]"); }

	//
	string_view GetCurrentSinaData();
	string_view GetCurrentTengxunData();

	// 使用Nlohmann json将数据转换为json格式。
	bool CreateJson(json& js, long lBeginPos = 0, long lEndPos = 0) const;

	// 测试用函数
	void Test_SetBuffer_(const string& strBuffer);

protected:
	chrono::sys_seconds m_tpTime;// 此数据的提取时间。time point格式
	string m_strStockCode; // 此数据的相关证券代码，可以空缺
	string m_sDataBuffer;
	string_view m_svDataBuffer; // m_sDataBuffer的string_view
	size_t m_lCurrentPos;
	size_t m_lCurrentParagraphStartPos; // 当前段起始位置
	string_view m_svCurrentParagraph{}; // 当前段数据

	string m_strErrorMessage; // 错误信息
};

using CWebDataPtr = shared_ptr<CWebData>;
