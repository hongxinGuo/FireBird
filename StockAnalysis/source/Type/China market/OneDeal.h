/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �ɽ����仯һ�κ�,����ҵ��ͳɽ��ľ��������
//
// �ɽ��ľ����������Ϊ���֣��򣬽�������ǿ�򣬡����Ǽ�λΪ��һλ�ã����������λ�������������ҳɽ���λ������һ����������
// ǿ���λ�����������ҳɽ���λ���ٸ����������ж�����֮�����ơ�
//
// �ҵ��ı仯��������__TYPE_CANCELED_BUY__��__TYPE_CANCELED_SELL__��ʶ����������Ϊcanceled sell����������Ϊcanceled buy��
//
// ѭ�򽥽������Ӻ������ܡ�
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


#include<memory>

enum {
  __TYPE_UNKNOWN__ = 0,
  __TYPE_BUY__ = 1,
  __TYPE_ATTACK_BUY__ = 2,
  __TYPE_STRONG_BUY__ = 3,
  __TYPE_CANCELED_BUY__ = 4,
  __TYPE_SELL__ = 11,
  __TYPE_STRONG_SELL__ = 12,
  __TYPE_ATTACK_SELL__ = 13,
  __TYPE_CANCELED_SELL = 14,
};

class COneDeal
{
public:
  COneDeal();
  ~COneDeal();

  time_t GetMarketTime(void) noexcept { return m_time; }
  void SetTime(time_t time) noexcept { m_time = time; }
  long GetType(void) const noexcept { return m_lType; }
  void SetType(long lType) noexcept { m_lType = lType; }
  long GetPrice(void) const noexcept { return m_lPrice; }
  void SetPrice(long lPrice) noexcept { m_lPrice = lPrice; }
  long GetVolume(void) const noexcept { return m_lVolume; }
  void SetVolume(long lVolume) noexcept { m_lVolume = lVolume; }

protected:
  time_t m_time;       // ����ʱ��
  long m_lType;      // ��������
  long m_lPrice;     // �ɽ��۸񡣵�λ��0.001Ԫ��
  long m_lVolume;    // ������������λ���ɡ�
};

typedef shared_ptr<COneDeal> COneDealPtr;
