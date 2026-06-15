#pragma once

namespace FireBirdTest {
	void GeneralCheck();

	void SystemMessageCheck();
	void EnvironmentCheck();
	void ChinaMarketCheck();
	void WorldMarketCheck();

	inline chrono::sys_seconds s_tCurrentSystemTime;
	inline chrono::local_days s_currentChinaMarketDate;
	inline chrono::local_days s_currentWorldMarketDate;
}
