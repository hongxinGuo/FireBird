#pragma once

using std::chrono::sys_seconds;
using std::chrono::local_days;

namespace FireBirdTest {
	void GeneralCheck();

	void SystemMessageCheck();
	void EnvironmentCheck();
	void ChinaMarketCheck();
	void WorldMarketCheck();

	inline sys_seconds s_tCurrentSystemTime;
	inline local_days s_currentChinaMarketDate;
	inline local_days s_currentWorldMarketDate;
}
