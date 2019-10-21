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

  // �����ӿں���
  bool GetWebData(void);

  virtual void ProcessCurrentWebData(void);
  virtual bool SucceedReadingAndStoringWebData(void);

  // ����Ϊ�̳������ʵ�ֵļ������ܺ�������ɾ�������
  virtual bool SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount) = 0; // ��ȡ���洢һ�����ݽ����ݶ���
  virtual void ProcessWebDataStored(void) = 0; // ����洢������ݶ���
  virtual void ReportDataError(void) = 0;       // ��ȡ�����б���
  virtual void ReportCommunicationError(void) = 0;  // �����������ʱ����
  virtual void InquireNextWebData(void) = 0;        // ������һ����������
  virtual int  GetInquiringStockStr(CString& strInquire) = 0;// ������һ����ѯ���ַ���
  virtual void StartReadingThread(void) = 0;    // ���������ȡ�̡߳�

  bool IsReadingSucceed(void) { if (m_fError) return false; else return true; }
  bool IsWebDataReceived(void);
  void SetWebDataReceived(bool fFlag);
  bool IsReadingWebData(void);
  void SetReadingWebData(bool fFlag);

public:
  CString m_strInquire;         // ��ѯ������ַ���
  char m_buffer[2048 * 1024];   // ���յ����ݵĻ�����
  long m_lByteRead;             // ���յ����ַ���
  bool m_fError;                // �����Ƿ��쳣

  CString m_strWebDataInquirePrefix; // ��ѯ�ַ���ǰ׺
  CString m_strWebDataInquireSuffix; // ��ѯ�ַ�����׺

  bool m_fReadingWebData;     // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
  CCriticalSection m_ReadingWebDataLock;  //�Ƿ����ʵʱ���ݵ�ͬ����
  bool m_fWebDataReceived; // ʵʱ�����ѽ�����ϱ�ʶ
  CCriticalSection m_WebDataReceivedLock;  // ʵʱ���ݽ�����ϱ�ʶ��ͬ����
};
