#pragma once

using namespace std;

class CRTWebData
{
public:
  CRTWebData() {
    m_pDataBuffer = nullptr;
    m_lBufferLength = 0;
  }
  ~CRTWebData() {}

public:
  char* m_pDataBuffer;
  long m_lBufferLength;
};

typedef shared_ptr<CRTWebData> CRTWebDataPtr;
