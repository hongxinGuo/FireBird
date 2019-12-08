#pragma once

using namespace std;
#include<memory>

class CWebRTData
{
public:
  CWebRTData() {
    m_pDataBuffer = nullptr;
    m_lBufferLength = 0;
    m_pCurrentPos = nullptr;
    m_lCurrentPos = 0;
  }
  ~CWebRTData() {
    if (m_pDataBuffer != nullptr) delete m_pDataBuffer;
  }

  void IncreaseCurrentPos(long lNumberOfChars = 1) noexcept { m_pCurrentPos += lNumberOfChars; m_lCurrentPos += lNumberOfChars; }

public:
  char* m_pDataBuffer;
  long m_lBufferLength;
  char* m_pCurrentPos; // 当前处理的位置
  long m_lCurrentPos;
};

typedef shared_ptr<CWebRTData> CWebRTDataPtr;
