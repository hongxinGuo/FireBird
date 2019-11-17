#pragma once

#include"stdafx.h"
#include"globedef.h"

#include"CriticalSectionBool.h"

class CWebData {
public:
  CWebData() noexcept;
  ~CWebData() {}

  // �����ӿں���
  bool GetWebData(void);

  virtual bool IsNeedProcessingCurrentWebData(void) { return m_fSucceed; }

  virtual void ProcessCurrentWebData(void); // Ĭ�ϴ���ǰ�������ݺ���
  virtual bool SucceedReadingAndStoringWebData(void); // Ĭ�϶�ȡ�洢����
  virtual bool ReadPrefix(void); // ����ǰ׺
  virtual bool IsReadingFinished(void); // ���������е�����
  virtual bool ReportStatus(long lNumberOfData);

  // ����Ϊ�̳������ʵ�ֵļ������ܺ�������ɾ�������
  virtual bool SucceedReadingAndStoringOneWebData(void) = 0; // ��ȡ���洢һ�����ݽ����ݶ���
  virtual void ProcessWebDataStored(void) = 0; // ����洢������ݶ���
  virtual void ReportDataError(void) = 0;       // ��ȡ�����б���
  virtual void ReportCommunicationError(void) = 0;  // �����������ʱ����
  virtual void InquireNextWebData(void) = 0;        // ������һ����������
  virtual int  GetInquiringStr(CString& strInquire, long lTotalNumber = 900, bool fSkipUnactiveStock = true) = 0;// ������һ����ѯ���ַ���
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

  bool IsReadingSucceed(void) { return m_fSucceed; }
  void SetReadingSucceed(bool fFlag) { m_fSucceed = fFlag; }
  bool IsWebDataReceived(void) { return m_WebDataReceived.IsTrue(); }
  void SetWebDataReceived(bool fFlag) { m_WebDataReceived.SetFlag(fFlag); }
  bool IsReadingWebData(void) { return m_ReadingWebData.IsTrue(); }
  void SetReadingWebData(bool fFlag) { m_ReadingWebData.SetFlag(fFlag); }

  void IncreaseCurrentPos(long lNumberOfChars = 1) { m_pCurrentPos += lNumberOfChars; m_lCurrentPos += lNumberOfChars; }
  void ResetCurrentPos(void) { m_pCurrentPos = m_buffer; m_lCurrentPos = 0; }

  long GetCurrentPos(void) { return m_lCurrentPos; }
  char* GetCurrentPosPtr(void) { return m_pCurrentPos; }

public:
  // ����Ϊ�����ú���
  void TESTSetBuffer(char* buffer, long lTotalNumber);

protected:
  CString m_strInquire;// ��ѯ������ַ���
  char m_buffer[2048 * 1024]; // ���յ����ݵĻ�����
  long m_lByteRead; // ���յ����ַ���
  bool m_fSucceed; // �����Ƿ��쳣

  char* m_pCurrentPos; // ��ǰ�����λ��
  long m_lCurrentPos;

  CString m_strWebDataInquirePrefix; // ��ѯ�ַ���ǰ׺
  CString m_strWebDataInquireSuffix; // ��ѯ�ַ�����׺

  CCriticalSectionBool m_ReadingWebData; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
  CCriticalSectionBool m_WebDataReceived;// ʵʱ�����ѽ�����ϱ�ʶ

  bool m_fReportStatus; //
};
