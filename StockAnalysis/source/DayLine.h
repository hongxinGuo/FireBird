#pragma once

#include"SetDayLine.h"
#include"SetDayLineInfo.h"

using namespace std;
#include<memory>

class CDayLine final : public CObject {
public:
  CDayLine();
  CDayLine(CDayLine&);
  void operator=(CDayLine&);
  void SetData(CSetDayLine* pDayLine);
  void SetData(CSetDayLineInfo* pDayLineInfo);
  void Reset(void);       // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��

public:
  long GetDay(void) noexcept { return m_lDay; }
  void SetDay(long lDay) noexcept { m_lDay = lDay; }
  time_t GetTime(void) noexcept { return m_time; }
  void SetTime(time_t t) noexcept { m_time = t; }
  WORD GetMarket(void) noexcept { return m_wMarket; }
  void SetMarket(WORD wMarket) noexcept { m_wMarket = wMarket; }
  CString GetStockCode(void) { return m_strStockCode; }
  void SetStockCode(CString str) noexcept { m_strStockCode = str; }
  CString GetStockName(void) { return m_strStockName; }
  void SetStockName(CString str) { m_strStockName = str; }

  long GetLastClose(void) noexcept { return m_lLastClose; }
  void SetLastClose(long fValue) noexcept { m_lLastClose = fValue; }
  long GetOpen(void) noexcept { return m_lOpen; }
  void SetOpen(long fValue) noexcept { m_lOpen = fValue; }
  long GetHigh(void) noexcept { return m_lHigh; }
  void SetHigh(long fValue) noexcept { m_lHigh = fValue; }
  long GetLow(void) noexcept { return m_lLow; }
  void SetLow(long fValue) noexcept { m_lLow = fValue; }
  long GetClose(void) noexcept { return m_lClose; }
  void SetClose(long fValue) noexcept { m_lClose = fValue; }
  double GetUpDown(void) noexcept { return m_fUpDown; }
  void SetUpDown(double fValue) noexcept { m_fUpDown = fValue; }
  double GetUpDownRate(void) noexcept { return m_dUpDownRate; }
  void SetUpDownRate(double dValue) noexcept { m_dUpDownRate = dValue; }
  double GetChangeHandRate(void) noexcept { return m_fChangeHandRate; }
  void SetChangeHandRate(double dValue) noexcept { m_fChangeHandRate = dValue; }
  INT64 GetVolume(void) noexcept { return m_lVolume; }
  void SetVolume(INT64 lValue) noexcept { m_lVolume = lValue; }
  INT64 GetAmount(void) noexcept { return m_lAmount; }
  void SetAmount(INT64 lValue) noexcept { m_lAmount = lValue; }

  INT64 GetTotalValue(void) noexcept { return m_lTotalValue; }
  void SetTotalValue(char* buffer);
  INT64 GetCurrentValue(void) noexcept { return m_lCurrentValue; }
  void SetCurrentValue(char* buffer);
  double GetRelativeStrong(void) noexcept { return m_dRelativeStrong; }
  void SetRelativeStrong(double dValue) noexcept { m_dRelativeStrong = dValue; }
  long GetTransactionNumber(void) noexcept { return m_lTransactionNumber; }
  void SetTransactionNumber(long lValue) noexcept { m_lTransactionNumber = lValue; }

  long GetTransactionNumberBelow5000(void) noexcept { return m_lTransactionNumberBelow5000; }
  void SetTransactionNumberBelow5000(long lValue) noexcept { m_lTransactionNumberBelow5000 = lValue; }
  long GetTransactionNumberBelow50000(void) noexcept { return m_lTransactionNumberBelow50000; }
  void SetTransactionNumberBelow50000(long lValue) noexcept { m_lTransactionNumberBelow50000 = lValue; }
  long GetTransactionNumberBelow200000(void) noexcept { return m_lTransactionNumberBelow200000; }
  void SetTransactionNumberBelow200000(long lValue) noexcept { m_lTransactionNumberBelow200000 = lValue; }
  long GetTransactionNumberAbove200000(void) noexcept { return m_lTransactionNumberAbove200000; }
  void SetTransactionNumberAbove200000(long lValue) noexcept { m_lTransactionNumberAbove200000 = lValue; }

  long GetAttackBuyVolume(void) noexcept { return m_lAttackBuyVolume; }
  void SetAttackBuyVolume(long fValue) noexcept { m_lAttackBuyVolume = fValue; }
  long GetStrongBuyVolume(void) noexcept { return m_lStrongBuyVolume; }
  void SetStrongBuyVolume(long fValue) noexcept { m_lStrongBuyVolume = fValue; }
  long GetAttackSellVolume(void) noexcept { return m_lAttackSellVolume; }
  void SetAttackSellVolume(long fValue) noexcept { m_lAttackSellVolume = fValue; }
  long GetStrongSellVolume(void) noexcept { return m_lStrongSellVolume; }
  void SetStrongSellVolume(long fValue) noexcept { m_lStrongSellVolume = fValue; }
  long GetUnknownVolume(void) noexcept { return m_lUnknownVolume; }
  void SetUnknownVolume(long fValue) noexcept { m_lUnknownVolume = fValue; }
  long GetCancelBuyVolume(void) noexcept { return m_lCancelBuyVolume; }
  void SetCancelBuyVolume(long fValue) noexcept { m_lCancelBuyVolume = fValue; }
  long GetCancelSellVolume(void) noexcept { return m_lCancelSellVolume; }
  void SetCancelSellVolume(long fValue) noexcept { m_lCancelSellVolume = fValue; }

