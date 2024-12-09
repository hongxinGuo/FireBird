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
#include"pch.h"
module;
export module FireBird.Type.OneDeal;

export {
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

	class COneDeal {
	public:
		COneDeal();
		~COneDeal();

		auto GetMarketTime() const noexcept { return m_tpTime; }
		void SetTime(std::chrono::sys_seconds time) noexcept { m_tpTime = time; }
		void SetTime(time_t time) noexcept { m_tpTime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(time)); }
		long GetType() const noexcept { return m_lType; }
		void SetType(long lType) noexcept { m_lType = lType; }
		long GetPrice() const noexcept { return m_lPrice; }
		void SetPrice(long lPrice) noexcept { m_lPrice = lPrice; }
		long GetVolume() const noexcept { return m_lVolume; }
		void SetVolume(long lVolume) noexcept { m_lVolume = lVolume; }

	protected:
		std::chrono::sys_seconds m_tpTime{ std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(0)) };      // ����ʱ��
		long m_lType{ 0 };      // ��������
		long m_lPrice{ 0 };     // �ɽ��۸񡣵�λ��0.001Ԫ��
		long m_lVolume{ 0 };    // ������������λ���ɡ�
	};

	using COneDealPtr = shared_ptr<COneDeal>;
}
