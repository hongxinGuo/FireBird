#pragma once

#include"afxmt.h"

class CThreadStutus {    // ���߳�״̬
public:
  CThreadStutus();

  // �߳��˳����״̬�����á��˱�־����ϵͳ�˳��������б���رյ��̹߳رպ󷽿�����ϵͳ�˳�������ϵͳ��Ҫ�ȴ���
  void SetExitingClientThreadInProcess(bool fFlag);
  bool IsExitingClientThreadInProcess(void);

  // ����ĳ���������ǿ���������á�
  void SetCalculateRSInProcess(bool fFlag);
  bool IsCalculateRSInProcess(void);

  // �����������������ǿ����������
  void SetCalculateDayLineRS(bool fFlag);
  bool IsCalculateDayLineRS(void);

  // ��ȡ������������
  void SetNeteaseDayLineReadingInProcess(bool fFlag);
  bool IsNeteaseDayLineReadingInProcess(void);

  // �洢������������
  void SetSavingDayLineInProcess(bool fFlag);
  bool IsSavingDayLineInProcess(void);

  // ��������ʵʱ������������
  void SetSinaRTDataReadingInProcess(bool fFlag);
  bool IsSinaRTDataReadingInProcess(void);

  // ������Ѷʵʱ������������
  void SetTengxunRTDataReadingInProcess(bool fFlag);
  bool IsTengxunRTDataReadingInProcess(void);

  // ����ʵʱ���ݽ��յ���������
  void SetSinaRTDataReceived(bool fFlag);
  bool IsSinaRTDataReceived(void);

  // ��Ѷʵʱ���ݽ��ܵ���������
  void SetTengxunRTDataReceived(bool fFlag);
  bool IsTengxunRTDataReceived(void);

  // ʵʱ������Ҫ������������
  void SetRTDataNeedCalculate(bool fFlag);
  bool IsRTDataNeedCalculate(void);

  // ��������׼������������
  void SetDayLineDataReady(bool fFlag);
  bool IsDayLineDataReady(void);

  // ʵʱ���ݼ�������������
  void SetCalculatingRTData(bool fFlag);
  bool IsCalculatingRTData(void);

  // ��ʱ���ݴ洢����������
  void SetSavingTempData(bool fFlag);
  bool IsSavingTempData(void);

  // ��Ʊ����洢����������
  void SetSavingStockCodeData(bool fFlag);
  bool IsSavingStockCodeData(void);

  void IncreaseNunberOfCalculatingRSThreads(void);
  void DecreaseNumberOfCalculatingRSThreads(void);
  bool IsCalculatingRSThreadAvailable(void);

protected:
  bool m_fExitingClientThreadInProcess;                // Ҫ��������߳��˳�
  CCriticalSection m_ExitingClientThreadInProcessLock;

  bool m_fCalculatingRelativeStrongInProcess; // �Ƿ��ڼ������ǿ�ȵĹ����б�ʶ
  CCriticalSection m_CalculateRSInProcessLock;

  bool m_fCalculatingDayLineRelativeStrong; // �Ƿ��ڼ������ǿ�ȵĹ����б�ʶ
  CCriticalSection m_CalculateDayLineRSLock;

  bool m_fNeteaseDayLineReadingInProcess; // �Ƿ�����ȡ������ʷ�����б�ʶ
  CCriticalSection m_NeteaseDayLineReadingInProcessLock;

  bool m_fSavingDayLineInProcess; // �Ƿ��ڴ洢������ʷ���ݵ����ݿ�����С�
  CCriticalSection m_SavingDayLineInProcessLock;

  bool m_fSinaRTDataReadingInProcess;  // ��������ʵʱ�����߳��Ƿ�ִ�б�ʶ
  CCriticalSection m_SinaRTDataReadingInProcessLock;

  bool m_fTengxunRTDataReadingInProcess;  // ������Ѷʵʱ�����߳��Ƿ�ִ�б�ʶ
  CCriticalSection m_TengxunRTDataReadingInProcessLock;

  bool m_fSinaRTDataReceived;             // ����ʵʱ�����ѽ�����ɱ�ʶ
  CCriticalSection m_SinaRTDataReceivedLock;

  bool m_fTengxunRTDataReceived;             // ��Ѷʵʱ�����ѽ�����ɱ�ʶ
  CCriticalSection m_TengxunRTDataReceivedLock;

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

  int m_iNumberOfCalculatingRSThreads;  // ���ڼ����������ǿ�ȵ��߳�����Ŀǰ���ͬʱ����16���߳�
  CCriticalSection m_NumberOfCalculatingRSThreads;
};
