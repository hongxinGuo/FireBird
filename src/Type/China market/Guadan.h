#pragma once

#include<vector>

enum {
	gl_ctGuadanAdd = 1,
	gl_ctGuadanDeal = 2,
	gl_ctGuadanWithdraw = 3,
};

class COneGuadan {
public:
	COneGuadan() noexcept {
		m_time = 0;
		m_lVolume = 0;
		m_lStatus = 0;
	}

	void SetTime(time_t t) noexcept { m_time = t; }
	time_t GetMarketTime() const noexcept { return m_time; }
	void SetVolume(const long lVolume) noexcept { m_lVolume = lVolume; }
	long GetVolume() const noexcept { return m_lVolume; }
	void SetStatus(const long lStatus) noexcept { m_lStatus = lStatus; }
	long GetStatus() const noexcept { return m_lStatus; }

	time_t m_time;
	long m_lVolume;
	long m_lStatus;
};

class CGuadan {
public:
	CGuadan();
	~CGuadan() = default;

	// ÒÑ²âÊÔº¯Êý
	void AddGuadan(COneGuadan oneGuadan);

	COneGuadan& GetGuadan();
	COneGuadan& GetGuadan(long lIndex);

	long GetCurrentPos() const noexcept { return m_lMaxPos; }
	long GetCompletedPos() const noexcept { return m_lCompletedPos; }

	long GetCurrentVolume() const noexcept { return m_lCurrentVolume; }
	long GetFormerVolume() const noexcept { return m_lFormerVolume; }
	void SetCurrentVolume(long lVolume);

	// ´ý²âÊÔº¯Êý
	void ReSet();
	void SetStatus(long lPos, long lStatus);
	//	void				ProcessGuadan(

protected:
	long m_lCurrentVolume; //
	long m_lFormerVolume; //
	vector<COneGuadan> m_vGuadan;
	long m_lMaxPos;
	long m_lCompletedPos;
};
