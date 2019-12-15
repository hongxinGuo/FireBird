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

  // �����ӿں���
  virtual bool GetWebData(void);

  virtual bool ReportStatus(long lNumberOfData);

  // ����Ϊ�̳������ʵ�ֵļ������ܺ�������ɾ�������
  virtual void InquireNextWebData(void) {}        // ������һ����������
  virtual int  GetInquiringStr(CString& strInquire, long lTotalNumber = 900, bool fSkipUnactiveStock = true) { return 0; }// ������һ����ѯ���ַ���
  virtual void StartReadingThread(void) {} // ���������ȡ�̡߳�

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

  char* m_pCurrentPos; // ��ǰ�����λ��
  long m_lCurrentPos;

  CString m_strWebDataInquirePrefix; // ��ѯ�ַ���ǰ׺
  CString m_strWebDataInquireSuffix; // ��ѯ�ַ�����׺

  atomic_bool m_fReadingWebData; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ

  bool m_fReportStatus; //
};

#endif
