#pragma once

#include<vector>

enum {
	gl_ctGuadanAdd = 1,
	gl_ctGuadanDeal = 2,
	gl_ctGuadanWithdraw = 3,
};

class COneGuadan {
public:
	void SetTime(chrono::sys_seconds time) noexcept { m_tpTime = time; }
	void SetTime(time_t t) noexcept { m_tpTime = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::from_time_t(t)); }
	chrono::sys_seconds GetMarketTime() const noexcept { return m_tpTime; }
	void SetVolume(const long lVolume) noexcept { m_lVolume = lVolume; }
	long GetVolume() const noexcept { return m_lVolume; }
	void SetStatus(const long lStatus) noexcept { m_lStatus = lStatus; }
	long GetStatus() const noexcept { return m_lStatus; }

	chrono::sys_seconds m_tpTime{ chrono::time_point_cast<chrono::seconds>(chrono::system_clock::from_time_t(0)) };
	long m_lVolume{ 0 };
	long m_lStatus{ 0 };
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
