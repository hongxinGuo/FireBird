module;

export module GlobeDef;

import std;
using std::shared_ptr;
using std::weak_ptr;
using std::chrono::time_point_cast;
using std::chrono::seconds;
using std::chrono::system_clock;
using std::chrono::sys_seconds;
using std::time_t;


export {
	extern sys_seconds gl_tpNow; // 所有的市场使用同一个协调世界时（Coordinated Universal Time）
	extern const std::chrono::time_zone* gl_pTimeZoneLocal; // 软件运行所在的当地时区
	inline time_t GetUTCTime() noexcept { return gl_tpNow.time_since_epoch().count(); }
	inline sys_seconds toSysTime(time_t tt) { return time_point_cast<seconds>(system_clock::from_time_t(tt)); }
	inline void TestSetUTCTime(time_t time) noexcept { gl_tpNow = time_point_cast<seconds>(system_clock::from_time_t(time)); }
	inline void TestSetUTCTime(sys_seconds st) noexcept { gl_tpNow = st; }
}
