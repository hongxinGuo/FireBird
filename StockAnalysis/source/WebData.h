#pragma once

#include"stdafx.h"
#include"globedef.h"

using namespace std;
#include<atomic>

class CWebData {
public:
  CWebData() noexcept;
  ~CWebData() {}

  // 公共接口函数
  virtual bool GetWebData(void);

  virtual bool IsNeedProcessingCurrentWebData(void) { return IsReadingSucceed(); }

  virtual void ProcessCurrentWebData(void); // 默认处理当前网络数据函数
  virtual bool SucceedReadingAndStoringWebData(void); // 默认读取存储函数
  virtual bool ReadPrefix(void); // 读入前缀
  virtual bool IsReadingFinished(void); // 读完了所有的数据
  virtual bool ReportStatus(long lNumberOfData);

  // 下列为继承类必须实现的几个功能函数，完成具体任务。
  virtual bool SucceedReadingAndStoringOneWebData(void) = 0; // 读取并存储一个数据进数据队列
  virtual void ProcessWebDataStored(void) = 0; // 处理存储后的数据队列
  virtual void ReportDataError(void) = 0;       // 读取数据中报错
  virtual void ReportCommunicationError(void) = 0;  // 网络出现问题时报错
  virtual void InquireNextWebData(void) = 0;        // 申请下一个网络数据
  virtual int  GetInquiringStr(CString& strInquire, long lTotalNumber = 900, bool fSkipUnactiveStock = true) = 0;// 申请下一个查询用字符串
  virtual void StartReadingThread(void) = 0;    // 调用网络读取线程。

  // 以下为实现函数
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

  bool IsReadingSucceed(void) { return m_fReceivedData; }
  void SetReadingSucceed(bool fFlag) { m_fReceivedData = fFlag; }
  bool IsWebDataReceived(void) { return m_fWebDataReceived; }
  void SetWebDataReceived(bool fFlag) { m_fWebDataReceived = fFlag; }
  bool IsReadingWebData(void) { return m_fReadingWebData; }
  void SetReadingWebData(bool fFlag) { m_fReadingWebData = fFlag; }

  void IncreaseCurrentPos(long lNumberOfChars = 1) { m_pCurrentPos += lNumberOfChars; m_lCurrentPos += lNumberOfChars; }
  void ResetCurrentPos(void) { m_pCurrentPos = m_buffer; m_lCurrentPos = 0; }

  long GetCurrentPos(void) { return m_lCurrentPos; }
  char* GetCurrentPosPtr(void) { return m_pCurrentPos; }

  bool IsReportStatus(void) { return m_fReportStatus; }

public:
  // 以下为测试用函数
  void __TESTSetBuffer(char* buffer, long lTotalNumber);

protected:
  CString m_strInquire;// 查询所需的字符串
  char m_buffer[2048 * 1024]; // 接收到数据的缓冲区
  long m_lByteRead; // 接收到的字符数
  bool m_fReceivedData; // 网络是否异常

  char* m_pCurrentPos; // 当前处理的位置
  long m_lCurrentPos;

  CString m_strWebDataInquirePrefix; // 查询字符串前缀
  CString m_strWebDataInquireSuffix; // 查询字符串后缀

  atomic_bool m_fReadingWebData; // 接收实时数据线程是否执行标识
  atomic_bool m_fWebDataReceived;// 实时数据已接收完毕标识

  bool m_fReportStatus; //
};
