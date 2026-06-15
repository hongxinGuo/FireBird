#include"pch.h"

#include"EPSSurprise.h"

CEPSSurprise::CEPSSurprise() {
	m_strSymbol = " ";
	m_lDate = chrono::local_days(chrono::days(0));
	m_dActual = 0.0;
	m_dEstimate = 0.0;
}
