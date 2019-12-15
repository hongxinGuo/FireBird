#ifndef __WEB_DATA_H__
#define __WEB_DATA_H__

#include"stdafx.h"
#include"globedef.h"

using namespace std;
#include<atomic>

class CWebData {
public:
  CWebData();
  ~CWebData() {}

  // 公共接口函数
  virtual bool GetWebData(void);

  virtual bool ReportStatus(long lNumberOfData);

  // 下列为继承类必须实现的几个功能函数，完成具体任务。
  virtual void InquireNextWebData(void) {}        // 申请下一个网络数据
  virtual int  GetInquiringStr(CString& strInquire, long lTotalNumber = 900, bool fSkipUnactiveStock = true) { return 0; }// 申请下一个查询用字符串
  virtual void StartReadingThread(void) {} // 调用网络读取线程。

  // 以下为实现函数
  void CreateTotalInquiringString(CString strMIddle);
  CString GetInquiringString(void) { return m_strInquire; }
  void SetInquiringString(CString str) noexcept { m_strInquire = str; }
  char* GetBufferAddr(void) noexcept { return m_buffer; }
  void AppendInquiringString(CString str) { m_strInquire += str; }
  long GetByteReaded(void)noexcept { return m_lByteRead; }
  void SetByteReaded(long lValue)noexcept { m_lByteRead = lValue; }
  void AddByteReaded(long lValue)noexcept { m_lByteRead += lValue; }

  CString GetInquiringStringPrefix(void) { return m_strWebDataInquirePrefix; }
  CString GetInquiringStringSuffix(void) { return m_strWebDataInquireSuffix; }

  bool IsReadingWebData(void) noexcept { return m_fReadingWebData; }
  void SetReadingWebData(bool fFlag) noexcept { m_fReadingWebData = fFlag; }

  void IncreaseCurrentPos(long lNumberOfChars = 1) noexcept { m_pCurrentPos += lNumberOfChars; m_lCurrentPos += lNumberOfChars; }
  void ResetCurrentPos(void) noexcept { m_pCurrentPos = m_buffer; m_lCurrentPos = 0; }

  long GetCurrentPos(void) noexcept { return m_lCurrentPos; }
  char* GetCurrentPosPtr(void) noexcept { return m_pCurrentPos; }

  bool IsReportStatus(void) noexcept { return m_fReportStatus; }

public:
  // 以下为测试用函数
  void __TESTSetBuffer(char* buffer, long lTotalNumber);

protected:
  CString m_strInquire;// 查询所需的字符串
  char m_buffer[2048 * 1024]; // 接收到数据的缓冲区
  long m_lByteRead; // 接收到的字符数

  char* m_pCurrentPos; // 当前处理的位置
  long m_lCurrentPos;

  CString m_strWebDataInquirePrefix; // 查询字符串前缀
  CString m_strWebDataInquireSuffix; // 查询字符串后缀

  atomic_bool m_fReadingWebData; // 接收实时数据线程是否执行标识

  bool m_fReportStatus; //
};

#endif
