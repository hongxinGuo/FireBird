/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͨ�����ݰ��ṹ�����ڴ洢����·����ȡ�ĸ���ԭʼ���ݡ�
//
// Ŀǰʹ��boost��propertytree���洢�ʹ������������е�json��ʽ�����ٶ���nlohmann��json����Ȳ��ϴ�����60%�����ʶ�����ת��֮��--20220516
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ClassDeclaration.h"

using namespace std;
#include<memory>
#include<vector>
#include<string>

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
	// ������ֵ��
	CWebData(const CWebData&) = delete;
	CWebData& operator=(const CWebData&) = delete;
	CWebData(const CWebData&&) noexcept = delete;
	CWebData& operator=(const CWebData&&) noexcept = delete;
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
	bool GetData(char* buffer, INT64 lDataLength); // Ĭ�ϴ�m_lCurrentPos��ʼ����
	bool SetData(char* buffer, INT64 lDataLength, INT64 lStartPosition);
	bool SetData(char* buffer, INT64 lDataLength); // Ĭ�ϴ�m_lCurrenPos��ʼ��䡣

	char GetData(INT64 lIndex) const { return m_sDataBuffer.at(lIndex); }
	void SetData(INT64 lIndex, char cValue) { m_sDataBuffer.at(lIndex) = cValue; }
	char GetCurrentPosData(void) const { return m_sDataBuffer.at(m_lCurrentPos); }
	void SetCurrentPosData(char cValue) { m_sDataBuffer.at(m_lCurrentPos) = cValue; }

	void SetJSonContentType(bool fFlag) noexcept { m_fJSonContentType = fFlag; }
	bool IsJSonContentType(void) noexcept { return m_fJSonContentType; }
	void SetParsed(bool fFlag) noexcept { m_fParsed = fFlag; }
	bool IsParsed(void) noexcept { return m_fParsed; }
	bool IsVoidJSon(void) noexcept { if (IsJSonContentType() && (m_sDataBuffer.size() == 2)) return true; else return false; }

	// ʹ��boost Property tree������ת��Ϊjson��ʽ��
	bool ParseWithPropertyTree(long lBeginPos = 0, long lEndPos = 0);
	bool CreatePropertyTree(long lBeginPos = 0, long lEndPos = 0);
	shared_ptr<ptree> GetPTree(void) { return m_ppt; }

	// ʹ��Nlohmann json������ת��Ϊjson��ʽ��
	bool ParseWithNlohmannJson(long lBeginPos = 0, long lEndPos = 0);
	bool CreateNlohmannJSon(long lBeginPos = 0, long lEndPos = 0);
	json* GetJSon(void) { return &m_js; }

	// �����ú���
	void __TEST_SetBuffer(CString strBuffer);

protected:
	time_t m_tTime; // �����ݵ���ȡʱ�䡣UTC��ʽ
	CString m_strStockCode; // �����ݵ����֤ȯ���룬���Կ�ȱ
	string m_sDataBuffer;
	INT64 m_lBufferLength;
	INT64 m_lCurrentPos;

	bool m_fJSonContentType;
	bool m_fParsed;
	shared_ptr<ptree> m_ppt;

	// ����Ϊnlohmann��ʽ��json���ݣ�Ŀǰ�ݲ�ʹ��
	json m_js;
};

typedef shared_ptr<CWebData> CWebDataPtr;
