#pragma once

#include"stdafx.h"
#include"afxinet.h"

#include"WebData.h"

using namespace std;
#include<atomic>

class CVirtualWebInquiry : public CObject {
public:
  CVirtualWebInquiry();
  virtual ~CVirtualWebInquiry() {}

  virtual bool ReadWebData(long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime = 0); // 唯一的网络实际读取函数
  bool ReadDataFromWebOnce(void);
  virtual UINT ReadWebFile(void); // 无法测试，故而虚拟化后使用Mock类。
  CWebDataPtr TransferWebDataToQueueData(void);

  // 唯一的公共接口函数
  virtual bool GetWebData(void);

  virtual bool ReportStatus(long lNumberOfData);
  // 下列为继承类必须实现的几个功能函数，完成具体任务。
  virtual bool PrepareNextInquiringStr(void) { ASSERT(0); return true; }
  virtual CString GetNextInquiringMiddleStr(long, bool) { ASSERT(0); return _T(""); }// 申请下一个查询用字符串
  virtual void StartReadingThread(void) { ASSERT(0); } // 调用网络读取线程。

  // 以下为实现函数
  void CreateTotalInquiringString(CString strMIddle);
  CString GetInquiringString(void) { return m_strInquire; }
  void SetInquiringString(CString str) noexcept { m_strInquire = str; }
  void AppendInquiringString(CString str) { m_strInquire += str; }
  char* GetBufferAddr(void) noexcept { return m_buffer; }
  long GetByteReaded(void)noexcept { return m_lByteRead; }
  void SetByteReaded(long lValue)noexcept { m_lByteRead = lValue; }
  void AddByteReaded(long lValue)noexcept { m_lByteRead += lValue; }

  CString GetInquiringStringPrefix(void) { return m_strWebDataInquirePrefix; }
  CString GetInquiringStringSuffix(void) { return m_strWebDataInquireSuffix; }

  bool IsReadingWebData(void) noexcept { return m_fReadingWebData; }
  void SetReadingWebData(bool fFlag) noexcept { m_fReadingWebData = fFlag; }

  bool IsWebError(void) noexcept { return m_fWebError; }
  void SetWebError(bool fFlag) noexcept { m_fWebError = fFlag; }

  void IncreaseCurrentPos(long lNumberOfChars = 1) noexcept { m_pCurrentPos += lNumberOfChars; m_lCurrentPos += lNumberOfChars; }
  void ResetCurrentPos(void) noexcept { m_pCurrentPos = m_buffer; m_lCurrentPos = 0; }

  long GetCurrentPos(void) noexcept { return m_lCurrentPos; }
  char* GetCurrentPosPtr(void) noexcept { return m_pCurrentPos; }

  char* GetCurrentReadPos(void) noexcept { return m_pCurrentReadPos; }

  bool IsReportStatus(void) noexcept { return m_fReportStatus; }

  CString GetConnection(void) noexcept { return m_strConnection; }

  long GetInquiringNumber(void) noexcept { return m_lInquiringNumber; }
  void SetInquiringNumber(long lValue) noexcept { m_lInquiringNumber = lValue; }

  long GetReadingThreadNumber(void) noexcept { return m_lReadingThreadNumber; }
  void SetReadingthreadNumber(long lValue) noexcept { m_lReadingThreadNumber = lValue; }
  bool IsReadingWebThreadRunning(void) noexcept { if (m_lReadingThreadNumber > 0) return true; else return false; }

public:
  // 以下为测试用函数
  void __TESTSetBuffer(char* buffer, long lTotalNumber);
  void __TESTSetBuffer(CString str);

protected:
  CHttpFile* m_pFile; // 网络文件指针
  DWORD m_dwWebErrorCode; //网络读取错误代码
  CString m_strInquire;// 查询所需的字符串
  char m_buffer[2048 * 1024]; // 接收到数据的缓冲区
  long m_lByteRead; // 接收到的字符数
  char* m_pCurrentReadPos; // 当前读入字符的存入位置
  long m_lCurrentByteRead; // 本次接收到到的字符数

  char* m_pCurrentPos; // 当前处理的位置
  long m_lCurrentPos;// 当前处理的位置

  CString m_strWebDataInquireMiddle; // 查询字符串中间字段
  CString m_strWebDataInquirePrefix; // 查询字符串前缀
  CString m_strWebDataInquireSuffix; // 查询字符串后缀

  atomic_bool m_fReadingWebData; // 接收实时数据线程是否执行标识
  atomic_bool m_fWebError; // 读取网络数据时是否出现错误标识

  bool m_fReportStatus; //

  long m_lInquiringNumber; // 每次查询数量

  static atomic_long m_lReadingThreadNumber; // 当前执行网络读取线程数

  CString m_strConnection;
};

typedef shared_ptr<CVirtualWebInquiry> CVirtualWebInquiryPtr;