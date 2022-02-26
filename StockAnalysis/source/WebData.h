/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网络通用数据包结构。
//
// 用于存储从网路上提取的各种原始数据。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

class CVirtualWebInquiry;
class CNeteaseDayLineWebData;

using namespace std;
#include<memory>
#include<vector>
#include<string>

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

class CWebData final : public CObject {
public:
	friend CVirtualWebInquiry;
	friend CNeteaseDayLineWebData;
	CWebData();
	~CWebData();

	bool IsProcessedAllTheData(void) const noexcept { if (m_lCurrentPos < m_lBufferLength) return false; else return true; }

	void IncreaseCurrentPos(long lNumberOfChars = 1) noexcept { m_lCurrentPos += lNumberOfChars; }
	void ResetCurrentPos(void) noexcept { m_lCurrentPos = 0; }
	bool OutOfRange(void) { if (m_lCurrentPos >= m_lBufferLength) return true; else return false; }
	void Resize(long lSize) { m_sDataBuffer.resize(lSize); m_lBufferLength = lSize; }

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
	bool SetData(char* buffer, INT64 lDataLength); // 默认从m_lCurrenPos开始填充。

	char GetData(INT64 lIndex) const { return m_sDataBuffer.at(lIndex); }
	void SetData(INT64 lIndex, char cValue) { m_sDataBuffer.at(lIndex) = cValue; }
	char GetCurrentPosData(void) const { return m_sDataBuffer.at(m_lCurrentPos); }
	void SetCurrentPosData(char cValue) { m_sDataBuffer.at(m_lCurrentPos) = cValue; }

	void SetJSonContentType(bool fFlag) noexcept { m_fJSonContentType = fFlag; }
	bool IsJSonContentType(void) noexcept { return m_fJSonContentType; }
	bool IsSucceedCreatePTree(void) noexcept { return m_fSucceedCreatePTree; }

	bool CreatePTree(ptree& pt, long lBeginPos = 0, long lEndPos = 0);
	bool CreatePTree(long lBeginPos = 0, long lEndPos = 0);
	shared_ptr<ptree> GetPTree(void) { return m_ppt; }

	// 测试用函数
	void __TEST_SetBuffer__(CString strBuffer);

protected:
	time_t m_tTime; // 此数据的提取时间。UTC格式
	CString m_strStockCode; // 此数据的相关证券代码，可以空缺
	string m_sDataBuffer;
	INT64 m_lBufferLength;
	INT64 m_lCurrentPos;

	bool m_fJSonContentType;
	bool m_fSucceedCreatePTree;
	shared_ptr<ptree> m_ppt;
};

typedef shared_ptr<CWebData> CWebDataPtr;
