#pragma once

namespace FireBirdTest {
	void GeneralCheck();

	void SystemMessageCheck();
	void EnvironmentCheck();
	void ChinaMarketCheck();
	void WorldMarketCheck();

	inline chrono::sys_seconds s_tCurrentSystemTime;
}
