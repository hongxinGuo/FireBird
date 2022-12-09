/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �ɽ����仯һ�κ�,����ҵ��ͳɽ��ľ��������
//
// �ɽ��ľ����������Ϊ���֣��򣬽�������ǿ�򣬡����Ǽ�λΪ��һλ�ã����������λ�������������ҳɽ���λ������һ����������
// ǿ���λ�����������ҳɽ���λ���ٸ����������ж�����֮�����ơ�
//
// �ҵ��ı仯��������_TYPE_CANCELED_BUY_��_TYPE_CANCELED_SELL_��ʶ����������Ϊcanceled sell����������Ϊcanceled buy��
//
// ѭ�򽥽������Ӻ������ܡ�
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


#include<memory>

enum {
  _TYPE_UNKNOWN_ = 0,
  _TYPE_BUY_ = 1,
  _TYPE_ATTACK_BUY_ = 2,
  _TYPE_STRONG_BUY_ = 3,
  _TYPE_CANCELED_BUY_ = 4,
  _TYPE_SELL_ = 11,
  _TYPE_STRONG_SELL_ = 12,
  _TYPE_ATTACK_SELL_ = 13,
  _TYPE_CANCELED_SELL = 14,
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
