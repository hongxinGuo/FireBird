#pragma once

#include"afxmt.h"

class CSystemStatus {    // �������ݽṹ
public:
  CSystemStatus();

  void SetExitingClientThreadInProcess(bool fFlag);
  bool IsExitingClientThreadInProcess(void);

  void SetCalculateRSInProcess(bool fFlag);
  bool IsCalculateRSInProcess(void);

  void SetDayLineReadingInProcess(bool fFlag);
  bool IsDayLineReadingInProcess(void);

  void SetSavingDayLineInProcess(bool fFlag);
  bool IsSavingDayLineInProcess(void);

  void SetRTDataReadingInProcess(bool fFlag);
  bool IsRTDataReadingInProcess(void);

  void SetRTDataReceived(bool fFlag);
  bool IsRTDataReceived(void);
  
  void SetRTDataNeedCalculate(bool fFlag);
  bool IsRTDataNeedCalculate(void);

  void SetDayLineDataReady(bool fFlag);
  bool IsDayLineDataReady(void);

  void SetCalculatingRTData(bool fFlag);
  bool IsCalculatingRTData(void);
  
  void SetSavingTempData(bool fFlag);
  bool IsSavingTempData(void);

  void SetSavingStockCodeData(bool fFlag);
  bool IsSavingStockCodeData(void);


protected:
  bool m_fExitingClientThreadInProcess;                // Ҫ��������߳��˳�
  CCriticalSection m_ExitingClientThreadInProcessLock;   

  bool m_fCalculatingRelativeStrongInProcess; // �Ƿ��ڼ������ǿ�ȵĹ����б�ʶ
  CCriticalSection m_CalculateRSInProcessLock;

  bool m_fDayLineReadingInProcess; // �Ƿ�����ȡ������ʷ�����б�ʶ
  CCriticalSection m_DayLineReadingInProcessLock;

  bool m_fSavingDayLineInProcess; // �Ƿ��ڴ洢������ʷ���ݵ����ݿ�����С�
  CCriticalSection m_SavingDayLineInProcessLock;

  bool m_fRTDataReadingInProcess;  // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
  CCriticalSection m_RTDataReadingInProcessLock;

  bool m_fRTDataReceived;             // ʵʱ�����ѽ�����ɱ�ʶ
  CCriticalSection m_RTDataReceivedLock;

  bool m_fRTDataNeedCalculate;                // ʵʱ������Ԥ����ã�׼������
  CCriticalSection m_RTDataNeedCalculateLock;

  bool m_fDayLineDataReady;      // ������������ȡ��ɱ�ʶ
  CCriticalSection m_DayLineDataReadyLock;

  bool m_fCalculatingRTData; // ʵʱ���ݼ����̹߳���״̬��ʶ
  CCriticalSection m_CalculatingRTDataLock;

  bool m_fSavingTempData; // ���������ʱ���ݴ洢״̬��ʶ
  CCriticalSection m_SavingTempDataLock;

  bool m_fSavingStockCodeData; // ��Ʊ�������ݴ洢״̬��ʶ
  CCriticalSection m_SavingStockCodeDataLock;

};
