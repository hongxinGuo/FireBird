#pragma once

#include"SetDayLine.h"

using namespace std;
#include<memory>

class CDayLine : public CObject {
public :
	CDayLine();
	CDayLine( CDayLine & );
	void operator=( CDayLine& );
  void SetData(CSetDayLine * pDayLine);

public:
  long		  GetDay(void) { return m_lDay; }
  void		  SetDay(long lDay) { m_lDay = lDay; }
	time_t	  GetTime(void) { return m_time; }
	void      SetTime(time_t t) { m_time = t; }
	WORD		  GetMarket(void) { return m_wMarket; }
	void      SetMarket(WORD wMarket) { m_wMarket = wMarket; }
	CString   GetStockCode(void) { return m_strStockCode; }
	void		  SetStockCode(CString str) { m_strStockCode = str; }
	CString   GetStockName(void) { return m_strStockName; }
	void		  SetStockName(CString str) { m_strStockName = str; }

  long		  GetLastClose(void) { return m_lLastClose; }
  void		  SetLastClose(long fValue) { m_lLastClose = fValue; }
  long		  GetOpen(void) { return m_lOpen; }
  void		  SetOpen(long fValue) { m_lOpen = fValue; }
  long		  GetHigh(void) { return m_lHigh; }
  void		  SetHigh(long fValue) { m_lHigh = fValue; }
  long		  GetLow(void) { return m_lLow; }
  void		  SetLow(long fValue) { m_lLow = fValue; }
  long		  GetClose(void) { return m_lClose; }
  void		  SetClose(long fValue) { m_lClose = fValue; }
  double		GetUpDown(void) { return m_fUpDown; }
	void		  SetUpDown(double fValue) { m_fUpDown = fValue; }
  double    GetUpDownRate(void) { return m_dUpDownRate; }
  void      SetUpDownRate(double dValue) { m_dUpDownRate = dValue; }
  double    GetChangeHandRate(void) { return m_fChangeHandRate; }
  void      SetChangeHandRate(double dValue) { m_fChangeHandRate = dValue; }
	long long GetVolume(void) { return m_lVolume; }
	void		  SetVolume(long long lValue) { m_lVolume = lValue; }
	long long	GetAmount(void) { return m_lAmount; }
	void		  SetAmount(long long dValue) { m_lAmount = dValue; }

	long long GetTotalValue(void) { return m_lTotalValue; }
	void      SetTotalValue(long long dValue) { m_lTotalValue = dValue; }
	long long GetCurrentValue(void) { return m_lCurrentValue; }
	void      SetCurrentValue(long long dValue) { m_lCurrentValue = dValue; }
  double    GetRelativeStrong(void) { return m_dRelativeStrong; }
  void      SetRelativeStrong(double dValue) { m_dRelativeStrong = dValue; }
  long      GetTransactionNumber(void) { return m_lTransactionNumber; }
  void      SetTransactionNumber(long lValue) { m_lTransactionNumber = lValue; }

  long      GetTransactionNumberBelow5000(void) { return m_lTransactionNumberBelow5000; }
  void      SetTransactionNumberBelow5000(long lValue) { m_lTransactionNumberBelow5000 = lValue; }
  long      GetTransactionNumberBelow50000(void) { return m_lTransactionNumberBelow50000; }
  void      SetTransactionNumberBelow50000(long lValue) { m_lTransactionNumberBelow50000 = lValue; }
  long      GetTransactionNumberBelow200000(void) { return m_lTransactionNumberBelow200000; }
  void      SetTransactionNumberBelow200000(long lValue) { m_lTransactionNumberBelow200000 = lValue; }
  long      GetTransactionNumberAbove200000(void) { return m_lTransactionNumberAbove200000; }
  void      SetTransactionNumberAbove200000(long lValue) { m_lTransactionNumberAbove200000 = lValue; }