  long GetOrdinaryBuyVolume(void) noexcept { return m_lOrdinaryBuyVolume; }
  void SetOrdinaryBuyVolume(long fValue) noexcept { m_lOrdinaryBuyVolume = fValue; }
  long GetAttackBuyBelow50000(void) noexcept { return m_lAttackBuyBelow50000; }
  void SetAttackBuyBelow50000(long fValue) noexcept { m_lAttackBuyBelow50000 = fValue; }
  long GetAttackBuyBelow200000(void) noexcept { return m_lAttackBuyBelow200000; }
  void SetAttackBuyBelow200000(long fValue) noexcept { m_lAttackBuyBelow200000 = fValue; }
  long GetAttackBuyAbove200000(void) noexcept { return m_lAttackBuyAbove200000; }
  void SetAttackBuyAbove200000(long fValue) noexcept { m_lAttackBuyAbove200000 = fValue; }
  long GetOrdinarySellVolume(void) noexcept { return m_lOrdinarySellVolume; }
  void SetOrdinarySellVolume(long fValue) noexcept { m_lOrdinarySellVolume = fValue; }
  long GetAttackSellBelow50000(void) noexcept { return m_lAttackSellBelow50000; }
  void SetAttackSellBelow50000(long fValue) noexcept { m_lAttackSellBelow50000 = fValue; }
  long GetAttackSellBelow200000(void) noexcept { return m_lAttackSellBelow200000; }
  void SetAttackSellBelow200000(long fValue) noexcept { m_lAttackSellBelow200000 = fValue; }
  long GetAttackSellAbove200000(void) noexcept { return m_lAttackSellAbove200000; }
  void SetAttackSellAbove200000(long fValue) noexcept { m_lAttackSellAbove200000 = fValue; }

  void Set3DayRS(double dValue) noexcept { m_d3DayRS = dValue; }
  double Get3DayRS(void) noexcept { return m_d3DayRS; }
  void Set5DayRS(double fValue) noexcept { m_d5DayRS = fValue; }
  double Get5DayRS(void) noexcept { return m_d5DayRS; }
  void Set10DayRS(double fValue) noexcept { m_d10DayRS = fValue; }
  double Get10DayRS(void) noexcept { return m_d10DayRS; }
  void Set30DayRS(double fValue) noexcept { m_d30DayRS = fValue; }
  double Get30DayRS(void) noexcept { return m_d30DayRS; }
  void Set60DayRS(double fValue) noexcept { m_d60DayRS = fValue; }
  double Get60DayRS(void) noexcept { return m_d60DayRS; }
  void Set120DayRS(double fValue) noexcept { m_d120DayRS = fValue; }
  double Get120DayRS(void) noexcept { return m_d120DayRS; }

protected:
  // need to save
  long m_lDay; // ����(YYYYMMDD)
  time_t m_time;
  WORD m_wMarket;
  CString m_strStockCode;
  CString m_strStockName;

  // ���¼����۸��ǷŴ���һǧ��
  long m_lLastClose; // ǰ���̡���λ��0.001Ԫ
  long m_lOpen; // ���̼�
  long m_lHigh; // ��߼�
  long m_lLow; // ��ͼ�
  long m_lClose; // ���̼�

  // ������ֵ��ʵ��ֵ
  double			m_fUpDown;						// �ǵ���
  double      m_dUpDownRate;
  double			m_fChangeHandRate;			// ������
  INT64	      m_lVolume;							// �ɽ���,��λ:��
  INT64		    m_lAmount;							// �ɽ����,��λ:Ԫ/��Ԫ�����̣�
  INT64	      m_lTotalValue;					// ����ֵ����λ����Ԫ
  INT64       m_lCurrentValue;				// ��ͨ��ֵ����λ����Ԫ

  long			  m_lOrdinaryBuyVolume;     // �������롣�ɽ��۽ӽ��������һ��������������λ����
  long			  m_lAttackBuyVolume;		    // ���Ͻ��������룬�ɽ��۳�����һ�۸񵫲����������ۡ�����ɽ�����������m_lStrongBuyVolume��
  long			  m_lStrongBuyVolume;		    // ����ǿ������,�ɽ��۳���֮ǰ���������ۡ�
  long			  m_lOrdinarySellVolume;
  long			  m_lAttackSellVolume;			// ����������������һ�۵����������
  long			  m_lStrongSellVolume;		  // ����ǿ������,�ɽ��۵���֮ǰ���������
  long			  m_lUnknownVolume;
  long			  m_lCancelBuyVolume;		    // �򵥳�����
  long			  m_lCancelSellVolume;			// ����������
  double      m_dRelativeStrong;        // ���ǿ��

  long        m_lTransactionNumber;
  long        m_lTransactionNumberBelow5000;
  long        m_lTransactionNumberBelow50000;
  long        m_lTransactionNumberBelow200000;
  long        m_lTransactionNumberAbove200000;

  long			  m_lAttackBuyBelow50000;
  long			  m_lAttackBuyBelow200000;
  long			  m_lAttackBuyAbove200000;
  long			  m_lAttackSellBelow50000;
  long			  m_lAttackSellBelow200000;
  long			  m_lAttackSellAbove200000;

public:
  // don't need to save
  double      m_d3DayRS;
  double			m_d5DayRS;
  double			m_d10DayRS;
  double			m_d30DayRS;
  double			m_d60DayRS;
  double			m_d120DayRS;
};

typedef shared_ptr<CDayLine> CDayLinePtr;
