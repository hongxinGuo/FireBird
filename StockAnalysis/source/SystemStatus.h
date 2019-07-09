#pragma once

#include"afxmt.h"

class CSystemStatus {    // �������ݽṹ
public:
  CSystemStatus();

  void SetCalculateRSInProcess(bool fFlag);
  bool IsCalculateRSInProcess(void);
    
  void SetReadingInProcess(bool fFlag);
  bool IsReadingInProcess(void);

  void SetDataBaseInProcess(bool fFlag);
  bool IsDataBaseInProcess(void);

  void SetRTDataReadingInProcess(bool fFlag);
  bool IsRTDataReadingInProcess(void);

  void SetRTDataReady(bool fFlag);
  bool IsRTDataReady(void);

  void SetDayLineDataReady(bool fFlag);
  bool IsDayLineDataReady(void);

protected:
  bool m_fCalculatingRelativeStrongInProcess; // �Ƿ��ڼ������ǿ�ȵĹ����б�ʶ
  CCriticalSection m_CalculateRSInProcessLock;

  bool m_fReadingInProcess; // �Ƿ�����ȡ�б�ʶ
  CCriticalSection m_ReadingInProcessLock;

  bool m_fDataBaseInProcess; // �Ƿ��ڴ洢������ʷ���ݵ����ݿ�����С�
  CCriticalSection m_DataBaseInProcessLock;

  bool m_fRTDataReadingInProcess;  // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
  CCriticalSection m_RTDataReadingInProcessLock;

  bool m_fRTDataReady;             // ʵʱ��������ȡ��ɱ�ʶ
  CCriticalSection m_RTDataReadyLock;

  bool m_fDayLineDataReady;      // ������������ȡ��ɱ�ʶ
  CCriticalSection m_DayLineDataReadyLock;

};
