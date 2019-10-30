#pragma once

#include"stdafx.h"
#include"globedef.h"

#include"CriticalSectionBool.h"

class CWebData {
public:
  CWebData() noexcept;
  ~CWebData() {}

  // 公共接口函数
  bool GetWebData(void);

  virtual bool IsNeedProcessingCurrentWebData(void) { return true; }
  virtual void ProcessCurrentWebData(void); // 默认处理当前网络数据函数
  virtual bool SucceedReadingAndStoringWebData(void); // 默认读取存储函数

  // 下列为继承类必须实现的几个功能函数，完成具体任务。
  virtual bool SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount) = 0; // 读取并存储一个数据进数据队列
  virtual void ProcessWebDataStored(void) = 0; // 处理存储后的数据队列
  virtual void ReportDataError(void) = 0;       // 读取数据中报错
  virtual void ReportCommunicationError(void) = 0;  // 网络出现问题时报错
  virtual void InquireNextWebData(void) = 0;        // 申请下一个网络数据
  virtual int  GetInquiringStr(CString& strInquire) = 0;// 申请下一个查询用字符串
  virtual void StartReadingThread(void) = 0;    // 调用网络读取线程。

  void CreateTotalInquiringString(CString strMIddle);
  CString GetInquiringString(void) { return m_strInquire; }
  void SetInquiringString(CString str) { m_strInquire = str; }
  char* GetBufferAddr(void) { return m_buffer; }
  void AppendInquiringString(CString str) { m_strInquire += str; }
  long GetByteReaded(void) { return m_lByteRead; }
  void SetByteReaded(long lValue) { m_lByteRead = lValue; }
  void AddByteReaded(long lValue) { m_lByteRead += lValue; }

  CString GetInquiringStringPrefix(void) { return m_strWebDataInquirePrefix; }
  CString GetInquiringStringSuffix(void) { return m_strWebDataInquireSuffix; }

  bool IsReadingSucceed(void) { if (m_fSucceed) return true; else return false; }
  void SetReadingSucceed(bool fFlag) { m_fSucceed = fFlag; }
  bool IsWebDataReceived(void) { return m_WebDataReceived.IsTrue(); }
  void SetWebDataReceived(bool fFlag) { m_WebDataReceived.SetFlag(fFlag); }
  bool IsReadingWebData(void) { return m_ReadingWebData.IsTrue(); }
  void SetReadingWebData(bool fFlag) { m_ReadingWebData.SetFlag(fFlag); }

public:

protected:
  CString m_strInquire;// 查询所需的字符串
  char m_buffer[2048 * 1024]; // 接收到数据的缓冲区
  long m_lByteRead; // 接收到的字符数
  bool m_fSucceed; // 网络是否异常

  CString m_strWebDataInquirePrefix; // 查询字符串前缀
  CString m_strWebDataInquireSuffix; // 查询字符串后缀

  CCriticalSectionBool m_ReadingWebData; // 接收实时数据线程是否执行标识
  CCriticalSectionBool m_WebDataReceived;// 实时数据已接收完毕标识
};
