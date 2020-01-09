#ifndef __WEB_DATA_H__
#define __WEB_DATA_H__

#include"stdafx.h"
#include"globedef.h"

#include"WebDataReceived.h"

using namespace std;
#include<atomic>

class CWebData {
public:
  CWebData();
  ~CWebData() {}

  bool ReadWebData(long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime = 0);
  bool ReadDataFromWebOnce(void);
  CWebDataReceivedPtr TransferWebDataToQueueData(void);

  // Ψһ�Ĺ����ӿں���
  virtual bool GetWebData(void);

  virtual bool ReportStatus(long lNumberOfData);
  // ����Ϊ�̳������ʵ�ֵļ������ܺ�������ɾ�������
  virtual void InquireNextWebData(void) { ASSERT(0); }        // ������һ����������
  virtual CString GetNextInquiringStr(long, bool) { ASSERT(0); return _T(""); }// ������һ����ѯ���ַ���
  virtual void StartReadingThread(void) { ASSERT(0); } // ���������ȡ�̡߳�

  // ����Ϊʵ�ֺ���
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

  void IncreaseCurrentPos(long lNumberOfChars = 1) noexcept { m_pCurrentPos += lNumberOfChars; m_lCurrentPos += lNumberOfChars; }
  void ResetCurrentPos(void) noexcept { m_pCurrentPos = m_buffer; m_lCurrentPos = 0; }

  long GetCurrentPos(void) noexcept { return m_lCurrentPos; }
  char* GetCurrentPosPtr(void) noexcept { return m_pCurrentPos; }

  bool IsReportStatus(void) noexcept { return m_fReportStatus; }

public:
  // ����Ϊ�����ú���
  void __TESTSetBuffer(char* buffer, long lTotalNumber);
  void __TESTSetBuffer(CString str);

protected:
  CHttpFile* m_pFile; // �����ļ�ָ��
  CString m_strInquire;// ��ѯ������ַ���
  char m_buffer[2048 * 1024]; // ���յ����ݵĻ�����
  long m_lByteRead; // ���յ����ַ���
  char* m_pCurrentReadPos; // ��ǰ�����ַ��Ĵ���λ��
  long m_lCurrentByteRead; // ���ν��յ������ַ���

  char* m_pCurrentPos; // ��ǰ�����λ��
  long m_lCurrentPos;// ��ǰ�����λ��

  CString m_strWebDataInquireMiddle; // ��ѯ�ַ����м��ֶ�
  CString m_strWebDataInquirePrefix; // ��ѯ�ַ���ǰ׺
  CString m_strWebDataInquireSuffix; // ��ѯ�ַ�����׺

  atomic_bool m_fReadingWebData; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ

  bool m_fReportStatus; //
};

#endif
