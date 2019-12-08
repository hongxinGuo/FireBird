#pragma once

using namespace std;
#include<memory>

class CRTWebData
{
public:
  CRTWebData() {
    m_pDataBuffer = nullptr;
    m_lBufferLength = 0;
  }
  ~CRTWebData() {
    if (m_pDataBuffer != nullptr) delete m_pDataBuffer;
  }

public:
  char* m_pDataBuffer;
  long m_lBufferLength;
};

typedef shared_ptr<CRTWebData> CRTWebDataPtr;
