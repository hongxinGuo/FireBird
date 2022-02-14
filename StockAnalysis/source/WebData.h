/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͨ�����ݰ��ṹ��
//
// ���ڴ洢����·����ȡ�ĸ���ԭʼ���ݡ�
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

class CVirtualWebInquiry;

using namespace std;
#include<memory>
#include<vector>

class CWebData : public CObject {
public:
	friend CVirtualWebInquiry;
	CWebData();
	~CWebData();

	bool IsProcessedAllTheData(void) const noexcept { if (m_lCurrentPos < m_lBufferLength) return false; else return true; }

	void IncreaseCurrentPos(long lNumberOfChars = 1) noexcept { m_lCurrentPos += lNumberOfChars; }
	void ResetCurrentPos(void) noexcept { m_lCurrentPos = 0; }

	void Resize(long lSize) { m_vDataBuffer.resize(lSize); m_lBufferLength = lSize; }

	time_t GetTime(void) const noexcept { return m_tTime; }
	void SetTime(time_t tTime) noexcept { m_tTime = tTime; }
	CString GetStockCode(void) const noexcept { return m_strStockCode; }
	void SetStockCode(CString strStockCode) noexcept { m_strStockCode = strStockCode; }
	INT64 GetBufferLength(void) const noexcept { return m_lBufferLength; }
	void SetBufferLength(INT64 lValue) noexcept { m_lBufferLength = lValue; }
	INT64 GetCurrentPos(void) const noexcept { return m_lCurrentPos; }
	void SetCurrentPos(INT64 lValue) noexcept { m_lCurrentPos = lValue; }

	bool GetData(char* buffer, INT64 lDataLength, INT64 lStartPosition);
	bool GetData(char* buffer, INT64 lDataLength); // Ĭ�ϴ�m_lCurrentPos��ʼ����
	bool SetData(char* buffer, INT64 lDataLength, INT64 lStartPosition);
	bool SetData(char* buffer, INT64 lDataLength); // Ĭ�ϴ�m_lCurrenPos��ʼ��䡣

	char GetData(INT64 lIndex) const { return m_vDataBuffer.at(lIndex); }
	void SetData(INT64 lIndex, char cValue) { m_vDataBuffer.at(lIndex) = cValue; }
	char GetCurrentPosData(void) const { return m_vDataBuffer.at(m_lCurrentPos); }
	void SetCurrentPosData(char cValue) { m_vDataBuffer.at(m_lCurrentPos) = cValue; }

	string CreateString(long lBeginPos = 0, long lEndPos = 0) { string str(m_vDataBuffer.begin() + lBeginPos, m_vDataBuffer.end() - lEndPos - 1); return str; }

	// �����ú���
	void __TEST_SetBuffer__(CString strBuffer);

protected:
	time_t m_tTime; // �����ݵ���ȡʱ�䡣UTC��ʽ
	CString m_strStockCode; // �����ݵ����֤ȯ���룬���Կ�ȱ
	vector<char> m_vDataBuffer;
	INT64 m_lBufferLength;
	INT64 m_lCurrentPos;
};

typedef shared_ptr<CWebData> CWebDataPtr;
