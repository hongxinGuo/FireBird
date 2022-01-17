#pragma once

#include"SetEconomicCalendar.h"

using namespace std;
#include<memory>

class CEconomicCalendar : public CObject {
public:
	CEconomicCalendar();

	void Load(CSetEconomicCalendar& setEconomicCalendar);
	void Append(CSetEconomicCalendar& setEconomicCalendar);
	void Save(CSetEconomicCalendar& setEconomicCalendar);

public:
	INT64 m_iTime; // 不存储这个数据
	CString m_strTime;
	CString m_strCountry;
	CString m_strEvent;
	CString m_strImpact;
	double m_dActual;
	double m_dEstimate;
	double m_dPrev;
	CString m_strUnit;
};

typedef shared_ptr<CEconomicCalendar> CEconomicCalendarPtr;
typedef shared_ptr<vector<CEconomicCalendarPtr>> CEconomicCalendarVectorPtr;