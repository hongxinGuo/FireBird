/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͨ�����ݰ��ṹ��
//
// ���ڴ洢����·����ȡ�ĸ���ԭʼ���ݡ�
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"stdafx.h"

using namespace std;
#include<memory>
#include<vector>

class CWebData : public CObject {
public:
  CWebData() noexcept {
    m_lTime = 0;
    m_pDataBuffer.resize(2048 * 1024);
    m_lBufferLength = 2048 * 1024;
    m_lCurrentPos = 0;
  }
  ~CWebData() {
  }

  bool IsProcessedAllTheData(void) noexcept { if (m_lCurrentPos < m_lBufferLength) return false; else return true; }

  void IncreaseCurrentPos(long lNumberOfChars = 1) noexcept { m_lCurrentPos += lNumberOfChars; }
  void ResetCurrentPos(void) noexcept { m_lCurrentPos = 0; }

  //void Resize(long lSize) noexcept { m_pDataBuffer.resize(lSize); m_lBufferLength = lSize; }

  INT64 GetTime(void) noexcept { return m_lTime; }
  void SetTime(INT64 lTime) noexcept { m_lTime = lTime; }
  long GetBufferLength(void) noexcept { return m_lBufferLength; }
  void SetBufferLength(long lValue) noexcept { m_lBufferLength = lValue; }
  long GetCurrentPos(void) noexcept { return m_lCurrentPos; }
  void SetCurrentPos(long lValue) noexcept { m_lCurrentPos = lValue; }

  bool GetData(char* buffer, long lDataLength, long lStartPosition);
  bool GetData(char* buffer, long lDataLength); // Ĭ�ϴ�m_lCurrentPos��ʼ����
  bool SetData(char* buffer, long lDataLength, long lStartPosition);
  bool SetData(char* buffer, long lDataLength); // Ĭ�ϴ�m_lCurrenPos��ʼ��䡣

  char GetData(long lIndex) { return m_pDataBuffer.at(lIndex); }
  void SetData(long lIndex, char cValue) { m_pDataBuffer.at(lIndex) = cValue; }
  char GetCurrentPosData(void) { return m_pDataBuffer.at(m_lCurrentPos); }
  void SetCurrentPosData(char cValue) { m_pDataBuffer.at(m_lCurrentPos) = cValue; }

  // �����ú���
  void __TEST_SetBuffer__(CString strBuffer);

protected:
  INT64 m_lTime; // �����ݵ���ȡʱ�䡣��ʽΪyyyymmddhhmmss
  vector<char> m_pDataBuffer;
  long m_lBufferLength;
  long m_lCurrentPos;
};

typedef shared_ptr<CWebData> CWebDataPtr;
