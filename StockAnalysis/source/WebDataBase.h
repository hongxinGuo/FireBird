#pragma once

#include"stdafx.h"
#include"globedef.h"

class CWebDataBase {
public:
  CWebDataBase() noexcept;
  ~CWebDataBase() {}

  // �����ӿں���
  bool GetWebData(void);

  virtual bool IsNeedProcessingCurrentWebData(void) { return true; }
  virtual void ProcessCurrentWebData(void); // Ĭ�ϴ�����ǰ�������ݺ���
  virtual bool SucceedReadingAndStoringWebData(void); // Ĭ�϶�ȡ�洢����

  // ����Ϊ�̳������ʵ�ֵļ������ܺ�������ɾ�������
  virtual bool SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount) = 0; // ��ȡ���洢һ�����ݽ����ݶ���
  virtual void ProcessWebDataStored(void) = 0; // �����洢������ݶ���
  virtual void ReportDataError(void) = 0;       // ��ȡ�����б���
  virtual void ReportCommunicationError(void) = 0;  // �����������ʱ����
  virtual void InquireNextWebData(void) = 0;        // ������һ����������
  virtual int  GetInquiringStockStr(CString& strInquire) = 0;// ������һ����ѯ���ַ���
  virtual void StartReadingThread(void) = 0;    // ���������ȡ�̡߳�

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
  bool IsWebDataReceived(void);
  void SetWebDataReceived(bool fFlag);
  bool IsReadingWebData(void);
  void SetReadingWebData(bool fFlag);

public:

protected:
  CString m_strInquire;         // ��ѯ������ַ���
  char m_buffer[2048 * 1024];   // ���յ����ݵĻ�����
  long m_lByteRead;             // ���յ����ַ���
  bool m_fSucceed;                // �����Ƿ��쳣

  CString m_strWebDataInquirePrefix; // ��ѯ�ַ���ǰ׺
  CString m_strWebDataInquireSuffix; // ��ѯ�ַ�����׺

  bool m_fReadingWebData;     // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
  CCriticalSection m_ReadingWebDataLock;  //�Ƿ����ʵʱ���ݵ�ͬ����
  bool m_fWebDataReceived; // ʵʱ�����ѽ�����ϱ�ʶ
  CCriticalSection m_WebDataReceivedLock;  // ʵʱ���ݽ�����ϱ�ʶ��ͬ����
};