#pragma once

class CRSReference {
public:
	bool m_fActive{ false };
	long m_lDayLength[4]{ 0, 0, 0, 0 }; // 日期长度
	long m_lStrongDayLength[4]{ 0, 0, 0, 0 }; // 满足条件的天数
	double m_dRSStrong[4]{ 50.0, 50.0, 50.0, 50.0 }; // 正值意思为高于此值，负值意思为低于此值
};
