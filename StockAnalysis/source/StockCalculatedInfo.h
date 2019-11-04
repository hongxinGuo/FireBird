#pragma once

#include"SetDayLine.h"
#include"SetDayLineToday.h"
#include"SetDayLineInfo.h"

class CStockCalculatedInfo {
public:
  CStockCalculatedInfo();
  ~CStockCalculatedInfo();
  void Reset(void);

  void StoreCalculatedInfo(CSetDayLineInfo& setDayLineInfo);
  void StoreTempInfo(CSetDayLineToday& setDayLineToday);
  void LoadAndCalculateTempInfo(CSetDayLineToday& setDayLineToday);

  INT64 GetAttackBuyAmount(void) noexcept { return m_lAttackBuyAmount; }
  INT64 GetAttackSellAmount(void) noexcept { return m_lAttackSellAmount; }
  INT64 GetOrdinaryBuyVolume(void) noexcept { return m_lOrdinaryBuyVolume; }
  INT64 GetOrdinarySellVolume(void) noexcept { return m_lOrdinarySellVolume; }
  INT64 GetAttackBuyVolume(void) noexcept { return m_lAttackBuyVolume; } // �������롣�ɽ��۸�����һ�۵����������ۡ����������������ǿ������
  INT64 GetStrongBuyVolume(void) noexcept { return m_lStrongBuyVolume; } // ����ǿ������,�ɽ��۳���֮ǰ����������
  INT64 GetCurrentAttackBuy(void) noexcept { return m_lCurrentAttackBuy; }
  long GetCurrentStrongBuy(void) noexcept { return m_lCurrentStrongBuy; }
  INT64 GetAttackSellVolume(void) noexcept { return m_lAttackSellVolume; } // �����������ɽ��۵�����һ�۵���������ۡ�
  INT64 GetStrongSellVolume(void) noexcept { return m_lStrongSellVolume; }
  long GetCurrentAttackSell(void) noexcept { return m_lCurrentAttackSell; }
  long GetCurrentStrongSell(void) noexcept { return m_lCurrentStrongSell; }
  INT64 GetUnknownVolume(void) noexcept { return m_lUnknownVolume; }
  long GetCurrentUnknown(void) noexcept { return m_lCurrentUnknown; }
  INT64 GetCancelBuyVolume(void) noexcept { return m_lCancelBuyVolume; }
  INT64 GetCancelSellVolume(void) noexcept { return m_lCancelSellVolume; }

  long GetTransactionNumber(void) noexcept { return m_lTransactionNumber; }
  void IncreaseTransactionNumber(long value = 1) { m_lTransactionNumber += value; }
  long GetTransactionNumberBelow5000(void) noexcept { return m_lTransactionNumberBelow5000; }
  void IncreaseTransactionNumberBelow5000(long value = 1) { m_lTransactionNumberBelow5000 += value; }
  long GetTransactionNumberBelow50000(void) noexcept { return m_lTransactionNumberBelow50000; }
  void IncreaseTransactionNumberBelow50000(long value = 1) { m_lTransactionNumberBelow50000 += value; }
  long GetTransactionNumberBelow200000(void) noexcept { return m_lTransactionNumberBelow200000; }
  void IncreaseTransactionNumberBelow200000(long value = 1) { m_lTransactionNumberBelow200000 += value; }
  long GetTransactionNumberAbove200000(void) noexcept { return m_lTransactionNumberAbove200000; }
  void IncreaseTransactionNumberAbove200000(long value = 1) { m_lTransactionNumberAbove200000 += value; }

  INT64 GetAttackBuyBelow50000(void) noexcept { return m_lAttackBuyBelow50000; }
  INT64 GetAttackBuyBelow200000(void) noexcept { return m_lAttackBuyBelow200000; }
  INT64 GetAttackBuyAbove200000(void) noexcept { return m_lAttackBuyAbove200000; }
  INT64 GetAttackSellBelow50000(void) noexcept { return m_lAttackSellBelow50000; }
  INT64 GetAttackSellBelow200000(void) noexcept { return m_lAttackSellBelow200000; }
  INT64 GetAttackSellAbove200000(void) noexcept { return m_lAttackSellAbove200000; }

