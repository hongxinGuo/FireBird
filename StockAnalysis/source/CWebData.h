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
  CString m_strInquire;         // ��ѯ������ַ���
  char* m_buffer;               // ���յ����ݵĻ�����
  long m_lByteRead;             // ���յ����ַ���
  bool m_fError;                // �����Ƿ��쳣

  CString m_strSinaRTDataInquire; // ��ѯ�ַ���ǰ׺

  bool m_fReadingWebData;     // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
  CCriticalSection m_ReadingWebDataLock;  //�Ƿ����ʵʱ���ݵ�ͬ����
  bool m_fWebDataReceived; // ʵʱ�����ѽ�����ϱ�ʶ
  CCriticalSection m_WebDataReceivedLock;  // ʵʱ���ݽ�����ϱ�ʶ��ͬ����
};
