#include"pch.h"

#include "TimeConvert.h"

using namespace std::chrono;

chrono::local_days XferToLocalDays(const string& sDate) {
	chrono::year_month_day ymd;
	istringstream ss(sDate);
	chrono::from_stream(ss, "%F", ymd);
	istringstream ss2(sDate);
	chrono::local_days ls;
	ss2 >> chrono::parse("%F", ls);
	chrono::year_month_day ymd2{ ls };
	return ls;
}

chrono::year_month_day GetNextMonth(chrono::year_month_day ymd) noexcept {
	using namespace std::chrono;
	year_month ym{ ymd.year(), ymd.month() };
	year_month nextYm = ym + months{ 1 };
	return year_month_day{ nextYm / day{ 1 } };
}

chrono::local_days GetNextMonth(chrono::local_days ld) noexcept {
	chrono::year_month_day ymd{ ld };
	chrono::year_month_day ymd2 = GetNextMonth(ymd);
	return chrono::local_days{ ymd2 };
}

chrono::local_days GetNextMonday(chrono::local_days ld) {
	weekday wd{ ld }; // 0 = Sunday, 1 = Monday, ...
	int wd_index = static_cast<int>(wd.c_encoding());
	int offset = (1 - wd_index + 7) % 7;
	if (offset == 0) offset = 7; // if already Monday, choose next week's Monday

	return ld + days{ offset };
}

chrono::local_days GetPrevMonday(chrono::local_days ld) {
	weekday wd{ ld };
	int wd_index = static_cast<int>(wd.c_encoding());
	int offset = (wd_index + 6) % 7 + 7;

	return ld - days{ offset };
}

chrono::local_days GetCurrentMonday(chrono::local_days ld) {
	weekday wd{ ld };
	int wd_index = static_cast<int>(wd.c_encoding());
	return ld - days{ (wd_index + 6) % 7 };
}

string FormatToMK(int64_t iNumber) {
	string s;
	if (iNumber > 1024 * 1024) { // 1M以上的流量？
		s = std::format("{:L}M", iNumber / (1024 * 1024));
	}
	else if (iNumber > 1024) { // 1K以上的流量？
		s = std::format("{:4L}K", iNumber / 1024);
	}
	else {
		s = std::format("{:4L}", iNumber);
	}

	return s;
}
string toFormattedDateString(long lDate) {
	return std::format("{:8d}", lDate);
}
