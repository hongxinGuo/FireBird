#pragma once

#include"stdafx.h"
#include"globedef.h"

class CWebData {
public:
  CWebData() noexcept {
    m_lByteRead = 0;
    m_fError = false;
    m_strInquire = "";
  }
  ~CWebData() {}

  // 公共接口函数
  bool GetWebData(void);

  virtual void ProcessWebData(void);
  virtual bool ReadAndSaveWebData(char*& pCurrentPos, long& iCount) const = 0;
  virtual void ReportDataError(void) const = 0;
  virtual void ReportCommunicationError(void) const = 0;
  virtual void InquireNextWebData(void) const = 0;
  virtual int  GetInquiringStockStr(CString& strInquire) const = 0;
  virtual void StartReadingThread(void) const = 0;
  virtual bool ProcessRTDataReveived(void) const = 0;

  bool IsReadingSucceed(void) { if (m_fError) return false; else return true; }
  bool IsWebDataReceived(void);
  void SetWebDataReceived(bool fFlag);
  bool IsReadingWebData(void);
  void SetReadingWebData(bool fFlag);

protected:
  CString m_strInquire;         // 查询所需的字符串
  char* m_buffer;               // 接收到数据的缓冲区
  long m_lByteRead;             // 接收到的字符数
  bool m_fError;                // 网络是否异常

  CString m_strSinaRTDataInquire; // 查询字符串前缀

  bool m_fReadingWebData;     // 接收实时数据线程是否执行标识
  CCriticalSection m_ReadingWebDataLock;  //是否接收实时数据的同步锁
  bool m_fWebDataReceived; // 实时数据已接收完毕标识
  CCriticalSection m_WebDataReceivedLock;  // 实时数据接收完毕标识的同步锁
};
