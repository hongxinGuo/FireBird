#pragma once

#include"afxmt.h"

class CSystemStatus {    // �������ݽṹ
public:
  CSystemStatus();

  void SetReadingInProcess(bool fFlag);
  bool IsReadingInProcess(void);

  void SetDataBaseInProcess(bool fFlag);
  bool IsDataBaseInProcess(void);

  void SetReceiveFromWebInProcess(bool fFlag);
  bool IsReceiveFromWebInProcess(void);

  void SetRTDataReady(bool fFlag);
  bool IsRTDataReady(void);

  void SetDayLineDataReady(bool fFlag);
  bool IsDayLineDataReady(void);

protected:
  bool m_fCalculatingRelativeStrongInProcess; // �Ƿ��ڼ������ǿ�ȵĹ����б�ʶ
  bool m_ffErrorProcessDayLine;              // ���δ������������Ƿ�����ı�ʶ

  bool m_fReadingInProcess; // �Ƿ�����ȡ�б�ʶ
  CCriticalSection m_ReadingInProcessLock;

  bool m_fDataBaseInProcess; // �Ƿ��ڴ洢������ʷ���ݵ����ݿ�����С�
  CCriticalSection m_DataBaseInProcessLock;

  bool m_fReceiveFromWebInProcess;  // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
  CCriticalSection m_ReceiveFromWebInProcessLock;

  bool m_fRTDataReady;             // ʵʱ��������ȡ��ɱ�ʶ
  CCriticalSection m_RTDataReadyLock;

  bool m_fDayLineDataReady;      // ������������ȡ��ɱ�ʶ
  CCriticalSection m_DayLineDataReadyLock;

};
