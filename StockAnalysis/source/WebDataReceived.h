#ifndef __WEB_DATA_RECEIVED_H__
#define __WEB_DATA_RECEIVED_H__

using namespace std;
#include<memory>
#include<vector>

class CWebDataReceived
{
public:
  CWebDataReceived() {
    m_lBufferLength = 0;
    m_lCurrentPos = 0;
  }
  ~CWebDataReceived() {
  }

  long GetCurrentPos(void) noexcept { return m_lCurrentPos; }
  char GetChar(void) { return m_vBuffer.at(m_lCurrentPos); }
  char GetChar(long lCurrentPos) { return m_vBuffer.at(lCurrentPos); }
  void SetChar(char cChar) { m_vBuffer.at(m_lCurrentPos) = cChar; }
  void SetChar(long lIndex, char cChar) { m_vBuffer.at(lIndex) = cChar; }
  void IncreaseCurrentPos(long lNumberOfChars = 1) noexcept { m_lCurrentPos += lNumberOfChars; }
  void ResetCurrentPos(void) noexcept { m_lCurrentPos = 0; }
  bool Copy(char* buffer, long lLength) { for (int i = 0; i < lLength; i++) buffer[i] = m_vBuffer.at(m_lCurrentPos + i); return true; }
  void Assign(long lLength, char* buffer) { for (int i = 0; i < lLength; i++) m_vBuffer.at(m_lCurrentPos + i) = buffer[i]; }
public:
  vector<char> m_vBuffer;
  long m_lBufferLength;
  long m_lCurrentPos;
};

typedef shared_ptr<CWebDataReceived> CWebDataReceivedPtr;

#endif
