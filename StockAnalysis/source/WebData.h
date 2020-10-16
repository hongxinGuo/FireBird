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
    m_pDataBuffer = nullptr;
    m_lBufferLength = 0;
    m_pCurrentPos = nullptr;
    m_lCurrentPos = 0;
  }
  ~CWebData() {
    if (m_pDataBuffer != nullptr) delete m_pDataBuffer;
  }

  bool IsProcessedAllTheData(void) noexcept { if (m_lCurrentPos < m_lBufferLength) return false; else return true; }

  void IncreaseCurrentPos(long lNumberOfChars = 1) noexcept { m_pCurrentPos += lNumberOfChars; m_lCurrentPos += lNumberOfChars; }
  void ResetCurrentPos(void) noexcept { m_pCurrentPos = m_pDataBuffer; m_lCurrentPos = 0; }

  INT64 GetTime(void) noexcept { return m_lTime; }
  void SetTime(INT64 lTime) noexcept { m_lTime = lTime; }
  long GetBufferLength(void) noexcept { return m_lBufferLength; }
  void SetBufferLength(long lValue) noexcept { m_lBufferLength = lValue; }
  long GetCurrentPos(void) noexcept { return m_lCurrentPos; }
  void SetCurrentPos(long lValue) noexcept { m_lCurrentPos = lValue; }

  bool CopyData(char* buffer, long lDataLength, long lStartPosition);
  bool CopyData(char* buffer, long lDataLength); // Ĭ�ϴ�m_lCurrentPos��ʼ����

  // �����ú���
  void __TEST_SetBuffer__(CString strBuffer);
  char* __TEST_GetBufferAddr(void) noexcept { return m_pDataBuffer; }

public:
  char* m_pDataBuffer;
  char* m_pCurrentPos; // ��ǰ�����λ��

protected:
  INT64 m_lTime; // �����ݵ���ȡʱ�䡣��ʽΪyyyymmddhhmmss
  long m_lBufferLength;
  long m_lCurrentPos;

  // ����vectorʵ������֮����
  vector<char> m_vDataBuffer;
};

typedef shared_ptr<CWebData> CWebDataPtr;