  long		  GetAttackBuyVolume(void) { return m_lAttackBuyVolume; }
  void		  SetAttackBuyVolume(long fValue) { m_lAttackBuyVolume = fValue; }
  long		  GetStrongBuyVolume(void) { return m_lStrongBuyVolume; }
  void		  SetStrongBuyVolume(long fValue) { m_lStrongBuyVolume = fValue; }
  long		  GetAttackSellVolume(void) { return m_lAttackSellVolume; }
  void		  SetAttackSellVolume(long fValue) { m_lAttackSellVolume = fValue; }
  long		  GetStrongSellVolume(void) { return m_lStrongSellVolume; }
  void		  SetStrongSellVolume(long fValue) { m_lStrongSellVolume = fValue; }
  long		  GetUnknownVolume(void) { return m_lUnknownVolume; }
  void		  SetUnknownVolume(long fValue) { m_lUnknownVolume = fValue; }
  long		  GetCancelBuyVolume(void) { return m_lCancelBuyVolume; }
  void		  SetCancelBuyVolume(long fValue) { m_lCancelBuyVolume = fValue; }
  long		  GetCancelSellVolume(void) { return m_lCancelSellVolume; }
  void		  SetCancelSellVolume(long fValue) { m_lCancelSellVolume = fValue; }

  long		  GetOrdinaryBuyVolume(void) { return m_lOrdinaryBuyVolume; }
  void		  SetOrdinaryBuyVolume(long fValue) { m_lOrdinaryBuyVolume = fValue; }
  long		  GetAttackBuyBelow50000(void) { return m_lAttackBuyBelow50000; }
  void		  SetAttackBuyBelow50000(long fValue) { m_lAttackBuyBelow50000 = fValue; }
  long		  GetAttackBuyBelow200000(void) { return m_lAttackBuyBelow200000; }
  void		  SetAttackBuyBelow200000(long fValue) { m_lAttackBuyBelow200000 = fValue; }
  long		  GetAttackBuyAbove200000(void) { return m_lAttackBuyAbove200000; }
  void		  SetAttackBuyAbove200000(long fValue) { m_lAttackBuyAbove200000 = fValue; }
  long		  GetOrdinarySellVolume(void) { return m_lOrdinarySellVolume; }
  void		  SetOrdinarySellVolume(long fValue) { m_lOrdinarySellVolume = fValue; }
  long		  GetAttackSellBelow50000(void) { return m_lAttackSellBelow50000; }
  void		  SetAttackSellBelow50000(long fValue) { m_lAttackSellBelow50000 = fValue; }
  long		  GetAttackSellBelow200000(void) { return m_lAttackSellBelow200000; }
  void		  SetAttackSellBelow200000(long fValue) { m_lAttackSellBelow200000 = fValue; }
  long		  GetAttackSellAbove200000(void) { return m_lAttackSellAbove200000; }
  void		  SetAttackSellAbove200000(long fValue) { m_lAttackSellAbove200000 = fValue; }

  void      Set3DAyRS(double dValue) { m_d3DayRS = dValue; }
  double    Get3DayRS(void) { return m_d3DayRS; }
  void		  Set5DayRS(double fValue) { m_d5DayRS = fValue; }
  double		Get5DayRS(void) { return m_d5DayRS; }
  void		  Set10DayRS(double fValue) { m_d10DayRS = fValue; }
  double		Get10DayRS(void) { return m_d10DayRS; }
  void		  Set30DayRS(double fValue) { m_d30DayRS = fValue; }
  double		Get30DayRS(void) { return m_d30DayRS; }
  void		  Set60DayRS(double fValue) { m_d60DayRS = fValue; }
  double		Get60DayRS(void) { return m_d60DayRS; }
  void		  Set120DayRS(double fValue) { m_d120DayRS = fValue; }
  double		Get120DayRS(void) { return m_d120DayRS; }

protected:
// need to save
	long			  m_lDay;									// ����(YYYYMMDD)
	time_t		  m_time;
	WORD			  m_wMarket;
	CString     m_strStockCode;
	CString		  m_strStockName;

  // ���¼����۸��ǷŴ���һǧ��
	long			  m_lLastClose;					  // ǰ���̡���λ��0.001Ԫ
	long			  m_lOpen;								// ���̼�
	long			  m_lHigh;								// ��߼�
	long			  m_lLow;								  // ��ͼ�
	long			  m_lClose;							  // ���̼�

  // ������ֵ��ʵ��ֵ
	double			m_fUpDown;						// �ǵ���
  double      m_dUpDownRate;
	double			m_fChangeHandRate;			// ������
	long long	  m_lVolume;							// �ɽ���,��λ:��
	long long		m_lAmount;							// �ɽ����,��λ:Ԫ/��Ԫ�����̣�
	long long	  m_lTotalValue;					// ����ֵ����λ����Ԫ
	long long   m_lCurrentValue;				// ��ͨ��ֵ����λ����Ԫ

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

