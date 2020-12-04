////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ȡ�������ݵĻ��ࡣ
// ������������ʹ���ض����ַ�����һ���Ϊ���ֽ��ַ��������ʶ��˻���ҲӦʹ�ù̶����ַ������Ա��ڲɼ����ݡ�
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

  virtual bool ReadWebData(long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime = 0); // Ψһ������ʵ�ʶ�ȡ����
  bool ReadDataFromWebOnce(long& lCurrentByteReaded);
  virtual UINT ReadWebFile(void); // �޷����ԣ��ʶ����⻯��ʹ��Mock�ࡣ
  CWebDataPtr TransferWebDataToQueueData(void);

  // Ψһ�Ĺ����ӿں���
  virtual bool GetWebData(void);

  void Reset(void) noexcept;
  virtual bool ReportStatus(long lNumberOfData);

  // ����Ϊ�̳������ʵ�ֵļ������ܺ�������ɾ������񡣲�������ñ����ຯ��
  virtual bool PrepareNextInquiringStr(void) { ASSERT(0); return true; }
  virtual CString GetNextInquiringMiddleStr(long, bool) { ASSERT(0); return _T(""); }// ������һ����ѯ���ַ���
  virtual void StartReadingThread(void) { ASSERT(0); } // ���������ȡ�̡߳�

  // ����Ϊʵ�ֺ���
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
  // ����Ϊ�����ú���
  void __TESTSetBuffer(char* buffer, long lTotalNumber);
  void __TESTSetBuffer(CString str);

protected:
  CHttpFile* m_pFile; // �����ļ�ָ��
  DWORD m_dwWebErrorCode; //�����ȡ�������
  CString m_strInquire;// ��ѯ������ַ���
  vector<char> m_buffer; // ���յ����ݵĻ�����
  long m_lByteRead; // ���յ����ַ���

  CString m_strWebDataInquireMiddle; // ��ѯ�ַ����м��ֶ�
  CString m_strWebDataInquirePrefix; // ��ѯ�ַ���ǰ׺
  CString m_strWebDataInquireSuffix; // ��ѯ�ַ�����׺

  atomic_bool m_fReadingWebData; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
  atomic_bool m_fWebError; // ��ȡ��������ʱ�Ƿ���ִ����ʶ

  bool m_fReportStatus; //

  long m_lInquiringNumber; // ÿ�β�ѯ����

  static atomic_long m_lReadingThreadNumber; // ��ǰִ�������ȡ�߳��������е������ȡ����ʹ��ͬһ���������ʶ�����Ϊ��̬��
  static atomic_long m_lTotalByteReaded; // ��ǰ�����ȡ�ֽ��������е������ȡ�����޸Ĵ˱������ʶ�����Ϊ��̬��

  CString m_strConnection;
};

typedef shared_ptr<CVirtualWebInquiry> CVirtualWebInquiryPtr;