////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读取网络数据的基类。
// 由于网络数据使用特定的字符集（一般皆为多字节字符集），故而此基类也应使用固定的字符集，以便于采集数据。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"stdafx.h"
#include"afxinet.h"

#include"WebData.h"

using namespace std;
#include<atomic>

class CVirtualWebInquiry : public CObject {
public:
  CVirtualWebInquiry();
  virtual ~CVirtualWebInquiry(void);

  virtual bool ReadWebData(long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime = 0); // 唯一的网络实际读取函数
  bool ReadDataFromWebOnce(long& lCurrentByteReaded);
  virtual UINT ReadWebFile(void); // 无法测试，故而虚拟化后使用Mock类。
  CWebDataPtr TransferWebDataToQueueData(void);

  // 唯一的公共接口函数
  virtual bool GetWebData(void);

  void Reset(void) noexcept;
  virtual bool ReportStatus(long lNumberOfData);

  // 下列为继承类必须实现的几个功能函数，完成具体任务。不允许调用本基类函数
  virtual bool PrepareNextInquiringStr(void) { ASSERT(0); return true; }
  virtual CString GetNextInquiringMiddleStr(long, bool) { ASSERT(0); return _T(""); }// 申请下一个查询用字符串
  virtual void StartReadingThread(void) { ASSERT(0); } // 调用网络读取线程。

  // 以下为实现函数
  void CreateTotalInquiringString(CString strMIddle);
  CString GetInquiringString(void) { return m_strInquire; }
  void SetInquiringString(CString str) { m_strInquire = str; }
  void AppendInquiringString(CString str) { m_strInquire += str; }

  char GetData(long lIndex) { return m_buffer.at(lIndex); }
  void SetData(long lIndex, char value) { m_buffer.at(lIndex) = value; }

  long GetByteReaded(void)noexcept { return m_lByteRead; }
  void SetByteReaded(long lValue)noexcept { m_lByteRead = lValue; }
  void AddByteReaded(long lValue)noexcept { m_lByteRead += lValue; }

  CString GetInquiringStringPrefix(void) { return m_strWebDataInquirePrefix; }
  void SetInquiryingStrPrefix(CString strPrefix) { m_strWebDataInquirePrefix = strPrefix; }
  CString GetInquiringStringSuffix(void) { return m_strWebDataInquireSuffix; }
  void SetInquiryingStringSuffix(CString strSuffix) { m_strWebDataInquireSuffix = strSuffix; }

  bool IsReadingWebData(void) noexcept { return m_fReadingWebData; }
  void SetReadingWebData(bool fFlag) noexcept { m_fReadingWebData = fFlag; }

  bool IsWebError(void) noexcept { return m_fWebError; }
  void SetWebError(bool fFlag) noexcept { m_fWebError = fFlag; }

  bool IsReportStatus(void) noexcept { return m_fReportStatus; }

  CString GetConnection(void) { return m_strConnection; }

  long GetInquiringNumber(void) noexcept { return m_lInquiringNumber; }
  void SetInquiringNumber(long lValue) noexcept { m_lInquiringNumber = lValue; }

  long GetReadingThreadNumber(void) noexcept { return m_lReadingThreadNumber; }
  void SetReadingthreadNumber(long lValue) noexcept { m_lReadingThreadNumber = lValue; }
  bool IsReadingWebThreadRunning(void) noexcept { if (m_lReadingThreadNumber > 0) return true; else return false; }

  long GetTotalByteReaded(void) noexcept { return m_lTotalByteReaded; }
  void SetTotalByteReaded(long lValue = 0) noexcept { m_lTotalByteReaded = lValue; }
  void ClearTotalByteReaded(void) noexcept { m_lTotalByteReaded = 0; }

public:
  // 以下为测试用函数
  void __TESTSetBuffer(char* buffer, long lTotalNumber);
  void __TESTSetBuffer(CString str);

protected:
  CHttpFile* m_pFile; // 网络文件指针
  DWORD m_dwWebErrorCode; //网络读取错误代码
  CString m_strInquire;// 查询所需的字符串
  vector<char> m_buffer; // 接收到数据的缓冲区
  long m_lByteRead; // 接收到的字符数

  CString m_strWebDataInquireMiddle; // 查询字符串中间字段
  CString m_strWebDataInquirePrefix; // 查询字符串前缀
  CString m_strWebDataInquireSuffix; // 查询字符串后缀

  atomic_bool m_fReadingWebData; // 接收实时数据线程是否执行标识
  atomic_bool m_fWebError; // 读取网络数据时是否出现错误标识

  bool m_fReportStatus; //

  long m_lInquiringNumber; // 每次查询数量

  static atomic_long m_lReadingThreadNumber; // 当前执行网络读取线程数。所有的网络读取器都使用同一个变量，故而声明为静态。
  static atomic_long m_lTotalByteReaded; // 当前网络读取字节数。所有的网络读取器都修改此变量，故而声明为静态。

  CString m_strConnection;
};

typedef shared_ptr<CVirtualWebInquiry> CVirtualWebInquiryPtr;