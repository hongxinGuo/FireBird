#pragma once

#include"stdafx.h"
#include"globedef.h"

using namespace std;
#include<atomic>

class CWebData {
public:
  CWebData();
  ~CWebData() {}

  // �����ӿں���
  virtual bool GetWebData(void);

  virtual bool IsReadingFinished(void); // ���������е�����
  virtual bool ReportStatus(long lNumberOfData);

  // ����Ϊ�̳������ʵ�ֵļ������ܺ�������ɾ�������
  virtual void InquireNextWebData(void) = 0;        // ������һ����������
  virtual int  GetInquiringStr(CString& strInquire, long lTotalNumber = 900, bool fSkipUnactiveStock = true) = 0;// ������һ����ѯ���ַ���
  virtual void StartReadingThread(void) = 0;    // ���������ȡ�̡߳�

  // ����Ϊʵ�ֺ���
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

  bool IsReadingSucceed(void) noexcept { return m_fReceivedData; }
  void SetReadingSucceed(bool fFlag) noexcept { m_fReceivedData = fFlag; }
  bool IsWebDataReceived(void) noexcept { return m_fWebDataReceived; }
  void SetWebDataReceived(bool fFlag) noexcept { m_fWebDataReceived = fFlag; }
  bool IsReadingWebData(void) noexcept { return m_fReadingWebData; }
  void SetReadingWebData(bool fFlag) noexcept { m_fReadingWebData = fFlag; }

  void IncreaseCurrentPos(long lNumberOfChars = 1) noexcept { m_pCurrentPos += lNumberOfChars; m_lCurrentPos += lNumberOfChars; }
  void ResetCurrentPos(void) noexcept { m_pCurrentPos = m_buffer; m_lCurrentPos = 0; }

  long GetCurrentPos(void) noexcept { return m_lCurrentPos; }
  char* GetCurrentPosPtr(void) noexcept { return m_pCurrentPos; }

  bool IsReportStatus(void) noexcept { return m_fReportStatus; }

public:
  // ����Ϊ�����ú���
  void __TESTSetBuffer(char* buffer, long lTotalNumber);

protected:
  CString m_strInquire;// ��ѯ������ַ���
  char m_buffer[2048 * 1024]; // ���յ����ݵĻ�����
  long m_lByteRead; // ���յ����ַ���
  bool m_fReceivedData; // �����Ƿ��쳣

  char* m_pCurrentPos; // ��ǰ�����λ��
  long m_lCurrentPos;

  CString m_strWebDataInquirePrefix; // ��ѯ�ַ���ǰ׺
  CString m_strWebDataInquireSuffix; // ��ѯ�ַ�����׺

  atomic_bool m_fReadingWebData; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
  atomic_bool m_fWebDataReceived;// ʵʱ�����ѽ�����ϱ�ʶ

  bool m_fReportStatus; //
};
