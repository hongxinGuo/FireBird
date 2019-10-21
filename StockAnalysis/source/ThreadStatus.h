#pragma once

#include"afxmt.h"

const int gl_cMaxCalculatingRSThreads = 8;

class CThreadStatus {    // ���߳�״̬
public:
  CThreadStatus();

  // �߳��˳����״̬�����á��˱�־����ϵͳ�˳��������б���رյ��̹߳رպ󷽿�����ϵͳ�˳�������ϵͳ��Ҫ�ȴ���
  void SetExitingThread(bool fFlag);
  bool IsExitingThread(void);

  // �����������������ǿ����������
  void SetCalculatingDayLineRS(bool fFlag);
  bool IsCalculatingDayLineRS(void);

  // ��ȡ������������
  void SetReadingNeteaseDayLine(bool fFlag);
  bool IsReadingNeteaseDayLine(void);

  // �洢������������
  void SetSavingDayLine(bool fFlag);
  bool IsSavingDayLine(void);

  // ������Ѷʵʱ������������
  void SetReadingTengxunRTData(bool fFlag);
  bool IsReadingTengxunRTData(void);

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

  // ����ִ�м����������ǿ�ȵļ��������������gl_cMaxCalculatingRSThreads���߳�ͬʱִ��
  void IncreaseNunberOfCalculatingRSThreads(void);  // ͬʱ�����߳�����һ
  void DecreaseNumberOfCalculatingRSThreads(void);  // ͬʱ�����߳�����һ
  bool IsCalculatingRSThreadAvailable(void);        // �Ƿ����������µĹ����߳�
  bool IsCalculatingRSThreadRunning(void);          // �������ߵ��߳��Ƿ���������

protected:
  bool m_fExitingThread;                // Ҫ��������߳��˳�
  CCriticalSection m_ExitingThreadLock;

  bool m_fCalculatingDayLineRelativeStrong; // �Ƿ��ڼ������ǿ�ȵĹ����б�ʶ
  CCriticalSection m_CalculatingDayLineRSLock;

  bool m_fReadingNeteaseDayLine; // �Ƿ�����ȡ������ʷ�����б�ʶ
  CCriticalSection m_ReadingNeteaseDayLineLock;

  bool m_fSavingDayLine; // �Ƿ��ڴ洢������ʷ���ݵ����ݿ�����С�
  CCriticalSection m_SavingDayLineLock;

  bool m_fReadingTengxunRTData;  // ������Ѷʵʱ�����߳��Ƿ�ִ�б�ʶ
  CCriticalSection m_ReadingTengxunRTDataLock;

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

  int m_iNumberOfCalculatingRSThreads;  // ���ڼ����������ǿ�ȵ��߳�����Ŀǰ���ͬʱ����gl_cMaxCalculatingRSThreads���߳�
  CCriticalSection m_NumberOfCalculatingRSThreads;
};
