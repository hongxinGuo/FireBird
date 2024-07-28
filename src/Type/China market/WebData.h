/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͨ�����ݰ��ṹ�����ڴ洢����·����ȡ�ĸ���ԭʼ���ݡ�
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ClassDeclaration.h"

#include"nlohmannJsonDeclaration.h"

constexpr int DefaultWebDataBufferSize_ = 1024 * 1024;

class CWebData final {
public:
	friend CVirtualDataSource;
	friend CInquireEngine;
	friend CDayLineWebData;

	CWebData();
	// ������ֵ��
	CWebData(const CWebData&) = delete;
	CWebData& operator=(const CWebData&) = delete;
	CWebData(const CWebData&&) noexcept = delete;
	CWebData& operator=(const CWebData&&) noexcept = delete;
	~CWebData();

	void Resize(const long lSize) { m_sDataBuffer.resize(lSize); }

	void IncreaseCurrentPos(const long lNumberOfChars = 1) noexcept { m_lCurrentPos += lNumberOfChars; }
	void ResetCurrentPos() noexcept { m_lCurrentPos = 0; }

	bool OutOfRange() const noexcept { return m_lCurrentPos >= m_sDataBuffer.size(); }

	bool IsLastDataParagraph() const noexcept { return m_lCurrentPos >= m_sDataBuffer.size() - 2; }// �Ѷ������һ������

	time_t GetTime() const noexcept { return m_tTime; }
	void SetTime(const time_t tTime) noexcept { m_tTime = tTime; }
	CString GetStockCode() const noexcept { return m_strStockCode; }
	void SetStockCode(const CString& strStockCode) noexcept { m_strStockCode = strStockCode; }
	size_t GetBufferLength() const noexcept { return m_sDataBuffer.size(); }

	string GetDataBuffer() noexcept { return m_sDataBuffer; }
	long GetCurrentPos() const noexcept { return m_lCurrentPos; }
	void SetCurrentPos(const long lValue) noexcept { m_lCurrentPos = lValue; }
	string_view GetStringView(const int iDataPos, const int iDataLength) const { return string_view(m_sDataBuffer.c_str() + iDataPos, iDataLength); }
	bool GetData(char* buffer, long lDataLength) const; // ��m_lCurrentPos��ʼ����
	bool SetData(const char* buffer, long lDataLength); // ��m_lCurrentPos��ʼ��䡣
	char GetData(const long lIndex) const { return m_sDataBuffer.at(lIndex); }
	void SetData(const long lIndex, const char cValue) { m_sDataBuffer.at(lIndex) = cValue; }
	char GetCurrentPosData() const { return m_sDataBuffer.at(m_lCurrentPos); }
	void SetCurrentPosData(const char cValue) { m_sDataBuffer.at(m_lCurrentPos) = cValue; }

	bool IsVoidJson() const noexcept { return m_sDataBuffer == _T("{}"); }

	//
	string_view GetCurrentSinaData();
	string_view GetCurrentTengxunData();

	// ʹ��Nlohmann json������ת��Ϊjson��ʽ��
	bool CreateJson(json& js, long lBeginPos = 0, long lEndPos = 0) const;

	// �����ú���
	void Test_SetBuffer_(CString strBuffer);

protected:
	time_t m_tTime; // �����ݵ���ȡʱ�䡣UTC��ʽ
	CString m_strStockCode; // �����ݵ����֤ȯ���룬���Կ�ȱ
	string m_sDataBuffer;
	long m_lCurrentPos;
	long m_lCurrentParagraphStartPos; // ��ǰ����ʼλ��
	string_view m_svCurrentParagraph{}; // ��ǰ������

	CString m_strErrorMessage; // ������Ϣ
};

using CWebDataPtr = shared_ptr<CWebData>;
