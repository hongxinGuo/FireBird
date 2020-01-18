#pragma once

#include"stdafx.h"

using namespace std;
#include<memory>

class CWebDataReceived : public CObject {
public:
  CWebDataReceived() {
    m_lTime = 0;
    m_pDataBuffer = nullptr;
    m_lBufferLength = 0;
    m_pCurrentPos = nullptr;
    m_lCurrentPos = 0;
  }
  ~CWebDataReceived() {
    if (m_pDataBuffer != nullptr) delete m_pDataBuffer;
  }

  long GetCurrentPos(void) noexcept { return m_lCurrentPos; }
  void SetCurrentPos(long lValue) noexcept { m_lCurrentPos = lValue; m_pCurrentPos = m_pDataBuffer + lValue; }
  char* GetBufferAddr(void) noexcept { return m_pDataBuffer; }
  void IncreaseCurrentPos(long lNumberOfChars = 1) noexcept { m_pCurrentPos += lNumberOfChars; m_lCurrentPos += lNumberOfChars; }
  void ResetCurrentPos(void) noexcept { m_pCurrentPos = m_pDataBuffer; m_lCurrentPos = 0; }
  long GetBufferLength(void) noexcept { return m_lBufferLength; }
  void SetBufferLength(long lValue) noexcept { m_lBufferLength = lValue; }

public:
  INT64 m_lTime; // 此数据的提取时间。格式为yyyymmddhhmmss
  char* m_pDataBuffer;
  long m_lBufferLength;
  char* m_pCurrentPos; // 当前处理的位置
  long m_lCurrentPos;
};

typedef shared_ptr<CWebDataReceived> CWebDataReceivedPtr;