  void SetAttackBuyAmount(INT64 value) noexcept { m_lAttackBuyAmount = value; }
  void SetAttackSellAmount(INT64 value) noexcept { m_lAttackSellAmount = value; }
  void SetOrdinaryBuyVolume(INT64 value) noexcept { m_lOrdinaryBuyVolume = value; }
  void IncreaseOrdinaryBuyVolume(INT64 value) noexcept { m_lOrdinaryBuyVolume += value; }
  void SetOrdinarySellVolume(INT64 value) noexcept { m_lOrdinarySellVolume = value; }
  void IncreaseOrdinarySellVolume(INT64 value) noexcept { m_lOrdinarySellVolume += value; }
  void SetAttackBuyVolume(INT64 value) noexcept { m_lAttackBuyVolume = value; } // �������롣�ɽ��۸�����һ�۵����������ۡ����������������ǿ������
  void IncreaseAttackBuyVolume(INT64 value) noexcept { m_lAttackBuyVolume += value; }
  void SetStrongBuyVolume(INT64 value) noexcept { m_lStrongBuyVolume = value; } // ����ǿ������,�ɽ��۳���֮ǰ����������
  void IncreaseStrongBuyVolume(INT64 value) noexcept { m_lStrongBuyVolume += value; }
  void SetCurrentAttackBuy(long value) noexcept { m_lCurrentAttackBuy = value; }
  void SetCurrentStrongBuy(long value) noexcept { m_lCurrentStrongBuy = value; }
  void SetAttackSellVolume(INT64 value) noexcept { m_lAttackSellVolume = value; } // �����������ɽ��۵�����һ�۵���������ۡ�
  void IncreaseAttackSellVolume(INT64 value) noexcept { m_lAttackSellVolume += value; }
  void SetStrongSellVolume(INT64 value) noexcept { m_lStrongSellVolume = value; }
  void IncreaseStrongSellVolume(INT64 value) noexcept { m_lStrongSellVolume += value; }
  void SetCurrentAttackSell(long value) noexcept { m_lCurrentAttackSell = value; }
  void SetCurrentStrongSell(long value) noexcept { m_lCurrentStrongSell = value; }
  void SetUnknownVolume(INT64 value) noexcept { m_lUnknownVolume = value; }
  void IncreaseUnknownVolume(INT64 value) noexcept { m_lUnknownVolume += value; }
  void SetCurrentUnknown(long value) noexcept { m_lCurrentUnknown = value; }
  void SetCancelBuyVolume(INT64 value) noexcept { m_lCancelBuyVolume = value; }
  void IncreaseCancelBuyVolume(INT64 value) noexcept { m_lCancelBuyVolume += value; }
  void SetCancelSellVolume(INT64 value) noexcept { m_lCancelSellVolume = value; }
  void IncreaseCancelSellVolume(INT64 value) noexcept { m_lCancelSellVolume += value; }

  void SetTransactionNumber(long value) noexcept { m_lTransactionNumber = value; }
  void SetTransactionNumberBelow5000(long value) noexcept { m_lTransactionNumberBelow5000 = value; }
  void SetTransactionNumberBelow50000(long value) noexcept { m_lTransactionNumberBelow50000 = value; }
  void SetTransactionNumberBelow200000(long value) noexcept { m_lTransactionNumberBelow200000 = value; }
  void SetTransactionNumberAbove200000(long value) noexcept { m_lTransactionNumberAbove200000 = value; }

  void SetAttackBuyBelow50000(INT64 value) noexcept { m_lAttackBuyBelow50000 = value; }
  void IncreaseAttackBuyBelow50000(INT64 value) noexcept { m_lAttackBuyBelow50000 += value; }
  void SetAttackBuyBelow200000(INT64 value) noexcept { m_lAttackBuyBelow200000 = value; }
  void IncreaseAttackBuyBelow200000(INT64 value) noexcept { m_lAttackBuyBelow200000 += value; }
  void SetAttackBuyAbove200000(INT64 value) noexcept { m_lAttackBuyAbove200000 = value; }
  void IncreaseAttackBuyAbove200000(INT64 value) noexcept { m_lAttackBuyAbove200000 += value; }
  void SetAttackSellBelow50000(INT64 value) noexcept { m_lAttackSellBelow50000 = value; }
  void IncreaseAttackSellBelow50000(INT64 value) noexcept { m_lAttackSellBelow50000 += value; }
  void SetAttackSellBelow200000(INT64 value) noexcept { m_lAttackSellBelow200000 = value; }
  void IncreaseAttackSellBelow200000(INT64 value) noexcept { m_lAttackSellBelow200000 += value; }
  void SetAttackSellAbove200000(INT64 value) noexcept { m_lAttackSellAbove200000 = value; }
  void IncreaseAttackSellAbove200000(INT64 value) noexcept { m_lAttackSellAbove200000 += value; }

protected:
  // ���±������ڷ������������������
  INT64 m_lAttackBuyAmount; // ����������
  INT64 m_lAttackSellAmount; // �����������
  INT64 m_lCurrentVolume;

  long m_lTransactionNumber; // �������յĳɽ�����
  long m_lTransactionNumberBelow5000; // �������յ���5000�ɵĳɽ�����
  long m_lTransactionNumberBelow50000; //
  long m_lTransactionNumberBelow200000; //
  long m_lTransactionNumberAbove200000; //

  INT64 m_lOrdinaryBuyVolume;
  INT64 m_lOrdinarySellVolume;
  INT64 m_lAttackBuyVolume;	// �������롣�ɽ��۸�����һ�۵����������ۡ����������������ǿ������
  INT64 m_lStrongBuyVolume;	// ����ǿ������,�ɽ��۳���֮ǰ����������
  long m_lCurrentAttackBuy;
  long m_lCurrentStrongBuy;
  INT64 m_lAttackSellVolume; // �����������ɽ��۵�����һ�۵���������ۡ�
  INT64 m_lStrongSellVolume; // ����ǿ������,�ɽ��۵���֮ǰ���������
  long m_lCurrentAttackSell;
  long m_lCurrentStrongSell;
  INT64 m_lUnknownVolume; // ����״̬�ĳɽ���
  long m_lCurrentUnknown; // ��ǰ�ɽ����еĲ���״̬�ɽ���
  INT64 m_lCancelBuyVolume;	// �򵥳�����
  INT64 m_lCancelSellVolume; // ����������

  INT64 m_lAttackBuyBelow50000;
  INT64 m_lAttackBuyBelow200000;
  INT64 m_lAttackBuyAbove200000;
  INT64 m_lAttackSellBelow50000;
  INT64 m_lAttackSellBelow200000;
  INT64 m_lAttackSellAbove200000;
};
